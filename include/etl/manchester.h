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

#if ETL_USING_8BIT_TYPES
    //*****************************************************************************
    /// Manchester encode 8 bits
    ///\ingroup manchester
    //*****************************************************************************
    template <typename TInput, typename TOutput = uint16_t>
    ETL_CONSTEXPR
        typename etl::enable_if<etl::is_integral<TInput>::value && etl::is_unsigned<TInput>::value && (etl::integral_limits<TInput>::bits == 8U) &&
                                    etl::is_integral<TOutput>::value && etl::is_unsigned<TOutput>::value && (etl::integral_limits<TOutput>::bits == 16U),
                                TOutput>::type
        manchester_encode(TInput in)
    {
        TOutput out = in;

        out = (out | (out << 4U)) & 0x0F0FU;
        out = (out | (out << 2U)) & 0x3333U;
        out = (out | (out << 1U)) & 0x5555U;
        return (out | (out << 1U)) ^ 0xAAAAU;
    }
#endif

    //*****************************************************************************
    /// Manchester encode 16 bits
    ///\ingroup manchester
    //*****************************************************************************
    template <typename TInput, typename TOutput = uint32_t>
    ETL_CONSTEXPR
        typename etl::enable_if<etl::is_integral<TInput>::value && etl::is_unsigned<TInput>::value && (etl::integral_limits<TInput>::bits == 16U) &&
                                    etl::is_integral<TOutput>::value && etl::is_unsigned<TOutput>::value && (etl::integral_limits<TOutput>::bits == 32U),
                                TOutput>::type
        manchester_encode(TInput in)
    {
        TOutput out = in;

        out = (out | (out << 8U)) & 0x00FF00FFUL;
        out = (out | (out << 4U)) & 0x0F0F0F0FUL;
        out = (out | (out << 2U)) & 0x33333333UL;
        out = (out | (out << 1U)) & 0x55555555UL;
        return (out | (out << 1U)) ^ 0xAAAAAAAAUL;
    }

#if ETL_USING_64BIT_TYPES
    //*****************************************************************************
    /// Manchester encode 32 bits
    ///\ingroup manchester
    //*****************************************************************************
    template <typename TInput, typename TOutput = uint64_t>
    ETL_CONSTEXPR
        typename etl::enable_if<etl::is_integral<TInput>::value && etl::is_unsigned<TInput>::value && (etl::integral_limits<TInput>::bits == 32U) &&
                                    etl::is_integral<TOutput>::value && etl::is_unsigned<TOutput>::value && (etl::integral_limits<TOutput>::bits == 64U),
                                TOutput>::type
        manchester_encode(TInput in)
    {
        TOutput out = in;

        out = (out | (out << 16U)) & 0x0000FFFF0000FFFFULL;
        out = (out | (out << 8U)) & 0x00FF00FF00FF00FFULL;
        out = (out | (out << 4U)) & 0x0F0F0F0F0F0F0F0FULL;
        out = (out | (out << 2U)) & 0x3333333333333333ULL;
        out = (out | (out << 1U)) & 0x5555555555555555ULL;
        return (out | (out << 1U)) ^ 0xAAAAAAAAAAAAAAAAULL;
    }
#endif

    template <typename T>
    struct select_uint;

    template <>
    struct select_uint<uint8_t>
    {
        using type = std::uint16_t;
    };
    template <>
    struct select_uint<uint16_t>
    {
        using type = std::uint32_t;
    };
    template <>
    struct select_uint<uint32_t>
    {
        using type = std::uint64_t;
    };

    template <typename TInput, typename TOutput = typename select_uint<TInput>::type, typename TEncode = TInput>
    typename etl::enable_if<etl::is_integral<TInput>::value && etl::is_unsigned<TInput>::value &&
                                etl::is_integral<TOutput>::value && etl::is_unsigned<TOutput>::value &&
                                etl::is_integral<TEncode>::value && etl::is_unsigned<TEncode>::value &&
                                (etl::integral_limits<TEncode>::bits <= 32) &&
                                (2 * etl::integral_limits<TInput>::bits == etl::integral_limits<TOutput>::bits),
                            void>::type
    manchester_encode_span(etl::span<const TInput> input, etl::span<TOutput> output)
    {
        using TOut = typename select_uint<TEncode>::type;

        ETL_ASSERT(output.size() >= input.size(), "");
        ETL_ASSERT((input.size() * sizeof(TInput)) % sizeof(TEncode) == 0, "");

        etl::span<const TEncode> in{reinterpret_cast<const TEncode *>(input.data()), (sizeof(TInput) * input.size()) / sizeof(TEncode)};
        etl::span<TOut> out{reinterpret_cast<TOut *>(output.data()), (sizeof(TOutput) * output.size()) / sizeof(TOut)};

        etl::transform(in.begin(), in.end(), out.begin(), etl::manchester_encode<TEncode>);
    }

