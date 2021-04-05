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

#ifndef ETL_STANDARD_DEVIATION_INCLUDED
#define ETL_STANDARD_DEVIATION_INCLUDED

#include "functional.h"
#include "type_traits.h"

#include <math.h>
#include <stdint.h>

namespace etl
{
  namespace private_standard_deviation
  {
    //***************************************************************************
    /// Types for generic covariance.
    //***************************************************************************
    template <typename TInput, typename TCalc>
    struct standard_deviation_types
    {
      TCalc    sum_of_squares;
      TCalc    sum;
      uint32_t counter;

      //*********************************
      /// Clear the histogram.
      //*********************************
      void clear()
      {
        sum_of_squares = TCalc(0);
        sum            = TCalc(0);
        counter        = 0U;
      }
    };

    //***************************************************************************
    /// Types for float covariance.
    //***************************************************************************
    template <typename TCalc>
    struct standard_deviation_types<float, TCalc>
    {
      float    sum_of_squares;
      float    sum;
      uint32_t counter;

      //*********************************
      /// Clear the histogram.
      //*********************************
      void clear()
      {
        sum_of_squares = float(0);
        sum            = float(0);
        counter        = 0U;
      }
    };

    //***************************************************************************
    /// Types for double covariance.
    //***************************************************************************
    template <typename TCalc>
    struct standard_deviation_types<double, TCalc>
    {
      double   sum_of_squares;
      double   sum;
      uint32_t counter;

      //*********************************
      /// Clear the histogram.
      //*********************************
      void clear()
      {
        sum_of_squares = double(0);
        sum            = double(0);
        counter        = 0U;
      }
    };
  }

  //***************************************************************************
  /// Constructor.
  //***************************************************************************
  template <typename TInput, typename TCalc = TInput>
  class standard_deviation 
    : public private_standard_deviation::standard_deviation_types<TInput, TCalc>
    , public etl::binary_function<TInput, TInput, void>
  {
  public:

    //*********************************
    /// Constructor.
    //*********************************
    standard_deviation()
      : recalulate(true)
    {
      this->clear();
    }

    //*********************************
    /// Constructor.
    //*********************************
    template <typename TIterator>
    standard_deviation(TIterator first, TIterator last)
      : recalulate(true)
    {
      this->clear();
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
      recalulate = true;
    }

    //*********************************
    /// Add a range.
    //*********************************
    template <typename TIterator>
    typename etl::enable_if<!etl::is_same<TIterator, TInput>::value, void>::type
      add(TIterator first, TIterator last)
    {
      while (first != last)
      {
        add(*first++);
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
    typename etl::enable_if<!etl::is_same<TIterator, TInput>::value, void>::type
      operator ()(TIterator first, TIterator last)
    {
      add(first, last);
    }

    //*********************************
    /// Get the standard_deviation.
    //*********************************
    double get_standard_deviation()
    {
      if (recalulate)
      {
        standard_deviation_value = 0.0;

        if (counter != 0)
        {
          double n = double(counter);

          double mean = sum / n;

          double stddev_squared = (sum_of_squares / n) - (mean * mean);

          if (stddev_squared > 0)
          {
            standard_deviation_value = sqrt(stddev_squared);
          }
        }

        recalulate = false;
      }

      return standard_deviation_value;
    }

    //*********************************
    /// Get the standard_deviation.
    //*********************************
    operator double()
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

  private:
  
    double standard_deviation_value;
    bool   recalulate;
  };
}

#endif
