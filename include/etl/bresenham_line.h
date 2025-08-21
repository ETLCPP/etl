///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2020 John Wellbelove

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

#ifndef ETL_BRESENHAM_LINE_INCLUDED
#define ETL_BRESENHAM_LINE_INCLUDED

#include "platform.h"
#include "iterator.h"
#include "static_assert.h"
#include "type_traits.h"
#include "utility.h"

#include <stdint.h>

namespace etl
{
  //***************************************************************************
  /// A pseudo-container that generates points on a line, using Bresenham's
  /// line algorithm.
  /// T is the type for the etl::coordinate_2d value type.
  /// TWork is the internal working variable type. Default is int16_t.
  //***************************************************************************
  template <typename T, typename TWork = int16_t>
  class bresenham_line
  {
  public:

    //***************************************************
    /// Standard container types.
    //***************************************************
    typedef etl::coordinate_2d<T> value_type;
    typedef size_t                size_type;
    typedef ptrdiff_t             difference_type;
    typedef value_type&           reference;
    typedef const value_type&     const_reference;
    typedef value_type*           pointer;
    typedef const value_type*     const_pointer;

    //***************************************************
    /// Const Iterator
    //***************************************************
    class const_iterator : public etl::iterator<ETL_OR_STD::forward_iterator_tag, const value_type>
    {
    public:

      friend class bresenham_line;

      //***************************************************
      /// Default constructor
      //***************************************************
      const_iterator()
        : p_bresenham_line(ETL_NULLPTR)
      {
      }

      //***************************************************
      /// Copy constructor
      //***************************************************
      const_iterator(const const_iterator& other)
        : p_bresenham_line(other.p_bresenham_line)
      {
      }

      //***************************************************
      /// Assignment operator
      //***************************************************
      const_iterator& operator =(const const_iterator& rhs)
      {
        p_bresenham_line = rhs.p_bresenham_line;

        return *this;
      }

      //***************************************************
      /// Pre-increment operator
      //***************************************************
      const_iterator& operator ++()
      {
        // Has the end of the series has been reached?
        if (p_bresenham_line->get_coordinate() == p_bresenham_line->back())
        {
          // Mark it as an end iterator.
          p_bresenham_line = ETL_NULLPTR;
        }
        else
        {
          p_bresenham_line->next();
        }

        return *this;
      }

      //***************************************************
      /// De-reference operator
      //***************************************************
      value_type operator *() const
      {
        return p_bresenham_line->get_coordinate();
      }

      //***************************************************
      /// Equality operator
      //***************************************************
      friend bool operator ==(const const_iterator& lhs, const const_iterator& rhs)
      {
        return lhs.p_bresenham_line == rhs.p_bresenham_line;
      }

      //***************************************************
      /// Inequality operator
      //***************************************************
      friend bool operator !=(const const_iterator& lhs, const const_iterator& rhs)
      {
        return !(lhs == rhs);
      }

    private:

      //***************************************************
      /// Constructor for use by bresenham_line
      //***************************************************
      const_iterator(bresenham_line<T>* pb)
        : p_bresenham_line(pb)
      {
      }

      bresenham_line<T>* p_bresenham_line;
    };

    //***************************************************
    /// Constructor.
    //***************************************************
    bresenham_line()
    {
      initialise(T(0), T(0), T(0), T(0));
    }

    //***************************************************
    /// Constructor.
    /// Supplied first and last coordinates
    //***************************************************
    bresenham_line(etl::coordinate_2d<T> first_, etl::coordinate_2d<T> last_)
    {
      initialise(first_.x, first_.y, last_.x, last_.y);
    }

    //***************************************************
    /// Constructor.
    /// Supplied first and last coordinates
    //***************************************************
    bresenham_line(T first_x, T first_y, T last_x, T last_y)
    {
      initialise(first_x, first_y, last_x, last_y);
    }

