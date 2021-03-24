
#pragma once

#include "../platform.h"
#include "../frame_check_sequence.h"
#include "../static_assert.h"
#include "../binary.h"

#include "stdint.h"

namespace etl
{
  namespace private_crc
  {
    //*****************************************************************************
    /// CRC Traits
    //*****************************************************************************
    template <size_t Table_Size>
    struct crc_traits
    {
      ETL_STATIC_ASSERT((Table_Size == 4U) || (Table_Size == 16U) || (Table_Size == 256U), "Table size must be 4, 16 or 256");
    };

    //*********************************
    // Traits for table size of 4.
    template <>
    struct crc_traits<4U>
    {
      static ETL_CONSTANT uint8_t Chunk_Mask = 0x03U;
      static ETL_CONSTANT size_t  Chunk_Bits = 2U;
    };

    //*********************************
    // Traits for table size of 16.
    template <>
    struct crc_traits<16U>
    {
      static ETL_CONSTANT uint8_t Chunk_Mask = 0x0FU;
      static ETL_CONSTANT size_t  Chunk_Bits = 4U;
    };

    //*********************************
    // Traits for table size of 256.
    template <>
    struct crc_traits<256U>
    {
      static ETL_CONSTANT uint8_t Chunk_Mask = 0xFFU;
      static ETL_CONSTANT size_t  Chunk_Bits = 8U;
    };

    //*****************************************************************************
    /// CRC Entry
    //*****************************************************************************
    template <typename TAccumulator, TAccumulator Polynomial, bool Reflect, TAccumulator Entry>
    class crc_entry;

    //*********************************
    // Not reflected
    template <typename TAccumulator, TAccumulator Polynomial, TAccumulator Entry>
    class crc_entry<TAccumulator, Polynomial, false, Entry>
    {
    private:

      static ETL_CONSTANT size_t Accumulator_Bits = etl::integral_limits<TAccumulator>::bits;
      static ETL_CONSTANT bool   Do_Poly          = (Entry & (TAccumulator(1U) << (Accumulator_Bits - 1U))) != 0U;

    public:

      static ETL_CONSTANT TAccumulator value = TAccumulator(Do_Poly ? (Entry << 1U) ^ Polynomial : (Entry << 1U));
    };

    //*********************************
    // Reflected
    template <typename TAccumulator, TAccumulator Polynomial, TAccumulator Entry>
    class crc_entry<TAccumulator, Polynomial, true, Entry>
    {
    private:

      static ETL_CONSTANT size_t Accumulator_Bits = etl::integral_limits<TAccumulator>::bits;
      static ETL_CONSTANT bool   Do_Poly          = (Entry & TAccumulator(1U)) != 0U;

    public:

      static ETL_CONSTANT TAccumulator value = TAccumulator(Do_Poly ? (Entry >> 1U) ^ etl::reverse_bits_const<TAccumulator, Polynomial>::value : (Entry >> 1U));
    };

    //*****************************************************************************
    /// CRC Table Entry
    //*****************************************************************************
    template <typename TAccumulator, size_t Index, TAccumulator Polynomial, bool Reflect, uint8_t Chunk_Bits>
    class crc_table_entry
    {
      ETL_STATIC_ASSERT((Chunk_Bits == 2U) || (Chunk_Bits == 4U) || (Chunk_Bits == 8U), "Chunk bits must be 2, 4 or 8");
    };

    //*********************************
    // Chunk bit size of 8.
    template <typename TAccumulator, size_t Index, TAccumulator Polynomial, bool Reflect>
    class crc_table_entry<TAccumulator, Index, Polynomial, Reflect, 8U>
    {
    public:

      static ETL_CONSTANT size_t       Accumulator_Bits = etl::integral_limits<TAccumulator>::bits;
      static ETL_CONSTANT size_t       Shift_Bits       = size_t(Accumulator_Bits - 8U);
      static ETL_CONSTANT TAccumulator Entry            = Reflect ? TAccumulator(Index) : TAccumulator(TAccumulator(Index) << Shift_Bits);

      static ETL_CONSTANT TAccumulator value = crc_entry<TAccumulator, Polynomial, Reflect,
                                               crc_entry<TAccumulator, Polynomial, Reflect,
                                               crc_entry<TAccumulator, Polynomial, Reflect,
                                               crc_entry<TAccumulator, Polynomial, Reflect,
                                               crc_entry<TAccumulator, Polynomial, Reflect,
                                               crc_entry<TAccumulator, Polynomial, Reflect,
                                               crc_entry<TAccumulator, Polynomial, Reflect,
                                               crc_entry<TAccumulator, Polynomial, Reflect, Entry>::value>::value>::value>::value>::value>::value>::value>::value;
    };

