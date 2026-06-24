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
#include "error_handler.h"
#include "exception.h"
#include "initializer_list.h"
#include "memory.h"
#include "placement_new.h"
#include "type_traits.h"
#include "utility.h"

namespace etl
{
  //*****************************************************************************
  // Forward declaration of etl::optional
  //*****************************************************************************
  template <typename T>
  class optional;

  //*****************************************************************************
  /// A null option type.
  ///\ingroup utilities
  //*****************************************************************************
  class nullopt_t
  {
  public:

    // Convertible to any type of null non-member pointer.
    template <class T>
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
  // Implementations for fundamental and non fundamental types.
  //*****************************************************************************
  namespace private_optional
  {
    template <typename T, bool UseFundamentalPath = etl::is_fundamental<T>::value && !etl::is_const<T>::value>
    class optional_impl;

    //*****************************************************************************
    // Implementation for non fundamental types.
    //*****************************************************************************
    template <typename T>
    class optional_impl<T, false>
    {
    protected:

      typedef T                       value_type;
      typedef optional_impl<T, false> this_type;

      //***************************************************************************
      /// Constructor.
      //***************************************************************************
      ETL_CONSTEXPR20_STL
      optional_impl() ETL_NOEXCEPT
        : storage()
      {
      }

      //***************************************************************************
      /// Constructor with nullopt.
      //***************************************************************************
      ETL_CONSTEXPR20_STL
      optional_impl(etl::nullopt_t) ETL_NOEXCEPT
        : storage()
      {
      }

#include "private/diagnostic_uninitialized_push.h"

      //***************************************************************************
      /// Copy constructor.
      //***************************************************************************
      ETL_CONSTEXPR20_STL
      optional_impl(const optional_impl& other) ETL_NOEXCEPT_IF(etl::is_nothrow_copy_constructible<T>::value)
      {
        if (other.has_value())
        {
          storage.construct(other.value());
        }
      }
#include "private/diagnostic_pop.h"

#if ETL_USING_CPP11
      //***************************************************************************
      /// Move constructor.
      //***************************************************************************
      ETL_CONSTEXPR20_STL
      optional_impl(optional_impl&& other) ETL_NOEXCEPT_IF(etl::is_nothrow_move_constructible<T>::value)
      {
        if (other.has_value())
        {
          storage.construct(etl::move(other.value()));
        }
      }

      //***************************************************************************
      /// Converting constructor from value type.
      /// Constructs T in-place from U&&, without requiring T to be
      /// copy/move constructible.
      //***************************************************************************
      template <typename U,
                typename etl::enable_if< etl::is_constructible<T, U&&>::value && !etl::is_same<typename etl::decay<U>::type, etl::in_place_t>::value
                                           && !etl::is_same<typename etl::decay<U>::type, optional_impl>::value,
                                         int>::type = 0>
      ETL_CONSTEXPR20_STL optional_impl(U&& value_) ETL_NOEXCEPT_IF((etl::is_nothrow_constructible<T, U&&>::value))
      {
        storage.construct(etl::forward<U>(value_));
      }

      //***************************************************************************
      /// Constructor from variadic args.
      //***************************************************************************
      template <typename... TArgs>
      ETL_CONSTEXPR20_STL optional_impl(etl::in_place_t, TArgs&&... args) ETL_NOEXCEPT_IF((etl::is_nothrow_constructible<T, TArgs...>::value))
      {
        storage.construct(etl::forward<TArgs>(args)...);
      }

  #if ETL_HAS_INITIALIZER_LIST
      //*******************************************
      /// Construct from initializer_list and arguments.
      //*******************************************
      template <typename U, typename... TArgs >
      ETL_CONSTEXPR20_STL optional_impl(etl::in_place_t, std::initializer_list<U> ilist, TArgs&&... args)
        ETL_NOEXCEPT_IF((etl::is_nothrow_constructible<T, std::initializer_list<U>, TArgs...>::value))
      {
        storage.construct(ilist, etl::forward<TArgs>(args)...);
      }
  #endif
#endif

      //***************************************************************************
      /// Destructor.
      //***************************************************************************
      ETL_CONSTEXPR20_STL
      ~optional_impl() ETL_NOEXCEPT
      {
        storage.destroy();
      }

      //***************************************************************************
      /// Assignment operator from nullopt.
      //***************************************************************************
      ETL_CONSTEXPR20_STL
      optional_impl& operator=(etl::nullopt_t) ETL_NOEXCEPT
      {
        if (has_value())
        {
          storage.destroy();
        }

        return *this;
      }

      //***************************************************************************
      /// Assignment operator from optional_impl.
      //***************************************************************************
      ETL_CONSTEXPR20_STL
      optional_impl& operator=(const optional_impl& other) ETL_NOEXCEPT_IF(etl::is_nothrow_copy_constructible<T>::value)
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
      /// Assignment operator from optional_impl.
      //***************************************************************************
      ETL_CONSTEXPR20_STL
      optional_impl& operator=(optional_impl&& other) ETL_NOEXCEPT_IF(etl::is_nothrow_move_constructible<T>::value)
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
#if ETL_USING_CPP11
      template <typename U,
                typename etl::enable_if< etl::is_constructible<T, U&&>::value && !etl::is_same<typename etl::decay<U>::type, optional_impl>::value,
                                         int>::type = 0>
      ETL_CONSTEXPR20_STL optional_impl& operator=(U&& value_) ETL_NOEXCEPT_IF((etl::is_nothrow_constructible<T, U&&>::value))
      {
        storage.construct(etl::forward<U>(value_));

        return *this;
      }
#else
      ETL_CONSTEXPR20_STL
      optional_impl& operator=(const T& value_)
      {
        storage.construct(value_);

        return *this;
      }
#endif

    public:

      //***************************************************************************
      /// Pointer operator.
      //***************************************************************************
      ETL_CONSTEXPR20_STL
      T* operator->()
      {
#if ETL_IS_DEBUG_BUILD
        ETL_ASSERT(has_value(), ETL_ERROR(optional_invalid));
#endif

        return &storage.u.value;
      }

      //***************************************************************************
      /// Pointer operator.
      //***************************************************************************
      ETL_CONSTEXPR20_STL
      const T* operator->() const
      {
#if ETL_IS_DEBUG_BUILD
        ETL_ASSERT(has_value(), ETL_ERROR(optional_invalid));
#endif

        return &storage.u.value;
      }

      //***************************************************************************
      /// Dereference operator.
      //***************************************************************************
      ETL_CONSTEXPR20_STL
      T& operator*() ETL_LVALUE_REF_QUALIFIER
      {
#if ETL_IS_DEBUG_BUILD
        ETL_ASSERT(has_value(), ETL_ERROR(optional_invalid));
#endif

        return storage.u.value;
      }

      //***************************************************************************
      /// Dereference operator.
      //***************************************************************************
      ETL_CONSTEXPR20_STL
      const T& operator*() const ETL_LVALUE_REF_QUALIFIER
      {
#if ETL_IS_DEBUG_BUILD
        ETL_ASSERT(has_value(), ETL_ERROR(optional_invalid));
#endif

        return storage.u.value;
      }

#if ETL_USING_CPP11
      //***************************************************************************
      /// Dereference operator.
      //***************************************************************************
      ETL_CONSTEXPR20_STL
      T&& operator*() &&
      {
  #if ETL_IS_DEBUG_BUILD
        ETL_ASSERT(has_value(), ETL_ERROR(optional_invalid));
  #endif

        return etl::move(storage.u.value);
      }

