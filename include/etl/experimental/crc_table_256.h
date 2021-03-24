
#pragma once

#include <stdint.h>
#include <limits.h>

#include "../binary.h"
#include "../integral_limits.h"

//*****************************************************************************
template <typename TAccumulator, TAccumulator Polynomial, TAccumulator Entry>
class entry
{
private:

  static ETL_CONSTANT size_t Accumulator_Bits = etl::integral_limits<TAccumulator>::bits;
  static ETL_CONSTANT bool   Do_Poly          = (Entry & (TAccumulator(1U) << (Accumulator_Bits - 1))) != 0U;

public:

  static const TAccumulator value = TAccumulator(Do_Poly ? (Entry << 1U) ^ Polynomial : (Entry << 1U));
};

//*****************************************************************************
/// 8 bit chunks
//*****************************************************************************
template <typename TAccumulator, size_t Index, TAccumulator Polynomial>
class table_entry_8
{
//private:
public:

  static const size_t Accumulator_Bits = entry<TAccumulator, Index, Polynomial>::Accumulator_Bits;
  static const size_t Shift_Bits       = size_t(Accumulator_Bits - 8U);
  static const TAccumulator Entry      = TAccumulator(Index << Shift_Bits);

public:

  static const TAccumulator value = entry<TAccumulator, Polynomial, 
                                    entry<TAccumulator, Polynomial, 
                                    entry<TAccumulator, Polynomial, 
                                    entry<TAccumulator, Polynomial,
                                    entry<TAccumulator, Polynomial, 
                                    entry<TAccumulator, Polynomial, 
                                    entry<TAccumulator, Polynomial, 
                                    entry<TAccumulator, Polynomial, Entry>::value>::value>::value>::value>::value>::value>::value>::value;
};

