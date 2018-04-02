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

#ifndef __ETL_SPSC_QUEUE_ATOMIC__
#define __ETL_SPSC_QUEUE_ATOMIC__

#include <stddef.h>
#include <stdint.h>

#include "platform.h"
#include "alignment.h"
#include "parameter_type.h"
#include "atomic.h"

#undef ETL_FILE
#define ETL_FILE "45"

namespace etl
{
  //***************************************************************************
  ///\ingroup spsc_queue
  ///\brief This is the base for all spsc_queues that contain a particular type.
  ///\details Normally a reference to this type will be taken from a derived spsc_queue.
  ///\code
  /// etl::spsc_queue_atomic<int, 10> myQueue;
  /// etl::iqueue_atomic<int>& iQueue = myQueue;
  ///\endcode
  /// This queue supports concurrent access by one producer and one consumer.
  /// \tparam T The type of value that the spsc_queue holds.
  //***************************************************************************
  template <typename T>
  class ispsc_queue_atomic
  {
  private:

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
      size_t write_index = write.load(etl::memory_order_relaxed);
      size_t next_index  = get_next_index(write_index, RESERVED);

      if (next_index != read.load(etl::memory_order_acquire))
      {
        ::new (&p_buffer[write_index]) T(value);

        write.store(next_index, etl::memory_order_release);

        return true;
      }
      
      // Queue is full.
      return false;
    }

    //*************************************************************************
    /// Pop a value from the queue.
    //*************************************************************************
    bool pop(reference value)
    {     
      size_t read_index = read.load(etl::memory_order_relaxed);
      
      if (read_index == write.load(etl::memory_order_acquire)) 
      {
        // Queue is empty
        return false;
      }

      size_t next_index = get_next_index(read_index, RESERVED);

      value = buffer[read_index];
      buffer[read_index].~T();

      read.store(next_index, etl::memory_order_release);

      return true;
    }

    //*************************************************************************
    /// Pop a value from the queue and discard.
    //*************************************************************************
    bool pop()
    {
      size_t read_index = read.load(etl::memory_order_relaxed);

      if (read_index == write.load(etl::memory_order_acquire))
      {
        // Queue is empty
        return false;
      }

      size_t next_index = get_next_index(read_index, RESERVED);

      p_buffer[read_index].~T();

      read.store(next_index, etl::memory_order_release);

      return true;
    }

    //*************************************************************************
    /// Pop a value from the queue from an ISR
    //*************************************************************************
    bool pop_from_isr(reference value)
    {
      return pop(value);
    }

    //*************************************************************************
    /// Pop a value from the queue from an ISR, and discard.
    //*************************************************************************
    bool pop_from_isr()
    {
      return pop();
    }

    //*************************************************************************
    /// Clear the queue.
    /// Must be called from thread that pops the queue or when there is no
    /// possibility of concurrent access.
    //*************************************************************************
    void clear()
    {
      while (pop())
      {
        // Do nothing.
      }
    }

    //*************************************************************************
    /// Is the queue empty?
    /// Accurate from the 'pop' thread.
    /// 'Not empty' is a guess from the 'push' thread.
    //*************************************************************************
    bool empty() const
    {
      return read.load(etl::memory_order_acquire) == write.load(etl::memory_order_acquire);
    }

    //*************************************************************************
    /// Is the queue full?
    /// Accurate from the 'push' thread.
    /// 'Not full' is a guess from the 'pop' thread.
    //*************************************************************************
    bool full() const
    {
      size_t next_index = get_next_index(write.load(etl::memory_order_acquire), RESERVED);

      return (next_index == read.load(etl::memory_order_acquire));
    }

    //*************************************************************************
    /// How many items in the queue?
    /// Due to concurrency, this is a guess.
    //*************************************************************************
    size_t size() const
    {
      size_t write_index = write.load(etl::memory_order_acquire);
      size_t read_index  = read.load(etl::memory_order_acquire);
      
      size_t n;

      if (write_index >= read_index)
      {
        n = write_index - read_index;
      }
      else
      {
        n = RESERVED - read_index + write_index - 1;
      }

      return n;
    }

    //*************************************************************************
    /// How much free space available in the queue.
    /// Due to concurrency, this is a guess.
    //*************************************************************************
    size_t available() const
    {
      return RESERVED - size() - 1;
    }

    //*************************************************************************
    /// How many items can the queue hold.
    //*************************************************************************
    size_t capacity() const
    {
      return RESERVED - 1;
    }

    //*************************************************************************
    /// How many items can the queue hold.
    //*************************************************************************
    size_t max_size() const
    {
      return RESERVED - 1;
    }

  protected:

    //*************************************************************************
    /// The constructor that is called from derived classes.
    //*************************************************************************
    ispsc_queue_atomic(T* p_buffer_, size_type max_size_)
      : p_buffer(p_buffer_),
        write(0),
        read(0),
        RESERVED(max_size_)
    {
    }

  private:

    // Disable copy construction and assignment.
    ispsc_queue_atomic(const ispsc_queue_atomic&);
    ispsc_queue_atomic& operator =(const ispsc_queue_atomic&);

    T* p_buffer;              ///< The internal buffer.
    etl::atomic_size_t write; ///< Where to input new data.
    etl::atomic_size_t read;  ///< Where to get the oldest data.
    const size_type RESERVED; ///< The maximum number of items in the queue.

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

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
#if defined(ETL_POLYMORPHIC_SPSC_QUEUE) || defined(ETL_POLYMORPHIC_CONTAINERS)
  public:
    virtual ~ispsc_queue_atomic()
    {
    }
#else
  protected:
    ~ispsc_queue_atomic()
    {
    }
#endif
  };

  //***************************************************************************
  ///\ingroup spsc_queue
  /// A fixed capacity spsc queue.
  /// This queue supports concurrent access by one producer and one consumer.
  /// \tparam T    The type this queue should support.
  /// \tparam SIZE The maximum capacity of the queue.
  //***************************************************************************
  template <typename T, size_t SIZE>
  class spsc_queue_atomic
  {
    typedef etl::ispsc_queue_atomic<T> base_t;

    static const size_t RESERVED_SIZE = SIZE + 1;

  public:

    static const size_t MAX_SIZE = SIZE;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    spsc_queue_atomic()
      : base_t(reinterpret_cast<T*>(&buffer[0]), RESERVED_SIZE)
    {
    }

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~spsc_queue_atomic()
    {
      base_t::clear();
    }

  private:

    /// The uninitialised buffer of T used in the spsc_queue.
    typename etl::aligned_storage<sizeof(T), etl::alignment_of<T>::value>::type buffer[RESERVED_SIZE];
  };
};

#endif