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

#ifndef ETL_SPSC_QUEUE_ATOMIC_INCLUDED
#define ETL_SPSC_QUEUE_ATOMIC_INCLUDED

#include <stddef.h>
#include <stdint.h>

#include "platform.h"
#include "alignment.h"
#include "parameter_type.h"
#include "atomic.h"

#undef ETL_FILE
#define ETL_FILE "47"

namespace etl
{
  class queue_spsc_atomic_base
  {
  public:

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
      size_t read_index = read.load(etl::memory_order_acquire);

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
    ETL_CONSTEXPR size_t capacity() const
    {
      return RESERVED - 1;
    }

    //*************************************************************************
    /// How many items can the queue hold.
    //*************************************************************************
    ETL_CONSTEXPR size_t max_size() const
    {
      return RESERVED - 1;
    }

  protected:

    queue_spsc_atomic_base(size_t reserved_)
      : write(0),
        read(0),
        RESERVED(reserved_)
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

    etl::atomic_size_t write; ///< Where to input new data.
    etl::atomic_size_t read;  ///< Where to get the oldest data.
    const size_t RESERVED;    ///< The maximum number of items in the queue.

  private:

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
#if defined(ETL_POLYMORPHIC_SPSC_QUEUE_ATOMIC) || defined(ETL_POLYMORPHIC_CONTAINERS)
  public:
    virtual ~queue_spsc_atomic_base()
    {
    }
#else
  protected:
    ~queue_spsc_atomic_base()
    {
    }
#endif
  };
  
  //***************************************************************************
  ///\ingroup queue_spsc_atomic
  ///\brief This is the base for all queue_spscs that contain a particular type.
  ///\details Normally a reference to this type will be taken from a derived queue_spsc.
  ///\code
  /// etl::queue_spsc_atomic<int, 10> myQueue;
  /// etl::iqueue_spsc_atomic<int>& iQueue = myQueue;
  ///\endcode
  /// This queue supports concurrent access by one producer and one consumer.
  /// \tparam T The type of value that the queue_spsc_atomic holds.
  //***************************************************************************
  template <typename T>
  class iqueue_spsc_atomic : public queue_spsc_atomic_base
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

      value = p_buffer[read_index];
      p_buffer[read_index].~T();

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

  protected:

    //*************************************************************************
    /// The constructor that is called from derived classes.
    //*************************************************************************
    iqueue_spsc_atomic(T* p_buffer_, size_type reserved_)
      : queue_spsc_atomic_base(reserved_),
        p_buffer(p_buffer_)
    {
    }

  private:

    // Disable copy construction and assignment.
    iqueue_spsc_atomic(const iqueue_spsc_atomic&);
    iqueue_spsc_atomic& operator =(const iqueue_spsc_atomic&);

    T* p_buffer; ///< The internal buffer.
  };

  //***************************************************************************
  ///\ingroup queue_spsc
  /// A fixed capacity spsc queue.
  /// This queue supports concurrent access by one producer and one consumer.
  /// \tparam T    The type this queue should support.
  /// \tparam SIZE The maximum capacity of the queue.
  //***************************************************************************
  template <typename T, size_t SIZE>
  class queue_spsc_atomic : public iqueue_spsc_atomic<T>
  {
    typedef etl::iqueue_spsc_atomic<T> base_t;

    static const size_t RESERVED_SIZE = SIZE + 1;

  public:

    static const size_t MAX_SIZE = SIZE;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    queue_spsc_atomic()
      : base_t(reinterpret_cast<T*>(&buffer[0]), RESERVED_SIZE)
    {
    }

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~queue_spsc_atomic()
    {
      base_t::clear();
    }

  private:

    /// The uninitialised buffer of T used in the queue_spsc.
    typename etl::aligned_storage<sizeof(T), etl::alignment_of<T>::value>::type buffer[RESERVED_SIZE];
  };
};

#endif