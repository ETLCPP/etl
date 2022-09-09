///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 Benedek Kupper, John Wellbelove

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

/**
 * @note Based on the works of Andrea Lattuada and James Munns:
 * https://blog.systems.ethz.ch/blog/2019/the-design-and-implementation-of-a-lock-free-ring-buffer-with-contiguous-reservations.html
 * Whose design was inspired by Simon Cooke:
 * https://www.codeproject.com/Articles/3479/The-Bip-Buffer-The-Circular-Buffer-with-a-Twist
 */

#ifndef ETL_BIP_BUFFER_SPSC_ATOMIC_INCLUDED
#define ETL_BIP_BUFFER_SPSC_ATOMIC_INCLUDED

#include "platform.h"
#include "alignment.h"
#include "parameter_type.h"
#include "atomic.h"
#include "memory.h"
#include "memory_model.h"
#include "integral_limits.h"
#include "utility.h"
#include "error_handler.h"
#include "span.h"
#include "file_error_numbers.h"

#include <stddef.h>
#include <stdint.h>

#if ETL_HAS_ATOMIC

namespace etl
{
  //***************************************************************************
  /// Base exception for a bip buffer.
  //***************************************************************************
  class bip_buffer_exception: public exception
  {
  public:

    bip_buffer_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
     : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Exception for an invalid reserve.
  //***************************************************************************
  class bip_buffer_reserve_invalid: public bip_buffer_exception
  {
  public:

    bip_buffer_reserve_invalid(string_type file_name_, numeric_type line_number_)
      : bip_buffer_exception(ETL_ERROR_TEXT("bip_buffer:reserve", ETL_BIP_BUFFER_SPSC_ATOMIC_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// The common base for a bip_buffer_spsc_atomic_base.
  //***************************************************************************
  template <const size_t MEMORY_MODEL = etl::memory_model::MEMORY_MODEL_LARGE>
  class bip_buffer_spsc_atomic_base
  {
  public:

    /// The type used for determining the size of buffer.
    typedef typename etl::size_type_lookup<MEMORY_MODEL>::type size_type;

    //*************************************************************************
    /// Returns true if the buffer is empty.
    //*************************************************************************
    bool empty() const
    {
      return size() == 0;
    }

    //*************************************************************************
    /// Returns true if the buffer is full.
    //*************************************************************************
    bool full() const
    {
      return available() == 0;
    }

    //*************************************************************************
    /// Returns the total used size, which may be split in two blocks
    /// so the size will always be smaller after a read commit.
    //*************************************************************************
    size_type size() const
    {
      size_type write_index = write.load(etl::memory_order_acquire);
      size_type read_index = read.load(etl::memory_order_acquire);

      // no wraparound
      if (write_index >= read_index)
      {
        // size is distance between read and write
        return write_index - read_index;
      }
      else
      {
        size_type last_index = last.load(etl::memory_order_acquire);

        // size is distance between beginning and write, plus read and last
        return (write_index - 0) + (last_index - read_index);
      }
    }

    //*************************************************************************
    /// Returns the largest contiguous available block size.
    //*************************************************************************
    size_type available() const
    {
      size_type write_index = write.load(etl::memory_order_acquire);
      size_type read_index = read.load(etl::memory_order_acquire);

      // no wraparound
      if (write_index >= read_index)
      {
        size_type forward_size = capacity() - write_index;

        // check if there's more space if wrapping around
        if (read_index > (forward_size + 1))
        {
          return read_index - 1;
        }
        else
        {
          return forward_size;
        }
      }
      else // read_index > write_index
      {
        return read_index - write_index - 1;
      }
    }

    //*************************************************************************
    /// Returns the maximum capacity of the buffer.
    //*************************************************************************
    size_type capacity() const
    {
      return RESERVED;
    }

    //*************************************************************************
    /// Returns the maximum size of the buffer.
    //*************************************************************************
    size_type max_size() const
    {
      return RESERVED;
    }

  protected:

    //*************************************************************************
    /// Constructs the buffer.
    //*************************************************************************
    bip_buffer_spsc_atomic_base(size_type reserved_)
     : read(0)
     , write(0)
     , last(0)
     , RESERVED(reserved_)
    {
    }

    //*************************************************************************
    void reset()
    {
      read.store(0, etl::memory_order_release);
      write.store(0, etl::memory_order_release);
      last.store(0, etl::memory_order_release);
    }

    //*************************************************************************
    size_type get_write_reserve(size_type* psize, size_type fallback_size = numeric_limits<size_type>::max())
    {
      size_type write_index = write.load(etl::memory_order_relaxed);
      size_type read_index = read.load(etl::memory_order_acquire);

      // No wraparound
      if (write_index >= read_index)
      {
        size_type forward_size = capacity() - write_index;

        // We still fit in linearly
        if (*psize <= forward_size)
        {
          return write_index;
        }
        // There isn't more space even when wrapping around,
        // or the linear size is good enough as fallback
        else if ((read_index <= (forward_size + 1)) || (fallback_size <= forward_size))
        {
          *psize = forward_size;
          return write_index;
        }
        // Better wrap around now
        else
        {
          // Check if size fits.
          // When wrapping, the write index cannot reach read index,
          // then we'd not be able to distinguish wrapped situation from linear.
          if (*psize >= read_index)
          {
            if (read_index > 0)
            {
              *psize = read_index - 1;
            }
            else
            {
              *psize = 0;
            }
          }

          return 0;
        }
      }
      else // read_index > write_index
      {
        // Doesn't fit
        if ((write_index + *psize) >= read_index)
        {
          *psize = read_index - write_index - 1;
        }

        return write_index;
      }
    }

    //*************************************************************************
    void apply_write_reserve(size_type windex, size_type wsize)
    {
      if (wsize > 0)
      {
        size_type write_index = write.load(etl::memory_order_relaxed);
        size_type read_index = read.load(etl::memory_order_acquire);

        // Wrapped around already
        if (write_index < read_index)
        {
          ETL_ASSERT_AND_RETURN((windex == write_index) && ((wsize + 1) <= read_index), ETL_ERROR(bip_buffer_reserve_invalid));
        }
        // No wraparound so far, also not wrapping around with this block
        else if (windex == write_index)
        {
          ETL_ASSERT_AND_RETURN(wsize <= (capacity() - write_index), ETL_ERROR(bip_buffer_reserve_invalid));

          // Move both indexes forward
          last.store(windex + wsize, etl::memory_order_release);
        }
        // Wrapping around now
        else
        {
          ETL_ASSERT_AND_RETURN((windex == 0) && ((wsize + 1) <= read_index), ETL_ERROR(bip_buffer_reserve_invalid));
        }
        
        // Always update write index
        write.store(windex + wsize, etl::memory_order_release);
      }
    }

    //*************************************************************************
    size_type get_read_reserve(size_type* psize)
    {
      size_type read_index = read.load(etl::memory_order_relaxed);
      size_type write_index = write.load(etl::memory_order_acquire);

      if (read_index > write_index)
      {
        // Writer has wrapped around
        size_type last_index = last.load(etl::memory_order_relaxed);

        if (read_index == last_index)
        {
          // Reader reached the end, start read from 0
          read_index = 0;
        }
        else // (read_index < last_index)
        {
          // Use the remaining buffer at the end
          write_index = last_index;
        }
      }
      else
      {
        // No wraparound, nothing to adjust
      }

      // Limit to max available size
      if ((write_index - read_index) < *psize)
      {
        *psize = write_index - read_index;
      }

      return read_index;
    }

    //*************************************************************************
    void apply_read_reserve(size_type rindex, size_type rsize)
    {
      if (rsize > 0)
      {
        size_type rsize_checker = rsize;
        ETL_ASSERT_AND_RETURN((rindex == get_read_reserve(&rsize_checker)) && (rsize == rsize_checker), ETL_ERROR(bip_buffer_reserve_invalid));

        read.store(rindex + rsize, etl::memory_order_release);
      }
    }

  private:

    etl::atomic<size_type> read;
    etl::atomic<size_type> write;
    etl::atomic<size_type> last;
    const size_type RESERVED;

#if defined(ETL_POLYMORPHIC_SPSC_BIP_BUFFER_ATOMIC) || defined(ETL_POLYMORPHIC_CONTAINERS)
  public:

    virtual ~bip_buffer_spsc_atomic_base()
    {
    }
#else
  protected:

    ~bip_buffer_spsc_atomic_base()
    {
    }
#endif
  };

  //***************************************************************************
  /// A fixed capacity bipartite buffer.
  //***************************************************************************
  template <typename T, const size_t MEMORY_MODEL = etl::memory_model::MEMORY_MODEL_LARGE>
  class ibip_buffer_spsc_atomic : public bip_buffer_spsc_atomic_base<MEMORY_MODEL>
  {
  private:

    typedef typename etl::bip_buffer_spsc_atomic_base<MEMORY_MODEL> base_t;
    using base_t::reset;
    using base_t::get_read_reserve;
    using base_t::apply_read_reserve;
    using base_t::get_write_reserve;
    using base_t::apply_write_reserve;

  public:

    typedef T                          value_type;      ///< The type stored in the buffer.
    typedef T&                         reference;       ///< A reference to the type used in the buffer.
    typedef const T&                   const_reference; ///< A const reference to the type used in the buffer.
#if ETL_USING_CPP11
    typedef T&&                        rvalue_reference;///< An rvalue_reference to the type used in the buffer.
#endif
    typedef typename base_t::size_type size_type;       ///< The type used for determining the size of the buffer.

    using base_t::max_size;

    //*************************************************************************
    // Reserves a memory area for reading (up to the max_reserve_size).
    //*************************************************************************
    span<T> read_reserve(size_type max_reserve_size = numeric_limits<size_type>::max())
    {
      size_type reserve_size = max_reserve_size;
      size_type rindex = get_read_reserve(&reserve_size);
        
      return span<T>(p_buffer + rindex, reserve_size);
    }

    //*************************************************************************
    // Commits the previously reserved read memory area
    // the reserve can be trimmed at the end before committing.
    // Throws bip_buffer_reserve_invalid
    //*************************************************************************
    void read_commit(const span<T> &reserve)
    {
      size_type rindex = etl::distance(p_buffer, reserve.data());
      apply_read_reserve(rindex, reserve.size());
    }

    //*************************************************************************
    // Reserves a memory area for writing up to the max_reserve_size.
    //*************************************************************************
    span<T> write_reserve(size_type max_reserve_size)
    {
      size_type reserve_size = max_reserve_size;
      size_type windex = get_write_reserve(&reserve_size);
        
      return span<T>(p_buffer + windex, reserve_size);
    }

    //*************************************************************************
    // Reserves an optimal memory area for writing. The buffer will only wrap
    // around if the available forward space is less than min_reserve_size.
    //*************************************************************************
    span<T> write_reserve_optimal(size_type min_reserve_size = 1U)
    {
        size_type reserve_size = numeric_limits<size_type>::max();
        size_type windex = get_write_reserve(&reserve_size, min_reserve_size);

        return span<T>(p_buffer + windex, reserve_size);
    }

    //*************************************************************************
    // Commits the previously reserved write memory area
    // the reserve can be trimmed at the end before committing.
    // Throws bip_buffer_reserve_invalid
    //*************************************************************************
    void write_commit(const span<T> &reserve)
    {
      size_type windex = etl::distance(p_buffer, reserve.data());
      apply_write_reserve(windex, reserve.size());
    }

    //*************************************************************************
    /// Clears the buffer, destructing any elements that haven't been read.
    //*************************************************************************
    void clear()
    {
      // the buffer might be split into two contiguous blocks
      for (span<T> reader = read_reserve(); reader.size() > 0; reader = read_reserve())
      {
        destroy(reader.begin(), reader.end());
        read_commit(reader);
      }
      // now the buffer is already empty
      // resetting the buffer here is beneficial to have
      // the whole buffer available for a single block,
      // but it requires synchronization between the writer and reader threads
      reset();
    }

  protected:

    //*************************************************************************
    ibip_buffer_spsc_atomic(T* p_buffer_, size_type reserved_)
      : base_t(reserved_)
      , p_buffer(p_buffer_)
    {
    }

  private:

    // Disable copy construction and assignment.
    ibip_buffer_spsc_atomic(const ibip_buffer_spsc_atomic&) ETL_DELETE;
    ibip_buffer_spsc_atomic& operator =(const ibip_buffer_spsc_atomic&) ETL_DELETE;

#if ETL_USING_CPP11
    ibip_buffer_spsc_atomic(ibip_buffer_spsc_atomic&&) = delete;
    ibip_buffer_spsc_atomic& operator =(ibip_buffer_spsc_atomic&&) = delete;
#endif

    T* const p_buffer;
  };

  //***************************************************************************
  /// A fixed capacity bipartite buffer.
  /// This buffer supports concurrent access by one producer and one consumer.
  /// \tparam T            The type this buffer should support.
  /// \tparam SIZE         The maximum capacity of the buffer.
  /// \tparam MEMORY_MODEL The memory model for the buffer. Determines the type of the internal counter variables.
  //***************************************************************************
  template <typename T, const size_t SIZE, const size_t MEMORY_MODEL = etl::memory_model::MEMORY_MODEL_LARGE>
  class bip_buffer_spsc_atomic : public ibip_buffer_spsc_atomic<T, MEMORY_MODEL>
  {
  private:

    typedef typename etl::ibip_buffer_spsc_atomic<T, MEMORY_MODEL> base_t;

  public:

    typedef typename base_t::size_type size_type;

  private:

    static ETL_CONSTANT size_type RESERVED_SIZE = size_type(SIZE);

  public:

    ETL_STATIC_ASSERT((SIZE <= (etl::integral_limits<size_type>::max)), "Size too large for memory model");

    static ETL_CONSTANT size_type MAX_SIZE = size_type(SIZE);

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    bip_buffer_spsc_atomic()
      : base_t(reinterpret_cast<T*>(buffer.raw), RESERVED_SIZE)
    {
    }

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~bip_buffer_spsc_atomic()
    {
      base_t::clear();
    }

  private:

    /// The uninitialised buffer of T used in the bip_buffer_spsc.
    etl::uninitialized_buffer_of<T, RESERVED_SIZE> buffer;
  };
}

#endif /* ETL_HAS_ATOMIC && ETL_USING_CPP11 */

#endif /* ETL_BIP_BUFFER_SPSC_ATOMIC_INCLUDED */
