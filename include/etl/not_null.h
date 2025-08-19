///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2025 John Wellbelove

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

#ifndef ETL_NOT_NULL_INCLUDED
#define ETL_NOT_NULL_INCLUDED

#include "platform.h"
#include "error_handler.h"
#include "exception.h"
#include "static_assert.h"
#include "memory.h"
#include "type_traits.h"

namespace etl 
{
  //***************************************************************************
  /// The base class for not_null exceptions.
  //***************************************************************************
  class not_null_exception : public exception
  {
  public:

    not_null_exception(string_type reason_, string_type file_name_, numeric_type line_number_) ETL_NOEXCEPT_IF_NO_THROW
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// The exception when the not_null contains a null.
  //***************************************************************************
  class not_null_contains_null : public not_null_exception
  {
  public:

    not_null_contains_null(string_type file_name_, numeric_type line_number_) ETL_NOEXCEPT_IF_NO_THROW
      : not_null_exception(ETL_ERROR_TEXT("not_null:contains null", ETL_NOT_NULL_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  // not_null
  // Primary template
  //***************************************************************************
  template <typename T>
  class not_null;

  //***************************************************************************
  // Specialisation for T*
  // A container for pointers that are not allowed to be null.
  //***************************************************************************
  template <typename T>
  class not_null<T*> 
  {
  public:

    typedef T        value_type;
    typedef T*       pointer;
    typedef const T* const_pointer;
    typedef T&       reference;
    typedef const T& const_reference;
    typedef pointer  underlying_type;

    //*********************************
    /// Constructs a not_null from a pointer.
    /// Asserts if the pointer is null.
    //*********************************
    ETL_CONSTEXPR14 explicit not_null(underlying_type ptr_) ETL_NOEXCEPT_IF_NO_THROW
      : ptr(ptr_) 
    {
      ETL_ASSERT(ptr_ != ETL_NULLPTR, ETL_ERROR(not_null_contains_null));
    }

    //*********************************
    /// Copy construct from a not_null pointer.
    //*********************************
    ETL_CONSTEXPR14 not_null(const etl::not_null<T*>& other) ETL_NOEXCEPT
      : ptr(other.get()) 
    {
    }

    //*********************************
    /// Assignment from a pointer.
    /// Asserts if the pointer is null.
    //*********************************
    ETL_CONSTEXPR14 not_null& operator =(underlying_type rhs) ETL_NOEXCEPT_IF_NO_THROW
    {
      ETL_ASSERT_OR_RETURN_VALUE(rhs != ETL_NULLPTR, ETL_ERROR(not_null_contains_null), *this);

      ptr = rhs;

      return *this;
    }

    //*********************************
    /// Assignment from a not_null.
    //*********************************
    ETL_CONSTEXPR14 not_null& operator =(const etl::not_null<T*>& rhs) ETL_NOEXCEPT
    {
      ptr = rhs.get();

      return *this;
    }

    //*********************************
    /// Gets the underlying pointer.
    //*********************************
    ETL_CONSTEXPR14 pointer get() const ETL_NOEXCEPT
    { 
      return ptr; 
    }

    //*********************************
    /// Implicit conversion to pointer.
    //*********************************
    ETL_CONSTEXPR14 operator pointer() const ETL_NOEXCEPT
    { 
      return ptr; 
    }

    //*********************************
    /// Dereference operator.
    //*********************************
    ETL_CONSTEXPR14 reference operator*() const ETL_NOEXCEPT
    { 
      return *ptr; 
    }

    //*********************************
    /// Arrow operator.
    //*********************************
    ETL_CONSTEXPR14 pointer operator->() const ETL_NOEXCEPT
    { 
      return ptr; 
    }

  private:

    /// The underlying pointer.
    pointer ptr;
  };

  //***************************************************************************
  // Partial specialisation for etl::unique_ptr
  // A container for unique_ptr that are not allowed to be null.
  //***************************************************************************
  template <typename T, typename TDeleter>
  class not_null<etl::unique_ptr<T, TDeleter> > 
  {
  private:

    typedef etl::not_null<etl::unique_ptr<T, TDeleter> > this_type;
    typedef etl::unique_ptr<T, TDeleter> underlying_type;

  public:

    typedef T        value_type;
    typedef T*       pointer;
    typedef const T* const_pointer;
    typedef T&       reference;
    typedef const T& const_reference;

#if ETL_USING_CPP11
    //*********************************
    /// Constructs a not_null from a unique_ptr.
    /// Asserts if the unique_ptr contains null.
    /// Moves from the unique_ptr.
    //*********************************
    ETL_CONSTEXPR14 explicit not_null(underlying_type&& u_ptr_) ETL_NOEXCEPT_IF_NO_THROW
      : u_ptr(etl::move(u_ptr_)) 
    {
      ETL_ASSERT(u_ptr.get() != ETL_NULLPTR, ETL_ERROR(not_null_contains_null));
    }

    //*********************************
    /// Assign from a unique_ptr.
    /// Asserts if the unique_ptr contains null.
    /// Moves from the unique_ptr.
    //*********************************
    ETL_CONSTEXPR14 not_null& operator =(underlying_type&& rhs) ETL_NOEXCEPT_IF_NO_THROW
    {
      ETL_ASSERT_OR_RETURN_VALUE(rhs.get() != ETL_NULLPTR, ETL_ERROR(not_null_contains_null), *this);

      u_ptr = etl::move(rhs);

      return *this;
    }
#endif

    //*********************************
    /// Gets the underlying ptr.
    //*********************************
    ETL_CONSTEXPR14 pointer get() const ETL_NOEXCEPT
    { 
      return u_ptr.get(); 
    }

    //*********************************
    /// Implicit conversion to pointer.
    //*********************************
    ETL_CONSTEXPR14 operator pointer() const ETL_NOEXCEPT
    { 
      return u_ptr.get(); 
    }
  
    //*********************************
    /// Dereference operator.
    //*********************************
    ETL_CONSTEXPR14 reference operator*() const ETL_NOEXCEPT
    { 
      return *u_ptr; 
    }
  
    //*********************************
    /// Arrow operator.
    //*********************************
    ETL_CONSTEXPR14 pointer operator->() const ETL_NOEXCEPT
    { 
      return u_ptr.get(); 
    }

  private:
  
    ETL_CONSTEXPR14 explicit not_null(const this_type& u_ptr_) ETL_NOEXCEPT ETL_DELETE;
    ETL_CONSTEXPR14 not_null& operator=(const this_type& rhs) ETL_NOEXCEPT  ETL_DELETE;

#if ETL_USING_CPP11
    ETL_CONSTEXPR14 explicit not_null(this_type&& u_ptr_) ETL_NOEXCEPT = delete;
    ETL_CONSTEXPR14 not_null& operator=(this_type&& rhs) ETL_NOEXCEPT = delete;
#endif

    /// The underlying unique_ptr.
    underlying_type u_ptr;
  };
}

#endif
