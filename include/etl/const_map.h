///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2025 John Wellbelove

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#ifndef ETL_CONST_MAP_INCLUDED
#define ETL_CONST_MAP_INCLUDED

#include "platform.h"
#include "algorithm.h"
#include "type_traits.h"
#include "functional.h"
#include "nth_type.h"

#include "private/comparator_is_transparent.h"

///\defgroup const_map const_map
///\ingroup containers

namespace etl
{
  template <typename TKey, typename TMapped, size_t Size, typename TKeyCompare = etl::less<TKey>>
  class const_map
  {
  public:

    using key_type         = TKey;
    using value_type       = ETL_OR_STD::pair<const TKey, TMapped>;
    using mapped_type      = TMapped ;
    using key_compare      = TKeyCompare;
    using const_reference  = const value_type&;
    using const_pointer    = const value_type*;
    using const_iterator   = const value_type*;
    using size_type        = size_t;

    /// Defines the parameter types
    using const_key_reference    = const key_type&;
    using const_mapped_reference = const mapped_type&;

    //*********************************************************************
    /// How to compare elements and keys.
    //*********************************************************************
    class value_compare
    {
    public:

      // Compare two value types.
      ETL_CONSTEXPR14 bool operator ()(const value_type& element1, const value_type& element2) const ETL_NOEXCEPT
      {
        return kcompare(element1.first, element2.first);
      }

      // Compare value type and key.
      ETL_CONSTEXPR14 bool operator ()(const value_type& element, const_key_reference key) const ETL_NOEXCEPT
      {
        return kcompare(element.first, key);
      }

      // Compare value types and key.
      // Enabled for transparent comparators.
      template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
      ETL_CONSTEXPR14 bool operator ()(const value_type& element, const K& key) const ETL_NOEXCEPT
      {
        return kcompare(element.first, key);
      }

      // Compare key and value type.
      ETL_CONSTEXPR14 bool operator ()(const_key_reference key, const value_type& element) const ETL_NOEXCEPT
      {
        return kcompare(key, element.first);
      }

      // Compare key and value type.
      // Enabled for transparent comparators.
      template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
      ETL_CONSTEXPR14 bool operator ()(const K& key, const value_type& element) const ETL_NOEXCEPT
      {
        return kcompare(key, element.first);
      }

      key_compare kcompare;
    };

    //*************************************************************************
    ///\brief Construct a const_map from a variadic list of elements.
    /// Static asserts if the element type is not constructible.
    /// Static asserts if the elements are not of type <code>value_type</code>.
    /// Static asserts if the number of elements is greater than the capacity of the const_map.
    //*************************************************************************
    template <typename... TElements>
    ETL_CONSTEXPR14 explicit const_map(TElements&&... elements) ETL_NOEXCEPT
      : element_list{etl::forward<TElements>(elements)...}
      , element_list_end{element_list + sizeof...(elements)}
    {
      static_assert((etl::is_default_constructible<key_type>::value),                   "key_type must be default constructible");
      static_assert((etl::is_default_constructible<mapped_type>::value),                "mapped_type must be default constructible");
      static_assert((etl::are_all_same<value_type, etl::decay_t<TElements>...>::value), "All elements must be value_type");
      static_assert(sizeof...(elements) <= Size,                                        "Number of elements exceeds capacity");
    }

    //*************************************************************************
    /// Check that the elements are valid for a map.
    /// The elements must be sorted and contain no duplicates.
    /// \return <b>true</b> if the elements are valid for the map.
    //*************************************************************************
    ETL_CONSTEXPR14 bool is_valid() const ETL_NOEXCEPT
    {
      return etl::is_unique_sorted(begin(), end(), vcompare);
    }

    //*************************************************************************
    ///\brief Returns a <code>const_iterator</code> to the beginning of the map.
    //*************************************************************************
    ETL_CONSTEXPR14 const_iterator begin() const ETL_NOEXCEPT
    {
      return element_list;
    }

    //*************************************************************************
    ///\brief Returns a <code>const_iterator</code> to the beginning of the map.
    //*************************************************************************
    ETL_CONSTEXPR14 const_iterator cbegin() const ETL_NOEXCEPT
    {
      return element_list;
    }

