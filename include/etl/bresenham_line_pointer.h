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

#ifndef ETL_BRESENHAM_LINE_POINTER_INCLUDED
#define ETL_BRESENHAM_LINE_POINTER_INCLUDED

#include <stdint.h>

#include "platform.h"
#include "iterator.h"
#include "static_assert.h"
#include "type_traits.h"
#include "utility.h"

namespace etl
{
  //***************************************************************************
  /// A pseudo-container that generates pointers to points on a line, using Bresenham's
  /// line algorithm.
  /// T is the type of the type of the value being pointerd to.
  /// TWork is the internal working variable type. Default is int16_t.
  //***************************************************************************
  template <typename T, typename TWork = int16_t>
  class bresenham_line_pointer
  {
  public:

    //***************************************************
    /// Standard container types.
    //***************************************************
    typedef size_t size_type;
    typedef T*     pointer;

    //***************************************************
    /// Iterator
    //***************************************************
    class iterator : public etl::iterator<ETL_OR_STD::forward_iterator_tag, T>
    {
    public:

      friend class bresenham_line_pointer;

      //***************************************************
      /// Default constructor
      //***************************************************
      iterator()
        : p_bresenham_line_pointer(ETL_NULLPTR)
      {
      }

      //***************************************************
      /// Constuctor
      //***************************************************
      iterator(const iterator& other)
        : p_bresenham_line_pointer(other.p_bresenham_line_pointer)
      {
      }

      //***************************************************
      /// Assignment operator
      //***************************************************
      iterator& operator =(const iterator& rhs)
      {
        p_bresenham_line_pointer = rhs.p_bresenham_line_pointer;

        return *this;
      }

      //***************************************************
      /// Pre-increment operator
      //***************************************************
      iterator& operator ++()
      {
        // Has the end of the series has been reached?
        if (p_bresenham_line_pointer->get_pointer() == p_bresenham_line_pointer->back())
        {
          // Mark it as an end iterator.
          p_bresenham_line_pointer = ETL_NULLPTR;
        }
        else
        {
          p_bresenham_line_pointer->next();
        }

        return *this;
      }

      //***************************************************
      /// De-reference operator
      //***************************************************
      pointer operator *()
      {
        return p_bresenham_line_pointer->get_pointer();
      }

      //***************************************************
      /// De-reference operator
      //***************************************************
      pointer operator *() const
      {
        return p_bresenham_line_pointer->get_pointer();
      }

      //***************************************************
      /// Equality operator
      //***************************************************
      friend bool operator ==(const iterator& lhs, const iterator& rhs)
      {
        return lhs.p_bresenham_line_pointer == rhs.p_bresenham_line_pointer;
      }

      //***************************************************
      /// Inequality operator
      //***************************************************
      friend bool operator !=(const iterator& lhs, const iterator& rhs)
      {
        return !(lhs == rhs);
      }

    private:

      //***************************************************
      /// Constructor for use by bresenham_line_pointer
      //***************************************************
      iterator(bresenham_line_pointer<T>* pb)
        : p_bresenham_line_pointer(pb)
      {
      }

      bresenham_line_pointer<T>* p_bresenham_line_pointer;
    };

    //***************************************************
    /// Constructor.
    /// Supplied first and last coordinates
    //***************************************************
    bresenham_line_pointer(pointer origin_, size_t stride_)
    {
      initialise(origin_, stride_);
    }

    //***************************************************
    /// Constructor.
    /// Supplied first and last coordinates
    //***************************************************
    bresenham_line_pointer(pointer origin_, size_t stride_, TWork first_x_, TWork first_y_, TWork last_x_, TWork last_y_)
    {
      initialise(origin_, stride_, first_x_, first_y_, last_x_, last_y_);
    }

    //***************************************************
    /// Resets the line.
    /// Supplied first and last coordinates
    //***************************************************
    void reset(TWork first_x_, TWork first_y_, TWork last_x_, TWork last_y_)
    {
      initialise(first_x_, first_y_, last_x_, last_y_);
    }

    //***************************************************
    /// Get a iterator to the first coordinate.
    /// Resets the Bresenham line.
    //***************************************************
    iterator begin()
    {
      pixel = front();

      return iterator(this);
    }

    //***************************************************
    /// Get a iterator to one past the last coordinate.
    //***************************************************
    iterator end() const
    {
      return iterator();
    }

    //***************************************************
    /// Get the first pointer.
    //***************************************************
    pointer front() const
    {
      return origin + first_x + (stride * first_y);
    }

    //***************************************************
    /// Get the last pointer.
    //***************************************************
    pointer back() const
    {
      return origin + last_x + (stride * last_y);
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
    friend bool operator ==(const bresenham_line_pointer& lhs, const bresenham_line_pointer& rhs)
    {
      return (lhs.front() == rhs.front()) && (lhs.back() == rhs.back());
    }

    //***************************************************
    /// Inequality operator
    //***************************************************
    friend bool operator !=(const bresenham_line_pointer& lhs, const bresenham_line_pointer& rhs)
    {
      return !(lhs == rhs);
    }

  private:

    //***************************************************
    /// 
    //***************************************************
    void initialise(pointer origin_, size_t stride_)
    {
      origin = origin_;
      stride = stride_;
    }

    //***************************************************
    /// 
    //***************************************************
    void initialise(pointer origin_, size_t stride_, TWork first_x_, TWork first_y_, TWork last_x_, TWork last_y_)
    {
      origin = origin_;
      stride = stride_;

      initialise(first_x_, first_y_, last_x_, last_y_);
    }

    //***************************************************
    /// 
    //***************************************************
    void initialise(TWork first_x_, TWork first_y_, TWork last_x_, TWork last_y_)
    {
      pixel              = origin + first_x_ + (stride * first_y_);
      first_x            = first_x_;
      first_y            = first_y_;
      last_x             = last_x_;
      last_y             = last_y_;
      x_increment        = (last_x_ < first_x_) ? -1 : 1;
      y_increment        = (last_y_ < first_y_) ? -work_t(stride) : work_t(stride);
      dx                 = (last_x_ < first_x_) ? first_x_ - last_x_ : last_x_ - first_x_;
      dy                 = (last_y_ < first_y_) ? first_y_ - last_y_ : last_y_ - first_y_;
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
          pixel += x_increment;
          balance -= dy;
        }

        pixel += y_increment;
        balance += dx;
      }
      else
      {
        // X is major axis.
        if (do_minor_increment)
        {
          pixel += y_increment;
          balance -= dx;
        }

        pixel += x_increment;
        balance += dy;
      }

      do_minor_increment = (balance >= 0);
    }

    //***************************************************
    /// Get the pointer to the current pixel.
    //***************************************************
    pointer get_pointer()
    {
      return pixel;
    }

    //***************************************************
    /// Get the pointer to the current pixel.
    //***************************************************
    const pointer get_pointer() const
    {
      return pixel;
    }

    typedef TWork work_t;

    pointer   origin;
    size_type stride;
    work_t    first_x;
    work_t    first_y;
    work_t    last_x;
    work_t    last_y;
    pointer   pixel;
    work_t    x_increment;
    work_t    y_increment;
    work_t    dx;
    work_t    dy;
    work_t    balance;
    bool      do_minor_increment;
  };
}

#endif

