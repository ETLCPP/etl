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

#include "../platform.h"
#include "../memory.h"
#include "../static_assert.h"
#include "../largest.h"

namespace etl
{
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
    ETL_CONSTEXPR mem_cast()
    {
    }

    //***********************************
    template <size_t Other_Size, size_t Other_Alignment>
    ETL_CONSTEXPR mem_cast(const mem_cast<Other_Size, Other_Alignment>& other)
    {
      ETL_STATIC_ASSERT(Size >= Other_Size, "Other size is too large");
      ETL_STATIC_ASSERT(Alignment >= Other_Alignment, "Other alignment incompatible");

      memcpy(buffer, other.buffer, Size_);
    }

    //***********************************
    template <typename T>
    ETL_CONSTEXPR T& get()
    {
      ETL_STATIC_ASSERT(sizeof(T) <= Size, "Size of T is too large");
      ETL_STATIC_ASSERT(Alignment >= etl::alignment_of<T>::value, "Alignment of T is incompatible");

      return *static_cast<T*>(buffer);
    }

    //***********************************
    template <typename T>
    ETL_CONSTEXPR const T& get() const
    {
      ETL_STATIC_ASSERT(sizeof(T) <= Size, "Size of T is too large");
      ETL_STATIC_ASSERT(Alignment >= etl::alignment_of<T>::value, "Alignment of T is incompatible");

      return *static_cast<T*>(buffer);
    }

    //***********************************
    template <typename T>
    ETL_CONSTEXPR operator T() const
    {
      ETL_STATIC_ASSERT(sizeof(T) <= Size, "Size of T is too large");
      ETL_STATIC_ASSERT(Alignment >= etl::alignment_of<T>::value, "Alignment of T is incompatible");

      return *static_cast<T*>(buffer);
    }

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
    ETL_CONSTEXPR size_t size() const
    {
      return Size;
    }

    //***********************************
    ETL_CONSTEXPR size_t alignment() const
    {
      return Alignment;
    }

    //***********************************
    ETL_CONSTEXPR char* data()
    {
      return buffer;
    }

    //***********************************
    ETL_CONSTEXPR const char* data() const
    {
      return buffer;
    }

  private:

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
    ETL_CONSTEXPR mem_cast_ptr()
      : pbuffer(ETL_NULLPTR)
    {
    }

    //***********************************
    ETL_CONSTEXPR mem_cast_ptr(char* pbuffer_)
      : pbuffer(pbuffer_)
    {
    }

    //***********************************
    template <size_t Other_Size>
    ETL_CONSTEXPR mem_cast_ptr(const mem_cast_ptr<Other_Size>& other, char* pbuffer_)
      : pbuffer(pbuffer_)
    {
      ETL_STATIC_ASSERT(Size >= Other_Size, "Other size is too large");

      memcpy(buffer, other.buffer, Size_);
    }

    //***********************************
    void set(char* pbuffer_)
    {
      pbuffer = pbuffer_;
    }

    //***********************************
    template <typename T>
    ETL_CONSTEXPR T& get()
    {
      ETL_STATIC_ASSERT((uintptr_t(pbuffer) % etl::alignment_of<T>::value) == 0, "Alignment of T is incompatible");

      return *reinterpret_cast<T*>(pbuffer);
    }

    //***********************************
    template <typename T>
    ETL_CONSTEXPR const T& get() const
    {
      ETL_STATIC_ASSERT((uintptr_t(pbuffer) % etl::alignment_of<T>::value) == 0, "Alignment of T is incompatible");

      return *reinterpret_cast<T*>(pbuffer);
    }

    //***********************************
    template <typename T>
    ETL_CONSTEXPR operator T() const
    {
      ETL_STATIC_ASSERT((uintptr_t(pbuffer) % etl::alignment_of<T>::value) == 0, "Alignment of T is incompatible");

      return *reinterpret_cast<T*>(pbuffer);
    }

    //***********************************
    template <size_t Other_Size>
    mem_cast_ptr& operator =(const mem_cast_ptr<Other_Size>& rhs)
    {
      ETL_STATIC_ASSERT(Size >= Other_Size, "RHS size is too large");

      memcpy(pbuffer, rhs.pbuffer, Size_);

      return *this;
    }

    //***********************************
    ETL_CONSTEXPR size_t size() const
    {
      return Size;
    }

    //***********************************
    ETL_CONSTEXPR char* data() const
    {
      return pbuffer;
    }

  private:

    char* pbuffer;
  };

#if ETL_CPP11_SUPPORTED
  //*****************************************************************************
  /// mem_cast_var
  //*****************************************************************************
  template <typename... TTypes>
  class mem_cast_types : public etl::mem_cast<etl::largest<TTypes...>::size,
                                              etl::largest<TTypes...>::alignment>
  {
  };
#endif
}

#endif