    //*************************************************************************
    ///\brief Returns a <code>const_iterator</code> to the end of the map.
    //*************************************************************************
    ETL_CONSTEXPR14 const_iterator end() const ETL_NOEXCEPT
    {
      return element_list_end;
    }

    //*************************************************************************
    ///\brief Returns a <code>const_iterator</code> to the end of the map.
    //*************************************************************************
    ETL_CONSTEXPR14 const_iterator cend() const ETL_NOEXCEPT
    {
      return element_list_end;
    }

    //*************************************************************************
    ///\brief Returns a <code>const_pointer</code> to the beginning of the map.
    //*************************************************************************
    ETL_CONSTEXPR14 const_pointer data() const ETL_NOEXCEPT
    {
      return element_list;
    }

    //*************************************************************************
    ///\brief Index operator.
    ///\param key The key of the element to return.
    ///\return A <code>const_mapped_reference</code> to the mapped value at the index.
    /// Undefined behaviour if the key is not in the map.
    //*************************************************************************
    ETL_CONSTEXPR14 const_mapped_reference operator[](const_key_reference key) const ETL_NOEXCEPT
    {
      const_iterator itr = find(key);
      
      return itr->second;
    }

    //*************************************************************************
    ///\brief Key index operator.
    /// Enabled for transparent comparators.
    ///\param key The key of the element to return.
    ///\return A <code>const_mapped_reference</code> to the mapped value at the key index.
    /// Undefined behaviour if the key is not in the map.
    //*************************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    ETL_CONSTEXPR14 const_mapped_reference operator[](const K& key) const ETL_NOEXCEPT
    {
      const_iterator itr = find(key);

      return itr->second;
    }

    //*************************************************************************
    ///\brief Gets the mapped value at the key index.
    ///\param key The key of the element to return.
    ///\return A <code>const_mapped_reference</code> to the mapped value at the index.
    /// Undefined behaviour if the key is not in the map.
    //*************************************************************************
    ETL_CONSTEXPR14 const_mapped_reference at(const_key_reference key) const ETL_NOEXCEPT
    {
      const_iterator itr = find(key);

      return itr->second;
    }

    //*************************************************************************
    ///\brief Gets the mapped value at the key index.
    /// Enabled if the comparator is transparent.
    ///\param key The key of the element to return.
    ///\return A <code>const_mapped_reference</code> to the mapped value at the index.
    /// Undefined behaviour if the key is not in the map.
    //*************************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    ETL_CONSTEXPR14 const_mapped_reference at(const K& key) const ETL_NOEXCEPT
    {
      const_iterator itr = find(key);

      return itr->second;
    }

    //*************************************************************************
    ///\brief Gets a const_iterator to the mapped value at the key index.
    ///\param key The key of the element to find.
    ///\return A <code>const_iterator</code> to the mapped value at the index,
    /// or end() if not found.
    //*************************************************************************
    ETL_CONSTEXPR14 const_iterator find(const_key_reference key) const ETL_NOEXCEPT
    {
      const_iterator itr = lower_bound(key);

      if ((itr != end()) && (keys_are_equal(itr->first, key)))
      {
        return itr;
      }

      return end();

    }

    //*************************************************************************
    ///\brief Gets a const_iterator to the mapped value at the key index.
    /// Enabled if the comparator is transparent.
    ///\param key The key of the element to find.
    ///\return A <code>const_iterator</code> to the mapped value at the index,
    /// or end() if not found.
    //*************************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    ETL_CONSTEXPR14 const_iterator find(const K& key) const ETL_NOEXCEPT
    {
      const_iterator itr = lower_bound(key);

      if ((itr != end()) && (keys_are_equal(itr->first, key)))
      {
        return itr;
      }

      return end();
    }

    //*************************************************************************
    ///\brief Checks if the map contains an element with key.
    ///\param key The key of the element to check.
    ///\return <b>true</b> if the map contains an element with key.
    //*************************************************************************
    ETL_CONSTEXPR14 bool contains(const_key_reference key) const ETL_NOEXCEPT
    {
      return find(key) != end();
    }