    //*********************************
    // Chunk bit size of 4.
    template <typename TAccumulator, size_t Index, TAccumulator Polynomial, bool Reflect>
    class crc_table_entry<TAccumulator, Index, Polynomial, Reflect, 4U>
    {
    public:

      static ETL_CONSTANT size_t       Accumulator_Bits = etl::integral_limits<TAccumulator>::bits;
      static ETL_CONSTANT size_t       Shift_Bits       = size_t(Accumulator_Bits - 4U);
      static ETL_CONSTANT TAccumulator Entry            = Reflect ? TAccumulator(Index) : TAccumulator(TAccumulator(Index) << Shift_Bits);

      static const TAccumulator value = crc_entry<TAccumulator, Polynomial, Reflect,
                                        crc_entry<TAccumulator, Polynomial, Reflect,
                                        crc_entry<TAccumulator, Polynomial, Reflect,
                                        crc_entry<TAccumulator, Polynomial, Reflect, Entry>::value>::value>::value>::value;
    };

    //*********************************
    // Chunk bit size of 2.
    template <typename TAccumulator, size_t Index, TAccumulator Polynomial, bool Reflect>
    class crc_table_entry<TAccumulator, Index, Polynomial, Reflect, 2U>
    {
    public:

      static ETL_CONSTANT size_t       Accumulator_Bits = etl::integral_limits<TAccumulator>::bits;
      static ETL_CONSTANT size_t       Shift_Bits       = size_t(Accumulator_Bits - 2U);
      static ETL_CONSTANT TAccumulator Entry            = Reflect ? TAccumulator(Index) : TAccumulator(TAccumulator(Index) << Shift_Bits);

      static const TAccumulator value = crc_entry<TAccumulator, Polynomial, Reflect,
                                        crc_entry<TAccumulator, Polynomial, Reflect, Entry>::value>::value;
    };

    //*************************************************************************
    // Accumulator_Bits > Chunk_Bits and not reflected
    template <typename TAccumulator, uint8_t Chunk_Mask, uint8_t Chunk_Bits, uint8_t Accumulator_Bits, bool Reflect>
    typename etl::enable_if<(Accumulator_Bits > Chunk_Bits) && !Reflect, TAccumulator>::type
      crc_update_chunk(TAccumulator crc, uint8_t value, const TAccumulator table[])
    {
      size_t index = (crc >> (Accumulator_Bits - Chunk_Bits)) ^ value;

      crc <<= Chunk_Bits;
      crc ^= table[index];

      return crc;
    }

    //*************************************************************************
    // Accumulator_Bits > Chunk_Bits and reflected
    template <typename TAccumulator, uint8_t Chunk_Mask, uint8_t Chunk_Bits, uint8_t Accumulator_Bits, bool Reflect>
    typename etl::enable_if<(Accumulator_Bits > Chunk_Bits) && Reflect, TAccumulator>::type
      crc_update_chunk(TAccumulator crc, uint8_t value, const TAccumulator table[])
    {
      size_t index = (crc & Chunk_Mask) ^ value;

      crc >>= Chunk_Bits;
      crc ^= table[index];

      return crc;
    }

    //*************************************************************************
    // Accumulator_Bits == Chunk_Bits and not reflected
    template <typename TAccumulator, uint8_t Chunk_Mask, uint8_t Chunk_Bits, uint8_t Accumulator_Bits, bool Reflect>
    typename etl::enable_if<(Accumulator_Bits == Chunk_Bits) && !Reflect, TAccumulator>::type
      crc_update_chunk(TAccumulator crc, uint8_t value, const TAccumulator table[])
    {
      size_t index = (crc >> (Accumulator_Bits - Chunk_Bits)) ^ value;

      crc = table[index];

      return crc;
    }

    //*************************************************************************
    // Accumulator_Bits == Chunk_Bits and reflected
    template <typename TAccumulator, uint8_t Chunk_Mask, uint8_t Chunk_Bits, uint8_t Accumulator_Bits, bool Reflect>
    typename etl::enable_if<(Accumulator_Bits == Chunk_Bits) && Reflect, TAccumulator>::type
      crc_update_chunk(TAccumulator crc, uint8_t value, const TAccumulator table[])
    {
      size_t index = (crc & Chunk_Mask) ^ value;

      crc = table[index];

      return crc;
    }

