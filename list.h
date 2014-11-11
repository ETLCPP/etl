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

#ifndef __ETL_LIST__
#define __ETL_LIST__

#include <cstddef>

#include "ilist.h"
#include "container.h"

//*****************************************************************************
///\defgroup list list
/// A linked list with the capacity defined at compile time.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //*************************************************************************
  /// A templated list implementation that uses a fixed size buffer.
  /// MAX_SIZE_ elements will be always be constructed.
  ///\note 'merge' and 'splice' and are not supported.
  //*************************************************************************
  template <typename T, const size_t MAX_SIZE_>
  class list : public ilist<T>
  {
  public:

    static const size_t MAX_SIZE = MAX_SIZE_;

  public:

    typedef T        value_type;
    typedef T*       pointer;
    typedef const T* const_pointer;
    typedef T&       reference;
    typedef const T& const_reference;
    typedef size_t   size_type;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    inline list()
      : ilist<T>(&node_pool[0], MAX_SIZE)
    {
    }

    //*************************************************************************
    /// Construct from size and value.
    //*************************************************************************
    inline explicit list(size_t initialSize, T value = T())
      : ilist<T>(&node_pool[0], MAX_SIZE)
    {
      ilist<T>::assign(initialSize, value);
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    explicit list(const list& other)
      : ilist<T>(&node_pool[0], MAX_SIZE)
    {
			ilist<T>::assign(other.cbegin(), other.cend());
    }

    //*************************************************************************
    /// Construct from range.
    //*************************************************************************
    template <typename TIterator>
    list(TIterator first, TIterator last)
      : ilist<T>(&node_pool[0], MAX_SIZE)
    {
      ilist<T>::assign(first, last);
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    list& operator = (const list& rhs)
    {
      ilist<T>::assign(rhs.cbegin(), rhs.cend());

      return *this;
    }

    //*************************************************************************
    /// Swap
    //*************************************************************************
    void swap(list& other)
    {
      // Re-align the node pointers for this list.
      if (this->terminal_node.previous != 0)
      {
        size_t index = std::distance(&node_pool[0], static_cast<Data_Node*>(this->terminal_node.previous));
        this->terminal_node.previous = &node_pool[index];
      }

      if (this->terminal_node.next != 0)
      {
        size_t index = std::distance(&node_pool[0], static_cast<Data_Node*>(this->terminal_node.next));
        this->terminal_node.next = &node_pool[index];
      }

      for (size_t i = 0; i < MAX_SIZE; ++i)
      {
        typename ilist<T>::Data_Node& data_node = node_pool[i];

        if (!data_node.is_free())
        {
          size_t index;

          index = std::distance(&node_pool[0], static_cast<Data_Node*>(data_node.previous));
          data_node.previous = &other.node_pool[index];

          index = std::distance(&node_pool[0], static_cast<Data_Node*>(data_node.next));
          data_node.next = &other.node_pool[index];
        }
      }

      // Re-align the node pointers for the other list.
      if (other.terminal_node.previous != 0)
      {
        size_t index = std::distance(&other.node_pool[0], static_cast<Data_Node*>(other.terminal_node.previous));
        other.terminal_node.previous = &other.node_pool[index];
      }

      if (other.terminal_node.next != 0)
      {
        size_t index = std::distance(&other.node_pool[0], static_cast<Data_Node*>(other.terminal_node.next));
        other.terminal_node.next = &other.node_pool[index];
      }

      for (size_t i = 0; i < MAX_SIZE; ++i)
      {
        typename ilist<T>::Data_Node& data_node = other.node_pool[i];

        if (!data_node.is_free())
        {
          size_t index;

          index = std::distance(&other.node_pool[0], static_cast<Data_Node*>(data_node.previous));
          data_node.previous = &node_pool[index];

          index = std::distance(&other.node_pool[0], static_cast<Data_Node*>(data_node.next));
          data_node.next = &node_pool[index];
        }
      }

      // Swap the data.
      std::swap_ranges(etl::begin(node_pool), etl::end(node_pool), etl::begin(other.node_pool));
      std::swap(this->next_free, other.next_free);
      std::swap(this->count, other.count);
    }

  private:

    /// The pool of nodes used in the list.
    typename ilist<T>::Data_Node node_pool[MAX_SIZE];
  };

  //*************************************************************************
  /// Swap
  //*************************************************************************
  template <typename T, const size_t MAX_SIZE>
  void swap(etl::list<T, MAX_SIZE>& first, etl::list<T, MAX_SIZE>& second)
  {
    first.swap(second);
  }
}

#endif
