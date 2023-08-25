///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2015 John Wellbelove

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

#ifndef ETL_OPTIONAL_INCLUDED
#define ETL_OPTIONAL_INCLUDED

#include "platform.h"
#include "alignment.h"
#include "memory.h"
#include "type_traits.h"
#include "exception.h"
#include "error_handler.h"
#include "utility.h"
#include "placement_new.h"

namespace etl
{
  //*****************************************************************************
  /// A null option type.
  ///\ingroup utilities
  //*****************************************************************************
  class nullopt_t
  {
  public:

    // Convertible to any type of null non-member pointer.
    template<class T>
    operator T*() const
    {
      return 0;
    }

  private:

    // Can't take address of nullopt.
    void operator&() const ETL_DELETE;
  };

  //*****************************************************************************
  /// A null option.
  ///\ingroup utilities
  //*****************************************************************************
  const nullopt_t nullopt = {};

  //***************************************************************************
  /// Exception for optional.
  ///\ingroup list
  //***************************************************************************
  class optional_exception : public exception
  {
  public:

    optional_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Invalid exception for optional.
  ///\ingroup list
  //***************************************************************************
  class optional_invalid : public optional_exception
  {
  public:

    optional_invalid(string_type file_name_, numeric_type line_number_)
      : optional_exception("optional:invalid", file_name_, line_number_)
    {
    }
  };

  //*****************************************************************************
  /// An optional type.
  /// If the optional type is not initialised then a type is not constructed.
  /// See http://en.cppreference.com/w/cpp/utility/optional
  ///\tparam T The type to store.
  ///\ingroup utilities
  //*****************************************************************************
  template <typename T, bool is_pod = etl::is_pod<T>::value>
  class optional;

  //*****************************************************************************
  /// For non POD types.
  //*****************************************************************************
  template <typename T>
  class optional<T, false>
  {
  public:

    typedef T value_type;

    //***************************************************************************
    /// Constructor.
    //***************************************************************************
    ETL_CONSTEXPR
    optional()
      : storage()
    {
    }

    //***************************************************************************
    /// Constructor with nullopt.
    //***************************************************************************
    ETL_CONSTEXPR
    optional(etl::nullopt_t)
      : storage()
    {
    }

#include "etl/private/diagnostic_uninitialized_push.h"
    //***************************************************************************
    /// Copy constructor.
    //***************************************************************************
    ETL_CONSTEXPR20_STL
    optional(const optional& other)
    {
      if (other.has_value())
      {
        storage.construct(other.value());
      }
    }
#include "etl/private/diagnostic_pop.h"

#if ETL_USING_CPP11
    //***************************************************************************
    /// Move constructor.
    //***************************************************************************
    ETL_CONSTEXPR20_STL
    optional(optional&& other)
    {
      if (other.has_value())
      {
        storage.construct(etl::move(other.value()));
      }
    }
#endif

    //***************************************************************************
    /// Constructor from value type.
    //***************************************************************************
    ETL_CONSTEXPR20_STL
    optional(const T& value_)
    {
      storage.construct(value_);
    }

#if ETL_USING_CPP11
    //***************************************************************************
    /// Constructor from value type.
    //***************************************************************************
    ETL_CONSTEXPR20_STL
    optional(T&& value_)
    {
      storage.construct(etl::move(value_));
    }
#endif

    //***************************************************************************
    /// Destructor.
    //***************************************************************************
    ETL_CONSTEXPR20_STL
    ~optional()
    {
      storage.destroy();
    }

    //***************************************************************************
    /// Assignment operator from nullopt.
    //***************************************************************************
    ETL_CONSTEXPR20_STL
    optional& operator =(etl::nullopt_t)
    {
      if (has_value())
      {
        storage.destroy();
      }

      return *this;
    }

    //***************************************************************************
    /// Assignment operator from optional.
    //***************************************************************************
    ETL_CONSTEXPR20_STL
    optional& operator =(const optional& other)
    {
      if (this != &other)
      {
        if (other.has_value())
        {
          storage.construct(other.value());
        }
        else
        {
          storage.destroy();
        }
      }

      return *this;
    }

#if ETL_USING_CPP11
    //***************************************************************************
    /// Assignment operator from optional.
    //***************************************************************************
    ETL_CONSTEXPR20_STL
    optional& operator =(optional&& other)
    {
      if (this != &other)
      {
        if (other.has_value())
        {
          storage.construct(etl::move(other.value()));
        }
        else
        {
          storage.destroy();
        }
      }

      return *this;
    }
#endif

