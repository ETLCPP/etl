///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2015 John Wellbelove

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

#ifndef ETL_BINARY_INCLUDED
#define ETL_BINARY_INCLUDED

///\defgroup binary binary
/// Binary utilities
///\ingroup utilities

#include "platform.h"
#include "type_traits.h"
#include "integral_limits.h"
#include "limits.h"
#include "static_assert.h"
#include "log.h"
#include "power.h"
#include "smallest.h"
#include "exception.h"
#include "error_handler.h"

#if __cpp_lib_bitops && ETL_USING_STL
  #include <bit>
#endif

namespace etl
{
  //***************************************************************************
  /// Exception for binary functions.
  ///\ingroup binary
  //***************************************************************************
  class binary_exception : public etl::exception
  {
  public:

    binary_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Full exception 'for out of range' errors.
  ///\ingroup binary
  //***************************************************************************
  class binary_out_of_range : public etl::binary_exception
  {
  public:

    binary_out_of_range(string_type file_name_, numeric_type line_number_)
      : etl::binary_exception(ETL_ERROR_TEXT("binary:out of range", ETL_BINARY_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Maximum value that can be contained in N bits.
  ///\ingroup binary
  //***************************************************************************
  /// Definition for non-zero NBITS.
  template <const size_t NBITS>
  struct max_value_for_nbits
  {
    typedef typename etl::smallest_uint_for_bits<NBITS>::type value_type;
    static ETL_CONSTANT value_type value = (value_type(1) << (NBITS - 1)) | max_value_for_nbits<NBITS - 1>::value;
  };

  /// Specialisation for when NBITS == 0.
  template <>
  struct max_value_for_nbits<0>
  {
      typedef etl::smallest_uint_for_bits<0>::type value_type;
      static ETL_CONSTANT value_type value = 0;
  };

  template <const size_t NBITS>
  ETL_CONSTANT typename max_value_for_nbits<NBITS>::value_type max_value_for_nbits<NBITS>::value;

#if __cpp_inline_variables && __cpp_constexpr
  template <const size_t NBITS>
  inline constexpr typename etl::max_value_for_nbits<NBITS>::value_type max_value_for_nbits_v = max_value_for_nbits<NBITS>::value;
#endif

  //***************************************************************************
  /// Rotate left.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 T rotate_left(T value)
  {
#if __cpp_lib_bitops && ETL_USING_STL
    return std::rotl(value, 1);
#else
    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Not an integral type");

    ETL_CONSTANT size_t SHIFT = etl::integral_limits<typename etl::make_unsigned<T>::type>::bits - 1U;

    return (value << 1U) | (value >> SHIFT);
#endif
  }

  //***************************************************************************
  /// Rotate left.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 T rotate_left(T value, size_t distance)
  {
#if __cpp_lib_bitops && ETL_USING_STL
    return std::rotl(value, distance);
#else
    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Not an integral type");

    ETL_CONSTANT size_t BITS = etl::integral_limits<typename etl::make_unsigned<T>::type>::bits;
    distance %= BITS;
    const size_t SHIFT = BITS - distance;

    if (SHIFT == BITS)
    {
      return value;
    }
    else
    {
      return (value << distance) | (value >> SHIFT);
    }
#endif
  }

  //***************************************************************************
  /// Rotate right.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 T rotate_right(T value)
  {
#if __cpp_lib_bitops && ETL_USING_STL
    return std::rotr(value, 1);
#else
    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Not an integral type");

    ETL_CONSTANT size_t SHIFT = etl::integral_limits<typename etl::make_unsigned<T>::type>::bits - 1U;

    return (value >> 1U) | (value << SHIFT);
#endif
  }

  //***************************************************************************
  /// Rotate right.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 T rotate_right(T value, size_t distance)
  {
#if __cpp_lib_bitops && ETL_USING_STL
    return std::rotr(value, distance);
#else
    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Not an integral type");

    ETL_CONSTANT size_t BITS = etl::integral_limits<typename etl::make_unsigned<T>::type>::bits;
    distance %= BITS;
    const size_t SHIFT = BITS - distance;

    if (SHIFT == BITS)
    {
      return value;
    }
    else
    {
      return (value >> distance) | (value << SHIFT);
    }
#endif
  }

  //***************************************************************************
  /// Rotate.
  /// Positive is left, negative is right.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 T rotate(T value, typename etl::make_signed<size_t>::type distance)
  {
    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Not an integral type");

    T result = T();

    if (distance > 0)
    {
      result = rotate_left(value, size_t(distance));
    }
    else
    {
      result = rotate_right(value, size_t(-distance));
    }

    return result;
  }

  //***************************************************************************
  /// Converts binary to Gray code.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR T binary_to_gray(T value)
  {
    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Not an integral type");

    return (value >> 1U) ^ value;
  }

  //***************************************************************************
  /// Fold a binary number down to a set number of bits using XOR.
  ///\ingroup binary
  //***************************************************************************
  template <typename TReturn, size_t NBITS, typename TValue>
  ETL_CONSTEXPR14 TReturn fold_bits(TValue value)
  {
    ETL_STATIC_ASSERT(integral_limits<TReturn>::bits >= NBITS, "Return type too small to hold result");

    ETL_CONSTANT TValue mask  = etl::power<2, NBITS>::value - 1U;
    ETL_CONSTANT size_t shift = NBITS;

    // Fold the value down to fit the width.
    TReturn folded_value = 0;

    // Keep shifting down and XORing the lower bits.
    while (value >= etl::max_value_for_nbits<NBITS>::value)
    {
      folded_value ^= value & mask;
      value >>= shift;
    }

    // Fold the remaining bits.
    folded_value ^= value & mask;

    return folded_value;
  }

  //***************************************************************************
  /// Sign extend.
  /// Converts an N bit binary number, where bit N-1 is the sign bit, to a signed integral type.
  ///\ingroup binary
  //***************************************************************************
  template <typename TReturn, size_t NBITS, typename TValue>
  ETL_CONSTEXPR14 TReturn sign_extend(TValue value)
  {
    ETL_STATIC_ASSERT(etl::is_integral<TValue>::value,  "TValue not an integral type");
    ETL_STATIC_ASSERT(etl::is_integral<TReturn>::value, "TReturn not an integral type");
    ETL_STATIC_ASSERT(NBITS <= etl::integral_limits<TReturn>::bits, "NBITS too large for return type");

    struct S
    {
      signed value : NBITS;
    } s = {0};

    return (s.value = value);
  }

  //***************************************************************************
  /// Sign extend.
  /// Converts an N bit binary number, where bit N-1 is the sign bit, and SHIFT
  /// is the right shift amount, to a signed integral type.
  ///\ingroup binary
  //***************************************************************************
  template <typename TReturn, size_t NBITS, size_t SHIFT, typename TValue>
  ETL_CONSTEXPR14 TReturn sign_extend(TValue value)
  {
    ETL_STATIC_ASSERT(etl::is_integral<TValue>::value,  "TValue not an integral type");
    ETL_STATIC_ASSERT(etl::is_integral<TReturn>::value, "TReturn not an integral type");
    ETL_STATIC_ASSERT(NBITS <= etl::integral_limits<TReturn>::bits, "NBITS too large for return type");
    ETL_STATIC_ASSERT(SHIFT <= etl::integral_limits<TReturn>::bits, "SHIFT too large");

    struct S
    {
      signed value : NBITS;
    } s = {0};

    return (s.value = (value >> SHIFT));
  }

  //***************************************************************************
  /// Sign extend.
  /// Converts an N bit binary number, where bit N-1 is the sign bit, to a signed integral type.
  ///\ingroup binary
  //***************************************************************************
  template <typename TReturn, typename TValue>
  ETL_CONSTEXPR14 TReturn sign_extend(TValue value, size_t NBITS)
  {
    ETL_STATIC_ASSERT(etl::is_integral<TValue>::value,  "TValue not an integral type");
    ETL_STATIC_ASSERT(etl::is_integral<TReturn>::value, "TReturn not an integral type");

    ETL_ASSERT((NBITS <= etl::integral_limits<TReturn>::bits), ETL_ERROR(binary_out_of_range));

    TReturn mask = TReturn(1) << (NBITS - 1);
    value = value & TValue((TValue(1) << NBITS) - 1);

    return TReturn((value ^ mask) - mask);
  }

  //***************************************************************************
  /// Sign extend.
  /// Converts an N bit binary number, where bit N-1 is the sign bit, and SHIFT
  /// is the right shift amount, to a signed integral type.
  ///\ingroup binary
  //***************************************************************************
  template <typename TReturn, typename TValue>
  ETL_CONSTEXPR14 TReturn sign_extend(TValue value, size_t NBITS, size_t SHIFT)
  {
    ETL_STATIC_ASSERT(etl::is_integral<TValue>::value,  "TValue not an integral type");
    ETL_STATIC_ASSERT(etl::is_integral<TReturn>::value, "TReturn not an integral type");

    ETL_ASSERT((NBITS <= etl::integral_limits<TReturn>::bits), ETL_ERROR(binary_out_of_range));

    TReturn mask = TReturn(1) << (NBITS - 1);
    value = (value >> SHIFT) & TValue((TValue(1) << NBITS) - 1);

    return TReturn((value ^ mask) - mask);
  }

  //***************************************************************************
  /// Find the position of the first set bit.
  /// Starts from LSB.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 uint_least8_t first_set_bit_position(T value)
  {
    return count_trailing_zeros(value);
  }

  //***************************************************************************
  /// Find the position of the first clear bit.
  /// Starts from LSB.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 uint_least8_t first_clear_bit_position(T value)
  {
    value = ~value;
    return count_trailing_zeros(value);
  }

  //***************************************************************************
  /// Find the position of the first bit that is clear or set.
  /// Starts from LSB.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 uint_least8_t first_bit_position(bool state, T value)
  {
    if (!state)
    {
      value = ~value;
    }

    return count_trailing_zeros(value);
  }

  //***************************************************************************
  /// Gets the value of the bit at POSITION
  /// Starts from LSB.
  ///\ingroup binary
  //***************************************************************************
  template <const size_t POSITION>
  struct bit
  {
    typedef typename etl::smallest_uint_for_bits<POSITION + 1>::type value_type;
    static ETL_CONSTANT value_type value = value_type(1) << POSITION;
  };

  template <const size_t POSITION>
  ETL_CONSTANT typename bit<POSITION>::value_type bit<POSITION>::value;

#if __cpp_inline_variables && __cpp_constexpr
  template <const size_t POSITION>
  inline constexpr typename bit<POSITION>::value_type bit_v = bit<POSITION>::value;
#endif

  //***************************************************************************
  /// Fills a value with a bit pattern.
  ///\ingroup binary
  //***************************************************************************
  template <typename TResult, typename TValue>
  ETL_CONSTEXPR TResult binary_fill(TValue value)
  {
    ETL_STATIC_ASSERT(sizeof(TResult) >= sizeof(TValue), "Result must be at least as large as the fill value");

    typedef typename etl::make_unsigned<TResult>::type unsigned_r_t;
    typedef typename etl::make_unsigned<TValue>::type  unsigned_v_t;

    return TResult(unsigned_v_t(value) * (unsigned_r_t(~unsigned_r_t(0U)) / unsigned_v_t(~unsigned_v_t(0U))));
  }

  //***************************************************************************
  /// Fills a value with a bit pattern. Partial compile time.
  ///\ingroup binary
  //***************************************************************************
  template <typename TResult, typename TValue, TValue N>
  ETL_CONSTEXPR TResult binary_fill()
  {
    ETL_STATIC_ASSERT(sizeof(TResult) >= sizeof(TValue), "Result must be at least as large as the fill value");

    typedef typename etl::make_unsigned<TResult>::type unsigned_r_t;
    typedef typename etl::make_unsigned<TValue>::type  unsigned_v_t;

    return TResult(unsigned_v_t(N) * (unsigned_r_t(~unsigned_r_t(0U)) / unsigned_v_t(~unsigned_v_t(0U))));
  }

#if ETL_USING_8BIT_TYPES
  //***************************************************************************
  /// Detects the presence of zero bytes.
  ///\ingroup binary
  //***************************************************************************
  template <typename TValue>
  ETL_CONSTEXPR14 bool has_zero_byte(TValue value)
  {
    typedef typename etl::make_unsigned<TValue>::type unsigned_t;
    ETL_CONSTEXPR14 const unsigned_t mask = etl::binary_fill<unsigned_t, uint8_t>(0x7FU);
    const unsigned_t temp = unsigned_t(~((((unsigned_t(value) & mask) + mask) | unsigned_t(value)) | mask));

    return (temp != 0U);
  }

  //***************************************************************************
  /// Detects the presence of zero bytes. Partial compile time.
  ///\ingroup binary
  //***************************************************************************
  template <typename TValue, TValue N>
  ETL_CONSTEXPR14 bool has_zero_byte()
  {
    typedef typename etl::make_unsigned<TValue>::type unsigned_t;
    ETL_CONSTEXPR14 const unsigned_t mask = etl::binary_fill<unsigned_t, uint8_t>(0x7FU);
    const unsigned_t temp = unsigned_t(~((((unsigned_t(N) & mask) + mask) | unsigned_t(N)) | mask));

    return (temp != 0U);
  }

  //***************************************************************************
  /// Detects the presence of a byte of value N. Run time.
  ///\ingroup binary
  //***************************************************************************
  template <typename TValue>
  ETL_CONSTEXPR14 bool has_byte_n(TValue value, uint8_t n)
  {
    return etl::has_zero_byte(TValue(value ^ etl::binary_fill<TValue, uint8_t>(n)));
  }

  //***************************************************************************
  /// Detects the presence of a byte of value N. Partial compile time.
  ///\ingroup binary
  //***************************************************************************
  template <typename TValue, TValue N>
  ETL_CONSTEXPR14 bool has_byte_n(TValue value)
  {
    return etl::has_zero_byte(TValue(value ^ etl::binary_fill<TValue, uint8_t>(N)));
  }
#endif

  //***************************************************************************
  /// Merges two values according to a mask.
  /// Ones in the mask select bits from 'first', zeros select bits from second.
  /// Mask is a function parameter.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR T binary_merge(T first, T second, T mask)
  {
    return second ^ ((second ^ first) & mask);
  }

  //***************************************************************************
  /// Merges two values according to a mask.
  /// Ones in the mask select bits from 'first', zeros select bits from second.
  /// Mask is a template parameter.
  ///\ingroup binary
  //***************************************************************************
  template <typename T, T MASK>
  ETL_CONSTEXPR T binary_merge(T first, T second)
  {
    return second ^ ((second ^ first) & MASK);
  }

  //***************************************************************************
  /// Reverse bits.
  ///\ingroup binary
  //***************************************************************************
  template <typename T, T Value>
  struct reverse_bits_const;

#if ETL_USING_8BIT_TYPES
  //***************************************************************************
  /// Reverse 8 bits.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 8U), T>::type
    reverse_bits(T value)
  {
    value = ((value & 0xAAU) >> 1U) | ((value & 0x55U) << 1U);
    value = ((value & 0xCCU) >> 2U) | ((value & 0x33U) << 2U);
    value = (value >> 4U) | ((value & 0x0FU) << 4U);

    return value;
  }

  //***********************************
  template <uint8_t Value>
  struct reverse_bits_const<uint8_t, Value>
  {
  private:

    static ETL_CONSTANT uint8_t value1 = uint8_t(((Value  & 0xAAU) >> 1U) | ((Value  & 0x55U) << 1U));
    static ETL_CONSTANT uint8_t value2 = uint8_t(((value1 & 0xCCU) >> 2U) | ((value1 & 0x33U) << 2U));

  public:

    static ETL_CONSTANT uint8_t value = uint8_t((value2 >> 4U) | ((value2 & 0x0FU) << 4U));
  };

  //***********************************
  template <int8_t Value>
  struct reverse_bits_const<int8_t, Value>
  {
  private:

    static ETL_CONSTANT int8_t value1 = int8_t(((Value  & 0xAAU) >> 1U) | ((Value  & 0x55U) << 1U));
    static ETL_CONSTANT int8_t value2 = int8_t(((value1 & 0xCCU) >> 2U) | ((value1 & 0x33U) << 2U));

  public:

    static ETL_CONSTANT int8_t value = int8_t((value2 >> 4U) | ((value2 & 0x0FU) << 4U));
  };
#endif

  //***************************************************************************
  /// Reverse 16 bits.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 16U), T>::type
    reverse_bits(T value)
  {
    value = ((value & 0xAAAAU) >> 1U) | ((value & 0x5555U) << 1U);
    value = ((value & 0xCCCCU) >> 2U) | ((value & 0x3333U) << 2U);
    value = ((value & 0xF0F0U) >> 4U) | ((value & 0x0F0FU) << 4U);
    value = (value >> 8U) | ((value & 0xFFU) << 8U);

    return value;
  }

  //***********************************
  template <uint16_t Value>
  struct reverse_bits_const<uint16_t, Value>
  {
  private:

    static ETL_CONSTANT uint16_t value1 = uint16_t(((Value  & 0xAAAAU) >> 1U) | ((Value  & 0x5555U) << 1U));
    static ETL_CONSTANT uint16_t value2 = uint16_t(((value1 & 0xCCCCU) >> 2U) | ((value1 & 0x3333U) << 2U));
    static ETL_CONSTANT uint16_t value3 = uint16_t(((value2 & 0xF0F0U) >> 4U) | ((value2 & 0x0F0FU) << 4U));

  public:

    static ETL_CONSTANT uint16_t value = uint16_t((value3 >> 8U) | ((value3 & 0xFFU) << 8U));
  };

  //***********************************
  template <int16_t Value>
  struct reverse_bits_const<int16_t, Value>
  {
  private:

    static ETL_CONSTANT int16_t value1 = int16_t(((Value  & 0xAAAAU) >> 1U) | ((Value  & 0x5555U) << 1U));
    static ETL_CONSTANT int16_t value2 = int16_t(((value1 & 0xCCCCU) >> 2U) | ((value1 & 0x3333U) << 2U));
    static ETL_CONSTANT int16_t value3 = int16_t(((value2 & 0xF0F0U) >> 4U) | ((value2 & 0x0F0FU) << 4U));

  public:

    static ETL_CONSTANT int16_t value = int16_t((value3 >> 8U) | ((value3 & 0xFFU) << 8U));
  };

  //***************************************************************************
  /// Reverse 32 bits.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 32U), T>::type
    reverse_bits(T value)
  {
    value = ((value & 0xAAAAAAAAUL) >>  1U) | ((value & 0x55555555UL) <<  1U);
    value = ((value & 0xCCCCCCCCUL) >>  2U) | ((value & 0x33333333UL) <<  2U);
    value = ((value & 0xF0F0F0F0UL) >>  4U) | ((value & 0x0F0F0F0FUL) <<  4U);
    value = ((value & 0xFF00FF00UL) >>  8U) | ((value & 0x00FF00FFUL) <<  8U);
    value = (value >> 16U) | ((value & 0xFFFFU) << 16U);

    return value;
  }

  //***********************************
  template <uint32_t Value>
  struct reverse_bits_const<uint32_t, Value>
  {
  private:

    static ETL_CONSTANT uint32_t value1 = uint32_t(((Value  & 0xAAAAAAAAUL) >>  1U) | ((Value  & 0x55555555UL) <<  1U));
    static ETL_CONSTANT uint32_t value2 = uint32_t(((value1 & 0xCCCCCCCCUL) >>  2U) | ((value1 & 0x33333333UL) <<  2U));
    static ETL_CONSTANT uint32_t value3 = uint32_t(((value2 & 0xF0F0F0F0UL) >>  4U) | ((value2 & 0x0F0F0F0FUL) <<  4U));
    static ETL_CONSTANT uint32_t value4 = uint32_t(((value3 & 0xFF00FF00UL) >>  8U) | ((value3 & 0x00FF00FFUL) <<  8U));

  public:

    static ETL_CONSTANT uint32_t value = uint32_t((value4 >> 16U) | ((value4 & 0xFFFFU) << 16U));
  };

  //***********************************
  template <int32_t Value>
  struct reverse_bits_const<int32_t, Value>
  {
  private:

    static ETL_CONSTANT int32_t value1 = int32_t(((Value  & 0xAAAAAAAAUL) >>  1U) | ((Value  & 0x55555555UL) <<  1U));
    static ETL_CONSTANT int32_t value2 = int32_t(((value1 & 0xCCCCCCCCUL) >>  2U) | ((value1 & 0x33333333UL) <<  2U));
    static ETL_CONSTANT int32_t value3 = int32_t(((value2 & 0xF0F0F0F0UL) >>  4U) | ((value2 & 0x0F0F0F0FUL) <<  4U));
    static ETL_CONSTANT int32_t value4 = int32_t(((value3 & 0xFF00FF00UL) >>  8U) | ((value3 & 0x00FF00FFUL) <<  8U));

  public:

    static ETL_CONSTANT int32_t value = int32_t((value4 >> 16U) | ((value4 & 0xFFFFUL) << 16U));
  };

#if ETL_USING_64BIT_TYPES
  //***************************************************************************
  /// Reverse 64 bits.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 64U), T>::type
    reverse_bits(T value)
  {
    value = ((value & 0xAAAAAAAAAAAAAAAAULL) >>  1U) | ((value & 0x5555555555555555ULL) <<  1U);
    value = ((value & 0xCCCCCCCCCCCCCCCCULL) >>  2U) | ((value & 0x3333333333333333ULL) <<  2U);
    value = ((value & 0xF0F0F0F0F0F0F0F0ULL) >>  4U) | ((value & 0x0F0F0F0F0F0F0F0FULL) <<  4U);
    value = ((value & 0xFF00FF00FF00FF00ULL) >>  8U) | ((value & 0x00FF00FF00FF00FFULL) <<  8U);
    value = ((value & 0xFFFF0000FFFF0000ULL) >> 16U) | ((value & 0x0000FFFF0000FFFFULL) << 16U);
    value = (value >> 32U) | ((value & 0xFFFFFFFFULL) << 32U);

    return value;
  }

  //***********************************
  template <uint64_t Value>
  struct reverse_bits_const<uint64_t, Value>
  {
  private:

    static ETL_CONSTANT uint64_t value1 = uint64_t(((Value  & 0xAAAAAAAAAAAAAAAAULL) >>  1U) | ((Value  & 0x5555555555555555ULL) <<  1U));
    static ETL_CONSTANT uint64_t value2 = uint64_t(((value1 & 0xCCCCCCCCCCCCCCCCULL) >>  2U) | ((value1 & 0x3333333333333333ULL) <<  2U));
    static ETL_CONSTANT uint64_t value3 = uint64_t(((value2 & 0xF0F0F0F0F0F0F0F0ULL) >>  4U) | ((value2 & 0x0F0F0F0F0F0F0F0FULL) <<  4U));
    static ETL_CONSTANT uint64_t value4 = uint64_t(((value3 & 0xFF00FF00FF00FF00ULL) >>  8U) | ((value3 & 0x00FF00FF00FF00FFULL) <<  8U));
    static ETL_CONSTANT uint64_t value5 = uint64_t(((value4 & 0xFFFF0000FFFF0000ULL) >> 16U) | ((value4 & 0x0000FFFF0000FFFFULL) << 16U));

  public:

    static ETL_CONSTANT uint64_t value = uint64_t((value5 >> 32U) | ((value5 & 0xFFFFFFFFULL) << 32U));
  };

  //***********************************
  template <int64_t Value>
  struct reverse_bits_const<int64_t, Value>
  {
  private:

    static ETL_CONSTANT int64_t value1 = int64_t(((Value  & 0xAAAAAAAAAAAAAAAAULL) >>  1U) | ((Value  & 0x5555555555555555ULL) <<  1U));
    static ETL_CONSTANT int64_t value2 = int64_t(((value1 & 0xCCCCCCCCCCCCCCCCULL) >>  2U) | ((value1 & 0x3333333333333333ULL) <<  2U));
    static ETL_CONSTANT int64_t value3 = int64_t(((value2 & 0xF0F0F0F0F0F0F0F0ULL) >>  4U) | ((value2 & 0x0F0F0F0F0F0F0F0FULL) <<  4U));
    static ETL_CONSTANT int64_t value4 = int64_t(((value3 & 0xFF00FF00FF00FF00ULL) >>  8U) | ((value3 & 0x00FF00FF00FF00FFULL) <<  8U));
    static ETL_CONSTANT int64_t value5 = int64_t(((value4 & 0xFFFF0000FFFF0000ULL) >> 16U) | ((value4 & 0x0000FFFF0000FFFFULL) << 16U));

  public:

    static ETL_CONSTANT int64_t value = int64_t((value5 >> 32U) | ((value5 & 0xFFFFFFFFULL) << 32U));
  };
#endif

  //***************************************************************************
  /// Reverse bits signed types.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_signed<T>::value, T>::type
    reverse_bits(T value)
  {
    typedef typename etl::make_unsigned<T>::type unsigned_t;

    return static_cast<T>(reverse_bits(static_cast<unsigned_t>(value)));
  }

  //***************************************************************************
  /// Reverse bytes 8 bit.
  ///\ingroup binary
  //***************************************************************************
#if ETL_USING_8BIT_TYPES
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 8U), T>::type
    reverse_bytes(T value)
  {
    return value;
  }
#endif

  //***************************************************************************
  /// Reverse bytes 16 bit.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 16U), T>::type
    reverse_bytes(T value)
  {
#if __cpp_lib_byteswap && ETL_USING_STL
    return std::byteswap(value);
#else
    return (value >> 8U) | (value << 8U);
#endif
  }

  //***************************************************************************
  /// Reverse bytes 32 bit.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 32U), T>::type
    reverse_bytes(T value)
  {
#if __cpp_lib_byteswap && ETL_USING_STL
    return std::byteswap(value);
#else
    value = ((value & 0xFF00FF00UL) >> 8U) | ((value & 0x00FF00FFUL) << 8U);
    value = (value >> 16U) | (value << 16U);

    return value;
#endif
  }

#if ETL_USING_64BIT_TYPES
  //***************************************************************************
  /// Reverse bytes 64 bit.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 64U), T>::type
    reverse_bytes(T value)
  {
#if __cpp_lib_byteswap && ETL_USING_STL
    return std::byteswap(value);
#else
    value = ((value & 0xFF00FF00FF00FF00ULL) >> 8U)  | ((value & 0x00FF00FF00FF00FFULL) << 8U);
    value = ((value & 0xFFFF0000FFFF0000ULL) >> 16U) | ((value & 0x0000FFFF0000FFFFULL) << 16U);
    value = (value >> 32U) | (value << 32U);

    return value;
#endif
  }
#endif

  //***************************************************************************
  /// Reverse bytes signed types.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_signed<T>::value, T>::type
    reverse_bytes(T value)
  {
    typedef typename etl::make_unsigned<T>::type unsigned_t;

    return static_cast<T>(reverse_bytes(static_cast<unsigned_t>(value)));
  }

#if ETL_USING_8BIT_TYPES
  //***************************************************************************
  /// Converts Gray code to binary.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 8U), T>::type
    gray_to_binary(T value)
  {
    value ^= (value >> 4U);
    value ^= (value >> 2U);
    value ^= (value >> 1U);

    return value;
  }
#endif

  //***************************************************************************
  /// Converts Gray code to binary.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 16U), T>::type
    gray_to_binary(T value)
  {
    value ^= (value >> 8U);
    value ^= (value >> 4U);
    value ^= (value >> 2U);
    value ^= (value >> 1U);

    return value;
  }

  //***************************************************************************
  /// Converts Gray code to binary.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 32U), T>::type
    gray_to_binary(T value)
  {
    value ^= (value >> 16U);
    value ^= (value >> 8U);
    value ^= (value >> 4U);
    value ^= (value >> 2U);
    value ^= (value >> 1U);

    return value;
  }

#if ETL_USING_64BIT_TYPES
  //***************************************************************************
  /// Converts Gray code to binary.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 64U), T>::type
    gray_to_binary(T value)
  {
    value ^= (value >> 32U);
    value ^= (value >> 16U);
    value ^= (value >> 8U);
    value ^= (value >> 4U);
    value ^= (value >> 2U);
    value ^= (value >> 1U);

    return value;
  }
#endif

  //***************************************************************************
  /// Converts Gray code to binary for signed.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_signed<T>::value, T>::type
    gray_to_binary(T value)
  {
    typedef typename etl::make_unsigned<T>::type unsigned_t;

    return static_cast<T>(gray_to_binary(static_cast<unsigned_t>(value)));
  }

#if ETL_USING_8BIT_TYPES
  //***************************************************************************
  /// Count set bits. 8 bits.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 8U), uint_least8_t>::type
    count_bits(T value)
  {
#if __cpp_lib_bitops && ETL_USING_STL
    return std::popcount(value);
#else
    uint32_t count = 0U;

    count = value - ((value >> 1U) & 0x55U);
    count = ((count >> 2U) & 0x33U) + (count & 0x33U);
    count = ((count >> 4U) + count) & 0x0FU;

    return uint_least8_t(count);
#endif
  }
#endif

  //***************************************************************************
  /// Count set bits. 16 bits.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 16U), uint_least8_t>::type
    count_bits(T value)
  {
#if __cpp_lib_bitops && ETL_USING_STL
    return std::popcount(value);
#else
    uint32_t count = 0U;

    count = value - ((value >> 1U) & 0x5555U);
    count = ((count >> 2U) & 0x3333U) + (count & 0x3333U);
    count = ((count >> 4U) + count) & 0x0F0FU;
    count = ((count >> 8U) + count) & 0x00FFU;

    return static_cast<uint_least8_t>(count);
#endif
  }

  //***************************************************************************
  /// Count set bits. 32 bits.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 32U), uint_least8_t>::type
    count_bits(T value)
  {
#if __cpp_lib_bitops && ETL_USING_STL
    return std::popcount(value);
#else
    uint32_t count = 0U;

    count = value - ((value >> 1U) & 0x55555555UL);
    count = ((count >> 2U) & 0x33333333UL) + (count & 0x33333333UL);
    count = ((count >> 4U)  + count) & 0x0F0F0F0FUL;
    count = ((count >> 8U)  + count) & 0x00FF00FFUL;
    count = ((count >> 16U) + count) & 0x0000FFUL;

    return static_cast<uint_least8_t>(count);
#endif
  }

#if ETL_USING_64BIT_TYPES
  //***************************************************************************
  /// Count set bits. 64 bits.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 64U), uint_least8_t>::type
    count_bits(T value)
  {
#if __cpp_lib_bitops && ETL_USING_STL
    return std::popcount(value);
#else
    uint64_t count = 0U;

    count = value - ((value >> 1U) & 0x5555555555555555ULL);
    count = ((count >> 2U) & 0x3333333333333333ULL) + (count & 0x3333333333333333ULL);
    count = ((count >> 4U)  + count) & 0x0F0F0F0F0F0F0F0FULL;
    count = ((count >> 8U)  + count) & 0x00FF00FF00FF00FFULL;
    count = ((count >> 16U) + count) & 0x0000FFFF0000FFFFULL;
    count = ((count >> 32U) + count) & 0x00000000FFFFFFFFULL;

    return static_cast<uint_least8_t>(count);
#endif
  }
#endif

  //***************************************************************************
  /// Count set bits for signed.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_signed<T>::value, uint_least8_t>::type
    count_bits(T value)
  {
    typedef typename etl::make_unsigned<T>::type unsigned_t;

    return static_cast<T>(count_bits(static_cast<unsigned_t>(value)));
  }

#if ETL_USING_8BIT_TYPES
  //***************************************************************************
  /// Parity. 8bits. 0 = even, 1 = odd
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 8U), uint_least8_t>::type
    parity(T value)
  {
    value ^= value >> 4U;
    value &= 0x0FU;
    return (0x6996U >> value) & 1U;
  }
#endif

