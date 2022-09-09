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

#ifndef ETL_MURMUR3_INCLUDED
#define ETL_MURMUR3_INCLUDED

#include "platform.h"
#include "ihash.h"
#include "binary.h"
#include "error_handler.h"

#include <stdint.h>

#if defined(ETL_COMPILER_KEIL)
#pragma diag_suppress 1300
#endif

///\defgroup murmur3 Murmur3 hash calculations
///\ingroup maths

namespace etl
{
  //***************************************************************************
  /// Calculates the murmur3 hash.
  /// See https://en.wikipedia.org/wiki/MurmurHash for more details.
  ///\ingroup murmur3
  //***************************************************************************
  template <typename THash>
  class murmur3
  {
  public:

#if ETL_NOT_USING_64BIT_TYPES
    ETL_STATIC_ASSERT((etl::is_same<THash, uint32_t>::value), "Only 32 bit types supported");
#else
    ETL_STATIC_ASSERT((etl::is_same<THash, uint32_t>::value || etl::is_same<THash, uint64_t>::value), "Only 32 & 64 bit types supported");
#endif

    typedef THash value_type;

    //*************************************************************************
    /// Default constructor.
    /// \param seed The seed value. Default = 0.
    //*************************************************************************
    murmur3(value_type seed_ = 0)
      : seed(seed_)
    {
      reset();
    }

    //*************************************************************************
    /// Constructor from range.
    /// \param begin Start of the range.
    /// \param end   End of the range.
    /// \param seed  The seed value. Default = 0.
    //*************************************************************************
    template<typename TIterator>
    murmur3(TIterator begin, const TIterator end, value_type seed_ = 0)
      : seed(seed_)
    {
      ETL_STATIC_ASSERT(sizeof(typename etl::iterator_traits<TIterator>::value_type) == 1, "Incompatible type");

      reset();
      while (begin != end)
      {
        block |= (*begin) << (block_fill_count * 8U);
        ++begin;

        if (++block_fill_count == FULL_BLOCK)
        {
          add_block();
          block_fill_count = 0;
          block = 0;
        }

        ++char_count;
      }
    }

    //*************************************************************************
    /// Resets the hash to the initial state.
    //*************************************************************************
    void reset()
    {
      hash             = seed;
      char_count       = 0;
      block            = 0;
      block_fill_count = 0;
      is_finalised     = false;
    }

    //*************************************************************************
    /// Adds a range.
    /// \param begin
    /// \param end
    //*************************************************************************
    template<typename TIterator>
    void add(TIterator begin, const TIterator end)
    {
      ETL_STATIC_ASSERT(sizeof(typename etl::iterator_traits<TIterator>::value_type) == 1, "Incompatible type");
      ETL_ASSERT(!is_finalised, ETL_ERROR(hash_finalised));

      while (begin != end)
      {
        block |= (*begin) << (block_fill_count * 8U);
        ++begin;

        if (++block_fill_count == FULL_BLOCK)
        {
          add_block();
          block_fill_count = 0;
          block = 0;
        }

        ++char_count;
      }
    }

    //*************************************************************************
    /// Adds a uint8_t value.
    /// If the hash has already been finalised then a 'hash_finalised' error will be emitted.
    /// \param value The char to add to the hash.
    //*************************************************************************
    void add(uint8_t value_)
    {
      // We can't add to a finalised hash!
      ETL_ASSERT(!is_finalised, ETL_ERROR(hash_finalised));

      block |= value_ << (block_fill_count * 8U);

      if (++block_fill_count == FULL_BLOCK)
      {
        add_block();
        block_fill_count = 0;
        block = 0;
      }

      ++char_count;
    }

    //*************************************************************************
    /// Gets the hash value.
    //*************************************************************************
    value_type value()
    {
      finalise();
      return hash;
    }

    //*************************************************************************
    /// Conversion operator to value_type.
    //*************************************************************************
    operator value_type ()
    {
      return value();
    }

  private:

    //*************************************************************************
    /// Adds a filled block to the hash.
    //*************************************************************************
    void add_block()
    {
      block *= CONSTANT1;
      block = rotate_left(block, SHIFT1);
      block *= CONSTANT2;

      hash ^= block;
      hash = rotate_left(hash, SHIFT2);
      hash = (hash * MULTIPLY) + ADD;
    }

    //*************************************************************************
    /// Finalises the hash.
    //*************************************************************************
    void finalise()
    {
      if (!is_finalised)
      {
        block *= CONSTANT1;
        block = rotate_left(block, SHIFT1);
        block *= CONSTANT2;

        hash ^= block;
        hash ^= char_count;
        hash ^= (hash >> 16U);
        hash *= 0x85EBCA6BUL;
        hash ^= (hash >> 13U);
        hash *= 0xC2B2AE35UL;
        hash ^= (hash >> 16U);

        is_finalised = true;
      }
    }

    bool       is_finalised;
    uint8_t    block_fill_count;
    size_t     char_count;
    value_type block;
    value_type hash;
    value_type seed;

    static ETL_CONSTANT uint8_t    FULL_BLOCK = 4U;
    static ETL_CONSTANT value_type CONSTANT1  = 0xCC9E2D51UL;
    static ETL_CONSTANT value_type CONSTANT2  = 0x1B873593UL;
    static ETL_CONSTANT value_type SHIFT1     = 15;
    static ETL_CONSTANT value_type SHIFT2     = 13;
    static ETL_CONSTANT value_type MULTIPLY   = 5;
    static ETL_CONSTANT value_type ADD        = 0xE6546B64UL;
  };
}

#endif