    //*************************************************************************
    ///\brief Checks if the map contains an element with key.
    /// Enabled if the comparator is transparent.
    ///\param key The key of the element to check.
    ///\return <b>true</b> if the map contains an element with key.
    //*************************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    ETL_CONSTEXPR14 bool contains(const K& key) const ETL_NOEXCEPT
    {
      return find(key) != end();
    }

    //*************************************************************************
    ///\brief Counts the numbeer elements with key.
    ///\param key The key of the element to count.
    ///\return 0 or 1
    //*************************************************************************
    ETL_CONSTEXPR14 size_type count(const_key_reference key) const ETL_NOEXCEPT
    {
      return contains(key) ? 1 : 0;
    }
      
    //*************************************************************************
    ///\brief Counts the numbeer elements with key.
    /// Enabled if the comparator is transparent.
    ///\param key The key of the element to count.
    ///\return 0 or 1
    //*************************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    ETL_CONSTEXPR14 size_type count(const K& key) const ETL_NOEXCEPT
    {
      return contains(key) ? 1 : 0;
    }

    //*************************************************************************
    ///\brief Returns a range containing all elements with the key. 
    /// The range is defined by a pair of two iterators, one to the 
    /// first element that is not less than the key and second to the first 
    /// element greater than the key.
    ///\param key The key of the element
    ///\return etl::pair or std::pair containing a pair of iterators.
    //*************************************************************************
    ETL_CONSTEXPR14 ETL_OR_STD::pair<const_iterator, const_iterator> equal_range(const_key_reference key) const ETL_NOEXCEPT
    {
      return etl::equal_range(begin(), end(), key, vcompare);
    }

    //*************************************************************************
    ///\brief Returns a range containing all elements with the key. 
    /// The range is defined by a pair of two iterators, one to the 
    /// first element that is not less than the key and second to the first 
    /// element greater than the key.
    /// Enabled if the comparator is transparent.
    ///\param key The key of the element
    ///\return etl::pair or std::pair containing a pair of iterators.
    //*************************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    ETL_CONSTEXPR14 ETL_OR_STD::pair<const_iterator, const_iterator> equal_range(const K& key) const ETL_NOEXCEPT
    {
      return etl::equal_range(begin(), end(), key, vcompare);
    }

    //*************************************************************************
    ///\brief Returns a const_iterator to the first element that is not less than the key. 
    /// Returns a const_iterator to the first element that is not less than the key.
    ///\param key The key of the element
    ///\return const_iterator to the element or end()
    //*************************************************************************
    ETL_CONSTEXPR14 const_iterator lower_bound(const_key_reference key) const ETL_NOEXCEPT
    {
      return etl::lower_bound(begin(), end(), key, vcompare);
    }

    //*************************************************************************
    ///\brief Returns a const_iterator to the first element that is not less than the key. 
    /// Returns a const_iterator to the first element that is not less than the key.
    /// Enabled if the comparator is transparent.
    ///\param key The key of the element
    ///\return const_iterator to the element or end()
    //*************************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    ETL_CONSTEXPR14 const_iterator lower_bound(const K& key) const ETL_NOEXCEPT
    {
      return etl::lower_bound(begin(), end(), key, vcompare);
    }

    //*************************************************************************
    ///\brief Returns a const_iterator to the first element that is greater than the key. 
    /// Returns a const_iterator to the first element that is greater than the key.
    ///\param key The key of the element
    ///\return const_iterator to the element or end()
    //*************************************************************************
    ETL_CONSTEXPR14 const_iterator upper_bound(const_key_reference key) const ETL_NOEXCEPT
    {
      return etl::upper_bound(begin(), end(), key, vcompare);
    }

    //*************************************************************************
    ///\brief Returns a const_iterator to the first element that is greater than the key. 
    /// Returns a const_iterator to the first element that is greater than the key.
    /// Enabled if the comparator is transparent.
    ///\param key The key of the element
    ///\return const_iterator to the element or end()
    //*************************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    ETL_CONSTEXPR14 const_iterator upper_bound(const K& key) const ETL_NOEXCEPT
    {
      return etl::upper_bound(begin(), end(), key, vcompare);
    }

