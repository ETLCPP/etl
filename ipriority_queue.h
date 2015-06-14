///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl

Copyright(c) 2015 jwellbelove, rlindeman

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

#ifndef __ETL_IPRIORITY_QUEUE__
#define __ETL_IPRIORITY_QUEUE__
#define __ETL_IN_IPRIORITY_QUEUE_H__

#include <stddef.h>
#include <algorithm>

#include "priority_queue_base.h"
#include "type_traits.h"
#include "parameter_type.h"

#ifndef ETL_THROW_EXCEPTIONS
#include "error_handler.h"
#endif

namespace etl
{
  //***************************************************************************
  ///\ingroup queue
  ///\brief This is the base for all priority queues that contain a particular type.
  ///\details Normally a reference to this type will be taken from a derived queue.
  /// The TContainer specified must provide the front, push_back, pop_back, and
  /// assign methods to work correctly with priority_queue.
  ///\code
  /// etl::priority_queue<int, 10> myPriorityQueue;
  /// etl::ipriority_queue<int>& iQueue = myPriorityQueue;
  ///\endcode
  /// \warning This priority queue cannot be used for concurrent access from
  /// multiple threads.
  /// \tparam T The type of value that the queue holds.
  /// \tparam TContainer to hold the T queue values
  /// \tparam TCompare to use in comparing T values
  //***************************************************************************
  template <typename T, typename TContainer, typename TCompare>
  class ipriority_queue : public priority_queue_base
  {
  public:

    typedef T                     value_type;         ///< The type stored in the queue.
    typedef TContainer            container_type;     ///< The container type used for priority queue.
    typedef T&                    reference;          ///< A reference to the type used in the queue.
    typedef const T&              const_reference;    ///< A const reference to the type used in the queue.
    typedef priority_queue_base::size_type size_type; ///< The type used for determining the size of the queue.
    typedef typename std::iterator_traits<typename TContainer::iterator>::difference_type difference_type;

  private:

    typedef typename parameter_type<T>::type parameter_t;

  public:

    //*************************************************************************
    /// Gets a reference to the highest priority value in the priority queue.<br>
    /// \return A reference to the highest priority value in the priority queue.
    //*************************************************************************
    reference top()
    {
      return container.front();
    }

    //*************************************************************************
    /// Gets a const reference to the highest priority value in the priority queue.<br>
    /// \return A const reference to the highest priority value in the priority queue.
    //*************************************************************************
    const_reference top() const
    {
      return container.front();
    }

    //*************************************************************************
    /// Adds a value to the queue.
    /// If ETL_THROW_EXCEPTIONS is defined, throws an etl::priority_queue_full
    /// is the priority queue is already full, otherwise does nothing if full.
    ///\param value The value to push to the queue.
    //*************************************************************************
    void push(parameter_t value)
    {
      if (!full())
      {
        // Put element at end
        container.push_back(value);
        // Pre-increment size
        ++current_size;
        // Make elements in container into heap
        std::push_heap(container.begin(), container.end(), TCompare());
      }
      else
#ifdef ETL_THROW_EXCEPTIONS
      {
        throw priority_queue_full();
      }
#else
      {
        error_handler::error(priority_queue_full());
      }
#endif
    }

    //*************************************************************************
    /// Clears the queue to the empty state.
    //*************************************************************************
    void clear()
    {
      container.clear();
      current_size = 0;
    }

    //*********************************************************************
    /// Assigns values to the priority queue.
    /// If ETL_THROW_EXCEPTIONS is defined, emits priority_queue_full if
    /// priority queue does not have enough free space.
    /// If ETL_THROW_EXCEPTIONS is defined, emits priority_iterator if the
    /// iterators are reversed.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*********************************************************************
    template <typename TIterator>
    void assign(TIterator first, TIterator last)
    {
#ifdef _DEBUG
      difference_type count = std::distance(first, last);

      if (count < 0)
      {
#ifdef ETL_THROW_EXCEPTIONS
        throw priority_queue_iterator();
#else
        error_handler::error(priority_queue_iterator());
#endif
      }

      if (static_cast<size_t>(count) > MAX_SIZE)
      {
#ifdef ETL_THROW_EXCEPTIONS
        throw priority_queue_full();
#else
        error_handler::error(priority_queue_full());
#endif
      }
#endif

      clear();
      container.assign(first, last);
      std::make_heap(container.begin(), container.end(), TCompare());
      current_size = container.size();
    }

    //*************************************************************************
    /// Removes the oldest value from the back of the priority queue.
    /// Does nothing if the priority queue is already empty.
    //*************************************************************************
    void pop()
    {
      if (!empty())
      {
        // Move largest element to end
	      std::pop_heap(container.begin(), container.end(), TCompare());
        // Actually remove largest element at end
        container.pop_back();
        // Decrement size
        --current_size;
      }
    }

  protected:

    //*************************************************************************
    /// Make this a clone of the supplied priority queue
    //*************************************************************************
    void clone(const ipriority_queue& other)
    {
      assign(other.container.cbegin(), other.container.cend());
    }

    //*************************************************************************
    /// The constructor that is called from derived classes.
    //*************************************************************************
    ipriority_queue(size_type max_size)
      : priority_queue_base(max_size)
    {
    }

  private:

    /// The container specified at instantiation of the priority_queue
    TContainer container;
  };
}

#undef __ETL_IN_IPRIORITY_QUEUE_H__
#endif
