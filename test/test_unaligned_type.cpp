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

#include "unit_test_framework.h"

#include "etl/integral_limits.h"
#include "etl/unaligned_type.h"

#include "etl/private/diagnostic_useless_cast_push.h"

#include <array>
#include <cstring>

namespace
{
  //***************************************************************************
  /// Representation level helpers.
  /// 'memcpy' is used rather than 'etl::bit_cast' so that these work in every
  /// configuration, including those without a bit_cast builtin.
  //***************************************************************************
  uint32_t bits_of(float value)
  {
    uint32_t bits = 0U;
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
  }

  //***************************************************************************
  float float_from_bits(uint32_t bits)
  {
    float value = 0.0f;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
  }

  //***************************************************************************
  uint64_t bits_of(double value)
  {
    uint64_t bits = 0U;
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
  }

  //***************************************************************************
  double double_from_bits(uint64_t bits)
  {
    double value = 0.0;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
  }

#if ETL_USING_CPP14 && ETL_USING_BUILTIN_BIT_CAST
  //***************************************************************************
  /// Encodes 'bits' as a floating point value in the storage of 'TUnaligned',
  /// then decodes it back, returning the resulting bit pattern.
  /// Used to check that the constexpr bit_cast path is representation
  /// preserving.
  //***************************************************************************
  template <typename TUnaligned, typename TFloat, typename TBits>
  constexpr TBits round_trip_bits(TBits bits)
  {
    return etl::bit_cast<TBits>(TUnaligned(etl::bit_cast<TFloat>(bits)).value());
  }

  //***************************************************************************
  /// The storage byte at 'index' when 'bits' is encoded in 'TUnaligned'.
  //***************************************************************************
  template <typename TUnaligned, typename TFloat, typename TBits>
  constexpr unsigned char storage_byte_of(TBits bits, size_t index)
  {
    return TUnaligned(etl::bit_cast<TFloat>(bits)).data()[index];
  }
#endif

  SUITE(test_unaligned_type)
  {
    //*************************************************************************
    TEST(test_copy_construction_8)
    {
      etl::le_uint32_t le_v1(0xEF);
      etl::be_uint32_t be_v1(0xEF);
      etl::le_uint32_t le_v2(le_v1); // Copy construct le from le.
      etl::be_uint32_t be_v2(be_v1); // Copy construct be from be.
      etl::le_uint32_t le_v3(be_v1); // Copy construct le from be.
      etl::be_uint32_t be_v3(le_v1); // Copy construct be from le.

      CHECK_EQUAL(int(0xEF), int(le_v2));
      CHECK_EQUAL(int(0xEF), int(be_v2));
      CHECK_EQUAL(int(0xEF), int(le_v3));
      CHECK_EQUAL(int(0xEF), int(be_v3));
    }

    //*************************************************************************
    TEST(test_copy_construction_16)
    {
      etl::le_uint32_t le_v1(0xEEEF);
      etl::be_uint32_t be_v1(0xEEEF);
      etl::le_uint32_t le_v2(le_v1); // Copy construct le from le.
      etl::be_uint32_t be_v2(be_v1); // Copy construct be from be.
      etl::le_uint32_t le_v3(be_v1); // Copy construct le from be.
      etl::be_uint32_t be_v3(le_v1); // Copy construct be from le.

      CHECK_EQUAL(int(0xEEEF), int(le_v2));
      CHECK_EQUAL(int(0xEEEF), int(be_v2));
      CHECK_EQUAL(int(0xEEEF), int(le_v3));
      CHECK_EQUAL(int(0xEEEF), int(be_v3));
    }

    //*************************************************************************
    TEST(test_copy_construction_32)
    {
      etl::le_uint32_t le_v1(0xEE2345EF);
      etl::be_uint32_t be_v1(0xEE2345EF);
      etl::le_uint32_t le_v2(le_v1); // Copy construct le from le.
      etl::be_uint32_t be_v2(be_v1); // Copy construct be from be.
      etl::le_uint32_t le_v3(be_v1); // Copy construct le from be.
      etl::be_uint32_t be_v3(le_v1); // Copy construct be from le.

      CHECK_EQUAL(int(0xEE2345EF), int(le_v2));
      CHECK_EQUAL(int(0xEE2345EF), int(be_v2));
      CHECK_EQUAL(int(0xEE2345EF), int(le_v3));
      CHECK_EQUAL(int(0xEE2345EF), int(be_v3));
    }

    //*************************************************************************
    TEST(test_copy_construction_64)
    {
      etl::le_uint64_t le_v1(0xEE23456789ABCDEF);
      etl::be_uint64_t be_v1(0xEE23456789ABCDEF);
      etl::le_uint64_t le_v2(le_v1); // Copy construct le from le.
      etl::be_uint64_t be_v2(be_v1); // Copy construct be from be.
      etl::le_uint64_t le_v3(be_v1); // Copy construct le from be.
      etl::be_uint64_t be_v3(le_v1); // Copy construct be from le.

      CHECK_EQUAL(uint64_t(0xEE23456789ABCDEF), uint64_t(le_v2));
      CHECK_EQUAL(uint64_t(0xEE23456789ABCDEF), uint64_t(be_v2));
      CHECK_EQUAL(uint64_t(0xEE23456789ABCDEF), uint64_t(le_v3));
      CHECK_EQUAL(uint64_t(0xEE23456789ABCDEF), uint64_t(be_v3));
    }

    //*************************************************************************
    TEST(test_copy_construction_float)
    {
      etl::le_float_t le_v1(3.1415927f);
      etl::be_float_t be_v1(3.1415927f);
      etl::le_float_t le_v2(le_v1); // Copy construct le from le.
      etl::be_float_t be_v2(be_v1); // Copy construct be from be.
      etl::le_float_t le_v3(be_v1); // Copy construct le from be.
      etl::be_float_t be_v3(le_v1); // Copy construct be from le.

      CHECK_EQUAL(3.1415927f, le_v2);
      CHECK_EQUAL(3.1415927f, be_v2);
      CHECK_EQUAL(3.1415927f, le_v3);
      CHECK_EQUAL(3.1415927f, be_v3);
    }

    //*************************************************************************
    TEST(test_copy_construction_double)
    {
      etl::le_double_t le_v1(3.1415927);
      etl::be_double_t be_v1(3.1415927);
      etl::le_double_t le_v2(le_v1); // Copy construct le from le.
      etl::be_double_t be_v2(be_v1); // Copy construct be from be.
      etl::le_double_t le_v3(be_v1); // Copy construct le from be.
      etl::be_double_t be_v3(le_v1); // Copy construct be from le.

      CHECK_EQUAL(3.1415927, le_v2);
      CHECK_EQUAL(3.1415927, be_v2);
      CHECK_EQUAL(3.1415927, le_v3);
      CHECK_EQUAL(3.1415927, be_v3);
    }

    //*************************************************************************
    TEST(test_copy_construction_long_double)
    {
      etl::le_long_double_t le_v1(3.1415927L);
      etl::be_long_double_t be_v1(3.1415927L);
      etl::le_long_double_t le_v2(le_v1); // Copy construct le from le.
      etl::be_long_double_t be_v2(be_v1); // Copy construct be from be.
      etl::le_long_double_t le_v3(be_v1); // Copy construct le from be.
      etl::be_long_double_t be_v3(le_v1); // Copy construct be from le.

      CHECK_EQUAL(3.1415927L, le_v2);
      CHECK_EQUAL(3.1415927L, be_v2);
      CHECK_EQUAL(3.1415927L, le_v3);
      CHECK_EQUAL(3.1415927L, be_v3);
    }

    //*************************************************************************
    TEST(test_construction_from_buffer)
    {
      const std::array<char, 4> buffer = {0x12, 0x34, 0x56, 0x78};

      const uint32_t le_value = 0x78563412;
      const uint32_t be_value = 0x12345678;

      etl::le_uint32_t le_v1(buffer.data());
      etl::be_uint32_t be_v1(buffer.data());

      etl::le_uint32_t le_v2(buffer.data(), buffer.size());
      etl::be_uint32_t be_v2(buffer.data(), buffer.size());

      CHECK_EQUAL(le_value, le_v1);
      CHECK_EQUAL(be_value, be_v1);

      CHECK_EQUAL(le_value, le_v2);
      CHECK_EQUAL(be_value, be_v2);

      CHECK_THROW(etl::le_uint32_t le_v3(buffer.data(), buffer.size() - 1), etl::unaligned_type_buffer_size);
      CHECK_THROW(etl::be_uint32_t be_v3(buffer.data(), buffer.size() - 1), etl::unaligned_type_buffer_size);
    }

    //*************************************************************************
    /// The 'const unsigned char*' overloads are distinct from the
    /// 'const void*' ones above, so check their buffer size contract too.
    /// Note that the size check is an ETL_ASSERT, so this only holds when
    /// checks are enabled. See the note on the constructor.
    //*************************************************************************
    TEST(test_construction_from_byte_buffer_size_boundary)
    {
      const std::array<unsigned char, 5> buffer = {0x12, 0x34, 0x56, 0x78, 0x9A};

      // Exactly the required number of bytes.
      etl::le_uint32_t le_exact(buffer.data(), sizeof(uint32_t));
      etl::be_uint32_t be_exact(buffer.data(), sizeof(uint32_t));

      CHECK_EQUAL(uint32_t(0x78563412), le_exact.value());
      CHECK_EQUAL(uint32_t(0x12345678), be_exact.value());

      // More bytes than required. The surplus is ignored.
      etl::le_uint32_t le_larger(buffer.data(), buffer.size());
      etl::be_uint32_t be_larger(buffer.data(), buffer.size());

      CHECK_EQUAL(uint32_t(0x78563412), le_larger.value());
      CHECK_EQUAL(uint32_t(0x12345678), be_larger.value());

      // One byte short.
      CHECK_THROW(etl::le_uint32_t le_short(buffer.data(), sizeof(uint32_t) - 1U), etl::unaligned_type_buffer_size);
      CHECK_THROW(etl::be_uint32_t be_short(buffer.data(), sizeof(uint32_t) - 1U), etl::unaligned_type_buffer_size);

      // Empty buffer.
      CHECK_THROW(etl::le_uint32_t le_empty(buffer.data(), 0U), etl::unaligned_type_buffer_size);
      CHECK_THROW(etl::be_uint32_t be_empty(buffer.data(), 0U), etl::unaligned_type_buffer_size);

      // The widest type is checked too, as it has the largest storage.
      CHECK_THROW(etl::le_uint64_t le_wide(buffer.data(), buffer.size()), etl::unaligned_type_buffer_size);
      CHECK_THROW(etl::be_uint64_t be_wide(buffer.data(), buffer.size()), etl::unaligned_type_buffer_size);

      // Single byte types accept a single byte buffer.
      etl::le_uint8_t le_byte(buffer.data(), 1U);
      etl::be_uint8_t be_byte(buffer.data(), 1U);

      CHECK_EQUAL(uint8_t(0x12), le_byte.value());
      CHECK_EQUAL(uint8_t(0x12), be_byte.value());

      CHECK_THROW(etl::le_uint8_t le_byte_empty(buffer.data(), 0U), etl::unaligned_type_buffer_size);
      CHECK_THROW(etl::be_uint8_t be_byte_empty(buffer.data(), 0U), etl::unaligned_type_buffer_size);
    }

    //*************************************************************************
    // The following tests demonstrate the 'decode' direction: given a raw byte
    // buffer (e.g. as received from a file, network socket or memory-mapped
    // device), interpret it as an explicitly little/big endian unaligned_type
    // and read back the correctly decoded native value.
    //*************************************************************************
    TEST(test_decode_buffer_uint16)
    {
      // The same two bytes, interpreted with an explicit endianness.
      const std::array<uint8_t, 2> buffer = {0x12, 0x34};

      etl::le_uint16_t le_v(buffer.data(), buffer.size());
      etl::be_uint16_t be_v(buffer.data(), buffer.size());

      CHECK_EQUAL(uint16_t(0x3412), le_v.value());
      CHECK_EQUAL(uint16_t(0x1234), be_v.value());
    }

    //*************************************************************************
    // 'bool' has no 'make_unsigned' equivalent, so check that it is supported.
    TEST(test_bool)
    {
      etl::unaligned_type<bool, etl::endian::little> le_v(true);
      etl::unaligned_type<bool, etl::endian::big>    be_v(false);

      CHECK_EQUAL(true, le_v.value());
      CHECK_EQUAL(false, be_v.value());

      le_v = false;
      be_v = true;

      CHECK_EQUAL(false, le_v.value());
      CHECK_EQUAL(true, be_v.value());
    }

    //*************************************************************************
    TEST(test_decode_buffer_int32)
    {
      const std::array<uint8_t, 4> buffer = {0x12, 0x34, 0x56, 0x78};

      etl::le_int32_t le_v(buffer.data(), buffer.size());
      etl::be_int32_t be_v(buffer.data(), buffer.size());

      CHECK_EQUAL(int32_t(0x78563412), le_v.value());
      CHECK_EQUAL(int32_t(0x12345678), be_v.value());
    }

    //*************************************************************************
    TEST(test_decode_buffer_uint64)
    {
      const std::array<uint8_t, 8> buffer = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};

      etl::le_uint64_t le_v(buffer.data(), buffer.size());
      etl::be_uint64_t be_v(buffer.data(), buffer.size());

      CHECK_EQUAL(uint64_t(0xEFCDAB8967452301), le_v.value());
      CHECK_EQUAL(uint64_t(0x0123456789ABCDEF), be_v.value());
    }