      //***************************************************************************
      /// Dereference operator.
      //***************************************************************************
      ETL_CONSTEXPR20_STL
      const T&& operator*() const&&
      {
  #if ETL_IS_DEBUG_BUILD
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
      ETL_CONSTEXPR20_STL ETL_EXPLICIT operator bool() const
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
      T&& value() &&
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
      ETL_CONSTEXPR20_STL etl::enable_if_t<etl::is_convertible<U, T>::value, T> value_or(U&& default_value) const&
      {
        return has_value() ? value() : static_cast<T>(etl::forward<U>(default_value));
      }

      //***************************************************************************
      /// Gets the value or a default if not valid.
      //***************************************************************************
      template <typename U>
      ETL_CONSTEXPR20_STL etl::enable_if_t<etl::is_convertible<U, T>::value, T> value_or(U&& default_value) &&
      {
        return has_value() ? etl::move(value()) : static_cast<T>(etl::forward<U>(default_value));
      }
#endif

      //***************************************************************************
      /// Swaps this value with another.
      //***************************************************************************
      ETL_CONSTEXPR20_STL
      void swap(optional_impl& other) ETL_NOEXCEPT_IF(etl::is_nothrow_move_constructible<T>::value)
      {
        optional_impl temp(ETL_MOVE(*this));
        *this = ETL_MOVE(other);
        other = ETL_MOVE(temp);
      }

      //***************************************************************************
      /// Reset back to invalid.
      //***************************************************************************
      ETL_CONSTEXPR20_STL
      void reset() ETL_NOEXCEPT
      {
        storage.destroy();
      }

      //*************************************************************************
      ///
      //*************************************************************************
      ETL_CONSTEXPR20_STL
      T& emplace(const optional_impl& other)
      {
#if ETL_IS_DEBUG_BUILD
        ETL_ASSERT(other.has_value(), ETL_ERROR(optional_invalid));
#endif

        storage.construct(other.value());

        return storage.u.value;
      }

#if ETL_USING_CPP11 && ETL_NOT_USING_STLPORT && !defined(ETL_OPTIONAL_FORCE_CPP03_IMPLEMENTATION)
      //*************************************************************************
      /// Emplaces a value from arbitrary constructor arguments.
      /// Disabled (via SFINAE) if the first argument is an optional_impl (or a
      /// derived type such as etl::optional<T>) so that the dedicated
      /// emplace(const optional_impl&) overload is selected instead.
      //*************************************************************************
      template < typename U, typename... URest,
                 typename etl::enable_if<
                   !etl::is_base_of< optional_impl, typename etl::remove_cv< typename etl::remove_reference<U>::type>::type>::value, int>::type = 0>
      ETL_CONSTEXPR20_STL T& emplace(U&& first, URest&&... rest) ETL_NOEXCEPT_IF((etl::is_nothrow_constructible<T, U&&, URest...>::value))
      {
        storage.construct(etl::forward<U>(first), etl::forward<URest>(rest)...);

        return storage.u.value;
      }

      //*************************************************************************
      /// Emplaces with zero arguments, i.e. default construct emplace.
      //*************************************************************************
      ETL_CONSTEXPR20_STL
      T& emplace() ETL_NOEXCEPT_IF(etl::is_nothrow_default_constructible<T>::value)
      {
        storage.construct();

        return storage.u.value;
      }
#else
      //*************************************************************************
      /// Emplaces a value.
      /// 0 parameters.
      //*************************************************************************
      T& emplace()
      {
        if (has_value())
        {
          // Destroy the old one.
          storage.destroy();
        }

        T* p          = ::new (&storage.u.value) T();
        storage.valid = true;

        return *p;
      }

      //*************************************************************************
      /// Emplaces a value.
      /// 1 parameter.
      //*************************************************************************
      template <typename T1>
      typename etl::enable_if<
        !etl::is_base_of< this_type, typename etl::remove_cv< typename etl::remove_reference<T1>::type>::type>::value
          && !etl::is_same< etl::optional<T>, typename etl::remove_cv< typename etl::remove_reference< T1>::type>::type>::value,
        T&>::type
        emplace(const T1& value1)
      {
        if (has_value())
        {
          // Destroy the old one.
          storage.destroy();
        }

        T* p          = ::new (&storage.u.value) T(value1);
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

        T* p          = ::new (&storage.u.value) T(value1, value2);
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

        T* p          = ::new (&storage.u.value) T(value1, value2, value3);
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

        T* p          = ::new (&storage.u.value) T(value1, value2, value3, value4);
        storage.valid = true;

        return *p;
      }
#endif

    private:

      struct dummy_t
      {
      };

      //*************************************
      // The storage for the optional value.
      //*************************************
      struct storage_type
      {
        typedef typename etl::remove_const<T>::type* pointer_type;

        //*******************************
        ETL_CONSTEXPR20_STL
        storage_type()
          : u()
          , valid(false)
        {
        }

        //*******************************
        ETL_CONSTEXPR20_STL
        void construct(const T& value_)
        {
          destroy();

          etl::construct_at(const_cast<pointer_type>(&u.value), value_);
          valid = true;
        }

#if ETL_USING_CPP11
        //*******************************
        ETL_CONSTEXPR20_STL
        void construct(T&& value_)
        {
          destroy();

          etl::construct_at(const_cast<pointer_type>(&u.value), etl::move(value_));
          valid = true;
        }

        //*******************************
        template <typename... TArgs>
        ETL_CONSTEXPR20_STL void construct(TArgs&&... args)
        {
          destroy();

          etl::construct_at(const_cast<pointer_type>(&u.value), etl::forward<TArgs>(args)...);
          valid = true;
        }
#endif

        //*******************************
        ETL_CONSTEXPR20_STL
        void destroy()
        {
          if (valid)
          {
            etl::destroy_at(const_cast<pointer_type>(&u.value));
            valid = false;
          }
        }

        //*******************************
        union union_type
        {
          ETL_CONSTEXPR20_STL
          union_type()
            : dummy()
          {
          }

          ETL_CONSTEXPR20_STL
          ~union_type() {}

          dummy_t dummy;
          T       value;
        } u;

        bool valid;
      };

      storage_type storage;
    };

    //*****************************************************************************
    // Implementation for fundamental types.
    //*****************************************************************************
    template <typename T>
    class optional_impl<T, true>
    {
    protected:

      typedef T                      value_type;
      typedef optional_impl<T, true> this_type;

      //***************************************************************************
      /// Constructor.
      //***************************************************************************
      ETL_CONSTEXPR14 optional_impl() ETL_NOEXCEPT
        : storage()
      {
      }

      //***************************************************************************
      /// Constructor with nullopt.
      //***************************************************************************
      ETL_CONSTEXPR14 optional_impl(etl::nullopt_t) ETL_NOEXCEPT
        : storage()
      {
      }

#include "private/diagnostic_uninitialized_push.h"