    //***************************************************************************
    /// Assignment operator from value type.
    //***************************************************************************
    ETL_CONSTEXPR20_STL
    optional& operator =(const T& value_)
    {
      storage.construct(value_);

      return *this;
    }

#if ETL_USING_CPP11
    //***************************************************************************
    /// Assignment operator from value type.
    //***************************************************************************
    ETL_CONSTEXPR20_STL
    optional& operator =(T&& value_)
    {
      storage.construct(etl::move(value_));

      return *this;
    }
#endif

    //***************************************************************************
    /// Pointer operator.
    //***************************************************************************
    ETL_CONSTEXPR20_STL
    T* operator ->()
    {
#if ETL_IS_DEBUG_BUILD && !(ETL_USING_CPP20 && ETL_USING_STL)
      ETL_ASSERT(has_value(), ETL_ERROR(optional_invalid));
#endif

      return &storage.u.value;
    }

    //***************************************************************************
    /// Pointer operator.
    //***************************************************************************
    ETL_CONSTEXPR20_STL
    const T* operator ->() const
    {
#if ETL_IS_DEBUG_BUILD && !(ETL_USING_CPP20 && ETL_USING_STL)
      ETL_ASSERT(has_value(), ETL_ERROR(optional_invalid));
#endif

      return &storage.u.value;
    }

    //***************************************************************************
    /// Dereference operator.
    //***************************************************************************
    ETL_CONSTEXPR20_STL
    T& operator *() ETL_LVALUE_REF_QUALIFIER
    {
#if ETL_IS_DEBUG_BUILD && !(ETL_USING_CPP20 && ETL_USING_STL)
      ETL_ASSERT(has_value(), ETL_ERROR(optional_invalid));
#endif

      return storage.u.value;
    }

    //***************************************************************************
    /// Dereference operator.
    //***************************************************************************
    ETL_CONSTEXPR20_STL
    const T& operator *() const ETL_LVALUE_REF_QUALIFIER
    {
#if ETL_IS_DEBUG_BUILD && !(ETL_USING_CPP20 && ETL_USING_STL)
      ETL_ASSERT(has_value(), ETL_ERROR(optional_invalid));
#endif

      return storage.u.value;
    }

#if ETL_USING_CPP11
    //***************************************************************************
    /// Dereference operator.
    //***************************************************************************
    ETL_CONSTEXPR20_STL
    T&& operator *()&&
    {
#if ETL_IS_DEBUG_BUILD && !(ETL_USING_CPP20 && ETL_USING_STL)
      ETL_ASSERT(has_value(), ETL_ERROR(optional_invalid));
#endif

      return etl::move(storage.u.value);
    }

    //***************************************************************************
    /// Dereference operator.
    //***************************************************************************
    ETL_CONSTEXPR20_STL
    const T&& operator *() const&&
    {
#if ETL_IS_DEBUG_BUILD && !(ETL_USING_CPP20 && ETL_USING_STL)
      ETL_ASSERT(has_value(), ETL_ERROR(optional_invalid));
#endif

      return etl::move(storage.u.value);
    }
#endif

    //***************************************************************************
    // Check whether optional contains value
    //***************************************************************************
    ETL_CONSTEXPR20_STL
    bool has_value() const ETL_NOEXCEPT
    {
      return storage.valid;
    }

    //***************************************************************************
    /// Bool conversion operator.
    //***************************************************************************
    ETL_CONSTEXPR20_STL
    ETL_EXPLICIT operator bool() const
    {
      return has_value();
    }

    //***************************************************************************
    /// Get a reference to the value.
    //***************************************************************************
    ETL_CONSTEXPR20_STL
    T& value() ETL_LVALUE_REF_QUALIFIER
    {
#if ETL_IS_DEBUG_BUILD
      ETL_ASSERT(has_value(), ETL_ERROR(optional_invalid));
#endif

      return storage.u.value;
    }

