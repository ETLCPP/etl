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

#include "array.h"
#include "array_view.h"
#include "function.h"

#if ETL_HAS_ATOMIC

namespace etl
{
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
                assert((rindex + rsize) <= capacity());
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

    template <typename TCallee, typename TItem>
    class buffer_reserve
    {
    public:
        typedef const TItem& const_reference;
        typedef const TItem* const_pointer;
        typedef const TItem* const_iterator;
        typedef ETL_OR_STD::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef TItem* pointer;
        typedef TItem& reference;
        typedef TItem* iterator;
        typedef ETL_OR_STD::reverse_iterator<iterator> reverse_iterator;

        typedef typename etl::array_view<TItem> view_t;
        typedef typename etl::function<TCallee, buffer_reserve*> commit_fn_t;

        ETL_CONSTEXPR
            buffer_reserve()
        {
        }

        ETL_CONSTEXPR
            buffer_reserve(TCallee& callee_, void(TCallee::* p_function_)(buffer_reserve*), TItem* begin_, const size_t size_)
             : m_view(begin_, size_), m_commit_fn(callee_, p_function_)
        {
        }

        buffer_reserve(buffer_reserve&& other)
            : m_view(other.m_view), m_commit_fn(other.m_commit_fn)
        {
            other.cancel();
        }

        buffer_reserve& operator= (buffer_reserve&& other)
        {
            m_view = other.m_view;
            m_commit_fn = other.m_commit_fn;
            other.cancel();
            return *this;
        }

        buffer_reserve(const buffer_reserve&) = delete;
        buffer_reserve& operator= (const buffer_reserve&) = delete;

        void truncate(const size_t new_size)
        {
            size_t prev_size = size();
            if (new_size <= prev_size)
            {
                m_view.remove_suffix(prev_size - new_size);
            }
            else
            {
                assert(false);
            }
        }

        void cancel()
        {
            truncate(0);
        }

        void commit()
        {
            if (size() > 0)
            {
                m_commit_fn(this);
                // prevent multiple commits
                truncate(0);
            }
        }

        void commit(const size_t final_size)
        {
            truncate(final_size);
            commit();
        }

        ~buffer_reserve()
        {
            commit();
        }

        view_t& view()
        {
            return m_view;
        }

        const view_t& view() const
        {
            return m_view;
        }

        reference front()
        {
            return view().front();
        }

        const_reference front() const
        {
            return view().front();
        }

        reference back()
        {
            return view().back();
        }

        const_reference back() const
        {
            return view().back();
        }

        pointer data()
        {
            return view().data();
        }

        const_pointer data() const
        {
            return view().data();
        }

        iterator begin()
        {
            return view().begin();
        }

        const_iterator begin() const
        {
            return view().begin();
        }

        const_iterator cbegin() const
        {
            return view().cbegin();
        }

        iterator end()
        {
            return view().end();
        }

        const_iterator end() const
        {
            return view().end();
        }

        const_iterator cend() const
        {
            return view().cend();
        }

        reverse_iterator rbegin()
        {
            return view().rbegin();
        }

        const_reverse_iterator rbegin() const
        {
            return view().rbegin();
        }

        const_reverse_iterator crbegin() const
        {
            return view().crbegin();
        }

        reverse_iterator rend()
        {
            return view().rend();
        }

        const_reverse_iterator rend() const
        {
            return view().rend();
        }

        const_reverse_iterator crend() const
        {
            return view().crend();
        }

        bool empty() const
        {
            return view().empty();
        }

        size_t size() const
        {
            return view().size();
        }

        size_t max_size() const
        {
            return view().max_size();
        }

        reference operator[](const size_t i)
        {
            return view()[i];
        }

        const_reference operator[](const size_t i) const
        {
            return view()[i];
        }

        reference at(const size_t i)
        {
            return view().at(i);
        }

        const_reference at(const size_t i) const
        {
            return view().at(i);
        }

    private:
        view_t m_view = {};
        commit_fn_t m_commit_fn;
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

        using reader_t = buffer_reserve<ibip_buffer_spsc_atomic, const T>;
        using writer_t = buffer_reserve<ibip_buffer_spsc_atomic, T>;

        reader_t read_reserve(size_type max_reserve_size)
        {
            size_type reserve_size = max_reserve_size;
            auto rindex = get_read_reserve(&reserve_size);
            return reader_t(*this, &ibip_buffer_spsc_atomic::read_commit, const_cast<const T*>(p_buffer + rindex), reserve_size);
        }

        writer_t write_reserve(size_type max_reserve_size)
        {
            size_type reserve_size = max_reserve_size;
            auto windex = get_write_reserve(&reserve_size);
            return writer_t(*this, &ibip_buffer_spsc_atomic::write_commit, const_cast<T*>(p_buffer + windex), reserve_size);
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

        void write_commit(writer_t* writer)
        {
            size_type windex = etl::distance(const_cast<T*>(p_buffer), writer->begin());
            apply_write_reserve(windex, writer->size());
        }

        void read_commit(reader_t* reader)
        {
            size_type rindex = etl::distance(const_cast<const T*>(p_buffer), reader->begin());
            apply_read_reserve(rindex, reader->size());
        }

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
