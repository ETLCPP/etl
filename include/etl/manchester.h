#ifndef ETL_MANCHESTER_INCLUDED
#define ETL_MANCHESTER_INCLUDED

///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 John Wellbelove

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

#include <etl/platform.h>
#include <etl/span.h>
#include <etl/static_assert.h>

///\defgroup manchester manchester
/// Manchester encoding and decoding
///\ingroup utilities

namespace etl
{
  namespace private_manchester
  {
    //*************************************************************************
    /// Type trait to determine if a type is supported as an encode input.
    ///\tparam TChunk The input type to check.
    //*************************************************************************
    template <typename TChunk>
    struct is_encodable
    {
      static const bool value =
#if ETL_USING_8BIT_TYPES
        etl::is_same<TChunk, uint8_t>::value ||
#endif
        etl::is_same<TChunk, uint16_t>::value
#if ETL_USING_64BIT_TYPES
        || etl::is_same<TChunk, uint32_t>::value
#endif
        ;
    };

    //*************************************************************************
    /// Type trait to determine if a type is supported as a decode input.
    ///\tparam TChunk The input type to check.
    //*************************************************************************
    template <typename TChunk>
    struct is_decodable
    {
      static const bool value =
#if ETL_USING_8BIT_TYPES
        etl::is_same<TChunk, uint16_t>::value ||
#endif
        etl::is_same<TChunk, uint32_t>::value
#if ETL_USING_64BIT_TYPES
        || etl::is_same<TChunk, uint64_t>::value
#endif
        ;
    };

    //*************************************************************************
    /// Type trait to determine the encoded type for a given decoded type.
    /// Encoding doubles the bit width: uint8_t->uint16_t, uint16_t->uint32_t, etc.
    ///\tparam T The decoded type.
    //*************************************************************************
    template <typename T>
    struct manchester_encoded
    {
      ETL_STATIC_ASSERT(sizeof(T) == 0, "Manchester encoding type should be one of [uint8_t, uint16_t, uint32_t]");
    };

    template <>
    struct manchester_encoded<uint8_t>
    {
      typedef uint16_t type;
    };
    template <>
    struct manchester_encoded<uint16_t>
    {
      typedef uint32_t type;
    };
    template <>
    struct manchester_encoded<uint32_t>
    {
      typedef uint64_t type;
    };

    //*************************************************************************
    /// Type trait to determine the decoded type for a given encoded type.
    /// Decoding halves the bit width: uint16_t->uint8_t, uint32_t->uint16_t, etc.
    ///\tparam T The encoded type.
    //*************************************************************************
    template <typename T>
    struct manchester_decoded
    {
      ETL_STATIC_ASSERT(sizeof(T) == 0, "Manchester decoding type should be one of [uint16_t, uint32_t, uint64_t]");
    };

    template <>
    struct manchester_decoded<uint16_t>
    {
      typedef uint8_t type;
    };
    template <>
    struct manchester_decoded<uint32_t>
    {
      typedef uint16_t type;
    };
    template <>
    struct manchester_decoded<uint64_t>
    {
      typedef uint32_t type;
    };

    //*************************************************************************
    /// Normal Manchester encoding type (no inversion).
    //*************************************************************************
    struct manchester_type_normal
    {
#if ETL_USING_64BIT_TYPES
      static const uint64_t inversion_mask = 0x0000000000000000ULL;
#else
      static const uint32_t inversion_mask = 0x00000000UL;
#endif
    };

    //*************************************************************************
    /// Inverted Manchester encoding type.
    //*************************************************************************
    struct manchester_type_inverted
    {
#if ETL_USING_64BIT_TYPES
      static const uint64_t inversion_mask = 0xFFFFFFFFFFFFFFFFULL;
#else
      static const uint32_t inversion_mask = 0xFFFFFFFFUL;
#endif
    };
  }  // namespace private_manchester

