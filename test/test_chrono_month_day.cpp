/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Documentation: 

Copyright(c) 2024 John Wellbelove

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

#include "etl/platform.h"

#include "unit_test_framework.h"

#include "etl/chrono.h"

#include <vector>
#include <algorithm>

// Set to 0 to reference against std::chrono
#define ETL_USING_ETL_CHRONO 1

#if ETL_USING_ETL_CHRONO
  #define Chrono etl::chrono
#else
  #if ETL_USING_CPP20
    #include <chrono>
    #define Chrono std::chrono
  #else
    #error std::chrono not supported
  #endif
#endif

namespace
{
  SUITE(test_chrono_month_day)
  {
    //*************************************************************************
    TEST(test_default_constructor)
    {
      Chrono::month_day md;

      CHECK_FALSE(md.ok()); // Default-constructed month_day is not valid
    }

    //*************************************************************************
    TEST(test_constructor_with_month_and_day)
    {
      Chrono::month_day md{Chrono::January, Chrono::day{15}};

      CHECK_TRUE(md.ok()); // Valid month_day
      CHECK_EQUAL(Chrono::January, md.month());
      CHECK_EQUAL(Chrono::day{15}, md.day());
    }

    //*************************************************************************
    TEST(test_invalid_month_day)
    {
      Chrono::month_day md{Chrono::month{13}, Chrono::day{15}}; // Invalid month (13)

      CHECK_FALSE(md.ok()); // Invalid month_day
    }

    //*************************************************************************
    TEST(test_invalid_day_in_month_day)
    {
      Chrono::month_day md{Chrono::January, Chrono::day{32}}; // Invalid day (32)

      CHECK_FALSE(md.ok()); // Invalid month_day
    }

#if ETL_USING_CPP20
    //*************************************************************************
    TEST(test_month_day_spaceship_operator)
    {
      Chrono::month_day md1{Chrono::January,  Chrono::day{15}};
      Chrono::month_day md2{Chrono::February, Chrono::day{10}};
      Chrono::month_day md3{Chrono::January,  Chrono::day{15}};

      CHECK_TRUE((md1 <=> md3) == std::strong_ordering::equal);   // Same month and day
      CHECK_TRUE((md1 <=> md2) == std::strong_ordering::less);    // Different month and day
      CHECK_TRUE((md2 <=> md1) == std::strong_ordering::greater); // Same month and day
    }
#endif

#if ETL_USING_ETL_CHRONO
    //*************************************************************************
    TEST(test_month_day_compare)
    {
      Chrono::month_day md1{Chrono::January,  Chrono::day{15}};
      Chrono::month_day md2{Chrono::February, Chrono::day{10}};
      Chrono::month_day md3{Chrono::January,  Chrono::day{15}};

      CHECK_TRUE(md1.compare(md3) == 0);  // Same month and day
      CHECK_TRUE(md1.compare(md2) == -1); // Different month and day
      CHECK_TRUE(md2.compare(md1) == 1);  // Same month and day
    }
#endif

    //*************************************************************************
    TEST(test_month_day_equality_operator)
    {
      Chrono::month_day md1{Chrono::January,  Chrono::day{15}};
      Chrono::month_day md2{Chrono::February, Chrono::day{10}};
      Chrono::month_day md3{Chrono::January,  Chrono::day{20}};

      CHECK_TRUE(md1 == md1);  // January == January
      CHECK_FALSE(md1 == md2); // January != February
      CHECK_FALSE(md1 == md3); // 20th != 15th in the same month
    }

    //*************************************************************************
    TEST(test_month_day_not_equality_operator)
    {
      Chrono::month_day md1{Chrono::January,  Chrono::day{15}};
      Chrono::month_day md2{Chrono::February, Chrono::day{10}};
      Chrono::month_day md3{Chrono::January,  Chrono::day{20}};

      CHECK_FALSE(md1 != md1);  // January == January
      CHECK_TRUE(md1 != md2); // January != February
      CHECK_TRUE(md1 != md3); // 20th != 15th in the same month
    }

    //*************************************************************************
    TEST(test_month_day_less_than_operator)
    {
      Chrono::month_day md1{Chrono::January,  Chrono::day{15}};
      Chrono::month_day md2{Chrono::February, Chrono::day{10}};
      Chrono::month_day md3{Chrono::January,  Chrono::day{20}};

      CHECK_TRUE(md1 < md2);  // January < February
      CHECK_TRUE(md1 < md3);  // 15th < 20th in the same month
      CHECK_FALSE(md2 < md1); // February !< January
      CHECK_FALSE(md3 < md1); // 20th !< 15th in the same month
    }

    //*************************************************************************
    TEST(test_month_day_less_than_equal_operator)
    {
      Chrono::month_day md1{Chrono::January,  Chrono::day{15}};
      Chrono::month_day md2{Chrono::February, Chrono::day{10}};
      Chrono::month_day md3{Chrono::January,  Chrono::day{20}};

      CHECK_TRUE(md1 <= md1);  // January <= January
      CHECK_TRUE(md1 <= md2);  // January <= February
      CHECK_TRUE(md1 <= md3);  // 15th <= 20th in the same month
      CHECK_FALSE(md2 <= md1); // February !<= January
      CHECK_FALSE(md3 <= md1); // 20th !<= 15th in the same month
    }

    //*************************************************************************
    TEST(test_month_day_greater_than_operator)
    {
      Chrono::month_day md1{Chrono::January,  Chrono::day{15}};
      Chrono::month_day md2{Chrono::February, Chrono::day{10}};
      Chrono::month_day md3{Chrono::January,  Chrono::day{20}};

      CHECK_TRUE(md2 > md1);  // February > January
      CHECK_TRUE(md3 > md1);  // 20th > 15th in the same month
      CHECK_FALSE(md1 > md2); // January !> February
      CHECK_FALSE(md1 > md3); // 15th !> 20th in the same month
    }

    //*************************************************************************
    TEST(test_month_day_greater_than_equal_operator)
    {
      Chrono::month_day md1{Chrono::January,  Chrono::day{15}};
      Chrono::month_day md2{Chrono::February, Chrono::day{10}};
      Chrono::month_day md3{Chrono::January,  Chrono::day{20}};

      CHECK_TRUE(md1 >= md1);  // January >= January
      CHECK_TRUE(md2 >= md1);  // February >= January
      CHECK_TRUE(md3 >= md1);  // 20th >= 15th in the same month
      CHECK_FALSE(md1 >= md2); // January !>= February
      CHECK_FALSE(md1 >= md3); // 15th !>= 20th in the same month
    }

    //*************************************************************************
    TEST(test_month_day_min_max)
    {
      Chrono::month_day md_min{Chrono::January, Chrono::day{1}};
      Chrono::month_day md_max{Chrono::December, Chrono::day{31}};

      CHECK_TRUE(md_min.ok());
      CHECK_TRUE(md_max.ok());

      CHECK_EQUAL(Chrono::January, md_min.month());
      CHECK_EQUAL(Chrono::day{1}, md_min.day());

      CHECK_EQUAL(Chrono::December, md_max.month());
      CHECK_EQUAL(Chrono::day{31}, md_max.day());
    }

  };
}
