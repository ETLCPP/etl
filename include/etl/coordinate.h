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

#ifndef ETL_COORDINATE_INCLUDED
#define ETL_COORDINATE_INCLUDED

#include "platform.h"

///\defgroup coordinate coordinate
///\ingroup coordinate

namespace etl
{
  //******************************************************************************
  /// 2D coordinate type with value.
  template <typename TAxis, typename TValue = void>
  struct coordinate_2d
  {
    typedef TValue value_type;
    typedef TAxis  axis_type;

    //***********************************
    coordinate_2d()
      : x(TAxis(0))
      , y(TAxis(0))
      , value(TValue(0))
    {
    }

    //***********************************
    coordinate_2d(TAxis x_, TAxis y_)
      : x(x_)
      , y(y_)
      , value(TValue(0))
    {
    }

    //***********************************
    coordinate_2d(TAxis x_, TAxis y_, TValue value_)
      : x(x_)
      , y(y_)
      , value(value_)
    {
    }

    //***********************************
    coordinate_2d& operator =(etl::coordinate_2d<TAxis, void> other)
    {
      x     = other.x;
      y     = other.y;
      value = TValue(0);

      return *this;
    }

    //***********************************
    coordinate_2d& operator +=(const coordinate_2d& rhs)
    {
      x += rhs.x;
      y += rhs.y;

      return *this;
    }

    //***********************************
    coordinate_2d& operator -=(const coordinate_2d& rhs)
    {
      x -= rhs.x;
      y -= rhs.y;

      return *this;
    }

    TAxis  x;
    TAxis  y;
    TValue value;
  };

  //******************************************************************************
  /// 2D coordinate type.
  template <typename TAxis>
  struct coordinate_2d<TAxis, void>
  {
    typedef void  value_type;
    typedef TAxis axis_type;

    //***********************************
    coordinate_2d()
      : x(TAxis(0))
      , y(TAxis(0))
    {
    }

    //***********************************
    coordinate_2d(TAxis x_, TAxis y_)
      : x(x_)
      , y(y_)
    {
    }

    //***********************************
    coordinate_2d(const coordinate_2d& other)
      : x(other.x)
      , y(other.y)
    {
    }

    //***********************************
    coordinate_2d& operator =(const coordinate_2d& other)
    {
      x = other.x;
      y = other.y;

      return *this;
    }

    //***********************************
    coordinate_2d& operator +=(const coordinate_2d& rhs)
    {
      x += rhs.x;
      y += rhs.y;

      return *this;
    }

    //***********************************
    coordinate_2d& operator -=(const coordinate_2d& rhs)
    {
      x -= rhs.x;
      y -= rhs.y;

      return *this;
    }

    TAxis x;
    TAxis y;
  };

  //******************************************************************************
  /// 3D coordinate type with value.
  template <typename TAxis, typename TValue = void>
  struct coordinate_3d
  {
    typedef TValue value_type;
    typedef TAxis  axis_type;

    //***********************************
    coordinate_3d()
      : x(TAxis(0))
      , y(TAxis(0))
      , z(TAxis(0))     
      , value(TValue(0))
    {
    }

    //***********************************
    coordinate_3d(TAxis x_, TAxis y_, TAxis z_, TValue value_)
      : x(x_)
      , y(y_)
      , z(z_)
      , value(value_)
    {
    }

    //***********************************
    coordinate_3d(TAxis x_, TAxis y_, TAxis z_)
      : x(x_)
      , y(y_)
      , z(z_)
      , value(TValue(0))
    {
    }

    //***********************************
    coordinate_3d(const coordinate_3d& other)
      : x(other.x)
      , y(other.y)
      , z(other.z)
      , value(other.value)
    {
    }

    //***********************************
    coordinate_3d& operator =(const coordinate_3d& other)
    {
      x     = other.x;
      y     = other.y;
      z     = other.z;
      value = other.value;

      return *this;
    }

    //***********************************
    coordinate_3d& operator +=(const coordinate_3d& rhs)
    {
      x += rhs.x;
      y += rhs.y;
      z += rhs.z;

      return *this;
    }

    //***********************************
    coordinate_3d& operator -=(const coordinate_3d& rhs)
    {
      x -= rhs.x;
      y -= rhs.y;
      z -= rhs.z;

      return *this;
    }

    TAxis x;
    TAxis y;
    TAxis z;
    TValue value;
  };

  //******************************************************************************
  /// 3D coordinate type.
  template <typename TAxis>
  struct coordinate_3d<TAxis, void>
  {
    typedef void  value_type;
    typedef TAxis axis_type;

    //***********************************
    coordinate_3d()
      : x(TAxis(0))
      , y(TAxis(0))
      , z(TAxis(0))
    {
    }

    //***********************************
    coordinate_3d(TAxis x_, TAxis y_, TAxis z_)
      : x(x_)
      , y(y_)
      , z(z_)
    {
    }

    //***********************************
    coordinate_3d(const coordinate_3d& other)
      : x(other.x)
      , y(other.y)
      , z(other.z)
    {
    }