//*****************************************************************************
template <typename TAccumulator, TAccumulator Polynomial>
struct crc_table_256
{
  static const TAccumulator* get_table()
  {
    static const TAccumulator table[256] =
    {
      table_entry_8<TAccumulator, 0U, Polynomial>::value,
      table_entry_8<TAccumulator, 1U, Polynomial>::value,
      table_entry_8<TAccumulator, 2U, Polynomial>::value,
      table_entry_8<TAccumulator, 3U, Polynomial>::value,
      table_entry_8<TAccumulator, 4U, Polynomial>::value,
      table_entry_8<TAccumulator, 5U, Polynomial>::value,
      table_entry_8<TAccumulator, 6U, Polynomial>::value,
      table_entry_8<TAccumulator, 7U, Polynomial>::value,
      table_entry_8<TAccumulator, 8U, Polynomial>::value,
      table_entry_8<TAccumulator, 9U, Polynomial>::value,
      table_entry_8<TAccumulator, 10U, Polynomial>::value,
      table_entry_8<TAccumulator, 11U, Polynomial>::value,
      table_entry_8<TAccumulator, 12U, Polynomial>::value,
      table_entry_8<TAccumulator, 13U, Polynomial>::value,
      table_entry_8<TAccumulator, 14U, Polynomial>::value,
      table_entry_8<TAccumulator, 15U, Polynomial>::value,
      table_entry_8<TAccumulator, 16U, Polynomial>::value,
      table_entry_8<TAccumulator, 17U, Polynomial>::value,
      table_entry_8<TAccumulator, 18U, Polynomial>::value,
      table_entry_8<TAccumulator, 19U, Polynomial>::value,
      table_entry_8<TAccumulator, 20U, Polynomial>::value,
      table_entry_8<TAccumulator, 21U, Polynomial>::value,
      table_entry_8<TAccumulator, 22U, Polynomial>::value,
      table_entry_8<TAccumulator, 23U, Polynomial>::value,
      table_entry_8<TAccumulator, 24U, Polynomial>::value,
      table_entry_8<TAccumulator, 25U, Polynomial>::value,
      table_entry_8<TAccumulator, 26U, Polynomial>::value,
      table_entry_8<TAccumulator, 27U, Polynomial>::value,
      table_entry_8<TAccumulator, 28U, Polynomial>::value,
      table_entry_8<TAccumulator, 29U, Polynomial>::value,
      table_entry_8<TAccumulator, 30U, Polynomial>::value,
      table_entry_8<TAccumulator, 31U, Polynomial>::value,
      table_entry_8<TAccumulator, 32U, Polynomial>::value,
      table_entry_8<TAccumulator, 33U, Polynomial>::value,
      table_entry_8<TAccumulator, 34U, Polynomial>::value,
      table_entry_8<TAccumulator, 35U, Polynomial>::value,
      table_entry_8<TAccumulator, 36U, Polynomial>::value,
      table_entry_8<TAccumulator, 37U, Polynomial>::value,
      table_entry_8<TAccumulator, 38U, Polynomial>::value,
      table_entry_8<TAccumulator, 39U, Polynomial>::value,
      table_entry_8<TAccumulator, 40U, Polynomial>::value,
      table_entry_8<TAccumulator, 41U, Polynomial>::value,
      table_entry_8<TAccumulator, 42U, Polynomial>::value,
      table_entry_8<TAccumulator, 43U, Polynomial>::value,
      table_entry_8<TAccumulator, 44U, Polynomial>::value,
      table_entry_8<TAccumulator, 45U, Polynomial>::value,
      table_entry_8<TAccumulator, 46U, Polynomial>::value,
      table_entry_8<TAccumulator, 47U, Polynomial>::value,
      table_entry_8<TAccumulator, 48U, Polynomial>::value,
      table_entry_8<TAccumulator, 49U, Polynomial>::value,
      table_entry_8<TAccumulator, 50U, Polynomial>::value,
      table_entry_8<TAccumulator, 51U, Polynomial>::value,
      table_entry_8<TAccumulator, 52U, Polynomial>::value,
      table_entry_8<TAccumulator, 53U, Polynomial>::value,
      table_entry_8<TAccumulator, 54U, Polynomial>::value,
      table_entry_8<TAccumulator, 55U, Polynomial>::value,
      table_entry_8<TAccumulator, 56U, Polynomial>::value,
      table_entry_8<TAccumulator, 57U, Polynomial>::value,
      table_entry_8<TAccumulator, 58U, Polynomial>::value,
      table_entry_8<TAccumulator, 59U, Polynomial>::value,
      table_entry_8<TAccumulator, 60U, Polynomial>::value,
      table_entry_8<TAccumulator, 61U, Polynomial>::value,
      table_entry_8<TAccumulator, 62U, Polynomial>::value,
      table_entry_8<TAccumulator, 63U, Polynomial>::value,
      table_entry_8<TAccumulator, 64U, Polynomial>::value,
      table_entry_8<TAccumulator, 65U, Polynomial>::value,
      table_entry_8<TAccumulator, 66U, Polynomial>::value,
      table_entry_8<TAccumulator, 67U, Polynomial>::value,
      table_entry_8<TAccumulator, 68U, Polynomial>::value,
      table_entry_8<TAccumulator, 69U, Polynomial>::value,
      table_entry_8<TAccumulator, 70U, Polynomial>::value,
      table_entry_8<TAccumulator, 71U, Polynomial>::value,
      table_entry_8<TAccumulator, 72U, Polynomial>::value,
      table_entry_8<TAccumulator, 73U, Polynomial>::value,
      table_entry_8<TAccumulator, 74U, Polynomial>::value,
      table_entry_8<TAccumulator, 75U, Polynomial>::value,
      table_entry_8<TAccumulator, 76U, Polynomial>::value,
      table_entry_8<TAccumulator, 77U, Polynomial>::value,
      table_entry_8<TAccumulator, 78U, Polynomial>::value,
      table_entry_8<TAccumulator, 79U, Polynomial>::value,
      table_entry_8<TAccumulator, 80U, Polynomial>::value,
      table_entry_8<TAccumulator, 81U, Polynomial>::value,
      table_entry_8<TAccumulator, 82U, Polynomial>::value,
      table_entry_8<TAccumulator, 83U, Polynomial>::value,
      table_entry_8<TAccumulator, 84U, Polynomial>::value,
      table_entry_8<TAccumulator, 85U, Polynomial>::value,
      table_entry_8<TAccumulator, 86U, Polynomial>::value,
      table_entry_8<TAccumulator, 87U, Polynomial>::value,
      table_entry_8<TAccumulator, 88U, Polynomial>::value,
      table_entry_8<TAccumulator, 89U, Polynomial>::value,
      table_entry_8<TAccumulator, 90U, Polynomial>::value,
      table_entry_8<TAccumulator, 91U, Polynomial>::value,
      table_entry_8<TAccumulator, 92U, Polynomial>::value,
      table_entry_8<TAccumulator, 93U, Polynomial>::value,
      table_entry_8<TAccumulator, 94U, Polynomial>::value,
      table_entry_8<TAccumulator, 95U, Polynomial>::value,
      table_entry_8<TAccumulator, 96U, Polynomial>::value,
      table_entry_8<TAccumulator, 97U, Polynomial>::value,
      table_entry_8<TAccumulator, 98U, Polynomial>::value,
      table_entry_8<TAccumulator, 99U, Polynomial>::value,
      table_entry_8<TAccumulator, 100U, Polynomial>::value,
      table_entry_8<TAccumulator, 101U, Polynomial>::value,
      table_entry_8<TAccumulator, 102U, Polynomial>::value,
      table_entry_8<TAccumulator, 103U, Polynomial>::value,
      table_entry_8<TAccumulator, 104U, Polynomial>::value,
      table_entry_8<TAccumulator, 105U, Polynomial>::value,
      table_entry_8<TAccumulator, 106U, Polynomial>::value,
      table_entry_8<TAccumulator, 107U, Polynomial>::value,
      table_entry_8<TAccumulator, 108U, Polynomial>::value,
      table_entry_8<TAccumulator, 109U, Polynomial>::value,
      table_entry_8<TAccumulator, 110U, Polynomial>::value,
      table_entry_8<TAccumulator, 111U, Polynomial>::value,
      table_entry_8<TAccumulator, 112U, Polynomial>::value,
      table_entry_8<TAccumulator, 113U, Polynomial>::value,
      table_entry_8<TAccumulator, 114U, Polynomial>::value,
      table_entry_8<TAccumulator, 115U, Polynomial>::value,
      table_entry_8<TAccumulator, 116U, Polynomial>::value,
      table_entry_8<TAccumulator, 117U, Polynomial>::value,
      table_entry_8<TAccumulator, 118U, Polynomial>::value,
      table_entry_8<TAccumulator, 119U, Polynomial>::value,
      table_entry_8<TAccumulator, 120U, Polynomial>::value,
      table_entry_8<TAccumulator, 121U, Polynomial>::value,
      table_entry_8<TAccumulator, 122U, Polynomial>::value,
      table_entry_8<TAccumulator, 123U, Polynomial>::value,
      table_entry_8<TAccumulator, 124U, Polynomial>::value,
      table_entry_8<TAccumulator, 125U, Polynomial>::value,
      table_entry_8<TAccumulator, 126U, Polynomial>::value,
      table_entry_8<TAccumulator, 127U, Polynomial>::value,
      table_entry_8<TAccumulator, 128U, Polynomial>::value,
      table_entry_8<TAccumulator, 129U, Polynomial>::value,
      table_entry_8<TAccumulator, 130U, Polynomial>::value,
      table_entry_8<TAccumulator, 131U, Polynomial>::value,
      table_entry_8<TAccumulator, 132U, Polynomial>::value,
      table_entry_8<TAccumulator, 133U, Polynomial>::value,
      table_entry_8<TAccumulator, 134U, Polynomial>::value,
      table_entry_8<TAccumulator, 135U, Polynomial>::value,
      table_entry_8<TAccumulator, 136U, Polynomial>::value,
      table_entry_8<TAccumulator, 137U, Polynomial>::value,
      table_entry_8<TAccumulator, 138U, Polynomial>::value,
      table_entry_8<TAccumulator, 139U, Polynomial>::value,
      table_entry_8<TAccumulator, 140U, Polynomial>::value,
      table_entry_8<TAccumulator, 141U, Polynomial>::value,
      table_entry_8<TAccumulator, 142U, Polynomial>::value,
      table_entry_8<TAccumulator, 143U, Polynomial>::value,
      table_entry_8<TAccumulator, 144U, Polynomial>::value,
      table_entry_8<TAccumulator, 145U, Polynomial>::value,
      table_entry_8<TAccumulator, 146U, Polynomial>::value,
      table_entry_8<TAccumulator, 147U, Polynomial>::value,
      table_entry_8<TAccumulator, 148U, Polynomial>::value,
      table_entry_8<TAccumulator, 149U, Polynomial>::value,
      table_entry_8<TAccumulator, 150U, Polynomial>::value,
      table_entry_8<TAccumulator, 151U, Polynomial>::value,
      table_entry_8<TAccumulator, 152U, Polynomial>::value,
      table_entry_8<TAccumulator, 153U, Polynomial>::value,
      table_entry_8<TAccumulator, 154U, Polynomial>::value,
      table_entry_8<TAccumulator, 155U, Polynomial>::value,
      table_entry_8<TAccumulator, 156U, Polynomial>::value,
      table_entry_8<TAccumulator, 157U, Polynomial>::value,
      table_entry_8<TAccumulator, 158U, Polynomial>::value,
      table_entry_8<TAccumulator, 159U, Polynomial>::value,
      table_entry_8<TAccumulator, 160U, Polynomial>::value,
      table_entry_8<TAccumulator, 161U, Polynomial>::value,
      table_entry_8<TAccumulator, 162U, Polynomial>::value,
      table_entry_8<TAccumulator, 163U, Polynomial>::value,
      table_entry_8<TAccumulator, 164U, Polynomial>::value,
      table_entry_8<TAccumulator, 165U, Polynomial>::value,
      table_entry_8<TAccumulator, 166U, Polynomial>::value,
      table_entry_8<TAccumulator, 167U, Polynomial>::value,
      table_entry_8<TAccumulator, 168U, Polynomial>::value,
      table_entry_8<TAccumulator, 169U, Polynomial>::value,
      table_entry_8<TAccumulator, 170U, Polynomial>::value,
      table_entry_8<TAccumulator, 171U, Polynomial>::value,
      table_entry_8<TAccumulator, 172U, Polynomial>::value,
      table_entry_8<TAccumulator, 173U, Polynomial>::value,
      table_entry_8<TAccumulator, 174U, Polynomial>::value,
      table_entry_8<TAccumulator, 175U, Polynomial>::value,
      table_entry_8<TAccumulator, 176U, Polynomial>::value,
      table_entry_8<TAccumulator, 177U, Polynomial>::value,
      table_entry_8<TAccumulator, 178U, Polynomial>::value,
      table_entry_8<TAccumulator, 179U, Polynomial>::value,
      table_entry_8<TAccumulator, 180U, Polynomial>::value,
      table_entry_8<TAccumulator, 181U, Polynomial>::value,
      table_entry_8<TAccumulator, 182U, Polynomial>::value,
      table_entry_8<TAccumulator, 183U, Polynomial>::value,
      table_entry_8<TAccumulator, 184U, Polynomial>::value,
      table_entry_8<TAccumulator, 185U, Polynomial>::value,
      table_entry_8<TAccumulator, 186U, Polynomial>::value,
      table_entry_8<TAccumulator, 187U, Polynomial>::value,
      table_entry_8<TAccumulator, 188U, Polynomial>::value,
      table_entry_8<TAccumulator, 189U, Polynomial>::value,
      table_entry_8<TAccumulator, 190U, Polynomial>::value,
      table_entry_8<TAccumulator, 191U, Polynomial>::value,
      table_entry_8<TAccumulator, 192U, Polynomial>::value,
      table_entry_8<TAccumulator, 193U, Polynomial>::value,
      table_entry_8<TAccumulator, 194U, Polynomial>::value,
      table_entry_8<TAccumulator, 195U, Polynomial>::value,
      table_entry_8<TAccumulator, 196U, Polynomial>::value,
      table_entry_8<TAccumulator, 197U, Polynomial>::value,
      table_entry_8<TAccumulator, 198U, Polynomial>::value,
      table_entry_8<TAccumulator, 199U, Polynomial>::value,
      table_entry_8<TAccumulator, 200U, Polynomial>::value,
      table_entry_8<TAccumulator, 201U, Polynomial>::value,
      table_entry_8<TAccumulator, 202U, Polynomial>::value,
      table_entry_8<TAccumulator, 203U, Polynomial>::value,
      table_entry_8<TAccumulator, 204U, Polynomial>::value,
      table_entry_8<TAccumulator, 205U, Polynomial>::value,
      table_entry_8<TAccumulator, 206U, Polynomial>::value,
      table_entry_8<TAccumulator, 207U, Polynomial>::value,
      table_entry_8<TAccumulator, 208U, Polynomial>::value,
      table_entry_8<TAccumulator, 209U, Polynomial>::value,
      table_entry_8<TAccumulator, 210U, Polynomial>::value,
      table_entry_8<TAccumulator, 211U, Polynomial>::value,
      table_entry_8<TAccumulator, 212U, Polynomial>::value,
      table_entry_8<TAccumulator, 213U, Polynomial>::value,
      table_entry_8<TAccumulator, 214U, Polynomial>::value,
      table_entry_8<TAccumulator, 215U, Polynomial>::value,
      table_entry_8<TAccumulator, 216U, Polynomial>::value,
      table_entry_8<TAccumulator, 217U, Polynomial>::value,
      table_entry_8<TAccumulator, 218U, Polynomial>::value,
      table_entry_8<TAccumulator, 219U, Polynomial>::value,
      table_entry_8<TAccumulator, 220U, Polynomial>::value,
      table_entry_8<TAccumulator, 221U, Polynomial>::value,
      table_entry_8<TAccumulator, 222U, Polynomial>::value,
      table_entry_8<TAccumulator, 223U, Polynomial>::value,
      table_entry_8<TAccumulator, 224U, Polynomial>::value,
      table_entry_8<TAccumulator, 225U, Polynomial>::value,
      table_entry_8<TAccumulator, 226U, Polynomial>::value,
      table_entry_8<TAccumulator, 227U, Polynomial>::value,
      table_entry_8<TAccumulator, 228U, Polynomial>::value,
      table_entry_8<TAccumulator, 229U, Polynomial>::value,
      table_entry_8<TAccumulator, 230U, Polynomial>::value,
      table_entry_8<TAccumulator, 231U, Polynomial>::value,
      table_entry_8<TAccumulator, 232U, Polynomial>::value,
      table_entry_8<TAccumulator, 233U, Polynomial>::value,
      table_entry_8<TAccumulator, 234U, Polynomial>::value,
      table_entry_8<TAccumulator, 235U, Polynomial>::value,
      table_entry_8<TAccumulator, 236U, Polynomial>::value,
      table_entry_8<TAccumulator, 237U, Polynomial>::value,
      table_entry_8<TAccumulator, 238U, Polynomial>::value,
      table_entry_8<TAccumulator, 239U, Polynomial>::value,
      table_entry_8<TAccumulator, 240U, Polynomial>::value,
      table_entry_8<TAccumulator, 241U, Polynomial>::value,
      table_entry_8<TAccumulator, 242U, Polynomial>::value,
      table_entry_8<TAccumulator, 243U, Polynomial>::value,
      table_entry_8<TAccumulator, 244U, Polynomial>::value,
      table_entry_8<TAccumulator, 245U, Polynomial>::value,
      table_entry_8<TAccumulator, 246U, Polynomial>::value,
      table_entry_8<TAccumulator, 247U, Polynomial>::value,
      table_entry_8<TAccumulator, 248U, Polynomial>::value,
      table_entry_8<TAccumulator, 249U, Polynomial>::value,
      table_entry_8<TAccumulator, 250U, Polynomial>::value,
      table_entry_8<TAccumulator, 251U, Polynomial>::value,
      table_entry_8<TAccumulator, 252U, Polynomial>::value,
      table_entry_8<TAccumulator, 253U, Polynomial>::value,
      table_entry_8<TAccumulator, 254U, Polynomial>::value,
      table_entry_8<TAccumulator, 255U, Polynomial>::value
    };

    return table;
  }
};
//
////
//// Define the CRC algorithm parameters
////
//template <typename TAccumulator, TAccumulator Polynomial, TAccumulator Initial, TAccumulator Xor_Out, bool Reverse>
//struct crc_algorithm
//{
//  using accumulator_type = TAccumulator;
//  static constexpr TAccumulator polynomial = Polynomial;
//  static constexpr TAccumulator initial_value = Initial;
//  static constexpr TAccumulator xor_out_value = Xor_Out;
//  static constexpr bool reverse = Reverse;
//};
//
////
//// Define the basic traits we use in our CRC accumulators and Table Lookup
////
//template <typename TAccumulator, const size_t Table_Size>
//struct crc_traits {};
//
//template <typename TAccumulator>
//struct crc_traits_256
//{
//  static constexpr std::size_t ACCUMULATOR_BITS = etl::integral_limits<TAccumulator>::bits;
//  static constexpr std::size_t CHUNK_BITS = 8;
//  static constexpr uint8_t CHUNK_MASK = 0xFFu;
//  static constexpr std::size_t TABLE_ENTRIES = 1u << CHUNK_BITS;
//
//  using table_type = std::array<TAccumulator, TABLE_ENTRIES>;
//};
//
////
//// CRC rotation policies.
////
//// Forward rotation means that we clock in data MSB->LSB and rotate the Accumulator register left
//// Reverse rotation means that we clock in data LSB->MSB and rotate the Accumulator register right
////
//
//template <typename TAccumulator>
//struct crc_forward_policy_256
//{
//  typedef crc_traits_256<TAccumulator> traits;
//
//  ETL_NODISCARD static constexpr TAccumulator update_impl_256(TAccumulator crc, 
//                                                                uint8_t value, 
//                                                                typename traits::table_type const& table)
//  {
//    crc = update_chunk(crc, value, table); // full byte
//    return crc;
//  }
//
//  ETL_NODISCARD static constexpr TAccumulator update_chunk(TAccumulator crc, 
//                                                           uint8_t value, 
//                                                           typename traits::table_type const& table)
//  {
//    value &= traits::CHUNK_MASK;
//
//    // Extract the most significant nibble of the crc and xor with the value nibble
//    uint8_t t = (crc >> (traits::ACCUMULATOR_BITS - traits::CHUNK_BITS)) ^ value;
//
//    // special case for when chunk size is same as accumulator size.
//    if constexpr (traits::ACCUMULATOR_BITS > traits::CHUNK_BITS) {
//      // shift crc left the size of the nibble
//      crc <<= traits::CHUNK_BITS;
//
//      // xor in the table data
//      crc ^= table[t];
//    }
//    else {
//      crc = table[t];
//    }
//
//    return crc;
//  }
//
//  ETL_NODISCARD static constexpr TAccumulator make_initial_value(TAccumulator init)
//  {
//    return init;
//  }
//};
//
//template <typename TAccumulator, const size_t Table_Size>
//struct crc_reverse_policy_256
//{
//  typedef crc_traits_256<TAccumulator> traits;
//
//  ETL_NODISCARD static constexpr TAccumulator update_impl(
//    TAccumulator crc, uint8_t value, typename traits::table_type const& table)
//  {
//    crc = update_chunk(crc, value, table); // full byte
//    return crc;
//  }
//
//  ETL_NODISCARD static constexpr TAccumulator update_chunk(
//    TAccumulator crc, uint8_t value, typename traits::table_type const& table)
//  {
//    // ensure we have only 4 bits
//    value &= traits::CHUNK_MASK;
//
//    // Extract the least significant nibble of the crc and xor with the value nibble
//    uint8_t t = (crc & traits::CHUNK_MASK) ^ value;
//
//    // special case for when chunk size is same as accumulator size.
//    if constexpr (traits::ACCUMULATOR_BITS > traits::CHUNK_BITS) {
//      // shit crc right the size of a nibble
//      crc >>= traits::CHUNK_BITS;
//
//      // xor in the table data
//      crc ^= table[t];
//    }
//    else {
//      crc = table[t];
//    }
//
//    return crc;
//  }
//
//  ETL_NODISCARD static constexpr TAccumulator make_initial_value(TAccumulator init)
//  {
//    return etl::reverse_bits(init);
//  }
//};
//
//
////
//// A generic CRC lookup table sized for computing a nibble (4 bits) at a time.
////
//// This is can be a large reduction of table space storage for embedded devices.
////
//template <typename TAccumulator, TAccumulator Polynomial, bool Reverse, size_t Table_Size, typename = std::enable_if_t<std::is_unsigned<TAccumulator>::value>>
//class crc_chunk_table
//{
//public:
//
//  using policy = typename std::conditional<Reverse, crc_reverse_policy<TAccumulator, Table_Size>, crc_forward_policy<TAccumulator, Table_Size>>::type;
//
//  using traits = typename policy::traits;
//
//
//  // update the given crc accumulator with the value
//  ETL_NODISCARD static constexpr TAccumulator update(TAccumulator crc, uint8_t value)
//  {
//    if constexpr (Table_Size == size_t::tiny) {
//      return policy::update_impl_tiny(crc, value, m_Table);
//    }
//    else if constexpr (Table_Size == size_t::small) {
//      return policy::update_impl_small(crc, value, m_Table);
//    }
//    else if constexpr (Table_Size == size_t::large) {
//      return policy::update_impl_large(crc, value, m_Table);
//    }
//    else {
//      static_assert("Unknown size_t");
//    }
//  }
//
//  // the crc accumulator initial value may need to be modified by the policy
//  // to account for rotation direction
//  ETL_NODISCARD static constexpr TAccumulator make_initial_value(TAccumulator init)
//  {
//    return policy::make_initial_value(init);
//  }
//
//private:
//
//};
//
//
////
//// The generic CRC accumulator that is table driven
////
//template <typename TAlgorithm, size_t Table_Size>
//class crc
//{
//public:
//  using algorithm = TAlgorithm;
//  using accumulator_type = typename algorithm::accumulator_type;
//
//  //
//  // Update the accumulator with a new byte
//  //
//  void update(uint8_t value) 
//  { 
//    m_crc = table_impl::update(m_crc, value); 
//  }
//
//  //
//  // Extract the final value of the accumulator.
//  //
//  ETL_NODISCARD accumulator_type final() { return m_crc ^ algorithm::xor_out_value; }
//
//  //
//  // Reset the state of the accumulator back to the INITIAL value.
//  //
//  void reset() { m_crc = table_impl::make_initial_value(algorithm::initial_value); }
//
//
//private:
//  using table_impl = crc_chunk_table<accumulator_type, algorithm::polynomial, algorithm::reverse, Table_Size>;
//
//  accumulator_type m_crc = table_impl::make_initial_value(algorithm::initial_value);
//};