    //***************************************************************************
    /// Get a const reference to the value.
    //***************************************************************************
    ETL_CONSTEXPR20_STL
    const T& value() const ETL_LVALUE_REF_QUALIFIER
    {
#if ETL_IS_DEBUG_BUILD
      ETL_ASSERT(has_value(), ETL_ERROR(optional_invalid));
#endif

      return storage.u.value;
    }

    //***************************************************************************
    /// Gets the value or a default if not valid.
    //***************************************************************************
    ETL_CONSTEXPR20_STL
    T value_or(const T& default_value) const ETL_LVALUE_REF_QUALIFIER
    {
      return has_value() ? value() : default_value;
    }

#if ETL_USING_CPP11
    //***************************************************************************
    /// Get an rvalue reference to the value.
    //***************************************************************************
    ETL_CONSTEXPR20_STL
    T&& value()&&
    {
#if ETL_IS_DEBUG_BUILD
      ETL_ASSERT(has_value(), ETL_ERROR(optional_invalid));
#endif

      return etl::move(storage.u.value);
    }

    //***************************************************************************
    /// Get a const rvalue reference to the value.
    //***************************************************************************
    ETL_CONSTEXPR20_STL
    const T&& value() const&&
    {
#if ETL_IS_DEBUG_BUILD
      ETL_ASSERT(has_value(), ETL_ERROR(optional_invalid));
#endif

      return etl::move(storage.u.value);
    }

    //***************************************************************************
    /// Gets the value or a default if not valid.
    //***************************************************************************
    template <typename U>
    ETL_CONSTEXPR20_STL
    etl::enable_if_t<etl::is_convertible<U, T>::value, T>
      value_or(U&& default_value) const&
    {
      return has_value() ? value() : etl::forward<T>(default_value);
    }

    //***************************************************************************
    /// Gets the value or a default if not valid.
    //***************************************************************************
    template <typename U>
    ETL_CONSTEXPR20_STL
    etl::enable_if_t<etl::is_convertible<U, T>::value, T>
      value_or(U&& default_value) &&
    {
      return has_value() ? etl::move(value()) : etl::forward<T>(default_value);
    }
#endif

    //***************************************************************************
    /// Swaps this value with another.
    //***************************************************************************
    ETL_CONSTEXPR20_STL
    void swap(optional& other)
    {
      optional temp(*this);
      *this = other;
      other = temp;
    }

    //***************************************************************************
    /// Reset back to invalid.
    //***************************************************************************
    ETL_CONSTEXPR20_STL
    void reset()
    {
      storage.destroy();
    }

#if ETL_USING_CPP11  && ETL_NOT_USING_STLPORT && !defined(ETL_OPTIONAL_FORCE_CPP03_IMPLEMENTATION)
    //*************************************************************************
    /// Emplaces a value.
    ///\param args The arguments to construct with.
    //*************************************************************************   
    template <typename ... TArgs>
    ETL_CONSTEXPR20_STL
    void emplace(TArgs&& ... args)
    {
      storage.construct(etl::forward<TArgs>(args)...);
    }
#else
    //*************************************************************************
    /// Emplaces a value.
    /// 1 parameter.
    //*************************************************************************
    T& emplace()
    {
      if (has_value())
      {
        // Destroy the old one.
        storage.destroy();
      }

      T* p = ::new (&storage.u.value) T();
      storage.valid = true;

      return *p;
    }

    //*************************************************************************
    /// Emplaces a value.
    /// 1 parameter.
    //*************************************************************************
    template <typename T1>
    T& emplace(const T1& value1)
    {
      if (has_value())
      {
        // Destroy the old one.
        storage.destroy();
      }

      T* p = ::new (&storage.u.value) T(value1);
      storage.valid = true;

      return *p;
    }

    //*************************************************************************
    /// Emplaces a value.
    /// 2 parameters.
    //*************************************************************************
    template <typename T1, typename T2>
    T& emplace(const T1& value1, const T2& value2)
    {
      if (has_value())
      {
        // Destroy the old one.
        storage.destroy();
      }

      T* p = ::new (&storage.u.value) T(value1, value2);
      storage.valid = true;

      return *p;
    }

