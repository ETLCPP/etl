///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 jwellbelove

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

#ifndef ETL_CRC32_INCLUDED
#define ETL_CRC32_INCLUDED

#include "platform.h"
#include "private/crc_implementation.h"

///\defgroup crc32 32 bit CRC calculation
///\ingroup crc

namespace etl
{
#if ETL_CPP11_SUPPORTED
  template <size_t Table_Size>
  using crc32_t = etl::crc_type<etl::private_crc::crc32_parameters, Table_Size>;
#else
  template <size_t Table_Size>
  class crc32_t : public etl::crc_type<etl::private_crc::crc32_parameters, Table_Size>
  {
  public:

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    crc32_t()
    {
      this->reset();
    }

    //*************************************************************************
    /// Constructor from range.
    /// \param begin Start of the range.
    /// \param end   End of the range.
    //*************************************************************************
    template<typename TIterator>
    crc32_t(TIterator begin, const TIterator end)
    {
      this->reset();
      this->add(begin, end);
    }
  };
#endif

  typedef etl::crc32_t<256U> crc32_t256;
  typedef etl::crc32_t<16U>  crc32_t16;
  typedef etl::crc32_t<4U>   crc32_t4;
  typedef crc32_t256         crc32;
}
#endif
