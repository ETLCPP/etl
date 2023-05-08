/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2017 John Wellbelove

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

#include "etl/array_wrapper.h"

#if defined(ETL_COMPILER_CLANG)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Warray-bounds"
#endif

namespace
{
  int data5[]  = { 0, 1, 2, 3, 4 };
  int data5b[] = { 100, 100, 100, 100, 100 };
  int data5c[] = { 0, 1, 2, 3, 4 };
  int data5d[] = { 100, 100, 100, 100, 100 };
  extern const int data5same[] = { 0, 1, 2, 3, 4 };
  extern const int data5greater[] = { 0, 1, 5, 3, 4 };
  extern const int data5smaller[] = { 0, 1, 1, 3, 4 };
  extern const int data4[] = { 0, 1, 2, 3 };
  extern const int data6[] = { 0, 1, 2, 3, 4, 5 };
  extern const int cdata5[] = { 5, 6, 7, 8, 9 };

  typedef ETL_ARRAY_WRAPPER(int, data5)              Data5;
  typedef ETL_ARRAY_WRAPPER(int, data5b)             Data5b;
  typedef ETL_ARRAY_WRAPPER(int, data5c)             Data5c;
  typedef ETL_ARRAY_WRAPPER(int, data5d)             Data5d;
  typedef ETL_ARRAY_WRAPPER(const int, data5same)    Data5Same;
  typedef ETL_ARRAY_WRAPPER(const int, data5greater) Data5Greater;
  typedef ETL_ARRAY_WRAPPER(const int, data5smaller) Data5Smaller;
  typedef ETL_ARRAY_WRAPPER(const int, cdata5)       CData5;
  typedef ETL_ARRAY_WRAPPER(const int, data4)        Data4;
  typedef ETL_ARRAY_WRAPPER(const int, data6)        Data6;

  int temp;

  void Save(int i)
  {
    temp = i;
  }

  void Restore(int& i)
  {
    i = temp;
  }

