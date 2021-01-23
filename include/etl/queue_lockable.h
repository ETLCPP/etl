///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2019 jwellbelove

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

#include <stddef.h>
#include <stdint.h>

#include "platform.h"
#include "memory.h"
#include "parameter_type.h"
#include "memory_model.h"
#include "integral_limits.h"
#include "function.h"
#include "utility.h"
#include "placement_new.h"
#include "ilockable.h"

#undef ETL_FILE
#define ETL_FILE "54"

namespace etl
{
  template <typename T, const size_t VMemory_Model = etl::memory_model::MEMORY_MODEL_LARGE>
  class iqueue_lockable_base
  {
  public:

    /// The type used for determining the size of queue.
    typedef typename etl::size_type_lookup<VMemory_Model>::type size_type;

    typedef T        value_type;      ///< The type stored in the queue.
    typedef T&       reference;       ///< A reference to the type used in the queue.
    typedef const T& const_reference; ///< A const reference to the type used in the queue.
#if ETL_CPP11_SUPPORTED
    typedef T&&      rvalue_reference;///< An rvalue reference to the type used in the queue.
#endif

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    virtual ~iqueue_lockable_base()
    {
    }

    //*************************************************************************
    /// Push a value to the queue from an ISR.
    //*************************************************************************
    bool push_from_unlocked(const_reference value)
    {
      return push_implementation(value);
    }

#if ETL_CPP11_SUPPORTED
    //*************************************************************************
    /// Push a value to the queue from an ISR.
    //*************************************************************************
    bool push_from_unlocked(rvalue_reference value)
    {
      return push_implementation(etl::move(value));
    }
#endif

    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    /// If asserts or exceptions are enabled, throws an etl::queue_full if the queue if already full.
    ///\param value The value to use to construct the item to push to the queue.
    //*************************************************************************
#if ETL_CPP11_SUPPORTED && ETL_NOT_USING_STLPORT && !defined(ETL_QUEUE_LOCKED_FORCE_CPP03)
    template <typename ... Args>
    bool emplace_from_unlocked(Args&&... args)
    {
      return emplace_implementation(etl::forward<Args>(args)...);
    }
#endif

    //*************************************************************************
    /// Pop a value from the queue from an ISR
    //*************************************************************************
    bool pop_from_unlocked(reference value)
    {
      return pop_implementation(value);
    }

#if ETL_CPP11_SUPPORTED
    //*************************************************************************
    /// Pop a value from the queue from an ISR
    //*************************************************************************
    bool pop_from_unlocked(rvalue_reference value)
    {
      return pop_implementation(etl::move(value));
    }
#endif

    //*************************************************************************
    /// Pop a value from the queue from an ISR, and discard.
    //*************************************************************************
    bool pop_from_unlocked()
    {
      return pop_implementation();
    }

