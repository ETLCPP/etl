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

#ifndef ETL_CRC_IMPLEMENTATION_INCLUDED
#define ETL_CRC_IMPLEMENTATION_INCLUDED

#include "../platform.h"
#include "../frame_check_sequence.h"
#include "../static_assert.h"
#include "../binary.h"
#include "../type_traits.h"

#include "stdint.h"

#include "crc_parameters.h"

#if defined(ETL_COMPILER_KEIL)
#pragma diag_suppress 1300
#endif

namespace etl
{
  namespace private_crc
  {
    //*****************************************************************************
    /// CRC Partial Table Entry
    //*****************************************************************************
    template <typename TAccumulator, size_t Accumulator_Bits, TAccumulator Polynomial, bool Reflect, TAccumulator Entry>
    class crc_partial_table_entry
    {
    private:

      static ETL_CONSTANT bool Do_Poly = Reflect ? (Entry & TAccumulator(1U)) != 0U 
                                                 : (Entry & (TAccumulator(1U) << (Accumulator_Bits - 1U))) != 0U;

    public:

      static ETL_CONSTANT TAccumulator value = Reflect ? TAccumulator(Do_Poly ? (Entry >> 1U) ^ etl::reverse_bits_const<TAccumulator, Polynomial>::value : (Entry >> 1U)) 
                                                       : TAccumulator(Do_Poly ? (Entry << 1U) ^ Polynomial : (Entry << 1U));
    };

    template <typename TAccumulator, size_t Accumulator_Bits, TAccumulator Polynomial, bool Reflect, TAccumulator Entry>
    ETL_CONSTANT TAccumulator crc_partial_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, Entry>::value;

    //*****************************************************************************
    /// CRC Table Entry
    //*****************************************************************************
    template <typename TAccumulator, size_t Accumulator_Bits, TAccumulator Polynomial, bool Reflect, size_t Index, uint8_t Chunk_Bits>
    class crc_table_entry
    {
      ETL_STATIC_ASSERT((Chunk_Bits == 2U) || (Chunk_Bits == 4U) || (Chunk_Bits == 8U), "Chunk bits must be 2, 4 or 8");
    };

    //*********************************
    // Chunk bit size of 2.
    template <typename TAccumulator, size_t Accumulator_Bits, TAccumulator Polynomial, bool Reflect, size_t Index>
    class crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, Index, 2U>
    {
    public:

      static ETL_CONSTANT size_t       Shift_Bits = size_t(Accumulator_Bits - 2U);
      static ETL_CONSTANT TAccumulator Entry      = Reflect ? TAccumulator(Index) : TAccumulator(TAccumulator(Index) << Shift_Bits);

      static ETL_CONSTANT TAccumulator value = crc_partial_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect,
                                               crc_partial_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, Entry>::value>::value;
    };

    template <typename TAccumulator, size_t Accumulator_Bits, TAccumulator Polynomial, bool Reflect, size_t Index>
    ETL_CONSTANT size_t crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, Index, 2U>::Shift_Bits;

    template <typename TAccumulator, size_t Accumulator_Bits, TAccumulator Polynomial, bool Reflect, size_t Index>
    ETL_CONSTANT TAccumulator crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, Index, 2U>::Entry;

    template <typename TAccumulator, size_t Accumulator_Bits, TAccumulator Polynomial, bool Reflect, size_t Index>
    ETL_CONSTANT TAccumulator crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, Index, 2U>::value;

    //*********************************
    // Chunk bit size of 4.
    template <typename TAccumulator, size_t Accumulator_Bits, TAccumulator Polynomial, bool Reflect, size_t Index>
    class crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, Index, 4U>
    {
    public:

      static ETL_CONSTANT size_t       Shift_Bits = size_t(Accumulator_Bits - 4U);
      static ETL_CONSTANT TAccumulator Entry      = Reflect ? TAccumulator(Index) : TAccumulator(TAccumulator(Index) << Shift_Bits);

      static ETL_CONSTANT TAccumulator value = crc_partial_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect,
                                               crc_partial_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect,
                                               crc_partial_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect,
                                               crc_partial_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, Entry>::value>::value>::value>::value;
    };

