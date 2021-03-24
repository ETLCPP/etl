
#pragma once

#include <stdint.h>
#include <limits.h>

//*****************************************************************************
template <typename TAccumulator, TAccumulator Polynomial, TAccumulator Entry>
class entry
{
private:

  static const size_t Accumulator_Bits = sizeof(TAccumulator) * CHAR_BIT;
  static const bool   Do_Poly          = (Entry & (TAccumulator(1U) << (Accumulator_Bits - 1))) != 0U;

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

  static const size_t Accumulator_Bits = sizeof(TAccumulator) * CHAR_BIT;
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
/// 4 bit chunks
//*****************************************************************************
template <typename TAccumulator, size_t Index, TAccumulator Polynomial>
class table_entry_4
{
private:

  static const size_t Accumulator_Bits = sizeof(TAccumulator) * CHAR_BIT;
  static const size_t Shift_Bits       = size_t(Accumulator_Bits - Chunk_4_Bits);
  static const TAccumulator Entry      = TAccumulator(Index << Shift_Bits);

public:

  static const TAccumulator value = entry<TAccumulator, Polynomial,
                                    entry<TAccumulator, Polynomial,
                                    entry<TAccumulator, Polynomial,
                                    entry<TAccumulator, Polynomial, Entry>::value>::value>::value>::value;
};

//*****************************************************************************
/// 2 bit chunks
//*****************************************************************************
template <typename TAccumulator, size_t Index, TAccumulator Polynomial>
class table_entry_2
{
private:

  static const size_t Accumulator_Bits = sizeof(TAccumulator) * CHAR_BIT;
  static const size_t Shift_Bits       = size_t(Accumulator_Bits - Chunk_2_Bits);
  static const TAccumulator Entry      = TAccumulator(Index << Shift_Bits);

public:

  static const TAccumulator value = entry<TAccumulator, Polynomial,
                                    entry<TAccumulator, Polynomial, Entry>::value>::value;
};

//*****************************************************************************
template <typename TAccumulator, TAccumulator Polynomial>
struct crc_table<TAccumulator, Polynomial, 256>
{
  static const TAccumulator* get_table()
  {
    static const TAccumulator table[256] =
    {
      table_entry<TAccumulator, 0U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 1U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 2U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 3U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 4U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 5U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 6U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 7U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 8U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 9U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 10U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 11U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 12U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 13U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 14U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 15U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 16U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 17U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 18U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 19U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 20U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 21U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 22U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 23U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 24U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 25U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 26U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 27U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 28U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 29U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 30U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 31U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 32U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 33U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 34U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 35U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 36U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 37U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 38U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 39U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 40U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 41U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 42U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 43U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 44U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 45U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 46U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 47U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 48U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 49U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 50U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 51U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 52U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 53U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 54U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 55U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 56U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 57U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 58U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 59U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 60U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 61U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 62U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 63U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 64U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 65U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 66U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 67U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 68U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 69U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 70U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 71U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 72U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 73U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 74U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 75U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 76U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 77U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 78U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 79U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 80U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 81U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 82U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 83U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 84U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 85U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 86U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 87U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 88U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 89U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 90U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 91U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 92U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 93U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 94U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 95U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 96U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 97U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 98U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 99U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 100U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 101U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 102U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 103U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 104U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 105U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 106U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 107U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 108U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 109U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 110U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 111U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 112U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 113U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 114U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 115U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 116U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 117U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 118U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 119U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 120U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 121U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 122U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 123U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 124U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 125U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 126U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 127U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 128U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 129U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 130U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 131U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 132U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 133U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 134U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 135U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 136U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 137U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 138U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 139U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 140U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 141U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 142U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 143U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 144U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 145U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 146U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 147U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 148U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 149U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 150U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 151U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 152U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 153U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 154U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 155U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 156U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 157U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 158U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 159U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 160U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 161U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 162U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 163U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 164U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 165U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 166U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 167U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 168U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 169U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 170U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 171U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 172U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 173U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 174U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 175U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 176U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 177U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 178U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 179U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 180U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 181U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 182U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 183U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 184U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 185U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 186U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 187U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 188U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 189U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 190U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 191U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 192U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 193U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 194U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 195U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 196U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 197U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 198U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 199U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 200U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 201U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 202U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 203U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 204U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 205U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 206U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 207U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 208U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 209U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 210U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 211U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 212U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 213U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 214U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 215U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 216U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 217U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 218U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 219U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 220U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 221U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 222U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 223U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 224U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 225U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 226U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 227U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 228U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 229U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 230U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 231U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 232U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 233U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 234U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 235U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 236U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 237U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 238U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 239U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 240U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 241U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 242U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 243U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 244U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 245U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 246U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 247U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 248U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 249U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 250U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 251U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 252U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 253U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 254U, Polynomial, Chunk_8_Bits>::value,
      table_entry<TAccumulator, 255U, Polynomial, Chunk_8_Bits>::value
    };

    return table;
  }
};

template <typename TAccumulator, TAccumulator Polynomial>
struct crc_table<TAccumulator, Polynomial, 16>
{
  static const TAccumulator* get_table()
  {
    static const TAccumulator table[16] =
    {
      table_entry<TAccumulator, 0U, Polynomial, Chunk_4_Bits>::value,
      table_entry<TAccumulator, 1U, Polynomial, Chunk_4_Bits>::value,
      table_entry<TAccumulator, 2U, Polynomial, Chunk_4_Bits>::value,
      table_entry<TAccumulator, 3U, Polynomial, Chunk_4_Bits>::value,
      table_entry<TAccumulator, 4U, Polynomial, Chunk_4_Bits>::value,
      table_entry<TAccumulator, 5U, Polynomial, Chunk_4_Bits>::value,
      table_entry<TAccumulator, 6U, Polynomial, Chunk_4_Bits>::value,
      table_entry<TAccumulator, 7U, Polynomial, Chunk_4_Bits>::value,
      table_entry<TAccumulator, 8U, Polynomial, Chunk_4_Bits>::value,
      table_entry<TAccumulator, 9U, Polynomial, Chunk_4_Bits>::value,
      table_entry<TAccumulator, 10U, Polynomial, Chunk_4_Bits>::value,
      table_entry<TAccumulator, 11U, Polynomial, Chunk_4_Bits>::value,
      table_entry<TAccumulator, 12U, Polynomial, Chunk_4_Bits>::value,
      table_entry<TAccumulator, 13U, Polynomial, Chunk_4_Bits>::value,
      table_entry<TAccumulator, 14U, Polynomial, Chunk_4_Bits>::value,
      table_entry<TAccumulator, 15U, Polynomial, Chunk_4_Bits>::value
    };

    return table;
  }
};

template <typename TAccumulator, TAccumulator Polynomial>
struct crc_table<TAccumulator, Polynomial, 4>
{
  static const TAccumulator* get_table()
  {
    static const TAccumulator table[4] =
    {
      table_entry<TAccumulator, 0U, Polynomial, Chunk_2_Bits>::value,
      table_entry<TAccumulator, 1U, Polynomial, Chunk_2_Bits>::value,
      table_entry<TAccumulator, 2U, Polynomial, Chunk_2_Bits>::value,
      table_entry<TAccumulator, 3U, Polynomial, Chunk_2_Bits>::value
    };

    return table;
  }
};


//
// Define the CRC algorithm parameters
//
template <typename TAccumulator, TAccumulator Polynomial, TAccumulator Initial, TAccumulator Xor_Out, bool Reverse>
struct crc_algorithm
{
  using accumulator_type = TAccumulator;
  static constexpr TAccumulator polynomial = Polynomial;
  static constexpr TAccumulator initial_value = Initial;
  static constexpr TAccumulator xor_out_value = Xor_Out;
  static constexpr bool reverse = Reverse;
};

//
// Define the basic traits we use in our CRC accumulators and Table Lookup
//
template <typename TAccumulator, const size_t Table_Size>
struct crc_traits {};

template <typename TAccumulator>
struct crc_traits<TAccumulator, size_t::tiny>
{
  static constexpr std::size_t ACCUMULATOR_BITS = sizeof(TAccumulator) * 8;
  static constexpr std::size_t CHUNK_BITS = 2;
  static constexpr uint8_t CHUNK_MASK = 0x03u;
  static constexpr std::size_t TABLE_ENTRIES = 1u << CHUNK_BITS;

  using table_type = std::array<TAccumulator, TABLE_ENTRIES>;
};

template <typename TAccumulator>
struct crc_traits<TAccumulator, size_t::small>
{
  static constexpr std::size_t ACCUMULATOR_BITS = sizeof(TAccumulator) * 8;
  static constexpr std::size_t CHUNK_BITS = 4;
  static constexpr uint8_t CHUNK_MASK = 0x0Fu;
  static constexpr std::size_t TABLE_ENTRIES = 1u << CHUNK_BITS;

