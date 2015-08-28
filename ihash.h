///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl

Copyright(c) 2015 jwellbelove

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

#ifndef __ETL_IHASH__
#define __ETL_IHASH__

#include <stdint.h>
#include <utility>

#include "exception.h"
#include "endian.h"

///\defgroup ihash Base class for all hash type classes.
///\ingroup hash

namespace etl
{
  //***************************************************************************
  ///\ingroup hash
  /// Exception base for hashes.
  //***************************************************************************
  class hash_exception : public exception
  {
  public:

    hash_exception(const char* what)
      : exception(what)
    {}
  };

  //***************************************************************************
  ///\ingroup vector
  /// Hash finalised exception.
  //***************************************************************************
  class hash_finalised : public hash_exception
  {
  public:

    hash_finalised()
      : hash_exception("ihash: finalised")
    {}
  };

  /// For the Americans
  typedef hash_finalised hash_finalized;

  //***************************************************************************
  /// Hash algorithm base class.
  /// \ingroup ihash
  //***************************************************************************
  class ihash
  {
  public:

    /// Generic return type.
    typedef std::pair<const uint8_t*, const uint8_t*> generic_digest;

    //*************************************************************************
    /// Default constructor.
    /// \param endianness The endianness to use for integral types larger than uint8_t.
    //*************************************************************************
    ihash(etl::endian endianness)
      : endianness(endianness)
    {
    }

    //*************************************************************************
    /// \param value The value to add to the hash.
    //*************************************************************************
    template<typename TValue>
    void add(TValue value)
    {
      uint8_t* p_data = reinterpret_cast<uint8_t*>(&value);

      if (endianness == endian::little)
      {
        for (int i = 0; i < sizeof(TValue); ++i)
        {
          add(p_data[i]);
        }
      }
      else
      {
        for (int i = sizeof(TValue) - 1; i >= 0; --i)
        {
          add(p_data[i]);
        }
      }
    }
    
    //*************************************************************************
    /// \param begin Start of the range.
    /// \param end   End of the range.
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
    /// \param value The value to add to the checksum.
    //*************************************************************************
    template<typename TValue>
    ihash& operator +=(TValue value)
    {
      add(value);
      return *this;
    }

    //*************************************************************************
    /// Get the endianness.
    //*************************************************************************
    etl::endian endian() const
    {
      return endianness;
    }

    //*************************************************************************
    /// Set the endianness.
    //*************************************************************************
    void endian(etl::endian e)
    {
      endianness = e;
    }

    //*************************************************************************
    /// \param value The uint8_t to add to the hash.
    //*************************************************************************
    virtual void add(uint8_t value) = 0;

    //*************************************************************************
    /// Resets the hash to the initial state.
    //*************************************************************************
    virtual void reset() = 0;

    //*************************************************************************
    /// Gets the result as a generic digest.
    //*************************************************************************
    virtual generic_digest digest() = 0;

  protected:

    //*************************************************************************
    /// Gets the result as a generic digest.
    /// Templated for derived class usage.
    //*************************************************************************
    template <typename T>
    generic_digest get_digest(const T& hash) const
    {
      const uint8_t* begin = reinterpret_cast<const uint8_t*>(&hash);

      return generic_digest(begin, begin + sizeof(hash));
    }

  private:

    etl::endian endianness;
  };
}

//*************************************************************************
/// Default streaming operator.
//*************************************************************************
template <typename T>
etl::ihash& operator << (etl::ihash& hash, T value)
{
  hash.add(value);
  return hash;
}

#endif
