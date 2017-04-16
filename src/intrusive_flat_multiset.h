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

#ifndef __ETL_INTRUSIVE_FLAT_MULTISET__
#define __ETL_INTRUSIVE_FLAT_MULTISET__

#include <iterator>
#include <algorithm>
#include <functional>
#include <utility>
#include <stddef.h>

#include "platform.h"
#include "type_traits.h"
#include "ivector.h"
#include "vector.h"
#include "ipool.h"
#include "error_handler.h"
#include "exception.h"

#undef ETL_FILE
#define ETL_FILE "33"

namespace etl
{
  //***************************************************************************
  ///\ingroup intrusive_flat_multiset
  /// Exception base for intrusive_flat_multisets
  //***************************************************************************
  class intrusive_flat_multiset_exception : public exception
  {
  public:

    intrusive_flat_multiset_exception(string_type what, string_type file_name, numeric_type line_number)
      : exception(what, file_name, line_number)
    {
    }
  };

  //***************************************************************************
  ///\ingroup intrusive_flat_multiset
  /// Vector full exception.
  //***************************************************************************
  class intrusive_flat_multiset_full : public intrusive_flat_multiset_exception
  {
  public:

    intrusive_flat_multiset_full(string_type file_name, numeric_type line_number)
      : intrusive_flat_multiset_exception(ETL_ERROR_TEXT("intrusive_flat_multiset:full", ETL_FILE"A"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  ///\ingroup intrusive_flat_multiset
  /// Vector iterator exception.
  //***************************************************************************
  class intrusive_flat_multiset_iterator : public intrusive_flat_multiset_exception
  {
  public:

    intrusive_flat_multiset_iterator(string_type file_name, numeric_type line_number)
      : intrusive_flat_multiset_exception(ETL_ERROR_TEXT("intrusive_flat_multiset:iterator", ETL_FILE"C"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  /// The base class for specifically sized intrusive_flat_multisets.
  /// Can be used as a reference type for all intrusive_flat_multisets containing a specific type.
  ///\ingroup intrusive_flat_multiset
  //***************************************************************************
  template <typename T, typename TKeyCompare = std::less<T> >
  class iintrusive_flat_multiset
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

  public:

    //*************************************************************************
    class iterator : public std::iterator<std::bidirectional_iterator_tag, value_type>
    {
    public:

      friend class iintrusive_flat_multiset;

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

      friend class iintrusive_flat_multiset;

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
        return etl::addressof(*(*ilookup));
      }

      pointer operator ->()
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

  public:

    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef typename std::iterator_traits<iterator>::difference_type difference_type;

    //*********************************************************************
    /// Returns an iterator to the beginning of the intrusive_flat_multiset.
    ///\return An iterator to the beginning of the intrusive_flat_multiset.
    //*********************************************************************
    iterator begin()
    {
      return iterator(lookup.begin());
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the intrusive_flat_multiset.
    ///\return A const iterator to the beginning of the intrusive_flat_multiset.
    //*********************************************************************
    const_iterator begin() const
    {
      return const_iterator(lookup.begin());
    }

    //*********************************************************************
    /// Returns an iterator to the end of the intrusive_flat_multiset.
    ///\return An iterator to the end of the intrusive_flat_multiset.
    //*********************************************************************
    iterator end()
    {
      return iterator(lookup.end());
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the intrusive_flat_multiset.
    ///\return A const iterator to the end of the intrusive_flat_multiset.
    //*********************************************************************
    const_iterator end() const
    {
      return const_iterator(lookup.end());
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the intrusive_flat_multiset.
    ///\return A const iterator to the beginning of the intrusive_flat_multiset.
    //*********************************************************************
    const_iterator cbegin() const
    {
      return const_iterator(lookup.cbegin());
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the intrusive_flat_multiset.
    ///\return A const iterator to the end of the intrusive_flat_multiset.
    //*********************************************************************
    const_iterator cend() const
    {
      return const_iterator(lookup.cend());
    }

    //*********************************************************************
    /// Returns an reverse iterator to the reverse beginning of the intrusive_flat_multiset.
    ///\return Iterator to the reverse beginning of the intrusive_flat_multiset.
    //*********************************************************************
    reverse_iterator rbegin()
    {
      return reverse_iterator(lookup.rbegin());
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the intrusive_flat_multiset.
    ///\return Const iterator to the reverse beginning of the intrusive_flat_multiset.
    //*********************************************************************
    const_reverse_iterator rbegin() const
    {
      return const_reverse_iterator(lookup.rbegin());
    }

    //*********************************************************************
    /// Returns a reverse iterator to the end + 1 of the intrusive_flat_multiset.
    ///\return Reverse iterator to the end + 1 of the intrusive_flat_multiset.
    //*********************************************************************
    reverse_iterator rend()
    {
      return reverse_iterator(lookup.rend());
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the end + 1 of the intrusive_flat_multiset.
    ///\return Const reverse iterator to the end + 1 of the intrusive_flat_multiset.
    //*********************************************************************
    const_reverse_iterator rend() const
    {
      return const_reverse_iterator(lookup.rend());
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the intrusive_flat_multiset.
    ///\return Const reverse iterator to the reverse beginning of the intrusive_flat_multiset.
    //*********************************************************************
    const_reverse_iterator crbegin() const
    {
      return const_reverse_iterator(lookup.crbegin());
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the end + 1 of the intrusive_flat_multiset.
    ///\return Const reverse iterator to the end + 1 of the intrusive_flat_multiset.
    //*********************************************************************
    const_reverse_iterator crend() const
    {
      return const_reverse_iterator(lookup.crend());
    }

    //*********************************************************************
    /// Assigns values to the intrusive_flat_multiset.
    /// If asserts or exceptions are enabled, emits intrusive_flat_multiset_full if the intrusive_flat_multiset does not have enough free space.
    /// If asserts or exceptions are enabled, emits intrusive_flat_multiset_iterator if the iterators are reversed.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*********************************************************************
    template <typename TIterator>
    void assign(TIterator first, TIterator last)
    {
#if defined(ETL_DEBUG)
      difference_type count = std::distance(first, last);
      ETL_ASSERT(count <= difference_type(capacity()), ETL_ERROR(intrusive_flat_multiset_full));
#endif

      clear();

      while (first != last)
      {
        insert(*first++);
      }
    }

    //*********************************************************************
    /// Inserts a value to the intrusive_flat_multiset.
    /// If asserts or exceptions are enabled, emits intrusive_flat_multiset_full if the intrusive_flat_multiset is already full.
    ///\param value    The value to insert.
    //*********************************************************************
    std::pair<iterator, bool> insert(value_type& value)
    {
      std::pair<iterator, bool> result(end(), false);

      ETL_ASSERT(!lookup.full(), ETL_ERROR(intrusive_flat_multiset_full));

      iterator i_element = std::lower_bound(begin(), end(), value, TKeyCompare());

      if (i_element == end())
      {
        // At the end. Doesn't exist.
        lookup.push_back(&value);
        result.first = --end();
        result.second = true;
      }
      else
      {
        // Not at the end.
        lookup.insert(i_element.ilookup, &value);
        result.first = i_element;
        result.second = true;
      }

      return result;
    }

    //*********************************************************************
    /// Inserts a value to the intrusive_flat_multiset.
    /// If asserts or exceptions are enabled, emits intrusive_flat_multiset_full if the intrusive_flat_multiset is already full.
    ///\param position The position to insert at.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(iterator position, value_type& value)
    {
      return insert(value).first;
    }

    //*********************************************************************
    /// Inserts a range of values to the intrusive_flat_multiset.
    /// If asserts or exceptions are enabled, emits intrusive_flat_multiset_full if the intrusive_flat_multiset does not have enough free space.
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
    size_t erase(value_type& key)
    {
      std::pair<iterator, iterator> range = equal_range(key);

      if (range.first == end())
      {
        return 0;
      }
      else
      {
        size_t count = std::distance(range.first, range.second);
        erase(range.first, range.second);
        return count;
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
      lookup.erase(first.ilookup, last.ilookup);;
    }

    //*************************************************************************
    /// Clears the intrusive_flat_multiset.
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
    iterator find(value_type& key)
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
    const_iterator find(value_type& key) const
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
    size_t count(value_type& key) const
    {
      std::pair<const_iterator, const_iterator> range = equal_range(key);

      return std::distance(range.first, range.second);
    }

    //*********************************************************************
    /// Finds the lower bound of a key
    ///\param key The key to search for.
    ///\return An iterator.
    //*********************************************************************
    iterator lower_bound(value_type& key)
    {
      return std::lower_bound(begin(), end(), key, TKeyCompare());
    }

    //*********************************************************************
    /// Finds the lower bound of a key
    ///\param key The key to search for.
    ///\return An iterator.
    //*********************************************************************
    const_iterator lower_bound(value_type& key) const
    {
      return std::lower_bound(cbegin(), cend(), key, TKeyCompare());
    }
    
    //*********************************************************************
    /// Finds the upper bound of a key
    ///\param key The key to search for.
    ///\return An iterator.
    //*********************************************************************
    iterator upper_bound(value_type& key)
    {
      return std::upper_bound(begin(), end(), key, TKeyCompare());
    }

    //*********************************************************************
    /// Finds the upper bound of a key
    ///\param key The key to search for.
    ///\return An iterator.
    //*********************************************************************
    const_iterator upper_bound(value_type& key) const
    {
      return std::upper_bound(cbegin(), cend(), key, TKeyCompare());
    }

    //*********************************************************************
    /// Finds the range of equal elements of a key
    ///\param key The key to search for.
    ///\return An iterator pair.
    //*********************************************************************
    std::pair<iterator, iterator> equal_range(value_type& key)
    {
      return std::equal_range(begin(), end(), key, TKeyCompare());
    }

    //*********************************************************************
    /// Finds the range of equal elements of a key
    ///\param key The key to search for.
    ///\return An iterator pair.
    //*********************************************************************
    std::pair<const_iterator, const_iterator> equal_range(value_type& key) const
    {
      return std::equal_range(begin(), end(), key, TKeyCompare());
    }

    //*************************************************************************
    /// Gets the current size of the intrusive_flat_multiset.
    ///\return The current size of the intrusive_flat_multiset.
    //*************************************************************************
    size_type size() const
    {
      return lookup.size();
    }

    //*************************************************************************
    /// Checks the 'empty' state of the intrusive_flat_multiset.
    ///\return <b>true</b> if empty.
    //*************************************************************************
    bool empty() const
    {
      return lookup.empty();
    }

    //*************************************************************************
    /// Checks the 'full' state of the intrusive_flat_multiset.
    ///\return <b>true</b> if full.
    //*************************************************************************
    bool full() const
    {
      return lookup.full();
    }

    //*************************************************************************
    /// Returns the capacity of the intrusive_flat_multiset.
    ///\return The capacity of the intrusive_flat_multiset.
    //*************************************************************************
    size_type capacity() const
    {
      return lookup.capacity();
    }

    //*************************************************************************
    /// Returns the maximum possible size of the intrusive_flat_multiset.
    ///\return The maximum size of the intrusive_flat_multiset.
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
    iintrusive_flat_multiset(lookup_t& lookup_)
      : lookup(lookup_)
    {
    }

  private:

    // Disable copy construction.
    iintrusive_flat_multiset(const iintrusive_flat_multiset&);
    iintrusive_flat_multiset& operator =(const iintrusive_flat_multiset&);

    lookup_t& lookup;
  };

  //***************************************************************************
  /// An intrusive flat set
  ///\ingroup intrusive_flat_multiset
  //***************************************************************************
  template <typename TKey, const size_t MAX_SIZE_, typename TKeyCompare = std::less<TKey> >
  class intrusive_flat_multiset : public iintrusive_flat_multiset<TKey, TKeyCompare>
  {
  public:

    static const size_t MAX_SIZE = MAX_SIZE_;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    intrusive_flat_multiset()
      : iintrusive_flat_multiset<TKey, TKeyCompare>(lookup)
    {
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    intrusive_flat_multiset(const intrusive_flat_multiset& other)
      : iintrusive_flat_multiset<TKey, TKeyCompare>(lookup)
    {
      iintrusive_flat_multiset<TKey, TKeyCompare>::assign(other.cbegin(), other.cend());
    }

    //*************************************************************************
    /// Constructor, from an iterator range.
    ///\tparam TIterator The iterator type.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*************************************************************************
    template <typename TIterator>
    intrusive_flat_multiset(TIterator first, TIterator last)
      : iintrusive_flat_multiset<TKey, TKeyCompare>(lookup)
    {
      iintrusive_flat_multiset<TKey, TKeyCompare>::assign(first, last);
    }

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~intrusive_flat_multiset()
    {
      iintrusive_flat_multiset<TKey, TKeyCompare>::clear();
    }

  private:

    typedef TKey value_type;
    
    // The vector that stores pointers to the nodes.
    etl::vector<value_type*, MAX_SIZE> lookup;
  };

  //***************************************************************************
  /// Equal operator.
  ///\param lhs Reference to the first intrusive_flat_multiset.
  ///\param rhs Reference to the second intrusive_flat_multiset.
  ///\return <b>true</b> if the arrays are equal, otherwise <b>false</b>
  ///\ingroup intrusive_flat_multiset
  //***************************************************************************
  template <typename T, typename TKeyCompare>
  bool operator ==(const etl::iintrusive_flat_multiset<T, TKeyCompare>& lhs, const etl::iintrusive_flat_multiset<T, TKeyCompare>& rhs)
  {
    return (lhs.size() == rhs.size()) && std::equal(lhs.begin(), lhs.end(), rhs.begin());
  }

  //***************************************************************************
  /// Not equal operator.
  ///\param lhs Reference to the first intrusive_flat_multiset.
  ///\param rhs Reference to the second intrusive_flat_multiset.
  ///\return <b>true</b> if the arrays are not equal, otherwise <b>false</b>
  ///\ingroup intrusive_flat_multiset
  //***************************************************************************
  template <typename T, typename TKeyCompare>
  bool operator !=(const etl::iintrusive_flat_multiset<T, TKeyCompare>& lhs, const etl::iintrusive_flat_multiset<T, TKeyCompare>& rhs)
  {
    return !(lhs == rhs);
  }
}

#undef ETL_FILE
#endif
