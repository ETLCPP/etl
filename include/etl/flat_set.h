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

#ifndef ETL_FLAT_SET_INCLUDED
#define ETL_FLAT_SET_INCLUDED

#include "platform.h"
#include "reference_flat_set.h"
#include "pool.h"
#include "placement_new.h"
#include "nth_type.h"
#include "type_traits.h"
#include "initializer_list.h"

#include "private/comparator_is_transparent.h"

//*****************************************************************************
///\defgroup flat_set flat_set
/// A flat_set with the capacity defined at compile time.
/// Has insertion of O(N) and find of O(logN)
/// Duplicate entries and not allowed.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //***************************************************************************
  /// The base class for specifically sized flat_sets.
  /// Can be used as a reference type for all flat_sets containing a specific type.
  ///\ingroup flat_set
  //***************************************************************************
  template <typename T, typename TKeyCompare = etl::less<T> >
  class iflat_set : private etl::ireference_flat_set<T, TKeyCompare>
  {
  private:

    typedef etl::ireference_flat_set<T, TKeyCompare> refset_t;
    typedef typename refset_t::lookup_t lookup_t;
    typedef etl::ipool storage_t;

    typedef const T& key_parameter_t;

  public:

    typedef T                 key_type;
    typedef T                 value_type;
    typedef TKeyCompare       key_compare;
    typedef value_type&       reference;
    typedef const value_type& const_reference;
#if ETL_USING_CPP11
    typedef value_type&&      rvalue_reference;
#endif
    typedef value_type*       pointer;
    typedef const value_type* const_pointer;
    typedef size_t            size_type;

    typedef typename refset_t::iterator       iterator;
    typedef typename refset_t::const_iterator const_iterator;

    typedef ETL_OR_STD::reverse_iterator<iterator>       reverse_iterator;
    typedef ETL_OR_STD::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef typename etl::iterator_traits<iterator>::difference_type difference_type;

  public:

    //*********************************************************************
    /// Returns an iterator to the beginning of the flat_set.
    ///\return An iterator to the beginning of the flat_set.
    //*********************************************************************
    iterator begin()
    {
      return refset_t::begin();
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the flat_set.
    ///\return A const iterator to the beginning of the flat_set.
    //*********************************************************************
    const_iterator begin() const
    {
      return refset_t::begin();
    }

    //*********************************************************************
    /// Returns an iterator to the end of the flat_set.
    ///\return An iterator to the end of the flat_set.
    //*********************************************************************
    iterator end()
    {
      return refset_t::end();
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the flat_set.
    ///\return A const iterator to the end of the flat_set.
    //*********************************************************************
    const_iterator end() const
    {
      return refset_t::end();
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the flat_set.
    ///\return A const iterator to the beginning of the flat_set.
    //*********************************************************************
    const_iterator cbegin() const
    {
      return refset_t::cbegin();
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the flat_set.
    ///\return A const iterator to the end of the flat_set.
    //*********************************************************************
    const_iterator cend() const
    {
      return refset_t::cend();
    }

    //*********************************************************************
    /// Returns an reverse iterator to the reverse beginning of the flat_set.
    ///\return Iterator to the reverse beginning of the flat_set.
    //*********************************************************************
    reverse_iterator rbegin()
    {
      return refset_t::rbegin();
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the flat_set.
    ///\return Const iterator to the reverse beginning of the flat_set.
    //*********************************************************************
    const_reverse_iterator rbegin() const
    {
      return refset_t::rbegin();
    }

    //*********************************************************************
    /// Returns a reverse iterator to the end + 1 of the flat_set.
    ///\return Reverse iterator to the end + 1 of the flat_set.
    //*********************************************************************
    reverse_iterator rend()
    {
      return refset_t::rend();
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the end + 1 of the flat_set.
    ///\return Const reverse iterator to the end + 1 of the flat_set.
    //*********************************************************************
    const_reverse_iterator rend() const
    {
      return refset_t::rend();
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the flat_set.
    ///\return Const reverse iterator to the reverse beginning of the flat_set.
    //*********************************************************************
    const_reverse_iterator crbegin() const
    {
      return refset_t::crbegin();
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the end + 1 of the flat_set.
    ///\return Const reverse iterator to the end + 1 of the flat_set.
    //*********************************************************************
    const_reverse_iterator crend() const
    {
      return refset_t::crend();
    }

    //*********************************************************************
    /// Assigns values to the flat_set.
    /// If asserts or exceptions are enabled, emits flat_set_full if the flat_set does not have enough free space.
    /// If asserts or exceptions are enabled, emits flat_set_iterator if the iterators are reversed.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*********************************************************************
    template <typename TIterator>
    void assign(TIterator first, TIterator last)
    {
#if ETL_IS_DEBUG_BUILD
      difference_type d = etl::distance(first, last);
      ETL_ASSERT(d <= difference_type(capacity()), ETL_ERROR(flat_set_full));
#endif

      clear();

      while (first != last)
      {
        insert(*first);
        ++first;
      }
    }

    //*********************************************************************
    /// Inserts a value to the flat_set.
    /// If asserts or exceptions are enabled, emits flat_set_full if the flat_set is already full.
    ///\param value    The value to insert.
    //*********************************************************************
    ETL_OR_STD::pair<iterator, bool> insert(const_reference value)
    {
      iterator i_element = lower_bound(value);

      ETL_OR_STD::pair<iterator, bool> result(i_element, false);

      // Doesn't already exist?
      if ((i_element == end()) || compare(value, *i_element))
      {
        ETL_ASSERT(!refset_t::full(), ETL_ERROR(flat_set_full));

        value_type* pvalue = storage.allocate<value_type>();
        ::new (pvalue) value_type(value);
        ETL_INCREMENT_DEBUG_COUNT
        result = refset_t::insert_at(i_element, *pvalue);
      }

      return result;
    }

#if ETL_USING_CPP11
    //*********************************************************************
    /// Inserts a value to the flat_set.
    /// If asserts or exceptions are enabled, emits flat_set_full if the flat_set is already full.
    ///\param value    The value to insert.
    //*********************************************************************
    ETL_OR_STD::pair<iterator, bool> insert(rvalue_reference value)
    {
      iterator i_element = lower_bound(value);

      ETL_OR_STD::pair<iterator, bool> result(i_element, false);

      // Doesn't already exist?
      if ((i_element == end()) || compare(value, *i_element))
      {
        ETL_ASSERT(!refset_t::full(), ETL_ERROR(flat_set_full));

        value_type* pvalue = storage.allocate<value_type>();
        ::new (pvalue) value_type(etl::move(value));
        ETL_INCREMENT_DEBUG_COUNT
        result = refset_t::insert_at(i_element, *pvalue);
      }

      return result;
    }
#endif

    //*********************************************************************
    /// Inserts a value to the flat_set.
    /// If asserts or exceptions are enabled, emits flat_set_full if the flat_set is already full.
    ///\param position The position to insert at.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(const_iterator /*position*/, const_reference value)
    {
      return insert(value).first;
    }

#if ETL_USING_CPP11
    //*********************************************************************
    /// Inserts a value to the flat_set.
    /// If asserts or exceptions are enabled, emits flat_set_full if the flat_set is already full.
    ///\param position The position to insert at.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(const_iterator /*position*/, rvalue_reference value)
    {
      return insert(etl::move(value)).first;
    }
#endif

    //*********************************************************************
    /// Inserts a range of values to the flat_set.
    /// If asserts or exceptions are enabled, emits flat_set_full if the flat_set does not have enough free space.
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
    /// Emplaces a value to the set.
    //*************************************************************************
    ETL_OR_STD::pair<iterator, bool> emplace(const_reference value)
    {
      return insert(value);
    }

    //*************************************************************************
    /// Emplaces a value to the set.
    //*************************************************************************
#if ETL_USING_CPP11 && ETL_NOT_USING_STLPORT
    template <typename ... Args>
    ETL_OR_STD::pair<iterator, bool> emplace(Args && ... args)
    {
      ETL_ASSERT(!full(), ETL_ERROR(flat_set_full));

      ETL_OR_STD::pair<iterator, bool> result;

      // Create it.
      value_type* pvalue = storage.allocate<value_type>();
      ::new (pvalue) value_type(etl::forward<Args>(args)...);

      iterator i_element = lower_bound(*pvalue);

      // Doesn't already exist?
      if ((i_element == end()) || compare(*pvalue, *i_element))
      {
        ETL_INCREMENT_DEBUG_COUNT
        result = refset_t::insert_at(i_element, *pvalue);
      }
      else
      {
        // Destroy it.
        pvalue->~value_type();
        storage.release(pvalue);
        result = ETL_OR_STD::pair<iterator, bool>(end(), false);
      }

      return result;
    }
#else
    //*************************************************************************
    /// Emplaces a value to the set.
    //*************************************************************************
    template <typename T1>
    ETL_OR_STD::pair<iterator, bool> emplace(const T1& value1)
    {
      ETL_ASSERT(!full(), ETL_ERROR(flat_set_full));

      ETL_OR_STD::pair<iterator, bool> result;

      // Create it.
      value_type* pvalue = storage.allocate<value_type>();
      ::new (pvalue) value_type(value1);

      iterator i_element = lower_bound(*pvalue);

      // Doesn't already exist?
      if ((i_element == end()) || compare(*pvalue, *i_element))
      {
        ETL_INCREMENT_DEBUG_COUNT
        result = refset_t::insert_at(i_element, *pvalue);
      }
      else
      {
        // Destroy it.
        pvalue->~value_type();
        storage.release(pvalue);
        result = ETL_OR_STD::pair<iterator, bool>(end(), false);
      }

      return result;
    }

    //*************************************************************************
    /// Emplaces a value to the set.
    //*************************************************************************
    template <typename T1, typename T2>
    ETL_OR_STD::pair<iterator, bool> emplace(const T1& value1, const T2& value2)
    {
      ETL_ASSERT(!full(), ETL_ERROR(flat_set_full));

      ETL_OR_STD::pair<iterator, bool> result;

      // Create it.
      value_type* pvalue = storage.allocate<value_type>();
      ::new (pvalue) value_type(value1, value2);

      iterator i_element = lower_bound(*pvalue);

      // Doesn't already exist?
      if ((i_element == end()) || compare(*pvalue, *i_element))
      {
        ETL_INCREMENT_DEBUG_COUNT
        result = refset_t::insert_at(i_element, *pvalue);
      }
      else
      {
        // Destroy it.
        pvalue->~value_type();
        storage.release(pvalue);
        result = ETL_OR_STD::pair<iterator, bool>(end(), false);
      }

      return result;
    }

    //*************************************************************************
    /// Emplaces a value to the set.
    //*************************************************************************
    template <typename T1, typename T2, typename T3>
    ETL_OR_STD::pair<iterator, bool> emplace(const T1& value1, const T2& value2, const T3& value3)
    {
      ETL_ASSERT(!full(), ETL_ERROR(flat_set_full));

      ETL_OR_STD::pair<iterator, bool> result;

      // Create it.
      value_type* pvalue = storage.allocate<value_type>();
      ::new (pvalue) value_type(value1, value2, value3);

      iterator i_element = lower_bound(*pvalue);

      // Doesn't already exist?
      if ((i_element == end()) || compare(*pvalue, *i_element))
      {
        ETL_INCREMENT_DEBUG_COUNT
        result = refset_t::insert_at(i_element, *pvalue);
      }
      else
      {
        // Destroy it.
        pvalue->~value_type();
        storage.release(pvalue);
        result = ETL_OR_STD::pair<iterator, bool>(end(), false);
      }

      return result;
    }

    //*************************************************************************
    /// Emplaces a value to the set.
    //*************************************************************************
    template <typename T1, typename T2, typename T3, typename T4>
    ETL_OR_STD::pair<iterator, bool> emplace(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
    {
      ETL_ASSERT(!full(), ETL_ERROR(flat_set_full));

      ETL_OR_STD::pair<iterator, bool> result;

      // Create it.
      value_type* pvalue = storage.allocate<value_type>();
      ::new (pvalue) value_type(value1, value2, value3, value4);

      iterator i_element = lower_bound(*pvalue);

      // Doesn't already exist?
      if ((i_element == end()) || compare(*pvalue, *i_element))
      {
        ETL_INCREMENT_DEBUG_COUNT
        result = refset_t::insert_at(i_element, *pvalue);
      }
      else
      {
        // Destroy it.
        pvalue->~value_type();
        storage.release(pvalue);
        result = ETL_OR_STD::pair<iterator, bool>(end(), false);
      }

      return result;
    }
#endif

    //*********************************************************************
    /// Erases an element.
    ///\param key The key to erase.
    ///\return The number of elements erased. 0 or 1.
    //*********************************************************************
    size_t erase(key_parameter_t key)
    {
      iterator i_element = find(key);

      if (i_element == end())
      {
        return 0;
      }
      else
      {
        etl::destroy_at(etl::addressof(*i_element));
        storage.release(etl::addressof(*i_element));
        refset_t::erase(i_element);
        ETL_DECREMENT_DEBUG_COUNT
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
        etl::destroy_at(etl::addressof(*i_element));
        storage.release(etl::addressof(*i_element));
        refset_t::erase(i_element);
        ETL_DECREMENT_DEBUG_COUNT
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
      etl::destroy_at(etl::addressof(*i_element));
      storage.release(etl::addressof(*i_element));
      ETL_DECREMENT_DEBUG_COUNT
      return refset_t::erase(i_element);
    }

    //*********************************************************************
    /// Erases an element.
    ///\param i_element Iterator to the element.
    //*********************************************************************
    iterator erase(const_iterator i_element)
    {
      etl::destroy_at(etl::addressof(*i_element));
      storage.release(etl::addressof(*i_element));
      ETL_DECREMENT_DEBUG_COUNT
      return refset_t::erase(i_element);
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
        etl::destroy_at(etl::addressof(*itr));
        storage.release(etl::addressof(*itr));
        ++itr;
        ETL_DECREMENT_DEBUG_COUNT
      }

      return refset_t::erase(first, last);
    }

    //*************************************************************************
    /// Clears the flat_set.
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
          etl::destroy_at(etl::addressof(*itr));
          storage.release(etl::addressof(*itr));
          ++itr;
          ETL_DECREMENT_DEBUG_COUNT
        }
      }

      ETL_RESET_DEBUG_COUNT
      refset_t::clear();
    }

    //*********************************************************************
    /// Finds an element.
    ///\param key The key to search for.
    ///\return An iterator pointing to the element or end() if not found.
    //*********************************************************************
    iterator find(key_parameter_t key)
    {
      return refset_t::find(key);
    }

#if ETL_USING_CPP11
    //*********************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    iterator find(const K& key)
    {
      return refset_t::find(key);
    }
#endif

    //*********************************************************************
    /// Finds an element.
    ///\param key The key to search for.
    ///\return An iterator pointing to the element or end() if not found.
    //*********************************************************************
    const_iterator find(key_parameter_t key) const
    {
      return refset_t::find(key);
    }

#if ETL_USING_CPP11
    //*********************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    const_iterator find(const K& key) const
    {
      return refset_t::find(key);
    }
#endif

    //*********************************************************************
    /// Counts an element.
    ///\param key The key to search for.
    ///\return 1 if the key exists, otherwise 0.
    //*********************************************************************
    size_t count(key_parameter_t key) const
    {
      return refset_t::count(key);
    }

#if ETL_USING_CPP11
    //*********************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    size_t count(const K& key) const
    {
      return refset_t::count(key);
    }
#endif

    //*********************************************************************
    /// Finds the lower bound of a key
    ///\param key The key to search for.
    ///\return An iterator.
    //*********************************************************************
    iterator lower_bound(key_parameter_t key)
    {
      return refset_t::lower_bound(key);
    }

#if ETL_USING_CPP11
    //*********************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    iterator lower_bound(const K& key)
    {
      return refset_t::lower_bound(key);
    }
#endif

    //*********************************************************************
    /// Finds the lower bound of a key
    ///\param key The key to search for.
    ///\return An iterator.
    //*********************************************************************
    const_iterator lower_bound(key_parameter_t key) const
    {
      return refset_t::lower_bound(key);
    }

#if ETL_USING_CPP11
    //*********************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    const_iterator lower_bound(const K& key) const
    {
      return refset_t::lower_bound(key);
    }
#endif

    //*********************************************************************
    /// Finds the upper bound of a key
    ///\param key The key to search for.
    ///\return An iterator.
    //*********************************************************************
    iterator upper_bound(key_parameter_t key)
    {
      return refset_t::upper_bound(key);
    }

#if ETL_USING_CPP11
    //*********************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    iterator upper_bound(const K& key)
    {
      return refset_t::upper_bound(key);
    }
#endif

    //*********************************************************************
    /// Finds the upper bound of a key
    ///\param key The key to search for.
    ///\return An iterator.
    //*********************************************************************
    const_iterator upper_bound(key_parameter_t key) const
    {
      return refset_t::upper_bound(key);
    }

#if ETL_USING_CPP11
    //*********************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    const_iterator upper_bound(const K& key) const
    {
      return refset_t::upper_bound(key);
    }
#endif

    //*********************************************************************
    /// Finds the range of equal elements of a key
    ///\param key The key to search for.
    ///\return An iterator pair.
    //*********************************************************************
    ETL_OR_STD::pair<iterator, iterator> equal_range(key_parameter_t key)
    {
      return refset_t::equal_range(key);
    }

#if ETL_USING_CPP11
    //*********************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    ETL_OR_STD::pair<iterator, iterator> equal_range(const K& key)
    {
      return refset_t::equal_range(key);
    }
#endif

    //*********************************************************************
    /// Finds the range of equal elements of a key
    ///\param key The key to search for.
    ///\return An iterator pair.
    //*********************************************************************
    ETL_OR_STD::pair<const_iterator, const_iterator> equal_range(key_parameter_t key) const
    {
      return refset_t::upper_bound(key);
    }

#if ETL_USING_CPP11
    //*********************************************************************
    template <typename K, typename KC = TKeyCompare, etl::enable_if_t<comparator_is_transparent<KC>::value, int> = 0>
    ETL_OR_STD::pair<const_iterator, const_iterator> equal_range(const K& key) const
    {
      return refset_t::upper_bound(key);
    }
#endif

    //*************************************************************************
    /// Check if the map contains the key.
    //*************************************************************************
    bool contains(key_parameter_t key) const
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
    iflat_set& operator = (const iflat_set& rhs)
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
    iflat_set& operator = (iflat_set&& rhs)
    {
      move_container(etl::move(rhs));

      return *this;
    }
#endif

    //*************************************************************************
    /// Gets the current size of the flat_set.
    ///\return The current size of the flat_set.
    //*************************************************************************
    size_type size() const
    {
      return refset_t::size();
    }

    //*************************************************************************
    /// Checks the 'empty' state of the flat_set.
    ///\return <b>true</b> if empty.
    //*************************************************************************
    bool empty() const
    {
      return refset_t::empty();
    }

    //*************************************************************************
    /// Checks the 'full' state of the flat_set.
    ///\return <b>true</b> if full.
    //*************************************************************************
    bool full() const
    {
      return refset_t::full();
    }

    //*************************************************************************
    /// Returns the capacity of the flat_set.
    ///\return The capacity of the flat_set.
    //*************************************************************************
    size_type capacity() const
    {
      return refset_t::capacity();
    }

    //*************************************************************************
    /// Returns the maximum possible size of the flat_set.
    ///\return The maximum size of the flat_set.
    //*************************************************************************
    size_type max_size() const
    {
      return refset_t::max_size();
    }

    //*************************************************************************
    /// Returns the remaining capacity.
    ///\return The remaining capacity.
    //*************************************************************************
    size_t available() const
    {
      return refset_t::available();
    }

  protected:

    //*********************************************************************
    /// Constructor.
    //*********************************************************************
    iflat_set(lookup_t& lookup_, storage_t& storage_)
      : refset_t(lookup_),
        storage(storage_)
    {
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Move a flat_set.
    /// Assumes the flat_set is initialised and empty.
    //*************************************************************************
    void move_container(iflat_set&& rhs)
    {
      if (&rhs != this)
      {
        this->clear();

        etl::iflat_set<T, TKeyCompare>::iterator first = rhs.begin();
        etl::iflat_set<T, TKeyCompare>::iterator last = rhs.end();

        // Move all of the elements.
        while (first != last)
        {
          typename etl::iflat_set<T, TKeyCompare>::iterator temp = first;
          ++temp;

          this->insert(etl::move(*first));
          first = temp;
        }
      }
    }
#endif

  private:

    // Disable copy construction.
    iflat_set(const iflat_set&);

    storage_t& storage;

    TKeyCompare compare;

    /// Internal debugging.
    ETL_DECLARE_DEBUG_COUNT

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
#if defined(ETL_POLYMORPHIC_FLAT_SET) || defined(ETL_POLYMORPHIC_CONTAINERS)
  public:
    virtual ~iflat_set()
    {
    }
#else
  protected:
    ~iflat_set()
    {
    }
#endif
  };

  //***************************************************************************
  /// Equal operator.
  ///\param lhs Reference to the first flat_set.
  ///\param rhs Reference to the second flat_set.
  ///\return <b>true</b> if the arrays are equal, otherwise <b>false</b>
  ///\ingroup flat_set
  //***************************************************************************
  template <typename T, typename TKeyCompare>
  bool operator ==(const etl::iflat_set<T, TKeyCompare>& lhs, const etl::iflat_set<T, TKeyCompare>& rhs)
  {
    return (lhs.size() == rhs.size()) && etl::equal(lhs.begin(), lhs.end(), rhs.begin());
  }

  //***************************************************************************
  /// Not equal operator.
  ///\param lhs Reference to the first flat_set.
  ///\param rhs Reference to the second flat_set.
  ///\return <b>true</b> if the arrays are not equal, otherwise <b>false</b>
  ///\ingroup flat_set
  //***************************************************************************
  template <typename T, typename TKeyCompare>
  bool operator !=(const etl::iflat_set<T, TKeyCompare>& lhs, const etl::iflat_set<T, TKeyCompare>& rhs)
  {
    return !(lhs == rhs);
  }

  //***************************************************************************
  /// A flat_set implementation that uses a fixed size buffer.
  ///\tparam T        The value type.
  ///\tparam TCompare The type to compare keys. Default = etl::less<T>
  ///\tparam MAX_SIZE_ The maximum number of elements that can be stored.
  ///\ingroup flat_set
  //***************************************************************************
  template <typename T, const size_t MAX_SIZE_, typename TCompare = etl::less<T> >
  class flat_set : public etl::iflat_set<T, TCompare>
  {
  public:

    static ETL_CONSTANT size_t MAX_SIZE = MAX_SIZE_;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    flat_set()
      : etl::iflat_set<T, TCompare>(lookup, storage)
    {
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    flat_set(const flat_set& other)
      : etl::iflat_set<T, TCompare>(lookup, storage)
    {
      this->assign(other.cbegin(), other.cend());
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Move constructor.
    //*************************************************************************
    flat_set(flat_set&& other)
      : etl::iflat_set<T, TCompare>(lookup, storage)
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
    flat_set(TIterator first, TIterator last)
      : etl::iflat_set<T, TCompare>(lookup, storage)
    {
      this->assign(first, last);
    }

#if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    /// Construct from initializer_list.
    //*************************************************************************
    flat_set(std::initializer_list<T> init)
      : etl::iflat_set<T, TCompare>(lookup, storage)
    {
      this->assign(init.begin(), init.end());
    }
#endif

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~flat_set()
    {
      this->clear();
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    flat_set& operator = (const flat_set& rhs)
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
    flat_set& operator = (flat_set&& rhs)
    {
      if (&rhs != this)
      {
        this->move_container(etl::move(rhs));
      }

      return *this;
    }
#endif

  private:

    typedef typename etl::iflat_set<T, TCompare>::value_type node_t;

    // The pool of nodes.
    etl::pool<node_t, MAX_SIZE> storage;

    // The vector that stores pointers to the nodes.
    etl::vector<node_t*, MAX_SIZE> lookup;
  };

  template <typename T, const size_t MAX_SIZE_, typename TCompare>
  ETL_CONSTANT size_t flat_set<T, MAX_SIZE_, TCompare>::MAX_SIZE;

  //*************************************************************************
  /// Template deduction guides.
  //*************************************************************************
#if ETL_USING_CPP17 && ETL_HAS_INITIALIZER_LIST
  template <typename... T>
  flat_set(T...) -> flat_set<etl::nth_type_t<0, T...>, sizeof...(T)>;
#endif

  //*************************************************************************
  /// Make
  //*************************************************************************
#if ETL_USING_CPP11 && ETL_HAS_INITIALIZER_LIST
  template <typename TKey, typename TKeyCompare = etl::less<TKey>, typename... T>
  constexpr auto make_flat_set(T&&... keys) -> etl::flat_set<TKey, sizeof...(T), TKeyCompare>
  {
    return { {etl::forward<T>(keys)...} };
  }
#endif
}

#endif
