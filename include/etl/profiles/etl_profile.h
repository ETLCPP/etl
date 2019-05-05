///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2017 jwellbelove, scott-eddy

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
#ifndef __ETL_PROFILE_H__
#define __ETL_PROFILE_H__
#if defined(PROFILE_MSVC)
  #include "msvc_x86.h"
#elif defined(PROFILE_GCC_GENERIC)
  #include "gcc_generic.h"
#elif defined(PROFILE_GCC_LINUX_X86)
  #include "gcc_linux_x86.h"
#elif defined(PROFILE_GCC_WINDOWS_X86)
  #include "gcc_windows_x86.h"
#elif defined(PROFILE_ARM_V5_GENERIC)
  #include "armv5.h"
#elif defined(PROFILE_ARM_V6_GENERIC)
  #include "armv6.h"
#elif defined(PROFILE_ARDUINO)
  #include "arduino_arm.h"
#elif defined(PROFILE_TICC)
  #include "ticc.h"
#elif defined(PROFILE_CPP03_GENERIC)
  #include "cpp03.h"
#elif defined(PROFILE_CPP11_GENERIC)
  #include "cpp11.h"
#elif defined(PROFILE_CPP14_GENERIC)
  #include "cpp14.h"
#elif defined(PROFILE_CPP17_GENERIC)
  #include "cpp17.h"
#elif defined(PROFILE_CUSTOM)
  #include "custom_profile.h"
#else
  #error Must provide a profile header file when buiding ETL. See https://www.etlcpp.com/setup.html
#endif

#endif // __ETL_PROFILE_H_
