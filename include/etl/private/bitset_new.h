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

#ifndef ETL_BITSET_NEW_INCLUDED
#define ETL_BITSET_NEW_INCLUDED

#include <string.h>
#include <stddef.h>
#include <stdint.h>

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
  template <typename TElement>
  class ibitset
  {
  protected:

    typedef typename etl::make_unsigned<TElement>::type element_type;
    typedef element_type* pointer;
    typedef const element_type* const_pointer;

    static ETL_CONSTANT size_t Bits_Per_Element = etl::integral_limits<element_type>::bits;
    static ETL_CONSTANT element_type All_Set    = etl::integral_limits<typename etl::make_unsigned<element_type>::type>::max;
    static ETL_CONSTANT element_type All_Clear  = element_type(0);

#if ETL_USING_CPP11
    typedef etl::span<element_type>       span_type;
    typedef etl::span<const element_type> const_span_type;
#endif

    enum
    {
      npos = etl::integral_limits<size_t>::max
    };

    //*************************************************************************
    /// Count the number of bits set.
    //*************************************************************************
    ETL_CONSTEXPR14 size_t count(const_pointer pdata, size_t Number_Of_Elements) const
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
    ETL_CONSTEXPR14 bool test(const_pointer pdata, size_t Number_Of_Elements, size_t position) const
    {
      size_t       index = 0U;
      element_type mask  = element_type(0);

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
    /// Set the bit at the position.
    //*************************************************************************
    ETL_CONSTEXPR14 void set(pointer pdata, size_t Number_Of_Elements, size_t position, bool value = true)
    {
      size_t    index = 0;
      element_type bit = 0;

      if (Number_Of_Elements == 0) 
      {
        return;
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

    //*************************************************************************
    /// Set from a string.
    //*************************************************************************
    void from_string(pointer pdata, size_t number_of_elements, size_t total_bits, const char* text)
    {
      size_t string_length = etl::strlen(text);
      size_t element_index = etl::min(number_of_elements - 1U, (string_length / Bits_Per_Element));     

      // Only reset elements we need to.
      while (element_index != number_of_elements)
      {
        pdata[element_index++] = All_Clear;
      }

      // Build from the string.
      size_t i = etl::min(total_bits, string_length);

      while (i > 0)
      {
        set(pdata, number_of_elements, --i, *text++ == ETL_STR('1'));
      }
    }

    //*************************************************************************
    /// Set from a wide string.
    //*************************************************************************
    void from_string(pointer pdata, size_t number_of_elements, size_t total_bits, const wchar_t* text)
    {
      size_t string_length = etl::strlen(text);
      size_t element_index = etl::min(number_of_elements - 1U, (string_length / Bits_Per_Element));

      // Only reset elements we need to.
      while (element_index != number_of_elements)
      {
        pdata[element_index++] = All_Clear;
      }

      // Build from the string.
      size_t i = etl::min(total_bits, string_length);

      while (i > 0)
      {
        set(pdata, number_of_elements, --i, *text++ == ETL_STRL('1'));
      }
    }

    //*************************************************************************
    /// Set from a u16 string.
    //*************************************************************************
    void from_string(pointer pdata, size_t number_of_elements, size_t total_bits, const char16_t* text)
    {
      size_t string_length = etl::strlen(text);
      size_t element_index = etl::min(number_of_elements - 1U, (string_length / Bits_Per_Element));

      // Only reset elements we need to.
      while (element_index != number_of_elements)
      {
        pdata[element_index++] = All_Clear;
      }

      // Build from the string.
      size_t i = etl::min(total_bits, string_length);

      while (i > 0)
      {
        set(pdata, number_of_elements, --i, *text++ == ETL_STRu('1'));
      }
    }

    //*************************************************************************
    /// Set from a u32 string.
    //*************************************************************************
    void from_string(pointer pdata, size_t number_of_elements, size_t total_bits, const char32_t* text)
    {
      size_t string_length = etl::strlen(text);
      size_t element_index = etl::min(number_of_elements - 1U, (string_length / Bits_Per_Element));

      // Only reset elements we need to.
      while (element_index != number_of_elements)
      {
        pdata[element_index++] = All_Clear;
      }

      // Build from the string.
      size_t i = etl::min(total_bits, string_length);

      while (i > 0)
      {
        set(pdata, number_of_elements, --i, *text++ == ETL_STRU('1'));
      }
    }

    //*************************************************************************
    /// Set from a string.
    //*************************************************************************
    ETL_CONSTEXPR14 void set(pointer pdata, size_t number_of_elements, size_t total_bits, const char* text)
    {
      from_string(pdata, number_of_elements, total_bits, text);
    }

    //*************************************************************************
    /// Set from a wstring.
    //*************************************************************************
    ETL_CONSTEXPR14 void set(pointer pdata, size_t number_of_elements, size_t total_bits, const wchar_t* text)
    {
      from_string(pdata, number_of_elements, total_bits, text);
    }

    //*************************************************************************
    /// Set from a u16string.
    //*************************************************************************
    ETL_CONSTEXPR14 void set(pointer pdata, size_t number_of_elements, size_t total_bits, const char16_t* text)
    {
      from_string(pdata, number_of_elements, total_bits, text);
    }

    //*************************************************************************
    /// Set from a u32string.
    //*************************************************************************
    ETL_CONSTEXPR14 void set(pointer pdata, size_t number_of_elements, size_t total_bits, const char32_t* text)
    {
      from_string(pdata, number_of_elements, total_bits, text);
    }

    //*************************************************************************
    /// Get as a value.
    //*************************************************************************
    template <typename T>
    ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value, T>::type
      value(const_pointer pdata, size_t number_of_elements) const
    {
      for (size_t i = sizeof(long long) / sizeof(element_type); i < number_of_elements; ++i)
      {
        ETL_ASSERT_AND_RETURN_VALUE(!(pdata[i]), ETL_ERROR(etl::bitset_overflow), T(0));
      }

      T v = T(0);

      const bool OK = (sizeof(T) * CHAR_BIT) >= (number_of_elements * Bits_Per_Element);

      ETL_ASSERT_AND_RETURN_VALUE(OK, ETL_ERROR(etl::bitset_type_too_small), T(0));

      if (OK)
      {
        uint_least8_t shift = 0U;

        for (size_t i = 0UL; i < number_of_elements; ++i)
        {
          v |= T(typename etl::make_unsigned<T>::type(pdata[i]) << shift);
          shift += uint_least8_t(Bits_Per_Element);
        }
      }

      return v;
    }

    //*************************************************************************
    /// Reset the bit at the position.
    //*************************************************************************
    ETL_CONSTEXPR14 void reset(pointer pdata, size_t number_of_elements, size_t position)
    {
      size_t       index = 0U;
      element_type bit = element_type(0);

      if (number_of_elements == 0)
      {
        return;
      }
      else if (number_of_elements == 1)
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

    //*************************************************************************
    /// Flip all of the bits.
    //*************************************************************************
    ETL_CONSTEXPR14 void flip(pointer pdata, size_t number_of_elements)
    {
      for (size_t i = 0UL; i < number_of_elements; ++i)
      {
        pdata[i] = ~pdata[i];
      }
    }

    //*************************************************************************
    /// Flip the bit at the position.
    //*************************************************************************
    ETL_CONSTEXPR14 void flip(pointer pdata, size_t number_of_elements, size_t total_bits, size_t position)
    {
      if (position < total_bits)
      {
        size_t    index = 0U;
        element_type bit = element_type(0);
        
        if (number_of_elements == 0)
        {
          return;
        }
        else if (number_of_elements == 1)
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
      }
    }

    //*************************************************************************
    // Are all the bits sets?
    //*************************************************************************
    ETL_CONSTEXPR14 bool all(const_pointer pdata, size_t number_of_elements, element_type top_mask) const ETL_NOEXCEPT
    {
      if (number_of_elements == 0UL)
      {
        return true;
      }

      // All but the last.
      for (size_t i = 0UL; i < (number_of_elements - 1U); ++i)
      {
        if (pdata[i] != All_Set)
        {
          return false;
        }
      }

      // The last.
      if (pdata[number_of_elements - 1U] != (All_Set & top_mask))
      {
        return false;
      }

      return true;
    }

    //*************************************************************************
    /// Are none of the bits set?
    //*************************************************************************
    ETL_CONSTEXPR14 bool none(const_pointer pdata, size_t number_of_elements) const ETL_NOEXCEPT
    {
      for (size_t i = 0UL; i < number_of_elements; ++i)
      {
        if (pdata[i] != 0)
        {
          return false;
        }
      }

      return true;
    }
//
//    //*************************************************************************
//    /// Finds the first bit in the specified state.
//    ///\param state The state to search for.
//    ///\returns The position of the bit or Number_Of_Elements if none were found.
//    //*************************************************************************
//    size_t find_first(bool state) const
//    {
//      return find_next(state, 0);
//    }
//
//    //*************************************************************************
//    /// Finds the next bit in the specified state.
//    ///\param state    The state to search for.
//    ///\param position The position to start from.
//    ///\returns The position of the bit or etl::ibitset<TElement>::npos if none were found.
//    //*************************************************************************
//    size_t find_next(bool state, size_t position) const
//    {
//      // Where to start.
//      size_t index;
//      size_t bit;
//
//      if (Number_Of_Elements == 0)
//      {
//        return etl::ibitset<TElement>::npos;
//      }
//      else if (Number_Of_Elements == 1)
//      {
//        index = 0;
//        bit = position;
//      }
//      else
//      {
//        index = position >> log2<Bits_Per_Element>::value;
//        bit = position & (Bits_Per_Element - 1);
//      }
//
//      element_type mask = 1 << bit;
//
//      // For each element in the bitset...
//      while (index < Number_Of_Elements)
//      {
//        element_type value = pdata[index];
//
//        // Needs checking?
//        if ((state && (value != ALL_CLEAR)) ||
//            (!state && (value != ALL_SET)))
//        {
//          // For each bit in the element...
//          while ((bit < Bits_Per_Element) && (position < Total_Bits))
//          {
//            // Equal to the required state?
//            if (((value & mask) != 0) == state)
//            {
//              return position;
//            }
//
//            // Move on to the next bit.
//            mask <<= 1;
//            ++position;
//            ++bit;
//          }
//        }
//        else
//        {
//          position += (Bits_Per_Element - bit);
//        }
//
//        // Start at the beginning for all other elements.
//        bit = 0;
//        mask = 1;
//
//        ++index;
//      }
//
//      return etl::ibitset<TElement>::npos;
//    }
// 

//
//    //*************************************************************************
//    /// operator &=
//    //*************************************************************************
//    ibitset& operator &=(const ibitset& other)
//    {
//      for (size_t i = 0UL; i < Number_Of_Elements; ++i)
//      {
//        pdata[i] &= other.pdata[i];
//      }
//
//      return *this;
//    }
//
//    //*************************************************************************
//    /// operator |=
//    //*************************************************************************
//    ibitset& operator |=(const ibitset& other)
//    {
//      for (size_t i = 0UL; i < Number_Of_Elements; ++i)
//      {
//        pdata[i] |= other.pdata[i];
//      }
//
//      return *this;
//    }
//
//    //*************************************************************************
//    /// operator ^=
//    //*************************************************************************
//    ibitset& operator ^=(const ibitset& other)
//    {
//      for (size_t i = 0UL; i < Number_Of_Elements; ++i)
//      {
//        pdata[i] ^= other.pdata[i];
//      }
//
//      return *this;
//    }
//
//    //*************************************************************************
//    /// operator <<=
//    //*************************************************************************
//    ibitset& operator<<=(size_t shift)
//    {
//      if (shift >= Total_Bits)
//      {
//        reset();
//      }
//      else if (Number_Of_Elements != 0UL)
//      {
//        // Just one element?
//        if (Number_Of_Elements == 1UL)
//        {
//          pdata[0] <<= shift;
//        }
//        else if (shift == Bits_Per_Element)
//        {
//          etl::copy_backward(pdata, pdata + Number_Of_Elements - 1U, pdata + Number_Of_Elements);
//          pdata[0] = 0;
//        }
//        else
//        {
//          // The place where the elements are split when shifting.
//          const size_t split_position = Bits_Per_Element - (shift % Bits_Per_Element);
//
//          // Where we are shifting from.
//          int src_index = int(Number_Of_Elements - (shift / Bits_Per_Element) - 1U);
//
//          // Where we are shifting to.
//          int dst_index = int(Number_Of_Elements - 1U);
//
//          // Shift control constants.
//          const size_t lsb_shift = Bits_Per_Element - split_position;
//          const size_t msb_shift = split_position;
//
//          const element_type lsb_mask         = element_type(etl::integral_limits<element_type>::max >> (Bits_Per_Element - split_position));
//          const element_type msb_mask         = etl::integral_limits<element_type>::max - lsb_mask;
//          const element_type lsb_shifted_mask = element_type(lsb_mask << lsb_shift);
//          
//          // First lsb.
//          element_type lsb = element_type((pdata[src_index] & lsb_mask) << lsb_shift);
//          pdata[dst_index] = lsb;
//          --src_index;
//
//          // Now do the shifting.
//          while (src_index >= 0)
//          {
//            // Shift msb.
//            element_type msb = element_type((pdata[src_index] & msb_mask) >> msb_shift);
//            pdata[dst_index] = pdata[dst_index] | msb;
//            --dst_index;
//
//            // Shift lsb.
//            element_type lsb = element_type((pdata[src_index] & lsb_mask) << lsb_shift);
//            pdata[dst_index] = lsb;
//            --src_index;
//          }
//
//          // Clear the remaining bits.
//          // First lsb.
//          pdata[dst_index] &= lsb_shifted_mask;
//          --dst_index;
//
//          // The other remaining bytes on the right.
//          while (dst_index >= 0)
//          {
//            pdata[dst_index] = 0;
//            --dst_index;
//          }
//        }
//
//        // Truncate any bits shifted to the left.
//        clear_unused_bits_in_msb();
//      }
//
//      return *this;
//    }
//
//    //*************************************************************************
//    /// operator >>=
//    //*************************************************************************
//    ibitset& operator>>=(size_t shift)
//    {
//      if (shift >= Total_Bits)
//      {
//        reset();
//      }
//      else if (Number_Of_Elements != 0UL)
//      {
//        // Just one element?
//        if (Number_Of_Elements == 1UL)
//        {
//          pdata[0] >>= shift;
//        }
//        // Shift is the size of an element?
//        else if (shift == Bits_Per_Element)
//        {
//          etl::copy(pdata + 1, pdata + Number_Of_Elements, pdata);
//          pdata[Number_Of_Elements - 1U] = 0;
//        }
//        else
//        {
//          // The place where the elements are split when shifting.
//          const size_t split_position = shift % Bits_Per_Element;
//
//          // Where we are shifting from.
//          int src_index = int(shift / Bits_Per_Element);
//
//          // Where we are shifting to.
//          int dst_index = 0;
//
//          // Shift control constants.
//          const size_t lsb_shift = Bits_Per_Element - split_position;
//          const size_t msb_shift = split_position;
//
//          const element_type lsb_mask         = element_type(etl::integral_limits<element_type>::max >> (Bits_Per_Element - split_position));
//          const element_type msb_mask         = etl::integral_limits<element_type>::max - lsb_mask;
//          const element_type msb_shifted_mask = element_type(msb_mask >> msb_shift);
//
//          // Now do the shifting.
//          while (src_index < int(Number_Of_Elements - 1))
//          {
//            // Shift msb.
//            element_type msb = element_type((pdata[src_index] & msb_mask) >> msb_shift);
//            ++src_index;
//            
//            // Shift lsb.
//            element_type lsb = element_type((pdata[src_index] & lsb_mask) << lsb_shift);
//            
//            // Combine them.
//            pdata[dst_index] = lsb | msb;
//            ++dst_index;
//          }
//
//          // Final msb.
//          element_type msb = element_type((pdata[src_index] & msb_mask) >> msb_shift);
//          pdata[dst_index] = msb;
//
//          // Clear the remaining bits.
//          // First msb.
//          pdata[dst_index] &= msb_shifted_mask;
//          ++dst_index;
//
//          // The other remaining bytes.
//          while (dst_index < int(Number_Of_Elements))
//          {
//            pdata[dst_index] = 0;
//            ++dst_index;
//          }
//        }
//      }
//
//      return *this;
//    }
//
//    //*************************************************************************
//    /// operator =
//    //*************************************************************************
//    ibitset& operator =(const ibitset& other)
//    {
//      if (this != &other)
//      {
//        etl::copy_n(other.pdata, Number_Of_Elements, pdata);
//      }
//
//      return *this;
//    }
//
//    //*************************************************************************
//    /// swap
//    //*************************************************************************
//    void swap(ibitset& other)
//    {
//      etl::swap_ranges(pdata, pdata + Number_Of_Elements, other.pdata);
//    }
//
//#if ETL_USING_CPP11
//    //*************************************************************************
//    /// span
//    /// Returns a span of the underlying data.
//    //*************************************************************************
//    span_type span()
//    {
//      return span_type(pdata, pdata + Number_Of_Elements);
//    }
//
//    //*************************************************************************
//    /// span
//    /// Returns a const span of the underlying data.
//    //*************************************************************************
//    const_span_type span() const
//    {
//      return const_span_type(pdata, pdata + Number_Of_Elements);
//    }
//#endif

  protected:

    //*************************************************************************
    /// Initialise from an unsigned long long.
    //*************************************************************************
    ETL_CONSTEXPR14 void initialise(pointer pdata, size_t number_of_elements, unsigned long long value)
    {
      const size_t Shift = (etl::integral_limits<unsigned long long>::bits <= (int)Bits_Per_Element) ? 0 : Bits_Per_Element;

      // Can we do it in one hit?
      if (Shift == 0)
      {
        pdata[0] = element_type(value);
      }
      else
      {
        size_t i = 0UL;

        // Set the non-zero elements.
        while ((value != 0) && (i != number_of_elements))
        {
          pdata[i++] = value & All_Set;
          value = value >> Shift;
        }

        // Clear the remaining elements.
        while (i != number_of_elements)
        {
          pdata[i++] = All_Clear;
        }
      }
    }

    ////*************************************************************************
    ///// Invert
    ////*************************************************************************
    //void invert()
    //{
    //  for (size_t i = 0UL; i < Number_Of_Elements; ++i)
    //  {
    //    pdata[i] = ~pdata[i];
    //  }

    //  clear_unused_bits_in_msb();
    //}

    ////*************************************************************************
    ///// Gets a reference to the specified bit.
    ////*************************************************************************
    //bit_reference get_bit_reference(size_t position)
    //{
    //  return bit_reference(*this, position);
    //}

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    //ibitset(size_t nbits_, size_t size_, element_type* pdata_)
    //  : Total_Bits(nbits_)
    //  , Number_Of_Elements(size_)
    //  , pdata(pdata_)
    //{
    //  const size_t allocated_bits = Number_Of_Elements * Bits_Per_Element;
    //  const size_t top_mask_shift = ((Bits_Per_Element - (allocated_bits - Total_Bits)) % Bits_Per_Element);
    //  Top_Mask = element_type(top_mask_shift == 0 ? ALL_SET : ~(ALL_SET << top_mask_shift));
    //}

    ////*************************************************************************
    ///// Compare bitsets.
    ////*************************************************************************
    //static bool is_equal(const ibitset& lhs, const ibitset&rhs)
    //{
    //  return etl::equal(lhs.pdata, lhs.pdata + lhs.Number_Of_Elements, rhs.pdata);
    //}

    //element_type Top_Mask;

    //*************************************************************************
    ETL_CONSTEXPR14 void copy_elements(const_pointer sb, pointer db, size_t count)
    {
      while (count != 0)
      {
        *db = *sb;
        ++db;
        ++sb;
        --count;
      }
    }

//
//    // Disable copy construction.
//    ibitset(const ibitset&);
//
//    size_t Total_Bits;
//    const size_t Number_Of_Elements;
//    element_type*   pdata;
//
//    //*************************************************************************
//    /// Destructor.
//    //*************************************************************************
//#if defined(ETL_POLYMORPHIC_BITSET) || defined(ETL_POLYMORPHIC_CONTAINERS)
//  public:
//    virtual ~ibitset()
//    {
//    }
//#else
//  protected:
//    ~ibitset()
//    {
//    }
//#endif
  };

  //*************************************************************************
  /// The class emulates an array of bool elements, but optimized for space allocation.
  /// Will accommodate any number of bits.
  ///\tparam Total_Bits The number of bits.
  ///\ingroup bitset
  //*************************************************************************
  template <size_t Total_Bits, typename TElement = char>
  class bitset : public etl::ibitset<TElement>
  {
  public:

    typedef typename etl::ibitset<TElement>::element_type element_type;
    typedef typename etl::ibitset<TElement>::pointer pointer;
    typedef typename etl::ibitset<TElement>::const_pointer const_pointer;
   
    static ETL_CONSTANT size_t Number_Of_Elements = (Total_Bits % Bits_Per_Element == 0) ? Total_Bits / Bits_Per_Element : Total_Bits / Bits_Per_Element + 1;
    static ETL_CONSTANT size_t Allocated_Bits     = Number_Of_Elements * Bits_Per_Element;
    static ETL_CONSTANT size_t Top_Mask_Shift     = ((Bits_Per_Element - (Allocated_Bits - Total_Bits)) % Bits_Per_Element);
    static ETL_CONSTANT element_type Top_Mask     = element_type(Top_Mask_Shift == 0 ? All_Set : ~(All_Set << Top_Mask_Shift));

    static ETL_CONSTANT size_t ALLOCATED_BITS = Allocated_Bits; ///< For backward compatibility.

    //*************************************************************************
    /// The reference type returned.
    //*************************************************************************
    class bit_reference
    {
    public:

      friend class bitset;

      //*******************************
      /// Conversion operator.
      //*******************************
      ETL_CONSTEXPR14 operator bool() const
      {
        return p_bitset->test(position);
      }

      //*******************************
      /// Assignment operator.
      //*******************************
      ETL_CONSTEXPR14 bit_reference& operator = (bool b)
      {
        p_bitset->set(position, b);
        return *this;
      }

      //*******************************
      /// Assignment operator.
      //*******************************
      ETL_CONSTEXPR14 bit_reference& operator = (const bit_reference& r)
      {
        p_bitset->set(position, bool(r));
        return *this;
      }

      //*******************************
      /// Flip the bit.
      //*******************************
      ETL_CONSTEXPR14 bit_reference& flip()
      {
        p_bitset->flip(position);
        return *this;
      }

      //*******************************
      /// Return the logical inverse of the bit.
      //*******************************
      ETL_CONSTEXPR14 bool operator~() const
      {
        return !p_bitset->test(position);
      }

    private:

      //*******************************
      /// Default constructor.
      //*******************************
      ETL_CONSTEXPR14 bit_reference()
        : p_bitset(ETL_NULLPTR)
        , position(0)
      {
      }

      //*******************************
      /// Constructor.
      //*******************************
      ETL_CONSTEXPR14 bit_reference(bitset<Total_Bits>& r_bitset, size_t position_)
        : p_bitset(&r_bitset)
        , position(position_)
      {
      }

      bitset<Total_Bits>* p_bitset; ///< The bitset.
      size_t              position; ///< The position in the bitset.
    };

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    ETL_CONSTEXPR14 bitset() ETL_NOEXCEPT
      : data()
    {
      reset();
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    ETL_CONSTEXPR14 bitset(const bitset<Total_Bits, TElement>& other) ETL_NOEXCEPT
      : data()
    {
      copy_elements(other.data, data, Number_Of_Elements);
    }

    //*************************************************************************
    /// Construct from a value.
    //*************************************************************************
    ETL_CONSTEXPR14 bitset(unsigned long long value) ETL_NOEXCEPT
      : data()
    {
      initialise(data, Number_Of_Elements, value);
      clear_unused_bits_in_msb();
    }

    //*************************************************************************
    /// Construct from a string.
    //*************************************************************************
    bitset(const char* text)
      : data()
    {
      etl::ibitset<TElement>::set(data, Number_Of_Elements, Total_Bits, text);
      clear_unused_bits_in_msb();
    }

    //*************************************************************************
    /// Construct from a string.
    //*************************************************************************
    bitset(const wchar_t* text)
      : data()
    {
      etl::ibitset<TElement>::set(data, Number_Of_Elements, Total_Bits, text);
      clear_unused_bits_in_msb();
    }

    //*************************************************************************
    /// Construct from a string.
    //*************************************************************************
    bitset(const char16_t* text)
      : data()
    {
      etl::ibitset<TElement>::set(data, Number_Of_Elements, Total_Bits, text);
      clear_unused_bits_in_msb();
    }

    //*************************************************************************
    /// Construct from a string.
    //*************************************************************************
    bitset(const char32_t* text)
      : data()
    {
      etl::ibitset<TElement>::set(data, Number_Of_Elements, Total_Bits, text);
      clear_unused_bits_in_msb();
    }

    //*************************************************************************
    /// Set all of the bits.
    //*************************************************************************
    ETL_CONSTEXPR14 bitset<Total_Bits, TElement>& set()
    {
      etl::fill_n(data, ETL_OR_STD::size(data), All_Set);
      clear_unused_bits_in_msb();

      return *this;
    }

    //*************************************************************************
    /// Set the bit at the position.
    //*************************************************************************
    ETL_CONSTEXPR14 bitset<Total_Bits, TElement>& set(size_t position, bool value = true) ETL_NOEXCEPT
    {
      etl::ibitset<TElement>::set(data, Number_Of_Elements, position, value);
      clear_unused_bits_in_msb();
      
      return *this;
    }

    //*************************************************************************
    /// Set from a string.
    //*************************************************************************
    bitset<Total_Bits, TElement>& set(const char* text)
    {
      ETL_ASSERT_AND_RETURN_VALUE(text != 0, ETL_ERROR(bitset_nullptr), *this);
      etl::ibitset<TElement>::set(data, Number_Of_Elements, Total_Bits, text);

      return *this;
    }

    //*************************************************************************
    /// Set from a string.
    //*************************************************************************
    bitset<Total_Bits, TElement>& set(const wchar_t* text)
    {
      ETL_ASSERT_AND_RETURN_VALUE(text != 0, ETL_ERROR(bitset_nullptr), *this);
      etl::ibitset<TElement>::set(data, Number_Of_Elements, Total_Bits, text);

      return *this;
    }

    //*************************************************************************
    /// Set from a string.
    //*************************************************************************
    bitset<Total_Bits, TElement>& set(const char16_t* text)
    {
      ETL_ASSERT_AND_RETURN_VALUE(text != 0, ETL_ERROR(bitset_nullptr), *this);
      etl::ibitset<TElement>::set(data, Number_Of_Elements, Total_Bits, text);

      return *this;
    }

    //*************************************************************************
    /// Set from a string.
    //*************************************************************************
    bitset<Total_Bits, TElement>& set(const char32_t* text)
    {
      ETL_ASSERT_AND_RETURN_VALUE(text != 0, ETL_ERROR(bitset_nullptr), *this);
      etl::ibitset<TElement>::set(data, Number_Of_Elements, Total_Bits, text);

      return *this;
    }

    //*************************************************************************
    /// Set from a string.
    //*************************************************************************
    bitset<Total_Bits, TElement>& from_string(const char* text)
    {
      etl::ibitset<TElement>::from_string(data, Number_Of_Elements, Total_Bits, text);

      return *this;
    }

    //*************************************************************************
    /// Set from a wide string.
    //*************************************************************************
    bitset<Total_Bits, TElement>& from_string(const wchar_t* text)
    {
      etl::ibitset<TElement>::from_string(data, Number_Of_Elements, Total_Bits, text);

      return *this;
    }

    //*************************************************************************
    /// Set from a u16 string.
    //*************************************************************************
    bitset<Total_Bits, TElement>& from_string(const char16_t* text)
    {
      etl::ibitset<TElement>::from_string(data, Number_Of_Elements, Total_Bits, text);

      return *this;
    }

    //*************************************************************************
    /// Set from a u32 string.
    //*************************************************************************
    bitset<Total_Bits, TElement>& from_string(const char32_t* text)
    {
      etl::ibitset<TElement>::from_string(data, Number_Of_Elements, Total_Bits, text);

      return *this;
    }

    //*************************************************************************
    /// Get as an integral value.
    //*************************************************************************
    template <typename T>
    ETL_CONSTEXPR14
      typename etl::enable_if<etl::is_integral<T>::value, T>::type
      value() const
    {
      ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Only integral types are supported");
      ETL_STATIC_ASSERT((sizeof(T) * CHAR_BIT) >= (Number_Of_Elements * Bits_Per_Element), "Type too small");

      return etl::ibitset<TElement>::value<T>(data, Number_Of_Elements);
    }

    //*************************************************************************
    /// Get as an unsigned long.
    //*************************************************************************
    ETL_CONSTEXPR14 unsigned long to_ulong() const
    {
      return value<unsigned long>();
    }

    //*************************************************************************
    /// Get as an unsigned long long.
    //*************************************************************************
    ETL_CONSTEXPR14 unsigned long long to_ullong() const
    {
      return value<unsigned long long>();
    }
    //*************************************************************************
    /// Reset all of the bits.
    //*************************************************************************
    ETL_CONSTEXPR14 bitset<Total_Bits, TElement>& reset() ETL_NOEXCEPT
    {
      etl::fill_n(data, Number_Of_Elements, All_Clear);

      return *this;
    }

    //*************************************************************************
    /// Reset the bit at the position.
    //*************************************************************************
    ETL_CONSTEXPR14 bitset<Total_Bits, TElement>& reset(size_t position) ETL_NOEXCEPT
    {
      etl::ibitset<TElement>::reset(data, Number_Of_Elements, position);
      return *this;
    }

    //*************************************************************************
    /// Tests a bit at a position.
    /// Positions greater than the number of configured bits will return <b>false</b>.
    //*************************************************************************
    ETL_CONSTEXPR14 bool test(size_t position) const
    {
      return etl::ibitset<TElement>::test(data, Number_Of_Elements, position);
    }
// 
   //*************************************************************************
   /// The number of bits in the bitset.
   //*************************************************************************
   ETL_CONSTEXPR size_t size() const ETL_NOEXCEPT
   {
     return Total_Bits;
   }
 
    //*************************************************************************
    /// Count the number of bits set.
    //*************************************************************************
    ETL_CONSTEXPR14 size_t count() const ETL_NOEXCEPT
    {
      return etl::ibitset<TElement>::count(data, Number_Of_Elements);
    }

    //*************************************************************************
    // Are all the bits sets?
    //*************************************************************************
    ETL_CONSTEXPR14 bool all() const ETL_NOEXCEPT
    {
      return etl::ibitset<TElement>::all(data, Number_Of_Elements, Top_Mask);
    }

    //*************************************************************************
    /// Are none of the bits set?
    //*************************************************************************
    ETL_CONSTEXPR14 bool none() const ETL_NOEXCEPT
    {
      return etl::ibitset<TElement>::none(data, Number_Of_Elements);
    }

    //*************************************************************************
    /// Are any of the bits set?
    //*************************************************************************
    ETL_CONSTEXPR14 bool any() const ETL_NOEXCEPT
    {
      return !none();
    }
 
    //*************************************************************************
    /// Flip all of the bits.
    //*************************************************************************
    ETL_CONSTEXPR14 bitset<Total_Bits, TElement>& flip()
    {
      etl::ibitset<TElement>::flip(data, Number_Of_Elements);
      clear_unused_bits_in_msb();

      return *this;
    }

    //*************************************************************************
    /// Flip the bit at the position.
    //*************************************************************************
    ETL_CONSTEXPR14 bitset<Total_Bits, TElement>& flip(size_t position)
    {
      etl::ibitset<TElement>::flip(data, Number_Of_Elements, Total_Bits, position);

      return *this;
    }

    //*************************************************************************
    /// Read [] operator.
    //*************************************************************************
    ETL_CONSTEXPR14 bool operator[] (size_t position) const
    {
      return etl::ibitset<TElement>::test(data, Number_Of_Elements, position);
    }

    //*************************************************************************
    /// Write [] operator.
    //*************************************************************************
    ETL_CONSTEXPR14 bit_reference operator [] (size_t position)
    {
      return bit_reference(*this, position);
    }

//    //*************************************************************************
//    /// Returns a string representing the bitset.
//    //*************************************************************************
//#if ETL_USING_CPP11
//    template <typename TString = etl::string<Total_Bits>>
//#else
//    template <typename TString>
//#endif
//    TString to_string(typename TString::value_type zero = typename TString::value_type('0'), typename TString::value_type one = typename TString::value_type('1')) const
//    {
//      TString result;
//
//      result.resize(Total_Bits, '\0');
//
//      ETL_ASSERT_AND_RETURN_VALUE((result.size() == Total_Bits), ETL_ERROR(etl::bitset_overflow), result);
//
//      for (size_t i = Total_Bits; i > 0; --i) 
//      {
//        result[Total_Bits - i] = test(i - 1) ? one : zero;
//      }
//      
//      return result;
//    }
//
//    //*************************************************************************
//    /// operator =
//    //*************************************************************************
//    bitset<Total_Bits, TElement>& operator =(const bitset<Total_Bits, TElement>& other)
//    {
//      if (this != &other)
//      {
//        etl::copy_n(other.data, Number_Of_Elements, data);
//      }
//
//      return *this;
//    }
//
//    //*************************************************************************
//    /// operator &=
//    //*************************************************************************
//    bitset<Total_Bits, TElement>& operator &=(const bitset<Total_Bits, TElement>& other)
//    {
//      etl::ibitset<TElement>::operator &=(other);
//      return *this;
//    }
//
//    //*************************************************************************
//    /// operator |=
//    //*************************************************************************
//    bitset<Total_Bits, TElement>& operator |=(const bitset<Total_Bits, TElement>& other)
//    {
//      etl::ibitset<TElement>::operator |=(other);
//      return *this;
//    }
//
//    //*************************************************************************
//    /// operator ^=
//    //*************************************************************************
//    bitset<Total_Bits, TElement>& operator ^=(const bitset<Total_Bits, TElement>& other)
//    {
//      etl::ibitset<TElement>::operator ^=(other);
//      return *this;
//    }
//
//    //*************************************************************************
//    /// operator ~
//    //*************************************************************************
//    bitset<Total_Bits, TElement> operator ~() const
//    {
//      etl::bitset<Total_Bits, TElement> temp(*this);
//
//      temp.invert();
//
//      return temp;
//    }
//
//    //*************************************************************************
//    /// operator <<
//    //*************************************************************************
//    bitset<Total_Bits, TElement> operator<<(size_t shift) const
//    {
//      etl::bitset<Total_Bits, TElement> temp(*this);
//
//      temp <<= shift;
//
//      return temp;
//    }
//
//    //*************************************************************************
//    /// operator <<=
//    //*************************************************************************
//    bitset<Total_Bits, TElement>& operator<<=(size_t shift)
//    {
//      etl::ibitset<TElement>::operator <<=(shift);
//      return *this;
//    }
//
//    //*************************************************************************
//    /// operator >>
//    //*************************************************************************
//    bitset<Total_Bits, TElement> operator>>(size_t shift) const
//    {
//      bitset<Total_Bits, TElement> temp(*this);
//
//      temp >>= shift;
//
//      return temp;
//    }
//
//    //*************************************************************************
//    /// operator >>=
//    //*************************************************************************
//    bitset<Total_Bits, TElement>& operator>>=(size_t shift)
//    {
//      etl::ibitset<TElement>::operator >>=(shift);
//      return *this;
//    }

    //*************************************************************************
    /// operator ==
    //*************************************************************************
    friend ETL_CONSTEXPR14 bool operator == (const bitset<Total_Bits, TElement>& lhs, const bitset<Total_Bits, TElement>& rhs)
    {
      return etl::equal(lhs.data, 
                        lhs.data + lhs.Number_Of_Elements,
                        rhs.data);
    }

  private:

    //*************************************************************************
    /// Correct the unused top bits after bit manipulation.
    //*************************************************************************
    ETL_CONSTEXPR14 void clear_unused_bits_in_msb() ETL_NOEXCEPT
    {
      data[Number_Of_Elements - 1U] &= Top_Mask;
    }

    element_type data[Number_Of_Elements > 0U ? Number_Of_Elements : 1U];
  };

  //***************************************************************************
  /// operator &
  ///\ingroup bitset
  //***************************************************************************
  template <size_t Total_Bits, typename TElement>
  bitset<Total_Bits> operator & (const bitset<Total_Bits, TElement>& lhs, const bitset<Total_Bits, TElement>& rhs) ETL_NOEXCEPT
  {
    bitset<Total_Bits> temp(lhs);
    temp &= rhs;
    return temp;
  }

  //***************************************************************************
  /// operator |
  ///\ingroup bitset
  //***************************************************************************
  template<size_t Total_Bits, typename TElement>
  bitset<Total_Bits> operator | (const bitset<Total_Bits, TElement>& lhs, const bitset<Total_Bits, TElement>& rhs) ETL_NOEXCEPT
  {
    bitset<Total_Bits> temp(lhs);
    temp |= rhs;
    return temp;
  }

  //***************************************************************************
  /// operator ^
  ///\ingroup bitset
  //***************************************************************************
  template<size_t Total_Bits, typename TElement>
  bitset<Total_Bits> operator ^ (const bitset<Total_Bits, TElement>& lhs, const bitset<Total_Bits, TElement>& rhs) ETL_NOEXCEPT
  {
    bitset<Total_Bits> temp(lhs);
    temp ^= rhs;
    return temp;
  }

  //***************************************************************************
  /// operator !=
  ///\ingroup bitset
  //***************************************************************************
  template<size_t Total_Bits, typename TElement>
  ETL_CONSTEXPR14 bool operator != (const bitset<Total_Bits, TElement>& lhs, const bitset<Total_Bits, TElement>& rhs) ETL_NOEXCEPT
  {
    return !(lhs == rhs);
  }
}

//*************************************************************************
/// swap
//*************************************************************************
template <size_t Total_Bits, typename TElement>
ETL_CONSTEXPR14 void swap(etl::bitset<Total_Bits, TElement>& lhs, etl::bitset<Total_Bits, TElement>& rhs) ETL_NOEXCEPT
{
  lhs.swap(rhs);
}

#include "minmax_pop.h"

#endif