      //***************************************************************************
      /// Copy constructor.
      //***************************************************************************
      ETL_CONSTEXPR14 optional_impl(const optional_impl& other) ETL_NOEXCEPT
      {
        if (other.has_value())
        {
          storage.construct(other.value());
        }
      }
#include "private/diagnostic_pop.h"

#if ETL_USING_CPP11
      //***************************************************************************
      /// Move constructor.
      //***************************************************************************
      ETL_CONSTEXPR14 optional_impl(optional_impl&& other) ETL_NOEXCEPT
      {
        if (other.has_value())
        {
          storage.construct(etl::move(other.value()));
        }
      }

      //***************************************************************************
      /// Constructor from value type.
      //***************************************************************************
      ETL_CONSTEXPR14 optional_impl(const T& value_) ETL_NOEXCEPT
      {
        storage.construct(value_);
      }

      //***************************************************************************
      /// Constructor from value type.
      //***************************************************************************
      ETL_CONSTEXPR14 optional_impl(T&& value_) ETL_NOEXCEPT
      {
        storage.construct(etl::move(value_));
      }

      //***************************************************************************
      /// Constructor from variadic args.
      //***************************************************************************
      template <typename... TArgs>
      ETL_CONSTEXPR14 optional_impl(etl::in_place_t, TArgs&&... args) ETL_NOEXCEPT
      {
        storage.construct(etl::forward<TArgs>(args)...);
      }

  #if ETL_HAS_INITIALIZER_LIST
      //*******************************************
      /// Construct from initializer_list and arguments.
      //*******************************************
      template <typename U, typename... TArgs >
      ETL_CONSTEXPR14 optional_impl(etl::in_place_t, std::initializer_list<U> ilist, TArgs&&... args) ETL_NOEXCEPT
      {
        storage.construct(ilist, etl::forward<TArgs>(args)...);
      }
  #endif
#endif

      //***************************************************************************
      /// Assignment operator from nullopt.
      //***************************************************************************
      ETL_CONSTEXPR14 optional_impl& operator=(etl::nullopt_t) ETL_NOEXCEPT
      {
        if (has_value())
        {
          storage.destroy();
        }

        return *this;
      }

      //***************************************************************************
      /// Assignment operator from optional_impl.
      //***************************************************************************
      ETL_CONSTEXPR14 optional_impl& operator=(const optional_impl& other) ETL_NOEXCEPT
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
      /// Assignment operator from optional_impl.
      //***************************************************************************
      ETL_CONSTEXPR14 optional_impl& operator=(optional_impl&& other) ETL_NOEXCEPT
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
      ETL_CONSTEXPR14 optional_impl& operator=(const T& value_) ETL_NOEXCEPT
      {
        storage.construct(value_);

        return *this;
      }

#if ETL_USING_CPP11
      //***************************************************************************
      /// Assignment operator from value type.
      //***************************************************************************
      ETL_CONSTEXPR14 optional_impl& operator=(T&& value_) ETL_NOEXCEPT
      {
        storage.construct(etl::move(value_));

        return *this;
      }
#endif

    public:

      //***************************************************************************
      /// Pointer operator.
      //***************************************************************************
      ETL_CONSTEXPR14 T* operator->()
      {
#if ETL_IS_DEBUG_BUILD
        ETL_ASSERT(has_value(), ETL_ERROR(optional_invalid));
#endif

        return &storage.value;
      }

      //***************************************************************************
      /// Pointer operator.
      //***************************************************************************
      ETL_CONSTEXPR14 const T* operator->() const
      {
#if ETL_IS_DEBUG_BUILD
        ETL_ASSERT(has_value(), ETL_ERROR(optional_invalid));
#endif

        return &storage.value;
      }

      //***************************************************************************
      /// Dereference operator.
      //***************************************************************************
      ETL_CONSTEXPR14 T& operator*() ETL_LVALUE_REF_QUALIFIER
      {
#if ETL_IS_DEBUG_BUILD
        ETL_ASSERT(has_value(), ETL_ERROR(optional_invalid));
#endif

        return storage.value;
      }

      //***************************************************************************
      /// Dereference operator.
      //***************************************************************************
      ETL_CONSTEXPR14 const T& operator*() const ETL_LVALUE_REF_QUALIFIER
      {
#if ETL_IS_DEBUG_BUILD
        ETL_ASSERT(has_value(), ETL_ERROR(optional_invalid));
#endif

        return storage.value;
      }

#if ETL_USING_CPP11
      //***************************************************************************
      /// Dereference operator.
      //***************************************************************************
      ETL_CONSTEXPR14 T&& operator*() &&
      {
  #if ETL_IS_DEBUG_BUILD
        ETL_ASSERT(has_value(), ETL_ERROR(optional_invalid));
  #endif

        return etl::move(storage.value);
      }

      //***************************************************************************
      /// Dereference operator.
      //***************************************************************************
      ETL_CONSTEXPR14 const T&& operator*() const&&
      {
  #if ETL_IS_DEBUG_BUILD
        ETL_ASSERT(has_value(), ETL_ERROR(optional_invalid));
  #endif

        return etl::move(storage.value);
      }
#endif

      //***************************************************************************
      // Check whether optional contains value
      //***************************************************************************
      ETL_CONSTEXPR14 bool has_value() const ETL_NOEXCEPT
      {
        return storage.valid;
      }

      //***************************************************************************
      /// Bool conversion operator.
      //***************************************************************************
      ETL_CONSTEXPR14 ETL_EXPLICIT operator bool() const
      {
        return has_value();
      }

      //***************************************************************************
      /// Get a reference to the value.
      //***************************************************************************
      ETL_CONSTEXPR14 T& value() ETL_LVALUE_REF_QUALIFIER
      {
#if ETL_IS_DEBUG_BUILD
        ETL_ASSERT(has_value(), ETL_ERROR(optional_invalid));
#endif

        return storage.value;
      }

      //***************************************************************************
      /// Get a const reference to the value.
      //***************************************************************************
      ETL_CONSTEXPR14 const T& value() const ETL_LVALUE_REF_QUALIFIER
      {
#if ETL_IS_DEBUG_BUILD
        ETL_ASSERT(has_value(), ETL_ERROR(optional_invalid));
#endif

        return storage.value;
      }

      //***************************************************************************
      /// Gets the value or a default if not valid.
      //***************************************************************************
      ETL_CONSTEXPR14 T value_or(const T& default_value) const ETL_LVALUE_REF_QUALIFIER
      {
        return has_value() ? value() : default_value;
      }

#if ETL_USING_CPP11
      //***************************************************************************
      /// Get an rvalue reference to the value.
      //***************************************************************************
      ETL_CONSTEXPR14 T&& value() &&
      {
  #if ETL_IS_DEBUG_BUILD
        ETL_ASSERT(has_value(), ETL_ERROR(optional_invalid));
  #endif

        return etl::move(storage.value);
      }

      //***************************************************************************
      /// Get a const rvalue reference to the value.
      //***************************************************************************
      ETL_CONSTEXPR14 const T&& value() const&&
      {
  #if ETL_IS_DEBUG_BUILD
        ETL_ASSERT(has_value(), ETL_ERROR(optional_invalid));
  #endif

        return etl::move(storage.value);
      }

