///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl

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

#include "integral_limits.h"
#include "nullptr.h"
#include "log.h"
#include "ibitset.h"

#if defined(COMPILER_KEIL)
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
  /// Will accomodate any number of bits.
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
      std::copy(other.data, other.data + ARRAY_SIZE, data);
    }

    //*************************************************************************
    /// Construct from a value.
    //*************************************************************************
    bitset(unsigned long long value)
      : ibitset(N, ARRAY_SIZE, data)
    {
      reset();

      const size_t SHIFT = (integral_limits<unsigned long long>::bits <= BITS_PER_ELEMENT) ? 0 : BITS_PER_ELEMENT;

      // Can we do it in one hit?
      if (SHIFT == 0)
      {
        data[0] = element_t(value);
      }
      else
      {
        size_t i = 0;

        while ((value != 0) && (i < ARRAY_SIZE))
        {
          data[i++] = value & ALL_SET;
          value = value >> SHIFT;
        }
      }

      data[ARRAY_SIZE - 1] &= TOP_MASK;
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
    bitset<N>&  set(const char* text)
    {
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
      for (size_t i = 0; i < ARRAY_SIZE; ++i)
      {
        data[i] = other.data[i];
      }

      return *this;
    }

    //*************************************************************************
    /// operator &=
    //*************************************************************************
    bitset<N>& operator &=(const bitset<N>& other)
    {
      for (size_t i = 0; i < ARRAY_SIZE; ++i)
      {
        data[i] &= other.data[i];
      }

      return *this;
    }

    //*************************************************************************
    /// operator |=
    //*************************************************************************
    bitset<N>& operator |=(const bitset<N>& other)
    {
      for (size_t i = 0; i < ARRAY_SIZE; ++i)
      {
        data[i] |= other.data[i];
      }

      return *this;
    }

    //*************************************************************************
    /// operator ^=
    //*************************************************************************
    bitset<N>& operator ^=(const bitset<N>& other)
    {
      for (size_t i = 0; i < ARRAY_SIZE; ++i)
      {
        data[i] ^= other.data[i];
      }

      return *this;
    }

    //*************************************************************************
    /// operator ~
    //*************************************************************************
    bitset<N> operator ~() const
    {
      bitset<N> temp;

      for (size_t i = 0; i < ARRAY_SIZE; ++i)
      {
        temp[i] = ~data[i];
      }

      return temp;
    }

    //*************************************************************************
    /// operator <<
    //*************************************************************************
    bitset<N> operator<<(size_t shift) const
    {
      bitset<N> temp;

      if (ARRAY_SIZE == 1)
      {
        temp.data[0] = data[0] << shift;
      }
      else
      {
        size_t source      = N - shift - 1;
        size_t destination = N - 1;

        for (size_t i = 0; i < (N - shift); ++i)
        {
          temp.set(destination--, test(source--));
        }
      }

      return temp;
    }

    //*************************************************************************
    /// operator <<=
    //*************************************************************************
    bitset<N>& operator<<=(size_t shift)
    {
      if (ARRAY_SIZE == 1)
      {
        data[0] <<= shift;
      }
      else
      {
        size_t source      = N - shift - 1;
        size_t destination = N - 1;

        for (size_t i = 0; i < (N - shift); ++i)
        {
          set(destination--, test(source--));
        }

        for (size_t i = 0; i < shift; ++i)
        {
          reset(destination--);
        }
      }

      return *this;
    }

    //*************************************************************************
    /// operator >>
    //*************************************************************************
    bitset<N> operator>>(size_t shift) const
    {
      bitset<N> temp;

      if (ARRAY_SIZE == 1)
      {
        temp.data[0] = data[0] >> shift;
      }
      else
      {
        size_t source      = shift;
        size_t destination = 0;

        while (source != N)
        {
          temp.set(destination++, test(source++));
        }
      }

      return temp;
    }

    //*************************************************************************
    /// operator >>=
    //*************************************************************************
    bitset<N>& operator>>=(size_t shift)
    {
      if (ARRAY_SIZE == 1)
      {
        data[0] >>= shift;
      }
      else
      {
        size_t source      = shift;
        size_t destination = 0;

        for (size_t i = 0; i < (N - shift); ++i)
        {
          set(destination++, test(source++));
        }

        for (size_t i = 0; i < shift; ++i)
        {
          reset(destination++);
        }
      }

      return *this;
    }

    //*************************************************************************
    /// swap
    //*************************************************************************
    void swap(bitset<N>& other)
    {
      for (size_t i = 0; i < ARRAY_SIZE; ++i)
      {
        std::swap(data, other.data);
      }
    }

    //*************************************************************************
    /// operator ==
    //*************************************************************************
    friend bool operator == (const bitset<N>& lhs, const bitset<N>& rhs)
    {
      for (size_t i = 0; i < ARRAY_SIZE; ++i)
      {
        if (lhs.data[i] != rhs.data[i])
        {
          return false;
        }
      }

      return true;
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