    //***********************************
    template <typename TValue>
    coordinate_3d& operator =(const coordinate_3d& other)
    {
      x = other.x;
      y = other.y;
      z = other.z;

      return *this;
    }

    //***********************************
    coordinate_3d& operator +=(const coordinate_3d& rhs)
    {
      x += rhs.x;
      y += rhs.y;
      z += rhs.z;

      return *this;
    }

    //***********************************
    coordinate_3d& operator -=(const coordinate_3d& rhs)
    {
      x -= rhs.x;
      y -= rhs.y;
      z -= rhs.z;

      return *this;
    }

    TAxis x;
    TAxis y;
    TAxis z;
  };

  //***************************************************************************
  /// Equality operator for 2D coordinate.
  //***************************************************************************
  template <typename TAxis, typename TValue>
  bool operator ==(const coordinate_2d<TAxis, TValue>& lhs, const coordinate_2d<TAxis, TValue>& rhs)
  {
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
  }

  //***************************************************************************
  /// Inequality operator for 2D coordinate.
  //***************************************************************************
  template <typename TAxis, typename TValue>
  bool operator !=(const coordinate_2d<TAxis, TValue>& lhs, const coordinate_2d<TAxis, TValue>& rhs)
  {
    return !(lhs == rhs);
  }

  //***************************************************************************
  /// Addition operator for 2D coordinate.
  //***************************************************************************
  template <typename TAxis, typename TValue>
  coordinate_2d<TAxis, TValue> operator +(const coordinate_2d<TAxis, TValue>& lhs, const coordinate_2d<TAxis, TValue>& rhs)
  {
    return coordinate_2d<TAxis, TValue>(lhs.x + rhs.x, lhs.y + rhs.y);
  }

  //***************************************************************************
  /// Subtraction operator for 2D coordinate.
  //***************************************************************************
  template <typename TAxis, typename TValue>
  coordinate_2d<TAxis, TValue> operator -(const coordinate_2d<TAxis, TValue>& lhs, const coordinate_2d<TAxis, TValue>& rhs)
  {
    return coordinate_2d<TAxis, TValue>(lhs.x - rhs.x, lhs.y - rhs.y);
  }

  //***************************************************************************
  /// Equality operator for 3D coordinate.
  //***************************************************************************
  template <typename TAxis, typename TValue>
  bool operator ==(const coordinate_3d<TAxis, TValue>& lhs, const coordinate_3d<TAxis, TValue>& rhs)
  {
    return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
  }

  //***************************************************************************
  /// Inequality operator for 3D coordinate.
  //***************************************************************************
  template <typename TAxis, typename TValue>
  bool operator !=(const coordinate_3d<TAxis, TValue>& lhs, const coordinate_3d<TAxis, TValue>& rhs)
  {
    return !(lhs == rhs);
  }
  
  //***************************************************************************
  /// Addition operator for 3D coordinate.
  //***************************************************************************
  template <typename TAxis, typename TValue>
  coordinate_3d<TAxis, TValue> operator +(const coordinate_3d<TAxis, TValue>& lhs, const coordinate_3d<TAxis, TValue>& rhs)
  {
    return coordinate_3d<TAxis, TValue>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
  }

  //***************************************************************************
  /// Subtraction operator for 3D coordinate.
  //***************************************************************************
  template <typename TAxis, typename TValue>
  coordinate_3d<TAxis, TValue> operator -(const coordinate_3d<TAxis, TValue>& lhs, const coordinate_3d<TAxis, TValue>& rhs)
  {
    return coordinate_3d<TAxis, TValue>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
  }

  //***************************************************************************
  /// Dot product for 2D coordinate.
  //***************************************************************************
  template <typename TAxis, typename TValue>
  TAxis dot(const etl::coordinate_2d<TAxis, TValue>& first, const etl::coordinate_2d<TAxis, TValue>& second)
  {
    return (first.x * second.x) + (first.y * second.y);
  }

  //***************************************************************************
  /// Dot product for 3D coordinate.
  //***************************************************************************
  template <typename TAxis, typename TValue>
  TAxis dot(const etl::coordinate_3d<TAxis, TValue>& first, const etl::coordinate_3d<TAxis, TValue>& second)
  {
    return (first.x * second.x) + (first.y * second.y) + (first.z * second.z);
  }

  //***************************************************************************
  /// Cross product for 2D coordinate.
  //***************************************************************************
  template <typename TAxis, typename TValue>
  TAxis cross(const etl::coordinate_2d<TAxis, TValue>& first, const etl::coordinate_2d<TAxis, TValue>& second)
  {
    return (first.x * second.y) - (first.y * second.x);
  }

  //***************************************************************************
  /// Cross product for 3D coordinate.
  //***************************************************************************
  template <typename TAxis, typename TValue>
  etl::coordinate_3d<TAxis, TValue> cross(const etl::coordinate_3d<TAxis, TValue>& first, const etl::coordinate_3d<TAxis, TValue>& second)
  {
    return etl::coordinate_3d<TAxis, TValue>((first.y * second.z) - (first.z * second.y),
                                             (first.z * second.x) - (first.x * second.z), 
                                             (first.x * second.y) - (first.y * second.x));
  }
}

#endif

