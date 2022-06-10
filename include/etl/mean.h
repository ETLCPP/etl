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

#ifndef ETL_MEAN_INCLUDED
#define ETL_MEAN_INCLUDED

#include "platform.h"
#include "functional.h"
#include "type_traits.h"

#include <math.h>
#include <stdint.h>

namespace etl
{
  namespace private_mean
  {
    //***************************************************************************
    /// Types for generic mean.
    //***************************************************************************
    template <typename TInput, typename TCalc>
    struct mean_traits
    {
      typedef TCalc calc_t;
    };

    //***************************************************************************
    /// Types for float mean.
    //***************************************************************************
    template <typename TCalc>
    struct mean_traits<float, TCalc>
    {
      typedef float calc_t;
    };

    //***************************************************************************
    /// Types for double mean.
    //***************************************************************************
    template <typename TCalc>
    struct mean_traits<double, TCalc>
    {
      typedef double calc_t;
    };
  }

  //***************************************************************************
  /// Mean.
  //***************************************************************************
  template <typename TInput, typename TCalc = TInput>
  class mean 
    : public private_mean::mean_traits<TInput, TCalc>
    , public etl::binary_function<TInput, TInput, void>
  {
  private:

    typedef typename private_mean::mean_traits<TInput, TCalc>::calc_t calc_t;

  public:

    //*********************************
    /// Constructor.
    //*********************************
    mean()
    {
      clear();
    }

    //*********************************
    /// Constructor.
    //*********************************
    template <typename TIterator>
    mean(TIterator first, TIterator last)
    {
      clear();
      add(first, last);
    }

    //*********************************
    /// Add a pair of values.
    //*********************************
    void add(TInput value)
    {
      sum += TCalc(value);
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
    /// Get the mean.
    //*********************************
    double get_mean() const
    {
      if (recalculate)
      {
        mean_value = 0.0;

        if (counter != 0)
        {
          double n = double(counter);
          mean_value = sum / n;
        }

        recalculate = false;
      }

      return mean_value;
    }

    //*********************************
    /// Get the mean.
    //*********************************
    operator double() const
    {
      return get_mean();
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
      sum         = calc_t(0);
      counter     = 0U;
      mean_value  = 0.0;
      recalculate = true;
    }

  private:
  
    calc_t   sum;
    uint32_t counter;
    mutable double mean_value;
    mutable bool   recalculate;
  };
}

#endif