    //***************************************************
    /// Resets the line.
    /// Supplied first and last coordinates
    //***************************************************
    void reset(etl::coordinate_2d<T> first_, etl::coordinate_2d<T> last_)
    {
      initialise(first_.x, first_.y, last_.x, last_.y);
    }

    //***************************************************
    /// Resets the line.
    /// Supplied first and last coordinates
    //***************************************************
    void reset(T first_x, T first_y, T last_x, T last_y)
    {
      initialise(first_x, first_y, last_x, last_y);
    }

    //***************************************************
    /// Get a const_iterator to the first coordinate.
    /// Resets the Bresenham line.
    //***************************************************
    const_iterator begin()
    {
      coordinate = first;

      return const_iterator(this);
    }

    //***************************************************
    /// Get a const_iterator to one past the last coordinate.
    //***************************************************
    const_iterator end() const
    {
      return const_iterator();
    }

    //***************************************************
    /// Get the first coordinate.
    //***************************************************
    const_reference front() const
    {
      return first;
    }

    //***************************************************
    /// Get the last coordinate.
    //***************************************************
    const_reference back() const
    {
      return last;
    }

    //***************************************************
    /// Get the size of the series.
    //***************************************************
    size_t size() const
    {
      if (y_is_major_axis())
      {
        return (dy / 2) + 1;       
      }
      else
      {
        return (dx / 2) + 1;
      }
    }

    //***************************************************
    /// Equality operator
    //***************************************************
    friend bool operator ==(const bresenham_line& lhs, const bresenham_line& rhs)
    {
      return (lhs.front() == rhs.front()) && (lhs.back() == rhs.back());
    }

    //***************************************************
    /// Inequality operator
    //***************************************************
    friend bool operator !=(const bresenham_line& lhs, const bresenham_line& rhs)
    {
      return !(lhs == rhs);
    }

  private:

    //***************************************************
    /// Get the current number of generated points.
    //***************************************************
    void initialise(T first_x, T first_y, T last_x, T last_y)
    {
      first              = value_type(first_x, first_y);
      last               = value_type(last_x,  last_y);
      coordinate         = first;
      x_increment        = (last_x < first_x) ? -1 : 1;
      y_increment        = (last_y < first_y) ? -1 : 1;
      dx                 = (last_x < first_x) ? first_x - last_x : last_x - first_x;
      dy                 = (last_y < first_y) ? first_y - last_y : last_y - first_y;
      do_minor_increment = false;

      if (y_is_major_axis())
      {
        dx *= 2;
        balance = dx - dy;
        dy *= 2;
      }
      else
      {
        dy *= 2;
        balance = dy - dx;
        dx *= 2;
      }
    }

    //***************************************************
    /// Returns true if Y is the major axis.
    //***************************************************
    bool y_is_major_axis() const
    {
      return dx < dy;
    }

    //***************************************************
    /// Calculate the next point.
    //***************************************************
    void next()
    {
      if (y_is_major_axis())
      {
        // Y is major axis.
        if (do_minor_increment)
        {
          coordinate.x = T(coordinate.x + x_increment);
          balance -= dy;
        }

        coordinate.y = T(coordinate.y + y_increment);
        balance += dx;
      }
      else
      {
        // X is major axis.
        if (do_minor_increment)
        {
          coordinate.y = T(coordinate.y + y_increment);
          balance -= dx;
        }

        coordinate.x = T(coordinate.x + x_increment);
        balance += dy;
      }

      do_minor_increment = (balance >= 0);
    }

    //***************************************************
    /// Get the current coordinate.
    //***************************************************
    value_type get_coordinate() const
    {
      return coordinate;
    }

    typedef TWork work_t;

    value_type first;
    value_type last;
    value_type coordinate;
    work_t     x_increment;
    work_t     y_increment;
    work_t     dx;
    work_t     dy;
    work_t     balance;
    bool       do_minor_increment;
  };
}

#endif

