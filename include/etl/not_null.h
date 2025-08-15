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
#include "ETL_STATIC_ASSERT.h"
#include "unique_ptr.h"
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
      : delegate_exception(ETL_ERROR_TEXT("not_null:contains null", ETL_NOT_NULL_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  // Primary template
  template <typename T>
  class not_null;

  // Specialisation for T*
  template <typename T>
  class not_null<T*> 
  {
  public:

    explicit not_null(T* ptr_)
      : ptr(ptr_) 
    {
      ETL_ASSERT(ptr != ETL_NULLPTR, ETL_ERROR(not_null_contains_null));
    }

    not_null(const etl::not_null& other)
      : ptr(other.get()) 
    {
    }

    not_null& operator =(const etl::not_null& other) 
    {
      ptr = other.get();

      return *this;
    }

    not_null& operator =(T* ptr_) 
    {
      ETL_ASSERT(ptr_ != ETL_NULLPTR, ETL_ERROR(not_null_contains_null));
      
      ptr = ptr_;

      return *this;
    }

    T* get() const 
    { 
      return ptr; 
    }

    operator T*() const 
    { 
      return ptr; 
    }

    typename etl::remove_pointer<T*>::type& operator*() const
    { 
      return *ptr; 
    }
  
    T operator->() const 
    { 
      return ptr; 
    }

  private:

    T* ptr;
  };

  // Partial specialisation for etl::unique_ptr
  template <typename T, typename TDeleter>
  class not_null<etl::unique_ptr<T, TDeleter>> 
  {
  public:

    explicit not_null(etl::unique_ptr<T, TDeleter>&& ptr_)
      : ptr(etl::move(ptr_)) 
    {
      ETL_ASSERT(ptr != ETL_NULLPTR, ETL_ERROR(not_null_contains_null));
    }

#if ETL_USING_CPP11
    not_null& operator =(etl::unique_ptr<T, TDeleter>&& ptr_) 
    {
      ETL_ASSERT(ptr_ != ETL_NULLPTR, ETL_ERROR(not_null_contains_null));

      ptr = etl::move(ptr_);

      return *this;
    }
#endif

    T* get() const 
    { 
      return ptr.get(); 
    }

    operator T*() const 
    { 
      return ptr.get(); 
    }
  
    T& operator*() const
    { 
      return *ptr; 
    }
  
    T* operator->() const 
    { 
      return ptr.get(); 
    }

    etl::unique_ptr<T, TDeleter>& unique() 
    { 
      return ptr; 
    }
  
    const etl::unique_ptr<T, TDeleter>& unique() const 
    { 
      return ptr; 
    }

  private:
  
    etl::unique_ptr<T, TDeleter> ptr;
  };
}
