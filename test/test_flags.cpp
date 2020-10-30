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

    F023467   = F0 | F2 | F3 | F4 | F6 | F7,
    F0246     = F0 | F2 | F4 | F6,
    F1246     = F1 | F2 | F4 | F6,
    F246      = F2 | F4 | F6,  
    F235      = F2 | F3 | F5,
    F46       = F4 | F6,
    F13       = F1 | F3,
    F02       = F0 | F2,
    F04       = F0 | F4,
    F03       = F0 | F3,
    F1357     = F1 | F3 | F5 | F7,
    F0347     = F0 | F3 | F4 | F7,
    F1346     = F1 | F3 | F4 | F6,
    F01234567 = F0 | F1 | F2 | F3 | F4 | F5 | F6 | F7,
    FNONE     = 0
  };

  using Flags      = etl::flags<uint8_t>;
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

      CHECK_EQUAL(int(FNONE), int(flags));
      CHECK_EQUAL(int(FNONE), int(flags.value()));

      flags.value(int(F1346));
      CHECK_EQUAL(int(F1346), int(flags));
      CHECK_EQUAL(int(F1346), int(flags.value()));
    }

    //*************************************************************************
    TEST(test_value_masked)
    {
      FlagsM0246 flags1(F0246);
      FlagsM0246 flags2(F1357);
      FlagsM0246 flags3(F01234567);

      CHECK_EQUAL(int(F0246), int(flags1));
      CHECK_EQUAL(int(FNONE), int(flags2));
      CHECK_EQUAL(int(F0246), int(flags3));
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

      CHECK_EQUAL(int(F1357), int((flags1.set(F1357, true).value())));
      CHECK_EQUAL(int(F1357), int((flags2.set<F1357>(true).value())));
      CHECK_EQUAL(int(F1357), int((flags3.set<F1357, true>().value())));
    }

    //*************************************************************************
    TEST(test_set_multiple_masked)
    {
      FlagsM0246 flags1;
      FlagsM0246 flags2;
      FlagsM0246 flags3;

      CHECK_EQUAL(int(F0246), int(flags1.set(F0246)));
      CHECK_EQUAL(int(FNONE), int(flags2.set(F1357)));
      CHECK_EQUAL(int(F0246), int(flags3.set(F01234567)));

      CHECK_EQUAL(int(F0246), int(flags1.set<F0246>()));
      CHECK_EQUAL(int(FNONE), int(flags2.set<F1357>()));
      CHECK_EQUAL(int(F0246), int(flags3.set<F01234567>()));
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
      Flags flags1(F01234567);

      CHECK_EQUAL(int(Flags::value_type(0)), flags1.clear());
    }

    //*************************************************************************
    TEST(test_reset)
    {
      Flags flags1(F0246);
      Flags flags2(F0246);

      CHECK_EQUAL(int(Flags::value_type(F246)), flags1.reset(F0));
      CHECK_EQUAL(int(Flags::value_type(F246)), flags2.reset<F0>());
    }

    //*************************************************************************
    TEST(test_all)
    {
      Flags flags1(FNONE);
      Flags flags2(F1346);
      Flags flags3(F01234567);

      CHECK(flags1.all() == false);
      CHECK(flags2.all() == false);
      CHECK(flags3.all() == true);
    }

    //*************************************************************************
    TEST(test_all_masked)
    {
      FlagsM0246 flags1(FNONE);
      FlagsM0246 flags2(F1346);
      FlagsM0246 flags3(F0246);
      FlagsM0246 flags4(F01234567);

      CHECK(flags1.all() == false);
      CHECK(flags2.all() == false);
      CHECK(flags3.all() == true);
      CHECK(flags4.all() == true);
    }

    //*************************************************************************
    TEST(test_all_of)
    {
      Flags flags1(F0246);

      CHECK(flags1.all_of(F0246)     == true);
      CHECK((flags1.all_of<F0246>()) == true);

      CHECK(flags1.all_of(F246)     == true);
      CHECK((flags1.all_of<F246>()) == true);

      CHECK(flags1.all_of(F1246)     == false);
      CHECK((flags1.all_of<F1246>()) == false);

      CHECK(flags1.all_of(F01234567)     == false);
      CHECK((flags1.all_of<F01234567>()) == false);

      Flags flags2(F1357);

      CHECK(flags2.all_of(F0246)     == false);
      CHECK((flags2.all_of<F0246>()) == false);

      CHECK(flags2.all_of(F246)     == false);
      CHECK((flags2.all_of<F246>()) == false);

      CHECK(flags2.all_of(F1246)     == false);
      CHECK((flags2.all_of<F1246>()) == false);

      CHECK(flags2.all_of(F01234567)     == false);
      CHECK((flags2.all_of<F01234567>()) == false);
    }

    //*************************************************************************
    TEST(test_all_of_masked)
    {
      FlagsM0246 flags1(F0246);
      
      CHECK(flags1.all_of(F0246)     == true);
      CHECK((flags1.all_of<F0246>()) == true);

      CHECK(flags1.all_of(F246)     == true);
      CHECK((flags1.all_of<F246>()) == true);

      CHECK(flags1.all_of(F1246)     == true);
      CHECK((flags1.all_of<F1246>()) == true);

      CHECK(flags1.all_of(F01234567)     == true);
      CHECK((flags1.all_of<F01234567>()) == true);

      FlagsM0246 flags2(F1357);

      CHECK(flags2.all_of(F0246)     == false);
      CHECK((flags2.all_of<F0246>()) == false);

      CHECK(flags2.all_of(F246)     == false);
      CHECK((flags2.all_of<F246>()) == false);

      CHECK(flags2.all_of(F1246)     == false);
      CHECK((flags2.all_of<F1246>()) == false);

      CHECK(flags2.all_of(F01234567)     == false);
      CHECK((flags2.all_of<F01234567>()) == false);
    }

    //*************************************************************************
    TEST(test_any)
    {
      Flags flags1(FNONE);
      Flags flags2(F3);

      CHECK(flags1.any() == false);
      CHECK(flags2.any() == true);
    }

    //*************************************************************************
    TEST(test_any_masked)
    {
      FlagsM0246 flags1(FNONE);
      FlagsM0246 flags2(F46);
      FlagsM0246 flags3(F03);
      FlagsM0246 flags4(F13);

      CHECK(flags1.any() == false);
      CHECK(flags2.any() == true);
      CHECK(flags3.any() == true);
      CHECK(flags4.any() == false);
    }

    //*************************************************************************
    TEST(test_any_of)
    {
      Flags flags(F0246);

      CHECK(flags.any_of(F0246)     == true);
      CHECK((flags.any_of<F0246>()) == true);

      CHECK(flags.any_of(F246)     == true);
      CHECK((flags.any_of<F246>()) == true);

      CHECK(flags.any_of(F1357)     == false);
      CHECK((flags.any_of<F1357>()) == false);

      CHECK(flags.any_of(F01234567)     == true);
      CHECK((flags.any_of<F01234567>()) == true);
    }

    //*************************************************************************
    TEST(test_any_of_masked)
    {
      FlagsM0246 flags(F0246);

      CHECK(flags.any_of(F0246)     == true);
      CHECK((flags.any_of<F0246>()) == true);

      CHECK(flags.any_of(F235)     == true);
      CHECK((flags.any_of<F235>()) == true);

      CHECK(flags.any_of(F1357)     == false);
      CHECK((flags.any_of<F1357>()) == false);

      CHECK(flags.any_of(F01234567)     == true);
      CHECK((flags.any_of<F01234567>()) == true);
    }

    //*************************************************************************
    TEST(test_none)
    {
      Flags flags1(FNONE);
      Flags flags2(F3);

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
      Flags flags1(FNONE);
      Flags flags2(F0246);

      CHECK(flags1.none_of(FNONE)       == true);
      CHECK(flags1.none_of(F01234567)   == true);
      CHECK(flags1.none_of<F01234567>() == true);

      CHECK(flags2.none_of(F01234567)   == false);
      CHECK(flags2.none_of<F01234567>() == false);
      CHECK(flags2.none_of(F0246)       == false);
      CHECK(flags2.none_of<F0246>()     == false);
      CHECK(flags2.none_of(F1357)       == true);
      CHECK(flags2.none_of<F1357>()     == true);
      CHECK(flags2.none_of(F01234567)   == false);
      CHECK(flags2.none_of<F01234567>() == false);
    }

    //*************************************************************************
    TEST(test_none_of_masked)
    {
      FlagsM0246 flags1(FNONE);
      FlagsM0246 flags2(F01234567);

      CHECK(flags1.none_of(FNONE)       == true);
      CHECK(flags1.none_of(F01234567)   == true);
      CHECK(flags1.none_of<F01234567>() == true);

      CHECK(flags2.none_of(F01234567)   == false);
      CHECK(flags2.none_of<F01234567>() == false);
      CHECK(flags2.none_of(F0246)       == false);
      CHECK(flags2.none_of<F0246>()     == false);
      CHECK(flags2.none_of(F1357)       == true);
      CHECK(flags2.none_of<F1357>()     == true);
      CHECK(flags2.none_of(F01234567)   == false);
      CHECK(flags2.none_of<F01234567>() == false);
    }

    //*************************************************************************
    TEST(test_flip)
    {
      Flags flags1(F0246);
      Flags flags2(F0246);
      Flags flags3(F0246);
      Flags flags4(F0246);

      CHECK_EQUAL(F46, flags1.flip(F02).value());
      CHECK_EQUAL(F46, (flags2.flip<F02>().value()));

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

      CHECK_EQUAL(int(F46), flags1.flip(F02).value());
      CHECK_EQUAL(int(F46), (flags2.flip<F02>().value()));

      CHECK_EQUAL(int(FNONE), flags3.flip(F01234567).value());
      CHECK_EQUAL(int(FNONE), (flags4.flip<F01234567>().value()));
    }

    //*************************************************************************
    TEST(test_assignment)
		{
      Flags flags1(F0246);
      Flags flags2(F1357);
      Flags flags3(F1357);

      flags2 = flags1;
      flags3 = flags1.value();

      CHECK_EQUAL(flags1, flags2);
      CHECK_EQUAL(flags1, flags3);
    }

    //*************************************************************************
    TEST(test_assignment_masked)
    {
      FlagsM0246 flags1(F46);
      FlagsM0246 flags2(F0246);
      FlagsM0246 flags3(F0246);

      flags2 = flags1;
      flags3 = F1346;

      CHECK_EQUAL(flags1, flags2);
      CHECK_EQUAL(flags1, flags3);
    }

    //*************************************************************************
    TEST(test_and_operation)
		{
      Flags flags1(F0246);
      Flags flags2(F0246);
      Flags flags3(F0246);
      Flags flags4(F0246);

      CHECK_EQUAL(int(F0246), int(flags1 &= F0246));
      CHECK_EQUAL(int(FNONE), int(flags2 &= F1357));
      CHECK_EQUAL(int(F0246), int(flags3 &= F01234567));
      CHECK_EQUAL(int(FNONE), int(flags4 &= FNONE));
    }

    //*************************************************************************
    TEST(test_and_operation_masked)
    {
      FlagsM0246 flags1(F0246);
      FlagsM0246 flags2(F0246);
      FlagsM0246 flags3(F0246);
      FlagsM0246 flags4(F0246);

      CHECK_EQUAL(int(F0246), int(flags1 &= F0246));
      CHECK_EQUAL(int(FNONE), int(flags2 &= F1357));
      CHECK_EQUAL(int(F0246), int(flags3 &= F01234567));
      CHECK_EQUAL(int(FNONE), int(flags4 &= FNONE));
    }

    //*************************************************************************
    TEST(test_or_operation)
    {
      Flags flags1(F0246);
      Flags flags2(F0246);
      Flags flags3(F0246);
      Flags flags4(F0246);

      CHECK_EQUAL(int(F0246),     int(flags1 |= F0246));
      CHECK_EQUAL(int(F01234567), int(flags2 |= F1357));
      CHECK_EQUAL(int(F01234567), int(flags3 |= F01234567));
      CHECK_EQUAL(int(F0246),     int(flags4 |= FNONE));
    }

    //*************************************************************************
    TEST(test_or_operation_masked)
    {
      FlagsM0246 flags1(F0246);
      FlagsM0246 flags2(F0246);
      FlagsM0246 flags3(F0246);
      FlagsM0246 flags4(F0246);

      CHECK_EQUAL(int(F0246), int(flags1 |= F0246));
      CHECK_EQUAL(int(F0246), int(flags2 |= F1357));
      CHECK_EQUAL(int(F0246), int(flags3 |= F01234567));
      CHECK_EQUAL(int(F0246), int(flags4 |= FNONE));
    }

    //*************************************************************************
    TEST(test_xor_operation)
    {
      Flags flags1(F0246);
      Flags flags2(F0246);
      Flags flags3(F0246);
      Flags flags4(F0246);

      CHECK_EQUAL(int(FNONE),     int(flags1 ^= F0246));
      CHECK_EQUAL(int(F01234567), int(flags2 ^= F1357));
      CHECK_EQUAL(int(F1357),     int(flags3 ^= F01234567));
      CHECK_EQUAL(int(F0246),     int(flags4 ^= FNONE));
    }

    //*************************************************************************
    TEST(test_xor_operation_masked)
    {
      FlagsM0246 flags1(F0246);
      FlagsM0246 flags2(F0246);
      FlagsM0246 flags3(F0246);
      FlagsM0246 flags4(F0246);

      CHECK_EQUAL(int(FNONE), int(flags1 ^= F0246));
      CHECK_EQUAL(int(F0246), int(flags2 ^= F1357));
      CHECK_EQUAL(int(FNONE), int(flags3 ^= F01234567));
      CHECK_EQUAL(int(F0246), int(flags4 ^= FNONE));
    }
  };
}