    //*************************************************************************
    /// Emplaces a value.
    /// 3 parameters.
    //*************************************************************************
    template <typename T1, typename T2, typename T3>
    T& emplace(const T1& value1, const T2& value2, const T3& value3)
    {
      if (has_value())
      {
        // Destroy the old one.
        storage.destroy();
      }

      T* p = ::new (&storage.u.value) T(value1, value2, value3);
      storage.valid = true;

      return *p;
    }

    //*************************************************************************
    /// Emplaces a value.
    /// 4 parameters.
    //*************************************************************************
    template <typename T1, typename T2, typename T3, typename T4>
    T& emplace(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
    {
      if (has_value())
      {
        // Destroy the old one.
        storage.destroy();
      }

      T* p = ::new (&storage.u.value) T(value1, value2, value3, value4);
      storage.valid = true;

      return *p;
    }
#endif

  private:

    struct storage_type
    {
      //*******************************
      ETL_CONSTEXPR20_STL
      storage_type()
        : u()
        , valid(false)
      {
      }

      //*******************************
      ETL_CONSTEXPR20_STL
      ~storage_type()
      {
        destroy();
      }

      //*******************************
      ETL_CONSTEXPR20_STL
      void construct(const T& value_)
      {
        if (valid)
        {
          u.value = value_;
        }
        else
        {
          etl::construct_at(&u.value, value_);
          valid = true;
        }
      }

#if ETL_USING_CPP11
      //*******************************
      ETL_CONSTEXPR20_STL
      void construct(T&& value_)
      {
        if (valid)
        {
          u.value = etl::move(value_);
        }
        else
        {
          etl::construct_at(&u.value, etl::move(value_));
          valid = true;
        }
      }

      //*******************************
      template <typename... TArgs>
      ETL_CONSTEXPR20_STL
      void construct(TArgs&&... args)
      {
        destroy();
        etl::construct_at(&u.value, etl::forward<TArgs>(args)...);
        valid = true;
      }
#endif

      //*******************************
      ETL_CONSTEXPR20_STL
      void destroy()
      {
        if (valid)
        {
          etl::destroy_at(&u.value);
          valid = false;
        }
      }

      //*******************************
      union union_type
      {
        ETL_CONSTEXPR20_STL
        union_type()
          : dummy(0)
        {
        }

        ETL_CONSTEXPR20_STL
        ~union_type()
        {
        }

        char dummy;
        T    value;
      } u;

      bool valid;
    };

    storage_type storage;
  };

  //*****************************************************************************
  /// For POD types.
  ///\tparam T The type to store.
  ///\ingroup utilities
  //*****************************************************************************
  template <typename T>
  class optional<T, true>
  {
  public:

    typedef T value_type;

    //***************************************************************************
    /// Constructor.
    //***************************************************************************
    ETL_CONSTEXPR14 optional()
      : valid(false)
      , storage()
    {
    }

    //***************************************************************************
    /// Constructor with nullopt.
    //***************************************************************************
    ETL_CONSTEXPR14 optional(etl::nullopt_t)
      : valid(false)
      , storage()
    {
    }

    //***************************************************************************
    /// Copy constructor.
    //***************************************************************************
    ETL_CONSTEXPR14 optional(const optional& other)
      : valid(bool(other))
      , storage(other.storage)
    {
    }

#if ETL_USING_CPP11
    //***************************************************************************
    /// Move constructor.
    //***************************************************************************
    ETL_CONSTEXPR14 optional(optional&& other)
      : valid(bool(other))
      , storage(etl::move(other.storage))
    {
    }
#endif

    //***************************************************************************
    /// Constructor from value type.
    //***************************************************************************
    ETL_CONSTEXPR14 optional(const T& value_)
      : valid(true)
    {
      storage.u.value = value_;
    }

#if ETL_USING_CPP11
    //***************************************************************************
    /// Constructor from value type.
    //***************************************************************************
    ETL_CONSTEXPR14 optional(T&& value_)
      : valid(true)
    {
      storage.u.value = etl::move(value_);
    }
#endif

    //***************************************************************************
    /// Assignment operator from nullopt.
    //***************************************************************************
    ETL_CONSTEXPR14 optional& operator =(etl::nullopt_t)
    {
      valid = false;
      return *this;
    }

