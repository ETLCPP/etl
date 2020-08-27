///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2020 jwellbelove

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

#include <stdint.h>

#include "platform.h"
#include "iterator.h"
#include "static_assert.h"
#include "type_traits.h"
#include "utility.h"

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
      /// Copy constuctor
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
        if (p_bresenham_line->n_coordinates_remaining == 0)
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
    /// Update the line.
    /// Supplied first and last coordinates
    //***************************************************
    void update_line(etl::coordinate_2d<T> first_, etl::coordinate_2d<T> last_)
    {
      initialise(first_.x, first_.y, last_.x, last_.y);
    }

    //***************************************************
    /// Update the line.
    /// Supplied first and last coordinates
    //***************************************************
    void update_line(T first_x, T first_y, T last_x, T last_y)
    {
      initialise(first_x, first_y, last_x, last_y);
    }

    //***************************************************
    /// Get a const_iterator to the first coordinate.
    /// Resets the Bresenham line.
    //***************************************************
    const_iterator begin()
    {
      n_coordinates_remaining = total_n_coordinates - 1; // We already have the first coordinate.
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
    /// Get the size of the series.
    //***************************************************
    size_t size() const
    {
      return size_t(total_n_coordinates);
    }

    //***************************************************
    /// Get the current number of generated points.
    //***************************************************
    size_t count() const
    {
      return size_t(total_n_coordinates - n_coordinates_remaining);
    }

  private:

    //***************************************************
    /// Get the current number of generated points.
    //***************************************************
    void initialise(T first_x, T first_y, T last_x, T last_y)
    {
      first              = value_type(first_x, first_y);
      coordinate         = value_type(first_x, first_y);
      x_increment        = (last_x < first_x) ? -1 : 1;
      y_increment        = (last_y < first_y) ? -1 : 1;
      dx                 = (last_x < first_x) ? first_x - last_x : last_x - first_x;
      dy                 = (last_y < first_y) ? first_y - last_y : last_y - first_y;
      do_minor_increment = false;

      if (is_y_major_axis())
      {
        total_n_coordinates = dy + 1;
        dx *= 2;
        balance = dx - dy;
        dy *= 2;
      }
      else
      {
        total_n_coordinates = dx + 1;
        dy *= 2;
        balance = dy - dx;
        dx *= 2;
      }

      n_coordinates_remaining = total_n_coordinates - 1; // We already have the first coordinate.
    }

    //***************************************************
    /// Returns true if Y is the major axis.
    //***************************************************
    bool is_y_major_axis() const
    {
      return dx < dy;
    }

    //***************************************************
    /// Calculate the next point.
    //***************************************************
    void next()
    {
      if (is_y_major_axis())
      {
        // Y major axis.
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
        // X major axis.
        if (do_minor_increment)
        {
          coordinate.y = T(coordinate.y + y_increment);
          balance -= dx;
        }

        coordinate.x = T(coordinate.x + x_increment);
        balance += dy;
      }

      --n_coordinates_remaining;

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
    value_type coordinate;
    work_t     x_increment;
    work_t     y_increment;
    work_t     dx;
    work_t     dy;
    work_t     total_n_coordinates;
    work_t     n_coordinates_remaining;
    work_t     balance;
    bool       do_minor_increment;
  };
}

#endif