    template <typename TAccumulator, size_t Accumulator_Bits, TAccumulator Polynomial, bool Reflect, size_t Index>
    ETL_CONSTANT size_t crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, Index, 4U>::Shift_Bits;

    template <typename TAccumulator, size_t Accumulator_Bits, TAccumulator Polynomial, bool Reflect, size_t Index>
    ETL_CONSTANT TAccumulator crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, Index, 4U>::Entry;

    template <typename TAccumulator, size_t Accumulator_Bits, TAccumulator Polynomial, bool Reflect, size_t Index>
    ETL_CONSTANT TAccumulator crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, Index, 4U>::value;

    //*********************************
    // Chunk bit size of 8.
    template <typename TAccumulator, size_t Accumulator_Bits, TAccumulator Polynomial, bool Reflect, size_t Index>
    class crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, Index, 8U>
    {
    public:

      static ETL_CONSTANT size_t       Shift_Bits = size_t(Accumulator_Bits - 8U);
      static ETL_CONSTANT TAccumulator Entry      = Reflect ? TAccumulator(Index) : TAccumulator(TAccumulator(Index) << Shift_Bits);

      static ETL_CONSTANT TAccumulator value = crc_partial_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect,
                                               crc_partial_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect,
                                               crc_partial_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect,
                                               crc_partial_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect,
                                               crc_partial_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect,
                                               crc_partial_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect,
                                               crc_partial_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect,
                                               crc_partial_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, Entry>::value>::value>::value>::value>::value>::value>::value>::value;
    };

    template <typename TAccumulator, size_t Accumulator_Bits, TAccumulator Polynomial, bool Reflect, size_t Index>
    ETL_CONSTANT size_t crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, Index, 8U>::Shift_Bits;

    template <typename TAccumulator, size_t Accumulator_Bits, TAccumulator Polynomial, bool Reflect, size_t Index>
    ETL_CONSTANT TAccumulator crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, Index, 8U>::Entry;

    template <typename TAccumulator, size_t Accumulator_Bits, TAccumulator Polynomial, bool Reflect, size_t Index>
    ETL_CONSTANT TAccumulator crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, Index, 8U>::value;

    //*****************************************************************************
    /// CRC Update Chunk
    //*****************************************************************************
    
    //*********************************
    // Accumulator_Bits > Chunk_Bits
    // Not Reflected
    template <typename TAccumulator, size_t Accumulator_Bits, size_t Chunk_Bits, uint8_t Chunk_Mask, bool Reflect>
    static
    typename etl::enable_if<(Accumulator_Bits > Chunk_Bits) && !Reflect, TAccumulator>::type
      crc_update_chunk(TAccumulator crc, uint8_t value, const TAccumulator table[])
    {
      value &= Chunk_Mask;

      uint8_t index = (crc >> (Accumulator_Bits - Chunk_Bits)) ^ value;

      crc <<= Chunk_Bits;
      crc ^= table[index];

      return crc;
    }

    //*********************************
    // Accumulator_Bits > Chunk_Bits
    // Reflected
    template <typename TAccumulator, size_t Accumulator_Bits, size_t Chunk_Bits, uint8_t Chunk_Mask, bool Reflect>
    static
    typename etl::enable_if<(Accumulator_Bits > Chunk_Bits) && Reflect, TAccumulator>::type
      crc_update_chunk(TAccumulator crc, uint8_t value, const TAccumulator table[])
    {
      value &= Chunk_Mask;

      uint8_t index = (crc & Chunk_Mask) ^ value;

      crc >>= Chunk_Bits;
      crc ^= table[index];

      return crc;
    }

    //*********************************
    // Accumulator_Bits == Chunk_Bits
    // Not Reflected
    template <typename TAccumulator, size_t Accumulator_Bits, size_t Chunk_Bits, uint8_t Chunk_Mask, bool Reflect>
    static
    typename etl::enable_if<(Accumulator_Bits == Chunk_Bits) && !Reflect, TAccumulator>::type
      crc_update_chunk(TAccumulator crc, uint8_t value, const TAccumulator table[])
    {
      value &= Chunk_Mask;

      uint8_t index = (crc >> (Accumulator_Bits - Chunk_Bits)) ^ value;

      crc = table[index];

      return crc;
    }

