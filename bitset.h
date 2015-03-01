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
#include <stdint.h>
#include <stddef.h>

#include "integral_limits.h"
#include "smallest.h"
#include "array.h"
#include "nullptr.h"
#include "log.h"
#include "ibitset.h"
#include "binary.h"

#if WIN32
#undef min
#endif

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
    // The type used for each element in the array.
    typedef typename smallest_uint_for_bits<N>::type element_t;

    static const element_t ALL_SET          = etl::integral_limits<element_t>::max;
    static const element_t ALL_CLEAR        = 0;
    static const size_t    BITS_PER_ELEMENT = etl::integral_limits<element_t>::bits;
    static const size_t    ARRAY_SIZE       = (N % BITS_PER_ELEMENT == 0) ? N / BITS_PER_ELEMENT : N / BITS_PER_ELEMENT + 1;

  public:

    static const size_t    TOTAL_BITS       = ARRAY_SIZE * BITS_PER_ELEMENT;

  private:

    static const size_t    TOP_MASK_SHIFT   = ((BITS_PER_ELEMENT - (TOTAL_BITS - N)) % BITS_PER_ELEMENT);
    static const element_t TOP_MASK         = element_t(TOP_MASK_SHIFT == 0 ? ALL_SET : ~(ALL_SET << TOP_MASK_SHIFT));

  public:

    //*************************************************************************
    /// The reference type returned.
    //*************************************************************************
    class bit_reference
    {
    public:

      friend class bitset<N>;

      //*******************************
      /// Conversion operator.
      //*******************************
      operator bool() const
      {
        return p_bitset->test(position);
      }

      //*******************************
      /// Assignment operator.
      //*******************************
      bit_reference& operator = (bool b)
      {
        p_bitset->set(position, b);
        return *this;
      }

      //*******************************
      /// Assignment operator.
      //*******************************
      bit_reference& operator = (const bit_reference& r)
      {
        p_bitset->set(position, bool(r));
        return *this;
      }

      //*******************************
      /// Flip the bit.
      //*******************************
      bit_reference& flip()
      {
        p_bitset->flip(position);
        return *this;
      }

      //*******************************
      /// Return the logical inverse of the bit.
      //*******************************
      bool operator~() const
      {
        return !p_bitset->test(position);
      }

    private:

      //*******************************
      /// Default constructor.
      //*******************************
      bit_reference()
        : p_bitset(nullptr),
          position(0)
      {
      }

      //*******************************
      /// Constructor.
      //*******************************
      bit_reference(bitset<N>& r_bitset, size_t position)
        : p_bitset(&r_bitset),
          position(position)
      {
      }

      bitset<N>* p_bitset; ///< The bitset.
      size_t     position; ///< The position in the bitset.
    };

    //*************************************************************************
    /// The iterator type.
    //*************************************************************************
    class iterator : public std::iterator<std::random_access_iterator_tag, bool>
    {
    public:

      friend class bitset<N>;
      friend class const_iterator;

      //*******************************
      /// Constructor
      //*******************************
      iterator()
        : position(0)
      {
      }

      //*******************************
      /// Copy constructor
      //*******************************
      iterator(const iterator& other)
        : position(other.position)
      {
      }

      //*******************************
      /// ++ operator (pre)
      //*******************************
      iterator& operator ++()
      {
        ++position;
				return *this;
      }

      //*******************************
      /// ++ operator (post)
      //*******************************
      iterator operator ++(int)
      {
        iterator temp(*this);
        ++position;
        return temp;
      }

      //*******************************
      /// -- operator (pre)
      //*******************************
      iterator& operator --()
      {
        --position;
				return *this;
      }

      //*******************************
      /// -- operator (post)
      //*******************************
      iterator operator --(int)
      {
        iterator temp(*this);
        --position;
        return temp;
      }

      //*******************************
      /// * operator
      //*******************************
      bit_reference operator *()
      {
        return bit_reference(*p_bitset, position);
      }

      //*******************************
      /// * operator const
      //*******************************
      bool operator *() const
      {
        return p_bitset->test(position);
      }

      //*******************************
      /// += operator
      //*******************************
      iterator& operator +=(int i)
      {
        position += i;
        return *this;
      }

      //*******************************
      /// -= operator
      //*******************************
      iterator& operator -=(int i)
      {
        position -= i;
        return *this;
      }

      //*******************************
      /// = operator
      //*******************************
      iterator& operator =(const iterator& other)
      {
        position = other.position;
        p_bitset = other.p_bitset;
        return *this;
      }

      //*******************************
      /// + operator
      //*******************************
      friend iterator operator +(const iterator& other, int i)
      {
        iterator temp(other);
        temp += i;
        return temp;
      }

      //*******************************
      /// - operator
      //*******************************
      friend iterator operator -(const iterator& other, int i)
      {
        iterator temp(other);
        temp -= i;
        return temp;
      }

      //*******************************
      /// - operator
      //*******************************
      friend long operator -(const iterator& lhs, const iterator& rhs)
      {
        return long(lhs.position) - long(rhs.position);
      }

      //*******************************
      /// == operator
      //*******************************
      friend bool operator ==(const iterator& lhs, const iterator& rhs)
      {
        return lhs.position == rhs.position;
      }

      //*******************************
      /// != operator
      //*******************************
      friend bool operator !=(const iterator& lhs, const iterator& rhs)
      {
        return lhs.position != rhs.position;
      }

      //*******************************
      /// < operator
      //*******************************
      friend bool operator <(const iterator& lhs, const iterator& rhs)
      {
        return lhs.position < rhs.position;
      }

      //*******************************
      /// > operator
      //*******************************
      friend bool operator >(const iterator& lhs, const iterator& rhs)
      {
        return lhs.position > rhs.position;
      }

      //*******************************
      /// <= operator
      //*******************************
      friend bool operator <=(const iterator& lhs, const iterator& rhs)
      {
        return lhs.position <= rhs.position;
      }

      //*******************************
      /// >= operator
      //*******************************
      friend bool operator >=(const iterator& lhs, const iterator& rhs)
      {
        return lhs.position >-rhs.position;
      }

    private:

      //*******************************
      /// Constructor
      //*******************************
      iterator(bitset<N>& r_bitset, size_t position)
        : p_bitset(&r_bitset),
          position(position)
      {}

      bitset<N>* p_bitset;
      size_t     position;
    };

    //*************************************************************************
    /// The const_iterator type.
    //*************************************************************************
    class const_iterator : public std::iterator<std::random_access_iterator_tag, const bool>
    {
    public:

      friend class bitset<N>;

      //*******************************
      /// Constructor
      //*******************************
      const_iterator()
        : position(0)
      {
      }

      //*******************************
      /// Copy constructor from iterator
      //*******************************
      const_iterator(const typename bitset<N>::iterator& other)
        : position(other.position)
      {
      }

      //*******************************
      /// Copy constructor
      //*******************************
      const_iterator(const const_iterator& other)
        : position(other.position)
      {
      }

      //*******************************
      /// ++ operator (pre)
      //*******************************
      const_iterator& operator ++()
      {
        ++position;
				return *this;
      }

      //*******************************
      /// ++ operator (post)
      //*******************************
      const_iterator operator ++(int)
      {
        const_iterator temp(*this);
        ++position;
        return temp;
      }

      //*******************************
      /// -- operator (pre)
      //*******************************
      const_iterator& operator --()
      {
        --position;
				return *this;
      }

      //*******************************
      /// -- operator (post)
      //*******************************
      const_iterator operator --(int)
      {
        const_iterator temp(*this);
        --position;
        return temp;
      }

      //*******************************
      /// * operator const
      //*******************************
      bool operator *() const
      {
        return p_bitset->test(position);
      }

      //*******************************
      /// += operator
      //*******************************
      const_iterator& operator +=(int i)
      {
        position += i;
        return *this;
      }

      //*******************************
      /// -= operator
      //*******************************
      const_iterator& operator -=(int i)
      {
        position -= i;
        return *this;
      }

      //*******************************
      /// = operator
      //*******************************
      const_iterator& operator =(const const_iterator& other)
      {
        position = other.position;
        p_bitset = other.p_bitset;
        return *this;
      }

      //*******************************
      /// + operator
      //*******************************
      friend const_iterator operator +(const const_iterator& other, int i)
      {
        const_iterator temp(other);
        temp += i;
        return temp;
      }

      //*******************************
      /// - operator
      //*******************************
      friend const_iterator operator -(const const_iterator& other, int i)
      {
        const_iterator temp(other);
        temp -= i;
        return temp;
      }

      //*******************************
      /// - operator
      //*******************************
      friend long operator -(const const_iterator& lhs, const const_iterator& rhs)
      {
        return long(lhs.position) - long(rhs.position);
      }

      //*******************************
      /// == operator
      //*******************************
      friend bool operator ==(const const_iterator& lhs, const const_iterator& rhs)
      {
        return lhs.position == rhs.position;
      }

      //*******************************
      /// != operator
      //*******************************
      friend bool operator !=(const const_iterator& lhs, const const_iterator& rhs)
      {
        return lhs.position != rhs.position;
      }

      //*******************************
      /// < operator
      //*******************************
      friend bool operator <(const const_iterator& lhs, const const_iterator& rhs)
      {
        return lhs.position < rhs.position;
      }

      //*******************************
      /// > operator
      //*******************************
      friend bool operator >(const const_iterator& lhs, const const_iterator& rhs)
      {
        return lhs.position > rhs.position;
      }

      //*******************************
      /// <= operator
      //*******************************
      friend bool operator <=(const const_iterator& lhs, const const_iterator& rhs)
      {
        return lhs.position <= rhs.position;
      }

      //*******************************
      /// >= operator
      //*******************************
      friend bool operator >=(const const_iterator& lhs, const const_iterator& rhs)
      {
        return lhs.position >- rhs.position;
      }

    private:

      //*******************************
      /// Constructor
      //*******************************
      const_iterator(bitset<N>& r_bitset, size_t position)
        : p_bitset(&r_bitset),
          position(position)
      {
      }

      bitset<N>* p_bitset;
      size_t     position;
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

      data.back() &= TOP_MASK;
    }

    //*************************************************************************
    /// Construct from a string.
    //*************************************************************************
    bitset(const char* text)
    {
      reset();

      size_t i = std::min(N, strlen(text));

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
        size_t    index;
        element_t bit;

        if (ARRAY_SIZE == 1)
        {
          index = 0;
          bit   = element_t(1) << position;
        }
        else
        {
          index = position >> log2<BITS_PER_ELEMENT>::value;
          bit   = element_t(1) << (position & (BITS_PER_ELEMENT - 1));
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
    /// Set from a string.
    //*************************************************************************
    bitset<N>&  set(const char* text)
    {
      reset();

      size_t i = std::min(N, strlen(text));

      while (i > 0)
      {
        set(--i, *text++ == '1');
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
        element_t bit;

        if (ARRAY_SIZE == 1)
        {
          index = 0;
          bit   = element_t(1) << position;
        }
        else
        {
          index = position >> log2<BITS_PER_ELEMENT>::value;
          bit   = element_t(1) << (position & (BITS_PER_ELEMENT - 1));
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
        size_t    index;
        element_t bit;

        if (ARRAY_SIZE == 1)
        {
          index = 0;
          bit   = element_t(1) << position;
        }
        else
        {
          index = position >> log2<BITS_PER_ELEMENT>::value;
          bit   = element_t(1) << (position & (BITS_PER_ELEMENT - 1));
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
    bit_reference operator [] (size_t position)
    {
      return bit_reference(*this, position);
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
        element_t mask;

        if (ARRAY_SIZE == 1)
        {
          index = 0;
          mask  = element_t(1) << position;
        }
        else
        {
          index = position >> log2<BITS_PER_ELEMENT>::value;
          mask  = element_t(1) << (position & (BITS_PER_ELEMENT - 1));
        }

        return (data[index] & mask) != 0;
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
    /// Are none of the bits set?
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
    /// Count the number of bits set.
    //*************************************************************************
    size_t count() const
    {
      size_t n = 0;

      for (size_t i = 0; i < ARRAY_SIZE; ++i)
      {
        n += etl::count_bits(data[i]);
      }

      return n;
    }

    //*************************************************************************
    /// The size of the bitset.
    //*************************************************************************
    size_t size() const
    {
      return N;
    }

    //*************************************************************************
    /// Finds the first bit in the specified state.
    ///\param state The state to search for.
    ///\returns The position of the bit or size() if none were found.
    //*************************************************************************
    size_t find_first(bool state) const
    {
      return find_next(state, 0);
    }

    //*************************************************************************
    /// Finds the next bit in the specified state.
    ///\param state    The state to search for.
    ///\param position The position to start from.
    ///\returns The position of the bit or size() if none were found.
    //*************************************************************************
    size_t find_next(bool state, size_t position) const
    {
      // Where to start.
      size_t index;
      size_t bit;

      if (ARRAY_SIZE == 1)
      {
        index = 0;
        bit   = position;
      }
      else
      {
        index = position >> log2<BITS_PER_ELEMENT>::value;
        bit   = position & (BITS_PER_ELEMENT - 1);
      }

      element_t mask = 1 << bit;

      // For each element in the bitset...
      while (index < ARRAY_SIZE)
      {
        element_t value = data[index];

        // Needs checking?
        if (( state && (value != ALL_CLEAR)) ||
            (!state && (value != ALL_SET)))
        {
          // For each bit in the element...
          while ((bit < BITS_PER_ELEMENT) && (position < N))
          {
            // Equal to the required state?
            if (((value & mask) != 0) == state)
            {
              return position;
            }

            // Move on to the next bit.
            mask <<= 1;
            ++position;
            ++bit;
          }
        }
        else
        {
          position += BITS_PER_ELEMENT;
        }

        // Start at the beginning for all other elements.
        bit  = 0;
        mask = 1;

        ++index;
      }

      return ibitset::npos;
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
      data.swap(other.data);
    }

    //*************************************************************************
    /// begin
    //*************************************************************************
    iterator begin()
    {
      return iterator(*this, 0);
    }

    //*************************************************************************
    /// begin
    //*************************************************************************
    const_iterator begin() const
    {
      return const_iterator(*this, 0);
    }

    //*************************************************************************
    /// cbegin
    //*************************************************************************
    const_iterator cbegin()
    {
      return const_iterator(*this, 0);
    }

    //*************************************************************************
    /// end
    //*************************************************************************
    iterator end()
    {
      return iterator(*this, N);
    }

    //*************************************************************************
    /// end
    //*************************************************************************
    const_iterator end() const
    {
      return const_iterator(*this, N);
    }

    //*************************************************************************
    /// cend
    //*************************************************************************
    const_iterator cend()
    {
      return const_iterator(*this, N);
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
       
    etl::array<element_t, ARRAY_SIZE> data;
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

#if WIN32
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#endif
