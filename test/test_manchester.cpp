#include "etl/manchester.h"

#include "unit_test_framework.h"
#include <algorithm>
#include <array>
#include <etl/span.h>

SUITE(test_manchester)
{

  TEST(encode8){
    CHECK_EQUAL(0xAAAA, (etl::manchester_encode<uint8_t>(0x00U)));
    CHECK_EQUAL(0x5555, (etl::manchester_encode<uint8_t>(0xFFU)));
    CHECK_EQUAL(0xAAA9, (etl::manchester_encode<uint8_t>(0x01U)));
    CHECK_EQUAL(0x6AAA, (etl::manchester_encode<uint8_t>(0x80U)));
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

  TEST(encode_span8)
  {
    std::array<const uint8_t, 4>  input{0x00, 0xFF, 0x01, 0x80};
    std::array<const uint16_t, 2> input2{0xFF00, 0x8001};
    std::array<const uint32_t, 1> input3{0x8001FF00};

    std::array<uint16_t, 4> output1;
    std::array<uint16_t, 4> output2;
    std::array<uint16_t, 4> output3;

    std::array<uint32_t, 2> output4;
    std::array<uint32_t, 2> output5;
    std::array<uint32_t, 2> output6;

    std::array<uint64_t, 1> output7;
    std::array<uint64_t, 1> output8;
    std::array<uint64_t, 1> output9;

    etl::manchester_encode_span<uint8_t, uint16_t, uint8_t>(input, output1);
    etl::manchester_encode_span<uint8_t, uint16_t, uint16_t>(input, output2);
    etl::manchester_encode_span<uint8_t, uint16_t, uint32_t>(input, output3);

    etl::manchester_encode_span<uint16_t, uint32_t, uint8_t>(input2, output4);
    etl::manchester_encode_span<uint16_t, uint32_t, uint16_t>(input2, output5);
    etl::manchester_encode_span<uint16_t, uint32_t, uint32_t>(input2, output6);

    etl::manchester_encode_span<uint32_t, uint64_t, uint8_t>(input3, output7);
    etl::manchester_encode_span<uint32_t, uint64_t, uint16_t>(input3, output8);
    etl::manchester_encode_span<uint32_t, uint64_t, uint32_t>(input3, output9);

    CHECK_EQUAL(0xAAAA, output1[0]);
    CHECK_EQUAL(0x5555, output1[1]);
    CHECK_EQUAL(0xAAA9, output1[2]);
    CHECK_EQUAL(0x6AAA, output1[3]);

    CHECK_EQUAL(0xAAAA, output2[0]);
    CHECK_EQUAL(0x5555, output2[1]);
    CHECK_EQUAL(0xAAA9, output2[2]);
    CHECK_EQUAL(0x6AAA, output2[3]);

    CHECK_EQUAL(0xAAAA, output3[0]);
    CHECK_EQUAL(0x5555, output3[1]);
    CHECK_EQUAL(0xAAA9, output3[2]);
    CHECK_EQUAL(0x6AAA, output3[3]);

    CHECK_EQUAL(0x5555AAAA, output4[0]);
    CHECK_EQUAL(0x6AAAAAA9, output4[1]);

    CHECK_EQUAL(0x5555AAAA, output5[0]);
    CHECK_EQUAL(0x6AAAAAA9, output5[1]);

    CHECK_EQUAL(0x5555AAAA, output6[0]);
    CHECK_EQUAL(0x6AAAAAA9, output6[1]);

    CHECK_EQUAL(0x6AAAAAA95555AAAA, output7[0]);
    CHECK_EQUAL(0x6AAAAAA95555AAAA, output8[0]);
    CHECK_EQUAL(0x6AAAAAA95555AAAA, output9[0]);
  }

  TEST(encode8_transform)
  {
    std::array<uint8_t, 4>  input{0x00, 0xFF, 0x01, 0x80};
    std::array<uint16_t, 4> output;

    std::transform(input.begin(), input.end(), output.begin(), etl::manchester_encode<uint8_t>);

    CHECK_EQUAL(0xAAAA, output[0]);
    CHECK_EQUAL(0x5555, output[1]);
    CHECK_EQUAL(0xAAA9, output[2]);
    CHECK_EQUAL(0x6AAA, output[3]);
  }

  TEST(encode16_transform)
  {
    std::array<uint8_t, 4>  input{0x00, 0xFF, 0x01, 0x80};
    std::array<uint16_t, 4> output;

    etl::span<uint16_t> input16{reinterpret_cast<uint16_t*>(input.data()), 2};
    etl::span<uint32_t> output32{reinterpret_cast<uint32_t*>(output.data()), 2};

    std::transform(input16.begin(), input16.end(), output32.begin(), etl::manchester_encode<uint16_t>);

    CHECK_EQUAL(0xAAAA, output[0]);
    CHECK_EQUAL(0x5555, output[1]);
    CHECK_EQUAL(0xAAA9, output[2]);
    CHECK_EQUAL(0x6AAA, output[3]);
  }

  TEST(encode32_transform)
  {
    std::array<uint8_t, 4>  input{0x00, 0xFF, 0x01, 0x80};
    std::array<uint16_t, 4> output;

    etl::span<uint32_t> input32{reinterpret_cast<uint32_t*>(input.data()), 1};
    etl::span<uint64_t> output64{reinterpret_cast<uint64_t*>(output.data()), 1};

    std::transform(input32.begin(), input32.end(), output64.begin(), etl::manchester_encode<uint32_t>);

    CHECK_EQUAL(0xAAAA, output[0]);
    CHECK_EQUAL(0x5555, output[1]);
    CHECK_EQUAL(0xAAA9, output[2]);
    CHECK_EQUAL(0x6AAA, output[3]);
  }

  TEST(decode16)
  {
    CHECK_EQUAL(0x00, (etl::manchester_decode(0xAAAAUL)));
    CHECK_EQUAL(0xFF, (etl::manchester_decode(0x5555UL)));
    CHECK_EQUAL(0x01, (etl::manchester_decode(0xAAA9UL)));
    CHECK_EQUAL(0x80, (etl::manchester_decode(0x6AAAUL)));
  }

  TEST(decode32)
  {
    CHECK_EQUAL(0xFF00UL, (etl::manchester_decode(0x5555AAAAUL)));
    CHECK_EQUAL(0x8001UL, (etl::manchester_decode(0x6AAAAAA9UL)));
  }

  TEST(decode64)
  {
    CHECK_EQUAL(0x8001FF00ULL, (etl::manchester_decode(0x6AAAAAA95555AAAAULL)));
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