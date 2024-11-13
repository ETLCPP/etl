///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2018 John Wellbelove

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

#include "platform.h"
#include "alignment.h"
#include "parameter_type.h"
#include "atomic.h"
#include "memory_model.h"
#include "integral_limits.h"
#include "utility.h"
#include "placement_new.h"

#include <stddef.h>
#include <stdint.h>

#if ETL_HAS_ATOMIC

namespace etl
{
  template <size_t Memory_Model = etl::memory_model::MEMORY_MODEL_LARGE>
  class queue_spsc_atomic_base
  {
  public:

    /// The type used for determining the size of queue.
    typedef typename etl::size_type_lookup<Memory_Model>::type size_type;

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
      size_type next_index = get_next_index(write.load(etl::memory_order_acquire), Reserved);

      return (next_index == read.load(etl::memory_order_acquire));
    }

    //*************************************************************************
    /// How many items in the queue?
    /// Due to concurrency, this is a guess.
    //*************************************************************************
    size_type size() const
    {
      size_type write_index = write.load(etl::memory_order_acquire);
      size_type read_index = read.load(etl::memory_order_acquire);

      size_type n;

      if (write_index >= read_index)
      {
        n = write_index - read_index;
      }
      else
      {
        n = Reserved - read_index + write_index;
      }

      return n;
    }

    //*************************************************************************
    /// How much free space available in the queue.
    /// Due to concurrency, this is a guess.
    //*************************************************************************
    size_type available() const
    {
      return Reserved - size() - 1;
    }

    //*************************************************************************
    /// How many items can the queue hold.
    //*************************************************************************
    size_type capacity() const
    {
      return Reserved - 1;
    }

    //*************************************************************************
    /// How many items can the queue hold.
    //*************************************************************************
    size_type max_size() const
    {
      return Reserved - 1;
    }

  protected:

    queue_spsc_atomic_base(size_type reserved_)
      : write(0),
        read(0),
        Reserved(reserved_)
    {
    }

    //*************************************************************************
    /// Calculate the next index.
    //*************************************************************************
    static size_type get_next_index(size_type index, size_type maximum)
    {
      ++index;

      if (index == maximum) ETL_UNLIKELY
      {
        index = 0;
      }

      return index;
    }

    etl::atomic<size_type> write; ///< Where to input new data.
    etl::atomic<size_type> read;  ///< Where to get the oldest data.
    const size_type Reserved;     ///< The maximum number of items in the queue.

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
  template <typename T, const size_t Memory_Model = etl::memory_model::MEMORY_MODEL_LARGE>
  class iqueue_spsc_atomic : public queue_spsc_atomic_base<Memory_Model>
  {
  private:

    typedef typename etl::queue_spsc_atomic_base<Memory_Model> base_t;

  public:

    typedef T                          value_type;      ///< The type stored in the queue.
    typedef T&                         reference;       ///< A reference to the type used in the queue.
    typedef const T&                   const_reference; ///< A const reference to the type used in the queue.
#if ETL_USING_CPP11
    typedef T&&                        rvalue_reference;///< An rvalue_reference to the type used in the queue.
#endif
    typedef typename base_t::size_type size_type;       ///< The type used for determining the size of the queue.

    using base_t::write;
    using base_t::read;
    using base_t::Reserved;
    using base_t::get_next_index;

