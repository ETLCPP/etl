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
#include "endian.h"
#include "ihash.h"

#include "error_handler.h"

#if defined(COMPILER_KEIL)
#pragma diag_suppress 1300 
#endif

///\defgroup jenkins Jenkins 32 & 64 bit hash calculations
///\ingroup maths

namespace etl
{
  //***************************************************************************
  /// Calculates the jenkins_64 hash.
  ///\tparam ENDIANNESS The endianness of the calculation for input types larger than uint8_t. Default = endian::little.
  ///\ingroup jenkins_64
  //***************************************************************************
  template <const int ENDIANNESS = endian::little>
  class jenkins_64 : public etl::ihash
  {
  public:

    typedef uint64_t value_type;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    jenkins_64()
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
    jenkins_64(TIterator begin, const TIterator end)
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
      hash = 0;
      is_finalised = false;
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
    /// \param value The char to add to the jenkins_64.
    //*************************************************************************
    void add(uint8_t value)
    {
      if (is_finalised)
      {
        ETL_ERROR(hash_finalised());
      }
      else
      {
        hash += value;
        hash += (hash << 10);
        hash ^= (hash >> 6);
      }
    }

    //*************************************************************************
    /// Gets the jenkins_64 value.
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

    //*************************************************************************
    /// Gets the generic digest value.
    //*************************************************************************
    generic_digest digest()
    {
      finalise();
      return ihash::get_digest(hash);
    }

  private:

    void finalise()
    {
      if (!is_finalised)
      {
        hash += (hash << 3);
        hash ^= (hash >> 11);
        hash += (hash << 15);
        is_finalised = true;
      }
    }

    value_type hash;
    bool is_finalised;
  };

  //***************************************************************************
  /// Calculates the jenkins_32 hash.
  ///\tparam ENDIANNESS The endianness of the calculation for input types larger than uint8_t. Default = endian::little.
  ///\ingroup jenkins_32
  //***************************************************************************
  template <const int ENDIANNESS = endian::little>
  class jenkins_32 : public etl::ihash
  {
  public:

    typedef uint32_t value_type;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    jenkins_32()
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
    jenkins_32(TIterator begin, const TIterator end)
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
      hash = 0;
      is_finalised = false;
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
    /// \param value The char to add to the jenkins_32.
    //*************************************************************************
    void add(uint8_t value)
    {
      if (is_finalised)
      {
        ETL_ERROR(hash_finalised());
      }
      else
      {
        hash += value;
        hash += (hash << 10);
        hash ^= (hash >> 6);
      }
    }

    //*************************************************************************
    /// Gets the jenkins_32 value.
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

    //*************************************************************************
    /// Gets the generic digest value.
    //*************************************************************************
    generic_digest digest()
    {
      finalise();
      return ihash::get_digest(hash);
    }

  private:

    //*************************************************************************
    /// Finalise the hash.
    //*************************************************************************
    void finalise()
    {
      if (!is_finalised)
      {
        hash += (hash << 3);
        hash ^= (hash >> 11);
        hash += (hash << 15);
        is_finalised = true;
      }
    }

    bool is_finalised;
    value_type hash;
  };
}

#endif