    //*************************************************************************
    /// The decode tests above cover a subset of the widths only, and no
    /// negative or extreme values. These cover every signed and unsigned
    /// width at its boundary values, in both directions.
    //*************************************************************************
    template <typename TLittle, typename TBig, typename TValue>
    void check_boundary_value(TValue value)
    {
      ETL_STATIC_ASSERT(sizeof(TValue) == TLittle::Size, "Mismatched size");
      ETL_STATIC_ASSERT(sizeof(TValue) == TBig::Size, "Mismatched size");

      const TLittle le_v(value);
      const TBig    be_v(value);

      // The two encodings are byte reversals of each other.
      for (size_t i = 0U; i < sizeof(TValue); ++i)
      {
        CHECK_EQUAL(int(le_v.data()[i]), int(be_v.data()[sizeof(TValue) - 1U - i]));
      }

      // Decoding the encoded bytes returns the original value.
      CHECK_EQUAL(value, TLittle(le_v.data(), sizeof(TValue)).value());
      CHECK_EQUAL(value, TBig(be_v.data(), sizeof(TValue)).value());
    }

    //*************************************************************************
    TEST(test_integral_boundary_values)
    {
      check_boundary_value<etl::le_int8_t, etl::be_int8_t, int8_t>(0);
      check_boundary_value<etl::le_int8_t, etl::be_int8_t, int8_t>(-1);
      check_boundary_value<etl::le_int8_t, etl::be_int8_t, int8_t>(etl::integral_limits<int8_t>::min);
      check_boundary_value<etl::le_int8_t, etl::be_int8_t, int8_t>(etl::integral_limits<int8_t>::max);

      check_boundary_value<etl::le_uint8_t, etl::be_uint8_t, uint8_t>(0U);
      check_boundary_value<etl::le_uint8_t, etl::be_uint8_t, uint8_t>(etl::integral_limits<uint8_t>::max);

      check_boundary_value<etl::le_int16_t, etl::be_int16_t, int16_t>(0);
      check_boundary_value<etl::le_int16_t, etl::be_int16_t, int16_t>(-1);
      check_boundary_value<etl::le_int16_t, etl::be_int16_t, int16_t>(etl::integral_limits<int16_t>::min);
      check_boundary_value<etl::le_int16_t, etl::be_int16_t, int16_t>(etl::integral_limits<int16_t>::max);

      check_boundary_value<etl::le_uint16_t, etl::be_uint16_t, uint16_t>(0U);
      check_boundary_value<etl::le_uint16_t, etl::be_uint16_t, uint16_t>(etl::integral_limits<uint16_t>::max);

      check_boundary_value<etl::le_int32_t, etl::be_int32_t, int32_t>(0);
      check_boundary_value<etl::le_int32_t, etl::be_int32_t, int32_t>(-1);
      check_boundary_value<etl::le_int32_t, etl::be_int32_t, int32_t>(etl::integral_limits<int32_t>::min);
      check_boundary_value<etl::le_int32_t, etl::be_int32_t, int32_t>(etl::integral_limits<int32_t>::max);

      check_boundary_value<etl::le_uint32_t, etl::be_uint32_t, uint32_t>(0U);
      check_boundary_value<etl::le_uint32_t, etl::be_uint32_t, uint32_t>(etl::integral_limits<uint32_t>::max);

      check_boundary_value<etl::le_int64_t, etl::be_int64_t, int64_t>(0);
      check_boundary_value<etl::le_int64_t, etl::be_int64_t, int64_t>(-1);
      check_boundary_value<etl::le_int64_t, etl::be_int64_t, int64_t>(etl::integral_limits<int64_t>::min);
      check_boundary_value<etl::le_int64_t, etl::be_int64_t, int64_t>(etl::integral_limits<int64_t>::max);

      check_boundary_value<etl::le_uint64_t, etl::be_uint64_t, uint64_t>(0U);
      check_boundary_value<etl::le_uint64_t, etl::be_uint64_t, uint64_t>(etl::integral_limits<uint64_t>::max);
    }

