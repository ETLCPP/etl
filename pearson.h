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

#ifndef __ETL_PEARSON__
#define __ETL_PEARSON__

#include <stdint.h>

#include "static_assert.h"
#include "type_traits.h"
#include "endian.h"
#include "ihash.h"
#include "array.h"
#include "container.h"

#if defined(COMPILER_KEIL)
#pragma diag_suppress 1300 
#endif

///\defgroup pearson Pearson hash calculation
///\ingroup pearson

namespace etl
{
  //***************************************************************************
  /// Pearson lookup table
  /// \ingroup pearson
  //***************************************************************************
  extern const uint8_t PEARSON_LOOKUP[];

  //***************************************************************************
  /// Calculates a Pearson hash
  ///\tparam HASH_LENGTH The number of elements in the hash.
  ///\tparam ENDIANNESS  The endianness of the calculation for input types larger than uint8_t. Default = endian::little.
  /// \ingroup pearson
  //***************************************************************************
  template <const size_t HASH_LENGTH, const int ENDIANNESS = endian::little>
  class pearson : public etl::ihash
  {
  public:

    typedef etl::array<uint8_t, HASH_LENGTH> value_type;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    pearson()
      : first(true),
        ihash(etl::endian(ENDIANNESS))
    {
      reset();
    }

    //*************************************************************************
    /// Constructor from range.
    /// \param begin Start of the range.
    /// \param end   End of the range.
    //*************************************************************************
    template<typename TIterator>
    pearson(TIterator begin, const TIterator end)
      : first(true),
        ihash(etl::endian(ENDIANNESS))
    {
      reset();
      add(begin, end);
    }

    //*************************************************************************
    /// Resets the hash to the initial state.
    //*************************************************************************
    void reset()
    {
      hash.fill(0);
    }

    //*************************************************************************
    /// Adds a range.
    /// \param begin
    /// \param end
    //*************************************************************************
    template<typename TIterator>
    void add(TIterator begin, const TIterator end)
    {
      ihash::add(begin, end);
    }

    //*************************************************************************
    /// Adds a value.
    /// \param value The value to add to the hash.
    //*************************************************************************
    template<typename TValue>
    void add(TValue value)
    {
      ihash::add(value);
    }

    //*************************************************************************
    /// \param value The char to add to the hash.
    //*************************************************************************
    void add(uint8_t value)
    {
      if (first)
      {
        for (size_t i = 0; i < HASH_SIZE; ++i)
        {
          hash[i] = PEARSON_LOOKUP[(uint32_t(value) + i) % 256];
        }

        first = false;
      }
      else
      {
        for (size_t i = 0; i < HASH_SIZE; ++i)
        {
          hash[i] = PEARSON_LOOKUP[hash[i] ^ value];
        }
      }
    }

    //*************************************************************************
    /// Gets the hash value.
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
      return value();
    }

    //*************************************************************************
    /// Gets the generic digest value.
    //*************************************************************************
    generic_digest digest()
    {
      return generic_digest(etl::begin(hash), etl::end(hash));
    }

  private:

    bool first;
    value_type hash;
  };
}

#endif
