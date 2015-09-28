///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl

Copyright(c) 2014 jwellbelove

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

#ifndef __ETL_HASH__
#define __ETL_HASH__

#include <stdint.h>

// The default hash calculation.
#include "fnv_1.h"

#include "type_traits.h"

///\defgroup hash Standard hash calculations
///\ingroup maths

namespace etl
{
  namespace __private_hash__
  {
    //*************************************************************************
    /// Hash to use when size_t is 32 bits.
    //*************************************************************************
    template <typename T>
    typename enable_if<sizeof(T) == sizeof(uint32_t), size_t>::type
    generic_hash(uint8_t* begin, uint8_t* end)
    {
      return fnv_1a_32(begin, end);
    }

    //*************************************************************************
    /// Hash to use when size_t is 64 bits.
    //*************************************************************************
    template <typename T>
    typename enable_if<sizeof(T) == sizeof(uint64_t), size_t>::type
    generic_hash(uint8_t* begin, uint8_t* end)
    {
      return fnv_1a_64(begin, end);
    }
  }

  //***************************************************************************
  /// Generic declaration for etl::hash
  ///\ingroup hash
  //***************************************************************************
  template <typename T> struct hash;

  //***************************************************************************
  /// Specialisation for bool.
  ///\ingroup hash
  //***************************************************************************
  template <> 
  struct hash <bool>
  {
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
      return static_cast<size_t>(v);
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
      return static_cast<size_t>(v);
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
      return static_cast<size_t>(v);
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
      return static_cast<size_t>(v);
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
      return static_cast<size_t>(v);
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
      if (sizeof(size_t) == sizeof(v))
      {
        return static_cast<size_t>(v);
      }
      else
      {
        uint8_t* p = reinterpret_cast<uint8_t*>(&v);

        return __private_hash__::generic_hash<size_t>(p, p + sizeof(v));
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
      if (sizeof(size_t) == sizeof(v))
      {
        return static_cast<size_t>(v);
      }
      else
      {
        uint8_t* p = reinterpret_cast<uint8_t*>(&v);

        return __private_hash__::generic_hash<size_t>(p, p + sizeof(v));
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
    size_t operator ()(unsigned long v) const
    {
      if (sizeof(size_t) == sizeof(v))
      {
        return static_cast<size_t>(v);
      }
      else
      {
        uint8_t* p = reinterpret_cast<uint8_t*>(&v);

        return __private_hash__::generic_hash<size_t>(p, p + sizeof(v));
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
    size_t operator ()(unsigned long long v) const
    {
      if (sizeof(size_t) == sizeof(v))
      {
        return static_cast<size_t>(v);
      }
      else
      {
        uint8_t* p = reinterpret_cast<uint8_t*>(&v);

        return __private_hash__::generic_hash<size_t>(p, p + sizeof(v));
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
      if (sizeof(size_t) == sizeof(v))
      {
        return *reinterpret_cast<size_t*>(&v);
      }
      else
      {
        uint8_t* p = reinterpret_cast<uint8_t*>(&v);

        return __private_hash__::generic_hash<size_t>(p, p + sizeof(v));
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
    size_t operator ()(double v) const
    {
      if (sizeof(size_t) == sizeof(v))
      {
        return *reinterpret_cast<size_t*>(&v);
      }
      else
      {
        uint8_t* p = reinterpret_cast<uint8_t*>(&v);

        return __private_hash__::generic_hash<size_t>(p, p + sizeof(v));
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
      if (sizeof(size_t) == sizeof(v))
      {
        return *reinterpret_cast<size_t*>(&v);
      }
      else
      {
        uint8_t* p = reinterpret_cast<uint8_t*>(&v);

        return __private_hash__::generic_hash<size_t>(p, p + sizeof(v));
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
    size_t operator ()(T* v) const
    {
      if (sizeof(size_t) == sizeof(v))
      {
        return reinterpret_cast<size_t>(v);
      }
      else
      {
        uint8_t* p = reinterpret_cast<uint8_t*>(&v);

        return __private_hash__::generic_hash<size_t>(p, p + sizeof(v));
      }
    }
  };
}

#endif
