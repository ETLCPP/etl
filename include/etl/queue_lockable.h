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

#ifndef ETL_QUEUE_LOCKABLE_INCLUDED
#define ETL_QUEUE_LOCKABLE_INCLUDED

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
  template <size_t VMemory_Model = etl::memory_model::MEMORY_MODEL_LARGE>
  class queue_lockable_base
  {
  public:

    /// The type used for determining the size of queue.
    typedef typename etl::size_type_lookup<VMemory_Model>::type size_type;

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    virtual ~queue_lockable_base()
    {
    }

    //*************************************************************************
    /// How much free space available in the queue. 
    /// Unlocked
    //*************************************************************************
    size_type available_unlocked() const
    {
      return available_implementation();
    }

    //*************************************************************************
    /// How much free space available in the queue.
    //*************************************************************************
    size_type available() const
    {
      this->lock();

      size_type result = available_implementation();

      this->unlock();

      return result;
    }

    //*************************************************************************
    /// Is the queue empty? 
    /// Unlocked.
    //*************************************************************************
    bool empty_unlocked() const
    {
      return empty_implementation();
    }

    //*************************************************************************
    /// Is the queue empty?
    //*************************************************************************
    bool empty() const
    {
      this->lock();

      size_type result = empty_implementation();

      this->unlock();

      return result;
    }

    //*************************************************************************
    /// Is the queue full? 
    /// Unlocked.
    //*************************************************************************
    bool full_unlocked() const
    {
      return full_implementation();
    }

    //*************************************************************************
    /// Is the queue full?
    //*************************************************************************
    bool full() const
    {
      this->lock();

      size_type result = full_implementation();

      this->unlock();

      return result;
    }

    //*************************************************************************
    /// How many items in the queue? 
    /// Unlocked.
    //*************************************************************************
    size_type size_unlocked() const
    {
      return size_implementation();
    }

    //*************************************************************************
    /// How many items in the queue?
    //*************************************************************************
    size_type size() const
    {
      this->lock();

      size_type result = size_implementation();

      this->unlock();

      return result;
    }

    //*************************************************************************
    /// How many items can the queue hold.
    //*************************************************************************
    size_type capacity() const
    {
      return Max_Size;
    }

    //*************************************************************************
    /// How many items can the queue hold.
    //*************************************************************************
    size_type max_size() const
    {
      return Max_Size;
    }

  protected:

    queue_lockable_base(size_type max_size_)
      : write_index(0),
        read_index(0),
        current_size(0),
        Max_Size(max_size_)
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

    //*************************************************************************
    /// The pure virtual lock and unlock functions.
    //*************************************************************************
    virtual void lock() const = 0;
    virtual void unlock() const = 0;

    size_type write_index;    ///< Where to input new data.
    size_type read_index;     ///< Where to get the oldest data.
    size_type current_size;   ///< The current size of the queue.
    const size_type Max_Size; ///< The maximum number of items in the queue.

  private:

    //*************************************************************************
    /// How much free space available in the queue.
    //*************************************************************************
    size_type available_implementation() const
    {
      return Max_Size - current_size;
    }

    //*************************************************************************
    /// Is the queue empty?
    //*************************************************************************
    bool empty_implementation() const
    {
      return (current_size == 0);
    }

    //*************************************************************************
    /// Is the queue full?
    //*************************************************************************
    bool full_implementation() const
    {
      return (current_size == Max_Size);;
    }

    //*************************************************************************
    /// How many items in the queue?
    //*************************************************************************
    size_type size_implementation() const
    {
      return current_size;
    }
  };

  //***************************************************************************
  ///\brief This is the base for all queues that contain a particular type.
  ///\details Normally a reference to this type will be taken from a derived queue_lockable.
  /// This queue supports concurrent access by one producer and one consumer.
  /// \tparam T The type of value that the queue_lockable holds.
  //***************************************************************************
  template <typename T, const size_t VMemory_Model = etl::memory_model::MEMORY_MODEL_LARGE>
  class iqueue_lockable : public etl::queue_lockable_base<VMemory_Model>
  {
  private:

    typedef queue_lockable_base<VMemory_Model> base_t;

  public:

    typedef T                          value_type;       ///< The type stored in the queue.
    typedef T&                         reference;        ///< A reference to the type used in the queue.
    typedef const T&                   const_reference;  ///< A const reference to the type used in the queue.
#if ETL_USING_CPP11
    typedef T&&                        rvalue_reference; ///< An rvalue reference to the type used in the queue.
#endif
    typedef typename base_t::size_type size_type;        ///< The type used for determining the size of the queue.

    //*************************************************************************
    /// Push a value to the queue without locking.
    //*************************************************************************
    bool push_unlocked(const_reference value)
    {
      return push_implementation(value);
    }

    //*************************************************************************
    /// Push a value to the queue.
    //*************************************************************************
    bool push(const_reference value)
    {
      this->lock();

      bool result = push_implementation(value);

      this->unlock();

      return result;
    }

#if ETL_USING_CPP11 && ETL_NOT_USING_STLPORT && !defined(ETL_QUEUE_LOCKABLE_FORCE_CPP03_IMPLEMENTATION)
    //*************************************************************************
    /// Push a value to the queue without locking.
    //*************************************************************************
    bool push_unlocked(rvalue_reference value)
    {
      return push_implementation(value);
    }

    //*************************************************************************
    /// Push a value to the queue.
    //*************************************************************************
    bool push(rvalue_reference value)
    {
      this->lock();

      bool result = push_implementation(etl::move(value));

      this->unlock();

      return result;
    }
#endif

#if ETL_USING_CPP11 && ETL_NOT_USING_STLPORT && !defined(ETL_QUEUE_LOCKABLE_FORCE_CPP03_IMPLEMENTATION)
    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    //*************************************************************************
    template <typename ... Args>
    bool emplace_unlocked(Args&&... args)
    {
      return emplace_implementation(etl::forward<Args>(args)...);
    }

    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    //*************************************************************************
    template <typename ... Args>
    bool emplace(Args&&... args)
    {
      this->lock();

      bool result = emplace_implementation(etl::forward<Args>(args)...);

      this->unlock();

      return result;
    }
#else
    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    //*************************************************************************
    template <typename T1>
    bool emplace_unlocked(const T1& value1)
    {
      return emplace_implementation(value1);
    }

    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    //*************************************************************************
    template <typename T1, typename T2>
    bool emplace_unlocked(const T1& value1, const T2& value2)
    {
      return emplace_implementation(value1, value2);
    }

    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    //*************************************************************************
    template <typename T1, typename T2, typename T3>
    bool emplace_unlocked(const T1& value1, const T2& value2, const T3& value3)
    {
      return emplace_implementation(value1, value2, value3);
    }

    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    //*************************************************************************
    template <typename T1, typename T2, typename T3, typename T4>
    bool emplace_unlocked(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
    {
      return emplace_implementation(value1, value2, value3, value4);
    }

    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    //*************************************************************************
    template <typename T1>
    bool emplace(const T1& value1)
    {
      this->lock();

      bool result = emplace_implementation(value1);

      this->unlock();

      return result;
    }

    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    //*************************************************************************
    template <typename T1, typename T2>
    bool emplace(const T1& value1, const T2& value2)
    {
      this->lock();

      bool result = emplace_implementation(value1, value2);

      this->unlock();

      return result;
    }

    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    //*************************************************************************
    template <typename T1, typename T2, typename T3>
    bool emplace(const T1& value1, const T2& value2, const T3& value3)
    {
      this->lock();

      bool result = emplace_implementation(value1, value2, value3);

      this->unlock();

      return result;
    }

    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    //*************************************************************************
    template <typename T1, typename T2, typename T3, typename T4>
    bool emplace(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
    {
      this->lock();

      bool result = emplace_implementation(value1, value2, value3, value4);

      this->unlock();

      return result;
    }
#endif

    //*************************************************************************
    /// Pop a value from the queue without locking, and discard.
    //*************************************************************************
    bool pop_unlocked()
    {
      return pop_implementation();
    }


    //*************************************************************************
    /// Pop a value from the queue and discard.
    //*************************************************************************
    bool pop()
    {
      this->lock();

      bool result = pop_implementation();

      this->unlock();

      return result;
    }

    //*************************************************************************
    /// Pop a value from the queue without locking
    //*************************************************************************
    bool pop_unlocked(reference value)
    {
      return pop_implementation(value);
    }

    //*************************************************************************
    /// Pop a value from the queue.
    //*************************************************************************
    bool pop(reference value)
    {
      this->lock();

      bool result = pop_implementation(value);

      this->unlock();

      return result;
    }

    //*************************************************************************
    /// Peek a value at the front of the queue without locking.
    //*************************************************************************
    reference front_unlocked()
    {
      return front_implementation();
    }

    //*************************************************************************
    /// Peek a value at the front of the queue without locking.
    //*************************************************************************
    const_reference front_unlocked() const
    {
      return front_implementation();
    }

    //*************************************************************************
    /// Peek a value at the front of the queue.
    //*************************************************************************
    reference front()
    {
      this->lock();

      reference result = front_implementation();

      this->unlock();

      return result;
    }

    //*************************************************************************
    /// Peek a value at the front of the queue.
    //*************************************************************************
    const_reference front() const
    {
      this->lock();

      const_reference result = front_implementation();

      this->unlock();

      return result;
    }

    //*************************************************************************
    /// Clear the queue, unlocked.
    //*************************************************************************
    void clear_unlocked()
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
      this->lock();

      while (pop_implementation())
      {
        // Do nothing.
      }

      this->unlock();
    }

  protected:

    //*************************************************************************
    /// The constructor that is called from derived classes.
    //*************************************************************************
    iqueue_lockable(T* p_buffer_, size_type max_size_)
      : base_t(max_size_)
      , p_buffer(p_buffer_)
    {
    }

  private:

    //*************************************************************************
    /// Push a value to the queue without locking.
    //*************************************************************************
    bool push_implementation(const_reference value)
    {
      if (this->current_size != this->Max_Size)
      {
        ::new (&p_buffer[this->write_index]) T(value);

        this->write_index = this->get_next_index(this->write_index, this->Max_Size);

        ++this->current_size;

        return true;
      }

      // Queue is full.
      return false;
    }

#if ETL_USING_CPP11 && ETL_NOT_USING_STLPORT && !defined(ETL_QUEUE_LOCKABLE_FORCE_CPP03_IMPLEMENTATION)
    //*************************************************************************
    /// Push a value to the queue without locking.
    //*************************************************************************
    bool push_implementation(rvalue_reference value)
    {
      if (this->current_size != this->Max_Size)
      {
        ::new (&p_buffer[this->write_index]) T(etl::move(value));

        this->write_index = this->get_next_index(this->write_index, this->Max_Size);

        ++this->current_size;

        return true;
      }

      // Queue is full.
      return false;
    }
#endif

#if ETL_USING_CPP11 && ETL_NOT_USING_STLPORT && !defined(ETL_QUEUE_LOCKABLE_FORCE_CPP03_IMPLEMENTATION)
    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    //*************************************************************************
    template <typename ... Args>
    bool emplace_implementation(Args&&... args)
    {
      if (this->current_size != this->Max_Size)
      {
        ::new (&p_buffer[this->write_index]) T(etl::forward<Args>(args)...);

        this->write_index = this->get_next_index(this->write_index, this->Max_Size);

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
      if (this->current_size != this->Max_Size)
      {
        ::new (&p_buffer[this->write_index]) T(value1);

        this->write_index = this->get_next_index(this->write_index, this->Max_Size);

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
      if (this->current_size != this->Max_Size)
      {
        ::new (&p_buffer[this->write_index]) T(value1, value2);

        this->write_index = this->get_next_index(this->write_index, this->Max_Size);

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
      if (this->current_size != this->Max_Size)
      {
        ::new (&p_buffer[this->write_index]) T(value1, value2, value3);

        this->write_index = this->get_next_index(this->write_index, this->Max_Size);

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
      if (this->current_size != this->Max_Size)
      {
        ::new (&p_buffer[this->write_index]) T(value1, value2, value3, value4);

        this->write_index = this->get_next_index(this->write_index, this->Max_Size);

        ++this->current_size;

        return true;
      }

      // Queue is full.
      return false;
    }
#endif

    //*************************************************************************
    /// Pop a value from the queue without locking, and discard.
    //*************************************************************************
    bool pop_implementation()
    {
      if (this->current_size == 0)
      {
        // Queue is empty
        return false;
      }

      p_buffer[this->read_index].~T();

      this->read_index = this->get_next_index(this->read_index, this->Max_Size);

      --this->current_size;

      return true;
    }

    //*************************************************************************
    /// Pop a value from the queue without locking
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

      this->read_index = this->get_next_index(this->read_index, this->Max_Size);

      --this->current_size;

      return true;
    }

    //*************************************************************************
    /// Peek a value at the front of the queue without locking
    //*************************************************************************
    reference front_implementation()
    {
      return p_buffer[this->read_index];;
    }

    //*************************************************************************
    /// Peek a value at the front of the queue without locking
    //*************************************************************************
    const_reference front_implementation() const
    {
      return p_buffer[this->read_index];;
    }

    // Disable copy construction and assignment.
    iqueue_lockable(const iqueue_lockable&) ETL_DELETE;
    iqueue_lockable& operator =(const iqueue_lockable&) ETL_DELETE;

#if ETL_USING_CPP11
    iqueue_lockable(iqueue_lockable&&) = delete;
    iqueue_lockable& operator =(iqueue_lockable&&) = delete;
#endif

    T* p_buffer; ///< The internal buffer.
  };

  //***************************************************************************
  /// A fixed capacity lockable queue.
  /// This queue supports concurrent access.
  /// \tparam T             The type this queue should support.
  /// \tparam VSize         The maximum capacity of the queue.
  /// \tparam VMemory_Model The memory model for the queue. Determines the type of the internal counter variables.
  //***************************************************************************
  template <typename T, size_t VSize, size_t VMemory_Model = etl::memory_model::MEMORY_MODEL_LARGE>
  class queue_lockable : public etl::iqueue_lockable<T, VMemory_Model>
  {
  private:

    typedef etl::iqueue_lockable<T, VMemory_Model> base_t;

  public:

    typedef typename base_t::size_type size_type;

    ETL_STATIC_ASSERT((VSize <= etl::integral_limits<size_type>::max), "Size too large for memory model");

    static ETL_CONSTANT size_type Max_Size     = size_type(VSize);
    static ETL_CONSTANT size_type Memory_Model = size_type(VMemory_Model);

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************

    queue_lockable()
      : base_t(reinterpret_cast<T*>(buffer.raw), Max_Size)
    {
    }

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~queue_lockable()
    {
      while (this->pop_unlocked())
      {
        // Do nothing.
      }
    }

  private:

    queue_lockable(const queue_lockable&) ETL_DELETE;
    queue_lockable& operator = (const queue_lockable&) ETL_DELETE;

#if ETL_USING_CPP11
    queue_lockable(queue_lockable&&) = delete;
    queue_lockable& operator =(queue_lockable&&) = delete;
#endif

    /// The uninitialised buffer of T used in the queue_lockable.
    etl::uninitialized_buffer_of<T, Max_Size> buffer;
  };

  template <typename T, size_t VSize, size_t VMemory_Model>
  ETL_CONSTANT typename queue_lockable<T, VSize, VMemory_Model>::size_type queue_lockable<T, VSize, VMemory_Model>::Max_Size;
  
  template <typename T, size_t VSize, size_t VMemory_Model>
  ETL_CONSTANT typename queue_lockable<T, VSize, VMemory_Model>::size_type queue_lockable<T, VSize, VMemory_Model>::Memory_Model;
}

#endif
