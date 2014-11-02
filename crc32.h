///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.

Copyright(c) 2014 jwellbelove

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

#ifndef __etl_crc32__
#define __etl_crc32__

#include <cstdint>

///\defgroup crc32 Thirty two bit CRC calculation
///\ingroup crc

namespace etl
{
  //***************************************************************************
  /// CRC32 table
  /// \ingroup crc32
  //***************************************************************************
  extern const uint32_t CRC32[];

  //***************************************************************************
  /// Calculates CRC32 using polynomial 0x04C11DB7.
  /// \ingroup crc32
  //***************************************************************************
  class crc32
  {
  public:

    typedef uint32_t value_type;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    crc32()
    {
      reset();
    }

    //*************************************************************************
    /// Constructor from range.
    /// \param begin Start of the range.
    /// \param end   End of the range.
    /// \return The CRC result.
    //*************************************************************************
    template<typename TIterator>
    crc32(TIterator begin, const TIterator end)
    {
      reset();
      add(begin, end);
    }

    //*************************************************************************
    /// Resets the CRC to the initial state.
    //*************************************************************************
    void reset()
    {
      crc = 0xFFFFFFFF;
    }

    //*************************************************************************
    /// \param value The value to add to the CRC.
    /// \return The CRC result.
    //*************************************************************************
    template<typename TValue>
    void add(TValue value)
    {
      // How many steps in a value?
      size_t steps = sizeof(TValue) / sizeof(uint8_t);

      uint8_t* p_data = reinterpret_cast<uint8_t*>(&value);

      for (size_t i = 0; i < steps; ++i)
      {
        crc = (crc >> 8) ^ CRC32[(crc ^ p_data[i]) & 0xFF];        
      }
    }

    //*************************************************************************
    /// \param begin Start of the range.
    /// \param end   End of the range.
    /// \return The CRC result.
    //*************************************************************************
    template<typename TIterator>
    void add(TIterator begin, const TIterator end)
    {
      while (begin != end)
      {
        add(*begin);
        ++begin;
      }
    }

    //*************************************************************************
    /// Gets the CRC value.
    //*************************************************************************
    value_type value() const
    {
      return crc ^ 0xFFFFFFFF;
    }

    //*************************************************************************
    /// Conversion operator to value_type.
    //*************************************************************************
    operator value_type () const
    {
      return crc ^ 0xFFFFFFFF;
    }

  private:

    value_type crc;
  };
}

#endif