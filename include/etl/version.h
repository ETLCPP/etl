///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2018 jwellbelove

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

#include "macros.h"

///\defgroup version version
/// Definitions of the ETL version
///\ingroup utilities

#define ETL_VERSION_MAJOR 14
#define ETL_VERSION_MINOR 26
#define ETL_VERSION_PATCH  7

#define ETL_VERSION       ETL_STRINGIFY(ETL_VERSION_MAJOR) ETL_STRINGIFY(ETL_VERSION_MINOR) ETL_STRINGIFY(ETL_VERSION_PATCH)
#define ETL_VERSION_W     ETL_WIDE_STRING(ETL_CONCAT(ETL_CONCAT(ETL_VERSION_MAJOR, ETL_VERSION_MINOR), ETL_VERSION_PATCH))
#define ETL_VERSION_U16   ETL_U16_STRING(ETL_CONCAT(ETL_CONCAT(ETL_VERSION_MAJOR, ETL_VERSION_MINOR), ETL_VERSION_PATCH))
#define ETL_VERSION_U32   ETL_U32_STRING(ETL_CONCAT(ETL_CONCAT(ETL_VERSION_MAJOR, ETL_VERSION_MINOR), ETL_VERSION_PATCH))
#define ETL_VERSION_VALUE ((ETL_VERSION_MAJOR * 10000) + (ETL_VERSION_MINOR * 100) + ETL_VERSION_PATCH)

#endif

