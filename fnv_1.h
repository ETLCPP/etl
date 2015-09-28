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

#ifndef __ETL_FNV_1__
#define __ETL_FNV_1__

#include <stdint.h>

#include "static_assert.h"
#include "type_traits.h"
#include "ihash.h"

#if defined(COMPILER_KEIL)
#pragma diag_suppress 1300 
#endif

///\defgroup fnv_1 FNV-1 & FNV-1a 32 & 64 bit hash calculations
///\ingroup maths

namespace etl
{
  //***************************************************************************
  /// Calculates the fnv_1_64 hash.
  ///\ingroup fnv_1_64
  //***************************************************************************
  class fnv_1_64
  {
  public:

    typedef uint64_t value_type;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    fnv_1_64()
    {
      reset();
    }

    //*************************************************************************
    /// Constructor from range.
    /// \param begin Start of the range.
    /// \param end   End of the range.
    //*************************************************************************
    template<typename TIterator>
    fnv_1_64(TIterator begin, const TIterator end)
    {
      STATIC_ASSERT(sizeof(typename std::iterator_traits<TIterator>::value_type) == 1, "Type not supported");

      reset();
      while (begin != end)
      {
        hash *= PRIME;
        hash ^= *begin++;
      }
    }

    //*************************************************************************
    /// Resets the CRC to the initial state.
    //*************************************************************************
    void reset()
    {
      hash = OFFSET_BASIS;
    }

    //*************************************************************************
    /// Adds a range.
    /// \param begin
    /// \param end
    //*************************************************************************
    template<typename TIterator>
    void add(TIterator begin, const TIterator end)
    {
      STATIC_ASSERT(sizeof(typename std::iterator_traits<TIterator>::value_type) == 1, "Type not supported");

      while (begin != end)
      {
        hash *= PRIME;
        hash ^= *begin++;
      }
    }

    //*************************************************************************
    /// \param value The char to add to the fnv_1_64.
    //*************************************************************************
    void add(uint8_t value)
    {
      hash *= PRIME;
      hash ^= value;
    }

    //*************************************************************************
    /// Gets the fnv_1_64 value.
    //*************************************************************************
    value_type value() const
    {
      return hash;
    }

    //*************************************************************************
    /// Conversion operator to value_type.
    //*************************************************************************
    operator value_type () const
    {
      return hash;
    }

  private:

    value_type hash;

    static const uint64_t OFFSET_BASIS = 0xCBF29CE484222325;
    static const uint64_t PRIME        = 0x00000100000001b3;
  };

  //***************************************************************************
  /// Calculates the fnv_1a_64 hash.
  ///\ingroup fnv_1a_64
  //***************************************************************************
  class fnv_1a_64
  {
  public:

    typedef uint64_t value_type;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    fnv_1a_64()
    {
      reset();
    }

    //*************************************************************************
    /// Constructor from range.
    /// \param begin Start of the range.
    /// \param end   End of the range.
    //*************************************************************************
    template<typename TIterator>
    fnv_1a_64(TIterator begin, const TIterator end)
    {
      STATIC_ASSERT(sizeof(typename std::iterator_traits<TIterator>::value_type) == 1, "Type not supported");

      reset();
      while (begin != end)
      {
        hash ^= *begin++;
        hash *= PRIME;
      }
    }

    //*************************************************************************
    /// Resets the CRC to the initial state.
    //*************************************************************************
    void reset()
    {
      hash = OFFSET_BASIS;
    }

    //*************************************************************************
    /// Adds a range.
    /// \param begin
    /// \param end
    //*************************************************************************
    template<typename TIterator>
    void add(TIterator begin, const TIterator end)
    {
      STATIC_ASSERT(sizeof(typename std::iterator_traits<TIterator>::value_type) == 1, "Type not supported");
      
      while (begin != end)
      {
        hash ^= *begin++;
        hash *= PRIME;
      }
    }

    //*************************************************************************
    /// \param value The char to add to the fnv_1a_64.
    //*************************************************************************
    void add(uint8_t value)
    {
      hash ^= value;
      hash *= PRIME;
    }

    //*************************************************************************
    /// Gets the fnv_1a_64 value.
    //*************************************************************************
    value_type value() const
    {
      return hash;
    }

