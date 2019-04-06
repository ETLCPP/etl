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

#ifndef ETL_FORMAT_SPEC_INCLUDED
#define ETL_FORMAT_SPEC_INCLUDED

///\ingroup string

#include "platform.h"
#include "type_traits.h"
#include "static_assert.h"

namespace etl
{
  template <typename TString>
  class format_spec
  {
  public:

    //***************************************************************************
    /// Default constructor.
    /// Sets:-
    /// Base = 10
    /// Width = 0
    /// Upper case (for hex) = true
    /// Rifght Justified = true
    //***************************************************************************
    format_spec()
      : base_(10)
      , width_(0)
      , upper_case_(true)
      , right_justified_(true)
      , fill_(typename TString::value_type(' '))
    {

    }

    //***************************************************************************
    /// Sets the base.
    /// \return A reference to the format_spec.
    //***************************************************************************
    format_spec& base(const uint32_t b)
    {
      base_ = static_cast<uint_least8_t>(b);
      return *this;
    }

    //***************************************************************************
    /// Gets the base.
    //***************************************************************************
    uint32_t base() const
    {
      return base_;
    }

    //***************************************************************************
    /// Sets the width.
    /// \return A reference to the format_spec.
    //***************************************************************************
    format_spec& width(const uint32_t w)
    {
      width_ = static_cast<uint_least8_t>(w);
      return *this;
    }

    //***************************************************************************
    /// Gets the width.
    //***************************************************************************
    uint32_t width() const
    {
      return width_;
    }

    //***************************************************************************
    /// Sets the upper case flag.
    /// \return A reference to the format_spec.
    //***************************************************************************
    format_spec& upper_case(const bool u)
    {
      upper_case_ = u;
      return *this;
    }

    //***************************************************************************
    /// Gets the upper case flag.
    //***************************************************************************
    bool upper_case(const bool u) const
    {
      return upper_case_;
    }

    //***************************************************************************
    /// Sets the fill character.
    /// \return A reference to the format_spec.
    //***************************************************************************
    format_spec& fill(const typename TString::value_type c)
    {
      fill_ = c;
      return *this;
    }

    //***************************************************************************
    /// Gets the fill character.
    //***************************************************************************
    typename TString::value_type fill() const
    {
      return fill_;
    }

    //***************************************************************************
    /// Sets the right justify flag.
    /// \return A reference to the format_spec.
    //***************************************************************************
    format_spec& right_justified(const bool r)
    {
      right_justified_ = r;
      return *this;
    }

    //***************************************************************************
    /// Gets the right justify flag.
    //***************************************************************************
    bool right_justified() const
    {
      return right_justified_;
    }

  private:

    uint_least8_t base_;
    uint_least8_t width_;
    bool upper_case_;
    bool right_justified_;
    typename TString::value_type fill_;
  };
}

#endif
