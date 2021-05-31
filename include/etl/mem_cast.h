///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 jwellbelove

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

#ifndef ETL_MEM_CAST_INCLUDED
#define ETL_MEM_CAST_INCLUDED

#include <stdint.h>
#include <string.h>

#include "platform.h"
#include "memory.h"
#include "static_assert.h"
#include "largest.h"
#include "utility.h"
#include "placement_new.h"
#include "exception.h"
#include "error_handler.h"
#include "file_error_numbers.h"

namespace etl
{
  //***************************************************************************
  /// The base class for array_wrapper exceptions.
  //***************************************************************************
  class mem_cast_exception : public etl::exception
  {
  public:

    mem_cast_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// The exception thrown when the buffer pointer alignment is not compatible.
  //***************************************************************************
  class mem_cast_alignment_exception : public etl::mem_cast_exception
  {
  public:

    mem_cast_alignment_exception(string_type file_name_, numeric_type line_number_)
      : mem_cast_exception(ETL_ERROR_TEXT("mem_cast:alignment", ETL_MEM_CAST_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// The exception thrown when the pointer is null.
  //***************************************************************************
  class mem_cast_nullptr_exception : public etl::mem_cast_exception
  {
  public:

    mem_cast_nullptr_exception(string_type file_name_, numeric_type line_number_)
      : mem_cast_exception(ETL_ERROR_TEXT("mem_cast:null pointer", ETL_MEM_CAST_FILE_ID"B"), file_name_, line_number_)
    {
    }
  };

  //*****************************************************************************
  /// mem_cast
  //*****************************************************************************
  template <size_t Size_, size_t Alignment_>
  class mem_cast
  {
  public:

    static ETL_CONSTANT size_t Size      = Size_;
    static ETL_CONSTANT size_t Alignment = Alignment_;

    //***********************************
    /// Default constructor
    //***********************************
    ETL_CONSTEXPR mem_cast()
      : buffer()
    {
    }

    //***********************************
    /// Copy constructor
    //***********************************
    template <size_t Other_Size, size_t Other_Alignment>
    ETL_CONSTEXPR mem_cast(const mem_cast<Other_Size, Other_Alignment>& other)
      : type_size(other.type_size)
    {
      ETL_STATIC_ASSERT(Size >= Other_Size, "Other size is too large");
      ETL_STATIC_ASSERT(Alignment >= Other_Alignment, "Other alignment incompatible");

      memcpy(buffer, other.buffer, Size_);
    }

#if ETL_CPP11_SUPPORTED
    //***********************************
    /// Emplace from parameters
    //***********************************
    template <typename T, typename... TArgs>
    void emplace(TArgs... args)
    {
      ::new (static_cast<void*>(buffer)) T(etl::forward<TArgs>(args)...);
    }
#endif

    //***********************************
    /// Get a reference to T
    //***********************************
    template <typename T>
    ETL_CONSTEXPR T& ref()
    {
      ETL_STATIC_ASSERT(sizeof(T) <= Size, "Size of T is too large");
      ETL_STATIC_ASSERT(Alignment >= etl::alignment_of<T>::value, "Alignment of T is incompatible");

      return *static_cast<T*>(buffer);
    }

    //***********************************
    /// Get a const reference to T
    //***********************************
    template <typename T>
    ETL_CONSTEXPR const T& ref() const
    {
      ETL_STATIC_ASSERT(sizeof(T) <= Size, "Size of T is too large");
      ETL_STATIC_ASSERT(Alignment >= etl::alignment_of<T>::value, "Alignment of T is incompatible");

      return *static_cast<const T*>(buffer);
    }

    //***********************************
    /// Assignment operator
    //***********************************
    template <size_t Other_Size, size_t Other_Alignment>
    ETL_CONSTEXPR mem_cast& operator =(const mem_cast<Other_Size, Other_Alignment>& rhs)
    {
      ETL_STATIC_ASSERT(Size >= Other_Size, "RHS size is too large");
      ETL_STATIC_ASSERT(Alignment >= Other_Alignment, "RHS alignment incompatible");

      memcpy(buffer, rhs.buffer, Size_);

      return *this;
    }

    //***********************************
    /// Get the size of the buffer
    //***********************************
    ETL_CONSTEXPR size_t size() const
    {
      return Size;
    }

    //***********************************
    /// Get the alignment of the buffer
    //***********************************
    ETL_CONSTEXPR size_t alignment() const
    {
      return Alignment;
    }

    //***********************************
    /// Get a pointer to the internal buffer
    //***********************************
    ETL_CONSTEXPR char* data()
    {
      return buffer;
    }

    //***********************************
    /// Get a const pointer to the internal buffer
    //***********************************
    ETL_CONSTEXPR const char* data() const
    {
      return buffer;
    }

  private:

    /// The internal buffer
    etl::uninitialized_buffer<Size, 1U, Alignment> buffer;
  };

  //*****************************************************************************
  /// mem_cast_ptr
  //*****************************************************************************
  template <size_t Size_>
  class mem_cast_ptr
  {
  public:

    static ETL_CONSTANT size_t Size = Size_;

    //***********************************
    /// Default constructor
    //***********************************
    ETL_CONSTEXPR mem_cast_ptr()
      : pbuffer(ETL_NULLPTR)
    {
    }

    //***********************************
    /// Construct with pointer to buffer
    //***********************************
    ETL_CONSTEXPR mem_cast_ptr(char* pbuffer_)
      : pbuffer(pbuffer_)
    {
    }

    //***********************************
    /// Copy construct with pointer to buffer
    //***********************************
    template <size_t Other_Size>
    ETL_CONSTEXPR mem_cast_ptr(const mem_cast_ptr<Other_Size>& other, char* pbuffer_)
      : pbuffer(pbuffer_)
    {
      ETL_ASSERT((pbuffer != ETL_NULLPTR), ETL_ERROR(etl::mem_cast_nullptr_exception));
      ETL_STATIC_ASSERT(Size >= Other_Size, "Other size is too large");

      memcpy(pbuffer, other.pbuffer, Size_);
    }

#if ETL_CPP11_SUPPORTED
    //***********************************
    /// Emplace from parameters
    //***********************************
    template <typename T, typename... TArgs>
    void emplace(TArgs... args)
    {
      ETL_ASSERT((pbuffer != ETL_NULLPTR), ETL_ERROR(etl::mem_cast_nullptr_exception));
      ETL_ASSERT((uintptr_t(pbuffer) % etl::alignment_of<T>::value) == 0, ETL_ERROR(etl::mem_cast_alignment_exception));

      ::new (pbuffer) T(etl::forward<TArgs>(args)...);
    }
#endif

    //***********************************
    /// Get a reference to T
    //***********************************
    template <typename T>
    ETL_CONSTEXPR T& ref()
    {
      ETL_ASSERT((pbuffer != ETL_NULLPTR), ETL_ERROR(etl::mem_cast_nullptr_exception));
      ETL_ASSERT((uintptr_t(pbuffer) % etl::alignment_of<T>::value) == 0, ETL_ERROR(etl::mem_cast_alignment_exception));

      return *reinterpret_cast<T*>(pbuffer);
    }

    //***********************************
    /// Get a const reference to T
    //***********************************
    template <typename T>
    ETL_CONSTEXPR const T& ref() const
    {
      ETL_ASSERT((pbuffer != ETL_NULLPTR), ETL_ERROR(etl::mem_cast_nullptr_exception));
      ETL_ASSERT((uintptr_t(pbuffer) % etl::alignment_of<T>::value) == 0, ETL_ERROR(etl::mem_cast_alignment_exception));

      return *reinterpret_cast<const T*>(pbuffer);
    }

    //***********************************
    template <size_t Other_Size>
    mem_cast_ptr& operator =(const mem_cast_ptr<Other_Size>& rhs)
    {
      ETL_ASSERT((pbuffer != ETL_NULLPTR), ETL_ERROR(etl::mem_cast_nullptr_exception));
      ETL_STATIC_ASSERT(Size >= Other_Size, "RHS size is too large");

      memcpy(pbuffer, rhs.pbuffer, Size_);

      return *this;
    }

    //***********************************
    /// Assignment operator
    //***********************************
    ETL_CONSTEXPR size_t size() const
    {
      return Size;
    }

    //***********************************
    /// Get a pointer to the internal buffer
    //***********************************
    void data(char* pbuffer_)
    {
      pbuffer = pbuffer_;
    }

    //***********************************
    /// Get a const pointer to the internal buffer
    //***********************************
    ETL_CONSTEXPR char* data() const
    {
      return pbuffer;
    }

  private:

    /// Pointer to the buffer
    char* pbuffer;
  };

#if ETL_CPP11_SUPPORTED
  //*****************************************************************************
  /// mem_cast_var
  /// mem_cast from a variadic list of types
  //*****************************************************************************
  template <typename... TTypes>
  using mem_cast_types = etl::mem_cast<etl::largest<TTypes...>::size, etl::largest<TTypes...>::alignment>;
#endif
}

#endif
