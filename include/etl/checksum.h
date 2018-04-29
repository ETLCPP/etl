
///\file

/******************************************************************************
The MIT License(MIT)
Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com
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

#ifndef ETL_CHECKSUM_INCLUDED
#define ETL_CHECKSUM_INCLUDED

#include <stdint.h>

#include "platform.h"
#include "binary.h"
#include "frame_check_sequence.h"

///\defgroup checksum Checksum calculation
///\ingroup maths

namespace etl
{
  //***************************************************************************
  /// Standard addition checksum policy.
  //***************************************************************************
  template <typename T>
  struct checksum_policy_sum
  {
    typedef T value_type;

    inline T initial() const
    {
      return 0;
    }

    inline T add(T sum, uint8_t value) const
    {
      return sum + value;
    }

    inline T final(T sum) const
    {
      return sum;
    }
  };

  //***************************************************************************
  /// BSD checksum policy.
  //***************************************************************************
  template <typename T>
  struct checksum_policy_bsd
  {
    typedef T value_type;

    inline T initial() const
    {
      return 0;
    }

    inline T add(T sum, uint8_t value) const
    {
      return etl::rotate_right(sum) + value;
    }

    inline T final(T sum) const
    {
      return sum;
    }
  };

  //***************************************************************************
  /// Standard XOR checksum policy.
  //***************************************************************************
  template <typename T>
  struct checksum_policy_xor
  {
    typedef T value_type;

    inline T initial() const
    {
      return 0;
    }

    inline T add(T sum, uint8_t value) const
    {
      return sum ^ value;
    }

    inline T final(T sum) const
    {
      return sum;
    }
  };

  //***************************************************************************
  /// XOR-rotate checksum policy.
  //***************************************************************************
  template <typename T>
  struct checksum_policy_xor_rotate
  {
    typedef T value_type;

    inline T initial() const
    {
      return 0;
    }

    inline T add(T sum, uint8_t value) const
    {
      return etl::rotate_left(sum) ^ value;
    }

    inline T final(T sum) const
    {
      return sum;
    }
  };

  //*************************************************************************
  /// Standard Checksum.
  //*************************************************************************
  template <typename T>
  class checksum : public etl::frame_check_sequence<etl::checksum_policy_sum<T> >
  {
  public:

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    checksum()
    {
      this->reset();
    }

    //*************************************************************************
    /// Constructor from range.
    /// \param begin Start of the range.
    /// \param end   End of the range.
    //*************************************************************************
    template<typename TIterator>
    checksum(TIterator begin, const TIterator end)
    {
      this->reset();
      this->add(begin, end);
    }
  };

  //*************************************************************************
  /// BSD Checksum.
  //*************************************************************************
  template <typename T>
  class bsd_checksum : public etl::frame_check_sequence<etl::checksum_policy_bsd<T> >
  {
  public:

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    bsd_checksum()
    {
      this->reset();
    }

    //*************************************************************************
    /// Constructor from range.
    /// \param begin Start of the range.
    /// \param end   End of the range.
    //*************************************************************************
    template<typename TIterator>
    bsd_checksum(TIterator begin, const TIterator end)
    {
      this->reset();
      this->add(begin, end);
    }
  };

  //*************************************************************************
  /// XOR Checksum.
  //*************************************************************************
  template <typename T>
  class xor_checksum : public etl::frame_check_sequence<etl::checksum_policy_xor<T> >
  {
  public:

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    xor_checksum()
    {
      this->reset();
    }

    //*************************************************************************
    /// Constructor from range.
    /// \param begin Start of the range.
    /// \param end   End of the range.
    //*************************************************************************
    template<typename TIterator>
    xor_checksum(TIterator begin, const TIterator end)
    {
      this->reset();
      this->add(begin, end);
    }
  };

  //*************************************************************************
  /// XOR-shift Checksum.
  //*************************************************************************
  template <typename T>
  class xor_rotate_checksum : public etl::frame_check_sequence<etl::checksum_policy_xor_rotate<T> >
  {
  public:

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    xor_rotate_checksum()
    {
      this->reset();
    }

    //*************************************************************************
    /// Constructor from range.
    /// \param begin Start of the range.
    /// \param end   End of the range.
    //*************************************************************************
    template<typename TIterator>
    xor_rotate_checksum(TIterator begin, const TIterator end)
    {
      this->reset();
      this->add(begin, end);
    }
  };
}

#endif