#if ETL_USING_8BIT_TYPES
    //*****************************************************************************
    /// Manchester decode 16 bits
    ///\ingroup manchester
    //*****************************************************************************
    template <typename TInput, typename TOutput = uint8_t>
    ETL_CONSTEXPR
        typename etl::enable_if<etl::is_integral<TInput>::value && etl::is_unsigned<TInput>::value && (etl::integral_limits<TInput>::bits == 16U) &&
                                    etl::is_integral<TOutput>::value && etl::is_unsigned<TOutput>::value && (etl::integral_limits<TOutput>::bits == 8U),
                                TOutput>::type
        manchester_decode(TInput in)
    {
        TInput out = (in ^ 0xAAAAU) & 0x5555U;
        out = (out | (out >> 1)) & 0x3333U;
        out = (out | (out >> 2)) & 0x0F0FU;
        return static_cast<TOutput>(out | (out >> 4U));
    }
#endif

    //*****************************************************************************
    /// Manchester valid 16 bits
    ///\ingroup manchester
    //*****************************************************************************
    template <typename TInput>
    ETL_CONSTEXPR
        typename etl::enable_if<etl::is_integral<TInput>::value && etl::is_unsigned<TInput>::value && (etl::integral_limits<TInput>::bits == 16U), bool>::type
        manchester_valid(TInput in)
    {
        return (((in ^ (in >> 1)) & 0x5555U) == 0x5555U);
    }

    //*****************************************************************************
    /// Manchester decode 32 bits
    ///\ingroup manchester
    //*****************************************************************************
    template <typename TInput, typename TOutput = uint16_t>
    ETL_CONSTEXPR
        typename etl::enable_if<etl::is_integral<TInput>::value && etl::is_unsigned<TInput>::value && (etl::integral_limits<TInput>::bits == 32U) &&
                                    etl::is_integral<TOutput>::value && etl::is_unsigned<TOutput>::value && (etl::integral_limits<TOutput>::bits == 16U),
                                TOutput>::type
        manchester_decode(TInput in)
    {
        TInput out = (in ^ 0xAAAAAAAAUL) & 0x55555555UL;
        out = (out | (out >> 1)) & 0x33333333UL;
        out = (out | (out >> 2)) & 0x0F0F0F0FUL;
        out = (out | (out >> 4)) & 0x00FF00FFUL;
        return static_cast<TOutput>(out | (out >> 8U));
    }

    //*****************************************************************************
    /// Manchester valid 32 bits
    ///\ingroup manchester
    //*****************************************************************************
    template <typename TInput>
    ETL_CONSTEXPR
        typename etl::enable_if<etl::is_integral<TInput>::value && etl::is_unsigned<TInput>::value && (etl::integral_limits<TInput>::bits == 32U), bool>::type
        manchester_valid(TInput in)
    {
        return (((in ^ (in >> 1)) & 0x55555555U) == 0x55555555U);
    }

    //*****************************************************************************
    /// Manchester decode 64 bits
    ///\ingroup manchester
    //*****************************************************************************
    template <typename TInput, typename TOutput = uint32_t>
    ETL_CONSTEXPR
        typename etl::enable_if<etl::is_integral<TInput>::value && etl::is_unsigned<TInput>::value && (etl::integral_limits<TInput>::bits == 64U) &&
                                    etl::is_integral<TOutput>::value && etl::is_unsigned<TOutput>::value && (etl::integral_limits<TOutput>::bits == 32U),
                                TOutput>::type
        manchester_decode(TInput in)
    {
        TInput out = (in ^ 0xAAAAAAAAAAAAAAAAULL) & 0x5555555555555555ULL;
        out = (out | (out >> 1)) & 0x3333333333333333ULL;
        out = (out | (out >> 2)) & 0x0F0F0F0F0F0F0F0FULL;
        out = (out | (out >> 4)) & 0x00FF00FF00FF00FFULL;
        out = (out | (out >> 8)) & 0x0000FFFF0000FFFFULL;
        return static_cast<TOutput>(out | (out >> 16U));
    }

    //*****************************************************************************
    /// Manchester valid 64 bits
    ///\ingroup manchester
    //*****************************************************************************
    template <typename TInput>
    ETL_CONSTEXPR
        typename etl::enable_if<etl::is_integral<TInput>::value && etl::is_unsigned<TInput>::value && (etl::integral_limits<TInput>::bits == 64U), bool>::type
        manchester_valid(TInput in)
    {
        return (((in ^ (in >> 1)) & 0x5555555555555555U) == 0x5555555555555555U);
    }

} // namespace etl

#endif