    //*************************************************************************
    /// Push a value to the queue.
    //*************************************************************************
    bool push(const_reference value)
    {
      size_type write_index = write.load(etl::memory_order_relaxed);
      size_type next_index  = get_next_index(write_index, Reserved);

      if (next_index != read.load(etl::memory_order_acquire))
      {
        ::new (&p_buffer[write_index]) T(value);

        write.store(next_index, etl::memory_order_release);

        return true;
      }

      // Queue is full.
      return false;
    }

#if ETL_USING_CPP11 && ETL_NOT_USING_STLPORT && !defined(ETL_QUEUE_ATOMIC_FORCE_CPP03_IMPLEMENTATION)
    //*************************************************************************
    /// Push a value to the queue.
    //*************************************************************************
    bool push(rvalue_reference value)
    {
      size_type write_index = write.load(etl::memory_order_relaxed);
      size_type next_index = get_next_index(write_index, Reserved);

      if (next_index != read.load(etl::memory_order_acquire))
      {
        ::new (&p_buffer[write_index]) T(etl::move(value));

        write.store(next_index, etl::memory_order_release);

        return true;
      }

      // Queue is full.
      return false;
    }
#endif

#if ETL_USING_CPP11 && ETL_NOT_USING_STLPORT && !defined(ETL_QUEUE_ATOMIC_FORCE_CPP03_IMPLEMENTATION)
    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    /// If asserts or exceptions are enabled, throws an etl::queue_full if the queue if already full.
    //*************************************************************************
    template <typename ... Args>
    bool emplace(Args&&... args)
    {
      size_type write_index = write.load(etl::memory_order_relaxed);
      size_type next_index  = get_next_index(write_index, Reserved);

      if (next_index != read.load(etl::memory_order_acquire))
      {
        ::new (&p_buffer[write_index]) T(etl::forward<Args>(args)...);

        write.store(next_index, etl::memory_order_release);

        return true;
      }

      // Queue is full.
      return false;
    }
#else
    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    /// If asserts or exceptions are enabled, throws an etl::queue_full if the queue if already full.
    //*************************************************************************
    bool emplace()
    {
      size_type write_index = write.load(etl::memory_order_relaxed);
      size_type next_index = get_next_index(write_index, Reserved);

      if (next_index != read.load(etl::memory_order_acquire))
      {
        ::new (&p_buffer[write_index]) T();

        write.store(next_index, etl::memory_order_release);

        return true;
      }

      // Queue is full.
      return false;
    }

    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    /// If asserts or exceptions are enabled, throws an etl::queue_full if the queue if already full.
    //*************************************************************************
    template <typename T1>
    bool emplace(const T1& value1)
    {
      size_type write_index = write.load(etl::memory_order_relaxed);
      size_type next_index  = get_next_index(write_index, Reserved);

      if (next_index != read.load(etl::memory_order_acquire))
      {
        ::new (&p_buffer[write_index]) T(value1);

        write.store(next_index, etl::memory_order_release);

        return true;
      }

      // Queue is full.
      return false;
    }

    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    /// If asserts or exceptions are enabled, throws an etl::queue_full if the queue if already full.
    //*************************************************************************
    template <typename T1, typename T2>
    bool emplace(const T1& value1, const T2& value2)
    {
      size_type write_index = write.load(etl::memory_order_relaxed);
      size_type next_index  = get_next_index(write_index, Reserved);

      if (next_index != read.load(etl::memory_order_acquire))
      {
        ::new (&p_buffer[write_index]) T(value1, value2);

        write.store(next_index, etl::memory_order_release);

        return true;
      }

      // Queue is full.
      return false;
    }

    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    /// If asserts or exceptions are enabled, throws an etl::queue_full if the queue if already full.
    //*************************************************************************
    template <typename T1, typename T2, typename T3>
    bool emplace(const T1& value1, const T2& value2, const T3& value3)
    {
      size_type write_index = write.load(etl::memory_order_relaxed);
      size_type next_index  = get_next_index(write_index, Reserved);

      if (next_index != read.load(etl::memory_order_acquire))
      {
        ::new (&p_buffer[write_index]) T(value1, value2, value3);

        write.store(next_index, etl::memory_order_release);

        return true;
      }

      // Queue is full.
      return false;
    }

    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    /// If asserts or exceptions are enabled, throws an etl::queue_full if the queue if already full.
    //*************************************************************************
    template <typename T1, typename T2, typename T3, typename T4>
    bool emplace(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
    {
      size_type write_index = write.load(etl::memory_order_relaxed);
      size_type next_index  = get_next_index(write_index, Reserved);

      if (next_index != read.load(etl::memory_order_acquire))
      {
        ::new (&p_buffer[write_index]) T(value1, value2, value3, value4);

        write.store(next_index, etl::memory_order_release);

        return true;
      }

      // Queue is full.
      return false;
    }
#endif

    //*************************************************************************
    /// Peek the next value in the queue without removing it.
    //*************************************************************************
    bool front(reference value)
    {
      size_type read_index = read.load(etl::memory_order_relaxed);

      if (read_index == write.load(etl::memory_order_acquire))
      {
        // Queue is empty
        return false;
      }

      value = p_buffer[read_index];

      return true;
    }

    //*************************************************************************
    /// Pop a value from the queue.
    //*************************************************************************
    bool pop(reference value)
    {
      size_type read_index = read.load(etl::memory_order_relaxed);

      if (read_index == write.load(etl::memory_order_acquire))
      {
        // Queue is empty
        return false;
      }

      size_type next_index = get_next_index(read_index, Reserved);

#if ETL_USING_CPP11 && ETL_NOT_USING_STLPORT && !defined(ETL_QUEUE_LOCKABLE_FORCE_CPP03_IMPLEMENTATION)
      value = etl::move(p_buffer[read_index]);
#else
      value = p_buffer[read_index];
#endif

      p_buffer[read_index].~T();

      read.store(next_index, etl::memory_order_release);

      return true;
    }

    //*************************************************************************
    /// Pop a value from the queue and discard.
    //*************************************************************************
    bool pop()
    {
      size_type read_index = read.load(etl::memory_order_relaxed);

      if (read_index == write.load(etl::memory_order_acquire))
      {
        // Queue is empty
        return false;
      }

      size_type next_index = get_next_index(read_index, Reserved);

      p_buffer[read_index].~T();

      read.store(next_index, etl::memory_order_release);

      return true;
    }

    //*************************************************************************
    /// Peek a value from the front of the queue.
    //*************************************************************************
    reference front()
    {
      size_type read_index = read.load(etl::memory_order_relaxed);

      return p_buffer[read_index];
    }

    //*************************************************************************
    /// Peek a value from the front of the queue.
    //*************************************************************************
    const_reference front() const
    {
      size_type read_index = read.load(etl::memory_order_relaxed);

      return p_buffer[read_index];
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
      : base_t(reserved_),
        p_buffer(p_buffer_)
    {
    }

  private:

    // Disable copy construction and assignment.
    iqueue_spsc_atomic(const iqueue_spsc_atomic&) ETL_DELETE;
    iqueue_spsc_atomic& operator =(const iqueue_spsc_atomic&) ETL_DELETE;

#if ETL_USING_CPP11
    iqueue_spsc_atomic(iqueue_spsc_atomic&&) = delete;
    iqueue_spsc_atomic& operator =(iqueue_spsc_atomic&&) = delete;
#endif

    T* p_buffer; ///< The internal buffer.
  };

  //***************************************************************************
  ///\ingroup queue_spsc
  /// A fixed capacity spsc queue.
  /// This queue supports concurrent access by one producer and one consumer.
  /// \tparam T            The type this queue should support.
  /// \tparam Size         The maximum capacity of the queue.
  /// \tparam Memory_Model The memory model for the queue. Determines the type of the internal counter variables.
  //***************************************************************************
  template <typename T, size_t Size, const size_t Memory_Model = etl::memory_model::MEMORY_MODEL_LARGE>
  class queue_spsc_atomic : public iqueue_spsc_atomic<T, Memory_Model>
  {
  private:

    typedef typename etl::iqueue_spsc_atomic<T, Memory_Model> base_t;

  public:

    typedef typename base_t::size_type size_type;

  private:

    static ETL_CONSTANT size_type Reserved_Size = size_type(Size + 1);

  public:

    ETL_STATIC_ASSERT((Size <= (etl::integral_limits<size_type>::max - 1)), "Size too large for memory model");

    static ETL_CONSTANT size_type MAX_SIZE = size_type(Size);

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    queue_spsc_atomic()
      : base_t(reinterpret_cast<T*>(&buffer[0]), Reserved_Size)
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
    typename etl::aligned_storage<sizeof(T), etl::alignment_of<T>::value>::type buffer[Reserved_Size];
  };

  template <typename T, size_t Size, const size_t Memory_Model>
  ETL_CONSTANT typename queue_spsc_atomic<T, Size, Memory_Model>::size_type queue_spsc_atomic<T, Size, Memory_Model>::MAX_SIZE;
}

#endif

#endif
