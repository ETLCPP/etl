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

#ifndef ETL_RMS_INCLUDED
#define ETL_RMS_INCLUDED

#include "platform.h"
#include "functional.h"
#include "type_traits.h"

#include <math.h>
#include <stdint.h>

namespace etl
{
  namespace private_rms
  {
    //***************************************************************************
    /// Types for generic correlation.
    //***************************************************************************
    template <typename TInput, typename TCalc>
    struct rms_traits
    {
      typedef TCalc calc_t;
    };

    //***************************************************************************
    /// Types for float correlation.
    //***************************************************************************
    template <typename TCalc>
    struct rms_traits<float, TCalc>
    {
      typedef float calc_t;
    };

    //***************************************************************************
    /// Types for double correlation.
    //***************************************************************************
    template <typename TCalc>
    struct rms_traits<double, TCalc>
    {
      typedef double calc_t;
    };
  }

  //***************************************************************************
  /// Standard Deviation.
  //***************************************************************************
  template <typename TInput, typename TCalc = TInput>
  class rms 
    : public private_rms::rms_traits<TInput, TCalc>
    , public etl::binary_function<TInput, TInput, void>
  {
  private:

    typedef typename private_rms::rms_traits<TInput, TCalc>::calc_t calc_t;

  public:

    //*********************************
    /// Constructor.
    //*********************************
    rms()
      : recalculate(true)
    {
      clear();
    }

    //*********************************
    /// Constructor.
    //*********************************
    template <typename TIterator>
    rms(TIterator first, TIterator last)
      : recalculate(true)
    {
      clear();
      add(first, last);
    }

    //*********************************
    /// Add a pair of values.
    //*********************************
    void add(TInput value)
    {
      sum_of_squares += TCalc(value * value);
      ++counter;
      recalculate = true;
    }

    //*********************************
    /// Add a range.
    //*********************************
    template <typename TIterator>
    void add(TIterator first, TIterator last)
    {
      while (first != last)
      {
        add(*first);
        ++first;
      }
    }

    //*********************************
    /// operator ()
    /// Add a pair of values.
    //*********************************
    void operator ()(TInput value)
    {
      add(value);
    }

    //*********************************
    /// operator ()
    /// Add a range.
    //*********************************
    template <typename TIterator>
    void operator ()(TIterator first, TIterator last)
    {
      add(first, last);
    }

    //*********************************
    /// Get the rms.
    //*********************************
    double get_rms() const
    {
      if (recalculate)
      {
        rms_value = 0.0;

        if (counter != 0)
        {
          double n = double(counter);
          double mean_of_squares = sum_of_squares / n;

          if (mean_of_squares > 0)
          {
            rms_value = sqrt(mean_of_squares);
          }
        }

        recalculate = false;
      }

      return rms_value;
    }

    //*********************************
    /// Get the rms.
    //*********************************
    operator double() const
    {
      return get_rms();
    }

    //*********************************
    /// Get the total number added entries.
    //*********************************
    size_t count() const
    {
      return size_t(counter);
    }

    //*********************************
    /// Clear the histogram.
    //*********************************
    void clear()
    {
      sum_of_squares = calc_t(0);
      counter        = 0U;
      rms_value      = 0.0;
      recalculate    = true;
    }

  private:
  
    calc_t   sum_of_squares;
    uint32_t counter;
    mutable double rms_value;
    mutable bool   recalculate;
  };
}

#endif
