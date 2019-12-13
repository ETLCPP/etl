///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2019 jwellbelove

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

#ifndef ETL_STL_CHOOSE_TAG_TYPES_INCLUDED
#define ETL_STL_CHOOSE_TAG_TYPES_INCLUDED

#include "../platform.h"

// When in the unit tests we have to ensure that the STL and ETL are using the same definitions.
#if defined(ETL_IN_UNIT_TEST) || !defined(ETL_NO_STL)
  #include <iterator>
  #define ETL_INPUT_ITERATOR_TAG         std::input_iterator_tag
  #define ETL_OUTPUT_ITERATOR_TAG        std::output_iterator_tag
  #define ETL_FORWARD_ITERATOR_TAG       std::forward_iterator_tag
  #define ETL_BIDIRECTIONAL_ITERATOR_TAG std::bidirectional_iterator_tag
  #define ETL_RANDOM_ACCESS_ITERATOR_TAG std::random_access_iterator_tag
#else
  #define ETL_INPUT_ITERATOR_TAG         etlstd::input_iterator_tag
  #define ETL_OUTPUT_ITERATOR_TAG        etlstd::output_iterator_tag
  #define ETL_FORWARD_ITERATOR_TAG       etlstd::forward_iterator_tag
  #define ETL_BIDIRECTIONAL_ITERATOR_TAG etlstd::bidirectional_iterator_tag
  #define ETL_RANDOM_ACCESS_ITERATOR_TAG etlstd::random_access_iterator_tag
#endif

#endif
