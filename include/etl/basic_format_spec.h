///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2019 jwellbelove

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

#ifndef ETL_BASIC_FORMAT_SPEC_INCLUDED
#define ETL_BASIC_FORMAT_SPEC_INCLUDED

///\ingroup string

#include "platform.h"
#include "type_traits.h"
#include "static_assert.h"

namespace etl
{
  //***************************************************************************
  // Stream formatting manipulators.
  //***************************************************************************
  namespace private_basic_format_spec
  {
    //*********************************
    struct base_spec
    {
      base_spec(uint_least8_t base_)
        : base(base_)
      {
      }

      const uint_least8_t base;
    };

    //*********************************
    struct width_spec
    {
      width_spec(uint_least8_t width_)
        : width(width_)
      {
      }

      const uint_least8_t width;
    };

    //*********************************
    template <typename TChar>
    struct fill_spec
    {
      fill_spec(TChar fill_)
        : fill(fill_)
      {
      }

      const TChar fill;
    };

    //*********************************
    struct precision_spec
    {
      precision_spec(uint_least8_t precision_)
        : precision(precision_)
      {
      }

      const uint_least8_t precision;
    };

    //*********************************
    struct upper_case_spec
    {
      upper_case_spec(bool upper_case_)
        : upper_case(upper_case_)
      {
      }

      const bool upper_case;
    };

    //*********************************
    struct boolalpha_spec
    {
      boolalpha_spec(bool boolalpha_)
        : boolalpha(boolalpha_)
      {
      }

      const bool boolalpha;
    };

    //*********************************
    struct left_spec
    {
    };

    //*********************************
    struct right_spec
    {
    };
  }

  //*********************************
  static private_basic_format_spec::base_spec set_base(uint32_t base)
  {
    return private_basic_format_spec::base_spec(base);
  }

  //*********************************
  static private_basic_format_spec::base_spec binary()
  {
    return private_basic_format_spec::base_spec(2U);
  }

  //*********************************
  static private_basic_format_spec::base_spec octal()
  {
    return private_basic_format_spec::base_spec(8U);
  }

  //*********************************
  static private_basic_format_spec::base_spec decimal()
  {
    return private_basic_format_spec::base_spec(10U);
  }

  //*********************************
  static private_basic_format_spec::base_spec hex()
  {
    return private_basic_format_spec::base_spec(16U);
  }

  //*********************************
  static private_basic_format_spec::width_spec set_width(uint32_t width)
  {
    return private_basic_format_spec::width_spec(width);
  }

  //*********************************
  template <typename TChar>
  static private_basic_format_spec::fill_spec<TChar> set_fill(TChar fill)
  {
    return private_basic_format_spec::fill_spec<TChar>(fill);
  }

  //*********************************
  static private_basic_format_spec::left_spec set_left()
  {
    return private_basic_format_spec::left_spec();
  }

  //*********************************
  static private_basic_format_spec::right_spec set_right()
  {
    return private_basic_format_spec::right_spec();
  }

  //*********************************
  static private_basic_format_spec::precision_spec set_precision(uint32_t precision)
  {
    return private_basic_format_spec::precision_spec(precision);
  }

  //*********************************
  static private_basic_format_spec::upper_case_spec set_upper_case(bool upper_case)
  {
    return private_basic_format_spec::upper_case_spec(upper_case);
  }

  //*********************************
  static private_basic_format_spec::boolalpha_spec set_boolalpha(bool boolalpha)
  {
    return private_basic_format_spec::boolalpha_spec(boolalpha);
  }

  //***************************************************************************
  /// basic_format_spec
  //***************************************************************************
  template <typename TString>
  class basic_format_spec
  {
  public:

    //***************************************************************************
    /// Default constructor.
    /// Sets:-
    /// Base = 10
    /// Width = 0
    /// Upper case (for hex) = true
    /// Left Justified = false
    //***************************************************************************
    basic_format_spec()
      : base_(10U)
      , width_(0U)
      , precision_(0U)
      , upper_case_(true)
      , left_justified_(false)
      , boolalpha_(false)
      , fill_(typename TString::value_type(' '))
    {

    }

    //***************************************************************************
    /// Sets the base.
    /// \return A reference to the basic_format_spec.
    //***************************************************************************
    basic_format_spec& base(uint32_t b)
    {
      base_ = static_cast<uint_least8_t>(b);
      return *this;
    }

    //***************************************************************************
    /// Sets the base to binary.
    /// \return A reference to the basic_format_spec.
    //***************************************************************************
    basic_format_spec& binary()
    {
      base(2);
      return *this;
    }

