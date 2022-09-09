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

#ifndef ETL_FNV_1_INCLUDED
#define ETL_FNV_1_INCLUDED

#include "platform.h"
#include "static_assert.h"
#include "type_traits.h"
#include "ihash.h"
#include "frame_check_sequence.h"

#include <stdint.h>

#if defined(ETL_COMPILER_KEIL)
#pragma diag_suppress 1300
#endif

///\defgroup fnv_1 FNV-1 & FNV-1a 32 & 64 bit hash calculations
///\ingroup maths

namespace etl
{
#if ETL_USING_64BIT_TYPES
  //***************************************************************************
  /// fnv_1 policy.
  /// Calculates FNV1.
  //***************************************************************************
  struct fnv_1_policy_64
  {
    typedef uint64_t value_type;

    uint64_t initial() const
    {
      return OFFSET_BASIS;
    }

    uint64_t add(uint64_t hash, uint8_t value) const
    {
      hash *= PRIME;
      hash ^= value;
      return  hash;
    }

    uint64_t final(uint64_t hash) const
    {
      return hash;
    }

    static ETL_CONSTANT uint64_t OFFSET_BASIS = 0xCBF29CE484222325ull;
    static ETL_CONSTANT uint64_t PRIME        = 0x00000100000001b3ull;
  };

  //***************************************************************************
  /// Calculates the fnv_1_64 hash.
  ///\ingroup fnv_1_64
  //***************************************************************************
  class fnv_1_64 : public etl::frame_check_sequence<fnv_1_policy_64>
  {
  public:

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    fnv_1_64()
    {
      this->reset();
    }

    //*************************************************************************
    /// Constructor from range.
    /// \param begin Start of the range.
    /// \param end   End of the range.
    //*************************************************************************
    template<typename TIterator>
    fnv_1_64(TIterator begin, const TIterator end)
    {
      this->reset();
      this->add(begin, end);
    }
  };

  //***************************************************************************
  /// fnv_1a policy.
  /// Calculates FNV1A.
  //***************************************************************************
  struct fnv_1a_policy_64
    {
    typedef uint64_t value_type;

    uint64_t initial() const
      {
      return OFFSET_BASIS;
    }

    uint64_t add(uint64_t hash, uint8_t value) const
    {
      hash ^= value;
      hash *= PRIME;
      return hash;
    }

    uint64_t final(uint64_t hash) const
    {
      return hash;
    }

    static ETL_CONSTANT uint64_t OFFSET_BASIS = 0xCBF29CE484222325ull;
    static ETL_CONSTANT uint64_t PRIME        = 0x00000100000001b3ull;
  };

  //***************************************************************************
  /// Calculates the fnv_1a_64 hash.
  ///\ingroup fnv_1a_64
  //***************************************************************************
  class fnv_1a_64 : public etl::frame_check_sequence<fnv_1a_policy_64>
  {
  public:

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    fnv_1a_64()
    {
      this->reset();
    }

    //*************************************************************************
    /// Constructor from range.
    /// \param begin Start of the range.
    /// \param end   End of the range.
    //*************************************************************************
    template<typename TIterator>
    fnv_1a_64(TIterator begin, const TIterator end)
    {
      this->reset();
      this->add(begin, end);
    }
  };
#endif

  //***************************************************************************
  /// fnv_1 policy.
  /// Calculates FNV1.
  //***************************************************************************
  struct fnv_1_policy_32
    {
    typedef uint32_t value_type;

    uint32_t initial() const
      {
      return OFFSET_BASIS;
    }

    uint32_t add(uint32_t hash, uint8_t value) const
    {
      hash *= PRIME;
      hash ^= value;
      return hash;
    }

    uint32_t final(uint32_t hash) const
    {
      return hash;
    }

    static ETL_CONSTANT uint32_t OFFSET_BASIS = 0x811C9DC5UL;
    static ETL_CONSTANT uint32_t PRIME        = 0x01000193UL;
  };

  //***************************************************************************
  /// Calculates the fnv_1_32 hash.
  ///\ingroup fnv_1_32
  //***************************************************************************
  class fnv_1_32 : public etl::frame_check_sequence<fnv_1_policy_32>
  {
  public:

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    fnv_1_32()
    {
      this->reset();
    }

    //*************************************************************************
    /// Constructor from range.
    /// \param begin Start of the range.
    /// \param end   End of the range.
    //*************************************************************************
    template<typename TIterator>
    fnv_1_32(TIterator begin, const TIterator end)
    {
      this->reset();
      this->add(begin, end);
    }
  };

  //***************************************************************************
  /// fnv_1a policy.
  /// Calculates FNV1A.
  //***************************************************************************
  struct fnv_1a_policy_32
    {
    typedef uint32_t value_type;

    uint32_t initial() const
      {
      return OFFSET_BASIS;
    }

    uint32_t add(uint32_t hash, uint8_t value) const
    {
      hash ^= value;
      hash *= PRIME;
      return hash;
    }

    uint32_t final(uint32_t hash) const
    {
      return hash;
    }

    static ETL_CONSTANT uint32_t OFFSET_BASIS = 0x811C9DC5UL;
    static ETL_CONSTANT uint32_t PRIME        = 0x01000193UL;
  };

  //***************************************************************************
  /// Calculates the fnv_1a_32 hash.
  ///\ingroup fnv_1a_32
  //***************************************************************************
  class fnv_1a_32 : public etl::frame_check_sequence<fnv_1a_policy_32>
  {
  public:

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    fnv_1a_32()
    {
      this->reset();
    }

    //*************************************************************************
    /// Constructor from range.
    /// \param begin Start of the range.
    /// \param end   End of the range.
    //*************************************************************************
    template<typename TIterator>
    fnv_1a_32(TIterator begin, const TIterator end)
    {
      this->reset();
      this->add(begin, end);
    }
  };
}

#endif
