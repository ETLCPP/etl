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

#ifndef ETL_COVARIANCE_INCLUDED
#define ETL_COVARIANCE_INCLUDED

#include "platform.h"
#include "functional.h"
#include "type_traits.h"

#include <math.h>
#include <stdint.h>

namespace etl
{
  namespace private_covariance
  {
    //***************************************************************************
    /// Types for generic covariance.
    //***************************************************************************
    template <typename TInput, typename TCalc>
    struct covariance_types
    {
      TCalc    inner_product;
      TCalc    sum1;
      TCalc    sum2;
      uint32_t counter;

      //*********************************
      /// Clear the histogram.
      //*********************************
      void clear()
      {
        inner_product = TCalc(0);
        sum1          = TCalc(0);
        sum2          = TCalc(0);
        counter       = 0U;
      }
    };

    //***************************************************************************
    /// Types for float covariance.
    //***************************************************************************
    template <typename TCalc>
    struct covariance_types<float, TCalc>
    {
      float    inner_product;
      float    sum1;
      float    sum2;
      uint32_t counter;

      //*********************************
      /// Clear the histogram.
      //*********************************
      void clear()
      {
        inner_product = float(0);
        sum1          = float(0);
        sum2          = float(0);
        counter       = 0U;
      }
    };

    //***************************************************************************
    /// Types for double covariance.
    //***************************************************************************
    template <typename TCalc>
    struct covariance_types<double, TCalc>
    {
      double   inner_product;
      double   sum1;
      double   sum2;
      uint32_t counter;

      //*********************************
      /// Clear the histogram.
      //*********************************
      void clear()
      {
        inner_product = double(0);
        sum1          = double(0);
        sum2          = double(0);
        counter       = 0U;
      }
    };
  }

  //***************************************************************************
  /// Covariance Type.
  //***************************************************************************
  struct covariance_type
  {
    static ETL_CONSTANT bool Sample     = false;
    static ETL_CONSTANT bool Population = true;
  };

  //***************************************************************************
  /// Constructor.
  //***************************************************************************
  template <bool Covariance_Type, typename TInput, typename TCalc = TInput>
  class covariance 
    : public private_covariance::covariance_types<TInput, TCalc>
    , public etl::binary_function<TInput, TInput, void>
  {
  private:

    static ETL_CONSTANT int Adjustment = (Covariance_Type == covariance_type::Population) ? 0 : 1;

  public:

    //*********************************
    /// Constructor.
    //*********************************
    covariance()
      : recalulate(true)
    {
      this->clear();
    }

    //*********************************
    /// Constructor.
    //*********************************
    template <typename TIterator>
    covariance(TIterator first1, TIterator last1, TIterator first2)
      : recalulate(true)
    {
      this->clear();
      add(first1, last1, first2);
    }

    //*********************************
    /// Add a pair of values.
    //*********************************
    void add(TInput value1, TInput value2)
    {
      inner_product += TCalc(value1 * value2);
      sum1          += TCalc(value1);
      sum2          += TCalc(value2);
      ++counter;
      recalulate = true;
    }

    //*********************************
    /// Add a range.
    //*********************************
    template <typename TIterator>
    typename etl::enable_if<!etl::is_same<TIterator, TInput>::value, void>::type
      add(TIterator first1, TIterator last1, TIterator first2)
    {
      while (first1 != last1)
      {
        add(*first1++, *first2++);
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
    typename etl::enable_if<!etl::is_same<TIterator, TInput>::value, void>::type
      operator ()(TIterator first1, TIterator last1, TIterator first2)
    {
      add(first1, last1, first2);
    }

    //*********************************
    /// Get the covaniance.
    //*********************************
    double get_covariance()
    {
      if (recalulate)
      {
        covariance_value = 0.0;

        if (counter != 0)
        {
          double n = double(counter);
          double adjustment = 1.0 / (n - Adjustment);

          double mean1 = sum1 / n;
          double mean2 = sum2 / n;

          covariance_value = (inner_product - (n * mean1 * mean2)) * adjustment;

          recalulate = false;
        }
      }

      return covariance_value;
    }

    //*********************************
    /// Get the covariance.
    //*********************************
    operator double()
    {
      return get_covariance();
    }

    //*********************************
    /// Get the total number added entries.
    //*********************************
    size_t count() const
    {
      return size_t(counter);
    }

  private:
  
    double covariance_value;
    bool   recalulate;
  };
}

#endif