  //***************************************************************************
  /// Parity. 16bits. 0 = even, 1 = odd
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 16U), uint_least8_t>::type
    parity(T value)
  {
    value ^= value >> 8U;
    value ^= value >> 4U;
    value &= 0x0FU;
    return (0x6996U >> value) & 1U;
  }

  //***************************************************************************
  /// Parity. 32bits. 0 = even, 1 = odd
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 32U), uint_least8_t>::type
    parity(T value)
  {
    value ^= value >> 16U;
    value ^= value >> 8U;
    value ^= value >> 4U;
    value &= 0x0FU;
    return (0x6996U >> value) & 1U;
  }

#if ETL_USING_64BIT_TYPES
  //***************************************************************************
  /// Parity. 64bits. 0 = even, 1 = odd
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 64U), uint_least8_t>::type
    parity(T value)
  {
    value ^= value >> 32U;
    value ^= value >> 16U;
    value ^= value >> 8U;
    value ^= value >> 4U;
    value &= 0x0FU;
    return (0x69966996UL >> value) & 1U;
  }
#endif

  //***************************************************************************
  /// Parity for signed.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_signed<T>::value, uint_least8_t>::type
    parity(T value)
  {
    typedef typename etl::make_unsigned<T>::type unsigned_t;

    return static_cast<T>(parity(static_cast<unsigned_t>(value)));
  }

