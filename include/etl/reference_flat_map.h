///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2017 jwellbelove

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

#ifndef ETL_REFERENCE_FLAT_MAP_INCLUDED
#define ETL_REFERENCE_FLAT_MAP_INCLUDED

#include <stddef.h>

#include "platform.h"
#include "vector.h"
#include "error_handler.h"
#include "debug_count.h"
#include "type_traits.h"
#include "parameter_type.h"
#include "exception.h"
#include "static_assert.h"

#undef ETL_FILE
#define ETL_FILE "30"

//*****************************************************************************
///\defgroup reference_flat_map reference_flat_map
/// An reference_flat_map with the capacity defined at compile time.
/// Has insertion of O(N) and search of O(logN)
/// Duplicate entries are not allowed.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //***************************************************************************
  ///\ingroup reference_flat_map
  /// Exception base for reference_flat_maps
  //***************************************************************************
  class flat_map_exception : public etl::exception
  {
  public:

    flat_map_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  ///\ingroup reference_flat_map
  /// Vector full exception.
  //***************************************************************************
  class flat_map_full : public etl::flat_map_exception
  {
  public:

    flat_map_full(string_type file_name_, numeric_type line_number_)
      : flat_map_exception(ETL_ERROR_TEXT("flat_map: full", ETL_FILE"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  ///\ingroup reference_flat_map
  /// Vector out of bounds exception.
  //***************************************************************************
  class flat_map_out_of_bounds : public etl::flat_map_exception
  {
  public:

    flat_map_out_of_bounds(string_type file_name_, numeric_type line_number_)
      : flat_map_exception(ETL_ERROR_TEXT("flat_map:bounds", ETL_FILE"B"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// The base class for specifically sized reference_flat_maps.
  /// Can be used as a reference type for all reference_flat_maps containing a specific type.
  ///\ingroup reference_flat_map
  //***************************************************************************
  template <typename TKey, typename TMapped, typename TKeyCompare = std::less<TKey> >
  class ireference_flat_map
  {
  public:

    typedef std::pair<const TKey, TMapped> value_type;

  protected:

    typedef etl::ivector<value_type*> lookup_t;

  public:

    typedef TKey              key_type;
    typedef TMapped           mapped_type;
    typedef TKeyCompare       key_compare;
    typedef value_type&       reference;
    typedef const value_type& const_reference;
    typedef value_type*       pointer;
    typedef const value_type* const_pointer;
    typedef size_t            size_type;

    //*************************************************************************
    class iterator : public std::iterator<std::bidirectional_iterator_tag, value_type>
    {
    public:

      friend class ireference_flat_map;

      iterator()
      {
      }

      iterator(typename lookup_t::iterator ilookup_)
        : ilookup(ilookup_)
      {
      }

      iterator(const iterator& other)
        : ilookup(other.ilookup)
      {
      }

      iterator& operator =(const iterator& other)
      {
        ilookup = other.ilookup;
        return *this;
      }

      iterator& operator ++()
      {
        ++ilookup;
        return *this;
      }

      iterator operator ++(int)
      {
        iterator temp(*this);
        ++ilookup;
        return temp;
      }

      iterator& operator --()
      {
        --ilookup;
        return *this;
      }

      iterator operator --(int)
      {
        iterator temp(*this);
        --ilookup;
        return temp;
      }

      reference operator *()
      {
        return *(*ilookup);
      }

      const_reference operator *() const
      {
        return *(*ilookup);
      }

      pointer operator &()
      {
        return etl::addressof(*(*ilookup));
      }

      const_pointer operator &() const
      {
        return &(*(*ilookup));
      }

      pointer operator ->()
      {
        return etl::addressof(*(*ilookup));
      }

      const_pointer operator ->() const
      {
        return etl::addressof(*(*ilookup));
      }

      friend bool operator == (const iterator& lhs, const iterator& rhs)
      {
        return lhs.ilookup == rhs.ilookup;
      }

      friend bool operator != (const iterator& lhs, const iterator& rhs)
      {
        return !(lhs == rhs);
      }

    private:

      typename lookup_t::iterator ilookup;
    };

    //*************************************************************************
    class const_iterator : public std::iterator<std::bidirectional_iterator_tag, const value_type>
    {
    public:

      friend class ireference_flat_map;

      const_iterator()
      {
      }

      const_iterator(typename lookup_t::const_iterator ilookup_)
        : ilookup(ilookup_)
      {
      }

      const_iterator(const typename ireference_flat_map::iterator& other)
        : ilookup(other.ilookup)
      {
      }

      const_iterator(const const_iterator& other)
        : ilookup(other.ilookup)
      {
      }

      const_iterator& operator =(const typename ireference_flat_map::iterator& other)
      {
        ilookup = other.ilookup;
        return *this;
      }

      const_iterator& operator =(const const_iterator& other)
      {
        ilookup = other.ilookup;
        return *this;
      }

      const_iterator& operator ++()
      {
        ++ilookup;
        return *this;
      }

      const_iterator operator ++(int)
      {
        const_iterator temp(*this);
        ++ilookup;
        return temp;
      }

      const_iterator& operator --()
      {
        --ilookup;
        return *this;
      }

      const_iterator operator --(int)
      {
        const_iterator temp(*this);
        --ilookup;
        return temp;
      }

      const_reference operator *() const
      {
        return *(*ilookup);
      }

      const_pointer operator &() const
      {
        return etl::addressof(*(*ilookup));
      }

      const_pointer operator ->() const
      {
        return etl::addressof(*(*ilookup));
      }

      friend bool operator == (const const_iterator& lhs, const const_iterator& rhs)
      {
        return lhs.ilookup == rhs.ilookup;
      }

      friend bool operator != (const const_iterator& lhs, const const_iterator& rhs)
      {
        return !(lhs == rhs);
      }

    private:

      typename lookup_t::const_iterator ilookup;
    };

    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef typename std::iterator_traits<iterator>::difference_type difference_type;

  protected:

    typedef typename etl::parameter_type<TKey>::type key_parameter_t;

  private:

    //*********************************************************************
    /// How to compare elements and keys.
    //*********************************************************************
    class Compare
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
    /// Returns an iterator to the beginning of the reference_flat_map.
    ///\return An iterator to the beginning of the reference_flat_map.
    //*********************************************************************
    iterator begin()
    {
      return iterator(lookup.begin());
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the reference_flat_map.
    ///\return A const iterator to the beginning of the reference_flat_map.
    //*********************************************************************
    const_iterator begin() const
    {
      return const_iterator(lookup.begin());
    }

    //*********************************************************************
    /// Returns an iterator to the end of the reference_flat_map.
    ///\return An iterator to the end of the reference_flat_map.
    //*********************************************************************
    iterator end()
    {
      return iterator(lookup.end());
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the reference_flat_map.
    ///\return A const iterator to the end of the reference_flat_map.
    //*********************************************************************
    const_iterator end() const
    {
      return const_iterator(lookup.end());
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the reference_flat_map.
    ///\return A const iterator to the beginning of the reference_flat_map.
    //*********************************************************************
    const_iterator cbegin() const
    {
      return const_iterator(lookup.cbegin());
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the reference_flat_map.
    ///\return A const iterator to the end of the reference_flat_map.
    //*********************************************************************
    const_iterator cend() const
    {
      return const_iterator(lookup.cend());
    }

    //*********************************************************************
    /// Returns an reverse iterator to the reverse beginning of the reference_flat_map.
    ///\return Iterator to the reverse beginning of the reference_flat_map.
    //*********************************************************************
    reverse_iterator rbegin()
    {
      return reverse_iterator(lookup.rbegin());
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the reference_flat_map.
    ///\return Const iterator to the reverse beginning of the reference_flat_map.
    //*********************************************************************
    const_reverse_iterator rbegin() const
    {
      return reverse_iterator(lookup.rbegin());
    }

    //*********************************************************************
    /// Returns a reverse iterator to the end + 1 of the reference_flat_map.
    ///\return Reverse iterator to the end + 1 of the reference_flat_map.
    //*********************************************************************
    reverse_iterator rend()
    {
      return reverse_iterator(lookup.rend());
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the end + 1 of the reference_flat_map.
    ///\return Const reverse iterator to the end + 1 of the reference_flat_map.
    //*********************************************************************
    const_reverse_iterator rend() const
    {
      return const_reverse_iterator(lookup.rend());
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the reference_flat_map.
    ///\return Const reverse iterator to the reverse beginning of the reference_flat_map.
    //*********************************************************************
    const_reverse_iterator crbegin() const
    {
      return const_reverse_iterator(lookup.crbegin());
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the end + 1 of the reference_flat_map.
    ///\return Const reverse iterator to the end + 1 of the reference_flat_map.
    //*********************************************************************
    const_reverse_iterator crend() const
    {
      return const_reverse_iterator(lookup.crend());
    }

    //*********************************************************************
    /// Returns a reference to the value at index 'key'
    ///\param i The index.
    ///\return A reference to the value at index 'key'
    //*********************************************************************
    mapped_type& operator [](key_parameter_t key)
    {
      iterator i_element = lower_bound(key);

      ETL_ASSERT(i_element != end(), ETL_ERROR(flat_map_out_of_bounds));

      return i_element->second;
    }

    //*********************************************************************
    /// Returns a const reference to the value at index 'key'
    ///\param i The index.
    ///\return A const reference to the value at index 'key'
    //*********************************************************************
    const mapped_type& operator [](key_parameter_t key) const
    {
      iterator i_element = lower_bound(key);

      ETL_ASSERT(i_element != end(), ETL_ERROR(flat_map_out_of_bounds));

      return i_element->second;
    }

    //*********************************************************************
    /// Returns a reference to the value at index 'key'
    /// If asserts or exceptions are enabled, emits an etl::flat_map_out_of_bounds if the key is not in the range.
    ///\param i The index.
    ///\return A reference to the value at index 'key'
    //*********************************************************************
    mapped_type& at(key_parameter_t key)
    {
      iterator i_element = lower_bound(key);

      ETL_ASSERT(i_element != end(), ETL_ERROR(flat_map_out_of_bounds));

      return i_element->second;
    }

    //*********************************************************************
    /// Returns a const reference to the value at index 'key'
    /// If asserts or exceptions are enabled, emits an etl::flat_map_out_of_bounds if the key is not in the range.
    ///\param i The index.
    ///\return A const reference to the value at index 'key'
    //*********************************************************************
    const mapped_type& at(key_parameter_t key) const
    {
      const_iterator i_element = lower_bound(key);

      ETL_ASSERT(i_element != end(), ETL_ERROR(flat_map_out_of_bounds));

      return i_element->second;
    }

    //*********************************************************************
    /// Assigns values to the reference_flat_map.
    /// If ETL_THROW_EXCEPTIONS & ETL_DEBUG are defined, emits flat_map_full if the reference_flat_map does not have enough free space.
    /// If ETL_THROW_EXCEPTIONS & ETL_DEBUG are defined, emits flat_map_iterator if the iterators are reversed.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*********************************************************************
    template <typename TIterator>
    void assign(TIterator first, TIterator last)
    {
      ETL_STATIC_ASSERT((etl::is_same<value_type, typename std::iterator_traits<TIterator>::value_type>::value), "Incompatible data for assign");

#if defined(ETL_DEBUG)
      difference_type d = std::distance(first, last);
      ETL_ASSERT(d <= difference_type(capacity()), ETL_ERROR(flat_map_full));
#endif

      clear();

      while (first != last)
      {
        insert(*first++);
      }
    }

    //*********************************************************************
    /// Inserts a value to the reference_flat_map.
    /// If asserts or exceptions are enabled, emits flat_map_full if the reference_flat_map is already full.
    ///\param value    The value to insert.
    //*********************************************************************
    std::pair<iterator, bool> insert(reference value)
    {
      iterator i_element = lower_bound(value.first);

      return insert_at(i_element, value);
    }

    //*********************************************************************
    /// Inserts a value to the reference_flat_map.
    /// If asserts or exceptions are enabled, emits flat_map_full if the reference_flat_map is already full.
    ///\param position The position to insert at.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(iterator position, reference value)
    {
      return insert(value).first;
    }

    //*********************************************************************
    /// Inserts a range of values to the reference_flat_map.
    /// If asserts or exceptions are enabled, emits flat_map_full if the reference_flat_map does not have enough free space.
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
        lookup.erase(i_element.ilookup);
        return 1;
      }
    }

    //*********************************************************************
    /// Erases an element.
    ///\param i_element Iterator to the element.
    //*********************************************************************
    void erase(iterator i_element)
    {
      lookup.erase(i_element.ilookup);
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
      lookup.erase(first.ilookup, last.ilookup);
    }

    //*************************************************************************
    /// Clears the reference_flat_map.
    //*************************************************************************
    void clear()
    {
      lookup.clear();
    }

    //*********************************************************************
    /// Finds an element.
    ///\param key The key to search for.
    ///\return An iterator pointing to the element or end() if not found.
    //*********************************************************************
    iterator find(key_parameter_t key)
    {
      iterator itr = lower_bound(key);

      if (itr != end())
      {
        if (!key_compare()(itr->first, key) && !key_compare()(key, itr->first))
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

    //*********************************************************************
    /// Finds an element.
    ///\param key The key to search for.
    ///\return An iterator pointing to the element or end() if not found.
    //*********************************************************************
    const_iterator find(key_parameter_t key) const
    {
      const_iterator itr = lower_bound(key);

      if (itr != end())
      {
        if (!key_compare()(itr->first, key) && !key_compare()(key, itr->first))
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

    //*********************************************************************
    /// Counts an element.
    ///\param key The key to search for.
    ///\return 1 if the key exists, otherwise 0.
    //*********************************************************************
    size_t count(key_parameter_t key) const
    {
      return (find(key) == end()) ? 0 : 1;
    }

    //*********************************************************************
    /// Finds the lower bound of a key
    ///\param key The key to search for.
    ///\return An iterator.
    //*********************************************************************
    iterator lower_bound(key_parameter_t key)
    {
      return std::lower_bound(begin(), end(), key, compare);
    }

    //*********************************************************************
    /// Finds the lower bound of a key
    ///\param key The key to search for.
    ///\return An iterator.
    //*********************************************************************
    const_iterator lower_bound(key_parameter_t key) const
    {
      return std::lower_bound(cbegin(), cend(), key, compare);
    }

    //*********************************************************************
    /// Finds the upper bound of a key
    ///\param key The key to search for.
    ///\return An iterator.
    //*********************************************************************
    iterator upper_bound(key_parameter_t key)
    {
      return std::upper_bound(begin(), end(), key, compare);
    }

    //*********************************************************************
    /// Finds the upper bound of a key
    ///\param key The key to search for.
    ///\return An iterator.
    //*********************************************************************
    const_iterator upper_bound(key_parameter_t key) const
    {
      return std::upper_bound(begin(), end(), key, compare);
    }

    //*********************************************************************
    /// Finds the range of equal elements of a key
    ///\param key The key to search for.
    ///\return An iterator pair.
    //*********************************************************************
    std::pair<iterator, iterator> equal_range(key_parameter_t key)
    {
      iterator i_lower = std::lower_bound(begin(), end(), key, compare);

      return std::make_pair(i_lower, std::upper_bound(i_lower, end(), key, compare));
    }

    //*********************************************************************
    /// Finds the range of equal elements of a key
    ///\param key The key to search for.
    ///\return An iterator pair.
    //*********************************************************************
    std::pair<const_iterator, const_iterator> equal_range(key_parameter_t key) const
    {
      const_iterator i_lower = std::lower_bound(cbegin(), cend(), key, compare);

      return std::make_pair(i_lower, std::upper_bound(i_lower, cend(), key, compare));
    }

    //*************************************************************************
    /// Gets the current size of the reference_flat_map.
    ///\return The current size of the reference_flat_map.
    //*************************************************************************
    size_type size() const
    {
      return lookup.size();
    }

    //*************************************************************************
    /// Checks the 'empty' state of the reference_flat_map.
    ///\return <b>true</b> if empty.
    //*************************************************************************
    bool empty() const
    {
      return lookup.empty();
    }

    //*************************************************************************
    /// Checks the 'full' state of the reference_flat_map.
    ///\return <b>true</b> if full.
    //*************************************************************************
    bool full() const
    {
      return lookup.full();
    }

    //*************************************************************************
    /// Returns the capacity of the reference_flat_map.
    ///\return The capacity of the reference_flat_map.
    //*************************************************************************
    size_type capacity() const
    {
      return lookup.capacity();
    }

    //*************************************************************************
    /// Returns the maximum possible size of the reference_flat_map.
    ///\return The maximum size of the reference_flat_map.
    //*************************************************************************
    size_type max_size() const
    {
      return lookup.max_size();
    }

    //*************************************************************************
    /// Returns the remaining capacity.
    ///\return The remaining capacity.
    //*************************************************************************
    size_t available() const
    {
      return lookup.available();
    }

  protected:

    //*********************************************************************
    /// Constructor.
    //*********************************************************************
    ireference_flat_map(lookup_t& lookup_)
      : lookup(lookup_)
    {
    }

    //*********************************************************************
    /// Inserts a value to the reference_flat_map.
    ///\param i_element The place to insert.
    ///\param value     The value to insert.
    //*********************************************************************
    std::pair<iterator, bool> insert_at(iterator i_element, value_type& value)
    {
      std::pair<iterator, bool> result(end(), false);

      if (i_element == end())
      {
        // At the end.
        ETL_ASSERT(!lookup.full(), ETL_ERROR(flat_map_full));

        lookup.push_back(&value);
        result.first = --end();
        result.second = true;
      }
      else
      {
        // Not at the end.
        result.first = i_element;

        // Existing element?
        if (TKeyCompare()(value.first, i_element->first) || TKeyCompare()(i_element->first, value.first))
        {
          // A new one.
          ETL_ASSERT(!lookup.full(), ETL_ERROR(flat_map_full));
          lookup.insert(i_element.ilookup, &value);
          result.second = true;
        }
      }

      return result;
    }

  private:

    // Disable copy construction and assignment.
    ireference_flat_map(const ireference_flat_map&);
    ireference_flat_map& operator = (const ireference_flat_map&);

    lookup_t& lookup;

    Compare compare;

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
#if defined(ETL_POLYMORPHIC_REFERENCE_FLAT_MAP) || defined(ETL_POLYMORPHIC_CONTAINERS)
  public:
    virtual ~ireference_flat_map()
    {
    }
#else
  protected:
    ~ireference_flat_map()
    {
    }
#endif
  };

  //***************************************************************************
  /// Equal operator.
  ///\param lhs Reference to the first reference_flat_map.
  ///\param rhs Reference to the second reference_flat_map.
  ///\return <b>true</b> if the arrays are equal, otherwise <b>false</b>
  ///\ingroup reference_flat_map
  //***************************************************************************
  template <typename TKey, typename TMapped, typename TKeyCompare>
  bool operator ==(const etl::ireference_flat_map<TKey, TMapped, TKeyCompare>& lhs, const etl::ireference_flat_map<TKey, TMapped, TKeyCompare>& rhs)
  {
    return (lhs.size() == rhs.size()) && std::equal(lhs.begin(), lhs.end(), rhs.begin());
  }

  //***************************************************************************
  /// Not equal operator.
  ///\param lhs Reference to the first reference_flat_map.
  ///\param rhs Reference to the second reference_flat_map.
  ///\return <b>true</b> if the arrays are not equal, otherwise <b>false</b>
  ///\ingroup reference_flat_map
  //***************************************************************************
  template <typename TKey, typename TMapped, typename TKeyCompare>
  bool operator !=(const etl::ireference_flat_map<TKey, TMapped, TKeyCompare>& lhs, const etl::ireference_flat_map<TKey, TMapped, TKeyCompare>& rhs)
  {
    return !(lhs == rhs);
  }

  //***************************************************************************
  /// A reference_flat_map implementation that uses a fixed size buffer.
  ///\tparam TKey     The key type.
  ///\tparam TValue   The value type.
  ///\tparam TCompare The type to compare keys. Default = std::less<TKey>
  ///\tparam MAX_SIZE_ The maximum number of elements that can be stored.
  ///\ingroup reference_flat_map
  //***************************************************************************
  template <typename TKey, typename TValue, const size_t MAX_SIZE_, typename TCompare = std::less<TKey> >
  class reference_flat_map : public ireference_flat_map<TKey, TValue, TCompare>
  {
  public:

    static const size_t MAX_SIZE = MAX_SIZE_;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    reference_flat_map()
      : ireference_flat_map<TKey, TValue, TCompare>(lookup)
    {
    }

    //*************************************************************************
    /// Constructor, from an iterator range.
    ///\tparam TIterator The iterator type.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*************************************************************************
    template <typename TIterator>
    reference_flat_map(TIterator first, TIterator last)
      : ireference_flat_map<TKey, TValue, TCompare>(lookup)
    {
      ireference_flat_map<TKey, TValue, TCompare>::assign(first, last);
    }

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~reference_flat_map()
    {
      ireference_flat_map<TKey, TValue, TCompare>::clear();
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    reference_flat_map& operator = (const reference_flat_map& rhs)
    {
      if (&rhs != this)
      {
        ireference_flat_map<TKey, TValue, TCompare>::assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

  private:

    reference_flat_map(const reference_flat_map&);

    typedef typename ireference_flat_map<TKey, TValue, TCompare>::value_type node_t;

    // The vector that stores pointers to the nodes.
    etl::vector<node_t*, MAX_SIZE> lookup;
  };

}

#undef ETL_FILE

#endif
