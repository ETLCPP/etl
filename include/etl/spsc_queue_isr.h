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

#ifndef __ETL_SPSC_QUEUE_ISR__
#define __ETL_SPSC_QUEUE_ISR__

#include <stddef.h>
#include <stdint.h>

#include "platform.h"
#include "alignment.h"
#include "parameter_type.h"

#undef ETL_FILE
#define ETL_FILE "46"

namespace etl
{
  //***************************************************************************
  ///\ingroup spsc_queue
  ///\brief This is the base for all spsc_queue_isrs that contain a particular type.
  ///\details Normally a reference to this type will be taken from a derived spsc_queue_isr.
  ///\code
  /// etl::spsc_queue_isr_isr<int, 10> myQueue;
  /// etl::iqueue_isr<int>& iQueue = myQueue;
  ///\endcode
  /// This queue supports concurrent access by one producer and one consumer.
  /// \tparam T The type of value that the spsc_queue_isr holds.
  //***************************************************************************
  template <typename T, typename TAccess>
  class ispsc_queue_isr
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
      access.disable();

      bool result = push_implementation(value);

      access.restore();

      return result;
    }

    //*************************************************************************
    /// Push a value to the queue from an ISR.
    //*************************************************************************
    bool push_from_isr(parameter_t value)
    {
      return push_implementation(value);
    }

    //*************************************************************************
    /// Pop a value from the queue.
    //*************************************************************************
    bool pop(reference value)
    {     
      access.disable();

      bool result = pop_implementation(value);

      access.restore();

      return result;
    }

    //*************************************************************************
    /// Pop a value from the queue and discard.
    //*************************************************************************
    bool pop()
    {
      access.disable();

      bool result = pop_implementation();

      access.restore();

      return result;
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
    /// Clear the queue.
    //*************************************************************************
    void clear()
    {
      access.disable();

      while (pop_implementation())
      {
        // Do nothing.
      }

      access.restore();
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
    /// Accurate from the 'pop' thread.
    /// 'Not empty' is a guess from the 'push' thread.
    //*************************************************************************
    bool empty() const
    {
      return (current_size == 0);
    }

    //*************************************************************************
    /// Is the queue full?
    /// Accurate from the 'push' thread.
    /// 'Not full' is a guess from the 'pop' thread.
    //*************************************************************************
    bool full() const
    {
      return (current_size = MAX_SIZE);
    }

    //*************************************************************************
    /// How many items in the queue?
    /// Due to concurrency, this is a guess.
    //*************************************************************************
    size_t size() const
    {
      return current_size;
    }

    //*************************************************************************
    /// How much free space available in the queue.
    /// Due to concurrency, this is a guess.
    //*************************************************************************
    size_t available() const
    {
      return MAX_SIZE - size() - 1;
    }

    //*************************************************************************
    /// How many items can the queue hold.
    //*************************************************************************
    size_t capacity() const
    {
      return MAX_SIZE - 1;
    }

    //*************************************************************************
    /// How many items can the queue hold.
    //*************************************************************************
    size_t max_size() const
    {
      return MAX_SIZE - 1;
    }

  protected:

    //*************************************************************************
    /// The constructor that is called from derived classes.
    //*************************************************************************
    ispsc_queue_isr(T* p_buffer_, size_type max_size_, TAccess& access_)
      : p_buffer(p_buffer_),
        write(0),
        read(0),
        current_size(0),
        MAX_SIZE(max_size_),
        access(&access_)
    {
    }

  private:

    // Disable copy construction and assignment.
    ispsc_queue_isr(const ispsc_queue_isr&);
    ispsc_queue_isr& operator =(const ispsc_queue_isr&);

    T* p_buffer;              ///< The internal buffer.
    size_type write;          ///< Where to input new data.
    size_type read;           ///< Where to get the oldest data.
    size_type current_size;   ///< The current size of the queue.
    const size_type MAX_SIZE; ///< The maximum number of items in the queue.
    TAccess& access;          ///< The object that enables/disables interrupts.

    //*************************************************************************
    /// Pop a value from the queue.
    //*************************************************************************
    bool pop_implementation(reference value)
    {
      size_t read_index = read;

      if (read_index == write)
      {
        // Queue is empty
        return false;
      }

      size_t next_index = get_next_index(read_index, MAX_SIZE);

      value = buffer[read_index];
      buffer[read_index].~T();

      read = next_index;

      return true;
    }

    //*************************************************************************
    /// Pop a value from the queue and discard.
    //*************************************************************************
    bool pop_implementation()
    {
      size_t read_index = read;

      if (read_index == write)
      {
        // Queue is empty
        return false;
      }

      size_t next_index = get_next_index(read_index, MAX_SIZE);

      p_buffer[read_index].~T();

      read = next_index;

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

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
#if defined(ETL_POLYMORPHIC_SPSC_QUEUE) || defined(ETL_POLYMORPHIC_CONTAINERS)
  public:
    virtual ~ispsc_queue_isr()
    {
    }
#else
  protected:
    ~ispsc_queue_isr()
    {
    }
#endif
  };

  //***************************************************************************
  ///\ingroup spsc_queue
  /// A fixed capacity spsc queue.
  /// This queue supports concurrent access by one producer and one consumer.
  /// \tparam T       The type this queue should support.
  /// \tparam SIZE    The maximum capacity of the queue.
  /// \tparam TAccess The type that will be passed to the queue to disable and restore interrupts.
  //***************************************************************************
  template <typename T, size_t SIZE, typename TAccess>
  class spsc_queue_isr<T, SIZE, etl::spsc_queue_isr_isr_tag> : public etl::ispsc_queue_isr<T, etl::spsc_queue_isr_isr_tag, TAccessPolicy>
  {
    typedef etl::ispsc_queue_isr<T, TAccess> base_t;

  public:

    static const size_t MAX_SIZE = SIZE;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    spsc_queue_isr(TAccess& access)
      : base_t(reinterpret_cast<T*>(&buffer[0]), MAX_SIZE, access)
    {
    }

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~spsc_queue_isr()
    {
      base_t::clear();
    }

  private:

    /// The uninitialised buffer of T used in the spsc_queue_isr.
    typename etl::aligned_storage<sizeof(T), etl::alignment_of<T>::value>::type buffer[MAX_SIZE];
  };
};

#endif