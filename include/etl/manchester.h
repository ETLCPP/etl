#ifndef ETL_MANCHESTER_INCLUDED
#define ETL_MANCHESTER_INCLUDED

// #include <etl/span.h>
// #include <stdint.h>

// namespace manchester
// {
//     void encode(const etl::span<const uint8_t> &input, const etl::span<uint16_t> &output);
//     bool decode(const etl::span<const uint16_t> &input, const etl::span<uint8_t> &output);
// }

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

#include <etl/limits.h>
#include <etl/platform.h>
#include <etl/span.h>
#include <etl/type_traits.h>

namespace etl
{
    //***************************************************************************
    /// Manchester encoding and decoding
    //***************************************************************************

    template <typename T>
    struct manchester_encoded
    {
        static_assert(sizeof(T) == 0, "Manchester encoding type should be one of [uint8_t, uint16_t, uint32_t]");
    };

    template <>
    struct manchester_encoded<uint8_t>
    {
        using type = uint16_t;
    };
    template <>
    struct manchester_encoded<uint16_t>
    {
        using type = uint32_t;
    };
    template <>
    struct manchester_encoded<uint32_t>
    {
        using type = uint64_t;
    };

    template <typename T>
    struct manchester_decoded
    {
        static_assert(sizeof(T) == 0, "Manchester decoding type should be one of [uint16_t, uint32_t, uint64_t]");
    };

    template <>
    struct manchester_decoded<uint16_t>
    {
        using type = uint8_t;
    };
    template <>
    struct manchester_decoded<uint32_t>
    {
        using type = uint16_t;
    };
    template <>
    struct manchester_decoded<uint64_t>
    {
        using type = uint32_t;
    };

    template <typename TEncode>
    ETL_CONSTEXPR14 void manchester_encode_in_place(TEncode in, typename manchester_encoded<TEncode>::type& out) ETL_DELETE;

    template <typename TEncode>
    ETL_NODISCARD ETL_CONSTEXPR14 typename manchester_encoded<TEncode>::type manchester_encode(TEncode in) ETL_DELETE;

#if ETL_USING_8BIT_TYPES
    //*****************************************************************************
    /// Manchester encode 8 bits
    ///\ingroup manchester
    //*****************************************************************************
    template <>
    ETL_CONSTEXPR void manchester_encode_in_place<uint8_t>(uint8_t in, uint16_t& out)
    {
        out = in;

        out = (out | (out << 4U)) & 0x0F0FU;
        out = (out | (out << 2U)) & 0x3333U;
        out = (out | (out << 1U)) & 0x5555U;
        out = (out | (out << 1U)) ^ 0xAAAAU;
    }

    template <>
    ETL_NODISCARD ETL_CONSTEXPR14 uint16_t manchester_encode<uint8_t>(uint8_t in)
    {
        uint16_t out {};
        manchester_encode_in_place(in, out);
        return out;
    }
#endif

    //*****************************************************************************
    /// Manchester encode 16 bits
    ///\ingroup manchester
    //*****************************************************************************
    template <>
    ETL_CONSTEXPR void manchester_encode_in_place(uint16_t in, uint32_t& out)
    {
        out = in;

        out = (out | (out << 8U)) & 0x00FF00FFUL;
        out = (out | (out << 4U)) & 0x0F0F0F0FUL;
        out = (out | (out << 2U)) & 0x33333333UL;
        out = (out | (out << 1U)) & 0x55555555UL;
        out = (out | (out << 1U)) ^ 0xAAAAAAAAUL;
    }

    template <>
    ETL_NODISCARD ETL_CONSTEXPR14 uint32_t manchester_encode(uint16_t in)
    {
        uint32_t out {};
        manchester_encode_in_place(in, out);
        return out;
    }

#if ETL_USING_64BIT_TYPES
    //*****************************************************************************
    /// Manchester encode 32 bits
    ///\ingroup manchester
    //*****************************************************************************
    template <>
    ETL_CONSTEXPR void manchester_encode_in_place(uint32_t in, uint64_t& out)
    {
        out = in;

        out = (out | (out << 16U)) & 0x0000FFFF0000FFFFULL;
        out = (out | (out << 8U)) & 0x00FF00FF00FF00FFULL;
        out = (out | (out << 4U)) & 0x0F0F0F0F0F0F0F0FULL;
        out = (out | (out << 2U)) & 0x3333333333333333ULL;
        out = (out | (out << 1U)) & 0x5555555555555555ULL;
        out = (out | (out << 1U)) ^ 0xAAAAAAAAAAAAAAAAULL;
    }

    template <>
    ETL_NODISCARD ETL_CONSTEXPR14 uint64_t manchester_encode(uint32_t in)
    {
        uint64_t out {};
        manchester_encode_in_place(in, out);
        return out;
    }
#endif

    // What happens on systems that don't have uint8_t / where CHAR_BIT is e.g. 16?
    template <typename TEncode = uint_least8_t>
    void manchester_encode_span(etl::span<const uint_least8_t> input, etl::span<uint_least8_t> output)
    {
        ETL_ASSERT(output.size() >= input.size() * 2, "");
        ETL_ASSERT(input.size() % sizeof(TEncode) == 0, "");

        while (!input.empty())
        {
            const TEncode& in = *reinterpret_cast<const TEncode*>(input.data());
            typename etl::manchester_encoded<TEncode>::type& out = *reinterpret_cast<typename etl::manchester_encoded<TEncode>::type*>(output.data());

            etl::manchester_encode_in_place(in, out);

            input.advance(sizeof(TEncode));
            output.advance(sizeof(typename etl::manchester_encoded<TEncode>::type));
        }
    }


