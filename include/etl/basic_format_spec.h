///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2019 John Wellbelove

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
  namespace private_basic_format_spec
  {
    //*******************************************************
    // Structures returned by stream formatting manipulators.
    //*******************************************************
    struct base_spec
    {
      ETL_CONSTEXPR base_spec(uint_least8_t base_)
        : base(base_)
      {
      }

      const uint_least8_t base;
    };

    //*********************************
    struct width_spec
    {
      ETL_CONSTEXPR width_spec(uint_least8_t width_)
        : width(width_)
      {
      }

      const uint_least8_t width;
    };

    //*********************************
    template <typename TChar>
    struct fill_spec
    {
      ETL_CONSTEXPR fill_spec(TChar fill_)
        : fill(fill_)
      {
      }

      const TChar fill;
    };

    //*********************************
    struct precision_spec
    {
      ETL_CONSTEXPR precision_spec(uint_least8_t precision_)
        : precision(precision_)
      {
      }

      const uint_least8_t precision;
    };

    //*********************************
    struct uppercase_spec
    {
      ETL_CONSTEXPR uppercase_spec(bool upper_case_)
        : upper_case(upper_case_)
      {
      }

      const bool upper_case;
    };

    //*********************************
    struct boolalpha_spec
    {
      ETL_CONSTEXPR boolalpha_spec(bool boolalpha_)
        : boolalpha(boolalpha_)
      {
      }

      const bool boolalpha;
    };

    //*********************************
    struct showbase_spec
    {
      ETL_CONSTEXPR showbase_spec(bool show_base_)
        : show_base(show_base_)
      {
      }

      const bool show_base;
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

  //***************************************************************************
  // Stream formatting manipulators.
  //***************************************************************************
  static ETL_CONSTEXPR private_basic_format_spec::base_spec setbase(uint32_t base)
  {
    return private_basic_format_spec::base_spec(base);
  }

  //*********************************
  static ETL_CONSTEXPR private_basic_format_spec::width_spec setw(uint32_t width)
  {
    return private_basic_format_spec::width_spec(width);
  }

  //*********************************
  template <typename TChar>
  static ETL_CONSTEXPR private_basic_format_spec::fill_spec<TChar> setfill(TChar fill)
  {
    return private_basic_format_spec::fill_spec<TChar>(fill);
  }

  //*********************************
  static ETL_CONSTEXPR private_basic_format_spec::precision_spec setprecision(uint32_t precision)
  {
    return private_basic_format_spec::precision_spec(precision);
  }

  //*********************************
  static ETL_CONSTANT private_basic_format_spec::base_spec bin(2U);

  //*********************************
  static ETL_CONSTANT private_basic_format_spec::base_spec oct(8U);

  //*********************************
  static ETL_CONSTANT private_basic_format_spec::base_spec dec(10U);

  //*********************************
  static ETL_CONSTANT private_basic_format_spec::base_spec hex(16U);

  //*********************************
  static ETL_CONSTANT private_basic_format_spec::left_spec left = private_basic_format_spec::left_spec();

  //*********************************
  static ETL_CONSTANT private_basic_format_spec::right_spec right = private_basic_format_spec::right_spec();

  //*********************************
  static ETL_CONSTANT private_basic_format_spec::boolalpha_spec boolalpha(true);

  //*********************************
  static ETL_CONSTANT private_basic_format_spec::boolalpha_spec noboolalpha(false);

  //*********************************
  static ETL_CONSTANT private_basic_format_spec::uppercase_spec uppercase(true);

  //*********************************
  static ETL_CONSTANT private_basic_format_spec::uppercase_spec nouppercase(false);

  //*********************************
  static ETL_CONSTANT private_basic_format_spec::showbase_spec showbase(true);

  //*********************************
  static ETL_CONSTANT private_basic_format_spec::showbase_spec noshowbase(false);

  //***************************************************************************
  /// basic_format_spec
  //***************************************************************************
  template <typename TString>
  class basic_format_spec
  {
  public:

    //***************************************************************************
    /// Default constructor.
    //***************************************************************************
    ETL_CONSTEXPR basic_format_spec()
      : base_(10U)
      , width_(0U)
      , precision_(0U)
      , upper_case_(false)
      , left_justified_(false)
      , boolalpha_(false)
      , show_base_(false)
      , fill_(typename TString::value_type(' '))
    {
    }

    //***************************************************************************
    /// Constructor.
    //***************************************************************************
    ETL_CONSTEXPR basic_format_spec(uint_least8_t base__,
                                    uint_least8_t width__,
                                    uint_least8_t precision__,
                                    bool upper_case__,
                                    bool left_justified__,
                                    bool boolalpha__,
                                    bool show_base__,
                                    typename TString::value_type fill__)
      : base_(base__)
      , width_(width__)
      , precision_(precision__)
      , upper_case_(upper_case__)
      , left_justified_(left_justified__)
      , boolalpha_(boolalpha__)
      , show_base_(show_base__)
      , fill_(fill__)
    {
    }

    //***************************************************************************
    /// Clears the format spec back to default.
    //***************************************************************************
    ETL_CONSTEXPR14 void clear()
    {
      base_           = 10U;
      width_          = 0U;
      precision_      = 0U;
      upper_case_     = false;
      left_justified_ = false;
      boolalpha_      = false;
      show_base_      = false;
      fill_           = typename TString::value_type(' ');
    }

    //***************************************************************************
    /// Sets the base.
    /// \return A reference to the basic_format_spec.
    //***************************************************************************
    ETL_CONSTEXPR14 basic_format_spec& base(uint32_t b)
    {
      base_ = static_cast<uint_least8_t>(b);
      return *this;
    }

    //***************************************************************************
    /// Sets the base to binary.
    /// \return A reference to the basic_format_spec.
    //***************************************************************************
    ETL_CONSTEXPR14 basic_format_spec& binary()
    {
      base(2);
      return *this;
    }

    //***************************************************************************
    /// Sets the base to octal.
    /// \return A reference to the basic_format_spec.
    //***************************************************************************
    ETL_CONSTEXPR14 basic_format_spec& octal()
    {
      base(8);
      return *this;
    }

    //***************************************************************************
    /// Sets the base to decimal.
    /// \return A reference to the basic_format_spec.
    //***************************************************************************
    ETL_CONSTEXPR14 basic_format_spec& decimal()
    {
      base(10);
      return *this;
    }

    //***************************************************************************
    /// Sets the base to hex.
    /// \return A reference to the basic_format_spec.
    //***************************************************************************
    ETL_CONSTEXPR14 basic_format_spec& hex()
    {
      base(16);
      return *this;
    }

    //***************************************************************************
    /// Gets the base.
    //***************************************************************************
    ETL_CONSTEXPR uint32_t get_base() const
    {
      return base_;
    }

    //***************************************************************************
    /// Sets the show base flag.
    /// \return A reference to the basic_format_spec.
    //***************************************************************************
    ETL_CONSTEXPR14 basic_format_spec& show_base(bool b)
    {
      show_base_ = b;
      return *this;
    }

    //***************************************************************************
    /// Gets the show base flag.
    //***************************************************************************
    ETL_CONSTEXPR bool is_show_base() const
    {
      return show_base_;
    }

    //***************************************************************************
    /// Sets the width.
    /// \return A reference to the basic_format_spec.
    //***************************************************************************
    ETL_CONSTEXPR14 basic_format_spec& width(uint32_t w)
    {
      width_ = static_cast<uint_least8_t>(w);
      return *this;
    }

    //***************************************************************************
    /// Gets the width.
    //***************************************************************************
    ETL_CONSTEXPR uint32_t get_width() const
    {
      return width_;
    }

    //***************************************************************************
    /// Sets the precision.
    /// \return A reference to the basic_format_spec.
    //***************************************************************************
    ETL_CONSTEXPR14 basic_format_spec& precision(uint32_t p)
    {
      precision_ = static_cast<uint_least8_t>(p);
      return *this;
    }

    //***************************************************************************
    /// Gets the precision.
    //***************************************************************************
    ETL_CONSTEXPR uint32_t get_precision() const
    {
      return precision_;
    }

    //***************************************************************************
    /// Sets the upper case flag.
    /// \return A reference to the basic_format_spec.
    //***************************************************************************
    ETL_CONSTEXPR14 basic_format_spec& upper_case(bool u)
    {
      upper_case_ = u;
      return *this;
    }

    //***************************************************************************
    /// Gets the upper case flag.
    //***************************************************************************
    ETL_CONSTEXPR bool is_upper_case() const
    {
      return upper_case_;
    }

    //***************************************************************************
    /// Sets the fill character.
    /// \return A reference to the basic_format_spec.
    //***************************************************************************
    ETL_CONSTEXPR14 basic_format_spec& fill(typename TString::value_type c)
    {
      fill_ = c;
      return *this;
    }

    //***************************************************************************
    /// Gets the fill character.
    //***************************************************************************
    ETL_CONSTEXPR typename TString::value_type get_fill() const
    {
      return fill_;
    }

    //***************************************************************************
    /// Sets the left justify flag.
    /// \return A reference to the basic_format_spec.
    //***************************************************************************
    ETL_CONSTEXPR14 basic_format_spec& left()
    {
      left_justified_ = true;
      return *this;
    }

    //***************************************************************************
    /// Gets the left justify flag.
    //***************************************************************************
    ETL_CONSTEXPR bool is_left() const
    {
      return left_justified_;
    }

    //***************************************************************************
    /// Sets the right justify flag.
    /// \return A reference to the basic_format_spec.
    //***************************************************************************
    ETL_CONSTEXPR14 basic_format_spec& right()
    {
      left_justified_ = false;
      return *this;
    }

    //***************************************************************************
    /// Gets the right justify flag.
    //***************************************************************************
    ETL_CONSTEXPR bool is_right() const
    {
      return !left_justified_;
    }

    //***************************************************************************
    /// Sets the bool alpha flag.
    /// \return A reference to the basic_format_spec.
    //***************************************************************************
    ETL_CONSTEXPR14 basic_format_spec& boolalpha(bool b)
    {
      boolalpha_ = b;
      return *this;
    }

    //***************************************************************************
    /// Gets the boolalpha flag.
    //***************************************************************************
    ETL_CONSTEXPR bool is_boolalpha() const
    {
      return boolalpha_;
    }

    //***************************************************************************
    /// Equality operator.
    //***************************************************************************
    ETL_CONSTEXPR friend bool operator ==(const basic_format_spec& lhs, const basic_format_spec& rhs)
    {
      return (lhs.base_ == rhs.base_) &&
             (lhs.width_ == rhs.width_) &&
             (lhs.precision_ == rhs.precision_) &&
             (lhs.upper_case_ == rhs.upper_case_) &&
             (lhs.left_justified_ == rhs.left_justified_) &&
             (lhs.boolalpha_ == rhs.boolalpha_) &&
             (lhs.show_base_ == rhs.show_base_) &&
             (lhs.fill_ == rhs.fill_);
    }

    //***************************************************************************
    /// Inequality operator.
    //***************************************************************************
    ETL_CONSTEXPR friend bool operator !=(const basic_format_spec& lhs, const basic_format_spec& rhs)
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
    bool show_base_;
    typename TString::value_type fill_;
  };
}

#endif
