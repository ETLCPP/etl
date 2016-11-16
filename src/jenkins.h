///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

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

#ifndef __ETL_JENKINS__
#define __ETL_JENKINS__

#include <stdint.h>
#include <iterator>

#include "platform.h"
#include "static_assert.h"
#include "type_traits.h"
#include "error_handler.h"
#include "ihash.h"

#if defined(ETL_COMPILER_KEIL)
#pragma diag_suppress 1300 
#endif

///\defgroup jenkins Jenkins 32 & 64 bit hash calculations
///\ingroup maths

namespace etl
{
  //***************************************************************************
  /// Jenkins32 policy.
  /// Calculates 32 bit Jenkins hash.
  //***************************************************************************
  struct jenkins32_policy
  {
    typedef uint32_t value_type;

    inline uint32_t initial() const
    {
      is_finalised = false;

      return 0;
    }

    inline uint32_t add(uint8_t hash, uint8_t value) const
    {
      ETL_ASSERT(!is_finalised, ETL_ERROR(hash_finalised));

      hash += value;
      hash += (hash << 10);
      hash ^= (hash >> 6);

      return hash;
    }

    inline uint32_t final(uint8_t hash) const
    {
      hash += (hash << 3);
      hash ^= (hash >> 11);
      hash += (hash << 15);
      is_finalised = true;

      return hash;
    }

    bool is_finalised;
  };

  //***************************************************************************
  /// Jenkins64 policy.
  /// Calculates 32 bit Jenkins hash.
  //***************************************************************************
  struct jenkins64_policy
  {
    typedef uint64_t value_type;

    inline uint64_t initial() const
    {
      is_finalised = false;

      return 0;
    }

    inline uint64_t add(uint8_t hash, uint8_t value) const
    {
      ETL_ASSERT(!is_finalised, ETL_ERROR(hash_finalised));

      hash += value;
      hash += (hash << 10);
      hash ^= (hash >> 6);

      return hash;
    }

    inline uint64_t final(uint8_t hash) const
    {
      hash += (hash << 3);
      hash ^= (hash >> 11);
      hash += (hash << 15);
      is_finalised = true;

      return hash;
    }

    bool is_finalised;
  };

  //*************************************************************************
  /// Jenkins32
  //*************************************************************************
  class jenkins32 : public etl::frame_check_sequence<etl::jenkins32_policy>
  {
  public:

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    jenkins32()
    {
      this->reset();
    }

    //*************************************************************************
    /// Constructor from range.
    /// \param begin Start of the range.
    /// \param end   End of the range.
    //*************************************************************************
    template<typename TIterator>
    jenkins32(TIterator begin, const TIterator end)
    {
      this->reset();
      this->add(begin, end);
    }
  };

  //*************************************************************************
  /// Jenkins64
  //*************************************************************************
  class jenkins64 : public etl::frame_check_sequence<etl::jenkins64_policy>
  {
  public:

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    jenkins64()
    {
      this->reset();
    }

    //*************************************************************************
    /// Constructor from range.
    /// \param begin Start of the range.
    /// \param end   End of the range.
    //*************************************************************************
    template<typename TIterator>
    jenkins64(TIterator begin, const TIterator end)
    {
      this->reset();
      this->add(begin, end);
    }
  };
}

#endif
