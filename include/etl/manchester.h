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
    template <typename TChunk>
    static ETL_CONSTEXPR14 void encode_in_place(TChunk in, typename manchester_encoded<TChunk>::type& out) ETL_DELETE;

    template <typename TChunk>
    ETL_NODISCARD static ETL_CONSTEXPR14 typename manchester_encoded<TChunk>::type encode(TChunk in) ETL_DELETE;

#if ETL_USING_8BIT_TYPES
    //*****************************************************************************
    /// Manchester encode 8 bits
    ///\ingroup manchester
    //*****************************************************************************
    template <>
    static ETL_CONSTEXPR14 void encode_in_place<uint8_t>(uint8_t in, uint16_t& out)
    {
      out = in;

      out = (out | (out << 4U)) & 0x0F0FU;
      out = (out | (out << 2U)) & 0x3333U;
      out = (out | (out << 1U)) & 0x5555U;
      out = (out | (out << 1U)) ^ (0xAAAAU ^ static_cast<uint16_t>(TType::invert_mask));
    }

    template <>
    ETL_NODISCARD static ETL_CONSTEXPR14 uint16_t encode<uint8_t>(uint8_t in)
    {
      uint16_t out{};
      encode_in_place(in, out);
      return out;
    }
#endif

    //*****************************************************************************
    /// Manchester encode 16 bits
    ///\ingroup manchester
    //*****************************************************************************
    template <>
    static ETL_CONSTEXPR14 void encode_in_place(uint16_t in, uint32_t& out)
    {
      out = in;

      out = (out | (out << 8U)) & 0x00FF00FFUL;
      out = (out | (out << 4U)) & 0x0F0F0F0FUL;
      out = (out | (out << 2U)) & 0x33333333UL;
      out = (out | (out << 1U)) & 0x55555555UL;
      out = (out | (out << 1U)) ^ (0xAAAAAAAAUL ^ static_cast<uint32_t>(TType::invert_mask));
    }

    template <>
    ETL_NODISCARD static ETL_CONSTEXPR14 uint32_t encode(uint16_t in)
    {
      uint32_t out{};
      encode_in_place(in, out);
      return out;
    }

#if ETL_USING_64BIT_TYPES
    //*****************************************************************************
    /// Manchester encode 32 bits
    ///\ingroup manchester
    //*****************************************************************************
    template <>
    static ETL_CONSTEXPR14 void encode_in_place(uint32_t in, uint64_t& out)
    {
      out = in;

      out = (out | (out << 16U)) & 0x0000FFFF0000FFFFULL;
      out = (out | (out << 8U)) & 0x00FF00FF00FF00FFULL;
      out = (out | (out << 4U)) & 0x0F0F0F0F0F0F0F0FULL;
      out = (out | (out << 2U)) & 0x3333333333333333ULL;
      out = (out | (out << 1U)) & 0x5555555555555555ULL;
      out = (out | (out << 1U)) ^ (0xAAAAAAAAAAAAAAAAULL ^ TType::invert_mask);
    }

    template <>
    ETL_NODISCARD static ETL_CONSTEXPR14 uint64_t encode(uint32_t in)
    {
      uint64_t out{};
      encode_in_place(in, out);
      return out;
    }
#endif

    // What happens on systems that don't have uint8_t / where CHAR_BIT is e.g. 16?
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

    template <typename TChunk>
    static ETL_CONSTEXPR14 void decode_in_place(TChunk in, typename manchester_decoded<TChunk>::type& out) ETL_DELETE;

    template <typename TChunk>
    ETL_NODISCARD static ETL_CONSTEXPR14 typename manchester_decoded<TChunk>::type decode(TChunk in) ETL_DELETE;

    template <typename TChunk>
    ETL_NODISCARD static ETL_CONSTEXPR bool valid(TChunk encoded) ETL_DELETE;

