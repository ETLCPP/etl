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
#include "initializer_list.h"
#include "nth_type.h"

//#include "private/minmax_push.h"
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
    using key_compare_type = TKeyCompare;
    using reference        = const value_type&;
    using const_reference  = const value_type&;
    using rvalue_reference = value_type&&;
    using pointer          = const value_type*;
    using const_pointer    = const value_type*;
    using iterator         = const value_type*;
    using const_iterator   = const value_type*;
    using size_type        = size_t;

    /// Defines the parameter types
    using const_key_reference    = const key_type&;
    using rvalue_key_reference   = key_type&&;
    using const_mapped_reference = const mapped_type&;

  private:

    //*********************************************************************
    /// How to compare elements and keys.
    //*********************************************************************
    class Compare
    {
    public:

      // Compare two value types.
      ETL_CONSTEXPR14 bool operator ()(const value_type& element1, const value_type& element2) const ETL_NOEXCEPT
      {
        return key_compare(element1.first, element2.first);
      }

      ETL_CONSTEXPR14 bool operator ()(const value_type& element, const_key_reference key) const ETL_NOEXCEPT
      {
        return key_compare(element.first, key);
      }

      template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
      ETL_CONSTEXPR14 bool operator ()(const value_type& element, const K& key) const ETL_NOEXCEPT
      {
        return key_compare(element.first, key);
      }

      ETL_CONSTEXPR14 bool operator ()(const_key_reference key, const value_type& element) const ETL_NOEXCEPT
      {
        return key_compare(key, element.first);
      }

      template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
      ETL_CONSTEXPR14 bool operator ()(const K& key, const value_type& element) const ETL_NOEXCEPT
      {
        return key_compare(element.first, key);
      }

      key_compare_type key_compare;
    };

  public:

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
      static_assert((etl::is_default_constructible<value_type>::value),                 "value_type must be default constructible");
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
      return etl::is_unique_sorted(begin(), end(), compare);
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
    ///\return A <code>const_mapped_reference</code> to the mapped value at the index.
    /// Undefined behaviour if the key is not in the map.
    //*************************************************************************
    ETL_CONSTEXPR14 const_mapped_reference at(const_key_reference key) const ETL_NOEXCEPT
    {
      const_iterator itr = find(key);

      return itr->second;
    }

    //*************************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    ETL_CONSTEXPR14 const_mapped_reference at(const K& key) const ETL_NOEXCEPT
    {
      const_iterator itr = find(key);

      return itr->second;
    }

    //*************************************************************************
    ETL_CONSTEXPR14 const_iterator find(const_key_reference key) const ETL_NOEXCEPT
    {
      iterator itr = lower_bound(key);

      if (itr != end())
      {
        if (keys_are_equal(itr->first, key))
        {
          return itr;
        }
        else
        {
          return end();
        }
      }

      return end();

    }

    //*************************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    ETL_CONSTEXPR14 const_iterator find(const K& key) const ETL_NOEXCEPT
    {
      iterator itr = lower_bound(key);

      if (itr != end())
      {
        if (keys_are_equal(itr->first, key))
        {
          return itr;
        }
        else
        {
          return end();
        }
      }

      return end();
    }

    //*************************************************************************
    ETL_CONSTEXPR14 bool contains(const_key_reference key) const ETL_NOEXCEPT
    {
      return find(key) != end();
    }

    //*************************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    ETL_CONSTEXPR14 bool contains(const K& key) const ETL_NOEXCEPT
    {
      return find(key) != end();
    }

    //*************************************************************************
    ETL_CONSTEXPR14 size_type count(const_key_reference key) const ETL_NOEXCEPT
    {
      return contains(key) ? 1 : 0;
    }
      
    //*************************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    ETL_CONSTEXPR14 size_type count(const K& key) const ETL_NOEXCEPT
    {
      return contains(key) ? 1 : 0;
    }

    //*************************************************************************
    ETL_CONSTEXPR14 ETL_OR_STD::pair<const_iterator, const_iterator> equal_range(const_key_reference key) const ETL_NOEXCEPT
    {
      return etl::equal_range(begin(), end(), key, compare);
    }

    //*************************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    ETL_CONSTEXPR14 ETL_OR_STD::pair<const_iterator, const_iterator> equal_range(const K& key) const ETL_NOEXCEPT
    {
      return etl::equal_range(begin(), end(), key, compare);
    }

    //*************************************************************************
    ETL_CONSTEXPR14 const_iterator lower_bound(const_key_reference key) const ETL_NOEXCEPT
    {
      const_iterator itr = etl::lower_bound(begin(), end(), key, compare);

      if ((itr != end()) && keys_are_equal(key, itr->first))
      {
        return itr;
      }
      else
      {
        return end();
      }
    }

    //*************************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    ETL_CONSTEXPR14 const_iterator lower_bound(const K& key) const ETL_NOEXCEPT
    {
      const_iterator itr = etl::lower_bound(begin(), end(), key, compare);

      if ((itr != end()) && keys_are_equal(key, itr->first))
      {
        return itr;
      }
      else
      {
        return end();
      }
    }

    //*************************************************************************
    ETL_CONSTEXPR14 const_iterator upper_bound(const_key_reference key) const ETL_NOEXCEPT
    {
      const_iterator itr = etl::upper_bound(begin(), end(), key, compare);

      if ((itr != end()) && keys_are_equal(key, itr->first))
      {
        return itr;
      }
      else
      {
        return end();
      }
    }

    //*************************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    ETL_CONSTEXPR14 const_iterator upper_bound(const K& key) const ETL_NOEXCEPT
    {
      const_iterator itr = etl::upper_bound(begin(), end(), key, compare);

      if ((itr != end()) && keys_are_equal(key, itr->first))
      {
        return itr;
      }
      else
      {
        return end();
      }
    }

    //*************************************************************************
    ETL_CONSTEXPR14 size_type empty() const ETL_NOEXCEPT
    {
      return size() == 0U;
    }

    //*************************************************************************
    ETL_CONSTEXPR14 size_type full() const ETL_NOEXCEPT
    {
      return size() == Size;
    }

    //*************************************************************************
    ETL_CONSTEXPR14 size_type size() const ETL_NOEXCEPT
    {
      return size_type(element_list_end - element_list);
    }

    //*************************************************************************
    ETL_CONSTEXPR14 size_type max_size() const ETL_NOEXCEPT
    {
      return Size;
    }

    //*************************************************************************
    ETL_CONSTEXPR14 size_type capacity() const ETL_NOEXCEPT
    {
      return Size;
    }

  private:

    //*********************************************************************
    /// Check to see if the keys are equal.
    //*********************************************************************
    bool keys_are_equal(const_key_reference key1, const_key_reference key2) const
    {
      return !key_compare_type()(key1, key2) && !key_compare_type()(key2, key1);
    }

    //*********************************************************************
    template <typename K1, typename K2, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    bool keys_are_equal(const K1& key1, const K2& key2) const
    {
      return !key_compare_type()(key1, key2) && !key_compare_type()(key2, key1);
    }

    Compare compare;

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
}

#endif