      //***************************************************************************
      /// Gets the value or a default if not valid.
      //***************************************************************************
      template <typename U>
      ETL_CONSTEXPR14 etl::enable_if_t<etl::is_convertible<U, T>::value, T> value_or(U&& default_value) const&
      {
        return has_value() ? value() : static_cast<T>(etl::forward<U>(default_value));
      }

      //***************************************************************************
      /// Gets the value or a default if not valid.
      //***************************************************************************
      template <typename U>
      ETL_CONSTEXPR14 etl::enable_if_t<etl::is_convertible<U, T>::value, T> value_or(U&& default_value) &&
      {
        return has_value() ? etl::move(value()) : static_cast<T>(etl::forward<U>(default_value));
      }
#endif

      //***************************************************************************
      /// Swaps this value with another.
      //***************************************************************************
      ETL_CONSTEXPR14 void swap(optional_impl& other) ETL_NOEXCEPT
      {
        optional_impl temp(*this);
        *this = other;
        other = temp;
      }

      //***************************************************************************
      /// Reset back to invalid.
      //***************************************************************************
      ETL_CONSTEXPR14 void reset() ETL_NOEXCEPT
      {
        storage.destroy();
      }

      //*************************************************************************
      ///
      //*************************************************************************
      ETL_CONSTEXPR20_STL
      T& emplace(const optional_impl& other)
      {
#if ETL_IS_DEBUG_BUILD
        ETL_ASSERT(other.has_value(), ETL_ERROR(optional_invalid));
#endif

        storage.construct(other.value());

        return storage.u.value;
      }

#if ETL_USING_CPP11 && ETL_NOT_USING_STLPORT && !defined(ETL_OPTIONAL_FORCE_CPP03_IMPLEMENTATION)
      //*************************************************************************
      /// Emplaces a value.
      ///\param args The arguments to construct with.
      //*************************************************************************
      template <typename... TArgs>
      ETL_CONSTEXPR14 T& emplace(TArgs&&... args) ETL_NOEXCEPT
      {
        storage.construct(etl::forward<TArgs>(args)...);

        return storage.value;
      }
#else
      //*************************************************************************
      /// Emplaces a value.
      /// 0 parameters.
      //*************************************************************************
      T& emplace()
      {
        if (has_value())
        {
          // Destroy the old one.
          storage.destroy();
        }

        T* p          = ::new (&storage.value) T();
        storage.valid = true;

        return *p;
      }

      //*************************************************************************
      /// Emplaces a value.
      /// 1 parameter.
      //*************************************************************************
      template <typename T1>
      typename etl::enable_if<
        !etl::is_base_of< this_type, typename etl::remove_cv< typename etl::remove_reference<T1>::type>::type>::value
          && !etl::is_same< etl::optional<T>, typename etl::remove_cv< typename etl::remove_reference< T1>::type>::type>::value,
        T&>::type
        emplace(const T1& value1)
      {
        if (has_value())
        {
          // Destroy the old one.
          storage.destroy();
        }

        T* p          = ::new (&storage.value) T(value1);
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

        T* p          = ::new (&storage.value) T(value1, value2);
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

        T* p          = ::new (&storage.value) T(value1, value2, value3);
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

        T* p          = ::new (&storage.value) T(value1, value2, value3, value4);
        storage.valid = true;

        return *p;
      }
#endif

    private:

      //*************************************
      // The storage for the optional value.
      //*************************************
      struct storage_type
      {
        //*******************************
        ETL_CONSTEXPR14 storage_type()
          : value()
          , valid(false)
        {
        }

        //*******************************
        ETL_CONSTEXPR14 void construct(const T& value_)
        {
          value = value_;
          valid = true;
        }

#if ETL_USING_CPP11
        //*******************************
        ETL_CONSTEXPR14 void construct(T&& value_)
        {
          value = value_;
          valid = true;
        }

        //*******************************
        template <typename... TArgs>
        ETL_CONSTEXPR14 void construct(TArgs&&... args)
        {
          value = T(etl::forward<TArgs>(args)...);
          valid = true;
        }
#endif

        //*******************************
        ETL_CONSTEXPR14 void destroy()
        {
          valid = false;
        }

        T    value;
        bool valid;
      };

      storage_type storage;
    };
  } // namespace private_optional

#define ETL_OPTIONAL_ENABLE_CPP14     typename etl::enable_if< etl::is_pod<typename etl::remove_cv<U>::type>::value, int>::type = 0
#define ETL_OPTIONAL_ENABLE_CPP20_STL typename etl::enable_if< !etl::is_pod<typename etl::remove_cv<U>::type>::value, int>::type = 0

#define ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP14 \
  ETL_CONSTEXPR14 typename etl::enable_if< etl::is_pod<typename etl::remove_cv<T>::type>::value, bool>::type
#define ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP20_STL \
  ETL_CONSTEXPR20_STL                                       \
  typename etl::enable_if< !etl::is_pod<typename etl::remove_cv<T>::type>::value, bool>::type

  //*****************************************************************************
  /// An optional type.
  /// If the optional type is not initialised then a type is not constructed.
  /// See http://en.cppreference.com/w/cpp/utility/optional
  ///\tparam T The type to store.
  ///\ingroup utilities
  //*****************************************************************************
  template <typename T>
  class optional : public private_optional::optional_impl<T>
  {
  private:

    typedef private_optional::optional_impl<T> impl_t;

  public:

    typedef T        value_type;
    typedef T*       iterator;
    typedef const T* const_iterator;

#if ETL_USING_CPP11
    //***************************************************************************
    /// Constructor.
    //***************************************************************************
    template <typename U = T, ETL_OPTIONAL_ENABLE_CPP14>
    ETL_CONSTEXPR14 optional() ETL_NOEXCEPT
      : impl_t()
    {
    }

    //***************************************************************************
    /// Constructor.
    //***************************************************************************
    template <typename U = T, ETL_OPTIONAL_ENABLE_CPP20_STL>
    ETL_CONSTEXPR20_STL optional() ETL_NOEXCEPT
      : impl_t()
    {
    }
#else
    optional() ETL_NOEXCEPT
      : impl_t()
    {
    }
#endif

#if ETL_USING_CPP11
    //***************************************************************************
    /// Constructor with nullopt.
    //***************************************************************************
    template <typename U = T, ETL_OPTIONAL_ENABLE_CPP14>
    ETL_CONSTEXPR14 optional(etl::nullopt_t) ETL_NOEXCEPT
      : impl_t(etl::nullopt)
    {
    }

