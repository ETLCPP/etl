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

#ifndef __ETL_JENKINS__
#define __ETL_JENKINS__

#include <stdint.h>
#include <iterator>

#include "static_assert.h"
#include "type_traits.h"
#include "error_handler.h"
#include "ihash.h"

#if defined(COMPILER_KEIL)
#pragma diag_suppress 1300 
#endif

///\defgroup jenkins Jenkins 32 & 64 bit hash calculations
///\ingroup maths

namespace etl
{
  //***************************************************************************
  /// Calculates the jenkins hash.
  ///\ingroup jenkins
  //***************************************************************************
  template <typename THash>
  class jenkins
  {
  public:

    STATIC_ASSERT((etl::is_same<THash, uint32_t>::value || etl::is_same<THash, uint64_t>::value), "Only 32 & 64 bit types supported");

    typedef THash value_type;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    jenkins()
    {
      reset();
    }

    //*************************************************************************
    /// Constructor from range.
    /// \param begin Start of the range.
    /// \param end   End of the range.
    //*************************************************************************
    template<typename TIterator>
    jenkins(TIterator begin, const TIterator end)
    {
      STATIC_ASSERT(sizeof(typename std::iterator_traits<TIterator>::value_type) == 1, "Incompatible type");

      reset();

      while (begin != end)
      {
        hash += *begin++;
        hash += (hash << 10);
        hash ^= (hash >> 6);
      }
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
      STATIC_ASSERT(sizeof(typename std::iterator_traits<TIterator>::value_type) == 1, "Incompatible type");

      if (is_finalised)
      {
        ETL_ERROR(hash_finalised());
      }
      else
      {
        while (begin != end)
        {
          hash += *begin++;
          hash += (hash << 10);
          hash ^= (hash >> 6);
        }
      }
    }

    //*************************************************************************
    /// \param value The char to add to the jenkins.
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
    /// Gets the jenkins value.
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
}

#endif
