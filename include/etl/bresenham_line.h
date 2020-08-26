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

#include <stddef.h>

#include "platform.h"
#include "iterator.h"
#include "exception.h"

namespace etl
{
  //***************************************************************************
  /// A pseudo-container that generates points on a line, using Bresenham's
  /// line algorithm.
  /// TPoint must support integral x & y members.
  /// TIntegral is the internal working variable type. Default is int.
  //***************************************************************************
  template <typename TPoint, typename TIntegral = int>
  class bresenham_line
  {
  public:

    //***************************************************
    /// Standard container types.
    //***************************************************
    typedef TPoint            value_type;
    typedef size_t            size_type;
    typedef ptrdiff_t         difference_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;

    class iterator : public etl::iterator<ETL_OR_STD::forward_iterator_tag, TPoint>
    {
    public:

      friend class bresenham_line;

      //***************************************************
      /// 
      //***************************************************
      iterator()
        : p_bresenham_line(ETL_NULLPTR)
      {
      }

      //***************************************************
      /// 
      //***************************************************
      iterator(const iterator& other)
        : p_bresenham_line(other.p_bresenham_line)
      {
      }

      //***************************************************
      /// 
      //***************************************************
      iterator& operator =(const iterator& rhs)
      {
        p_bresenham_line = rhs.p_bresenham_line;
      }

      //***************************************************
      /// 
      //***************************************************
      iterator& operator ++()
      {
        // Has the end of the series has been reached?
        if (p_bresenham_line->current_count == 0)
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
      /// 
      //***************************************************
      value_type operator *() const
      {
        return p_bresenham_line->get_point();
      }

      //***************************************************
      /// 
      //***************************************************
      value_type operator ->() const
      {
        return p_bresenham_line->get_point();
      }

      //***************************************************
      /// 
      //***************************************************
      friend bool operator ==(const iterator& lhs, const iterator& rhs)
      {
        return lhs.p_bresenham_line == rhs.p_bresenham_line;
      }

      //***************************************************
      /// 
      //***************************************************
      friend bool operator !=(const iterator& lhs, const iterator& rhs)
      {
        return !(lhs == rhs);
      }

    private:

      //***************************************************
      /// 
      //***************************************************
      iterator(bresenham_line<TPoint, TIntegral>* pb)
        : p_bresenham_line(pb)
      {
      }

      bresenham_line<TPoint, TIntegral>* p_bresenham_line;
    };

    //***************************************************
    /// Constructor.
    //***************************************************
    bresenham_line(const_reference first_, const_reference last_)
      : first(first_)
      , last(last_)
      , current(first_)
      , x_increment((last_.x < first_.x) ? -1 : 1)
      , y_increment((last_.y < first_.y) ? -1 : 1)
      , do_minor_increment(false)
    {
      dx = (last_.x < first_.x) ? first_.x - last_.x : last_.x - first_.x;
      dy = (last_.y < first_.y) ? first_.y - last_.y : last_.y - first_.y;

      if (is_y_major_axis())
      {
        total_count = dy;
        dx *= 2;
        balance = dx - dy;
        dy *= 2;
      }
      else
      {
        total_count = dx;
        dy *= 2;
        balance = dy - dx;
        dx *= 2;
      }

      current_count = total_count;
    }

    //***************************************************
    /// 
    //***************************************************
    iterator begin()
    {
      current_count = total_count;
      current = first;

      return iterator(this);
    }

    //***************************************************
    /// 
    //***************************************************
    iterator end() const
    {
      return iterator();
    }

    //***************************************************
    /// Get the size of the series.
    //***************************************************
    size_t size() const
    {
      return total_count;
    }

    //***************************************************
    /// Get the current number of generated points.
    //***************************************************
    size_t count() const
    {
      return current_count;
    }

  private:

    //***************************************************
    /// Returns true if Y is the major axis.
    //***************************************************
    int is_y_major_axis() const
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
          current.x += x_increment;
          balance -= dy;
        }

        current.y += y_increment;
        balance += dx;
      }
      else
      {
        // X major axis.
        if (do_minor_increment)
        {
          current.y += y_increment;
          balance -= dx;
        }

        current.x += x_increment;
        balance += dy;
      }

      --current_count;

      do_minor_increment = (balance >= 0);
    }

    //***************************************************
    /// Get the current point.
    //***************************************************
    value_type get_point() const
    {
      return current;
    }

    const value_type   first;
    const value_type   last;
    value_type         current;
    const TIntegral    x_increment;
    const TIntegral    y_increment;
    TIntegral          dx;
    TIntegral          dy;
    TIntegral          total_count;
    TIntegral          current_count;
    TIntegral          balance;
    bool               do_minor_increment;
  };
}

#endif

