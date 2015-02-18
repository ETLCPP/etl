///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.

Copyright(c) 2014 jwellbelove

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

#ifndef __ETL_IFLAT_MAP__
#define __ETL_IFLAT_MAP__
#define __ETL_IN_IFLAT_MAP_H__

#include <iterator>
#include <algorithm>
#include <functional>
#include <utility>
#include <stddef.h>

#include "flat_map_base.h"
#include "type_traits.h"
#include "parameter_type.h"
#include "ivector.h"

#ifndef ETL_THROW_EXCEPTIONS
#include "error_handler.h"
#endif

namespace etl
{
  //***************************************************************************
  /// The base class for specifically sized flat_maps.
  /// Can be used as a reference type for all flat_maps containing a specific type.
  ///\ingroup flat_map
  //***************************************************************************
  template <typename TKey, typename TMapped, typename TKeyCompare>
  class iflat_map : public flat_map_base
  {
  public:

    typedef std::pair<TKey, TMapped> value_type;

  private:

    typedef etl::ivector<value_type> buffer_t;

  public:

    typedef TKey                                      key_type;
    typedef TMapped                                   mapped_type;
    typedef TKeyCompare                               key_compare;
    typedef value_type&                               reference;
    typedef const value_type&                         const_reference;
    typedef value_type*                               pointer;
    typedef const value_type*                         const_pointer;
    typedef typename buffer_t::iterator               iterator;
    typedef typename buffer_t::const_iterator         const_iterator;
    typedef typename buffer_t::reverse_iterator       reverse_iterator;
    typedef typename buffer_t::const_reverse_iterator const_reverse_iterator;
    typedef size_t                                    size_type;
    typedef typename std::iterator_traits<iterator>::difference_type difference_type;

  protected:

    typedef typename parameter_type<TKey>::type key_value_parameter_t;

    //*************************************************************************
    /// How to compare elements.
    //*************************************************************************
    struct compare_element
    {
      bool operator ()(const value_type& value1, const value_type& value2) const
      {
        return key_compare()(value1.first, value2.first);
      }
    };

  public:

    //*********************************************************************
    /// Assignment operator.
    /// The source flat_map can be larger than the destination, but
    /// only the elements that will fit in the destination will be copied.
    ///\param other The other flat_map.
    //*********************************************************************
    iflat_map& operator = (iflat_map& other)
    {
      buffer.operator=(other.buffer);

      return *this;
    }

