/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl

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

#ifndef __ETL_STATIC_ASSERT__
#define __ETL_STATIC_ASSERT__

#if defined(COMPILER_MICROSOFT)
  #define STATIC_ASSERT(Condition, Message) static_assert(Condition, Message)
#elif defined(COMPILER_GCC)
  #define STATIC_ASSERT(Condition, Message) static_assert(Condition, Message)
#else
	template <bool Condition>
  struct STATIC_ASSERTION_FAILURE;

  template <>
  struct STATIC_ASSERTION_FAILURE<true> {};

  #define STATIC_ASSERT(Condition, Message) enum { assertdummy = sizeof(STATIC_ASSERTION_FAILURE<(bool)(Condition)>) }
#endif

#endif
