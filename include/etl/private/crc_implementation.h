
#pragma once

#include "../platform.h"
#include "../frame_check_sequence.h"
#include "../static_assert.h"
#include "../binary.h"

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
    /// CRC Entry
    //*****************************************************************************
    template <typename TCrcParameters, typename TCrcParameters::accumulator_type Entry>
    class crc_entry
    {
    private:

      typedef typename TCrcParameters::accumulator_type accumulator_type;

      static ETL_CONSTANT size_t           Accumulator_Bits = TCrcParameters::Accumulator_Bits;
      static ETL_CONSTANT accumulator_type Polynomial       = TCrcParameters::Polynomial;
      static ETL_CONSTANT bool             Reflect          = TCrcParameters::Reflect;

      static ETL_CONSTANT bool Do_Poly = Reflect ? (Entry & accumulator_type(1U)) != 0U 
                                                 : (Entry & (accumulator_type(1U) << (Accumulator_Bits - 1U))) != 0U;

    public:

      static ETL_CONSTANT accumulator_type value = Reflect ? accumulator_type(Do_Poly ? (Entry >> 1U) ^ etl::reverse_bits_const<accumulator_type, Polynomial>::value : (Entry >> 1U)) 
                                                           : accumulator_type(Do_Poly ? (Entry << 1U) ^ Polynomial : (Entry << 1U));
    };

    //*****************************************************************************
    /// CRC Table Entry
    //*****************************************************************************
    template <typename TCrcParameters, size_t Index, uint8_t Chunk_Bits>
    class crc_table_entry
    {
      ETL_STATIC_ASSERT((Chunk_Bits == 2U) || (Chunk_Bits == 4U) || (Chunk_Bits == 8U), "Chunk bits must be 2, 4 or 8");
    };

    //*********************************
    // Chunk bit size of 8.
    template <typename TCrcParameters, size_t Index>
    class crc_table_entry<TCrcParameters, Index, 8U>
    {
    public:

      typedef typename TCrcParameters::accumulator_type accumulator_type;

      static ETL_CONSTANT size_t           Accumulator_Bits = TCrcParameters::Accumulator_Bits;
      static ETL_CONSTANT size_t           Shift_Bits       = size_t(Accumulator_Bits - 8U);
      static ETL_CONSTANT accumulator_type Entry            = TCrcParameters::Reflect ? accumulator_type(Index) : accumulator_type(accumulator_type(Index) << Shift_Bits);

      static ETL_CONSTANT accumulator_type value = crc_entry<TCrcParameters,
                                                   crc_entry<TCrcParameters,
                                                   crc_entry<TCrcParameters,
                                                   crc_entry<TCrcParameters,
                                                   crc_entry<TCrcParameters,
                                                   crc_entry<TCrcParameters,
                                                   crc_entry<TCrcParameters,
                                                   crc_entry<TCrcParameters, Entry>::value>::value>::value>::value>::value>::value>::value>::value;
    };

    //*********************************
    // Chunk bit size of 4.
    template <typename TCrcParameters, size_t Index>
    class crc_table_entry<TCrcParameters, Index, 4U>
    {
    public:

      typedef typename TCrcParameters::accumulator_type accumulator_type;

      static ETL_CONSTANT size_t           Accumulator_Bits = TCrcParameters::Accumulator_Bits;
      static ETL_CONSTANT size_t           Shift_Bits       = size_t(Accumulator_Bits - 4U);
      static ETL_CONSTANT accumulator_type Entry            = TCrcParameters::Reflect ? accumulator_type(Index) : accumulator_type(accumulator_type(Index) << Shift_Bits);

      static const accumulator_type value = crc_entry<TCrcParameters,
                                            crc_entry<TCrcParameters,
                                            crc_entry<TCrcParameters,
                                            crc_entry<TCrcParameters, Entry>::value>::value>::value>::value;
    };

    //*********************************
    // Chunk bit size of 2.
    template <typename TCrcParameters, size_t Index>
    class crc_table_entry<TCrcParameters, Index, 2U>
    {
    public:

      typedef typename TCrcParameters::accumulator_type accumulator_type;

      static ETL_CONSTANT size_t           Accumulator_Bits = TCrcParameters::Accumulator_Bits;
      static ETL_CONSTANT size_t           Shift_Bits       = size_t(Accumulator_Bits - 2U);
      static ETL_CONSTANT accumulator_type Entry            = TCrcParameters::Reflect ? accumulator_type(Index) : accumulator_type(accumulator_type(Index) << Shift_Bits);

      static const accumulator_type value = crc_entry<TCrcParameters,
                                            crc_entry<TCrcParameters, Entry>::value>::value;
    };

    ////*************************************************************************
    //// Accumulator_Bits > Chunk_Bits and not reflected
    //template <typename TAccumulator, uint8_t Chunk_Mask, uint8_t Chunk_Bits, uint8_t Accumulator_Bits, bool Reflect>
    //typename etl::enable_if<(Accumulator_Bits > Chunk_Bits) && !Reflect, TAccumulator>::type
    //  crc_update_chunk(TAccumulator crc, uint8_t value, const TAccumulator table[])
    //{
    //  size_t index = (crc >> (Accumulator_Bits - Chunk_Bits)) ^ value;

    //  crc <<= Chunk_Bits;
    //  crc ^= table[index];

    //  return crc;
    //}

    ////*************************************************************************
    //// Accumulator_Bits > Chunk_Bits and reflected
    //template <typename TAccumulator, uint8_t Chunk_Mask, uint8_t Chunk_Bits, uint8_t Accumulator_Bits, bool Reflect>
    //typename etl::enable_if<(Accumulator_Bits > Chunk_Bits) && Reflect, TAccumulator>::type
    //  crc_update_chunk(TAccumulator crc, uint8_t value, const TAccumulator table[])
    //{
    //  size_t index = size_t((crc & Chunk_Mask) ^ value);

    //  crc >>= Chunk_Bits;
    //  crc ^= table[index];

    //  return crc;
    //}

    ////*************************************************************************
    //// Accumulator_Bits == Chunk_Bits and not reflected
    //template <typename TAccumulator, uint8_t Chunk_Mask, uint8_t Chunk_Bits, uint8_t Accumulator_Bits, bool Reflect>
    //typename etl::enable_if<(Accumulator_Bits == Chunk_Bits) && !Reflect, TAccumulator>::type
    //  crc_update_chunk(TAccumulator crc, uint8_t value, const TAccumulator table[])
    //{
    //  size_t index = size_t((crc >> (Accumulator_Bits - Chunk_Bits)) ^ value);

    //  crc = table[index];

    //  return crc;
    //}

    ////*************************************************************************
    //// Accumulator_Bits == Chunk_Bits and reflected
    //template <typename TAccumulator, uint8_t Chunk_Mask, uint8_t Chunk_Bits, uint8_t Accumulator_Bits, bool Reflect>
    //typename etl::enable_if<(Accumulator_Bits == Chunk_Bits) && Reflect, TAccumulator>::type
    //  crc_update_chunk(TAccumulator crc, uint8_t value, const TAccumulator table[])
    //{
    //  size_t index = size_t((crc & Chunk_Mask) ^ value);

    //  crc = table[index];

    //  return crc;
    //}

    //*****************************************************************************
    // CRC Policies.
    //*****************************************************************************
    template <typename TCrcParameters, size_t Table_Size>
    struct crc_policy;

    //*********************************
    // Policy for 256 entry table.
    template <typename TCrcParameters>
    struct crc_policy<TCrcParameters, 256U>
    {
      typedef typename TCrcParameters::accumulator_type accumulator_type;
      typedef accumulator_type value_type;

      static ETL_CONSTANT size_t  Table_Size = 256U;
      static ETL_CONSTANT uint8_t Chunk_Mask = Table_Size - 1U;
      static ETL_CONSTANT size_t  Chunk_Bits = 8U;
      static ETL_CONSTANT size_t  Accumulator_Bits = TCrcParameters::Accumulator_Bits;    

      //*************************************************************************
      ETL_CONSTEXPR accumulator_type initial() const
      {
        return TCrcParameters::Initial;
      }

      //*************************************************************************
      static accumulator_type update_chunk(accumulator_type crc, uint8_t value, const accumulator_type table[])
      {
        //// Extract the most significant nibble of the crc and xor with the value nibble.
        //size_t index = size_t((crc >> (etl::integral_limits<uint8_t>::bits - Chunk_Bits)) ^ value);

        //crc <<= Chunk_Bits;
        //crc ^= table[index];

        value &= Chunk_Mask;

        if ETL_IF_CONSTEXPR(TCrcParameters::Reflect)
        {
          uint8_t index = (crc & Chunk_Mask) ^ value;

          // special case for when chunk size is same as accumulator size.
          if ETL_IF_CONSTEXPR(Accumulator_Bits > Chunk_Bits) 
          {
            crc >>= Chunk_Bits;
            crc ^= table[index];
          } 
          else 
          {
            crc = table[index];
          }

          return crc;
        }
        else
        {
          // Extract the most significant nibble of the crc and xor with the value nibble
          uint8_t index = (crc >> (Accumulator_Bits - Chunk_Bits)) ^ value;

          // special case for when chunk size is same as accumulator size.
          if ETL_IF_CONSTEXPR(Accumulator_Bits > Chunk_Bits)
          {
            crc <<= Chunk_Bits;
            crc ^= table[index];
          }
          else
          {
            crc = table[index];
          }

          return crc;
        }
      }

      //*************************************************************************
      accumulator_type add(accumulator_type crc, uint8_t value) const
      {
        static ETL_CONSTANT accumulator_type table[Table_Size] =
        {
          crc_table_entry<TCrcParameters, 0U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 1U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 2U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 3U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 4U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 5U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 6U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 7U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 8U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 9U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 10U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 11U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 12U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 13U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 14U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 15U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 16U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 17U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 18U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 19U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 20U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 21U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 22U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 23U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 24U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 25U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 26U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 27U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 28U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 29U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 30U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 31U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 32U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 33U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 34U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 35U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 36U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 37U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 38U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 39U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 40U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 41U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 42U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 43U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 44U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 45U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 46U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 47U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 48U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 49U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 50U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 51U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 52U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 53U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 54U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 55U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 56U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 57U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 58U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 59U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 60U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 61U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 62U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 63U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 64U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 65U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 66U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 67U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 68U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 69U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 70U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 71U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 72U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 73U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 74U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 75U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 76U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 77U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 78U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 79U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 80U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 81U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 82U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 83U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 84U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 85U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 86U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 87U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 88U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 89U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 90U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 91U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 92U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 93U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 94U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 95U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 96U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 97U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 98U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 99U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 100U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 101U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 102U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 103U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 104U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 105U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 106U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 107U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 108U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 109U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 110U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 111U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 112U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 113U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 114U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 115U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 116U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 117U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 118U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 119U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 120U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 121U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 122U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 123U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 124U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 125U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 126U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 127U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 128U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 129U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 130U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 131U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 132U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 133U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 134U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 135U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 136U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 137U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 138U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 139U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 140U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 141U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 142U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 143U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 144U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 145U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 146U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 147U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 148U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 149U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 150U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 151U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 152U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 153U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 154U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 155U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 156U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 157U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 158U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 159U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 160U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 161U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 162U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 163U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 164U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 165U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 166U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 167U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 168U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 169U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 170U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 171U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 172U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 173U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 174U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 175U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 176U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 177U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 178U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 179U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 180U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 181U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 182U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 183U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 184U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 185U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 186U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 187U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 188U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 189U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 190U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 191U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 192U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 193U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 194U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 195U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 196U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 197U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 198U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 199U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 200U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 201U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 202U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 203U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 204U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 205U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 206U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 207U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 208U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 209U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 210U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 211U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 212U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 213U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 214U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 215U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 216U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 217U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 218U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 219U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 220U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 221U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 222U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 223U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 224U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 225U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 226U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 227U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 228U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 229U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 230U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 231U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 232U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 233U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 234U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 235U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 236U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 237U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 238U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 239U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 240U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 241U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 242U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 243U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 244U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 245U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 246U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 247U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 248U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 249U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 250U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 251U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 252U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 253U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 254U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 255U, Chunk_Bits>::value
        };

        crc = update_chunk(crc, value, table);
        //crc = crc_update_chunk<accumulator_type, Chunk_Mask, Chunk_Bits, Accumulator_Bits, TCrcParameters::Reflect>(crc, value, table);

        return crc;
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
    struct crc_policy<TCrcParameters, 16U>
    {
      typedef typename TCrcParameters::accumulator_type accumulator_type;
      typedef accumulator_type value_type;

      static ETL_CONSTANT size_t  Table_Size = 16U;
      static ETL_CONSTANT uint8_t Chunk_Mask = Table_Size - 1U;
      static ETL_CONSTANT size_t  Chunk_Bits = 4U;
      static ETL_CONSTANT size_t  Accumulator_Bits = TCrcParameters::Accumulator_Bits;

      typedef accumulator_type value_type;

      //*************************************************************************
      ETL_CONSTEXPR accumulator_type initial() const
      {
        return TCrcParameters::Initial;
      }

      //*************************************************************************
      static accumulator_type update_chunk(accumulator_type crc, uint8_t value, const accumulator_type table[])
      {
        //// Extract the most significant nibble of the crc and xor with the value nibble.
        //size_t index = size_t((crc >> (etl::integral_limits<uint8_t>::bits - Chunk_Bits)) ^ value);

        //crc <<= Chunk_Bits;
        //crc ^= table[index];

        value &= Chunk_Mask;

        if ETL_IF_CONSTEXPR(TCrcParameters::Reflect)
        {
          uint8_t index = (crc & Chunk_Mask) ^ value;

          // special case for when chunk size is same as accumulator size.
          if ETL_IF_CONSTEXPR(Accumulator_Bits > Chunk_Bits) 
          {
            crc >>= Chunk_Bits;
            crc ^= table[index];
          } 
          else  
          {
            crc = table[index];
          }

          return crc;
        }
        else
        {
          // Extract the most significant nibble of the crc and xor with the value nibble
          uint8_t index = (crc >> (Accumulator_Bits - Chunk_Bits)) ^ value;

          // special case for when chunk size is same as accumulator size.
          if ETL_IF_CONSTEXPR(Accumulator_Bits > Chunk_Bits)
          {
            crc <<= Chunk_Bits;
            crc ^= table[index];
          }
          else
          {
            crc = table[index];
          }

          return crc;
        }
      }

      //*************************************************************************
      accumulator_type add(accumulator_type crc, uint8_t value) const
      {
        static ETL_CONSTANT accumulator_type table[Table_Size] =
        {
          crc_table_entry<TCrcParameters, 0U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 1U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 2U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 3U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 4U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 5U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 6U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 7U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 8U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 9U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 10U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 11U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 12U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 13U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 14U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 15U, Chunk_Bits>::value
        };

        if ETL_IF_CONSTEXPR(TCrcParameters::Reflect)
        {
          crc = update_chunk(crc, value & Chunk_Mask, table);
          crc = update_chunk(crc, value >> Chunk_Bits, table);
        }
        else
        {
          crc = update_chunk(crc, value >> Chunk_Bits, table);
          crc = update_chunk(crc, value & Chunk_Mask, table);
        }

        return crc;
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
    struct crc_policy<TCrcParameters, 4U>
    {
      typedef typename TCrcParameters::accumulator_type accumulator_type;
      typedef accumulator_type value_type;

      static ETL_CONSTANT size_t  Table_Size = 4U;
      static ETL_CONSTANT uint8_t Chunk_Mask = Table_Size - 1U;
      static ETL_CONSTANT size_t  Chunk_Bits = 2U;
      static ETL_CONSTANT size_t  Accumulator_Bits = TCrcParameters::Accumulator_Bits;

      //*************************************************************************
      ETL_CONSTEXPR accumulator_type initial() const
      {
        return TCrcParameters::Initial;
      }

      //*************************************************************************
      static accumulator_type update_chunk(accumulator_type crc, uint8_t value, const accumulator_type table[])
      {
        value &= Chunk_Mask;

        if ETL_IF_CONSTEXPR(TCrcParameters::Reflect)
        {
          uint8_t index = (crc & Chunk_Mask) ^ value;

          // special case for when chunk size is same as accumulator size.
          if ETL_IF_CONSTEXPR(Accumulator_Bits > Chunk_Bits) 
          {
            crc >>= Chunk_Bits;
            crc ^= table[index];
          } 
          else 
          {
            crc = table[index];
          }

          return crc;
        }
        else
        {
          // Extract the most significant nibble of the crc and xor with the value nibble
          uint8_t index = (crc >> (Accumulator_Bits - Chunk_Bits)) ^ value;

          // special case for when chunk size is same as accumulator size.
          if ETL_IF_CONSTEXPR(Accumulator_Bits > Chunk_Bits)
          {
            crc <<= Chunk_Bits;
            crc ^= table[index];
          }
          else
          {
            crc = table[index];
          }

          return crc;
        }
      }

      //*************************************************************************
      accumulator_type add(accumulator_type crc, uint8_t value) const
      {
        static ETL_CONSTANT accumulator_type table[Table_Size] =
        {
          crc_table_entry<TCrcParameters, 0U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 1U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 2U, Chunk_Bits>::value,
          crc_table_entry<TCrcParameters, 3U, Chunk_Bits>::value
        };
        
        if ETL_IF_CONSTEXPR(TCrcParameters::Reflect)
        {
          crc = update_chunk(crc, value & Chunk_Mask, table);
          crc = update_chunk(crc, (value >> (Chunk_Bits * 1U)) & Chunk_Mask, table);
          crc = update_chunk(crc, (value >> (Chunk_Bits * 2U)) & Chunk_Mask, table);
          crc = update_chunk(crc, (value >> (Chunk_Bits * 3U)) & Chunk_Mask, table);          
        }
        else
        {
          crc = update_chunk(crc, (value >> (Chunk_Bits * 3U)) & Chunk_Mask, table);
          crc = update_chunk(crc, (value >> (Chunk_Bits * 2U)) & Chunk_Mask, table);
          crc = update_chunk(crc, (value >> (Chunk_Bits * 1U)) & Chunk_Mask, table);
          crc = update_chunk(crc, value & Chunk_Mask, table);
        }

        return crc;
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
