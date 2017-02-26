///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

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

#include <stddef.h>
#include <algorithm>

#include "platform.h"
#include "type_traits.h"
#include "parameter_type.h"
#include "error_handler.h"
#include "exception.h"

#undef ETL_FILE
#define ETL_FILE "12"

namespace etl
{
  //***************************************************************************
  /// The base class for priority_queue exceptions.
  ///\ingroup queue
  //***************************************************************************
  class priority_queue_exception : public exception
  {
  public:

    priority_queue_exception(string_type what, string_type file_name, numeric_type line_number)
      : exception(what, file_name, line_number)
    {
    }
  };

  //***************************************************************************
  /// The exception thrown when the queue is full.
  ///\ingroup queue
  //***************************************************************************
  class priority_queue_full : public priority_queue_exception
  {
  public:

    priority_queue_full(string_type file_name, numeric_type line_number)
      : priority_queue_exception(ETL_ERROR_TEXT("priority_queue:full", ETL_FILE"A"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  /// The priority queue iterator exception on reversed iterators
  ///\ingroup queue
  //***************************************************************************
  class priority_queue_iterator : public priority_queue_exception
  {
  public:

    priority_queue_iterator(string_type file_name, numeric_type line_number)
      : priority_queue_exception(ETL_ERROR_TEXT("priority_queue:iterator", ETL_FILE"B"), file_name, line_number)
    {
    }
  };
  
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
  class ipriority_queue
  {
  public:

    typedef T                     value_type;         ///< The type stored in the queue.
    typedef TContainer            container_type;     ///< The container type used for priority queue.
    typedef TCompare              compare_type;       ///< The comparison type.
    typedef T&                    reference;          ///< A reference to the type used in the queue.
    typedef const T&              const_reference;    ///< A const reference to the type used in the queue.
    typedef typename TContainer::size_type size_type; ///< The type used for determining the size of the queue.
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
    /// If asserts or exceptions are enabled, throws an etl::priority_queue_full
    /// is the priority queue is already full, otherwise does nothing if full.
    ///\param value The value to push to the queue.
    //*************************************************************************
    void push(parameter_t value)
    {
      ETL_ASSERT(!full(), ETL_ERROR(priority_queue_full));

      // Put element at end
      container.push_back(value);
      // Make elements in container into heap
      std::push_heap(container.begin(), container.end(), TCompare());
    }

    //*************************************************************************
    /// Assigns values to the priority queue.
    /// If asserts or exceptions are enabled, emits priority_queue_full if
    /// priority queue does not have enough free space.
    /// If asserts or exceptions are enabled, emits priority_iterator if the
    /// iterators are reversed.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*************************************************************************
    template <typename TIterator>
    void assign(TIterator first, TIterator last)
    {
#if defined(ETL_DEBUG)
      difference_type count = std::distance(first, last);
      ETL_ASSERT(count >= 0, ETL_ERROR(priority_queue_iterator));
      ETL_ASSERT(static_cast<size_t>(count) <= max_size(), ETL_ERROR(priority_queue_full));
#endif

      clear();
      container.assign(first, last);
      std::make_heap(container.begin(), container.end(), TCompare());
    }

    //*************************************************************************
    /// Removes the oldest value from the back of the priority queue.
    /// Does nothing if the priority queue is already empty.
    //*************************************************************************
    void pop()
    {
        // Move largest element to end
        std::pop_heap(container.begin(), container.end(), TCompare());
        // Actually remove largest element at end
        container.pop_back();
    }

    //*************************************************************************
    /// Returns the current number of items in the priority queue.
    //*************************************************************************
    size_type size() const
    {
      return container.size();
    }

    //*************************************************************************
    /// Returns the maximum number of items that can be queued.
    //*************************************************************************
    size_type max_size() const
    {
      return container.max_size();
    }

    //*************************************************************************
    /// Checks to see if the priority queue is empty.
    /// \return <b>true</b> if the queue is empty, otherwise <b>false</b>
    //*************************************************************************
    bool empty() const
    {
      return container.empty();
    }

    //*************************************************************************
    /// Checks to see if the priority queue is full.
    /// \return <b>true</b> if the priority queue is full, otherwise <b>false</b>
    //*************************************************************************
    bool full() const
    {
      return container.size() == container.max_size();
    }

    //*************************************************************************
    /// Returns the remaining capacity.
    ///\return The remaining capacity.
    //*************************************************************************
    size_t available() const
    {
      return container.max_size() - container.size();
    }

    //*************************************************************************
    /// Clears the queue to the empty state.
    //*************************************************************************
    void clear()
    {
      container.clear();
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
    ipriority_queue()
    {
    }

  private:

    // Disable copy construction.
    ipriority_queue(const ipriority_queue&);

    /// The container specified at instantiation of the priority_queue
    TContainer container;
  };
}

#undef ETL_FILE
#endif
