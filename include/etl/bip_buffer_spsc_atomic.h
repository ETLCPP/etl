///\file

/**
 * @note Based on the works of Andrea Lattuada and James Munns:
 * https://blog.systems.ethz.ch/blog/2019/the-design-and-implementation-of-a-lock-free-ring-buffer-with-contiguous-reservations.html
 * Whose design was inspired by Simon Cooke:
 * https://www.codeproject.com/Articles/3479/The-Bip-Buffer-The-Circular-Buffer-with-a-Twist
 */
#ifndef ETL_BIP_BUFFER_SPSC_ATOMIC_INCLUDED
#define ETL_BIP_BUFFER_SPSC_ATOMIC_INCLUDED

#include <stddef.h>
#include <stdint.h>

#include "platform.h"
#include "alignment.h"
#include "parameter_type.h"
#include "atomic.h"
#include "memory_model.h"
#include "integral_limits.h"
#include "utility.h"

#include "span.h"

#if ETL_HAS_ATOMIC

namespace etl
{
    class bip_buffer_exception: public exception
    {
    public:
        bip_buffer_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
                : exception(reason_, file_name_, line_number_)
        {
        }
    };

    class bip_buffer_reserve_invalid: public bip_buffer_exception
    {
    public:
        bip_buffer_reserve_invalid(string_type file_name_, numeric_type line_number_)
          : bip_buffer_exception("bip_buffer:reserve", file_name_, line_number_)
        {
        }
    };

    template <const size_t MEMORY_MODEL = etl::memory_model::MEMORY_MODEL_LARGE>
    class bip_buffer_spsc_atomic_base
    {
    public:
        /// The type used for determining the size of queue.
        typedef typename etl::size_type_lookup<MEMORY_MODEL>::type size_type;

        bool empty() const
        {
            return size() == 0;
        }

        bool full() const
        {
            return available() == 0;
        }

        // returns the total used size, which may be split in two blocks
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

        // returns the largest contiguous available block size
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

        size_type capacity() const
        {
            return RESERVED;
        }

        size_type max_size() const
        {
            return RESERVED;
        }

        void clear()
        {
            read.store(0, etl::memory_order_release);
            write.store(0, etl::memory_order_release);
            last.store(RESERVED, etl::memory_order_release);
        }

    protected:
        bip_buffer_spsc_atomic_base(size_type reserved_)
            : read(0),
            write(0),
            last(reserved_),
            RESERVED(reserved_)
        {
        }

