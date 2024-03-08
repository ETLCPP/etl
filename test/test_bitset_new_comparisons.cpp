/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2022 John Wellbelove

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

#include "etl/private/bitset_new.h"

namespace
{
  using ull = unsigned long long;

  SUITE(test_bitset_new_comparisons)
  {
    //*************************************************************************
    TEST(test_equality_single_storage_model)
    {
      ETL_CONSTEXPR14 etl::bitset<64, uint64_t> data1(0x0123456789012345ULL);
      ETL_CONSTEXPR14 etl::bitset<64, uint64_t> data2(0x0123456789012345ULL);

      etl::bitset_ext<64, uint64_t>::buffer_type buffer3;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer4;
      etl::bitset_ext<64, uint64_t> data3(0x0123456789012345ULL, buffer3);
      etl::bitset_ext<64, uint64_t> data4(0x0123456789012345ULL, buffer4);

      // Double check we are using the single element storage model.
      CHECK(etl::bitset_storage_model::Single == data1.Storage_Model);
      CHECK(etl::bitset_storage_model::Single == data2.Storage_Model);
      CHECK(etl::bitset_storage_model::Single == data3.Storage_Model);
      CHECK(etl::bitset_storage_model::Single == data4.Storage_Model);

      CHECK(data1 == data2); // bitset     == bitset
      CHECK(data1 == data3); // bitset     == bitset_ext
      CHECK(data3 == data1); // bitset_ext == bitset
      CHECK(data3 == data4); // bitset_ext == bitset_ext
    }

    //*************************************************************************
    TEST(test_inequality_single_storage_model)
    {
      ETL_CONSTEXPR14 etl::bitset<64, uint64_t> data1(0x0123456789012345ULL);
      ETL_CONSTEXPR14 etl::bitset<64, uint64_t> data2(0x0123456784012345ULL);

      etl::bitset_ext<64, uint64_t>::buffer_type buffer3;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer4;
      etl::bitset_ext<64, uint64_t> data3(0x0123456787012345ULL, buffer3);
      etl::bitset_ext<64, uint64_t> data4(0x0123426789012345ULL, buffer4);

      // Double check we are using the single element storage model.
      CHECK(etl::bitset_storage_model::Single == data1.Storage_Model);
      CHECK(etl::bitset_storage_model::Single == data2.Storage_Model);
      CHECK(etl::bitset_storage_model::Single == data3.Storage_Model);
      CHECK(etl::bitset_storage_model::Single == data4.Storage_Model);

      CHECK(data1 != data2); // bitset     != bitset
      CHECK(data1 != data3); // bitset     != bitset_ext
      CHECK(data3 != data1); // bitset_ext != bitset
      CHECK(data3 != data4); // bitset_ext != bitset_ext
    }

    //*************************************************************************
    TEST(test_equality_multi_storage_model)
    {
      ETL_CONSTEXPR14 etl::bitset<64> data1(0x0123456789012345ULL);
      ETL_CONSTEXPR14 etl::bitset<64> data2(0x0123456789012345ULL);

      etl::bitset_ext<64>::buffer_type buffer3;
      etl::bitset_ext<64>::buffer_type buffer4;
      etl::bitset_ext<64> data3(0x0123456789012345ULL, buffer3);
      etl::bitset_ext<64> data4(0x0123456789012345ULL, buffer4);

      // Double check we are using the multi element storage model.
      CHECK(etl::bitset_storage_model::Multi == data1.Storage_Model);
      CHECK(etl::bitset_storage_model::Multi == data2.Storage_Model);
      CHECK(etl::bitset_storage_model::Multi == data3.Storage_Model);
      CHECK(etl::bitset_storage_model::Multi == data4.Storage_Model);

      CHECK(data1 == data2); // bitset     == bitset
      CHECK(data1 == data3); // bitset     == bitset_ext
      CHECK(data3 == data1); // bitset_ext == bitset
      CHECK(data3 == data4); // bitset_ext == bitset_ext
    }

    //*************************************************************************
    TEST(test_inequality_multi_storage_model)
    {
      ETL_CONSTEXPR14 etl::bitset<64> data1(0x0123456789012345ULL);
      ETL_CONSTEXPR14 etl::bitset<64> data2(0x0123456784012345ULL);

      etl::bitset_ext<64>::buffer_type buffer3;
      etl::bitset_ext<64>::buffer_type buffer4;
      etl::bitset_ext<64> data3(0x0123456787012345ULL, buffer3);
      etl::bitset_ext<64> data4(0x0123426789012345ULL, buffer4);

      // Double check we are using the multi element storage model.
      CHECK(etl::bitset_storage_model::Multi == data1.Storage_Model);
      CHECK(etl::bitset_storage_model::Multi == data2.Storage_Model);
      CHECK(etl::bitset_storage_model::Multi == data3.Storage_Model);
      CHECK(etl::bitset_storage_model::Multi == data4.Storage_Model);

      CHECK(data1 != data2); // bitset     != bitset
      CHECK(data1 != data3); // bitset     != bitset_ext
      CHECK(data3 != data1); // bitset_ext != bitset
      CHECK(data3 != data4); // bitset_ext != bitset_ext
    }

    //*************************************************************************
    TEST(test_equality_mixed_storage_model)
    {
      ETL_CONSTEXPR14 etl::bitset<64, uint64_t> data1(0x0123456789012345ULL);
      ETL_CONSTEXPR14 etl::bitset<64> data2(0x0123456789012345ULL);

      etl::bitset_ext<64, uint64_t>::buffer_type buffer3;
      etl::bitset_ext<64>::buffer_type buffer4;
      etl::bitset_ext<64, uint64_t> data3(0x0123456789012345ULL, buffer3);
      etl::bitset_ext<64> data4(0x0123456789012345ULL, buffer4);

      // Double check we are using the single/multi element storage model.
      CHECK(etl::bitset_storage_model::Single == data1.Storage_Model);
      CHECK(etl::bitset_storage_model::Multi  == data2.Storage_Model);
      CHECK(etl::bitset_storage_model::Single == data3.Storage_Model);
      CHECK(etl::bitset_storage_model::Multi  == data4.Storage_Model);

      CHECK(data1 == data2); // bitset     (S) == bitset     (M)
      CHECK(data2 == data1); // bitset     (M) == bitset     (S)
      CHECK(data3 == data4); // bitset_ext (S) == bitset_ext (M)
      CHECK(data4 == data3); // bitset_ext (M) == bitset_ext (S)
      CHECK(data1 == data4); // bitset     (S) == bitset_ext (M)
      CHECK(data4 == data1); // bitset_ext (M) == bitset     (S)
      CHECK(data2 == data3); // bitset     (M) == bitset_ext (S)
      CHECK(data3 == data2); // bitset_ext (S) == bitset     (M)
    }

    //*************************************************************************
    TEST(test_inequality_mixed_storage_model)
    {
      ETL_CONSTEXPR14 etl::bitset<64, uint64_t> data1(0x0123456789012345ULL);
      ETL_CONSTEXPR14 etl::bitset<64> data2(0x0123456784012345ULL);

      etl::bitset_ext<64, uint64_t>::buffer_type buffer3;
      etl::bitset_ext<64>::buffer_type buffer4;
      etl::bitset_ext<64, uint64_t> data3(0x0123456787012345ULL, buffer3);
      etl::bitset_ext<64> data4(0x0123426789012345ULL, buffer4);

      // Double check we are using the single/multi element storage model.
      CHECK(etl::bitset_storage_model::Single == data1.Storage_Model);
      CHECK(etl::bitset_storage_model::Multi  == data2.Storage_Model);
      CHECK(etl::bitset_storage_model::Single == data3.Storage_Model);
      CHECK(etl::bitset_storage_model::Multi  == data4.Storage_Model);

      CHECK(data1 != data2); // bitset     (S) != bitset     (M)
      CHECK(data2 != data1); // bitset     (M) != bitset     (S)
      CHECK(data3 != data4); // bitset_ext (S) != bitset_ext (M)
      CHECK(data4 != data3); // bitset_ext (M) != bitset_ext (S)
      CHECK(data1 != data4); // bitset     (S) != bitset_ext (M)
      CHECK(data4 != data1); // bitset_ext (M) != bitset     (S)
      CHECK(data2 != data3); // bitset     (M) != bitset_ext (S)
      CHECK(data3 != data2); // bitset_ext (S) != bitset     (M) 
    }

    //*************************************************************************
    TEST(test_equality_mixed_storage_model_different_element_types)
    {
      ETL_CONSTEXPR14 etl::bitset<64, uint32_t> data1(0x0123456789012345ULL);
      ETL_CONSTEXPR14 etl::bitset<64, uint8_t>  data2(0x0123456789012345ULL);

      etl::bitset_ext<64, uint32_t>::buffer_type buffer3;
      etl::bitset_ext<64, uint8_t>::buffer_type  buffer4;
      etl::bitset_ext<64, uint32_t> data3(0x0123456789012345ULL, buffer3);
      etl::bitset_ext<64, uint8_t>  data4(0x0123456789012345ULL, buffer4);

      // Double check we are using the multi element storage model.
      CHECK(etl::bitset_storage_model::Multi == data1.Storage_Model);
      CHECK(etl::bitset_storage_model::Multi == data2.Storage_Model);
      CHECK(etl::bitset_storage_model::Multi == data3.Storage_Model);
      CHECK(etl::bitset_storage_model::Multi == data4.Storage_Model);

      CHECK(data1 == data2); // bitset     (S) == bitset     (M)
      CHECK(data2 == data1); // bitset     (M) == bitset     (S)
      CHECK(data3 == data4); // bitset_ext (S) == bitset_ext (M)
      CHECK(data4 == data3); // bitset_ext (M) == bitset_ext (S)
      CHECK(data1 == data4); // bitset     (S) == bitset_ext (M)
      CHECK(data4 == data1); // bitset_ext (M) == bitset     (S)
      CHECK(data2 == data3); // bitset     (M) == bitset_ext (S)
      CHECK(data3 == data2); // bitset_ext (S) == bitset     (M)
    }

    //*************************************************************************
    TEST(test_inequality_mixed_storage_model_different_element_types)
    {
      ETL_CONSTEXPR14 etl::bitset<64, uint32_t> data1(0x0123456789012345ULL);
      ETL_CONSTEXPR14 etl::bitset<64, uint8_t>  data2(0x0123456784012345ULL);

      etl::bitset_ext<64, uint32_t>::buffer_type buffer3;
      etl::bitset_ext<64, uint8_t>::buffer_type  buffer4;
      etl::bitset_ext<64, uint32_t> data3(0x0123456787012345ULL, buffer3);
      etl::bitset_ext<64, uint8_t>  data4(0x0123426789012345ULL, buffer4);

      // Double check we are using the multi element storage model.
      CHECK(etl::bitset_storage_model::Multi == data1.Storage_Model);
      CHECK(etl::bitset_storage_model::Multi == data2.Storage_Model);
      CHECK(etl::bitset_storage_model::Multi == data3.Storage_Model);
      CHECK(etl::bitset_storage_model::Multi == data4.Storage_Model);

      CHECK(data1 != data2); // bitset     (S) != bitset     (M)
      CHECK(data2 != data1); // bitset     (M) != bitset     (S)
      CHECK(data3 != data4); // bitset_ext (S) != bitset_ext (M)
      CHECK(data4 != data3); // bitset_ext (M) != bitset_ext (S)
      CHECK(data1 != data4); // bitset     (S) != bitset_ext (M)
      CHECK(data4 != data1); // bitset_ext (M) != bitset     (S)
      CHECK(data2 != data3); // bitset     (M) != bitset_ext (S)
      CHECK(data3 != data2); // bitset_ext (S) != bitset     (M)
    }
  };
}