    template <typename TDecode>
    ETL_CONSTEXPR void manchester_decode_in_place(TDecode in, typename manchester_decoded<TDecode>::type& out) ETL_DELETE;

    template <typename TDecode>
    ETL_NODISCARD ETL_CONSTEXPR14 typename manchester_decoded<TDecode>::type manchester_decode(TDecode in) ETL_DELETE;

    template <typename TDecode>
    ETL_NODISCARD ETL_CONSTEXPR bool manchester_valid(TDecode encoded) ETL_DELETE;

#if ETL_USING_8BIT_TYPES
    //*****************************************************************************
    /// Manchester decode 16 bits
    ///\ingroup manchester
    //*****************************************************************************
    template<>
    ETL_CONSTEXPR void manchester_decode_in_place(uint16_t in, uint8_t& out)
    {
        in = (in ^ 0xAAAAU) & 0x5555U;
        in = (in | (in >> 1)) & 0x3333U;
        in = (in | (in >> 2)) & 0x0F0FU;
        out = static_cast<uint8_t>(in | (in >> 4U));
    }

    template<>
    ETL_NODISCARD ETL_NODISCARD ETL_CONSTEXPR14 uint8_t manchester_decode(uint16_t in)
    {
        uint8_t out {};
        manchester_decode_in_place(in, out);
        return out;
    }
#endif

    //*****************************************************************************
    /// Manchester valid 16 bits
    ///\ingroup manchester
    //*****************************************************************************
    template <>
    ETL_NODISCARD ETL_CONSTEXPR bool manchester_valid(uint16_t encoded)
    {
        return (((encoded ^ (encoded >> 1)) & 0x5555U) == 0x5555U);
    }

    //*****************************************************************************
    /// Manchester decode 32 bits
    ///\ingroup manchester
    //*****************************************************************************
    template<>
    ETL_CONSTEXPR void manchester_decode_in_place(uint32_t in, uint16_t& out)
    {
        in = (in ^ 0xAAAAAAAAUL) & 0x55555555UL;
        in = (in | (in >> 1)) & 0x33333333UL;
        in = (in | (in >> 2)) & 0x0F0F0F0FUL;
        in = (in | (in >> 4)) & 0x00FF00FFUL;
        out = static_cast<uint16_t>(in | (in >> 8U));
    }

    template<>
    ETL_NODISCARD ETL_CONSTEXPR14 uint16_t manchester_decode(uint32_t in)
    {
        uint16_t out {};
        manchester_decode_in_place(in, out);
        return out;
    }

    //*****************************************************************************
    /// Manchester valid 32 bits
    ///\ingroup manchester
    //*****************************************************************************
    template <>
    ETL_NODISCARD ETL_CONSTEXPR bool manchester_valid(uint32_t encoded)
    {
        return (((encoded ^ (encoded >> 1)) & 0x55555555U) == 0x55555555U);
    }

    //*****************************************************************************
    /// Manchester decode 64 bits
    ///\ingroup manchester
    //*****************************************************************************
    template<>
    ETL_CONSTEXPR void manchester_decode_in_place(uint64_t in, uint32_t& out)
    {
        in = (in ^ 0xAAAAAAAAAAAAAAAAULL) & 0x5555555555555555ULL;
        in = (in | (in >> 1)) & 0x3333333333333333ULL;
        in = (in | (in >> 2)) & 0x0F0F0F0F0F0F0F0FULL;
        in = (in | (in >> 4)) & 0x00FF00FF00FF00FFULL;
        in = (in | (in >> 8)) & 0x0000FFFF0000FFFFULL;
        out = static_cast<uint32_t>(in | (in >> 16U));
    }

    template<>
    ETL_NODISCARD ETL_CONSTEXPR14 uint32_t manchester_decode(uint64_t in)
    {
        uint32_t out {};
        manchester_decode_in_place(in, out);
        return out;
    }

    //*****************************************************************************
    /// Manchester valid 64 bits
    ///\ingroup manchester
    //*****************************************************************************
    template <>
    ETL_NODISCARD ETL_CONSTEXPR bool manchester_valid(uint64_t encoded)
    {
        return (((encoded ^ (encoded >> 1)) & 0x5555555555555555U) == 0x5555555555555555U);
    }

    template <typename TDecode = manchester_encoded<uint_least8_t>::type>
    void manchester_decode_span(etl::span<const uint_least8_t> input, etl::span<uint_least8_t> output)
    {
        ETL_ASSERT(input.size() >= output.size() * 2, "");
        ETL_ASSERT(input.size() % sizeof(TDecode) == 0, "");

        while (!input.empty())
        {
            const TDecode& in = *reinterpret_cast<const TDecode*>(input.data());
            typename etl::manchester_decoded<TDecode>::type& out = *reinterpret_cast<typename etl::manchester_decoded<TDecode>::type*>(output.data());

            etl::manchester_decode_in_place(in, out);

            input.advance(sizeof(TDecode));
            output.advance(sizeof(typename etl::manchester_decoded<TDecode>::type));
        }
    }

    ETL_CONSTEXPR14 void manchester_decode_span_constexpr(etl::span<const uint_least8_t> input, etl::span<uint_least8_t> output)
    {
        ETL_ASSERT(input.size() >= output.size() * 2, "");
        ETL_ASSERT(input.size() % sizeof(uint16_t) == 0, "");

        for (size_t i = 0; i < output.size(); ++i)
        {
            const uint16_t encoded = (static_cast<uint16_t>(input[i]) << 8) | input[i + 1];
            output[i] = etl::manchester_decode<uint16_t>(encoded);
        }
    }

} // namespace etl

#endif