#if ETL_USING_8BIT_TYPES
  //***************************************************************************
  /// Count trailing zeros.
  /// Uses a binary search.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 8U), uint_least8_t>::type
    count_trailing_zeros(T value)
  {
#if __cpp_lib_bitops && ETL_USING_STL
    return std::countr_zero(value);
#else
    uint_least8_t count = 0U;

    if (value & 0x1U)
    {
      count = 0U;
    }
    else
    {
      count = 1U;

      if ((value & 0xFU) == 0U)
      {
        value >>= 4U;
        count += 4U;
      }

      if ((value & 0x3U) == 0U)
      {
        value >>= 2U;
        count += 2U;
      }

      if ((value & 0x1U) == 0U)
      {
        value >>= 1U;
        count += 1U;
      }

      count -= (value & 0x1U);
    }

    return count;
#endif
  }
#endif

  //***************************************************************************
  /// Count trailing zeros. 16bit.
  /// Uses a binary search.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 16U), uint_least8_t>::type
    count_trailing_zeros(T value)
  {
#if __cpp_lib_bitops && ETL_USING_STL
    return std::countr_zero(value);
#else
    uint_least8_t count = 0U;

    if (value & 0x1U)
    {
      count = 0U;
    }
    else
    {
      count = 1U;

      if ((value & 0xFFU) == 0U)
      {
        value >>= 8U;
        count += 8U;
      }

      if ((value & 0xFU) == 0U)
      {
        value >>= 4U;
        count += 4U;
      }

      if ((value & 0x3U) == 0U)
      {
        value >>= 2U;
        count += 2U;
      }

      if ((value & 0x1U) == 0U)
      {
        value >>= 1U;
        count += 1U;
      }

      count -= value & 0x1U;
    }

    return count;
#endif
  }

  //***************************************************************************
  /// Count trailing zeros. 32bit.
  /// Uses a binary search.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 32U), uint_least8_t>::type
    count_trailing_zeros(T value)
  {
#if __cpp_lib_bitops && ETL_USING_STL
    return std::countr_zero(value);
#else
    uint_least8_t count = 0U;

    if (value & 0x1UL)
    {
      count = 0U;
    }
    else
    {
      count = 1U;

      if ((value & 0xFFFFUL) == 0UL)
      {
        value >>= 16U;
        count += 16U;
      }

      if ((value & 0xFFUL) == 0UL)
      {
        value >>= 8U;
        count += 8U;
      }

      if ((value & 0xFUL) == 0UL)
      {
        value >>= 4U;
        count += 4U;
      }

      if ((value & 0x3UL) == 0UL)
      {
        value >>= 2U;
        count += 2U;
      }

      if ((value & 0x1U) == 0U)
      {
        value >>= 1U;
        count += 1U;
      }

      count -= value & 0x1UL;
    }

    return count;
#endif
  }

