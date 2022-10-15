///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2022 John Wellbelove

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

#ifndef ETL_UNALIGNED_TYPE_INCLUDED
#define ETL_UNALIGNED_TYPE_INCLUDED

///\defgroup unaligned_types unaligned_types
/// Unaligned types utilities
///\ingroup utilities

#include "platform.h"
#include "type_traits.h"
#include "endianness.h"
#include "iterator.h"
#include "algorithm.h"

#include <string.h>

namespace etl
{
  namespace private_unaligned_type
  {
    //*************************************************************************
    /// unaligned_type_common
    /// Contains all functionality that doesn't require the type.
    //*************************************************************************
    template <size_t Size_>
    class unaligned_type_common
    {
    public:

      static ETL_CONSTANT size_t Size = Size_;

      typedef char*                                 pointer;
      typedef const char*                           const_pointer;
      typedef char*                                 iterator;
      typedef const char*                           const_iterator;
      typedef etl::reverse_iterator<iterator>       reverse_iterator;
      typedef etl::reverse_iterator<const_iterator> const_reverse_iterator;

      //*************************************************************************
      /// Default constructor
      //*************************************************************************
      ETL_CONSTEXPR unaligned_type_common()
        : storage()
      {
      }

      //*************************************************************************
      /// Size of the storage.
      //*************************************************************************
      ETL_CONSTEXPR size_t size() const
      {
        return Size;
      }

      //*************************************************************************
      /// Pointer to the beginning of the storage.
      //*************************************************************************
      pointer data()
      {
        return storage;
      }

      //*************************************************************************
      /// Const pointer to the beginning of the storage.
      //*************************************************************************
      ETL_CONSTEXPR const_pointer data() const
      {
        return storage;
      }

      //*************************************************************************
      /// Iterator to the beginning of the storage.
      //*************************************************************************
      iterator begin()
      {
        return iterator(storage);
      }

      //*************************************************************************
      /// Const iterator to the beginning of the storage.
      //*************************************************************************
      ETL_CONSTEXPR const_iterator begin() const
      {
        return const_iterator(storage);
      }

      //*************************************************************************
      /// Const iterator to the beginning of the storage.
      //*************************************************************************
      ETL_CONSTEXPR const_iterator cbegin() const
      {
        return const_iterator(storage);
      }

      //*************************************************************************
      /// Reverse iterator to the beginning of the storage.
      //*************************************************************************
      reverse_iterator rbegin()
      {
        return reverse_iterator(storage + Size);
      }

      //*************************************************************************
      /// Const reverse iterator to the beginning of the storage.
      //*************************************************************************
      ETL_CONSTEXPR14 const_reverse_iterator rbegin() const
      {
        return const_reverse_iterator(storage + Size);
      }

      //*************************************************************************
      /// Const reverse iterator to the beginning of the storage.
      //*************************************************************************
      ETL_CONSTEXPR14 const_reverse_iterator crbegin() const
      {
        return const_reverse_iterator(storage + Size);
      }

      //*************************************************************************
      /// Iterator to the end of the storage.
      //*************************************************************************
      iterator end()
      {
        return iterator(storage + Size);
      }

      //*************************************************************************
      /// Const iterator to the end of the storage.
      //*************************************************************************
      ETL_CONSTEXPR const_iterator end() const
      {
        return const_iterator(storage + Size);
      }

      //*************************************************************************
      /// Const iterator to the end of the storage.
      //*************************************************************************
      ETL_CONSTEXPR const_iterator cend() const
      {
        return const_iterator(storage + Size);
      }

      //*************************************************************************
      /// Reverse iterator to the end of the storage.
      //*************************************************************************
      reverse_iterator rend()
      {
        return reverse_iterator(storage);
      }

      //*************************************************************************
      /// Const reverse iterator to the end of the storage.
      //*************************************************************************
      ETL_CONSTEXPR14 const_reverse_iterator rend() const
      {
        return const_reverse_iterator(storage);
      }

      //*************************************************************************
      /// Const reverse iterator to the end of the storage.
      //*************************************************************************
      ETL_CONSTEXPR14 const_reverse_iterator crend() const
      {
        return const_reverse_iterator(storage);
      }

