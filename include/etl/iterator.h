///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2017 jwellbelove

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

#ifndef ETL_ITERATOR_INCLUDED
#define ETL_ITERATOR_INCLUDED

#include "platform.h"

#include "stl/iterator.h"

#include "type_traits.h"

///\defgroup iterator iterator
///\ingroup utilities

namespace etl
{
  //***************************************************************************
  /// iterator
  //***************************************************************************
  template <typename TCategory, typename T, typename TDistance = ptrdiff_t, typename TPointer = T* , typename TReference = T& >
  struct iterator
  {
    typedef T          value_type;
    typedef TDistance  difference_type;
    typedef TPointer   pointer;
    typedef TReference reference;
    typedef TCategory  iterator_category;
  };

  //***************************************************************************
  // Helper templates.
  //***************************************************************************
  template <typename T>
  struct is_input_iterator
  {
    static const bool value = etl::is_same<typename ETL_STD::iterator_traits<T>::iterator_category, ETL_INPUT_ITERATOR_TAG>::value;
  };

  template <typename T>
  struct is_output_iterator
  {
    static const bool value = etl::is_same<typename ETL_STD::iterator_traits<T>::iterator_category, ETL_OUTPUT_ITERATOR_TAG>::value;
  };

  template <typename T>
  struct is_forward_iterator
  {
    static const bool value = etl::is_same<typename ETL_STD::iterator_traits<T>::iterator_category, ETL_FORWARD_ITERATOR_TAG>::value;
  };

  template <typename T>
  struct is_bidirectional_iterator
  {
    static const bool value = etl::is_same<typename ETL_STD::iterator_traits<T>::iterator_category, ETL_BIDIRECTIONAL_ITERATOR_TAG>::value;
  };

  template <typename T>
  struct is_random_iterator
  {
    static const bool value = etl::is_same<typename ETL_STD::iterator_traits<T>::iterator_category, ETL_RANDOM_ACCESS_ITERATOR_TAG>::value;
  };

  template <typename T>
  struct is_input_iterator_concept
  {
    static const bool value = etl::is_input_iterator<T>::value ||
                              etl::is_forward_iterator<T>::value ||
                              etl::is_bidirectional_iterator<T>::value ||
                              etl::is_random_iterator<T>::value;
  };

  template <typename T>
  struct is_output_iterator_concept
  {
    static const bool value = etl::is_output_iterator<T>::value ||
                              etl::is_forward_iterator<T>::value ||
                              etl::is_bidirectional_iterator<T>::value ||
                              etl::is_random_iterator<T>::value;
  };

  template <typename T>
  struct is_forward_iterator_concept
  {
    static const bool value = etl::is_forward_iterator<T>::value ||
                              etl::is_bidirectional_iterator<T>::value ||
                              etl::is_random_iterator<T>::value;
  };

  template <typename T>
  struct is_bidirectional_iterator_concept
  {
    static const bool value = etl::is_bidirectional_iterator<T>::value ||
                              etl::is_random_iterator<T>::value;
  };

  template <typename T>
  struct is_random_iterator_concept
  {
    static const bool value = etl::is_random_iterator<T>::value;
  };

}

#endif

