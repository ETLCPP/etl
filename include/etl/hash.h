///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 John Wellbelove

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

#ifndef ETL_HASH_INCLUDED
#define ETL_HASH_INCLUDED

#include "platform.h"

#if ETL_USING_8BIT_TYPES

// The default hash calculation.
#include "fnv_1.h"
#include "type_traits.h"
#include "static_assert.h"

#include <stdint.h>
#include <stdlib.h>

///\defgroup hash Standard hash calculations
///\ingroup maths

namespace etl
{
  namespace private_hash
  {
    //*************************************************************************
    /// Hash to use when size_t is 16 bits.
    /// T is always expected to be size_t.
    //*************************************************************************
    template <typename T>
    typename enable_if<sizeof(T) == sizeof(uint16_t), size_t>::type
    generic_hash(const uint8_t* begin, const uint8_t* end)
    {
      uint32_t h = fnv_1a_32(begin, end);

      return static_cast<size_t>(h ^ (h >> 16U));
    }

    //*************************************************************************
    /// Hash to use when size_t is 32 bits.
    /// T is always expected to be size_t.
    //*************************************************************************
    template <typename T>
    typename enable_if<sizeof(T) == sizeof(uint32_t), size_t>::type
    generic_hash(const uint8_t* begin, const uint8_t* end)
    {
      return fnv_1a_32(begin, end);
    }

#if ETL_USING_64BIT_TYPES
    //*************************************************************************
    /// Hash to use when size_t is 64 bits.
    /// T is always expected to be size_t.
    //*************************************************************************
    template <typename T>
    typename enable_if<sizeof(T) == sizeof(uint64_t), size_t>::type
    generic_hash(const uint8_t* begin, const uint8_t* end)
    {
      return fnv_1a_64(begin, end);
    }
#endif

    //*************************************************************************
    /// Primary definition of base hash class, by default is poisoned
    //*************************************************************************
    template<typename T, bool IsEnum=false>
    struct hash_base
    {
    private:
      hash_base();                                  // Can't default construct
      hash_base(const hash_base& other);            // Can't copy construct
      hash_base& operator=(const hash_base& other); // Can't copy assign

#if ETL_USING_CPP11
      hash_base(hash_base&& other);            // Can't move construct
      hash_base& operator=(hash_base&& other); // Can't move assign
#endif
    };

    // Specialization for enums depends on definitions for integers, so it comes later
  }

#if ETL_USING_CPP11
  //***************************************************************************
  /// Generic declaration for etl::hash, including default for enums
  ///\ingroup hash
  //***************************************************************************
  template <typename T>
  struct hash : private_hash::hash_base<T, etl::is_enum<T>::value>{};
#else
  //***************************************************************************
  /// Generic declaration for etl::hash
  ///\ingroup hash
  //***************************************************************************
  template <typename T> struct hash;
#endif

  //***************************************************************************
  /// Specialisation for bool.
  ///\ingroup hash
  //***************************************************************************
  template <>
  struct hash <bool>
  {
    ETL_STATIC_ASSERT(sizeof(size_t) >= sizeof(bool), "size_t smaller than type");

    size_t operator ()(bool v) const
    {
      return static_cast<size_t>(v);
    }
  };

  //***************************************************************************
  /// Specialisation for char.
  ///\ingroup hash
  //***************************************************************************
  template <>
  struct hash<char>
  {
    ETL_STATIC_ASSERT(sizeof(size_t) >= sizeof(char), "size_t smaller than type");

    size_t operator ()(char v) const
    {
      return static_cast<size_t>(v);
    }
  };

  //***************************************************************************
  /// Specialisation for signed char.
  ///\ingroup hash
  //***************************************************************************
  template<> struct
  hash<signed char>
  {
    ETL_STATIC_ASSERT(sizeof(size_t) >= sizeof(signed char), "size_t smaller than type");

    size_t operator ()(signed char v) const
    {
      return static_cast<size_t>(v);
    }
  };

  //***************************************************************************
  /// Specialisation for unsigned char.
  ///\ingroup hash
  //***************************************************************************
  template<>
  struct hash<unsigned char>
  {
    ETL_STATIC_ASSERT(sizeof(size_t) >= sizeof(unsigned char), "size_t smaller than type");

    size_t operator ()(unsigned char v) const
    {
      return static_cast<size_t>(v);
    }
  };