  SUITE(test_array_wrapper)
  {
    //*************************************************************************
    TEST(test_constructor)
    {
      Data5 aw5;
      CHECK_ARRAY_EQUAL(data5, aw5.data(), aw5.size());

      const Data5 caw5a;
      CHECK_ARRAY_EQUAL(data5, caw5a.data(), caw5a.size());

      CData5 caw5b;
      CHECK_ARRAY_EQUAL(cdata5, caw5b.data(), caw5b.size());
    }

    //*************************************************************************
    TEST(test_indexes)
    {
      CHECK_EQUAL(5 ,               Data5::SIZE);
      CHECK_EQUAL(Data5::SIZE,      Data5::MAX_SIZE);
      CHECK_EQUAL(0,                Data5::FRONT);
      CHECK_EQUAL(Data5::SIZE - 1,  Data5::BACK);
      CHECK_EQUAL(0,                Data5::BEGIN);
      CHECK_EQUAL(Data5::SIZE,      Data5::END);
      CHECK_EQUAL(Data5::SIZE - 1,  Data5::RBEGIN);
      CHECK_EQUAL(Data5::BEGIN - 1, Data5::REND);
    }

    //*************************************************************************
    TEST(test_at)
    {
      Data5 aw5;

      CHECK_EQUAL(data5[2], aw5.at(2));

      Save(data5[2]);
      aw5.at(2) = 100;
      CHECK_EQUAL(100, data5[2]);
      CHECK_EQUAL(100, aw5.at(2));
      Restore(data5[2]);

      CHECK_THROW(aw5.at(aw5.size()), etl::array_wrapper_bounds);
    }

    //*************************************************************************
    TEST(test_at_const)
    {
      const Data5 caw5a; // array_wrapper is const.
      CData5 caw5b;      // array wrapped by array_wrapper is const.

      CHECK_EQUAL(data5[2], caw5a.at(2));
      CHECK_EQUAL(cdata5[2], caw5b.at(2));

      CHECK_THROW(caw5a.at(caw5a.size()), etl::array_wrapper_bounds);

      // These lines should fail to compile.
      // caw5a.at(2) = 100;
      // caw5b.at(100) = 100;
    }

    //*************************************************************************
    TEST(test_index_operator)
    {
      Data5 aw5;

      CHECK_EQUAL(data5[2], aw5[2]);

      Save(data5[2]);
      aw5[2] = 100;
      CHECK_EQUAL(100, data5[2]);
      CHECK_EQUAL(100, aw5[2]);
      Restore(data5[2]);
    }

    //*************************************************************************
    TEST(test_index_operator_const)
    {
      const Data5 caw5a; // array_wrapper is const.
      CData5 caw5b;      // array wrapped by array_wrapper is const.

      CHECK_EQUAL(data5[2], caw5a[2]);
      CHECK_EQUAL(cdata5[2], caw5b[2]);

      // These lines should fail to compile.
      // caw5a[2] = 100;
      // caw5b[100] = 100;
    }

    //*************************************************************************
    TEST(test_front)
    {
      Data5 aw5;
      CHECK_EQUAL(data5[Data5::FRONT], aw5.front());
    }

    //*************************************************************************
    TEST(test_front_const)
    {
      const Data5 caw5;
      CHECK_EQUAL(data5[Data5::FRONT], caw5.front());

      CData5 caw5b;
      CHECK_EQUAL(cdata5[Data5::FRONT], caw5b.front());
    }

    //*************************************************************************
    TEST(test_back)
    {
      Data5 aw5;
      CHECK_EQUAL(data5[Data5::BACK], aw5.back());
    }

    //*************************************************************************
    TEST(test_back_const)
    {
      const Data5 caw5a;
      CHECK_EQUAL(data5[Data5::BACK], caw5a.back());

      CData5 caw5b;
      CHECK_EQUAL(cdata5[CData5::BACK], caw5b.back());
    }

    //*************************************************************************
    TEST(test_data)
    {
      Data5 aw5;
      CHECK_EQUAL(data5, aw5.data());
    }

    //*************************************************************************
    TEST(test_data_const)
    {
      const Data5 caw5a;
      CHECK_EQUAL(data5, caw5a.data());

      CData5 caw5b;
      CHECK_EQUAL(cdata5, caw5b.data());
    }

    //*************************************************************************
    TEST(test_begin)
    {
      Data5 aw5;

      CHECK_EQUAL(0, Data5::BEGIN);
      CHECK_EQUAL(data5, aw5.begin());
    }

    //*************************************************************************
    TEST(test_begin_const)
    {
      const Data5 caw5a;
      CHECK_EQUAL(data5, caw5a.begin());
      CHECK_EQUAL(data5, caw5a.cbegin());

      CData5 caw5b;
      CHECK_EQUAL(cdata5, caw5b.begin());
      CHECK_EQUAL(cdata5, caw5b.cbegin());
    }

    //*************************************************************************
    TEST(test_end)
    {
      Data5 aw5;
      CHECK_EQUAL(&data5[Data5::END], aw5.end());
    }

    //*************************************************************************
    TEST(test_end_const)
    {
      const Data5 caw5a;
      CHECK_EQUAL(&data5[Data5::END], caw5a.end());
      CHECK_EQUAL(&data5[Data5::END], caw5a.cend());

      CData5 caw5b;
      CHECK_EQUAL(&cdata5[CData5::END], caw5b.end());
      CHECK_EQUAL(&cdata5[CData5::END], caw5b.cend());
    }

    //*************************************************************************
    TEST(test_rbegin)
    {
      Data5 aw5;
      CHECK_EQUAL(&data5[Data5::RBEGIN], &*aw5.rbegin());
    }

    //*************************************************************************
    TEST(test_rend)
    {
      Data5 aw5;
      CHECK_EQUAL(Data5::SIZE, std::distance(aw5.rbegin(), aw5.rend()));
    }

    //*************************************************************************
    TEST(test_crbegin)
    {
      Data5 aw5;
      CHECK_EQUAL(data5 + Data5::RBEGIN, &*aw5.crbegin());
    }

    //*************************************************************************
    TEST(test_crend)
    {
      const Data5 caw5a;
      CHECK_EQUAL(Data5::SIZE, std::distance(caw5a.rbegin(), caw5a.rend()));
      CHECK_EQUAL(Data5::SIZE, std::distance(caw5a.rbegin(), caw5a.rend()));

      CData5 caw5b;
      CHECK_EQUAL(Data5::SIZE, std::distance(caw5b.rbegin(), caw5b.rend()));
      CHECK_EQUAL(Data5::SIZE, std::distance(caw5b.rbegin(), caw5b.rend()));
    }

    //*************************************************************************
    TEST(test_iterator)
    {
      Data5 aw5;
      Data5::iterator itr = aw5.begin();
      int* p = data5;

      while (p != data5 + Data5::END)
      {
        CHECK_EQUAL(*p, *itr);
        ++p;
        ++itr;
      }

      Save(data5[2]);
      itr = aw5.begin() + 2;
      *itr = 100;
      CHECK_EQUAL(100, data5[2]);
      Restore(data5[2]);
    }

    //*************************************************************************
    TEST(test_const_iterator)
    {
      const Data5 caw5a;
      Data5::const_iterator itr = caw5a.begin();
      int* p = data5;

      while (p != data5 + Data5::END)
      {
        CHECK_EQUAL(*p, *itr);
        ++p;
        ++itr;
      }

      // These lines should fail to compile.
      // *itr = 100;
    }

    //*************************************************************************
    TEST(test_reverse_iterator)
    {
      Data5 aw5;
      Data5::reverse_iterator itr = aw5.rbegin();
      int* p = data5 + Data5::RBEGIN;

      while (itr != aw5.rend())
      {
        CHECK_EQUAL(*p, *itr);
        --p;
        ++itr;
      }

      Save(data5[2]);
      itr = aw5.rbegin() + 2;
      *itr = 100;
      CHECK_EQUAL(100, data5[2]);
      Restore(data5[2]);
    }

    //*************************************************************************
    TEST(test_const_reverse_iterator)
    {
      Data5 aw5;
      Data5::const_reverse_iterator itr = aw5.crbegin();
      int* p = data5 + Data5::RBEGIN;

      while (itr != aw5.crend())
      {
        CHECK_EQUAL(*p, *itr);
        --p;
        ++itr;
      }
    }

    //*************************************************************************
    TEST(test_size)
    {
      Data5 aw5;
      CHECK_EQUAL(5 , aw5.SIZE);
      CHECK_EQUAL(5U, aw5.size());

      const Data5 caw5a;
      CHECK_EQUAL(5 , caw5a.SIZE);
      CHECK_EQUAL(5U, caw5a.size());

      CData5 caw5b;
      CHECK_EQUAL(5 , caw5b.SIZE);
      CHECK_EQUAL(5U, caw5b.size());
    }

    //*************************************************************************
    TEST(test_max_size)
    {
      Data5 aw5;
      CHECK_EQUAL(5 , aw5.MAX_SIZE);
      CHECK_EQUAL(5U, aw5.max_size());

      const Data5 caw5a;
      CHECK_EQUAL(5 , caw5a.MAX_SIZE);
      CHECK_EQUAL(5U, caw5a.max_size());

      CData5 caw5b;
      CHECK_EQUAL(5 , caw5b.MAX_SIZE);
      CHECK_EQUAL(5U, caw5b.max_size());
    }

    //*************************************************************************
    TEST(test_fill)
    {
      Data5 aw5;
      Data5Same aw5same;

      aw5.fill(100);
      CHECK_ARRAY_EQUAL(data5b, aw5.data(), Data5::SIZE);

      std::copy(aw5same.begin(), aw5same.end(), aw5.begin());
    }

    //*************************************************************************
    TEST(test_swap)
    {
      Data5  aw5;
      Data5b aw5b;
      Data5c aw5c;
      Data5d aw5d;

      swap(aw5, aw5b);

      CHECK_ARRAY_EQUAL(aw5d.begin(), aw5.begin(),  aw5.size());
      CHECK_ARRAY_EQUAL(aw5c.begin(), aw5b.begin(), aw5b.size());

      // Put them back.
      swap(aw5, aw5b);
    }

    //*************************************************************************
    TEST(test_equal)
    {
      Data5  aw5;
      Data5Same aw5same;
      Data5Greater aw5greater;
      Data5Smaller aw5smaller;
      Data4 aw4;
      Data6 aw6;

      CHECK(aw5 == aw5same);
      CHECK(!(aw5 == aw5greater));
      CHECK(!(aw5 == aw5smaller));
      CHECK(!(aw5 == aw4));
      CHECK(!(aw5 == aw6));
    }

    //*************************************************************************
    TEST(test_not_equal)
    {
      Data5  aw5;
      Data5Same aw5same;
      Data5Greater aw5greater;
      Data5Smaller aw5smaller;
      Data4 aw4;
      Data6 aw6;

      CHECK(!(aw5 != aw5same));
      CHECK(aw5 != aw5greater);
      CHECK(aw5 != aw5smaller);
      CHECK(aw5 != aw4);
      CHECK(aw5 != aw6);
    }

    //*************************************************************************
    TEST(test_less_than)
    {
      Data5  aw5;
      Data5Same aw5same;
      Data5Greater aw5greater;
      Data5Smaller aw5smaller;
      Data4 aw4;
      Data6 aw6;

      CHECK(!(aw5 < aw5same));
      CHECK(!(aw5same < aw5));

      CHECK(aw5 < aw5greater);
      CHECK(!(aw5greater < aw5));

      CHECK(!(aw5 < aw5smaller));
      CHECK(aw5smaller < aw5);

      CHECK(!(aw5 < aw4));
      CHECK(aw4 < aw5);

      CHECK(aw5 < aw6);
      CHECK(!(aw6 < aw5));
    }

    //*************************************************************************
    TEST(test_less_than_equal)
    {
      Data5  aw5;
      Data5Same aw5same;
      Data5Greater aw5greater;
      Data5Smaller aw5smaller;
      Data4 aw4;
      Data6 aw6;

      CHECK(aw5 <= aw5same);
      CHECK(aw5same <= aw5);

      CHECK(aw5 <= aw5greater);
      CHECK(!(aw5greater <= aw5));

      CHECK(!(aw5 <= aw5smaller));
      CHECK(aw5smaller <= aw5);

      CHECK(!(aw5 <= aw4));
      CHECK(aw4 <= aw5);

      CHECK(aw5 <= aw6);
      CHECK(!(aw6 <= aw5));
    }

    //*************************************************************************
    TEST(test_greater_than)
    {
      Data5  aw5;
      Data5Same aw5same;
      Data5Greater aw5greater;
      Data5Smaller aw5smaller;
      Data4 aw4;
      Data6 aw6;

      CHECK(!(aw5 > aw5same));
      CHECK(!(aw5same > aw5));

      CHECK(!(aw5 > aw5greater));
      CHECK(aw5greater > aw5);

      CHECK(aw5 > aw5smaller);
      CHECK(!(aw5smaller > aw5));

      CHECK(aw5 > aw4);
      CHECK(!(aw4 > aw5));

      CHECK(!(aw5 > aw6));
      CHECK(aw6 > aw5);
    }

    //*************************************************************************
    TEST(test_greater_than_equal)
    {
      Data5 aw5;
      Data5Same aw5same;
      Data5Greater aw5greater;
      Data5Smaller aw5smaller;
      Data4 aw4;
      Data6 aw6;

      CHECK(aw5 >= aw5same);
      CHECK(aw5same >= aw5);

      CHECK(!(aw5 >= aw5greater));
      CHECK(aw5greater >= aw5);

      CHECK(aw5 >= aw5smaller);
      CHECK(!(aw5smaller >= aw5));

      CHECK(aw5 >= aw4);
      CHECK(!(aw4 >= aw5));

      CHECK(!(aw5 >= aw6));
      CHECK(aw6 >= aw5);
    }

    //*************************************************************************
    TEST(test_hash)
    {
      Data5  aw5;
      size_t hash = etl::hash<Data5>()(aw5);

      size_t compare_hash = etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(data5), reinterpret_cast<const uint8_t*>(data5 + 5U));

      CHECK_EQUAL(compare_hash, hash);
    }
  };
}

#if defined(ETL_COMPILER_CLANG)
  #pragma clang diagnostic pop
#endif

