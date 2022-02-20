/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2022 jwellbelove

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

#include "unit_test_framework.h"

#include "etl/platform.h"

namespace
{
  SUITE(test_library_traits)
  {
    TEST(test_library_traits)
    {
        CHECK_EQUAL(false, etl::library_traits::error_on_string_truncation);
        CHECK_EQUAL(true,  etl::library_traits::has_64bit_types);
        CHECK_EQUAL(true,  etl::library_traits::has_8bit_types);
        CHECK_EQUAL(true,  etl::library_traits::has_atomic);
        CHECK_EQUAL(true,  etl::library_traits::has_ideque_repair);
        CHECK_EQUAL(true,  etl::library_traits::has_istring_repair);
        CHECK_EQUAL(true,  etl::library_traits::has_ivector_repair);
        CHECK_EQUAL(true,  etl::library_traits::has_large_char);
        CHECK_EQUAL(true,  etl::library_traits::has_nullptr);
        CHECK_EQUAL(false, etl::library_traits::has_polymorphic_messages);
        CHECK_EQUAL(true,  etl::library_traits::has_string_clear_after_use);
        CHECK_EQUAL(true,  etl::library_traits::has_string_truncation_checks);
        CHECK_EQUAL(true,  etl::library_traits::is_debug_build);
    }
  };
}
