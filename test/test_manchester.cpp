#include "etl/manchester.h"

#include "unit_test_framework.h"
#include <etl/algorithm.h>
#include <etl/array.h>
#include <etl/span.h>

SUITE(test_manchester){
  TEST(encode_uint8_t){
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

constexpr etl::array<uint8_t, 8> manchester_encoded(etl::span<const uint_least8_t> decoded)
{
  etl::array<uint8_t, 8> encoded{0, 0, 0, 0, 0, 0, 0, 0};
  etl::manchester::encode_span(decoded, encoded);
  return encoded;
}

TEST(encode_span)
{
  constexpr etl::array<const uint8_t, 4> decoded{0x00, 0xFF, 0x01, 0x80};

  etl::array<uint8_t, 8> encoded0;
  etl::array<uint8_t, 8> encoded1;
  etl::array<uint8_t, 8> encoded2;
  etl::array<uint8_t, 8> encoded3;

  etl::manchester::encode_span(decoded, encoded0);
  etl::manchester::encode_span<uint8_t>(decoded, encoded1);
  etl::manchester::encode_span<uint16_t>(decoded, encoded2);
  etl::manchester::encode_span<uint32_t>(decoded, encoded3);

  CHECK_EQUAL(0xAA, encoded0[0]);
  CHECK_EQUAL(0xAA, encoded0[1]);
  CHECK_EQUAL(0x55, encoded0[2]);
  CHECK_EQUAL(0x55, encoded0[3]);
  CHECK_EQUAL(0xA9, encoded0[4]);
  CHECK_EQUAL(0xAA, encoded0[5]);
  CHECK_EQUAL(0xAA, encoded0[6]);
  CHECK_EQUAL(0x6A, encoded0[7]);

  CHECK_TRUE(encoded0 == encoded1);
  CHECK_TRUE(encoded0 == encoded2);
  CHECK_TRUE(encoded0 == encoded3);

#if ETL_USING_CPP14
  static_assert(manchester_encoded(decoded)[0] == 0xAA, "Compile time encoding on range failed");
  static_assert(manchester_encoded(decoded)[1] == 0xAA, "Compile time encoding on range failed");
  static_assert(manchester_encoded(decoded)[2] == 0x55, "Compile time encoding on range failed");
  static_assert(manchester_encoded(decoded)[3] == 0x55, "Compile time encoding on range failed");
  static_assert(manchester_encoded(decoded)[4] == 0xA9, "Compile time encoding on range failed");
  static_assert(manchester_encoded(decoded)[5] == 0xAA, "Compile time encoding on range failed");
  static_assert(manchester_encoded(decoded)[6] == 0xAA, "Compile time encoding on range failed");
  static_assert(manchester_encoded(decoded)[7] == 0x6A, "Compile time encoding on range failed");
#endif
}

TEST(encode_span_inverted)
{
  etl::array<const uint8_t, 4> decoded{0x00, 0xFF, 0x01, 0x80};

  etl::array<uint8_t, 8> encoded0;
  etl::array<uint8_t, 8> encoded1;
  etl::array<uint8_t, 8> encoded2;
  etl::array<uint8_t, 8> encoded3;

  etl::manchester_inverted::encode_span(decoded, encoded0);
  etl::manchester_inverted::encode_span<uint8_t>(decoded, encoded1);
  etl::manchester_inverted::encode_span<uint16_t>(decoded, encoded2);
  etl::manchester_inverted::encode_span<uint32_t>(decoded, encoded3);

  CHECK_EQUAL(0x55, encoded0[0]);
  CHECK_EQUAL(0x55, encoded0[1]);
  CHECK_EQUAL(0xAA, encoded0[2]);
  CHECK_EQUAL(0xAA, encoded0[3]);
  CHECK_EQUAL(0x56, encoded0[4]);
  CHECK_EQUAL(0x55, encoded0[5]);
  CHECK_EQUAL(0x55, encoded0[6]);
  CHECK_EQUAL(0x95, encoded0[7]);

  CHECK_TRUE(encoded0 == encoded1);
  CHECK_TRUE(encoded0 == encoded2);
  CHECK_TRUE(encoded0 == encoded3);
}

TEST(encode_span_invalid_source)
{
  etl::array<const uint8_t, 3> invalid_source{0x00, 0xFF, 0x01};
  etl::array<uint8_t, 8>       valid_destination;

  CHECK_THROW({ etl::manchester::encode_span<uint16_t>(invalid_source, valid_destination); }, etl::manchester_invalid_size);
  CHECK_THROW({ etl::manchester::encode_span<uint32_t>(invalid_source, valid_destination); }, etl::manchester_invalid_size);
  CHECK_THROW({ etl::manchester_inverted::encode_span<uint16_t>(invalid_source, valid_destination); }, etl::manchester_invalid_size);
  CHECK_THROW({ etl::manchester_inverted::encode_span<uint32_t>(invalid_source, valid_destination); }, etl::manchester_invalid_size);
}

TEST(encode_span_invalid_destination)
{
  etl::array<const uint8_t, 4> valid_source{0x00, 0xFF, 0x01, 0x80};
  etl::array<uint8_t, 7>       invalid_destination;

  CHECK_THROW({ etl::manchester::encode_span<uint8_t>(valid_source, invalid_destination); }, etl::manchester_invalid_size);
  CHECK_THROW({ etl::manchester::encode_span<uint16_t>(valid_source, invalid_destination); }, etl::manchester_invalid_size);
  CHECK_THROW({ etl::manchester::encode_span<uint32_t>(valid_source, invalid_destination); }, etl::manchester_invalid_size);
  CHECK_THROW({ etl::manchester_inverted::encode_span<uint8_t>(valid_source, invalid_destination); }, etl::manchester_invalid_size);
  CHECK_THROW({ etl::manchester_inverted::encode_span<uint16_t>(valid_source, invalid_destination); }, etl::manchester_invalid_size);
  CHECK_THROW({ etl::manchester_inverted::encode_span<uint32_t>(valid_source, invalid_destination); }, etl::manchester_invalid_size);
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

constexpr etl::array<uint8_t, 4> manchester_decoded(etl::span<const uint_least8_t> encoded)
{
  etl::array<uint8_t, 4> decoded{0, 0, 0, 0};
  etl::manchester::decode_span(encoded, decoded);
  return decoded;
}

TEST(decode_span)
{
  constexpr etl::array<const uint8_t, 8> encoded{0xAA, 0xAA, 0x55, 0x55, 0xA9, 0xAA, 0XAA, 0x6A};

  etl::array<uint8_t, 4> decoded0;
  etl::array<uint8_t, 4> decoded1;
  etl::array<uint8_t, 4> decoded2;
  etl::array<uint8_t, 4> decoded3;

  etl::manchester::decode_span(encoded, decoded0);
  etl::manchester::decode_span<uint16_t>(encoded, decoded1);
  etl::manchester::decode_span<uint32_t>(encoded, decoded2);
  etl::manchester::decode_span<uint64_t>(encoded, decoded3);

  CHECK_EQUAL(0x00, decoded0[0]);
  CHECK_EQUAL(0xFF, decoded0[1]);
  CHECK_EQUAL(0x01, decoded0[2]);
  CHECK_EQUAL(0x80, decoded0[3]);

#if ETL_USING_CPP14
  static_assert(manchester_decoded(encoded)[0] == 0x00, "Compile time decoding on range failed");
  static_assert(manchester_decoded(encoded)[1] == 0xFF, "Compile time decoding on range failed");
  static_assert(manchester_decoded(encoded)[2] == 0x01, "Compile time decoding on range failed");
  static_assert(manchester_decoded(encoded)[3] == 0x80, "Compile time decoding on range failed");
#endif

  CHECK_TRUE(decoded0 == decoded1);
  CHECK_TRUE(decoded0 == decoded2);
  CHECK_TRUE(decoded0 == decoded3);
}

TEST(decode_span_inverted)
{
  etl::array<const uint8_t, 8> encoded{0x55, 0x55, 0xAA, 0xAA, 0x56, 0x55, 0X55, 0x95};

  etl::array<uint8_t, 4> decoded0;
  etl::array<uint8_t, 4> decoded1;
  etl::array<uint8_t, 4> decoded2;
  etl::array<uint8_t, 4> decoded3;

  etl::manchester_inverted::decode_span(encoded, decoded0);
  etl::manchester_inverted::decode_span<uint16_t>(encoded, decoded1);
  etl::manchester_inverted::decode_span<uint32_t>(encoded, decoded2);
  etl::manchester_inverted::decode_span<uint64_t>(encoded, decoded3);

  CHECK_EQUAL(0x00, decoded0[0]);
  CHECK_EQUAL(0xFF, decoded0[1]);
  CHECK_EQUAL(0x01, decoded0[2]);
  CHECK_EQUAL(0x80, decoded0[3]);

  CHECK_TRUE(decoded0 == decoded1);
  CHECK_TRUE(decoded0 == decoded2);
  CHECK_TRUE(decoded0 == decoded3);
}

TEST(decode_span_invalid_source)
{
  etl::array<const uint8_t, 7> invalid_source{0x55, 0x55, 0xAA, 0xAA, 0x56, 0x55, 0X55};
  etl::array<uint8_t, 4>       valid_destination;

  CHECK_THROW({ etl::manchester::decode_span<uint16_t>(invalid_source, valid_destination); }, etl::manchester_invalid_size);
  CHECK_THROW({ etl::manchester::decode_span<uint32_t>(invalid_source, valid_destination); }, etl::manchester_invalid_size);
  CHECK_THROW({ etl::manchester::decode_span<uint64_t>(invalid_source, valid_destination); }, etl::manchester_invalid_size);
  CHECK_THROW({ etl::manchester_inverted::decode_span<uint16_t>(invalid_source, valid_destination); }, etl::manchester_invalid_size);
  CHECK_THROW({ etl::manchester_inverted::decode_span<uint32_t>(invalid_source, valid_destination); }, etl::manchester_invalid_size);
  CHECK_THROW({ etl::manchester_inverted::decode_span<uint64_t>(invalid_source, valid_destination); }, etl::manchester_invalid_size);
}

TEST(decode_span_invalid_destination)
{
  etl::array<const uint8_t, 8> valid_source{0x55, 0x55, 0xAA, 0xAA, 0x56, 0x55, 0X55, 0x95};
  etl::array<uint8_t, 3>       invalid_destination;

  CHECK_THROW({ etl::manchester::decode_span<uint16_t>(valid_source, invalid_destination); }, etl::manchester_invalid_size);
  CHECK_THROW({ etl::manchester::decode_span<uint32_t>(valid_source, invalid_destination); }, etl::manchester_invalid_size);
  CHECK_THROW({ etl::manchester::decode_span<uint64_t>(valid_source, invalid_destination); }, etl::manchester_invalid_size);
  CHECK_THROW({ etl::manchester_inverted::decode_span<uint16_t>(valid_source, invalid_destination); }, etl::manchester_invalid_size);
  CHECK_THROW({ etl::manchester_inverted::decode_span<uint32_t>(valid_source, invalid_destination); }, etl::manchester_invalid_size);
  CHECK_THROW({ etl::manchester_inverted::decode_span<uint64_t>(valid_source, invalid_destination); }, etl::manchester_invalid_size);
}

TEST(decode_span_fast)
{
  etl::array<const uint8_t, 8> encoded{0xAA, 0xAA, 0x55, 0x55, 0xA9, 0xAA, 0XAA, 0x6A};

  etl::array<uint8_t, 4> decoded0;
  etl::array<uint8_t, 4> decoded1;
  etl::array<uint8_t, 4> decoded2;
  etl::array<uint8_t, 4> decoded3;

  etl::manchester::decode_span_fast(encoded, decoded0);
  etl::manchester::decode_span_fast<uint16_t>(encoded, decoded1);
  etl::manchester::decode_span_fast<uint32_t>(encoded, decoded2);
  etl::manchester::decode_span_fast<uint64_t>(encoded, decoded3);

  CHECK_EQUAL(0x00, decoded0[0]);
  CHECK_EQUAL(0xFF, decoded0[1]);
  CHECK_EQUAL(0x01, decoded0[2]);
  CHECK_EQUAL(0x80, decoded0[3]);

  CHECK_TRUE(decoded0 == decoded1);
  CHECK_TRUE(decoded0 == decoded2);
  CHECK_TRUE(decoded0 == decoded3);
}

TEST(valid16)
{
  CHECK_TRUE(etl::manchester::is_valid<uint16_t>(0xAAAAUL));
  CHECK_TRUE(etl::manchester_inverted::is_valid<uint16_t>(0xAAAAUL));
  CHECK_FALSE(etl::manchester::is_valid<uint16_t>(0xAAA8UL));
  CHECK_FALSE(etl::manchester_inverted::is_valid<uint16_t>(0xAAA8UL));

#if ETL_USING_CPP14
  static_assert(etl::manchester::is_valid<uint16_t>(0xAAAAUL), "Compile time manchester validity check failed");
  static_assert(etl::manchester_inverted::is_valid<uint16_t>(0xAAAAUL), "Compile time manchester validity check failed");
#endif
}

TEST(valid32)
{
  CHECK_TRUE(etl::manchester::is_valid<uint32_t>(0xAAAAAAAAUL));
  CHECK_TRUE(etl::manchester_inverted::is_valid<uint32_t>(0xAAAAAAAAUL));
  CHECK_FALSE(etl::manchester::is_valid<uint32_t>(0xAAAAAAA8UL));
  CHECK_FALSE(etl::manchester_inverted::is_valid<uint32_t>(0xAAAAAAA8UL));

#if ETL_USING_CPP14
  static_assert(etl::manchester::is_valid<uint32_t>(0xAAAAAAAAUL), "Compile time manchester validity check failed");
  static_assert(etl::manchester_inverted::is_valid<uint32_t>(0xAAAAAAAAUL), "Compile time manchester validity check failed");
#endif
}

TEST(valid64)
{
  CHECK_TRUE(etl::manchester::is_valid<uint64_t>(0xAAAAAAAAAAAAAAAAULL));
  CHECK_TRUE(etl::manchester_inverted::is_valid<uint64_t>(0xAAAAAAAAAAAAAAAAULL));
  CHECK_FALSE(etl::manchester::is_valid<uint64_t>(0xAAAAAAAAAAAAAAA8ULL));
  CHECK_FALSE(etl::manchester_inverted::is_valid<uint64_t>(0xAAAAAAAAAAAAAAA8ULL));

#if ETL_USING_CPP14
  static_assert(etl::manchester::is_valid<uint64_t>(0xAAAAAAAAAAAAAAAAULL), "Compile time manchester validity check failed");
  static_assert(etl::manchester_inverted::is_valid<uint64_t>(0xAAAAAAAAAAAAAAAAULL), "Compile time manchester validity check failed");
#endif
}

TEST(valid_span)
{
  constexpr etl::array<uint8_t, 8> encoded1{0xAA, 0xAA, 0x55, 0x55, 0xA9, 0xAA, 0XAA, 0x6A};
  constexpr etl::array<uint8_t, 8> encoded2{0xAA, 0xAA, 0x55, 0x55, 0xA9, 0xAA, 0XAB, 0x6A};

  CHECK_TRUE(etl::manchester::is_valid(encoded1));
  CHECK_FALSE(etl::manchester::is_valid(encoded2));

#if ETL_USING_CPP14
  static_assert(etl::manchester::is_valid(encoded1), "Compile time manchester validity check failed");
  static_assert(!etl::manchester::is_valid(encoded2), "Compile time manchester validity check failed");
#endif
}

TEST(valid_span_on_invalid_source)
{
  constexpr etl::array<uint8_t, 7> invalid_source{0xAA, 0xAA, 0x55, 0x55, 0xA9, 0xAA, 0XAA};
  CHECK_THROW({ etl::manchester::is_valid(invalid_source); }, etl::manchester_invalid_size);
}
}
;