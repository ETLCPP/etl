
///\file

/******************************************************************************
The MIT License(MIT)
Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com
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

#ifndef ETL_FRAME_CHECK_SEQUENCE_INCLUDED
#define ETL_FRAME_CHECK_SEQUENCE_INCLUDED

#include <stdint.h>

#include "platform.h"
#include "static_assert.h"
#include "type_traits.h"
#include "binary.h"

#include "stl/iterator.h"

ETL_STATIC_ASSERT(ETL_8BIT_SUPPORT, "This file does not currently support targets with no 8bit type");

///\defgroup frame_check_sequence Frame check sequence calculation
///\ingroup maths

namespace etl
{
  //***************************************************************************
  /// Calculates a frame check sequence according to the specified policy.
  ///\tparam TPolicy The type used to enact the policy.
  ///\ingroup frame_check_sequence
  //***************************************************************************
  template <typename TPolicy>
  class frame_check_sequence
  {
  public:

    typedef TPolicy policy_type;
    typedef typename policy_type::value_type value_type;

    ETL_STATIC_ASSERT(etl::is_unsigned<value_type>::value, "Signed frame check type not supported");

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    frame_check_sequence()
    {
      reset();
    }

    //*************************************************************************
    /// Constructor from range.
    /// \param begin Start of the range.
    /// \param end   End of the range.
    //*************************************************************************
    template<typename TIterator>
    frame_check_sequence(TIterator begin, const TIterator end)
    {
      ETL_STATIC_ASSERT(sizeof(typename std::iterator_traits<TIterator>::value_type) == 1, "Type not supported");

      reset();
      add(begin, end);
    }

    //*************************************************************************
    /// Resets the FCS to the initial state.
    //*************************************************************************
    void reset()
    {
      frame_check = policy.initial();
    }

    //*************************************************************************
    /// Adds a range.
    /// \param begin
    /// \param end
    //*************************************************************************
    template<typename TIterator>
    void add(TIterator begin, const TIterator end)
    {
      ETL_STATIC_ASSERT(sizeof(typename std::iterator_traits<TIterator>::value_type) == 1, "Type not supported");

      while (begin != end)
      {
        frame_check = policy.add(frame_check, *begin++);
      }
    }

    //*************************************************************************
    /// \param value The uint8_t to add to the FCS.
    //*************************************************************************
    void add(uint8_t value_)
    {
      frame_check = policy.add(frame_check, value_);
    }

    //*************************************************************************
    /// Gets the FCS value.
    //*************************************************************************
    value_type value()
    {
      return policy.final(frame_check);
    }

    //*************************************************************************
    /// Conversion operator to value_type.
    //*************************************************************************
    operator value_type ()
    {
      return policy.final(frame_check);
    }

  private:

    value_type  frame_check;
    policy_type policy;
  };
}

#endif
