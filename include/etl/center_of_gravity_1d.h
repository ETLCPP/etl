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

#ifndef ETL_CENTER_OF_GRAVITY_1D_INCLUDED
#define ETL_CENTER_OF_GRAVITY_1D_INCLUDED

#include "platform.h"
#include "functional.h"
#include "type_traits.h"
#include "scaled_rounding.h"

namespace etl
{
  //***************************************************************************
  /// 
  //***************************************************************************
  template <typename TSum, typename TProduct>
  class center_of_gravity_1d
  {
  public:

    typedef TSum     sum_type;
    typedef TProduct product_type;

    //*************************************************************************
    /// Constructor
    //*************************************************************************
    center_of_gravity_1d()
      : product(TProduct(0))
      , sum(TSum(0))
    {

    }

    //*************************************************************************
    /// Add a new coordinate to the calculation.
    //*************************************************************************
    template <typename TAxis, typename TValue>
    void add(TAxis x, TValue value = TValue(1))
    {
      product += (x * value);
      sum       += value;
    }

    //*************************************************************************
    /// Added a new coordinate to the calculation.
    //*************************************************************************
    template <typename TAxis, typename TValue>
    void operator()(TAxis x, TValue value = TValue(1))
    {
      add(x, value);
    }

    //*************************************************************************
    /// Get the x coordinate of the center of gravity.
    /// Floating point.
    //*************************************************************************
    template <typename T>
    typename etl::enable_if<etl::is_floating_point<T>::value, T>::type
    get() const
    {
      if (sum == T(0))
      {
        return T(0);
      }
      else
      {
        return T(product) / T(sum);
      }
    }

    //*************************************************************************
    /// Get the x coordinate of the center of gravity.
    /// Not floating point.
    //*************************************************************************
    template <typename T>
    typename etl::enable_if<!etl::is_floating_point<T>::value, T>::type
    get() const
    {
      if (sum == T(0))
      {
        return T(0);
      }
      else
      {
        return static_cast<T>(product / sum);
      }
    }

    //*************************************************************************
    /// Get the x coordinate of the center of gravity.
    /// Not floating point, rounding.
    //*************************************************************************
    template <typename T, typename etl::scaled_rounding_t<T>::type Scale>
    typename etl::enable_if<!etl::is_floating_point<T>::value, T>::type
    get(T(*scaled_rounding)(T)) const
    {
      if (sum == T(0))
      {
        return T(0);
      }
      else
      {
        return static_cast<T>(scaled_rounding((Scale * product) / sum) / Scale);
      }
    }

    //*************************************************************************
    /// Clear the center of gravity calculation.
    //*************************************************************************
    void clear()
    {
      product = TProduct(0);
      sum       = TSum(0);
    }

  private:

    TProduct product;
    TSum     sum;
  };
}

#endif
