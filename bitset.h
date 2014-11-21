///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.

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

#include <cstring>
#include <cstddef>
#include <algorithm>
#include <cstdint>

#include "integral_limits.h"
#include "smallest.h"
#include "array.h"
#include "nullptr.h"

#if WIN32
#undef min
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
  class bitset
  {
  public:

    //*************************************************************************
    /// The reference type returned.
    //*************************************************************************
    class reference
    {
    public:

      friend class bitset<N>;

      //*******************************
      /// Conversion operator.
      //*******************************
      inline operator bool() const
      {
        return p_bitset->test(position);
      }

      //*******************************
      /// Assignment operator.
      //*******************************
      inline reference& operator = (bool b)
      {
        p_bitset->set(position, b);
        return *this;
      }

      //*******************************
      /// Assignment operator.
      //*******************************
      inline reference& operator = (const reference& r)
      {
        p_bitset->set(position, bool(r));
        return *this;
      }

      //*******************************
      /// Flip the bit.
      //*******************************
      inline reference& flip()
      {
        p_bitset->flip(position);
        return *this;
      }

      //*******************************
      /// Return the logical inverse of the bit.
      //*******************************
      inline bool operator~() const
      {
        return !p_bitset->test(position);
      }

    private:

      //*******************************
      /// Default constructor.
      //*******************************
      inline reference()
        : p_bitset(nullptr),
          position(0)
      {
      }

      //*******************************
      /// Constructor.
      //*******************************
      inline reference(bitset<N>& r_bitset, size_t position)
        : p_bitset(&r_bitset),
          position(position)
      {
      }

      bitset<N>* p_bitset; ///< The bitset.
      size_t     position; ///< The position in the bitset.
    };

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    bitset()
    {
      reset();
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    bitset(const bitset<N>& other)
    {
      data = other.data;
    }

    //*************************************************************************
    /// Construct from a value.
    //*************************************************************************
    bitset(unsigned long long value)
    {
      reset();

      const size_t SHIFT = (integral_limits<unsigned long long>::bits <= BITS_PER_ELEMENT) ? 0 : BITS_PER_ELEMENT;

      // Can we do it in one hit?
      if (SHIFT == 0)
      {
        data[0] = value;
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

      data.back() &= TOP_MASK;
    }

    //*************************************************************************
    /// Construct from a string.
    //*************************************************************************
    bitset(const char* text)
    {
      reset();

      size_t i = std::min(N, std::strlen(text));

      while (i > 0)
      {
        set(--i, *text++ == '1');
      }
    }

    //*************************************************************************
    /// Set all of the bits.
    //*************************************************************************
    bitset<N>& set()
    {
      data.fill(ALL_SET);
      data.back() &= TOP_MASK;

      return *this;
    }

    //*************************************************************************
    /// Set the bit at the position.
    //*************************************************************************
    bitset<N>& set(size_t position, bool value = true)
    {     
      if (position < N)
      {
        size_t       index;
        element_type bit;

        if (ARRAY_SIZE == 1)
        {
          index = 0;
          bit   = element_type(1) << position;
        }
        else
        {
          index = position / BITS_PER_ELEMENT;
          bit   = element_type(1) << position % BITS_PER_ELEMENT;
        }

        if (value)
        {
          data[index] |= bit;
        }
        else
        {
          data[index] &= ~bit;
        }
      }

      return *this;
    }

    //*************************************************************************
    /// Reset all of the bits.
    //*************************************************************************
    bitset<N>& reset()
    {
      data.fill(ALL_CLEAR);
      return *this;
    }

    //*************************************************************************
    /// Reset the bit at the position.
    //*************************************************************************
    bitset<N>& reset(size_t position)
    {
      if (position < N)
      {
        size_t       index;
        element_type bit;

        if (ARRAY_SIZE == 1)
        {
          index = 0;
          bit   = element_type(1) << position;
        }
        else
        {
          index = position / BITS_PER_ELEMENT;
          bit = element_type(1) << position % BITS_PER_ELEMENT;
        }

        data[index] &= ~bit;
      }

      return *this;
    }

    //*************************************************************************
    /// Flip all of the bits.
    //*************************************************************************
    bitset<N>& flip()
    {
      for (size_t i = 0; i < ARRAY_SIZE; ++i)
      {
        data[i] = ~data[i];
      }

      data.back() &= TOP_MASK;

      return *this;
    }

    //*************************************************************************
    /// Flip the bit at the position.
    //*************************************************************************
    bitset<N>& flip(size_t position)
    {
      if (position < N)
      {
        size_t       index;
        element_type bit;

        if (ARRAY_SIZE == 1)
        {
          index = 0;
          bit   = element_type(1) << position;
        }
        else
        {
          index = position / BITS_PER_ELEMENT;
          bit   = element_type(1) << position % BITS_PER_ELEMENT;
        }

        data[index] ^= bit;
      }

      return *this;
    }

    //*************************************************************************
    /// Read [] operator.
    //*************************************************************************
    bool operator[] (size_t position) const
    {
      return test(position);
    }

    //*************************************************************************
    /// Write [] operator.
    //*************************************************************************
    reference operator [] (size_t position)
    {
      return reference(*this, position);
    }

    //*************************************************************************
    /// Tests a bit at a position.
    /// Positions greater than the number of configured bits will return <b>false</b>.
    //*************************************************************************
    bool test(size_t position) const
    {
      if (position < N)
      {
        size_t       index;
        element_type bit;

        if (ARRAY_SIZE == 1)
        {
          index = 0;
          bit   = element_type(1) << position;
        }
        else
        {
          index = position / BITS_PER_ELEMENT;
          bit = element_type(1) << position % BITS_PER_ELEMENT;
        }

        return (data[index] & bit) != 0;
      }
      else
      {
        return false;
      }
    }

    //*************************************************************************
    // Are all the bits sets?
    //*************************************************************************
    bool all() const
    {
      // All but the last.
      for (size_t i = 0; i < (ARRAY_SIZE - 1); ++i)
      {
        if (data[i] != ALL_SET)
        {
          return false;
        }
      }

      // The last.
      if (data[ARRAY_SIZE - 1] != (ALL_SET & TOP_MASK))
      {
        return false;
      }

      return true;
    }

    //*************************************************************************
    /// Are any of the bits set?
    //*************************************************************************
    bool any() const
    {
      return !none();
    }

    //*************************************************************************
    /// Are none of th bits set?
    //*************************************************************************
    bool none() const
    {
      for (size_t i = 0; i < ARRAY_SIZE; ++i)
      {
        if (data[i] != 0)
        {
          return false;
        }
      }

      return true;
    }

    //*************************************************************************
    size_t count() const
    {
      size_t n = 0;

      for (size_t i = 0; i < ARRAY_SIZE; ++i)
      {
        element_type value = data[i];

        while (value != 0)
        {
          n += (value & 1);

          value >>= 1;
        }
      }

      return n;
    }

    //*************************************************************************
    size_t size() const
    {
      return N;
    }

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
    bitset<N>& operator |=(const bitset<N>& other)
    {
      for (size_t i = 0; i < ARRAY_SIZE; ++i)
      {
        data[i] |= other.data[i];
      }

      return *this;
    }

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
    bitset<N> operator<<(size_t shift) const
    {
      bitset<N> temp;

      if (ARRAY_SIZE == 1)
      {
        temp.data[0] = data[0] << shift;
      }
      else
      {
        size_t source = N - shift - 1;
        size_t destination = N - 1;

        for (size_t i = 0; i < (N - shift); ++i)
        {
          temp.set(destination--, test(source--));
        }
      }

      return temp;
    }

    //*************************************************************************
    bitset<N>& operator<<=(size_t shift)
    {
      if (ARRAY_SIZE == 1)
      {
        data[0] <<= shift;
      }
      else
      {
        size_t source = N - shift - 1;
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
    bitset<N> operator>>(size_t shift) const
    {
      bitset<N> temp;

      if (ARRAY_SIZE == 1)
      {
        temp.data[0] = data[0] >> shift;
      }
      else
      {
        size_t source = shift;
        size_t destination = 0;

        while (source != N)
        {
          temp.set(destination++, test(source++));
        }
      }

      return temp;
    }

    //*************************************************************************
    bitset<N>& operator>>=(size_t shift)
    {
      if (ARRAY_SIZE == 1)
      {
        data[0] >>= shift;
      }
      else
      {
        size_t source = shift;
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

    //***************************************************************************
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
       
    // The type used for each element in the array.
    typedef typename smallest_uint_for_bits<N>::type element_type;

    static const element_type ALL_SET          = etl::integral_limits<element_type>::max;
    static const element_type ALL_CLEAR        = 0;
    static const size_t       BITS_PER_ELEMENT = etl::integral_limits<element_type>::bits;
    static const size_t       ARRAY_SIZE       = (N % BITS_PER_ELEMENT == 0) ? N / BITS_PER_ELEMENT : N / BITS_PER_ELEMENT + 1;
    static const size_t       TOTAL_BITS       = ARRAY_SIZE * BITS_PER_ELEMENT;
    static const size_t       TOP_MASK_SHIFT   = ((BITS_PER_ELEMENT - (TOTAL_BITS - N)) % BITS_PER_ELEMENT);
    static const element_type TOP_MASK         = element_type(TOP_MASK_SHIFT == 0 ? ALL_SET : ~(ALL_SET << TOP_MASK_SHIFT));

    etl::array<element_type, ARRAY_SIZE> data;
  };

  //***************************************************************************
  ///\ingroup bitset
  template <const size_t N>
  bitset<N> operator & (const bitset<N>& lhs, const bitset<N>& rhs)
  {
    bitset<N> temp(lhs);
    temp &= rhs;
    return temp;
  }

  //***************************************************************************
  ///\ingroup bitset
  template<const size_t N>
  bitset<N> operator | (const bitset<N>& lhs, const bitset<N>& rhs)
  {
    bitset<N> temp(lhs);
    temp |= rhs;
    return temp;
  }

  //***************************************************************************
  ///\ingroup bitset
  template<const size_t N>
  bitset<N> operator ^ (const bitset<N>& lhs, const bitset<N>& rhs)
  {
    bitset<N> temp(lhs);
    temp ^= rhs;
    return temp;
  }

  //***************************************************************************
  ///\ingroup bitset
  template<const size_t N>
  bool operator != (const bitset<N>& lhs, const bitset<N>& rhs)
  {
    return !(lhs == rhs);
  }
}

#if WIN32
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#endif
