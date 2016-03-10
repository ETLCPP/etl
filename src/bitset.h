///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2014 jwellbelove

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

#ifndef __ETL_BITSET__
#define __ETL_BITSET__

#include <algorithm>
#include <iterator>
#include <string.h>
#include <stddef.h>

#include "platform.h"
#include "integral_limits.h"
#include "algorithm.h"
#include "nullptr.h"
#include "log.h"
#include "ibitset.h"

#define ETL_NO_CHECKS
#include "error_handler.h"

#if defined(ETL_COMPILER_KEIL)
#pragma diag_suppress 1300 
#endif

//*****************************************************************************
///\defgroup bitset bitset
/// Similar to std::bitset but without requiring std::string.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //*************************************************************************
  /// The class emulates an array of bool elements, but optimized for space allocation.
  /// Will accommodate any number of bits.
  /// Does not use std::string.
  ///\tparam N The number of bits.
  ///\ingroup bitset
  //*************************************************************************
  template <const size_t N>
  class bitset : public ibitset
  {

    static const size_t    ARRAY_SIZE = (N % BITS_PER_ELEMENT == 0) ? N / BITS_PER_ELEMENT : N / BITS_PER_ELEMENT + 1;

  public:

    static const size_t    ALLOCATED_BITS = ARRAY_SIZE * BITS_PER_ELEMENT;

  public:
    
    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    bitset()
      : ibitset(N, ARRAY_SIZE, data)
    {
      reset();
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    bitset(const bitset<N>& other)
      : ibitset(N, ARRAY_SIZE, data)
    {
      etl::copy_n(other.data, ARRAY_SIZE, data);
    }

    //*************************************************************************
    /// Construct from a value.
    //*************************************************************************
    bitset(unsigned long long value)
      : ibitset(N, ARRAY_SIZE, data)
    {
      initialise(value);
    }

    //*************************************************************************
    /// Construct from a string.
    //*************************************************************************
    bitset(const char* text)
      : ibitset(N, ARRAY_SIZE, data)
    {
      set(text);
    }

    //*************************************************************************
    /// Set all of the bits.
    //*************************************************************************
    bitset<N>& set()
    {
      ibitset::set();
      return *this;
    }

    //*************************************************************************
    /// Set the bit at the position.
    //*************************************************************************
    bitset<N>& set(size_t position, bool value = true)
    {     
      ibitset::set(position, value);
      return *this;
    }

    //*************************************************************************
    /// Set from a string.
    //*************************************************************************
    bitset<N>& set(const char* text)
    {
      ETL_ASSERT(text != 0, ETL_ERROR(bitset_nullptr));
      ibitset::set(text);

      return *this;
    }

    //*************************************************************************
    /// Reset all of the bits.
    //*************************************************************************
    bitset<N>& reset()
    {
      ibitset::reset();
      return *this;
    }

    //*************************************************************************
    /// Reset the bit at the position.
    //*************************************************************************
    bitset<N>& reset(size_t position)
    {
      ibitset::reset(position);
      return *this;
    }

    //*************************************************************************
    /// Flip all of the bits.
    //*************************************************************************
    bitset<N>& flip()
    {
      ibitset::flip();
      return *this;
    }

    //*************************************************************************
    /// Flip the bit at the position.
    //*************************************************************************
    bitset<N>& flip(size_t position)
    {
      ibitset::flip(position);
      return *this;
    }

    //*************************************************************************
    /// operator =
    //*************************************************************************
    bitset<N>& operator =(const bitset<N>& other)
    {
      if (this != &other)
      {
        etl::copy_n(other.data, ARRAY_SIZE, data);
      }

      return *this;
    }

    //*************************************************************************
    /// operator &=
    //*************************************************************************
    bitset<N>& operator &=(const bitset<N>& other)
    {
      ibitset::operator &=(other);
      return *this;
    }

    //*************************************************************************
    /// operator |=
    //*************************************************************************
    bitset<N>& operator |=(const bitset<N>& other)
    {
      ibitset::operator |=(other);
      return *this;
    }

    //*************************************************************************
    /// operator ^=
    //*************************************************************************
    bitset<N>& operator ^=(const bitset<N>& other)
    {
      ibitset::operator ^=(other);
      return *this;
    }

    //*************************************************************************
    /// operator ~
    //*************************************************************************
    bitset<N> operator ~() const
    {
      bitset<N> temp(*this);

      temp.invert();
      
      return temp;
    }

    //*************************************************************************
    /// operator <<
    //*************************************************************************
    bitset<N> operator<<(size_t shift) const
    {
      bitset<N> temp(*this);

      temp <<= shift;

      return temp;
    }

    //*************************************************************************
    /// operator <<=
    //*************************************************************************
    bitset<N>& operator<<=(size_t shift)
    {
      ibitset::operator <<=(shift);
      return *this;
    }

    //*************************************************************************
    /// operator >>
    //*************************************************************************
    bitset<N> operator>>(size_t shift) const
    {
      bitset<N> temp(*this);

      temp >>= shift;

      return temp;
    }

    //*************************************************************************
    /// operator >>=
    //*************************************************************************
    bitset<N>& operator>>=(size_t shift)
    {
      ibitset::operator >>=(shift);
      return *this;
    }

    //*************************************************************************
    /// operator ==
    //*************************************************************************
    friend bool operator == (const bitset<N>& lhs, const bitset<N>& rhs)
    {
      return ibitset::is_equal(lhs, rhs);
    }

  private:
       
    element_t data[ARRAY_SIZE];
  };

  //***************************************************************************
  /// operator &
  ///\ingroup bitset
  //***************************************************************************
  template <const size_t N>
  bitset<N> operator & (const bitset<N>& lhs, const bitset<N>& rhs)
  {
    bitset<N> temp(lhs);
    temp &= rhs;
    return temp;
  }

  //***************************************************************************
  /// operator |
  ///\ingroup bitset
  //***************************************************************************
  template<const size_t N>
  bitset<N> operator | (const bitset<N>& lhs, const bitset<N>& rhs)
  {
    bitset<N> temp(lhs);
    temp |= rhs;
    return temp;
  }

  //***************************************************************************
  /// operator ^
  ///\ingroup bitset
  //***************************************************************************
  template<const size_t N>
  bitset<N> operator ^ (const bitset<N>& lhs, const bitset<N>& rhs)
  {
    bitset<N> temp(lhs);
    temp ^= rhs;
    return temp;
  }

  //***************************************************************************
  /// operator !=
  ///\ingroup bitset
  //***************************************************************************
  template<const size_t N>
  bool operator != (const bitset<N>& lhs, const bitset<N>& rhs)
  {
    return !(lhs == rhs);
  }
}

//*************************************************************************
/// swap
//*************************************************************************
template <const size_t N>
void swap(etl::bitset<N>& lhs, etl::bitset<N>& rhs)
{
  lhs.swap(rhs);
}

#endif
