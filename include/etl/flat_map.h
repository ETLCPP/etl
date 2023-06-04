///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2015 John Wellbelove

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

#ifndef ETL_FLAT_MAP_INCLUDED
#define ETL_FLAT_MAP_INCLUDED

#include "platform.h"
#include "reference_flat_map.h"
#include "pool.h"
#include "placement_new.h"
#include "nth_type.h"
#include "utility.h"
#include "type_traits.h"
#include "initializer_list.h"

#include "private/comparator_is_transparent.h"

//*****************************************************************************
///\defgroup flat_map flat_map
/// A flat_map based on a sorted vector with the capacity defined at
/// compile time. This container is best used for tables that are occasionally
/// updated and spend most of their time being searched.
/// Has insertion of O(N) and find of O(logN).
/// Duplicate entries are not allowed.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //***************************************************************************
  /// The base class for specifically sized flat_maps.
  /// Can be used as a reference type for all flat_maps containing a specific type.
  ///\ingroup flat_map
  //***************************************************************************
  template <typename TKey, typename TMapped, typename TKeyCompare = etl::less<TKey> >
  class iflat_map : private etl::ireference_flat_map<TKey, TMapped, TKeyCompare>
  {
  private:

    typedef etl::ireference_flat_map<TKey, TMapped, TKeyCompare> refmap_t;
    typedef typename refmap_t::lookup_t lookup_t;
    typedef etl::ipool storage_t;

  public:

    typedef ETL_OR_STD::pair<const TKey, TMapped> value_type;
    typedef TKey              key_type;
    typedef TMapped           mapped_type;
    typedef TKeyCompare       key_compare;
    typedef value_type&       reference;
    typedef const value_type& const_reference;
#if ETL_USING_CPP11
    typedef value_type&&      rvalue_reference;
#endif
    typedef value_type*       pointer;
    typedef const value_type* const_pointer;
    typedef size_t            size_type;

    typedef const key_type&    const_key_reference;
#if ETL_USING_CPP11
    typedef key_type&&         rvalue_key_reference;
#endif
    typedef mapped_type&       mapped_reference;
    typedef const mapped_type& const_mapped_reference;

    typedef typename refmap_t::iterator       iterator;
    typedef typename refmap_t::const_iterator const_iterator;

    typedef ETL_OR_STD::reverse_iterator<iterator>       reverse_iterator;
    typedef ETL_OR_STD::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef typename etl::iterator_traits<iterator>::difference_type difference_type;

  private:

    //*********************************************************************
    /// How to compare elements and keys.
    //*********************************************************************
    class compare
    {
    public:

      bool operator ()(const value_type& element, key_type key) const
      {
        return comp(element.first, key);
      }

      bool operator ()(key_type key, const value_type& element) const
      {
        return comp(key, element.first);
      }

      key_compare comp;
    };

  public:

    //*********************************************************************
    /// Returns an iterator to the beginning of the flat_map.
    ///\return An iterator to the beginning of the flat_map.
    //*********************************************************************
    iterator begin()
    {
      return refmap_t::begin();
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the flat_map.
    ///\return A const iterator to the beginning of the flat_map.
    //*********************************************************************
    const_iterator begin() const
    {
      return refmap_t::begin();
    }

    //*********************************************************************
    /// Returns an iterator to the end of the flat_map.
    ///\return An iterator to the end of the flat_map.
    //*********************************************************************
    iterator end()
    {
      return refmap_t::end();
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the flat_map.
    ///\return A const iterator to the end of the flat_map.
    //*********************************************************************
    const_iterator end() const
    {
      return refmap_t::end();
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the flat_map.
    ///\return A const iterator to the beginning of the flat_map.
    //*********************************************************************
    const_iterator cbegin() const
    {
      return refmap_t::cbegin();
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the flat_map.
    ///\return A const iterator to the end of the flat_map.
    //*********************************************************************
    const_iterator cend() const
    {
      return refmap_t::cend();
    }

    //*********************************************************************
    /// Returns an reverse iterator to the reverse beginning of the flat_map.
    ///\return Iterator to the reverse beginning of the flat_map.
    //*********************************************************************
    reverse_iterator rbegin()
    {
      return refmap_t::rbegin();
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the flat_map.
    ///\return Const iterator to the reverse beginning of the flat_map.
    //*********************************************************************
    const_reverse_iterator rbegin() const
    {
      return refmap_t::rbegin();
    }

    //*********************************************************************
    /// Returns a reverse iterator to the end + 1 of the flat_map.
    ///\return Reverse iterator to the end + 1 of the flat_map.
    //*********************************************************************
    reverse_iterator rend()
    {
      return refmap_t::rend();
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the end + 1 of the flat_map.
    ///\return Const reverse iterator to the end + 1 of the flat_map.
    //*********************************************************************
    const_reverse_iterator rend() const
    {
      return refmap_t::rend();
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the flat_map.
    ///\return Const reverse iterator to the reverse beginning of the flat_map.
    //*********************************************************************
    const_reverse_iterator crbegin() const
    {
      return refmap_t::crbegin();
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the end + 1 of the flat_map.
    ///\return Const reverse iterator to the end + 1 of the flat_map.
    //*********************************************************************
    const_reverse_iterator crend() const
    {
      return refmap_t::crend();
    }

#if ETL_USING_CPP11
    //*********************************************************************
    /// Returns a reference to the value at index 'key'
    ///\param i The index.
    ///\return A reference to the value at index 'key'
    //*********************************************************************
    mapped_reference operator [](rvalue_key_reference key)
    {
      iterator i_element = lower_bound(key);

      // Doesn't already exist?
      if ((i_element == end()) || compare(key, i_element->first))
      {
        insert_default_value(i_element, etl::move(key));
      }

      return i_element->second;
    }
#endif

    //*********************************************************************
    /// Returns a reference to the value at index 'key'
    ///\param i The index.
    ///\return A reference to the value at index 'key'
    //*********************************************************************
    mapped_reference operator [](const_key_reference key)
    {
      iterator i_element = lower_bound(key);

      // Doesn't already exist?
      if ((i_element == end()) || compare(key, i_element->first))
      {
        insert_default_value(i_element, key);
      }

      return i_element->second;
    }

    //*********************************************************************
    /// Returns a reference to the value at index 'key'
    /// If asserts or exceptions are enabled, emits an etl::flat_map_out_of_bounds if the key is not in the range.
    ///\param i The index.
    ///\return A reference to the value at index 'key'
    //*********************************************************************
    mapped_reference at(const_key_reference key)
    {
      return refmap_t::at(key);
    }

#if ETL_USING_CPP11
    //*********************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    mapped_reference at(const K& key)
    {
      return refmap_t::at(key);
    }
#endif

    //*********************************************************************
    /// Returns a const reference to the value at index 'key'
    /// If asserts or exceptions are enabled, emits an etl::flat_map_out_of_bounds if the key is not in the range.
    ///\param i The index.
    ///\return A const reference to the value at index 'key'
    //*********************************************************************
    const_mapped_reference at(const_key_reference key) const
    {
      return refmap_t::at(key);
    }

#if ETL_USING_CPP11
    //*********************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    const_mapped_reference at(const K& key) const
    {
      return refmap_t::at(key);
    }
#endif

    //*********************************************************************
    /// Assigns values to the flat_map.
    /// If ETL_THROW_EXCEPTIONS & ETL_DEBUG are defined, emits flat_map_full if the flat_map does not have enough free space.
    /// If ETL_THROW_EXCEPTIONS & ETL_DEBUG are defined, emits flat_map_iterator if the iterators are reversed.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*********************************************************************
    template <typename TIterator>
    void assign(TIterator first, TIterator last)
    {
#if ETL_IS_DEBUG_BUILD
      difference_type d = etl::distance(first, last);
      ETL_ASSERT(d <= difference_type(capacity()), ETL_ERROR(flat_map_full));
#endif

      clear();

      while (first != last)
      {
        insert(*first);
        ++first;
      }
    }

    //*********************************************************************
    /// Inserts a value to the flat_map.
    /// If asserts or exceptions are enabled, emits flat_map_full if the flat_map is already full.
    ///\param value    The value to insert.
    //*********************************************************************
    ETL_OR_STD::pair<iterator, bool> insert(const_reference value)
    {
      iterator i_element = lower_bound(value.first);

      ETL_OR_STD::pair<iterator, bool> result(i_element, false);

      // Doesn't already exist?
      if ((i_element == end()) || compare(value.first, i_element->first))
      {
        result = insert_value(i_element, value);
      }

      return result;
    }

#if ETL_USING_CPP11
    //*********************************************************************
    /// Moves a value to the flat_map.
    /// If asserts or exceptions are enabled, emits flat_map_full if the flat_map is already full.
    ///\param value    The value to insert.
    //*********************************************************************
    ETL_OR_STD::pair<iterator, bool> insert(rvalue_reference value)
    {
      iterator i_element = lower_bound(value.first);

      ETL_OR_STD::pair<iterator, bool> result(i_element, false);

      // Doesn't already exist?
      if ((i_element == end()) || compare(value.first, i_element->first))
      {
        //result = insert_value(i_element, etl::move(value.first), etl::move(value.second));
        result = insert_value(i_element, etl::move(value));
      }

      return result;
    }
#endif

    //*********************************************************************
    /// Inserts a value to the flat_map.
    /// If asserts or exceptions are enabled, emits flat_map_full if the flat_map is already full.
    ///\param position The position to insert at.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(const_iterator /*position*/, const_reference value)
    {
      return insert(value).first;
    }

#if ETL_USING_CPP11
    //*********************************************************************
    /// Moves a value to the flat_map.
    /// If asserts or exceptions are enabled, emits flat_map_full if the flat_map is already full.
    ///\param position The position to insert at.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(const_iterator /*position*/, rvalue_reference value)
    {
      return insert(etl::move(value)).first;
    }
#endif

    //*********************************************************************
    /// Inserts a range of values to the flat_map.
    /// If asserts or exceptions are enabled, emits flat_map_full if the flat_map does not have enough free space.
    ///\param position The position to insert at.
    ///\param first    The first element to add.
    ///\param last     The last + 1 element to add.
    //*********************************************************************
    template <class TIterator>
    void insert(TIterator first, TIterator last)
    {
      while (first != last)
      {
        insert(*first);
        ++first;
      }
    }

    //*************************************************************************
    /// Emplaces a value to the map.
    //*************************************************************************
    ETL_OR_STD::pair<iterator, bool> emplace(const value_type& value)
    {
      return emplace(value.first, value.second);
    }

#if ETL_USING_CPP11 && ETL_NOT_USING_STLPORT
    //*************************************************************************
    /// Emplaces a value to the map.
    //*************************************************************************
    template <typename ... Args>
    ETL_OR_STD::pair<iterator, bool> emplace(const_key_reference key, Args && ... args)
    {
      ETL_ASSERT(!full(), ETL_ERROR(flat_map_full));

      // Create it.
      value_type* pvalue = storage.allocate<value_type>();
      ::new ((void*)etl::addressof(pvalue->first)) key_type(key);
      ::new ((void*)etl::addressof(pvalue->second)) mapped_type(etl::forward<Args>(args)...);

      iterator i_element = lower_bound(key);

      ETL_OR_STD::pair<iterator, bool> result(i_element, false);

      // Doesn't already exist?
      if ((i_element == end()) || compare(key, i_element->first))
      {
        ETL_INCREMENT_DEBUG_COUNT
        result = refmap_t::insert_at(i_element, *pvalue);
      }
      else
      {
        pvalue->~value_type();
        storage.release(pvalue);
      }

      return result;
    }

#else

    //*************************************************************************
    /// Emplaces a value to the map.
    //*************************************************************************
    template <typename T1>
    ETL_OR_STD::pair<iterator, bool> emplace(const_key_reference key, const T1& value1)
    {
      ETL_ASSERT(!full(), ETL_ERROR(flat_map_full));

      // Create it.
      value_type* pvalue = storage.allocate<value_type>();
      ::new ((void*)etl::addressof(pvalue->first)) key_type(key);
      ::new ((void*)etl::addressof(pvalue->second)) mapped_type(value1);

      iterator i_element = lower_bound(key);

      ETL_OR_STD::pair<iterator, bool> result(i_element, false);

      // Doesn't already exist?
      if ((i_element == end()) || compare(key, i_element->first))
      {
        ETL_INCREMENT_DEBUG_COUNT
        result = refmap_t::insert_at(i_element, *pvalue);
      }
      else
      {
        pvalue->~value_type();
        storage.release(pvalue);
      }

      return result;
    }

    //*************************************************************************
    /// Emplaces a value to the map.
    //*************************************************************************
    template <typename T1, typename T2>
    ETL_OR_STD::pair<iterator, bool> emplace(const_key_reference key, const T1& value1, const T2& value2)
    {
      ETL_ASSERT(!full(), ETL_ERROR(flat_map_full));

      // Create it.
      value_type* pvalue = storage.allocate<value_type>();
      ::new ((void*)etl::addressof(pvalue->first)) key_type(key);
      ::new ((void*)etl::addressof(pvalue->second)) mapped_type(value1, value2);

      iterator i_element = lower_bound(key);

      ETL_OR_STD::pair<iterator, bool> result(i_element, false);

      // Doesn't already exist?
      if ((i_element == end()) || compare(key, i_element->first))
      {
        ETL_INCREMENT_DEBUG_COUNT
        result = refmap_t::insert_at(i_element, *pvalue);
      }
      else
      {
        pvalue->~value_type();
        storage.release(pvalue);
      }

      return result;
    }

    //*************************************************************************
    /// Emplaces a value to the map.
    //*************************************************************************
    template <typename T1, typename T2, typename T3>
    ETL_OR_STD::pair<iterator, bool> emplace(const_key_reference key, const T1& value1, const T2& value2, const T3& value3)
    {
      ETL_ASSERT(!full(), ETL_ERROR(flat_map_full));

      // Create it.
      value_type* pvalue = storage.allocate<value_type>();
      ::new ((void*)etl::addressof(pvalue->first)) key_type(key);
      ::new ((void*)etl::addressof(pvalue->second)) mapped_type(value1, value2, value3);

      iterator i_element = lower_bound(key);

      ETL_OR_STD::pair<iterator, bool> result(i_element, false);

      // Doesn't already exist?
      if ((i_element == end()) || compare(key, i_element->first))
      {
        ETL_INCREMENT_DEBUG_COUNT
        result = refmap_t::insert_at(i_element, *pvalue);
      }
      else
      {
        pvalue->~value_type();
        storage.release(pvalue);
      }

      return result;
    }

    //*************************************************************************
    /// Emplaces a value to the map.
    //*************************************************************************
    template <typename T1, typename T2, typename T3, typename T4>
    ETL_OR_STD::pair<iterator, bool> emplace(const_key_reference key, const T1& value1, const T2& value2, const T3& value3, const T4& value4)
    {
      ETL_ASSERT(!full(), ETL_ERROR(flat_map_full));

      // Create it.
      value_type* pvalue = storage.allocate<value_type>();
      ::new ((void*)etl::addressof(pvalue->first)) key_type(key);
      ::new ((void*)etl::addressof(pvalue->second)) mapped_type(value1, value2, value3, value4);

      iterator i_element = lower_bound(key);

      ETL_OR_STD::pair<iterator, bool> result(i_element, false);

      // Doesn't already exist?
      if ((i_element == end()) || compare(key, i_element->first))
      {
        ETL_INCREMENT_DEBUG_COUNT
        result = refmap_t::insert_at(i_element, *pvalue);
      }
      else
      {
        pvalue->~value_type();
        storage.release(pvalue);
      }

      return result;
    }

#endif

    //*********************************************************************
    /// Erases an element.
    ///\param key The key to erase.
    ///\return The number of elements erased. 0 or 1.
    //*********************************************************************
    size_t erase(const_key_reference key)
    {
      iterator i_element = find(key);

      if (i_element == end())
      {
        return 0;
      }
      else
      {
        i_element->~value_type();
        storage.release(etl::addressof(*i_element));
        refmap_t::erase(i_element);
        ETL_DECREMENT_DEBUG_COUNT;
        return 1;
      }
    }

#if ETL_USING_CPP11
    //*********************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    size_t erase(K&& key)
    {
      iterator i_element = find(etl::forward<K>(key));

      if (i_element == end())
      {
        return 0;
      }
      else
      {
        i_element->~value_type();
        storage.release(etl::addressof(*i_element));
        refmap_t::erase(i_element);
        ETL_DECREMENT_DEBUG_COUNT;
        return 1;
      }
    }
#endif

    //*********************************************************************
    /// Erases an element.
    ///\param i_element Iterator to the element.
    //*********************************************************************
    iterator erase(iterator i_element)
    {
      i_element->~value_type();
      storage.release(etl::addressof(*i_element));
      ETL_DECREMENT_DEBUG_COUNT
      return refmap_t::erase(i_element);
    }

    //*********************************************************************
    /// Erases an element.
    ///\ param i_element Iterator to the element.
    //*********************************************************************
    iterator erase(const_iterator i_element)
    {
      i_element->~value_type();
      storage.release(etl::addressof(*i_element));
      ETL_DECREMENT_DEBUG_COUNT
      return refmap_t::erase(i_element);
    }

    //*********************************************************************
    /// Erases a range of elements.
    /// The range includes all the elements between first and last, including the
    /// element pointed by first, but not the one pointed by last.
    ///\param first Iterator to the first element.
    ///\param last  Iterator to the last element.
    //*********************************************************************
    iterator erase(const_iterator first, const_iterator last)
    {
      const_iterator itr = first;

      while (itr != last)
      {
        itr->~value_type();
        storage.release(etl::addressof(*itr));
        ++itr;
        ETL_DECREMENT_DEBUG_COUNT
      }

      return refmap_t::erase(first, last);
    }

    //*************************************************************************
    /// Clears the flat_map.
    //*************************************************************************
    void clear()
    {
      if ETL_IF_CONSTEXPR(etl::is_trivially_destructible<value_type>::value)
      {
        storage.release_all();
      }
      else
      {
        iterator itr = begin();

        while (itr != end())
        {
          itr->~value_type();
          storage.release(etl::addressof(*itr));
          ++itr;
        }
      }

      ETL_RESET_DEBUG_COUNT
      refmap_t::clear();
    }

    //*********************************************************************
    /// Finds an element.
    ///\param key The key to search for.
    ///\return An iterator pointing to the element or end() if not found.
    //*********************************************************************
    iterator find(const_key_reference key)
    {
      return refmap_t::find(key);
    }

#if ETL_USING_CPP11
    //*********************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    iterator find(const K& key)
    {
      return refmap_t::find(key);
    }
#endif

    //*********************************************************************
    /// Finds an element.
    ///\param key The key to search for.
    ///\return An iterator pointing to the element or end() if not found.
    //*********************************************************************
    const_iterator find(const_key_reference key) const
    {
      return refmap_t::find(key);
    }

#if ETL_USING_CPP11
    //*********************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    const_iterator find(const K& key) const
    {
      return refmap_t::find(key);
    }
#endif

    //*********************************************************************
    /// Counts an element.
    ///\param key The key to search for.
    ///\return 1 if the key exists, otherwise 0.
    //*********************************************************************
    size_t count(const_key_reference key) const
    {
      return refmap_t::count(key);
    }

#if ETL_USING_CPP11
    //*********************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    size_t count(const K& key) const
    {
      return refmap_t::count(key);
    }
#endif

    //*********************************************************************
    /// Finds the lower bound of a key
    ///\param key The key to search for.
    ///\return An iterator.
    //*********************************************************************
    iterator lower_bound(const_key_reference key)
    {
      return refmap_t::lower_bound(key);
    }

#if ETL_USING_CPP11
    //*********************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    iterator lower_bound(const K& key)
    {
      return refmap_t::lower_bound(key);
    }
#endif

    //*********************************************************************
    /// Finds the lower bound of a key
    ///\param key The key to search for.
    ///\return An iterator.
    //*********************************************************************
    const_iterator lower_bound(const_key_reference key) const
    {
      return refmap_t::lower_bound(key);
    }

#if ETL_USING_CPP11
    //*********************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    const_iterator lower_bound(const K& key) const
    {
      return refmap_t::lower_bound(key);
    }
#endif

    //*********************************************************************
    /// Finds the upper bound of a key
    ///\param key The key to search for.
    ///\return An iterator.
    //*********************************************************************
    iterator upper_bound(const_key_reference key)
    {
      return refmap_t::upper_bound(key);
    }

#if ETL_USING_CPP11
    //*********************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    iterator upper_bound(const K& key)
    {
      return refmap_t::upper_bound(key);
    }
#endif

    //*********************************************************************
    /// Finds the upper bound of a key
    ///\param key The key to search for.
    ///\return An iterator.
    //*********************************************************************
    const_iterator upper_bound(const_key_reference key) const
    {
      return refmap_t::upper_bound(key);
    }

#if ETL_USING_CPP11
    //*********************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    const_iterator upper_bound(const K& key) const
    {
      return refmap_t::upper_bound(key);
    }
#endif

    //*********************************************************************
    /// Finds the range of equal elements of a key
    ///\param key The key to search for.
    ///\return An iterator pair.
    //*********************************************************************
    ETL_OR_STD::pair<iterator, iterator> equal_range(const_key_reference key)
    {
      return refmap_t::equal_range(key);
    }

#if ETL_USING_CPP11
    //*********************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    ETL_OR_STD::pair<iterator, iterator> equal_range(const K& key)
    {
      return refmap_t::equal_range(key);
    }
#endif

    //*********************************************************************
    /// Finds the range of equal elements of a key
    ///\param key The key to search for.
    ///\return An iterator pair.
    //*********************************************************************
    ETL_OR_STD::pair<const_iterator, const_iterator> equal_range(const_key_reference key) const
    {
      return refmap_t::equal_range(key);
    }

#if ETL_USING_CPP11
    //*********************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    ETL_OR_STD::pair<const_iterator, const_iterator> equal_range(const K& key) const
    {
      return refmap_t::equal_range(key);
    }
#endif

    //*************************************************************************
    /// Check if the map contains the key.
    //*************************************************************************
    bool contains(const_key_reference key) const
    {
      return find(key) != end();
    }

#if ETL_USING_CPP11
    //*************************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    bool contains(const K& k) const
    {
      return find(k) != end();
    }
#endif

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    iflat_map& operator = (const iflat_map& rhs)
    {
      if (&rhs != this)
      {
        assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Move assignment operator.
    //*************************************************************************
    iflat_map& operator = (iflat_map&& rhs)
    {
      move_container(etl::move(rhs));

      return *this;
    }
#endif

    //*************************************************************************
    /// Gets the current size of the flat_map.
    ///\return The current size of the flat_map.
    //*************************************************************************
    size_type size() const
    {
      return refmap_t::size();
    }

    //*************************************************************************
    /// Checks the 'empty' state of the flat_map.
    ///\return <b>true</b> if empty.
    //*************************************************************************
    bool empty() const
    {
      return refmap_t::empty();
    }

    //*************************************************************************
    /// Checks the 'full' state of the flat_map.
    ///\return <b>true</b> if full.
    //*************************************************************************
    bool full() const
    {
      return refmap_t::full();
    }

    //*************************************************************************
    /// Returns the capacity of the flat_map.
    ///\return The capacity of the flat_map.
    //*************************************************************************
    size_type capacity() const
    {
      return refmap_t::capacity();
    }

    //*************************************************************************
    /// Returns the maximum possible size of the flat_map.
    ///\return The maximum size of the flat_map.
    //*************************************************************************
    size_type max_size() const
    {
      return refmap_t::max_size();
    }

    //*************************************************************************
    /// Returns the remaining capacity.
    ///\return The remaining capacity.
    //*************************************************************************
    size_t available() const
    {
      return refmap_t::available();
    }

  protected:

    //*********************************************************************
    /// Constructor.
    //*********************************************************************
    iflat_map(lookup_t& lookup_, storage_t& storage_)
      : refmap_t(lookup_),
        storage(storage_)
    {
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Move a flat_map.
    /// Assumes the flat_map is initialised and empty.
    //*************************************************************************
    void move_container(iflat_map&& rhs)
    {
      if (&rhs != this)
      {
        this->clear();

        etl::iflat_map<TKey, TMapped, TKeyCompare>::iterator first = rhs.begin();
        etl::iflat_map<TKey, TMapped, TKeyCompare>::iterator last  = rhs.end();

        // Move all of the elements.
        while (first != last)
        {
          typename etl::iflat_map<TKey, TMapped, TKeyCompare>::iterator temp = first;
          ++temp;

          this->insert(etl::move(*first));
          first = temp;
        }
      }
    }
#endif

  private:

    // Disable copy construction.
    iflat_map(const iflat_map&);

    storage_t& storage;

    TKeyCompare compare;

    /// Internal debugging.
    ETL_DECLARE_DEBUG_COUNT

#if ETL_USING_CPP11
    //*************************************************************************
    template <typename TValueType>
    ETL_OR_STD::pair<iterator, bool> insert_value(iterator i_element, TValueType&& value)
    {
      ETL_ASSERT(!refmap_t::full(), ETL_ERROR(flat_map_full));

      value_type* pvalue = storage.allocate<value_type>();
      ::new (pvalue) value_type(etl::forward<TValueType>(value));
      ETL_INCREMENT_DEBUG_COUNT
      return refmap_t::insert_at(i_element, *pvalue);
    }
#else
    //*************************************************************************
    ETL_OR_STD::pair<iterator, bool> insert_value(iterator i_element, const_reference value)
    {
      ETL_ASSERT(!refmap_t::full(), ETL_ERROR(flat_map_full));

      value_type* pvalue = storage.allocate<value_type>();
      ::new (pvalue) value_type(value_type(value));
      ETL_INCREMENT_DEBUG_COUNT
      return refmap_t::insert_at(i_element, *pvalue);
    }
#endif

#if ETL_USING_CPP11
    //*************************************************************************
    ETL_OR_STD::pair<iterator, bool> insert_default_value(iterator i_element, rvalue_key_reference key)
    {
      ETL_ASSERT(!refmap_t::full(), ETL_ERROR(flat_map_full));

      value_type* pvalue = storage.allocate<value_type>();
      ::new ((void*)etl::addressof(pvalue->first)) key_type(etl::move(key));
      ::new ((void*)etl::addressof(pvalue->second)) mapped_type();
      ETL_INCREMENT_DEBUG_COUNT

      return refmap_t::insert_at(i_element, *pvalue);
    }
#endif

    //*************************************************************************
    ETL_OR_STD::pair<iterator, bool> insert_default_value(iterator i_element, const_key_reference key)
    {
      ETL_ASSERT(!refmap_t::full(), ETL_ERROR(flat_map_full));

      value_type* pvalue = storage.allocate<value_type>();
      ::new ((void*)etl::addressof(pvalue->first)) key_type(key);
      ::new ((void*)etl::addressof(pvalue->second)) mapped_type();
      ETL_INCREMENT_DEBUG_COUNT
      
      return refmap_t::insert_at(i_element, *pvalue);
    }

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
#if defined(ETL_POLYMORPHIC_FLAT_MAP) || defined(ETL_POLYMORPHIC_CONTAINERS)
  public:
    virtual ~iflat_map()
    {
    }
#else
  protected:
    ~iflat_map()
    {
    }
#endif
  };

  //***************************************************************************
  /// Equal operator.
  ///\param lhs Reference to the first flat_map.
  ///\param rhs Reference to the second flat_map.
  ///\return <b>true</b> if the arrays are equal, otherwise <b>false</b>
  ///\ingroup flat_map
  //***************************************************************************
  template <typename TKey, typename TMapped, typename TKeyCompare>
  bool operator ==(const etl::iflat_map<TKey, TMapped, TKeyCompare>& lhs, const etl::iflat_map<TKey, TMapped, TKeyCompare>& rhs)
  {
    return (lhs.size() == rhs.size()) && etl::equal(lhs.begin(), lhs.end(), rhs.begin());
  }

  //***************************************************************************
  /// Not equal operator.
  ///\param lhs Reference to the first flat_map.
  ///\param rhs Reference to the second flat_map.
  ///\return <b>true</b> if the arrays are not equal, otherwise <b>false</b>
  ///\ingroup flat_map
  //***************************************************************************
  template <typename TKey, typename TMapped, typename TKeyCompare>
  bool operator !=(const etl::iflat_map<TKey, TMapped, TKeyCompare>& lhs, const etl::iflat_map<TKey, TMapped, TKeyCompare>& rhs)
  {
    return !(lhs == rhs);
  }

  //***************************************************************************
  /// A flat_map implementation that uses a fixed size buffer.
  ///\tparam TKey     The key type.
  ///\tparam TValue   The value type.
  ///\tparam TCompare The type to compare keys. Default = etl::less<TKey>
  ///\tparam MAX_SIZE_ The maximum number of elements that can be stored.
  ///\ingroup flat_map
  //***************************************************************************
  template <typename TKey, typename TValue, const size_t MAX_SIZE_, typename TCompare = etl::less<TKey> >
  class flat_map : public etl::iflat_map<TKey, TValue, TCompare>
  {
  public:

    static ETL_CONSTANT size_t MAX_SIZE = MAX_SIZE_;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    flat_map()
      : etl::iflat_map<TKey, TValue, TCompare>(lookup, storage)
    {
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    flat_map(const flat_map& other)
      : etl::iflat_map<TKey, TValue, TCompare>(lookup, storage)
    {
      this->assign(other.cbegin(), other.cend());
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Move constructor.
    //*************************************************************************
    flat_map(flat_map&& other)
      : etl::iflat_map<TKey, TValue, TCompare>(lookup, storage)
    {
      if (&other != this)
      {
        this->move_container(etl::move(other));
      }
    }
#endif

    //*************************************************************************
    /// Constructor, from an iterator range.
    ///\tparam TIterator The iterator type.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*************************************************************************
    template <typename TIterator>
    flat_map(TIterator first, TIterator last)
      : etl::iflat_map<TKey, TValue, TCompare>(lookup, storage)
    {
      this->assign(first, last);
    }

#if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    /// Construct from initializer_list.
    //*************************************************************************
    flat_map(std::initializer_list<typename etl::iflat_map<TKey, TValue, TCompare>::value_type> init)
      : etl::iflat_map<TKey, TValue, TCompare>(lookup, storage)
    {
      this->assign(init.begin(), init.end());
    }
#endif

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~flat_map()
    {
      this->clear();
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    flat_map& operator = (const flat_map& rhs)
    {
      if (&rhs != this)
      {
        this->assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Move assignment operator.
    //*************************************************************************
    flat_map& operator = (flat_map&& rhs)
    {
      if (&rhs != this)
      {
        this->move_container(etl::move(rhs));
      }

      return *this;
    }
#endif

  private:

    typedef typename etl::iflat_map<TKey, TValue, TCompare>::value_type node_t;

    /// The pool of nodes.
    etl::pool<node_t, MAX_SIZE> storage;

    /// The vector that stores pointers to the nodes.
    etl::vector<node_t*, MAX_SIZE> lookup;
  };

  template <typename TKey, typename TValue, const size_t MAX_SIZE_, typename TCompare>
  ETL_CONSTANT size_t flat_map<TKey, TValue, MAX_SIZE_, TCompare>::MAX_SIZE;

  //*************************************************************************
  /// Template deduction guides.
  //*************************************************************************
#if ETL_USING_CPP17 && ETL_HAS_INITIALIZER_LIST
  template <typename... TPairs>
  flat_map(TPairs...) -> flat_map<typename etl::nth_type_t<0, TPairs...>::first_type,
                                  typename etl::nth_type_t<0, TPairs...>::second_type,
                                  sizeof...(TPairs)>;
#endif

  //*************************************************************************
  /// Make
  //*************************************************************************
#if ETL_USING_CPP11 && ETL_HAS_INITIALIZER_LIST
  template <typename TKey, typename TMapped, typename TKeyCompare = etl::less<TKey>, typename... TPairs>
  constexpr auto make_flat_map(TPairs&&... pairs) -> etl::flat_map<TKey, TMapped, sizeof...(TPairs), TKeyCompare>
  {
    return { {etl::forward<TPairs>(pairs)...} };
  }
#endif
}

#endif
