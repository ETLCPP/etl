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

#include "etl/unaligned_type.h"
#include "etl/integral_limits.h"

#include "etl/private/diagnostic_useless_cast_push.h"

#include <array>

namespace
{
  using storage_c_t    = std::array<unsigned char, sizeof(char)>;
  using storage_s_t    = std::array<unsigned char, sizeof(short)>;
  using storage_i_t    = std::array<unsigned char, sizeof(int)>;
  using storage_l_t    = std::array<unsigned char, sizeof(long)>;
  using storage_ll_t   = std::array<unsigned char, sizeof(long long)>;
  using storage_f_t    = std::array<unsigned char, sizeof(float)>;
  using storage_d_t    = std::array<unsigned char, sizeof(double)>;
  using storage_ld_t   = std::array<unsigned char, sizeof(long double)>;
  using storage_i8_t   = std::array<unsigned char, sizeof(int8_t)>;
  using storage_i16_t  = std::array<unsigned char, sizeof(int16_t)>;
  using storage_i32_t  = std::array<unsigned char, sizeof(int32_t)>;
  using storage_i64_t  = std::array<unsigned char, sizeof(int64_t)>;

  SUITE(test_unaligned_type_ext)
  {
    //*************************************************************************
    TEST(test_copy_construction_8)
    {
      storage_i32_t store_le1;
      storage_i32_t store_be1;
      storage_i32_t store_le2;
      storage_i32_t store_be2;
      storage_i32_t store_le3;
      storage_i32_t store_be3;

      etl::le_uint32_ext_t le_v1(store_le1.data());
      le_v1 = 0xEF;
      etl::be_uint32_ext_t be_v1(0xEF, store_be1.data());

      etl::le_uint32_ext_t le_v2(le_v1, store_le2.data()); // Copy construct le from le.
      etl::be_uint32_ext_t be_v2(be_v1, store_be2.data()); // Copy construct be from be.
      etl::le_uint32_ext_t le_v3(be_v1, store_le3.data()); // Copy construct le from be.
      etl::be_uint32_ext_t be_v3(le_v1, store_be3.data()); // Copy construct be from le.

      CHECK_EQUAL(int(0xEF), int(le_v2));
      CHECK_EQUAL(int(0xEF), int(be_v2));
      CHECK_EQUAL(int(0xEF), int(le_v3));
      CHECK_EQUAL(int(0xEF), int(be_v3));
    }

    //*************************************************************************
    TEST(test_move_construction_8)
    {
      storage_i32_t store_le1;
      storage_i32_t store_be1;

      etl::le_uint32_ext_t le_v1(0xEF, store_le1.data());
      etl::be_uint32_ext_t be_v1(0xEF, store_be1.data());

      etl::le_uint32_ext_t le_v2(etl::move(le_v1)); // Move construct le from le.
      etl::be_uint32_ext_t be_v2(etl::move(be_v1)); // Move construct be from be.

      CHECK_TRUE(nullptr == le_v1.data());
      CHECK_TRUE(nullptr == be_v1.data());
      CHECK_EQUAL(int(0xEF), int(le_v2));
      CHECK_EQUAL(int(0xEF), int(be_v2));
    }

    //*************************************************************************
    TEST(test_copy_construction_16)
    {
      storage_i32_t store_le1;
      storage_i32_t store_be1;
      storage_i32_t store_le2;
      storage_i32_t store_be2;
      storage_i32_t store_le3;
      storage_i32_t store_be3;

      etl::le_uint32_ext_t le_v1(store_le1.data());
      le_v1 = 0xEEEF;
      etl::be_uint32_ext_t be_v1(0xEEEF, store_be1.data());

      etl::le_uint32_ext_t le_v2(le_v1, store_le2.data()); // Copy construct le from le.
      etl::be_uint32_ext_t be_v2(be_v1, store_be2.data()); // Copy construct be from be.
      etl::le_uint32_ext_t le_v3(be_v1, store_le3.data()); // Copy construct le from be.
      etl::be_uint32_ext_t be_v3(le_v1, store_be3.data()); // Copy construct be from le.

      CHECK_EQUAL(int(0xEEEF), int(le_v2));
      CHECK_EQUAL(int(0xEEEF), int(be_v2));
      CHECK_EQUAL(int(0xEEEF), int(le_v3));
      CHECK_EQUAL(int(0xEEEF), int(be_v3));
    }

    //*************************************************************************
    TEST(test_move_construction_16)
    {
      storage_i32_t store_le1;
      storage_i32_t store_be1;

      etl::le_uint32_ext_t le_v1(0xEEEF, store_le1.data());
      etl::be_uint32_ext_t be_v1(0xEEEF, store_be1.data());

      etl::le_uint32_ext_t le_v2(etl::move(le_v1)); // Move construct le from le.
      etl::be_uint32_ext_t be_v2(etl::move(be_v1)); // Move construct be from be.

      CHECK_TRUE(nullptr == le_v1.data());
      CHECK_TRUE(nullptr == be_v1.data());
      CHECK_EQUAL(int(0xEEEF), int(le_v2));
      CHECK_EQUAL(int(0xEEEF), int(be_v2));
    }

    //*************************************************************************
    TEST(test_copy_construction_32)
    {
      storage_i32_t store_le1;
      storage_i32_t store_be1;
      storage_i32_t store_le2;
      storage_i32_t store_be2;
      storage_i32_t store_le3;
      storage_i32_t store_be3;

      etl::le_uint32_ext_t le_v1(store_le1.data());
      le_v1 = 0xEE2345EF;
      etl::be_uint32_ext_t be_v1(0xEE2345EF, store_be1.data());

      etl::le_uint32_ext_t le_v2(le_v1, store_le2.data()); // Copy construct le from le.
      etl::be_uint32_ext_t be_v2(be_v1, store_be2.data()); // Copy construct be from be.
      etl::le_uint32_ext_t le_v3(be_v1, store_le3.data()); // Copy construct le from be.
      etl::be_uint32_ext_t be_v3(le_v1, store_be3.data()); // Copy construct be from le.

      CHECK_EQUAL(int(0xEE2345EF), int(le_v2));
      CHECK_EQUAL(int(0xEE2345EF), int(be_v2));
      CHECK_EQUAL(int(0xEE2345EF), int(le_v3));
      CHECK_EQUAL(int(0xEE2345EF), int(be_v3));
    }

    //*************************************************************************
    TEST(test_move_construction_32)
    {
      storage_i32_t store_le1;
      storage_i32_t store_be1;

      etl::le_uint32_ext_t le_v1(0xEE2345EF, store_le1.data());
      etl::be_uint32_ext_t be_v1(0xEE2345EF, store_be1.data());

      etl::le_uint32_ext_t le_v2(etl::move(le_v1)); // Move construct le from le.
      etl::be_uint32_ext_t be_v2(etl::move(be_v1)); // Move construct be from be.

      CHECK_TRUE(nullptr == le_v1.data());
      CHECK_TRUE(nullptr == be_v1.data());
      CHECK_EQUAL(int(0xEE2345EF), int(le_v2));
      CHECK_EQUAL(int(0xEE2345EF), int(be_v2));
    }

    //*************************************************************************
    TEST(test_copy_construction_64)
    {
      storage_i64_t store_le1;
      storage_i64_t store_be1;
      storage_i64_t store_le2;
      storage_i64_t store_be2;
      storage_i64_t store_le3;
      storage_i64_t store_be3;

      etl::le_uint64_ext_t le_v1(store_le1.data());
      le_v1 = 0xEE23456789ABCDEF;
      etl::be_uint64_ext_t be_v1(0xEE23456789ABCDEF, store_be1.data());

      etl::le_uint64_ext_t le_v2(le_v1, store_le2.data()); // Copy construct le from le.
      etl::be_uint64_ext_t be_v2(be_v1, store_be2.data()); // Copy construct be from be.
      etl::le_uint64_ext_t le_v3(be_v1, store_le3.data()); // Copy construct le from be.
      etl::be_uint64_ext_t be_v3(le_v1, store_be3.data()); // Copy construct be from le.

      CHECK_EQUAL(int(0xEE23456789ABCDEF), int(le_v2));
      CHECK_EQUAL(int(0xEE23456789ABCDEF), int(be_v2));
      CHECK_EQUAL(int(0xEE23456789ABCDEF), int(le_v3));
      CHECK_EQUAL(int(0xEE23456789ABCDEF), int(be_v3));
    }

    //*************************************************************************
    TEST(test_move_construction_64)
    {
      storage_i64_t store_le1;
      storage_i64_t store_be1;

      etl::le_uint64_ext_t le_v1(0xEE23456789ABCDEF, store_le1.data());
      etl::be_uint64_ext_t be_v1(0xEE23456789ABCDEF, store_be1.data());

      etl::le_uint64_ext_t le_v2(etl::move(le_v1)); // Move construct le from le.
      etl::be_uint64_ext_t be_v2(etl::move(be_v1)); // Move construct be from be.

      CHECK_TRUE(nullptr == le_v1.data());
      CHECK_TRUE(nullptr == be_v1.data());
      CHECK_EQUAL(int(0xEE23456789ABCDEF), int(le_v2));
      CHECK_EQUAL(int(0xEE23456789ABCDEF), int(be_v2));
    }

    //*************************************************************************
    TEST(test_copy_construction_float)
    {
      storage_f_t store_le1;
      storage_f_t store_be1;
      storage_f_t store_le2;
      storage_f_t store_be2;
      storage_f_t store_le3;
      storage_f_t store_be3;

      etl::le_float_ext_t le_v1(store_le1.data());
      le_v1 = 3.1415927f;
      etl::be_float_ext_t be_v1(3.1415927f, store_be1.data());

      etl::le_float_ext_t le_v2(le_v1, store_le2.data()); // Copy construct le from le.
      etl::be_float_ext_t be_v2(be_v1, store_be2.data()); // Copy construct be from be.
      etl::le_float_ext_t le_v3(be_v1, store_le3.data()); // Copy construct le from be.
      etl::be_float_ext_t be_v3(le_v1, store_be3.data()); // Copy construct be from le.

      CHECK_EQUAL(3.1415927f, le_v2);
      CHECK_EQUAL(3.1415927f, be_v2);
      CHECK_EQUAL(3.1415927f, le_v3);
      CHECK_EQUAL(3.1415927f, be_v3);
    }

    //*************************************************************************
    TEST(test_move_construction_float)
    {
      storage_f_t store_le1;
      storage_f_t store_be1;

      etl::le_float_ext_t le_v1(3.1415927f, store_le1.data());
      etl::be_float_ext_t be_v1(3.1415927f, store_be1.data());

      etl::le_float_ext_t le_v2(etl::move(le_v1)); // Move construct le from le.
      etl::be_float_ext_t be_v2(etl::move(be_v1)); // Move construct be from be.

      CHECK_TRUE(nullptr == le_v1.data());
      CHECK_TRUE(nullptr == be_v1.data());
      CHECK_EQUAL(3.1415927f, le_v2);
      CHECK_EQUAL(3.1415927f, be_v2);
    }

    //*************************************************************************
    TEST(test_copy_construction_double)
    {
      storage_d_t store_le1;
      storage_d_t store_be1;
      storage_d_t store_le2;
      storage_d_t store_be2;
      storage_d_t store_le3;
      storage_d_t store_be3;

      etl::le_double_ext_t le_v1(store_le1.data());
      le_v1 = 3.1415927;
      etl::be_double_ext_t be_v1(3.1415927, store_be1.data());

      etl::le_double_ext_t le_v2(le_v1, store_le2.data()); // Copy construct le from le.
      etl::be_double_ext_t be_v2(be_v1, store_be2.data()); // Copy construct be from be.
      etl::le_double_ext_t le_v3(be_v1, store_le3.data()); // Copy construct le from be.
      etl::be_double_ext_t be_v3(le_v1, store_be3.data()); // Copy construct be from le.

      CHECK_EQUAL(3.1415927, le_v2);
      CHECK_EQUAL(3.1415927, be_v2);
      CHECK_EQUAL(3.1415927, le_v3);
      CHECK_EQUAL(3.1415927, be_v3);
    }

    //*************************************************************************
    TEST(test_move_construction_double)
    {
      storage_d_t store_le1;
      storage_d_t store_be1;

      etl::le_double_ext_t le_v1(3.1415927, store_le1.data());
      etl::be_double_ext_t be_v1(3.1415927, store_be1.data());

      etl::le_double_ext_t le_v2(etl::move(le_v1)); // Move construct le from le.
      etl::be_double_ext_t be_v2(etl::move(be_v1)); // Move construct be from be.

      CHECK_TRUE(nullptr == le_v1.data());
      CHECK_TRUE(nullptr == be_v1.data());
      CHECK_EQUAL(3.1415927, le_v2);
      CHECK_EQUAL(3.1415927, be_v2);
    }

    //*************************************************************************
    TEST(test_copy_construction_long_double)
    {
      storage_ld_t store_le1;
      storage_ld_t store_be1;
      storage_ld_t store_le2;
      storage_ld_t store_be2;
      storage_ld_t store_le3;
      storage_ld_t store_be3;

      etl::le_long_double_ext_t le_v1(store_le1.data());
      le_v1 = 3.1415927L;
      etl::be_long_double_ext_t be_v1(3.1415927L, store_be1.data());

      etl::le_long_double_ext_t le_v2(le_v1, store_le2.data()); // Copy construct le from le.
      etl::be_long_double_ext_t be_v2(be_v1, store_be2.data()); // Copy construct be from be.
      etl::le_long_double_ext_t le_v3(be_v1, store_le3.data()); // Copy construct le from be.
      etl::be_long_double_ext_t be_v3(le_v1, store_be3.data()); // Copy construct be from le.

      CHECK_EQUAL(3.1415927L, le_v2);
      CHECK_EQUAL(3.1415927L, be_v2);
      CHECK_EQUAL(3.1415927L, le_v3);
      CHECK_EQUAL(3.1415927L, be_v3);
    }

    //*************************************************************************
    TEST(test_move_construction_long_double)
    {
      storage_ld_t store_le1;
      storage_ld_t store_be1;

      etl::le_long_double_ext_t le_v1(3.1415927L, store_le1.data());
      etl::be_long_double_ext_t be_v1(3.1415927L, store_be1.data());

      etl::le_long_double_ext_t le_v2(etl::move(le_v1)); // Move construct le from le.
      etl::be_long_double_ext_t be_v2(etl::move(be_v1)); // Move construct be from be.

      CHECK_TRUE(nullptr == le_v1.data());
      CHECK_TRUE(nullptr == be_v1.data());
      CHECK_EQUAL(3.1415927L, le_v2);
      CHECK_EQUAL(3.1415927L, be_v2);
    }

    //*************************************************************************
    TEST(test_construction_from_buffer)
    {
      std::array<unsigned char, 4> buffer = { 0x12, 0x34, 0x56, 0x78 };

      const uint32_t le_value = 0x78563412;
      const uint32_t be_value = 0x12345678;

      etl::le_uint32_ext_t le_v1(buffer.data());
      etl::be_uint32_ext_t be_v1(buffer.data());

      CHECK_EQUAL(le_value, le_v1);
      CHECK_EQUAL(be_value, be_v1);
    }

    //*************************************************************************
    TEST(test_set_storage_buffers)
    {
      // Define storeage buffers.
      storage_i32_t store_le1;
      storage_i32_t store_le2;

      storage_i32_t store_be1;
      storage_i32_t store_be2;

      // Fill each with data.
      const uint32_t value1 = 0x01234567;
      const uint32_t value2 = 0x12345678;
      
      etl::le_uint32_ext_t le_v1(value1, store_le1.data());
      etl::le_uint32_ext_t le_v2(value2, store_le2.data());

      etl::be_uint32_ext_t be_v1(value1, store_be1.data());
      etl::be_uint32_ext_t be_v2(value2, store_be2.data());

      // Check each little endian storage.
      etl::le_uint32_ext_t le_v(nullptr);
      
      le_v.set_storage(store_le1.data());
      CHECK_EQUAL(value1, le_v);
      CHECK_EQUAL(store_le1.data(), le_v.data());
      
      le_v.set_storage(store_le2.data());
      CHECK_EQUAL(value2, le_v);
      CHECK_EQUAL(store_le2.data(), le_v.data());

      // Check each big endian storage.
      etl::be_uint32_ext_t be_v(nullptr);

      be_v.set_storage(store_be1.data());
      CHECK_EQUAL(value1, be_v);
      CHECK_EQUAL(store_be1.data(), be_v.data());

      be_v.set_storage(store_be2.data());
      CHECK_EQUAL(value2, be_v);
      CHECK_EQUAL(store_be2.data(), be_v.data());
    }

    //*************************************************************************
    TEST(test_endianness)
    {
      CHECK_EQUAL(etl::endian::big,    etl::be_int16_ext_t::Endian);
      CHECK_EQUAL(etl::endian::little, etl::le_int16_ext_t::Endian);
    }

    //*************************************************************************
    TEST(test_le_sizes)
    {
      CHECK_EQUAL(sizeof(char),               etl::le_char_ext_t::Size);
      CHECK_EQUAL(sizeof(signed char),        etl::le_schar_ext_t::Size);
      CHECK_EQUAL(sizeof(unsigned char),      etl::le_uchar_ext_t::Size);
      CHECK_EQUAL(sizeof(short),              etl::le_short_ext_t::Size);
      CHECK_EQUAL(sizeof(unsigned short),     etl::le_ushort_ext_t::Size);
      CHECK_EQUAL(sizeof(int),                etl::le_int_ext_t::Size);
      CHECK_EQUAL(sizeof(unsigned int),       etl::le_uint_ext_t::Size);
      CHECK_EQUAL(sizeof(long),               etl::le_long_ext_t::Size);
      CHECK_EQUAL(sizeof(unsigned long),      etl::le_ulong_ext_t::Size);
      CHECK_EQUAL(sizeof(long long),          etl::le_long_long_ext_t::Size);
      CHECK_EQUAL(sizeof(unsigned long long), etl::le_ulong_long_ext_t::Size);
      CHECK_EQUAL(sizeof(float),              etl::le_float_ext_t::Size);
      CHECK_EQUAL(sizeof(double),             etl::le_double_ext_t::Size);
      CHECK_EQUAL(sizeof(long double),        etl::le_long_double_ext_t::Size);

      CHECK_EQUAL(sizeof(int8_t),   etl::le_int8_ext_t::Size);
      CHECK_EQUAL(sizeof(uint8_t),  etl::le_uint8_ext_t::Size);
      CHECK_EQUAL(sizeof(int16_t),  etl::le_int16_ext_t::Size);
      CHECK_EQUAL(sizeof(uint16_t), etl::le_uint16_ext_t::Size);
      CHECK_EQUAL(sizeof(int32_t),  etl::le_int32_ext_t::Size);
      CHECK_EQUAL(sizeof(uint32_t), etl::le_uint32_ext_t::Size);
      CHECK_EQUAL(sizeof(int64_t),  etl::le_int64_ext_t::Size);
      CHECK_EQUAL(sizeof(uint64_t), etl::le_uint64_ext_t::Size);
    }

    //*************************************************************************
    TEST(test_be_sizes)
    {
      CHECK_EQUAL(sizeof(char),               etl::be_char_ext_t::Size);
      CHECK_EQUAL(sizeof(signed char),        etl::be_schar_ext_t::Size);
      CHECK_EQUAL(sizeof(unsigned char),      etl::be_uchar_ext_t::Size);
      CHECK_EQUAL(sizeof(short),              etl::be_short_ext_t::Size);
      CHECK_EQUAL(sizeof(unsigned short),     etl::be_ushort_ext_t::Size);
      CHECK_EQUAL(sizeof(int),                etl::be_int_ext_t::Size);
      CHECK_EQUAL(sizeof(unsigned int),       etl::be_uint_ext_t::Size);
      CHECK_EQUAL(sizeof(long),               etl::be_long_ext_t::Size);
      CHECK_EQUAL(sizeof(unsigned long),      etl::be_ulong_ext_t::Size);
      CHECK_EQUAL(sizeof(long long),          etl::be_long_long_ext_t::Size);
      CHECK_EQUAL(sizeof(unsigned long long), etl::be_ulong_long_ext_t::Size);
      CHECK_EQUAL(sizeof(float),              etl::be_float_ext_t::Size);
      CHECK_EQUAL(sizeof(double),             etl::be_double_ext_t::Size);
      CHECK_EQUAL(sizeof(long double),        etl::be_long_double_ext_t::Size);

      CHECK_EQUAL(sizeof(int8_t),   etl::be_int8_ext_t::Size);
      CHECK_EQUAL(sizeof(uint8_t),  etl::be_uint8_ext_t::Size);
      CHECK_EQUAL(sizeof(int16_t),  etl::be_int16_ext_t::Size);
      CHECK_EQUAL(sizeof(uint16_t), etl::be_uint16_ext_t::Size);
      CHECK_EQUAL(sizeof(int32_t),  etl::be_int32_ext_t::Size);
      CHECK_EQUAL(sizeof(uint32_t), etl::be_uint32_ext_t::Size);
      CHECK_EQUAL(sizeof(int64_t),  etl::be_int64_ext_t::Size);
      CHECK_EQUAL(sizeof(uint64_t), etl::be_uint64_ext_t::Size);
    }

    //*************************************************************************
    TEST(test_net_sizes)
    {
      CHECK_EQUAL(sizeof(char),               etl::net_char_ext_t::Size);
      CHECK_EQUAL(sizeof(signed char),        etl::net_schar_ext_t::Size);
      CHECK_EQUAL(sizeof(unsigned char),      etl::net_uchar_ext_t::Size);
      CHECK_EQUAL(sizeof(short),              etl::net_short_ext_t::Size);
      CHECK_EQUAL(sizeof(unsigned short),     etl::net_ushort_ext_t::Size);
      CHECK_EQUAL(sizeof(int),                etl::net_int_ext_t::Size);
      CHECK_EQUAL(sizeof(unsigned int),       etl::net_uint_ext_t::Size);
      CHECK_EQUAL(sizeof(long),               etl::net_long_ext_t::Size);
      CHECK_EQUAL(sizeof(unsigned long),      etl::net_ulong_ext_t::Size);
      CHECK_EQUAL(sizeof(long long),          etl::net_long_long_ext_t::Size);
      CHECK_EQUAL(sizeof(unsigned long long), etl::net_ulong_long_ext_t::Size);
      CHECK_EQUAL(sizeof(float),              etl::net_float_ext_t::Size);
      CHECK_EQUAL(sizeof(double),             etl::net_double_ext_t::Size);
      CHECK_EQUAL(sizeof(long double),        etl::net_long_double_ext_t::Size);

      CHECK_EQUAL(sizeof(int8_t),   etl::net_int8_ext_t::Size);
      CHECK_EQUAL(sizeof(uint8_t),  etl::net_uint8_ext_t::Size);
      CHECK_EQUAL(sizeof(int16_t),  etl::net_int16_ext_t::Size);
      CHECK_EQUAL(sizeof(uint16_t), etl::net_uint16_ext_t::Size);
      CHECK_EQUAL(sizeof(int32_t),  etl::net_int32_ext_t::Size);
      CHECK_EQUAL(sizeof(uint32_t), etl::net_uint32_ext_t::Size);
      CHECK_EQUAL(sizeof(int64_t),  etl::net_int64_ext_t::Size);
      CHECK_EQUAL(sizeof(uint64_t), etl::net_uint64_ext_t::Size);
    }

#if ETL_HAS_CONSTEXPR_ENDIANNESS
    //*************************************************************************
    TEST(test_host_sizes)
    {
      CHECK_EQUAL(sizeof(char),               etl::host_char_ext_t::Size);
      CHECK_EQUAL(sizeof(signed char),        etl::host_schar_ext_t::Size);
      CHECK_EQUAL(sizeof(unsigned char),      etl::host_uchar_ext_t::Size);
      CHECK_EQUAL(sizeof(short),              etl::host_short_ext_t::Size);
      CHECK_EQUAL(sizeof(unsigned short),     etl::host_ushort_ext_t::Size);
      CHECK_EQUAL(sizeof(int),                etl::host_int_ext_t::Size);
      CHECK_EQUAL(sizeof(unsigned int),       etl::host_uint_ext_t::Size);
      CHECK_EQUAL(sizeof(long),               etl::host_long_ext_t::Size);
      CHECK_EQUAL(sizeof(unsigned long),      etl::host_ulong_ext_t::Size);
      CHECK_EQUAL(sizeof(long long),          etl::host_long_long_ext_t::Size);
      CHECK_EQUAL(sizeof(unsigned long long), etl::host_ulong_long_ext_t::Size);
      CHECK_EQUAL(sizeof(float),              etl::host_float_ext_t::Size);
      CHECK_EQUAL(sizeof(double),             etl::host_double_ext_t::Size);
      CHECK_EQUAL(sizeof(long double),        etl::host_long_double_ext_t::Size);

      CHECK_EQUAL(sizeof(int8_t),   etl::host_int8_ext_t::Size);
      CHECK_EQUAL(sizeof(uint8_t),  etl::host_uint8_t::Size);
      CHECK_EQUAL(sizeof(int16_t),  etl::host_int16_ext_t::Size);
      CHECK_EQUAL(sizeof(uint16_t), etl::host_uint16_ext_t::Size);
      CHECK_EQUAL(sizeof(int32_t),  etl::host_int32_ext_t::Size);
      CHECK_EQUAL(sizeof(uint32_t), etl::host_uint32_ext_t::Size);
      CHECK_EQUAL(sizeof(int64_t),  etl::host_int64_ext_t::Size);
      CHECK_EQUAL(sizeof(uint64_t), etl::host_uint64_ext_t::Size);
    }
#endif

    //*************************************************************************
    TEST(test_le_equality)
    {
      // char
      storage_c_t store_c1;
      storage_c_t store_c2;
      storage_c_t store_c3;
      storage_c_t store_c4;

      CHECK(etl::le_char_ext_t(0x01, store_c1.data()) == etl::le_char_ext_t(0x01, store_c2.data()));
      CHECK(char(0x01)                                   == etl::le_char_ext_t(0x01, store_c3.data()));
      CHECK(etl::le_char_ext_t(0x01, store_c4.data()) == char(0x01));

      CHECK(etl::le_schar_ext_t(0x01, store_c1.data()) == etl::le_schar_ext_t(0x01, store_c2.data()));
      CHECK((signed char)(0x01)                           == etl::le_schar_ext_t(0x01, store_c3.data()));
      CHECK(etl::le_schar_ext_t(0x01, store_c4.data()) == (signed char)(0x01));

      CHECK(etl::le_uchar_ext_t(0x01U, store_c1.data()) == etl::le_uchar_ext_t(0x01U, store_c2.data()));
      CHECK(0x01U                                          == etl::le_uchar_ext_t(0x01U, store_c3.data()));
      CHECK(etl::le_uchar_ext_t(0x01U, store_c4.data()) == 0x01U);

      // short
      storage_s_t store_s1;
      storage_s_t store_s2;
      storage_s_t store_s3;
      storage_s_t store_s4;

      CHECK(etl::le_short_ext_t(0x0123, store_s1.data()) == etl::le_short_ext_t(0x0123, store_s2.data()));
      CHECK(short(0x0123)                                   == etl::le_short_ext_t(0x0123, store_s3.data()));
      CHECK(etl::le_short_ext_t(0x0123, store_s4.data()) == short(0x0123));

      CHECK(etl::le_ushort_ext_t(0x0123, store_s1.data()) == etl::le_ushort_ext_t(0x0123, store_s2.data()));
      CHECK((unsigned short)(0x0123)                         == etl::le_ushort_ext_t(0x0123, store_s3.data()));
      CHECK(etl::le_ushort_ext_t(0x0123, store_s4.data()) == (unsigned short)(0x0123));

      // int
      storage_i_t store_i1;
      storage_i_t store_i2;
      storage_i_t store_i3;
      storage_i_t store_i4;

      CHECK(etl::le_int_ext_t(0x01234567, store_i1.data()) == etl::le_int_ext_t(0x01234567, store_i2.data()));
      CHECK(int(0x01234567)                                   == etl::le_int_ext_t(0x01234567, store_i3.data()));
      CHECK(etl::le_int_ext_t(0x01234567, store_i4.data()) == int(0x01234567));

      CHECK(etl::le_uint_ext_t(0x01234567U, store_i1.data()) == etl::le_uint_ext_t(0x01234567U, store_i2.data()));
      CHECK((unsigned int)(0x01234567U)                         == etl::le_uint_ext_t(0x01234567U, store_i3.data()));
      CHECK(etl::le_uint_ext_t(0x01234567U, store_i4.data()) == (unsigned int)(0x01234567U));

      if (sizeof(long) == 4U)
      {
        // long
        storage_l_t store_l1;
        storage_l_t store_l2;
        storage_l_t store_l3;
        storage_l_t store_l4;

        CHECK(etl::le_long_ext_t(0x01234567L, store_l1.data()) == etl::le_long_ext_t(0x01234567L, store_l2.data()));
        CHECK(long(0x01234567L)                                   == etl::le_long_ext_t(0x01234567L, store_l3.data()));
        CHECK(etl::le_long_ext_t(0x01234567L, store_l4.data()) == long(0x01234567L));

        CHECK(etl::le_ulong_ext_t(0x01234567UL, store_l1.data()) == etl::le_ulong_ext_t(0x01234567UL, store_l2.data()));
        CHECK((unsigned long)(0x01234567UL)                         == etl::le_ulong_ext_t(0x01234567UL, store_l3.data()));
        CHECK(etl::le_ulong_ext_t(0x01234567UL, store_l4.data()) == (unsigned long)(0x01234567UL));
      }

      if (sizeof(long long) == 8U)
      {
        // long long
        storage_ll_t store_ll1;
        storage_ll_t store_ll2;
        storage_ll_t store_ll3;
        storage_ll_t store_ll4;

        CHECK(etl::le_long_long_ext_t(0x0123456789ABCDEF, store_ll1.data()) == etl::le_long_long_ext_t(0x0123456789ABCDEF, store_ll2.data()));
        CHECK((long long)(0x0123456789ABCDEF)                                  == etl::le_long_long_ext_t(0x0123456789ABCDEF, store_ll3.data()));
        CHECK(etl::le_long_long_ext_t(0x0123456789ABCDEF, store_ll4.data()) == (long long)(0x0123456789ABCDEF));

        CHECK(etl::le_ulong_long_ext_t(0x0123456789ABCDEFU, store_ll1.data()) == etl::le_ulong_long_ext_t(0x0123456789ABCDEFU, store_ll2.data()));
        CHECK((unsigned long long)(0x0123456789ABCDEFU)                          == etl::le_ulong_long_ext_t(0x0123456789ABCDEFU, store_ll3.data()));
        CHECK(etl::le_ulong_long_ext_t(0x0123456789ABCDEFU, store_ll4.data()) == (unsigned long long)(0x0123456789ABCDEFU));
      }

      // float
      storage_f_t store_f1;
      storage_f_t store_f2;
      storage_f_t store_f3;
      storage_f_t store_f4;

      CHECK_CLOSE(etl::le_float_ext_t(3.1415927f, store_f1.data()), etl::le_float_ext_t(3.1415927f, store_f2.data()), 0.001);
      CHECK_CLOSE(3.1415927f, etl::le_float_ext_t(3.1415927f, store_f3.data()), 0.001);
      CHECK_CLOSE(etl::le_float_ext_t(3.1415927f, store_f4.data()), 3.1415927f, 0.001);

      // double
      storage_d_t store_d1;
      storage_d_t store_d2;
      storage_d_t store_d3;
      storage_d_t store_d4;

      CHECK_CLOSE(etl::le_double_ext_t(3.1415927, store_d1.data()), etl::le_double_ext_t(3.1415927, store_d2.data()), 0.001);
      CHECK_CLOSE(3.1415927, etl::le_double_ext_t(3.1415927, store_d3.data()), 0.001);
      CHECK_CLOSE(etl::le_double_ext_t(3.1415927, store_d4.data()), 3.1415927, 0.001);

      // long double
      storage_ld_t store_ld1;
      storage_ld_t store_ld2;
      storage_ld_t store_ld3;
      storage_ld_t store_ld4;

      CHECK_CLOSE(etl::le_long_double_ext_t(3.1415927L, store_ld1.data()).value(), etl::le_long_double_ext_t(3.1415927L, store_ld2.data()).value(), 0.001);
      CHECK_CLOSE(3.1415927L, etl::le_long_double_ext_t(3.1415927L, store_ld3.data()).value(), 0.001);
      CHECK_CLOSE(etl::le_long_double_ext_t(3.1415927L, store_ld4.data()).value(), 3.1415927L, 0.001);
    }

    //*************************************************************************
    TEST(test_le_inequality)
    {
      // char
      storage_c_t store_c1;
      storage_c_t store_c2;
      storage_c_t store_c3;
      storage_c_t store_c4;

      CHECK(etl::le_char_ext_t(0x01, store_c1.data()) != etl::le_char_ext_t(0x02, store_c2.data()));
      CHECK(char(0x01)                                   != etl::le_char_ext_t(0x02, store_c3.data()));
      CHECK(etl::le_char_ext_t(0x01, store_c4.data()) != char(0x02));

      CHECK(etl::le_schar_ext_t(0x01, store_c1.data()) != etl::le_schar_ext_t(0x02, store_c2.data()));
      CHECK((signed char)(0x01)                           != etl::le_schar_ext_t(0x02, store_c3.data()));
      CHECK(etl::le_schar_ext_t(0x01, store_c4.data()) != (signed char)(0x02));

      CHECK(etl::le_uchar_ext_t(0x01U, store_c1.data()) != etl::le_uchar_ext_t(0x02U, store_c2.data()));
      CHECK(0x01U                                          != etl::le_uchar_ext_t(0x02U, store_c3.data()));
      CHECK(etl::le_uchar_ext_t(0x01U, store_c4.data()) != 0x02U);

      // short
      storage_s_t store_s1;
      storage_s_t store_s2;
      storage_s_t store_s3;
      storage_s_t store_s4;

      CHECK(etl::le_short_ext_t(0x0123, store_s1.data()) != etl::le_short_ext_t(0x0223, store_s2.data()));
      CHECK(short(0x0123)                                   != etl::le_short_ext_t(0x0223, store_s3.data()));
      CHECK(etl::le_short_ext_t(0x0123, store_s4.data()) != short(0x0223));

      CHECK(etl::le_ushort_ext_t(0x0123, store_s1.data()) != etl::le_ushort_ext_t(0x0223, store_s2.data()));
      CHECK((unsigned short)(0x0123)                         != etl::le_ushort_ext_t(0x0223, store_s3.data()));
      CHECK(etl::le_ushort_ext_t(0x0123, store_s4.data()) != (unsigned short)(0x0223));

      // int
      storage_i_t store_i1;
      storage_i_t store_i2;
      storage_i_t store_i3;
      storage_i_t store_i4;

      CHECK(etl::le_int_ext_t(0x01234567, store_i1.data()) != etl::le_int_ext_t(0x02234567, store_i2.data()));
      CHECK(int(0x01234567)                                   != etl::le_int_ext_t(0x02234567, store_i3.data()));
      CHECK(etl::le_int_ext_t(0x01234567, store_i4.data()) != int(0x02234567));

      CHECK(etl::le_uint_ext_t(0x01234567U, store_i1.data()) != etl::le_uint_ext_t(0x02234567U, store_i2.data()));
      CHECK((unsigned int)(0x01234567U)                         != etl::le_uint_ext_t(0x02234567U, store_i3.data()));
      CHECK(etl::le_uint_ext_t(0x01234567U, store_i4.data()) != (unsigned int)(0x02234567U));

      if (sizeof(long) != 4U)
      {
        // long
        storage_l_t store_l1;
        storage_l_t store_l2;
        storage_l_t store_l3;
        storage_l_t store_l4;

        CHECK(etl::le_long_ext_t(0x01234567L, store_l1.data()) != etl::le_long_ext_t(0x02234567L, store_l2.data()));
        CHECK(long(0x01234567L)                                   != etl::le_long_ext_t(0x02234567L, store_l3.data()));
        CHECK(etl::le_long_ext_t(0x01234567L, store_l4.data()) != long(0x02234567L));

        CHECK(etl::le_ulong_ext_t(0x01234567UL, store_l1.data()) != etl::le_ulong_ext_t(0x02234567UL, store_l2.data()));
        CHECK((unsigned long)(0x01234567UL)                         != etl::le_ulong_ext_t(0x02234567UL, store_l3.data()));
        CHECK(etl::le_ulong_ext_t(0x01234567UL, store_l4.data()) != (unsigned long)(0x02234567UL));
      }

      if (sizeof(long long) == 8U)
      {
        // long long
        storage_ll_t store_ll1;
        storage_ll_t store_ll2;
        storage_ll_t store_ll3;
        storage_ll_t store_ll4;

        CHECK(etl::le_long_long_ext_t(0x0123456789ABCDEF, store_ll1.data()) != etl::le_long_long_ext_t(0x0223456789ABCDEF, store_ll2.data()));
        CHECK((long long)(0x0123456789ABCDEF)                                  != etl::le_long_long_ext_t(0x0223456789ABCDEF, store_ll3.data()));
        CHECK(etl::le_long_long_ext_t(0x0123456789ABCDEF, store_ll4.data()) != (long long)(0x0223456789ABCDEF));

        CHECK(etl::le_ulong_long_ext_t(0x0123456789ABCDEFU, store_ll1.data()) != etl::le_ulong_long_ext_t(0x0223456789ABCDEFU, store_ll2.data()));
        CHECK((unsigned long long)(0x0123456789ABCDEFU)                          != etl::le_ulong_long_ext_t(0x0223456789ABCDEFU, store_ll3.data()));
        CHECK(etl::le_ulong_long_ext_t(0x0123456789ABCDEFU, store_ll4.data()) != (unsigned long long)(0x0223456789ABCDEFU));
      }

      // float
      storage_f_t store_f1;
      storage_f_t store_f2;
      storage_f_t store_f3;
      storage_f_t store_f4;

      CHECK_FLOAT_DIFFERENT(etl::le_float_ext_t(3.1415927f, store_f1.data()), etl::le_float_ext_t(2.1415927f, store_f2.data()));
      CHECK_FLOAT_DIFFERENT(3.1415927f, etl::le_float_ext_t(2.1415927f, store_f3.data()));
      CHECK_FLOAT_DIFFERENT(etl::le_float_ext_t(3.1415927f, store_f4.data()), 2.1415927f);

      // double
      storage_d_t store_d1;
      storage_d_t store_d2;
      storage_d_t store_d3;
      storage_d_t store_d4;

      CHECK_FLOAT_DIFFERENT(etl::le_double_ext_t(3.1415927, store_d1.data()), etl::le_double_ext_t(2.1415927, store_d2.data()));
      CHECK_FLOAT_DIFFERENT(3.1415927, etl::le_double_ext_t(2.1415927, store_d3.data()));
      CHECK_FLOAT_DIFFERENT(etl::le_double_ext_t(3.1415927, store_d4.data()), 2.1415927);

      // long double
      storage_ld_t store_ld1;
      storage_ld_t store_ld2;
      storage_ld_t store_ld3;
      storage_ld_t store_ld4; 

      CHECK_FLOAT_DIFFERENT(etl::le_long_double_ext_t(3.1415927L, store_ld1.data()), etl::le_long_double_ext_t(2.1415927L, store_ld2.data()));
      CHECK_FLOAT_DIFFERENT(3.1415927L, etl::le_long_double_ext_t(2.1415927L, store_ld3.data()));
      CHECK_FLOAT_DIFFERENT(etl::le_long_double_ext_t(3.1415927L, store_ld4.data()), 2.1415927L);
    }

    //*************************************************************************
    TEST(test_be_equality)
    {
      // char
      storage_c_t store_c_be1;
      storage_c_t store_c_be2;
      storage_c_t store_c_be3;
      storage_c_t store_c_be4;

      CHECK(etl::be_char_ext_t(0x01, store_c_be1.data()) == etl::be_char_ext_t(0x01, store_c_be2.data()));
      CHECK(char(0x01)                                   == etl::be_char_ext_t(0x01, store_c_be3.data()));
      CHECK(etl::be_char_ext_t(0x01, store_c_be4.data()) == char(0x01));

      CHECK(etl::be_schar_ext_t(0x01, store_c_be1.data()) == etl::be_schar_ext_t(0x01, store_c_be2.data()));
      CHECK((signed char)(0x01)                           == etl::be_schar_ext_t(0x01, store_c_be3.data()));
      CHECK(etl::be_schar_ext_t(0x01, store_c_be4.data()) == (signed char)(0x01));

      CHECK(etl::be_uchar_ext_t(0x01U, store_c_be1.data()) == etl::be_uchar_ext_t(0x01U, store_c_be2.data()));
      CHECK(0x01U                                          == etl::be_uchar_ext_t(0x01U, store_c_be3.data()));
      CHECK(etl::be_uchar_ext_t(0x01U, store_c_be4.data()) == 0x01U);

      // short
      storage_s_t store_s_be1;
      storage_s_t store_s_be2;
      storage_s_t store_s_be3;
      storage_s_t store_s_be4;

      CHECK(etl::be_short_ext_t(0x0123, store_s_be1.data()) == etl::be_short_ext_t(0x0123, store_s_be2.data()));
      CHECK(short(0x0123)                                   == etl::be_short_ext_t(0x0123, store_s_be3.data()));
      CHECK(etl::be_short_ext_t(0x0123, store_s_be4.data()) == short(0x0123));

      CHECK(etl::be_ushort_ext_t(0x0123, store_s_be1.data()) == etl::be_ushort_ext_t(0x0123, store_s_be2.data()));
      CHECK((unsigned short)(0x0123)                         == etl::be_ushort_ext_t(0x0123, store_s_be3.data()));
      CHECK(etl::be_ushort_ext_t(0x0123, store_s_be4.data()) == (unsigned short)(0x0123));

      // int
      storage_i_t store_i_be1;
      storage_i_t store_i_be2;
      storage_i_t store_i_be3;
      storage_i_t store_i_be4;

      CHECK(etl::be_int_ext_t(0x01234567, store_i_be1.data()) == etl::be_int_ext_t(0x01234567, store_i_be2.data()));
      CHECK(int(0x01234567)                                   == etl::be_int_ext_t(0x01234567, store_i_be3.data()));
      CHECK(etl::be_int_ext_t(0x01234567, store_i_be4.data()) == int(0x01234567));

      CHECK(etl::be_uint_ext_t(0x01234567U, store_i_be1.data()) == etl::be_uint_ext_t(0x01234567U, store_i_be2.data()));
      CHECK((unsigned int)(0x01234567U)                         == etl::be_uint_ext_t(0x01234567U, store_i_be3.data()));
      CHECK(etl::be_uint_ext_t(0x01234567U, store_i_be4.data()) == (unsigned int)(0x01234567U));

      if (sizeof(long) == 4U)
      {
        // long
        storage_l_t store_l_be1;
        storage_l_t store_l_be2;
        storage_l_t store_l_be3;
        storage_l_t store_l_be4;

        CHECK(etl::be_long_ext_t(0x01234567L, store_l_be1.data()) == etl::be_long_ext_t(0x01234567L, store_l_be2.data()));
        CHECK(long(0x01234567L)                                   == etl::be_long_ext_t(0x01234567L, store_l_be3.data()));
        CHECK(etl::be_long_ext_t(0x01234567L, store_l_be4.data()) == long(0x01234567L));

        CHECK(etl::be_ulong_ext_t(0x01234567UL, store_l_be1.data()) == etl::be_ulong_ext_t(0x01234567UL, store_l_be2.data()));
        CHECK((unsigned long)(0x01234567UL)                         == etl::be_ulong_ext_t(0x01234567UL, store_l_be3.data()));
        CHECK(etl::be_ulong_ext_t(0x01234567UL, store_l_be4.data()) == (unsigned long)(0x01234567UL));
      }

      if (sizeof(long long) == 8U)
      {
        // long long
        storage_ll_t store_ll_be1;
        storage_ll_t store_ll_be2;
        storage_ll_t store_ll_be3;
        storage_ll_t store_ll_be4;

        CHECK(etl::be_long_long_ext_t(0x0123456789ABCDEF, store_ll_be1.data()) == etl::be_long_long_ext_t(0x0123456789ABCDEF, store_ll_be2.data()));
        CHECK((long long)(0x0123456789ABCDEF)                                  == etl::be_long_long_ext_t(0x0123456789ABCDEF, store_ll_be3.data()));
        CHECK(etl::be_long_long_ext_t(0x0123456789ABCDEF, store_ll_be4.data()) == (long long)(0x0123456789ABCDEF));

        CHECK(etl::be_ulong_long_ext_t(0x0123456789ABCDEFU, store_ll_be1.data()) == etl::be_ulong_long_ext_t(0x0123456789ABCDEFU, store_ll_be2.data()));
        CHECK((unsigned long long)(0x0123456789ABCDEFU)                          == etl::be_ulong_long_ext_t(0x0123456789ABCDEFU, store_ll_be3.data()));
        CHECK(etl::be_ulong_long_ext_t(0x0123456789ABCDEFU, store_ll_be4.data()) == (unsigned long long)(0x0123456789ABCDEFU));
      }

      // float
      storage_f_t store_f_be1;
      storage_f_t store_f_be2;
      storage_f_t store_f_be3;
      storage_f_t store_f_be4;

      CHECK_CLOSE(etl::be_float_ext_t(3.1415927f, store_f_be1.data()), etl::be_float_ext_t(3.1415927f, store_f_be2.data()), 0.001);
      CHECK_CLOSE(3.1415927f, etl::be_float_ext_t(3.1415927f, store_f_be3.data()), 0.001);
      CHECK_CLOSE(etl::be_float_ext_t(3.1415927f, store_f_be4.data()), 3.1415927f, 0.001);

      // double
      storage_d_t store_d_be1;
      storage_d_t store_d_be2;
      storage_d_t store_d_be3;
      storage_d_t store_d_be4;

      CHECK_CLOSE(etl::be_double_ext_t(3.1415927, store_d_be1.data()), etl::be_double_ext_t(3.1415927, store_d_be2.data()), 0.001);
      CHECK_CLOSE(3.1415927, etl::be_double_ext_t(3.1415927, store_d_be3.data()), 0.001);
      CHECK_CLOSE(etl::be_double_ext_t(3.1415927, store_d_be4.data()), 3.1415927, 0.001);

      // long double
      storage_ld_t store_ld_be1;
      storage_ld_t store_ld_be2;
      storage_ld_t store_ld_be3;
      storage_ld_t store_ld_be4;

      CHECK_CLOSE(etl::be_long_double_ext_t(3.1415927L, store_ld_be1.data()).value(), etl::be_long_double_ext_t(3.1415927L, store_ld_be2.data()).value(), 0.001);
      CHECK_CLOSE(3.1415927L, etl::be_long_double_ext_t(3.1415927L, store_ld_be3.data()).value(), 0.001);
      CHECK_CLOSE(etl::be_long_double_ext_t(3.1415927L, store_ld_be4.data()).value(), 3.1415927L, 0.001);
    }

    //*************************************************************************
    TEST(test_be_inequality)
    {
      // char
      storage_c_t store_c_be1;
      storage_c_t store_c_be2;
      storage_c_t store_c_be3;
      storage_c_t store_c_be4;

      CHECK(etl::be_char_ext_t(0x01, store_c_be1.data()) != etl::be_char_ext_t(0x02, store_c_be2.data()));
      CHECK(char(0x01)                                   != etl::be_char_ext_t(0x02, store_c_be3.data()));
      CHECK(etl::be_char_ext_t(0x01, store_c_be4.data()) != char(0x02));

      CHECK(etl::be_schar_ext_t(0x01, store_c_be1.data()) != etl::be_schar_ext_t(0x02, store_c_be2.data()));
      CHECK((signed char)(0x01)                           != etl::be_schar_ext_t(0x02, store_c_be3.data()));
      CHECK(etl::be_schar_ext_t(0x01, store_c_be4.data()) != (signed char)(0x02));

      CHECK(etl::be_uchar_ext_t(0x01U, store_c_be1.data()) != etl::be_uchar_ext_t(0x02U, store_c_be2.data()));
      CHECK(0x01U                                          != etl::be_uchar_ext_t(0x02U, store_c_be3.data()));
      CHECK(etl::be_uchar_ext_t(0x01U, store_c_be4.data()) != 0x02U);

      // short
      storage_s_t store_s_be1;
      storage_s_t store_s_be2;
      storage_s_t store_s_be3;
      storage_s_t store_s_be4;

      CHECK(etl::be_short_ext_t(0x0123, store_s_be1.data()) != etl::be_short_ext_t(0x0223, store_s_be2.data()));
      CHECK(short(0x0123)                                   != etl::be_short_ext_t(0x0223, store_s_be3.data()));
      CHECK(etl::be_short_ext_t(0x0123, store_s_be4.data()) != short(0x0223));

      CHECK(etl::be_ushort_ext_t(0x0123, store_s_be1.data()) != etl::be_ushort_ext_t(0x0223, store_s_be2.data()));
      CHECK((unsigned short)(0x0123)                         != etl::be_ushort_ext_t(0x0223, store_s_be3.data()));
      CHECK(etl::be_ushort_ext_t(0x0123, store_s_be4.data()) != (unsigned short)(0x0223));

      // int
      storage_i_t store_i_be1;
      storage_i_t store_i_be2;
      storage_i_t store_i_be3;
      storage_i_t store_i_be4;

      CHECK(etl::be_int_ext_t(0x01234567, store_i_be1.data()) != etl::be_int_ext_t(0x02234567, store_i_be2.data()));
      CHECK(int(0x01234567)                                   != etl::be_int_ext_t(0x02234567, store_i_be3.data()));
      CHECK(etl::be_int_ext_t(0x01234567, store_i_be4.data()) != int(0x02234567));

      CHECK(etl::be_uint_ext_t(0x01234567U, store_i_be1.data()) != etl::be_uint_ext_t(0x02234567U, store_i_be2.data()));
      CHECK((unsigned int)(0x01234567U)                         != etl::be_uint_ext_t(0x02234567U, store_i_be3.data()));
      CHECK(etl::be_uint_ext_t(0x01234567U, store_i_be4.data()) != (unsigned int)(0x02234567U));

      if (sizeof(long) != 4U)
      {
        // long
        storage_l_t store_l_be1;
        storage_l_t store_l_be2;
        storage_l_t store_l_be3;
        storage_l_t store_l_be4;

        CHECK(etl::be_long_ext_t(0x01234567L, store_l_be1.data()) != etl::be_long_ext_t(0x02234567L, store_l_be2.data()));
        CHECK(long(0x01234567L)                                   != etl::be_long_ext_t(0x02234567L, store_l_be3.data()));
        CHECK(etl::be_long_ext_t(0x01234567L, store_l_be4.data()) != long(0x02234567L));

        CHECK(etl::be_ulong_ext_t(0x01234567UL, store_l_be1.data()) != etl::be_ulong_ext_t(0x02234567UL, store_l_be2.data()));
        CHECK((unsigned long)(0x01234567UL)                         != etl::be_ulong_ext_t(0x02234567UL, store_l_be3.data()));
        CHECK(etl::be_ulong_ext_t(0x01234567UL, store_l_be4.data()) != (unsigned long)(0x02234567UL));
      }

      if (sizeof(long long) == 8U)
      {
        // long long
        storage_ll_t store_ll_be1;
        storage_ll_t store_ll_be2;
        storage_ll_t store_ll_be3;
        storage_ll_t store_ll_be4;

        CHECK(etl::be_long_long_ext_t(0x0123456789ABCDEF, store_ll_be1.data()) != etl::be_long_long_ext_t(0x0223456789ABCDEF, store_ll_be2.data()));
        CHECK((long long)(0x0123456789ABCDEF)                                  != etl::be_long_long_ext_t(0x0223456789ABCDEF, store_ll_be3.data()));
        CHECK(etl::be_long_long_ext_t(0x0123456789ABCDEF, store_ll_be4.data()) != (long long)(0x0223456789ABCDEF));

        CHECK(etl::be_ulong_long_ext_t(0x0123456789ABCDEFU, store_ll_be1.data()) != etl::be_ulong_long_ext_t(0x0223456789ABCDEFU, store_ll_be2.data()));
        CHECK((unsigned long long)(0x0123456789ABCDEFU)                          != etl::be_ulong_long_ext_t(0x0223456789ABCDEFU, store_ll_be3.data()));
        CHECK(etl::be_ulong_long_ext_t(0x0123456789ABCDEFU, store_ll_be4.data()) != (unsigned long long)(0x0223456789ABCDEFU));
      }

      // float
      storage_f_t store_f_be1;
      storage_f_t store_f_be2;
      storage_f_t store_f_be3;
      storage_f_t store_f_be4;

      CHECK_FLOAT_DIFFERENT(etl::be_float_ext_t(3.1415927f, store_f_be1.data()), etl::be_float_ext_t(2.1415927f, store_f_be2.data()));
      CHECK_FLOAT_DIFFERENT(3.1415927f, etl::be_float_ext_t(2.1415927f, store_f_be3.data()));
      CHECK_FLOAT_DIFFERENT(etl::be_float_ext_t(3.1415927f, store_f_be4.data()), 2.1415927f);

      // double
      storage_d_t store_d_be1;
      storage_d_t store_d_be2;
      storage_d_t store_d_be3;
      storage_d_t store_d_be4;

      CHECK_FLOAT_DIFFERENT(etl::be_double_ext_t(3.1415927, store_d_be1.data()), etl::be_double_ext_t(2.1415927, store_d_be2.data()));
      CHECK_FLOAT_DIFFERENT(3.1415927, etl::be_double_ext_t(2.1415927, store_d_be3.data()));
      CHECK_FLOAT_DIFFERENT(etl::be_double_ext_t(3.1415927, store_d_be4.data()), 2.1415927);

      // long double
      storage_ld_t store_ld_be1;
      storage_ld_t store_ld_be2;
      storage_ld_t store_ld_be3;
      storage_ld_t store_ld_be4; 

      CHECK_FLOAT_DIFFERENT(etl::be_long_double_ext_t(3.1415927L, store_ld_be1.data()), etl::be_long_double_ext_t(2.1415927L, store_ld_be2.data()));
      CHECK_FLOAT_DIFFERENT(3.1415927L, etl::be_long_double_ext_t(2.1415927L, store_ld_be3.data()));
      CHECK_FLOAT_DIFFERENT(etl::be_long_double_ext_t(3.1415927L, store_ld_be4.data()), 2.1415927L);
    }

    //*************************************************************************
    TEST(test_le_assignment)
    {
      storage_c_t store_c1;
      storage_c_t store_c2;
      storage_c_t store_c3;

      etl::le_char_ext_t le_char(store_c1.data());
      le_char = 0x01;
      CHECK(0x01 == (char)le_char);

      etl::le_schar_ext_t le_schar(store_c2.data());
      le_schar = 0x01;
      CHECK(0x01 == (signed char)le_schar);

      etl::le_uchar_ext_t le_uchar(store_c3.data());
      le_uchar = 0x01;
      CHECK(0x01 == (unsigned char)le_uchar);

      storage_s_t store_s1;
      storage_s_t store_s2;

      etl::le_short_ext_t le_short(store_s1.data());
      le_short = 0x0123;
      CHECK(0x0123 == (short)le_short);

      etl::le_ushort_ext_t le_ushort(store_s2.data());
      le_ushort = 0x0123;
      CHECK(0x0123 == (unsigned short)le_ushort);

      storage_i_t store_i1;
      storage_i_t store_i2;

      etl::le_int_ext_t le_int(store_i1.data());
      le_int = 0x01234567;
      CHECK(0x01234567 == (int)le_int);

      etl::le_uint_ext_t le_uint(store_i2.data());
      le_uint = 0x01234567;
      CHECK(0x01234567 == (unsigned int)le_uint);

      storage_l_t store_l1;
      storage_l_t store_l2;

      if (sizeof(long) == 4U)
      {
        etl::le_long_ext_t le_long(store_l1.data());
        le_long = 0x01234567;
        CHECK(0x01234567 == (long)le_long);

        etl::le_ulong_ext_t le_ulong(store_l2.data());
        le_ulong = 0x01234567;
        CHECK(0x01234567 == (unsigned long)le_ulong);
      }

      if (sizeof(long long) == 8U)
      {
        storage_ll_t store_ll1;
        storage_ll_t store_ll2;

        etl::le_long_long_ext_t le_long(store_ll1.data());
        le_long = 0x0123456789ABCDEF;
        CHECK(0x0123456789ABCDEF == (long long)le_long);

        etl::le_ulong_long_ext_t le_ulong(store_ll2.data());
        le_ulong = 0x0123456789ABCDEF;
        CHECK(0x0123456789ABCDEF == (unsigned long long)le_ulong);
      }

      // float
      storage_f_t store_f;

      etl::le_float_ext_t le_float(store_f.data());
      le_float = 3.1415927f;
      CHECK_CLOSE(3.1415927f, le_float, 0.001);

      // double
      storage_d_t store_d;

      etl::le_double_ext_t le_double(store_d.data());
      le_double = 3.1415927;
      CHECK_CLOSE(3.1415927, le_double, 0.001);

      // long double
      storage_ld_t store_ld;

      etl::le_long_double_ext_t le_long_double(store_ld.data());
      le_long_double = 3.1415927L;
      CHECK_CLOSE(3.1415927L, le_long_double, 0.001);
    }

    //*************************************************************************
    TEST(test_be_assignment)
    {
      storage_c_t store_c1;
      storage_c_t store_c2;
      storage_c_t store_c3;

      etl::be_char_ext_t be_char(store_c1.data());
      be_char = 0x01;
      CHECK(0x01 == (char)be_char);

      etl::be_schar_ext_t be_schar(store_c2.data());
      be_schar = 0x01;
      CHECK(0x01 == (signed char)be_schar);

      etl::be_uchar_ext_t be_uchar(store_c3.data());
      be_uchar = 0x01;
      CHECK(0x01 == (unsigned char)be_uchar);

      storage_s_t store_s1;
      storage_s_t store_s2;

      etl::be_short_ext_t be_short(store_s1.data());
      be_short = 0x0123;
      CHECK(0x0123 == (short)be_short);

      etl::be_ushort_ext_t be_ushort(store_s2.data());
      be_ushort = 0x0123;
      CHECK(0x0123 == (unsigned short)be_ushort);

      storage_i_t store_i1;
      storage_i_t store_i2;

      etl::be_int_ext_t be_int(store_i1.data());
      be_int = 0x01234567;
      CHECK(0x01234567 == (int)be_int);

      etl::be_uint_ext_t be_uint(store_i2.data());
      be_uint = 0x01234567;
      CHECK(0x01234567 == (unsigned int)be_uint);

      storage_l_t store_l1;
      storage_l_t store_l2;

      if (sizeof(long) == 4U)
      {
        etl::be_long_ext_t be_long(store_l1.data());
        be_long = 0x01234567;
        CHECK(0x01234567 == (long)be_long);

        etl::be_ulong_ext_t be_ulong(store_l2.data());
        be_ulong = 0x01234567;
        CHECK(0x01234567 == (unsigned long)be_ulong);
      }

      if (sizeof(long long) == 8U)
      {
        storage_ll_t store_ll1;
        storage_ll_t store_ll2;

        etl::be_long_long_ext_t be_long(store_ll1.data());
        be_long = 0x0123456789ABCDEF;
        CHECK(0x0123456789ABCDEF == (long long)be_long);

        etl::be_ulong_long_ext_t be_ulong(store_ll2.data());
        be_ulong = 0x0123456789ABCDEF;
        CHECK(0x0123456789ABCDEF == (unsigned long long)be_ulong);
      }

      // float
      storage_f_t store_f;

      etl::be_float_ext_t be_float(store_f.data());
      be_float = 3.1415927f;
      CHECK_CLOSE(3.1415927f, be_float, 0.001);

      // double
      storage_d_t store_d;

      etl::be_double_ext_t be_double(store_d.data());
      be_double = 3.1415927;
      CHECK_CLOSE(3.1415927, be_double, 0.001);

      // long double
      storage_ld_t store_ld;

      etl::be_long_double_ext_t be_long_double(store_ld.data());
      be_long_double = 3.1415927L;
      CHECK_CLOSE(3.1415927L, be_long_double, 0.001);
    }

    //*************************************************************************
    TEST(test_cross_assignment)
    {
      storage_i32_t store_le1;
      storage_i32_t store_be1;
      storage_i32_t store_le2;
      storage_i32_t store_be2;

      etl::le_uint32_ext_t le_v1(0x01234567, store_le1.data());
      etl::be_uint32_ext_t be_v1(0x01234567, store_be1.data());
      etl::le_uint32_ext_t le_v2(store_le2.data());
      etl::be_uint32_ext_t be_v2(store_be2.data());

      le_v2 = be_v1; // Assign le from be.
      be_v2 = le_v1; // Assign be from le.

      CHECK_EQUAL(int(0x01234567), int(le_v2));
      CHECK_EQUAL(int(0x01234567), int(be_v2));
    }

    //*************************************************************************
    TEST(test_cross_move_assignment)
    {
      storage_i32_t store_le1;
      storage_i32_t store_be1;
      storage_i32_t store_le2;
      storage_i32_t store_be2;

      etl::le_uint32_ext_t le_v1(0x01234567, store_le1.data());
      etl::be_uint32_ext_t be_v1(0x01234567, store_be1.data());
      etl::le_uint32_ext_t le_v2(store_le2.data());
      etl::be_uint32_ext_t be_v2(store_be2.data());

      le_v2 = etl::move(be_v1); // Assign le from be.
      be_v2 = etl::move(le_v1); // Assign be from le.

      CHECK_TRUE(nullptr == le_v1.data());
      CHECK_TRUE(nullptr == be_v1.data());
      CHECK_EQUAL(int(0x01234567), int(le_v2));
      CHECK_EQUAL(int(0x01234567), int(be_v2));
    }

    //*************************************************************************
    TEST(test_cross_assignment_float)
    {
      storage_f_t store_le1;
      storage_f_t store_be1;
      storage_f_t store_le2;
      storage_f_t store_be2;
      storage_f_t store_le3;
      storage_f_t store_be3;

      etl::le_float_ext_t le_v1(3.1415927f, store_le1.data());
      etl::be_float_ext_t be_v1(3.1415927f, store_be1.data());
      etl::le_float_ext_t le_v2(store_le2.data());
      etl::be_float_ext_t be_v2(store_be2.data());
      etl::le_float_ext_t le_v3(store_le3.data());
      etl::be_float_ext_t be_v3(store_be3.data());

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
      storage_d_t store_le1;
      storage_d_t store_be1;
      storage_d_t store_le2;
      storage_d_t store_be2;
      storage_d_t store_le3;
      storage_d_t store_be3;

      etl::le_double_ext_t le_v1(3.1415927, store_le1.data());
      etl::be_double_ext_t be_v1(3.1415927, store_be1.data());
      etl::le_double_ext_t le_v2(store_le2.data());
      etl::be_double_ext_t be_v2(store_be2.data());
      etl::le_double_ext_t le_v3(store_le3.data());
      etl::be_double_ext_t be_v3(store_be3.data());

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
      storage_ld_t store_le1;
      storage_ld_t store_be1;
      storage_ld_t store_le2;
      storage_ld_t store_be2;
      storage_ld_t store_le3;
      storage_ld_t store_be3;

      etl::le_long_double_ext_t le_v1(3.1415927L, store_le1.data());
      etl::be_long_double_ext_t be_v1(3.1415927L, store_be1.data());
      etl::le_long_double_ext_t le_v2(store_le2.data());
      etl::be_long_double_ext_t be_v2(store_be2.data());
      etl::le_long_double_ext_t le_v3(store_le3.data());
      etl::be_long_double_ext_t be_v3(store_be3.data());

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
      storage_s_t store_s;

      etl::be_uint16_ext_t test(0x1234, store_s.data());
      
      CHECK_EQUAL(0x12, *test.data());
      CHECK_EQUAL(0x34, *(test.data() + 1U));
    }

    //*************************************************************************
    TEST(test_begin_end_iterator)
    {
      using iterator       = etl::be_uint16_t::iterator;
      using const_iterator = etl::be_uint16_t::const_iterator;

      storage_i16_t store_i1;
      storage_i16_t store_i2;

      etl::be_uint16_ext_t       test(0x1234, store_i1.data());
      const etl::be_uint16_ext_t const_test(0x1234, store_i2.data());

      iterator itr;
      const_iterator citr;

      //*******************************
      itr = test.begin();
      CHECK_EQUAL(int(0x12), int(*itr));
      ++itr;
      CHECK_EQUAL(int(0x34), int(*itr));
      ++itr;
      CHECK(itr == test.end());

      //*******************************
      itr = test.begin();
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

      storage_i16_t store_i1;
      storage_i16_t store_i2;

      etl::be_uint16_ext_t       test(0x1234, store_i1.data());
      const etl::be_uint16_ext_t const_test(0x1234, store_i2.data());

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
      itr = test.rbegin();
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
      storage_i16_t store_i1;
      storage_i16_t store_i2;
      storage_i16_t store_i3;
      storage_i16_t store_i4;

      etl::le_uint16_ext_t       test_le(0x1234, store_i1.data());
      const etl::le_uint16_ext_t const_test_le(0x1234, store_i2.data());

      etl::be_uint16_ext_t       test_be(0x1234, store_i3.data());
      const etl::be_uint16_ext_t const_test_be(0x1234, store_i4.data());

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
      storage_c_t store_c1;
      storage_c_t store_c2;
      storage_c_t store_c3;
      storage_s_t store_s1;
      storage_s_t store_s2;
      storage_i_t store_i1;
      storage_i_t store_i2;
      storage_l_t store_l1;
      storage_l_t store_l2;
      storage_ll_t store_ll1;
      storage_ll_t store_ll2;

      // char
      CHECK(char(0xFE) == etl::le_char_ext_t(char(0xFE), store_c1.data()));
      CHECK((signed char)(0xFE) == etl::le_schar_ext_t((signed char)(0xFE), store_c2.data()));
      CHECK((unsigned char)(0xFEU) == etl::le_uchar_ext_t((unsigned char)(0xFEU), store_c3.data()));

      // short
      CHECK(short(0xFEFD) == etl::le_short_ext_t(short(0xFEFD), store_s1.data()));
      CHECK((unsigned short)(0xFEFD) == etl::le_ushort_ext_t((unsigned short)(0xFEFD), store_s2.data()));

      // int
      CHECK(int(0xFEFDFCFB) == etl::le_int_ext_t(0xFEFDFCFB, store_i1.data()));
      CHECK((unsigned int)(0xFEFDFCFBU) == etl::le_uint_ext_t(0xFEFDFCFBU, store_i2.data()));

      if (sizeof(long) == 4U)
      {
        // long
        CHECK(long(0xFEFDFCFB) == etl::le_long_ext_t(0xFEFDFCFB, store_l1.data()));
        CHECK((unsigned long)(0xFEFDFCFBU) == etl::le_ulong_ext_t(0xFEFDFCFBU, store_l2.data()));
      }

      if (sizeof(long long) == 8U)
      {
        // long long
        CHECK((long long)(0xFEFDFCFBFAF9F8F7) == etl::le_long_long_ext_t(0xFEFDFCFBFAF9F8F7, store_ll1.data()));
        CHECK((unsigned long long)(0xFEFDFCFBFAF9F8F7U) == etl::le_ulong_long_ext_t(0xFEFDFCFBFAF9F8F7U, store_ll2.data()));
      }
    }

    //*************************************************************************
    TEST(test_value)
    {
      storage_i16_t store_i16_1;
      storage_i16_t store_i16_2;

      etl::le_uint16_ext_t test_le(0x1234, store_i16_1.data());
      etl::be_uint16_ext_t test_be(0x1234, store_i16_2.data());

      CHECK_EQUAL(0x1234, test_le.value());
      CHECK_EQUAL(0x1234, test_be.value());
    }

    //*************************************************************************
    TEST(test_storage_bytes)
    {
      storage_i16_t store_i16_1;
      storage_i16_t store_i16_2;

      etl::le_uint16_ext_t test_le(0x1234, store_i16_1.data());
      etl::be_uint16_ext_t test_be(0x1234, store_i16_2.data());

      int lev0 = test_le[0];
      int lev1 = test_le[1];

      int bev0 = test_be[0];
      int bev1 = test_be[1];

      CHECK_EQUAL(0x34, lev0);
      CHECK_EQUAL(0x12, lev1);
      CHECK_EQUAL(0x12, bev0);
      CHECK_EQUAL(0x34, bev1);
    }
  };
}

#include "etl/private/diagnostic_pop.h"
