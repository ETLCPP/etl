///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2015 jwellbelove

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

#ifndef ETL_FLAT_MULTMAP_INCLUDED
#define ETL_FLAT_MULTMAP_INCLUDED

#include <new>

#include "platform.h"
#include "reference_flat_multimap.h"
#include "pool.h"
#include "utility.h"

#if ETL_CPP11_SUPPORTED && ETL_NOT_USING_STLPORT && ETL_USING_STL
  #include <initializer_list>
#endif

#undef ETL_FILE
#define ETL_FILE "3"

//*****************************************************************************
///\defgroup flat_multimap flat_multimap
/// A flat_multimap with the capacity defined at compile time.
/// Has insertion of O(N) and find of O(logN)
/// Duplicate entries and not allowed.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //***************************************************************************
  /// The base class for specifically sized flat_multimaps.
  /// Can be used as a reference type for all flat_multimaps containing a specific type.
  ///\ingroup flat_multimap
  //***************************************************************************
  template <typename TKey, typename TMapped, typename TKeyCompare = etl::less<TKey> >
  class iflat_multimap : public etl::ireference_flat_multimap<TKey, TMapped, TKeyCompare>
  {
  public:

    typedef ETL_OR_STD::pair<const TKey, TMapped> value_type;

  private:

    typedef etl::ireference_flat_multimap<TKey, TMapped, TKeyCompare> refmap_t;
    typedef typename refmap_t::lookup_t lookup_t;
    typedef etl::ipool         storage_t;

  public:

    typedef TKey              key_type;
    typedef TMapped           mapped_type;
    typedef TKeyCompare       key_compare;
    typedef value_type&       reference;
    typedef const value_type& const_reference;
#if ETL_CPP11_SUPPORTED
    typedef value_type&&      rvalue_reference;
#endif
    typedef value_type*       pointer;
    typedef const value_type* const_pointer;
    typedef size_t            size_type;

    typedef typename refmap_t::iterator       iterator;
    typedef typename refmap_t::const_iterator const_iterator;

    typedef ETL_OR_STD::reverse_iterator<iterator>       reverse_iterator;
    typedef ETL_OR_STD::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef typename etl::iterator_traits<iterator>::difference_type difference_type;

  protected:

    typedef const key_type& key_parameter_t;

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
    /// Returns an iterator to the beginning of the flat_multimap.
    ///\return An iterator to the beginning of the flat_multimap.
    //*********************************************************************
    iterator begin()
    {
      return refmap_t::begin();
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the flat_multimap.
    ///\return A const iterator to the beginning of the flat_multimap.
    //*********************************************************************
    const_iterator begin() const
    {
      return refmap_t::begin();
    }

    //*********************************************************************
    /// Returns an iterator to the end of the flat_multimap.
    ///\return An iterator to the end of the flat_multimap.
    //*********************************************************************
    iterator end()
    {
      return refmap_t::end();
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the flat_multimap.
    ///\return A const iterator to the end of the flat_multimap.
    //*********************************************************************
    const_iterator end() const
    {
      return refmap_t::end();
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the flat_multimap.
    ///\return A const iterator to the beginning of the flat_multimap.
    //*********************************************************************
    const_iterator cbegin() const
    {
      return refmap_t::cbegin();
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the flat_multimap.
    ///\return A const iterator to the end of the flat_multimap.
    //*********************************************************************
    const_iterator cend() const
    {
      return refmap_t::cend();
    }

    //*********************************************************************
    /// Returns an reverse iterator to the reverse beginning of the flat_multimap.
    ///\return Iterator to the reverse beginning of the flat_multimap.
    //*********************************************************************
    reverse_iterator rbegin()
    {
      return refmap_t::rbegin();
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the flat_multimap.
    ///\return Const iterator to the reverse beginning of the flat_multimap.
    //*********************************************************************
    const_reverse_iterator rbegin() const
    {
      return refmap_t::rbegin();
    }

    //*********************************************************************
    /// Returns a reverse iterator to the end + 1 of the flat_multimap.
    ///\return Reverse iterator to the end + 1 of the flat_multimap.
    //*********************************************************************
    reverse_iterator rend()
    {
      return refmap_t::rend();
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the end + 1 of the flat_multimap.
    ///\return Const reverse iterator to the end + 1 of the flat_multimap.
    //*********************************************************************
    const_reverse_iterator rend() const
    {
      refmap_t::rend();
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the flat_multimap.
    ///\return Const reverse iterator to the reverse beginning of the flat_multimap.
    //*********************************************************************
    const_reverse_iterator crbegin() const
    {
      return refmap_t::crbegin();
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the end + 1 of the flat_multimap.
    ///\return Const reverse iterator to the end + 1 of the flat_multimap.
    //*********************************************************************
    const_reverse_iterator crend() const
    {
      return refmap_t::crend();
    }

    //*********************************************************************
    /// Assigns values to the flat_multimap.
    /// If asserts or exceptions are enabled, emits flat_multimap_full if the flat_multimap does not have enough free space.
    /// If asserts or exceptions are enabled, emits flat_multimap_iterator if the iterators are reversed.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*********************************************************************
    template <typename TIterator>
    void assign(TIterator first, TIterator last)
    {
#if defined(ETL_DEBUG)
      difference_type d = etl::distance(first, last);
      ETL_ASSERT(d <= difference_type(capacity()), ETL_ERROR(flat_multimap_full));
#endif

      clear();

      while (first != last)
      {
        insert(*first++);
      }
    }

    //*********************************************************************
    /// Inserts a value to the flat_multimap.
    /// If asserts or exceptions are enabled, emits flat_multimap_full if the flat_multimap is already full.
    ///\param value    The value to insert.
    //*********************************************************************
    ETL_OR_STD::pair<iterator, bool> insert(const value_type& value)
    {
      ETL_ASSERT(!refmap_t::full(), ETL_ERROR(flat_multimap_full));

      ETL_OR_STD::pair<iterator, bool> result(end(), false);

      iterator i_element = lower_bound(value.first);

      value_type* pvalue = storage.allocate<value_type>();
      ::new (pvalue) value_type(value);
      ETL_INCREMENT_DEBUG_COUNT
      result = refmap_t::insert_at(i_element, *pvalue);

      return result;
    }

#if ETL_CPP11_SUPPORTED
    //*********************************************************************
    /// Inserts a value to the flat_multimap.
    /// If asserts or exceptions are enabled, emits flat_multimap_full if the flat_multimap is already full.
    ///\param value    The value to insert.
    //*********************************************************************
    ETL_OR_STD::pair<iterator, bool> insert(rvalue_reference value)
    {
      ETL_ASSERT(!refmap_t::full(), ETL_ERROR(flat_multimap_full));

      ETL_OR_STD::pair<iterator, bool> result(end(), false);

      iterator i_element = lower_bound(value.first);

      value_type* pvalue = storage.allocate<value_type>();
      ::new (pvalue) value_type(etl::move(value));
      ETL_INCREMENT_DEBUG_COUNT
      result = refmap_t::insert_at(i_element, *pvalue);

      return result;
    }
#endif

    //*********************************************************************
    /// Inserts a value to the flat_multimap.
    /// If asserts or exceptions are enabled, emits flat_multimap_full if the flat_multimap_full is already full.
    ///\param position The position to insert at.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(iterator position, const value_type& value)
    {
      return insert(value).first;
    }

#if ETL_CPP11_SUPPORTED
    //*********************************************************************
    /// Moves a value to the flat_multimap.
    /// If asserts or exceptions are enabled, emits flat_multimap_full if the flat_multimap_full is already full.
    ///\param position The position to insert at.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(iterator position, rvalue_reference value)
    {
      return insert(etl::move(value)).first;
    }
#endif

    //*********************************************************************
    /// Inserts a range of values to the flat_multimap.
    /// If asserts or exceptions are enabled, emits flat_multimap_full if the flat_multimap does not have enough free space.
    ///\param position The position to insert at.
    ///\param first    The first element to add.
    ///\param last     The last + 1 element to add.
    //*********************************************************************
    template <class TIterator>
    void insert(TIterator first, TIterator last)
    {
      while (first != last)
      {
        insert(*first++);
      }
    }

    //*************************************************************************
    /// Emplaces a value to the map.
    //*************************************************************************
    ETL_OR_STD::pair<iterator, bool> emplace(const value_type& value)
    {
      return insert(value);
    }

    //*************************************************************************
    /// Emplaces a value to the map.
    //*************************************************************************
    ETL_OR_STD::pair<iterator, bool> emplace(const key_type& key, const mapped_type& value)
    {
      ETL_ASSERT(!full(), ETL_ERROR(flat_multimap_full));

      // Create it.
      value_type* pvalue = storage.allocate<value_type>();
      ::new ((void*)etl::addressof(pvalue->first)) key_type(key);
      ::new ((void*)etl::addressof(pvalue->second)) mapped_type(value);
      iterator i_element = lower_bound(key);
      ETL_INCREMENT_DEBUG_COUNT

      return refmap_t::insert_at(i_element, *pvalue);
    }

#if ETL_CPP11_SUPPORTED && ETL_NOT_USING_STLPORT
    //*************************************************************************
    /// Emplaces a value to the map.
    //*************************************************************************
    template <typename ... Args>
    ETL_OR_STD::pair<iterator, bool> emplace(const key_type& key, Args && ... args)
    {
      ETL_ASSERT(!full(), ETL_ERROR(flat_multimap_full));

      // Create it.
      value_type* pvalue = storage.allocate<value_type>();
      ::new ((void*)etl::addressof(pvalue->first)) key_type(key);
      ::new ((void*)etl::addressof(pvalue->second)) mapped_type(etl::forward<Args>(args)...);
      iterator i_element = lower_bound(key);
      ETL_INCREMENT_DEBUG_COUNT

      return refmap_t::insert_at(i_element, *pvalue);
    }

#else
    //*************************************************************************
    /// Emplaces a value to the map.
    //*************************************************************************
    template <typename T1>
    ETL_OR_STD::pair<iterator, bool> emplace(const key_type& key, const T1& value1)
    {
      ETL_ASSERT(!full(), ETL_ERROR(flat_multimap_full));

      // Create it.
      value_type* pvalue = storage.allocate<value_type>();
      ::new ((void*)etl::addressof(pvalue->first)) key_type(key);
      ::new ((void*)etl::addressof(pvalue->second)) mapped_type(value1);
      iterator i_element = lower_bound(key);
      ETL_INCREMENT_DEBUG_COUNT

      return refmap_t::insert_at(i_element, *pvalue);
    }

    //*************************************************************************
    /// Emplaces a value to the map.
    //*************************************************************************
    template <typename T1, typename T2>
    ETL_OR_STD::pair<iterator, bool> emplace(const key_type& key, const T1& value1, const T2& value2)
    {
      ETL_ASSERT(!full(), ETL_ERROR(flat_multimap_full));

      // Create it.
      value_type* pvalue = storage.allocate<value_type>();
      ::new ((void*)etl::addressof(pvalue->first)) key_type(key);
      ::new ((void*)etl::addressof(pvalue->second)) mapped_type(value1, value2);
      iterator i_element = lower_bound(key);
      ETL_INCREMENT_DEBUG_COUNT

      return refmap_t::insert_at(i_element, *pvalue);
    }

    //*************************************************************************
    /// Emplaces a value to the map.
    //*************************************************************************
    template <typename T1, typename T2, typename T3>
    ETL_OR_STD::pair<iterator, bool> emplace(const key_type& key, const T1& value1, const T2& value2, const T3& value3)
    {
      ETL_ASSERT(!full(), ETL_ERROR(flat_multimap_full));

      // Create it.
      value_type* pvalue = storage.allocate<value_type>();
      ::new ((void*)etl::addressof(pvalue->first)) key_type(key);
      ::new ((void*)etl::addressof(pvalue->second)) mapped_type(value1, value2, value3);
      iterator i_element = lower_bound(key);
      ETL_INCREMENT_DEBUG_COUNT

      return refmap_t::insert_at(i_element, *pvalue);
    }

    //*************************************************************************
    /// Emplaces a value to the map.
    //*************************************************************************
    template <typename T1, typename T2, typename T3, typename T4>
    ETL_OR_STD::pair<iterator, bool> emplace(const key_type& key, const T1& value1, const T2& value2, const T3& value3, const T4& value4)
    {
      ETL_ASSERT(!full(), ETL_ERROR(flat_multimap_full));

      // Create it.
      value_type* pvalue = storage.allocate<value_type>();
      ::new ((void*)etl::addressof(pvalue->first)) key_type(key);
      ::new ((void*)etl::addressof(pvalue->second)) mapped_type(value1, value2, value3, value4);
      iterator i_element = lower_bound(key);
      ETL_INCREMENT_DEBUG_COUNT

      return refmap_t::insert_at(i_element, *pvalue);
    }

#endif

    //*********************************************************************
    /// Erases an element.
    ///\param key The key to erase.
    ///\return The number of elements erased. 0 or 1.
    //*********************************************************************
    size_t erase(key_parameter_t key)
    {
      ETL_OR_STD::pair<iterator, iterator> range = equal_range(key);

      if (range.first == end())
      {
        return 0;
      }
      else
      {
        size_t d = etl::distance(range.first, range.second);
        erase(range.first, range.second);
        return d;
      }
    }

    //*********************************************************************
    /// Erases an element.
    ///\param i_element Iterator to the element.
    //*********************************************************************
    void erase(iterator i_element)
    {
      i_element->~value_type();
      storage.release(etl::addressof(*i_element));
      refmap_t::erase(i_element);
      ETL_DECREMENT_DEBUG_COUNT
    }

    //*********************************************************************
    /// Erases a range of elements.
    /// The range includes all the elements between first and last, including the
    /// element pointed by first, but not the one pointed by last.
    ///\param first Iterator to the first element.
    ///\param last  Iterator to the last element.
    //*********************************************************************
    void erase(iterator first, iterator last)
    {
      iterator itr = first;

      while (itr != last)
      {
        itr->~value_type();
        storage.release(etl::addressof(*itr));
        ++itr;
        ETL_DECREMENT_DEBUG_COUNT
      }

      refmap_t::erase(first, last);
    }

    //*************************************************************************
    /// Clears the flat_multimap.
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
    iterator find(key_parameter_t key)
    {
      return refmap_t::find(key);
    }

    //*********************************************************************
    /// Finds an element.
    ///\param key The key to search for.
    ///\return An iterator pointing to the element or end() if not found.
    //*********************************************************************
    const_iterator find(key_parameter_t key) const
    {
      return refmap_t::find(key);
    }

    //*********************************************************************
    /// Counts an element.
    ///\param key The key to search for.
    ///\return 1 if the key exists, otherwise 0.
    //*********************************************************************
    size_t count(key_parameter_t key) const
    {
      return refmap_t::count(key);
    }

    //*********************************************************************
    /// Finds the lower bound of a key
    ///\param key The key to search for.
    ///\return An iterator.
    //*********************************************************************
    iterator lower_bound(key_parameter_t key)
    {
      return refmap_t::lower_bound(key);
    }

    //*********************************************************************
    /// Finds the lower bound of a key
    ///\param key The key to search for.
    ///\return An iterator.
    //*********************************************************************
    const_iterator lower_bound(key_parameter_t key) const
    {
      return refmap_t::lower_bound(key);
    }

    //*********************************************************************
    /// Finds the upper bound of a key
    ///\param key The key to search for.
    ///\return An iterator.
    //*********************************************************************
    iterator upper_bound(key_parameter_t key)
    {
      return refmap_t::upper_bound(key);
    }

    //*********************************************************************
    /// Finds the upper bound of a key
    ///\param key The key to search for.
    ///\return An iterator.
    //*********************************************************************
    const_iterator upper_bound(key_parameter_t key) const
    {
      return refmap_t::upper_bound(key);
    }

    //*********************************************************************
    /// Finds the range of equal elements of a key
    ///\param key The key to search for.
    ///\return An iterator pair.
    //*********************************************************************
    ETL_OR_STD::pair<iterator, iterator> equal_range(key_parameter_t key)
    {
      return refmap_t::equal_range(key);
    }

    //*********************************************************************
    /// Finds the range of equal elements of a key
    ///\param key The key to search for.
    ///\return An iterator pair.
    //*********************************************************************
    ETL_OR_STD::pair<const_iterator, const_iterator> equal_range(key_parameter_t key) const
    {
      return refmap_t::equal_range(key);
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    iflat_multimap& operator = (const iflat_multimap& rhs)
    {
      if (&rhs != this)
      {
        assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

#if ETL_CPP11_SUPPORTED
    //*************************************************************************
    /// Move assignment operator.
    //*************************************************************************
    iflat_multimap& operator = (iflat_multimap&& rhs)
    {
      move_container(etl::move(rhs));

      return *this;
    }
#endif

    //*************************************************************************
    /// Gets the current size of the flat_multiset.
    ///\return The current size of the flat_multiset.
    //*************************************************************************
    size_type size() const
    {
      return refmap_t::size();
    }

    //*************************************************************************
    /// Checks the 'empty' state of the flat_multiset.
    ///\return <b>true</b> if empty.
    //*************************************************************************
    bool empty() const
    {
      return refmap_t::empty();
    }

    //*************************************************************************
    /// Checks the 'full' state of the flat_multiset.
    ///\return <b>true</b> if full.
    //*************************************************************************
    bool full() const
    {
      return refmap_t::full();
    }

    //*************************************************************************
    /// Returns the capacity of the flat_multiset.
    ///\return The capacity of the flat_multiset.
    //*************************************************************************
    size_type capacity() const
    {
      return refmap_t::capacity();
    }

    //*************************************************************************
    /// Returns the maximum possible size of the flat_multiset.
    ///\return The maximum size of the flat_multiset.
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
    iflat_multimap(lookup_t& lookup_, storage_t& storage_)
      : refmap_t(lookup_),
        storage(storage_)
    {
    }

#if ETL_CPP11_SUPPORTED
    //*************************************************************************
    /// Move a flat_multimap.
    /// Assumes the rhs is initialised and empty.
    //*************************************************************************
    void move_container(iflat_multimap&& rhs)
    {
      if (&rhs != this)
      {
        this->clear();

        etl::iflat_multimap<TKey, TMapped, TKeyCompare>::iterator first = rhs.begin();
        etl::iflat_multimap<TKey, TMapped, TKeyCompare>::iterator last = rhs.end();

        // Add all of the elements.
        while (first != last)
        {
          this->insert(etl::move(*first++));
        }
      }
    }
#endif

  private:

    // Disable copy construction.
    iflat_multimap(const iflat_multimap&);

    storage_t& storage;

    /// Internal debugging.
    ETL_DECLARE_DEBUG_COUNT

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
#if defined(ETL_POLYMORPHIC_FLAT_MULTIMAP) || defined(ETL_POLYMORPHIC_CONTAINERS)
  public:
    virtual ~iflat_multimap()
    {
    }
#else
  protected:
    ~iflat_multimap()
    {
    }
#endif
  };

  //***************************************************************************
  /// Equal operator.
  ///\param lhs Reference to the first flat_multimap.
  ///\param rhs Reference to the second flat_multimap.
  ///\return <b>true</b> if the arrays are equal, otherwise <b>false</b>
  ///\ingroup flat_multimap
  //***************************************************************************
  template <typename TKey, typename TMapped, typename TKeyCompare>
  bool operator ==(const etl::iflat_multimap<TKey, TMapped, TKeyCompare>& lhs, const etl::iflat_multimap<TKey, TMapped, TKeyCompare>& rhs)
  {
    return (lhs.size() == rhs.size()) && etl::equal(lhs.begin(), lhs.end(), rhs.begin());
  }

  //***************************************************************************
  /// Not equal operator.
  ///\param lhs Reference to the first flat_multimap.
  ///\param rhs Reference to the second flat_multimap.
  ///\return <b>true</b> if the arrays are not equal, otherwise <b>false</b>
  ///\ingroup flat_multimap
  //***************************************************************************
  template <typename TKey, typename TMapped, typename TKeyCompare>
  bool operator !=(const etl::iflat_multimap<TKey, TMapped, TKeyCompare>& lhs, const etl::iflat_multimap<TKey, TMapped, TKeyCompare>& rhs)
  {
    return !(lhs == rhs);
  }

  //***************************************************************************
  /// A flat_multimap implementation that uses a fixed size buffer.
  ///\tparam TKey     The key type.
  ///\tparam TValue   The value type.
  ///\tparam TCompare The type to compare keys. Default = etl::less<TKey>
  ///\tparam MAX_SIZE_ The maximum number of elements that can be stored.
  ///\ingroup flat_multimap
  //***************************************************************************
  template <typename TKey, typename TValue, const size_t MAX_SIZE_, typename TCompare = etl::less<TKey> >
  class flat_multimap : public etl::iflat_multimap<TKey, TValue, TCompare>
  {
  public:

    static const size_t MAX_SIZE = MAX_SIZE_;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    flat_multimap()
      : etl::iflat_multimap<TKey, TValue, TCompare>(lookup, storage)
    {
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    flat_multimap(const flat_multimap& other)
      : etl::iflat_multimap<TKey, TValue, TCompare>(lookup, storage)
    {
      this->assign(other.cbegin(), other.cend());
    }

#if ETL_CPP11_SUPPORTED
    //*************************************************************************
    /// Move constructor.
    //*************************************************************************
    flat_multimap(flat_multimap&& other)
      : etl::iflat_multimap<TKey, TValue, TCompare>(lookup, storage)
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
    flat_multimap(TIterator first, TIterator last)
      : etl::iflat_multimap<TKey, TValue, TCompare>(lookup, storage)
    {
      this->assign(first, last);
    }

#if ETL_CPP11_SUPPORTED && ETL_NOT_USING_STLPORT && ETL_USING_STL
    //*************************************************************************
    /// Construct from initializer_list.
    //*************************************************************************
    flat_multimap(std::initializer_list<typename etl::iflat_multimap<TKey, TValue, TCompare>::value_type> init)
      : etl::iflat_multimap<TKey, TValue, TCompare>(lookup, storage)
    {
      this->assign(init.begin(), init.end());
    }
#endif

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~flat_multimap()
    {
      this->clear();
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    flat_multimap& operator = (const flat_multimap& rhs)
    {
      if (&rhs != this)
      {
        this->assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

#if ETL_CPP11_SUPPORTED
    //*************************************************************************
    /// Move assignment operator.
    //*************************************************************************
    flat_multimap& operator = (flat_multimap&& rhs)
    {
      if (&rhs != this)
      {
        this->move_container(etl::move(rhs));
      }

      return *this;
    }
#endif

  private:

    typedef typename etl::iflat_multimap<TKey, TValue, TCompare>::value_type node_t;

    // The pool of nodes.
    etl::pool<node_t, MAX_SIZE> storage;

    // The vector that stores pointers to the nodes.
    etl::vector<node_t*, MAX_SIZE> lookup;
  };

  //*************************************************************************
  /// Template deduction guides.
  //*************************************************************************
#if ETL_CPP17_SUPPORTED && ETL_NOT_USING_STLPORT && ETL_USING_STL
  template <typename T, typename... Ts>
  flat_multimap(T, Ts...)
    ->flat_multimap<etl::enable_if_t<(etl::is_same_v<T, Ts> && ...), typename T::first_type>,
    etl::enable_if_t<(etl::is_same_v<T, Ts> && ...), typename T::second_type>,
    1U + sizeof...(Ts)>;
#endif 
}

#undef ETL_FILE

#endif