    //*********************************
    // Accumulator_Bits == Chunk_Bits
    // Reflected
    template <typename TAccumulator, size_t Accumulator_Bits, size_t Chunk_Bits, uint8_t Chunk_Mask, bool Reflect>
    static
    typename etl::enable_if<(Accumulator_Bits == Chunk_Bits) && Reflect, TAccumulator>::type
      crc_update_chunk(TAccumulator crc, uint8_t value, const TAccumulator table[])
    {
      value &= Chunk_Mask;

      uint8_t index = (crc & Chunk_Mask) ^ value;

      crc = table[index];

      return crc;
    }

    //*****************************************************************************
    // CRC Tables.
    //*****************************************************************************
    template <typename TAccumulator, size_t Accumulator_Bits, size_t Chunk_Bits, uint8_t Chunk_Mask, TAccumulator Polynomial, bool Reflect, size_t Table_Size>
    struct crc_table;

    //*********************************
    // Table size of 4.
    template <typename TAccumulator, size_t Accumulator_Bits, size_t Chunk_Bits, uint8_t Chunk_Mask, TAccumulator Polynomial, bool Reflect>
    struct crc_table<TAccumulator, Accumulator_Bits, Chunk_Bits, Chunk_Mask, Polynomial, Reflect, 4U>
    {
      //*************************************************************************
      TAccumulator add(TAccumulator crc, uint8_t value) const
      {
        static ETL_CONSTANT TAccumulator table[4U] =
        {
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 0U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 1U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 2U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 3U, Chunk_Bits>::value
        };
        
        if ETL_IF_CONSTEXPR(Reflect)
        {
          crc = crc_update_chunk<TAccumulator, Accumulator_Bits, Chunk_Bits, Chunk_Mask, Reflect>(crc, value, table);
          crc = crc_update_chunk<TAccumulator, Accumulator_Bits, Chunk_Bits, Chunk_Mask, Reflect>(crc, (value >> (Chunk_Bits * 1U)), table);
          crc = crc_update_chunk<TAccumulator, Accumulator_Bits, Chunk_Bits, Chunk_Mask, Reflect>(crc, (value >> (Chunk_Bits * 2U)), table);
          crc = crc_update_chunk<TAccumulator, Accumulator_Bits, Chunk_Bits, Chunk_Mask, Reflect>(crc, (value >> (Chunk_Bits * 3U)), table);          
        }
        else
        {
          crc = crc_update_chunk<TAccumulator, Accumulator_Bits, Chunk_Bits, Chunk_Mask, Reflect>(crc, (value >> (Chunk_Bits * 3U)), table);
          crc = crc_update_chunk<TAccumulator, Accumulator_Bits, Chunk_Bits, Chunk_Mask, Reflect>(crc, (value >> (Chunk_Bits * 2U)), table);
          crc = crc_update_chunk<TAccumulator, Accumulator_Bits, Chunk_Bits, Chunk_Mask, Reflect>(crc, (value >> (Chunk_Bits * 1U)), table);
          crc = crc_update_chunk<TAccumulator, Accumulator_Bits, Chunk_Bits, Chunk_Mask, Reflect>(crc, value, table);
        }

        return crc;
      }
    };