    //*************************************************************************
    /// How much free space available in the queue.
    /// Called from ISR.
    //*************************************************************************
    size_type available_from_unlocked() const
    {
      return Max_Size - current_size;
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
    /// Is the queue empty?
    /// Called from ISR.
    //*************************************************************************
    bool empty_from_unlocked() const
    {
      return (current_size == 0);
    }

    //*************************************************************************
    /// Is the queue full?
    /// Called from ISR.
    //*************************************************************************
    bool full_from_unlocked() const
    {
      return (current_size == Max_Size);
    }

    //*************************************************************************
    /// How many items in the queue?
    /// Called from ISR.
    //*************************************************************************
    size_type size_from_unlocked() const
    {
      return current_size;
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

    iqueue_lockable_base(T* p_buffer_, size_type max_size_)
      : p_buffer(p_buffer_),
        write_index(0),
        read_index(0),
        current_size(0),
        Max_Size(max_size_)
    {
    }

    //*************************************************************************
    /// Push a value to the queue.
    //*************************************************************************
    bool push_implementation(const_reference value)
    {
      if (current_size != Max_Size)
      {
        ::new (&p_buffer[write_index]) T(value);

        write_index = get_next_index(write_index, Max_Size);

        ++current_size;

        return true;
      }

      // Queue is full.
      return false;
    }

#if ETL_CPP11_SUPPORTED
    //*************************************************************************
    /// Push a value to the queue.
    //*************************************************************************
    bool push_implementation(rvalue_reference value)
    {
      if (current_size != Max_Size)
      {
        ::new (&p_buffer[write_index]) T(etl::move(value));

        write_index = get_next_index(write_index, Max_Size);

        ++current_size;

        return true;
      }

      // Queue is full.
      return false;
    }
#endif

#if ETL_CPP11_SUPPORTED && ETL_NOT_USING_STLPORT && !defined(ETL_QUEUE_LOCKED_FORCE_CPP03)
    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    /// If asserts or exceptions are enabled, throws an etl::queue_full if the queue if already full.
    ///\param value The value to use to construct the item to push to the queue.
    //*************************************************************************
    template <typename ... Args>
    bool emplace_implementation(Args&&... args)
    {
      if (current_size != Max_Size)
      {
        ::new (&p_buffer[write_index]) T(etl::forward<Args>(args)...);

        write_index = get_next_index(write_index, Max_Size);

        ++current_size;

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
    template <typename T1>
    bool emplace_implementation(const T1& value1)
    {
      if (current_size != Max_Size)
      {
        ::new (&p_buffer[write_index]) T(value1);

        write_index = get_next_index(write_index, Max_Size);

        ++current_size;

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
    bool emplace_implementation(const T1& value1, const T2& value2)
    {
      if (current_size != Max_Size)
      {
        ::new (&p_buffer[write_index]) T(value1, value2);

        write_index = get_next_index(write_index, Max_Size);

        ++current_size;

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
    bool emplace_implementation(const T1& value1, const T2& value2, const T3& value3)
    {
      if (current_size != Max_Size)
      {
        ::new (&p_buffer[write_index]) T(value1, value2, value3);

        write_index = get_next_index(write_index, Max_Size);

        ++current_size;

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
    bool emplace_implementation(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
    {
      if (current_size != Max_Size)
      {
        ::new (&p_buffer[write_index]) T(value1, value2, value3, value4);

        write_index = get_next_index(write_index, Max_Size);

        ++current_size;

        return true;
      }

      // Queue is full.
      return false;
    }

#endif

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

      read_index = get_next_index(read_index, Max_Size);

      --current_size;

      return true;
    }

#if ETL_CPP11_SUPPORTED
    //*************************************************************************
    /// Pop a value from the queue.
    //*************************************************************************
    bool pop_implementation(rvalue_reference value)
    {
      if (current_size == 0)
      {
        // Queue is empty
        return false;
      }

      value = etl::move(p_buffer[read_index]);
      p_buffer[read_index].~T();

      read_index = get_next_index(read_index, Max_Size);

      --current_size;

      return true;
    }
#endif

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

      read_index = get_next_index(read_index, Max_Size);

      --current_size;

      return true;
    }

    //*************************************************************************
    /// Calculate the next index.
    //*************************************************************************
    static size_type get_next_index(size_type index, size_type maximum)
    {
      ++index;

      if (index == maximum)
      {
        index = 0;
      }

      return index;
    }

    T* p_buffer;              ///< The internal buffer.
    size_type write_index;    ///< Where to input new data.
    size_type read_index;     ///< Where to get the oldest data.
    size_type current_size;   ///< The current size of the queue.
    const size_type Max_Size; ///< The maximum number of items in the queue.
  };

  //***************************************************************************
  ///\brief This is the base for all queues that contain a particular type.
  ///\details Normally a reference to this type will be taken from a derived queue_lockable.
  /// This queue supports concurrent access by one producer and one consumer.
  /// \tparam T The type of value that the queue_lockable holds.
  //***************************************************************************
  template <typename T, const size_t VMemory_Model = etl::memory_model::MEMORY_MODEL_LARGE>
  class iqueue_lockable : public etl::iqueue_lockable_base<T, VMemory_Model>
                        , public etl::ilockable
  {
  private:

    typedef iqueue_lockable_base<T, VMemory_Model> base_t;

  public:

    typedef typename base_t::value_type       value_type;      ///< The type stored in the queue.
    typedef typename base_t::reference        reference;       ///< A reference to the type used in the queue.
    typedef typename base_t::const_reference  const_reference; ///< A const reference to the type used in the queue.
#if ETL_CPP11_SUPPORTED
    typedef typename base_t::rvalue_reference rvalue_reference;///< An rvalue reference to the type used in the queue.
#endif
    typedef typename base_t::size_type        size_type;       ///< The type used for determining the size of the queue.

    //*************************************************************************
    /// Push a value to the queue.
    //*************************************************************************
    bool push(const_reference value)
    {
      lock();

      bool result = this->push_implementation(value);

      unlock();

      return result;
    }

#if ETL_CPP11_SUPPORTED
    //*************************************************************************
    /// Push a value to the queue.
    //*************************************************************************
    bool push(rvalue_reference value)
    {
      lock();

      bool result = this->push_implementation(etl::move(value));

      unlock();

      return result;
    }
#endif

    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    /// If asserts or exceptions are enabled, throws an etl::queue_full if the queue if already full.
    //*************************************************************************
#if ETL_CPP11_SUPPORTED && ETL_NOT_USING_STLPORT && !defined(ETL_QUEUE_LOCKED_FORCE_CPP03)
    template <typename ... Args>
    bool emplace(Args&&... args)
    {
      lock();

      bool result = this->emplace_implementation(etl::forward<Args>(args)...);

      unlock();

      return result;
    }
#else
    //*************************************************************************
    /// Constructs a value in the queue 'in place'.
    /// If asserts or exceptions are enabled, throws an etl::queue_full if the queue if already full.
    //*************************************************************************
    template <typename T1>
    bool emplace(const T1& value1)
    {
      lock();

      bool result = this->emplace_implementation(value1);

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

      bool result = this->emplace_implementation(value1, value2);

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

      bool result = this->emplace_implementation(value1, value2, value3);

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

      bool result = this->emplace_implementation(value1, value2, value3, value4);

      unlock();

      return result;
    }
#endif

    //*************************************************************************
    /// Pop a value from the queue.
    //*************************************************************************
    bool pop(reference value)
    {
      lock();

      bool result = this->pop_implementation(value);

      unlock();

      return result;
    }

#if ETL_CPP11_SUPPORTED
    //*************************************************************************
    /// Pop a value from the queue.
    //*************************************************************************
    bool pop(rvalue_reference value)
    {
      lock();

      bool result = this->pop_implementation(etl::move(value));

      unlock();

      return result;
    }
#endif

    //*************************************************************************
    /// Pop a value from the queue and discard.
    //*************************************************************************
    bool pop()
    {
      lock();

      bool result = this->pop_implementation();

      unlock();

      return result;
    }

    //*************************************************************************
    /// Clear the queue.
    //*************************************************************************
    void clear()
    {
      lock();

      while (this->pop_implementation())
      {
        // Do nothing.
      }

      unlock();
    }

    //*************************************************************************
    /// Is the queue empty?
    //*************************************************************************
    bool empty() const
    {
      lock();

      size_type result = (this->current_size == 0);

      unlock();

      return result;
    }

    //*************************************************************************
    /// Is the queue full?
    //*************************************************************************
    bool full() const
    {
      lock();

      size_type result = (this->current_size == this->Max_Size);

      unlock();

      return result;
    }

    //*************************************************************************
    /// How many items in the queue?
    //*************************************************************************
    size_type size() const
    {
      lock();

      size_type result = this->current_size;

      unlock();

      return result;
    }

    //*************************************************************************
    /// How much free space available in the queue.
    //*************************************************************************
    size_type available() const
    {
      lock();

      size_type result = this->Max_Size - this->current_size;

      unlock();

      return result;
    }

  protected:

    //*************************************************************************
    /// The constructor that is called from derived classes.
    //*************************************************************************
    iqueue_lockable(T* p_buffer_, size_type max_size_)
      : base_t(p_buffer_, max_size_)
    {
    }

  private:

    // Disable copy construction and assignment.
    iqueue_lockable(const iqueue_lockable&) ETL_DELETE;
    iqueue_lockable& operator =(const iqueue_lockable&) ETL_DELETE;

#if ETL_CPP11_SUPPORTED
    iqueue_lockable(iqueue_lockable&&) = delete;
    iqueue_lockable& operator =(iqueue_lockable&&) = delete;
#endif
  };

  //***************************************************************************
  /// A fixed capacity lockable queue.
  /// This queue supports concurrent access.
  /// \tparam T            The type this queue should support.
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
      while (this->pop_implementation())
      {
        // Do nothing.
      }
    }

  private:

    queue_lockable(const queue_lockable&) ETL_DELETE;
    queue_lockable& operator = (const queue_lockable&) ETL_DELETE;

#if ETL_CPP11_SUPPORTED
    queue_lockable(queue_lockable&&) = delete;
    queue_lockable& operator =(queue_lockable&&) = delete;
#endif

    /// The uninitialised buffer of T used in the queue_lockable.
    etl::uninitialized_buffer_of<T, Max_Size> buffer;
  };
}

#undef ETL_FILE

#endif