#if ETL_USING_8BIT_TYPES
    //*****************************************************************************
    /// Manchester decode 16 bits
    ///\ingroup manchester
    //*****************************************************************************
    template <>
    static ETL_CONSTEXPR14 void decode_in_place(uint16_t in, uint8_t& out)
    {
      in = (in ^ (0xAAAAU ^ static_cast<uint16_t>(TType::invert_mask))) & 0x5555U;
      in = (in | (in >> 1)) & 0x3333U;
      in = (in | (in >> 2)) & 0x0F0FU;
      out = static_cast<uint8_t>(in | (in >> 4U));
    }

    template <>
    ETL_NODISCARD static ETL_CONSTEXPR14 uint8_t decode(uint16_t in)
    {
      uint8_t out{};
      decode_in_place(in, out);
      return out;
    }
#endif

    //*****************************************************************************
    /// Manchester valid 16 bits
    ///\ingroup manchester
    //*****************************************************************************
    template <>
    ETL_NODISCARD static ETL_CONSTEXPR bool valid(uint16_t encoded)
    {
      return (((encoded ^ (encoded >> 1)) & 0x5555U) == 0x5555U);
    }

    //*****************************************************************************
    /// Manchester decode 32 bits
    ///\ingroup manchester
    //*****************************************************************************
    template <>
    static ETL_CONSTEXPR14 void decode_in_place(uint32_t in, uint16_t& out)
    {
      in = (in ^ (0xAAAAAAAAUL ^ static_cast<uint32_t>(TType::invert_mask))) & 0x55555555UL;
      in = (in | (in >> 1)) & 0x33333333UL;
      in = (in | (in >> 2)) & 0x0F0F0F0FUL;
      in = (in | (in >> 4)) & 0x00FF00FFUL;
      out = static_cast<uint16_t>(in | (in >> 8U));
    }

    template <>
    ETL_NODISCARD static ETL_CONSTEXPR14 uint16_t decode(uint32_t in)
    {
      uint16_t out{};
      decode_in_place(in, out);
      return out;
    }

    //*****************************************************************************
    /// Manchester valid 32 bits
    ///\ingroup manchester
    //*****************************************************************************
    template <>
    ETL_NODISCARD static ETL_CONSTEXPR bool valid(uint32_t encoded)
    {
      return (((encoded ^ (encoded >> 1)) & 0x55555555U) == 0x55555555U);
    }

#if ETL_USING_64BIT_TYPES
    //*****************************************************************************
    /// Manchester decode 64 bits
    ///\ingroup manchester
    //*****************************************************************************
    template <>
    static ETL_CONSTEXPR14 void decode_in_place(uint64_t in, uint32_t& out)
    {
      in = (in ^ (0xAAAAAAAAAAAAAAAAULL ^ TType::invert_mask)) & 0x5555555555555555ULL;
      in = (in | (in >> 1)) & 0x3333333333333333ULL;
      in = (in | (in >> 2)) & 0x0F0F0F0F0F0F0F0FULL;
      in = (in | (in >> 4)) & 0x00FF00FF00FF00FFULL;
      in = (in | (in >> 8)) & 0x0000FFFF0000FFFFULL;
      out = static_cast<uint32_t>(in | (in >> 16U));
    }

    template <>
    ETL_NODISCARD static ETL_CONSTEXPR14 uint32_t decode(uint64_t in)
    {
      uint32_t out{};
      decode_in_place(in, out);
      return out;
    }

    //*****************************************************************************
    /// Manchester valid 64 bits
    ///\ingroup manchester
    //*****************************************************************************
    template <>
    ETL_NODISCARD static ETL_CONSTEXPR bool valid(uint64_t encoded)
    {
      return (((encoded ^ (encoded >> 1)) & 0x5555555555555555U) == 0x5555555555555555U);
    }
#endif

    static ETL_CONSTEXPR14 bool valid_span(etl::span<const uint_least8_t> encoded)
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

    template <typename TChunk = manchester_encoded<uint_least8_t>::type>
    static void decode_span(etl::span<const uint_least8_t> source, etl::span<uint_least8_t> destination)
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

    template <>
    static ETL_CONSTEXPR14 void decode_span<typename manchester_encoded<uint_least8_t>::type>(etl::span<const uint_least8_t> source, etl::span<uint_least8_t> destination)
    {
      typedef typename manchester_encoded<uint_least8_t>::type TChunk;
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
