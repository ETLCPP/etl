///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2026 BMW AG

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

#ifndef ETL_FUTURE_INCLUDED
#define ETL_FUTURE_INCLUDED

#include "platform.h"
#include "alignment.h"
#include "atomic.h"
#include "error_handler.h"
#include "exception.h"
#include "file_error_numbers.h"
#include "memory.h"
#include "nullptr.h"
#include "placement_new.h"
#include "static_assert.h"
#include "type_traits.h"
#include "utility.h"

#if ETL_HAS_ATOMIC

namespace etl
{
  //***************************************************************************
  /// Exception types for future/promise.
  //***************************************************************************
  class future_exception : public etl::exception
  {
  public:

    future_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  class future_not_ready : public etl::future_exception
  {
  public:

    future_not_ready(string_type file_name_, numeric_type line_number_)
      : future_exception(ETL_ERROR_TEXT("future:not ready", ETL_FUTURE_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  class promise_already_satisfied : public etl::future_exception
  {
  public:

    promise_already_satisfied(string_type file_name_, numeric_type line_number_)
      : future_exception(ETL_ERROR_TEXT("promise:already satisfied", ETL_FUTURE_FILE_ID"B"), file_name_, line_number_)
    {
    }
  };

  class future_no_state : public etl::future_exception
  {
  public:

    future_no_state(string_type file_name_, numeric_type line_number_)
      : future_exception(ETL_ERROR_TEXT("future:no state", ETL_FUTURE_FILE_ID"C"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Shared state between a promise and a future.
  /// No heap allocation. The storage and lifetime of the shared_state are
  /// managed by the user, e.g. as an element of a fixed-size table.
  ///
  /// Thread safety: set_value() and is_ready()/get() may be called from
  /// different threads provided that at most ONE thread calls set_value()
  /// and at most ONE thread calls get(). The atomic ready flag provides
  /// acquire/release ordering.
  //***************************************************************************
  template <typename T>
  class shared_state
  {
  public:

    ETL_STATIC_ASSERT(!etl::is_array<T>::value, "etl::shared_state: T must not be an array type");

    shared_state()
      : ready_(false)
    {
    }

    ~shared_state()
    {
      if (is_ready())
      {
        ptr()->~T();
      }
    }

    //*************************************************************************
    /// Store the result value and mark the state as ready.
    //*************************************************************************
    void set_value(const T& value)
    {
      ETL_ASSERT(!is_ready(), ETL_ERROR(promise_already_satisfied));
      ::new (static_cast<void*>(&storage_)) T(value);
      ready_.store(true, etl::memory_order_release);
    }

  #if ETL_USING_CPP11
    //*************************************************************************
    /// Store the result value (move) and mark the state as ready.
    //*************************************************************************
    void set_value(T&& value)
    {
      ETL_ASSERT(!is_ready(), ETL_ERROR(promise_already_satisfied));
      ::new (static_cast<void*>(&storage_)) T(etl::move(value));
      ready_.store(true, etl::memory_order_release);
    }
  #endif

    //*************************************************************************
    /// Check whether the result is available.
    //*************************************************************************
    ETL_NODISCARD
    bool is_ready() const ETL_NOEXCEPT
    {
      return ready_.load(etl::memory_order_acquire);
    }

    //*************************************************************************
    /// Get the stored value.
    /// Asserts etl::future_not_ready if the state is not ready.
    //*************************************************************************
    ETL_NODISCARD
    T& get()
    {
      ETL_ASSERT(is_ready(), ETL_ERROR(future_not_ready));
      return *ptr();
    }

    ETL_NODISCARD
    const T& get() const
    {
      ETL_ASSERT(is_ready(), ETL_ERROR(future_not_ready));
      return *ptr();
    }

    //*************************************************************************
    /// Reset for reuse.
    //*************************************************************************
    void reset()
    {
      if (is_ready())
      {
        ptr()->~T();
        ready_.store(false, etl::memory_order_release);
      }
    }

  private:

    //*************************************************************************
    /// Pointer to the object in the storage.
    /// Laundered, as the storage is reused after reset().
    //*************************************************************************
    T* ptr()
    {
      return etl::launder(reinterpret_cast<T*>(&storage_));
    }

    const T* ptr() const
    {
      return etl::launder(reinterpret_cast<const T*>(&storage_));
    }

    // Non-copyable.
    shared_state(const shared_state&) ETL_DELETE;
    shared_state& operator=(const shared_state&) ETL_DELETE;

    typename etl::aligned_storage<sizeof(T), etl::alignment_of<T>::value>::type storage_;
    etl::atomic_bool                                                            ready_;
  };

  //***************************************************************************
  /// Specialisation for references — stores a pointer to the referenced object.
  //***************************************************************************
  template <typename T>
  class shared_state<T&>
  {
  public:

    shared_state()
      : p_value_(ETL_NULLPTR)
      , ready_(false)
    {
    }

    //*************************************************************************
    /// Store the reference and mark the state as ready.
    //*************************************************************************
    void set_value(T& value)
    {
      ETL_ASSERT(!is_ready(), ETL_ERROR(promise_already_satisfied));
      p_value_ = etl::addressof(value);
      ready_.store(true, etl::memory_order_release);
    }

    //*************************************************************************
    /// Check whether the result is available.
    //*************************************************************************
    ETL_NODISCARD
    bool is_ready() const ETL_NOEXCEPT
    {
      return ready_.load(etl::memory_order_acquire);
    }

    //*************************************************************************
    /// Get the stored reference.
    /// Asserts etl::future_not_ready if the state is not ready.
    //*************************************************************************
    ETL_NODISCARD
    T& get() const
    {
      ETL_ASSERT(is_ready(), ETL_ERROR(future_not_ready));
      return *p_value_;
    }

    //*************************************************************************
    /// Reset for reuse.
    //*************************************************************************
    void reset()
    {
      ready_.store(false, etl::memory_order_release);
      p_value_ = ETL_NULLPTR;
    }

  private:

    // Non-copyable.
    shared_state(const shared_state&) ETL_DELETE;
    shared_state& operator=(const shared_state&) ETL_DELETE;

    T*               p_value_;
    etl::atomic_bool ready_;
  };

  //***************************************************************************
  /// Specialisation for void — no value storage, just a ready flag.
  //***************************************************************************
  template <>
  class shared_state<void>
  {
  public:

    shared_state()
      : ready_(false)
    {
    }

    void set_value()
    {
      ETL_ASSERT(!is_ready(), ETL_ERROR(promise_already_satisfied));
      ready_.store(true, etl::memory_order_release);
    }

    ETL_NODISCARD
    bool is_ready() const ETL_NOEXCEPT
    {
      return ready_.load(etl::memory_order_acquire);
    }

    void reset()
    {
      ready_.store(false, etl::memory_order_release);
    }

  private:

    shared_state(const shared_state&) ETL_DELETE;
    shared_state& operator=(const shared_state&) ETL_DELETE;

    etl::atomic_bool ready_;
  };

  namespace private_future
  {
    //*************************************************************************
    /// The result type of future<T>::get().
    /// const T& for values, R& for T = R&.
    /// (C++03 has no reference collapsing, so const T& cannot be used for both.)
    //*************************************************************************
    template <typename T>
    struct future_get_result
    {
      typedef const T& type;
    };

    template <typename T>
    struct future_get_result<T&>
    {
      typedef T& type;
    };
  } // namespace private_future

  //***************************************************************************
  /// A lightweight future — a non-owning handle to a shared_state<T>.
  /// The shared_state lifetime must be managed externally.
  /// Has the semantics of std::shared_future: it is copyable, get() does not
  /// consume the value and may be called more than once, by every copy.
  /// get() returns const T&, or R& for T = R&.
  //***************************************************************************
  template <typename T>
  class future
  {
  public:

    future() ETL_NOEXCEPT
      : state_(ETL_NULLPTR)
    {
    }

    explicit future(shared_state<T>& state) ETL_NOEXCEPT
      : state_(&state)
    {
    }

    //*************************************************************************
    /// Returns true when the result is available.
    //*************************************************************************
    ETL_NODISCARD
    bool is_ready() const ETL_NOEXCEPT
    {
      return (state_ != ETL_NULLPTR) && state_->is_ready();
    }

    //*************************************************************************
    /// Returns true if this future is associated with a shared state.
    //*************************************************************************
    ETL_NODISCARD
    bool valid() const ETL_NOEXCEPT
    {
      return state_ != ETL_NULLPTR;
    }

    //*************************************************************************
    /// Get the result.
    /// Asserts etl::future_no_state if not valid (no shared state).
    /// Asserts etl::future_not_ready if valid but the result is not ready.
    //*************************************************************************
    ETL_NODISCARD
    typename private_future::future_get_result<T>::type get() const
    {
      ETL_ASSERT(valid(), ETL_ERROR(future_no_state));
      ETL_ASSERT(!valid() || state_->is_ready(), ETL_ERROR(future_not_ready));
      return state_->get();
    }

  private:

    shared_state<T>* state_;
  };

  //***************************************************************************
  /// Specialisation for void.
  //***************************************************************************
  template <>
  class future<void>
  {
  public:

    future() ETL_NOEXCEPT
      : state_(ETL_NULLPTR)
    {
    }

    explicit future(shared_state<void>& state) ETL_NOEXCEPT
      : state_(&state)
    {
    }

    ETL_NODISCARD
    bool is_ready() const ETL_NOEXCEPT
    {
      return (state_ != ETL_NULLPTR) && state_->is_ready();
    }

    ETL_NODISCARD
    bool valid() const ETL_NOEXCEPT
    {
      return state_ != ETL_NULLPTR;
    }

    //*************************************************************************
    /// Asserts etl::future_no_state if not valid (no shared state).
    /// Asserts etl::future_not_ready if valid but the result is not ready.
    //*************************************************************************
    void get() const
    {
      ETL_ASSERT(valid(), ETL_ERROR(future_no_state));
      ETL_ASSERT(!valid() || state_->is_ready(), ETL_ERROR(future_not_ready));
    }

  private:

    shared_state<void>* state_;
  };

  namespace private_future
  {
    //*************************************************************************
    /// Common base of the promise specialisations.
    /// Like std::promise, a promise is move-only (C++11), so there is only
    /// ever one producer. Non-copyable in C++03.
    //*************************************************************************
    template <typename T>
    class promise_base
    {
    public:

      //***********************************************************************
      /// Returns true if the promise is associated with a shared state.
      //***********************************************************************
      ETL_NODISCARD
      bool valid() const ETL_NOEXCEPT
      {
        return state_ != ETL_NULLPTR;
      }

      //***********************************************************************
      /// Get the associated future.
      /// Asserts etl::future_no_state if the promise is not valid.
      //***********************************************************************
      ETL_NODISCARD
      etl::future<T> get_future()
      {
        ETL_ASSERT(valid(), ETL_ERROR(future_no_state));
        return etl::future<T>(*state_);
      }

    protected:

      promise_base() ETL_NOEXCEPT
        : state_(ETL_NULLPTR)
      {
      }

      explicit promise_base(etl::shared_state<T>& state) ETL_NOEXCEPT
        : state_(&state)
      {
      }

  #if ETL_USING_CPP11
      //***********************************************************************
      /// Move constructor. The moved from promise is no longer valid.
      //***********************************************************************
      promise_base(promise_base&& other) ETL_NOEXCEPT
        : state_(other.state_)
      {
        other.state_ = ETL_NULLPTR;
      }

      //***********************************************************************
      /// Move assignment. The moved from promise is no longer valid.
      //***********************************************************************
      promise_base& operator=(promise_base&& other) ETL_NOEXCEPT
      {
        if (this != &other)
        {
          state_       = other.state_;
          other.state_ = ETL_NULLPTR;
        }

        return *this;
      }
  #endif

      etl::shared_state<T>* state_;

    private:

      // Non-copyable.
      promise_base(const promise_base&) ETL_DELETE;
      promise_base& operator=(const promise_base&) ETL_DELETE;
    };
  } // namespace private_future

  //***************************************************************************
  /// A lightweight promise — a non-owning handle that can set the value
  /// in a shared_state<T>.
  //***************************************************************************
  template <typename T>
  class promise : public private_future::promise_base<T>
  {
  public:

    promise() ETL_NOEXCEPT
      : private_future::promise_base<T>()
    {
    }

    explicit promise(etl::shared_state<T>& state) ETL_NOEXCEPT
      : private_future::promise_base<T>(state)
    {
    }

    //*************************************************************************
    /// Set the value (fulfil the promise).
    //*************************************************************************
    void set_value(const T& value)
    {
      ETL_ASSERT(this->valid(), ETL_ERROR(future_no_state));
      this->state_->set_value(value);
    }

  #if ETL_USING_CPP11
    void set_value(T&& value)
    {
      ETL_ASSERT(this->valid(), ETL_ERROR(future_no_state));
      this->state_->set_value(etl::move(value));
    }
  #endif
  };

  //***************************************************************************
  /// Specialisation for references.
  //***************************************************************************
  template <typename T>
  class promise<T&> : public private_future::promise_base<T&>
  {
  public:

    promise() ETL_NOEXCEPT
      : private_future::promise_base<T&>()
    {
    }

    explicit promise(etl::shared_state<T&>& state) ETL_NOEXCEPT
      : private_future::promise_base<T&>(state)
    {
    }

    void set_value(T& value)
    {
      ETL_ASSERT(this->valid(), ETL_ERROR(future_no_state));
      this->state_->set_value(value);
    }
  };

  //***************************************************************************
  /// Specialisation for void.
  //***************************************************************************
  template <>
  class promise<void> : public private_future::promise_base<void>
  {
  public:

    promise() ETL_NOEXCEPT
      : private_future::promise_base<void>()
    {
    }

    explicit promise(etl::shared_state<void>& state) ETL_NOEXCEPT
      : private_future::promise_base<void>(state)
    {
    }

    void set_value()
    {
      ETL_ASSERT(this->valid(), ETL_ERROR(future_no_state));
      this->state_->set_value();
    }
  };

} // namespace etl

#endif // ETL_HAS_ATOMIC

#endif // ETL_FUTURE_INCLUDED
