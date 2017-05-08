///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

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

#ifndef __ETL_FLAT_SET__
#define __ETL_FLAT_SET__

#include <stddef.h>
#include <iterator>
#include <functional>
#include <algorithm>
#include <utility>

#include "vector.h"
#include "pool.h"
#include "exception.h"
#include "debug_count.h"
#include "platform.h"
#include "type_traits.h"
#include "parameter_type.h"
#include "vector.h"
#include "pool.h"
#include "error_handler.h"

#undef ETL_FILE
#define ETL_FILE "5"

//*****************************************************************************
///\defgroup flat_set flat_set
/// A flat_set with the capacity defined at compile time.
/// Has insertion of O(N) and flat_set of O(logN)
/// Duplicate entries and not allowed.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //***************************************************************************
  ///\ingroup flat_set
  /// Exception base for flat_sets
  //***************************************************************************
  class flat_set_exception : public etl::exception
  {
  public:

    flat_set_exception(string_type what, string_type file_name, numeric_type line_number)
      : etl::exception(what, file_name, line_number)
    {
    }
  };

  //***************************************************************************
  ///\ingroup flat_set
  /// Vector full exception.
  //***************************************************************************
  class flat_set_full : public etl::flat_set_exception
  {
  public:

    flat_set_full(string_type file_name, numeric_type line_number)
      : etl::flat_set_exception(ETL_ERROR_TEXT("flat_set:full", ETL_FILE"A"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  ///\ingroup flat_set
  /// Vector iterator exception.
  //***************************************************************************
  class flat_set_iterator : public etl::flat_set_exception
  {
  public:

    flat_set_iterator(string_type file_name, numeric_type line_number)
      : etl::flat_set_exception(ETL_ERROR_TEXT("flat_set:iterator", ETL_FILE"C"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  ///\ingroup flat_set
  /// The base class for all templated flat_set types.
  //***************************************************************************
  class flat_set_base
  {
  public:

    typedef size_t size_type;

  protected:

    etl::debug_count construct_count;
  };

  //***************************************************************************
  /// The base class for specifically sized flat_sets.
  /// Can be used as a reference type for all flat_sets containing a specific type.
  ///\ingroup flat_set
  //***************************************************************************
  template <typename T, typename TKeyCompare = std::less<T> >
  class iflat_set : public etl::flat_set_base
  {
  public:

    typedef T                 key_type;
    typedef T                 value_type;
    typedef TKeyCompare       key_compare;
    typedef value_type&       reference;
    typedef const value_type& const_reference;
    typedef value_type*       pointer;
    typedef const value_type* const_pointer;
    typedef size_t            size_type;

  private:

    typedef etl::ivector<value_type*> lookup_t;
    typedef etl::ipool                storage_t;

  public:

    //*************************************************************************
    class iterator : public std::iterator<std::bidirectional_iterator_tag, value_type>
    {
    public:

      friend class iflat_set;

      iterator()
      {
      }

      iterator(typename lookup_t::iterator ilookup)
        : ilookup(ilookup)
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

      friend class iflat_set;

      const_iterator()
      {
      }

      const_iterator(typename lookup_t::const_iterator ilookup)
        : ilookup(ilookup)
      {
      }

      const_iterator(const iterator& other)
        : ilookup(other.ilookup)
      {
      }

      const_iterator(const const_iterator& other)
        : ilookup(other.ilookup)
      {
      }

      const_iterator& operator =(const iterator& other)
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

  protected:

    typedef typename etl::parameter_type<T>::type parameter_t;

  public:

    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef typename std::iterator_traits<iterator>::difference_type difference_type;

    //*********************************************************************
    /// Returns an iterator to the beginning of the flat_set.
    ///\return An iterator to the beginning of the flat_set.
    //*********************************************************************
    iterator begin()
    {
      return iterator(lookup.begin());
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the flat_set.
    ///\return A const iterator to the beginning of the flat_set.
    //*********************************************************************
    const_iterator begin() const
    {
      return const_iterator(lookup.begin());
    }

    //*********************************************************************
    /// Returns an iterator to the end of the flat_set.
    ///\return An iterator to the end of the flat_set.
    //*********************************************************************
    iterator end()
    {
      return iterator(lookup.end());
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the flat_set.
    ///\return A const iterator to the end of the flat_set.
    //*********************************************************************
    const_iterator end() const
    {
      return const_iterator(lookup.end());
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the flat_set.
    ///\return A const iterator to the beginning of the flat_set.
    //*********************************************************************
    const_iterator cbegin() const
    {
      return const_iterator(lookup.cbegin());
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the flat_set.
    ///\return A const iterator to the end of the flat_set.
    //*********************************************************************
    const_iterator cend() const
    {
      return const_iterator(lookup.cend());
    }

    //*********************************************************************
    /// Returns an reverse iterator to the reverse beginning of the flat_set.
    ///\return Iterator to the reverse beginning of the flat_set.
    //*********************************************************************
    reverse_iterator rbegin()
    {
      return reverse_iterator(lookup.rbegin());
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the flat_set.
    ///\return Const iterator to the reverse beginning of the flat_set.
    //*********************************************************************
    const_reverse_iterator rbegin() const
    {
      return const_reverse_iterator(lookup.rbegin());
    }

    //*********************************************************************
    /// Returns a reverse iterator to the end + 1 of the flat_set.
    ///\return Reverse iterator to the end + 1 of the flat_set.
    //*********************************************************************
    reverse_iterator rend()
    {
      return reverse_iterator(lookup.rend());
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the end + 1 of the flat_set.
    ///\return Const reverse iterator to the end + 1 of the flat_set.
    //*********************************************************************
    const_reverse_iterator rend() const
    {
      return const_reverse_iterator(lookup.rend());
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the flat_set.
    ///\return Const reverse iterator to the reverse beginning of the flat_set.
    //*********************************************************************
    const_reverse_iterator crbegin() const
    {
      return const_reverse_iterator(lookup.crbegin());
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the end + 1 of the flat_set.
    ///\return Const reverse iterator to the end + 1 of the flat_set.
    //*********************************************************************
    const_reverse_iterator crend() const
    {
      return const_reverse_iterator(lookup.crend());
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
#if defined(ETL_DEBUG)
      difference_type count = std::distance(first, last);
      ETL_ASSERT(count <= difference_type(capacity()), ETL_ERROR(flat_set_full));
#endif

      clear();

      while (first != last)
      {
        insert(*first++);
      }
    }

    //*********************************************************************
    /// Inserts a value to the flat_set.
    /// If asserts or exceptions are enabled, emits flat_set_full if the flat_set is already full.
    ///\param value    The value to insert.
    //*********************************************************************
    std::pair<iterator, bool> insert(parameter_t value)
    {
      std::pair<iterator, bool> result(end(), false);

      ETL_ASSERT(!lookup.full(), ETL_ERROR(flat_set_full));

      iterator i_element = std::lower_bound(begin(), end(), value, TKeyCompare());

      if (i_element == end())
      {
        // At the end. Doesn't exist.
        value_type* pvalue = storage.allocate<value_type>();
        ::new (pvalue) value_type(value);
        lookup.push_back(pvalue);
        result.first = --end();
        result.second = true;
        ++construct_count;
      }
      else
      {
        // Not at the end.
        // Does not exist already?
        if (*i_element != value)
        {
          value_type* pvalue = storage.allocate<value_type>();
          ::new (pvalue) value_type(value);
          lookup.insert(i_element.ilookup, pvalue);
          result.first = i_element;
          result.second = true;
          ++construct_count;
        }
        else
        {
          result.first = i_element;
          result.second = false;
        }
      }

      return result;
    }

    //*********************************************************************
    /// Inserts a value to the flat_set.
    /// If asserts or exceptions are enabled, emits flat_set_full if the flat_set is already full.
    ///\param position The position to insert at.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(iterator position, parameter_t value)
    {
      return insert(value).first;
    }

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
        insert(*first++);
      }
    }

    //*********************************************************************
    /// Erases an element.
    ///\param key The key to erase.
    ///\return The number of elements erased. 0 or 1.
    //*********************************************************************
    size_t erase(parameter_t key)
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
        lookup.erase(i_element.ilookup);
        --construct_count;
        return 1;
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
      lookup.erase(i_element.ilookup);
      --construct_count;
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
        --construct_count;
      }

      lookup.erase(first.ilookup, last.ilookup);;
    }

    //*************************************************************************
    /// Clears the flat_set.
    //*************************************************************************
    void clear()
    {
      erase(begin(), end());
    }

    //*********************************************************************
    /// Finds an element.
    ///\param key The key to search for.
    ///\return An iterator pointing to the element or end() if not found.
    //*********************************************************************
    iterator find(parameter_t key)
    {
      iterator itr = std::lower_bound(begin(), end(), key, TKeyCompare());

      if (itr != end())
      {
        if (!key_compare()(*itr, key) && !key_compare()(key, *itr))
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
    const_iterator find(parameter_t key) const
    {
      const_iterator itr = std::lower_bound(begin(), end(), key, TKeyCompare());

      if (itr != end())
      {
        if (!key_compare()(*itr, key) && !key_compare()(key, *itr))
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
    size_t count(parameter_t key) const
    {
      return (find(key) == end()) ? 0 : 1;
    }

    //*********************************************************************
    /// Finds the lower bound of a key
    ///\param key The key to search for.
    ///\return An iterator.
    //*********************************************************************
    iterator lower_bound(parameter_t key)
    {
      return std::lower_bound(begin(), end(), key, TKeyCompare());
    }

    //*********************************************************************
    /// Finds the lower bound of a key
    ///\param key The key to search for.
    ///\return An iterator.
    //*********************************************************************
    const_iterator lower_bound(parameter_t key) const
    {
      return std::lower_bound(cbegin(), cend(), key, TKeyCompare());
    }

    //*********************************************************************
    /// Finds the upper bound of a key
    ///\param key The key to search for.
    ///\return An iterator.
    //*********************************************************************
    iterator upper_bound(parameter_t key)
    {
      return std::upper_bound(begin(), end(), key, TKeyCompare());
    }

    //*********************************************************************
    /// Finds the upper bound of a key
    ///\param key The key to search for.
    ///\return An iterator.
    //*********************************************************************
    const_iterator upper_bound(parameter_t key) const
    {
      return std::upper_bound(cbegin(), cend(), key, TKeyCompare());
    }

    //*********************************************************************
    /// Finds the range of equal elements of a key
    ///\param key The key to search for.
    ///\return An iterator pair.
    //*********************************************************************
    std::pair<iterator, iterator> equal_range(parameter_t key)
    {
      return std::equal_range(begin(), end(), key, TKeyCompare());
    }

    //*********************************************************************
    /// Finds the range of equal elements of a key
    ///\param key The key to search for.
    ///\return An iterator pair.
    //*********************************************************************
    std::pair<const_iterator, const_iterator> equal_range(parameter_t key) const
    {
      return std::upper_bound(cbegin(), cend(), key, TKeyCompare());
    }

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

    //*************************************************************************
    /// Gets the current size of the flat_set.
    ///\return The current size of the flat_set.
    //*************************************************************************
    size_type size() const
    {
      return lookup.size();
    }

    //*************************************************************************
    /// Checks the 'empty' state of the flat_set.
    ///\return <b>true</b> if empty.
    //*************************************************************************
    bool empty() const
    {
      return lookup.empty();
    }

    //*************************************************************************
    /// Checks the 'full' state of the flat_set.
    ///\return <b>true</b> if full.
    //*************************************************************************
    bool full() const
    {
      return lookup.full();
    }

    //*************************************************************************
    /// Returns the capacity of the flat_set.
    ///\return The capacity of the flat_set.
    //*************************************************************************
    size_type capacity() const
    {
      return lookup.capacity();
    }

    //*************************************************************************
    /// Returns the maximum possible size of the flat_set.
    ///\return The maximum size of the flat_set.
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
    iflat_set(lookup_t& lookup_, storage_t& storage)
      : lookup(lookup_),
      storage(storage)
    {
    }

  private:

    // Disable copy construction.
    iflat_set(const iflat_set&);

    lookup_t&  lookup;
    storage_t& storage;
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
    return (lhs.size() == rhs.size()) && std::equal(lhs.begin(), lhs.end(), rhs.begin());
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

  template <typename T, const size_t MAX_SIZE_, typename TCompare = std::less<T> >
  //***************************************************************************
  /// A flat_set implementation that uses a fixed size buffer.
  ///\tparam T        The value type.
  ///\tparam TCompare The type to compare keys. Default = std::less<T>
  ///\tparam MAX_SIZE_ The maximum number of elements that can be stored.
  ///\ingroup flat_set
  //***************************************************************************
  class flat_set : public etl::iflat_set<T, TCompare>
  {
  public:

    static const size_t MAX_SIZE = MAX_SIZE_;

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
      etl::iflat_set<T, TCompare>::assign(other.cbegin(), other.cend());
    }

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
      etl::iflat_set<T, TCompare>::assign(first, last);
    }

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~flat_set()
    {
      etl::iflat_set<T, TCompare>::clear();
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    flat_set& operator = (const flat_set& rhs)
    {
      if (&rhs != this)
      {
        etl::iflat_set<T, TCompare>::assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

  private:

    typedef typename etl::iflat_set<T, TCompare>::value_type node_t;

    // The pool of nodes.
    etl::pool<node_t, MAX_SIZE> storage;

    // The vector that stores pointers to the nodes.
    etl::vector<node_t*, MAX_SIZE> lookup;
  };
}

#undef ETL_FILE

#endif