    //***************************************************************************
    /// Sets the base to octal.
    /// \return A reference to the basic_format_spec.
    //***************************************************************************
    basic_format_spec& octal()
    {
      base(8);
      return *this;
    }

    //***************************************************************************
    /// Sets the base to decimal.
    /// \return A reference to the basic_format_spec.
    //***************************************************************************
    basic_format_spec& decimal()
    {
      base(10);
      return *this;
    }

    //***************************************************************************
    /// Sets the base to hex.
    /// \return A reference to the basic_format_spec.
    //***************************************************************************
    basic_format_spec& hex()
    {
      base(16);
      return *this;
    }

    //***************************************************************************
    /// Gets the base.
    //***************************************************************************
    uint32_t get_base() const
    {
      return base_;
    }

    //***************************************************************************
    /// Sets the width.
    /// \return A reference to the basic_format_spec.
    //***************************************************************************
    basic_format_spec& width(uint32_t w)
    {
      width_ = static_cast<uint_least8_t>(w);
      return *this;
    }

    //***************************************************************************
    /// Gets the width.
    //***************************************************************************
    uint32_t get_width() const
    {
      return width_;
    }

    //***************************************************************************
    /// Sets the precision.
    /// \return A reference to the basic_format_spec.
    //***************************************************************************
    basic_format_spec& precision(uint32_t p)
    {
      precision_ = static_cast<uint_least8_t>(p);
      return *this;
    }

    //***************************************************************************
    /// Gets the precision.
    //***************************************************************************
    uint32_t get_precision() const
    {
      return precision_;
    }

    //***************************************************************************
    /// Sets the upper case flag.
    /// \return A reference to the basic_format_spec.
    //***************************************************************************
    basic_format_spec& upper_case(bool u)
    {
      upper_case_ = u;
      return *this;
    }

    //***************************************************************************
    /// Gets the upper case flag.
    //***************************************************************************
    bool is_upper_case() const
    {
      return upper_case_;
    }

    //***************************************************************************
    /// Sets the fill character.
    /// \return A reference to the basic_format_spec.
    //***************************************************************************
    basic_format_spec& fill(typename TString::value_type c)
    {
      fill_ = c;
      return *this;
    }

    //***************************************************************************
    /// Gets the fill character.
    //***************************************************************************
    typename TString::value_type get_fill() const
    {
      return fill_;
    }

    //***************************************************************************
    /// Sets the left justify flag.
    /// \return A reference to the basic_format_spec.
    //***************************************************************************
    basic_format_spec& left()
    {
      left_justified_ = true;
      return *this;
    }

    //***************************************************************************
    /// Gets the left justify flag.
    //***************************************************************************
    bool is_left() const
    {
      return left_justified_;
    }

    //***************************************************************************
    /// Sets the right justify flag.
    /// \return A reference to the basic_format_spec.
    //***************************************************************************
    basic_format_spec& right()
    {
      left_justified_ = false;
      return *this;
    }

    //***************************************************************************
    /// Gets the right justify flag.
    //***************************************************************************
    bool is_right() const
    {
      return !left_justified_;
    }

    //***************************************************************************
    /// Sets the bool alpha flag.
    /// \return A reference to the basic_format_spec.
    //***************************************************************************
    basic_format_spec& boolalpha(bool b)
    {
      boolalpha_ = b;
      return *this;
    }

    //***************************************************************************
    /// Gets the boolalpha flag.
    //***************************************************************************
    bool is_boolalpha() const
    {
      return boolalpha_;
    }

    //***************************************************************************
    /// Equality operator.
    //***************************************************************************
    friend bool operator ==(const basic_format_spec& lhs, const basic_format_spec& rhs)
    {
      return (lhs.base_ == rhs.base_) &&
             (lhs.width_ == rhs.width_) &&
             (lhs.precision_ == rhs.precision_) &&
             (lhs.upper_case_ == rhs.upper_case_) &&
             (lhs.left_justified_ == rhs.left_justified_) &&
             (lhs.boolalpha_ == rhs.boolalpha_) &&
             (lhs.fill_ == rhs.fill_);
    }

    //***************************************************************************
    /// Inequality operator.
    //***************************************************************************
    friend bool operator !=(const basic_format_spec& lhs, const basic_format_spec& rhs)
    {
      return !(lhs == rhs);
    }

  private:

    uint_least8_t base_;
    uint_least8_t width_;
    uint_least8_t precision_;
    bool upper_case_;
    bool left_justified_;
    bool boolalpha_;
    typename TString::value_type fill_;
  };
}

#endif
