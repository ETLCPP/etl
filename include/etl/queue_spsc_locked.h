///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2019 John Wellbelove

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

#ifndef ETL_SPSC_QUEUE_LOCKED_INCLUDED
#define ETL_SPSC_QUEUE_LOCKED_INCLUDED

#include "platform.h"
#include "memory.h"
#include "parameter_type.h"
#include "memory_model.h"
#include "integral_limits.h"
#include "function.h"
#include "utility.h"
#include "placement_new.h"

#include <stddef.h>
#include <stdint.h>

namespace etl
{
  template <size_t MEMORY_MODEL = etl::memory_model::MEMORY_MODEL_LARGE>
  class iqueue_spsc_locked_base
  {
  public:

    /// The type used for determining the size of queue.
    typedef typename etl::size_type_lookup<MEMORY_MODEL>::type size_type;

    //*************************************************************************
    /// How much free space available in the queue.
    //*************************************************************************
    size_type available_from_unlocked() const
    {
      return available_implementation();
    }

    //*************************************************************************
    /// Is the queue full?
    //*************************************************************************
    bool full_from_unlocked() const
    {
      return full_implementation();
    }

    //*************************************************************************
    /// How many items in the queue?
    //*************************************************************************
    size_type size_from_unlocked() const
    {
      return size_implementation();
    }

    //*************************************************************************
    /// Is the queue empty?
    //*************************************************************************
    bool empty_from_unlocked() const
    {
      return empty_implementation();
    }

    //*************************************************************************
    /// How many items can the queue hold.
    //*************************************************************************
    size_type capacity() const
    {
      return MAX_SIZE;
    }

    //*************************************************************************
    /// How many items can the queue hold.
    //*************************************************************************
    size_type max_size() const
    {
      return MAX_SIZE;
    }

  protected:

    iqueue_spsc_locked_base(size_type max_size_)
      : write_index(0),
        read_index(0),
        current_size(0),
        MAX_SIZE(max_size_)
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

    size_type write_index;    ///< Where to input new data.
    size_type read_index;     ///< Where to get the oldest data.
    size_type current_size;   ///< The current size of the queue.
    const size_type MAX_SIZE; ///< The maximum number of items in the queue.

  protected:

    //*************************************************************************
    /// How much free space available in the queue.
    //*************************************************************************
    size_type available_implementation() const
    {
      return MAX_SIZE - current_size;
    }

    //*************************************************************************
    /// Is the queue full?
    //*************************************************************************
    bool full_implementation() const
    {
      return (current_size == MAX_SIZE);
    }

    //*************************************************************************
    /// How many items in the queue?
    //*************************************************************************
    size_type size_implementation() const
    {
      return current_size;
    }

    //*************************************************************************
    /// Is the queue empty?
    //*************************************************************************
    bool empty_implementation() const
    {
      return (current_size == 0);
    }

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
#if defined(ETL_POLYMORPHIC_SPSC_QUEUE_ISR) || defined(ETL_POLYMORPHIC_CONTAINERS)
  public:
    virtual ~iqueue_spsc_locked_base()
    {
    }
#else
  protected:
    ~iqueue_spsc_locked_base()
    {
    }
#endif
  };

  //***************************************************************************
  ///\ingroup queue_spsc
  ///\brief This is the base for all queue_spsc_locked that contain a particular type.
  ///\details Normally a reference to this type will be taken from a derived queue_spsc_locked.
  /// This queue supports concurrent access by one producer and one consumer.
  /// \tparam T The type of value that the queue_spsc_locked holds.
  //***************************************************************************
  template <typename T, const size_t MEMORY_MODEL = etl::memory_model::MEMORY_MODEL_LARGE>
  class iqueue_spsc_locked : public iqueue_spsc_locked_base<MEMORY_MODEL>
  {
  private:

    typedef iqueue_spsc_locked_base<MEMORY_MODEL> base_t;

  public:

    typedef T                          value_type;       ///< The type stored in the queue.
    typedef T&                         reference;        ///< A reference to the type used in the queue.
    typedef const T&                   const_reference;  ///< A const reference to the type used in the queue.
#if ETL_USING_CPP11
    typedef T&&                        rvalue_reference; ///< An rvalue reference to the type used in the queue.
#endif
    typedef typename base_t::size_type size_type;        ///< The type used for determining the size of the queue.

    //*************************************************************************
    /// Push a value to the queue.
    //*************************************************************************
    bool push_from_unlocked(const_reference value)
    {
      return push_implementation(value);
    }