    //*************************************************************************
    /// Conversion operator to value_type.
    //*************************************************************************
    operator value_type () const
    {
      return hash;
    }

  private:

    value_type hash;

    static const uint64_t OFFSET_BASIS = 0xCBF29CE484222325;
    static const uint64_t PRIME        = 0x00000100000001b3;
  };

  //***************************************************************************
  /// Calculates the fnv_1_32 hash.
  ///\ingroup fnv_1_32
  //***************************************************************************
  class fnv_1_32
  {
  public:

    typedef uint32_t value_type;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    fnv_1_32()
    {
      reset();
    }

    //*************************************************************************
    /// Constructor from range.
    /// \param begin Start of the range.
    /// \param end   End of the range.
    //*************************************************************************
    template<typename TIterator>
    fnv_1_32(TIterator begin, const TIterator end)
    {
      STATIC_ASSERT(sizeof(typename std::iterator_traits<TIterator>::value_type) == 1, "Type not supported");

      reset();
      while (begin != end)
      {
        hash *= PRIME;
        hash ^= *begin++;
      }
    }

    //*************************************************************************
    /// Resets the CRC to the initial state.
    //*************************************************************************
    void reset()
    {
      hash = OFFSET_BASIS;
    }

    //*************************************************************************
    /// Adds a range.
    /// \param begin
    /// \param end
    //*************************************************************************
    template<typename TIterator>
    void add(TIterator begin, const TIterator end)
    {
      STATIC_ASSERT(sizeof(typename std::iterator_traits<TIterator>::value_type) == 1, "Type not supported");

      while (begin != end)
      {
        hash *= PRIME;
        hash ^= *begin++;
      }
    }

    //*************************************************************************
    /// \param value The char to add to the fnv_1_32.
    //*************************************************************************
    void add(uint8_t value)
    {
      hash *= PRIME;
      hash ^= value;
    }

    //*************************************************************************
    /// Gets the fnv_1_32 value.
    //*************************************************************************
    value_type value() const
    {
      return hash;
    }

    //*************************************************************************
    /// Conversion operator to value_type.
    //*************************************************************************
    operator value_type () const
    {
      return hash;
    }

  private:

    value_type hash;

    static const uint32_t OFFSET_BASIS = 0x811C9DC5;
    static const uint32_t PRIME        = 0x01000193;
  };

  //***************************************************************************
  /// Calculates the fnv_1a_32 hash.
  ///\ingroup fnv_1a_32
  //***************************************************************************
  class fnv_1a_32
  {
  public:

    typedef uint32_t value_type;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    fnv_1a_32()
    {
      reset();
    }

    //*************************************************************************
    /// Constructor from range.
    /// \param begin Start of the range.
    /// \param end   End of the range.
    //*************************************************************************
    template<typename TIterator>
    fnv_1a_32(TIterator begin, const TIterator end)
    {
      STATIC_ASSERT(sizeof(typename std::iterator_traits<TIterator>::value_type) == 1, "Type not supported");

      reset();
      while (begin != end)
      {
        hash ^= *begin++;
        hash *= PRIME;
      }
    }

    //*************************************************************************
    /// Resets the CRC to the initial state.
    //*************************************************************************
    void reset()
    {
      hash = OFFSET_BASIS;
    }

    //*************************************************************************
    /// Adds a range.
    /// \param begin
    /// \param end
    //*************************************************************************
    template<typename TIterator>
    void add(TIterator begin, const TIterator end)
    {
      STATIC_ASSERT(sizeof(typename std::iterator_traits<TIterator>::value_type) == 1, "Type not supported");

      while (begin != end)
      {
        hash ^= *begin++;
        hash *= PRIME;
      }
    }

    //*************************************************************************
    /// \param value The char to add to the fnv_1a_32.
    //*************************************************************************
    void add(uint8_t value)
    {
      hash ^= value;
      hash *= PRIME;
    }

    //*************************************************************************
    /// Gets the fnv_1a_32 value.
    //*************************************************************************
    value_type value() const
    {
      return hash;
    }

    //*************************************************************************
    /// Conversion operator to value_type.
    //*************************************************************************
    operator value_type () const
    {
      return hash;
    }

  private:

    value_type hash;

    static const uint32_t OFFSET_BASIS = 0x811C9DC5;
    static const uint32_t PRIME        = 0x01000193;
  };
}

#endif
