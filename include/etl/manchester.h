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

namespace etl
{
  //***************************************************************************
  /// Manchester encoding and decoding
  //***************************************************************************

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

  template <typename TChunk>
  struct is_supported_encode_input
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

  template <typename TChunk>
  struct is_supported_decode_input
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

  struct manchester_type_normal
  {
    static const uint64_t invert_mask = 0x0000000000000000;
  };

  struct manchester_type_inverted
  {
    static const uint64_t invert_mask = 0xFFFFFFFFFFFFFFFF;
  };

  template <typename TType>
  struct manchester_base
  {
#if ETL_USING_8BIT_TYPES
    template <typename TChunk>
    static ETL_CONSTEXPR14 typename etl::enable_if<etl::is_same<TChunk, uint8_t>::value, void>::type
    encode_in_place(TChunk in, uint16_t& out)
    {
      out = in;

      out = (out | (out << 4U)) & 0x0F0FU;
      out = (out | (out << 2U)) & 0x3333U;
      out = (out | (out << 1U)) & 0x5555U;
      out = (out | (out << 1U)) ^ (0xAAAAU ^ static_cast<uint16_t>(TType::invert_mask));
    }
#endif

    template <typename TChunk>
    static ETL_CONSTEXPR14 typename etl::enable_if<etl::is_same<TChunk, uint16_t>::value, void>::type
    encode_in_place(TChunk in, uint32_t& out)
    {
      out = in;

      out = (out | (out << 8U)) & 0x00FF00FFUL;
      out = (out | (out << 4U)) & 0x0F0F0F0FUL;
      out = (out | (out << 2U)) & 0x33333333UL;
      out = (out | (out << 1U)) & 0x55555555UL;
      out = (out | (out << 1U)) ^ (0xAAAAAAAAUL ^ static_cast<uint32_t>(TType::invert_mask));
    }

#if ETL_USING_64BIT_TYPES
    template <typename TChunk>
    static ETL_CONSTEXPR14 typename etl::enable_if<etl::is_same<TChunk, uint32_t>::value, void>::type
    encode_in_place(TChunk in, uint64_t& out)
    {
      out = in;

      out = (out | (out << 16U)) & 0x0000FFFF0000FFFFULL;
      out = (out | (out << 8U)) & 0x00FF00FF00FF00FFULL;
      out = (out | (out << 4U)) & 0x0F0F0F0F0F0F0F0FULL;
      out = (out | (out << 2U)) & 0x3333333333333333ULL;
      out = (out | (out << 1U)) & 0x5555555555555555ULL;
      out = (out | (out << 1U)) ^ (0xAAAAAAAAAAAAAAAAULL ^ TType::invert_mask);
    }
#endif

    template <typename TChunk>
    static typename etl::enable_if<!is_supported_encode_input<TChunk>::value, void>::type
    encode_in_place(TChunk in, typename manchester_encoded<TChunk>::type& out) = delete;

    template <typename TChunk>
    ETL_NODISCARD static ETL_CONSTEXPR14 typename etl::enable_if<is_supported_encode_input<TChunk>::value, typename manchester_encoded<TChunk>::type>::type
    encode(TChunk in)
    {
      typename manchester_encoded<TChunk>::type out = 0;
      encode_in_place(in, out);
      return out;
    }

    template <typename TChunk>
    static typename etl::enable_if<!is_supported_encode_input<TChunk>::value, typename manchester_encoded<TChunk>::type>::type
    encode(TChunk in) = delete;

