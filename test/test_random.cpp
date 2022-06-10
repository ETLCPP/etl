/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2016 John Wellbelove

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

#include <stdint.h>

#include "etl/random.h"
#include "etl/crc32.h"

#include <vector>
#include <algorithm>
#include <fstream>

namespace
{
  SUITE(test_random)
  {
    //*************************************************************************
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
        for (size_t i = 0UL; i < out1.size(); ++i)
        {
          file << out1[i] << "\n";
        }
      }

      file.close();
    }

    //*************************************************************************
    TEST(test_random_xorshift_range)
    {
      etl::random_xorshift r;

      uint32_t low  = 1234UL;
      uint32_t high = 9876UL;

      for (int i = 0; i < 100000; ++i)
      {
        uint32_t n = r.range(low, high);

        CHECK(n >= low);
        CHECK(n <= high);
      }
    }

    //*************************************************************************
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
        for (size_t i = 0UL; i < out1.size(); ++i)
        {
          file << out1[i] << "\n";
        }
      }

      file.close();
    }

    //*************************************************************************
    TEST(test_random_lcg_range)
    {
      etl::random_lcg r;

      uint32_t low  = 1234UL;
      uint32_t high = 9876UL;

      for (int i = 0; i < 100000; ++i)
      {
        uint32_t n = r.range(low, high);

        CHECK(n >= low);
        CHECK(n <= high);
      }
    }

    //*************************************************************************
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
        for (size_t i = 0UL; i < out1.size(); ++i)
        {
          file << out1[i] << "\n";
        }
      }

      file.close();
    }

    //*************************************************************************
    TEST(test_random_clcg_range)
    {
      etl::random_clcg r;

      uint32_t low = 1234UL;
      uint32_t high = 9876UL;

      for (int i = 0; i < 100000; ++i)
      {
        uint32_t n = r.range(low, high);

        CHECK(n >= low);
        CHECK(n <= high);
      }
    }

    //*************************************************************************
    TEST(test_random_lsfr_sequence)
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
        for (size_t i = 0UL; i < out1.size(); ++i)
        {
          file << out1[i] << "\n";
        }
      }

      file.close();
    }

    //*************************************************************************
    TEST(test_random_lsfr_range)
    {
      etl::random_lsfr r;

      uint32_t low  = 1234UL;
      uint32_t high = 9876UL;

      for (int i = 0; i < 100000; ++i)
      {
        uint32_t n = r.range(low, high);

        CHECK(n >= low);
        CHECK(n <= high);
      }
    }

    //*************************************************************************
    TEST(test_random_mwc_sequence)
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
        for (size_t i = 0UL; i < out1.size(); ++i)
        {
          file << out1[i] << "\n";
        }
      }

      file.close();
    }

    //*************************************************************************
    TEST(test_random_mwc_range)
    {
      etl::random_mwc r;

      uint32_t low = 1234UL;
      uint32_t high = 9876UL;

      for (int i = 0; i < 100000; ++i)
      {
        uint32_t n = r.range(low, high);

        CHECK(n >= low);
        CHECK(n <= high);
      }
    }

    //*************************************************************************
    TEST(test_random_pcg_sequence)
    {
      std::vector<uint32_t> out1(10000);
      etl::random_pcg r;

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

      std::ofstream file("random_pcg.csv");

      if (!file.fail())
      {
        for (size_t i = 0UL; i < out1.size(); ++i)
        {
          file << out1[i] << "\n";
        }
      }

      file.close();
    }

    //*************************************************************************
    TEST(test_random_pcg_range)
    {
      etl::random_pcg r;

      uint32_t low = 1234UL;
      uint32_t high = 9876UL;

      for (int i = 0; i < 100000; ++i)
      {
        uint32_t n = r.range(low, high);

        CHECK(n >= low);
        CHECK(n <= high);
      }
    }

    //*************************************************************************
    TEST(test_random_hash_sequence)
    {
      std::vector<uint32_t> out1(10000);
      etl::random_hash<etl::crc32> r;

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

      std::ofstream file("random_hash.csv");

      if (!file.fail())
      {
        for (size_t i = 0UL; i < out1.size(); ++i)
        {
          file << out1[i] << "\n";
        }
      }

      file.close();
    }

    //*************************************************************************
    TEST(test_random_hash_range)
    {
      etl::random_hash<etl::crc32> r;

      uint32_t low  = 1234UL;
      uint32_t high = 9876UL;

      for (int i = 0; i < 100000; ++i)
      {
        uint32_t n = r.range(low, high);

        CHECK(n >= low);
        CHECK(n <= high);
      }
    }

  };
}