    //***************************************************************************
    /// Assignment operator from optional.
    //***************************************************************************
    ETL_CONSTEXPR14 optional& operator =(const optional& other)
    {
      if (this != &other)
      {
        storage.u = other.storage.u;
        valid   = other.valid;
      }

      return *this;
    }

#if ETL_USING_CPP11
    //***************************************************************************
    /// Assignment operator from optional.
    //***************************************************************************
    ETL_CONSTEXPR14 optional& operator =(optional&& other)
    {
      if (this != &other)
      {
        storage.u = etl::move(other.storage.u);
        valid   = other.valid;
      }

      return *this;
    }
#endif

    //***************************************************************************
    /// Assignment operator from value type.
    //***************************************************************************
    ETL_CONSTEXPR14 optional& operator =(const T& value_)
    {
      storage.u.value = value_;
      valid = true;

      return *this;
    }

#if ETL_USING_CPP11
    //***************************************************************************
    /// Assignment operator from value type.
    //***************************************************************************
    ETL_CONSTEXPR14 optional& operator =(T&& value_)
    {
      storage.u.value = etl::move(value_);
      valid = true;

      return *this;
    }
#endif

    //***************************************************************************
    /// Pointer operator.
    //***************************************************************************
    ETL_CONSTEXPR14 T* operator ->()
    {
#if ETL_IS_DEBUG_BUILD
      ETL_ASSERT(valid, ETL_ERROR(optional_invalid));
#endif

      return &storage.u.value;
    }

    //***************************************************************************
    /// Pointer operator.
    //***************************************************************************
    ETL_CONSTEXPR14 const T* operator ->() const
    {
#if ETL_IS_DEBUG_BUILD
      ETL_ASSERT(valid, ETL_ERROR(optional_invalid));
#endif

      return &storage.u.value;
    }

    //***************************************************************************
    /// Dereference operator.
    //***************************************************************************
    ETL_CONSTEXPR14 T& operator *() ETL_LVALUE_REF_QUALIFIER
    {
#if ETL_IS_DEBUG_BUILD
      ETL_ASSERT(valid, ETL_ERROR(optional_invalid));
#endif

      return storage.u.value;
    }

    //***************************************************************************
    /// Dereference operator.
    //***************************************************************************
    ETL_CONSTEXPR14 const T& operator *() const ETL_LVALUE_REF_QUALIFIER
    {
#if ETL_IS_DEBUG_BUILD
      ETL_ASSERT(valid, ETL_ERROR(optional_invalid));
#endif

      return storage.u.value;
    }

#if ETL_USING_CPP11
    //***************************************************************************
    /// Dereference operator.
    //***************************************************************************
    ETL_CONSTEXPR14 T&& operator *()&&
    {
#if ETL_IS_DEBUG_BUILD
      ETL_ASSERT(valid, ETL_ERROR(optional_invalid));
#endif

      return etl::move(storage.u.value);
    }

    //***************************************************************************
    /// Dereference operator.
    //***************************************************************************
    ETL_CONSTEXPR14 const T&& operator *() const&&
    {
#if ETL_IS_DEBUG_BUILD
      ETL_ASSERT(valid, ETL_ERROR(optional_invalid));
#endif

      return etl::move(storage.u.value);
    }
#endif

    //***************************************************************************
    /// Bool conversion operator.
    //***************************************************************************
    ETL_CONSTEXPR14 
    ETL_EXPLICIT operator bool() const
    {
      return valid;
    }

    //***************************************************************************
    // Check whether optional contains value
    //***************************************************************************
    ETL_CONSTEXPR14 bool has_value() const ETL_NOEXCEPT
    {
      return valid;
    }

    //***************************************************************************
    /// Get a reference to the value.
    //***************************************************************************
    ETL_CONSTEXPR14 T& value() ETL_LVALUE_REF_QUALIFIER
    {
#if ETL_IS_DEBUG_BUILD
      ETL_ASSERT(valid, ETL_ERROR(optional_invalid));
#endif

      return storage.u.value;
    }

    //***************************************************************************
    /// Get a const reference to the value.
    //***************************************************************************
    ETL_CONSTEXPR14 const T& value() const ETL_LVALUE_REF_QUALIFIER
    {
#if ETL_IS_DEBUG_BUILD
      ETL_ASSERT(valid, ETL_ERROR(optional_invalid));
#endif

      return storage.u.value;
    }

