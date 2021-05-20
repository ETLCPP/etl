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

#ifndef ETL_CENTER_OF_GRAVITY_2D_INCLUDED
#define ETL_CENTER_OF_GRAVITY_2D_INCLUDED

#include "platform.h"
#include "functional.h"
#include "type_traits.h"
#include "scaled_rounding.h"
#include "utility.h"

namespace etl
{
  //***************************************************************************
  /// 
  //***************************************************************************
  template <typename TSum, typename TProduct>
  class center_of_gravity_2d
  {
  public:

    typedef TSum     sum_type;
    typedef TProduct product_type;

    //*************************************************************************
    /// Constructor
    //*************************************************************************
    center_of_gravity_2d()
      : x_product(TProduct(0))
      , y_product(TProduct(0))
      , sum(TSum(0))
    {

    }

    //*************************************************************************
    /// Add a new coordinate to the calculation.
    //*************************************************************************
    template <typename TAxis, typename TValue>
    void add(TAxis x, TAxis y, TValue value = TValue(1))
    {
      x_product += (x * value);
      y_product += (y * value);
      sum       += value;
    }

    //*************************************************************************
    /// Add a new coordinate to the calculation.
    //*************************************************************************
    template <typename TAxis, typename TValue>
    void add(etl::coordinate_2d<TAxis> coord, TValue value = TValue(1))
    {
      x_product += (coord.x * value);
      y_product += (coord.y * value);
      sum       += value;
    }

    //*************************************************************************
    /// Add a new coordinate to the calculation.
    //*************************************************************************
    template <typename TAxis, typename TValue>
    void add(etl::coordinate_2d<TAxis, TValue> coord)
    {
      x_product += (coord.x * coord.value);
      y_product += (coord.y * coord.value);
      sum       += coord.value;
    }

    //*************************************************************************
    /// Added a new coordinate to the calculation.
    //*************************************************************************
    template <typename TAxis, typename TValue>
    void operator()(TAxis x, TAxis y, TValue value = TValue(1))
    {
      add(x, y, value);
    }

    //*************************************************************************
    /// Added a new coordinate to the calculation.
    //*************************************************************************
    template <typename TAxis, typename TValue>
    void operator()(etl::coordinate_2d<TAxis, void> coord, TValue value = TValue(1))
    {
      add(coord, value);
    }

    //*************************************************************************
    /// Added a new coordinate to the calculation.
    //*************************************************************************
    template <typename TAxis, typename TValue>
    void operator()(etl::coordinate_2d<TAxis, TValue> coord)
    {
      add(coord);
    }

    //*************************************************************************
    /// Get the x coordinate of the center of gravity.
    //*************************************************************************
    template <typename T>
    typename etl::enable_if<etl::is_floating_point<T>::value, T>::type
    get_x() const
    {
      if (sum == T(0))
      {
        return T(0);
      }
      else
      {
        return T(x_product) / T(sum);
      }
    }

    template <typename T>
    typename etl::enable_if<!etl::is_floating_point<T>::value, T>::type
    get_x() const
    {
      if (sum == T(0))
      {
        return T(0);
      }
      else
      {
        return static_cast<T>(x_product / sum);
      }
    }

    template <typename T, typename etl::scaled_rounding_t<T>::type Scale>
    typename etl::enable_if<!etl::is_floating_point<T>::value, T>::type
    get_x(T(*scaled_rounding)(T)) const
    {
      if (sum == T(0))
      {
        return T(0);
      }
      else
      {
        return static_cast<T>(scaled_rounding((Scale * x_product) / sum) / Scale);
      }
    }

    //*************************************************************************
    /// Get the y coordinate of the center of gravity.
    //*************************************************************************
    template <typename T>
    typename etl::enable_if<etl::is_floating_point<T>::value, T>::type
    get_y() const
    {
      if (sum == T(0))
      {
        return T(0);
      }
      else
      {
        return T(y_product) / T(sum);
      }
    }

    template <typename T>
    typename etl::enable_if<!etl::is_floating_point<T>::value, T>::type
    get_y() const
    {
      if (sum == T(0))
      {
        return T(0);
      }
      else
      {
        return static_cast<T>(y_product / sum);
      }
    }

    template <typename T, typename etl::scaled_rounding_t<T>::type Scale>
    typename etl::enable_if<!etl::is_floating_point<T>::value, T>::type
    get_y(T(*scaled_rounding)(T)) const
    {
      if (sum == T(0))
      {
        return T(0);
      }
      else
      {
        return static_cast<T>(scaled_rounding((Scale * y_product) / sum) / Scale);
      }
    }
         
    //*************************************************************************
    /// Get the coordinate of the center of gravity.
    //*************************************************************************
    template <typename TCoordinate, typename TAxis = typename TCoordinate::axis_type>
    TCoordinate get() const
    {
      return TCoordinate(get_x<TAxis>(), get_y<TAxis>());
    }

    template <typename TCoordinate, typename TAxis = typename TCoordinate::axis_type, typename etl::scaled_rounding_t<TAxis>::type Scale>
    TCoordinate get(TAxis(*scaled_rounding)(TAxis)) const
    {
      return TCoordinate(get_x<TAxis, Scale>(scaled_rounding), get_y<TAxis, Scale>(scaled_rounding));
    }

    //*************************************************************************
    /// Clear the center of gravity calculation.
    //*************************************************************************
    void clear()
    {
      x_product = TProduct(0);
      y_product = TProduct(0);
      sum       = TSum(0);
    }

  private:

    TProduct x_product;
    TProduct y_product;
    TSum     sum;
  };
}

#endif