    //***************************************************************************
    /// Constructor with nullopt.
    //***************************************************************************
    template <typename U = T, ETL_OPTIONAL_ENABLE_CPP20_STL>
    ETL_CONSTEXPR20_STL optional(etl::nullopt_t) ETL_NOEXCEPT
      : impl_t(etl::nullopt)
    {
    }
#else
    //***************************************************************************
    /// Constructor with nullopt.
    //***************************************************************************
    optional(etl::nullopt_t) ETL_NOEXCEPT
      : impl_t(etl::nullopt)
    {
    }
#endif

#include "private/diagnostic_uninitialized_push.h"

#if ETL_USING_CPP11
#else
    //***************************************************************************
    /// Copy constructor.
    //***************************************************************************
    optional(const optional& other)
      : impl_t(other)
    {
    }
#endif
#include "private/diagnostic_pop.h"

#if ETL_USING_CPP11
    //***************************************************************************
    /// Converting constructor from value type.
    /// Constructs T in-place from U&&, without requiring T to be
    /// copy/move constructible.
    //***************************************************************************
    template < typename U,
               typename etl::enable_if< etl::is_constructible<T, U&&>::value && !etl::is_same<typename etl::decay<U>::type, etl::optional<T>>::value
                                          && !etl::is_same<typename etl::decay<U>::type, etl::in_place_t>::value
                                          && !etl::is_same<typename etl::decay<U>::type, etl::nullopt_t>::value
                                          && etl::is_pod<typename etl::remove_cv<T>::type>::value,
                                        int>::type = 0>
    ETL_CONSTEXPR14 optional(U&& value_) ETL_NOEXCEPT_IF((etl::is_nothrow_constructible<T, U&&>::value))
      : impl_t(etl::forward<U>(value_))
    {
    }

    //***************************************************************************
    /// Converting constructor from value type.
    //***************************************************************************
    template < typename U,
               typename etl::enable_if< etl::is_constructible<T, U&&>::value && !etl::is_same<typename etl::decay<U>::type, etl::optional<T>>::value
                                          && !etl::is_same<typename etl::decay<U>::type, etl::in_place_t>::value
                                          && !etl::is_same<typename etl::decay<U>::type, etl::nullopt_t>::value
                                          && !etl::is_pod<typename etl::remove_cv<T>::type>::value,
                                        int>::type = 0>
    ETL_CONSTEXPR20_STL optional(U&& value_) ETL_NOEXCEPT_IF((etl::is_nothrow_constructible<T, U&&>::value))
      : impl_t(etl::forward<U>(value_))
    {
    }
#else
    //***************************************************************************
    /// Construct from value type.
    //***************************************************************************
    optional(const T& value_)
      : impl_t(value_)
    {
    }
#endif

#if ETL_USING_CPP11
    //***************************************************************************
    /// Emplace construct from arguments.
    //***************************************************************************
    template <typename U = T, ETL_OPTIONAL_ENABLE_CPP14, typename... Args>
    ETL_CONSTEXPR14 explicit optional(etl::in_place_t, Args&&... args) ETL_NOEXCEPT_IF((etl::is_nothrow_constructible<T, Args...>::value))
      : impl_t(etl::in_place_t{}, etl::forward<Args>(args)...)
    {
    }

    //***************************************************************************
    /// Emplace construct from arguments.
    //***************************************************************************
    template <typename U = T, ETL_OPTIONAL_ENABLE_CPP20_STL, typename... Args>
    ETL_CONSTEXPR20_STL explicit optional(etl::in_place_t, Args&&... args) ETL_NOEXCEPT_IF((etl::is_nothrow_constructible<T, Args...>::value))
      : impl_t(etl::in_place_t{}, etl::forward<Args>(args)...)
    {
    }

  #if ETL_HAS_INITIALIZER_LIST
    //*******************************************
    /// Construct from initializer_list and arguments.
    //*******************************************
    template <typename U = T, ETL_OPTIONAL_ENABLE_CPP14, typename... TArgs>
    ETL_CONSTEXPR14 explicit optional(etl::in_place_t, std::initializer_list<U> ilist, TArgs&&... args)
      ETL_NOEXCEPT_IF((etl::is_nothrow_constructible<T, std::initializer_list<U>, TArgs...>::value))
      : impl_t(etl::in_place_t{}, ilist, etl::forward<TArgs>(args)...)
    {
    }

    //*******************************************
    /// Construct from initializer_list and arguments.
    //*******************************************
    template <typename U = T, ETL_OPTIONAL_ENABLE_CPP20_STL, typename... TArgs>
    ETL_CONSTEXPR20_STL explicit optional(etl::in_place_t, std::initializer_list<U> ilist, TArgs&&... args)
      ETL_NOEXCEPT_IF((etl::is_nothrow_constructible<T, std::initializer_list<U>, TArgs...>::value))
      : impl_t(etl::in_place_t{}, ilist, etl::forward<TArgs>(args)...)
    {
    }
  #endif
#endif

#if ETL_USING_CPP11
    //***************************************************************************
    /// Assignment operator from nullopt.
    //***************************************************************************
    template <typename U = T, ETL_OPTIONAL_ENABLE_CPP14>
    ETL_CONSTEXPR14 optional& operator=(etl::nullopt_t) ETL_NOEXCEPT
    {
      impl_t::operator=(etl::nullopt);

      return *this;
    }

    //***************************************************************************
    /// Assignment operator from nullopt.
    //***************************************************************************
    template <typename U = T, ETL_OPTIONAL_ENABLE_CPP20_STL>
    ETL_CONSTEXPR20_STL optional& operator=(etl::nullopt_t) ETL_NOEXCEPT
    {
      impl_t::operator=(etl::nullopt);

      return *this;
    }
#else
    //***************************************************************************
    /// Assignment operator from nullopt.
    //***************************************************************************
    optional& operator=(etl::nullopt_t) ETL_NOEXCEPT
    {
      impl_t::operator=(etl::nullopt);

      return *this;
    }
#endif

#if ETL_USING_CPP11
#else
    //***************************************************************************
    /// Assignment operator from optional.
    //***************************************************************************
    optional& operator=(const optional& other)
    {
      impl_t::operator=(other);

      return *this;
    }
#endif

#if ETL_USING_CPP11
    //***************************************************************************
    /// Converting assignment operator from value type.
    //***************************************************************************
    template < typename U,
               typename etl::enable_if< etl::is_constructible<T, U&&>::value && !etl::is_same<typename etl::decay<U>::type, etl::optional<T>>::value
                                          && !etl::is_same<typename etl::decay<U>::type, etl::nullopt_t>::value
                                          && etl::is_pod<typename etl::remove_cv<T>::type>::value,
                                        int>::type = 0>
    ETL_CONSTEXPR14 optional& operator=(U&& value_) ETL_NOEXCEPT_IF((etl::is_nothrow_constructible<T, U&&>::value))
    {
      impl_t::operator=(etl::forward<U>(value_));

      return *this;
    }

    //***************************************************************************
    /// Converting assignment operator from value type.
    //***************************************************************************
    template < typename U,
               typename etl::enable_if< etl::is_constructible<T, U&&>::value && !etl::is_same<typename etl::decay<U>::type, etl::optional<T>>::value
                                          && !etl::is_same<typename etl::decay<U>::type, etl::nullopt_t>::value
                                          && !etl::is_pod<typename etl::remove_cv<T>::type>::value,
                                        int>::type = 0>
    ETL_CONSTEXPR20_STL optional& operator=(U&& value_) ETL_NOEXCEPT_IF((etl::is_nothrow_constructible<T, U&&>::value))
    {
      impl_t::operator=(etl::forward<U>(value_));

      return *this;
    }
#else
    //***************************************************************************
    /// Assignment operator from value type.
    //***************************************************************************
    optional& operator=(const T& value_)
    {
      impl_t::operator=(value_);

      return *this;
    }
#endif

