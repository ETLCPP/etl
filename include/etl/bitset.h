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

#ifndef ETL_BITSET_INCLUDED
#define ETL_BITSET_INCLUDED

#include <string.h>
#include <stddef.h>
#include <stdint.h>

#include "platform.h"

#include "stl/algorithm.h"
#include "stl/iterator.h"

#include "integral_limits.h"
#include "algorithm.h"
#include "nullptr.h"
#include "log.h"
#include "exception.h"
#include "integral_limits.h"
#include "binary.h"

#include "private/minmax_push.h"

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
  //***************************************************************************
  /// Exception base for bitset
  ///\ingroup bitset
  //***************************************************************************
  class bitset_exception : public etl::exception
  {
  public:

    bitset_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Bitset nullptr exception.
  ///\ingroup bitset
  //***************************************************************************
  class bitset_nullptr : public bitset_exception
  {
  public:

    bitset_nullptr(string_type file_name_, numeric_type line_number_)
      : bitset_exception("bitset: nullptr", file_name_, line_number_)
    {
    }
  };

  //*************************************************************************
  /// The base class for etl::bitset
  ///\ingroup bitset
  //*************************************************************************
  class ibitset
  {
  protected:

    // The type used for each element in the array.
#if !defined(ETL_BITSET_ELEMENT_TYPE)
    typedef uint_least8_t element_t;
#else
    typedef ETL_BITSET_ELEMENT_TYPE element_t;
#endif

  public:

    static const element_t ALL_SET = etl::integral_limits<element_t>::max;
    static const element_t ALL_CLEAR = 0;

    static const size_t    BITS_PER_ELEMENT = etl::integral_limits<element_t>::bits;

    enum
    {
      npos = etl::integral_limits<size_t>::max
    };

    //*************************************************************************
    /// The reference type returned.
    //*************************************************************************
    class bit_reference
    {
    public:

      friend class ibitset;

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
      bit_reference(ibitset& r_bitset, size_t position_)
        : p_bitset(&r_bitset),
        position(position_)
      {
      }

      ibitset* p_bitset; ///< The bitset.
      size_t   position; ///< The position in the bitset.
    };

    //*************************************************************************
    /// The size of the bitset.
    //*************************************************************************
    size_t size() const
    {
      return NBITS;
    }

    //*************************************************************************
    /// Count the number of bits set.
    //*************************************************************************
    size_t count() const
    {
      size_t n = 0;

      for (size_t i = 0; i < SIZE; ++i)
      {
        n += etl::count_bits(pdata[i]);
      }

      return n;
    }

    //*************************************************************************
    /// Tests a bit at a position.
    /// Positions greater than the number of configured bits will return <b>false</b>.
    //*************************************************************************
    bool test(size_t position) const
    {
      size_t       index;
      element_t mask;

      if (SIZE == 1)
      {
        index = 0;
        mask = element_t(1) << position;
      }
      else
      {
        index = position >> etl::log2<BITS_PER_ELEMENT>::value;
        mask = element_t(1) << (position & (BITS_PER_ELEMENT - 1));
      }

      return (pdata[index] & mask) != 0;
    }

    //*************************************************************************
    /// Set the bit at the position.
    //*************************************************************************
    ibitset& set()
    {
      for (size_t i = 0; i < SIZE; ++i)
      {
        pdata[i] = ALL_SET;
      }

      pdata[SIZE - 1] &= TOP_MASK;

      return *this;
    }

    //*************************************************************************
    /// Set the bit at the position.
    //*************************************************************************
    ibitset& set(size_t position, bool value = true)
    {
      size_t    index;
      element_t bit;

      if (SIZE == 1)
      {
        index = 0;
        bit = element_t(1) << position;
      }
      else
      {
        index = position >> etl::log2<BITS_PER_ELEMENT>::value;
        bit = element_t(1) << (position & (BITS_PER_ELEMENT - 1));
      }

      if (value)
      {
        pdata[index] |= bit;
      }
      else
      {
        pdata[index] &= ~bit;
      }

      return *this;
    }

    //*************************************************************************
    /// Set from a string.
    //*************************************************************************
    ibitset& set(const char* text)
    {
      reset();

      size_t i = std::min(NBITS, strlen(text));

      while (i > 0)
      {
        set(--i, *text++ == '1');
      }

      return *this;
    }

    //*************************************************************************
    /// Resets the bitset.
    //*************************************************************************
    ibitset& reset()
    {
      for (size_t i = 0; i < SIZE; ++i)
      {
        pdata[i] = ALL_CLEAR;
      }

      return *this;
    }

    //*************************************************************************
    /// Reset the bit at the position.
    //*************************************************************************
    ibitset& reset(size_t position)
    {
      size_t       index;
      element_t bit;

      if (SIZE == 1)
      {
        index = 0;
        bit = element_t(1) << position;
      }
      else
      {
        index = position >> etl::log2<BITS_PER_ELEMENT>::value;
        bit = element_t(1) << (position & (BITS_PER_ELEMENT - 1));
      }

      pdata[index] &= ~bit;

      return *this;
    }

    //*************************************************************************
    /// Flip all of the bits.
    //*************************************************************************
    ibitset& flip()
    {
      for (size_t i = 0; i < SIZE; ++i)
      {
        pdata[i] = ~pdata[i];
      }

      pdata[SIZE - 1] &= TOP_MASK;

      return *this;
    }

    //*************************************************************************
    /// Flip the bit at the position.
    //*************************************************************************
    ibitset& flip(size_t position)
    {
      if (position < NBITS)
      {
        size_t    index;
        element_t bit;

        if (SIZE == 1)
        {
          index = 0;
          bit = element_t(1) << position;
        }
        else
        {
          index = position >> log2<BITS_PER_ELEMENT>::value;
          bit = element_t(1) << (position & (BITS_PER_ELEMENT - 1));
        }

        pdata[index] ^= bit;
      }

      return *this;
    }

    //*************************************************************************
    // Are all the bits sets?
    //*************************************************************************
    bool all() const
    {
      // All but the last.
      for (size_t i = 0; i < (SIZE - 1); ++i)
      {
        if (pdata[i] != ALL_SET)
        {
          return false;
        }
      }

      // The last.
      if (pdata[SIZE - 1] != (ALL_SET & TOP_MASK))
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
      for (size_t i = 0; i < SIZE; ++i)
      {
        if (pdata[i] != 0)
        {
          return false;
        }
      }

      return true;
    }

    //*************************************************************************
    /// Finds the first bit in the specified state.
    ///\param state The state to search for.
    ///\returns The position of the bit or SIZE if none were found.
    //*************************************************************************
    size_t find_first(bool state) const
    {
      return find_next(state, 0);
    }

    //*************************************************************************
    /// Finds the next bit in the specified state.
    ///\param state    The state to search for.
    ///\param position The position to start from.
    ///\returns The position of the bit or SIZE if none were found.
    //*************************************************************************
    size_t find_next(bool state, size_t position) const
    {
      // Where to start.
      size_t index;
      size_t bit;

      if (SIZE == 1)
      {
        index = 0;
        bit = position;
      }
      else
      {
        index = position >> log2<BITS_PER_ELEMENT>::value;
        bit = position & (BITS_PER_ELEMENT - 1);
      }

      element_t mask = 1 << bit;

      // For each element in the bitset...
      while (index < SIZE)
      {
        element_t value = pdata[index];

        // Needs checking?
        if ((state && (value != ALL_CLEAR)) ||
          (!state && (value != ALL_SET)))
        {
          // For each bit in the element...
          while ((bit < BITS_PER_ELEMENT) && (position < NBITS))
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
        bit = 0;
        mask = 1;

        ++index;
      }

      return ibitset::npos;
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
    /// operator &=
    //*************************************************************************
    ibitset& operator &=(const ibitset& other)
    {
      for (size_t i = 0; i < SIZE; ++i)
      {
        pdata[i] &= other.pdata[i];
      }

      return *this;
    }

    //*************************************************************************
    /// operator |=
    //*************************************************************************
    ibitset& operator |=(const ibitset& other)
    {
      for (size_t i = 0; i < SIZE; ++i)
      {
        pdata[i] |= other.pdata[i];
      }

      return *this;
    }

    //*************************************************************************
    /// operator ^=
    //*************************************************************************
    ibitset& operator ^=(const ibitset& other)
    {
      for (size_t i = 0; i < SIZE; ++i)
      {
        pdata[i] ^= other.pdata[i];
      }

      return *this;
    }

    //*************************************************************************
    /// operator <<=
    //*************************************************************************
    ibitset& operator<<=(size_t shift)
    {
      if (SIZE == 1)
      {
        pdata[0] <<= shift;
      }
      else
      {
        size_t source = NBITS - shift - 1;
        size_t destination = NBITS - 1;

        for (size_t i = 0; i < (NBITS - shift); ++i)
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
    /// operator >>=
    //*************************************************************************
    ibitset& operator>>=(size_t shift)
    {
      if (SIZE == 1)
      {
        pdata[0] >>= shift;
      }
      else
      {
        size_t source = shift;
        size_t destination = 0;

        for (size_t i = 0; i < (NBITS - shift); ++i)
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
    /// operator =
    //*************************************************************************
    ibitset& operator =(const ibitset& other)
    {
      if (this != &other)
      {
        etl::copy_n(other.pdata, SIZE, pdata);
      }

      return *this;
    }

    //*************************************************************************
    /// swap
    //*************************************************************************
    void swap(ibitset& other)
    {
      std::swap_ranges(pdata, pdata + SIZE, other.pdata);
    }

  protected:

    //*************************************************************************
    /// Initialise from an unsigned long long.
    //*************************************************************************
    ibitset& initialise(unsigned long long value)
    {
      reset();

      const size_t SHIFT = (integral_limits<unsigned long long>::bits <= (int)BITS_PER_ELEMENT) ? 0 : BITS_PER_ELEMENT;

      // Can we do it in one hit?
      if (SHIFT == 0)
      {
        pdata[0] = element_t(value);
      }
      else
      {
        size_t i = 0;

        while ((value != 0) && (i < SIZE))
        {
          pdata[i++] = value & ALL_SET;
          value = value >> SHIFT;
        }
      }

      pdata[SIZE - 1] &= TOP_MASK;

      return *this;
    }

    //*************************************************************************
    /// Invert
    //*************************************************************************
    void invert()
    {
      for (size_t i = 0; i < SIZE; ++i)
      {
        pdata[i] = ~pdata[i];
      }
    }

    //*************************************************************************
    /// Gets a reference to the specified bit.
    //*************************************************************************
    bit_reference get_bit_reference(size_t position)
    {
      return bit_reference(*this, position);
    }

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    ibitset(size_t nbits_, size_t size_, element_t* pdata_)
      : NBITS(nbits_),
        SIZE(size_),
        pdata(pdata_)
    {
      size_t allocated_bits = SIZE * BITS_PER_ELEMENT;
      size_t top_mask_shift = ((BITS_PER_ELEMENT - (allocated_bits - NBITS)) % BITS_PER_ELEMENT);
      TOP_MASK = element_t(top_mask_shift == 0 ? ALL_SET : ~(ALL_SET << top_mask_shift));
    }

    //*************************************************************************
    /// Compare bitsets.
    //*************************************************************************
    static bool is_equal(const ibitset& lhs, const ibitset&rhs)
    {
      return std::equal(lhs.pdata, lhs.pdata + lhs.SIZE, rhs.pdata);
    }

    element_t TOP_MASK;

  private:

    // Disable copy construction.
    ibitset(const ibitset&);

    const size_t NBITS;
    const size_t SIZE;
    element_t*   pdata;

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
#if defined(ETL_POLYMORPHIC_BITSET) || defined(ETL_POLYMORPHIC_CONTAINERS)
  public:
    virtual ~ibitset()
    {
    }
#else
  protected:
    ~ibitset()
    {
    }
#endif
  };

  //*************************************************************************
  /// The class emulates an array of bool elements, but optimized for space allocation.
  /// Will accommodate any number of bits.
  /// Does not use std::string.
  ///\tparam N The number of bits.
  ///\ingroup bitset
  //*************************************************************************
  template <const size_t MAXN>
  class bitset : public etl::ibitset
  {

    static const size_t ARRAY_SIZE = (MAXN % BITS_PER_ELEMENT == 0) ? MAXN / BITS_PER_ELEMENT : MAXN / BITS_PER_ELEMENT + 1;

  public:

    static const size_t ALLOCATED_BITS = ARRAY_SIZE * BITS_PER_ELEMENT;

  public:

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    bitset()
      : etl::ibitset(MAXN, ARRAY_SIZE, data)
    {
      reset();
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    bitset(const bitset<MAXN>& other)
      : etl::ibitset(MAXN, ARRAY_SIZE, data)
    {
      etl::copy_n(other.data, ARRAY_SIZE, data);
    }

    //*************************************************************************
    /// Construct from a value.
    //*************************************************************************
    bitset(unsigned long long value)
      : etl::ibitset(MAXN, ARRAY_SIZE, data)
    {
      initialise(value);
    }

    //*************************************************************************
    /// Construct from a string.
    //*************************************************************************
    bitset(const char* text)
      : etl::ibitset(MAXN, ARRAY_SIZE, data)
    {
      set(text);
    }

    //*************************************************************************
    /// Set all of the bits.
    //*************************************************************************
    bitset<MAXN>& set()
    {
      etl::ibitset::set();
      return *this;
    }

    //*************************************************************************
    /// Set the bit at the position.
    //*************************************************************************
    bitset<MAXN>& set(size_t position, bool value = true)
    {
      etl::ibitset::set(position, value);
      return *this;
    }

    //*************************************************************************
    /// Set from a string.
    //*************************************************************************
    bitset<MAXN>& set(const char* text)
    {
      ETL_ASSERT(text != 0, ETL_ERROR(bitset_nullptr));
      etl::ibitset::set(text);

      return *this;
    }

    //*************************************************************************
    /// Reset all of the bits.
    //*************************************************************************
    bitset<MAXN>& reset()
    {
      ibitset::reset();
      return *this;
    }

    //*************************************************************************
    /// Reset the bit at the position.
    //*************************************************************************
    bitset<MAXN>& reset(size_t position)
    {
      etl::ibitset::reset(position);
      return *this;
    }

    //*************************************************************************
    /// Flip all of the bits.
    //*************************************************************************
    bitset<MAXN>& flip()
    {
      ibitset::flip();
      return *this;
    }

    //*************************************************************************
    /// Flip the bit at the position.
    //*************************************************************************
    bitset<MAXN>& flip(size_t position)
    {
      etl::ibitset::flip(position);
      return *this;
    }

    //*************************************************************************
    /// operator =
    //*************************************************************************
    bitset<MAXN>& operator =(const bitset<MAXN>& other)
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
    bitset<MAXN>& operator &=(const bitset<MAXN>& other)
    {
      etl::ibitset::operator &=(other);
      return *this;
    }

    //*************************************************************************
    /// operator |=
    //*************************************************************************
    bitset<MAXN>& operator |=(const bitset<MAXN>& other)
    {
      etl::ibitset::operator |=(other);
      return *this;
    }

    //*************************************************************************
    /// operator ^=
    //*************************************************************************
    bitset<MAXN>& operator ^=(const bitset<MAXN>& other)
    {
      ibitset::operator ^=(other);
      return *this;
    }

    //*************************************************************************
    /// operator ~
    //*************************************************************************
    bitset<MAXN> operator ~() const
    {
      etl::bitset<MAXN> temp(*this);

      temp.invert();

      return temp;
    }

    //*************************************************************************
    /// operator <<
    //*************************************************************************
    bitset<MAXN> operator<<(size_t shift) const
    {
      etl::bitset<MAXN> temp(*this);

      temp <<= shift;

      return temp;
    }

    //*************************************************************************
    /// operator <<=
    //*************************************************************************
    bitset<MAXN>& operator<<=(size_t shift)
    {
      etl::ibitset::operator <<=(shift);
      return *this;
    }

    //*************************************************************************
    /// operator >>
    //*************************************************************************
    bitset<MAXN> operator>>(size_t shift) const
    {
      bitset<MAXN> temp(*this);

      temp >>= shift;

      return temp;
    }

    //*************************************************************************
    /// operator >>=
    //*************************************************************************
    bitset<MAXN>& operator>>=(size_t shift)
    {
      etl::ibitset::operator >>=(shift);
      return *this;
    }

    //*************************************************************************
    /// operator ==
    //*************************************************************************
    friend bool operator == (const bitset<MAXN>& lhs, const bitset<MAXN>& rhs)
    {
      return etl::ibitset::is_equal(lhs, rhs);
    }

  private:

    element_t data[ARRAY_SIZE];
  };

  //***************************************************************************
  /// operator &
  ///\ingroup bitset
  //***************************************************************************
  template <const size_t MAXN>
  bitset<MAXN> operator & (const bitset<MAXN>& lhs, const bitset<MAXN>& rhs)
  {
    bitset<MAXN> temp(lhs);
    temp &= rhs;
    return temp;
  }

  //***************************************************************************
  /// operator |
  ///\ingroup bitset
  //***************************************************************************
  template<const size_t MAXN>
  bitset<MAXN> operator | (const bitset<MAXN>& lhs, const bitset<MAXN>& rhs)
  {
    bitset<MAXN> temp(lhs);
    temp |= rhs;
    return temp;
  }

  //***************************************************************************
  /// operator ^
  ///\ingroup bitset
  //***************************************************************************
  template<const size_t MAXN>
  bitset<MAXN> operator ^ (const bitset<MAXN>& lhs, const bitset<MAXN>& rhs)
  {
    bitset<MAXN> temp(lhs);
    temp ^= rhs;
    return temp;
  }

  //***************************************************************************
  /// operator !=
  ///\ingroup bitset
  //***************************************************************************
  template<const size_t MAXN>
  bool operator != (const bitset<MAXN>& lhs, const bitset<MAXN>& rhs)
  {
    return !(lhs == rhs);
  }
}

//*************************************************************************
/// swap
//*************************************************************************
template <const size_t MAXN>
void swap(etl::bitset<MAXN>& lhs, etl::bitset<MAXN>& rhs)
{
  lhs.swap(rhs);
}

#include "private/minmax_pop.h"

#endif