        size_type get_write_reserve(size_type* psize)
        {
            size_type write_index = write.load(etl::memory_order_relaxed);
            size_type read_index = read.load(etl::memory_order_acquire);

            // no wraparound
            if (write_index >= read_index)
            {
                size_type forward_size = capacity() - write_index;

                // we still fit in linearly
                if (*psize <= forward_size)
                {
                    return write_index;
                }
                // there isn't more space even when wrapping around
                else if (read_index <= (forward_size + 1))
                {
                    *psize = forward_size;
                    return write_index;
                }
                // better wrap around now
                else
                {
                    // check if size fits
                    // when wrapping, the write index cannot reach read index,
                    // then we'd not be able to distinguish wrapped situation from linear
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
                // doesn't fit
                if ((write_index + *psize) >= read_index)
                {
                    *psize = read_index - write_index - 1;
                }
                return write_index;
            }
        }

        void apply_write_reserve(size_type windex, size_type wsize)
        {
            if (wsize > 0)
            {
                size_type write_index = write.load(etl::memory_order_relaxed);
                size_type read_index = read.load(etl::memory_order_acquire);

                // no wraparound so far, also not wrapping around with this block
                if ((write_index >= read_index) && (windex > 0))
                {
                    // move both indexes forward
                    last.store(windex + wsize, etl::memory_order_relaxed);
                }
                // always update write index
                write.store(windex + wsize, etl::memory_order_release);
            }
        }

        size_type get_read_reserve(size_type* psize)
        {
            size_type read_index = read.load(etl::memory_order_relaxed);
            size_type write_index = write.load(etl::memory_order_acquire);

            if (read_index > write_index)
            {
                // writer has wrapped around

                size_type last_index = last.load(etl::memory_order_relaxed);
                if (read_index == last_index)
                {
                    // reader reached the end, start read from 0
                    read_index = 0;
                }
                else // (read_index < last_index)
                {
                    // use the remaining buffer at the end
                    write_index = last_index;
                }
            }
            else
            {
                // no wraparound, nothing to adjust
            }

            // limit to max available size
            if ((write_index - read_index) < *psize)
            {
                *psize = write_index - read_index;
            }
            return read_index;
        }

        void apply_read_reserve(size_type rindex, size_type rsize)
        {
            if (rsize > 0)
            {
                read.store(rindex + rsize, etl::memory_order_release);
            }
        }

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

    template <typename T, const size_t MEMORY_MODEL = etl::memory_model::MEMORY_MODEL_LARGE>
    class ibip_buffer_spsc_atomic : public bip_buffer_spsc_atomic_base<MEMORY_MODEL>
    {
    private:
        typedef typename etl::bip_buffer_spsc_atomic_base<MEMORY_MODEL> base_t;
        using base_t::write;
        using base_t::read;
        using base_t::last;
        using base_t::get_read_reserve;
        using base_t::apply_read_reserve;
        using base_t::get_write_reserve;
        using base_t::apply_write_reserve;

    public:
        typedef T                          value_type;      ///< The type stored in the queue.
        typedef T&                         reference;       ///< A reference to the type used in the queue.
        typedef const T&                   const_reference; ///< A const reference to the type used in the queue.
#if ETL_CPP11_SUPPORTED
        typedef T&&                        rvalue_reference;///< An rvalue_reference to the type used in the queue.
#endif
        typedef typename base_t::size_type size_type;       ///< The type used for determining the size of the queue.

        span<T> read_reserve(size_type max_reserve_size)
        {
            size_type reserve_size = max_reserve_size;
            auto rindex = get_read_reserve(&reserve_size);
            return span<T>(p_buffer + rindex, reserve_size);
        }

        void read_commit(const span<T> &reserve)
        {
            ETL_ASSERT(reserve == read_reserve(reserve.size()), ETL_ERROR(bip_buffer_reserve_invalid));
            size_type rindex = etl::distance(p_buffer, reserve.data());
            apply_read_reserve(rindex, reserve.size());
        }

        span<T> write_reserve(size_type max_reserve_size)
        {
            size_type reserve_size = max_reserve_size;
            auto windex = get_write_reserve(&reserve_size);
            return span<T>(p_buffer + windex, reserve_size);
        }

        void write_commit(const span<T> &reserve)
        {
            ETL_ASSERT(reserve == write_reserve(reserve.size()), ETL_ERROR(bip_buffer_reserve_invalid));
            size_type windex = etl::distance(p_buffer, reserve.data());
            apply_write_reserve(windex, reserve.size());
        }

    protected:
        ibip_buffer_spsc_atomic(T* p_buffer_, size_type reserved_)
            : base_t(reserved_),
            p_buffer(p_buffer_)
        {
        }

    private:
        // Disable copy construction and assignment.
        ibip_buffer_spsc_atomic(const ibip_buffer_spsc_atomic&) ETL_DELETE;
        ibip_buffer_spsc_atomic& operator =(const ibip_buffer_spsc_atomic&) ETL_DELETE;

#if ETL_CPP11_SUPPORTED
        ibip_buffer_spsc_atomic(ibip_buffer_spsc_atomic&&) = delete;
        ibip_buffer_spsc_atomic& operator =(ibip_buffer_spsc_atomic&&) = delete;
#endif

        T* const p_buffer;
    };

    template <typename T, const size_t SIZE, const size_t MEMORY_MODEL = etl::memory_model::MEMORY_MODEL_LARGE>
    class bip_buffer_spsc_atomic : public ibip_buffer_spsc_atomic<T, MEMORY_MODEL>
    {
    private:
        typedef typename etl::ibip_buffer_spsc_atomic<T, MEMORY_MODEL> base_t;

    public:
        typedef typename base_t::size_type size_type;

    private:
        static const size_type RESERVED_SIZE = size_type(SIZE);

    public:
        ETL_STATIC_ASSERT((SIZE <= (etl::integral_limits<size_type>::max - 1)), "Size too large for memory model");

        static const size_type MAX_SIZE = size_type(SIZE);

        bip_buffer_spsc_atomic()
            : base_t(buffer.data(), RESERVED_SIZE)
        {
        }

    private:
        typename etl::array<T, RESERVED_SIZE> buffer;
    };
}

#endif /* ETL_HAS_ATOMIC */

#endif /* ETL_BIP_BUFFER_SPSC_ATOMIC_INCLUDED */
