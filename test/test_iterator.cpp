/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

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

#include <UnitTest++/UnitTest++.h>

#include "../src/iterator.h"

#include <iterator>

struct input : public std::iterator<std::input_iterator_tag, int>
{

};

struct output : public std::iterator<std::output_iterator_tag, int>
{

};

struct forward : public std::iterator<std::forward_iterator_tag, int>
{

};

struct bidirectional : public std::iterator<std::bidirectional_iterator_tag, int>
{

};

struct random : public std::iterator<std::random_access_iterator_tag, int>
{

};

typedef int* pointer;
typedef const int* const_pointer;

namespace
{
  SUITE(test_iterator)
  {
    TEST(test_input)
    {
      CHECK(etl::is_input_iterator<input>::value);
      CHECK(!etl::is_output_iterator<input>::value);
      CHECK(!etl::is_forward_iterator<input>::value);
      CHECK(!etl::is_bidirectional_iterator<input>::value);
      CHECK(!etl::is_random_iterator<input>::value);

      CHECK(etl::is_input_iterator_concept<input>::value);
      CHECK(!etl::is_output_iterator_concept<input>::value);
      CHECK(!etl::is_forward_iterator_concept<input>::value);
      CHECK(!etl::is_bidirectional_iterator_concept<input>::value);
      CHECK(!etl::is_random_iterator_concept<input>::value);
    }

    TEST(test_output)
    {
      CHECK(!etl::is_input_iterator<output>::value);
      CHECK(etl::is_output_iterator<output>::value);
      CHECK(!etl::is_forward_iterator<output>::value);
      CHECK(!etl::is_bidirectional_iterator<output>::value);
      CHECK(!etl::is_random_iterator<output>::value);

      CHECK(!etl::is_input_iterator_concept<output>::value);
      CHECK(etl::is_output_iterator_concept<output>::value);
      CHECK(!etl::is_forward_iterator_concept<output>::value);
      CHECK(!etl::is_bidirectional_iterator_concept<output>::value);
      CHECK(!etl::is_random_iterator_concept<output>::value);
    }

    TEST(test_forward)
    {
      CHECK(!etl::is_input_iterator<forward>::value);
      CHECK(!etl::is_output_iterator<forward>::value);
      CHECK(etl::is_forward_iterator<forward>::value);
      CHECK(!etl::is_bidirectional_iterator<forward>::value);
      CHECK(!etl::is_random_iterator<forward>::value);

      CHECK(etl::is_input_iterator_concept<forward>::value);
      CHECK(etl::is_output_iterator_concept<forward>::value);
      CHECK(etl::is_forward_iterator_concept<forward>::value);
      CHECK(!etl::is_bidirectional_iterator_concept<forward>::value);
      CHECK(!etl::is_random_iterator_concept<forward>::value);
    }

    TEST(test_bidirectional)
    {
      CHECK(!etl::is_input_iterator<bidirectional>::value);
      CHECK(!etl::is_output_iterator<bidirectional>::value);
      CHECK(!etl::is_forward_iterator<bidirectional>::value);
      CHECK(etl::is_bidirectional_iterator<bidirectional>::value);
      CHECK(!etl::is_random_iterator<bidirectional>::value);

      CHECK(etl::is_input_iterator_concept<bidirectional>::value);
      CHECK(etl::is_output_iterator_concept<bidirectional>::value);
      CHECK(etl::is_forward_iterator_concept<bidirectional>::value);
      CHECK(etl::is_bidirectional_iterator_concept<bidirectional>::value);
      CHECK(!etl::is_random_iterator_concept<bidirectional>::value);
    }

    TEST(test_random)
    {
      CHECK(!etl::is_input_iterator<random>::value);
      CHECK(!etl::is_output_iterator<random>::value);
      CHECK(!etl::is_forward_iterator<random>::value);
      CHECK(!etl::is_bidirectional_iterator<random>::value);
      CHECK(etl::is_random_iterator<random>::value);

      CHECK(etl::is_input_iterator_concept<random>::value);
      CHECK(etl::is_output_iterator_concept<random>::value);
      CHECK(etl::is_forward_iterator_concept<random>::value);
      CHECK(etl::is_bidirectional_iterator_concept<random>::value);
      CHECK(etl::is_random_iterator_concept<random>::value);
    }

    TEST(test_pointer)
    {
      CHECK(!etl::is_input_iterator<pointer>::value);
      CHECK(!etl::is_output_iterator<pointer>::value);
      CHECK(!etl::is_forward_iterator<pointer>::value);
      CHECK(!etl::is_bidirectional_iterator<pointer>::value);
      CHECK(etl::is_random_iterator<pointer>::value);

      CHECK(etl::is_input_iterator_concept<pointer>::value);
      CHECK(etl::is_output_iterator_concept<pointer>::value);
      CHECK(etl::is_forward_iterator_concept<pointer>::value);
      CHECK(etl::is_bidirectional_iterator_concept<pointer>::value);
      CHECK(etl::is_random_iterator_concept<pointer>::value);
    }

    TEST(test_const_pointer)
    {
      CHECK(!etl::is_input_iterator<const_pointer>::value);
      CHECK(!etl::is_output_iterator<const_pointer>::value);
      CHECK(!etl::is_forward_iterator<const_pointer>::value);
      CHECK(!etl::is_bidirectional_iterator<const_pointer>::value);
      CHECK(etl::is_random_iterator<const_pointer>::value);

      CHECK(etl::is_input_iterator_concept<const_pointer>::value);
      CHECK(etl::is_output_iterator_concept<const_pointer>::value);
      CHECK(etl::is_forward_iterator_concept<const_pointer>::value);
      CHECK(etl::is_bidirectional_iterator_concept<const_pointer>::value);
      CHECK(etl::is_random_iterator_concept<const_pointer>::value);
    }
  };
}
