#include "etl/manchester.h"

#include "unit_test_framework.h"
#include <etl/algorithm.h>
#include <etl/array.h>
#include <etl/span.h>


SUITE(test_manchester)
{
  TEST(encode_uint8_t)
  {
    CHECK_EQUAL(0xAAAA, (etl::manchester::encode<uint8_t>(0x00U)));
    CHECK_EQUAL(0x5555, (etl::manchester::encode<uint8_t>(0xFFU)));
    CHECK_EQUAL(0xAAA9, (etl::manchester::encode<uint8_t>(0x01U)));
    CHECK_EQUAL(0x6AAA, (etl::manchester::encode<uint8_t>(0x80U)));

#if ETL_USING_CPP14
    static_assert(0xAAAA == etl::manchester::encode<uint8_t>(0x00U), "Compile time manchester encoding failed");
#endif
  }

  TEST(encode_uint8_t_inverted)
  {
    CHECK_EQUAL(0x5555, (etl::manchester_inverted::encode<uint8_t>(0x00U)));
    CHECK_EQUAL(0xAAAA, (etl::manchester_inverted::encode<uint8_t>(0xFFU)));
    CHECK_EQUAL(0x5556, (etl::manchester_inverted::encode<uint8_t>(0x01U)));
    CHECK_EQUAL(0x9555, (etl::manchester_inverted::encode<uint8_t>(0x80U)));

#if ETL_USING_CPP14
    static_assert(0x5555 == etl::manchester_inverted::encode<uint8_t>(0x00U), "Compile time manchester encoding failed");
#endif
  }

  TEST(encode_uint16_t)
  {
    CHECK_EQUAL(0x5555AAAA, (etl::manchester::encode<uint16_t>(0xFF00UL)));
    CHECK_EQUAL(0x6AAAAAA9, (etl::manchester::encode<uint16_t>(0x8001UL)));

#if ETL_USING_CPP14
    static_assert(0x5555AAAA == etl::manchester::encode<uint16_t>(0xFF00UL), "Compile time manchester encoding failed");
#endif
  }

  TEST(encode_uint16_t_inverted)
  {
    CHECK_EQUAL(0xAAAA5555, (etl::manchester_inverted::encode<uint16_t>(0xFF00UL)));
    CHECK_EQUAL(0x95555556, (etl::manchester_inverted::encode<uint16_t>(0x8001UL)));

#if ETL_USING_CPP14
    static_assert(0xAAAA5555 == etl::manchester_inverted::encode<uint16_t>(0xFF00UL), "Compile time manchester encoding failed");
#endif
  }

  TEST(encode_uint32_t)
  {
    CHECK_EQUAL(0x6AAAAAA95555AAAA, (etl::manchester::encode<uint32_t>(0x8001FF00ULL)));

#if ETL_USING_CPP14
    static_assert(0x6AAAAAA95555AAAA == etl::manchester::encode<uint32_t>(0x8001FF00ULL), "Compile time manchester encoding failed");
#endif
  }

  TEST(encode_uint32_t_inverted)
  {
    CHECK_EQUAL(0x95555556AAAA5555, (etl::manchester_inverted::encode<uint32_t>(0x8001FF00ULL)));

#if ETL_USING_CPP14
    static_assert(0x95555556AAAA5555 == etl::manchester_inverted::encode<uint32_t>(0x8001FF00ULL), "Compile time manchester encoding failed");
#endif
  }

  TEST(encode_span)
  {
    etl::array<const uint8_t, 4>  input{0x00, 0xFF, 0x01, 0x80};

    alignas(uint16_t) etl::array<uint8_t, 8> output0;
    alignas(uint16_t) etl::array<uint8_t, 8> output1;
    alignas(uint32_t) etl::array<uint8_t, 8> output2;
    alignas(uint64_t) etl::array<uint8_t, 8> output3;

    etl::manchester::encode_span(input, output0);
    etl::manchester::encode_span<uint8_t>(input, output1);
    etl::manchester::encode_span<uint16_t>(input, output2);
    etl::manchester::encode_span<uint32_t>(input, output3);

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

  TEST(encode_span_inverted)
  {
    etl::array<const uint8_t, 4>  input{0x00, 0xFF, 0x01, 0x80};

    alignas(uint16_t) etl::array<uint8_t, 8> output0;
    alignas(uint16_t) etl::array<uint8_t, 8> output1;
    alignas(uint32_t) etl::array<uint8_t, 8> output2;
    alignas(uint64_t) etl::array<uint8_t, 8> output3;

    etl::manchester_inverted::encode_span(input, output0);
    etl::manchester_inverted::encode_span<uint8_t>(input, output1);
    etl::manchester_inverted::encode_span<uint16_t>(input, output2);
    etl::manchester_inverted::encode_span<uint32_t>(input, output3);

    CHECK_EQUAL(0x55, output0[0]);
    CHECK_EQUAL(0x55, output0[1]);
    CHECK_EQUAL(0xAA, output0[2]);
    CHECK_EQUAL(0xAA, output0[3]);
    CHECK_EQUAL(0x56, output0[4]);
    CHECK_EQUAL(0x55, output0[5]);
    CHECK_EQUAL(0x55, output0[6]);
    CHECK_EQUAL(0x95, output0[7]);

    CHECK_TRUE(output0 == output1);
    CHECK_TRUE(output0 == output2);
    CHECK_TRUE(output0 == output3);
  }

  TEST(decode_uint16_t)
  {
    CHECK_EQUAL(0x00, (etl::manchester::decode<uint16_t>(0xAAAAUL)));
    CHECK_EQUAL(0xFF, (etl::manchester::decode<uint16_t>(0x5555UL)));
    CHECK_EQUAL(0x01, (etl::manchester::decode<uint16_t>(0xAAA9UL)));
    CHECK_EQUAL(0x80, (etl::manchester::decode<uint16_t>(0x6AAAUL)));

#if ETL_USING_CPP14
    static_assert(0x00 == etl::manchester::decode<uint16_t>(0xAAAAUL), "Compile time manchester decoding failed");
#endif
  }

  TEST(decode_uint16_t_inverted)
  {
    CHECK_EQUAL(0x00, (etl::manchester_inverted::decode<uint16_t>(0x5555UL)));
    CHECK_EQUAL(0xFF, (etl::manchester_inverted::decode<uint16_t>(0xAAAAUL)));
    CHECK_EQUAL(0x01, (etl::manchester_inverted::decode<uint16_t>(0x5556UL)));
    CHECK_EQUAL(0x80, (etl::manchester_inverted::decode<uint16_t>(0x9555UL)));

#if ETL_USING_CPP14
    static_assert(0x00 == etl::manchester_inverted::decode<uint16_t>(0x5555UL), "Compile time manchester decoding failed");
#endif
  }

  TEST(decode_uint32_t)
  {
    CHECK_EQUAL(0xFF00UL, (etl::manchester::decode<uint32_t>(0x5555AAAAUL)));
    CHECK_EQUAL(0x8001UL, (etl::manchester::decode<uint32_t>(0x6AAAAAA9UL)));

#if ETL_USING_CPP14
    static_assert(0xFF00UL == etl::manchester::decode<uint32_t>(0x5555AAAAUL), "Compile time manchester decoding failed");
#endif
  }

  TEST(decode_uint32_t_inverted)
  {
    CHECK_EQUAL(0xFF00UL, (etl::manchester_inverted::decode<uint32_t>(0xAAAA5555UL)));
    CHECK_EQUAL(0x8001UL, (etl::manchester_inverted::decode<uint32_t>(0x95555556UL)));

#if ETL_USING_CPP14
    static_assert(0xFF00UL == etl::manchester_inverted::decode<uint32_t>(0xAAAA5555UL), "Compile time manchester decoding failed");
#endif
  }

  TEST(decode_uint64_t)
  {
    CHECK_EQUAL(0x8001FF00ULL, (etl::manchester::decode<uint64_t>(0x6AAAAAA95555AAAAULL)));

#if ETL_USING_CPP14
    static_assert(0x8001FF00ULL == etl::manchester::decode<uint64_t>(0x6AAAAAA95555AAAAULL), "Compile time manchester decoding failed");
#endif
  }

  TEST(decode_uint64_t_inverted)
  {
    CHECK_EQUAL(0x8001FF00ULL, (etl::manchester_inverted::decode<uint64_t>(0x95555556AAAA5555ULL)));

#if ETL_USING_CPP14
    static_assert(0x8001FF00ULL == etl::manchester_inverted::decode<uint64_t>(0x95555556AAAA5555ULL), "Compile time manchester decoding failed");
#endif
  }

  TEST(decode_span)
  {
    etl::array<const uint8_t, 8>  input{0xAA, 0xAA, 0x55, 0x55, 0xA9, 0xAA, 0XAA, 0x6A};

    alignas(uint8_t) etl::array<uint8_t, 4> output0;
    alignas(uint8_t) etl::array<uint8_t, 4> output1;
    alignas(uint16_t) etl::array<uint8_t, 4> output2;
    alignas(uint32_t) etl::array<uint8_t, 4> output3;

    etl::manchester::decode_span(input, output0);
    etl::manchester::decode_span<uint16_t>(input, output1);
    etl::manchester::decode_span<uint32_t>(input, output2);
    etl::manchester::decode_span<uint64_t>(input, output3);

    CHECK_EQUAL(0x00, output0[0]);
    CHECK_EQUAL(0xFF, output0[1]);
    CHECK_EQUAL(0x01, output0[2]);
    CHECK_EQUAL(0x80, output0[3]);

    CHECK_TRUE(output0 == output1);
    CHECK_TRUE(output0 == output2);
    CHECK_TRUE(output0 == output3);
  }

  TEST(decode_span_inverted)
  {
    etl::array<const uint8_t, 8>  input{0x55, 0x55, 0xAA, 0xAA, 0x56, 0x55, 0X55, 0x95};

    alignas(uint8_t) etl::array<uint8_t, 4> output0;
    alignas(uint8_t) etl::array<uint8_t, 4> output1;
    alignas(uint16_t) etl::array<uint8_t, 4> output2;
    alignas(uint32_t) etl::array<uint8_t, 4> output3;

    etl::manchester_inverted::decode_span(input, output0);
    etl::manchester_inverted::decode_span<uint16_t>(input, output1);
    etl::manchester_inverted::decode_span<uint32_t>(input, output2);
    etl::manchester_inverted::decode_span<uint64_t>(input, output3);

    CHECK_EQUAL(0x00, output0[0]);
    CHECK_EQUAL(0xFF, output0[1]);
    CHECK_EQUAL(0x01, output0[2]);
    CHECK_EQUAL(0x80, output0[3]);

    CHECK_TRUE(output0 == output1);
    CHECK_TRUE(output0 == output2);
    CHECK_TRUE(output0 == output3);
  }

    TEST(decode_span_fast)
  {
    etl::array<const uint8_t, 8>  input{0xAA, 0xAA, 0x55, 0x55, 0xA9, 0xAA, 0XAA, 0x6A};

    alignas(uint8_t) etl::array<uint8_t, 4> output0;
    alignas(uint8_t) etl::array<uint8_t, 4> output1;
    alignas(uint16_t) etl::array<uint8_t, 4> output2;
    alignas(uint32_t) etl::array<uint8_t, 4> output3;

    etl::manchester::decode_span_fast(input, output0);
    etl::manchester::decode_span_fast<uint16_t>(input, output1);
    etl::manchester::decode_span_fast<uint32_t>(input, output2);
    etl::manchester::decode_span_fast<uint64_t>(input, output3);

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
    CHECK_TRUE(etl::manchester::valid<uint16_t>(0xAAAAUL));
    CHECK_TRUE(etl::manchester_inverted::valid<uint16_t>(0xAAAAUL));
    CHECK_FALSE(etl::manchester::valid<uint16_t>(0xAAA8UL));
    CHECK_FALSE(etl::manchester_inverted::valid<uint16_t>(0xAAA8UL));

#if ETL_USING_CPP14
    static_assert(etl::manchester::valid<uint16_t>(0xAAAAUL), "Compile time manchester validity check failed");
    static_assert(etl::manchester_inverted::valid<uint16_t>(0xAAAAUL), "Compile time manchester validity check failed");
#endif
  }

  TEST(valid32)
  {
    CHECK_TRUE(etl::manchester::valid<uint32_t>(0xAAAAAAAAUL));
    CHECK_TRUE(etl::manchester_inverted::valid<uint32_t>(0xAAAAAAAAUL));
    CHECK_FALSE(etl::manchester::valid<uint32_t>(0xAAAAAAA8UL));
    CHECK_FALSE(etl::manchester_inverted::valid<uint32_t>(0xAAAAAAA8UL));

#if ETL_USING_CPP14
    static_assert(etl::manchester::valid<uint32_t>(0xAAAAAAAAUL), "Compile time manchester validity check failed");
    static_assert(etl::manchester_inverted::valid<uint32_t>(0xAAAAAAAAUL), "Compile time manchester validity check failed");
#endif
  }

  TEST(valid64)
  {
    CHECK_TRUE(etl::manchester::valid<uint64_t>(0xAAAAAAAAAAAAAAAAULL));
    CHECK_TRUE(etl::manchester_inverted::valid<uint64_t>(0xAAAAAAAAAAAAAAAAULL));
    CHECK_FALSE(etl::manchester::valid<uint64_t>(0xAAAAAAAAAAAAAAA8ULL));
    CHECK_FALSE(etl::manchester_inverted::valid<uint64_t>(0xAAAAAAAAAAAAAAA8ULL));

#if ETL_USING_CPP14
    static_assert(etl::manchester::valid<uint64_t>(0xAAAAAAAAAAAAAAAAULL), "Compile time manchester validity check failed");
    static_assert(etl::manchester_inverted::valid<uint64_t>(0xAAAAAAAAAAAAAAAAULL), "Compile time manchester validity check failed");
#endif
  }
};

// #if ETL_USING_CPP14
// constexpr etl::array<uint8_t, 4> manchester_encoded(etl::span<const uint_least8_t> input)
// {
//   alignas(uint8_t) etl::array<uint8_t, 4> output {0, 0, 0, 0};
//   etl::manchester::decode_span(input, output);

//   return output;
// }

// constexpr etl::array<const uint8_t, 8>  input{0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0XAA, 0xAA};

// constexpr etl::array<uint8_t, 4> bla = manchester_encoded(input);
// static_assert(bla[0] == 0, "Compile time computation of many values failed");
// #endif