#if ETL_USING_64BIT_TYPES
  //***************************************************************************
  /// Count trailing zeros. 64bit.
  /// Uses a binary search.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 64U), uint_least8_t>::type
    count_trailing_zeros(T value)
  {
#if __cpp_lib_bitops && ETL_USING_STL
    return std::countr_zero(value);
#else
      uint_least8_t count = 0U;

      if (value & 0x1ULL)
      {
        count = 0U;
      }
      else
      {
        count = 1U;

        if ((value & 0xFFFFFFFFULL) == 0ULL)
        {
          value >>= 32U;
          count += 32U;
        }

        if ((value & 0xFFFFULL) == 0ULL)
        {
          value >>= 16U;
          count += 16U;
        }

        if ((value & 0xFFULL) == 0ULL)
        {
          value >>= 8U;
          count += 8U;
        }

        if ((value & 0xFULL) == 0ULL)
        {
          value >>= 4U;
          count += 4U;
        }

        if ((value & 0x3ULL) == 0ULL)
        {
          value >>= 2U;
          count += 2U;
        }

        if ((value & 0x1U) == 0U)
        {
          value >>= 1U;
          count += 1U;
        }

        count -= value & 0x1ULL;
      }

      return count;
#endif
  }
#endif

  //***************************************************************************
  /// Count trailing zeros for signed.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_signed<T>::value, uint_least8_t>::type
    count_trailing_zeros(T value)
  {
    typedef typename etl::make_unsigned<T>::type unsigned_t;

    return static_cast<T>(count_trailing_zeros(static_cast<unsigned_t>(value)));
  }