    //*************************************************************************
    /// Push a value to the queue.
    //*************************************************************************
    bool push(const_reference value)
    {
      lock();

      bool result = push_implementation(value);

      unlock();

      return result;
    }

#if ETL_USING_CPP11 && ETL_NOT_USING_STLPORT && !defined(ETL_QUEUE_LOCKED_FORCE_CPP03_IMPLEMENTATION)
    //*************************************************************************
    /// Push a value to the queue.
    /// Unlocked.
    //*************************************************************************
    bool push_from_unlocked(rvalue_reference value)
    {
      return push_implementation(etl::move(value));
    }

    //*************************************************************************
    /// Push a value to the queue.
    /// Locked.
    //*************************************************************************
    bool push(rvalue_reference value)
    {
      lock();

      bool result = push_implementation(etl::move(value));

      unlock();

      return result;
    }
#endif

#if ETL_USING_CPP11 && ETL_NOT_USING_STLPORT && !defined(ETL_QUEUE_LOCKED_FORCE_CPP03_IMPLEMENTATION)
    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    /// Unlocked.
    //*************************************************************************
    template <typename ... Args>
    bool emplace_from_unlocked(Args&&... args)
    {
      return emplace_implementation(etl::forward<Args>(args)...);
    }
    
    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    /// Locked.
    //*************************************************************************
    template <typename ... Args>
    bool emplace(Args&&... args)
    {
      lock();

      bool result = emplace_implementation(etl::forward<Args>(args)...);

      unlock();

      return result;
    }
#else
    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    /// If asserts or exceptions are enabled, throws an etl::queue_full if the queue if already full.
    //*************************************************************************
    template <typename T1>
    bool emplace_from_unlocked(const T1& value1)
    {
      return emplace_implementation(value1);
    }

    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    /// If asserts or exceptions are enabled, throws an etl::queue_full if the queue if already full.
    //*************************************************************************
    template <typename T1, typename T2>
    bool emplace_from_unlocked(const T1& value1, const T2& value2)
    {
      return emplace_implementation(value1, value2);
    }

    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    /// If asserts or exceptions are enabled, throws an etl::queue_full if the queue if already full.
    //*************************************************************************
    template <typename T1, typename T2, typename T3>
    bool emplace_from_unlocked(const T1& value1, const T2& value2, const T3& value3)
    {
      return emplace_implementation(value1, value2, value3);
    }

    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    /// If asserts or exceptions are enabled, throws an etl::queue_full if the queue if already full.
    //*************************************************************************
    template <typename T1, typename T2, typename T3, typename T4>
    bool emplace_from_unlocked(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
    {
      return emplace_implementation(value1, value2, value3, value4);
    }

    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    /// If asserts or exceptions are enabled, throws an etl::queue_full if the queue if already full.
    //*************************************************************************
    template <typename T1>
    bool emplace(const T1& value1)
    {
      lock();

      bool result = emplace_implementation(value1);

      unlock();

      return result;
    }

    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    /// If asserts or exceptions are enabled, throws an etl::queue_full if the queue if already full.
    //*************************************************************************
    template <typename T1, typename T2>
    bool emplace(const T1& value1, const T2& value2)
    {
      lock();

      bool result = emplace_implementation(value1, value2);

      unlock();

      return result;
    }

    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    /// If asserts or exceptions are enabled, throws an etl::queue_full if the queue if already full.
    //*************************************************************************
    template <typename T1, typename T2, typename T3>
    bool emplace(const T1& value1, const T2& value2, const T3& value3)
    {
      lock();

      bool result = emplace_implementation(value1, value2, value3);

      unlock();

      return result;
    }

    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    /// If asserts or exceptions are enabled, throws an etl::queue_full if the queue if already full.
    //*************************************************************************
    template <typename T1, typename T2, typename T3, typename T4>
    bool emplace(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
    {
      lock();

      bool result = emplace_implementation(value1, value2, value3, value4);

      unlock();

      return result;
    }
#endif

    //*************************************************************************
    /// Pop a value from the queue.
    /// Unlocked
    //*************************************************************************
    bool pop_from_unlocked(reference value)
    {
      return pop_implementation(value);;
    }

    //*************************************************************************
    /// Pop a value from the queue.
    //*************************************************************************
    bool pop(reference value)
    {
      lock();

      bool result = pop_implementation(value);

      unlock();

      return result;
    }

    //*************************************************************************
    /// Pop a value from the queue and discard.
    /// Unlocked
    //*************************************************************************
    bool pop_from_unlocked()
    {
      return pop_implementation();
    }

    //*************************************************************************
    /// Pop a value from the queue and discard.
    //*************************************************************************
    bool pop()
    {
      lock();

      bool result = pop_implementation();

      unlock();

      return result;
    }

    //*************************************************************************
    /// Peek a value from the front of the queue.
    /// Unlocked
    //*************************************************************************
    reference front_from_unlocked()
    {
      return front_implementation();
    }

    //*************************************************************************
    /// Peek a value from the front of the queue.
    /// Unlocked
    //*************************************************************************
    const_reference front_from_unlocked() const
    {
      return front_implementation();
    }

    //*************************************************************************
    /// Peek a value from the front of the queue.
    //*************************************************************************
    reference front()
    {
      lock();

      reference result = front_implementation();

      unlock();

      return result;
    }

    //*************************************************************************
    /// Peek a value from the front of the queue.
    //*************************************************************************
    const_reference front() const
    {
      lock();

      const_reference result = front_implementation();

      unlock();

      return result;
    }

    //*************************************************************************
    /// Clear the queue from the ISR.
    //*************************************************************************
    void clear_from_unlocked()
    {
      while (pop_implementation())
      {
        // Do nothing.
      }
    }

    //*************************************************************************
    /// Clear the queue.
    //*************************************************************************
    void clear()
    {
      lock();

      while (pop_implementation())
      {
        // Do nothing.
      }

      unlock();
    }

    //*************************************************************************
    /// How much free space available in the queue.
    //*************************************************************************
    size_type available() const
    {
      lock();

      size_type result = this->available_implementation();

      unlock();

      return result;
    }

    //*************************************************************************
    /// Is the queue full?
    //*************************************************************************
    bool full() const
    {
      lock();

      size_type result = this->full_implementation();

      unlock();

      return result;
    }

    //*************************************************************************
    /// How many items in the queue?
    //*************************************************************************
    size_type size() const
    {
      lock();

      size_type result = this->size_implementation();

      unlock();

      return result;
    }

    //*************************************************************************
    /// Is the queue empty?
    //*************************************************************************
    bool empty() const
    {
      lock();

      bool result = this->empty_implementation();

      unlock();

      return result;
    }

  protected:

    //*************************************************************************
    /// The constructor that is called from derived classes.
    //*************************************************************************
    iqueue_spsc_locked(T* p_buffer_, size_type max_size_, const etl::ifunction<void>& lock_, const etl::ifunction<void>& unlock_)
      : base_t(max_size_)
      , p_buffer(p_buffer_)
      , lock(lock_)
      , unlock(unlock_)
    {
    }

  private:

    //*************************************************************************
    /// Push a value to the queue.
    //*************************************************************************
    bool push_implementation(const_reference value)
    {
      if (this->current_size != this->MAX_SIZE)
      {
        ::new (&p_buffer[this->write_index]) T(value);

        this->write_index = this->get_next_index(this->write_index, this->MAX_SIZE);

        ++this->current_size;

        return true;
      }

      // Queue is full.
      return false;
    }

#if ETL_USING_CPP11 && ETL_NOT_USING_STLPORT && !defined(ETL_QUEUE_LOCKED_FORCE_CPP03_IMPLEMENTATION)
    //*************************************************************************
    /// Push a value to the queue.
    /// Unlocked.
    //*************************************************************************
    bool push_implementation(rvalue_reference value)
    {
      if (this->current_size != this->MAX_SIZE)
      {
        ::new (&p_buffer[this->write_index]) T(etl::move(value));

        this->write_index = this->get_next_index(this->write_index, this->MAX_SIZE);

        ++this->current_size;

        return true;
      }

      // Queue is full.
      return false;
    }
#endif

#if ETL_USING_CPP11 && ETL_NOT_USING_STLPORT && !defined(ETL_QUEUE_LOCKED_FORCE_CPP03_IMPLEMENTATION)
    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    /// Unlocked.
    //*************************************************************************
    template <typename ... Args>
    bool emplace_implementation(Args&&... args)
    {
      if (this->current_size != this->MAX_SIZE)
      {
        ::new (&p_buffer[this->write_index]) T(etl::forward<Args>(args)...);

        this->write_index = this->get_next_index(this->write_index, this->MAX_SIZE);

        ++this->current_size;

        return true;
      }

      // Queue is full.
      return false;
    }
#else
    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    //*************************************************************************
    template <typename T1>
    bool emplace_implementation(const T1& value1)
    {
      if (this->current_size != this->MAX_SIZE)
      {
        ::new (&p_buffer[this->write_index]) T(value1);

        this->write_index = this->get_next_index(this->write_index, this->MAX_SIZE);

        ++this->current_size;

        return true;
      }

      // Queue is full.
      return false;
    }

    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    //*************************************************************************
    template <typename T1, typename T2>
    bool emplace_implementation(const T1& value1, const T2& value2)
    {
      if (this->current_size != this->MAX_SIZE)
      {
        ::new (&p_buffer[this->write_index]) T(value1, value2);

        this->write_index = this->get_next_index(this->write_index, this->MAX_SIZE);

        ++this->current_size;

        return true;
      }

      // Queue is full.
      return false;
    }

    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    //*************************************************************************
    template <typename T1, typename T2, typename T3>
    bool emplace_implementation(const T1& value1, const T2& value2, const T3& value3)
    {
      if (this->current_size != this->MAX_SIZE)
      {
        ::new (&p_buffer[this->write_index]) T(value1, value2, value3);

        this->write_index = this->get_next_index(this->write_index, this->MAX_SIZE);

        ++this->current_size;

        return true;
      }

      // Queue is full.
      return false;
    }

    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    //*************************************************************************
    template <typename T1, typename T2, typename T3, typename T4>
    bool emplace_implementation(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
    {
      if (this->current_size != this->MAX_SIZE)
      {
        ::new (&p_buffer[this->write_index]) T(value1, value2, value3, value4);

        this->write_index = this->get_next_index(this->write_index, this->MAX_SIZE);

        ++this->current_size;

        return true;
      }

      // Queue is full.
      return false;
    }
#endif

    //*************************************************************************
    /// Pop a value from the queue.
    /// Unlocked
    //*************************************************************************
    bool pop_implementation(reference value)
    {
      if (this->current_size == 0)
      {
        // Queue is empty
        return false;
      }

#if ETL_USING_CPP11 && ETL_NOT_USING_STLPORT && !defined(ETL_QUEUE_LOCKABLE_FORCE_CPP03_IMPLEMENTATION)
      value = etl::move(p_buffer[this->read_index]);
#else
      value = p_buffer[this->read_index];
#endif

      p_buffer[this->read_index].~T();

      this->read_index = this->get_next_index(this->read_index, this->MAX_SIZE);

      --this->current_size;

      return true;
    }

    //*************************************************************************
    /// Peek a value from the front of the queue.
    /// Unlocked
    //*************************************************************************
    reference front_implementation()
    {
      return p_buffer[this->read_index];
    }

    //*************************************************************************
    /// Peek a value from the front of the queue.
    /// Unlocked
    //*************************************************************************
    const_reference front_implementation() const
    {
      return p_buffer[this->read_index];
    }

    //*************************************************************************
    /// Pop a value from the queue.
    /// Unlocked
    //*************************************************************************
    bool pop_implementation()
    {
      if (this->current_size == 0)
      {
        // Queue is empty
        return false;
      }

      p_buffer[this->read_index].~T();

      this->read_index = this->get_next_index(this->read_index, this->MAX_SIZE);

      --this->current_size;

      return true;
    }

    // Disable copy construction and assignment.
    iqueue_spsc_locked(const iqueue_spsc_locked&) ETL_DELETE;
    iqueue_spsc_locked& operator =(const iqueue_spsc_locked&) ETL_DELETE;

#if ETL_USING_CPP11
    iqueue_spsc_locked(iqueue_spsc_locked&&) = delete;
    iqueue_spsc_locked& operator =(iqueue_spsc_locked&&) = delete;
#endif

    T* p_buffer; ///< The internal buffer.

    const etl::ifunction<void>& lock;   ///< The callback that locks interrupts.
    const etl::ifunction<void>& unlock; ///< The callback that unlocks interrupts.
  };

  //***************************************************************************
  ///\ingroup queue_spsc
  /// A fixed capacity spsc queue.
  /// This queue supports concurrent access by one producer and one consumer.
  /// \tparam T            The type this queue should support.
  /// \tparam SIZE         The maximum capacity of the queue.
  /// \tparam MEMORY_MODEL The memory model for the queue. Determines the type of the internal counter variables.
  //***************************************************************************
  template <typename T, size_t SIZE, const size_t MEMORY_MODEL = etl::memory_model::MEMORY_MODEL_LARGE>
  class queue_spsc_locked : public etl::iqueue_spsc_locked<T, MEMORY_MODEL>
  {
  private:

    typedef etl::iqueue_spsc_locked<T, MEMORY_MODEL> base_t;

  public:

    typedef typename base_t::size_type size_type;

    ETL_STATIC_ASSERT((SIZE <= etl::integral_limits<size_type>::max), "Size too large for memory model");

    static ETL_CONSTANT size_type MAX_SIZE = size_type(SIZE);

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************

    queue_spsc_locked(const etl::ifunction<void>& lock,
                      const etl::ifunction<void>& unlock)
      : base_t(reinterpret_cast<T*>(buffer.raw), MAX_SIZE, lock, unlock)
    {
    }

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~queue_spsc_locked()
    {
      base_t::clear();
    }

  private:

    queue_spsc_locked(const queue_spsc_locked&) ETL_DELETE;
    queue_spsc_locked& operator = (const queue_spsc_locked&) ETL_DELETE;

#if ETL_USING_CPP11
    queue_spsc_locked(queue_spsc_locked&&) = delete;
    queue_spsc_locked& operator =(queue_spsc_locked&&) = delete;
#endif

    /// The uninitialised buffer of T used in the queue_lockable.
    etl::uninitialized_buffer_of<T, MAX_SIZE> buffer;
  };
}

#endif
