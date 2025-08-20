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

#ifndef ETL_CONST_SET_INCLUDED
#define ETL_CONST_SET_INCLUDED

#include "platform.h"

#if ETL_NOT_USING_CPP11
  #error NOT SUPPORTED FOR C++03 OR BELOW
#endif

#include "algorithm.h"
#include "type_traits.h"
#include "functional.h"
#include "nth_type.h"
#include "span.h"

#include "private/comparator_is_transparent.h"

///\defgroup const_set const_set
///\ingroup containers

namespace etl
{
  template <typename TKey, typename TKeyCompare>
  class iconst_set
  {
  public:

    using key_type        = TKey;
    using value_type      = TKey;
    using key_compare     = TKeyCompare;
    using value_compare   = TKeyCompare;
    using const_reference = const value_type&;
    using const_pointer   = const value_type*;
    using const_iterator  = const value_type*;
    using size_type       = size_t;

    //*************************************************************************
    /// Check that the elements are valid for a set.
    /// The elements must be sorted and contain no duplicates.
    /// \return <b>true</b> if the elements are valid for the set.
    //*************************************************************************
    ETL_CONSTEXPR14 bool is_valid() const ETL_NOEXCEPT
    {
      return etl::is_unique_sorted(begin(), end(), vcompare);
    }

    //*************************************************************************
    ///\brief Returns a <code>const_iterator</code> to the beginning of the set.
    //*************************************************************************
    ETL_CONSTEXPR14 const_iterator begin() const ETL_NOEXCEPT
    {
      return element_list;
    }

    //*************************************************************************
    ///\brief Returns a <code>const_iterator</code> to the beginning of the set.
    //*************************************************************************
    ETL_CONSTEXPR14 const_iterator cbegin() const ETL_NOEXCEPT
    {
      return element_list;
    }

    //*************************************************************************
    ///\brief Returns a <code>const_iterator</code> to the end of the set.
    //*************************************************************************
    ETL_CONSTEXPR14 const_iterator end() const ETL_NOEXCEPT
    {
      return element_list_end;
    }

    //*************************************************************************
    ///\brief Returns a <code>const_iterator</code> to the end of the set.
    //*************************************************************************
    ETL_CONSTEXPR14 const_iterator cend() const ETL_NOEXCEPT
    {
      return element_list_end;
    }

    //*************************************************************************
    ///\brief Returns a <code>const_pointer</code> to the beginning of the set.
    //*************************************************************************
    ETL_CONSTEXPR14 const_pointer data() const ETL_NOEXCEPT
    {
      return element_list;
    }

    //*************************************************************************
    ///\brief Gets a const_iterator to the key.
    ///\param key The key of the element to find.
    ///\return A <code>const_iterator</code> to the key,
    /// or end() if not found.
    //*************************************************************************
    ETL_CONSTEXPR14 const_iterator find(const key_type& key) const ETL_NOEXCEPT
    {
      const_iterator itr = lower_bound(key);

      if ((itr != end()) && (keys_are_equal(*itr, key)))
      {
        return itr;
      }

      return end();
    }

    //*************************************************************************
    ///\brief Gets a const_iterator to the key.
    /// Enabled if the comparator is transparent.
    ///\param key The key of the element to find.
    ///\return A <code>const_iterator</code> to the key,
    /// or end() if not found.
    //*************************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    ETL_CONSTEXPR14 const_iterator find(const K& key) const ETL_NOEXCEPT
    {
      const_iterator itr = lower_bound(key);

      if ((itr != end()) && (keys_are_equal(*itr, key)))
      {
        return itr;
      }

      return end();
    }

    //*************************************************************************
    ///\brief Checks if the set contains an element with key.
    ///\param key The key of the element to check.
    ///\return <b>true</b> if the set contains an element with key.
    //*************************************************************************
    ETL_CONSTEXPR14 bool contains(const key_type& key) const ETL_NOEXCEPT
    {
      return find(key) != end();
    }

    //*************************************************************************
    ///\brief Checks if the set contains an element with key.
    /// Enabled if the comparator is transparent.
    ///\param key The key of the element to check.
    ///\return <b>true</b> if the set contains an element with key.
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
    ETL_CONSTEXPR14 size_type count(const key_type& key) const ETL_NOEXCEPT
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
    ETL_CONSTEXPR14 ETL_OR_STD::pair<const_iterator, const_iterator> equal_range(const key_type& key) const ETL_NOEXCEPT
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
    ETL_CONSTEXPR14 const_iterator lower_bound(const key_type& key) const ETL_NOEXCEPT
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
    ETL_CONSTEXPR14 const_iterator upper_bound(const key_type& key) const ETL_NOEXCEPT
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
    /// Checks if the set is empty.
    ///\return <b>true</b> if the set is empty.
    //*************************************************************************
    ETL_CONSTEXPR14 size_type empty() const ETL_NOEXCEPT
    {
      return size() == 0U;
    }

