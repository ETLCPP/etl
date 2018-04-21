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

#ifndef ETL_SPSC_QUEUE_ISR_INCLUDED
#define ETL_SPSC_QUEUE_ISR_INCLUDED

#include <stddef.h>
#include <stdint.h>

#include "platform.h"
#include "alignment.h"
#include "parameter_type.h"

#undef ETL_FILE
#define ETL_FILE "46"

namespace etl
{
  template <typename T>
  class queue_spsc_isr_base
  {
  protected:

    typedef typename etl::parameter_type<T>::type parameter_t;

  public:

    typedef T        value_type;      ///< The type stored in the queue.
    typedef T&       reference;       ///< A reference to the type used in the queue.
    typedef const T& const_reference; ///< A const reference to the type used in the queue.
    typedef size_t   size_type;       ///< The type used for determining the size of the queue.

    //*************************************************************************
    /// Push a value to the queue from an ISR.
    //*************************************************************************
    bool push_from_isr(parameter_t value)
    {
      return push_implementation(value);
    }

    //*************************************************************************
    /// Pop a value from the queue from an ISR
    //*************************************************************************
    bool pop_from_isr(reference value)
    {
      return pop_implementation(value);
    }

    //*************************************************************************
    /// Pop a value from the queue from an ISR, and discard.
    //*************************************************************************
    bool pop_from_isr()
    {
      return pop_implementation();
    }

    //*************************************************************************
    /// How much free space available in the queue.
    /// Called from ISR.
    //*************************************************************************
    size_t available_from_isr() const
    {
      return MAX_SIZE - current_size;
    }

    //*************************************************************************
    /// Clear the queue from the ISR.
    //*************************************************************************
    void clear_from_isr()
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
    bool empty_from_isr() const
    {
      return (current_size == 0);
    }

    //*************************************************************************
    /// Is the queue full?
    /// Called from ISR.
    //*************************************************************************
    bool full_from_isr() const
    {
      return (current_size == MAX_SIZE);
    }

    //*************************************************************************
    /// How many items in the queue?
    /// Called from ISR.
    //*************************************************************************
    size_t size_from_isr() const
    {
      return current_size;
    }

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

    queue_spsc_isr_base(T* p_buffer_, size_type max_size_)
      : p_buffer(p_buffer_),
        write_index(0),
        read_index(0),
        current_size(0),
        MAX_SIZE(max_size_)
    {
    }

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

    T* p_buffer;              ///< The internal buffer.
    size_type write_index;    ///< Where to input new data.
    size_type read_index;     ///< Where to get the oldest data.
    size_type current_size;   ///< The current size of the queue.
    const size_type MAX_SIZE; ///< The maximum number of items in the queue.

  private:

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
#if defined(ETL_POLYMORPHIC_SPSC_QUEUE_ISR) || defined(ETL_POLYMORPHIC_CONTAINERS)
  public:
    virtual ~queue_spsc_isr_base()
    {
    }
#else
  protected:
    ~queue_spsc_isr_base()
    {
    }
#endif
  };

  //***************************************************************************
  ///\ingroup queue_spsc
  ///\brief This is the base for all queue_spsc_isrs that contain a particular type.
  ///\details Normally a reference to this type will be taken from a derived queue_spsc_isr.
  ///\code
  /// etl::queue_spsc_isr_isr<int, 10> myQueue;
  /// etl::iqueue_isr<int>& iQueue = myQueue;
  ///\endcode
  /// This queue supports concurrent access by one producer and one consumer.
  /// \tparam T The type of value that the queue_spsc_isr holds.
  //***************************************************************************
  template <typename T, typename TAccess>
  class iqueue_spsc_isr : public queue_spsc_isr_base<T>
  {
  private:

    typedef typename queue_spsc_isr_base<T>::parameter_t parameter_t;

  public:

