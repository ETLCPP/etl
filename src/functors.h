///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2017 jwellbelove

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

#ifndef __ETL_FUNCTORS__
#define __ETL_FUNCTORS__

///\defgroup functors functors
/// A set of functors for use with for_each
///\ingroup utilities

#include "type_traits.h"
#include "static_assert.h"

namespace etl
{
  //***************************************************************************
  /// Finds the mean
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TSum, typename TCount = size_t>
  class mean
  {
  public:

    mean()
      : sum(TSum()),
        count(0)
    {
    }

    mean(TSum initial_value)
      : sum(initial_value()),
        count(0)
    {
    }

    template <typename TValue>
    void operator()(TValue value)
    {
      sum += value;
      ++count;
    }

    template <typename TResult>
    typename etl::enable_if<etl::is_floating_point<TResult>::value, TResult>::type
     result() const
    {
      return TResult(sum) / TResult(count);
    }

    template <typename TResult>
    typename etl::enable_if<!etl::is_floating_point<TResult>::value, TResult>::type
     result() const
    {
      return TResult(sum / count);
    }

    void reset()
    {
      sum   = TSum();
      count = 0;
    }

  private:

    TSum   sum;
    TCount count;
  };

  //***************************************************************************
  /// Finds the mean on a rolling basis.
  /// Defaults to an initial value of TMean() and a ratio of 0.1
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TMean, typename TRatio = double>
  class rolling_mean
  {
  public:

    STATIC_ASSERT(etl::is_floating_point<TRatio>::value, "TRatio must be a floating point type");

    rolling_mean(TMean  initial_value = TMean(),
                 TRatio ratio_        = TRatio(0.1)
      : mean(initial_value),
        ratio(ratio_)
    {
    }

    template <typename TValue>
    void operator()(TValue value)
    {
      mean -= (mean * ratio);
      mean += (value * ratio);
    }

    void set_ratio(TRatio ratio_)
    {
      ratio = ratio_;
    }

    TMean result() const
    {
      return mean;
    }

    void reset()
    {
      mean = TMean();
    }

  private:

    TMean mean;
  };
}

#endif