    //***************************************************************************
    /// Returns an iterator to the beginning of the optional.
    //***************************************************************************
    ETL_CONSTEXPR20_STL
    iterator begin() ETL_NOEXCEPT
    {
      return this->has_value() ? this->operator->() : ETL_NULLPTR;
    }

    //***************************************************************************
    /// Returns a const iterator to the beginning of the optional.
    //***************************************************************************
    ETL_CONSTEXPR20_STL
    const_iterator begin() const ETL_NOEXCEPT
    {
      return this->has_value() ? this->operator->() : ETL_NULLPTR;
    }

    //***************************************************************************
    /// Returns an iterator to the end of the optional.
    //***************************************************************************
    ETL_CONSTEXPR20_STL
    iterator end() ETL_NOEXCEPT
    {
      return this->has_value() ? this->operator->() + 1 : ETL_NULLPTR;
    }

    //***************************************************************************
    /// Returns a const iterator to the end of the optional.
    //***************************************************************************
    ETL_CONSTEXPR20_STL
    const_iterator end() const ETL_NOEXCEPT
    {
      return this->has_value() ? this->operator->() + 1 : ETL_NULLPTR;
    }
  };

#include "private/diagnostic_uninitialized_push.h"

  //***************************************************************************
  /// Equality operator. cppreference 1
  //***************************************************************************
  template <typename T>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP14 operator==(const etl::optional<T>& lhs, const etl::optional<T>& rhs)
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
  /// Equality operator. cppreference 1
  //***************************************************************************
  template <typename T>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP20_STL operator==(const etl::optional<T>& lhs, const etl::optional<T>& rhs)
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
  /// Inequality operator. cppreference 2
  //***************************************************************************
  template <typename T>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP14 operator!=(const etl::optional<T>& lhs, const etl::optional<T>& rhs)
  {
    return !(lhs == rhs);
  }

  //***************************************************************************
  /// Inequality operator. cppreference 2
  //***************************************************************************
  template <typename T>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP20_STL operator!=(const etl::optional<T>& lhs, const etl::optional<T>& rhs)
  {
    return !(lhs == rhs);
  }

  //***************************************************************************
  /// Less than operator. cppreference 3
  //***************************************************************************
  template <typename T>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP14 operator<(const etl::optional<T>& lhs, const etl::optional<T>& rhs)
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
  /// Less than operator. cppreference 3
  //***************************************************************************
  template <typename T>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP20_STL operator<(const etl::optional<T>& lhs, const etl::optional<T>& rhs)
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
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP14 operator<=(const etl::optional<T>& lhs, const etl::optional<T>& rhs)
  {
    return !(rhs < lhs);
  }

  //***************************************************************************
  /// Less than equal operator. cppreference 4
  //***************************************************************************
  template <typename T>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP20_STL operator<=(const etl::optional<T>& lhs, const etl::optional<T>& rhs)
  {
    return !(rhs < lhs);
  }

  //***************************************************************************
  /// greater than operator. cppreference 5
  //***************************************************************************
  template <typename T>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP14 operator>(const etl::optional<T>& lhs, const etl::optional<T>& rhs)
  {
    return (rhs < lhs);
  }

  //***************************************************************************
  /// greater than operator. cppreference 5
  //***************************************************************************
  template <typename T>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP20_STL operator>(const etl::optional<T>& lhs, const etl::optional<T>& rhs)
  {
    return (rhs < lhs);
  }

  //***************************************************************************
  /// greater than equal operator. cppreference 6
  //***************************************************************************
  template <typename T>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP14 operator>=(const etl::optional<T>& lhs, const etl::optional<T>& rhs)
  {
    return !(lhs < rhs);
  }

  //***************************************************************************
  /// greater than equal operator. cppreference 6
  //***************************************************************************
  template <typename T>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP20_STL operator>=(const etl::optional<T>& lhs, const etl::optional<T>& rhs)
  {
    return !(lhs < rhs);
  }

  //***************************************************************************
  /// Equality operator. cppreference 7
  //***************************************************************************
  template <typename T>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP14 operator==(const etl::optional<T>& lhs, etl::nullopt_t)
  {
    return !lhs.has_value();
  }

  //***************************************************************************
  /// Equality operator. cppreference 7
  //***************************************************************************
  template <typename T>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP20_STL operator==(const etl::optional<T>& lhs, etl::nullopt_t)
  {
    return !lhs.has_value();
  }

  //***************************************************************************
  /// Equality operator. cppreference 8
  //***************************************************************************
  template <typename T>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP14 operator==(etl::nullopt_t, const etl::optional<T>& rhs)
  {
    return !rhs.has_value();
  }

  //***************************************************************************
  /// Equality operator. cppreference 8
  //***************************************************************************
  template <typename T>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP20_STL operator==(etl::nullopt_t, const etl::optional<T>& rhs)
  {
    return !rhs.has_value();
  }

  //***************************************************************************
  /// Inequality operator. cppreference 9
  //***************************************************************************
  template <typename T>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP14 operator!=(const etl::optional<T>& lhs, etl::nullopt_t)
  {
    return !(lhs == etl::nullopt);
  }

  //***************************************************************************
  /// Inequality operator. cppreference 9
  //***************************************************************************
  template <typename T>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP20_STL operator!=(const etl::optional<T>& lhs, etl::nullopt_t)
  {
    return !(lhs == etl::nullopt);
  }

  //***************************************************************************
  /// Inequality operator. cppreference 10
  //***************************************************************************
  template <typename T>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP14 operator!=(etl::nullopt_t, const etl::optional<T>& rhs)
  {
    return !(etl::nullopt == rhs);
  }

  //***************************************************************************
  /// Inequality operator. cppreference 10
  //***************************************************************************
  template <typename T>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP20_STL operator!=(etl::nullopt_t, const etl::optional<T>& rhs)
  {
    return !(etl::nullopt == rhs);
  }

  //***************************************************************************
  /// Less than operator. cppreference 11
  //***************************************************************************
  template <typename T>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP14 operator<(const etl::optional<T>&, etl::nullopt_t)
  {
    return false;
  }

  //***************************************************************************
  /// Less than operator. cppreference 11
  //***************************************************************************
  template <typename T>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP20_STL operator<(const etl::optional<T>&, etl::nullopt_t)
  {
    return false;
  }

  //***************************************************************************
  /// Less than operator. cppreference 12
  //***************************************************************************
  template <typename T>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP14 operator<(etl::nullopt_t, const etl::optional<T>& rhs)
  {
    return rhs.has_value();
  }

  //***************************************************************************
  /// Less than operator. cppreference 12
  //***************************************************************************
  template <typename T>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP20_STL operator<(etl::nullopt_t, const etl::optional<T>& rhs)
  {
    return rhs.has_value();
  }

  //***************************************************************************
  /// Less than equal operator. cppreference 13
  //***************************************************************************
  template <typename T>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP14 operator<=(const etl::optional<T>& lhs, etl::nullopt_t)
  {
    return !lhs.has_value();
  }

