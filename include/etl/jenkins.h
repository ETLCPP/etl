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

#ifndef ETL_JENKINS_INCLUDED
#define ETL_JENKINS_INCLUDED

#include "platform.h"
#include "static_assert.h"
#include "type_traits.h"
#include "error_handler.h"
#include "ihash.h"
#include "frame_check_sequence.h"
#include "iterator.h"

#include <stdint.h>

#if defined(ETL_COMPILER_KEIL)
#pragma diag_suppress 1300
#endif

///\defgroup jenkins Jenkins 32 hash calculation
///\ingroup maths

namespace etl
{
  //***************************************************************************
  /// Jenkins policy.
  /// Calculates 32 bit Jenkins hash.
  //***************************************************************************
  struct jenkins_policy
  {
    typedef uint32_t value_type;

    uint32_t initial() const
    {
      is_finalised = false;

      return 0;
    }

    uint32_t add(value_type hash, uint8_t value) const
    {
      ETL_ASSERT(!is_finalised, ETL_ERROR(hash_finalised));

      hash += value;
      hash += (hash << 10U);
      hash ^= (hash >> 6U);

      return hash;
    }

    uint32_t final(value_type hash) const
    {
      hash += (hash << 3U);
      hash ^= (hash >> 11U);
      hash += (hash << 15U);
      is_finalised = true;

      return hash;
    }

    mutable bool is_finalised;
  };

  //*************************************************************************
  /// jenkins
  //*************************************************************************
  class jenkins : public etl::frame_check_sequence<etl::jenkins_policy>
  {
  public:

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    jenkins()
    {
      this->reset();
    }

    //*************************************************************************
    /// Constructor from range.
    /// \param begin Start of the range.
    /// \param end   End of the range.
    //*************************************************************************
    template<typename TIterator>
    jenkins(TIterator begin, const TIterator end)
    {
      this->reset();
      this->add(begin, end);
    }
  };
}

#endif