    //*********************************************************************
    /// Returns an iterator to the beginning of the flat_map.
    ///\return An iterator to the beginning of the flat_map.
    //*********************************************************************
    iterator begin()
    {
      return buffer.begin();
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the flat_map.
    ///\return A const iterator to the beginning of the flat_map.
    //*********************************************************************
    const_iterator begin() const
    {
      return buffer.begin();
    }

    //*********************************************************************
    /// Returns an iterator to the end of the flat_map.
    ///\return An iterator to the end of the flat_map.
    //*********************************************************************
    iterator end()
    {
      return buffer.end();
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the flat_map.
    ///\return A const iterator to the end of the flat_map.
    //*********************************************************************
    const_iterator end() const
    {
      return buffer.end();
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the flat_map.
    ///\return A const iterator to the beginning of the flat_map.
    //*********************************************************************
    const_iterator cbegin() const
    {
      return buffer.cbegin();
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the flat_map.
    ///\return A const iterator to the end of the flat_map.
    //*********************************************************************
    const_iterator cend() const
    {
      return buffer.cend();
    }

    //*********************************************************************
    /// Returns an reverse iterator to the reverse beginning of the flat_map.
    ///\return Iterator to the reverse beginning of the flat_map.
    //*********************************************************************
    reverse_iterator rbegin()
    {
	    return buffer.rbegin();
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the flat_map.
    ///\return Const iterator to the reverse beginning of the flat_map.
    //*********************************************************************
    const_reverse_iterator rbegin() const
    {
      return buffer.rbegin();
    }

    //*********************************************************************
    /// Returns a reverse iterator to the end + 1 of the flat_map.
    ///\return Reverse iterator to the end + 1 of the flat_map.
    //*********************************************************************
    reverse_iterator rend()
    {
      return buffer.rend();
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the end + 1 of the flat_map.
    ///\return Const reverse iterator to the end + 1 of the flat_map.
    //*********************************************************************
    const_reverse_iterator rend() const
    {
      return buffer.rend();
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the flat_map.
    ///\return Const reverse iterator to the reverse beginning of the flat_map.
    //*********************************************************************
    const_reverse_iterator crbegin() const
    {
      return buffer.crbegin();
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the end + 1 of the flat_map.
    ///\return Const reverse iterator to the end + 1 of the flat_map.
    //*********************************************************************
    const_reverse_iterator crend() const
    {
      return buffer.crend();
    }

    //*********************************************************************
    /// Returns a reference to the value at index 'key'
    ///\param i The index.
    ///\return A reference to the value at index 'key'
    //*********************************************************************
    mapped_type& operator [](key_value_parameter_t key)
    {
      iterator i_element = std::lower_bound(begin(), end(), value_type(key, TMapped()), compare_element());

      if (i_element->first != key)
      {
        // Doesn't exist, so create a new one.
        i_element = insert(std::make_pair(key, mapped_type()));
      }

      return i_element->second;
    }

    //*********************************************************************
    /// Returns a reference to the value at index 'key'
    /// If ETL_THROW_EXCEPTIONS is defined, emits an etl::flat_map_out_of_bounds if the key is not in the range.
    ///\param i The index.
    ///\return A reference to the value at index 'key'
    //*********************************************************************
    mapped_type& at(key_value_parameter_t key)
    {
      iterator i_element = std::lower_bound(begin(), end(), value_type(key, TMapped()), compare_element());

      if (i_element == end())
      {
        // Doesn't exist.
#if ETL_THROW_EXCEPTIONS
        throw flat_map_out_of_bounds();
#else
        error_handler::error(flat_map_out_of_bounds());

#endif
      }

      return i_element->second;
    }

    //*********************************************************************
    /// Returns a const reference to the value at index 'key'
    /// If ETL_THROW_EXCEPTIONS is defined, emits an etl::flat_map_out_of_bounds if the key is not in the range.
    ///\param i The index.
    ///\return A const reference to the value at index 'key'
    //*********************************************************************
    const mapped_type& at(key_value_parameter_t key) const
    {
      typename buffer_t::const_iterator i_element = std::lower_bound(begin(), end(), value_type(key, TMapped()), compare_element());

      if (i_element == end())
      {
        // Doesn't exist.
#if ETL_THROW_EXCEPTIONS
        throw flat_map_out_of_bounds();
#else
        error_handler::error(flat_map_out_of_bounds());

#endif
      }

      return i_element->second;
    }

    //*********************************************************************
    /// Assigns values to the flat_map.
    /// If ETL_THROW_EXCEPTIONS is defined, emits flat_map_full if the flat_map does not have enough free space.
    /// If ETL_THROW_EXCEPTIONS is defined, emits flat_map_iterator if the iterators are reversed.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*********************************************************************
    template <typename TIterator>
    void assign(TIterator first, TIterator last)
    {
      clear();

      while (first != last)
      {
        insert(*first++);
      }
    }

    //*********************************************************************
    /// Inserts a value to the flat_map.
    /// If ETL_THROW_EXCEPTIONS is defined, emits flat_map_full if the flat_map is already full.
    ///\param position The position to insert at.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(const value_type& value)
    {
      iterator i_element = std::lower_bound(begin(), end(), value, compare_element());

      if (i_element == end())
      {
        // At the end.
        if (buffer.full())
        {
#ifdef ETL_THROW_EXCEPTIONS
          throw flat_map_full();
#else
          error_handler::error(flat_map_full());
#endif
        }
        else
        {
          buffer.push_back(value);
        }
      }
      else
      {
        // Not at the end.
        // Existing element?
        if (value.first == i_element->first)
        {
          // Yes.
          i_element->second = value.second;
        }
        else
        {
          // A new one.
          if (buffer.full())
          {
#ifdef ETL_THROW_EXCEPTIONS
            throw flat_map_full();
#else
            error_handler::error(flat_map_full());
#endif
          }
          else
          {
            buffer.insert(i_element, value);
          }
        }
      }

      return i_element;
    }

    //*********************************************************************
    /// Inserts a range of values to the flat_map.
    /// If ETL_THROW_EXCEPTIONS is defined, emits flat_map_full if the flat_map does not have enough free space.
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
    ///\param i_element Iterator to the element.
    ///\return An iterator pointing to the element that followed the erased element.
    //*********************************************************************
    iterator erase(iterator i_element)
    {
      return buffer.erase(i_element);
    }

    //*********************************************************************
    /// Erases a range of elements.
    /// The range includes all the elements between first and last, including the
    /// element pointed by first, but not the one pointed by last.
    ///\param first Iterator to the first element.
    ///\param last  Iterator to the last element.
    ///\return An iterator pointing to the element that followed the erased element.
    //*********************************************************************
    iterator erase(iterator first, iterator last)
    {
      return buffer.erase(first, last);
    }

    //*************************************************************************
    /// Clears the flat_map.
    //*************************************************************************
    void clear()
    {
      buffer.clear();
    }

    //*********************************************************************
    /// Finds an element.
    ///\param key The key to search for.
    ///\return An iterator pointing to the element or end() if not found.
    //*********************************************************************
    iterator find(key_value_parameter_t key)
    {
      return std::lower_bound(begin(), end(), value_type(key, TMapped()), compare_element());
    }

    //*********************************************************************
    /// Finds an element.
    ///\param key The key to search for.
    ///\return An iterator pointing to the element or end() if not found.
    //*********************************************************************
    const_iterator find(key_value_parameter_t key) const
    {
      return std::lower_bound(begin(), end(), value_type(key, TMapped()), compare_element());
    }

  protected:

    //*********************************************************************
    /// Constructor.
    //*********************************************************************
    iflat_map(buffer_t& buffer)
      : flat_map_base(buffer),
        buffer(buffer)
    {
    }

  private:

    buffer_t& buffer;
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
    return (lhs.size() == rhs.size()) && std::equal(lhs.begin(), lhs.end(), rhs.begin());
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
}

#undef __ETL_IN_IFLAT_MAP_H__
#endif