      //*************************************************************************
      /// Index operator.
      //*************************************************************************
      char& operator[](int i)
      {
        return storage[i];
      }

      //*************************************************************************
      /// Const index operator.
      //*************************************************************************
      ETL_CONSTEXPR const char& operator[](int i) const
      {
        return storage[i];
      }

    protected:

      char storage[Size];
    };
  }

  //*************************************************************************
  /// unaligned_type
  //*************************************************************************
  template <typename T, int Endian_>
  class unaligned_type : public private_unaligned_type::unaligned_type_common<sizeof(T)>
  {
  public:

    ETL_STATIC_ASSERT(etl::is_integral<T>::value || etl::is_floating_point<T>::value, "Unaligned type must be integral or floating point");

    typedef T value_type;

    static ETL_CONSTANT int Endian  = Endian_;
    static ETL_CONSTANT size_t Size = private_unaligned_type::unaligned_type_common<sizeof(T)>::Size;

    //*************************************************************************
    /// Default constructor
    //*************************************************************************
    ETL_CONSTEXPR unaligned_type()
    {
    }

    //*************************************************************************
    /// Construct from a value.
    //*************************************************************************
    ETL_CONSTEXPR14 unaligned_type(T value)
    {
      unaligned_copy<T>::copy(value, this->storage);
    }

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    template <int Endian_Other>
    ETL_CONSTEXPR14 unaligned_type(const unaligned_type<T, Endian_Other>& other)
    {
      unaligned_copy<T>::copy(other.data(), Endian_Other, this->storage);
    }

    //*************************************************************************
    /// Assignment operator
    //*************************************************************************
    ETL_CONSTEXPR14 unaligned_type& operator =(T value)
    {
      unaligned_copy<T>::copy(value, this->storage);

      return *this;
    }

    //*************************************************************************
    /// Assignment operator from other endianness.
    //*************************************************************************
    template <int Endian_Other>
    ETL_CONSTEXPR14 unaligned_type& operator =(const unaligned_type<T, Endian_Other>& other)
    {
      unaligned_copy<T>::copy(other.data(), Endian_Other, this->storage);

      return *this;
    }

    //*************************************************************************
    /// Equality operator
    //*************************************************************************
    friend ETL_CONSTEXPR14 bool operator ==(const unaligned_type& lhs, const unaligned_type& rhs)
    {
      return etl::equal(lhs.data(), lhs.data() + lhs.Size, rhs.data());
    }

    //*************************************************************************
    /// Equality operator
    //*************************************************************************
    friend ETL_CONSTEXPR14 bool operator ==(const unaligned_type& lhs, T rhs)
    {
      T lhs_value = lhs;
      return (lhs_value == rhs);
    }

    //*************************************************************************
    /// Equality operator
    //*************************************************************************
    friend ETL_CONSTEXPR14 bool operator ==(T lhs, const unaligned_type& rhs)
    {
      T rhs_value = rhs;
      return (lhs == rhs_value);
    }

    //*************************************************************************
    /// Inequality operator
    //*************************************************************************
    friend ETL_CONSTEXPR14 bool operator !=(const unaligned_type& lhs, T rhs)
    {
      return !(lhs == rhs);
    }

    //*************************************************************************
    /// Inequality operator
    //*************************************************************************
    friend ETL_CONSTEXPR14 bool operator !=(const unaligned_type& lhs, const unaligned_type& rhs)
    {
      return !(lhs == rhs);
    }

    //*************************************************************************
    /// Inequality operator
    //*************************************************************************
    friend ETL_CONSTEXPR14 bool operator !=(T lhs, const unaligned_type& rhs)
    {
      return !(lhs == rhs);
    }

    //*************************************************************************
    /// Conversion operator
    //*************************************************************************
    ETL_CONSTEXPR14 operator T() const
    {
      T value = T();

      unaligned_copy<T>::copy(this->storage, value);

      return value;
    }

