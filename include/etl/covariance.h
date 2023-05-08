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
    struct covariance_traits
    {
      typedef TCalc calc_t;
    };

    //***************************************************************************
    /// Types for float covariance.
    //***************************************************************************
    template <typename TCalc>
    struct covariance_traits<float, TCalc>
    {
      typedef float calc_t;
    };

    //***************************************************************************
    /// Types for double covariance.
    //***************************************************************************
    template <typename TCalc>
    struct covariance_traits<double, TCalc>
    {
      typedef double calc_t;
    };
  }

  //***************************************************************************
  /// Covariance Type.
  //***************************************************************************
  namespace private_covariance
  {
    template<typename T = void>
    struct covariance_type_statics
    {
      static ETL_CONSTANT bool Sample = false;
      static ETL_CONSTANT bool Population = true;
    };

    template<typename T>
    ETL_CONSTANT bool covariance_type_statics<T>::Sample;
    
    template<typename T>
    ETL_CONSTANT bool covariance_type_statics<T>::Population;
  }

  struct covariance_type : public private_covariance::covariance_type_statics<>
  {
  };

  //***************************************************************************
  /// Covariance.
  //***************************************************************************
  template <bool Covariance_Type, typename TInput, typename TCalc = TInput>
  class covariance 
    : public private_covariance::covariance_traits<TInput, TCalc>
    , public etl::binary_function<TInput, TInput, void>
  {
  private:

    static ETL_CONSTANT int Adjustment = (Covariance_Type == covariance_type::Population) ? 0 : 1;

    typedef typename private_covariance::covariance_traits<TInput, TCalc>::calc_t calc_t;

  public:

    //*********************************
    /// Constructor.
    //*********************************
    covariance()
    {
      clear();
    }

    //*********************************
    /// Constructor.
    //*********************************
    template <typename TIterator>
    covariance(TIterator first1, TIterator last1, TIterator first2)
    {
      clear();
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
    /// Get the covariance.
    //*********************************
    double get_covariance() const
    {
      if (recalculate)
      {
        covariance_value = 0.0;

        if (counter != 0)
        {
          double n = double(counter);
          double adjustment = 1.0 / (n * (n - Adjustment));

          covariance_value = ((n * inner_product) - (sum1 * sum2)) * adjustment;

          recalculate = false;
        }
      }

      return covariance_value;
    }

    //*********************************
    /// Get the covariance.
    //*********************************
    operator double() const
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

    //*********************************
    /// Clear the covariance.
    //*********************************
    void clear()
    {
      inner_product    = calc_t(0);
      sum1             = calc_t(0);
      sum2             = calc_t(0);
      counter          = 0U;
      covariance_value = 0.0;
      recalculate      = true;
    }

  private:
  
    calc_t   inner_product;
    calc_t   sum1;
    calc_t   sum2;
    uint32_t counter;
    mutable double covariance_value;
    mutable bool   recalculate;
  };

  template <bool Covariance_Type, typename TInput, typename TCalc>
  ETL_CONSTANT int covariance<Covariance_Type, TInput, TCalc>::Adjustment;
}

#endif
