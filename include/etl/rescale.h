///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 jwellbelove

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

#ifndef ETL_RESCALE_INCLUDED
#define ETL_RESCALE_INCLUDED

#include "platform.h"
#include "functional.h"
#include "type_traits.h"
#include "algorithm.h"

#include <math.h>
#include <stdint.h>

namespace etl
{
  //***************************************************************************
  /// Rescale function.
  //***************************************************************************
  template<typename TInput, typename TOutput>
  class rescale : public etl::unary_function<TOutput, TInput>
  {
  public:

    //*****************************************************************
    /// Constructor.
    //*****************************************************************
    rescale(TInput  input_min_value_,
            TInput  input_max_value_,
            TOutput output_min_value_,
            TOutput output_max_value_)
        : input_min_value(input_min_value_)
        , output_min_value(output_min_value_)
        , output_max_value(output_max_value_)
        , multiplier(double(output_max_value_ - output_min_value_) / double(input_max_value_ - input_min_value_))
    {    
    }

    //*****************************************************************
    /// operator ()
    //*****************************************************************
    TOutput operator ()(TInput value) const
    {
      return TOutput(((value - input_min_value) * multiplier)) + output_min_value;;
    }

  private:

    const TInput  input_min_value;
    const TOutput output_min_value;
    const TOutput output_max_value;
    double multiplier;    
  };
}

#endif
