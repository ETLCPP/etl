/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2020 jwellbelove

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

#include "UnitTest++/UnitTest++.h"

#include "etl/flags.h"
#include "etl/binary.h"
#include "etl/string.h"
#include "etl/wstring.h"
#include "etl/u16string.h"
#include "etl/u32string.h"

namespace
{
  enum
  {
    F0 = etl::bit<0>::value,
    F1 = etl::bit<1>::value,
    F2 = etl::bit<2>::value,
    F3 = etl::bit<3>::value,
    F4 = etl::bit<4>::value,
    F5 = etl::bit<5>::value,
    F6 = etl::bit<6>::value,
    F7 = etl::bit<7>::value,
    F0246 = F0 | F2 | F4 | F6,
    F1357 = F1 | F3 | F5 | F7,
    F01234567 = F0 | F1 | F2 | F3 | F4 | F5 | F6 | F7,
    F_NONE = 0
  };

  using Flags    = etl::flags<uint8_t>;
  using FlagsM0246 = etl::flags<uint8_t, F0246>;

  SUITE(test_flags)
  {
    //*************************************************************************
    TEST(test_constructor)
    {
      Flags flags;

      CHECK_EQUAL(0U, flags);
      CHECK_EQUAL(0U, flags.value());
    }

    //*************************************************************************
    TEST(test_value)
    {
      Flags flags;

      CHECK_EQUAL(Flags::value_type(0), flags);
      CHECK_EQUAL(Flags::value_type(0), flags.value());

      flags.value(0x5A);
      CHECK_EQUAL(Flags::value_type(0x5A), flags);
      CHECK_EQUAL(Flags::value_type(0x5A), flags.value());
    }

    //*************************************************************************
    TEST(test_set)
    {
      Flags flags1;
      Flags flags2;
      Flags flags3;

      CHECK_EQUAL(int(Flags::value_type(F1357)), int((flags1.set(F7, true).set(F6, false).set(F5, true).set(F4, false).set(F3, true).set(F2, false).set(F1, true).set(F0, false).value())));
      CHECK_EQUAL(int(Flags::value_type(F1357)), int((flags2.set<F7>(true).set<F6>(false).set<F5>(true).set<F4>(false).set<F3>(true).set<F2>(false).set<F1>(true).set<F0>(false).value())));
      CHECK_EQUAL(int(Flags::value_type(F1357)), int((flags3.set<F7, true>().set<F6, false>().set<F5, true>().set<F4, false>().set<F3, true>().set<F2, false>().set<F1, true>().set<F0, false>().value())));
    }

    //*************************************************************************
    TEST(test_set_multiple)
    {
      Flags flags1;
      Flags flags2;
      Flags flags3;

      CHECK_EQUAL(int(Flags::value_type(F1357)), int((flags1.set(F1357, true).value())));
      CHECK_EQUAL(int(Flags::value_type(F1357)), int((flags2.set<F1357>(true).value())));
      CHECK_EQUAL(int(Flags::value_type(F1357)), int((flags3.set<F1357, true>().value())));
    }

    //*************************************************************************
    TEST(test_set_reset)
    {
      Flags flags1(F01234567);
      Flags flags2(F01234567);

      CHECK_EQUAL(int(Flags::value_type(F1357)), int((flags1.set(F7).reset(F6).set(F5).reset(F4).set(F3).reset(F2).set(F1).reset(F0).value())));
      CHECK_EQUAL(int(Flags::value_type(F1357)), int((flags2.set<F7>().reset<F6>().set<F5>().reset<F4>().set<F3>().reset<F2>().set<F1>().reset<F0>().value())));
    }

    //*************************************************************************
    TEST(test_clear)
    {
      Flags flags1(0xFF);

      CHECK_EQUAL(int(Flags::value_type(0)), flags1.clear());
    }

    //*************************************************************************
    TEST(test_reset)
    {
      Flags flags1(F0246);
      Flags flags2(F0246);

      CHECK_EQUAL(int(Flags::value_type(0x54)), flags1.reset(F0));
      CHECK_EQUAL(int(Flags::value_type(0x54)), flags2.reset<F0>());
    }

    //*************************************************************************
    TEST(test_all)
    {
      Flags flags1(F_NONE);
      Flags flags2(0x5A);
      Flags flags3(0xFF);

      CHECK(flags1.all() == false);
      CHECK(flags2.all() == false);
      CHECK(flags3.all() == true);
    }

    //*************************************************************************
    TEST(test_all_masked)
    {
      FlagsM0246 flags1(F_NONE);
      FlagsM0246 flags2(0x5A);
      FlagsM0246 flags3(F0246);
      FlagsM0246 flags4(0xFF);

      CHECK(flags1.all() == false);
      CHECK(flags2.all() == false);
      CHECK(flags3.all() == true);
      CHECK(flags4.all() == true);
    }

    //*************************************************************************
    TEST(test_all_of)
    {
      Flags flags1(F0246);

      CHECK(flags1.all_of(F0, F2, F4, F6)     == true);
      CHECK((flags1.all_of<F0, F2, F4, F6>()) == true);

      CHECK(flags1.all_of(F2, F4, F6)     == true);
      CHECK((flags1.all_of<F2, F4, F6>()) == true);

      CHECK(flags1.all_of(F1, F2, F4, F6)     == false);
      CHECK((flags1.all_of<F1, F2, F4, F6>()) == false);

      CHECK(flags1.all_of(F0, F1, F2, F3, F4, F5, F6, F7)     == false);
      CHECK((flags1.all_of<F0, F1, F2, F3, F4, F5, F6, F7>()) == false);

      Flags flags2(F1357);

      CHECK(flags2.all_of(F0, F2, F4, F6)     == false);
      CHECK((flags2.all_of<F0, F2, F4, F6>()) == false);

      CHECK(flags2.all_of(F2, F4, F6)     == false);
      CHECK((flags2.all_of<F2, F4, F6>()) == false);

      CHECK(flags2.all_of(F1, F2, F4, F6)     == false);
      CHECK((flags2.all_of<F1, F2, F4, F6>()) == false);

      CHECK(flags2.all_of(F0, F1, F2, F3, F4, F5, F6, F7)     == false);
      CHECK((flags2.all_of<F0, F1, F2, F3, F4, F5, F6, F7>()) == false);
    }

    //*************************************************************************
    TEST(test_all_of_masked)
    {
      FlagsM0246 flags1(F0246);
      
      CHECK(flags1.all_of(F0, F2, F4, F6) == true);
      CHECK((flags1.all_of<F0, F2, F4, F6>()) == true);

      CHECK(flags1.all_of(F2, F4, F6) == true);
      CHECK((flags1.all_of<F2, F4, F6>()) == true);

      CHECK(flags1.all_of(F1, F2, F4, F6) == false);
      CHECK((flags1.all_of<F1, F2, F4, F6>()) == false);

      CHECK(flags1.all_of(F0, F1, F2, F3, F4, F5, F6, F7) == false);
      CHECK((flags1.all_of<F0, F1, F2, F3, F4, F5, F6, F7>()) == false);

      FlagsM0246 flags2(F1357);

      CHECK(flags2.all_of(F0, F2, F4, F6) == false);
      CHECK((flags2.all_of<F0, F2, F4, F6>()) == false);

      CHECK(flags2.all_of(F2, F4, F6) == false);
      CHECK((flags2.all_of<F2, F4, F6>()) == false);

      CHECK(flags2.all_of(F1, F2, F4, F6) == false);
      CHECK((flags2.all_of<F1, F2, F4, F6>()) == false);

      CHECK(flags2.all_of(F0, F1, F2, F3, F4, F5, F6, F7) == false);
      CHECK((flags2.all_of<F0, F1, F2, F3, F4, F5, F6, F7>()) == false);
    }

    //*************************************************************************
    TEST(test_any)
    {
      Flags flags1(F_NONE);
      Flags flags2(0x08);

      CHECK(flags1.any() == false);
      CHECK(flags2.any() == true);
    }

    //*************************************************************************
    TEST(test_any_masked)
    {
      FlagsM0246 flags1(F_NONE);
      FlagsM0246 flags2(0x50);
      FlagsM0246 flags3(0x0A);

      CHECK(flags1.any() == false);
      CHECK(flags2.any() == true);
      CHECK(flags3.any() == false);
    }

    //*************************************************************************
    TEST(test_any_of)
    {
      Flags flags(F0246);

      CHECK(flags.any_of(F0, F2, F4, F6)     == true);
      CHECK((flags.any_of<F0, F2, F4, F6>()) == true);

      CHECK(flags.any_of(F2, F4, F6)     == true);
      CHECK((flags.any_of<F2, F4, F6>()) == true);

      CHECK(flags.any_of(F1, F3, F5, F7)     == false);
      CHECK((flags.any_of<F1, F3, F5, F7>()) == false);

      CHECK(flags.any_of(F0, F1, F2, F3, F4, F5, F6, F7)     == true);
      CHECK((flags.any_of<F0, F1, F2, F3, F4, F5, F6, F7>()) == true);
    }

    //*************************************************************************
    TEST(test_any_of_masked)
    {
      FlagsM0246 flags(F0246);

      CHECK(flags.any_of(F0, F2, F4, F6) == true);
      CHECK((flags.any_of<F0, F2, F4, F6>()) == true);

      CHECK(flags.any_of(F2, F3, F5) == true);
      CHECK((flags.any_of<F2, F3, F5>()) == true);

      CHECK(flags.any_of(F1, F3, F5, F7) == false);
      CHECK((flags.any_of<F1, F3, F5, F7>()) == false);

      CHECK(flags.any_of(F0, F1, F2, F3, F4, F5, F6, F7) == true);
      CHECK((flags.any_of<F0, F1, F2, F3, F4, F5, F6, F7>()) == true);
    }

    //*************************************************************************
    TEST(test_none)
    {
      Flags flags1(F_NONE);
      Flags flags2(0x08);

      CHECK(flags1.none() == true);
      CHECK(flags2.none() == false);
    }

    //*************************************************************************
    TEST(test_none_masked)
    {
      FlagsM0246 flags1(F0246);
      FlagsM0246 flags2(F1357);

      CHECK(flags1.none() == false);
      CHECK(flags2.none() == true);
    }

    //*************************************************************************
    TEST(test_none_of)
    {
      Flags flags1(F_NONE);
      Flags flags2(F0246);

      CHECK(flags1.none_of(F_NONE) == true);
      CHECK(flags1.none_of(F01234567) == true);
      CHECK(flags1.none_of(F0, F1, F2, F3, F4, F5, F6, F7) == true);
      CHECK(flags1.none_of<F01234567>() == true);
      CHECK((flags1.none_of<F0, F1, F2, F3, F4, F5, F6, F7>() == true));

      CHECK(flags2.none_of(F01234567) == false);
      CHECK(flags2.none_of<F01234567>() == false);
      CHECK(flags2.none_of(F0246) == false);
      CHECK(flags2.none_of<F0246>() == false);
      CHECK(flags2.none_of(F1357) == true);
      CHECK(flags2.none_of<F1357>() == true);
      CHECK(flags2.none_of(F01234567) == false);
      CHECK(flags2.none_of<F01234567>() == false);
      CHECK(flags2.none_of(F0, F1, F2, F3, F4, F5, F6, F7) == false);
      CHECK((flags2.none_of<F0, F1, F2, F3, F4, F5, F6, F7>() == false));
    }

    //*************************************************************************
    TEST(test_none_of_masked)
    {
      FlagsM0246 flags1(F_NONE);
      FlagsM0246 flags2(F01234567);

      CHECK(flags1.none_of(F_NONE) == true);
      CHECK(flags1.none_of(F01234567) == true);
      CHECK(flags1.none_of(F0, F1, F2, F3, F4, F5, F6, F7) == true);
      CHECK(flags1.none_of<F01234567>() == true);
      CHECK((flags1.none_of<F0, F1, F2, F3, F4, F5, F6, F7>() == true));

      CHECK(flags2.none_of(F01234567) == false);
      CHECK(flags2.none_of<F01234567>() == false);
      CHECK(flags2.none_of(F0246) == false);
      CHECK(flags2.none_of<F0246>() == false);
      CHECK(flags2.none_of(F1357) == true);
      CHECK(flags2.none_of<F1357>() == true);
      CHECK(flags2.none_of(F01234567) == false);
      CHECK(flags2.none_of<F01234567>() == false);
      CHECK(flags2.none_of(F0, F1, F2, F3, F4, F5, F6, F7) == false);
      CHECK((flags2.none_of<F0, F1, F2, F3, F4, F5, F6, F7>() == false));
    }


    //*************************************************************************
    TEST(test_flip)
    {
      Flags flags1(F0246);
      Flags flags2(F0246);
      Flags flags3(F0246);
      Flags flags4(F0246);

      CHECK_EQUAL(F4 | F6, flags1.flip(F0, F2).value());
      CHECK_EQUAL(F4 | F6, (flags2.flip<F0, F2>().value()));

      CHECK_EQUAL(F1357, flags3.flip(F01234567).value());
      CHECK_EQUAL(F1357, (flags4.flip<F01234567>().value()));
    }

    //*************************************************************************
    TEST(test_flip_masked)
    {
      FlagsM0246 flags1(F0246);
      FlagsM0246 flags2(F0246);
      FlagsM0246 flags3(F0246);
      FlagsM0246 flags4(F0246);

      CHECK_EQUAL(int(F4 | F6), flags1.flip(F0, F2).value());
      CHECK_EQUAL(int(F4 | F6), (flags2.flip<F0, F2>().value()));

      CHECK_EQUAL(int(F_NONE), flags3.flip(F01234567).value());
      CHECK_EQUAL(int(F_NONE), (flags4.flip<F01234567>().value()));
    }
  };
}