    //*************************************************************************
    /// Checks if the map is empty.
    ///\return <b>true</b> if the map is empty.
    //*************************************************************************
    ETL_CONSTEXPR14 size_type empty() const ETL_NOEXCEPT
    {
      return size() == 0U;
    }

    //*************************************************************************
    /// Checks if the map is full.
    ///\return <b>true</b> if the map is full.
    //*************************************************************************
    ETL_CONSTEXPR14 size_type full() const ETL_NOEXCEPT
    {
      return size() == Size;
    }

    //*************************************************************************
    /// Gets the size of the map.
    ///\return The size of the map.
    //*************************************************************************
    ETL_CONSTEXPR14 size_type size() const ETL_NOEXCEPT
    {
      return size_type(element_list_end - element_list);
    }

    //*************************************************************************
    /// Gets the maximum size of the map.
    ///\return The maximum size of the map.
    //*************************************************************************
    ETL_CONSTEXPR14 size_type max_size() const ETL_NOEXCEPT
    {
      return Size;
    }

    //*************************************************************************
    /// Gets the capacity of the map.
    /// This is always equal to max_size().
    ///\return The capacity of the map.
    //*************************************************************************
    ETL_CONSTEXPR14 size_type capacity() const ETL_NOEXCEPT
    {
      return Size;
    }

    //*************************************************************************
    /// How to compare two key elements.
    ///\return An instance of the key_compare type.
    //*************************************************************************
    ETL_CONSTEXPR14 key_compare key_comp() const ETL_NOEXCEPT
    {
      return vcompare.kcompare;
    }

    //*************************************************************************
    /// How to compare two value elements.
    ///\return An instance of the value_compare type.
    //*************************************************************************
    ETL_CONSTEXPR14 value_compare value_comp() const ETL_NOEXCEPT
    {
      return vcompare;
    }

  private:

    //*********************************************************************
    /// Check to see if the keys are equal.
    //*********************************************************************
    ETL_CONSTEXPR14 bool keys_are_equal(const_key_reference key1, const_key_reference key2) const ETL_NOEXCEPT
    {
      return !key_compare()(key1, key2) && !key_compare()(key2, key1);
    }

    //*********************************************************************
    /// Check to see if the keys are equal.
    /// Enabled if the comparator is transparent.
    //*********************************************************************
    template <typename K1, typename K2, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    ETL_CONSTEXPR14 bool keys_are_equal(const K1& key1, const K2& key2) const ETL_NOEXCEPT
    {
      return !key_compare()(key1, key2) && !key_compare()(key2, key1);
    }

    value_compare vcompare;

    value_type  element_list[Size];
    value_type* element_list_end;
  };

  //*************************************************************************
  /// Template deduction guides.
  //*************************************************************************
#if ETL_USING_CPP17
  template <typename... TPairs>
  const_map(TPairs...) -> const_map<typename etl::nth_type_t<0, TPairs...>::first_type, 
                                    typename etl::nth_type_t<0, TPairs...>::second_type, 
                                    sizeof...(TPairs)>;
#endif


  //*************************************************************************
  /// Equality test.
  //*************************************************************************
  template <typename TKey, typename TMapped, size_t Size, typename TKeyCompare>
  ETL_CONSTEXPR14 bool operator ==(const etl::const_map<TKey, TMapped, Size, TKeyCompare>& lhs,
                                   const etl::const_map<TKey, TMapped, Size, TKeyCompare>& rhs)
  {
    return etl::equal(lhs.begin(), lhs.end(), rhs.begin());
  }

  //*************************************************************************
  /// Inequality test.
  //*************************************************************************
  template <typename TKey, typename TMapped, size_t Size, typename TKeyCompare>
  ETL_CONSTEXPR14 bool operator !=(const etl::const_map<TKey, TMapped, Size, TKeyCompare>& lhs,
                                   const etl::const_map<TKey, TMapped, Size, TKeyCompare>& rhs)
  {
    return !(lhs == rhs);
  }
}

#endif