  //***************************************************************************
  /// Exception for Manchester.
  //***************************************************************************
  class manchester_exception : public etl::exception
  {
  public:
    manchester_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Invalid size exception for Manchester
  //***************************************************************************
  class manchester_invalid_size : public etl::manchester_exception
  {
  public:
    manchester_invalid_size(string_type file_name_, numeric_type line_number_)
      : etl::manchester_exception("manchester:size", file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  ///\ingroup manchester
  /// Base template class for Manchester encoding and decoding.
  ///\tparam TType The Manchester encoding type (normal or inverted).
  //***************************************************************************
  template <typename TManchesterType>
  struct manchester_base
  {
    ETL_STATIC_ASSERT((etl::is_same<TManchesterType, private_manchester::manchester_type_normal>::value ||
                       etl::is_same<TManchesterType, private_manchester::manchester_type_inverted>::value),
                      "TManchesterType must be manchester_type_normal or manchester_type_inverted");

    ETL_STATIC_ASSERT(CHAR_BIT == etl::numeric_limits<uint_least8_t>::digits, "Manchester requires uint_least8_t to have the same number of bits as CHAR (CHAR_BITS)");

    //*************************************************************************
    // Encoding functions
    //*************************************************************************

#if ETL_USING_8BIT_TYPES
    //*************************************************************************
    /// Encode a uint8_t value in place to a uint16_t.
    ///\param decoded The value to encode.
    ///\param encoded The encoded value.
    //*************************************************************************
    template <typename TChunk>
    static ETL_CONSTEXPR14 typename etl::enable_if<etl::is_same<TChunk, uint8_t>::value, void>::type
    encode_in_place(TChunk decoded, uint16_t& encoded)
    {
      encoded = decoded;

      encoded = (encoded | (encoded << 4U)) & 0x0F0FU;
      encoded = (encoded | (encoded << 2U)) & 0x3333U;
      encoded = (encoded | (encoded << 1U)) & 0x5555U;
      encoded = (encoded | (encoded << 1U)) ^ (0xAAAAU ^ static_cast<uint16_t>(TManchesterType::inversion_mask));
    }
#endif

    //*************************************************************************
    /// Encode a uint16_t value in place to a uint32_t.
    ///\param decoded The value to encode.
    ///\param encoded The encoded value.
    //*************************************************************************
    template <typename TChunk>
    static ETL_CONSTEXPR14 typename etl::enable_if<etl::is_same<TChunk, uint16_t>::value, void>::type
    encode_in_place(TChunk decoded, uint32_t& encoded)
    {
      encoded = decoded;

      encoded = (encoded | (encoded << 8U)) & 0x00FF00FFUL;
      encoded = (encoded | (encoded << 4U)) & 0x0F0F0F0FUL;
      encoded = (encoded | (encoded << 2U)) & 0x33333333UL;
      encoded = (encoded | (encoded << 1U)) & 0x55555555UL;
      encoded = (encoded | (encoded << 1U)) ^ (0xAAAAAAAAUL ^ static_cast<uint32_t>(TManchesterType::inversion_mask));
    }

    //*************************************************************************
    /// Encode a uint32_t value in place to a uint64_t.
    ///\param decoded The value to encode.
    ///\param encoded The encoded value.
    //*************************************************************************
    template <typename TChunk>
    static ETL_CONSTEXPR14 typename etl::enable_if<etl::is_same<TChunk, uint32_t>::value, void>::type
    encode_in_place(TChunk decoded, uint64_t& encoded)
    {
      encoded = decoded;

      encoded = (encoded | (encoded << 16U)) & 0x0000FFFF0000FFFFULL;
      encoded = (encoded | (encoded << 8U)) & 0x00FF00FF00FF00FFULL;
      encoded = (encoded | (encoded << 4U)) & 0x0F0F0F0F0F0F0F0FULL;
      encoded = (encoded | (encoded << 2U)) & 0x3333333333333333ULL;
      encoded = (encoded | (encoded << 1U)) & 0x5555555555555555ULL;
      encoded = (encoded | (encoded << 1U)) ^ (0xAAAAAAAAAAAAAAAAULL ^ TManchesterType::inversion_mask);
    }
#endif

    template <typename TChunk>
    static typename etl::enable_if<!private_manchester::is_encodable<TChunk>::value, void>::type
    encode_in_place(TChunk decoded, typename private_manchester::manchester_encoded<TChunk>::type& encoded) ETL_DELETE;

    //*************************************************************************
    /// Encode a value and return the Manchester encoded result.
    ///\param decoded The value to encode.
    ///\return The Manchester encoded value.
    //*************************************************************************
    template <typename TChunk>
    ETL_NODISCARD static ETL_CONSTEXPR14 typename private_manchester::manchester_encoded<TChunk>::type encode(TChunk decoded)
    {
      ETL_STATIC_ASSERT(private_manchester::is_encodable<TChunk>::value, "TChunk must be an encodable type");

      typename private_manchester::manchester_encoded<TChunk>::type encoded = 0;
      encode_in_place(decoded, encoded);
      return encoded;
    }

    //*************************************************************************
    /// Encode a span of data with the selected chunk size.
    ///\param source      The source data to encode.
    ///\param destination The destination buffer for encoded data.
    ///\tparam TChunk     The chunk size for encoding (default: uint_least8_t).
    //*************************************************************************
    template <typename TChunk>
    static typename etl::enable_if<!etl::is_same<TChunk, uint_least8_t>::value, void>::type
    encode_span(etl::span<const uint_least8_t> source, etl::span<uint_least8_t> destination)
    {
      typedef TChunk                                                             TDecoded;
      typedef typename etl::private_manchester::manchester_encoded<TChunk>::type TEncoded;

      ETL_ASSERT(destination.size() >= source.size() * 2, ETL_ERROR(manchester_invalid_size));
      ETL_ASSERT(source.size() % sizeof(TDecoded) == 0, ETL_ERROR(manchester_invalid_size));

      size_t dest_index = 0;
      size_t source_index = 0;
      for (size_t i = 0; i < source.size() / sizeof(TDecoded); ++i)
      {
        TDecoded decoded_value = 0;
        memcpy(&decoded_value, &source[source_index], sizeof(TDecoded));
        const TEncoded encoded_value = encode(decoded_value);
        memcpy(&destination[dest_index], &encoded_value, sizeof(TEncoded));

        source_index += sizeof(TDecoded);
        dest_index += sizeof(TEncoded);
      }
    }

    //*************************************************************************
    /// Encode a span of data with the minimum chunk size. This version is
    /// constexpr so that it can be used to decode data at compile time.
    ///\param source      The source data to encode.
    ///\param destination The destination buffer for encoded data.
    ///\tparam TChunk     The chunk size for encoding (default: uint_least8_t).
    //*************************************************************************
    template <typename TChunk = uint_least8_t>
    static typename etl::enable_if<etl::is_same<TChunk, uint_least8_t>::value, void>::type constexpr encode_span(etl::span<const uint_least8_t> source, etl::span<uint_least8_t> destination)
    {
      typedef TChunk                                                             TDecoded;
      typedef typename etl::private_manchester::manchester_encoded<TChunk>::type TEncoded;

      ETL_ASSERT(destination.size() >= source.size() * 2, ETL_ERROR(manchester_invalid_size));
      ETL_ASSERT(source.size() % sizeof(TDecoded) == 0, ETL_ERROR(manchester_invalid_size));

      size_t dest_index = 0;
      size_t source_index = 0;
      for (size_t i = 0; i < source.size() / sizeof(TDecoded); ++i)
      {
        const TEncoded encoded_value = encode(source[source_index]);
        destination[dest_index] = static_cast<uint_least8_t>(encoded_value);
        destination[dest_index + 1] = static_cast<uint_least8_t>(encoded_value >> CHAR_BIT);

        source_index += sizeof(TDecoded);
        dest_index += sizeof(TEncoded);
      }
    }

    //*************************************************************************
    // Decoding functions
    //*************************************************************************

#if ETL_USING_8BIT_TYPES
#if ETL_USING_8BIT_TYPES
    //*************************************************************************
    /// Decode a uint16_t value in place to a uint8_t.
    ///\param encoded The value to decode.
    ///\param decoded The decoded value.
    //*************************************************************************
    template <typename TChunk>
    static ETL_CONSTEXPR14 typename etl::enable_if<etl::is_same<TChunk, uint16_t>::value, void>::type
    decode_in_place(TChunk encoded, uint8_t& decoded)
    {
      encoded = (encoded ^ (0xAAAAU ^ static_cast<uint16_t>(TManchesterType::inversion_mask))) & 0x5555U;
      encoded = (encoded | (encoded >> 1)) & 0x3333U;
      encoded = (encoded | (encoded >> 2)) & 0x0F0FU;
      decoded = static_cast<uint8_t>(encoded | (encoded >> 4U));
    }
#endif

    //*************************************************************************
    /// Decode a uint32_t value in place to a uint16_t.
    ///\param encoded The value to decode.
    ///\param decoded The decoded value.
    //*************************************************************************
    template <typename TChunk>
    static ETL_CONSTEXPR14 typename etl::enable_if<etl::is_same<TChunk, uint32_t>::value, void>::type
    decode_in_place(TChunk encoded, uint16_t& decoded)
    {
      encoded = (encoded ^ (0xAAAAAAAAUL ^ static_cast<uint32_t>(TManchesterType::inversion_mask))) & 0x55555555UL;
      encoded = (encoded | (encoded >> 1)) & 0x33333333UL;
      encoded = (encoded | (encoded >> 2)) & 0x0F0F0F0FUL;
      encoded = (encoded | (encoded >> 4)) & 0x00FF00FFUL;
      decoded = static_cast<uint16_t>(encoded | (encoded >> 8U));
    }

#if ETL_USING_64BIT_TYPES
    //*************************************************************************
    /// Decode a uint64_t value in place to a uint32_t.
    ///\param encoded The value to decode.
    ///\param decoded The decoded value.
    //*************************************************************************
    template <typename TChunk>
    static ETL_CONSTEXPR14 typename etl::enable_if<etl::is_same<TChunk, uint64_t>::value, void>::type
    decode_in_place(TChunk encoded, uint32_t& decoded)
    {
      encoded = (encoded ^ (0xAAAAAAAAAAAAAAAAULL ^ TManchesterType::inversion_mask)) & 0x5555555555555555ULL;
      encoded = (encoded | (encoded >> 1)) & 0x3333333333333333ULL;
      encoded = (encoded | (encoded >> 2)) & 0x0F0F0F0F0F0F0F0FULL;
      encoded = (encoded | (encoded >> 4)) & 0x00FF00FF00FF00FFULL;
      encoded = (encoded | (encoded >> 8)) & 0x0000FFFF0000FFFFULL;
      decoded = static_cast<uint32_t>(encoded | (encoded >> 16U));
    }
#endif

    template <typename TChunk>
    static typename etl::enable_if<!private_manchester::is_decodable<TChunk>::value, void>::type
    decode_in_place(TChunk encoded, typename private_manchester::manchester_decoded<TChunk>::type& decoded) ETL_DELETE;

    //*************************************************************************
    /// Decode a value and return the Manchester decoded result.
    ///\param encoded The value to decode.
    ///\return The Manchester decoded value.
    //*************************************************************************
    template <typename TChunk>
    ETL_NODISCARD static ETL_CONSTEXPR14
      typename private_manchester::manchester_decoded<TChunk>::type
      decode(TChunk encoded)
    {
      ETL_STATIC_ASSERT(private_manchester::is_decodable<TChunk>::value, "TChunk must be a decodable type");

      typename private_manchester::manchester_decoded<TChunk>::type decoded = 0;
      decode_in_place(encoded, decoded);
      return decoded;
    }

    //*************************************************************************
    // Validation functions
    //*************************************************************************

    //*************************************************************************
    /// Validate that a value contains valid Manchester encoded data.
    ///\param encoded The encoded value to validate.
    ///\return True if the value is valid Manchester encoding.
    //*************************************************************************
    template <typename TChunk>
    ETL_NODISCARD static ETL_CONSTEXPR14
      typename etl::enable_if<private_manchester::is_decodable<TChunk>::value, bool>::type
      is_valid(TChunk encoded)
    {
      const TChunk mask = static_cast<TChunk>(0x5555555555555555ULL);
      return (((encoded ^ (encoded >> 1)) & mask) == mask);
    }

    //*************************************************************************
    /// Validate that a span contains valid Manchester encoded data.
    ///\param encoded The span of encoded data to validate.
    ///\return True if all data is valid Manchester encoding.
    //*************************************************************************
    ETL_NODISCARD static ETL_CONSTEXPR14 bool is_valid(etl::span<const uint_least8_t> encoded)
    {
      ETL_ASSERT(encoded.size() % sizeof(uint16_t) == 0, ETL_ERROR(manchester_invalid_size));

      for (size_t i = 0; i < encoded.size(); i += 2)
      {
        const uint16_t chunk = static_cast<uint16_t>((encoded[i + 1] << 8) | encoded[i]);
        if (!is_valid<uint16_t>(chunk))
        {
          return false;
        }
      }

      return true;
    }

    //*************************************************************************
    // Span decoding functions
    //*************************************************************************

    //*************************************************************************
    /// Decode a span of data using specified chunk type.
    ///\param source      The source encoded data to decode.
    ///\param destination The destination buffer for decoded data.
    ///\tparam TChunk     The chunk type for decoding.
    //*************************************************************************
    template <typename TChunk>
    static typename etl::enable_if<!etl::is_same<TChunk, typename private_manchester::manchester_encoded<uint_least8_t>::type>::value, void>::type
    decode_span(etl::span<const uint_least8_t> source, etl::span<uint_least8_t> destination)
    {
      typedef typename private_manchester::manchester_decoded<TChunk>::type TDecoded;
      typedef TChunk                                                        TEncoded;

      ETL_ASSERT(destination.size() * 2 >= source.size(), ETL_ERROR(manchester_invalid_size));
      ETL_ASSERT(source.size() % sizeof(TChunk) == 0, ETL_ERROR(manchester_invalid_size));

      size_t dest_index = 0;
      size_t source_index = 0;
      for (size_t i = 0; i < source.size() / sizeof(TEncoded); ++i)
      {
        TChunk encoded_value = 0;
        memcpy(&encoded_value, &source[source_index], sizeof(TEncoded));
        const TDecoded decoded_value = decode(encoded_value);
        memcpy(&destination[dest_index], &decoded_value, sizeof(TDecoded));

        source_index += sizeof(TEncoded);
        dest_index += sizeof(TDecoded);
      }
    }

    //*************************************************************************
    /// Decode a span of data using the smalles chunk type. This version is
    /// constexpr so that it can be used to decode data at compile time.
    ///\param source      The source encoded data to decode.
    ///\param destination The destination buffer for decoded data.
    ///\tparam TChunk     The chunk type for decoding (default type).
    //*************************************************************************
    template <typename TChunk = typename private_manchester::manchester_encoded<uint_least8_t>::type>
    static ETL_CONSTEXPR14 typename etl::enable_if<etl::is_same<TChunk, typename private_manchester::manchester_encoded<uint_least8_t>::type>::value, void>::type
    decode_span(etl::span<const uint_least8_t> source, etl::span<uint_least8_t> destination)
    {
      typedef uint_least8_t TDecoded;

      ETL_ASSERT(destination.size() * 2 >= source.size(), ETL_ERROR(manchester_invalid_size));
      ETL_ASSERT(source.size() % sizeof(TChunk) == 0, ETL_ERROR(manchester_invalid_size));

      size_t dest_index = 0;
      size_t source_index = 0;
      for (size_t i = 0; i < source.size() / sizeof(TChunk); ++i)
      {
        const TChunk encoded_value = static_cast<TChunk>((source[source_index + 1] << CHAR_BIT) | source[source_index]);
        destination[dest_index] = decode<TChunk>(encoded_value);

        source_index += sizeof(TChunk);
        dest_index += sizeof(TDecoded);
      }
    }

    //*************************************************************************
    /// Fast decode a span of data using pointer arithmetic.
    ///\param source      The source encoded data to decode.
    ///\param destination The destination buffer for decoded data.
    ///\tparam TChunk     The chunk size for decoding (default: uint16_t).
    //*************************************************************************
    template <typename TChunk = private_manchester::manchester_encoded<uint_least8_t>::type>
    static void decode_span_fast(etl::span<const uint_least8_t> source, etl::span<uint_least8_t> destination)
    {
      ETL_ASSERT(destination.size() * 2 >= source.size(), ETL_ERROR(manchester_invalid_size));
      ETL_ASSERT(source.size() % sizeof(TChunk) == 0, ETL_ERROR(manchester_invalid_size));

      while (!source.empty())
      {
        const TChunk&                                                       encoded = *reinterpret_cast<const TChunk*>(source.data());
        typename etl::private_manchester::manchester_decoded<TChunk>::type& decoded = *reinterpret_cast<typename etl::private_manchester::manchester_decoded<TChunk>::type*>(destination.data());

        decode_in_place(encoded, decoded);

        source.advance(sizeof(TChunk));
        destination.advance(sizeof(typename etl::private_manchester::manchester_decoded<TChunk>::type));
      }
    }
  };

  //***************************************************************************
  ///\ingroup manchester
  /// Manchester encoder using normal encoding (no inversion).
  //***************************************************************************
  typedef manchester_base<private_manchester::manchester_type_normal> manchester;

  //***************************************************************************
  ///\ingroup manchester
  /// Manchester encoder using inverted encoding.
  //***************************************************************************
  typedef manchester_base<private_manchester::manchester_type_inverted> manchester_inverted;

}  // namespace etl

#endif