    //***************************************************************************
    /// Gets the value or a default if no valid.
    //***************************************************************************
    ETL_CONSTEXPR14 T value_or(const T& default_value) const ETL_LVALUE_REF_QUALIFIER
    {
      return valid ? value() : default_value;
    }

#if ETL_USING_CPP11
    //***************************************************************************
    /// Get an rvalue reference to the value.
    //***************************************************************************
    ETL_CONSTEXPR14 T&& value()&&
    {
#if ETL_IS_DEBUG_BUILD
      ETL_ASSERT(valid, ETL_ERROR(optional_invalid));
#endif

      return etl::move(storage.u.value);
    }

    //***************************************************************************
    /// Get a const rvalue reference to the value.
    //***************************************************************************
    ETL_CONSTEXPR14 const T&& value() const&&
    {
#if ETL_IS_DEBUG_BUILD
      ETL_ASSERT(valid, ETL_ERROR(optional_invalid));
#endif

      return etl::move(storage.u.value);
    }

    //***************************************************************************
    /// Gets the value or a default if not valid.
    //***************************************************************************
    template <typename U>
    ETL_CONSTEXPR20_STL
    etl::enable_if_t<etl::is_convertible<U, T>::value, T>
      value_or(U&& default_value) const&
    {
      if (has_value())
      {
        return value();
      }
      else
      {
        return static_cast<T>(etl::forward<U>(default_value));
      }
    }

    //***************************************************************************
    /// Gets the value or a default if not valid.
    //***************************************************************************
    template <typename U>
    ETL_CONSTEXPR20_STL
    etl::enable_if_t<etl::is_convertible<U, T>::value, T>
      value_or(U&& default_value) &&
    {
      if (has_value())
      {
        return etl::move(value());
      }
      else
      {
        return static_cast<T>(etl::forward<U>(default_value));
      }
    }
#endif

    //***************************************************************************
    /// Swaps this value with another.
    //***************************************************************************
    ETL_CONSTEXPR14 void swap(optional& other)
    {
      optional temp(*this);
      *this = other;
      other = temp;
    }

    //***************************************************************************
    /// Reset back to invalid.
    //***************************************************************************
    ETL_CONSTEXPR14 void reset()
    {
      valid = false;
    }

#if ETL_USING_CPP11  && ETL_NOT_USING_STLPORT && !defined(ETL_OPTIONAL_FORCE_CPP03_IMPLEMENTATION)
    //*************************************************************************
    /// Emplaces a value.
    ///\param args The arguments to construct with.
    //*************************************************************************
    template <typename ... Args>
    ETL_CONSTEXPR14 void emplace(Args && ... args)
    {
      storage.u.value = T(ETL_OR_STD::forward<Args>(args)...);
      valid = true;
    }
#else
    //*************************************************************************
    /// Emplaces a value.
    /// 0 parameters.
    //*************************************************************************
    void emplace()
    {
      storage.u.value = value_type();
      valid = true;
    }

    //*************************************************************************
    /// Emplaces a value.
    /// 1 parameter.
    //*************************************************************************
    template <typename T1>
    void emplace(const T1& value1)
    {
      storage.u.value = value1;
      valid = true;
    }

    //*************************************************************************
    /// Emplaces a value.
    /// 2 parameters.
    //*************************************************************************
    template <typename T1, typename T2>
    void emplace(const T1& value1, const T2& value2)
    {
      storage.u.value = T(value1, value2);
      valid = true;
    }

    //*************************************************************************
    /// Emplaces a value.
    /// 3 parameters.
    //*************************************************************************
    template <typename T1, typename T2, typename T3>
    void emplace(const T1& value1, const T2& value2, const T3& value3)
    {
      storage.u.value = T(value1, value2, value3);
      valid = true;
    }

    //*************************************************************************
    /// Emplaces a value.
    /// 4 parameters.
    //*************************************************************************
    template <typename T1, typename T2, typename T3, typename T4>
    void emplace(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
    {
      storage.u.value = T(value1, value2, value3, value4);
      valid = true;
    }
#endif

  private:

    bool valid;

    struct storage_type
    {
      storage_type()
      {
      }

      union union_type
      {
        union_type()
          : dummy(0)
        {
        }

        char dummy;
        T value;
      };

      union_type u;
    };

    storage_type storage;
  };

#include "etl/private/diagnostic_uninitialized_push.h"

