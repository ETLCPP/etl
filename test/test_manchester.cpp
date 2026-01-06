#include "etl/manchester.h"

#include "unit_test_framework.h"
#include <etl/algorithm.h>
#include <etl/array.h>
#include <etl/span.h>


SUITE(test_manchester)
{
  TEST(encode8){
    CHECK_EQUAL(0xAAAA, (etl::manchester_encode<uint8_t>(0x00U)));
    CHECK_EQUAL(0x5555, (etl::manchester_encode<uint8_t>(0xFFU)));
    CHECK_EQUAL(0xAAA9, (etl::manchester_encode<uint8_t>(0x01U)));
    CHECK_EQUAL(0x6AAA, (etl::manchester_encode<uint8_t>(0x80U)));

#if ETL_USING_CPP14
    static_assert(0xAAAA == etl::manchester_encode<uint8_t>(0x00U));
    static_assert(0x5555 == etl::manchester_encode<uint8_t>(0xFFU));
    static_assert(0xAAA9 == etl::manchester_encode<uint8_t>(0x01U));
    static_assert(0x6AAA == etl::manchester_encode<uint8_t>(0x80U));
#endif
  }

  TEST(encode16)
  {
    CHECK_EQUAL(0x5555AAAA, (etl::manchester_encode<uint16_t>(0xFF00UL)));
    CHECK_EQUAL(0x6AAAAAA9, (etl::manchester_encode<uint16_t>(0x8001UL)));
  }

  TEST(encode32)
  {
    CHECK_EQUAL(0x6AAAAAA95555AAAA, (etl::manchester_encode<uint32_t>(0x8001FF00ULL)));
  }

  TEST(encode_span)
  {
    etl::array<const uint8_t, 4>  input{0x00, 0xFF, 0x01, 0x80};

    alignas(uint16_t) etl::array<uint8_t, 8> output0;
    alignas(uint16_t) etl::array<uint8_t, 8> output1;
    alignas(uint32_t) etl::array<uint8_t, 8> output2;
    alignas(uint64_t) etl::array<uint8_t, 8> output3;

    etl::manchester_encode_span(input, output0);
    etl::manchester_encode_span<uint8_t>(input, output1);
    etl::manchester_encode_span<uint16_t>(input, output2);
    etl::manchester_encode_span<uint32_t>(input, output3);

    CHECK_EQUAL(0xAA, output0[0]);
    CHECK_EQUAL(0xAA, output0[1]);
    CHECK_EQUAL(0x55, output0[2]);
    CHECK_EQUAL(0x55, output0[3]);
    CHECK_EQUAL(0xA9, output0[4]);
    CHECK_EQUAL(0xAA, output0[5]);
    CHECK_EQUAL(0xAA, output0[6]);
    CHECK_EQUAL(0x6A, output0[7]);

    CHECK_TRUE(output0 == output1);
    CHECK_TRUE(output0 == output2);
    CHECK_TRUE(output0 == output3);
  }

  TEST(encode8_transform)
  {
    etl::array<uint8_t, 4>  input{0x00, 0xFF, 0x01, 0x80};
    etl::array<uint16_t, 4> output;

    etl::transform(input.begin(), input.end(), output.begin(), etl::manchester_encode<uint8_t>);

    CHECK_EQUAL(0xAAAA, output[0]);
    CHECK_EQUAL(0x5555, output[1]);
    CHECK_EQUAL(0xAAA9, output[2]);
    CHECK_EQUAL(0x6AAA, output[3]);
  }

  TEST(decode16)
  {
    CHECK_EQUAL(0x00, (etl::manchester_decode<uint16_t>(0xAAAAUL)));
    CHECK_EQUAL(0xFF, (etl::manchester_decode<uint16_t>(0x5555UL)));
    CHECK_EQUAL(0x01, (etl::manchester_decode<uint16_t>(0xAAA9UL)));
    CHECK_EQUAL(0x80, (etl::manchester_decode<uint16_t>(0x6AAAUL)));
  }

  TEST(decode32)
  {
    CHECK_EQUAL(0xFF00UL, (etl::manchester_decode<uint32_t>(0x5555AAAAUL)));
    CHECK_EQUAL(0x8001UL, (etl::manchester_decode<uint32_t>(0x6AAAAAA9UL)));
  }

  TEST(decode64)
  {
    CHECK_EQUAL(0x8001FF00ULL, (etl::manchester_decode<uint64_t>(0x6AAAAAA95555AAAAULL)));
  }

  TEST(decode_span)
  {
    etl::array<const uint8_t, 8>  input{0xAA, 0xAA, 0x55, 0x55, 0xA9, 0xAA, 0XAA, 0x6A};

    alignas(uint8_t) etl::array<uint8_t, 4> output0;
    alignas(uint8_t) etl::array<uint8_t, 4> output1;
    alignas(uint16_t) etl::array<uint8_t, 4> output2;
    alignas(uint32_t) etl::array<uint8_t, 4> output3;

    etl::manchester_decode_span(input, output0);
    etl::manchester_decode_span<uint16_t>(input, output1);
    etl::manchester_decode_span<uint32_t>(input, output2);
    etl::manchester_decode_span<uint64_t>(input, output3);

    CHECK_EQUAL(0x00, output0[0]);
    CHECK_EQUAL(0xFF, output0[1]);
    CHECK_EQUAL(0x01, output0[2]);
    CHECK_EQUAL(0x80, output0[3]);

    CHECK_TRUE(output0 == output1);
    CHECK_TRUE(output0 == output2);
    CHECK_TRUE(output0 == output3);
  }

  TEST(valid16)
  {
    CHECK_TRUE(etl::manchester_valid<uint16_t>(0xAAAAUL));
    CHECK_FALSE(etl::manchester_valid<uint16_t>(0xAAA8UL));
    CHECK_TRUE(etl::manchester_valid<uint16_t>(0xAAAAUL));
  }

  TEST(valid32)
  {
    CHECK_TRUE(etl::manchester_valid<uint32_t>(0xAAAAAAAAUL));
    CHECK_FALSE(etl::manchester_valid<uint32_t>(0xAAAAAAA8UL));
    CHECK_TRUE(etl::manchester_valid<uint32_t>(0xAAAAAAAAUL));
  }

  TEST(valid64)
  {
    CHECK_TRUE(etl::manchester_valid<uint64_t>(0xAAAAAAAAAAAAAAAAULL));
    CHECK_FALSE(etl::manchester_valid<uint64_t>(0xAAAAAAAAAAAAAAA8ULL));
    CHECK_TRUE(etl::manchester_valid<uint64_t>(0xAAAAAAAAAAAAAAAAULL));
  }
};

#if ETL_USING_CPP14
constexpr etl::array<uint8_t, 4> manchester_encoded(etl::span<const uint_least8_t> input)
{
  alignas(uint8_t) etl::array<uint8_t, 4> output {0, 0, 0, 0};
  etl::manchester_decode_span_constexpr(input, output);

  return output;
}

constexpr etl::array<const uint8_t, 8>  input{0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0XAA, 0xAA};

constexpr etl::array<uint8_t, 4> bla = manchester_encoded(input);
static_assert(bla[0] == 0);
#endif