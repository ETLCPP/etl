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

#ifndef ETL_VERSION_INCLUDED
#define ETL_VERSION_INCLUDED

#include "platform.h"
#include "macros.h"

///\defgroup version version
/// Definitions of the ETL version
///\ingroup utilities

#define ETL_VERSION_MAJOR 20
#define ETL_VERSION_MINOR 27
#define ETL_VERSION_PATCH 3


#define ETL_VERSION       ETL_STRINGIFY(ETL_VERSION_MAJOR) "." ETL_STRINGIFY(ETL_VERSION_MINOR) "." ETL_STRINGIFY(ETL_VERSION_PATCH)
#define ETL_VERSION_W     ETL_STRINGIFY(ETL_VERSION_MAJOR) L"." ETL_STRINGIFY(ETL_VERSION_MINOR) L"." ETL_STRINGIFY(ETL_VERSION_PATCH)
#define ETL_VERSION_U16   ETL_STRINGIFY(ETL_VERSION_MAJOR) u"." ETL_STRINGIFY(ETL_VERSION_MINOR) u"." ETL_STRINGIFY(ETL_VERSION_PATCH)
#define ETL_VERSION_U32   ETL_STRINGIFY(ETL_VERSION_MAJOR) U"." ETL_STRINGIFY(ETL_VERSION_MINOR) U"." ETL_STRINGIFY(ETL_VERSION_PATCH)
#define ETL_VERSION_VALUE ((ETL_VERSION_MAJOR * 10000) + (ETL_VERSION_MINOR * 100) + ETL_VERSION_PATCH)

namespace etl
{
  namespace traits
  {
    static ETL_CONSTANT long version                   = ETL_VERSION_VALUE;
    static ETL_CONSTANT long version_major             = ETL_VERSION_MAJOR;
    static ETL_CONSTANT long version_minor             = ETL_VERSION_MINOR;
    static ETL_CONSTANT long version_patch             = ETL_VERSION_PATCH;
#if ETL_USING_CPP11
    static constexpr const char*     version_string    = ETL_VERSION;
    static constexpr const wchar_t*  version_wstring   = ETL_VERSION_W;
    static constexpr const char16_t* version_u16string = ETL_VERSION_U16;
    static constexpr const char32_t* version_u32string = ETL_VERSION_U32;
#else
    static const char*     version_string              = ETL_VERSION;
    static const wchar_t*  version_wstring             = ETL_VERSION_W;
#endif
  }
}

#endif

