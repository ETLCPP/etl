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

#include "iforward_list.h"
#include "container.h"

//*****************************************************************************
///\defgroup forward_list forward_list
/// A linked forward_list with the capacity defined at compile time.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //*************************************************************************
  /// A templated forward_list implementation that uses a fixed size buffer.
  /// MAX_SIZE_ elements will be always be constructed.
  ///\note 'merge' and 'splice_after' and are not supported.
  //*************************************************************************
  template <typename T, const size_t MAX_SIZE_>
  class forward_list : public iforward_list<T>
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
    inline forward_list()
      : iforward_list<T>(&node_pool[0], MAX_SIZE)
    {
    }

    //*************************************************************************
    /// Construct from size and value.
    //*************************************************************************
    inline explicit forward_list(size_t initialSize, typename iforward_list<T>::parameter_t value = T())
      : iforward_list<T>(&node_pool[0], MAX_SIZE)
    {
      iforward_list<T>::assign(initialSize, value);
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    explicit forward_list(const forward_list& other)
      : iforward_list<T>(&node_pool[0], MAX_SIZE)
    {
			iforward_list<T>::assign(other.cbegin(), other.cend());
    }

    //*************************************************************************
    /// Construct from range.
    //*************************************************************************
    template <typename TIterator>
    forward_list(TIterator first, TIterator last)
      : iforward_list<T>(&node_pool[0], MAX_SIZE)
    {
      iforward_list<T>::assign(first, last);
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    forward_list& operator = (const forward_list& rhs)
    {
      iforward_list<T>::assign(rhs.cbegin(), rhs.cend());

      return *this;
    }

    //*************************************************************************
    /// Swap
    //*************************************************************************
    void swap(forward_list& other)
    {
      // Re-align the node pointers for this forward_list.
      if (this->start_node.next != 0)
      {
        size_t index = std::distance(&node_pool[0], static_cast<typename iforward_list<T>::Data_Node*>(this->start_node.next));
        this->start_node.next = &node_pool[index];
      }

      if (this->end_node.next != 0)
      {
        size_t index = std::distance(&node_pool[0], static_cast<typename iforward_list<T>::Data_Node*>(this->end_node.next));
        this->end_node.next = &node_pool[index];
      }

      for (size_t i = 0; i < MAX_SIZE; ++i)
      {
        typename iforward_list<T>::Node& node = node_pool[i];

        if (!node.is_free())
        {
          size_t index = std::distance(&node_pool[0], static_cast<typename iforward_list<T>::Data_Node*>(node.next));
          node.next = &other.node_pool[index];
        }
      }

      // Re-align the node pointers for the other forward_list.
      if (other.start_node.next != 0)
      {
        size_t index = std::distance(&other.node_pool[0], static_cast<typename iforward_list<T>::Data_Node*>(other.start_node.next));
        other.start_node.next = &other.node_pool[index];
      }

      if (other.end_node.next != 0)
      {
        size_t index = std::distance(&other.node_pool[0], static_cast<typename iforward_list<T>::Data_Node*>(other.end_node.next));
        other.end_node.next = &other.node_pool[index];
      }

      for (size_t i = 0; i < MAX_SIZE; ++i)
      {
        typename iforward_list<T>::Node& node = other.node_pool[i];

        if (!node.is_free())
        {
          size_t index = std::distance(&other.node_pool[0], static_cast<typename iforward_list<T>::Data_Node*>(node.next));
          node.next = &node_pool[index];
        }
      }

      // Swap the data.
      std::swap_ranges(etl::begin(node_pool), etl::end(node_pool), etl::begin(other.node_pool));
      std::swap(this->next_free, other.next_free);
      std::swap(this->count, other.count);
    }

  private:

    /// The pool of nodes used in the forward_list.
    typename iforward_list<T>::Data_Node node_pool[MAX_SIZE];
  };

  //*************************************************************************
  /// Swap
  //*************************************************************************
  template <typename T, const size_t MAX_SIZE>
  void swap(etl::forward_list<T, MAX_SIZE>& first, etl::forward_list<T, MAX_SIZE>& second)
  {
    first.swap(second);
  }
}

#endif
