///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2015 jwellbelove

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

#ifndef __ETL_BINARY__
#define __ETL_BINARY__

///\defgroup binary binary
/// Binary utilities
///\ingroup utilities

#include <limits>
#include <assert.h>

#include "platform.h"
#include "type_traits.h"
#include "integral_limits.h"
#include "static_assert.h"
#include "log.h"
#include "power.h"
#include "smallest.h"
#include "platform.h"

namespace etl
{
  //***************************************************************************
  /// Maximum value that can be contained in N bits.
  //***************************************************************************
  /// Definition for non-zero NBITS.
  template <const size_t NBITS>
  struct max_value_for_nbits
  {
    typedef typename etl::smallest_uint_for_bits<NBITS>::type value_type;
    static const value_type value = (value_type(1) << (NBITS - 1)) | max_value_for_nbits<NBITS - 1>::value;
  };

  /// Specialisation for when NBITS == 0.
  template <>
  struct max_value_for_nbits<0>
  {
      typedef etl::smallest_uint_for_bits<0>::type value_type;
      static const value_type value = 0;
  };

  template <const size_t NBITS>
  const typename max_value_for_nbits<NBITS>::value_type max_value_for_nbits<NBITS>::value;

  //***************************************************************************
  /// Rotate left.
  //***************************************************************************
  template <typename T>
  T rotate_left(T value)
  {
    STATIC_ASSERT(etl::is_integral<T>::value, "Not an integral type");

    const size_t SHIFT = etl::integral_limits<typename etl::make_unsigned<T>::type>::bits - 1;

    return (value << 1) | (value >> SHIFT);
  }

  //***************************************************************************
  /// Rotate left.
  //***************************************************************************
  template <typename T>
  T rotate_left(T value, size_t distance)
  {
    STATIC_ASSERT(etl::is_integral<T>::value, "Not an integral type");

    const size_t BITS = etl::integral_limits<typename etl::make_unsigned<T>::type>::bits;
    distance %= BITS;
    const size_t SHIFT = BITS - distance;

    return (value << distance) | (value >> SHIFT);
  }

  //***************************************************************************
  /// Rotate right.
  //***************************************************************************
  template <typename T>
  T rotate_right(T value)
  {
    STATIC_ASSERT(etl::is_integral<T>::value, "Not an integral type");

    const size_t SHIFT = etl::integral_limits<typename etl::make_unsigned<T>::type>::bits - 1;

    return (value >> 1) | (value << SHIFT);
  }

  //***************************************************************************
  /// Rotate right.
  //***************************************************************************
  template <typename T>
  T rotate_right(T value, size_t distance)
  {
    STATIC_ASSERT(etl::is_integral<T>::value, "Not an integral type");

    const size_t BITS = etl::integral_limits<typename etl::make_unsigned<T>::type>::bits;
    distance %= BITS;
    const size_t SHIFT = BITS - distance;

    return (value >> distance) | (value << SHIFT);
  }