    //*************************************************************************
    /// Checks if the set is full.
    ///\return <b>true</b> if the set is full.
    //*************************************************************************
    ETL_CONSTEXPR14 size_type full() const ETL_NOEXCEPT
    {
      return (max_elements != 0) && (size() == max_elements);
    }

    //*************************************************************************
    /// Gets the size of the set.
    ///\return The size of the set.
    //*************************************************************************
    ETL_CONSTEXPR14 size_type size() const ETL_NOEXCEPT
    {
      return size_type(element_list_end - element_list);
    }

    //*************************************************************************
    /// Gets the maximum size of the set.
    ///\return The maximum size of the set.
    //*************************************************************************
    ETL_CONSTEXPR14 size_type max_size() const ETL_NOEXCEPT
    {
      return max_elements;
    }

    //*************************************************************************
    /// Gets the capacity of the set.
    /// This is always equal to max_size().
    ///\return The capacity of the set.
    //*************************************************************************
    ETL_CONSTEXPR14 size_type capacity() const ETL_NOEXCEPT
    {
      return max_elements;
    }

    //*************************************************************************
    /// How to compare two key elements.
    ///\return An instance of the key_compare type.
    //*************************************************************************
    ETL_CONSTEXPR14 key_compare key_comp() const ETL_NOEXCEPT
    {
      return key_compare();
    }

    //*************************************************************************
    /// How to compare two value elements.
    ///\return An instance of the value_compare type.
    //*************************************************************************
    ETL_CONSTEXPR14 value_compare value_comp() const ETL_NOEXCEPT
    {
      return value_compare();
    }

  protected:

    //*************************************************************************
    /// Constructor
    //*************************************************************************
    template <typename... TElements>
    ETL_CONSTEXPR14 explicit iconst_set(const value_type* element_list_, size_type size_, size_type max_elements_) ETL_NOEXCEPT
      : element_list(element_list_)
      , element_list_end{element_list_ + size_}
      , max_elements(max_elements_)
    {
    }

  private:

    //*********************************************************************
    /// Check to see if the keys are equal.
    //*********************************************************************
    ETL_CONSTEXPR14 bool keys_are_equal(const_reference key1, const_reference key2) const ETL_NOEXCEPT
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

    key_compare   kcompare;
    value_compare vcompare;

    const value_type* element_list;
    const value_type* element_list_end;
    size_type         max_elements;
  };

  //*********************************************************************
  /// Map type designed for constexpr.
  //*********************************************************************
  template <typename TKey, size_t Size, typename TKeyCompare = etl::less<TKey>>
  class const_set : public iconst_set<TKey, TKeyCompare>
  {
  public:

    using base_t = iconst_set<TKey, TKeyCompare>;

    using key_type        = typename base_t::key_type;
    using value_type      = typename base_t::value_type;
    using key_compare     = typename base_t::key_compare;
    using const_reference = typename base_t::const_reference;
    using const_pointer   = typename base_t::const_pointer;
    using const_iterator  = typename base_t::const_iterator;
    using size_type       = typename base_t::size_type;

    /// Defines the parameter types
    using const_key_reference = const key_type&;

    static_assert((etl::is_default_constructible<key_type>::value), "key_type must be default constructible");

    //*************************************************************************
    ///\brief Construct a const_set from a variadic list of elements.
    /// Static asserts if the elements are not of type <code>value_type</code>.
    /// Static asserts if the number of elements is greater than the capacity of the const_set.
    //*************************************************************************
    template <typename... TElements>
    ETL_CONSTEXPR14 explicit const_set(TElements&&... elements) ETL_NOEXCEPT
      : iconst_set<TKey, TKeyCompare>(element_list, sizeof...(elements), Size)
      , element_list{etl::forward<TElements>(elements)...}
    {
      static_assert((etl::are_all_same<value_type, etl::decay_t<TElements>...>::value), "All elements must be key_type");
      static_assert(sizeof...(elements) <= Size,                                        "Number of elements exceeds capacity");
    }

  private:

    value_type element_list[Size];
  };

  //*************************************************************************
  /// Template deduction guides.
  //*************************************************************************
#if ETL_USING_CPP17
  template <typename... TElements>
  const_set(TElements...) -> const_set<etl::nth_type_t<0, TElements...>, sizeof...(TElements)>;
#endif

  //*********************************************************************
  /// Map type designed for constexpr.
  //*********************************************************************
  template <typename TKey, typename TKeyCompare = etl::less<TKey>>
  class const_set_ext : public iconst_set<TKey, TKeyCompare>
  {
  public:

