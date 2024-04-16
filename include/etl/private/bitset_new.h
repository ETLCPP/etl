///\file
/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2022 John Wellbelove

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
#include "../enum_type.h"
#include "../largest.h"
#include "../smallest.h"

#include <string.h>
#include <stddef.h>
#include <stdint.h>

#if ETL_USING_STL
#include <algorithm>
#endif

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
  /// Flags to indicate whether the bitset is contained in a single element
  /// or spread over an array of elements.
  ///\ingroup bitset
  //***************************************************************************
  struct bitset_storage_model
  {
    enum enum_type
    {
      Undefined = 0,
      Single    = 1,
      Multi     = 2
    };

    ETL_DECLARE_ENUM_TYPE(bitset_storage_model, char)
    ETL_ENUM_TYPE(Undefined, "Undefined")
    ETL_ENUM_TYPE(Single,    "Single")
    ETL_ENUM_TYPE(Multi,     "Multi")
    ETL_END_ENUM_TYPE
  };

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
  /// Bitset type_too_small exception.
  ///\ingroup bitset
  //***************************************************************************
  class bitset_string_too_small : public bitset_exception
  {
  public:

    bitset_string_too_small(string_type file_name_, numeric_type line_number_)
      : bitset_exception(ETL_ERROR_TEXT("bitset:type_too_small", ETL_BITSET_FILE_ID"A"), file_name_, line_number_)
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

  //***************************************************************************
  /// Bitset nullptr buffer exception.
  ///\ingroup bitset
  //***************************************************************************
  class bitset_invalid_buffer : public bitset_exception
  {
  public:

    bitset_invalid_buffer(string_type file_name_, numeric_type line_number_)
      : bitset_exception(ETL_ERROR_TEXT("bitset:invalid buffer", ETL_BITSET_FILE_ID"D"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  namespace private_bitset
  {
    template <typename TElement>
    class bitset_impl_common
    {
    public:

      typedef TElement        element_type;
      typedef TElement*       pointer;
      typedef const TElement* const_pointer;

      static ETL_CONSTANT size_t   npos              = etl::integral_limits<size_t>::max;
      static ETL_CONSTANT size_t   Bits_Per_Element  = etl::integral_limits<TElement>::bits;
      static ETL_CONSTANT TElement All_Set_Element   = etl::integral_limits<TElement>::max;
      static ETL_CONSTANT TElement All_Clear_Element = element_type(0);
    };

    template <typename TElement>
    ETL_CONSTANT size_t bitset_impl_common<TElement>::npos;

    template <typename TElement>
    ETL_CONSTANT size_t bitset_impl_common<TElement>::Bits_Per_Element;

    template <typename TElement>
    ETL_CONSTANT TElement bitset_impl_common<TElement>::All_Set_Element;

    template <typename TElement>
    ETL_CONSTANT TElement bitset_impl_common<TElement>::All_Clear_Element;
  }

  //*************************************************************************
  /// Bitset implementation declaration.
  ///\ingroup bitset
  //*************************************************************************
  template <typename TElement, char Bitset_Layout>
  class bitset_impl;

  //*************************************************************************
  /// The implementation class for single element etl::bitset
  ///\ingroup bitset
  //*************************************************************************
  template <typename TElement>
  class bitset_impl<TElement, etl::bitset_storage_model::Single> : public etl::private_bitset::bitset_impl_common<TElement>
  {
  public:

    using typename etl::private_bitset::bitset_impl_common<TElement>::element_type;
    using typename etl::private_bitset::bitset_impl_common<TElement>::pointer;
    using typename etl::private_bitset::bitset_impl_common<TElement>::const_pointer;

    using etl::private_bitset::bitset_impl_common<TElement>::Bits_Per_Element;
    using etl::private_bitset::bitset_impl_common<TElement>::All_Set_Element;
    using etl::private_bitset::bitset_impl_common<TElement>::All_Clear_Element;

    using etl::private_bitset::bitset_impl_common<TElement>::npos;

    //*************************************************************************
    /// Set all of the bits.
    //*************************************************************************
    ETL_CONSTEXPR14 
    static 
    void set_all(pointer      pbuffer,
                 size_t       /*number_of_elements*/,
                 element_type top_mask) ETL_NOEXCEPT
    {
      *pbuffer = All_Set_Element & top_mask;
    }

    //*************************************************************************
    /// Set the bit at the position.
    //*************************************************************************
    ETL_CONSTEXPR14
    static
    void set_position(pointer pbuffer, 
                      size_t  position, 
                      bool    value = true)
    {
      const element_type mask = element_type(element_type(1) << position);

      if (value == true)
      {
        *pbuffer |= mask;
      }
      else
      {
        *pbuffer &= ~mask;
      }
    }

    //*************************************************************************
    /// Set the bit at the position.
    //*************************************************************************
    template <size_t Position>
    ETL_CONSTEXPR14
    static
    void set_position(pointer pbuffer,
                      bool    value = true)
    {
      const element_type mask = element_type(element_type(1) << Position);

      if (value == true)
      {
        *pbuffer |= mask;
      }
      else
      {
        *pbuffer &= ~mask;
      }
    }

    //*************************************************************************
    /// Set the bit at the position.
    //*************************************************************************
    template <size_t Position, bool Value>
    ETL_CONSTEXPR14
    static
    void set_position(pointer pbuffer)
    {
      const element_type mask = element_type(element_type(1) << Position);

      if (Value == true)
      {
        *pbuffer |= mask;
      }
      else
      {
        *pbuffer &= ~mask;
      }
    }

    //*************************************************************************
    /// Reset all of the bits.
    //*************************************************************************
    ETL_CONSTEXPR14
    static
    void reset_all(pointer pbuffer, 
                   size_t  /*number_of_elements*/) ETL_NOEXCEPT
    {
      *pbuffer = All_Clear_Element;
    }

    //*************************************************************************
    /// Reset the bit at the position.
    //*************************************************************************
    ETL_CONSTEXPR14
    static
    void reset_position(pointer pbuffer, 
                        size_t  position)
    {
      const element_type mask = element_type(element_type(1) << position);
      *pbuffer &= ~mask;
    }

    //*************************************************************************
    /// Set from a string.
    //*************************************************************************
    ETL_CONSTEXPR14
    static 
    void from_string(pointer     pbuffer, 
                     size_t      /*number_of_elements*/, 
                     size_t      active_bits, 
                     const char* text) ETL_NOEXCEPT
    {
      reset_all(pbuffer, 1U);

      if (text != ETL_NULLPTR)
      {
        size_t string_length = etl::strlen(text);

        // Build from the string.
        string_length = etl::min(active_bits, string_length);

        element_type mask = element_type(element_type(1) << (string_length - 1U));

        for (size_t i = 0U; i < string_length; ++i)
        {
          if (text[i] == ETL_STR('1'))
          {
            *pbuffer |= mask;
          }

          mask >>= 1U;
        }
      }
    }

    //*************************************************************************
    /// Set from a wide string.
    //*************************************************************************
    ETL_CONSTEXPR14
    static 
    void from_string(pointer        pbuffer, 
                     size_t         /*number_of_elements*/, 
                     size_t         active_bits, 
                     const wchar_t* text) ETL_NOEXCEPT
    {
      reset_all(pbuffer, 1U);

      if (text != ETL_NULLPTR)
      {
        size_t string_length = etl::strlen(text);

        // Build from the string.
        string_length = etl::min(active_bits, string_length);

        element_type mask = element_type(element_type(1) << (string_length - 1U));

        for (size_t i = 0U; i < string_length; ++i)
        {
          if (text[i] == ETL_STRL('1'))
          {
            *pbuffer |= mask;
          }

          mask >>= 1U;
        }
      }
    }

    //*************************************************************************
    /// Set from a u16 string.
    //*************************************************************************
    ETL_CONSTEXPR14
    static 
    void from_string(pointer         pbuffer, 
                     size_t          /*number_of_elements*/, 
                     size_t          active_bits, 
                     const char16_t* text) ETL_NOEXCEPT
    {
      reset_all(pbuffer, 1U);

      if (text != ETL_NULLPTR)
      {
        size_t string_length = etl::strlen(text);

        // Build from the string.
        string_length = etl::min(active_bits, string_length);

        element_type mask = element_type(element_type(1) << (string_length - 1U));

        for (size_t i = 0U; i < string_length; ++i)
        {
          if (text[i] == ETL_STRu('1'))
          {
            *pbuffer |= mask;
          }

          mask >>= 1U;
        }
      }
    }

    //*************************************************************************
    /// Set from a u32 string.
    //*************************************************************************
    ETL_CONSTEXPR14 
    static 
    void from_string(pointer         pbuffer, 
                     size_t          /*number_of_elements*/, 
                     size_t          active_bits, 
                     const char32_t* text) ETL_NOEXCEPT
    {
      reset_all(pbuffer, 1U);

      if (text != ETL_NULLPTR)
      {
        size_t string_length = etl::strlen(text);

        // Build from the string.
        string_length = etl::min(active_bits, string_length);

        element_type mask = element_type(element_type(1) << (string_length - 1U));

        for (size_t i = 0U; i < string_length; ++i)
        {
          if (text[i] == ETL_STRU('1'))
          {
            *pbuffer |= mask;
          }

          mask >>= 1U;
        }
      }
    }

    //*************************************************************************
    /// Get as an integral value.
    //*************************************************************************
    template <typename T>
    ETL_CONSTEXPR14
    static
    T value(const_pointer pbuffer, 
            size_t        /*number_of_elements*/) ETL_NOEXCEPT
    {
      return T(*pbuffer);
    }

    //*************************************************************************
    /// Extract an integral value from an arbitrary position and length.
    //*************************************************************************
    template <typename T>
    ETL_CONSTEXPR14
    static
    T extract(const_pointer pbuffer, 
              size_t        position, 
              size_t        length = etl::integral_limits<T>::bits)
    {
      typedef typename etl::make_unsigned<T>::type unsigned_t;

      const unsigned_t Mask  = etl::make_lsb_mask<unsigned_t>(length);
      const unsigned_t Shift = position % Bits_Per_Element;

      unsigned_t value = static_cast<unsigned_t>(*pbuffer >> Shift) & Mask;

      if ((length != etl::integral_limits<T>::bits) && etl::integral_limits<T>::is_signed)
      {
        value = etl::sign_extend<T>(value, length);
      }

      return static_cast<T>(value);
    }

    //*************************************************************************
    /// Extract an integral value from an arbitrary position and length.
    //*************************************************************************
#if ETL_USING_CPP11
    template <typename T, size_t Position, size_t Length = etl::integral_limits<T>::bits>
#else
		template <typename T, size_t Position, size_t Length>
#endif
    ETL_CONSTEXPR14
    static
    T extract(const_pointer pbuffer)
    {
      typedef typename etl::make_unsigned<T>::type unsigned_t;

      const unsigned_t Mask  = etl::make_lsb_mask<unsigned_t>(Length);
      const unsigned_t Shift = Position % Bits_Per_Element;

      unsigned_t value = static_cast<unsigned_t>(*pbuffer >> Shift) & Mask;

      if ((Length != etl::integral_limits<T>::bits) && etl::integral_limits<T>::is_signed)
      {
        value = etl::sign_extend<T>(value, Length);
      }

      return static_cast<T>(value);
    }

    //*************************************************************************
    /// Tests a bit at a position.
    /// Positions greater than the number of configured bits will return <b>false</b>.
    //*************************************************************************
    ETL_CONSTEXPR14 
    static 
    bool test(const_pointer pbuffer, 
              size_t        position)
    {
      const element_type mask = element_type(element_type(1) << position);
      return (*pbuffer & mask) != 0U;
    }

    //*************************************************************************
    /// Count the number of bits set.
    //*************************************************************************
    ETL_CONSTEXPR14
    static
    size_t count(const_pointer pbuffer, 
                 size_t        /*number_of_elements*/) ETL_NOEXCEPT
    {
      return etl::count_bits(*pbuffer);
    }

    //*************************************************************************
    // Are all the bits sets?
    //*************************************************************************
    ETL_CONSTEXPR14
    static
    bool all(const_pointer pbuffer, 
             size_t        /*number_of_elements*/, 
             element_type  top_mask) ETL_NOEXCEPT
    {
      return (*pbuffer & top_mask) == top_mask;
    }

    //*************************************************************************
    // Are all the mask bits sets?
    //*************************************************************************
    ETL_CONSTEXPR14 
    static 
    bool all(const_pointer pbuffer, 
             size_t        /*number_of_elements*/, 
             element_type  top_mask, 
             element_type  mask) ETL_NOEXCEPT
    {
      return (*pbuffer & top_mask & mask) == mask;
    }

    //*************************************************************************
    /// Are none of the bits set?
    //*************************************************************************
    ETL_CONSTEXPR14 
    static 
    bool none(const_pointer pbuffer, 
              size_t        /*number_of_elements*/) ETL_NOEXCEPT
    {
      return *pbuffer == All_Clear_Element;
    }

    //*************************************************************************
    /// Are none of the mask bits set?
    //*************************************************************************
    ETL_CONSTEXPR14 
    static 
    bool none(const_pointer pbuffer, 
              size_t        /*number_of_elements*/, 
              element_type  mask) ETL_NOEXCEPT
    {
      return (*pbuffer & mask) == All_Clear_Element;
    }

    //*************************************************************************
    /// Are any of the bits set?
    //*************************************************************************
    ETL_CONSTEXPR14 
    static 
    bool any(const_pointer pbuffer, 
             size_t        /*number_of_elements*/) ETL_NOEXCEPT
    {
      return *pbuffer != All_Clear_Element;
    }

    //*************************************************************************
    /// Are any of the mask bits set?
    //*************************************************************************
    ETL_CONSTEXPR14 
    static 
    bool any(const_pointer pbuffer, 
             size_t        /*number_of_elements*/, 
             element_type  mask) ETL_NOEXCEPT
    {
      return (*pbuffer & mask) != All_Clear_Element;
    }

    //*************************************************************************
    /// Flip all of the bits.
    //*************************************************************************
    ETL_CONSTEXPR14 
    static 
    void flip_all(pointer pbuffer, 
                     size_t  /*number_of_elements*/) ETL_NOEXCEPT
    {
      *pbuffer = ~*pbuffer;
    }

    //*************************************************************************
    /// Flip some of the bits.
    //*************************************************************************
    ETL_CONSTEXPR14 
    static 
    void flip_bits(pointer      pbuffer, 
                   element_type mask = etl::integral_limits<element_type>::max) ETL_NOEXCEPT
    {
      *pbuffer ^= mask;
    }

    //*************************************************************************
    /// Flip the bit at the position.
    //*************************************************************************
    ETL_CONSTEXPR14 
    static 
    void flip_position(pointer pbuffer, 
                       size_t  position)
    {
      const element_type mask = element_type(element_type(1) << position);
      *pbuffer ^= mask;
    }

    //*************************************************************************
    /// Returns a string representing the bitset.
    //*************************************************************************
    template <typename TString>
    ETL_CONSTEXPR14 
    static 
    TString to_string(const_pointer pbuffer,
                      size_t active_bits,
                      typename TString::value_type zero = typename TString::value_type('0'),
                      typename TString::value_type one  = typename TString::value_type('1'))
    {
      TString result;

      result.resize(active_bits, '\0');

      // Check that the string type can contain the digits.
      ETL_ASSERT_OR_RETURN_VALUE(result.size() == active_bits, ETL_ERROR(etl::bitset_string_too_small), result);

      for (size_t i = active_bits; i > 0; --i)
      {
        result[active_bits - i] = test(pbuffer, i - 1) ? one : zero;
      }

      return result;
    }

    //*************************************************************************
    /// Finds the next bit in the specified state.
    ///\param state    The state to search for.
    ///\param position The position to start from.
    ///\returns The position of the bit or npos if none were found.
    //*************************************************************************
    ETL_CONSTEXPR14 
    static 
    size_t find_next(const_pointer pbuffer, 
                     size_t        /*number_of_elements*/, 
                     size_t        active_bits, 
                     bool          state, 
                     size_t        position) ETL_NOEXCEPT
    {
      if (position < active_bits)
      {
        // Where to start.
        size_t bit = position;

        element_type mask = 1U << position;

        // Needs checking?
        if ((state && (*pbuffer != All_Clear_Element)) || (!state && (*pbuffer != All_Set_Element)))
        {
          // For each bit in the element...
          while (bit < active_bits)
          {
            // Equal to the required state?
            if (((*pbuffer & mask) != 0) == state)
            {
              return bit;
            }

            // Move on to the next bit.
            mask <<= 1;
            ++bit;
          }
        }
      }

      return npos;
    }

    //*************************************************************************
    /// operator assignment
    /// Assigns rhs to lhs
    //*************************************************************************
    ETL_CONSTEXPR14 
    static 
    void operator_assignment(pointer       lhs_pbuffer, 
                             const_pointer rhs_pbuffer, 
                             size_t        /*number_of_elements*/) ETL_NOEXCEPT
    {
      *lhs_pbuffer = *rhs_pbuffer;
    }

    //*************************************************************************
    /// operator and
    /// AND lhs and rhs and put the result in lhs
    //*************************************************************************
    ETL_CONSTEXPR14 
    static 
    void operator_and(pointer       lhs_pbuffer, 
                      const_pointer rhs_pbuffer, 
                      size_t        /*number_of_elements*/) ETL_NOEXCEPT
    {
      *lhs_pbuffer &= *rhs_pbuffer;
    }

    //*************************************************************************
    /// operator or
    /// OR lhs and rhs and put the result in lhs
    //*************************************************************************
    ETL_CONSTEXPR14 
    static 
    void operator_or(pointer       lhs_pbuffer, 
                     const_pointer rhs_pbuffer, 
                     size_t        /*number_of_elements*/) ETL_NOEXCEPT
    {
      *lhs_pbuffer |= *rhs_pbuffer;
    }

    //*************************************************************************
    /// operator xor
    /// XOR lhs and rhs and put the result in lhs
    //*************************************************************************
    ETL_CONSTEXPR14 
    static 
    void operator_xor(pointer       lhs_pbuffer, 
                      const_pointer rhs_pbuffer, 
                      size_t        /*number_of_elements*/) ETL_NOEXCEPT
    {
      *lhs_pbuffer ^= *rhs_pbuffer;
    }

    //*************************************************************************
    /// operator ~
    /// NOT the value in the buffer
    //*************************************************************************
    ETL_CONSTEXPR14 
    static 
    void operator_not(pointer pbuffer, 
                      size_t  /*number_of_elements*/) ETL_NOEXCEPT
    {
      *pbuffer = ~*pbuffer;
    }

    //*************************************************************************
    /// operator_shift_left
    //*************************************************************************
    ETL_CONSTEXPR14 
    static 
    void operator_shift_left(pointer pbuffer, 
                             size_t  /*number_of_elements*/, 
                             size_t  active_bits, 
                             size_t  shift) ETL_NOEXCEPT
    {
      if (shift >= active_bits)
      {
        reset_all(pbuffer, 1U);
      }
      else
      {
        *pbuffer <<= shift;
      }
    }

    //*************************************************************************
    /// operator_shift_right
    //*************************************************************************
    ETL_CONSTEXPR14 
    static 
    void operator_shift_right(pointer pbuffer, 
                              size_t  /*number_of_elements*/, 
                              size_t  active_bits, 
                              size_t  shift) ETL_NOEXCEPT
    {
      if (shift >= active_bits)
      {
        reset_all(pbuffer, 1U);
      }
      else
      {
        *pbuffer >>= shift;
      }
    }

    //*************************************************************************
    /// operator_equality
    //*************************************************************************
    ETL_CONSTEXPR14 
    static
    bool operator_equality(const_pointer lhs_pbuffer, 
                           const_pointer rhs_pbuffer, 
                           size_t        /*number_of_elements*/) ETL_NOEXCEPT
    {
      return (*lhs_pbuffer == *rhs_pbuffer);
    }

    //*************************************************************************
    /// Initialise from an unsigned long long.
    //*************************************************************************
    template <typename TElementType>
    ETL_CONSTEXPR14 
    static 
    void initialise(pointer            pbuffer, 
                    size_t             /*number_of_elements*/,
                    unsigned long long value) ETL_NOEXCEPT
    {
      *pbuffer = static_cast<TElementType>(value);
    }

    //*************************************************************************
    /// swap
    //*************************************************************************
    ETL_CONSTEXPR14 
    static
    void swap(pointer lhs_pbuffer, 
              pointer rhs_pbuffer, 
              size_t  /*number_of_elements*/) ETL_NOEXCEPT
    {
      element_type temp = *lhs_pbuffer;
      *lhs_pbuffer = *rhs_pbuffer;
      *rhs_pbuffer = temp;
    }
  };

  //*************************************************************************
  /// The implementation class for multi element etl::bitset
  ///\ingroup bitset
  //*************************************************************************
  template <typename TElement>
  class bitset_impl<TElement, etl::bitset_storage_model::Multi> : public etl::private_bitset::bitset_impl_common<TElement>
  {
  private:

    typedef etl::private_bitset::bitset_impl_common<TElement> common;

  public:

    using typename etl::private_bitset::bitset_impl_common<TElement>::element_type;
    using typename etl::private_bitset::bitset_impl_common<TElement>::pointer;
    using typename etl::private_bitset::bitset_impl_common<TElement>::const_pointer;

    using etl::private_bitset::bitset_impl_common<TElement>::Bits_Per_Element;
    using etl::private_bitset::bitset_impl_common<TElement>::All_Set_Element;
    using etl::private_bitset::bitset_impl_common<TElement>::All_Clear_Element;

    using etl::private_bitset::bitset_impl_common<TElement>::npos;

    //*************************************************************************
    /// Check to see if the requested extract is contained within one element.
    //*************************************************************************
    template <size_t Position, size_t Length, size_t Bits_Per_Element>
    struct value_is_in_one_element
    {
      static ETL_CONSTANT bool value = ((Position + Length - 1) >> etl::log2<Bits_Per_Element>::value) == (Position >> etl::log2<Bits_Per_Element>::value);
    };

    //*************************************************************************
    /// Tests a bit at a position.
    /// Positions greater than the number of configured bits will return <b>false</b>.
    //*************************************************************************
    ETL_CONSTEXPR14 
    static
    bool test(const_pointer pbuffer,
              size_t        position) ETL_NOEXCEPT
    {
      size_t index      = position >> etl::log2<Bits_Per_Element>::value;
      element_type mask = element_type(1) << (position & (Bits_Per_Element - 1));

      return (pbuffer[index] & mask) != 0;
    }

    //*************************************************************************
    /// Count the number of bits set.
    //*************************************************************************
    ETL_CONSTEXPR14
    static
    size_t count(const_pointer pbuffer,
                 size_t        number_of_elements) ETL_NOEXCEPT
    {
      size_t count = 0;

      while (number_of_elements-- != 0)
      {
        count += etl::count_bits(*pbuffer++);
      }

      return count;
    }

    //*************************************************************************
    // Are all the bits sets?
    //*************************************************************************
    ETL_CONSTEXPR14
    static
    bool all(const_pointer pbuffer,
             size_t        number_of_elements,
             element_type  top_mask) ETL_NOEXCEPT
    {
      // All but the last.
      while (number_of_elements-- != 1U)
      {
        if (*pbuffer++ != All_Set_Element)
        {
          return false;
        }
      }

      // The last.
      if ((*pbuffer & top_mask) != top_mask)
      {
        return false;
      }

      return true;
    }

    //*************************************************************************
    /// Are none of the bits set?
    //*************************************************************************
    ETL_CONSTEXPR14
      static
      bool none(const_pointer pbuffer,
                size_t        number_of_elements) ETL_NOEXCEPT
    {
      while (number_of_elements-- != 0)
      {
        if (*pbuffer++ != 0)
        {
          return false;
        }
      }

      return true;
    }

    //*************************************************************************
    /// Are any of the bits set?
    //*************************************************************************
    ETL_CONSTEXPR14
    static
    bool any(const_pointer pbuffer,
             size_t        number_of_elements) ETL_NOEXCEPT
    {
      bool any_set = false;

      while (number_of_elements-- != 0)
      {
        if (*pbuffer++ != All_Clear_Element)
        {
          any_set = true;
          break;
        }
      }

      return any_set;
    }

    //*************************************************************************
    /// Set all bits
    //*************************************************************************
    ETL_CONSTEXPR14
    static
    void set_all(pointer      pbuffer,
                 size_t       number_of_elements,
                 element_type top_mask) ETL_NOEXCEPT
    {
      while (number_of_elements-- != 1U)
      {
        *pbuffer++ = All_Set_Element;
      }

      *pbuffer = (All_Set_Element & top_mask);
    }

    //*************************************************************************
    /// Set the bit at the position.
    //*************************************************************************
    ETL_CONSTEXPR14
    static
    void set_position(pointer pbuffer, 
                      size_t  position, 
                      bool    value = true) ETL_NOEXCEPT
    {
      size_t       index = position >> etl::log2<Bits_Per_Element>::value;
      element_type bit   = element_type(1) << (position & (Bits_Per_Element - 1));

      if (value == true)
      {
        pbuffer[index] |= bit;
      }
      else
      {
        pbuffer[index] &= ~bit;
      }
    }

    //*************************************************************************
    /// Set the bit at the position.
    //*************************************************************************
    template <size_t Position>
    ETL_CONSTEXPR14
    static
    void set_position(pointer pbuffer,
                      bool    value = true)
    {
      size_t       index = Position >> etl::log2<Bits_Per_Element>::value;
      element_type bit   = element_type(1) << (Position & (Bits_Per_Element - 1));

      if (value == true)
      {
        pbuffer[index] |= bit;
      }
      else
      {
        pbuffer[index] &= ~bit;
      }
    }

    //*************************************************************************
    /// Set the bit at the position.
    //*************************************************************************
    template <size_t Position, bool Value>
    ETL_CONSTEXPR14
    static
    void set_position(pointer pbuffer)
    {
      size_t       index = Position >> etl::log2<Bits_Per_Element>::value;
      element_type bit = element_type(1) << (Position & (Bits_Per_Element - 1));

      if (Value == true)
      {
        pbuffer[index] |= bit;
      }
      else
      {
        pbuffer[index] &= ~bit;
      }
    }

    //*************************************************************************
    /// Set from a string.
    //*************************************************************************
    ETL_CONSTEXPR14 
    static 
    void from_string(pointer     pbuffer, 
                     size_t      number_of_elements, 
                     size_t      total_bits, 
                     const char* text) ETL_NOEXCEPT
    {
      if (text == ETL_NULLPTR)
      {
        etl::fill_n(pbuffer, number_of_elements, All_Clear_Element);
      }
      else
      {
        size_t string_length = etl::strlen(text);
        size_t index = etl::min(number_of_elements - 1U, (string_length / Bits_Per_Element));

        // Only reset elements we need to.
        while (index != number_of_elements)
        {
          pbuffer[index++] = All_Clear_Element;
        }

        // Build from the string.
        size_t i = etl::min(total_bits, string_length);

        while (i > 0)
        {
          set_position(pbuffer, --i, *text++ == ETL_STR('1'));
        }
      }
    }

    //*************************************************************************
    /// Set from a wide string.
    //*************************************************************************
    ETL_CONSTEXPR14
    static
    void from_string(pointer        pbuffer, 
                     size_t         number_of_elements, 
                     size_t         total_bits, 
                     const wchar_t* text) ETL_NOEXCEPT
    {
      if (text == ETL_NULLPTR)
      {
        etl::fill_n(pbuffer, number_of_elements, All_Clear_Element);
      }
      else
      {
        size_t string_length = etl::strlen(text);
        size_t index = etl::min(number_of_elements - 1U, (string_length / Bits_Per_Element));

        // Only reset elements we need to.
        while (index != number_of_elements)
        {
          pbuffer[index++] = All_Clear_Element;
        }

        // Build from the string.
        size_t i = etl::min(total_bits, string_length);

        while (i > 0)
        {
          set_position(pbuffer, --i, *text++ == ETL_STRL('1'));
        }
      }
    }

    //*************************************************************************
    /// Set from a u16 string.
    //*************************************************************************
    ETL_CONSTEXPR14
    static
    void from_string(pointer         pbuffer, 
                     size_t          number_of_elements, 
                     size_t          total_bits, 
                     const char16_t* text) ETL_NOEXCEPT
    {
      if (text == ETL_NULLPTR)
      {
        etl::fill_n(pbuffer, number_of_elements, All_Clear_Element);
      }
      else
      {
        size_t string_length = etl::strlen(text);
        size_t index = etl::min(number_of_elements - 1U, (string_length / Bits_Per_Element));

        // Only reset elements we need to.
        while (index != number_of_elements)
        {
          pbuffer[index++] = All_Clear_Element;
        }

        // Build from the string.
        size_t i = etl::min(total_bits, string_length);

        while (i > 0)
        {
          set_position(pbuffer, --i, *text++ == ETL_STRu('1'));
        }
      }
    }

    //*************************************************************************
    /// Set from a u32 string.
    //*************************************************************************
    ETL_CONSTEXPR14
    static
    void from_string(pointer         pbuffer, 
                     size_t          number_of_elements, 
                     size_t          total_bits, 
                     const char32_t* text) ETL_NOEXCEPT
    {
      if (text == ETL_NULLPTR)
      {
        etl::fill_n(pbuffer, number_of_elements, All_Clear_Element);
      }
      else
      {
        size_t string_length = etl::strlen(text);
        size_t index = etl::min(number_of_elements - 1U, (string_length / Bits_Per_Element));

        // Only reset elements we need to.
        while (index != number_of_elements)
        {
          pbuffer[index++] = All_Clear_Element;
        }

        // Build from the string.
        size_t i = etl::min(total_bits, string_length);

        while (i > 0)
        {
          set_position(pbuffer, --i, *text++ == ETL_STRU('1'));
        }
      }
    }

    //*************************************************************************
    /// Reset all of the bits.
    //*************************************************************************
    ETL_CONSTEXPR14
    static
    void reset_all(pointer pbuffer,
                   size_t  number_of_elements) ETL_NOEXCEPT
    {
      while (number_of_elements-- != 0U)
      {
        *pbuffer++ = All_Clear_Element;
      }
    }

    //*************************************************************************
    /// Reset the bit at the position.
    //*************************************************************************
    ETL_CONSTEXPR14
    static
    void reset_position(pointer pbuffer, 
                        size_t  position) ETL_NOEXCEPT
    {
      const size_t       index = position >> etl::log2<Bits_Per_Element>::value;;
      const element_type bit = element_type(1) << (position & (Bits_Per_Element - 1));

      pbuffer[index] &= ~bit;
    }

    //*************************************************************************
    /// Get as a value.
    //*************************************************************************
    template <typename T>
    ETL_CONSTEXPR14
    static
    T value(const_pointer pbuffer, 
            size_t        number_of_elements) ETL_NOEXCEPT
    {
      T v = T(0);

      const bool OK = (sizeof(T) * CHAR_BIT) >= (number_of_elements * Bits_Per_Element);

      if (OK)
      {
        uint_least8_t shift = 0U;

        for (size_t i = 0UL; i < number_of_elements; ++i)
        {
          v |= T(typename etl::make_unsigned<T>::type(pbuffer[i]) << shift);
          shift += uint_least8_t(Bits_Per_Element);
        }
      }

      return v;
    }

    //*************************************************************************
    /// Extract an value from multiple elements.
    //*************************************************************************
    template <typename T>
    ETL_CONSTEXPR14
    static 
    typename etl::make_unsigned<T>::type extract_from_multiple_elements(const element_type* pbuffer,
                                                                        int                 element_index,
                                                                        size_t              active_bits_in_msb,
                                                                        size_t              length) ETL_NOEXCEPT
    {
      typedef typename etl::make_unsigned<T>::type unsigned_t;

      unsigned_t value(0);

      // Extract the first element, if partially filled.
      if (active_bits_in_msb < Bits_Per_Element)
      {
        element_type mask = etl::make_lsb_mask< element_type>(active_bits_in_msb);
        value = pbuffer[element_index] & mask;
        length -= active_bits_in_msb;
        if (length >= Bits_Per_Element)
        {
          value = value << Bits_Per_Element;
        }
        --element_index;
      }

      // Loop through the fully filled elements
      while (length >= Bits_Per_Element)
      {
        value |= pbuffer[element_index];
        length -= Bits_Per_Element;
        if (length >= Bits_Per_Element)
        {
          value = value << Bits_Per_Element;
        }
        --element_index;
      }

      // Extract the last element, if partially filled.
      if (length != 0)
      {
        value = value << length;
        element_type mask = etl::make_lsb_mask< element_type>(length);
        value |= (pbuffer[element_index] >> (Bits_Per_Element - length)) & mask;
      }

      return value;
    }

    //*************************************************************************
    /// Extract an integral value from an arbitrary position and length.
    /// Run time position and length.
    //*************************************************************************
    template <typename T>
    ETL_CONSTEXPR14
    static
    typename etl::make_unsigned<T>::type extract_from_buffer(const_pointer pbuffer, 
                                                             size_t        position, 
                                                             size_t        length) ETL_NOEXCEPT
    {
      typedef typename etl::make_unsigned<T>::type unsigned_t;

      unsigned_t value(0);

      const int Msb_Element_Index = (position + length - 1) >> etl::log2<Bits_Per_Element>::value;
      const int Lsb_Element_Index = position >> etl::log2<Bits_Per_Element>::value;

      // Is the value contained within one element?
      if (Msb_Element_Index == Lsb_Element_Index)
      {
        const unsigned_t Mask  = etl::make_lsb_mask< unsigned_t>(length);
        const unsigned_t Shift = position % Bits_Per_Element;

        value = static_cast<unsigned_t>(pbuffer[Msb_Element_Index] >> Shift) & Mask;
      }
      else
      {
        // Get the number of active bits in the msb element 
        size_t active_bits_in_msb = (position + length) - (Msb_Element_Index * Bits_Per_Element);

        // Start with index of the element containing the msb.
        int element_index = Msb_Element_Index;

        value = extract_from_multiple_elements<T>(pbuffer, element_index, active_bits_in_msb, length);
      }

      return value;
    }

    //*************************************************************************
    /// Extract an integral value when the value fits in one element.
    /// Compile time position and length.
    //*************************************************************************
    template <typename T, size_t Position, size_t Length>
    ETL_CONSTEXPR14
    static
    typename etl::enable_if<value_is_in_one_element<Position, Length, Bits_Per_Element>::value, typename etl::make_unsigned<T>::type>::type
      extract_from_buffer(const_pointer pbuffer)
    {
      typedef typename etl::make_unsigned<T>::type unsigned_t;

      const int        Element_Index = (Position + Length - 1) >> etl::log2<Bits_Per_Element>::value;
      const unsigned_t Mask          = etl::lsb_mask<unsigned_t, Length>::value;
      const unsigned_t Shift         = Position % Bits_Per_Element;
      
      return  static_cast<unsigned_t>(pbuffer[Element_Index] >> Shift) & Mask;
    }

    //*************************************************************************
    /// Extract an integral value when the value spans more than one element.
    /// Compile time position and length.
    //*************************************************************************
    template <typename T, size_t Position, size_t Length>
    ETL_CONSTEXPR14
    static
    typename etl::enable_if<!value_is_in_one_element<Position, Length, Bits_Per_Element>::value, typename etl::make_unsigned<T>::type>::type
      extract_from_buffer(const_pointer pbuffer)
    {      
      // Start with index of the element containing the msb.
      const int Msb_Element_Index = (Position + Length - 1) >> etl::log2<Bits_Per_Element>::value;

      // Get the number of active bits in the first element 
      const size_t Active_Bits_In_Msb = ((Position + Length - 1) % Bits_Per_Element) + 1;

      return extract_from_multiple_elements<T>(pbuffer, Msb_Element_Index, Active_Bits_In_Msb, Length);
    }

    //*************************************************************************
    /// Extract an integral value from an arbitrary position and length.
    //*************************************************************************
    template <typename T>
    ETL_CONSTEXPR14
    static
    T extract(const_pointer pbuffer,
              size_t        position,
              size_t        length = etl::integral_limits<T>::bits)
    {
      typedef typename etl::make_unsigned<T>::type unsigned_t;

      unsigned_t value = extract_from_buffer<unsigned_t>(pbuffer, position, length);

      if ((length != etl::integral_limits<T>::bits) && etl::integral_limits<T>::is_signed)
      {
        value = etl::sign_extend<T>(value, length);
      }

      return static_cast<T>(value);
    }

    //*************************************************************************
    /// Extract an integral value from an arbitrary position and length.
    //*************************************************************************
    template <typename T, size_t Position, size_t Length>
    ETL_CONSTEXPR14
    static
    T extract(const_pointer pbuffer)
    {
      typedef typename etl::make_unsigned<T>::type unsigned_t;

      unsigned_t value = extract_from_buffer<unsigned_t, Position, Length>(pbuffer);

      if ((Length != etl::integral_limits<T>::bits) && etl::integral_limits<T>::is_signed)
      {
        value = etl::sign_extend<T>(value, Length);
      }

      return static_cast<T>(value);
    }

    //*************************************************************************
    /// Flip all of the bits.
    //*************************************************************************
    ETL_CONSTEXPR14
    static
    void flip_all(pointer pbuffer, 
                     size_t  number_of_elements) ETL_NOEXCEPT
    {
      operator_not(pbuffer, number_of_elements);
    }

    //*************************************************************************
    /// Flip the bit at the position.
    //*************************************************************************
    ETL_CONSTEXPR14
    static
    void flip_position(pointer pbuffer, 
                       size_t  position) ETL_NOEXCEPT
    {
      const size_t       index = position >> etl::log2<Bits_Per_Element>::value;;
      const element_type bit   = element_type(1) << (position & (Bits_Per_Element - 1));

      pbuffer[index] ^= bit;
    }

    //*************************************************************************
    /// Finds the next bit in the specified state.
    ///\param state    The state to search for.
    ///\param position The position to start from.
    ///\returns The position of the bit or npos if none were found.
    //*************************************************************************
    ETL_CONSTEXPR14
    static
    size_t find_next(const_pointer pbuffer, 
                     size_t        number_of_elements, 
                     size_t        total_bits, 
                     bool          state, 
                     size_t        position) ETL_NOEXCEPT
    {
      // Where to start.
      size_t index = position >> log2<Bits_Per_Element>::value;
      size_t bit   = position & (Bits_Per_Element - 1);

      element_type mask = 1 << bit;

      // For each element in the bitset...
      while (index < number_of_elements)
      {
        element_type value = pbuffer[index];

        // Needs checking?
        if ((state && (value != All_Clear_Element)) ||
            (!state && (value != All_Set_Element)))
        {
          // For each bit in the element...
          while ((bit < Bits_Per_Element) && (position < total_bits))
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

      return npos;
    }

    //*************************************************************************
    /// Returns a string representing the bitset.
    //*************************************************************************
    template <typename TString>
    ETL_CONSTEXPR14
    static
    TString to_string(const_pointer                pbuffer,
                      size_t                       active_bits, 
                      typename TString::value_type zero, 
                      typename TString::value_type one)
    {
      TString result;

      result.resize(active_bits, '\0');

      // Check that the string type can contain the digits.
      ETL_ASSERT_OR_RETURN_VALUE(result.size() == active_bits, ETL_ERROR(etl::bitset_string_too_small), result);      

      for (size_t i = active_bits; i > 0; --i)
      {
        result[active_bits - i] = test(pbuffer, i - 1) ? one : zero;
      }

      return result;
    }

    //*************************************************************************
    /// operator assignment
    //*************************************************************************
    ETL_CONSTEXPR14
    static
    void operator_assignment(pointer       lhs_pbuffer, 
                             const_pointer rhs_pbuffer, 
                             size_t        number_of_elements) ETL_NOEXCEPT
    {
      while (number_of_elements-- != 0)
      {
        *lhs_pbuffer = *rhs_pbuffer;
        ++lhs_pbuffer;
        ++rhs_pbuffer;
      }
    }

    //*************************************************************************
    /// operator and
    //*************************************************************************
    ETL_CONSTEXPR14
    static
    void operator_and(pointer       lhs_pbuffer, 
                      const_pointer rhs_pbuffer, 
                      size_t        number_of_elements) ETL_NOEXCEPT
    {
      while (number_of_elements-- != 0)
      {
        *lhs_pbuffer &= *rhs_pbuffer;
        ++lhs_pbuffer;
        ++rhs_pbuffer;
      }
    }

    //*************************************************************************
    /// operator or
    //*************************************************************************
    ETL_CONSTEXPR14
    static
    void operator_or(pointer       lhs_pbuffer, 
                     const_pointer rhs_pbuffer, 
                     size_t        number_of_elements) ETL_NOEXCEPT
    {
      while (number_of_elements-- != 0)
      {
        *lhs_pbuffer |= *rhs_pbuffer;
        ++lhs_pbuffer;
        ++rhs_pbuffer;
      }
    }

    //*************************************************************************
    /// operator xor
    //*************************************************************************
    ETL_CONSTEXPR14
    static
    void operator_xor(pointer       lhs_pbuffer, 
                      const_pointer rhs_pbuffer, 
                      size_t        number_of_elements) ETL_NOEXCEPT
    {
      while (number_of_elements-- != 0)
      {
        *lhs_pbuffer ^= *rhs_pbuffer;
        ++lhs_pbuffer;
        ++rhs_pbuffer;
      }
    }

    //*************************************************************************
    /// operator not
    //*************************************************************************
    ETL_CONSTEXPR14
    static
    void operator_not(pointer pbuffer, 
                      size_t  number_of_elements) ETL_NOEXCEPT
    {
      while (number_of_elements-- != 0)
      {
        *pbuffer = ~*pbuffer;
        ++pbuffer;
      }
    }

    //*************************************************************************
    /// operator_shift_left
    //*************************************************************************
    ETL_CONSTEXPR14
    static
    void operator_shift_left(pointer pbuffer, 
                             size_t  number_of_elements, 
                             size_t  active_bits, 
                             size_t  shift) ETL_NOEXCEPT
    {
      if (shift >= active_bits)
      {
        reset_all(pbuffer, number_of_elements);
      }
      else
      {
        // The place where the elements are split when shifting.
        const size_t split_position = Bits_Per_Element - (shift % Bits_Per_Element);

        // Where we are shifting from.
        int src_index = int(number_of_elements - (shift / Bits_Per_Element) - 1U);

        // Where we are shifting to.
        int dst_index = int(number_of_elements - 1U);

        // Shift control constants.
        const size_t lsb_shift = Bits_Per_Element - split_position;
        const size_t msb_shift = split_position;

        const element_type lsb_mask = element_type(etl::integral_limits<element_type>::max >> (Bits_Per_Element - split_position));
        const element_type msb_mask = etl::integral_limits<element_type>::max - lsb_mask;
        const element_type lsb_shifted_mask = element_type(lsb_mask << lsb_shift);

        // First lsb.
        element_type lsb = element_type((pbuffer[src_index] & lsb_mask) << lsb_shift);
        pbuffer[dst_index] = lsb;
        --src_index;

        // Now do the shifting.
        while (src_index >= 0)
        {
          // Shift msb.
          element_type msb = element_type((pbuffer[src_index] & msb_mask) >> msb_shift);
          pbuffer[dst_index] = pbuffer[dst_index] | msb;
          --dst_index;

          // Shift lsb.
          lsb = element_type((pbuffer[src_index] & lsb_mask) << lsb_shift);
          pbuffer[dst_index] = lsb;
          --src_index;
        }

        // Clear the remaining bits.
        // First lsb.
        pbuffer[dst_index] &= lsb_shifted_mask;

        // The other remaining bytes on the right.
        for (int i = 0; i < dst_index; ++i)
        {
          pbuffer[i] = 0;
        }
      }
    }

    //*************************************************************************
    /// operator_shift_right
    //*************************************************************************
    ETL_CONSTEXPR14
    static
    void operator_shift_right(pointer pbuffer, 
                              size_t  number_of_elements, 
                              size_t  active_bits, 
                              size_t  shift) ETL_NOEXCEPT
    {
      if (shift >= active_bits)
      {
        reset_all(pbuffer, number_of_elements);
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

        const element_type lsb_mask = element_type(etl::integral_limits<element_type>::max >> (Bits_Per_Element - split_position));
        const element_type msb_mask = etl::integral_limits<element_type>::max - lsb_mask;
        const element_type msb_shifted_mask = element_type(msb_mask >> msb_shift);

        // Now do the shifting.
        while (src_index < int(number_of_elements - 1))
        {
          // Shift msb.
          element_type msb = element_type((pbuffer[src_index] & msb_mask) >> msb_shift);
          ++src_index;

          // Shift lsb.
          element_type lsb = element_type((pbuffer[src_index] & lsb_mask) << lsb_shift);

          // Combine them.
          pbuffer[dst_index] = lsb | msb;
          ++dst_index;
        }

        // Final msb.
        element_type msb = element_type((pbuffer[src_index] & msb_mask) >> msb_shift);
        pbuffer[dst_index] = msb;

        // Clear the remaining bits.
        // First msb.
        pbuffer[dst_index] &= msb_shifted_mask;
        ++dst_index;

        // The other remaining bytes.
        while (dst_index < int(number_of_elements))
        {
          pbuffer[dst_index] = 0;
          ++dst_index;
        }
      }
    }

    //*************************************************************************
    /// operator_equality
    //*************************************************************************
    ETL_CONSTEXPR14
    static
    bool operator_equality(const_pointer lhs_pbuffer, 
                           const_pointer rhs_pbuffer, 
                           size_t        number_of_elements) ETL_NOEXCEPT
    {
      return etl::equal(lhs_pbuffer, lhs_pbuffer + number_of_elements, rhs_pbuffer);
    }

    //*************************************************************************
    /// Initialise from an unsigned long long.
    /// Enabled if the number of bits in the element equals the number of bits in an unsigned long long.
    //*************************************************************************
    template <typename TElementType>
    ETL_CONSTEXPR14
    static
    typename etl::enable_if<etl::integral_limits<TElementType>::bits == etl::integral_limits<unsigned long long>::bits, void>::type
      initialise(pointer            pbuffer, 
                 size_t             number_of_elements, 
                 unsigned long long value) ETL_NOEXCEPT
    {
      size_t i = 0UL;
      
      // Set the non-zero elements.
      pbuffer[i++] = value;

      // Clear the remaining elements.
      while (i != number_of_elements)
      {
        pbuffer[i++] = All_Clear_Element;
      }
    }

    //*************************************************************************
    /// Initialise from an unsigned long long.
    /// Enabled if the number of bits in the element doesn't equal the number of bits in an unsigned long long.
    //*************************************************************************
    template <typename TElementType>
    ETL_CONSTEXPR14
    static
    typename etl::enable_if<etl::integral_limits<TElementType>::bits != etl::integral_limits<unsigned long long>::bits, void>::type
      initialise(pointer            pbuffer,
                 size_t             number_of_elements,
                 unsigned long long value) ETL_NOEXCEPT
    {
      size_t i = 0UL;

      // Set the non-zero elements.
      const unsigned long long Shift = etl::integral_limits<element_type>::bits;

      while ((value != 0) && (i != number_of_elements))
      {
        pbuffer[i++] = value & All_Set_Element;
        value = value >> Shift;
      }

      // Clear the remaining elements.
      while (i != number_of_elements)
      {
        pbuffer[i++] = All_Clear_Element;
      }
    }

    //*************************************************************************
    /// Swap bitset buffers.
    //*************************************************************************
    ETL_CONSTEXPR14
    static
    void swap(pointer pbuffer1, 
              pointer pbuffer2, 
              size_t  number_of_elements)
    {
      etl::swap_ranges(pbuffer1, pbuffer1 + number_of_elements, pbuffer2);
    }
  };

  namespace private_bitset
  {
    //***************************************************************************
    template <size_t Active_Bits, typename TElement>
    class bitset_common : public etl::private_bitset::bitset_impl_common<TElement>
    {
    public:

      typedef typename etl::private_bitset::bitset_impl_common<TElement>::element_type element_type;
      
      using etl::private_bitset::bitset_impl_common<TElement>::Bits_Per_Element;
      using etl::private_bitset::bitset_impl_common<TElement>::All_Set_Element;
      using etl::private_bitset::bitset_impl_common<TElement>::All_Clear_Element;

      static ETL_CONSTANT size_t Number_Of_Elements = (Active_Bits % Bits_Per_Element == 0) ? Active_Bits / Bits_Per_Element : Active_Bits / Bits_Per_Element + 1;
      static ETL_CONSTANT size_t Size               = Active_Bits;
      static ETL_CONSTANT size_t Allocated_Bits     = Number_Of_Elements * Bits_Per_Element;

#if ETL_USING_CPP11
      static ETL_CONSTANT etl::bitset_storage_model Storage_Model = (bitset_common<Active_Bits, TElement>::Number_Of_Elements == 1U) ? etl::bitset_storage_model::Single : etl::bitset_storage_model::Multi;
#else
      static ETL_CONSTANT etl::bitset_storage_model Storage_Model;
#endif

      typedef etl::span<element_type,       Number_Of_Elements> span_type;
      typedef etl::span<const element_type, Number_Of_Elements> const_span_type;

    private:

      static ETL_CONSTANT size_t Top_Mask_Shift = ((Bits_Per_Element - ((Number_Of_Elements * Bits_Per_Element) - Active_Bits)) % Bits_Per_Element);

    public:

      static ETL_CONSTANT TElement Top_Mask = element_type(Top_Mask_Shift == 0 ? All_Set_Element : ~(All_Set_Element << Top_Mask_Shift));
    };


    template <size_t Active_Bits, typename TElement>
    ETL_CONSTANT size_t bitset_common<Active_Bits, TElement>::Number_Of_Elements;

    template <size_t Active_Bits, typename TElement>
    ETL_CONSTANT size_t bitset_common<Active_Bits, TElement>::Size;

#if ETL_USING_CPP11
    template <size_t Active_Bits, typename TElement>
    ETL_CONSTANT etl::bitset_storage_model bitset_common<Active_Bits, TElement>::Storage_Model;
#else
    template <size_t Active_Bits, typename TElement>
    ETL_CONSTANT etl::bitset_storage_model bitset_common<Active_Bits, TElement>::Storage_Model = (bitset_common<Active_Bits, TElement>::Number_Of_Elements == 1U) ? etl::bitset_storage_model::Single : etl::bitset_storage_model::Multi;
#endif

    template <size_t Active_Bits, typename TElement>
    ETL_CONSTANT size_t bitset_common<Active_Bits, TElement>::Top_Mask_Shift;

    template <size_t Active_Bits, typename TElement>
    ETL_CONSTANT TElement bitset_common<Active_Bits, TElement>::Top_Mask;  
  }

  //***************************************************************************
  /// Bitset forward declaration
  //***************************************************************************
  template <size_t   Active_Bits = 0U,
            typename TElement    = unsigned char>
  class bitset;

  //***************************************************************************
  /// Specialisation for zero bits.
  //***************************************************************************
  template <>
  class bitset<0U, unsigned char> : public etl::private_bitset::bitset_common<0U, unsigned char>
  {
  public:

    typedef etl::private_bitset::bitset_common<0U, unsigned char>::element_type    element_type;
    typedef etl::private_bitset::bitset_common<0U, unsigned char>::span_type       span_type;
    typedef etl::private_bitset::bitset_common<0U, unsigned char>::const_span_type const_span_type;

    using etl::private_bitset::bitset_common<0U, unsigned char>::Bits_Per_Element;
    using etl::private_bitset::bitset_common<0U, unsigned char>::All_Set_Element;
    using etl::private_bitset::bitset_common<0U, unsigned char>::All_Clear_Element;
    using etl::private_bitset::bitset_common<0U, unsigned char>::Number_Of_Elements;
    using etl::private_bitset::bitset_common<0U, unsigned char>::Size;
    using etl::private_bitset::bitset_common<0U, unsigned char>::Storage_Model;
    using etl::private_bitset::bitset_common<0U, unsigned char>::Top_Mask;
    using etl::private_bitset::bitset_common<0U, unsigned char>::Allocated_Bits;
  };

  //*************************************************************************
  /// The bitset top level template.
  //*************************************************************************
  template <size_t Active_Bits, typename TElement>
  class bitset : public etl::private_bitset::bitset_common<Active_Bits, TElement>
  {
  public:

    ETL_STATIC_ASSERT(etl::is_unsigned<TElement>::value, "The element type must be unsigned");

    typedef typename etl::private_bitset::bitset_common<Active_Bits, TElement>::element_type    element_type;
    typedef typename etl::private_bitset::bitset_common<Active_Bits, TElement>::span_type       span_type;
    typedef typename etl::private_bitset::bitset_common<Active_Bits, TElement>::const_span_type const_span_type;

    using etl::private_bitset::bitset_common<Active_Bits, TElement>::Bits_Per_Element;
    using etl::private_bitset::bitset_common<Active_Bits, TElement>::All_Set_Element;
    using etl::private_bitset::bitset_common<Active_Bits, TElement>::All_Clear_Element;
    using etl::private_bitset::bitset_common<Active_Bits, TElement>::Number_Of_Elements;
    using etl::private_bitset::bitset_common<Active_Bits, TElement>::Size;
    using etl::private_bitset::bitset_common<Active_Bits, TElement>::Storage_Model;
    using etl::private_bitset::bitset_common<Active_Bits, TElement>::Top_Mask;
    using etl::private_bitset::bitset_common<Active_Bits, TElement>::Allocated_Bits;

    //*************************************************************************
    /// The reference type returned.
    //*************************************************************************
    class bit_reference
    {
    public:

      friend class bitset;

      //*******************************
      /// Copy constructor.
      //*******************************
      ETL_CONSTEXPR14 bit_reference(const bit_reference& other) ETL_NOEXCEPT
        : p_bitset(other.p_bitset)
        , position(other.position)
      {
      }

      //*******************************
      /// Conversion operator.
      //*******************************
      ETL_CONSTEXPR14 operator bool() const ETL_NOEXCEPT
      {
        return p_bitset->test(position);
      }

      //*******************************
      /// Assignment operator.
      //*******************************
      ETL_CONSTEXPR14 bit_reference& operator = (bool b) ETL_NOEXCEPT
      {
        p_bitset->set(position, b);
        return *this;
      }

      //*******************************
      /// Assignment operator.
      //*******************************
      ETL_CONSTEXPR14 bit_reference& operator = (const bit_reference& r) ETL_NOEXCEPT
      {
        p_bitset->set(position, bool(r));
        return *this;
      }

      //*******************************
      /// Flip the bit.
      //*******************************
      ETL_CONSTEXPR14 bit_reference& flip() ETL_NOEXCEPT
      {
        p_bitset->flip(position);
        return *this;
      }

      //*******************************
      /// Return the logical inverse of the bit.
      //*******************************
      ETL_CONSTEXPR14 bool operator~() const ETL_NOEXCEPT
      {
        return !p_bitset->test(position);
      }

    private:

      //*******************************
      /// Default constructor.
      //*******************************
      ETL_CONSTEXPR14 bit_reference() ETL_NOEXCEPT
        : p_bitset(ETL_NULLPTR)
        , position(0)
      {
      }

      //*******************************
      /// Constructor.
      //*******************************
      ETL_CONSTEXPR14 bit_reference(bitset<Active_Bits, TElement>& r_bitset, size_t position_) ETL_NOEXCEPT
        : p_bitset(&r_bitset)
        , position(position_)
      {
      }

      bitset<Active_Bits, TElement>* p_bitset; ///< The bitset.
      size_t position; ///< The position in the bitset.
    };

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    ETL_CONSTEXPR14 bitset() ETL_NOEXCEPT
      : buffer()
    {
      implementation::reset_all(buffer, Number_Of_Elements);
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    ETL_CONSTEXPR14 bitset(const bitset<Active_Bits, TElement>& other) ETL_NOEXCEPT
      : buffer()
    {
      implementation::operator_assignment(buffer, other.buffer, Number_Of_Elements);
    }

    //*************************************************************************
    /// Construct from a value.
    //*************************************************************************
    template <typename TValue>
    ETL_CONSTEXPR14 bitset(TValue value, typename etl::enable_if<is_integral<TValue>::value>::type* = 0) ETL_NOEXCEPT
      : buffer()
    {
      implementation::template initialise<element_type>(buffer, Number_Of_Elements, value);
    }

    //*************************************************************************
    /// Construct from a string.
    //*************************************************************************
    template <typename TPString>
    ETL_CONSTEXPR14 bitset(TPString text, typename etl::enable_if<is_same<TPString, const char*>::value>::type* = 0) ETL_NOEXCEPT
      : buffer()
    {
      implementation::from_string(buffer, Number_Of_Elements, Active_Bits, text);
    }

    //*************************************************************************
    /// Construct from a string.
    //*************************************************************************
    template <typename TPString>
    ETL_CONSTEXPR14 bitset(TPString text, typename etl::enable_if<is_same<TPString, const wchar_t*>::value>::type* = 0) ETL_NOEXCEPT
      : buffer()
    {
      implementation::from_string(buffer, Number_Of_Elements, Active_Bits, text);
    }

    //*************************************************************************
    /// Construct from a string.
    //*************************************************************************
    template <typename TPString>
    ETL_CONSTEXPR14 bitset(TPString text, typename etl::enable_if<is_same<TPString, const char16_t*>::value>::type* = 0) ETL_NOEXCEPT
      : buffer()
    {
      implementation::from_string(buffer, Number_Of_Elements, Active_Bits, text);
    }

    //*************************************************************************
    /// Construct from a string.
    //*************************************************************************
    template <typename TPString>
    ETL_CONSTEXPR14 bitset(TPString text, typename etl::enable_if<is_same<TPString, const char32_t*>::value>::type* = 0) ETL_NOEXCEPT
      : buffer()
    {
      implementation::from_string(buffer, Number_Of_Elements, Active_Bits, text);
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    ETL_CONSTEXPR14 bitset& operator =(const bitset<Active_Bits, TElement>& other) ETL_NOEXCEPT
    {
      implementation::operator_assignment(buffer, other.buffer, Number_Of_Elements);

      return *this;
    }

    //*************************************************************************
    /// Set all of the bits.
    //*************************************************************************
    ETL_CONSTEXPR14 bitset<Active_Bits, TElement>& set() ETL_NOEXCEPT
    {
      implementation::set_all(buffer, Number_Of_Elements, Top_Mask);

      return *this;
    }

    //*************************************************************************
    /// Set the bit at the position.
    //*************************************************************************
    ETL_CONSTEXPR14
    bitset<Active_Bits, TElement>& set(size_t position, bool value = true)
    {
      ETL_ASSERT_OR_RETURN_VALUE(position < Active_Bits, ETL_ERROR(bitset_overflow), *this);

      implementation::set_position(buffer, position, value);
      
      return *this;
    }

    //*************************************************************************
    /// Set the bit at the position.
    //*************************************************************************
    template <size_t Position>
    ETL_CONSTEXPR14 bitset<Active_Bits, TElement>& set(bool value = true)
    {
      ETL_STATIC_ASSERT(Position < Active_Bits, "Position out of bounds");

      implementation::template set_position<Position>(buffer, value);

      return *this;
    }

    //*************************************************************************
    /// Set the bit at the position.
    //*************************************************************************
    template <size_t Position, bool Value>
    ETL_CONSTEXPR14 bitset<Active_Bits, TElement>& set()
    {
      ETL_STATIC_ASSERT(Position < Active_Bits, "Position out of bounds");

      implementation::template set_position<Position, Value>(buffer);

      return *this;
    }

    //*************************************************************************
    /// Set from a string.
    //*************************************************************************
    template <typename TPString>
    ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_same<TPString, const char*>::value, bitset<Active_Bits, TElement>&>::type
      set(TPString text) ETL_NOEXCEPT
    {
      implementation::from_string(buffer, Number_Of_Elements, Active_Bits, text);

      return *this;
    }

    //*************************************************************************
    /// Set from a wide string.
    //*************************************************************************
    template <typename TPString>
    ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_same<TPString, const wchar_t*>::value, bitset<Active_Bits, TElement>&>::type
      set(TPString text) ETL_NOEXCEPT
    {
      implementation::from_string(buffer, Number_Of_Elements, Active_Bits, text);

      return *this;
    }

    //*************************************************************************
    /// Set from a char16 string.
    //*************************************************************************
    template <typename TPString>
    ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_same<TPString, const char16_t*>::value, bitset<Active_Bits, TElement>&>::type
      set(TPString text) ETL_NOEXCEPT
    {
      implementation::from_string(buffer, Number_Of_Elements, Active_Bits, text);

      return *this;
    }

    //*************************************************************************
    /// Set from a char32 string.
    //*************************************************************************
    template <typename TPString>
    ETL_CONSTEXPR14 
    typename etl::enable_if<etl::is_same<TPString, const char32_t*>::value, bitset<Active_Bits, TElement>&>::type
      set(TPString text) ETL_NOEXCEPT
    {
      implementation::from_string(buffer, Number_Of_Elements, Active_Bits, text);

      return *this;
    }

    //*************************************************************************
    /// Set from a string.
    //*************************************************************************
    ETL_CONSTEXPR14 bitset<Active_Bits, TElement>& from_string(const char* text) ETL_NOEXCEPT
    {
      implementation::from_string(buffer, Number_Of_Elements, Active_Bits, text);

      return *this;
    }

    //*************************************************************************
    /// Set from a wide string.
    //*************************************************************************
    ETL_CONSTEXPR14 bitset<Active_Bits, TElement>& from_string(const wchar_t* text) ETL_NOEXCEPT
    {
      implementation::from_string(buffer, Number_Of_Elements, Active_Bits, text);

      return *this;
    }

    //*************************************************************************
    /// Set from a u16 string.
    //*************************************************************************
    ETL_CONSTEXPR14 bitset<Active_Bits, TElement>& from_string(const char16_t* text) ETL_NOEXCEPT
    {
      implementation::from_string(buffer, Number_Of_Elements, Active_Bits, text);

      return *this;
    }

    //*************************************************************************
    /// Set from a u32 string.
    //*************************************************************************
    ETL_CONSTEXPR14 bitset<Active_Bits, TElement>& from_string(const char32_t* text) ETL_NOEXCEPT
    {
      implementation::from_string(buffer, Number_Of_Elements, Active_Bits, text);

      return *this;
    }

    //*************************************************************************
    /// Get as an integral value.
    //*************************************************************************
    template <typename T>
    ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value, T>::type
      value() const ETL_NOEXCEPT
    {
      ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Only integral types are supported");
      ETL_STATIC_ASSERT(etl::integral_limits<T>::bits >= (Number_Of_Elements * Bits_Per_Element), "Type too small");

      return implementation::template value<T>(buffer, Number_Of_Elements);
    }

    //*************************************************************************
    /// Extract an integral value from an arbitrary position and length.
    /// Run time position and length.
    //*************************************************************************
    template <typename T>
    ETL_CONSTEXPR14
    T extract(size_t position, size_t length = etl::integral_limits<T>::bits) const
    {
      ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Only integral types are supported");

      ETL_ASSERT_OR_RETURN_VALUE(length <= etl::integral_limits<T>::bits, ETL_ERROR(bitset_overflow), 0);
      ETL_ASSERT_OR_RETURN_VALUE((position + length) <= Active_Bits,      ETL_ERROR(bitset_overflow), 0);

      return implementation::template extract<T>(buffer, position, length);
    }

    //*************************************************************************
    /// Extract an integral value from an arbitrary position and length.
    /// Compile time position and length.
    //*************************************************************************
#if ETL_USING_CPP11    
		template <typename T, size_t Position, size_t Length = etl::integral_limits<T>::bits>
#else
	  template <typename T, size_t Position, size_t Length>
#endif
    ETL_CONSTEXPR14
    T extract() const
    {
      ETL_STATIC_ASSERT(etl::is_integral<T>::value,              "Only integral types are supported");
      ETL_STATIC_ASSERT(Length <= etl::integral_limits<T>::bits, "Length is larger that the required type");
      ETL_STATIC_ASSERT((Position + Length) <= Active_Bits,      "Position/Length overflows bitset");

      return implementation::template extract<T, Position, Length>(buffer);
    }

    //*************************************************************************
    /// Get as an unsigned long.
    //*************************************************************************
    unsigned long to_ulong() const
    {
      ETL_ASSERT(etl::integral_limits<unsigned long>::bits >= Active_Bits, ETL_ERROR(etl::bitset_overflow));

      return implementation::template value<unsigned long>(buffer, Number_Of_Elements);
    }

    //*************************************************************************
    /// Get as an unsigned long long.
    //*************************************************************************
    unsigned long long to_ullong() const
    {
      ETL_ASSERT(etl::integral_limits<unsigned long long>::bits >= Active_Bits, ETL_ERROR(etl::bitset_overflow));

      return implementation::template value<unsigned long long>(buffer, Number_Of_Elements);
    }

    //*************************************************************************
    /// Reset all of the bits.
    //*************************************************************************
    ETL_CONSTEXPR14 bitset<Active_Bits, TElement>& reset() ETL_NOEXCEPT
    {
      implementation::reset_all(buffer, Number_Of_Elements);

      return *this;
    }

    //*************************************************************************
    /// Reset the bit at the position.
    //*************************************************************************
    ETL_CONSTEXPR14 bitset<Active_Bits, TElement>& reset(size_t position)
    {
      ETL_ASSERT_OR_RETURN_VALUE(position < Active_Bits, ETL_ERROR(bitset_overflow), *this);

      implementation::reset_position(buffer, position);

      return *this;
    }

    //*************************************************************************
    /// Tests a bit at a position.
    /// Positions greater than the number of configured bits will return <b>false</b>.
    //*************************************************************************
    ETL_CONSTEXPR14 bool test(size_t position) const
    {
      ETL_ASSERT_OR_RETURN_VALUE(position < Active_Bits, ETL_ERROR(bitset_overflow), false);

      return implementation::test(buffer, position);
    }
 
    //*************************************************************************
    /// Tests a bit at a position.
    /// Positions greater than the number of configured bits will not compile.
    //*************************************************************************
    template <size_t Position>
    ETL_CONSTEXPR14 bool test() const
    {
      ETL_STATIC_ASSERT(Position < Active_Bits, "Position out of bounds");

      return implementation::test(buffer, Position);
    }

    //*************************************************************************
    /// The number of bits in the bitset.
    //*************************************************************************
    static ETL_CONSTEXPR size_t size() ETL_NOEXCEPT
    {
      return Active_Bits;
    }

    //*************************************************************************
    /// The number of storage elements in the bitset.
    //*************************************************************************
    static ETL_CONSTEXPR size_t number_of_elements() ETL_NOEXCEPT
    {
      return Number_Of_Elements;
    }

    //*************************************************************************
    /// The value of a set element.
    //*************************************************************************
    static ETL_CONSTEXPR element_type all_set_element() ETL_NOEXCEPT
    {
      return All_Set_Element;
    }

    //*************************************************************************
    /// The value of a clear element.
    //*************************************************************************
    static ETL_CONSTEXPR element_type all_clear_element() ETL_NOEXCEPT
    {
      return All_Clear_Element;
    }

    //*************************************************************************
    /// The number of bits in an element.
    //*************************************************************************
    static ETL_CONSTEXPR size_t bits_per_element() ETL_NOEXCEPT
    {
      return Bits_Per_Element;
    }

    //*************************************************************************
    /// The mask for the msb element.
    //*************************************************************************
    static ETL_CONSTEXPR element_type top_mask() ETL_NOEXCEPT
    {
      return Top_Mask;
    }

    //*************************************************************************
    /// The total number of bits of storage, including unused.
    //*************************************************************************
    static ETL_CONSTEXPR size_t allocated_bits() ETL_NOEXCEPT
    {
      return Number_Of_Elements * Bits_Per_Element;
    }

    //*************************************************************************
    /// The storage model for the bitset.
    /// etl::bitset_storage_model::Single
    /// etl::bitset_storage_model::Multi
    //*************************************************************************
    static ETL_CONSTEXPR etl::bitset_storage_model storage_model() ETL_NOEXCEPT
    {
      return Storage_Model;
    }

    //*************************************************************************
    /// Count the number of bits set.
    //*************************************************************************
    ETL_CONSTEXPR14 size_t count() const ETL_NOEXCEPT
    {
      return implementation::count(buffer, Number_Of_Elements);
    }

    //*************************************************************************
    // Are all the bits sets?
    //*************************************************************************
    ETL_CONSTEXPR14 bool all() const ETL_NOEXCEPT
    {
      return implementation::all(buffer, Number_Of_Elements, Top_Mask);
    }

    //*************************************************************************
    // Are all the mask bits sets?
    //*************************************************************************
    ETL_CONSTEXPR14 bool all(element_type mask) const ETL_NOEXCEPT
    {
      ETL_STATIC_ASSERT(Storage_Model == etl::bitset_storage_model::Single, "Not supported for 'Multi' bitset storage model");

      return implementation::all(buffer, Number_Of_Elements, Top_Mask, mask);
    }

    //*************************************************************************
    /// Are none of the bits set?
    //*************************************************************************
    ETL_CONSTEXPR14 bool none() const ETL_NOEXCEPT
    {
      return implementation::none(buffer, Number_Of_Elements);
    }

    //*************************************************************************
    /// Are none of the mask bits set?
    //*************************************************************************
    ETL_CONSTEXPR14 bool none(element_type mask) const ETL_NOEXCEPT
    {
      ETL_STATIC_ASSERT(Storage_Model == etl::bitset_storage_model::Single, "Not supported for 'Multi' bitset storage model");

      return implementation::none(buffer, Number_Of_Elements, mask);
    }

    //*************************************************************************
    /// Are any of the bits set?
    //*************************************************************************
    ETL_CONSTEXPR14 bool any() const ETL_NOEXCEPT
    {
      return implementation::any(buffer, Number_Of_Elements);
    }
 
    //*************************************************************************
    /// Are any of the mask bits set?
    //*************************************************************************
    ETL_CONSTEXPR14 bool any(element_type mask) const ETL_NOEXCEPT
    {
      ETL_STATIC_ASSERT(Storage_Model == etl::bitset_storage_model::Single, "Not supported for 'Multi' bitset storage model");

      return implementation::any(buffer, Number_Of_Elements, mask);
    }

    //*************************************************************************
    /// Flip all of the bits.
    //*************************************************************************
    ETL_CONSTEXPR14 bitset<Active_Bits, TElement>& flip() ETL_NOEXCEPT
    {
      implementation::flip_all(buffer, Number_Of_Elements);

      return *this;
    }

    //*************************************************************************
    /// Flip the bit at the position.
    //*************************************************************************
    ETL_CONSTEXPR14 bitset<Active_Bits, TElement>& flip(size_t position)
    {
      ETL_ASSERT_OR_RETURN_VALUE(position < Active_Bits, ETL_ERROR(bitset_overflow), *this);

      implementation::flip_position(buffer, position);

      return *this;
    }

    //*************************************************************************
    /// Read [] operator.
    //*************************************************************************
    ETL_CONSTEXPR14 bool operator[] (size_t position) const ETL_NOEXCEPT
    {
      return implementation::test(buffer, position);
    }

    //*************************************************************************
    /// Write [] operator.
    //*************************************************************************
    ETL_CONSTEXPR14 bit_reference operator [] (size_t position) ETL_NOEXCEPT
    {
      return bit_reference(*this, position);
    }

    //*************************************************************************
    /// Returns a string representing the bitset.
    //*************************************************************************
#if ETL_USING_CPP11
    template <typename TString = etl::string<Active_Bits>>
#else
    template <typename TString>
#endif
    ETL_CONSTEXPR14 TString to_string(typename TString::value_type zero = typename TString::value_type('0'),
                                      typename TString::value_type one  = typename TString::value_type('1')) const
    {
      return implementation::template to_string<TString>(buffer, Active_Bits, zero, one);
    }

    //*************************************************************************
    /// Finds the first bit in the specified state.
    ///\param state The state to search for.
    ///\returns The position of the bit or npos if none were found.
    //*************************************************************************
    ETL_CONSTEXPR14 size_t find_first(bool state) const ETL_NOEXCEPT
    {
      return implementation::find_next(buffer, Number_Of_Elements, Active_Bits, state, 0);
    }

    //*************************************************************************
    /// Finds the next bit in the specified state.
    ///\param state    The state to search for.
    ///\param position The position to start from.
    ///\returns The position of the bit or npos if none were found.
    //*************************************************************************
    ETL_CONSTEXPR14 size_t find_next(bool state, size_t position) const ETL_NOEXCEPT
    {
      return implementation::find_next(buffer, Number_Of_Elements, Active_Bits, state, position);
    }

    //*************************************************************************
    /// operator &
    //*************************************************************************
    ETL_CONSTEXPR14 bitset<Active_Bits, TElement> operator &(const bitset<Active_Bits, TElement>& other) const ETL_NOEXCEPT
    {
      etl::bitset<Active_Bits, TElement> temp(*this);

      implementation::operator_and(temp.buffer, other.buffer, Number_Of_Elements);

      return temp;
    }

    //*************************************************************************
    /// operator &=
    //*************************************************************************
    ETL_CONSTEXPR14 bitset<Active_Bits, TElement>& operator &=(const bitset<Active_Bits, TElement>& other) ETL_NOEXCEPT
    {
      implementation::operator_and(buffer, other.buffer, Number_Of_Elements);

      return *this;
    }

    //*************************************************************************
    /// operator |
    //*************************************************************************
    ETL_CONSTEXPR14 bitset<Active_Bits, TElement> operator |(const bitset<Active_Bits, TElement>& other) const ETL_NOEXCEPT
    {
      etl::bitset<Active_Bits, TElement> temp(*this);

      implementation::operator_or(temp.buffer, other.buffer, Number_Of_Elements);

      return temp;
    }

    //*************************************************************************
    /// operator |=
    //*************************************************************************
    ETL_CONSTEXPR14 bitset<Active_Bits, TElement>& operator |=(const bitset<Active_Bits, TElement>& other) ETL_NOEXCEPT
    {
      implementation::operator_or(&buffer[0], &other.buffer[0], Number_Of_Elements);

      return *this;
    }

    //*************************************************************************
    /// operator ^
    //*************************************************************************
    ETL_CONSTEXPR14 bitset<Active_Bits, TElement> operator ^(const bitset<Active_Bits, TElement>& other) const ETL_NOEXCEPT
    {
      etl::bitset<Active_Bits, TElement> temp(*this);

      implementation::operator_xor(temp.buffer, other.buffer, Number_Of_Elements);

      return temp;
    }

    //*************************************************************************
    /// operator ^=
    //*************************************************************************
    ETL_CONSTEXPR14 bitset<Active_Bits, TElement>& operator ^=(const bitset<Active_Bits, TElement>& other) ETL_NOEXCEPT
    {
      implementation::operator_xor(buffer, other.buffer, Number_Of_Elements);

      return *this;
    }

    //*************************************************************************
    /// operator ~
    //*************************************************************************
    ETL_CONSTEXPR14 bitset<Active_Bits, TElement> operator ~() const ETL_NOEXCEPT
    {
      etl::bitset<Active_Bits, TElement> temp(*this);

      implementation::flip_all(temp.buffer, Number_Of_Elements);

      return temp;
    }

    //*************************************************************************
    /// operator <<
    //*************************************************************************
    ETL_CONSTEXPR14 bitset<Active_Bits, TElement> operator <<(size_t shift) const ETL_NOEXCEPT
    {
      etl::bitset<Active_Bits, TElement> temp(*this);

      implementation::operator_shift_left(temp.buffer, Number_Of_Elements, Active_Bits, shift);

      return temp;
    }

    //*************************************************************************
    /// operator <<=
    //*************************************************************************
    ETL_CONSTEXPR14 bitset<Active_Bits, TElement>& operator <<=(size_t shift) ETL_NOEXCEPT
    {
      if (shift >= Active_Bits)
      {
        implementation::reset_all(buffer, Number_Of_Elements);
      }
      else
      {
        implementation::operator_shift_left(buffer, Number_Of_Elements, Active_Bits, shift);
      }
      
      return *this;
    }

    //*************************************************************************
    /// operator >>
    //*************************************************************************
    ETL_CONSTEXPR14 bitset<Active_Bits, TElement> operator >>(size_t shift) const ETL_NOEXCEPT
    {
      bitset<Active_Bits, TElement> temp(*this);

      implementation::operator_shift_right(temp.buffer, Number_Of_Elements, Active_Bits, shift);

      return temp;
    }

    //*************************************************************************
    /// operator >>=
    //*************************************************************************
    ETL_CONSTEXPR14 bitset<Active_Bits, TElement>& operator >>=(size_t shift) ETL_NOEXCEPT
    {
      if (shift >= Active_Bits)
      {
        implementation::reset_all(buffer, Number_Of_Elements);
      }
      else
      {
        implementation::operator_shift_right(buffer, Number_Of_Elements, Active_Bits, shift);
      }

      return *this;
    }

    //*************************************************************************
    /// operator ==
    //*************************************************************************
    friend ETL_CONSTEXPR14 bool operator ==(const bitset<Active_Bits, TElement>& lhs, const bitset<Active_Bits, TElement>& rhs) ETL_NOEXCEPT
    {
      return implementation::operator_equality(lhs.buffer, rhs.buffer, lhs.Number_Of_Elements);
    }

    //*************************************************************************
    /// operator !=
    //*************************************************************************
    friend ETL_CONSTEXPR14 bool operator !=(const bitset<Active_Bits, TElement>& lhs, const bitset<Active_Bits, TElement>& rhs) ETL_NOEXCEPT
    {
      return !(lhs == rhs);
    }

    //*************************************************************************
    /// swap
    //*************************************************************************
    ETL_CONSTEXPR14 void swap(etl::bitset<Active_Bits, TElement>& other) ETL_NOEXCEPT
    {
      implementation::swap(buffer, other.buffer, Number_Of_Elements);
    }

    //*************************************************************************
    /// span
    /// Returns a span of the underlying buffer.
    //*************************************************************************
    ETL_CONSTEXPR14 span_type span() ETL_NOEXCEPT
    {
      return span_type(buffer, Number_Of_Elements);
    }

    //*************************************************************************
    /// span
    /// Returns a const span of the underlying buffer.
    //*************************************************************************
    ETL_CONSTEXPR14 const_span_type span() const ETL_NOEXCEPT
    {
      return const_span_type(buffer, Number_Of_Elements);
    }

  private:

    //*************************************************************************
    /// Correct the unused top bits after bit manipulation.
    //*************************************************************************
    //ETL_CONSTEXPR14 static void clear_unused_bits_in_msb(pointer pbuffer) ETL_NOEXCEPT
    //{
    //  pbuffer[Number_Of_Elements - 1U] &= Top_Mask;
    //}

    // The implementation of the bitset functionality.
    typedef etl::bitset_impl<element_type, (Number_Of_Elements == 1U) ? etl::bitset_storage_model::Single : etl::bitset_storage_model::Multi> implementation;

    // The storage for the bitset.
    element_type buffer[Number_Of_Elements];
  };

  //***************************************************************************
  /// operator &
  ///\ingroup bitset
  //***************************************************************************
  template <size_t Active_Bits, typename TElement>
  ETL_CONSTEXPR14 bitset<Active_Bits> operator & (const bitset<Active_Bits, TElement>& lhs, const bitset<Active_Bits, TElement>& rhs) ETL_NOEXCEPT
  {
    bitset<Active_Bits> temp(lhs);
    temp &= rhs;
    return temp;
  }

  //***************************************************************************
  /// operator |
  ///\ingroup bitset
  //***************************************************************************
  template<size_t Active_Bits, typename TElement>
  ETL_CONSTEXPR14 bitset<Active_Bits> operator | (const bitset<Active_Bits, TElement>& lhs, const bitset<Active_Bits, TElement>& rhs) ETL_NOEXCEPT
  {
    bitset<Active_Bits> temp(lhs);
    temp |= rhs;
    return temp;
  }

  //***************************************************************************
  /// operator ^
  ///\ingroup bitset
  //***************************************************************************
  template<size_t Active_Bits, typename TElement>
  ETL_CONSTEXPR14 bitset<Active_Bits> operator ^ (const bitset<Active_Bits, TElement>& lhs, const bitset<Active_Bits, TElement>& rhs) ETL_NOEXCEPT
  {
    bitset<Active_Bits> temp(lhs);
    temp ^= rhs;
    return temp;
  }
}

//***************************************************************************
/// operator !=
///\ingroup bitset
//***************************************************************************
template<size_t Active_Bits, typename TElement>
ETL_CONSTEXPR14 bool operator != (const etl::bitset<Active_Bits, TElement>& lhs, const etl::bitset<Active_Bits, TElement>& rhs) ETL_NOEXCEPT
{
  return !(lhs == rhs);
}

//*************************************************************************
/// swap
//*************************************************************************
template <size_t Active_Bits, typename TElement>
ETL_CONSTEXPR14 void swap(etl::bitset<Active_Bits, TElement>& lhs, etl::bitset<Active_Bits, TElement>& rhs) ETL_NOEXCEPT
{
  lhs.swap(rhs);
}

//***************************************************************************
/// bitset_ext
//***************************************************************************
namespace etl
{
  //***************************************************************************
  template <size_t Active_Bits = 0U,
            typename TElement = unsigned char>
  class bitset_ext;

  //***************************************************************************
  /// Specialisation for zero bits.
  //***************************************************************************
  template <>
  class bitset_ext<0U, unsigned char> : public etl::private_bitset::bitset_common<0U, unsigned char>
  {
  public:

    typedef etl::private_bitset::bitset_common<0U, unsigned char>::element_type    element_type;
    typedef etl::private_bitset::bitset_common<0U, unsigned char>::span_type       span_type;
    typedef etl::private_bitset::bitset_common<0U, unsigned char>::const_span_type const_span_type;

    using etl::private_bitset::bitset_common<0U, unsigned char>::Bits_Per_Element;
    using etl::private_bitset::bitset_common<0U, unsigned char>::All_Set_Element;
    using etl::private_bitset::bitset_common<0U, unsigned char>::All_Clear_Element;
    using etl::private_bitset::bitset_common<0U, unsigned char>::Number_Of_Elements;
    using etl::private_bitset::bitset_common<0U, unsigned char>::Size;
    using etl::private_bitset::bitset_common<0U, unsigned char>::Storage_Model;
    using etl::private_bitset::bitset_common<0U, unsigned char>::Top_Mask;
    using etl::private_bitset::bitset_common<0U, unsigned char>::Allocated_Bits;
  };

  //*************************************************************************
  /// A bitset that uses externally declared storage.
  //*************************************************************************
  template <size_t Active_Bits, typename TElement>
  class bitset_ext : public etl::private_bitset::bitset_common<Active_Bits, TElement>
  {
  public:

    ETL_STATIC_ASSERT(etl::is_unsigned<TElement>::value, "The element type must be unsigned");

    typedef typename etl::private_bitset::bitset_common<Active_Bits, TElement>::element_type    element_type;
    typedef typename etl::private_bitset::bitset_common<Active_Bits, TElement>::span_type       span_type;
    typedef typename etl::private_bitset::bitset_common<Active_Bits, TElement>::const_span_type const_span_type;
    
    using etl::private_bitset::bitset_common<Active_Bits, TElement>::Bits_Per_Element;
    using etl::private_bitset::bitset_common<Active_Bits, TElement>::All_Set_Element;
    using etl::private_bitset::bitset_common<Active_Bits, TElement>::All_Clear_Element;
    using etl::private_bitset::bitset_common<Active_Bits, TElement>::Number_Of_Elements;
    using etl::private_bitset::bitset_common<Active_Bits, TElement>::Size;
    using etl::private_bitset::bitset_common<Active_Bits, TElement>::Storage_Model;
    using etl::private_bitset::bitset_common<Active_Bits, TElement>::Top_Mask;
    using etl::private_bitset::bitset_common<Active_Bits, TElement>::Allocated_Bits;

    typedef etl::array<TElement, Number_Of_Elements> buffer_type;

    //*************************************************************************
    /// The reference type returned.
    //*************************************************************************
    class bit_reference
    {
    public:

      friend class bitset_ext;

      //*******************************
      /// Copy constructor
      //*******************************
      ETL_CONSTEXPR14 bit_reference(const bit_reference& other) ETL_NOEXCEPT
        : p_bitset(other.p_bitset)
        , position(other.position)
      {
      }

      //*******************************
      /// Conversion operator.
      //*******************************
      ETL_CONSTEXPR14 operator bool() const ETL_NOEXCEPT
      {
        return p_bitset->test(position);
      }

      //*******************************
      /// Assignment operator.
      //*******************************
      ETL_CONSTEXPR14 bit_reference& operator = (bool b) ETL_NOEXCEPT
      {
        p_bitset->set(position, b);
        return *this;
      }

      //*******************************
      /// Assignment operator.
      //*******************************
      ETL_CONSTEXPR14 bit_reference& operator = (const bit_reference& r) ETL_NOEXCEPT
      {
        p_bitset->set(position, bool(r));
        return *this;
      }

      //*******************************
      /// Flip the bit.
      //*******************************
      ETL_CONSTEXPR14 bit_reference& flip() ETL_NOEXCEPT
      {
        p_bitset->flip(position);
        return *this;
      }

      //*******************************
      /// Return the logical inverse of the bit.
      //*******************************
      ETL_CONSTEXPR14 bool operator~() const ETL_NOEXCEPT
      {
        return !p_bitset->test(position);
      }

    private:

      //*******************************
      /// Default constructor.
      //*******************************
      ETL_CONSTEXPR14 bit_reference() ETL_NOEXCEPT
        : p_bitset(ETL_NULLPTR)
        , position(0)
      {
      }

      //*******************************
      /// Constructor.
      //*******************************
      ETL_CONSTEXPR14 bit_reference(bitset_ext<Active_Bits, TElement>& r_bitset, size_t position_) ETL_NOEXCEPT
        : p_bitset(&r_bitset)
        , position(position_)
      {
      }

      bitset_ext<Active_Bits, TElement>* p_bitset; ///< The bitset.
      size_t position; ///< The position in the bitset.
    };

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    ETL_CONSTEXPR14 explicit bitset_ext(element_type* pbuffer_)
      : pbuffer(pbuffer_)
    {
      ETL_ASSERT(pbuffer != ETL_NULLPTR, ETL_ERROR(etl::bitset_invalid_buffer));
      implementation::reset_all(pbuffer, Number_Of_Elements);
    }

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    ETL_CONSTEXPR14 explicit bitset_ext(buffer_type& buffer)
      : pbuffer(buffer.data())
    {
      ETL_ASSERT(pbuffer != ETL_NULLPTR, ETL_ERROR(etl::bitset_invalid_buffer));
      implementation::reset_all(pbuffer, Number_Of_Elements);
    }

    //*************************************************************************
    /// Construct copy.
    //*************************************************************************
    ETL_CONSTEXPR14 bitset_ext(const bitset_ext<Active_Bits, TElement>& other, element_type* pbuffer_)
      : pbuffer(pbuffer_)
    {
      ETL_ASSERT(pbuffer != ETL_NULLPTR, ETL_ERROR(etl::bitset_invalid_buffer));
      implementation::operator_assignment(pbuffer, other.pbuffer, Number_Of_Elements);
    }

    //*************************************************************************
    /// Construct copy.
    //*************************************************************************
    ETL_CONSTEXPR14 bitset_ext(const bitset_ext<Active_Bits, TElement>& other, buffer_type& buffer) ETL_NOEXCEPT
      : pbuffer(buffer.data())
    {
      implementation::operator_assignment(pbuffer, other.pbuffer, Number_Of_Elements);
    }

    //*************************************************************************
    /// Copy Constructor (Deleted).
    //*************************************************************************
    ETL_CONSTEXPR14 bitset_ext(const bitset_ext<Active_Bits, TElement>& other) ETL_NOEXCEPT ETL_DELETE;

    //*************************************************************************
    /// Construct from a value.
    //*************************************************************************
    ETL_CONSTEXPR14 bitset_ext(unsigned long long value, element_type* pbuffer_)
      : pbuffer(pbuffer_)
    {
      ETL_ASSERT(pbuffer != ETL_NULLPTR, ETL_ERROR(etl::bitset_invalid_buffer));
      implementation::template initialise<element_type>(pbuffer, Number_Of_Elements, value);
    }

    //*************************************************************************
    /// Construct from a value.
    //*************************************************************************
    template <typename TValue>
    ETL_CONSTEXPR14 bitset_ext(TValue value, buffer_type& buffer, typename etl::enable_if<is_integral<TValue>::value>::type* = 0) ETL_NOEXCEPT
      : pbuffer(buffer.data())
    {
      implementation::template initialise<element_type>(pbuffer, Number_Of_Elements, value);
    }

    //*************************************************************************
    /// Construct from a string.
    //*************************************************************************
    template <typename TPString>
    ETL_CONSTEXPR14 bitset_ext(TPString text, element_type* pbuffer_, typename etl::enable_if<is_same<TPString, const char*>::value>::type* = 0)
      : pbuffer(pbuffer_)
    {
      ETL_ASSERT(pbuffer != ETL_NULLPTR, ETL_ERROR(etl::bitset_invalid_buffer));
      implementation::from_string(pbuffer, Number_Of_Elements, Active_Bits, text);
    }

    //*************************************************************************
    /// Construct from a string.
    //*************************************************************************
    template <typename TPString>
    ETL_CONSTEXPR14 bitset_ext(TPString text, buffer_type& buffer, typename etl::enable_if<is_same<TPString, const char*>::value>::type* = 0) ETL_NOEXCEPT
      : pbuffer(buffer.data())
    {
      implementation::from_string(pbuffer, Number_Of_Elements, Active_Bits, text);
    }

    //*************************************************************************
    /// Construct from a string.
    //*************************************************************************
    template <typename TPString>
    ETL_CONSTEXPR14 bitset_ext(TPString text, element_type* pbuffer_, typename etl::enable_if<is_same<TPString, const wchar_t*>::value>::type* = 0)
      : pbuffer(pbuffer_)
    {
      ETL_ASSERT(pbuffer != ETL_NULLPTR, ETL_ERROR(etl::bitset_invalid_buffer));
      implementation::from_string(pbuffer, Number_Of_Elements, Active_Bits, text);
    }

    //*************************************************************************
    /// Construct from a string.
    //*************************************************************************
    template <typename TPString>
    ETL_CONSTEXPR14 bitset_ext(TPString text, buffer_type& buffer, typename etl::enable_if<is_same<TPString, const wchar_t*>::value>::type* = 0) ETL_NOEXCEPT
      : pbuffer(buffer.data())
    {
      implementation::from_string(pbuffer, Number_Of_Elements, Active_Bits, text);
    }

    //*************************************************************************
    /// Construct from a string.
    //*************************************************************************
    template <typename TPString>
    ETL_CONSTEXPR14 bitset_ext(TPString text, element_type* pbuffer_, typename etl::enable_if<is_same<TPString, const char16_t*>::value>::type* = 0)
      : pbuffer(pbuffer_)
    {
      ETL_ASSERT(pbuffer != ETL_NULLPTR, ETL_ERROR(etl::bitset_invalid_buffer));
      implementation::from_string(pbuffer, Number_Of_Elements, Active_Bits, text);
    }

    //*************************************************************************
    /// Construct from a string.
    //*************************************************************************
    template <typename TPString>
    ETL_CONSTEXPR14 bitset_ext(TPString text, buffer_type& buffer, typename etl::enable_if<is_same<TPString, const char16_t*>::value>::type* = 0) ETL_NOEXCEPT
      : pbuffer(buffer.data())
    {
      implementation::from_string(pbuffer, Number_Of_Elements, Active_Bits, text);
    }

    //*************************************************************************
    /// Construct from a string.
    //*************************************************************************
    template <typename TPString>
    ETL_CONSTEXPR14 bitset_ext(TPString text, element_type* pbuffer_, typename etl::enable_if<is_same<TPString, const char32_t*>::value>::type* = 0)
      : pbuffer(pbuffer_)
    {
      ETL_ASSERT(pbuffer != ETL_NULLPTR, ETL_ERROR(etl::bitset_invalid_buffer));
      implementation::from_string(pbuffer, Number_Of_Elements, Active_Bits, text);
    }

    //*************************************************************************
    /// Construct from a string.
    //*************************************************************************
    template <typename TPString>
    ETL_CONSTEXPR14 bitset_ext(TPString text, buffer_type& buffer, typename etl::enable_if<is_same<TPString, const char32_t*>::value>::type* = 0) ETL_NOEXCEPT
      : pbuffer(buffer.data())
    {
      implementation::from_string(pbuffer, Number_Of_Elements, Active_Bits, text);
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    ETL_CONSTEXPR14 bitset_ext& operator =(const bitset_ext<Active_Bits, TElement>& other) ETL_NOEXCEPT
    {
      implementation::operator_assignment(pbuffer, other.pbuffer, Number_Of_Elements);

      return *this;
    }

    //*************************************************************************
    /// Set all of the bits.
    //*************************************************************************
    ETL_CONSTEXPR14 bitset_ext<Active_Bits, TElement>& set() ETL_NOEXCEPT
    {
      implementation::set_all(pbuffer, Number_Of_Elements, Top_Mask);

      return *this;
    }

    //*************************************************************************
    /// Set the bit at the position.
    //*************************************************************************
    ETL_CONSTEXPR14
      bitset_ext<Active_Bits, TElement>& set(size_t position, bool value = true)
    {
      ETL_ASSERT_OR_RETURN_VALUE(position < Active_Bits, ETL_ERROR(bitset_overflow), *this);

      implementation::set_position(pbuffer, position, value);

      return *this;
    }

    //*************************************************************************
    /// Set the bit at the position.
    //*************************************************************************
    template <size_t Position>
    ETL_CONSTEXPR14 bitset_ext<Active_Bits, TElement>& set(bool value = true)
    {
      ETL_STATIC_ASSERT(Position < Active_Bits, "Position out of bounds");

      implementation::template set_position<Position>(pbuffer, value);

      return *this;
    }

    //*************************************************************************
    /// Set the bit at the position.
    //*************************************************************************
    template <size_t Position, bool Value>
    ETL_CONSTEXPR14 bitset_ext<Active_Bits, TElement>& set()
    {
      ETL_STATIC_ASSERT(Position < Active_Bits, "Position out of bounds");

      implementation::template set_position<Position, Value>(pbuffer);

      return *this;
    }

    //*************************************************************************
    /// Set from a string.
    //*************************************************************************
    template <typename TPString>
    ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_same<TPString, const char*>::value, bitset_ext<Active_Bits, TElement>&>::type
      set(TPString text) ETL_NOEXCEPT
    {
      implementation::from_string(pbuffer, Number_Of_Elements, Active_Bits, text);

      return *this;
    }

    //*************************************************************************
    /// Set from a wide string.
    //*************************************************************************
    template <typename TPString>
    ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_same<TPString, const wchar_t*>::value, bitset_ext<Active_Bits, TElement>&>::type
      set(TPString text) ETL_NOEXCEPT
    {
      implementation::from_string(pbuffer, Number_Of_Elements, Active_Bits, text);

      return *this;
    }

    //*************************************************************************
    /// Set from a char16 string.
    //*************************************************************************
    template <typename TPString>
    ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_same<TPString, const char16_t*>::value, bitset_ext<Active_Bits, TElement>&>::type
      set(TPString text) ETL_NOEXCEPT
    {
      implementation::from_string(pbuffer, Number_Of_Elements, Active_Bits, text);

      return *this;
    }

    //*************************************************************************
    /// Set from a char32 string.
    //*************************************************************************
    template <typename TPString>
    ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_same<TPString, const char32_t*>::value, bitset_ext<Active_Bits, TElement>&>::type
      set(TPString text) ETL_NOEXCEPT
    {
      implementation::from_string(pbuffer, Number_Of_Elements, Active_Bits, text);

      return *this;
    }

    //*************************************************************************
    /// Set from a string.
    //*************************************************************************
    ETL_CONSTEXPR14 bitset_ext<Active_Bits, TElement>& from_string(const char* text) ETL_NOEXCEPT
    {
      implementation::from_string(pbuffer, Number_Of_Elements, Active_Bits, text);

      return *this;
    }

    //*************************************************************************
    /// Set from a wide string.
    //*************************************************************************
    ETL_CONSTEXPR14 bitset_ext<Active_Bits, TElement>& from_string(const wchar_t* text) ETL_NOEXCEPT
    {
      implementation::from_string(pbuffer, Number_Of_Elements, Active_Bits, text);

      return *this;
    }

    //*************************************************************************
    /// Set from a u16 string.
    //*************************************************************************
    ETL_CONSTEXPR14 bitset_ext<Active_Bits, TElement>& from_string(const char16_t* text) ETL_NOEXCEPT
    {
      implementation::from_string(pbuffer, Number_Of_Elements, Active_Bits, text);

      return *this;
    }

    //*************************************************************************
    /// Set from a u32 string.
    //*************************************************************************
    ETL_CONSTEXPR14 bitset_ext<Active_Bits, TElement>& from_string(const char32_t* text) ETL_NOEXCEPT
    {
      implementation::from_string(pbuffer, Number_Of_Elements, Active_Bits, text);

      return *this;
    }

    //*************************************************************************
    /// Get as an integral value.
    //*************************************************************************
    template <typename T>
    ETL_CONSTEXPR14
    T value() const ETL_NOEXCEPT
    {
      ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Only integral types are supported");
      ETL_STATIC_ASSERT(etl::integral_limits<T>::bits >= (Number_Of_Elements * Bits_Per_Element), "Type too small");

      return implementation::template value<T>(pbuffer, Number_Of_Elements);
    }

    //*************************************************************************
    /// Extract an integral value from an arbitrary position and length.
    /// Run time position and length.
    //*************************************************************************
    template <typename T>
    ETL_CONSTEXPR14
      T extract(size_t position, size_t length = etl::integral_limits<T>::bits)
    {
      ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Only integral types are supported");

      ETL_ASSERT_OR_RETURN_VALUE(length <= etl::integral_limits<T>::bits, ETL_ERROR(bitset_overflow), 0);
      ETL_ASSERT_OR_RETURN_VALUE((position + length) <= Active_Bits,      ETL_ERROR(bitset_overflow), 0);

      return implementation::template extract<T>(pbuffer, position, length);
    }

    //*************************************************************************
    /// Extract an integral value from an arbitrary position and length.
    /// Compile time position and length.
    //*************************************************************************
#if ETL_USING_CPP11
    template <typename T, size_t Position, size_t Length = etl::integral_limits<T>::bits>
#else
    template <typename T, size_t Position, size_t Length>
#endif
    ETL_CONSTEXPR14
    T extract() const
    {
      ETL_STATIC_ASSERT(etl::is_integral<T>::value,              "Only integral types are supported");
      ETL_STATIC_ASSERT(Length <= etl::integral_limits<T>::bits, "Length is larger that the required type");
      ETL_STATIC_ASSERT((Position + Length) <= Active_Bits,      "Position/Length overflows bitset");

      return implementation::template extract<T, Position, Length>(pbuffer);
    }

    //*************************************************************************
    /// Get as an unsigned long.
    //*************************************************************************
    unsigned long to_ulong() const
    {
      ETL_ASSERT(etl::integral_limits<unsigned long>::bits >= Active_Bits, ETL_ERROR(etl::bitset_overflow));

      return implementation::template value<unsigned long>(pbuffer, Number_Of_Elements);
    }

    //*************************************************************************
    /// Get as an unsigned long long.
    //*************************************************************************
    unsigned long long to_ullong() const
    {
      ETL_ASSERT(etl::integral_limits<unsigned long long>::bits >= Active_Bits, ETL_ERROR(etl::bitset_overflow));

      return implementation::template value<unsigned long long>(pbuffer, Number_Of_Elements);
    }

    //*************************************************************************
    /// Reset all of the bits.
    //*************************************************************************
    ETL_CONSTEXPR14 bitset_ext<Active_Bits, TElement>& reset() ETL_NOEXCEPT
    {
      implementation::reset_all(pbuffer, Number_Of_Elements);

      return *this;
    }

    //*************************************************************************
    /// Reset the bit at the position.
    //*************************************************************************
    ETL_CONSTEXPR14 bitset_ext<Active_Bits, TElement>& reset(size_t position)
    {
      ETL_ASSERT_OR_RETURN_VALUE(position < Active_Bits, ETL_ERROR(bitset_overflow), *this);

      implementation::reset_position(pbuffer, position);

      return *this;
    }

    //*************************************************************************
    /// Tests a bit at a position.
    /// Positions greater than the number of configured bits will return <b>false</b>.
    //*************************************************************************
    ETL_CONSTEXPR14 bool test(size_t position) const
    {
      ETL_ASSERT_OR_RETURN_VALUE(position < Active_Bits, ETL_ERROR(bitset_overflow), false);
      
      return implementation::test(pbuffer, position);
    }

    //*************************************************************************
    /// Tests a bit at a position.
    /// Positions greater than the number of configured bits will not compile.
    //*************************************************************************
    template <size_t Position>
    ETL_CONSTEXPR14 bool test() const
    {
      ETL_STATIC_ASSERT(Position < Active_Bits, "Position out of bounds");

      return implementation::test(pbuffer, Position);
    }

    //*************************************************************************
    /// The number of bits in the bitset.
    //*************************************************************************
    static ETL_CONSTEXPR size_t size() ETL_NOEXCEPT
    {
      return Active_Bits;
    }

    //*************************************************************************
    /// The number of storage elements in the bitset.
    //*************************************************************************
    static ETL_CONSTEXPR size_t number_of_elements() ETL_NOEXCEPT
    {
      return Number_Of_Elements;
    }

    //*************************************************************************
    /// The value of a set element.
    //*************************************************************************
    static ETL_CONSTEXPR element_type all_set_element() ETL_NOEXCEPT
    {
      return All_Set_Element;
    }

    //*************************************************************************
    /// The value of a clear element.
    //*************************************************************************
    static ETL_CONSTEXPR element_type all_clear_element() ETL_NOEXCEPT
    {
      return All_Clear_Element;
    }

    //*************************************************************************
    /// The mask for the msb element.
    //*************************************************************************
    static ETL_CONSTEXPR element_type top_mask() ETL_NOEXCEPT
    {
      return Top_Mask;
    }

    //*************************************************************************
    /// The number of bits in an element.
    //*************************************************************************
    static ETL_CONSTEXPR size_t bits_per_element() ETL_NOEXCEPT
    {
      return Bits_Per_Element;
    }

    //*************************************************************************
    /// The total number of bits of storage, including unused.
    //*************************************************************************
    static ETL_CONSTEXPR size_t allocated_bits() ETL_NOEXCEPT
    {
      return Number_Of_Elements * Bits_Per_Element;
    }

    //*************************************************************************
    /// The storage model for the bitset.
    /// etl::bitset_storage_model::Single
    /// etl::bitset_storage_model::Multi
    //*************************************************************************
    static ETL_CONSTEXPR etl::bitset_storage_model storage_model() ETL_NOEXCEPT
    {
      return Storage_Model;
    }

    //*************************************************************************
    /// Count the number of bits set.
    //*************************************************************************
    ETL_CONSTEXPR14 size_t count() const ETL_NOEXCEPT
    {
      return implementation::count(pbuffer, Number_Of_Elements);
    }

    //*************************************************************************
    // Are all the bits sets?
    //*************************************************************************
    ETL_CONSTEXPR14 bool all() const ETL_NOEXCEPT
    {
      return implementation::all(pbuffer, Number_Of_Elements, Top_Mask);
    }

    //*************************************************************************
    // Are all the mask bits sets?
    //*************************************************************************
    ETL_CONSTEXPR14 bool all(element_type mask) const ETL_NOEXCEPT
    {
      ETL_STATIC_ASSERT(Storage_Model == etl::bitset_storage_model::Single, "Not supported for 'Multi' bitset storage model");

      return implementation::all(pbuffer, Number_Of_Elements, Top_Mask, mask);
    }

    //*************************************************************************
    /// Are none of the bits set?
    //*************************************************************************
    ETL_CONSTEXPR14 bool none() const ETL_NOEXCEPT
    {
      return implementation::none(pbuffer, Number_Of_Elements);
    }

    //*************************************************************************
    /// Are none of the mask bits set?
    //*************************************************************************
    ETL_CONSTEXPR14 bool none(element_type mask) const ETL_NOEXCEPT
    {
      ETL_STATIC_ASSERT(Storage_Model == etl::bitset_storage_model::Single, "Not supported for 'Multi' bitset storage model");

      return implementation::none(pbuffer, Number_Of_Elements, mask);
    }

    //*************************************************************************
    /// Are any of the bits set?
    //*************************************************************************
    ETL_CONSTEXPR14 bool any() const ETL_NOEXCEPT
    {
      return implementation::any(pbuffer, Number_Of_Elements);
    }

    //*************************************************************************
    /// Are any of the mask bits set?
    //*************************************************************************
    ETL_CONSTEXPR14 bool any(element_type mask) const ETL_NOEXCEPT
    {
      ETL_STATIC_ASSERT(Storage_Model == etl::bitset_storage_model::Single, "Not supported for 'Multi' bitset storage model");

      return implementation::any(pbuffer, Number_Of_Elements, mask);
    }

    //*************************************************************************
    /// Flip all of the bits.
    //*************************************************************************
    ETL_CONSTEXPR14 bitset_ext<Active_Bits, TElement>& flip() ETL_NOEXCEPT
    {
      implementation::flip_all(pbuffer, Number_Of_Elements);

      return *this;
    }

    //*************************************************************************
    /// Flip the bit at the position.
    //*************************************************************************
    ETL_CONSTEXPR14 bitset_ext<Active_Bits, TElement>& flip(size_t position)
    {
      ETL_ASSERT_OR_RETURN_VALUE(position < Active_Bits, ETL_ERROR(bitset_overflow), *this);
      
      implementation::flip_position(pbuffer, position);

      return *this;
    }

    //*************************************************************************
    /// Read [] operator.
    //*************************************************************************
    ETL_CONSTEXPR14 bool operator[] (size_t position) const ETL_NOEXCEPT
    {
      return implementation::test(pbuffer, position);
    }

    //*************************************************************************
    /// Write [] operator.
    //*************************************************************************
    ETL_CONSTEXPR14 bit_reference operator [] (size_t position) ETL_NOEXCEPT
    {
      return bit_reference(*this, position);
    }

    //*************************************************************************
    /// Returns a string representing the bitset.
    //*************************************************************************
#if ETL_USING_CPP11
    template <typename TString = etl::string<Active_Bits>>
#else
    template <typename TString>
#endif
    ETL_CONSTEXPR14 TString to_string(typename TString::value_type zero = typename TString::value_type('0'),
                                      typename TString::value_type one  = typename TString::value_type('1')) const
    {
      return implementation::template to_string<TString>(pbuffer, Active_Bits, zero, one);
    }

    //*************************************************************************
    /// Finds the first bit in the specified state.
    ///\param state The state to search for.
    ///\returns The position of the bit or npos if none were found.
    //*************************************************************************
    ETL_CONSTEXPR14 size_t find_first(bool state) const ETL_NOEXCEPT
    {
      return implementation::find_next(pbuffer, Number_Of_Elements, Active_Bits, state, 0);
    }

    //*************************************************************************
    /// Finds the next bit in the specified state.
    ///\param state    The state to search for.
    ///\param position The position to start from.
    ///\returns The position of the bit or npos if none were found.
    //*************************************************************************
    ETL_CONSTEXPR14 size_t find_next(bool state, size_t position) const ETL_NOEXCEPT
    {
      return implementation::find_next(pbuffer, Number_Of_Elements, Active_Bits, state, position);
    }

    //*************************************************************************
    /// operator &=
    //*************************************************************************
    ETL_CONSTEXPR14 bitset_ext<Active_Bits, TElement>& operator &=(const bitset_ext<Active_Bits, TElement>& other) ETL_NOEXCEPT
    {
      implementation::operator_and(&pbuffer[0], &other.pbuffer[0], Number_Of_Elements);

      return *this;
    }

    //*************************************************************************
    /// operator |=
    //*************************************************************************
    ETL_CONSTEXPR14 bitset_ext<Active_Bits, TElement>& operator |=(const bitset_ext<Active_Bits, TElement>& other) ETL_NOEXCEPT
    {
      implementation::operator_or(&pbuffer[0], &other.pbuffer[0], Number_Of_Elements);

      return *this;
    }

    //*************************************************************************
    /// operator ^=
    //*************************************************************************
    ETL_CONSTEXPR14 bitset_ext<Active_Bits, TElement>& operator ^=(const bitset_ext<Active_Bits, TElement>& other) ETL_NOEXCEPT
    {
      implementation::operator_xor(&pbuffer[0], &other.pbuffer[0], Number_Of_Elements);

      return *this;
    }

    //*************************************************************************
    /// operator <<=
    //*************************************************************************
    ETL_CONSTEXPR14 bitset_ext<Active_Bits, TElement>& operator <<=(size_t shift) ETL_NOEXCEPT
    {
      implementation::operator_shift_left(pbuffer, Number_Of_Elements, Active_Bits, shift);

      return *this;
    }

    //*************************************************************************
    /// operator >>=
    //*************************************************************************
    ETL_CONSTEXPR14 bitset_ext<Active_Bits, TElement>& operator >>=(size_t shift) ETL_NOEXCEPT
    {
      implementation::operator_shift_right(pbuffer, Number_Of_Elements, Active_Bits, shift);

      return *this;
    }

    //*************************************************************************
    /// operator ==
    //*************************************************************************
    friend ETL_CONSTEXPR14 bool operator ==(const bitset_ext<Active_Bits, TElement>& lhs, const bitset_ext<Active_Bits, TElement>& rhs) ETL_NOEXCEPT
    {
      return implementation::operator_equality(lhs.pbuffer, rhs.pbuffer, lhs.Number_Of_Elements);
    }

    //*************************************************************************
    /// operator !=
    //*************************************************************************
    friend ETL_CONSTEXPR14 bool operator !=(const bitset_ext<Active_Bits, TElement>& lhs, const bitset_ext<Active_Bits, TElement>& rhs) ETL_NOEXCEPT
    {
      return !(lhs == rhs);
    }

    //*************************************************************************
    /// swap
    //*************************************************************************
    ETL_CONSTEXPR14 void swap(etl::bitset_ext<Active_Bits, TElement>& other) ETL_NOEXCEPT
    {
      implementation::swap(pbuffer, other.pbuffer, Number_Of_Elements);
    }

    //*************************************************************************
    /// span
    /// Returns a span of the underlying pbuffer.
    //*************************************************************************
    ETL_CONSTEXPR14 span_type span() ETL_NOEXCEPT
    {
      return span_type(pbuffer, Number_Of_Elements);
    }

    //*************************************************************************
    /// span
    /// Returns a const span of the underlying pbuffer.
    //*************************************************************************
    ETL_CONSTEXPR14 const_span_type span() const ETL_NOEXCEPT
    {
      return const_span_type(pbuffer, Number_Of_Elements);
    }

  private:

    // The implementation of the bitset functionality.
    typedef etl::bitset_impl<element_type, (Number_Of_Elements == 1U) ? etl::bitset_storage_model::Single : etl::bitset_storage_model::Multi> implementation;

    // Pointer to the storage for the bitset.
    element_type* pbuffer;
  };
}

//***************************************************************************
/// operator !=
///\ingroup bitset
//***************************************************************************
template<size_t Active_Bits, typename TElement>
ETL_CONSTEXPR14 bool operator != (const etl::bitset_ext<Active_Bits, TElement>& lhs, const etl::bitset_ext<Active_Bits, TElement>& rhs) ETL_NOEXCEPT
{
  return !(lhs == rhs);
}

//*************************************************************************
/// swap
//*************************************************************************
template <size_t Active_Bits, typename TElement>
ETL_CONSTEXPR14 void swap(etl::bitset_ext<Active_Bits, TElement>& lhs, etl::bitset_ext<Active_Bits, TElement>& rhs) ETL_NOEXCEPT
{
  lhs.swap(rhs);
}

namespace etl
{
  namespace private_bitset
  {
    //*************************************************************************
    /// Compare bitset spans.
    /// Assumes that the lhs span's element type is larger than rhs's.
    //*************************************************************************
    template <typename TLhsSpan, typename TRhsSpan>
    bool compare_bitset_spans(const TLhsSpan& lhs_span, const TRhsSpan& rhs_span)
    {
      typedef typename TLhsSpan::value_type lhs_element_t;
      typedef typename TRhsSpan::value_type rhs_element_t;

      const int steps = static_cast<int>(sizeof(lhs_element_t) / sizeof(rhs_element_t));

      typename TLhsSpan::iterator lhs_itr = lhs_span.begin();
      typename TRhsSpan::iterator rhs_itr = rhs_span.begin();

      while (lhs_itr != lhs_span.end())
      {
        const lhs_element_t& lhs_value = *lhs_itr;

        // Build the rhs element in terms of the lhs element type.
        lhs_element_t rhs_value = 0;

        const int shift_step = etl::integral_limits<rhs_element_t>::bits;
        int shift = 0;

        for (int i = 0; i < steps; ++i)
        {
          rhs_value |= (static_cast<lhs_element_t>(*rhs_itr) << shift);
          ++rhs_itr;
          shift += shift_step;
        }

        if (lhs_value != rhs_value)
        {
          return false;
        }

        ++lhs_itr;
      }

      return true;
    }
  }
}

//***************************************************************************
/// operator ==
/// bitset 
/// Different element types
///\ingroup bitset
//***************************************************************************
template<size_t Active_Bits, typename TLhsElement, typename TRhsElement>
ETL_CONSTEXPR14
typename etl::enable_if<!etl::is_same<TLhsElement, TRhsElement>::value, bool>::type
  operator == (const etl::bitset<Active_Bits, TLhsElement>& lhs, const etl::bitset<Active_Bits, TRhsElement>& rhs) ETL_NOEXCEPT
{
  // Get a span of each type.
  typename etl::bitset<Active_Bits, TLhsElement>::const_span_type lhs_span = lhs.span();
  typename etl::bitset<Active_Bits, TRhsElement>::const_span_type rhs_span = rhs.span();

  // Put the bitset with the largest element type as the first argument.
  if ETL_IF_CONSTEXPR(sizeof(TLhsElement) > sizeof(TRhsElement))
  {
    return etl::private_bitset::compare_bitset_spans(lhs_span, rhs_span);
  }
  else
  {
    return etl::private_bitset::compare_bitset_spans(rhs_span, lhs_span);
  }
}

//***************************************************************************
/// operator !=
/// bitset 
/// Different element types
///\ingroup bitset
//***************************************************************************
template<size_t Active_Bits, typename TLhsElement, typename TRhsElement>
ETL_CONSTEXPR14
typename etl::enable_if<!etl::is_same<TLhsElement, TRhsElement>::value, bool>::type
  operator != (const etl::bitset<Active_Bits, TLhsElement>& lhs, const etl::bitset<Active_Bits, TRhsElement>& rhs) ETL_NOEXCEPT
{
  return !(lhs == rhs);
}

//***************************************************************************
/// operator ==
/// bitset_ext
/// Different element types
///\ingroup bitset
//***************************************************************************
template<size_t Active_Bits, typename TLhsElement, typename TRhsElement>
ETL_CONSTEXPR14
typename etl::enable_if<!etl::is_same<TLhsElement, TRhsElement>::value, bool>::type
  operator == (const etl::bitset_ext<Active_Bits, TLhsElement>& lhs, const etl::bitset_ext<Active_Bits, TRhsElement>& rhs) ETL_NOEXCEPT
{
  // Get a span of each type.
  typename etl::bitset_ext<Active_Bits, TLhsElement>::const_span_type lhs_span = lhs.span();
  typename etl::bitset_ext<Active_Bits, TRhsElement>::const_span_type rhs_span = rhs.span();

  // Put the bitset with the largest element type as the first argument.
  if ETL_IF_CONSTEXPR(sizeof(TLhsElement) > sizeof(TRhsElement))
  {
    return etl::private_bitset::compare_bitset_spans(lhs_span, rhs_span);
  }
  else
  {
    return etl::private_bitset::compare_bitset_spans(rhs_span, lhs_span);
  }
}

//***************************************************************************
/// operator !=
/// bitset_ext
/// Different element types
///\ingroup bitset
//***************************************************************************
template<size_t Active_Bits, typename TLhsElement, typename TRhsElement>
ETL_CONSTEXPR14
typename etl::enable_if<!etl::is_same<TLhsElement, TRhsElement>::value, bool>::type
  operator != (const etl::bitset_ext<Active_Bits, TLhsElement>& lhs, const etl::bitset_ext<Active_Bits, TRhsElement>& rhs) ETL_NOEXCEPT
{
  return !(lhs == rhs);
}

//***************************************************************************
/// operator ==
/// bitset compared with bitset_ext, same element types.
///\ingroup bitset
//***************************************************************************
template<size_t Active_Bits, typename TElement>
ETL_CONSTEXPR14
bool operator == (const etl::bitset<Active_Bits, TElement>& lhs, const etl::bitset_ext<Active_Bits, TElement>& rhs) ETL_NOEXCEPT
{
  const char   Storage_Model      = etl::bitset<Active_Bits, TElement>::Storage_Model;
  const size_t Number_Of_Elements = etl::bitset<Active_Bits, TElement>::Number_Of_Elements;

  typename etl::bitset<Active_Bits, TElement>::const_span_type     lhs_span = lhs.span();
  typename etl::bitset_ext<Active_Bits, TElement>::const_span_type rhs_span = rhs.span();

  typedef etl::bitset_impl<TElement, Storage_Model> implementation;

  return implementation::operator_equality(lhs_span.begin(), rhs_span.begin(), Number_Of_Elements);
}

//***************************************************************************
/// operator !=
/// bitset compared with bitset_ext, same element types.
///\ingroup bitset
//***************************************************************************
template<size_t Active_Bits, typename TElement>
ETL_CONSTEXPR14
bool operator != (const etl::bitset<Active_Bits, TElement>& lhs, const etl::bitset_ext<Active_Bits, TElement>& rhs) ETL_NOEXCEPT
{
  return !(lhs == rhs);
}

//***************************************************************************
/// operator ==
/// bitset_ext compared with bitset, same element types.
///\ingroup bitset
//***************************************************************************
template<size_t Active_Bits, typename TElement>
ETL_CONSTEXPR14
bool operator == (const etl::bitset_ext<Active_Bits, TElement>& lhs, const etl::bitset<Active_Bits, TElement>& rhs) ETL_NOEXCEPT
{
  const char   Storage_Model      = etl::bitset<Active_Bits, TElement>::Storage_Model;
  const size_t Number_Of_Elements = etl::bitset<Active_Bits, TElement>::Number_Of_Elements;

  typename etl::bitset_ext<Active_Bits, TElement>::const_span_type lhs_span = lhs.span();
  typename etl::bitset<Active_Bits, TElement>::const_span_type     rhs_span = rhs.span();

  typedef etl::bitset_impl<TElement, Storage_Model> implementation;

  return implementation::operator_equality(lhs_span.begin(), rhs_span.begin(), Number_Of_Elements);
}

//***************************************************************************
/// operator !=
/// bitset_ext compared with bitset, same element types.
///\ingroup bitset
//***************************************************************************
template<size_t Active_Bits, typename TElement>
ETL_CONSTEXPR14
bool operator != (const etl::bitset_ext<Active_Bits, TElement>& lhs, const etl::bitset<Active_Bits, TElement>& rhs) ETL_NOEXCEPT
{
  return !(lhs == rhs);
}

//***************************************************************************
/// operator ==
/// bitset compared with bitset_ext, different element types. 
///\ingroup bitset
//***************************************************************************
template<size_t Active_Bits, typename TLhsElement, typename TRhsElement>
ETL_CONSTEXPR14
typename etl::enable_if<!etl::is_same<TLhsElement, TRhsElement>::value, bool>::type
operator == (const etl::bitset<Active_Bits, TLhsElement>& lhs, const etl::bitset_ext<Active_Bits, TRhsElement>& rhs) ETL_NOEXCEPT
{
  // Get a span of each type.
  typename etl::bitset<Active_Bits, TLhsElement>::const_span_type     lhs_span = lhs.span();
  typename etl::bitset_ext<Active_Bits, TRhsElement>::const_span_type rhs_span = rhs.span();

  // Put the bitset with the largest element type as the first argument.
  if ETL_IF_CONSTEXPR(sizeof(TLhsElement) > sizeof(TRhsElement))
  {
    return etl::private_bitset::compare_bitset_spans(lhs_span, rhs_span);
  }
  else
  {
    return etl::private_bitset::compare_bitset_spans(rhs_span, lhs_span);
  }
}

//***************************************************************************
/// operator !=
/// bitset compared with bitset_ext, different element types. 
///\ingroup bitset
//***************************************************************************
template<size_t Active_Bits, typename TLhsElement, typename TRhsElement>
ETL_CONSTEXPR14
typename etl::enable_if<!etl::is_same<TLhsElement, TRhsElement>::value, bool>::type
  operator != (const etl::bitset<Active_Bits, TLhsElement>& lhs, const etl::bitset_ext<Active_Bits, TRhsElement>& rhs) ETL_NOEXCEPT
{
  return !(lhs == rhs);
}

//***************************************************************************
/// operator ==
/// bitset_ext compared with bitset, different element types. 
///\ingroup bitset
//***************************************************************************
template<size_t Active_Bits, typename TLhsElement, typename TRhsElement>
ETL_CONSTEXPR14
typename etl::enable_if<!etl::is_same<TLhsElement, TRhsElement>::value, bool>::type
  operator == (const etl::bitset_ext<Active_Bits, TLhsElement>& lhs, const etl::bitset<Active_Bits, TRhsElement>& rhs) ETL_NOEXCEPT
{
  // Get a span of each type.
  typename etl::bitset_ext<Active_Bits, TLhsElement>::const_span_type lhs_span = lhs.span();
  typename etl::bitset<Active_Bits, TRhsElement>::const_span_type     rhs_span = rhs.span();

  // Put the bitset with the largest element type as the first argument.
  if ETL_IF_CONSTEXPR(sizeof(TLhsElement) > sizeof(TRhsElement))
  {
    return etl::private_bitset::compare_bitset_spans(lhs_span, rhs_span);
  }
  else
  {
    return etl::private_bitset::compare_bitset_spans(rhs_span, lhs_span);
  }
}

//***************************************************************************
/// operator !=
/// bitset_ext compared with bitset, different element types. 
///\ingroup bitset
//***************************************************************************
template<size_t Active_Bits, typename TLhsElement, typename TRhsElement>
ETL_CONSTEXPR14
typename etl::enable_if<!etl::is_same<TLhsElement, TRhsElement>::value, bool>::type
  operator != (const etl::bitset_ext<Active_Bits, TLhsElement>& lhs, const etl::bitset<Active_Bits, TRhsElement>& rhs) ETL_NOEXCEPT
{
  return !(lhs == rhs);
}

#include "minmax_pop.h"

#endif
