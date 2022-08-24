///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2018 John Wellbelove

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

#ifndef ETL_CPP17_NO_STL_INCLUDED
#define ETL_CPP17_NO_STL_INCLUDED

//*****************************************************************************
// Generic C++17 without STL support
//*****************************************************************************

#define ETL_TARGET_DEVICE_GENERIC
#define ETL_TARGET_OS_NONE
#define ETL_COMPILER_GENERIC
#define ETL_CPP11_SUPPORTED 1
#define ETL_CPP14_SUPPORTED 1
#define ETL_CPP17_SUPPORTED 1
#define ETL_NO_NULLPTR_SUPPORT 0
#define ETL_NO_LARGE_CHAR_SUPPORT 0
#define ETL_CPP11_TYPE_TRAITS_IS_TRIVIAL_SUPPORTED 0
#define ETL_NO_STL

#endif