  //***************************************************************************
  /// Less than equal operator. cppreference 13
  //***************************************************************************
  template <typename T>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP20_STL operator<=(const etl::optional<T>& lhs, etl::nullopt_t)
  {
    return !lhs.has_value();
  }

  //***************************************************************************
  /// Less than equal operator. cppreference 14
  //***************************************************************************
  template <typename T>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP14 operator<=(etl::nullopt_t, const etl::optional<T>&)
  {
    return true;
  }

  //***************************************************************************
  /// Less than equal operator. cppreference 14
  //***************************************************************************
  template <typename T>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP20_STL operator<=(etl::nullopt_t, const etl::optional<T>&)
  {
    return true;
  }

  //***************************************************************************
  /// Greater than operator. cppreference 15
  //***************************************************************************
  template <typename T>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP14 operator>(const etl::optional<T>& lhs, etl::nullopt_t)
  {
    return lhs.has_value();
  }

  //***************************************************************************
  /// Greater than operator. cppreference 15
  //***************************************************************************
  template <typename T>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP20_STL operator>(const etl::optional<T>& lhs, etl::nullopt_t)
  {
    return lhs.has_value();
  }

  //***************************************************************************
  /// Greater than operator. cppreference 16
  //***************************************************************************
  template <typename T>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP14 operator>(etl::nullopt_t, const etl::optional<T>&)
  {
    return false;
  }

  //***************************************************************************
  /// Greater than operator. cppreference 16
  //***************************************************************************
  template <typename T>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP20_STL operator>(etl::nullopt_t, const etl::optional<T>&)
  {
    return false;
  }

  //***************************************************************************
  /// Greater than equal operator. cppreference 17
  //***************************************************************************
  template <typename T>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP14 operator>=(const etl::optional<T>&, etl::nullopt_t)
  {
    return true;
  }

  //***************************************************************************
  /// Greater than equal operator. cppreference 17
  //***************************************************************************
  template <typename T>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP20_STL operator>=(const etl::optional<T>&, etl::nullopt_t)
  {
    return true;
  }

  //***************************************************************************
  /// Greater than equal operator. cppreference 18
  //***************************************************************************
  template <typename T>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP14 operator>=(etl::nullopt_t, const etl::optional<T>& rhs)
  {
    return !rhs.has_value();
  }

  //***************************************************************************
  /// Greater than equal operator. cppreference 18
  //***************************************************************************
  template <typename T>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP20_STL operator>=(etl::nullopt_t, const etl::optional<T>& rhs)
  {
    return !rhs.has_value();
  }

  //***************************************************************************
  /// Equality operator. cppreference 19
  //**************************************************************************
  template <typename T, typename U>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP14 operator==(const etl::optional<T>& lhs, const U& rhs)
  {
    return lhs.has_value() ? lhs.value() == rhs : false;
  }

  //***************************************************************************
  /// Equality operator. cppreference 19
  //**************************************************************************
  template <typename T, typename U>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP20_STL operator==(const etl::optional<T>& lhs, const U& rhs)
  {
    return lhs.has_value() ? lhs.value() == rhs : false;
  }

  //***************************************************************************
  /// Inequality operator. cppreference 21
  //**************************************************************************
  template <typename T, typename U>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP14 operator!=(const etl::optional<T>& lhs, const U& rhs)
  {
    return !(lhs == rhs);
  }

  //***************************************************************************
  /// Inequality operator. cppreference 21
  //**************************************************************************
  template <typename T, typename U>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP20_STL operator!=(const etl::optional<T>& lhs, const U& rhs)
  {
    return !(lhs == rhs);
  }

  //***************************************************************************
  /// Equality operator. cppreference 20
  //**************************************************************************
  template <typename T, typename U>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP14 operator==(const U& lhs, const etl::optional<T>& rhs)
  {
    return rhs.has_value() ? rhs.value() == lhs : false;
  }

  //***************************************************************************
  /// Equality operator. cppreference 20
  //**************************************************************************
  template <typename T, typename U>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP20_STL operator==(const U& lhs, const etl::optional<T>& rhs)
  {
    return rhs.has_value() ? rhs.value() == lhs : false;
  }

  //***************************************************************************
  /// Inequality operator. cppreference 22
  //**************************************************************************
  template <typename T, typename U>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP14 operator!=(const U& lhs, const etl::optional<T>& rhs)
  {
    return !(lhs == rhs);
  }

  //***************************************************************************
  /// Inequality operator. cppreference 22
  //**************************************************************************
  template <typename T, typename U>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP20_STL operator!=(const U& lhs, const etl::optional<T>& rhs)
  {
    return !(lhs == rhs);
  }

  //***************************************************************************
  /// Less than operator. cppreference 23
  //***************************************************************************
  template <typename T, typename U>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP14 operator<(const etl::optional<T>& lhs, const U& rhs)
  {
    return lhs.has_value() ? lhs.value() < rhs : true;
  }

  //***************************************************************************
  /// Less than operator. cppreference 23
  //***************************************************************************
  template <typename T, typename U>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP20_STL operator<(const etl::optional<T>& lhs, const U& rhs)
  {
    return lhs.has_value() ? lhs.value() < rhs : true;
  }

  //***************************************************************************
  /// Less than operator. cppreference 24
  //***************************************************************************
  template <typename T, typename U>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP14 operator<(const U& lhs, const etl::optional<T>& rhs)
  {
    return rhs.has_value() ? lhs < rhs.value() : false;
  }

  //***************************************************************************
  /// Less than operator. cppreference 24
  //***************************************************************************
  template <typename T, typename U>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP20_STL operator<(const U& lhs, const etl::optional<T>& rhs)
  {
    return rhs.has_value() ? lhs < rhs.value() : false;
  }

  //***************************************************************************
  /// Less than equal operator. cppreference 25
  //***************************************************************************
  template <typename T, typename U>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP14 operator<=(const etl::optional<T>& lhs, const U& rhs)
  {
    return lhs.has_value() ? lhs.value() <= rhs : true;
  }

  //***************************************************************************
  /// Less than equal operator. cppreference 25
  //***************************************************************************
  template <typename T, typename U>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP20_STL operator<=(const etl::optional<T>& lhs, const U& rhs)
  {
    return lhs.has_value() ? lhs.value() <= rhs : true;
  }

  //***************************************************************************
  /// Less than equal operator. cppreference 26
  //***************************************************************************
  template <typename T, typename U>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP14 operator<=(const U& lhs, const etl::optional<T>& rhs)
  {
    return rhs.has_value() ? lhs <= rhs.value() : false;
  }

  //***************************************************************************
  /// Less than equal operator. cppreference 26
  //***************************************************************************
  template <typename T, typename U>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP20_STL operator<=(const U& lhs, const etl::optional<T>& rhs)
  {
    return rhs.has_value() ? lhs <= rhs.value() : false;
  }

  //***************************************************************************
  /// Greater than operator. cppreference 27
  //***************************************************************************
  template <typename T, typename U>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP14 operator>(const etl::optional<T>& lhs, const U& rhs)
  {
    return lhs.has_value() ? lhs.value() > rhs : false;
  }