  //***************************************************************************
  /// Specialisation for wchar_t.
  ///\ingroup hash
  //***************************************************************************
  template<>
  struct hash<wchar_t>
  {
    size_t operator ()(wchar_t v) const
    {
      // If it's the same size as a size_t.
      if ETL_IF_CONSTEXPR(sizeof(size_t) >= sizeof(v))
      {
        return static_cast<size_t>(v);
      }
      else
      {
        uint8_t* p = reinterpret_cast<uint8_t*>(&v);
        return private_hash::generic_hash<size_t>(p, p + sizeof(v));
      }
    }
  };

  //***************************************************************************
  /// Specialisation for short.
  ///\ingroup hash
  //***************************************************************************
  template<>
  struct hash<short>
  {
    size_t operator ()(short v) const
    {
      // If it's the same size as a size_t.
      if ETL_IF_CONSTEXPR(sizeof(size_t) >= sizeof(v))
      {
        return static_cast<size_t>(v);
      }
      else
      {
        uint8_t* p = reinterpret_cast<uint8_t*>(&v);
        return private_hash::generic_hash<size_t>(p, p + sizeof(v));
      }
    }
  };

  //***************************************************************************
  /// Specialisation for unsigned short.
  ///\ingroup hash
  //***************************************************************************
  template<>
  struct hash<unsigned short>
  {
    size_t operator ()(unsigned short v) const
    {
      // If it's the same size as a size_t.
      if ETL_IF_CONSTEXPR(sizeof(size_t) >= sizeof(v))
      {
        return static_cast<size_t>(v);
      }
      else
      {
        uint8_t* p = reinterpret_cast<uint8_t*>(&v);
        return private_hash::generic_hash<size_t>(p, p + sizeof(v));
      }
    }
  };

  //***************************************************************************
  /// Specialisation for int.
  ///\ingroup hash
  //***************************************************************************
  template<>
  struct hash<int>
  {
    size_t operator ()(int v) const
    {
      // If it's the same size as a size_t.
      if ETL_IF_CONSTEXPR(sizeof(size_t) >= sizeof(v))
      {
        return static_cast<size_t>(v);
      }
      else
      {
        uint8_t* p = reinterpret_cast<uint8_t*>(&v);
        return private_hash::generic_hash<size_t>(p, p + sizeof(v));
      }
    }
  };

  //***************************************************************************
  /// Specialisation for unsigned int.
  ///\ingroup hash
  //***************************************************************************
  template<>
  struct hash<unsigned int>
  {
    size_t operator ()(unsigned int v) const
    {
      // If it's the same size as a size_t.
      if ETL_IF_CONSTEXPR(sizeof(size_t) >= sizeof(v))
      {
        return static_cast<size_t>(v);
      }
      else
      {
        uint8_t* p = reinterpret_cast<uint8_t*>(&v);
        return private_hash::generic_hash<size_t>(p, p + sizeof(v));
      }
    }
  };

  //***************************************************************************
  /// Specialisation for long.
  ///\ingroup hash
  //***************************************************************************
  template<>
  struct hash<long>
  {
    size_t operator ()(long v) const
    {
      // If it's the same size as a size_t.
      if ETL_IF_CONSTEXPR(sizeof(size_t) >= sizeof(v))
      {
        return static_cast<size_t>(v);
      }
      else
      {
        uint8_t* p = reinterpret_cast<uint8_t*>(&v);
        return private_hash::generic_hash<size_t>(p, p + sizeof(v));
      }
    }
  };

  //***************************************************************************
  /// Specialisation for long long.
  ///\ingroup hash
  //***************************************************************************
  template<>
  struct hash<long long>
  {
    size_t operator ()(long long v) const
    {
      // If it's the same size as a size_t.
      if ETL_IF_CONSTEXPR(sizeof(size_t) >= sizeof(v))
      {
        return static_cast<size_t>(v);
      }
      else
      {
        uint8_t* p = reinterpret_cast<uint8_t*>(&v);
        return private_hash::generic_hash<size_t>(p, p + sizeof(v));
      }
    }
  };

