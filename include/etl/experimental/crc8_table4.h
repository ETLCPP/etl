
#pragma once

#include "../platform.h"
#include "../frame_check_sequence.h"

#include "../binary.h"

#include "stdint.h"

namespace etl
{
  namespace crc
  {
    //*****************************************************************************
    //
    //*****************************************************************************
    template <uint8_t Polynomial, uint8_t Initial, uint8_t Xor_Out, bool Reflect>
    struct crc8_policy_table4;

    //*****************************************************************************
    // Specialisation for Reflect = false
    //*****************************************************************************
    template <uint8_t Polynomial, uint8_t Initial, uint8_t Xor_Out>
    struct crc8_policy_table4<Polynomial, Initial, Xor_Out, false>
    {
      static ETL_CONSTANT uint8_t Mask       = 0x03U;
      static ETL_CONSTANT uint8_t Chunk_Size = 2U;

      typedef uint8_t value_type;

      //*****************************************************************************
      template <uint8_t Polynomial, uint8_t Entry>
      class entry
      {
      public:

        static ETL_CONSTANT size_t  Accumulator_Bits = etl::integral_limits<uint8_t>::bits;
        static ETL_CONSTANT bool    Do_Poly          = (Entry & (uint8_t(1U) << (Accumulator_Bits - 1U))) != 0U;
        static ETL_CONSTANT uint8_t value            = uint8_t(Do_Poly ? (Entry << 1U) ^ Polynomial : (Entry << 1U));
      };

      //*************************************************************************
      template <size_t Index, uint8_t Polynomial>
      class table_entry
      {
      public:

        static ETL_CONSTANT size_t  Accumulator_Bits = entry<Index, Polynomial>::Accumulator_Bits;
        static ETL_CONSTANT size_t  Shift_Bits       = size_t(Accumulator_Bits - Chunk_Size);
        static ETL_CONSTANT uint8_t Entry            = uint8_t(Index << Shift_Bits);

        static ETL_CONSTANT uint8_t value = entry<Polynomial,
                                            entry<Polynomial, Entry>::value>::value;
      };

      //*************************************************************************
      ETL_CONSTEXPR uint8_t initial() const
      {
        return Initial;
      }

      //*************************************************************************
      uint8_t update_chunk(uint8_t crc, uint8_t value, const uint8_t* table) const
      {
        // Extract the most significant nibble of the crc and xor with the value nibble.
        uint8_t index = (crc >> (etl::integral_limits<uint8_t>::bits - Chunk_Size)) ^ value;

        crc <<= Chunk_Size;
        crc ^= table[index];

        return crc;
      }

      //*************************************************************************
      uint8_t add(uint8_t crc, uint8_t value) const
      {
        static ETL_CONSTANT uint8_t table[4] =
        {
          table_entry<0U, Polynomial>::value,
          table_entry<1U, Polynomial>::value,
          table_entry<2U, Polynomial>::value,
          table_entry<3U, Polynomial>::value
        };

        crc = update_chunk(crc, (value >> (Chunk_Size * 3U)) & Mask, table);
        crc = update_chunk(crc, (value >> (Chunk_Size * 2U)) & Mask, table);
        crc = update_chunk(crc, (value >> (Chunk_Size * 1U)) & Mask, table);
        crc = update_chunk(crc, value & Mask, table);
    
        return crc;
      }

      //*************************************************************************
      uint8_t final(uint8_t crc) const
      {
        return crc ^ Xor_Out;
      }
    };

    //*****************************************************************************
    // Specialisation for Reflect = true
    //*****************************************************************************
    template <uint8_t Polynomial, uint8_t Initial, uint8_t Xor_Out>
    struct crc8_policy_table4<Polynomial, Initial, Xor_Out, true>
    {
      static ETL_CONSTANT uint8_t Mask       = 0x03;
      static ETL_CONSTANT uint8_t Chunk_Size = 2U;

      typedef uint8_t value_type;

      //*****************************************************************************
      template <uint8_t Polynomial, uint8_t Entry>
      class entry
      {
      public:

        static ETL_CONSTANT size_t  Accumulator_Bits = etl::integral_limits<uint8_t>::bits;
        static ETL_CONSTANT bool    Do_Poly          = (Entry & uint8_t(1U)) != 0U;
        static ETL_CONSTANT uint8_t value            = uint8_t(Do_Poly ? (Entry >> 1U) ^ etl::reverse_bits_const<uint8_t, Polynomial>::value : (Entry >> 1U));
      };

      //*************************************************************************
      template <size_t Index, uint8_t Polynomial>
      class table_entry
      {
      public:

        static ETL_CONSTANT size_t  Accumulator_Bits = entry<Index, Polynomial>::Accumulator_Bits;
        static ETL_CONSTANT size_t  Shift_Bits       = size_t(Accumulator_Bits - Chunk_Size);
        static ETL_CONSTANT uint8_t Entry            = uint8_t(Index << Shift_Bits);

        static ETL_CONSTANT uint8_t value = entry<Polynomial,
                                            entry<Polynomial,
                                            entry<Polynomial,
                                            entry<Polynomial, Entry>::value>::value>::value>::value;
      };

      //*************************************************************************
      ETL_CONSTEXPR uint8_t initial() const
      {
        return Initial;
      }

      //*************************************************************************
      uint8_t update_chunk(uint8_t crc, uint8_t value, const uint8_t* const table) const
      {
        // Extract the most significant nibble of the crc and xor with the value nibble.
        uint8_t index = (crc >> (Accumulator_Bits - Chunk_Size)) ^ value;

        crc <<= Chunk_Size;
        crc ^= table[index];

        return crc;
      }

      //*************************************************************************
      uint8_t add(uint8_t crc, uint8_t value) const
      {
        static ETL_CONSTANT uint8_t table[4] =
        {
          table_entry<0U, Polynomial>::value,
          table_entry<1U, Polynomial>::value,
          table_entry<2U, Polynomial>::value,
          table_entry<3U, Polynomial>::value
        };

        crc = update_chunk(crc, (value >> (Chunk_Size * 3U)) & Mask, table);
        crc = update_chunk(crc, (value >> (Chunk_Size * 2U)) & Mask, table);
        crc = update_chunk(crc, (value >> (Chunk_Size * 1U)) & Mask, table);
        crc = update_chunk(crc, value & Mask, table);
    
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
    class crc8_table4 : public etl::frame_check_sequence<etl::crc::crc8_policy_table4<Polynomial, Initial, Xor_Out, Reflect> >
    {
    public:

      //*************************************************************************
      /// Default constructor.
      //*************************************************************************
      crc8_table4()
      {
        this->reset();
      }

      //*************************************************************************
      /// Constructor from range.
      /// \param begin Start of the range.
      /// \param end   End of the range.
      //*************************************************************************
      template<typename TIterator>
      crc8_table4(TIterator begin, const TIterator end)
      {
        this->reset();
        this->add(begin, end);
      }
    };
  }
}
