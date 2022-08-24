///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 John Wellbelove

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

#ifndef ETL_QUANTIZE_INCLUDED
#define ETL_QUANTIZE_INCLUDED

#include "platform.h"
#include "functional.h"
#include "type_traits.h"

#include <math.h>
#include <stdint.h>

namespace etl
{
  //***************************************************************************
  /// Quantize .
  //***************************************************************************
  template<typename TInput, typename TCompare = etl::less<TInput> >
  class quantize  : public etl::unary_function<TInput, TInput>
  {
  public:

    //*****************************************************************
    // Constructor.
    //*****************************************************************
    quantize (const TInput* p_thresholds_, const TInput* p_quantizations_, size_t n_quantizations_, TCompare compare_ = TCompare())
      : p_thresholds(p_thresholds_)
      , p_quantizations(p_quantizations_)
      , n_levels(n_quantizations_ - 1U)
      , compare(compare_)
    {
    }

    //*****************************************************************
    // operator ()
    //*****************************************************************
    TInput operator ()(TInput value) const
    {
      for (size_t i = 0UL; i < n_levels; ++i)
      {
        if (compare(value, p_thresholds[i]))
        {
          return p_quantizations[i];
        }
      }

      return p_quantizations[n_levels];
    }

  private:

    const TInput* const p_thresholds;
    const TInput* const p_quantizations;
    const size_t   n_levels;
    const TCompare compare;
  };
}

#endif
