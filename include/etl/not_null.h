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

    not_null_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
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

    not_null_contains_null(string_type file_name_, numeric_type line_number_)
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

    //*********************************
    /// Constructs a not_null from a pointer.
    /// Asserts if the pointer is null.
    //*********************************
    explicit not_null(T* ptr_)
      : ptr(ptr_) 
    {
      ETL_ASSERT(ptr_ != ETL_NULLPTR, ETL_ERROR(not_null_contains_null));
    }

    //*********************************
    /// Copy constructor from a not_null pointer.
    //*********************************
    not_null(const etl::not_null<T*>& other)
      : ptr(other.get()) 
    {
    }

    //*********************************
    /// Assignment from a not_null.
    //*********************************
    not_null& operator =(const etl::not_null<T*>& rhs) 
    {
      ptr = rhs.get();

      return *this;
    }

    //*********************************
    /// Assignment from a pointer.
    /// Asserts if the pointer is null.
    //*********************************
    not_null& operator =(T* rhs) 
    {
      ETL_ASSERT_OR_RETURN_VALUE(rhs != ETL_NULLPTR, ETL_ERROR(not_null_contains_null), *this);
      
      ptr = rhs;

      return *this;
    }

    //*********************************
    /// Gets the underlying pointer.
    //*********************************
    T* get() const 
    { 
      return ptr; 
    }

    //*********************************
    /// Implicit conversion to T*.
    //*********************************
    operator T*() const 
    { 
      return ptr; 
    }

    //*********************************
    /// Dereference operator.
    //*********************************
    T& operator*() const
    { 
      return *ptr; 
    }
  
    //*********************************
    /// Arrow operator.
    //*********************************
    T* operator->() const 
    { 
      return ptr; 
    }

  private:

    /// The underlying pointer.
    T* ptr;
  };

  //***************************************************************************
  // Partial specialisation for etl::unique_ptr
  // A container for unique_ptr that are not allowed to be null.
  //***************************************************************************
  template <typename T, typename TDeleter>
  class not_null<etl::unique_ptr<T, TDeleter>> 
  {
  private:

    // The unique_ptr type.
    typedef etl::unique_ptr<T, TDeleter> unique_ptr_type;

  public:

    //*********************************
    /// Constructs a not_null from a unique_ptr.
    /// Asserts if the unique_ptr is null.
    //*********************************
    explicit not_null(unique_ptr_type&& u_ptr_)
      : u_ptr(etl::move(u_ptr_)) 
    {
      ETL_ASSERT(u_ptr.get() != ETL_NULLPTR, ETL_ERROR(not_null_contains_null));
    }

#if ETL_USING_CPP11
    //*********************************
    /// Constructs a not_null from a unique_ptr.
    //*********************************
    not_null(etl::not_null<unique_ptr_type>&& other)
      : u_ptr(etl::move(other.u_ptr)) 
    {
    }

    //*********************************
    /// Assign from a unique_ptr.
    /// Asserts if the unique_ptr is null.
    //*********************************
    not_null& operator =(unique_ptr_type&& rhs) 
    {
      ETL_ASSERT_OR_RETURN_VALUE(rhs.get() != ETL_NULLPTR, ETL_ERROR(not_null_contains_null), *this);

      u_ptr = etl::move(rhs);

      return *this;
    }

    //*********************************
    /// Assign from a not_null.
    //*********************************
    not_null& operator =(etl::not_null<unique_ptr_type>&& rhs) 
    {
      u_ptr = etl::move(rhs.u_ptr);

      return *this;
    }
#endif

    //*********************************
    /// Gets the underlying unique_ptr.
    //*********************************
    T* get() const 
    { 
      return u_ptr.get(); 
    }

    //*********************************
    /// Implicit conversion to T*.
    //*********************************
    operator T*() const 
    { 
      return u_ptr.get(); 
    }
  
    //*********************************
    /// Dereference operator.
    //*********************************
    T& operator*() const
    { 
      return *u_ptr; 
    }
  
    //*********************************
    /// Arrow operator.
    //*********************************
    T* operator->() const 
    { 
      return u_ptr.get(); 
    }

    //*********************************
    /// Gets the underlying unique_ptr.
    //*********************************
    unique_ptr_type& unique() 
    { 
      return u_ptr; 
    }
  
    //*********************************
    /// Gets the underlying unique_ptr.
    //*********************************
    const unique_ptr_type& unique() const 
    { 
      return u_ptr; 
    }

  private:
  
    /// The underlying unique_ptr.
    unique_ptr_type u_ptr;
  };
}

#endif