#if ETL_USING_8BIT_TYPES
  //***************************************************************************
  /// Count trailing zeros. bit.
  /// Uses a binary search.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 8U), uint_least8_t>::type
    count_trailing_ones(T value)
  {
#if __cpp_lib_bitops && ETL_USING_STL
    return std::countr_one(value);
#else
    uint_least8_t count = 0U;

    if ((value & 0x1U) == 0x0U)
    {
      count = 0U;
    }
    else
    {
      count = 1U;

      if ((value & 0xFU) == 0xFU)
      {
        value >>= 4U;
        count += 4U;
      }

      if ((value & 0x3U) == 0x3U)
      {
        value >>= 2U;
        count += 2U;
      }

      if ((value & 0x1U) == 0x1U)
      {
        value >>= 1U;
        count += 1U;
      }

      count -= ((value & 0x1U) == 0x0U);
    }

    return count;
#endif
  }
#endif

  //***************************************************************************
  /// Count trailing zeros. 16bit.
  /// Uses a binary search.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 16U), uint_least8_t>::type
    count_trailing_ones(T value)
  {
#if __cpp_lib_bitops && ETL_USING_STL
    return std::countr_one(value);
#else
    uint_least8_t count = 0U;

    if ((value & 0x1U) == 0x0U)
    {
      count = 0U;
    }
    else
    {
      count = 1U;

      if ((value & 0xFFU) == 0xFFU)
      {
        value >>= 8U;
        count += 8U;
      }

      if ((value & 0xFU) == 0xFU)
      {
        value >>= 4U;
        count += 4U;
      }

      if ((value & 0x3U) == 0x3U)
      {
        value >>= 2U;
        count += 2U;
      }

      if ((value & 0x1U) == 0x1U)
      {
        value >>= 1U;
        count += 1U;
      }

      count -= ((value & 0x1U) == 0x0U);
    }

    return count;
#endif
  }

  //***************************************************************************
  /// Count trailing zeros. 32bit.
  /// Uses a binary search.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 32U), uint_least8_t>::type
    count_trailing_ones(T value)
  {
#if __cpp_lib_bitops && ETL_USING_STL
    return std::countr_one(value);
#else
    uint_least8_t count = 0U;

    if ((value & 0x1UL) == 0x0UL)
    {
      count = 0U;
    }
    else
    {
      count = 1U;

      if ((value & 0xFFFFUL) == 0xFFFFUL)
      {
        value >>= 16U;
        count += 16U;
      }

      if ((value & 0xFFUL) == 0xFFUL)
      {
        value >>= 8U;
        count += 8U;
      }

      if ((value & 0xFUL) == 0xFUL)
      {
        value >>= 4U;
        count += 4U;
      }

      if ((value & 0x3UL) == 0x3UL)
      {
        value >>= 2U;
        count += 2U;
      }

      if ((value & 0x1UL) == 0x1UL)
      {
        value >>= 1U;
        count += 1U;
      }

      count -= ((value & 0x1UL) == 0x0UL);
    }

    return count;
#endif
  }