    //*************************************************************************
    /// Get the value.
    //*************************************************************************
    ETL_CONSTEXPR14 T value() const
    {
      T value = T();

      unaligned_copy<T>::copy(this->storage, value);

      return value;
    }

    //*************************************************************************
    /// Unaligned copy
    //*************************************************************************
    template <typename U, size_t Size = sizeof(U)>
    struct unaligned_copy;

    //*******************************************
    // Size == 1
    //*******************************************
    template <typename U>
    struct unaligned_copy<U, 1U>
    {
      //*******************************
      static ETL_CONSTEXPR14 void copy(T value, char* store)
      {
        store[0] = static_cast<char>(value);
      }

      //*******************************
      static ETL_CONSTEXPR14 void copy(const char* store, T& value)
      {
        value = static_cast<T>(store[0]);
      }

      //*******************************
      static ETL_CONSTEXPR14 void copy(const char* src, int /*endian_src*/, char* dst)
      {
        dst[0] = src[0];
      }
    };

    //*******************************************
    // Size == 2
    //*******************************************
    template <typename U>
    struct unaligned_copy<U, 2U>
    {
      //*******************************
      static ETL_CONSTEXPR14 void copy(T value, char* store)
      {
        if (Endian == etl::endianness::value())
        {
          store[0] = static_cast<char>(value);
          store[1] = static_cast<char>(value >> (1U * CHAR_BIT));
        }
        else
        {
          store[1] = static_cast<char>(value);
          store[0] = static_cast<char>(value >> (1U * CHAR_BIT));
        }
      }

      //*******************************
      static ETL_CONSTEXPR14 void copy(const char* store, T& value)
      {
        if (Endian == etl::endianness::value())
        {
          value  = static_cast<T>(static_cast<unsigned char>(store[0]));
          value |= static_cast<T>(static_cast<unsigned char>(store[1])) << (1U * CHAR_BIT);
        }
        else
        {
          value  = static_cast<T>(static_cast<unsigned char>(store[1]));
          value |= static_cast<T>(static_cast<unsigned char>(store[0])) << (1U * CHAR_BIT);
        }
      }

      //*******************************
      static ETL_CONSTEXPR14 void copy(const char* src, int endian_src, char* dst)
      {
        if (Endian == endian_src)
        {
          dst[0] = src[0];
          dst[1] = src[1];
        }
        else
        {
          dst[0] = src[1];
          dst[1] = src[0];
        }
      }
    };

    //*******************************************
    // Size == 4
    //*******************************************
    template <typename U>
    struct unaligned_copy<U, 4U>
    {
      static ETL_CONSTEXPR14 void copy(T value, char* store)
      {
        if (Endian == etl::endianness::value())
        {
          store[0] = static_cast<char>(value);
          store[1] = static_cast<char>(value >> (1U * CHAR_BIT));
          store[2] = static_cast<char>(value >> (2U * CHAR_BIT));
          store[3] = static_cast<char>(value >> (3U * CHAR_BIT));
        }
        else
        {
          store[3] = static_cast<char>(value);
          store[2] = static_cast<char>(value >> (1U * CHAR_BIT));
          store[1] = static_cast<char>(value >> (2U * CHAR_BIT));
          store[0] = static_cast<char>(value >> (3U * CHAR_BIT));
        }
      }

      //*******************************
      static ETL_CONSTEXPR14 void copy(const char* store, T& value)
      {
        if (Endian == etl::endianness::value())
        {
          value  = static_cast<T>(static_cast<unsigned char>(store[0]));
          value |= static_cast<T>(static_cast<unsigned char>(store[1])) << (1U * CHAR_BIT);
          value |= static_cast<T>(static_cast<unsigned char>(store[2])) << (2U * CHAR_BIT);
          value |= static_cast<T>(static_cast<unsigned char>(store[3])) << (3U * CHAR_BIT);
        }
        else
        {
          value  = static_cast<T>(static_cast<unsigned char>(store[3]));
          value |= static_cast<T>(static_cast<unsigned char>(store[2])) << (1U * CHAR_BIT);
          value |= static_cast<T>(static_cast<unsigned char>(store[1])) << (2U * CHAR_BIT);
          value |= static_cast<T>(static_cast<unsigned char>(store[0])) << (3U * CHAR_BIT);
        }
      }

