///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2015 John Wellbelove, rlindeman

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

#ifndef ETL_PRIORITY_QUEUE_INCLUDED
#define ETL_PRIORITY_QUEUE_INCLUDED

#include "platform.h"
#include "algorithm.h"
#include "utility.h"
#include "functional.h"
#include "iterator.h"
#include "vector.h"
#include "type_traits.h"
#include "parameter_type.h"
#include "error_handler.h"
#include "exception.h"

#include <stddef.h>

//*****************************************************************************
///\defgroup queue queue
/// A priority queue with the capacity defined at compile time,
/// written in the STL style.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //***************************************************************************
  /// The base class for priority_queue exceptions.
  ///\ingroup queue
  //***************************************************************************
  class priority_queue_exception : public exception
  {
  public:

    priority_queue_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// The exception thrown when the queue is full.
  ///\ingroup queue
  //***************************************************************************
  class priority_queue_full : public etl::priority_queue_exception
  {
  public:

    priority_queue_full(string_type file_name_, numeric_type line_number_)
      : priority_queue_exception(ETL_ERROR_TEXT("priority_queue:full", ETL_PRIORITY_QUEUE_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// The priority queue iterator exception on reversed iterators
  ///\ingroup queue
  //***************************************************************************
  class priority_queue_iterator : public etl::priority_queue_exception
  {
  public:

    priority_queue_iterator(string_type file_name_, numeric_type line_number_)
      : priority_queue_exception(ETL_ERROR_TEXT("priority_queue:iterator", ETL_PRIORITY_QUEUE_FILE_ID"B"), file_name_, line_number_)
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
  template <typename T, typename TContainer, typename TCompare = etl::less<T> >
  class ipriority_queue
  {
  public:

    typedef T                     value_type;         ///< The type stored in the queue.
    typedef TContainer            container_type;     ///< The container type used for priority queue.
    typedef TCompare              compare_type;       ///< The comparison type.
    typedef T&                    reference;          ///< A reference to the type used in the queue.
    typedef const T&              const_reference;    ///< A const reference to the type used in the queue.
#if ETL_USING_CPP11
    typedef T&&                   rvalue_reference;   ///< An rvalue reference to the type used in the queue.
#endif
    typedef typename TContainer::size_type size_type; ///< The type used for determining the size of the queue.
    typedef typename etl::iterator_traits<typename TContainer::iterator>::difference_type difference_type;

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
    /// is the priority queue is already full.
    ///\param value The value to push to the queue.
    //*************************************************************************
    void push(const_reference value)
    {
      ETL_ASSERT(!full(), ETL_ERROR(etl::priority_queue_full));

      // Put element at end
      container.push_back(value);
      // Make elements in container into heap
      etl::push_heap(container.begin(), container.end(), compare);
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Moves a value to the queue.
    /// If asserts or exceptions are enabled, throws an etl::priority_queue_full
    /// is the priority queue is already full.
    ///\param value The value to push to the queue.
    //*************************************************************************
    void push(rvalue_reference value)
    {
      ETL_ASSERT(!full(), ETL_ERROR(etl::priority_queue_full));

      // Put element at end
      container.push_back(etl::move(value));
      // Make elements in container into heap
      etl::push_heap(container.begin(), container.end(), compare);
    }
#endif

#if ETL_USING_CPP11 && ETL_NOT_USING_STLPORT && !defined(ETL_PRIORITY_QUEUE_FORCE_CPP03_IMPLEMENTATION)
    //*************************************************************************
    /// Emplaces a value to the queue.
    /// If asserts or exceptions are enabled, throws an etl::priority_queue_full
    /// is the priority queue is already full.
    ///\param value The value to push to the queue.
    //*************************************************************************
    template <typename ... Args>
    void emplace(Args && ... args)
    {
      ETL_ASSERT(!full(), ETL_ERROR(etl::priority_queue_full));

      // Put element at end
      container.emplace_back(etl::forward<Args>(args)...);
      // Make elements in container into heap
      etl::push_heap(container.begin(), container.end(), compare);
    }
#else
    //*************************************************************************
    /// Emplaces a value to the queue.
    /// If asserts or exceptions are enabled, throws an etl::priority_queue_full
    /// is the priority queue is already full.
    ///\param value The value to push to the queue.
    //*************************************************************************
    void emplace()
    {
      ETL_ASSERT(!full(), ETL_ERROR(etl::priority_queue_full));

      // Put element at end
      container.emplace_back();
      // Make elements in container into heap
      etl::push_heap(container.begin(), container.end(), compare);
    }

    //*************************************************************************
    /// Emplaces a value to the queue.
    /// If asserts or exceptions are enabled, throws an etl::priority_queue_full
    /// is the priority queue is already full.
    ///\param value The value to push to the queue.
    //*************************************************************************
    template <typename T1>
    void emplace(const T1& value1)
    {
      ETL_ASSERT(!full(), ETL_ERROR(etl::priority_queue_full));

      // Put element at end
      container.emplace_back(value1);
      // Make elements in container into heap
      etl::push_heap(container.begin(), container.end(), compare);
    }

    //*************************************************************************
    /// Emplaces a value to the queue.
    /// If asserts or exceptions are enabled, throws an etl::priority_queue_full
    /// is the priority queue is already full.
    ///\param value The value to push to the queue.
    //*************************************************************************
    template <typename T1, typename T2>
    void emplace(const T1& value1, const T2& value2)
    {
      ETL_ASSERT(!full(), ETL_ERROR(etl::priority_queue_full));

      // Put element at end
      container.emplace_back(value1, value2);
      // Make elements in container into heap
      etl::push_heap(container.begin(), container.end(), compare);
    }

    //*************************************************************************
    /// Emplaces a value to the queue.
    /// If asserts or exceptions are enabled, throws an etl::priority_queue_full
    /// is the priority queue is already full.
    ///\param value The value to push to the queue.
    //*************************************************************************
    template <typename T1, typename T2, typename T3>
    void emplace(const T1& value1, const T2& value2, const T3& value3)
    {
      ETL_ASSERT(!full(), ETL_ERROR(etl::priority_queue_full));

      // Put element at end
      container.emplace_back(value1, value2, value3);
      // Make elements in container into heap
      etl::push_heap(container.begin(), container.end(), compare);
    }

    //*************************************************************************
    /// Emplaces a value to the queue.
    /// If asserts or exceptions are enabled, throws an etl::priority_queue_full
    /// is the priority queue is already full.
    ///\param value The value to push to the queue.
    //*************************************************************************
    template <typename T1, typename T2, typename T3, typename T4>
    void emplace(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
    {
      ETL_ASSERT(!full(), ETL_ERROR(etl::priority_queue_full));

      // Put element at end
      container.emplace_back(value1, value2, value3, value4);
      // Make elements in container into heap
      etl::push_heap(container.begin(), container.end(), compare);
    }
#endif

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
#if ETL_IS_DEBUG_BUILD
      difference_type d = etl::distance(first, last);
      ETL_ASSERT(d >= 0, ETL_ERROR(etl::priority_queue_iterator));
      ETL_ASSERT(static_cast<size_t>(d) <= max_size(), ETL_ERROR(etl::priority_queue_full));
#endif

      clear();
      container.assign(first, last);
      etl::make_heap(container.begin(), container.end(), compare);
    }

    //*************************************************************************
    /// Removes the oldest value from the back of the priority queue.
    /// Does nothing if the priority queue is already empty.
    //*************************************************************************
    void pop()
    {
      // Move largest element to end
      etl::pop_heap(container.begin(), container.end(), compare);
      // Actually remove largest element at end
      container.pop_back();
    }

    //*************************************************************************
    /// Gets the highest priority value in the priority queue
    /// and assigns it to destination and removes it from the queue.
    //*************************************************************************
    void pop_into(reference destination)
    {
      destination = ETL_MOVE(top());
      pop();
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
    size_type available() const
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

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    ipriority_queue& operator = (const ipriority_queue& rhs)
    {
      if (&rhs != this)
      {
        clone(rhs);
      }

      return *this;
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Move assignment operator.
    //*************************************************************************
    ipriority_queue& operator = (ipriority_queue&& rhs)
    {
      if (&rhs != this)
      {
        clear();
        move(etl::move(rhs));
      }

      return *this;
    }
#endif

  protected:

    //*************************************************************************
    /// Make this a clone of the supplied priority queue
    //*************************************************************************
    void clone(const ipriority_queue& other)
    {
      assign(other.container.cbegin(), other.container.cend());
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Make this a moved version of the supplied priority queue
    //*************************************************************************
    void move(ipriority_queue&& other)
    {
      while (!other.empty())
      {
        push(etl::move(other.top()));
        other.pop();
      }
    }
#endif

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

    TCompare compare;
  };

  //***************************************************************************
  ///\ingroup priority_queue
  /// A fixed capacity priority queue.
  /// This queue does not support concurrent access by different threads.
  /// \tparam T    The type this queue should support.
  /// \tparam SIZE The maximum capacity of the queue.
  //***************************************************************************
  template <typename T, const size_t SIZE, typename TContainer = etl::vector<T, SIZE>, typename TCompare = etl::less<typename TContainer::value_type> >
  class priority_queue : public etl::ipriority_queue<T, TContainer, TCompare>
  {
  public:

    typedef typename TContainer::size_type size_type;
    typedef TContainer                     container_type;

    static ETL_CONSTANT size_type MAX_SIZE = size_type(SIZE);

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    priority_queue()
      : etl::ipriority_queue<T, TContainer, TCompare>()
    {
    }

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    priority_queue(const priority_queue& rhs)
      : etl::ipriority_queue<T, TContainer, TCompare>()
    {
      etl::ipriority_queue<T, TContainer, TCompare>::clone(rhs);
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Move constructor
    //*************************************************************************
    priority_queue(priority_queue&& rhs)
      : etl::ipriority_queue<T, TContainer, TCompare>()
    {
      etl::ipriority_queue<T, TContainer, TCompare>::move(etl::move(rhs));
    }
#endif

    //*************************************************************************
    /// Constructor, from an iterator range.
    ///\tparam TIterator The iterator type.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*************************************************************************
    template <typename TIterator>
    priority_queue(TIterator first, TIterator last)
      : etl::ipriority_queue<T, TContainer, TCompare>()
    {
      etl::ipriority_queue<T, TContainer, TCompare>::assign(first, last);
    }

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~priority_queue()
    {
      etl::ipriority_queue<T, TContainer, TCompare>::clear();
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    priority_queue& operator = (const priority_queue& rhs)
    {
      if (&rhs != this)
      {
        etl::ipriority_queue<T, TContainer, TCompare>::clone(rhs);
      }

      return *this;
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Move assignment operator.
    //*************************************************************************
    priority_queue& operator = (priority_queue&& rhs)
    {
      if (&rhs != this)
      {
        etl::ipriority_queue<T, TContainer, TCompare>::clear();
        etl::ipriority_queue<T, TContainer, TCompare>::move(etl::move(rhs));
      }

      return *this;
    }
#endif
  };

  template <typename T, const size_t SIZE, typename TContainer, typename TCompare>
  ETL_CONSTANT typename priority_queue<T, SIZE, TContainer, TCompare>::size_type priority_queue<T, SIZE, TContainer, TCompare>::MAX_SIZE;
}

#endif
