/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2025 BMW AG

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

#include "etl/ratio.h"

#if ETL_USING_CPP11
namespace
{
  SUITE(test_ratio)
  {
    //*************************************************************************
    TEST(test_ratio_add)
    {
      using r1 = etl::ratio<1, 2>;
      using r2 = etl::ratio<2, 3>;
      using r3 = etl::ratio_add<r1, r2>;

      CHECK((etl::ratio_equal<r3, etl::ratio<7, 6>>::value));
      CHECK((!etl::ratio_equal<r3, etl::ratio<1, 6>>::value));
    }

    //*************************************************************************
    TEST(test_ratio_subtract)
    {
      using r1 = etl::ratio<1, 2>;
      using r2 = etl::ratio<2, 3>;
      using r3 = etl::ratio_subtract<r1, r2>;

      CHECK((etl::ratio_equal<r3, etl::ratio<-1, 6>>::value));
      CHECK((!etl::ratio_equal<r3, etl::ratio<-2, 6>>::value));
    }

    //*************************************************************************
    TEST(test_ratio_multiply)
    {
      using r1 = etl::ratio<1, 2>;
      using r2 = etl::ratio<2, 3>;
      using r3 = etl::ratio_multiply<r1, r2>;

      CHECK((etl::ratio_equal<r3, etl::ratio<1, 3>>::value));
      CHECK((!etl::ratio_equal<r3, etl::ratio<1, 4>>::value));
    }

    //*************************************************************************
    TEST(test_ratio_divide)
    {
      using r1 = etl::ratio<1, 2>;
      using r2 = etl::ratio<2, 3>;
      using r3 = etl::ratio_divide<r1, r2>;

      CHECK((!etl::ratio_not_equal<r3, etl::ratio<3, 4>>::value));
      CHECK((etl::ratio_not_equal<r3, etl::ratio<4, 3>>::value));
    }

    //*************************************************************************
    TEST(test_ratio_equal)
    {
      using r1 = etl::ratio<1, 2>;
      using r2 = etl::ratio<2, 3>;

      CHECK((etl::ratio_equal<r1, r1>::value));
      CHECK((!etl::ratio_equal<r1, r2>::value));

#if ETL_USING_CPP14
      CHECK((etl::ratio_equal_v<r1, r1>));
      CHECK((!etl::ratio_equal_v<r1, r2>));
#endif
    }

    //*************************************************************************
    TEST(test_ratio_not_equal)
    {
      using r1 = etl::ratio<1, 2>;
      using r2 = etl::ratio<2, 3>;

      CHECK((!etl::ratio_not_equal<r1, r1>::value));
      CHECK((etl::ratio_not_equal<r1, r2>::value));

#if ETL_USING_CPP14
      CHECK((!etl::ratio_not_equal_v<r1, r1>));
      CHECK((etl::ratio_not_equal_v<r1, r2>));
#endif
    }

    //*************************************************************************
    TEST(test_ratio_less)
    {
      using r1 = etl::ratio<1, 2>;
      using r2 = etl::ratio<2, 3>;

      CHECK((etl::ratio_less<r1, r2>::value));
      CHECK((!etl::ratio_less<r2, r1>::value));

#if ETL_USING_CPP14
      CHECK((etl::ratio_less_v<r1, r2>));
      CHECK((!etl::ratio_less_v<r1, r1>));
#endif
    }

    //*************************************************************************
    TEST(test_ratio_less_equal)
    {
      using r1 = etl::ratio<1, 2>;
      using r2 = etl::ratio<2, 3>;

      CHECK((etl::ratio_less_equal<r1, r1>::value));
      CHECK((etl::ratio_less_equal<r1, r2>::value));

#if ETL_USING_CPP14
      CHECK((etl::ratio_less_equal_v<r1, r1>));
      CHECK((etl::ratio_less_equal_v<r1, r1>));
#endif
    }

    //*************************************************************************
    TEST(test_ratio_greater)
    {
      using r1 = etl::ratio<4, 3>;
      using r2 = etl::ratio<2, 3>;

      CHECK((etl::ratio_greater<r1, r2>::value));
      CHECK((!etl::ratio_greater<r2, r1>::value));

#if ETL_USING_CPP14
      CHECK((etl::ratio_greater_v<r1, r2>));
      CHECK((!etl::ratio_greater_v<r2, r1>));
#endif
    }

    //*************************************************************************
    TEST(test_ratio_greater_equal)
    {
      using r1 = etl::ratio<4, 3>;
      using r2 = etl::ratio<2, 3>;

      CHECK((etl::ratio_greater_equal<r1, r1>::value));
      CHECK((etl::ratio_greater_equal<r1, r2>::value));

#if ETL_USING_CPP14
      CHECK((etl::ratio_greater_equal_v<r1, r1>));
      CHECK((etl::ratio_greater_equal_v<r1, r2>));
#endif
    }
  };

}
#endif