    //*********************************
    // Table size of 16.
    template <typename TAccumulator, size_t Accumulator_Bits, size_t Chunk_Bits, uint8_t Chunk_Mask, TAccumulator Polynomial, bool Reflect>
    struct crc_table<TAccumulator, Accumulator_Bits, Chunk_Bits, Chunk_Mask, Polynomial, Reflect, 16U>
    {
      //*************************************************************************
      TAccumulator add(TAccumulator crc, uint8_t value) const
      {
        static ETL_CONSTANT TAccumulator table[16U] =
        {
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 0U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 1U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 2U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 3U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 4U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 5U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 6U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 7U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 8U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 9U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 10U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 11U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 12U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 13U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 14U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 15U, Chunk_Bits>::value
        };

        if ETL_IF_CONSTEXPR(Reflect)
        {
          crc = crc_update_chunk<TAccumulator, Accumulator_Bits, Chunk_Bits, Chunk_Mask, Reflect>(crc, value, table);
          crc = crc_update_chunk<TAccumulator, Accumulator_Bits, Chunk_Bits, Chunk_Mask, Reflect>(crc, value >> Chunk_Bits, table);
        }
        else
        {
          crc = crc_update_chunk<TAccumulator, Accumulator_Bits, Chunk_Bits, Chunk_Mask, Reflect>(crc, value >> Chunk_Bits, table);
          crc = crc_update_chunk<TAccumulator, Accumulator_Bits, Chunk_Bits, Chunk_Mask, Reflect>(crc, value, table);
        }

        return crc;
      }
    };

    //*********************************
    // Table size of 256.
    template <typename TAccumulator, size_t Accumulator_Bits, size_t Chunk_Bits, uint8_t Chunk_Mask, TAccumulator Polynomial, bool Reflect>
    struct crc_table<TAccumulator, Accumulator_Bits, Chunk_Bits, Chunk_Mask, Polynomial, Reflect, 256U>
    {
      //*************************************************************************
      TAccumulator add(TAccumulator crc, uint8_t value) const
      {
        static ETL_CONSTANT TAccumulator table[256U] =
        {
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 0U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 1U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 2U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 3U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 4U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 5U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 6U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 7U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 8U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 9U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 10U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 11U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 12U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 13U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 14U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 15U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 16U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 17U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 18U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 19U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 20U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 21U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 22U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 23U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 24U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 25U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 26U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 27U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 28U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 29U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 30U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 31U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 32U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 33U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 34U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 35U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 36U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 37U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 38U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 39U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 40U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 41U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 42U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 43U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 44U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 45U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 46U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 47U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 48U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 49U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 50U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 51U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 52U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 53U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 54U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 55U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 56U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 57U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 58U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 59U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 60U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 61U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 62U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 63U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 64U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 65U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 66U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 67U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 68U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 69U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 70U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 71U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 72U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 73U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 74U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 75U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 76U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 77U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 78U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 79U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 80U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 81U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 82U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 83U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 84U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 85U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 86U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 87U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 88U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 89U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 90U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 91U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 92U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 93U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 94U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 95U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 96U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 97U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 98U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 99U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 100U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 101U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 102U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 103U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 104U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 105U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 106U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 107U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 108U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 109U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 110U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 111U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 112U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 113U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 114U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 115U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 116U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 117U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 118U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 119U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 120U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 121U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 122U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 123U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 124U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 125U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 126U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 127U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 128U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 129U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 130U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 131U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 132U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 133U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 134U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 135U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 136U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 137U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 138U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 139U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 140U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 141U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 142U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 143U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 144U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 145U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 146U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 147U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 148U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 149U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 150U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 151U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 152U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 153U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 154U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 155U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 156U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 157U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 158U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 159U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 160U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 161U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 162U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 163U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 164U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 165U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 166U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 167U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 168U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 169U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 170U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 171U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 172U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 173U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 174U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 175U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 176U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 177U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 178U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 179U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 180U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 181U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 182U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 183U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 184U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 185U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 186U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 187U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 188U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 189U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 190U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 191U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 192U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 193U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 194U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 195U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 196U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 197U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 198U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 199U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 200U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 201U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 202U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 203U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 204U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 205U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 206U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 207U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 208U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 209U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 210U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 211U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 212U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 213U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 214U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 215U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 216U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 217U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 218U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 219U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 220U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 221U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 222U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 223U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 224U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 225U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 226U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 227U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 228U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 229U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 230U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 231U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 232U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 233U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 234U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 235U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 236U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 237U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 238U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 239U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 240U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 241U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 242U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 243U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 244U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 245U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 246U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 247U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 248U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 249U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 250U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 251U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 252U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 253U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 254U, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, Accumulator_Bits, Polynomial, Reflect, 255U, Chunk_Bits>::value
        };

        crc = crc_update_chunk<TAccumulator, Accumulator_Bits, Chunk_Bits, Chunk_Mask, Reflect>(crc, value, table);

        return crc;
      }
    };