    //*************************************************************************
    /// Explicit byte level checks for the widths that the decode tests above
    /// do not cover, using negative values so that the sign bit is exercised.
    //*************************************************************************
    TEST(test_decode_buffer_negative_values)
    {
      // -2 == 0xFE, 0xFFFE, 0xFFFFFFFE, 0xFFFFFFFFFFFFFFFE.
      const std::array<unsigned char, 8> buffer = {0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

      CHECK_EQUAL(int8_t(-2), etl::le_int8_t(buffer.data(), buffer.size()).value());
      CHECK_EQUAL(int8_t(-2), etl::be_int8_t(buffer.data(), buffer.size()).value());

      CHECK_EQUAL(int16_t(-2), etl::le_int16_t(buffer.data(), buffer.size()).value());
      CHECK_EQUAL(int32_t(-2), etl::le_int32_t(buffer.data(), buffer.size()).value());
      CHECK_EQUAL(int64_t(-2), etl::le_int64_t(buffer.data(), buffer.size()).value());

      // Big endian reads the same bytes in the opposite order.
      CHECK_EQUAL(int16_t(-257), etl::be_int16_t(buffer.data(), buffer.size()).value());
      CHECK_EQUAL(int32_t(-16777217), etl::be_int32_t(buffer.data(), buffer.size()).value());
      CHECK_EQUAL(int64_t(-72057594037927937LL), etl::be_int64_t(buffer.data(), buffer.size()).value());
    }

    //*************************************************************************
    TEST(test_decode_buffer_float)
    {
      // 1.5f in IEEE-754 single precision is 0x3FC00000.
      const std::array<uint8_t, 4> le_buffer = {0x00, 0x00, 0xC0, 0x3F};
      const std::array<uint8_t, 4> be_buffer = {0x3F, 0xC0, 0x00, 0x00};

      etl::le_float_t le_v(le_buffer.data(), le_buffer.size());
      etl::be_float_t be_v(be_buffer.data(), be_buffer.size());

      CHECK_EQUAL(1.5f, le_v.value());
      CHECK_EQUAL(1.5f, be_v.value());
    }

    //*************************************************************************
    TEST(test_decode_buffer_double)
    {
      // 1.5 in IEEE-754 double precision is 0x3FF8000000000000.
      const std::array<uint8_t, 8> le_buffer = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x3F};
      const std::array<uint8_t, 8> be_buffer = {0x3F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

      etl::le_double_t le_v(le_buffer.data(), le_buffer.size());
      etl::be_double_t be_v(be_buffer.data(), be_buffer.size());

      CHECK_EQUAL(1.5, le_v.value());
      CHECK_EQUAL(1.5, be_v.value());
    }

    //*************************************************************************
    /// Representation level checks for 'float'.
    /// Ordinary values such as 1.5f have a zero mantissa tail, a clear sign
    /// bit and a mid range exponent, so they would still pass under several
    /// plausible masking or byte ordering faults. These patterns exercise the
    /// sign of zero, the infinities, a NaN payload, the subnormal range and
    /// the normal range limits.
    //*************************************************************************
    TEST(test_float_representations)
    {
      static const uint32_t patterns[] = {
        0x00000000U, // +0.0
        0x80000000U, // -0.0
        0x7F800000U, // +infinity
        0xFF800000U, // -infinity
        0x7FC0DEADU, // Quiet NaN with a payload.
        0xFFC0DEADU, // Negative quiet NaN with a payload.
        0x00000001U, // Smallest positive subnormal.
        0x007FFFFFU, // Largest subnormal.
        0x00800000U, // Smallest positive normal.
        0x7F7FFFFFU, // Largest normal.
        0xBFC00000U  // -1.5
      };

      for (size_t i = 0U; i < (sizeof(patterns) / sizeof(patterns[0])); ++i)
      {
        const uint32_t bits = patterns[i];

        etl::le_float_t le_v(float_from_bits(bits));
        etl::be_float_t be_v(float_from_bits(bits));

        // The storage must hold the exact IEEE-754 bytes, in the requested order.
        for (size_t b = 0U; b < sizeof(float); ++b)
        {
          CHECK_EQUAL(int((bits >> (8U * b)) & 0xFFU), int(le_v.data()[b]));
          CHECK_EQUAL(int((bits >> (8U * (sizeof(float) - 1U - b))) & 0xFFU), int(be_v.data()[b]));
        }

        // The value must survive the round trip bit for bit, including the
        // sign of zero and the NaN payload.
        CHECK_EQUAL(bits, bits_of(le_v.value()));
        CHECK_EQUAL(bits, bits_of(be_v.value()));

        // Decoding the raw bytes must give the same result as encoding them.
        etl::le_float_t le_decoded(le_v.data(), le_v.size());
        etl::be_float_t be_decoded(be_v.data(), be_v.size());

        CHECK_EQUAL(bits, bits_of(le_decoded.value()));
        CHECK_EQUAL(bits, bits_of(be_decoded.value()));
      }
    }

    //*************************************************************************
    /// Representation level checks for 'double'. See the note on the 'float'
    /// test above.
    //*************************************************************************
    TEST(test_double_representations)
    {
      static const uint64_t patterns[] = {
        0x0000000000000000ULL, // +0.0
        0x8000000000000000ULL, // -0.0
        0x7FF0000000000000ULL, // +infinity
        0xFFF0000000000000ULL, // -infinity
        0x7FF80000DEADBEEFULL, // Quiet NaN with a payload.
        0xFFF80000DEADBEEFULL, // Negative quiet NaN with a payload.
        0x0000000000000001ULL, // Smallest positive subnormal.
        0x000FFFFFFFFFFFFFULL, // Largest subnormal.
        0x0010000000000000ULL, // Smallest positive normal.
        0x7FEFFFFFFFFFFFFFULL, // Largest normal.
        0xBFF8000000000000ULL  // -1.5
      };

      for (size_t i = 0U; i < (sizeof(patterns) / sizeof(patterns[0])); ++i)
      {
        const uint64_t bits = patterns[i];

        etl::le_double_t le_v(double_from_bits(bits));
        etl::be_double_t be_v(double_from_bits(bits));

        for (size_t b = 0U; b < sizeof(double); ++b)
        {
          CHECK_EQUAL(int((bits >> (8U * b)) & 0xFFU), int(le_v.data()[b]));
          CHECK_EQUAL(int((bits >> (8U * (sizeof(double) - 1U - b))) & 0xFFU), int(be_v.data()[b]));
        }

        CHECK_EQUAL(bits, bits_of(le_v.value()));
        CHECK_EQUAL(bits, bits_of(be_v.value()));

        etl::le_double_t le_decoded(le_v.data(), le_v.size());
        etl::be_double_t be_decoded(be_v.data(), be_v.size());

        CHECK_EQUAL(bits, bits_of(le_decoded.value()));
        CHECK_EQUAL(bits, bits_of(be_decoded.value()));
      }
    }

    //*************************************************************************
    /// 'long double' is typically 80, 96 or 128 bits wide, so it has no same
    /// sized unsigned integer proxy and falls back to the non bit_cast,
    /// non constexpr path. That fallback still has to work.
    //*************************************************************************
    TEST(test_long_double_round_trip)
    {
      const long double value = 1.5L;

      etl::le_long_double_t le_v(value);
      etl::be_long_double_t be_v(value);

      CHECK_EQUAL(sizeof(long double), le_v.size());
      CHECK_EQUAL(sizeof(long double), be_v.size());

      // The little and big endian storage must be byte reversals of each other.
      for (size_t i = 0U; i < sizeof(long double); ++i)
      {
        CHECK_EQUAL(int(le_v.data()[i]), int(be_v.data()[sizeof(long double) - 1U - i]));
      }

      CHECK_EQUAL(value, le_v.value());
      CHECK_EQUAL(value, be_v.value());

      // Decoding the raw bytes must give the same result as encoding them.
      etl::le_long_double_t le_decoded(le_v.data(), le_v.size());
      etl::be_long_double_t be_decoded(be_v.data(), be_v.size());

      CHECK_EQUAL(value, le_decoded.value());
      CHECK_EQUAL(value, be_decoded.value());
    }

#if ETL_HAS_CONSTEXPR_ENDIANNESS
    //*************************************************************************
    TEST(test_decode_buffer_host_order)
    {
      // Decoding a buffer known to be in host order must yield the same
      // value regardless of what that host order actually is.
      const uint32_t value = 0x12345678U;

      const etl::host_uint32_t encoded(value);
      etl::host_uint32_t       decoded(encoded.data(), encoded.size());

      CHECK_EQUAL(value, decoded.value());
    }
#endif

#if ETL_USING_CPP14
    //*************************************************************************
    // Demonstrates that decoding a raw byte buffer with an explicit endianness
    // is now usable at compile time (constexpr), via the 'const unsigned char*'
    // constructor overloads - not just runtime CHECK_EQUAL.
    //*************************************************************************
    TEST(test_constexpr_decode_buffer_integral)
    {
      static ETL_CONSTANT unsigned char le_buffer[4] = {0x78, 0x56, 0x34, 0x12};
      static ETL_CONSTANT unsigned char be_buffer[4] = {0x12, 0x34, 0x56, 0x78};

      constexpr etl::le_uint32_t le_v(le_buffer, sizeof(le_buffer));
      constexpr etl::be_uint32_t be_v(be_buffer, sizeof(be_buffer));

      static_assert(le_v.value() == 0x12345678U, "le_uint32_t constexpr decode from buffer + size");
      static_assert(be_v.value() == 0x12345678U, "be_uint32_t constexpr decode from buffer + size");

      // Buffer-only overload (no explicit size).
      constexpr etl::le_uint32_t le_v2(le_buffer);
      constexpr etl::be_uint32_t be_v2(be_buffer);

      static_assert(le_v2.value() == 0x12345678U, "le_uint32_t constexpr decode from buffer");
      static_assert(be_v2.value() == 0x12345678U, "be_uint32_t constexpr decode from buffer");

      // Prove the checks above are genuine compile-time facts, not just
      // syntactically valid constexpr, by also exercising them at runtime.
      CHECK_EQUAL(0x12345678U, le_v.value());
      CHECK_EQUAL(0x12345678U, be_v.value());
    }

  #if ETL_USING_BUILTIN_BIT_CAST
    //*************************************************************************
    TEST(test_constexpr_decode_buffer_float)
    {
      // 1.5f in IEEE-754 single precision is 0x3FC00000.
      static ETL_CONSTANT unsigned char le_buffer[4] = {0x00, 0x00, 0xC0, 0x3F};
      static ETL_CONSTANT unsigned char be_buffer[4] = {0x3F, 0xC0, 0x00, 0x00};

      constexpr etl::le_float_t le_v(le_buffer, sizeof(le_buffer));
      constexpr etl::be_float_t be_v(be_buffer, sizeof(be_buffer));

      // Bit patterns are compared, rather than the float values, to avoid
      // '-Wfloat-equal'.
      static_assert(etl::bit_cast<uint32_t>(le_v.value()) == 0x3FC00000U, "le_float_t constexpr decode from buffer");
      static_assert(etl::bit_cast<uint32_t>(be_v.value()) == 0x3FC00000U, "be_float_t constexpr decode from buffer");

      CHECK_EQUAL(1.5f, le_v.value());
      CHECK_EQUAL(1.5f, be_v.value());
    }

    //*************************************************************************
    /// The constexpr path bit_casts through a same sized unsigned integer.
    /// Check, at compile time, that this is exact for the awkward
    /// representations too, not just for ordinary values such as 1.5f.
    //*************************************************************************
    TEST(test_constexpr_float_representations)
    {
      // Encode, then decode, and require the bit pattern to be unchanged.
      static_assert(round_trip_bits<etl::le_float_t, float>(0x00000000U) == 0x00000000U, "le +0.0");
      static_assert(round_trip_bits<etl::le_float_t, float>(0x80000000U) == 0x80000000U, "le -0.0");
      static_assert(round_trip_bits<etl::le_float_t, float>(0x7F800000U) == 0x7F800000U, "le +infinity");
      static_assert(round_trip_bits<etl::le_float_t, float>(0xFF800000U) == 0xFF800000U, "le -infinity");
      static_assert(round_trip_bits<etl::le_float_t, float>(0x7FC0DEADU) == 0x7FC0DEADU, "le NaN payload");
      static_assert(round_trip_bits<etl::le_float_t, float>(0x00000001U) == 0x00000001U, "le smallest subnormal");
      static_assert(round_trip_bits<etl::le_float_t, float>(0x007FFFFFU) == 0x007FFFFFU, "le largest subnormal");
      static_assert(round_trip_bits<etl::le_float_t, float>(0x00800000U) == 0x00800000U, "le smallest normal");
      static_assert(round_trip_bits<etl::le_float_t, float>(0x7F7FFFFFU) == 0x7F7FFFFFU, "le largest normal");

      static_assert(round_trip_bits<etl::be_float_t, float>(0x80000000U) == 0x80000000U, "be -0.0");
      static_assert(round_trip_bits<etl::be_float_t, float>(0xFF800000U) == 0xFF800000U, "be -infinity");
      static_assert(round_trip_bits<etl::be_float_t, float>(0x7FC0DEADU) == 0x7FC0DEADU, "be NaN payload");
      static_assert(round_trip_bits<etl::be_float_t, float>(0x00000001U) == 0x00000001U, "be smallest subnormal");
      static_assert(round_trip_bits<etl::be_float_t, float>(0x7F7FFFFFU) == 0x7F7FFFFFU, "be largest normal");

      // The stored bytes must be the IEEE-754 bytes, in the requested order.
      static_assert(storage_byte_of<etl::le_float_t, float>(0x7FC0DEADU, 0U) == 0xAD, "le NaN byte 0");
      static_assert(storage_byte_of<etl::le_float_t, float>(0x7FC0DEADU, 1U) == 0xDE, "le NaN byte 1");
      static_assert(storage_byte_of<etl::le_float_t, float>(0x7FC0DEADU, 2U) == 0xC0, "le NaN byte 2");
      static_assert(storage_byte_of<etl::le_float_t, float>(0x7FC0DEADU, 3U) == 0x7F, "le NaN byte 3");

      static_assert(storage_byte_of<etl::be_float_t, float>(0x7FC0DEADU, 0U) == 0x7F, "be NaN byte 0");
      static_assert(storage_byte_of<etl::be_float_t, float>(0x7FC0DEADU, 1U) == 0xC0, "be NaN byte 1");
      static_assert(storage_byte_of<etl::be_float_t, float>(0x7FC0DEADU, 2U) == 0xDE, "be NaN byte 2");
      static_assert(storage_byte_of<etl::be_float_t, float>(0x7FC0DEADU, 3U) == 0xAD, "be NaN byte 3");

      // Prove the checks above are genuine compile time facts, not just
      // syntactically valid constexpr, by also exercising them at runtime.
      CHECK_EQUAL(0x80000000U, (round_trip_bits<etl::le_float_t, float>(0x80000000U)));
      CHECK_EQUAL(0x7FC0DEADU, (round_trip_bits<etl::be_float_t, float>(0x7FC0DEADU)));
    }

    //*************************************************************************
    /// As 'test_constexpr_float_representations', for the 8 byte proxy.
    //*************************************************************************
    TEST(test_constexpr_double_representations)
    {
      static_assert(round_trip_bits<etl::le_double_t, double>(0x0000000000000000ULL) == 0x0000000000000000ULL, "le +0.0");
      static_assert(round_trip_bits<etl::le_double_t, double>(0x8000000000000000ULL) == 0x8000000000000000ULL, "le -0.0");
      static_assert(round_trip_bits<etl::le_double_t, double>(0x7FF0000000000000ULL) == 0x7FF0000000000000ULL, "le +infinity");
      static_assert(round_trip_bits<etl::le_double_t, double>(0xFFF0000000000000ULL) == 0xFFF0000000000000ULL, "le -infinity");
      static_assert(round_trip_bits<etl::le_double_t, double>(0x7FF80000DEADBEEFULL) == 0x7FF80000DEADBEEFULL, "le NaN payload");
      static_assert(round_trip_bits<etl::le_double_t, double>(0x0000000000000001ULL) == 0x0000000000000001ULL, "le smallest subnormal");
      static_assert(round_trip_bits<etl::le_double_t, double>(0x000FFFFFFFFFFFFFULL) == 0x000FFFFFFFFFFFFFULL, "le largest subnormal");
      static_assert(round_trip_bits<etl::le_double_t, double>(0x0010000000000000ULL) == 0x0010000000000000ULL, "le smallest normal");
      static_assert(round_trip_bits<etl::le_double_t, double>(0x7FEFFFFFFFFFFFFFULL) == 0x7FEFFFFFFFFFFFFFULL, "le largest normal");

      static_assert(round_trip_bits<etl::be_double_t, double>(0x8000000000000000ULL) == 0x8000000000000000ULL, "be -0.0");
      static_assert(round_trip_bits<etl::be_double_t, double>(0xFFF0000000000000ULL) == 0xFFF0000000000000ULL, "be -infinity");
      static_assert(round_trip_bits<etl::be_double_t, double>(0x7FF80000DEADBEEFULL) == 0x7FF80000DEADBEEFULL, "be NaN payload");
      static_assert(round_trip_bits<etl::be_double_t, double>(0x0000000000000001ULL) == 0x0000000000000001ULL, "be smallest subnormal");
      static_assert(round_trip_bits<etl::be_double_t, double>(0x7FEFFFFFFFFFFFFFULL) == 0x7FEFFFFFFFFFFFFFULL, "be largest normal");

      static_assert(storage_byte_of<etl::le_double_t, double>(0x7FF80000DEADBEEFULL, 0U) == 0xEF, "le NaN byte 0");
      static_assert(storage_byte_of<etl::le_double_t, double>(0x7FF80000DEADBEEFULL, 7U) == 0x7F, "le NaN byte 7");

      static_assert(storage_byte_of<etl::be_double_t, double>(0x7FF80000DEADBEEFULL, 0U) == 0x7F, "be NaN byte 0");
      static_assert(storage_byte_of<etl::be_double_t, double>(0x7FF80000DEADBEEFULL, 7U) == 0xEF, "be NaN byte 7");

      CHECK_EQUAL(0x8000000000000000ULL, (round_trip_bits<etl::le_double_t, double>(0x8000000000000000ULL)));
      CHECK_EQUAL(0x7FF80000DEADBEEFULL, (round_trip_bits<etl::be_double_t, double>(0x7FF80000DEADBEEFULL)));
    }
  #endif
#endif

    //*************************************************************************
    TEST(test_endianness)
    {
      CHECK_EQUAL(etl::endian::big, etl::be_int16_t::Endian);
      CHECK_EQUAL(etl::endian::little, etl::le_int16_t::Endian);
    }

    //*************************************************************************
    TEST(test_le_sizes)
    {
      CHECK_EQUAL(sizeof(char), etl::le_char_t::Size);
      CHECK_EQUAL(sizeof(signed char), etl::le_schar_t::Size);
      CHECK_EQUAL(sizeof(unsigned char), etl::le_uchar_t::Size);
      CHECK_EQUAL(sizeof(short), etl::le_short_t::Size);
      CHECK_EQUAL(sizeof(unsigned short), etl::le_ushort_t::Size);
      CHECK_EQUAL(sizeof(int), etl::le_int_t::Size);
      CHECK_EQUAL(sizeof(unsigned int), etl::le_uint_t::Size);
      CHECK_EQUAL(sizeof(long), etl::le_long_t::Size);
      CHECK_EQUAL(sizeof(unsigned long), etl::le_ulong_t::Size);
      CHECK_EQUAL(sizeof(long long), etl::le_long_long_t::Size);
      CHECK_EQUAL(sizeof(unsigned long long), etl::le_ulong_long_t::Size);
      CHECK_EQUAL(sizeof(float), etl::le_float_t::Size);
      CHECK_EQUAL(sizeof(double), etl::le_double_t::Size);
      CHECK_EQUAL(sizeof(long double), etl::le_long_double_t::Size);

      CHECK_EQUAL(sizeof(int8_t), etl::le_int8_t::Size);
      CHECK_EQUAL(sizeof(uint8_t), etl::le_uint8_t::Size);
      CHECK_EQUAL(sizeof(int16_t), etl::le_int16_t::Size);
      CHECK_EQUAL(sizeof(uint16_t), etl::le_uint16_t::Size);
      CHECK_EQUAL(sizeof(int32_t), etl::le_int32_t::Size);
      CHECK_EQUAL(sizeof(uint32_t), etl::le_uint32_t::Size);
      CHECK_EQUAL(sizeof(int64_t), etl::le_int64_t::Size);
      CHECK_EQUAL(sizeof(uint64_t), etl::le_uint64_t::Size);

      // check if net size equals gross size on platform
      CHECK_EQUAL(sizeof(int8_t), sizeof(etl::le_int8_t));
      CHECK_EQUAL(sizeof(uint8_t), sizeof(etl::le_uint8_t));
      CHECK_EQUAL(sizeof(int16_t), sizeof(etl::le_int16_t));
      CHECK_EQUAL(sizeof(uint16_t), sizeof(etl::le_uint16_t));
      CHECK_EQUAL(sizeof(int32_t), sizeof(etl::le_int32_t));
      CHECK_EQUAL(sizeof(uint32_t), sizeof(etl::le_uint32_t));
      CHECK_EQUAL(sizeof(int64_t), sizeof(etl::le_int64_t));
      CHECK_EQUAL(sizeof(uint64_t), sizeof(etl::le_uint64_t));
    }

    //*************************************************************************
    TEST(test_be_sizes)
    {
      CHECK_EQUAL(sizeof(char), etl::be_char_t::Size);
      CHECK_EQUAL(sizeof(signed char), etl::be_schar_t::Size);
      CHECK_EQUAL(sizeof(unsigned char), etl::be_uchar_t::Size);
      CHECK_EQUAL(sizeof(short), etl::be_short_t::Size);
      CHECK_EQUAL(sizeof(unsigned short), etl::be_ushort_t::Size);
      CHECK_EQUAL(sizeof(int), etl::be_int_t::Size);
      CHECK_EQUAL(sizeof(unsigned int), etl::be_uint_t::Size);
      CHECK_EQUAL(sizeof(long), etl::be_long_t::Size);
      CHECK_EQUAL(sizeof(unsigned long), etl::be_ulong_t::Size);
      CHECK_EQUAL(sizeof(long long), etl::be_long_long_t::Size);
      CHECK_EQUAL(sizeof(unsigned long long), etl::be_ulong_long_t::Size);
      CHECK_EQUAL(sizeof(float), etl::be_float_t::Size);
      CHECK_EQUAL(sizeof(double), etl::be_double_t::Size);
      CHECK_EQUAL(sizeof(long double), etl::be_long_double_t::Size);

      CHECK_EQUAL(sizeof(int8_t), etl::be_int8_t::Size);
      CHECK_EQUAL(sizeof(uint8_t), etl::be_uint8_t::Size);
      CHECK_EQUAL(sizeof(int16_t), etl::be_int16_t::Size);
      CHECK_EQUAL(sizeof(uint16_t), etl::be_uint16_t::Size);
      CHECK_EQUAL(sizeof(int32_t), etl::be_int32_t::Size);
      CHECK_EQUAL(sizeof(uint32_t), etl::be_uint32_t::Size);
      CHECK_EQUAL(sizeof(int64_t), etl::be_int64_t::Size);
      CHECK_EQUAL(sizeof(uint64_t), etl::be_uint64_t::Size);

      // check if net size equals gross size on platform
      CHECK_EQUAL(sizeof(int8_t), sizeof(etl::be_int8_t));
      CHECK_EQUAL(sizeof(uint8_t), sizeof(etl::be_uint8_t));
      CHECK_EQUAL(sizeof(int16_t), sizeof(etl::be_int16_t));
      CHECK_EQUAL(sizeof(uint16_t), sizeof(etl::be_uint16_t));
      CHECK_EQUAL(sizeof(int32_t), sizeof(etl::be_int32_t));
      CHECK_EQUAL(sizeof(uint32_t), sizeof(etl::be_uint32_t));
      CHECK_EQUAL(sizeof(int64_t), sizeof(etl::be_int64_t));
      CHECK_EQUAL(sizeof(uint64_t), sizeof(etl::be_uint64_t));
    }

    //*************************************************************************
    TEST(test_net_sizes)
    {
      CHECK_EQUAL(sizeof(char), etl::net_char_t::Size);
      CHECK_EQUAL(sizeof(signed char), etl::net_schar_t::Size);
      CHECK_EQUAL(sizeof(unsigned char), etl::net_uchar_t::Size);
      CHECK_EQUAL(sizeof(short), etl::net_short_t::Size);
      CHECK_EQUAL(sizeof(unsigned short), etl::net_ushort_t::Size);
      CHECK_EQUAL(sizeof(int), etl::net_int_t::Size);
      CHECK_EQUAL(sizeof(unsigned int), etl::net_uint_t::Size);
      CHECK_EQUAL(sizeof(long), etl::net_long_t::Size);
      CHECK_EQUAL(sizeof(unsigned long), etl::net_ulong_t::Size);
      CHECK_EQUAL(sizeof(long long), etl::net_long_long_t::Size);
      CHECK_EQUAL(sizeof(unsigned long long), etl::net_ulong_long_t::Size);
      CHECK_EQUAL(sizeof(float), etl::net_float_t::Size);
      CHECK_EQUAL(sizeof(double), etl::net_double_t::Size);
      CHECK_EQUAL(sizeof(long double), etl::net_long_double_t::Size);

      CHECK_EQUAL(sizeof(int8_t), etl::net_int8_t::Size);
      CHECK_EQUAL(sizeof(uint8_t), etl::net_uint8_t::Size);
      CHECK_EQUAL(sizeof(int16_t), etl::net_int16_t::Size);
      CHECK_EQUAL(sizeof(uint16_t), etl::net_uint16_t::Size);
      CHECK_EQUAL(sizeof(int32_t), etl::net_int32_t::Size);
      CHECK_EQUAL(sizeof(uint32_t), etl::net_uint32_t::Size);
      CHECK_EQUAL(sizeof(int64_t), etl::net_int64_t::Size);
      CHECK_EQUAL(sizeof(uint64_t), etl::net_uint64_t::Size);
    }

#if ETL_HAS_CONSTEXPR_ENDIANNESS
    //*************************************************************************
    TEST(test_host_sizes)
    {
      CHECK_EQUAL(sizeof(char), etl::host_char_t::Size);
      CHECK_EQUAL(sizeof(signed char), etl::host_schar_t::Size);
      CHECK_EQUAL(sizeof(unsigned char), etl::host_uchar_t::Size);
      CHECK_EQUAL(sizeof(short), etl::host_short_t::Size);
      CHECK_EQUAL(sizeof(unsigned short), etl::host_ushort_t::Size);
      CHECK_EQUAL(sizeof(int), etl::host_int_t::Size);
      CHECK_EQUAL(sizeof(unsigned int), etl::host_uint_t::Size);
      CHECK_EQUAL(sizeof(long), etl::host_long_t::Size);
      CHECK_EQUAL(sizeof(unsigned long), etl::host_ulong_t::Size);
      CHECK_EQUAL(sizeof(long long), etl::host_long_long_t::Size);
      CHECK_EQUAL(sizeof(unsigned long long), etl::host_ulong_long_t::Size);
      CHECK_EQUAL(sizeof(float), etl::host_float_t::Size);
      CHECK_EQUAL(sizeof(double), etl::host_double_t::Size);
      CHECK_EQUAL(sizeof(long double), etl::host_long_double_t::Size);

      CHECK_EQUAL(sizeof(int8_t), etl::host_int8_t::Size);
      CHECK_EQUAL(sizeof(uint8_t), etl::host_uint8_t::Size);
      CHECK_EQUAL(sizeof(int16_t), etl::host_int16_t::Size);
      CHECK_EQUAL(sizeof(uint16_t), etl::host_uint16_t::Size);
      CHECK_EQUAL(sizeof(int32_t), etl::host_int32_t::Size);
      CHECK_EQUAL(sizeof(uint32_t), etl::host_uint32_t::Size);
      CHECK_EQUAL(sizeof(int64_t), etl::host_int64_t::Size);
      CHECK_EQUAL(sizeof(uint64_t), etl::host_uint64_t::Size);
    }
#endif

    //*************************************************************************
    TEST(test_le_equality)
    {
      // char
      CHECK(etl::le_char_t(0x01) == etl::le_char_t(0x01));
      CHECK(char(0x01) == etl::le_char_t(0x01));
      CHECK(etl::le_char_t(0x01) == char(0x01));

      CHECK(etl::le_schar_t(0x01) == etl::le_schar_t(0x01));
      CHECK((signed char)(0x01) == etl::le_schar_t(0x01));
      CHECK(etl::le_schar_t(0x01) == (signed char)(0x01));

      CHECK(etl::le_uchar_t(0x01U) == etl::le_uchar_t(0x01U));
      CHECK((unsigned char)(0x01U) == etl::le_uchar_t(0x01U));
      CHECK(etl::le_uchar_t(0x01U) == (unsigned char)(0x01U));

      // short
      CHECK(etl::le_short_t(0x0123) == etl::le_short_t(0x0123));
      CHECK(short(0x0123) == etl::le_short_t(0x0123));
      CHECK(etl::le_short_t(0x0123) == short(0x0123));

      CHECK(etl::le_ushort_t(0x0123) == etl::le_ushort_t(0x0123));
      CHECK((unsigned short)(0x0123) == etl::le_ushort_t(0x0123));
      CHECK(etl::le_ushort_t(0x0123) == (unsigned short)(0x0123));

      // int
      CHECK(etl::le_int_t(0x01234567) == etl::le_int_t(0x01234567));
      CHECK(int(0x01234567) == etl::le_int_t(0x01234567));
      CHECK(etl::le_int_t(0x01234567) == int(0x01234567));

      CHECK(etl::le_uint_t(0x01234567U) == etl::le_uint_t(0x01234567U));
      CHECK((unsigned int)(0x01234567U) == etl::le_uint_t(0x01234567U));
      CHECK(etl::le_uint_t(0x01234567U) == (unsigned int)(0x01234567U));

      if (sizeof(long) == 4U)
      {
        // long
        CHECK(etl::le_long_t(0x01234567) == etl::le_long_t(0x01234567));
        CHECK(long(0x01234567) == etl::le_long_t(0x01234567));
        CHECK(etl::le_long_t(0x01234567) == long(0x01234567));

        CHECK(etl::le_ulong_t(0x01234567U) == etl::le_ulong_t(0x01234567U));
        CHECK((unsigned long)(0x01234567U) == etl::le_ulong_t(0x01234567U));
        CHECK(etl::le_ulong_t(0x01234567U) == (unsigned long)(0x01234567U));
      }

      if (sizeof(long long) == 8U)
      {
        // long long
        CHECK(etl::le_long_long_t(0x0123456789ABCDEF) == etl::le_long_long_t(0x0123456789ABCDEF));
        CHECK((long long)(0x0123456789ABCDEF) == etl::le_long_long_t(0x0123456789ABCDEF));
        CHECK(etl::le_long_long_t(0x0123456789ABCDEF) == (long long)(0x0123456789ABCDEF));

        CHECK(etl::le_ulong_long_t(0x0123456789ABCDEFU) == etl::le_ulong_long_t(0x0123456789ABCDEFU));
        CHECK((unsigned long long)(0x0123456789ABCDEFU) == etl::le_ulong_long_t(0x0123456789ABCDEFU));
        CHECK(etl::le_ulong_long_t(0x0123456789ABCDEFU) == (unsigned long long)(0x0123456789ABCDEFU));
      }

      // float
      CHECK_CLOSE(etl::le_float_t(3.1415927f), etl::le_float_t(3.1415927f), 0.001);
      CHECK_CLOSE(3.1415927f, etl::le_float_t(3.1415927f), 0.001);
      CHECK_CLOSE(etl::le_float_t(3.1415927f), 3.1415927f, 0.001);

      // double
      CHECK_CLOSE(etl::le_double_t(3.1415927), etl::le_double_t(3.1415927), 0.001);
      CHECK_CLOSE(3.1415927, etl::le_double_t(3.1415927), 0.001);
      CHECK_CLOSE(etl::le_double_t(3.1415927), 3.1415927, 0.001);

      // long double
      CHECK_CLOSE(etl::le_long_double_t(3.1415927L), etl::le_long_double_t(3.1415927L), 0.001);
      CHECK_CLOSE(3.1415927L, etl::le_long_double_t(3.1415927L), 0.001);
      CHECK_CLOSE(etl::le_long_double_t(3.1415927L), 3.1415927L, 0.001);
    }

    //*************************************************************************
    TEST(test_le_inequality)
    {
      // char
      CHECK(etl::le_char_t(0x01) != etl::le_char_t(0x02));
      CHECK(char(0x01) != etl::le_char_t(0x02));
      CHECK(etl::le_char_t(0x01) != char(0x02));

      CHECK(etl::le_schar_t(0x01) != etl::le_schar_t(0x02));
      CHECK((signed char)(0x01) != etl::le_schar_t(0x02));
      CHECK(etl::le_schar_t(0x01) != (signed char)(0x02));

      CHECK(etl::le_uchar_t(0x01U) != etl::le_uchar_t(0x02U));
      CHECK((unsigned char)(0x01U) != etl::le_uchar_t(0x02U));
      CHECK(etl::le_uchar_t(0x01U) != (unsigned char)(0x02U));

      // short
      CHECK(etl::le_short_t(0x0123) != etl::le_short_t(0x0223));
      CHECK(short(0x0123) != etl::le_short_t(0x0223));
      CHECK(etl::le_short_t(0x0123) != short(0x0223));

      CHECK(etl::le_ushort_t(0x0123) != etl::le_ushort_t(0x0223));
      CHECK((unsigned short)(0x0123) != etl::le_ushort_t(0x0223));
      CHECK(etl::le_ushort_t(0x0123) != (unsigned short)(0x0223));

      // int
      CHECK(etl::le_int_t(0x01234567) != etl::le_int_t(0x02234567));
      CHECK(int(0x01234567) != etl::le_int_t(0x02234567));
      CHECK(etl::le_int_t(0x01234567) != int(0x02234567));

      CHECK(etl::le_uint_t(0x01234567U) != etl::le_uint_t(0x02234567U));
      CHECK((unsigned int)(0x01234567U) != etl::le_uint_t(0x02234567U));
      CHECK(etl::le_uint_t(0x01234567U) != (unsigned int)(0x02234567U));

      if (sizeof(long) == 4U)
      {
        // long
        CHECK(etl::le_long_t(0x01234567) != etl::le_long_t(0x02234567));
        CHECK(long(0x01234567) != etl::le_long_t(0x02234567));
        CHECK(etl::le_long_t(0x01234567) != long(0x02234567));

        CHECK(etl::le_ulong_t(0x01234567U) != etl::le_ulong_t(0x02234567U));
        CHECK((unsigned long)(0x01234567U) != etl::le_ulong_t(0x02234567U));
        CHECK(etl::le_ulong_t(0x01234567U) != (unsigned long)(0x02234567U));
      }

      if (sizeof(long long) == 8U)
      {
        // long long
        CHECK(etl::le_long_long_t(0x0123456789ABCDEF) != etl::le_long_long_t(0x0223456789ABCDEF));
        CHECK((long long)(0x0123456789ABCDEF) != etl::le_long_long_t(0x0223456789ABCDEF));
        CHECK(etl::le_long_long_t(0x0123456789ABCDEF) != (long long)(0x0223456789ABCDEF));

        CHECK(etl::le_ulong_long_t(0x0123456789ABCDEFU) != etl::le_ulong_long_t(0x0223456789ABCDEFU));
        CHECK((unsigned long long)(0x0123456789ABCDEFU) != etl::le_ulong_long_t(0x0223456789ABCDEFU));
        CHECK(etl::le_ulong_long_t(0x0123456789ABCDEFU) != (unsigned long long)(0x0223456789ABCDEFU));
      }

      // float
      CHECK_FLOAT_DIFFERENT(etl::le_float_t(3.1415927f), etl::le_float_t(2.7182818f));
      CHECK_FLOAT_DIFFERENT(3.1415927f, etl::le_float_t(2.7182818f));
      CHECK_FLOAT_DIFFERENT(etl::le_float_t(3.1415927f), 2.7182818f);

      // double
      CHECK_FLOAT_DIFFERENT(etl::le_double_t(3.1415927), etl::le_double_t(2.7182818));
      CHECK_FLOAT_DIFFERENT(3.1415927, etl::le_double_t(2.7182818));
      CHECK_FLOAT_DIFFERENT(etl::le_double_t(3.1415927), 2.7182818);

      // long double
      CHECK_FLOAT_DIFFERENT(etl::le_long_double_t(3.1415927L), etl::le_long_double_t(2.7182818L));
      CHECK_FLOAT_DIFFERENT(3.1415927L, etl::le_long_double_t(2.7182818L));
      CHECK_FLOAT_DIFFERENT(etl::le_long_double_t(3.1415927L), 2.7182818L);
    }

    //*************************************************************************
    TEST(test_be_equality)
    {
      // char
      CHECK(etl::be_char_t(0x01) == etl::be_char_t(0x01));
      CHECK(char(0x01) == etl::be_char_t(0x01));
      CHECK(etl::be_char_t(0x01) == char(0x01));

      CHECK(etl::be_schar_t(0x01) == etl::be_schar_t(0x01));
      CHECK((signed char)(0x01) == etl::be_schar_t(0x01));
      CHECK(etl::be_schar_t(0x01) == (signed char)(0x01));

      CHECK(etl::be_uchar_t(0x01U) == etl::be_uchar_t(0x01U));
      CHECK((unsigned char)(0x01U) == etl::be_uchar_t(0x01U));
      CHECK(etl::be_uchar_t(0x01U) == (unsigned char)(0x01U));

      // short
      CHECK(etl::be_short_t(0x0123) == etl::be_short_t(0x0123));
      CHECK(short(0x0123) == etl::be_short_t(0x0123));
      CHECK(etl::be_short_t(0x0123) == short(0x0123));

      CHECK(etl::be_ushort_t(0x0123) == etl::be_ushort_t(0x0123));
      CHECK((unsigned short)(0x0123) == etl::be_ushort_t(0x0123));
      CHECK(etl::be_ushort_t(0x0123) == (unsigned short)(0x0123));

      // int
      CHECK(etl::be_int_t(0x01234567) == etl::be_int_t(0x01234567));
      CHECK(int(0x01234567) == etl::be_int_t(0x01234567));
      CHECK(etl::be_int_t(0x01234567) == int(0x01234567));

      CHECK(etl::be_uint_t(0x01234567U) == etl::be_uint_t(0x01234567U));
      CHECK((unsigned int)(0x01234567U) == etl::be_uint_t(0x01234567U));
      CHECK(etl::be_uint_t(0x01234567U) == (unsigned int)(0x01234567U));

      if (sizeof(long) == 4U)
      {
        // long
        CHECK(etl::be_long_t(0x01234567) == etl::be_long_t(0x01234567));
        CHECK(long(0x01234567) == etl::be_long_t(0x01234567));
        CHECK(etl::be_long_t(0x01234567) == long(0x01234567));

        CHECK(etl::be_ulong_t(0x01234567U) == etl::be_ulong_t(0x01234567U));
        CHECK((unsigned long)(0x01234567U) == etl::be_ulong_t(0x01234567U));
        CHECK(etl::be_ulong_t(0x01234567U) == (unsigned long)(0x01234567U));
      }

      if (sizeof(long long) == 8U)
      {
        // long long
        CHECK(etl::be_long_long_t(0x0123456789ABCDEF) == etl::be_long_long_t(0x0123456789ABCDEF));
        CHECK((long long)(0x0123456789ABCDEF) == etl::be_long_long_t(0x0123456789ABCDEF));
        CHECK(etl::be_long_long_t(0x0123456789ABCDEF) == (long long)(0x0123456789ABCDEF));

        CHECK(etl::be_ulong_long_t(0x0123456789ABCDEFU) == etl::be_ulong_long_t(0x0123456789ABCDEFU));
        CHECK((unsigned long long)(0x0123456789ABCDEFU) == etl::be_ulong_long_t(0x0123456789ABCDEFU));
        CHECK(etl::be_ulong_long_t(0x0123456789ABCDEFU) == (unsigned long long)(0x0123456789ABCDEFU));
      }

      // float
      CHECK_CLOSE(etl::be_float_t(3.1415927f), etl::be_float_t(3.1415927f), 0.001);
      CHECK_CLOSE(3.1415927f, etl::be_float_t(3.1415927f), 0.001);
      CHECK_CLOSE(etl::be_float_t(3.1415927f), 3.1415927f, 0.001);

      // double
      CHECK_CLOSE(etl::be_double_t(3.1415927), etl::be_double_t(3.1415927), 0.001);
      CHECK_CLOSE(3.1415927, etl::be_double_t(3.1415927), 0.001);
      CHECK_CLOSE(etl::be_double_t(3.1415927), 3.1415927, 0.001);

      // long double
      CHECK_CLOSE(etl::be_long_double_t(3.1415927L), etl::be_long_double_t(3.1415927L), 0.001);
      CHECK_CLOSE(3.1415927L, etl::be_long_double_t(3.1415927L), 0.001);
      CHECK_CLOSE(etl::be_long_double_t(3.1415927L), 3.1415927L, 0.001);
    }

    //*************************************************************************
    TEST(test_be_inequality)
    {
      // char
      CHECK(etl::be_char_t(0x01) != etl::be_char_t(0x02));
      CHECK(char(0x01) != etl::be_char_t(0x02));
      CHECK(etl::be_char_t(0x01) != char(0x02));

      CHECK(etl::be_schar_t(0x01) != etl::be_schar_t(0x02));
      CHECK((signed char)(0x01) != etl::be_schar_t(0x02));
      CHECK(etl::be_schar_t(0x01) != (signed char)(0x02));

      CHECK(etl::be_uchar_t(0x01U) != etl::be_uchar_t(0x02U));
      CHECK((unsigned char)(0x01U) != etl::be_uchar_t(0x02U));
      CHECK(etl::be_uchar_t(0x01U) != (unsigned char)(0x02U));

      // short
      CHECK(etl::be_short_t(0x0123) != etl::be_short_t(0x0223));
      CHECK(short(0x0123) != etl::be_short_t(0x0223));
      CHECK(etl::be_short_t(0x0123) != short(0x0223));

      CHECK(etl::be_ushort_t(0x0123) != etl::be_ushort_t(0x0223));
      CHECK((unsigned short)(0x0123) != etl::be_ushort_t(0x0223));
      CHECK(etl::be_ushort_t(0x0123) != (unsigned short)(0x0223));

      // int
      CHECK(etl::be_int_t(0x01234567) != etl::be_int_t(0x02234567));
      CHECK(int(0x01234567) != etl::be_int_t(0x02234567));
      CHECK(etl::be_int_t(0x01234567) != int(0x02234567));

      CHECK(etl::be_uint_t(0x01234567U) != etl::be_uint_t(0x02234567U));
      CHECK((unsigned int)(0x01234567U) != etl::be_uint_t(0x02234567U));
      CHECK(etl::be_uint_t(0x01234567U) != (unsigned int)(0x02234567U));

      if (sizeof(long) == 4U)
      {
        // long
        CHECK(etl::be_long_t(0x01234567) != etl::be_long_t(0x02234567));
        CHECK(long(0x01234567) != etl::be_long_t(0x02234567));
        CHECK(etl::be_long_t(0x01234567) != long(0x02234567));

        CHECK(etl::be_ulong_t(0x01234567U) != etl::be_ulong_t(0x02234567U));
        CHECK((unsigned long)(0x01234567U) != etl::be_ulong_t(0x02234567U));
        CHECK(etl::be_ulong_t(0x01234567U) != (unsigned long)(0x02234567U));
      }

      if (sizeof(long long) == 8U)
      {
        // long long
        CHECK(etl::be_long_long_t(0x0123456789ABCDEF) != etl::be_long_long_t(0x0223456789ABCDEF));
        CHECK((long long)(0x0123456789ABCDEF) != etl::be_long_long_t(0x0223456789ABCDEF));
        CHECK(etl::be_long_long_t(0x0123456789ABCDEF) != (long long)(0x0223456789ABCDEF));

        CHECK(etl::be_ulong_long_t(0x0123456789ABCDEFU) != etl::be_ulong_long_t(0x0223456789ABCDEFU));
        CHECK((unsigned long long)(0x0123456789ABCDEFU) != etl::be_ulong_long_t(0x0223456789ABCDEFU));
        CHECK(etl::be_ulong_long_t(0x0123456789ABCDEFU) != (unsigned long long)(0x0223456789ABCDEFU));
      }

      // float
      CHECK_FLOAT_DIFFERENT(etl::be_float_t(3.1415927f), etl::be_float_t(2.7182818f));
      CHECK_FLOAT_DIFFERENT(3.1415927f, etl::be_float_t(2.7182818f));
      CHECK_FLOAT_DIFFERENT(etl::be_float_t(3.1415927f), 2.7182818f);

      // double
      CHECK_FLOAT_DIFFERENT(etl::be_double_t(3.1415927), etl::be_double_t(2.7182818));
      CHECK_FLOAT_DIFFERENT(3.1415927, etl::be_double_t(2.7182818));
      CHECK_FLOAT_DIFFERENT(etl::be_double_t(3.1415927), 2.7182818);

      // long double
      CHECK_FLOAT_DIFFERENT(etl::be_long_double_t(3.1415927L), etl::be_long_double_t(2.7182818L));
      CHECK_FLOAT_DIFFERENT(3.1415927L, etl::be_long_double_t(2.7182818L));
      CHECK_FLOAT_DIFFERENT(etl::be_long_double_t(3.1415927L), 2.7182818L);
    }

    //*************************************************************************
    TEST(test_le_assignment)
    {
      etl::le_char_t le_char;
      le_char = 0x01;
      CHECK(0x01 == (char)le_char);

      etl::le_schar_t le_schar;
      le_schar = 0x01;
      CHECK(0x01 == (signed char)le_schar);

      etl::le_uchar_t le_uchar;
      le_uchar = 0x01;
      CHECK(0x01 == (unsigned char)le_uchar);

      etl::le_short_t le_short;
      le_short = 0x0123;
      CHECK(0x0123 == (short)le_short);

      etl::le_ushort_t le_ushort;
      le_ushort = 0x0123;
      CHECK(0x0123 == (unsigned short)le_ushort);

      etl::le_int_t le_int;
      le_int = 0x01234567;
      CHECK(0x01234567 == (int)le_int);

      etl::le_uint_t le_uint;
      le_uint = 0x01234567;
      CHECK(0x01234567 == (unsigned int)le_uint);

      if (sizeof(long) == 4U)
      {
        etl::le_long_t le_long;
        le_long = 0x01234567;
        CHECK(0x01234567 == (long)le_long);

        etl::le_ulong_t le_ulong;
        le_ulong = 0x01234567;
        CHECK(0x01234567 == (unsigned long)le_ulong);
      }

      if (sizeof(long long) == 8U)
      {
        etl::le_long_long_t le_long;
        le_long = 0x0123456789ABCDEF;
        CHECK(0x0123456789ABCDEF == (long long)le_long);

        etl::le_ulong_long_t le_ulong;
        le_ulong = 0x0123456789ABCDEF;
        CHECK(0x0123456789ABCDEF == (unsigned long long)le_ulong);
      }

      // float
      etl::le_float_t le_float;
      le_float = 3.1415927f;
      CHECK_CLOSE(3.1415927f, le_float, 0.001);

      // double
      etl::le_double_t le_double;
      le_double = 3.1415927;
      CHECK_CLOSE(3.1415927, le_double, 0.001);

      // long double
      etl::le_long_double_t le_long_double;
      le_long_double = 3.1415927L;
      CHECK_CLOSE(3.1415927L, le_long_double, 0.001);
    }

    //*************************************************************************
    TEST(test_be_assignment)
    {
      etl::le_char_t be_char;
      be_char = 0x01;
      CHECK(0x01 == (char)be_char);

      etl::be_schar_t be_schar;
      be_schar = 0x01;
      CHECK(0x01 == (signed char)be_schar);

      etl::be_uchar_t be_uchar;
      be_uchar = 0x01;
      CHECK(0x01 == (unsigned char)be_uchar);

      etl::be_short_t be_short;
      be_short = 0x0123;
      CHECK(0x0123 == (short)be_short);

      etl::be_ushort_t be_ushort;
      be_ushort = 0x0123;
      CHECK(0x0123 == (unsigned short)be_ushort);

      etl::be_int_t be_int;
      be_int = 0x01234567;
      CHECK(0x01234567 == (int)be_int);

      etl::be_uint_t be_uint;
      be_uint = 0x01234567;
      CHECK(0x01234567 == (unsigned int)be_uint);

      if (sizeof(long) == 4U)
      {
        etl::be_long_t be_long;
        be_long = 0x01234567;
        CHECK(0x01234567 == (long)be_long);

        etl::be_ulong_t be_ulong;
        be_ulong = 0x01234567;
        CHECK(0x01234567 == (unsigned long)be_ulong);
      }

      if (sizeof(long long) == 8U)
      {
        etl::be_long_long_t be_long;
        be_long = 0x0123456789ABCDEF;
        CHECK(0x0123456789ABCDEF == (long long)be_long);

        etl::be_ulong_long_t be_ulong;
        be_ulong = 0x0123456789ABCDEF;
        CHECK(0x0123456789ABCDEF == (unsigned long long)be_ulong);
      }

      // float
      etl::be_float_t be_float;
      be_float = 3.1415927f;
      CHECK_CLOSE(3.1415927f, be_float, 0.001);

      // double
      etl::be_double_t be_double;
      be_double = 3.1415927;
      CHECK_CLOSE(3.1415927, be_double, 0.001);

      // long double
      etl::be_long_double_t be_long_double;
      be_long_double = 3.1415927L;
      CHECK_CLOSE(3.1415927L, be_long_double, 0.001);
    }

    //*************************************************************************
    TEST(test_cross_assignment)
    {
      etl::le_uint32_t le_v1(0x01234567);
      etl::be_uint32_t be_v1(0x01234567);
      etl::le_uint32_t le_v2;
      etl::be_uint32_t be_v2;
      etl::le_uint32_t le_v3;
      etl::be_uint32_t be_v3;

      le_v2 = le_v1; // Assign le from le.
      be_v2 = be_v1; // Assign be from be.
      le_v3 = be_v1; // Assign le from be.
      be_v3 = le_v1; // Assign be from le.

      CHECK_EQUAL(int(0x01234567), int(le_v2));
      CHECK_EQUAL(int(0x01234567), int(be_v2));
      CHECK_EQUAL(int(0x01234567), int(le_v3));
      CHECK_EQUAL(int(0x01234567), int(be_v3));
    }

    //*************************************************************************
    TEST(test_cross_assignment_float)
    {
      etl::le_float_t le_v1(3.1415927f);
      etl::be_float_t be_v1(3.1415927f);
      etl::le_float_t le_v2;
      etl::be_float_t be_v2;
      etl::le_float_t le_v3;
      etl::be_float_t be_v3;

      le_v2 = le_v1; // Assign le from le.
      be_v2 = be_v1; // Assign be from be.
      le_v3 = be_v1; // Assign le from be.
      be_v3 = le_v1; // Assign be from le.

      CHECK_CLOSE(3.1415927f, le_v2, 0.001);
      CHECK_CLOSE(3.1415927f, be_v2, 0.001);
      CHECK_CLOSE(3.1415927f, le_v3, 0.001);
      CHECK_CLOSE(3.1415927f, be_v3, 0.001);
    }

    //*************************************************************************
    TEST(test_cross_assignment_double)
    {
      etl::le_double_t le_v1(3.1415927);
      etl::be_double_t be_v1(3.1415927);
      etl::le_double_t le_v2;
      etl::be_double_t be_v2;
      etl::le_double_t le_v3;
      etl::be_double_t be_v3;

      le_v2 = le_v1; // Assign le from le.
      be_v2 = be_v1; // Assign be from be.
      le_v3 = be_v1; // Assign le from be.
      be_v3 = le_v1; // Assign be from le.

      CHECK_CLOSE(3.1415927, le_v2, 0.001);
      CHECK_CLOSE(3.1415927, be_v2, 0.001);
      CHECK_CLOSE(3.1415927, le_v3, 0.001);
      CHECK_CLOSE(3.1415927, be_v3, 0.001);
    }

    //*************************************************************************
    TEST(test_cross_assignment_long_double)
    {
      etl::le_long_double_t le_v1(3.1415927L);
      etl::be_long_double_t be_v1(3.1415927L);
      etl::le_long_double_t le_v2;
      etl::be_long_double_t be_v2;
      etl::le_long_double_t le_v3;
      etl::be_long_double_t be_v3;

      le_v2 = le_v1; // Assign le from le.
      be_v2 = be_v1; // Assign be from be.
      le_v3 = be_v1; // Assign le from be.
      be_v3 = le_v1; // Assign be from le.

      CHECK_CLOSE(3.1415927L, le_v2, 0.001);
      CHECK_CLOSE(3.1415927L, be_v2, 0.001);
      CHECK_CLOSE(3.1415927L, le_v3, 0.001);
      CHECK_CLOSE(3.1415927L, be_v3, 0.001);
    }

    //*************************************************************************
    TEST(test_data)
    {
      etl::be_uint16_t test(0x1234);

      CHECK_EQUAL(0x12, *test.data());
      CHECK_EQUAL(0x34, *(test.data() + 1U));
    }

    //*************************************************************************
    TEST(test_begin_end_iterator)
    {
      using iterator       = etl::be_uint16_t::iterator;
      using const_iterator = etl::be_uint16_t::const_iterator;

      etl::be_uint16_t       test(0x1234);
      const etl::be_uint16_t const_test(0x1234);

      iterator       itr;
      const_iterator citr;

      //*******************************
      itr = test.begin();
      CHECK_EQUAL(int(0x12), int(*itr));
      ++itr;
      CHECK_EQUAL(int(0x34), int(*itr));
      ++itr;
      CHECK(itr == test.end());

      //*******************************
      itr  = test.begin();
      *itr = 0x34;
      CHECK_EQUAL(int(0x34), int(*itr));
      ++itr;
      *itr = 0x12;
      CHECK_EQUAL(0x12, *itr);
      ++itr;
      CHECK(itr == test.end());

      //*******************************
      citr = const_test.begin();
      CHECK_EQUAL(0x12, *citr);
      ++citr;
      CHECK_EQUAL(0x34, *citr);
      ++citr;
      CHECK(citr == const_test.end());

      //*******************************
      citr = const_test.cbegin();
      CHECK_EQUAL(0x12, *citr);
      ++citr;
      CHECK_EQUAL(0x34, *citr);
      ++citr;
      CHECK(citr == const_test.cend());
    }

    //*************************************************************************
    TEST(test_rbegin_rend_iterator)
    {
      using reverse_iterator       = etl::be_uint16_t::reverse_iterator;
      using const_reverse_iterator = etl::be_uint16_t::const_reverse_iterator;

      etl::be_uint16_t       test(0x1234);
      const etl::be_uint16_t const_test(0x1234);

      reverse_iterator       itr;
      const_reverse_iterator citr;

      //*******************************
      itr = test.rbegin();
      CHECK_EQUAL(0x34, *itr);
      ++itr;
      CHECK_EQUAL(0x12, *itr);
      ++itr;
      CHECK(itr == test.rend());

      //*******************************
      itr  = test.rbegin();
      *itr = 0x12;
      CHECK_EQUAL(0x12, *itr);
      ++itr;
      *itr = 0x34;
      CHECK_EQUAL(0x34, *itr);
      ++itr;
      CHECK(itr == test.rend());

      //*******************************
      citr = const_test.rbegin();
      CHECK_EQUAL(0x34, *citr);
      ++citr;
      CHECK_EQUAL(0x12, *citr);
      ++citr;
      CHECK(citr == const_test.rend());

      //*******************************
      citr = const_test.crbegin();
      CHECK_EQUAL(0x34, *citr);
      ++citr;
      CHECK_EQUAL(0x12, *citr);
      ++citr;
      CHECK(citr == const_test.crend());
    }

    //*************************************************************************
    TEST(test_index_operator)
    {
      etl::le_uint16_t       test_le(0x1234);
      const etl::le_uint16_t const_test_le(0x1234);

      etl::be_uint16_t       test_be(0x1234);
      const etl::be_uint16_t const_test_be(0x1234);

      CHECK_EQUAL(0x34, test_le[0]);
      CHECK_EQUAL(0x12, test_le[1]);

      test_le[0] = 0x56;
      test_le[1] = 0x78;
      CHECK_EQUAL(0x56, test_le[0]);
      CHECK_EQUAL(0x78, test_le[1]);

      CHECK_EQUAL(0x34, const_test_le[0]);
      CHECK_EQUAL(0x12, const_test_le[1]);

      CHECK_EQUAL(0x12, test_be[0]);
      CHECK_EQUAL(0x34, test_be[1]);

      test_be[0] = 0x56;
      test_be[1] = 0x78;
      CHECK_EQUAL(0x56, test_be[0]);
      CHECK_EQUAL(0x78, test_be[1]);

      CHECK_EQUAL(0x12, const_test_be[0]);
      CHECK_EQUAL(0x34, const_test_be[1]);
    }

    //*************************************************************************
    TEST(test_le_negative_numbers)
    {
      // char
      CHECK(char(0xFE) == etl::le_char_t(char(0xFE)));
      CHECK((signed char)(0xFE) == etl::le_schar_t((signed char)(0xFE)));
      CHECK((unsigned char)(0xFEU) == etl::le_uchar_t((unsigned char)(0xFEU)));

      // short
      CHECK(short(0xFEFD) == etl::le_short_t(short(0xFEFD)));
      CHECK((unsigned short)(0xFEFD) == etl::le_ushort_t((unsigned short)(0xFEFD)));

      // int
      CHECK(static_cast<int>(0xFEFDFCFB) == etl::le_int_t(static_cast<int>(0xFEFDFCFB)));
      CHECK((unsigned int)(0xFEFDFCFBU) == etl::le_uint_t(0xFEFDFCFBU));

      if (sizeof(long) == 4U)
      {
        // long
        CHECK(static_cast<long>(0xFEFDFCFB) == etl::le_long_t(static_cast<long>(0xFEFDFCFB)));
        CHECK((unsigned long)(0xFEFDFCFBU) == etl::le_ulong_t(0xFEFDFCFBU));
      }

      if (sizeof(long long) == 8U)
      {
        // long long
        CHECK(static_cast<long long>(0xFEFDFCFBFAF9F8F7) == etl::le_long_long_t(static_cast<long long>(0xFEFDFCFBFAF9F8F7)));
        CHECK((unsigned long long)(0xFEFDFCFBFAF9F8F7U) == etl::le_ulong_long_t(0xFEFDFCFBFAF9F8F7U));
      }
    }

    //*************************************************************************
    TEST(test_value)
    {
      etl::le_uint16_t test_le(0x1234);
      etl::be_uint16_t test_be(0x1234);

      CHECK_EQUAL(0x1234, test_le.value());
      CHECK_EQUAL(0x1234, test_be.value());

      CHECK_EQUAL(0x1234, test_le);
      CHECK_EQUAL(0x1234, test_be);
    }

    //*************************************************************************
    TEST(test_storage_bytes)
    {
      etl::le_uint16_t test_le(0x1234);
      etl::be_uint16_t test_be(0x1234);

      int lev0 = test_le[0];
      int lev1 = test_le[1];

      int bev0 = test_be[0];
      int bev1 = test_be[1];

      CHECK_EQUAL(0x34, lev0);
      CHECK_EQUAL(0x12, lev1);
      CHECK_EQUAL(0x12, bev0);
      CHECK_EQUAL(0x34, bev1);
    }

#if ETL_HAS_CONSTEXPR_ENDIANNESS
    //*************************************************************************
    TEST(test_constexpr_endianness_integral_round_trip)
    {
      // Store a known value in LE, BE, and host-order unaligned types.
      const uint32_t value = 0x12345678U;

      etl::le_uint32_t   le_v(value);
      etl::be_uint32_t   be_v(value);
      etl::host_uint32_t host_v(value);

      // All must read back the original value.
      CHECK_EQUAL(value, uint32_t(le_v));
      CHECK_EQUAL(value, uint32_t(be_v));
      CHECK_EQUAL(value, uint32_t(host_v));

      // Verify the storage byte order is correct.
      // LE stores LSB first: 0x78, 0x56, 0x34, 0x12
      CHECK_EQUAL(0x78, int(le_v[0]));
      CHECK_EQUAL(0x56, int(le_v[1]));
      CHECK_EQUAL(0x34, int(le_v[2]));
      CHECK_EQUAL(0x12, int(le_v[3]));

      // BE stores MSB first: 0x12, 0x34, 0x56, 0x78
      CHECK_EQUAL(0x12, int(be_v[0]));
      CHECK_EQUAL(0x34, int(be_v[1]));
      CHECK_EQUAL(0x56, int(be_v[2]));
      CHECK_EQUAL(0x78, int(be_v[3]));

      // Host-order must match one of the above depending on the platform.
      if (etl::endianness::value() == etl::endian::little)
      {
        CHECK_EQUAL(0x78, int(host_v[0]));
        CHECK_EQUAL(0x56, int(host_v[1]));
        CHECK_EQUAL(0x34, int(host_v[2]));
        CHECK_EQUAL(0x12, int(host_v[3]));
      }
      else
      {
        CHECK_EQUAL(0x12, int(host_v[0]));
        CHECK_EQUAL(0x34, int(host_v[1]));
        CHECK_EQUAL(0x56, int(host_v[2]));
        CHECK_EQUAL(0x78, int(host_v[3]));
      }
    }

    //*************************************************************************
    TEST(test_constexpr_endianness_float_round_trip)
    {
      // Store a known float value in LE, BE, and host-order unaligned types.
      const float value = 3.1415927f;

      etl::le_float_t   le_v(value);
      etl::be_float_t   be_v(value);
      etl::host_float_t host_v(value);

      // All must read back the original value.
      CHECK_CLOSE(value, float(le_v), 0.0001f);
      CHECK_CLOSE(value, float(be_v), 0.0001f);
      CHECK_CLOSE(value, float(host_v), 0.0001f);

      // LE and BE storage bytes must be the reverse of each other.
      CHECK_EQUAL(int(le_v[0]), int(be_v[3]));
      CHECK_EQUAL(int(le_v[1]), int(be_v[2]));
      CHECK_EQUAL(int(le_v[2]), int(be_v[1]));
      CHECK_EQUAL(int(le_v[3]), int(be_v[0]));
    }

    //*************************************************************************
    TEST(test_constexpr_endianness_cross_endian_copy)
    {
      // Verify that converting between LE <-> BE via host works correctly.
      const uint16_t value = 0xABCDU;

      etl::le_uint16_t   le_v(value);
      etl::be_uint16_t   be_v(value);
      etl::host_uint16_t host_from_le(le_v);
      etl::host_uint16_t host_from_be(be_v);

      CHECK_EQUAL(value, uint16_t(host_from_le));
      CHECK_EQUAL(value, uint16_t(host_from_be));

      // Round-trip: host -> le -> read back
      etl::le_uint16_t le_from_host(host_from_le);
      CHECK_EQUAL(value, uint16_t(le_from_host));

      // Round-trip: host -> be -> read back
      etl::be_uint16_t be_from_host(host_from_be);
      CHECK_EQUAL(value, uint16_t(be_from_host));
    }
#endif

#if ETL_USING_CPP14
    //*************************************************************************
    // Demonstrates that integral endian conversion is now usable at compile
    // time (constexpr), via genuine static_assert checks - not just runtime
    // CHECK_EQUAL. The conversion is evaluated entirely by the compiler.
    //*************************************************************************
    TEST(test_constexpr_integral_endian_conversion)
    {
      // Constructing from a value and reading it back via value()/operator T()
      // is constexpr for integral types, regardless of endianness.
      constexpr etl::le_uint32_t le_v(0x12345678U);
      constexpr etl::be_uint32_t be_v(0x12345678U);

      static_assert(le_v.value() == 0x12345678U, "le_uint32_t constexpr value() round trip");
      static_assert(be_v.value() == 0x12345678U, "be_uint32_t constexpr value() round trip");

      static_assert(uint32_t(le_v) == 0x12345678U, "le_uint32_t constexpr operator T() round trip");
      static_assert(uint32_t(be_v) == 0x12345678U, "be_uint32_t constexpr operator T() round trip");

      // Verify the underlying byte order is also usable at compile time.
      static_assert(le_v.data()[0] == 0x78, "le_uint32_t constexpr byte 0");
      static_assert(le_v.data()[1] == 0x56, "le_uint32_t constexpr byte 1");
      static_assert(le_v.data()[2] == 0x34, "le_uint32_t constexpr byte 2");
      static_assert(le_v.data()[3] == 0x12, "le_uint32_t constexpr byte 3");

      static_assert(be_v.data()[0] == 0x12, "be_uint32_t constexpr byte 0");
      static_assert(be_v.data()[1] == 0x34, "be_uint32_t constexpr byte 1");
      static_assert(be_v.data()[2] == 0x56, "be_uint32_t constexpr byte 2");
      static_assert(be_v.data()[3] == 0x78, "be_uint32_t constexpr byte 3");

      // Prove the checks above are genuine compile-time facts, not just
      // syntactically valid constexpr, by also exercising them at runtime.
      CHECK_EQUAL(0x12345678U, uint32_t(le_v));
      CHECK_EQUAL(0x12345678U, uint32_t(be_v));
    }
#endif

#if ETL_USING_CPP14 && ETL_USING_BUILTIN_BIT_CAST
    //*************************************************************************
    // Demonstrates that floating point endian conversion is now usable at
    // compile time (constexpr) too, via etl::bit_cast (no std::bit_cast or
    // STL dependency required).
    //*************************************************************************
    TEST(test_constexpr_float_endian_conversion)
    {
      constexpr etl::le_float_t le_v(1.5f);
      constexpr etl::be_float_t be_v(1.5f);

      // Bit patterns are compared, rather than the float values, to avoid
      // '-Wfloat-equal'.
      static_assert(etl::bit_cast<uint32_t>(le_v.value()) == 0x3FC00000U, "le_float_t constexpr value() round trip");
      static_assert(etl::bit_cast<uint32_t>(be_v.value()) == 0x3FC00000U, "be_float_t constexpr value() round trip");

      static_assert(etl::bit_cast<uint32_t>(float(le_v)) == 0x3FC00000U, "le_float_t constexpr operator T() round trip");
      static_assert(etl::bit_cast<uint32_t>(float(be_v)) == 0x3FC00000U, "be_float_t constexpr operator T() round trip");

      // LE and BE storage bytes must be the reverse of each other, checked at
      // compile time.
      static_assert(le_v.data()[0] == be_v.data()[3], "byte 0/3 mismatch");
      static_assert(le_v.data()[1] == be_v.data()[2], "byte 1/2 mismatch");
      static_assert(le_v.data()[2] == be_v.data()[1], "byte 2/1 mismatch");
      static_assert(le_v.data()[3] == be_v.data()[0], "byte 3/0 mismatch");

      CHECK_EQUAL(1.5f, float(le_v));
      CHECK_EQUAL(1.5f, float(be_v));
    }
#endif
  }
} // namespace

#include "etl/private/diagnostic_pop.h"