      //*******************************
      static ETL_CONSTEXPR14 void copy(const char* src, int endian_src, char* dst)
      {
        if (Endian == endian_src)
        {
          dst[0] = src[0];
          dst[1] = src[1];
          dst[2] = src[2];
          dst[3] = src[3];
        }
        else
        {
          dst[0] = src[3];
          dst[1] = src[2];
          dst[2] = src[1];
          dst[3] = src[0];
        }
      }
    };

    //*******************************************
    // Size == 8
    //*******************************************
    template <typename U>
    struct unaligned_copy<U, 8U>
    {
      static ETL_CONSTEXPR14 void copy(T value, char* store)
      {
        if (Endian == etl::endianness::value())
        {
          store[0] = static_cast<char>(value);
          store[1] = static_cast<char>(value >> (1U * CHAR_BIT));
          store[2] = static_cast<char>(value >> (2U * CHAR_BIT));
          store[3] = static_cast<char>(value >> (3U * CHAR_BIT));
          store[4] = static_cast<char>(value >> (4U * CHAR_BIT));
          store[5] = static_cast<char>(value >> (5U * CHAR_BIT));
          store[6] = static_cast<char>(value >> (6U * CHAR_BIT));
          store[7] = static_cast<char>(value >> (7U * CHAR_BIT));
        }
        else
        {
          store[7] = static_cast<char>(value);
          store[6] = static_cast<char>(value >> (1U * CHAR_BIT));
          store[5] = static_cast<char>(value >> (2U * CHAR_BIT));
          store[4] = static_cast<char>(value >> (3U * CHAR_BIT));
          store[3] = static_cast<char>(value >> (4U * CHAR_BIT));
          store[2] = static_cast<char>(value >> (5U * CHAR_BIT));
          store[1] = static_cast<char>(value >> (6U * CHAR_BIT));
          store[0] = static_cast<char>(value >> (7U * CHAR_BIT));
        }
      }

      //*******************************
      static ETL_CONSTEXPR14 void copy(const char* store, T& value)
      {
        if (Endian == etl::endianness::value())
        {
          value  = static_cast<T>(static_cast<unsigned char>(store[0]));
          value |= static_cast<T>(static_cast<unsigned char>(store[1])) << (1U * CHAR_BIT);
          value |= static_cast<T>(static_cast<unsigned char>(store[2])) << (2U * CHAR_BIT);
          value |= static_cast<T>(static_cast<unsigned char>(store[3])) << (3U * CHAR_BIT);
          value |= static_cast<T>(static_cast<unsigned char>(store[4])) << (4U * CHAR_BIT);
          value |= static_cast<T>(static_cast<unsigned char>(store[5])) << (5U * CHAR_BIT);
          value |= static_cast<T>(static_cast<unsigned char>(store[6])) << (6U * CHAR_BIT);
          value |= static_cast<T>(static_cast<unsigned char>(store[7])) << (7U * CHAR_BIT);
        }
        else
        {
          value  = static_cast<T>(static_cast<unsigned char>(store[7]));
          value |= static_cast<T>(static_cast<unsigned char>(store[6])) << (1U * CHAR_BIT);
          value |= static_cast<T>(static_cast<unsigned char>(store[5])) << (2U * CHAR_BIT);
          value |= static_cast<T>(static_cast<unsigned char>(store[4])) << (3U * CHAR_BIT);
          value |= static_cast<T>(static_cast<unsigned char>(store[3])) << (4U * CHAR_BIT);
          value |= static_cast<T>(static_cast<unsigned char>(store[2])) << (5U * CHAR_BIT);
          value |= static_cast<T>(static_cast<unsigned char>(store[1])) << (6U * CHAR_BIT);
          value |= static_cast<T>(static_cast<unsigned char>(store[0])) << (7U * CHAR_BIT);
        }
      }