  //***************************************************************************
  /// Equality operator. cppreference 1
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 bool operator ==(const etl::optional<T>& lhs, const etl::optional<T>& rhs)
  {
    if (lhs.has_value() != rhs.has_value())
    {
      return false;
    }
    else if (!lhs.has_value() && !rhs.has_value())
    {
      return true;
    }
    else
    {
      return lhs.value() == rhs.value();
    }
  }

  //***************************************************************************
  /// Equality operator. cppreference 2
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 bool operator !=(const etl::optional<T>& lhs, const etl::optional<T>& rhs)
  {
    return !(lhs == rhs);
  }

  //***************************************************************************
  /// Less than operator. cppreference 3
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 bool operator <(const etl::optional<T>& lhs, const etl::optional<T>& rhs)
  {
    if (!rhs.has_value())
    {
      return false;
    }
    else if (!lhs.has_value())
    {
      return true;
    }
    else
    {
      return lhs.value() < rhs.value();
    }
  }

  //***************************************************************************
  /// Less than equal operator. cppreference 4
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 bool operator <=(const etl::optional<T>& lhs, const etl::optional<T>& rhs)
  {
    return !(rhs < lhs);
  }

  //***************************************************************************
  /// greater than operator. cppreference 5
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 bool operator >(const etl::optional<T>& lhs, const etl::optional<T>& rhs)
  {
    return (rhs < lhs);
  }

  //***************************************************************************
  /// greater than equal operator. cppreference 6
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 bool operator >=(const etl::optional<T>& lhs, const etl::optional<T>& rhs)
  {
    return !(lhs < rhs);
  }

  //***************************************************************************
  /// Equality operator. cppreference 7
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 bool operator ==(const etl::optional<T>& lhs, etl::nullopt_t)
  {
    return !lhs.has_value();
  }

  //***************************************************************************
  /// Equality operator. cppreference 8
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 bool operator ==(etl::nullopt_t, const etl::optional<T>& rhs)
  {
    return !rhs.has_value();
  }

  //***************************************************************************
  /// Inequality operator. cppreference 9
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 bool operator !=(const etl::optional<T>& lhs, etl::nullopt_t)
  {
    return !(lhs == etl::nullopt);
  }

  //***************************************************************************
  /// Inequality operator. cppreference 10
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 bool operator !=(etl::nullopt_t , const etl::optional<T>& rhs)
  {
    return !(etl::nullopt == rhs);
  }

  //***************************************************************************
  /// Less than operator. cppreference 11
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 bool operator <(const etl::optional<T>&, etl::nullopt_t)
  {
    return false;
  }

  //***************************************************************************
  /// Less than operator. cppreference 12
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 bool operator <(etl::nullopt_t, const etl::optional<T>& rhs)
  {
    return rhs.has_value();
  }

  //***************************************************************************
  /// Less than equal operator. cppreference 13
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 bool operator <=(const etl::optional<T>& lhs, etl::nullopt_t)
  {
    return !lhs.has_value();
  }

  //***************************************************************************
  /// Less than equal operator. cppreference 14
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 bool operator <=(etl::nullopt_t, const etl::optional<T>&)
  {
    return true;
  }

  //***************************************************************************
  /// Greater than operator. cppreference 15
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 bool operator >(const etl::optional<T>& lhs, etl::nullopt_t)
  {
    return lhs.has_value();
  }

  //***************************************************************************
  /// Greater than operator. cppreference 16
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 bool operator >(etl::nullopt_t, const etl::optional<T>&)
  {
    return false;
  }

  //***************************************************************************
  /// Greater than equal operator. cppreference 17
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 bool operator >=(const etl::optional<T>&, etl::nullopt_t)
  {
    return true;
  }

  //***************************************************************************
  /// Greater than equal operator. cppreference 18
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 bool operator >=(etl::nullopt_t, const etl::optional<T>& rhs)
  {
    return !rhs.has_value();
  }

  //***************************************************************************
  /// Equality operator. cppreference 19
  //**************************************************************************
  template <typename T, typename U>
  ETL_CONSTEXPR14 bool operator ==(const etl::optional<T>& lhs, const U& rhs)
  {
    return lhs.has_value() ? lhs.value() == rhs : false;
  }