    using base_t = iconst_set<TKey, TKeyCompare>;

    using key_type        = typename base_t::key_type;
    using value_type      = typename base_t::value_type;
    using key_compare     = typename base_t::key_compare;
    using const_reference = typename base_t::const_reference;
    using const_pointer   = typename base_t::const_pointer;
    using const_iterator  = typename base_t::const_iterator;
    using size_type       = typename base_t::size_type;

    /// Defines the parameter types
    using const_reference = const key_type&;

    static_assert((etl::is_default_constructible<key_type>::value), "key_type must be default constructible");

    //*************************************************************************
    ///\brief Default construct a const_set.
    //*************************************************************************
    ETL_CONSTEXPR14 const_set_ext() ETL_NOEXCEPT
      : iconst_set<TKey, TKeyCompare>(nullptr, 0, 0)
    {
    }

    //*************************************************************************
    ///\brief Construct a const_set from a variadic list of elements.
    //*************************************************************************
    template <size_type Size>
    ETL_CONSTEXPR14 explicit const_set_ext(const etl::span<const value_type, Size>& sp) ETL_NOEXCEPT
      : iconst_set<TKey, TKeyCompare>(sp.data(), Size, Size)
    {
    }

    //*************************************************************************
    ///\brief Construct a const_set from an array.
    //*************************************************************************
    template <size_type Size>
    ETL_CONSTEXPR14 explicit const_set_ext(const value_type(&begin_)[Size]) ETL_NOEXCEPT
      : iconst_set<TKey, TKeyCompare>(begin_, Size, Size)
    {
    }
  };

  //*************************************************************************
  /// Template deduction guides.
  //*************************************************************************
#if ETL_USING_CPP17
  template <typename TElements, size_t Size>
  const_set_ext(const etl::span<TElements, Size>&) -> const_set_ext<TElements>;

  template <typename TElements, size_t Size>
  const_set_ext(const TElements(&)[Size]) -> const_set_ext<TElements>;
#endif

  //*************************************************************************
  /// Equality test.
  //*************************************************************************
  template <typename TKey, typename TKeyCompare>
  ETL_CONSTEXPR14 bool operator ==(const etl::iconst_set<TKey, TKeyCompare>& lhs,
                                   const etl::iconst_set<TKey, TKeyCompare>& rhs) ETL_NOEXCEPT
  {
    return (lhs.size() == rhs.size()) && etl::equal(lhs.begin(), lhs.end(), rhs.begin());
  }

  //*************************************************************************
  /// Inequality test.
  //*************************************************************************
  template <typename TKey, typename TKeyCompare>
  ETL_CONSTEXPR14 bool operator !=(const etl::iconst_set<TKey, TKeyCompare>& lhs,
                                   const etl::iconst_set<TKey, TKeyCompare>& rhs) ETL_NOEXCEPT
  {
    return !(lhs == rhs);
  }

  //*************************************************************************
  /// Less-than.
  //*************************************************************************
  template <typename TKey, typename TKeyCompare>
  ETL_CONSTEXPR14 bool operator <(const etl::iconst_set<TKey, TKeyCompare>& lhs,
                                  const etl::iconst_set<TKey, TKeyCompare>& rhs) ETL_NOEXCEPT
  {
    return etl::lexicographical_compare(lhs.begin(), lhs.end(), 
                                        rhs.begin(), rhs.end(), 
                                        lhs.value_comp());
  }

  //*************************************************************************
  /// Greater-than.
  //*************************************************************************
  template <typename TKey, typename TKeyCompare>
  ETL_CONSTEXPR14 bool operator >(const etl::iconst_set<TKey, TKeyCompare>& lhs,
                                  const etl::iconst_set<TKey, TKeyCompare>& rhs) ETL_NOEXCEPT
  {
    return (rhs < lhs);
  }

  //*************************************************************************
  /// Less-than-equal.
  //*************************************************************************
  template <typename TKey, typename TKeyCompare>
  ETL_CONSTEXPR14 bool operator <=(const etl::iconst_set<TKey, TKeyCompare>& lhs,
                                   const etl::iconst_set<TKey, TKeyCompare>& rhs) ETL_NOEXCEPT
  {
    return !(rhs < lhs);
  }

  //*************************************************************************
  /// Greater-than-equal.
  //*************************************************************************
  template <typename TKey, typename TKeyCompare>
  ETL_CONSTEXPR14 bool operator >=(const etl::iconst_set<TKey, TKeyCompare>& lhs,
                                   const etl::iconst_set<TKey, TKeyCompare>& rhs) ETL_NOEXCEPT
  {
    return !(lhs < rhs);
  }
}

#endif
