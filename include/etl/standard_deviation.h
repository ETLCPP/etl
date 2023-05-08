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

#ifndef ETL_STANDARD_DEVIATION_INCLUDED
#define ETL_STANDARD_DEVIATION_INCLUDED

#include "platform.h"
#include "functional.h"
#include "type_traits.h"

#include <math.h>
#include <stdint.h>

namespace etl
{
  namespace private_standard_deviation
  {
    //***************************************************************************
    /// Types for generic standard_deviation.
    //***************************************************************************
    template <typename TInput, typename TCalc>
    struct standard_deviation_traits
    {
      typedef TCalc calc_t;
    };

    //***************************************************************************
    /// Types for float standard_deviation.
    //***************************************************************************
    template <typename TCalc>
    struct standard_deviation_traits<float, TCalc>
    {
      typedef float calc_t;
    };

    //***************************************************************************
    /// Types for double standard_deviation.
    //***************************************************************************
    template <typename TCalc>
    struct standard_deviation_traits<double, TCalc>
    {
      typedef double calc_t;
    };
  }

  //***************************************************************************
  /// Standard Deviation Type.
  //***************************************************************************
  namespace private_standard_deviation
  {
    template<typename T = void>
    struct standard_deviation_type_helper
    {
      static ETL_CONSTANT bool Sample = false;
      static ETL_CONSTANT bool Population = true;
    };

    template <typename T>
    ETL_CONSTANT bool standard_deviation_type_helper<T>::Sample;

    template <typename T>
    ETL_CONSTANT bool standard_deviation_type_helper<T>::Population;
  }

  struct standard_deviation_type : public private_standard_deviation::standard_deviation_type_helper<>
  {
  };

  //***************************************************************************
  /// Standard Deviation.
  //***************************************************************************
  template <bool Standard_Deviation_Type, typename TInput, typename TCalc = TInput>
  class standard_deviation 
    : public private_standard_deviation::standard_deviation_traits<TInput, TCalc>
    , public etl::binary_function<TInput, TInput, void>
  {
  private:

    static ETL_CONSTANT int Adjustment = (Standard_Deviation_Type == standard_deviation_type::Population) ? 0 : 1;

    typedef typename private_standard_deviation::standard_deviation_traits<TInput, TCalc>::calc_t calc_t;

  public:

    //*********************************
    /// Constructor.
    //*********************************
    standard_deviation()
    {
      clear();
    }

    //*********************************
    /// Constructor.
    //*********************************
    template <typename TIterator>
    standard_deviation(TIterator first, TIterator last)
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
      sum            += TCalc(value);
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
    /// Get the variance.
    //*********************************
    double get_variance() const
    {
      calculate();

      return variance_value;
    }

    //*********************************
    /// Get the standard_deviation.
    //*********************************
    double get_standard_deviation() const
    {
      calculate();

      return standard_deviation_value;
    }

    //*********************************
    /// Get the standard_deviation.
    //*********************************
    operator double() const
    {
      return get_standard_deviation();
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
      sum_of_squares           = calc_t(0);
      sum                      = calc_t(0);
      counter                  = 0U;
      variance_value           = 0.0;
      standard_deviation_value = 0.0;
      recalculate              = true;
    }

  private:
  
    //*********************************
    /// Do the calculation.
    //*********************************
    void calculate() const
    {
      if (recalculate)
      {
        standard_deviation_value = 0.0;
        variance_value = 0.0;

        if (counter != 0)
        {
          double n = double(counter);
          double adjustment = 1.0 / (n * (n - Adjustment));

          double square_of_sum = (sum * sum);

          variance_value = ((n * sum_of_squares) - square_of_sum) * adjustment;

          if (variance_value > 0)
          {
            standard_deviation_value = sqrt(variance_value);
          }
        }

        recalculate = false;
      }
    }

    calc_t   sum_of_squares;
    calc_t   sum;
    uint32_t counter;
    mutable double variance_value;
    mutable double standard_deviation_value;
    mutable bool   recalculate;
  };
}

#endif
