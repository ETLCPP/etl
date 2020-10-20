///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2020 jwellbelove

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

#ifndef ETL_CIRCULAR_BUFFER_INCLUDED
#define ETL_CIRCULAR_BUFFER_INCLUDED

#include "platform.h"
#include "vector.h"
#include "exception.h"
#include "error_handler.h"
#include "memory.h"
#include "memory_model.h"
#include "type_traits.h"
#include "iterator.h"

#undef ETL_FILE
#define ETL_FILE "56"

namespace etl
{
  //***************************************************************************
  /// Exception for the circular_buffer.
  //***************************************************************************
  class circular_buffer_exception : public etl::exception
  {
  public:

    circular_buffer_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Empty exception for the circular_buffer.
  //***************************************************************************
  class circular_buffer_empty : public etl::circular_buffer_exception
  {
  public:

    circular_buffer_empty(string_type file_name_, numeric_type line_number_)
      : etl::circular_buffer_exception(ETL_ERROR_TEXT("circular_buffer:empty", ETL_FILE"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  ///
  //***************************************************************************
  class circular_buffer_base
  {
  public:

    /// The type used for determining the size of queue.
    typedef size_t size_type;

    //*************************************************************************
    size_type size() const
    {
      return (in >= out) ? in - out : BUFFER_SIZE - (out - in);
    }

    //*************************************************************************
    bool empty() const
    {
      return in == out;
    }

    //*************************************************************************
    bool full() const
    {
      return (in + 1U) % BUFFER_SIZE == out;
    }

    //*************************************************************************
    size_type max_size() const
    {
      return BUFFER_SIZE - 1U;
    }

    //*************************************************************************
    size_type capacity() const
    {
      return BUFFER_SIZE - 1U;
    }

  protected:

    //*************************************************************************
    circular_buffer_base(size_type BUFFER_SIZE_)
      : BUFFER_SIZE(BUFFER_SIZE_)
      , in(0U)
      , out(0U)
    {
    }

    const size_type BUFFER_SIZE;
    size_type in;            ///< Index to the next write.
    size_type out;           ///< Index to the next read.
    ETL_DECLARE_DEBUG_COUNT; ///< Internal debugging.
  };

  //***************************************************************************
  ///
  //***************************************************************************
  template <typename T>
  class icircular_buffer : public circular_buffer_base
  {
  public:

    typedef T           value_type;
    typedef T&          reference;
    typedef const T&    const_reference;
#if ETL_CPP11_SUPPORTED
    typedef T&&         rvalue_reference;
#endif
    typedef T*          pointer;
    typedef const T*    const_pointer;

    //*************************************************************************
    /// Iterator iterating through the circular buffer.
    //*************************************************************************
    class const_iterator : public etl::iterator<ETL_OR_STD::bidirectional_iterator_tag, const T>
    {
    public:

      friend class icircular_buffer;
      friend class const_cyclic_iterator;

      //*************************************************************************
      /// Constructor
      //*************************************************************************
      const_iterator()
        : picb(ETL_NULLPTR)
        , current(0U)
      {
      }

      //*************************************************************************
      /// * operator
      //*************************************************************************
      const T& operator *() const
      {
        return picb->pbuffer[current];
      }

      //*************************************************************************
      /// -> operator
      //*************************************************************************
      const T* operator ->() const
      {
        return picb->pbuffer[current];
      }

      //*************************************************************************
      /// Pre-increment.
      //*************************************************************************
      const_iterator& operator ++()
      {
        ++current;

        // Did we reach the end of the buffer?
        if (current == picb->BUFFER_SIZE)
        {
          current = 0U;
        }

        return (*this);
      }

      //*************************************************************************
      /// Post increment.
      //*************************************************************************
      const_iterator operator ++(int)
      {
        const_iterator original(*this);

        ++(*this);

        return (original);
      }

      //*************************************************************************
      /// Pre-decrement.
      //*************************************************************************
      const_iterator& operator --()
      {
        // Are we at the end of the buffer?
        if (current == 0U)
        {
          current = picb->BUFFER_SIZE - 1;
        }
        else
        {
          --current;
        }

        return (*this);
      }

      //*************************************************************************
      /// Post increment.
      //*************************************************************************
      const_iterator operator --(int)
      {
        const_iterator original(*this);

        --(*this);

        return (original);
      }

      //*************************************************************************
      /// Add offset.
      //*************************************************************************
      const_iterator& operator +=(int n)
      {
        if (n > 0)
        {
          while (n--)
          {
            ++*this;
          }
        }
        else
        {
          while (n++)
          {
            --*this;
          }
        }

        return (*this);
      }

      //*************************************************************************
      /// Subtract offset.
      //*************************************************************************
      const_iterator& operator -=(int n)
      {
        return (this->operator+=(-n));
      }

      //*************************************************************************
      /// Add offset.
      //*************************************************************************
      friend const_iterator operator +(const const_iterator& lhs, int n)
      {
        const_iterator temp = lhs;

        temp += n;

        return temp;
      }

      //*************************************************************************
      /// Subtract offset.
      //*************************************************************************
      friend const_iterator operator -(const const_iterator& lhs, int n)
      {
        const_iterator temp = lhs;

        temp -= n;

        return temp;
      }

      //*************************************************************************
      /// Equality operator
      //*************************************************************************
      friend bool operator == (const const_iterator& lhs, const const_iterator& rhs)
      {
        return (lhs.current == rhs.current);
      }

      //*************************************************************************
      /// Inequality operator
      //*************************************************************************
      friend bool operator != (const const_iterator& lhs, const const_iterator& rhs)
      {
        return !(lhs == rhs);
      }

    protected:

      //*************************************************************************
      /// Protected constructor. Only icircular_buffer can create one.
      //*************************************************************************
      const_iterator(const icircular_buffer<T>* picb_, size_type current_)
        : picb(picb_)
        , current(current_)
      {
      }

    private:

      const icircular_buffer<T>* picb;
      size_type current;
    };

    friend class const_iterator;

    typedef etl::reverse_iterator<const_iterator> const_reverse_iterator;

    //*************************************************************************
    /// Gets a const iterator to the start of the buffer.
    //*************************************************************************
    const_iterator begin() const
    {
      return const_iterator(this, out);
    }

    //*************************************************************************
    /// Gets a const iterator to the start of the buffer.
    //*************************************************************************
    const_iterator cbegin() const
    {
      return const_iterator(this, out);
    }

    //*************************************************************************
    /// Gets a const iterator to the end of the buffer.
    //*************************************************************************
    const_iterator end() const
    {
      return const_iterator(this, in);
    }

    //*************************************************************************
    /// Gets a const iterator to the end of the buffer.
    //*************************************************************************
    const_iterator cend() const
    {
      return const_iterator(this, in);
    }

    //*************************************************************************
    /// Gets a const reverse iterator to the start of the buffer.
    //*************************************************************************
    const_reverse_iterator rbegin() const
    {
      return const_reverse_iterator(end());
    }

    //*************************************************************************
    /// Gets a const reverse iterator to the start of the buffer.
    //*************************************************************************
    const_reverse_iterator crbegin() const
    {
      return const_reverse_iterator(end());
    }

    //*************************************************************************
    /// Gets a const reverse iterator to the end of the buffer.
    //*************************************************************************
    const_reverse_iterator rend() const
    {
      return const_reverse_iterator(begin());
    }

    //*************************************************************************
    /// Gets a const reverse iterator to the end of the buffer.
    //*************************************************************************
    const_reverse_iterator crend() const
    {
      return const_reverse_iterator(begin());
    }

    //*************************************************************************
    /// Get a const reference to the item at the front of the buffer.
    /// Asserts an error if the buffer is empty.
    //*************************************************************************
    const_reference front() const
    {
      ETL_ASSERT(!empty(), ETL_ERROR(circular_buffer_empty));

      return pbuffer[out];
    }

    //*************************************************************************
    /// Get a const reference to the item at the back of the buffer.
    /// Asserts an error if the buffer is empty.
    //*************************************************************************
    const_reference back() const
    {
      ETL_ASSERT(!empty(), ETL_ERROR(circular_buffer_empty));

      return pbuffer[in == 0U ? BUFFER_SIZE - 1 : in - 1U];
    }

    //*************************************************************************
    /// push.
    /// Adds an item to the buffer.
    /// If the buffer is filled then the oldest item is overwritten.
    //*************************************************************************
    void push(const_reference item)
    {
      ::new (&pbuffer[in]) T(item);
      in = (in + 1U) % BUFFER_SIZE;

      // Did we catch up with the 'out' index?
      if (in == out)
      {
        // Forget about the oldest one.      
        pbuffer[out].~T();
        out = (out + 1U) % BUFFER_SIZE;
      }
      else
      {
        ETL_INCREMENT_DEBUG_COUNT;
      }
    }

#if ETL_CPP11_SUPPORTED
    //*************************************************************************
    /// push.
    /// Adds an item to the buffer.
    /// If the buffer is filled then the oldest item is overwritten.
    //*************************************************************************
    void push(rvalue_reference item)
    {
      ::new (&pbuffer[in]) T(etl::move(item));
      in = (in + 1U) % BUFFER_SIZE;

      // Did we catch up with the 'out' index?
      if (in == out)
      {
        // Forget about the oldest item.
        pbuffer[out].~T();
        out = (out + 1U) % BUFFER_SIZE;
      }
      else
      {
        ETL_INCREMENT_DEBUG_COUNT;
      }
    }
#endif

    //*************************************************************************
    /// Push a buffer from an iterator range.
    //*************************************************************************
    template <typename TIterator>
    void push(TIterator first, const TIterator& last)
    {
      while (first != last)
      {
        push(*first++);
      }
    }

    //*************************************************************************
    /// pop
    //*************************************************************************
    void pop()
    {
      ETL_ASSERT(!empty(), ETL_ERROR(circular_buffer_empty));
      pbuffer[out].~T();
      out = (out + 1U) % BUFFER_SIZE;
      ETL_DECREMENT_DEBUG_COUNT
    }

    //*************************************************************************
    /// pop(n)
    //*************************************************************************
    void pop(size_type n)
    {
      while (n-- != 0U)
      {
        pop();
      }
    }

    //*************************************************************************
    /// Clears the buffer.
    //*************************************************************************
    void clear()
    {
      if ETL_IF_CONSTEXPR(etl::is_trivially_destructible<T>::value)
      {
        in    = 0U;
        out   = 0U;
        ETL_RESET_DEBUG_COUNT
      }
      else
      {
        while (!empty())
        {
          pop();
        }
      }
    }

  protected:

    //*************************************************************************
    /// Protected constructor.
    //*************************************************************************
    icircular_buffer(pointer pbuffer_, size_type max_length)
      : circular_buffer_base(max_length + 1U)
      , pbuffer(pbuffer_)
    {
    }

    pointer pbuffer;

  private:

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
#if defined(ETL_POLYMORPHIC_CIRCULAR_BUFFER) || defined(ETL_POLYMORPHIC_CONTAINERS)
  public:
    virtual ~icircular_buffer()
    {
    }
#else
  protected:
    ~icircular_buffer()
    {
    }
#endif
  };

  //***************************************************************************
  /// A fixed capacity circular buffer.
  //***************************************************************************
  template <typename T, size_t MAX_SIZE_>
  class circular_buffer : public icircular_buffer<T>
  {
  public:

    static ETL_CONSTANT size_type MAX_SIZE = size_type(MAX_SIZE_);

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    circular_buffer()
      : icircular_buffer(reinterpret_cast<T*>(buffer.raw), MAX_SIZE)
    {
    }

    //*************************************************************************
    /// Constructor.
    /// Constructs a buffer from an iterator range.
    //*************************************************************************
    template <typename TIterator>
    circular_buffer(TIterator first, const TIterator& last)
      : icircular_buffer(reinterpret_cast<T*>(buffer.raw), MAX_SIZE)
    {
      while (first != last)
      {
        this->push(*first++);
      }
    }

#if ETL_CPP11_SUPPORTED && ETL_NOT_USING_STLPORT && ETL_USING_STL
    //*************************************************************************
    /// Construct from initializer_list.
    //*************************************************************************
    circular_buffer(std::initializer_list<T> init)
      : icircular_buffer<T>(reinterpret_cast<T*>(buffer.raw), MAX_SIZE)
    {
      this->push(init.begin(), init.end());
    }
#endif

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~circular_buffer()
    {
      this->clear();
    }

  private:

    circular_buffer(const circular_buffer&) ETL_DELETE;
    circular_buffer(circular_buffer&&) ETL_DELETE;

    circular_buffer& operator =(const circular_buffer&) ETL_DELETE;
    circular_buffer& operator =(circular_buffer&&) ETL_DELETE;

    etl::uninitialized_buffer_of<T, MAX_SIZE + 1> buffer;
  };

  //*************************************************************************
  /// Template deduction guides.
  //*************************************************************************
#if ETL_CPP17_SUPPORTED
  template <typename T, typename... Ts>
  circular_buffer(T, Ts...)
    ->circular_buffer<etl::enable_if_t<(etl::is_same_v<T, Ts> && ...), T>, 1U + sizeof...(Ts)>;
#endif 
}

#undef ETL_FILE

#endif