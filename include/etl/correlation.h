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

#ifndef ETL_CORRELATION_INCLUDED
#define ETL_CORRELATION_INCLUDED

#include "platform.h"
#include "functional.h"
#include "type_traits.h"

#include <math.h>
#include <stdint.h>

namespace etl
{
  namespace private_correlation
  {
    //***************************************************************************
    /// Types for generic correlation.
    //***************************************************************************
    template <typename TInput, typename TCalc>
    struct correlation_traits
    {
      typedef TCalc calc_t;
    };

    //***************************************************************************
    /// Types for float correlation.
    //***************************************************************************
    template <typename TCalc>
    struct correlation_traits<float, TCalc>
    {
      typedef float calc_t;
    };

    //***************************************************************************
    /// Types for double correlation.
    //***************************************************************************
    template <typename TCalc>
    struct correlation_traits<double, TCalc>
    {
      typedef double calc_t;
    };
  }

  //***************************************************************************
  /// Correlation Type.
  //***************************************************************************
  struct correlation_type
  {
    static ETL_CONSTANT bool Sample     = false;
    static ETL_CONSTANT bool Population = true;
  };

  //***************************************************************************
  /// Correlation.
  //***************************************************************************
  template <bool Correlation_Type, typename TInput, typename TCalc = TInput>
  class correlation 
    : public private_correlation::correlation_traits<TInput, TCalc>
    , public etl::binary_function<TInput, TInput, void>
  {
  private:

    static ETL_CONSTANT int Adjustment = (Correlation_Type == correlation_type::Population) ? 0 : 1;

    typedef typename private_correlation::correlation_traits<TInput, TCalc>::calc_t calc_t;

  public:

    //*********************************
    /// Constructor.
    //*********************************
    correlation()
    {
      clear();
    }

    //*********************************
    /// Constructor.
    //*********************************
    template <typename TIterator>
    correlation(TIterator first1, TIterator last1, TIterator first2)
    {
      clear();
      add(first1, last1, first2);
    }

    //*********************************
    /// Add a pair of values.
    //*********************************
    void add(TInput value1, TInput value2)
    {
      inner_product   += TCalc(value1 * value2);
      sum_of_squares1 += TCalc(value1 * value1);
      sum_of_squares2 += TCalc(value2 * value2);
      sum1            += TCalc(value1);
      sum2            += TCalc(value2);
      ++counter;
      recalculate = true;
    }

    //*********************************
    /// Add a range.
    //*********************************
    template <typename TIterator>
    void add(TIterator first1, TIterator last1, TIterator first2)
    {
      while (first1 != last1)
      {
        add(*first1, *first2);
        ++first1;
        ++first2;
      }
    }

    //*********************************
    /// operator ()
    /// Add a pair of values.
    //*********************************
    void operator ()(TInput value1, TInput value2)
    {
      add(value1, value2);
    }

    //*********************************
    /// operator ()
    /// Add a range.
    //*********************************
    template <typename TIterator>
    void operator ()(TIterator first1, TIterator last1, TIterator first2)
    {
      add(first1, last1, first2);
    }

    //*********************************
    /// Get the correlation.
    //*********************************
    double get_covariance() const
    {
      calculate();

      return covariance_value;
    }

    //*********************************
    /// Get the correlation.
    //*********************************
    double get_correlation() const
    {
      calculate();

      return correlation_value;
    }

    //*********************************
    /// Get the correlation.
    //*********************************
    operator double() const
    {
      return get_correlation();
    }

    //*********************************
    /// Get the total number added entries.
    //*********************************
    size_t count() const
    {
      return size_t(counter);
    }

    //*********************************
    /// Clear the correlation.
    //*********************************
    void clear()
    {
      inner_product     = calc_t(0);
      sum_of_squares1   = calc_t(0);
      sum_of_squares2   = calc_t(0);
      sum1              = calc_t(0);
      sum2              = calc_t(0);
      counter           = 0U;
      covariance_value  = 0.0;
      correlation_value = 0.0;
      recalculate       = true;
    }

  private:
  
    //*********************************
    /// Do the calculation.
    //*********************************
    void calculate() const
    {
      if (recalculate)
      {
        correlation_value = 0.0;
        covariance_value  = 0.0;

        if (counter != 0)
        {
          double n = double(counter);
          double adjustment = 1.0 / (n * (n - Adjustment));

          double square_of_sum1 = (sum1 * sum1);
          double square_of_sum2 = (sum2 * sum2);

          double variance1 = ((n * sum_of_squares1) - square_of_sum1) * adjustment;
          double variance2 = ((n * sum_of_squares2) - square_of_sum2) * adjustment;

          double stddev1 = 0.0;
          double stddev2 = 0.0;

          if (variance1 > 0)
          {
            stddev1 = sqrt(variance1);
          }

          if (variance2 > 0)
          {
            stddev2 = sqrt(variance2);
          }

          covariance_value = ((n * inner_product) - (sum1 * sum2)) * adjustment;

          if ((stddev1 > 0.0) && (stddev2 > 0.0))
          {            
            correlation_value = covariance_value / (stddev1 * stddev2);
          }
        }

        recalculate = false;
      }
    }

    calc_t   inner_product;
    calc_t   sum_of_squares1;
    calc_t   sum_of_squares2;
    calc_t   sum1;
    calc_t   sum2;
    uint32_t counter;
    mutable double covariance_value;
    mutable double correlation_value;
    mutable bool   recalculate;
  };
}

#endif