  //***************************************************************************
  /// Specialisation for unsigned long.
  ///\ingroup hash
  //***************************************************************************
  template<>
  struct hash<unsigned long>
  {
    size_t  operator ()(unsigned long v) const
    {
      // If it's the same size as a size_t.
      if ETL_IF_CONSTEXPR(sizeof(size_t) >= sizeof(v))
      {
        return static_cast<size_t>(v);
      }
      else
      {
        uint8_t* p = reinterpret_cast<uint8_t*>(&v);
        return private_hash::generic_hash<size_t>(p, p + sizeof(v));
      }
    }
  };

  //***************************************************************************
  /// Specialisation for unsigned long long.
  ///\ingroup hash
  //***************************************************************************
  template<>
  struct hash<unsigned long long>
  {
    size_t  operator ()(unsigned long long v) const
    {
      // If it's the same size as a size_t.
      if ETL_IF_CONSTEXPR(sizeof(size_t) >= sizeof(v))
      {
        return static_cast<size_t>(v);
      }
      else
      {
        uint8_t* p = reinterpret_cast<uint8_t*>(&v);
        return private_hash::generic_hash<size_t>(p, p + sizeof(v));
      }
    }
  };

  //***************************************************************************
  /// Specialisation for float.
  ///\ingroup hash
  //***************************************************************************
  template<>
  struct hash<float>
  {
    size_t operator ()(float v) const
    {
      // If it's the same size as a size_t.
      if ETL_IF_CONSTEXPR(sizeof(size_t) == sizeof(v))
      {
        union
        {
          size_t s;
          float  v;
        } u;

        if (v == -0.0f)
        { // -0.0 and 0.0 are represented differently at bit level
          v = 0.0f;
        }
        u.v = v;

        return u.s;
      }
      else
      {
        uint8_t* p = reinterpret_cast<uint8_t*>(&v);
        return private_hash::generic_hash<size_t>(p, p + sizeof(v));
      }
    }
  };

  //***************************************************************************
  /// Specialisation for double.
  ///\ingroup hash
  //***************************************************************************
  template<>
  struct hash<double>
  {
    size_t  operator ()(double v) const
    {
      // If it's the same size as a size_t.
      if ETL_IF_CONSTEXPR(sizeof(size_t) == sizeof(v))
      {
        union
        {
          size_t s;
          double v;
        } u;

        if (v == -0.0)
        { // -0.0 and 0.0 are represented differently at bit level
          v = 0.0;
        }
        u.v = v;

        return u.s;
      }
      else
      {
        uint8_t* p = reinterpret_cast<uint8_t*>(&v);
        return private_hash::generic_hash<size_t>(p, p + sizeof(v));
      }
    }
  };

  //***************************************************************************
  /// Specialisation for long double.
  ///\ingroup hash
  //***************************************************************************
  template<>
  struct hash<long double>
  {
    size_t operator ()(long double v) const
    {
      // If it's the same size as a size_t.
      if ETL_IF_CONSTEXPR(sizeof(size_t) == sizeof(v))
      {
        union
        {
          size_t s;
          long double v;
        } u;

        if (v == -0.0L)
        { // -0.0 and 0.0 are represented differently at bit level
          v = 0.0L;
        }
        u.v = v;

        return u.s;
      }
      else
      {
        uint8_t* p = reinterpret_cast<uint8_t*>(&v);
        return private_hash::generic_hash<size_t>(p, p + sizeof(v));
      }
    }
  };

  //***************************************************************************
  /// Specialisation for pointers.
  ///\ingroup hash
  //***************************************************************************
  template <typename T>
  struct hash<T*>
  {
    size_t operator ()(const T* v) const
    {
      // If it's the same size as a size_t.
      if (sizeof(size_t) == sizeof(T*))
      {
        union
        {
          size_t s;
          const T* v;
        } u;

        u.v = v;

        return u.s;
      }
      else
      {
        uint8_t* p = reinterpret_cast<uint8_t*>(&v);
        return private_hash::generic_hash<size_t>(p, p + sizeof(v));
      }
    }
  };

  namespace private_hash
  {
    //*************************************************************************
    /// Specialization for enums
    //*************************************************************************
    template<typename T>
    struct hash_base<T, true>
    {
      size_t operator()(T v) const
      {
        if (sizeof(size_t) >= sizeof(T))
        {
          return static_cast<size_t>(v);
        }
        else
        {
          return ::etl::hash<unsigned long long>()(static_cast<unsigned long long>(v));
        }
      }
    };
  }
}

#endif // ETL_USING_8BIT_TYPES

#endif