#if ETL_USING_64BIT_TYPES
  //***************************************************************************
  /// Count trailing zeros. 64bit.
  /// Uses a binary search.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 64U), uint_least8_t>::type
    count_trailing_ones(T value)
  {
#if __cpp_lib_bitops && ETL_USING_STL
    return std::countr_one(value);
#else
    uint_least8_t count = 0U;

    if ((value & 0x1ULL) == 0x0ULL)
    {
      count = 0U;
    }
    else
    {
      count = 1U;

      if ((value & 0xFFFFULL) == 0xFFFFULL)
      {
        value >>= 16U;
        count += 16U;
      }

      if ((value & 0xFFULL) == 0xFFULL)
      {
        value >>= 8U;
        count += 8U;
      }

      if ((value & 0xFULL) == 0xFULL)
      {
        value >>= 4U;
        count += 4U;
      }

      if ((value & 0x3ULL) == 0x3ULL)
      {
        value >>= 2U;
        count += 2U;
      }

      if ((value & 0x1ULL) == 0x1ULL)
      {
        value >>= 1U;
        count += 1U;
      }

      count -= ((value & 0x1ULL) == 0x0ULL);
    }

    return count;
#endif
  }
#endif

  //***************************************************************************
  /// Count trailing ones for signed.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value&& etl::is_signed<T>::value, uint_least8_t>::type
    count_trailing_ones(T value)
  {
    typedef typename etl::make_unsigned<T>::type unsigned_t;

    return static_cast<T>(count_trailing_ones(static_cast<unsigned_t>(value)));
  }

#if ETL_USING_8BIT_TYPES
  //***************************************************************************
  /// Count leading zeros.
  /// Uses a binary search.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 8U), uint_least8_t>::type
    count_leading_zeros(T value)
  {
#if __cpp_lib_bitops && ETL_USING_STL
    return std::countl_zero(value);
#else
    uint_least8_t count = 0U;

    if (value & 0x80U)
    {
      count = 0U;
    }
    else
    {
      count = 1U;

      if ((value & 0xF0U) == 0U)
      {
        value <<= 4U;
        count += 4U;
      }

      if ((value & 0xC0U) == 0U)
      {
        value <<= 2U;
        count += 2U;
      }

      if ((value & 0x80U) == 0U)
      {
        value <<= 1U;
        count += 1U;
      }

      count -= ((value & 0x80U) == 0x80U);
    }

    return count;
#endif
  }
#endif

  //***************************************************************************
  /// Count leading zeros.
  /// Uses a binary search.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 16U), uint_least8_t>::type
    count_leading_zeros(T value)
  {
#if __cpp_lib_bitops && ETL_USING_STL
    return std::countl_zero(value);
#else
    uint_least8_t count = 0U;

    if (value & 0x8000U)
    {
      count = 0U;
    }
    else
    {
      count = 1U;

      if ((value & 0xFF00U) == 0U)
      {
        value <<= 8U;
        count += 8U;
      }

      if ((value & 0xF000U) == 0U)
      {
        value <<= 4U;
        count += 4U;
      }

      if ((value & 0xC000U) == 0U)
      {
        value <<= 2U;
        count += 2U;
      }

      if ((value & 0x8000U) == 0U)
      {
        value <<= 1U;
        count += 1U;
      }

      count -= ((value & 0x8000U) == 0x8000U);
    }

    return count;
#endif
  }

  //***************************************************************************
  /// Count leading zeros.
  /// Uses a binary search.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 32U), uint_least8_t>::type
    count_leading_zeros(T value)
  {
#if __cpp_lib_bitops && ETL_USING_STL
    return std::countl_zero(value);
#else
    uint_least8_t count = 0U;

    if (value & 0x80000000UL)
    {
      count = 0U;
    }
    else
    {
      count = 1U;

      if ((value & 0xFFFF0000UL) == 0U)
      {
        value <<= 16U;
        count += 16U;
      }

      if ((value & 0xFF000000UL) == 0U)
      {
        value <<= 8U;
        count += 8U;
      }

      if ((value & 0xF0000000UL) == 0U)
      {
        value <<= 4U;
        count += 4U;
      }

      if ((value & 0xC0000000UL) == 0U)
      {
        value <<= 2U;
        count += 2U;
      }

      if ((value & 0x80000000UL) == 0U)
      {
        value <<= 1U;
        count += 1U;
      }

      count -= ((value & 0x80000000UL) == 0x80000000UL);
    }

    return count;
#endif
  }

#if ETL_USING_64BIT_TYPES
  //***************************************************************************
  /// Count leading zeros.
  /// Uses a binary search.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 64U), uint_least8_t>::type
    count_leading_zeros(T value)
  {
#if __cpp_lib_bitops && ETL_USING_STL
    return std::countl_zero(value);
#else
    uint_least8_t count = 0U;

    if (value & 0x8000000000000000ULL)
    {
      count = 0U;
    }
    else
    {
      count = 1U;

      if ((value & 0xFFFFFFFFF0000000ULL) == 0U)
      {
        value <<= 32U;
        count += 32U;
      }

      if ((value & 0xFFFF000000000000ULL) == 0U)
      {
        value <<= 16U;
        count += 16U;
      }

      if ((value & 0xFF00000000000000ULL) == 0U)
      {
        value <<= 8U;
        count += 8U;
      }

      if ((value & 0xF000000000000000ULL) == 0U)
      {
        value <<= 4U;
        count += 4U;
      }

      if ((value & 0xC000000000000000ULL) == 0U)
      {
        value <<= 2U;
        count += 2U;
      }

      if ((value & 0x8000000000000000ULL) == 0U)
      {
        value <<= 1U;
        count += 1U;
      }

      count -= ((value & 0x8000000000000000ULL) == 0x8000000000000000ULL);
    }

    return count;
#endif
  }
#endif

  //***************************************************************************
  /// Count leading zeros for signed.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_signed<T>::value, uint_least8_t>::type
    count_leading_zeros(T value)
  {
    typedef typename etl::make_unsigned<T>::type unsigned_t;

    return static_cast<T>(count_trailing_ones(static_cast<unsigned_t>(value)));
  }

#if ETL_USING_8BIT_TYPES
  //***************************************************************************
  /// Count leading ones.
  /// Uses a binary search.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 8U), uint_least8_t>::type
    count_leading_ones(T value)
  {
#if __cpp_lib_bitops && ETL_USING_STL
    return std::countl_one(value);
#else
    uint_least8_t count = 0U;

    if ((value & 0x80U) == 0U)
    {
      count = 0U;
    }
    else
    {
      count = 1U;

      if ((value & 0xF0U) == 0xF0U)
      {
        value <<= 4U;
        count += 4U;
      }

      if ((value & 0xC0U) == 0xC0U)
      {
        value <<= 2U;
        count += 2U;
      }

      if ((value & 0x80U) == 0x80U)
      {
        value <<= 1U;
        count += 1U;
      }

      count -= ((value & 0x80U) == 0x0U);
    }

    return count;
#endif
  }
#endif

  //***************************************************************************
  /// Count leading ones.
  /// Uses a binary search.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 16U), uint_least8_t>::type
    count_leading_ones(T value)
  {
#if __cpp_lib_bitops && ETL_USING_STL
    return std::countl_one(value);
#else
    uint_least8_t count = 0U;

    if ((value & 0x8000U) == 0U)
    {
      count = 0U;
    }
    else
    {
      count = 1U;

      if ((value & 0xFF00U) == 0xFF00U)
      {
        value <<= 8U;
        count += 8U;
      }

      if ((value & 0xF000U) == 0xF000U)
      {
        value <<= 4U;
        count += 4U;
      }

      if ((value & 0xC000U) == 0xC000U)
      {
        value <<= 2U;
        count += 2U;
      }

      if ((value & 0x8000U) == 0x8000U)
      {
        value <<= 1U;
        count += 1U;
      }

      count -= ((value & 0x8000U) == 0U);
    }

    return count;
#endif
  }

  //***************************************************************************
  /// Count leading ones.
  /// Uses a binary search.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 32U), uint_least8_t>::type
    count_leading_ones(T value)
  {
#if __cpp_lib_bitops && ETL_USING_STL
    return std::countl_one(value);
#else
    uint_least8_t count = 0U;

    if ((value & 0x80000000UL) == 0UL)
    {
      count = 0U;
    }
    else
    {
      count = 1U;

      if ((value & 0xFFFF0000UL) == 0xFFFF0000UL)
      {
        value <<= 8U;
        count += 8U;
      }

      if ((value & 0xFF000000UL) == 0xFF000000UL)
      {
        value <<= 8U;
        count += 8U;
      }

      if ((value & 0xF0000000UL) == 0xF0000000UL)
      {
        value <<= 4U;
        count += 4U;
      }

      if ((value & 0xC0000000UL) == 0xC0000000UL)
      {
        value <<= 2U;
        count += 2U;
      }

      if ((value & 0x80000000UL) == 0x80000000UL)
      {
        value <<= 1U;
        count += 1U;
      }

      count -= ((value & 0x80000000UL) == 0UL);
    }

    return count;
#endif
  }

