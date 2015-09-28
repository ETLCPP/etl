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

#ifndef __ETL_BSDCHECKSUM__
#define __ETL_BSDCHECKSUM__

#include <stdint.h>

#include "static_assert.h"
#include "type_traits.h"
#include "binary.h"
#include "ihash.h"

///\defgroup bsdchecksum BSD Checksum calculation
///\ingroup maths

namespace etl
{
  //***************************************************************************
  /// Calculates the checksum.
  ///\tparam TSum The type used for the sum.
  ///\ingroup checksum
  //***************************************************************************
  template <typename TSum>
  class bsd_checksum
  {
  public:

    STATIC_ASSERT(is_unsigned<TSum>::value, "Signed TSum template parameter not supported");

    typedef TSum value_type;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    bsd_checksum()
    {
      reset();
    }

    //*************************************************************************
    /// Constructor from range.
    /// \param begin Start of the range.
    /// \param end   End of the range.
    //*************************************************************************
    template<typename TIterator>
    bsd_checksum(TIterator begin, const TIterator end)
    {
      STATIC_ASSERT(sizeof(typename std::iterator_traits<TIterator>::value_type) == 1, "Only 8 bit types supported");
      reset();
      
      while (begin != end)
      {
        sum = rotate_right(sum) + *begin++;
      }
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
      STATIC_ASSERT(sizeof(typename std::iterator_traits<TIterator>::value_type) == 1, "Only 8 bit types supported");

      while (begin != end)
      {
        sum = rotate_right(sum) + *begin++;
      }
    }

    //*************************************************************************
    /// \param value The uint8_t to add to the checksum.
    //*************************************************************************
    void add(uint8_t value)
    {
      sum = rotate_right(sum) + value;
    }

    //*************************************************************************
    /// Gets the checksum value.
    //*************************************************************************
    value_type value()
    {
      return sum;
    }

    //*************************************************************************
    /// Conversion operator to value_type.
    //*************************************************************************
    operator value_type ()
    {
      return sum;
    }

  private:

    value_type sum;
  };
}

#endif
