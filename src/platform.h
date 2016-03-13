///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2016 jwellbelove

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

#if defined(__linux__)
#define ETL_PLATFORM_LINUX
#endif

#if defined(__WIN32) || defined(__WIN64)
#define ETL_PLATFORM_WINDOWS
#endif

#if defined(__IAR_SYSTEMS_ICC__)
#define ETL_COMPILER_IAR
#endif

#if defined(__KEIL__) && !defined(__GNUC__)
#define ETL_COMPILER_KEIL
#endif

#if defined(__ghs__)
#define ETL_COMPILER_GREEN_HILLS
#endif

#if defined(__INTEL_COMPILER)
#define ETL_COMPILER_INTEL
#endif

#if defined(_MSC_VER)
#define ETL_COMPILER_MICROSOFT
#endif

#if defined(__GNUC__)
#define ETL_COMPILER_GCC
#endif

#if (defined(ETL_COMPILER_MICROSOFT) && (_MSC_VER < 1600)) || \
     defined(ETL_COMPILER_KEIL) || \
     defined(ETL_COMPILER_IAR) || \
     (defined(ETL_COMPILER_GCC) && (__cplusplus < 201103L))
  #define NO_NULLPTR_SUPPORT
#endif