  //***************************************************************************
  /// Inequality operator. cppreference 21
  //**************************************************************************
  template <typename T, typename U>
  ETL_CONSTEXPR14 bool operator !=(const etl::optional<T>& lhs, const U& rhs)
  {
    return !(lhs == rhs);
  }

  //***************************************************************************
  /// Equality operator. cppreference 20
  //**************************************************************************
  template <typename T, typename U>
  ETL_CONSTEXPR14 bool operator ==(const U& lhs, const etl::optional<T>& rhs)
  {
    return rhs.has_value() ? rhs.value() == lhs : false;
  }

  //***************************************************************************
  /// Inequality operator. cppreference 22
  //**************************************************************************
  template <typename T, typename U>
  ETL_CONSTEXPR14 bool operator !=(const U& lhs, const etl::optional<T>& rhs)
  {
    return !(lhs == rhs);
  }

  //***************************************************************************
  /// Less than operator. cppreference 23
  //***************************************************************************
  template <typename T, typename U>
  ETL_CONSTEXPR14 bool operator <(const etl::optional<T>& lhs, const U& rhs)
  {
    return lhs.has_value() ? lhs.value() < rhs : true;
  }

  //***************************************************************************
  /// Less than operator. cppreference 24
  //***************************************************************************
  template <typename T, typename U>
  ETL_CONSTEXPR14 bool operator <(const U& lhs, const etl::optional<T>& rhs)
  {
    return rhs.has_value() ? lhs < rhs.value() : false;
  }

  //***************************************************************************
  /// Less than equal operator. cppreference 25
  //***************************************************************************
  template <typename T, typename U>
  ETL_CONSTEXPR14 bool operator <=(const etl::optional<T>& lhs, const U& rhs)
  {
    return lhs.has_value() ? lhs.value() <= rhs : true;
  }

  //***************************************************************************
  /// Less than equal operator. cppreference 26
  //***************************************************************************
  template <typename T, typename U>
  ETL_CONSTEXPR14 bool operator <=(const U& lhs, const etl::optional<T>& rhs)
  {
    return rhs.has_value() ? lhs <= rhs.value() : false;
  }

  //***************************************************************************
  /// Greater than operator. cppreference 27
  //***************************************************************************
  template <typename T, typename U>
  ETL_CONSTEXPR14 bool operator >(const etl::optional<T>& lhs, const U& rhs)
  {
    return lhs.has_value() ? lhs.value() > rhs  : false;
  }

  //***************************************************************************
  /// Greater than operator. cppreference 28
  //***************************************************************************
  template <typename T, typename U>
  ETL_CONSTEXPR14 bool operator >(const U& lhs, const etl::optional<T>& rhs)
  {
    return rhs.has_value() ? lhs > rhs.value() : true;
  }

  //***************************************************************************
  /// Greater than equal operator. cppreference 29
  //***************************************************************************
  template <typename T, typename U>
  ETL_CONSTEXPR14 bool operator >=(const etl::optional<T>& lhs, const U& rhs)
  {
    return lhs.has_value() ? lhs.value() >= rhs : false;
  }

  //***************************************************************************
  /// Greater than equal operator. cppreference 30
  //***************************************************************************
  template <typename T, typename U>
  ETL_CONSTEXPR14 bool operator >=(const U& lhs, const etl::optional<T>& rhs)
  {
    return rhs.has_value() ? lhs >= rhs.value() : true;
  }

#include "etl/private/diagnostic_pop.h"

  //***************************************************************************
  /// Make an optional.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 etl::optional<typename etl::decay<T>::type> make_optional(T& value)
  {
    return etl::optional<typename etl::decay<T>::type>(value);
  }

  //***************************************************************************
  /// Template deduction guides.
  //***************************************************************************
#if ETL_CPP17_SUPPORTED
  template <typename T>
  optional(T) -> optional<T>;
#endif
}

//*************************************************************************
/// Swaps the values.
//*************************************************************************
template <typename T>
ETL_CONSTEXPR14 void swap(etl::optional<T>& lhs, etl::optional<T>& rhs)
{
  lhs.swap(rhs);
}

#endif
