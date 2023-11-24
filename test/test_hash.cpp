/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 John Wellbelove

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

#include <type_traits>
#include <iterator>
#include <string>
#include <vector>
#include <stdint.h>

#include "etl/hash.h"

// for testing user-defined hash specializations
namespace { class CustomType{}; }
namespace etl
{
  template <>
  struct hash<CustomType>
  {
      size_t operator()(CustomType) {return 0;}
  };
}

namespace
{
  SUITE(test_hash)
  {
    //*************************************************************************
    TEST(test_hash_bool)
    {
      size_t hash = etl::hash<bool>()(false);
      CHECK_EQUAL(0U, hash);

      hash = etl::hash<bool>()(true);
      CHECK_EQUAL(1U, hash);
    }

    //*************************************************************************
    TEST(test_hash_char)
    {
      size_t hash = etl::hash<char>()(0x5A);

      CHECK_EQUAL(0x5AU, hash);
    }

    //*************************************************************************
    TEST(test_hash_signed_char)
    {
      size_t hash = etl::hash<signed char>()(0x5A);

      CHECK_EQUAL(0x5AU, hash);
    }

    //*************************************************************************
    TEST(test_hash_unsigned_char)
    {
      size_t hash = etl::hash<unsigned char>()(0x5AU);

      CHECK_EQUAL(0x5AU, hash);
    }

    //*************************************************************************
    TEST(test_hash_short)
    {
      size_t hash = etl::hash<short>()(0x5AA5);

      CHECK_EQUAL(0x5AA5U, hash);
    }

    //*************************************************************************
    TEST(test_hash_unsigned_short)
    {
      size_t hash = etl::hash<unsigned short>()(0x5AA5U);

      CHECK_EQUAL(0x5AA5U, hash);
    }

    //*************************************************************************
    TEST(test_hash_int)
    {
      size_t hash = etl::hash<int>()(0x5AA555AA);

      CHECK_EQUAL(0x5AA555AAUL, hash);
    }

    //*************************************************************************
    TEST(test_hash_unsigned_int)
    {
      size_t hash = etl::hash<unsigned int>()(0x5AA555AAU);

      CHECK_EQUAL(0x5AA555AAUL, hash);
    }

    //*************************************************************************
    TEST(test_hash_long)
    {
      size_t hash = etl::hash<long>()(0x5AA555AAL);

      CHECK_EQUAL(0x5AA555AAUL, hash);
    }

    //*************************************************************************
    TEST(test_hash_unsigned_long)
    {
      size_t hash = etl::hash<unsigned long>()(0x5AA555AAUL);

      CHECK_EQUAL(0x5AA555AAUL, hash);
    }

    //*************************************************************************
    TEST(test_hash_long_long)
    {
      size_t hash = etl::hash<long long>()(0x5AA555AA3CC333CCLL);

      if (ETL_PLATFORM_32BIT)
      {
        CHECK_EQUAL(0xEC6A8D69UL, hash);
      }

      if (ETL_PLATFORM_64BIT)
      {
        CHECK_EQUAL(0x5AA555AA3CC333CCULL, hash);
      }
    }

    //*************************************************************************
    TEST(test_hash_unsigned_long_long)
    {
      size_t hash = etl::hash<unsigned long long>()(0x5AA555AA3CC333CCULL);

      if (ETL_PLATFORM_32BIT)
      {
        CHECK_EQUAL(0xEC6A8D69UL, hash);
      }

      if (ETL_PLATFORM_64BIT)
      {
        CHECK_EQUAL(0x5AA555AA3CC333CCULL, hash);
      }
    }

    //*************************************************************************
    TEST(test_hash_float)
    {
      size_t hash = etl::hash<float>()((float)(1.2345));

      if (ETL_PLATFORM_32BIT)
      {
        CHECK_EQUAL(0X3F9E0419U, hash);
      }

      if (ETL_PLATFORM_64BIT)
      {
        CHECK_EQUAL(9821047038287739023U, hash);
      }
    }

    //*************************************************************************
    TEST(test_hash_double)
    {
      size_t hash = etl::hash<double>()(1.2345);

      if (ETL_PLATFORM_32BIT)
      {
        CHECK_EQUAL(0x86FBF224UL, hash);
      }

      if (ETL_PLATFORM_64BIT)
      {
        CHECK_EQUAL(0x3FF3C083126E978DULL, hash);
      }
    }

    TEST(test_hash_floating_point_negative_zero)
    {
      if (sizeof(float) == sizeof(size_t)) {
        etl::hash<float> hasher{};
        size_t hash1 = hasher(0.0);
        size_t hash2 = hasher(-0.0);
        CHECK_EQUAL(hash1, hash2);
      }

      if (sizeof(double) == sizeof(size_t)) {
        etl::hash<double> hasher{};
        size_t hash1 = hasher(0.0);
        size_t hash2 = hasher(-0.0);
        CHECK_EQUAL(hash1, hash2);
      }

      if (sizeof(long double) == sizeof(size_t)) {
        etl::hash<long double> hasher{};
        size_t hash1 = hasher(0.0);
        size_t hash2 = hasher(-0.0);
        CHECK_EQUAL(hash1, hash2);
      }
    }

    //*************************************************************************
    TEST(test_hash_pointer)
    {
      int i{};
      size_t hash = etl::hash<int*>()(&i);

      CHECK_EQUAL(size_t(&i), hash);
    }

    //*************************************************************************
    TEST(test_hash_const_pointer)
    {
      int i{};
      size_t hash = etl::hash<const int*>()(&i);

      CHECK_EQUAL(size_t(&i), hash);
    }

    //*************************************************************************
    TEST(test_hash_const_pointer_const)
    {
      int i{};
      const int * const pi = &i;

      size_t hash = etl::hash<const int *>()(pi);

      CHECK_EQUAL(size_t(&i), hash);
    }

    //*************************************************************************
    TEST(test_hash_enums)
    {
      enum class MyEnumClass : char 
      {
        OneE = 0x1E
      };
      
      enum MyEnum : char 
      {
        MyEnum_TwoF = 0x2F
      };

      size_t hash = etl::hash<MyEnumClass>()(MyEnumClass::OneE);
      CHECK_EQUAL(static_cast<size_t>(MyEnumClass::OneE), hash);

      hash = etl::hash<MyEnum>()(MyEnum_TwoF);
      CHECK_EQUAL(0x2F, hash);
    }

    //*************************************************************************
    TEST(test_hash_big_enums) 
    {
      constexpr unsigned long long big_number = 0x5AA555AA3CC333CCULL;
      enum class MyBigEnumClass : unsigned long long 
      {
        Big = big_number
      };
      
      size_t hash = etl::hash<MyBigEnumClass>()(MyBigEnumClass::Big);
      size_t expectedHash = etl::hash<unsigned long long>()(big_number);
      CHECK_EQUAL(expectedHash, hash);
    }

    //*************************************************************************
    TEST(test_hash_poisoned) 
    {
        // Unspecialized hash<> should be disabled (unusable) - see https://en.cppreference.com/w/cpp/utility/hash
        class A {};
        typedef etl::hash<A> general_hasher;

        CHECK_FALSE(std::is_default_constructible<general_hasher>::value);
        CHECK_FALSE(std::is_copy_constructible<general_hasher>::value);
        CHECK_FALSE(std::is_move_constructible<general_hasher>::value);
        CHECK_FALSE(std::is_copy_assignable<general_hasher>::value);
        CHECK_FALSE(std::is_move_assignable<general_hasher>::value);
    }

    //*************************************************************************
    TEST(test_hash_custom) 
    {
        typedef etl::hash<CustomType> custom_hasher;

        CHECK_TRUE(std::is_default_constructible<custom_hasher>::value);
        CHECK_TRUE(std::is_copy_constructible<custom_hasher>::value);
        CHECK_TRUE(std::is_move_constructible<custom_hasher>::value);
        CHECK_TRUE(std::is_copy_assignable<custom_hasher>::value);
        CHECK_TRUE(std::is_move_assignable<custom_hasher>::value);
    }
  };
}

