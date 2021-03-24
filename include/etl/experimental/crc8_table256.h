
#pragma once

#include "../platform.h"
#include "../frame_check_sequence.h"
#include "../binary.h"

#include "crc_common.h"

#include "stdint.h"

namespace etl
{
  namespace crc
  {
    //*****************************************************************************
    //
    //*****************************************************************************
    template <uint8_t Polynomial, uint8_t Initial, uint8_t Xor_Out, bool Reflect>
    struct crc8_policy_table256
    {
      static ETL_CONSTANT uint8_t Chunk_Mask = etl::private_crc::crc_traits<256U>::Chunk_Mask;
      static ETL_CONSTANT uint8_t Chunk_Bits = etl::private_crc::crc_traits<256U>::Chunk_Bits;

      typedef uint8_t value_type;

      //*************************************************************************
      ETL_CONSTEXPR uint8_t initial() const
      {
        return Initial;
      }

      //*************************************************************************
      static uint8_t update_chunk(uint8_t crc, uint8_t value, const uint8_t* const table)
      {
        uint8_t t = crc ^ value;

        crc = table[t];

        return crc;
      }

      //*************************************************************************
      uint8_t add(uint8_t crc, uint8_t value) const
      {
        static ETL_CONSTANT uint8_t table[256] =
        {
          etl::private_crc::table_entry<uint8_t, 0U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 1U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 2U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 3U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 4U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 5U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 6U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 7U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 8U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 9U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 10U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 11U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 12U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 13U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 14U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 15U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 16U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 17U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 18U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 19U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 20U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 21U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 22U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 23U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 24U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 25U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 26U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 27U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 28U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 29U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 30U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 31U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 32U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 33U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 34U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 35U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 36U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 37U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 38U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 39U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 40U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 41U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 42U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 43U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 44U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 45U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 46U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 47U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 48U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 49U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 50U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 51U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 52U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 53U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 54U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 55U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 56U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 57U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 58U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 59U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 60U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 61U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 62U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 63U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 64U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 65U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 66U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 67U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 68U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 69U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 70U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 71U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 72U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 73U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 74U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 75U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 76U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 77U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 78U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 79U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 80U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 81U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 82U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 83U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 84U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 85U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 86U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 87U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 88U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 89U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 90U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 91U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 92U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 93U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 94U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 95U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 96U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 97U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 98U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 99U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 100U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 101U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 102U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 103U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 104U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 105U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 106U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 107U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 108U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 109U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 110U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 111U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 112U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 113U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 114U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 115U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 116U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 117U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 118U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 119U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 120U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 121U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 122U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 123U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 124U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 125U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 126U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 127U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 128U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 129U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 130U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 131U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 132U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 133U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 134U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 135U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 136U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 137U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 138U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 139U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 140U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 141U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 142U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 143U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 144U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 145U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 146U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 147U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 148U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 149U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 150U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 151U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 152U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 153U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 154U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 155U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 156U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 157U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 158U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 159U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 160U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 161U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 162U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 163U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 164U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 165U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 166U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 167U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 168U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 169U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 170U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 171U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 172U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 173U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 174U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 175U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 176U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 177U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 178U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 179U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 180U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 181U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 182U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 183U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 184U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 185U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 186U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 187U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 188U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 189U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 190U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 191U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 192U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 193U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 194U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 195U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 196U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 197U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 198U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 199U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 200U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 201U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 202U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 203U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 204U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 205U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 206U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 207U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 208U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 209U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 210U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 211U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 212U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 213U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 214U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 215U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 216U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 217U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 218U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 219U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 220U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 221U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 222U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 223U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 224U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 225U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 226U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 227U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 228U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 229U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 230U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 231U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 232U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 233U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 234U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 235U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 236U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 237U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 238U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 239U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 240U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 241U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 242U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 243U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 244U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 245U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 246U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 247U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 248U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 249U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 250U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 251U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 252U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 253U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 254U, Polynomial, Reflect, Chunk_Bits>::value,
          etl::private_crc::table_entry<uint8_t, 255U, Polynomial, Reflect, Chunk_Bits>::value
        };

        crc = update_chunk(crc, value, table);
    
        return crc;
      }

      //*************************************************************************
      uint8_t final(uint8_t crc) const
      {
        return crc ^ Xor_Out;
      }
    };

    //*****************************************************************************
    //
    //*****************************************************************************
    template <uint8_t Polynomial, uint8_t Initial, uint8_t Xor_In, uint8_t Xor_Out, bool Reflect>
    class crc8_table256 : public etl::frame_check_sequence<etl::crc::crc8_policy_table256<Polynomial, Initial, Xor_Out, Reflect> >
    {
    public:

      //*************************************************************************
      /// Default constructor.
      //*************************************************************************
      crc8_table256()
      {
        this->reset();
      }

      //*************************************************************************
      /// Constructor from range.
      /// \param begin Start of the range.
      /// \param end   End of the range.
      //*************************************************************************
      template<typename TIterator>
      crc8_table256(TIterator begin, const TIterator end)
      {
        this->reset();
        this->add(begin, end);
      }
    };
  }
}