  using table_type = std::array<TAccumulator, TABLE_ENTRIES>;
};

template <typename TAccumulator>
struct crc_traits<TAccumulator, size_t::large>
{
  static constexpr std::size_t ACCUMULATOR_BITS = sizeof(TAccumulator) * 8;
  static constexpr std::size_t CHUNK_BITS = 8;
  static constexpr uint8_t CHUNK_MASK = 0xFFu;
  static constexpr std::size_t TABLE_ENTRIES = 1u << CHUNK_BITS;

  using table_type = std::array<TAccumulator, TABLE_ENTRIES>;
};

//
// CRC rotation policies.
//
// Forward rotation means that we clock in data MSB->LSB and rotate the Accumulator register left
// Reverse rotation means that we clock in data LSB->MSB and rotate the Accumulator register right
//

template <typename TAccumulator, const size_t Table_Size>
struct crc_forward_policy
{
  using traits = crc_traits<TAccumulator, Table_Size>;


  [[nodiscard]] static constexpr TAccumulator update_impl_tiny(
    TAccumulator crc, uint8_t value, typename traits::table_type const& table)
  {
    crc = update_chunk(crc, (value >> (traits::CHUNK_BITS * 3)) & traits::CHUNK_MASK, table);     // high chunk
    crc = update_chunk(crc, (value >> (traits::CHUNK_BITS * 2)) & traits::CHUNK_MASK, table);
    crc = update_chunk(crc, (value >> (traits::CHUNK_BITS * 1)) & traits::CHUNK_MASK, table);
    crc = update_chunk(crc, value & traits::CHUNK_MASK, table);                                 // low chunk
    return crc;
  }

  [[nodiscard]] static constexpr TAccumulator update_impl_small(
    TAccumulator crc, uint8_t value, typename traits::table_type const& table)
  {
    crc = update_chunk(crc, value >> traits::CHUNK_BITS, table);     // high nibble
    crc = update_chunk(crc, value & traits::CHUNK_MASK, table);      // low nibble
    return crc;
  }

  [[nodiscard]] static constexpr TAccumulator update_impl_large(
    TAccumulator crc, uint8_t value, typename traits::table_type const& table)
  {
    crc = update_chunk(crc, value, table); // full byte
    return crc;
  }

  [[nodiscard]] static constexpr TAccumulator update_chunk(
    TAccumulator crc, uint8_t value, typename traits::table_type const& table)
  {
    // ensure we have only 4 bits
    value &= traits::CHUNK_MASK;

    // Extract the most significant nibble of the crc and xor with the value nibble
    uint8_t t = (crc >> (traits::ACCUMULATOR_BITS - traits::CHUNK_BITS)) ^ value;

    // special case for when chunk size is same as accumulator size.
    if constexpr (traits::ACCUMULATOR_BITS > traits::CHUNK_BITS) {
      // shit crc left the size of the nibble
      crc <<= traits::CHUNK_BITS;

      // xor in the table data
      crc ^= table[t];
    }
    else {
      crc = table[t];
    }

    return crc;
  }

  [[nodiscard]] static constexpr TAccumulator generate_entry(TAccumulator const polynomial, uint8_t const index)
  {
    // initialise with the register in the upper bits
    TAccumulator entry = TAccumulator(index) << (traits::ACCUMULATOR_BITS - traits::CHUNK_BITS);

    for (std::size_t i = 0; i < traits::CHUNK_BITS; i++)
    {
      // We are processing MSBs / rotating left so we need to check the high bit
      if (entry & (TAccumulator(1u) << (traits::ACCUMULATOR_BITS - 1))) {
        entry = (entry << 1) ^ polynomial;
      }
      else {
        entry = (entry << 1);
      }
    }

    return entry;
  }

  [[nodiscard]] static constexpr TAccumulator make_initial_value(TAccumulator init)
  {
    return init;
  }
};

template <typename TAccumulator, const size_t Table_Size>
struct crc_reverse_policy
{
  using traits = crc_traits<TAccumulator, Table_Size>;


  [[nodiscard]] static constexpr TAccumulator update_impl_tiny(
    TAccumulator crc, uint8_t value, typename traits::table_type const& table)
  {
    crc = update_chunk(crc, value & traits::CHUNK_MASK, table);                                 // low chunk
    crc = update_chunk(crc, (value >> (traits::CHUNK_BITS * 1)) & traits::CHUNK_MASK, table);
    crc = update_chunk(crc, (value >> (traits::CHUNK_BITS * 2)) & traits::CHUNK_MASK, table);
    crc = update_chunk(crc, (value >> (traits::CHUNK_BITS * 3)) & traits::CHUNK_MASK, table);     // high chunk
    return crc;
  }

