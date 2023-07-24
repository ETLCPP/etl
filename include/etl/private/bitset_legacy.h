///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 John Wellbelove

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

#ifndef ETL_BITSET_LEGACY_INCLUDED
#define ETL_BITSET_LEGACY_INCLUDED

#include "../platform.h"
#include "../algorithm.h"
#include "../iterator.h"
#include "../integral_limits.h"
#include "../algorithm.h"
#include "../nullptr.h"
#include "../log.h"
#include "../exception.h"
#include "../integral_limits.h"
#include "../binary.h"
#include "../char_traits.h"
#include "../static_assert.h"
#include "../error_handler.h"
#include "../span.h"
#include "../string.h"

#include <string.h>
#include <stddef.h>
#include <stdint.h>

#include "minmax_push.h"

#if defined(ETL_COMPILER_KEIL)
#pragma diag_suppress 1300
#endif

#if ETL_USING_CPP11
  #define ETL_STR(x)  x
  #define ETL_STRL(x) L##x
  #define ETL_STRu(x) u##x
  #define ETL_STRU(x) U##x
#else
  #define ETL_STR(x)  x
  #define ETL_STRL(x) x
  #define ETL_STRu(x) x
  #define ETL_STRU(x) x
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
  /// Bitset null pointer exception.
  ///\ingroup bitset
  //***************************************************************************
  class bitset_nullptr : public bitset_exception
  {
  public:

    bitset_nullptr(string_type file_name_, numeric_type line_number_)
      : bitset_exception(ETL_ERROR_TEXT("bitset:null pointer", ETL_BITSET_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Bitset type_too_small exception.
  ///\ingroup bitset
  //***************************************************************************
  class bitset_type_too_small : public bitset_exception
  {
  public:

    bitset_type_too_small(string_type file_name_, numeric_type line_number_)
      : bitset_exception(ETL_ERROR_TEXT("bitset:type_too_small", ETL_BITSET_FILE_ID"B"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Bitset overflow exception.
  ///\ingroup bitset
  //***************************************************************************
  class bitset_overflow : public bitset_exception
  {
  public:

    bitset_overflow(string_type file_name_, numeric_type line_number_)
      : bitset_exception(ETL_ERROR_TEXT("bitset:overflow", ETL_BITSET_FILE_ID"C"), file_name_, line_number_)
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
    #define ETL_BITSET_ELEMENT_TYPE uint_least8_t
#endif

  public:

    typedef typename etl::make_unsigned<ETL_BITSET_ELEMENT_TYPE>::type element_type;
    typedef element_type element_t; // Backward compatibility

    static ETL_CONSTANT element_type ALL_SET = etl::integral_limits<element_type>::max;
    static ETL_CONSTANT element_type ALL_CLEAR = 0;

    static ETL_CONSTANT size_t    Bits_Per_Element = etl::integral_limits<element_type>::bits;

#if ETL_USING_CPP11
    typedef etl::span<element_type>       span_type;
    typedef etl::span<const element_type> const_span_type;
#endif

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
      /// Copy constructor.
      //*******************************
      bit_reference(const bit_reference& other)
        : p_bitset(other.p_bitset)
        , position(other.position)
      {
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
        : p_bitset(ETL_NULLPTR)
        , position(0)
      {
      }

      //*******************************
      /// Constructor.
      //*******************************
      bit_reference(ibitset& r_bitset, size_t position_)
        : p_bitset(&r_bitset)
        , position(position_)
      {
      }

      ibitset* p_bitset; ///< The bitset.
      size_t   position; ///< The position in the bitset.
    };

    //*************************************************************************
    /// The number of bits in the bitset.
    //*************************************************************************
    size_t size() const
    {
      return Active_Bits;
    }

    //*************************************************************************
    /// Count the number of bits set.
    //*************************************************************************
    size_t count() const
    {
      size_t n = 0UL;

      for (size_t i = 0UL; i < Number_Of_Elements; ++i)
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
      ETL_ASSERT_OR_RETURN_VALUE(position < Active_Bits, ETL_ERROR(bitset_overflow), false);
      size_t    index;
      element_type mask;

      if (position >= Active_Bits)
      {
        return false;
      }
      if (Number_Of_Elements == 0)
      {
        return false;
      }
      else if (Number_Of_Elements == 1)
      {
        index = 0;
        mask = element_type(1) << position;
      }
      else
      {
        index = position >> etl::log2<Bits_Per_Element>::value;
        mask = element_type(1) << (position & (Bits_Per_Element - 1));
      }

      return (pdata[index] & mask) != 0;
    }

    //*************************************************************************
    /// Set all bits.
    //*************************************************************************
    ibitset& set()
    {
      ::memset(pdata, 0xFF, Number_Of_Elements);
      pdata[Number_Of_Elements - 1U] = Top_Mask;

      return *this;
    }

    //*************************************************************************
    /// Set the bit at the position.
    //*************************************************************************
    ibitset& set(size_t position, bool value = true)
    {
      ETL_ASSERT_OR_RETURN_VALUE(position < Active_Bits, ETL_ERROR(bitset_overflow), *this);
      size_t    index;
      element_type bit;

      if (position < Active_Bits)
      {
        if (Number_Of_Elements == 0) 
        {
          return *this;
        }
        else if (Number_Of_Elements == 1)
        {
          index = 0;
          bit = element_type(1) << position;
        }
        else
        {
          index = position >> etl::log2<Bits_Per_Element>::value;
          bit = element_type(1) << (position & (Bits_Per_Element - 1));
        }

        if (value)
        {
          pdata[index] |= bit;
        }
        else
        {
          pdata[index] &= ~bit;
        }
      }

      return *this;
    }

    //*************************************************************************
    /// Set from a string.
    //*************************************************************************
    ibitset& from_string(const char* text)
    {
      reset();

      size_t i = etl::min(Active_Bits, etl::strlen(text));

      while (i > 0)
      {
        set(--i, *text++ == ETL_STRL('1'));
      }

      return *this;
    }

    //*************************************************************************
    /// Set from a wide string.
    //*************************************************************************
    ibitset& from_string(const wchar_t* text)
    {
      reset();

      size_t i = etl::min(Active_Bits, etl::strlen(text));

      while (i > 0)
      {
        set(--i, *text++ == ETL_STRL('1'));
      }

      return *this;
    }

    //*************************************************************************
    /// Set from a u16 string.
    //*************************************************************************
    ibitset& from_string(const char16_t* text)
    {
      reset();

      size_t i = etl::min(Active_Bits, etl::strlen(text));

      while (i > 0)
      {
        set(--i, *text++ == ETL_STRu('1'));
      }

      return *this;
    }

    //*************************************************************************
    /// Set from a u32 string.
    //*************************************************************************
    ibitset& from_string(const char32_t* text)
    {
      reset();

      size_t i = etl::min(Active_Bits, etl::strlen(text));

      while (i > 0)
      {
        set(--i, *text++ == ETL_STRU('1'));
      }

      return *this;
    }

    //*************************************************************************
    /// Set from a string.
    //*************************************************************************
    ibitset& set(const char* text)
    {
      from_string(text);

      return *this;
    }

    //*************************************************************************
    /// Set from a wstring.
    //*************************************************************************
    ibitset& set(const wchar_t* text)
    {
      from_string(text);

      return *this;
    }

    //*************************************************************************
    /// Set from a u16string.
    //*************************************************************************
    ibitset& set(const char16_t* text)
    {
      from_string(text);

      return *this;
    }

    //*************************************************************************
    /// Set from a u32string.
    //*************************************************************************
    ibitset& set(const char32_t* text)
    {
      from_string(text);

      return *this;
    }

    //*************************************************************************
    /// Put to a value.
    //*************************************************************************
    template <typename T>
    typename etl::enable_if<etl::is_integral<T>::value, T>::type
      value() const
    {
      T v = T(0);

      const bool OK = (sizeof(T) * CHAR_BIT) >= (Number_Of_Elements * Bits_Per_Element);

      ETL_ASSERT_OR_RETURN_VALUE(OK, ETL_ERROR(etl::bitset_type_too_small), T(0));

      if (OK)
      {
        uint_least8_t shift = 0U;

        for (size_t i = 0UL; i < Number_Of_Elements; ++i)
        {
          v |= T(typename etl::make_unsigned<T>::type(pdata[i]) << shift);
          shift += uint_least8_t(Bits_Per_Element);
        }
      }

      return v;
    }

    //*************************************************************************
    /// Put to a unsigned long.
    //*************************************************************************
    unsigned long to_ulong() const
    {
      return value<unsigned long>();
    }

    //*************************************************************************
    /// Put to a unsigned long long.
    //*************************************************************************
    unsigned long long to_ullong() const
    {
      return value<unsigned long long>();
    }

    //*************************************************************************
    /// Resets the bitset.
    //*************************************************************************
    ibitset& reset()
    {
      ::memset(pdata, 0x00, Number_Of_Elements);

      return *this;
    }

    //*************************************************************************
    /// Reset the bit at the position.
    //*************************************************************************
    ibitset& reset(size_t position)
    {
      ETL_ASSERT_OR_RETURN_VALUE(position < Active_Bits, ETL_ERROR(bitset_overflow), *this);
      size_t       index;
      element_type bit;

      if (position < Active_Bits)
      {
        if (Number_Of_Elements == 0)
        {
          return *this;
        }
        else if (Number_Of_Elements == 1)
        {
          index = 0;
          bit = element_type(1) << position;
        }
        else
        {
          index = position >> etl::log2<Bits_Per_Element>::value;
          bit = element_type(1) << (position & (Bits_Per_Element - 1));
        }

        pdata[index] &= ~bit;
      }

      return *this;
    }

    //*************************************************************************
    /// Flip all of the bits.
    //*************************************************************************
    ibitset& flip()
    {
      for (size_t i = 0UL; i < Number_Of_Elements; ++i)
      {
        pdata[i] = ~pdata[i];
      }

      clear_unused_bits_in_msb();

      return *this;
    }

    //*************************************************************************
    /// Flip the bit at the position.
    //*************************************************************************
    ibitset& flip(size_t position)
    {
      ETL_ASSERT_OR_RETURN_VALUE(position < Active_Bits, ETL_ERROR(bitset_overflow), *this);
      size_t    index;
      element_type bit;
      
      if (Number_Of_Elements == 0)
      {
        return *this;
      }
      else if (Number_Of_Elements == 1)
      {
        index = 0;
        bit = element_type(1) << position;
      }
      else
      {
        index = position >> log2<Bits_Per_Element>::value;
        bit = element_type(1) << (position & (Bits_Per_Element - 1));
      }

      pdata[index] ^= bit;

      return *this;
    }

    //*************************************************************************
    // Are all the bits sets?
    //*************************************************************************
    bool all() const
    {
      if (Number_Of_Elements == 0UL)
      {
        return true;
      }

      // All but the last.
      for (size_t i = 0UL; i < (Number_Of_Elements - 1U); ++i)
      {
        if (pdata[i] != ALL_SET)
        {
          return false;
        }
      }

      // The last.
      if (pdata[Number_Of_Elements - 1U] != (ALL_SET & Top_Mask))
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
      for (size_t i = 0UL; i < Number_Of_Elements; ++i)
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
    ///\returns The position of the bit or Number_Of_Elements if none were found.
    //*************************************************************************
    size_t find_first(bool state) const
    {
      return find_next(state, 0);
    }

    //*************************************************************************
    /// Finds the next bit in the specified state.
    ///\param state    The state to search for.
    ///\param position The position to start from.
    ///\returns The position of the bit or ibitset::npos if none were found.
    //*************************************************************************
    size_t find_next(bool state, size_t position) const
    {
      // Where to start.
      size_t index;
      size_t bit;

      if (Number_Of_Elements == 0)
      {
        return ibitset::npos;
      }
      else if (Number_Of_Elements == 1)
      {
        index = 0;
        bit = position;
      }
      else
      {
        index = position >> log2<Bits_Per_Element>::value;
        bit = position & (Bits_Per_Element - 1);
      }

      element_type mask = 1 << bit;

      // For each element in the bitset...
      while (index < Number_Of_Elements)
      {
        element_type value = pdata[index];

        // Needs checking?
        if ((state && (value != ALL_CLEAR)) ||
            (!state && (value != ALL_SET)))
        {
          // For each bit in the element...
          while ((bit < Bits_Per_Element) && (position < Active_Bits))
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
          position += (Bits_Per_Element - bit);
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
      for (size_t i = 0UL; i < Number_Of_Elements; ++i)
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
      for (size_t i = 0UL; i < Number_Of_Elements; ++i)
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
      for (size_t i = 0UL; i < Number_Of_Elements; ++i)
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
      if (shift >= Active_Bits)
      {
        reset();
      }
      else if (Number_Of_Elements != 0UL)
      {
        // Just one element?
        if (Number_Of_Elements == 1UL)
        {
          pdata[0] <<= shift;
        }
        else if (shift == Bits_Per_Element)
        {
          etl::copy_backward(pdata, pdata + Number_Of_Elements - 1U, pdata + Number_Of_Elements);
          pdata[0] = 0;
        }
        else
        {
          // The place where the elements are split when shifting.
          const size_t split_position = Bits_Per_Element - (shift % Bits_Per_Element);

          // Where we are shifting from.
          int src_index = int(Number_Of_Elements - (shift / Bits_Per_Element) - 1U);

          // Where we are shifting to.
          int dst_index = int(Number_Of_Elements - 1U);

          // Shift control constants.
          const size_t lsb_shift = Bits_Per_Element - split_position;
          const size_t msb_shift = split_position;

          const element_type lsb_mask         = element_type(etl::integral_limits<element_type>::max >> (Bits_Per_Element - split_position));
          const element_type msb_mask         = etl::integral_limits<element_type>::max - lsb_mask;
          const element_type lsb_shifted_mask = element_type(lsb_mask << lsb_shift);
          
          // First lsb.
          element_type lsb = element_type((pdata[src_index] & lsb_mask) << lsb_shift);
          pdata[dst_index] = lsb;
          --src_index;

          // Now do the shifting.
          while (src_index >= 0)
          {
            // Shift msb.
            element_type msb = element_type((pdata[src_index] & msb_mask) >> msb_shift);
            pdata[dst_index] = pdata[dst_index] | msb;
            --dst_index;

            // Shift lsb.
            element_type lsb = element_type((pdata[src_index] & lsb_mask) << lsb_shift);
            pdata[dst_index] = lsb;
            --src_index;
          }

          // Clear the remaining bits.
          // First lsb.
          pdata[dst_index] &= lsb_shifted_mask;
          --dst_index;

          // The other remaining elements.
          for (int i = 0; i <= dst_index; ++i)
          {
            pdata[i] = 0;
          }
        }

        // Truncate any bits shifted to the left.
        clear_unused_bits_in_msb();
      }

      return *this;
    }

    //*************************************************************************
    /// operator >>=
    //*************************************************************************
    ibitset& operator>>=(size_t shift)
    {
      if (shift >= Active_Bits)
      {
        reset();
      }
      else if (Number_Of_Elements != 0UL)
      {
        // Just one element?
        if (Number_Of_Elements == 1UL)
        {
          pdata[0] >>= shift;
        }
        // Shift is the size of an element?
        else if (shift == Bits_Per_Element)
        {
          etl::copy(pdata + 1, pdata + Number_Of_Elements, pdata);
          pdata[Number_Of_Elements - 1U] = 0;
        }
        else
        {
          // The place where the elements are split when shifting.
          const size_t split_position = shift % Bits_Per_Element;

          // Where we are shifting from.
          int src_index = int(shift / Bits_Per_Element);

          // Where we are shifting to.
          int dst_index = 0;

          // Shift control constants.
          const size_t lsb_shift = Bits_Per_Element - split_position;
          const size_t msb_shift = split_position;

          const element_type lsb_mask         = element_type(etl::integral_limits<element_type>::max >> (Bits_Per_Element - split_position));
          const element_type msb_mask         = etl::integral_limits<element_type>::max - lsb_mask;
          const element_type msb_shifted_mask = element_type(msb_mask >> msb_shift);

          // Now do the shifting.
          while (src_index < int(Number_Of_Elements - 1))
          {
            // Shift msb.
            element_type msb = element_type((pdata[src_index] & msb_mask) >> msb_shift);
            ++src_index;
            
            // Shift lsb.
            element_type lsb = element_type((pdata[src_index] & lsb_mask) << lsb_shift);
            
            // Combine them.
            pdata[dst_index] = lsb | msb;
            ++dst_index;
          }

          // Final msb.
          element_type msb = element_type((pdata[src_index] & msb_mask) >> msb_shift);
          pdata[dst_index] = msb;

          // Clear the remaining bits.
          // First msb.
          pdata[dst_index] &= msb_shifted_mask;
          ++dst_index;

          // The other remaining elements.
          for (int i = dst_index; i < int(Number_Of_Elements); ++i)
          {
            pdata[i] = 0;
          }
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
        etl::copy_n(other.pdata, Number_Of_Elements, pdata);
      }

      return *this;
    }

    //*************************************************************************
    /// swap
    //*************************************************************************
    void swap(ibitset& other)
    {
      etl::swap_ranges(pdata, pdata + Number_Of_Elements, other.pdata);
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// span
    /// Returns a span of the underlying data.
    //*************************************************************************
    span_type span()
    {
      return span_type(pdata, pdata + Number_Of_Elements);
    }

    //*************************************************************************
    /// span
    /// Returns a const span of the underlying data.
    //*************************************************************************
    const_span_type span() const
    {
      return const_span_type(pdata, pdata + Number_Of_Elements);
    }
#endif

  protected:

    //*************************************************************************
    /// Initialise from an unsigned long long.
    //*************************************************************************
    ibitset& initialise(unsigned long long value)
    {
      reset();

      const size_t Shift = (integral_limits<unsigned long long>::bits <= (int)Bits_Per_Element) ? 0 : Bits_Per_Element;

      // Can we do it in one hit?
      if (Shift == 0)
      {
        pdata[0] = element_type(value);
      }
      else
      {
        size_t i = 0UL;

        while ((value != 0) && (i < Number_Of_Elements))
        {
          pdata[i++] = value & ALL_SET;
          value = value >> Shift;
        }
      }

      clear_unused_bits_in_msb();

      return *this;
    }

    //*************************************************************************
    /// Invert
    //*************************************************************************
    void invert()
    {
      for (size_t i = 0UL; i < Number_Of_Elements; ++i)
      {
        pdata[i] = ~pdata[i];
      }

      clear_unused_bits_in_msb();
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
    ibitset(size_t nbits_, size_t size_, element_type* pdata_)
      : Active_Bits(nbits_)
      , Number_Of_Elements(size_)
      , pdata(pdata_)
    {
      const size_t allocated_bits = Number_Of_Elements * Bits_Per_Element;
      const size_t top_mask_shift = ((Bits_Per_Element - (allocated_bits - Active_Bits)) % Bits_Per_Element);
      Top_Mask = element_type(top_mask_shift == 0 ? ALL_SET : ~(ALL_SET << top_mask_shift));
    }

    //*************************************************************************
    /// Compare bitsets.
    //*************************************************************************
    static bool is_equal(const ibitset& lhs, const ibitset&rhs)
    {
      return etl::equal(lhs.pdata, lhs.pdata + lhs.Number_Of_Elements, rhs.pdata);
    }

    element_type Top_Mask;

  private:

    //*************************************************************************
    /// Correct the unused top bits after bit manipulation.
    //*************************************************************************
    void clear_unused_bits_in_msb()
    {
      pdata[Number_Of_Elements - 1U] &= Top_Mask;
    }

    // Disable copy construction.
    ibitset(const ibitset&);

    const size_t  Active_Bits;
    const size_t  Number_Of_Elements;
    element_type* pdata;

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

  ETL_CONSTANT ibitset::element_type ibitset::ALL_SET;

  ETL_CONSTANT ibitset::element_type ibitset::ALL_CLEAR;

  ETL_CONSTANT size_t ibitset::Bits_Per_Element;

  //*************************************************************************
  /// The class emulates an array of bool elements, but optimized for space allocation.
  /// Will accommodate any number of bits.
  /// Does not use std::string.
  ///\tparam MaxN The number of bits.
  ///\ingroup bitset
  //*************************************************************************
  template <size_t MaxN>
  class bitset : public etl::ibitset
  {

    static ETL_CONSTANT size_t Array_Size = (MaxN % Bits_Per_Element == 0) ? MaxN / Bits_Per_Element : MaxN / Bits_Per_Element + 1;

  public:

    static ETL_CONSTANT size_t ALLOCATED_BITS = Array_Size * Bits_Per_Element;
    static ETL_CONSTANT size_t Allocated_Bits = ALLOCATED_BITS;

  public:

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    bitset()
      : etl::ibitset(MaxN, Array_Size, data)
    {
      reset();
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    bitset(const bitset<MaxN>& other)
      : etl::ibitset(MaxN, Array_Size, data)
    {
      etl::copy_n(other.data, Array_Size, data);
    }

    //*************************************************************************
    /// Construct from a value.
    //*************************************************************************
    bitset(unsigned long long value)
      : etl::ibitset(MaxN, Array_Size, data)
    {
      initialise(value);
    }

    //*************************************************************************
    /// Construct from a string.
    //*************************************************************************
    bitset(const char* text)
      : etl::ibitset(MaxN, Array_Size, data)
    {
      set(text);
    }

    //*************************************************************************
    /// Construct from a string.
    //*************************************************************************
    bitset(const wchar_t* text)
      : etl::ibitset(MaxN, Array_Size, data)
    {
      set(text);
    }

    //*************************************************************************
    /// Construct from a string.
    //*************************************************************************
    bitset(const char16_t* text)
      : etl::ibitset(MaxN, Array_Size, data)
    {
      set(text);
    }

    //*************************************************************************
    /// Construct from a string.
    //*************************************************************************
    bitset(const char32_t* text)
      : etl::ibitset(MaxN, Array_Size, data)
    {
      set(text);
    }

    //*************************************************************************
    /// Set all of the bits.
    //*************************************************************************
    bitset<MaxN>& set()
    {
      etl::ibitset::set();
      return *this;
    }

    //*************************************************************************
    /// Set the bit at the position.
    //*************************************************************************
    bitset<MaxN>& set(size_t position, bool value = true)
    {
      etl::ibitset::set(position, value);
      return *this;
    }

    //*************************************************************************
    /// Set from a string.
    //*************************************************************************
    bitset<MaxN>& set(const char* text)
    {
      ETL_ASSERT_OR_RETURN_VALUE(text != 0, ETL_ERROR(bitset_nullptr), *this);
      etl::ibitset::set(text);

      return *this;
    }

    //*************************************************************************
    /// Set from a string.
    //*************************************************************************
    bitset<MaxN>& set(const wchar_t* text)
    {
      ETL_ASSERT_OR_RETURN_VALUE(text != 0, ETL_ERROR(bitset_nullptr), *this);
      etl::ibitset::set(text);

      return *this;
    }

    //*************************************************************************
    /// Set from a string.
    //*************************************************************************
    bitset<MaxN>& set(const char16_t* text)
    {
      ETL_ASSERT_OR_RETURN_VALUE(text != 0, ETL_ERROR(bitset_nullptr), *this);
      etl::ibitset::set(text);

      return *this;
    }

    //*************************************************************************
    /// Set from a string.
    //*************************************************************************
    bitset<MaxN>& set(const char32_t* text)
    {
      ETL_ASSERT_OR_RETURN_VALUE(text != 0, ETL_ERROR(bitset_nullptr), *this);
      etl::ibitset::set(text);

      return *this;
    }

    //*************************************************************************
    /// Set from a string.
    //*************************************************************************
    bitset<MaxN>& from_string(const char* text)
    {
      ibitset::from_string(text);

      return *this;
    }

    //*************************************************************************
    /// Set from a wide string.
    //*************************************************************************
    bitset<MaxN>& from_string(const wchar_t* text)
    {
      ibitset::from_string(text);

      return *this;
    }

    //*************************************************************************
    /// Set from a u16 string.
    //*************************************************************************
    bitset<MaxN>& from_string(const char16_t* text)
    {
      ibitset::from_string(text);

      return *this;
    }

    //*************************************************************************
    /// Set from a u32 string.
    //*************************************************************************
    bitset<MaxN>& from_string(const char32_t* text)
    {
      ibitset::from_string(text);

      return *this;
    }

    //*************************************************************************
    /// Put to a value.
    //*************************************************************************
    template <typename T>
    typename etl::enable_if<etl::is_integral<T>::value, T>::type
      value() const
    {
      ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Only integral types are supported");
      ETL_STATIC_ASSERT((sizeof(T) * CHAR_BIT) >= (Array_Size * Bits_Per_Element), "Type too small");

      return ibitset::value<T>();
    }

    //*************************************************************************
    /// Reset all of the bits.
    //*************************************************************************
    bitset<MaxN>& reset()
    {
      ibitset::reset();
      return *this;
    }

    //*************************************************************************
    /// Reset the bit at the position.
    //*************************************************************************
    bitset<MaxN>& reset(size_t position)
    {
      etl::ibitset::reset(position);
      return *this;
    }

    //*************************************************************************
    /// Flip all of the bits.
    //*************************************************************************
    bitset<MaxN>& flip()
    {
      ibitset::flip();
      return *this;
    }

    //*************************************************************************
    /// Flip the bit at the position.
    //*************************************************************************
    bitset<MaxN>& flip(size_t position)
    {
      etl::ibitset::flip(position);
      return *this;
    }

    //*************************************************************************
    /// Returns a string representing the bitset.
    //*************************************************************************
#if ETL_USING_CPP11
    template <typename TString = etl::string<MaxN>>
#else
    template <typename TString>
#endif
    TString to_string(typename TString::value_type zero = typename TString::value_type('0'), typename TString::value_type one = typename TString::value_type('1')) const
    {
      TString result;

      result.resize(MaxN, '\0');

      ETL_ASSERT_OR_RETURN_VALUE((result.size() == MaxN), ETL_ERROR(etl::bitset_overflow), result);

      for (size_t i = MaxN; i > 0; --i) 
      {
        result[MaxN - i] = test(i - 1) ? one : zero;
      }
      
      return result;
    }

    //*************************************************************************
    /// operator =
    //*************************************************************************
    bitset<MaxN>& operator =(const bitset<MaxN>& other)
    {
      if (this != &other)
      {
        etl::copy_n(other.data, Array_Size, data);
      }

      return *this;
    }

    //*************************************************************************
    /// operator &=
    //*************************************************************************
    bitset<MaxN>& operator &=(const bitset<MaxN>& other)
    {
      etl::ibitset::operator &=(other);
      return *this;
    }

    //*************************************************************************
    /// operator |=
    //*************************************************************************
    bitset<MaxN>& operator |=(const bitset<MaxN>& other)
    {
      etl::ibitset::operator |=(other);
      return *this;
    }

    //*************************************************************************
    /// operator ^=
    //*************************************************************************
    bitset<MaxN>& operator ^=(const bitset<MaxN>& other)
    {
      ibitset::operator ^=(other);
      return *this;
    }

    //*************************************************************************
    /// operator ~
    //*************************************************************************
    bitset<MaxN> operator ~() const
    {
      etl::bitset<MaxN> temp(*this);

      temp.invert();

      return temp;
    }

    //*************************************************************************
    /// operator <<
    //*************************************************************************
    bitset<MaxN> operator<<(size_t shift) const
    {
      etl::bitset<MaxN> temp(*this);

      temp <<= shift;

      return temp;
    }

    //*************************************************************************
    /// operator <<=
    //*************************************************************************
    bitset<MaxN>& operator<<=(size_t shift)
    {
      etl::ibitset::operator <<=(shift);
      return *this;
    }

    //*************************************************************************
    /// operator >>
    //*************************************************************************
    bitset<MaxN> operator>>(size_t shift) const
    {
      bitset<MaxN> temp(*this);

      temp >>= shift;

      return temp;
    }

    //*************************************************************************
    /// operator >>=
    //*************************************************************************
    bitset<MaxN>& operator>>=(size_t shift)
    {
      etl::ibitset::operator >>=(shift);
      return *this;
    }

    //*************************************************************************
    /// operator ==
    //*************************************************************************
    friend bool operator == (const bitset<MaxN>& lhs, const bitset<MaxN>& rhs)
    {
      return etl::ibitset::is_equal(lhs, rhs);
    }

  private:

    element_type data[Array_Size > 0U ? Array_Size : 1U];
  };

  template <size_t MaxN>
  ETL_CONSTANT size_t bitset<MaxN>::ALLOCATED_BITS;
  
  template <size_t MaxN>
  ETL_CONSTANT size_t bitset<MaxN>::Allocated_Bits;

  //***************************************************************************
  /// operator &
  ///\ingroup bitset
  //***************************************************************************
  template <size_t MaxN>
  bitset<MaxN> operator & (const bitset<MaxN>& lhs, const bitset<MaxN>& rhs)
  {
    bitset<MaxN> temp(lhs);
    temp &= rhs;
    return temp;
  }

  //***************************************************************************
  /// operator |
  ///\ingroup bitset
  //***************************************************************************
  template<size_t MaxN>
  bitset<MaxN> operator | (const bitset<MaxN>& lhs, const bitset<MaxN>& rhs)
  {
    bitset<MaxN> temp(lhs);
    temp |= rhs;
    return temp;
  }

  //***************************************************************************
  /// operator ^
  ///\ingroup bitset
  //***************************************************************************
  template<size_t MaxN>
  bitset<MaxN> operator ^ (const bitset<MaxN>& lhs, const bitset<MaxN>& rhs)
  {
    bitset<MaxN> temp(lhs);
    temp ^= rhs;
    return temp;
  }

  //***************************************************************************
  /// operator !=
  ///\ingroup bitset
  //***************************************************************************
  template<size_t MaxN>
  bool operator != (const bitset<MaxN>& lhs, const bitset<MaxN>& rhs)
  {
    return !(lhs == rhs);
  }
}

//*************************************************************************
/// swap
//*************************************************************************
template <size_t MaxN>
void swap(etl::bitset<MaxN>& lhs, etl::bitset<MaxN>& rhs)
{
  lhs.swap(rhs);
}

#include "minmax_pop.h"

#endif