    //*****************************************************************************
    //
    //*****************************************************************************
    template <typename TAccumulator, TAccumulator Polynomial, TAccumulator Initial, TAccumulator Xor_Out, bool Reflect, size_t Table_Size>
    struct crc_policy
    {
      ETL_STATIC_ASSERT((Table_Size == 4U) || (Table_Size == 16U) || (Table_Size == 256U), "Table size must be 4, 16 or 256");
    };

    //*********************************
    template <typename TAccumulator, TAccumulator Polynomial, TAccumulator Initial, TAccumulator Xor_Out, bool Reflect>
    struct crc_policy<TAccumulator, Polynomial, Initial, Xor_Out, Reflect, 256U>
    {
      static ETL_CONSTANT size_t  Table_Size       = 256U;
      static ETL_CONSTANT uint8_t Chunk_Mask       = crc_traits<Table_Size>::Chunk_Mask;
      static ETL_CONSTANT uint8_t Chunk_Bits       = crc_traits<Table_Size>::Chunk_Bits;
      static ETL_CONSTANT size_t  Accumulator_Bits = etl::integral_limits<TAccumulator>::bits;

      typedef TAccumulator value_type;

      //*************************************************************************
      ETL_CONSTEXPR TAccumulator initial() const
      {
        return Initial;
      }

      //*************************************************************************
      TAccumulator add(TAccumulator crc, uint8_t value) const
      {
        static ETL_CONSTANT TAccumulator table[Table_Size] =
        {
          crc_table_entry<TAccumulator, 0U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 1U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 2U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 3U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 4U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 5U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 6U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 7U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 8U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 9U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 10U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 11U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 12U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 13U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 14U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 15U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 16U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 17U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 18U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 19U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 20U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 21U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 22U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 23U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 24U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 25U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 26U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 27U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 28U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 29U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 30U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 31U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 32U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 33U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 34U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 35U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 36U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 37U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 38U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 39U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 40U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 41U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 42U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 43U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 44U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 45U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 46U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 47U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 48U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 49U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 50U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 51U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 52U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 53U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 54U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 55U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 56U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 57U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 58U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 59U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 60U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 61U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 62U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 63U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 64U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 65U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 66U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 67U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 68U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 69U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 70U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 71U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 72U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 73U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 74U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 75U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 76U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 77U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 78U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 79U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 80U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 81U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 82U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 83U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 84U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 85U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 86U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 87U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 88U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 89U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 90U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 91U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 92U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 93U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 94U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 95U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 96U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 97U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 98U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 99U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 100U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 101U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 102U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 103U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 104U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 105U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 106U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 107U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 108U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 109U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 110U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 111U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 112U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 113U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 114U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 115U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 116U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 117U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 118U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 119U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 120U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 121U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 122U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 123U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 124U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 125U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 126U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 127U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 128U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 129U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 130U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 131U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 132U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 133U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 134U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 135U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 136U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 137U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 138U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 139U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 140U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 141U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 142U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 143U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 144U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 145U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 146U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 147U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 148U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 149U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 150U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 151U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 152U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 153U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 154U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 155U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 156U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 157U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 158U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 159U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 160U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 161U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 162U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 163U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 164U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 165U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 166U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 167U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 168U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 169U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 170U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 171U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 172U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 173U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 174U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 175U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 176U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 177U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 178U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 179U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 180U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 181U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 182U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 183U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 184U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 185U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 186U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 187U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 188U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 189U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 190U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 191U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 192U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 193U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 194U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 195U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 196U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 197U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 198U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 199U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 200U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 201U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 202U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 203U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 204U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 205U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 206U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 207U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 208U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 209U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 210U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 211U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 212U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 213U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 214U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 215U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 216U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 217U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 218U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 219U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 220U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 221U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 222U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 223U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 224U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 225U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 226U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 227U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 228U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 229U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 230U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 231U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 232U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 233U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 234U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 235U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 236U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 237U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 238U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 239U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 240U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 241U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 242U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 243U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 244U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 245U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 246U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 247U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 248U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 249U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 250U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 251U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 252U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 253U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 254U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 255U, Polynomial, Reflect, Chunk_Bits>::value
        };

        crc = crc_update_chunk<TAccumulator, Chunk_Mask, Chunk_Bits, Accumulator_Bits, Reflect>(crc, value, table);

        return crc;
      }

      //*************************************************************************
      TAccumulator final(TAccumulator crc) const
      {
        return crc ^ Xor_Out;
      }
    };