    typedef typename queue_spsc_isr_base<T>::value_type      value_type;      ///< The type stored in the queue.
    typedef typename queue_spsc_isr_base<T>::reference       reference;       ///< A reference to the type used in the queue.
    typedef typename queue_spsc_isr_base<T>::const_reference const_reference; ///< A const reference to the type used in the queue.
    typedef typename queue_spsc_isr_base<T>::size_type       size_type;       ///< The type used for determining the size of the queue.

    //*************************************************************************
    /// Push a value to the queue.
    //*************************************************************************
    bool push(parameter_t value)
    {
      TAccess::lock();

      bool result = this->push_implementation(value);

      TAccess::unlock();

      return result;
    }

    //*************************************************************************
    /// Pop a value from the queue.
    //*************************************************************************
    bool pop(reference value)
    {
      TAccess::lock();

      bool result = this->pop_implementation(value);

      TAccess::unlock();

      return result;
    }

    //*************************************************************************
    /// Pop a value from the queue and discard.
    //*************************************************************************
    bool pop()
    {
      TAccess::lock();

      bool result = this->pop_implementation();

      TAccess::unlock();

      return result;
    }

    //*************************************************************************
    /// Clear the queue.
    //*************************************************************************
    void clear()
    {
      TAccess::lock();

      while (this->pop_implementation())
      {
        // Do nothing.
      }

      TAccess::unlock();
    }

    //*************************************************************************
    /// Is the queue empty?
    //*************************************************************************
    bool empty() const
    {
      TAccess::lock();

      size_t result = (this->current_size == 0);

      TAccess::unlock();

      return result;
    }

    //*************************************************************************
    /// Is the queue full?
    //*************************************************************************
    bool full() const
    {
      TAccess::lock();

      size_t result = (this->current_size == this->MAX_SIZE);

      TAccess::unlock();

      return result;
    }

    //*************************************************************************
    /// How many items in the queue?
    //*************************************************************************
    size_t size() const
    {
      TAccess::lock();

      size_t result = this->current_size;

      TAccess::unlock();

      return result;
    }

    //*************************************************************************
    /// How much free space available in the queue.
    //*************************************************************************
    size_t available() const
    {
      TAccess::lock();

      size_t result = this->MAX_SIZE - this->current_size;

      TAccess::unlock();

      return result;
    }

  protected:

    //*************************************************************************
    /// The constructor that is called from derived classes.
    //*************************************************************************
    iqueue_spsc_isr(T* p_buffer_, size_type max_size_)
      : queue_spsc_isr_base<T>(p_buffer_, max_size_)
    {
    }

  private:

    // Disable copy construction and assignment.
    iqueue_spsc_isr(const iqueue_spsc_isr&);
    iqueue_spsc_isr& operator =(const iqueue_spsc_isr&);

    TAccess access; ///< The object that locks/unlocks interrupts.
  };

  //***************************************************************************
  ///\ingroup queue_spsc
  /// A fixed capacity spsc queue.
  /// This queue supports concurrent access by one producer and one consumer.
  /// \tparam T       The type this queue should support.
  /// \tparam SIZE    The maximum capacity of the queue.
  /// \tparam TAccess The type that will lock and unlock interrupts.
  //***************************************************************************
  template <typename T, size_t SIZE, typename TAccess>
  class queue_spsc_isr : public etl::iqueue_spsc_isr<T, TAccess>
  {
    typedef etl::iqueue_spsc_isr<T, TAccess> base_t;

  public:

    static const size_t MAX_SIZE = SIZE;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    queue_spsc_isr()
      : base_t(reinterpret_cast<T*>(&buffer[0]), MAX_SIZE)
    {
    }

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~queue_spsc_isr()
    {
      base_t::clear();
    }

  private:

    queue_spsc_isr(const queue_spsc_isr&);
    queue_spsc_isr& operator = (const queue_spsc_isr&);

    /// The uninitialised buffer of T used in the queue_spsc_isr.
    typename etl::aligned_storage<sizeof(T), etl::alignment_of<T>::value>::type buffer[MAX_SIZE];
  };
};

#endif