  //***************************************************************************
  /// Rotate.
  /// Positive is left, negative is right.
  //***************************************************************************
  template <typename T>
  T rotate(T value, typename etl::make_signed<size_t>::type distance)
  {
    STATIC_ASSERT(etl::is_integral<T>::value, "Not an integral type");

    T result;

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
  /// Reverse bits.
  //***************************************************************************
#if ETL_8BIT_SUPPORT
  uint8_t reverse_bits(uint8_t value);
  inline int8_t reverse_bits(int8_t value) { return int8_t(reverse_bits(uint8_t(value))); }
#endif
  uint16_t reverse_bits(uint16_t value);
  inline int16_t reverse_bits(int16_t value) { return int16_t(reverse_bits(uint16_t(value))); }
  uint32_t reverse_bits(uint32_t value);
  inline int32_t reverse_bits(int32_t value) { return int32_t(reverse_bits(uint32_t(value))); }
  uint64_t reverse_bits(uint64_t value);
  inline int64_t reverse_bits(int64_t value) { return int64_t(reverse_bits(uint64_t(value))); }

  //***************************************************************************
  /// Reverse bytes.
  //***************************************************************************
#if ETL_8BIT_SUPPORT
  inline uint8_t reverse_bytes(uint8_t value) { return value; }
  inline int8_t reverse_bytes(int8_t value) { return value; }
#endif
  uint16_t reverse_bytes(uint16_t value);
  inline int16_t reverse_bytes(int16_t value) { return int16_t(reverse_bytes(uint16_t(value))); }
  uint32_t reverse_bytes(uint32_t value);
  inline int32_t reverse_bytes(int32_t value) { return int32_t(reverse_bytes(uint32_t(value))); }
  uint64_t reverse_bytes(uint64_t value);
  inline int64_t reverse_bytes(int64_t value) { return int64_t(reverse_bytes(uint64_t(value))); }

  //***************************************************************************
  /// Converts binary to Gray code.
  //***************************************************************************
  template <typename T>
  T binary_to_gray(T value)
  {
    STATIC_ASSERT(etl::is_integral<T>::value, "Not an integral type");

    return (value >> 1) ^ value;
  }

  //***************************************************************************
  /// Converts Gray code to binary.
  //***************************************************************************
#if ETL_8BIT_SUPPORT
  uint8_t gray_to_binary(uint8_t value);
  inline int8_t gray_to_binary(int8_t value) { return int8_t(gray_to_binary(uint8_t(value))); }
#endif
  uint16_t gray_to_binary(uint16_t value);
  inline int16_t gray_to_binary(int16_t value) { return int16_t(gray_to_binary(uint16_t(value))); }
  uint32_t gray_to_binary(uint32_t value);
  inline int32_t gray_to_binary(int32_t value) { return int32_t(gray_to_binary(uint32_t(value))); }
  uint64_t gray_to_binary(uint64_t value);
  inline int64_t gray_to_binary(int64_t value) { return int64_t(gray_to_binary(uint64_t(value))); }

  //***************************************************************************
  /// Count set bits.
  //***************************************************************************
#if ETL_8BIT_SUPPORT
  uint_least8_t count_bits(uint8_t value);
  inline uint_least8_t count_bits(int8_t value) { return count_bits(uint8_t(value)); }
#endif
  uint_least8_t count_bits(uint16_t value);
  inline uint_least8_t count_bits(int16_t value) { return count_bits(uint16_t(value)); }
  uint_least8_t count_bits(uint32_t value);
  inline uint_least8_t count_bits(int32_t value) { return count_bits(uint32_t(value)); }
  uint_least8_t count_bits(uint64_t value);
  inline uint_least8_t count_bits(int64_t value) { return count_bits(uint64_t(value)); }

  //***************************************************************************
  /// Parity. 0 = even, 1 = odd
  //***************************************************************************
#if ETL_8BIT_SUPPORT
  uint_least8_t parity(uint8_t value);
  inline uint_least8_t parity(int8_t value) { return parity(uint8_t(value)); }
#endif
  uint_least8_t parity(uint16_t value);
  inline uint_least8_t parity(int16_t value) { return parity(uint16_t(value)); }
  uint_least8_t parity(uint32_t value);
  inline uint_least8_t parity(int32_t value) { return parity(uint32_t(value)); }
  uint_least8_t parity(uint64_t value);
  inline uint_least8_t parity(int64_t value) { return parity(uint64_t(value)); }

  //***************************************************************************
  /// Fold a binary number down to a set number of bits using XOR.
  //***************************************************************************
  template <typename TReturn, const size_t NBITS, typename TValue>
  TReturn fold_bits(TValue value)
  {
    STATIC_ASSERT(integral_limits<TReturn>::bits >= NBITS, "Return type too small to hold result");

    const TValue mask  = etl::power<2, NBITS>::value - 1;
    const size_t shift = NBITS;

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
  //***************************************************************************
  template <typename TReturn, const size_t NBITS, typename TValue>
  TReturn sign_extend(TValue value)
  {
    STATIC_ASSERT(etl::is_integral<TValue>::value,  "TValue not an integral type");
    STATIC_ASSERT(etl::is_integral<TReturn>::value, "TReturn not an integral type");
    STATIC_ASSERT(etl::is_signed<TReturn>::value,   "TReturn not a signed type");
    STATIC_ASSERT(NBITS <= std::numeric_limits<TReturn>::digits, "NBITS too large for return type");

    const TReturn negative = (TReturn(1) << (NBITS - 1));
    TReturn signed_value = value & ((1 << NBITS) - 1);

    if ((signed_value & negative) != 0)
    {
      const TReturn sign_bits = ~((TReturn(1) << NBITS) - 1);
      signed_value |= sign_bits;
    }

    return signed_value;
  }

  //***************************************************************************
  /// Sign extend.
  /// Converts an N bit binary number, where bit N-1 is the sign bit, and SHIFT
  /// is the right shift amount, to a signed integral type.
  //***************************************************************************
  template <typename TReturn, const size_t NBITS, const size_t SHIFT, typename TValue>
  TReturn sign_extend(TValue value)
  {
    STATIC_ASSERT(etl::is_integral<TValue>::value, "TValue not an integral type");
    STATIC_ASSERT(etl::is_integral<TReturn>::value, "TReturn not an integral type");
    STATIC_ASSERT(etl::is_signed<TReturn>::value, "TReturn not a signed type");
    STATIC_ASSERT(NBITS <= std::numeric_limits<TReturn>::digits, "NBITS too large for return type");
    STATIC_ASSERT(SHIFT <= std::numeric_limits<TReturn>::digits, "SHIFT too large");

    const TReturn negative = (TReturn(1) << (NBITS - 1));
    TReturn signed_value = (value >> SHIFT) & ((1 << NBITS) - 1);

    if ((signed_value & negative) != 0)
    {
      const TReturn sign_bits = ~((TReturn(1) << NBITS) - 1);
      signed_value |= sign_bits;
    }

    return signed_value;
  }

  //***************************************************************************
  /// Sign extend.
  /// Converts an N bit binary number, where bit N-1 is the sign bit, to a signed integral type.
  //***************************************************************************
  template <typename TReturn, typename TValue>
  TReturn sign_extend(TValue value, const size_t NBITS)
  {
    STATIC_ASSERT(etl::is_integral<TValue>::value,  "TValue not an integral type");
    STATIC_ASSERT(etl::is_integral<TReturn>::value, "TReturn not an integral type");
    STATIC_ASSERT(etl::is_signed<TReturn>::value,   "TReturn not a signed type");
    assert(NBITS <= std::numeric_limits<TReturn>::digits);

    const TReturn negative = (TReturn(1) << (NBITS - 1));
    TReturn signed_value = value & ((1 << NBITS) - 1);

    if ((signed_value & negative) != 0)
    {
      const TReturn sign_bits = ~((TReturn(1) << NBITS) - 1);
      signed_value |= sign_bits;
    }

    return signed_value;
  }

  //***************************************************************************
  /// Sign extend.
  /// Converts an N bit binary number, where bit N-1 is the sign bit, and SHIFT
  /// is the right shift amount, to a signed integral type.
  //***************************************************************************
  template <typename TReturn, typename TValue>
  TReturn sign_extend(TValue value, const size_t NBITS, const size_t SHIFT)
  {
    STATIC_ASSERT(etl::is_integral<TValue>::value, "TValue not an integral type");
    STATIC_ASSERT(etl::is_integral<TReturn>::value, "TReturn not an integral type");
    STATIC_ASSERT(etl::is_signed<TReturn>::value, "TReturn not a signed type");
    assert(NBITS <= std::numeric_limits<TReturn>::digits);

    const TReturn negative = (TReturn(1) << (NBITS - 1));
    TReturn signed_value = (value >> SHIFT) & ((1 << NBITS) - 1);

    if ((signed_value & negative) != 0)
    {
      const TReturn sign_bits = ~((TReturn(1) << NBITS) - 1);
      signed_value |= sign_bits;
    }

    return signed_value;
  }

  //***************************************************************************
  /// Count trailing zeros. bit.
  /// Uses a binary search.
  //***************************************************************************
#if ETL_8BIT_SUPPORT
  uint_least8_t count_trailing_zeros(uint8_t value);
  inline uint_least8_t count_trailing_zeros(int8_t value) { return count_trailing_zeros(uint8_t(value)); }
#endif
  uint_least8_t count_trailing_zeros(uint16_t value);
  inline uint_least8_t count_trailing_zeros(int16_t value) { return count_trailing_zeros(uint16_t(value)); }
  uint_least8_t count_trailing_zeros(uint32_t value);
  inline uint_least8_t count_trailing_zeros(int32_t value) { return count_trailing_zeros(uint32_t(value)); }
  uint_least8_t count_trailing_zeros(uint64_t value);
  inline uint_least8_t count_trailing_zeros(int64_t value) { return count_trailing_zeros(uint64_t(value)); }

  //***************************************************************************
  /// Find the position of the first set bit.
  /// Starts from LSB.
  //***************************************************************************
  template <typename T>
  uint_least8_t first_set_bit_position(T value)
  {
    return count_trailing_zeros(value);
  }

  //***************************************************************************
  /// Find the position of the first clear bit.
  /// Starts from LSB.
  //***************************************************************************
  template <typename T>
  uint_least8_t first_clear_bit_position(T value)
  {
    value = ~value;
    return count_trailing_zeros(value);
  }

  //***************************************************************************
  /// Find the position of the first bit that is clear or set.
  /// Starts from LSB.
  //***************************************************************************
  template <typename T>
  uint_least8_t first_bit_position(bool state, T value)
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
  //***************************************************************************
  template <const size_t POSITION>
  struct bit
  {
    typedef typename etl::smallest_uint_for_bits<POSITION + 1>::type value_type;
    static const value_type value = value_type(1) << POSITION;
  };

  template <const size_t POSITION>
  const typename bit<POSITION>::value_type bit<POSITION>::value;

  //***************************************************************************
  /// 8 bit binary constants.
  //***************************************************************************
  enum binary_constant
  {
    b00000000 = 0,
    b00000001 = 1,
    b00000010 = 2,
    b00000011 = 3,
    b00000100 = 4,
    b00000101 = 5,
    b00000110 = 6,
    b00000111 = 7,
    b00001000 = 8,
    b00001001 = 9,
    b00001010 = 10,
    b00001011 = 11,
    b00001100 = 12,
    b00001101 = 13,
    b00001110 = 14,
    b00001111 = 15,
    b00010000 = 16,
    b00010001 = 17,
    b00010010 = 18,
    b00010011 = 19,
    b00010100 = 20,
    b00010101 = 21,
    b00010110 = 22,
    b00010111 = 23,
    b00011000 = 24,
    b00011001 = 25,
    b00011010 = 26,
    b00011011 = 27,
    b00011100 = 28,
    b00011101 = 29,
    b00011110 = 30,
    b00011111 = 31,
    b00100000 = 32,
    b00100001 = 33,
    b00100010 = 34,
    b00100011 = 35,
    b00100100 = 36,
    b00100101 = 37,
    b00100110 = 38,
    b00100111 = 39,
    b00101000 = 40,
    b00101001 = 41,
    b00101010 = 42,
    b00101011 = 43,
    b00101100 = 44,
    b00101101 = 45,
    b00101110 = 46,
    b00101111 = 47,
    b00110000 = 48,
    b00110001 = 49,
    b00110010 = 50,
    b00110011 = 51,
    b00110100 = 52,
    b00110101 = 53,
    b00110110 = 54,
    b00110111 = 55,
    b00111000 = 56,
    b00111001 = 57,
    b00111010 = 58,
    b00111011 = 59,
    b00111100 = 60,
    b00111101 = 61,
    b00111110 = 62,
    b00111111 = 63,
    b01000000 = 64,
    b01000001 = 65,
    b01000010 = 66,
    b01000011 = 67,
    b01000100 = 68,
    b01000101 = 69,
    b01000110 = 70,
    b01000111 = 71,
    b01001000 = 72,
    b01001001 = 73,
    b01001010 = 74,
    b01001011 = 75,
    b01001100 = 76,
    b01001101 = 77,
    b01001110 = 78,
    b01001111 = 79,
    b01010000 = 80,
    b01010001 = 81,
    b01010010 = 82,
    b01010011 = 83,
    b01010100 = 84,
    b01010101 = 85,
    b01010110 = 86,
    b01010111 = 87,
    b01011000 = 88,
    b01011001 = 89,
    b01011010 = 90,
    b01011011 = 91,
    b01011100 = 92,
    b01011101 = 93,
    b01011110 = 94,
    b01011111 = 95,
    b01100000 = 96,
    b01100001 = 97,
    b01100010 = 98,
    b01100011 = 99,
    b01100100 = 100,
    b01100101 = 101,
    b01100110 = 102,
    b01100111 = 103,
    b01101000 = 104,
    b01101001 = 105,
    b01101010 = 106,
    b01101011 = 107,
    b01101100 = 108,
    b01101101 = 109,
    b01101110 = 110,
    b01101111 = 111,
    b01110000 = 112,
    b01110001 = 113,
    b01110010 = 114,
    b01110011 = 115,
    b01110100 = 116,
    b01110101 = 117,
    b01110110 = 118,
    b01110111 = 119,
    b01111000 = 120,
    b01111001 = 121,
    b01111010 = 122,
    b01111011 = 123,
    b01111100 = 124,
    b01111101 = 125,
    b01111110 = 126,
    b01111111 = 127,
    b10000000 = 128,
    b10000001 = 129,
    b10000010 = 130,
    b10000011 = 131,
    b10000100 = 132,
    b10000101 = 133,
    b10000110 = 134,
    b10000111 = 135,
    b10001000 = 136,
    b10001001 = 137,
    b10001010 = 138,
    b10001011 = 139,
    b10001100 = 140,
    b10001101 = 141,
    b10001110 = 142,
    b10001111 = 143,
    b10010000 = 144,
    b10010001 = 145,
    b10010010 = 146,
    b10010011 = 147,
    b10010100 = 148,
    b10010101 = 149,
    b10010110 = 150,
    b10010111 = 151,
    b10011000 = 152,
    b10011001 = 153,
    b10011010 = 154,
    b10011011 = 155,
    b10011100 = 156,
    b10011101 = 157,
    b10011110 = 158,
    b10011111 = 159,
    b10100000 = 160,
    b10100001 = 161,
    b10100010 = 162,
    b10100011 = 163,
    b10100100 = 164,
    b10100101 = 165,
    b10100110 = 166,
    b10100111 = 167,
    b10101000 = 168,
    b10101001 = 169,
    b10101010 = 170,
    b10101011 = 171,
    b10101100 = 172,
    b10101101 = 173,
    b10101110 = 174,
    b10101111 = 175,
    b10110000 = 176,
    b10110001 = 177,
    b10110010 = 178,
    b10110011 = 179,
    b10110100 = 180,
    b10110101 = 181,
    b10110110 = 182,
    b10110111 = 183,
    b10111000 = 184,
    b10111001 = 185,
    b10111010 = 186,
    b10111011 = 187,
    b10111100 = 188,
    b10111101 = 189,
    b10111110 = 190,
    b10111111 = 191,
    b11000000 = 192,
    b11000001 = 193,
    b11000010 = 194,
    b11000011 = 195,
    b11000100 = 196,
    b11000101 = 197,
    b11000110 = 198,
    b11000111 = 199,
    b11001000 = 200,
    b11001001 = 201,
    b11001010 = 202,
    b11001011 = 203,
    b11001100 = 204,
    b11001101 = 205,
    b11001110 = 206,
    b11001111 = 207,
    b11010000 = 208,
    b11010001 = 209,
    b11010010 = 210,
    b11010011 = 211,
    b11010100 = 212,
    b11010101 = 213,
    b11010110 = 214,
    b11010111 = 215,
    b11011000 = 216,
    b11011001 = 217,
    b11011010 = 218,
    b11011011 = 219,
    b11011100 = 220,
    b11011101 = 221,
    b11011110 = 222,
    b11011111 = 223,
    b11100000 = 224,
    b11100001 = 225,
    b11100010 = 226,
    b11100011 = 227,
    b11100100 = 228,
    b11100101 = 229,
    b11100110 = 230,
    b11100111 = 231,
    b11101000 = 232,
    b11101001 = 233,
    b11101010 = 234,
    b11101011 = 235,
    b11101100 = 236,
    b11101101 = 237,
    b11101110 = 238,
    b11101111 = 239,
    b11110000 = 240,
    b11110001 = 241,
    b11110010 = 242,
    b11110011 = 243,
    b11110100 = 244,
    b11110101 = 245,
    b11110110 = 246,
    b11110111 = 247,
    b11111000 = 248,
    b11111001 = 249,
    b11111010 = 250,
    b11111011 = 251,
    b11111100 = 252,
    b11111101 = 253,
    b11111110 = 254,
    b11111111 = 255
  };

  enum bit_constant
  {
    b0  = 0x1,
    b1  = 0x2,
    b2  = 0x4,
    b3  = 0x8,
    b4  = 0x10,
    b5  = 0x20,
    b6  = 0x40,
    b7  = 0x80,
    b8  = 0x100,
    b9  = 0x200,
    b10 = 0x400,
    b11 = 0x800,
    b12 = 0x1000,
    b13 = 0x2000,
    b14 = 0x4000,
    b15 = 0x8000,
    b16 = 0x10000,
    b17 = 0x20000,
    b18 = 0x40000,
    b19 = 0x80000,
    b20 = 0x100000,
    b21 = 0x200000,
    b22 = 0x400000,
    b23 = 0x800000,
    b24 = 0x1000000,
    b25 = 0x2000000,
    b26 = 0x4000000,
    b27 = 0x8000000,
    b28 = 0x10000000,
    b29 = 0x20000000,
    b30 = 0x40000000,
    b31 = 0x80000000
  };
}

#endif