    //*****************************************************************************
    // CRC Policies.
    //*****************************************************************************
    template <typename TCrcParameters, size_t Table_Size>
    struct crc_policy;

    //*********************************
    // Policy for 256 entry table.
    template <typename TCrcParameters>
    struct crc_policy<TCrcParameters, 256U> : public crc_table<typename TCrcParameters::accumulator_type, 
                                                               TCrcParameters::Accumulator_Bits, 
                                                               8U, 
                                                               0xFFU, 
                                                               TCrcParameters::Polynomial, 
                                                               TCrcParameters::Reflect, 
                                                               256U> 
    {
      typedef typename TCrcParameters::accumulator_type accumulator_type;
      typedef accumulator_type value_type;

      //*************************************************************************
      ETL_CONSTEXPR accumulator_type initial() const
      {
        return TCrcParameters::Reflect ? etl::reverse_bits_const<accumulator_type, TCrcParameters::Initial>::value
                                       : TCrcParameters::Initial;
      }

      //*************************************************************************
      accumulator_type final(accumulator_type crc) const
      {
        return crc ^ TCrcParameters::Xor_Out;
      }
    };

    //*********************************
    // Policy for 16 entry table.
    template <typename TCrcParameters>
    struct crc_policy<TCrcParameters, 16U> : public crc_table<typename TCrcParameters::accumulator_type, 
                                                              TCrcParameters::Accumulator_Bits, 
                                                              4U, 
                                                              0x0FU, 
                                                              TCrcParameters::Polynomial, 
                                                              TCrcParameters::Reflect, 
                                                              16U> 
    {
      typedef typename TCrcParameters::accumulator_type accumulator_type;
      typedef accumulator_type value_type;

      //*************************************************************************
      ETL_CONSTEXPR accumulator_type initial() const
      {
        return TCrcParameters::Reflect ? etl::reverse_bits_const<accumulator_type, TCrcParameters::Initial>::value
                                       : TCrcParameters::Initial;
      }

      //*************************************************************************
      accumulator_type final(accumulator_type crc) const
      {
        return crc ^ TCrcParameters::Xor_Out;
      }
    };

    //*********************************
    // Policy for 4 entry table.
    template <typename TCrcParameters>
    struct crc_policy<TCrcParameters, 4U> : public crc_table<typename TCrcParameters::accumulator_type, 
                                                             TCrcParameters::Accumulator_Bits, 
                                                             2U, 
                                                             0x03U, 
                                                             TCrcParameters::Polynomial, 
                                                             TCrcParameters::Reflect, 
                                                             4U> 
    {
      typedef typename TCrcParameters::accumulator_type accumulator_type;
      typedef accumulator_type value_type;

      //*************************************************************************
      ETL_CONSTEXPR accumulator_type initial() const
      {
        return TCrcParameters::Reflect ? etl::reverse_bits_const<accumulator_type, TCrcParameters::Initial>::value
                                       : TCrcParameters::Initial;
      }

      //*************************************************************************
      accumulator_type final(accumulator_type crc) const
      {
        return crc ^ TCrcParameters::Xor_Out;
      }
    };
  }

  //*****************************************************************************
  /// Basic parameterised CRC type.
  //*****************************************************************************
  template <typename TCrcParameters, size_t Table_Size>
  class crc_type : public etl::frame_check_sequence<private_crc::crc_policy<TCrcParameters, Table_Size> >
  {
  public:

    ETL_STATIC_ASSERT((Table_Size == 4U) || (Table_Size == 16U) || (Table_Size == 256U), "Table size must be 4, 16 or 256");

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    crc_type()
    {
      this->reset();
    }

    //*************************************************************************
    /// Constructor from range.
    /// \param begin Start of the range.
    /// \param end   End of the range.
    //*************************************************************************
    template<typename TIterator>
    crc_type(TIterator begin, const TIterator end)
    {
      this->reset();
      this->add(begin, end);
    }
  };
}

#endif
