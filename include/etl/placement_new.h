///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2019 John Wellbelove

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

#ifndef ETL_PLACEMENT_NEW_INCLUDED
#define ETL_PLACEMENT_NEW_INCLUDED

#include "platform.h"

//*************************************
// Figure out if we can use the standard library <new> header, if haven't already done so in etl_profile.h
#if !defined(ETL_USING_STD_NEW)
  #if defined(__has_include)
    #if __has_include(<new>)
      #define ETL_USING_STD_NEW 1
    #else
      #define ETL_USING_STD_NEW 0
    #endif
  #elif (defined(ARDUINO) && defined(__AVR__))
    #define ETL_USING_STD_NEW 0
  #else
    #define ETL_USING_STD_NEW 1
  #endif
#endif

#if ETL_USING_STD_NEW
  #include <new>
#else
  // Define placement new if no new header is available
  inline void* operator new(size_t, void* p) { return p; }
  inline void* operator new[](size_t, void* p) { return p; }

  inline void operator delete(void*, void*) ETL_NOEXCEPT {}
  inline void operator delete[](void*, void*) ETL_NOEXCEPT{}

#endif

#endif
