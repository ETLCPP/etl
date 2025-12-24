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

#include <vector>
#include <string.h>

#include "etl/bloom_filter.h"

#include "etl/fnv_1.h"
#include "etl/crc16.h"
#include "etl/crc16_ccitt.h"
#include "etl/crc32.h"

#include "etl/char_traits.h"

struct hash1_t
{
  typedef const char* argument_type;

  size_t operator ()(argument_type text) const
  {
    return etl::fnv_1a_32(text, text + etl::char_traits<char>::length(text));
  }
};

struct hash2_t
{
  typedef const char* argument_type;

  size_t operator ()(argument_type text) const
  {
    return etl::crc32(text, text + etl::char_traits<char>::length(text));
  }
};

struct hash3_t
{
  typedef const char* argument_type;

  size_t operator ()(argument_type text) const
  {
    return etl::crc16(text, text + etl::char_traits<char>::length(text)) | (etl::crc16_ccitt(text, text + etl::char_traits<char>::length(text)) << 16);
  }
};

std::vector<const char*> exist_text     = { "The", "rain", "in", "Spain", "falls", "mainly", "on", "the", "plain" };
std::vector<const char*> not_exist_text = { "My", "hovercraft", "is", "full", "of", "eels" };

namespace
{
  SUITE(test_bloom_filter)
  {
    //*************************************************************************
    TEST(test_bloom_filter1)
    {
      etl::bloom_filter<256, hash1_t> bloom;

      for (size_t i = 0UL; i < exist_text.size(); ++i)
      {
        bloom.add(exist_text[i]);
      }

      // Check for false negatives.
      bool all_exist = true;

      for (size_t i = 0UL; i < exist_text.size(); ++i)
      {
        all_exist = all_exist && bloom.exists(exist_text[i]);
      }

      CHECK(all_exist);

      // Check for false positives. There should be none for this set.
      bool any_exist = false;

      for (size_t i = 0UL; i < not_exist_text.size(); ++i)
      {
        any_exist = any_exist || bloom.exists(not_exist_text[i]);
      }

      CHECK(!any_exist);

      size_t usage = bloom.usage();
      CHECK(usage > 0);
      CHECK(usage < 100);

      size_t count = bloom.count();
      CHECK(count > 0);
      CHECK(count < 256);
    }

    //*************************************************************************
    TEST(test_bloom_filter2)
    {
      etl::bloom_filter<256, hash1_t, hash2_t> bloom;

      for (size_t i = 0UL; i < exist_text.size(); ++i)
      {
        bloom.add(exist_text[i]);
      }

      // Check for false negatives.
      bool all_exist = true;

      for (size_t i = 0UL; i < exist_text.size(); ++i)
      {
        all_exist = all_exist && bloom.exists(exist_text[i]);
      }

      CHECK(all_exist);

      // Check for false positives. There should be none for this set.
      bool any_exist = false;

      for (size_t i = 0UL; i < not_exist_text.size(); ++i)
      {
        any_exist = any_exist || bloom.exists(not_exist_text[i]);
      }

      CHECK(!any_exist);

      size_t usage = bloom.usage();
      CHECK(usage < 100);

      size_t count = bloom.count();
      CHECK(count > 0);
      CHECK(count < 256);
    }

    //*************************************************************************
    TEST(test_bloom_filter3)
    {
      etl::bloom_filter<256, hash1_t, hash2_t, hash3_t> bloom;

      for (size_t i = 0UL; i < exist_text.size(); ++i)
      {
        bloom.add(exist_text[i]);
      }

      // Check for false negatives.
      bool all_exist = true;

      for (size_t i = 0UL; i < exist_text.size(); ++i)
      {
        all_exist = all_exist && bloom.exists(exist_text[i]);
      }

      CHECK(all_exist);

      // Check for false positives. There should be none for this set.
      bool any_exist = false;

      for (size_t i = 0UL; i < not_exist_text.size(); ++i)
      {
        any_exist = any_exist || bloom.exists(not_exist_text[i]);
      }

      CHECK(!any_exist);

      size_t usage = bloom.usage();
      CHECK(usage < 100);

      size_t count = bloom.count();
      CHECK(count > 0);
      CHECK(count < 256);
    }

    //*************************************************************************
    TEST(test_width)
    {
      typedef etl::bloom_filter<256, hash1_t> Bloom;
      Bloom bloom;

      CHECK_EQUAL(256U, bloom.width());
      CHECK_EQUAL(256U, Bloom::WIDTH);
    }

    //*************************************************************************
    TEST(test_count)
    {
      etl::bloom_filter<64, hash1_t> bloom;

      CHECK(bloom.count() == 0);

      for (size_t i = 0UL; i < exist_text.size(); ++i)
      {
        bloom.add(exist_text[i]);
      }

      CHECK(bloom.count() > 0);
    }

    //*************************************************************************
    TEST(test_usage)
    {
      etl::bloom_filter<64, hash1_t> bloom;

      CHECK(bloom.usage() == 0);

      for (size_t i = 0UL; i < exist_text.size(); ++i)
      {
        bloom.add(exist_text[i]);
      }

      CHECK(bloom.usage() > 0);
    }

    //*************************************************************************
    TEST(test_clear)
    {
      etl::bloom_filter<64, hash1_t> bloom;

      for (size_t i = 0UL; i < exist_text.size(); ++i)
      {
        bloom.add(exist_text[i]);
      }

      bloom.clear();

      size_t usage = bloom.usage();
      size_t count = bloom.count();

      CHECK_EQUAL(0U, usage);
      CHECK_EQUAL(0U, count);

      // Check that we get no matches.
      bool any_exist = false;

      for (size_t i = 0UL; i < not_exist_text.size(); ++i)
      {
        any_exist = any_exist || bloom.exists(not_exist_text[i]);
      }

      CHECK(!any_exist);
    }
  };
}