    template <typename TChunk = uint_least8_t>
    static void encode_span(etl::span<const uint_least8_t> source, etl::span<uint_least8_t> destination)
    {
      ETL_ASSERT(destination.size() >= source.size() * 2, "Manchester encoding requires destination storage to be at least twice the size of the source storage");
      ETL_ASSERT(source.size() % sizeof(TChunk) == 0, "Manchester encoding requires the source storage size to be an integer multiple of the encoding chunk size");

      while (!source.empty())
      {
        const TChunk&                                   in = *reinterpret_cast<const TChunk*>(source.data());
        typename etl::manchester_encoded<TChunk>::type& out = *reinterpret_cast<typename etl::manchester_encoded<TChunk>::type*>(destination.data());

        encode_in_place(in, out);

        source.advance(sizeof(TChunk));
        destination.advance(sizeof(typename etl::manchester_encoded<TChunk>::type));
      }
    }

#if ETL_USING_8BIT_TYPES
    template <typename TChunk>
    static ETL_CONSTEXPR14 typename etl::enable_if<etl::is_same<TChunk, uint16_t>::value, void>::type
    decode_in_place(TChunk in, uint8_t& out)
    {
      in = (in ^ (0xAAAAU ^ static_cast<uint16_t>(TType::invert_mask))) & 0x5555U;
      in = (in | (in >> 1)) & 0x3333U;
      in = (in | (in >> 2)) & 0x0F0FU;
      out = static_cast<uint8_t>(in | (in >> 4U));
    }
#endif

    template <typename TChunk>
    static ETL_CONSTEXPR14 typename etl::enable_if<etl::is_same<TChunk, uint32_t>::value, void>::type
    decode_in_place(TChunk in, uint16_t& out)
    {
      in = (in ^ (0xAAAAAAAAUL ^ static_cast<uint32_t>(TType::invert_mask))) & 0x55555555UL;
      in = (in | (in >> 1)) & 0x33333333UL;
      in = (in | (in >> 2)) & 0x0F0F0F0FUL;
      in = (in | (in >> 4)) & 0x00FF00FFUL;
      out = static_cast<uint16_t>(in | (in >> 8U));
    }

#if ETL_USING_64BIT_TYPES
    template <typename TChunk>
    static ETL_CONSTEXPR14 typename etl::enable_if<etl::is_same<TChunk, uint64_t>::value, void>::type
    decode_in_place(TChunk in, uint32_t& out)
    {
      in = (in ^ (0xAAAAAAAAAAAAAAAAULL ^ TType::invert_mask)) & 0x5555555555555555ULL;
      in = (in | (in >> 1)) & 0x3333333333333333ULL;
      in = (in | (in >> 2)) & 0x0F0F0F0F0F0F0F0FULL;
      in = (in | (in >> 4)) & 0x00FF00FF00FF00FFULL;
      in = (in | (in >> 8)) & 0x0000FFFF0000FFFFULL;
      out = static_cast<uint32_t>(in | (in >> 16U));
    }
#endif

    template <typename TChunk>
    static typename etl::enable_if<!is_supported_decode_input<TChunk>::value, void>::type
    decode_in_place(TChunk in, typename manchester_decoded<TChunk>::type& out) = delete;

    template <typename TChunk>
    ETL_NODISCARD static ETL_CONSTEXPR14 typename etl::enable_if<is_supported_decode_input<TChunk>::value, typename manchester_decoded<TChunk>::type>::type
    decode(TChunk in)
    {
      typename manchester_decoded<TChunk>::type out = 0;
      decode_in_place(in, out);
      return out;
    }

    template <typename TChunk>
    static typename etl::enable_if<!is_supported_decode_input<TChunk>::value, typename manchester_decoded<TChunk>::type>::type
    decode(TChunk in) = delete;

#if ETL_USING_8BIT_TYPES
    template <typename TChunk>
    ETL_NODISCARD static ETL_CONSTEXPR typename etl::enable_if<etl::is_same<TChunk, uint16_t>::value, bool>::type
    valid(TChunk encoded)
    {
      return (((encoded ^ (encoded >> 1)) & 0x5555U) == 0x5555U);
    }
#endif

    template <typename TChunk>
    ETL_NODISCARD static ETL_CONSTEXPR typename etl::enable_if<etl::is_same<TChunk, uint32_t>::value, bool>::type
    valid(TChunk encoded)
    {
      return (((encoded ^ (encoded >> 1)) & 0x55555555U) == 0x55555555U);
    }

#if ETL_USING_64BIT_TYPES
    template <typename TChunk>
    ETL_NODISCARD static ETL_CONSTEXPR typename etl::enable_if<etl::is_same<TChunk, uint64_t>::value, bool>::type
    valid(TChunk encoded)
    {
      return (((encoded ^ (encoded >> 1)) & 0x5555555555555555U) == 0x5555555555555555U);
    }
#endif

