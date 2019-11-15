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

#ifndef ETL_REFERENCE_FLAT_MULTISET_INCLUDED
#define ETL_REFERENCE_FLAT_MULTISET_INCLUDED

#include <stddef.h>

#include "platform.h"

#include "stl/algorithm.h"
#include "stl/iterator.h"
#include "stl/functional.h"
#include "stl/utility.h"

#include "type_traits.h"
#include "vector.h"
#include "pool.h"
#include "error_handler.h"
#include "exception.h"

#undef ETL_FILE
#define ETL_FILE "33"

namespace etl
{
  //***************************************************************************
  ///\ingroup reference_flat_multiset
  /// Exception base for reference_flat_multisets
  //***************************************************************************
  class flat_multiset_exception : public exception
  {
  public:

    flat_multiset_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  ///\ingroup reference_flat_multiset
  /// Vector full exception.
  //***************************************************************************
  class flat_multiset_full : public flat_multiset_exception
  {
  public:

    flat_multiset_full(string_type file_name_, numeric_type line_number_)
      : flat_multiset_exception(ETL_ERROR_TEXT("flat_multiset:full", ETL_FILE"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  ///\ingroup reference_flat_multiset
  /// Vector iterator exception.
  //***************************************************************************
  class flat_multiset_iterator : public flat_multiset_exception
  {
  public:

    flat_multiset_iterator(string_type file_name_, numeric_type line_number_)
      : flat_multiset_exception(ETL_ERROR_TEXT("flat_multiset:iterator", ETL_FILE"C"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// The base class for specifically sized reference_flat_multisets.
  /// Can be used as a reference type for all reference_flat_multisets containing a specific type.
  ///\ingroup reference_flat_multiset
  //***************************************************************************
  template <typename T, typename TKeyCompare = std::less<T> >
  class ireference_flat_multiset
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

  protected:

    typedef etl::ivector<value_type*> lookup_t;

  public:

    //*************************************************************************
    class iterator : public std::iterator<std::bidirectional_iterator_tag, value_type>
    {
    public:

      friend class ireference_flat_multiset;

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

      friend class ireference_flat_multiset;

      const_iterator()
      {
      }

      const_iterator(typename lookup_t::const_iterator ilookup_)
        : ilookup(ilookup_)
      {
      }

      const_iterator(const typename ireference_flat_multiset::iterator& other)
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
    /// Returns an iterator to the beginning of the reference_flat_multiset.
    ///\return An iterator to the beginning of the reference_flat_multiset.
    //*********************************************************************
    iterator begin()
    {
      return iterator(lookup.begin());
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the reference_flat_multiset.
    ///\return A const iterator to the beginning of the reference_flat_multiset.
    //*********************************************************************
    const_iterator begin() const
    {
      return const_iterator(lookup.begin());
    }

    //*********************************************************************
    /// Returns an iterator to the end of the reference_flat_multiset.
    ///\return An iterator to the end of the reference_flat_multiset.
    //*********************************************************************
    iterator end()
    {
      return iterator(lookup.end());
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the reference_flat_multiset.
    ///\return A const iterator to the end of the reference_flat_multiset.
    //*********************************************************************
    const_iterator end() const
    {
      return const_iterator(lookup.end());
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the reference_flat_multiset.
    ///\return A const iterator to the beginning of the reference_flat_multiset.
    //*********************************************************************
    const_iterator cbegin() const
    {
      return const_iterator(lookup.cbegin());
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the reference_flat_multiset.
    ///\return A const iterator to the end of the reference_flat_multiset.
    //*********************************************************************
    const_iterator cend() const
    {
      return const_iterator(lookup.cend());
    }

    //*********************************************************************
    /// Returns an reverse iterator to the reverse beginning of the reference_flat_multiset.
    ///\return Iterator to the reverse beginning of the reference_flat_multiset.
    //*********************************************************************
    reverse_iterator rbegin()
    {
      return reverse_iterator(lookup.rbegin());
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the reference_flat_multiset.
    ///\return Const iterator to the reverse beginning of the reference_flat_multiset.
    //*********************************************************************
    const_reverse_iterator rbegin() const
    {
      return const_reverse_iterator(lookup.rbegin());
    }

    //*********************************************************************
    /// Returns a reverse iterator to the end + 1 of the reference_flat_multiset.
    ///\return Reverse iterator to the end + 1 of the reference_flat_multiset.
    //*********************************************************************
    reverse_iterator rend()
    {
      return reverse_iterator(lookup.rend());
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the end + 1 of the reference_flat_multiset.
    ///\return Const reverse iterator to the end + 1 of the reference_flat_multiset.
    //*********************************************************************
    const_reverse_iterator rend() const
    {
      return const_reverse_iterator(lookup.rend());
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the reference_flat_multiset.
    ///\return Const reverse iterator to the reverse beginning of the reference_flat_multiset.
    //*********************************************************************
    const_reverse_iterator crbegin() const
    {
      return const_reverse_iterator(lookup.crbegin());
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the end + 1 of the reference_flat_multiset.
    ///\return Const reverse iterator to the end + 1 of the reference_flat_multiset.
    //*********************************************************************
    const_reverse_iterator crend() const
    {
      return const_reverse_iterator(lookup.crend());
    }

    //*********************************************************************
    /// Assigns values to the reference_flat_multiset.
    /// If asserts or exceptions are enabled, emits reference_flat_multiset_full if the reference_flat_multiset does not have enough free space.
    /// If asserts or exceptions are enabled, emits reference_flat_multiset_iterator if the iterators are reversed.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*********************************************************************
    template <typename TIterator>
    void assign(TIterator first, TIterator last)
    {
#if defined(ETL_DEBUG)
      difference_type d = std::distance(first, last);
      ETL_ASSERT(d <= difference_type(capacity()), ETL_ERROR(flat_multiset_full));
#endif

      clear();

      while (first != last)
      {
        insert(*first++);
      }
    }

    //*********************************************************************
    /// Inserts a value to the reference_flat_multiset.
    /// If asserts or exceptions are enabled, emits reference_flat_multiset_full if the reference_flat_multiset is already full.
    ///\param value    The value to insert.
    //*********************************************************************
    std::pair<iterator, bool> insert(value_type& value)
    {
      std::pair<iterator, bool> result(end(), false);

      ETL_ASSERT(!lookup.full(), ETL_ERROR(flat_multiset_full));

      iterator i_element = std::lower_bound(begin(), end(), value, compare);

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
    /// Inserts a value to the reference_flat_multiset.
    /// If asserts or exceptions are enabled, emits reference_flat_multiset_full if the reference_flat_multiset is already full.
    ///\param position The position to insert at.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(iterator position, value_type& value)
    {
      return insert(value).first;
    }

    //*********************************************************************
    /// Inserts a range of values to the reference_flat_multiset.
    /// If asserts or exceptions are enabled, emits reference_flat_multiset_full if the reference_flat_multiset does not have enough free space.
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
      std::pair<iterator, iterator> range = equal_range(key);

      if (range.first == end())
      {
        return 0;
      }
      else
      {
        size_t d = std::distance(range.first, range.second);
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
    /// Clears the reference_flat_multiset.
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
    iterator find(parameter_t key)
    {
      iterator itr = std::lower_bound(begin(), end(), key, compare);

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
      const_iterator itr = std::lower_bound(begin(), end(), key, compare);

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
      std::pair<const_iterator, const_iterator> range = equal_range(key);

      return std::distance(range.first, range.second);
    }

    //*********************************************************************
    /// Finds the lower bound of a key
    ///\param key The key to search for.
    ///\return An iterator.
    //*********************************************************************
    iterator lower_bound(parameter_t key)
    {
      return std::lower_bound(begin(), end(), key, compare);
    }

    //*********************************************************************
    /// Finds the lower bound of a key
    ///\param key The key to search for.
    ///\return An iterator.
    //*********************************************************************
    const_iterator lower_bound(parameter_t key) const
    {
      return std::lower_bound(cbegin(), cend(), key, compare);
    }

    //*********************************************************************
    /// Finds the upper bound of a key
    ///\param key The key to search for.
    ///\return An iterator.
    //*********************************************************************
    iterator upper_bound(parameter_t key)
    {
      return std::upper_bound(begin(), end(), key, compare);
    }

    //*********************************************************************
    /// Finds the upper bound of a key
    ///\param key The key to search for.
    ///\return An iterator.
    //*********************************************************************
    const_iterator upper_bound(parameter_t key) const
    {
      return std::upper_bound(cbegin(), cend(), key, compare);
    }

    //*********************************************************************
    /// Finds the range of equal elements of a key
    ///\param key The key to search for.
    ///\return An iterator pair.
    //*********************************************************************
    std::pair<iterator, iterator> equal_range(parameter_t key)
    {
      return std::equal_range(begin(), end(), key, compare);
    }

    //*********************************************************************
    /// Finds the range of equal elements of a key
    ///\param key The key to search for.
    ///\return An iterator pair.
    //*********************************************************************
    std::pair<const_iterator, const_iterator> equal_range(parameter_t key) const
    {
      return std::equal_range(begin(), end(), key, compare);
    }

    //*************************************************************************
    /// Gets the current size of the reference_flat_multiset.
    ///\return The current size of the reference_flat_multiset.
    //*************************************************************************
    size_type size() const
    {
      return lookup.size();
    }

    //*************************************************************************
    /// Checks the 'empty' state of the reference_flat_multiset.
    ///\return <b>true</b> if empty.
    //*************************************************************************
    bool empty() const
    {
      return lookup.empty();
    }

    //*************************************************************************
    /// Checks the 'full' state of the reference_flat_multiset.
    ///\return <b>true</b> if full.
    //*************************************************************************
    bool full() const
    {
      return lookup.full();
    }

    //*************************************************************************
    /// Returns the capacity of the reference_flat_multiset.
    ///\return The capacity of the reference_flat_multiset.
    //*************************************************************************
    size_type capacity() const
    {
      return lookup.capacity();
    }

    //*************************************************************************
    /// Returns the maximum possible size of the reference_flat_multiset.
    ///\return The maximum size of the reference_flat_multiset.
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
    ireference_flat_multiset(lookup_t& lookup_)
      : lookup(lookup_)
    {
    }

    //*********************************************************************
    /// Inserts a value to the reference_flat_set.
    ///\param i_element The place to insert.
    ///\param value     The value to insert.
    //*********************************************************************
    std::pair<iterator, bool> insert_at(iterator i_element, reference value)
    {
      std::pair<iterator, bool> result(end(), false);

      if (i_element == end())
      {
        // At the end.
        ETL_ASSERT(!lookup.full(), ETL_ERROR(flat_multiset_full));

        lookup.push_back(&value);
        result.first = --end();
        result.second = true;
      }
      else
      {
        // Not at the end.
        result.first = i_element;

        // A new one.
        ETL_ASSERT(!lookup.full(), ETL_ERROR(flat_multiset_full));
        lookup.insert(i_element.ilookup, &value);
        result.second = true;
      }

      return result;
    }

  private:

    // Disable copy construction.
    ireference_flat_multiset(const ireference_flat_multiset&);
    ireference_flat_multiset& operator =(const ireference_flat_multiset&);

    lookup_t& lookup;

    TKeyCompare compare;

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
#if defined(ETL_POLYMORPHIC_REFERENCE_FLAT_MULTISET) || defined(ETL_POLYMORPHIC_CONTAINERS)
  public:
    virtual ~ireference_flat_multiset()
    {
    }
#else
  protected:
    ~ireference_flat_multiset()
    {
    }
#endif
  };

  //***************************************************************************
  /// An reference flat set
  ///\ingroup reference_flat_multiset
  //***************************************************************************
  template <typename TKey, const size_t MAX_SIZE_, typename TKeyCompare = std::less<TKey> >
  class reference_flat_multiset : public ireference_flat_multiset<TKey, TKeyCompare>
  {
  public:

    static const size_t MAX_SIZE = MAX_SIZE_;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    reference_flat_multiset()
      : ireference_flat_multiset<TKey, TKeyCompare>(lookup)
    {
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    reference_flat_multiset(const reference_flat_multiset& other)
      : ireference_flat_multiset<TKey, TKeyCompare>(lookup)
    {
      ireference_flat_multiset<TKey, TKeyCompare>::assign(other.cbegin(), other.cend());
    }

    //*************************************************************************
    /// Constructor, from an iterator range.
    ///\tparam TIterator The iterator type.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*************************************************************************
    template <typename TIterator>
    reference_flat_multiset(TIterator first, TIterator last)
      : ireference_flat_multiset<TKey, TKeyCompare>(lookup)
    {
      ireference_flat_multiset<TKey, TKeyCompare>::assign(first, last);
    }

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~reference_flat_multiset()
    {
      ireference_flat_multiset<TKey, TKeyCompare>::clear();
    }

  private:

    typedef TKey value_type;

    // The vector that stores pointers to the nodes.
    etl::vector<value_type*, MAX_SIZE> lookup;
  };

  //***************************************************************************
  /// Equal operator.
  ///\param lhs Reference to the first reference_flat_multiset.
  ///\param rhs Reference to the second reference_flat_multiset.
  ///\return <b>true</b> if the arrays are equal, otherwise <b>false</b>
  ///\ingroup reference_flat_multiset
  //***************************************************************************
  template <typename T, typename TKeyCompare>
  bool operator ==(const etl::ireference_flat_multiset<T, TKeyCompare>& lhs, const etl::ireference_flat_multiset<T, TKeyCompare>& rhs)
  {
    return (lhs.size() == rhs.size()) && std::equal(lhs.begin(), lhs.end(), rhs.begin());
  }

  //***************************************************************************
  /// Not equal operator.
  ///\param lhs Reference to the first reference_flat_multiset.
  ///\param rhs Reference to the second reference_flat_multiset.
  ///\return <b>true</b> if the arrays are not equal, otherwise <b>false</b>
  ///\ingroup reference_flat_multiset
  //***************************************************************************
  template <typename T, typename TKeyCompare>
  bool operator !=(const etl::ireference_flat_multiset<T, TKeyCompare>& lhs, const etl::ireference_flat_multiset<T, TKeyCompare>& rhs)
  {
    return !(lhs == rhs);
  }
}

#undef ETL_FILE
#endif