      //*******************************
      static ETL_CONSTEXPR14 void copy(const char* src, int endian_src, char* dst)
      {
        if (Endian == endian_src)
        {
          dst[0] = src[0];
          dst[1] = src[1];
          dst[2] = src[2];
          dst[3] = src[3];
          dst[4] = src[4];
          dst[5] = src[5];
          dst[6] = src[6];
          dst[7] = src[7];
        }
        else
        {
          dst[0] = src[7];
          dst[1] = src[6];
          dst[2] = src[5];
          dst[3] = src[4];
          dst[4] = src[3];
          dst[5] = src[2];
          dst[6] = src[1];
          dst[7] = src[0];
        }
      }
    };
  };

#if ETL_HAS_CONSTEXPR_ENDIANNESS
  // Host order
  typedef unaligned_type<char,               etl::endianness::value()> host_char_t;
  typedef unaligned_type<signed char,        etl::endianness::value()> host_schar_t;
  typedef unaligned_type<unsigned char,      etl::endianness::value()> host_uchar_t;
  typedef unaligned_type<short,              etl::endianness::value()> host_short_t;
  typedef unaligned_type<unsigned short,     etl::endianness::value()> host_ushort_t;
  typedef unaligned_type<int,                etl::endianness::value()> host_int_t;
  typedef unaligned_type<unsigned int,       etl::endianness::value()> host_uint_t;
  typedef unaligned_type<long,               etl::endianness::value()> host_long_t;
  typedef unaligned_type<unsigned long,      etl::endianness::value()> host_ulong_t;
  typedef unaligned_type<long long,          etl::endianness::value()> host_long_long_t;
  typedef unaligned_type<unsigned long long, etl::endianness::value()> host_ulong_long_t;
#if ETL_USING_8BIT_TYPES
  typedef unaligned_type<int8_t,             etl::endianness::value()> host_int8_t;
  typedef unaligned_type<uint8_t,            etl::endianness::value()> host_uint8_t;
#endif
  typedef unaligned_type<int16_t,            etl::endianness::value()> host_int16_t;
  typedef unaligned_type<uint16_t,           etl::endianness::value()> host_uint16_t;
  typedef unaligned_type<int32_t,            etl::endianness::value()> host_int32_t;
  typedef unaligned_type<uint32_t,           etl::endianness::value()> host_uint32_t;
#if ETL_USING_64BIT_TYPES
  typedef unaligned_type<int64_t,            etl::endianness::value()> host_int64_t;
  typedef unaligned_type<uint64_t,           etl::endianness::value()> host_uint64_t;
#endif
  typedef unaligned_type<float,              etl::endianness::value()> host_float_t;
  typedef unaligned_type<double,             etl::endianness::value()> host_double_t;
  typedef unaligned_type<long double,        etl::endianness::value()> host_long_double_t;
#endif

  // Little Endian
  typedef unaligned_type<char,               etl::endian::little> le_char_t;
  typedef unaligned_type<signed char,        etl::endian::little> le_schar_t;
  typedef unaligned_type<unsigned char,      etl::endian::little> le_uchar_t;
  typedef unaligned_type<short,              etl::endian::little> le_short_t;
  typedef unaligned_type<unsigned short,     etl::endian::little> le_ushort_t;
  typedef unaligned_type<int,                etl::endian::little> le_int_t;
  typedef unaligned_type<unsigned int,       etl::endian::little> le_uint_t;
  typedef unaligned_type<long,               etl::endian::little> le_long_t;
  typedef unaligned_type<unsigned long,      etl::endian::little> le_ulong_t;
  typedef unaligned_type<long long,          etl::endian::little> le_long_long_t;
  typedef unaligned_type<unsigned long long, etl::endian::little> le_ulong_long_t;
#if ETL_USING_8BIT_TYPES
  typedef unaligned_type<int8_t,             etl::endian::little> le_int8_t;
  typedef unaligned_type<uint8_t,            etl::endian::little> le_uint8_t;
#endif
  typedef unaligned_type<int16_t,            etl::endian::little> le_int16_t;
  typedef unaligned_type<uint16_t,           etl::endian::little> le_uint16_t;
  typedef unaligned_type<int32_t,            etl::endian::little> le_int32_t;
  typedef unaligned_type<uint32_t,           etl::endian::little> le_uint32_t;
#if ETL_USING_64BIT_TYPES
  typedef unaligned_type<int64_t,            etl::endian::little> le_int64_t;
  typedef unaligned_type<uint64_t,           etl::endian::little> le_uint64_t;
#endif
  typedef unaligned_type<float,              etl::endian::little> le_float_t;
  typedef unaligned_type<double,             etl::endian::little> le_double_t;
  typedef unaligned_type<long double,        etl::endian::little> le_long_double_t;