    template <typename TChunk>
    static typename etl::enable_if<!is_supported_decode_input<TChunk>::value, bool>::type
    valid(TChunk encoded) = delete;

    ETL_NODISCARD static ETL_CONSTEXPR14 bool valid_span(etl::span<const uint_least8_t> encoded)
    {
      ETL_ASSERT(encoded.size() % sizeof(uint16_t) == 0, "");

      for (size_t i = 0; i < encoded.size(); i += 2)
      {
        const uint16_t chunk = static_cast<uint16_t>((encoded[i + 1] << 8) | encoded[i]);
        if (!valid<uint16_t>(chunk))
        {
          return false;
        }
      }

      return true;
    }

    template <typename TChunk>
    static typename etl::enable_if<!etl::is_same<TChunk, typename manchester_encoded<uint_least8_t>::type>::value, void>::type
    decode_span(etl::span<const uint_least8_t> source, etl::span<uint_least8_t> destination)
    {
      typedef typename manchester_decoded<TChunk>::type TChunkDecoded;

      ETL_ASSERT(destination.size() * 2 >= source.size(), "Manchester decoding requires destination storage to be no less than half the source storage");
      ETL_ASSERT(source.size() % sizeof(TChunk) == 0, "Manchester decoding requires the source storage size to be an integer multiple of the decoding chunk size");

      size_t dest_index = 0;
      size_t source_index = 0;
      for (size_t i = 0; i < source.size() / sizeof(TChunk); ++i)
      {
        TChunk encoded = 0;
        memcpy(&encoded, &source[source_index], sizeof(TChunk));
        const TChunkDecoded decoded = decode(encoded);
        memcpy(&destination[dest_index], &decoded, sizeof(TChunkDecoded));

        source_index += sizeof(TChunk);
        dest_index += sizeof(TChunkDecoded);
      }
    }

    template <typename TChunk = typename manchester_encoded<uint_least8_t>::type>
    static ETL_CONSTEXPR14 typename etl::enable_if<etl::is_same<TChunk, typename manchester_encoded<uint_least8_t>::type>::value, void>::type
    decode_span(etl::span<const uint_least8_t> source, etl::span<uint_least8_t> destination)
    {
      typedef uint_least8_t TChunkDecoded;

      ETL_ASSERT(destination.size() * 2 >= source.size(), "Manchester decoding requires destination storage to be no less than half the source storage");
      ETL_ASSERT(source.size() % sizeof(TChunk) == 0, "Manchester decoding requires the source storage size to be an integer multiple of the decoding chunk size");

      size_t dest_index = 0;
      size_t source_index = 0;
      for (size_t i = 0; i < source.size() / sizeof(TChunk); ++i)
      {
        const TChunk encoded = static_cast<TChunk>((source[source_index + 1] << 8) | source[source_index]);
        destination[dest_index] = decode<TChunk>(encoded);

        source_index += sizeof(TChunk);
        dest_index += sizeof(TChunkDecoded);
      }
    }

    template <typename TChunk = manchester_encoded<uint_least8_t>::type>
    static void decode_span_fast(etl::span<const uint_least8_t> source, etl::span<uint_least8_t> destination)
    {
      ETL_ASSERT(destination.size() * 2 >= source.size(), "Manchester decoding requires destination storage to be no less than half the source storage");
      ETL_ASSERT(source.size() % sizeof(TChunk) == 0, "Manchester decoding requires the source storage size to be an integer multiple of the decoding chunk size");

      while (!source.empty())
      {
        const TChunk&                                   in = *reinterpret_cast<const TChunk*>(source.data());
        typename etl::manchester_decoded<TChunk>::type& out = *reinterpret_cast<typename etl::manchester_decoded<TChunk>::type*>(destination.data());

        decode_in_place(in, out);

        source.advance(sizeof(TChunk));
        destination.advance(sizeof(typename etl::manchester_decoded<TChunk>::type));
      }
    }
  };

  typedef manchester_base<manchester_type_normal>   manchester;
  typedef manchester_base<manchester_type_inverted> manchester_inverted;

}  // namespace etl

#endif
