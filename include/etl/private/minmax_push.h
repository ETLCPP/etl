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

// The header include guard has been intentionally omitted.
// This file is intended to evaluated multiple times by design.
#ifndef ETL_PUSHED_MIN_MAX
  #if !defined(ETL_COMPILER_GREEN_HILLS) && !defined(ETL_COMPILER_IAR) && !defined(ETL_COMPILER_TASKING)
    #if !defined(ETL_COMPILER_ARM5)
      #pragma push_macro("min")
      #pragma push_macro("max")
    #endif

    #ifdef min
      #ifndef ETL_RESTORE_MIN
        #define ETL_RESTORE_MIN
      #endif
      #undef min
    #endif

    #ifdef max
      #ifndef ETL_RESTORE_MAX
        #define ETL_RESTORE_MAX
      #endif
      #undef max
    #endif
  #endif

  #define ETL_PUSHED_MIN_MAX
  
#else
  #error minmax_push without matching pop
#endif