    //*********************************
    template <typename TAccumulator, TAccumulator Polynomial, TAccumulator Initial, TAccumulator Xor_Out, bool Reflect>
    struct crc_policy<TAccumulator, Polynomial, Initial, Xor_Out, Reflect, 16U>
    {
      static ETL_CONSTANT size_t  Table_Size = 16U;
      static ETL_CONSTANT uint8_t Chunk_Mask       = crc_traits<Table_Size>::Chunk_Mask;
      static ETL_CONSTANT uint8_t Chunk_Bits       = crc_traits<Table_Size>::Chunk_Bits;
      static ETL_CONSTANT size_t  Accumulator_Bits = etl::integral_limits<TAccumulator>::bits;

      typedef TAccumulator value_type;

      //*************************************************************************
      ETL_CONSTEXPR TAccumulator initial() const
      {
        return Initial;
      }

      //*************************************************************************
      static TAccumulator update_chunk(TAccumulator crc, uint8_t value, const TAccumulator table[])
      {
        // Extract the most significant nibble of the crc and xor with the value nibble.
        size_t index = (crc >> (etl::integral_limits<uint8_t>::bits - Chunk_Bits)) ^ value;

        crc <<= Chunk_Bits;
        crc ^= table[index];

        return crc;
      }

      //*************************************************************************
      TAccumulator add(TAccumulator crc, uint8_t value) const
      {
        static ETL_CONSTANT TAccumulator table[Table_Size] =
        {
          crc_table_entry<TAccumulator, 0U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 1U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 2U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 3U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 4U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 5U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 6U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 7U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 8U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 9U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 10U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 11U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 12U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 13U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 14U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 15U, Polynomial, Reflect, Chunk_Bits>::value
        };

        crc = update_chunk(crc, (value >> Chunk_Bits) & Chunk_Mask, table);
        crc = update_chunk(crc, value & Chunk_Mask, table);

        return crc;
      }

      //*************************************************************************
      TAccumulator final(TAccumulator crc) const
      {
        return crc ^ Xor_Out;
      }
    };


    //*********************************
    template <typename TAccumulator, TAccumulator Polynomial, TAccumulator Initial, TAccumulator Xor_Out, bool Reflect>
    struct crc_policy<TAccumulator, Polynomial, Initial, Xor_Out, Reflect, 4U>
    {
      static ETL_CONSTANT size_t  Table_Size       = 4U;
      static ETL_CONSTANT uint8_t Chunk_Mask       = crc_traits<Table_Size>::Chunk_Mask;
      static ETL_CONSTANT uint8_t Chunk_Bits       = crc_traits<Table_Size>::Chunk_Bits;
      static ETL_CONSTANT size_t  Accumulator_Bits = etl::integral_limits<TAccumulator>::bits;

      typedef TAccumulator value_type;

      //*************************************************************************
      ETL_CONSTEXPR TAccumulator initial() const
      {
        return Initial;
      }

      //*************************************************************************
      static TAccumulator update_chunk(TAccumulator crc, uint8_t value, const TAccumulator table[])
      {
        // Extract the most significant nibble of the crc and xor with the value nibble.
        size_t index = (crc >> (etl::integral_limits<uint8_t>::bits - Chunk_Bits)) ^ value;

        crc <<= Chunk_Bits;
        crc ^= table[index];

        return crc;
      }

      //*************************************************************************
      TAccumulator add(TAccumulator crc, uint8_t value) const
      {
        static ETL_CONSTANT TAccumulator table[Table_Size] =
        {
          crc_table_entry<TAccumulator, 0U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 1U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 2U, Polynomial, Reflect, Chunk_Bits>::value,
          crc_table_entry<TAccumulator, 3U, Polynomial, Reflect, Chunk_Bits>::value
        };

        crc = update_chunk(crc, (value >> (Chunk_Bits * 3U)) & Chunk_Mask, table);
        crc = update_chunk(crc, (value >> (Chunk_Bits * 2U)) & Chunk_Mask, table);
        crc = update_chunk(crc, (value >> (Chunk_Bits * 1U)) & Chunk_Mask, table);
        crc = update_chunk(crc, value & Chunk_Mask, table);

        return crc;
      }

      //*************************************************************************
      TAccumulator final(TAccumulator crc) const
      {
        return crc ^ Xor_Out;
      }
    };

    //*****************************************************************************
    ///
    //*****************************************************************************
    template <typename TAccumulator, TAccumulator Polynomial, TAccumulator Initial, TAccumulator Xor_Out, bool Reflect, size_t Table_Size>
    class crc_type : public etl::frame_check_sequence<crc_policy<TAccumulator, Polynomial, Initial, Xor_Out, Reflect, Table_Size> >
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
}
