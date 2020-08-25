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
    typedef value_type&       reference;
    typedef const value_type& const_reference;
    typedef value_type*       pointer;
    typedef const value_type* const_pointer;

    friend class const_iterator;

    class const_iterator : public etl::iterator<ETL_OR_STD::forward_iterator_tag, TPoint>
    {
    public:

      friend class bresenham_line;

      //***************************************************
      /// 
      //***************************************************
      const_iterator()
        : p_bresenham_line(ETL_NULLPTR)
      {
      }

      //***************************************************
      /// 
      //***************************************************
      const_iterator(const const_iterator& other)
        : p_bresenham_line(other.p_bresenham_line)
      {
      }

      //***************************************************
      /// 
      //***************************************************
      const_iterator& operator =(const const_iterator& rhs)
      {
        p_bresenham_line = rhs.p_bresenham_line;
      }

      //***************************************************
      /// 
      //***************************************************
      const_iterator& operator ++()
      {
        // Has the end of the series has been reached?
        if (p_bresenham_line->count == 0)
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

    private:

      //***************************************************
      /// 
      //***************************************************
      const_iterator(bresenham_line<value_type>* pb)
        : p_bresenham_line(pb)
      {
      }

      bresenham_line<TPoint>* p_bresenham_line;
    };

    //***************************************************
    /// Constructor.
    //***************************************************
    bresenham_line(const_reference start_, const_reference end_)
      : start(start_)
      , end(end_)
      , current(start_)
      , x_increment((end_.x < start_.x) ? -1 : 1)
      , y_increment((end_.y < start_.y) ? -1 : 1)
      , do_minor_increment(true)
    {
      dx = (end_.x < start_.x) ? start_.x - end_.x : end_.x - start_.x;
      dy = (end_.y < start_.y) ? start_.y - end_.y : end_.y - start_.y;

      if (is_y_major_axis())
      {
        total_count = dy;
        dx          *= 2;
        balance     = dx - dy;
        dy          *= 2;
      }
      else
      {
        total_count = dx;
        dy          *= 2;
        balance     = dy - dx;
        dx          *= 2;
      }

      current_count = total_count;
    }

    //***************************************************
    /// 
    //***************************************************
    const_iterator begin() const
    {
      current_count = total_count;
      current       = start;

      return const_iterator(this);
    }

    //***************************************************
    /// 
    //***************************************************
    const_iterator cbegin() const
    {
      current_count = total_count;
      current       = start;

      return const_iterator(this);
    }

    //***************************************************
    /// 
    //***************************************************
    const_iterator end() const
    {
      return const_iterator();
    }

    //***************************************************
    /// 
    //***************************************************
    const_iterator cend() const
    {
      return const_iterator();
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

    const value_type start;
    const value_type end;
    value_type       current;
    const TIntegral  x_increment;
    const TIntegral  y_increment;
    TIntegral        dx;
    TIntegral        dy;
    TIntegral        total_count;
    TIntegral        current_count;
    TIntegral        balance;
    bool             do_minor_increment;
  };

#endif