  // Big Endian
  typedef unaligned_type<char,               etl::endian::big> be_char_t;
  typedef unaligned_type<signed char,        etl::endian::big> be_schar_t;
  typedef unaligned_type<unsigned char,      etl::endian::big> be_uchar_t;
  typedef unaligned_type<short,              etl::endian::big> be_short_t;
  typedef unaligned_type<unsigned short,     etl::endian::big> be_ushort_t;
  typedef unaligned_type<int,                etl::endian::big> be_int_t;
  typedef unaligned_type<unsigned int,       etl::endian::big> be_uint_t;
  typedef unaligned_type<long,               etl::endian::big> be_long_t;
  typedef unaligned_type<unsigned long,      etl::endian::big> be_ulong_t;
  typedef unaligned_type<long long,          etl::endian::big> be_long_long_t;
  typedef unaligned_type<unsigned long long, etl::endian::big> be_ulong_long_t;
#if ETL_USING_8BIT_TYPES
  typedef unaligned_type<int8_t,             etl::endian::big> be_int8_t;
  typedef unaligned_type<uint8_t,            etl::endian::big> be_uint8_t;
#endif
  typedef unaligned_type<int16_t,            etl::endian::big> be_int16_t;
  typedef unaligned_type<uint16_t,           etl::endian::big> be_uint16_t;
  typedef unaligned_type<int32_t,            etl::endian::big> be_int32_t;
  typedef unaligned_type<uint32_t,           etl::endian::big> be_uint32_t;
#if ETL_USING_64BIT_TYPES
  typedef unaligned_type<int64_t,            etl::endian::big> be_int64_t;
  typedef unaligned_type<uint64_t,           etl::endian::big> be_uint64_t;
#endif
  typedef unaligned_type<float,              etl::endian::big> be_float_t;
  typedef unaligned_type<double,             etl::endian::big> be_double_t;
  typedef unaligned_type<long double,        etl::endian::big> be_long_double_t;

  // Network Order
  typedef be_char_t        net_char_t;
  typedef be_schar_t       net_schar_t;
  typedef be_uchar_t       net_uchar_t;
  typedef be_short_t       net_short_t;
  typedef be_ushort_t      net_ushort_t;
  typedef be_int_t         net_int_t;
  typedef be_uint_t        net_uint_t;
  typedef be_long_t        net_long_t;
  typedef be_ulong_t       net_ulong_t;
  typedef be_long_long_t   net_long_long_t;
  typedef be_ulong_long_t  net_ulong_long_t;
#if ETL_USING_8BIT_TYPES
  typedef be_int8_t        net_int8_t;
  typedef be_uint8_t       net_uint8_t;
#endif
  typedef be_int16_t       net_int16_t;
  typedef be_uint16_t      net_uint16_t;
  typedef be_int32_t       net_int32_t;
  typedef be_uint32_t      net_uint32_t;
#if ETL_USING_64BIT_TYPES
  typedef be_int64_t       net_int64_t;
  typedef be_uint64_t      net_uint64_t;
#endif
  typedef be_float_t       net_float_t;
  typedef be_double_t      net_double_t;
  typedef be_long_double_t net_long_double_t;

#if __cpp_alias_templates
  template <typename T, int Endian>
  using unaligned_type_t = typename etl::unaligned_type<T, Endian>::type;
#endif

#if __cpp_inline_variables && __cpp_constexpr
  template <typename T, int Endian>
  constexpr size_t unaligned_type_v = etl::unaligned_type<T, Endian>::Size;
#endif
}

#endif
