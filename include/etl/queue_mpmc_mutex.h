///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2018 jwellbelove

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

#ifndef __ETL_MPMC_QUEUE_MUTEX__
#define __ETL_MPMC_QUEUE_MUTEX__

#include <stddef.h>
#include <stdint.h>

#include "platform.h"
#include "alignment.h"
#include "parameter_type.h"
#include "mutex.h"

#undef ETL_FILE
#define ETL_FILE "48"

namespace etl
{
  class queue_mpmc_mutex_base
  {
  public:

    //*************************************************************************
    /// How many items can the queue hold.
    //*************************************************************************
    ETL_CONSTEXPR size_t capacity() const
    {
      return MAX_SIZE;
    }

    //*************************************************************************
    /// How many items can the queue hold.
    //*************************************************************************
    ETL_CONSTEXPR size_t max_size() const
    {
      return MAX_SIZE;
    }

  protected:

    queue_mpmc_mutex_base(size_t max_size_)
      : write_index(0),
        read_index(0),
        current_size(0),
        MAX_SIZE(max_size_)
    {
    }

    //*************************************************************************
    /// Calculate the next index.
    //*************************************************************************
    static size_t get_next_index(size_t index, size_t maximum)
    {
      ++index;

      if (index == maximum)
      {
        index = 0;
      }

      return index;
    }

    size_t write_index;    ///< Where to input new data.
    size_t read_index;     ///< Where to get the oldest data.
    size_t current_size;   ///< The current size of the queue.
    const size_t MAX_SIZE; ///< The maximum number of items in the queue.

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
#if defined(ETL_POLYMORPHIC_MPMC_QUEUE_MUTEX) || defined(ETL_POLYMORPHIC_CONTAINERS)
  public:
    virtual ~queue_mpmc_mutex_base()
    {
    }
#else
  protected:
    ~queue_mpmc_mutex_base()
    {
    }
#endif
  };

  //***************************************************************************
  ///\ingroup queue_mpmc
  ///\brief This is the base for all queue_mpmc_mutexs that contain a particular type.
  ///\details Normally a reference to this type will be taken from a derived queue_mpmc_mutex.
  ///\code
  /// etl::queue_mpmc_mutex<int, 10> myQueue;
  /// etl::iqueue_mpmc_mutex<int>& iQueue = myQueue;
  ///\endcode
  /// This queue supports concurrent access by one producer and one consumer.
  /// \tparam T The type of value that the queue_mpmc_mutex holds.
  //***************************************************************************
  template <typename T>
  class iqueue_mpmc_mutex : public queue_mpmc_mutex_base
  {
  protected:

    typedef typename etl::parameter_type<T>::type parameter_t;

  public:

    typedef T        value_type;      ///< The type stored in the queue.
    typedef T&       reference;       ///< A reference to the type used in the queue.
    typedef const T& const_reference; ///< A const reference to the type used in the queue.
    typedef size_t   size_type;       ///< The type used for determining the size of the queue.

    //*************************************************************************
    /// Push a value to the queue.
    //*************************************************************************
    bool push(parameter_t value)
    {
      access.lock();

      bool result = push_implementation(value);

      access.unlock();

      return result;
    }

    //*************************************************************************
    /// Pop a value from the queue.
    //*************************************************************************
    bool pop(reference value)
    {
      access.lock();

      bool result = pop_implementation(value);

      access.unlock();

      return result;
    }

    //*************************************************************************
    /// Pop a value from the queue and discard.
    //*************************************************************************
    bool pop()
    {
      access.lock();

      bool result = pop_implementation();

      access.unlock();

      return result;
    }

    //*************************************************************************
    /// Clear the queue.
    //*************************************************************************
    void clear()
    {
      access.lock();

      while (pop_implementation())
      {
        // Do nothing.
      }

      access.unlock();
    }

    //*************************************************************************
    /// Is the queue empty?
    //*************************************************************************
    bool empty() const
    {
      access.lock();

      size_t result = (current_size == 0);

      access.unlock();

      return result;
    }

    //*************************************************************************
    /// Is the queue full?
    //*************************************************************************
    bool full() const
    {
      access.lock();

      size_t result = (current_size == MAX_SIZE);

      access.unlock();

      return result;
    }

    //*************************************************************************
    /// How many items in the queue?
    //*************************************************************************
    size_t size() const
    {
      access.lock();

      size_t result = current_size;

      access.unlock();

      return result;
    }

    //*************************************************************************
    /// How much free space available in the queue.
    //*************************************************************************
    size_t available() const
    {
      access.lock();

      size_t result = MAX_SIZE - current_size;

      access.unlock();

      return result;
    }

  protected:

    //*************************************************************************
    /// The constructor that is called from derived classes.
    //*************************************************************************
    iqueue_mpmc_mutex(T* p_buffer_, size_type max_size_)
      : queue_mpmc_mutex_base(max_size_),
        p_buffer(p_buffer_)
    {
    }

  private:

    //*************************************************************************
    /// Push a value to the queue.
    //*************************************************************************
    bool push_implementation(parameter_t value)
    {
      if (current_size != MAX_SIZE)
      {
        ::new (&p_buffer[write_index]) T(value);

        write_index = get_next_index(write_index, MAX_SIZE);

        ++current_size;

        return true;
      }

      // Queue is full.
      return false;
    }

    //*************************************************************************
    /// Pop a value from the queue.
    //*************************************************************************
    bool pop_implementation(reference value)
    {
      if (current_size == 0)
      {
        // Queue is empty
        return false;
      }

      value = p_buffer[read_index];
      p_buffer[read_index].~T();

      read_index = get_next_index(read_index, MAX_SIZE);;

      --current_size;

      return true;
    }

    //*************************************************************************
    /// Pop a value from the queue and discard.
    //*************************************************************************
    bool pop_implementation()
    {
      if (current_size == 0)
      {
        // Queue is empty
        return false;
      }

      p_buffer[read_index].~T();

      read_index = get_next_index(read_index, MAX_SIZE);

      --current_size;

      return true;
    }

    // Disable copy construction and assignment.
    iqueue_mpmc_mutex(const iqueue_mpmc_mutex&);
    iqueue_mpmc_mutex& operator =(const iqueue_mpmc_mutex&);

    T* p_buffer; ///< The internal buffer.

    mutable etl::mutex access; ///< The object that locks/unlocks access.
  };

  //***************************************************************************
  ///\ingroup queue_mpmc
  /// A fixed capacity mpmc queue.
  /// This queue supports concurrent access by one producer and one consumer.
  /// \tparam T      The type this queue should support.
  /// \tparam SIZE   The maximum capacity of the queue.
  //***************************************************************************
  template <typename T, size_t SIZE>
  class queue_mpmc_mutex : public etl::iqueue_mpmc_mutex<T>
  {
    typedef etl::iqueue_mpmc_mutex<T> base_t;

  public:

    static const size_t MAX_SIZE = SIZE;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    queue_mpmc_mutex()
      : base_t(reinterpret_cast<T*>(&buffer[0]), MAX_SIZE)
    {
    }

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~queue_mpmc_mutex()
    {
      base_t::clear();
    }

  private:

    queue_mpmc_mutex(const queue_mpmc_mutex&);
    queue_mpmc_mutex& operator = (const queue_mpmc_mutex&);

    /// The uninitialised buffer of T used in the queue_mpmc_mutex.
    typename etl::aligned_storage<sizeof(T), etl::alignment_of<T>::value>::type buffer[MAX_SIZE];
  };
};

#endif
