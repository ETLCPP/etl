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

#ifndef __ETL_CHECKSUM__
#define __ETL_CHECKSUM__

#include <stdint.h>

#include "static_assert.h"
#include "type_traits.h"
#include "endian.h"
#include "ihash.h"

///\defgroup checksum Checksum calculation
///\ingroup maths

namespace etl
{
  //***************************************************************************
  /// Calculates the checksum.
  ///\tparam TSum       The type used for the sum.
  ///\tparam ENDIANNESS The endianness of the calculation for input types larger than uint8_t. Default = endian::little.
  ///\ingroup checksum
  //***************************************************************************
  template <typename TSum, const int ENDIANNESS = endian::little>
  class checksum : public etl::ihash
  {
  public:

    STATIC_ASSERT(is_unsigned<TSum>::value, "Signed TSum template parameter not supported");

    typedef TSum value_type;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    checksum()
      : ihash(etl::endian(ENDIANNESS))
    {
      reset();
    }

    //*************************************************************************
    /// Constructor from range.
    /// \param begin Start of the range.
    /// \param end   End of the range.
    //*************************************************************************
    template<typename TIterator>
    checksum(TIterator begin, const TIterator end)
      : ihash(etl::endian(ENDIANNESS))
    {
      reset();
      add(begin, end);
    }

    //*************************************************************************
    /// Resets the CRC to the initial state.
    //*************************************************************************
    void reset()
    {
      sum = 0;
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
    /// \param value The value to add to the checksum.
    //*************************************************************************
    template<typename TValue>
    void add(TValue value)
    {
      ihash::add(value);
    }

    //*************************************************************************
    /// \param value The uint8_t to add to the checksum.
    //*************************************************************************
    void add(uint8_t value)
    {
      sum += value;
    }

    //*************************************************************************
    /// Gets the checksum value.
    //*************************************************************************
    value_type value() const
    {
      return sum;
    }

    //*************************************************************************
    /// Conversion operator to value_type.
    //*************************************************************************
    operator value_type () const
    {
      return sum;
    }

    //*************************************************************************
    /// Gets the generic digest value.
    //*************************************************************************
    generic_digest digest()
    {
      return ihash::get_digest(sum);
    }

  private:

    value_type sum;
  };
}

#endif
