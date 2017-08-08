/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2016 jwellbelove

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

#include "UnitTest++.h"

#include <stdint.h>

#include "random.h"

#include <vector>
#include <algorithm>
#include <fstream>

namespace
{
  SUITE(test_random)
  {
    //=========================================================================
    TEST(test_random_xorshift_sequence)
    {
      std::vector<uint32_t> out1(10000);
      etl::random_xorshift r;

      struct generator
      {
        generator(etl::random& r_)
          : r(r_)
        {
        }

        uint32_t operator()()
        {
          return r();
        }

        etl::random& r;
      };

      std::generate(out1.begin(), out1.end(), generator(r));

      std::ofstream file("random_xorshift.csv");

      if (!file.fail())
      {
        for (size_t i = 0; i < out1.size(); i += 2)
        {
          file << out1[i] << "," << out1[i + 1] << "\n";
        }
      }

      file.close();
    }

    //=========================================================================
    TEST(test_random_xorshift_range)
    {
      etl::random_xorshift r;

      uint32_t low  = 1234;
      uint32_t high = 9876;

      for (int i = 0; i < 100000; ++i)
      {
        uint32_t n = r.range(low, high);

        CHECK(n >= low);
        CHECK(n <= high);
      }
    }

    //=========================================================================
    TEST(test_random_lcg_sequence)
    {
      std::vector<uint32_t> out1(10000);
      etl::random_lcg r;

      struct generator
      {
        generator(etl::random& r_)
          : r(r_)
        {
        }

        uint32_t operator()()
        {
          return r();
        }

        etl::random& r;
      };

      std::generate(out1.begin(), out1.end(), generator(r));

      std::ofstream file("random_lcg.csv");

      if (!file.fail())
      {
        for (size_t i = 0; i < out1.size(); i += 2)
        {
          file << out1[i] << "," << out1[i + 1] << "\n";
        }
      }

      file.close();
    }

    //=========================================================================
    TEST(test_random_lcg_range)
    {
      etl::random_lcg r;

      uint32_t low  = 1234;
      uint32_t high = 9876;

      for (int i = 0; i < 100000; ++i)
      {
        uint32_t n = r.range(low, high);

        CHECK(n >= low);
        CHECK(n <= high);
      }
    }

    //=========================================================================
    TEST(test_random_clcg_sequence)
    {
      std::vector<uint32_t> out1(10000);
      etl::random_clcg r;

      struct generator
      {
        generator(etl::random& r_)
          : r(r_)
        {
        }

        uint32_t operator()()
        {
          return r();
        }

        etl::random& r;
      };

      std::generate(out1.begin(), out1.end(), generator(r));

      std::ofstream file("random_clcg.csv");

      if (!file.fail())
      {
        for (size_t i = 0; i < out1.size(); i += 2)
        {
          file << out1[i] << "," << out1[i + 1] << "\n";
        }
      }

      file.close();
    }

    //=========================================================================
    TEST(test_random_clcg_range)
    {
      etl::random_clcg r;

      uint32_t low = 1234;
      uint32_t high = 9876;

      for (int i = 0; i < 100000; ++i)
      {
        uint32_t n = r.range(low, high);

        CHECK(n >= low);
        CHECK(n <= high);
      }
    }

    //=========================================================================
    TEST(test_random_lfsr_sequence)
    {
      std::vector<uint32_t> out1(10000);
      etl::random_lsfr r;

      struct generator
      {
        generator(etl::random& r_)
          : r(r_)
        {
        }

        uint32_t operator()()
        {
          return r();
        }

        etl::random& r;
      };

      std::generate(out1.begin(), out1.end(), generator(r));

      std::ofstream file("random_lsfr.csv");

      if (!file.fail())
      {
        for (size_t i = 0; i < out1.size(); i += 2)
        {
          file << out1[i] << "," << out1[i + 1] << "\n";
        }
      }

      file.close();
    }

    //=========================================================================
    TEST(test_random_lfsr_range)
    {
      etl::random_lsfr r;

      uint32_t low  = 1234;
      uint32_t high = 9876;

      for (int i = 0; i < 100000; ++i)
      {
        uint32_t n = r.range(low, high);

        CHECK(n >= low);
        CHECK(n <= high);
      }
    }

    //=========================================================================
    TEST(test_random_fast_sequence)
    {
      std::vector<uint32_t> out1(10000);
      etl::random_mwc r;

      struct generator
      {
        generator(etl::random& r_)
          : r(r_)
        {
        }

        uint32_t operator()()
        {
          return r();
        }

        etl::random& r;
      };

      std::generate(out1.begin(), out1.end(), generator(r));

      std::ofstream file("random_mwc.csv");

      if (!file.fail())
      {
        for (size_t i = 0; i < out1.size(); i += 2)
        {
          file << out1[i] << "," << out1[i + 1] << "\n";
        }
      }

      file.close();
    }

    //=========================================================================
    TEST(test_random_fast_range)
    {
      etl::random_mwc r;

      uint32_t low = 1234;
      uint32_t high = 9876;

      for (int i = 0; i < 100000; ++i)
      {
        uint32_t n = r.range(low, high);

        CHECK(n >= low);
        CHECK(n <= high);
      }
    }
  };
}