#if ETL_USING_64BIT_TYPES
  //***************************************************************************
  /// Count leading ones.
  /// Uses a binary search.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 64U), uint_least8_t>::type
    count_leading_ones(T value)
  {
#if __cpp_lib_bitops && ETL_USING_STL
    return std::countl_one(value);
#else
    uint_least8_t count = 0U;

    if ((value & 0x8000000000000000ULL) == 0ULL)
    {
      count = 0U;
    }
    else
    {
      count = 1U;

      if ((value & 0xFFFFFFFF00000000ULL) == 0xFFFFFFFF00000000ULL)
      {
        value <<= 8U;
        count += 8U;
      }

      if ((value & 0xFFFF000000000000ULL) == 0xFFFF000000000000ULL)
      {
        value <<= 8U;
        count += 8U;
      }

      if ((value & 0xFF00000000000000ULL) == 0xFF00000000000000ULL)
      {
        value <<= 8U;
        count += 8U;
      }

      if ((value & 0xF000000000000000ULL) == 0xF000000000000000ULL)
      {
        value <<= 4U;
        count += 4U;
      }

      if ((value & 0xC000000000000000ULL) == 0xC000000000000000ULL)
      {
        value <<= 2U;
        count += 2U;
      }

      if ((value & 0x8000000000000000ULL) == 0x8000000000000000ULL)
      {
        value <<= 1U;
        count += 1U;
      }

      count -= ((value & 0x8000000000000000ULL) == 0ULL);
    }

    return count;
#endif
  }
#endif

  //***************************************************************************
  /// Count leading ones for signed.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_signed<T>::value, uint_least8_t>::type
    count_leading_ones(T value)
  {
    typedef typename etl::make_unsigned<T>::type unsigned_t;

    return static_cast<T>(count_leading_ones(static_cast<unsigned_t>(value)));
  }

#if ETL_USING_8BIT_TYPES
  //*****************************************************************************
  /// Binary interleave
  ///\ingroup binary
  //*****************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value&& etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 8U), uint16_t>::type
    binary_interleave(T first, T second)
  {
	  uint16_t f = uint16_t(first);
	  uint16_t s = uint16_t(second);

	  f = (f | (f << 4U)) & 0x0F0FU;
	  f = (f | (f << 2U)) & 0x3333U;
	  f = (f | (f << 1U)) & 0x5555U;

	  s = (s | (s << 4U)) & 0x0F0FU;
	  s = (s | (s << 2U)) & 0x3333U;
	  s = (s | (s << 1U)) & 0x5555U;

	  return (f | (s << 1U));
  }
#endif

  //*****************************************************************************
  /// Binary interleave
  ///\ingroup binary
  //*****************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 16U), uint32_t>::type
    binary_interleave(T first, T second)
  {
	  uint32_t f = uint32_t(first);
	  uint32_t s = uint32_t(second);

	  f = (f | (f << 8U)) & 0x00FF00FFUL;
	  f = (f | (f << 4U)) & 0x0F0F0F0FUL;
	  f = (f | (f << 2U)) & 0x33333333UL;
	  f = (f | (f << 1U)) & 0x55555555UL;

	  s = (s | (s << 8U)) & 0x00FF00FFUL;
	  s = (s | (s << 4U)) & 0x0F0F0F0FUL;
	  s = (s | (s << 2U)) & 0x33333333UL;
	  s = (s | (s << 1U)) & 0x55555555UL;

	  return (f | (s << 1U));
  }

#if ETL_USING_64BIT_TYPES
  //*****************************************************************************
  /// Binary interleave
  ///\ingroup binary
  //*****************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value && (etl::integral_limits<T>::bits == 32U), uint64_t>::type
    binary_interleave(T first, T second)
  {
	  uint64_t f = uint64_t(first);
	  uint64_t s = uint64_t(second);

	  f = (f | (f << 16U)) & 0x0000FFFF0000FFFFULL;
	  f = (f | (f << 8U))  & 0x00FF00FF00FF00FFULL;
	  f = (f | (f << 4U))  & 0x0F0F0F0F0F0F0F0FULL;
	  f = (f | (f << 2U))  & 0x3333333333333333ULL;
	  f = (f | (f << 1U))  & 0x5555555555555555ULL;

	  s = (s | (s << 16U)) & 0x0000FFFF0000FFFFULL;
	  s = (s | (s << 8U))  & 0x00FF00FF00FF00FFULL;
	  s = (s | (s << 4U))  & 0x0F0F0F0F0F0F0F0FULL;
	  s = (s | (s << 2U))  & 0x3333333333333333ULL;
	  s = (s | (s << 1U))  & 0x5555555555555555ULL;

	  return (f | (s << 1U));
  }