  //***************************************************************************
  /// Greater than operator. cppreference 27
  //***************************************************************************
  template <typename T, typename U>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP20_STL operator>(const etl::optional<T>& lhs, const U& rhs)
  {
    return lhs.has_value() ? lhs.value() > rhs : false;
  }

  //***************************************************************************
  /// Greater than operator. cppreference 28
  //***************************************************************************
  template <typename T, typename U>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP14 operator>(const U& lhs, const etl::optional<T>& rhs)
  {
    return rhs.has_value() ? lhs > rhs.value() : true;
  }

  //***************************************************************************
  /// Greater than operator. cppreference 28
  //***************************************************************************
  template <typename T, typename U>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP20_STL operator>(const U& lhs, const etl::optional<T>& rhs)
  {
    return rhs.has_value() ? lhs > rhs.value() : true;
  }

  //***************************************************************************
  /// Greater than equal operator. cppreference 29
  //***************************************************************************
  template <typename T, typename U>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP14 operator>=(const etl::optional<T>& lhs, const U& rhs)
  {
    return lhs.has_value() ? lhs.value() >= rhs : false;
  }

  //***************************************************************************
  /// Greater than equal operator. cppreference 29
  //***************************************************************************
  template <typename T, typename U>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP20_STL operator>=(const etl::optional<T>& lhs, const U& rhs)
  {
    return lhs.has_value() ? lhs.value() >= rhs : false;
  }

  //***************************************************************************
  /// Greater than equal operator. cppreference 30
  //***************************************************************************
  template <typename T, typename U>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP14 operator>=(const U& lhs, const etl::optional<T>& rhs)
  {
    return rhs.has_value() ? lhs >= rhs.value() : true;
  }

  //***************************************************************************
  /// Greater than equal operator. cppreference 30
  //***************************************************************************
  template <typename T, typename U>
  ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP20_STL operator>=(const U& lhs, const etl::optional<T>& rhs)
  {
    return rhs.has_value() ? lhs >= rhs.value() : true;
  }

#include "private/diagnostic_pop.h"

#if ETL_CPP11_SUPPORTED
  //***************************************************************************
  /// Creates an optional object from `value`.
  //***************************************************************************
  template <typename T, typename U = T, ETL_OPTIONAL_ENABLE_CPP14>
  ETL_CONSTEXPR14 etl::optional<typename etl::decay<T>::type> make_optional(T&& value) //
    ETL_NOEXCEPT_IF((etl::is_nothrow_constructible<typename etl::decay<T>::type, T&&>::value))
  {
    return etl::optional<typename etl::decay<T>::type>(etl::forward<T>(value));
  }
  template <typename T, typename U = T, ETL_OPTIONAL_ENABLE_CPP20_STL>
  ETL_CONSTEXPR20_STL etl::optional<typename etl::decay<T>::type> make_optional(T&& value) //
    ETL_NOEXCEPT_IF((etl::is_nothrow_constructible<typename etl::decay<T>::type, T&&>::value))
  {
    return etl::optional<typename etl::decay<T>::type>(etl::forward<T>(value));
  }

  //***************************************************************************
  /// Creates an optional object constructed in-place from `args...` .
  /// Equivalent to `return etl::optional<T>(etl::in_place, etl::forward<Args>(args)...);`.
  /// This overload participates in overload resolution only if
  ///   `etl::is_constructible_v<T, Args...>` is true.
  //***************************************************************************
  template <typename T, typename... Args,                                                      //
            typename etl::enable_if< etl::is_constructible<T, Args...>::value, int>::type = 0, //
            typename U = T, ETL_OPTIONAL_ENABLE_CPP14>
  ETL_CONSTEXPR14 etl::optional<T> make_optional(Args&&... args)                                     //
    ETL_NOEXCEPT_IF((etl::is_nothrow_constructible<T, Args...>::value))
  {
    return etl::optional<T>(etl::in_place_t{}, etl::forward<Args>(args)...);
  }
  template <typename T, typename... Args,                                                      //
            typename etl::enable_if< etl::is_constructible<T, Args...>::value, int>::type = 0, //
            typename U                                                                    = T, ETL_OPTIONAL_ENABLE_CPP20_STL>
  ETL_CONSTEXPR20_STL etl::optional<T> make_optional(Args&&... args) //
    ETL_NOEXCEPT_IF((etl::is_nothrow_constructible<T, Args...>::value))
  {
    return etl::optional<T>(etl::in_place_t{}, etl::forward<Args>(args)...);
  }

  #if ETL_HAS_INITIALIZER_LIST
  //***************************************************************************
  /// Creates an optional object constructed in-place from `ilist` and `args...`.
  /// Equivalent to `return etl::optional<T>(std::in_place, ilist, std::forward<Args>(args)...);`.
  /// This overload participates in overload resolution only if
  ///   `etl::is_constructible_v<T, std::initializer_list<U>&, Args...>` is true.
  //***************************************************************************
  template <typename T, typename TL, typename... Args,                                                                     //
            typename etl::enable_if< etl::is_constructible<T, std::initializer_list<TL>&, Args...>::value, int>::type = 0, //
            typename U = T, ETL_OPTIONAL_ENABLE_CPP14>
  ETL_CONSTEXPR14 etl::optional<T> make_optional(std::initializer_list<TL> ilist, Args&&... args)
    ETL_NOEXCEPT_IF((etl::is_nothrow_constructible<T, std::initializer_list<TL>&, Args...>::value))
  {
    return etl::optional<T>(etl::in_place_t{}, ilist, etl::forward<Args>(args)...);
  }
  template <typename T, typename TL, typename... Args,                                                                     //
            typename etl::enable_if< etl::is_constructible<T, std::initializer_list<TL>&, Args...>::value, int>::type = 0, //
            typename U = T, ETL_OPTIONAL_ENABLE_CPP20_STL>
  ETL_CONSTEXPR20_STL etl::optional<T> make_optional(std::initializer_list<TL> ilist, Args&&... args)
    ETL_NOEXCEPT_IF((etl::is_nothrow_constructible<T, std::initializer_list<TL>&, Args...>::value))
  {
    return etl::optional<T>(etl::in_place_t{}, ilist, etl::forward<Args>(args)...);
  }
  #endif
#else
  //***************************************************************************
  /// Make an optional.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR20_STL etl::optional<typename etl::decay<T>::type> make_optional(T& value)
  {
    return etl::optional<typename etl::decay<T>::type>(value);
  }
#endif

  //***************************************************************************
  /// Template deduction guides.
  //***************************************************************************
#if ETL_CPP17_SUPPORTED
  template <typename T>
  optional(T) -> optional<T>;
#endif

  //*************************************************************************
  /// Swaps the values.
  //*************************************************************************
  template <typename T>
  ETL_CONSTEXPR20_STL void swap(etl::optional<T>& lhs, etl::optional<T>& rhs) ETL_NOEXCEPT_FROM(lhs.swap(rhs))
  {
    lhs.swap(rhs);
  }

} // namespace etl

#undef ETL_OPTIONAL_ENABLE_CPP14
#undef ETL_OPTIONAL_ENABLE_CPP20_STL

#undef ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP14
#undef ETL_OPTIONAL_ENABLE_CONSTEXPR_BOOL_RETURN_CPP20_STL

#endif