  [[nodiscard]] static constexpr TAccumulator update_impl_small(
    TAccumulator crc, uint8_t value, typename traits::table_type const& table)
  {
    crc = update_chunk(crc, value & traits::CHUNK_MASK, table);      // low nibble
    crc = update_chunk(crc, value >> traits::CHUNK_BITS, table);     // high nibble
    return crc;
  }

  [[nodiscard]] static constexpr TAccumulator update_impl_large(
    TAccumulator crc, uint8_t value, typename traits::table_type const& table)
  {
    crc = update_chunk(crc, value, table); // full byte
    return crc;
  }

  [[nodiscard]] static constexpr TAccumulator update_chunk(
    TAccumulator crc, uint8_t value, typename traits::table_type const& table)
  {
    // ensure we have only 4 bits
    value &= traits::CHUNK_MASK;

    // Extract the least significant nibble of the crc and xor with the value nibble
    uint8_t t = (crc & traits::CHUNK_MASK) ^ value;

    // special case for when chunk size is same as accumulator size.
    if constexpr (traits::ACCUMULATOR_BITS > traits::CHUNK_BITS) {
      // shit crc right the size of a nibble
      crc >>= traits::CHUNK_BITS;

      // xor in the table data
      crc ^= table[t];
    }
    else {
      crc = table[t];
    }

    return crc;
  }

  [[nodiscard]] static constexpr TAccumulator generate_entry(TAccumulator const polynomial, uint8_t const index)
  {
    // initialise with the register in the lower bits
    TAccumulator entry = TAccumulator(index);

    for (std::size_t i = 0; i < traits::CHUNK_BITS; i++)
    {
      // we are processing LSBs/rotating right
      if (entry & 0x1u) {
        entry = (entry >> 1) ^ util::reverse_bits(polynomial);
      }
      else {
        entry = (entry >> 1);
      }
    }

    return entry;
  }

  [[nodiscard]] static constexpr TAccumulator make_initial_value(TAccumulator init)
  {
    return util::reverse_bits(init);
  }
};


//
// A generic CRC lookup table sized for computing a nibble (4 bits) at a time.
//
// This is can be a large reduction of table space storage for embedded devices.
//
template <typename TAccumulator, TAccumulator Polynomial, bool Reverse, size_t Table_Size, typename = std::enable_if_t<std::is_unsigned<TAccumulator>::value>>
class crc_chunk_table
{
public:

  using policy = typename std::conditional<Reverse, crc_reverse_policy<TAccumulator, Table_Size>, crc_forward_policy<TAccumulator, Table_Size>>::type;

  using traits = typename policy::traits;


  // update the given crc accumulator with the value
  [[nodiscard]] static constexpr TAccumulator update(TAccumulator crc, uint8_t value)
  {
    if constexpr (Table_Size == size_t::tiny) {
      return policy::update_impl_tiny(crc, value, m_Table);
    }
    else if constexpr (Table_Size == size_t::small) {
      return policy::update_impl_small(crc, value, m_Table);
    }
    else if constexpr (Table_Size == size_t::large) {
      return policy::update_impl_large(crc, value, m_Table);
    }
    else {
      static_assert("Unknown size_t");
    }
  }

  // the crc accumulator initial value may need to be modified by the policy
  // to account for rotation direction
  [[nodiscard]] static constexpr TAccumulator make_initial_value(TAccumulator init)
  {
    return policy::make_initial_value(init);
  }

private:

};


//
// The generic CRC accumulator that is table driven
//
template <typename TAlgorithm, size_t Table_Size>
class crc
{
public:
  using algorithm = TAlgorithm;
  using accumulator_type = typename algorithm::accumulator_type;

  //
  // Update the accumulator with a new byte
  //
  void update(uint8_t value) 
  { 
    m_crc = table_impl::update(m_crc, value); 
  }

  //
  // Extract the final value of the accumulator.
  //
  [[nodiscard]] accumulator_type final() { return m_crc ^ algorithm::xor_out_value; }

  //
  // Reset the state of the accumulator back to the INITIAL value.
  //
  void reset() { m_crc = table_impl::make_initial_value(algorithm::initial_value); }


private:
  using table_impl = crc_chunk_table<accumulator_type, algorithm::polynomial, algorithm::reverse, Table_Size>;

  accumulator_type m_crc = table_impl::make_initial_value(algorithm::initial_value);
};