#endif

  //*****************************************************************************
  /// Binary interleave for signed
  ///\ingroup binary
  //*****************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_signed<T>::value, T>::type  
    binary_interleave(T first, T second)
  {
    return int64_t(binary_interleave(uint16_t(first), uint16_t(second)));
  }

  //***************************************************************************
  /// Checks if odd.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR typename etl::enable_if<etl::is_integral<T>::value, bool>::type
   is_odd(T value)
  {
	  return ((static_cast<typename etl::make_unsigned<T>::type>(value) & 1U) != 0U);
  }

  //***************************************************************************
  /// Checks if even.
  ///\ingroup binary
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR typename etl::enable_if<etl::is_integral<T>::value, bool>::type
    is_even(T value)
  {
    return ((static_cast<typename etl::make_unsigned<T>::type>(value) & 1U) == 0U);
  }

  //***********************************
  template <typename T, size_t NBits>
  class lsb_mask
  {
  public:

    static ETL_CONSTANT T value = static_cast<T>(etl::max_value_for_nbits<NBits>::value);
  };

  //***********************************
  template <typename T>
  ETL_CONSTEXPR14 T make_lsb_mask(size_t nbits)
  {
    typedef typename etl::make_unsigned<T>::type type;

    return (nbits == etl::integral_limits<type>::bits) ? static_cast<T>(etl::integral_limits<type>::max)
                                                       : static_cast<T>((static_cast<type>(1U) << nbits) - 1U);
  };

  //***********************************
  template <typename T, size_t NBits>
  class msb_mask
  {
  public:

    static ETL_CONSTANT T value = static_cast<T>(etl::reverse_bits_const<T, lsb_mask<T, NBits>::value>::value);
  };

  //***********************************
  template <typename T>
  ETL_CONSTEXPR T make_msb_mask(size_t nbits)
  {
    return static_cast<T>(etl::reverse_bits(make_lsb_mask<T>(nbits)));
  };

  //***************************************************************************
  /// 8 bit binary byte constants.
  ///\ingroup binary
  //***************************************************************************
  enum binary_constant
  {
    b00000000 = 0U,
    b00000001 = 1U,
    b00000010 = 2U,
    b00000011 = 3U,
    b00000100 = 4U,
    b00000101 = 5U,
    b00000110 = 6U,
    b00000111 = 7U,
    b00001000 = 8U,
    b00001001 = 9U,
    b00001010 = 10U,
    b00001011 = 11U,
    b00001100 = 12U,
    b00001101 = 13U,
    b00001110 = 14U,
    b00001111 = 15U,
    b00010000 = 16U,
    b00010001 = 17U,
    b00010010 = 18U,
    b00010011 = 19U,
    b00010100 = 20U,
    b00010101 = 21U,
    b00010110 = 22U,
    b00010111 = 23U,
    b00011000 = 24U,
    b00011001 = 25U,
    b00011010 = 26U,
    b00011011 = 27U,
    b00011100 = 28U,
    b00011101 = 29U,
    b00011110 = 30U,
    b00011111 = 31U,
    b00100000 = 32U,
    b00100001 = 33U,
    b00100010 = 34U,
    b00100011 = 35U,
    b00100100 = 36U,
    b00100101 = 37U,
    b00100110 = 38U,
    b00100111 = 39U,
    b00101000 = 40U,
    b00101001 = 41U,
    b00101010 = 42U,
    b00101011 = 43U,
    b00101100 = 44U,
    b00101101 = 45U,
    b00101110 = 46U,
    b00101111 = 47U,
    b00110000 = 48U,
    b00110001 = 49U,
    b00110010 = 50U,
    b00110011 = 51U,
    b00110100 = 52U,
    b00110101 = 53U,
    b00110110 = 54U,
    b00110111 = 55U,
    b00111000 = 56U,
    b00111001 = 57U,
    b00111010 = 58U,
    b00111011 = 59U,
    b00111100 = 60U,
    b00111101 = 61U,
    b00111110 = 62U,
    b00111111 = 63U,
    b01000000 = 64U,
    b01000001 = 65U,
    b01000010 = 66U,
    b01000011 = 67U,
    b01000100 = 68U,
    b01000101 = 69U,
    b01000110 = 70U,
    b01000111 = 71U,
    b01001000 = 72U,
    b01001001 = 73U,
    b01001010 = 74U,
    b01001011 = 75U,
    b01001100 = 76U,
    b01001101 = 77U,
    b01001110 = 78U,
    b01001111 = 79U,
    b01010000 = 80U,
    b01010001 = 81U,
    b01010010 = 82U,
    b01010011 = 83U,
    b01010100 = 84U,
    b01010101 = 85U,
    b01010110 = 86U,
    b01010111 = 87U,
    b01011000 = 88U,
    b01011001 = 89U,
    b01011010 = 90U,
    b01011011 = 91U,
    b01011100 = 92U,
    b01011101 = 93U,
    b01011110 = 94U,
    b01011111 = 95U,
    b01100000 = 96U,
    b01100001 = 97U,
    b01100010 = 98U,
    b01100011 = 99U,
    b01100100 = 100U,
    b01100101 = 101U,
    b01100110 = 102U,
    b01100111 = 103U,
    b01101000 = 104U,
    b01101001 = 105U,
    b01101010 = 106U,
    b01101011 = 107U,
    b01101100 = 108U,
    b01101101 = 109U,
    b01101110 = 110U,
    b01101111 = 111U,
    b01110000 = 112U,
    b01110001 = 113U,
    b01110010 = 114U,
    b01110011 = 115U,
    b01110100 = 116U,
    b01110101 = 117U,
    b01110110 = 118U,
    b01110111 = 119U,
    b01111000 = 120U,
    b01111001 = 121U,
    b01111010 = 122U,
    b01111011 = 123U,
    b01111100 = 124U,
    b01111101 = 125U,
    b01111110 = 126U,
    b01111111 = 127U,
    b10000000 = 128U,
    b10000001 = 129U,
    b10000010 = 130U,
    b10000011 = 131U,
    b10000100 = 132U,
    b10000101 = 133U,
    b10000110 = 134U,
    b10000111 = 135U,
    b10001000 = 136U,
    b10001001 = 137U,
    b10001010 = 138U,
    b10001011 = 139U,
    b10001100 = 140U,
    b10001101 = 141U,
    b10001110 = 142U,
    b10001111 = 143U,
    b10010000 = 144U,
    b10010001 = 145U,
    b10010010 = 146U,
    b10010011 = 147U,
    b10010100 = 148U,
    b10010101 = 149U,
    b10010110 = 150U,
    b10010111 = 151U,
    b10011000 = 152U,
    b10011001 = 153U,
    b10011010 = 154U,
    b10011011 = 155U,
    b10011100 = 156U,
    b10011101 = 157U,
    b10011110 = 158U,
    b10011111 = 159U,
    b10100000 = 160U,
    b10100001 = 161U,
    b10100010 = 162U,
    b10100011 = 163U,
    b10100100 = 164U,
    b10100101 = 165U,
    b10100110 = 166U,
    b10100111 = 167U,
    b10101000 = 168U,
    b10101001 = 169U,
    b10101010 = 170U,
    b10101011 = 171U,
    b10101100 = 172U,
    b10101101 = 173U,
    b10101110 = 174U,
    b10101111 = 175U,
    b10110000 = 176U,
    b10110001 = 177U,
    b10110010 = 178U,
    b10110011 = 179U,
    b10110100 = 180U,
    b10110101 = 181U,
    b10110110 = 182U,
    b10110111 = 183U,
    b10111000 = 184U,
    b10111001 = 185U,
    b10111010 = 186U,
    b10111011 = 187U,
    b10111100 = 188U,
    b10111101 = 189U,
    b10111110 = 190U,
    b10111111 = 191U,
    b11000000 = 192U,
    b11000001 = 193U,
    b11000010 = 194U,
    b11000011 = 195U,
    b11000100 = 196U,
    b11000101 = 197U,
    b11000110 = 198U,
    b11000111 = 199U,
    b11001000 = 200U,
    b11001001 = 201U,
    b11001010 = 202U,
    b11001011 = 203U,
    b11001100 = 204U,
    b11001101 = 205U,
    b11001110 = 206U,
    b11001111 = 207U,
    b11010000 = 208U,
    b11010001 = 209U,
    b11010010 = 210U,
    b11010011 = 211U,
    b11010100 = 212U,
    b11010101 = 213U,
    b11010110 = 214U,
    b11010111 = 215U,
    b11011000 = 216U,
    b11011001 = 217U,
    b11011010 = 218U,
    b11011011 = 219U,
    b11011100 = 220U,
    b11011101 = 221U,
    b11011110 = 222U,
    b11011111 = 223U,
    b11100000 = 224U,
    b11100001 = 225U,
    b11100010 = 226U,
    b11100011 = 227U,
    b11100100 = 228U,
    b11100101 = 229U,
    b11100110 = 230U,
    b11100111 = 231U,
    b11101000 = 232U,
    b11101001 = 233U,
    b11101010 = 234U,
    b11101011 = 235U,
    b11101100 = 236U,
    b11101101 = 237U,
    b11101110 = 238U,
    b11101111 = 239U,
    b11110000 = 240U,
    b11110001 = 241U,
    b11110010 = 242U,
    b11110011 = 243U,
    b11110100 = 244U,
    b11110101 = 245U,
    b11110110 = 246U,
    b11110111 = 247U,
    b11111000 = 248U,
    b11111001 = 249U,
    b11111010 = 250U,
    b11111011 = 251U,
    b11111100 = 252U,
    b11111101 = 253U,
    b11111110 = 254U,
    b11111111 = 255U
  };

  //***************************************************************************
  /// Binary bit constants.
  ///\ingroup binary
  //***************************************************************************
  enum bit_constant
  {
    b0  = 0x1UL,
    b1  = 0x2UL,
    b2  = 0x4UL,
    b3  = 0x8UL,
    b4  = 0x10UL,
    b5  = 0x20UL,
    b6  = 0x40UL,
    b7  = 0x80UL,
    b8  = 0x100UL,
    b9  = 0x200UL,
    b10 = 0x400UL,
    b11 = 0x800UL,
    b12 = 0x1000UL,
    b13 = 0x2000UL,
    b14 = 0x4000UL,
    b15 = 0x8000UL,
    b16 = 0x10000UL,
    b17 = 0x20000UL,
    b18 = 0x40000UL,
    b19 = 0x80000UL,
    b20 = 0x100000UL,
    b21 = 0x200000UL,
    b22 = 0x400000UL,
    b23 = 0x800000UL,
    b24 = 0x1000000UL,
    b25 = 0x2000000UL,
    b26 = 0x4000000UL,
    b27 = 0x8000000UL,
    b28 = 0x10000000UL,
    b29 = 0x20000000UL,
    b30 = 0x40000000UL,
    b31 = 0x80000000UL
  };
}

#endif
