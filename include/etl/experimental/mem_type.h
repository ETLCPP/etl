#pragma once

#include <stdint.h>

#include "../platform.h"
#include "../memory.h"
#include "../static_assert.h"

//*****************************************************************************
/// mem_type
//*****************************************************************************
template <size_t Size_ , size_t Alignment_>
class mem_type
{
public:

  static constexpr size_t Size      = Size_;
  static constexpr size_t Alignment = Alignment_;

  //***********************************
  template <size_t Other_Size, size_t Other_Alignment>
  constexpr mem_type(const mem_type<Other_Size, Other_Alignment>& other)
  {
    ETL_STATIC_ASSERT(Size      >= Other_Size,      "Other size is too large");
    ETL_STATIC_ASSERT(Alignment >= Other_Alignment, "Other alignment incompatible");

    memcpy(buffer, other.buffer, Size_);
  }

  //***********************************
  template <typename T>
  constexpr T& get()
  {
    ETL_STATIC_ASSERT(sizeof(T) <= Size, "Size of T is too large");
    ETL_STATIC_ASSERT(Alignment >= etl::alignment_of<T>::value, "Alignment of T is incompatible");

    return *reinterpret_cast<T*>(buffer);
  }

  //***********************************
  template <typename T>
  constexpr const T& get() const
  {
    ETL_STATIC_ASSERT(sizeof(T) <= Size, "Size of T is too large");
    ETL_STATIC_ASSERT(Alignment >= etl::alignment_of<T>::value, "Alignment of T is incompatible");

    return *reinterpret_cast<T*>(buffer);
  }

  //***********************************
  template <typename T>
  constexpr operator T() const
  {
    ETL_STATIC_ASSERT(sizeof(T) <= Size, "Size of T is too large");
    ETL_STATIC_ASSERT(Alignment >= etl::alignment_of<T>::value, "Alignment of T is incompatible");

    return *reinterpret_cast<T*>(buffer);
  }

  //***********************************
  template <size_t Other_Size, size_t Other_Alignment>
  constexpr mem_type& operator =(const mem_type<Other_Size, Other_Alignment>& rhs)
  {
    ETL_STATIC_ASSERT(Size >= Other_Size, "RHS size is too large");
    ETL_STATIC_ASSERT(Alignment >= Other_Alignment, "RHS alignment incompatible");

    memcpy(buffer, rhs.buffer, Size_);

    return *this;
  }

  //***********************************
  constexpr size_t size() const
  {
    return Size;
  }

  //***********************************
  constexpr size_t alignment() const
  {
    return Alignment;
  }

  //***********************************
  constexpr char* data() const
  {
    return buffer;
  }

private:

  etl::aligned_storage<Size, Alignment> buffer;
};

//*****************************************************************************
/// mem_type_ptr
//*****************************************************************************
template <size_t Size_>
class mem_type_ptr
{
public:

  static constexpr size_t Size = Size_;

  //***********************************
  constexpr mem_type_ptr()
    : pbuffer(ETL_NULLPTR)
  {
  }

  //***********************************
  constexpr mem_type_ptr(char* pbuffer_)
    : pbuffer(pbuffer_)
  {
  }

  //***********************************
  template <size_t Other_Size>
  constexpr mem_type_ptr(const mem_type_ptr<Other_Size& other, char* pbuffer_)
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
  constexpr T& get()
  {
    ETL_STATIC_ASSERT((uintptr_t(pbuffer) % etl::alignment_of<T>::value) == 0, "Alignment of T is incompatible");

    return *reinterpret_cast<T*>(pbuffer);
  }

  //***********************************
  template <typename T>
  constexpr const T& get() const
  {
    ETL_STATIC_ASSERT((uintptr_t(pbuffer) % etl::alignment_of<T>::value) == 0, "Alignment of T is incompatible");

    return *reinterpret_cast<T*>(pbuffer);
  }

  //***********************************
  template <typename T>
  constexpr operator T() const
  {
    ETL_STATIC_ASSERT((uintptr_t(pbuffer) % etl::alignment_of<T>::value) == 0, "Alignment of T is incompatible");

    return *reinterpret_cast<T*>(pbuffer);
  }

  //***********************************
  template <size_t Other_Size>
  mem_type_ptr& operator =(const mem_type_ptr<Other_Size>& rhs)
  {
    ETL_STATIC_ASSERT(Size >= Other_Size, "RHS size is too large");

    memcpy(pbuffer, rhs.pbuffer, Size_);

    return *this;
  }

  //***********************************
  constexpr size_t size() const
  {
    return Size;
  }

  //***********************************
  constexpr char* data() const
  {
    return pbuffer;
  }

private:

  char* pbuffer;
};
