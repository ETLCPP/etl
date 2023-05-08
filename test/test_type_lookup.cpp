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

#include "etl/type_lookup.h"

#include <type_traits>

namespace
{
  //***********************************
  template <size_t ID_>
  struct Type
  {
    enum
    {
      ID = ID_
    };
  };

  typedef Type<1>  Type1;
  typedef Type<2>  Type2;
  typedef Type<3>  Type3;
  typedef Type<4>  Type4;
  typedef Type<5>  Type5;
  typedef Type<6>  Type6;
  typedef Type<7>  Type7;
  typedef Type<8>  Type8;
  typedef Type<9>  Type9;
  typedef Type<10> Type10;
  typedef Type<11> Type11;
  typedef Type<12> Type12;
  typedef Type<13> Type13;
  typedef Type<14> Type14;
  typedef Type<15> Type15;
  typedef Type<16> Type16;
  typedef Type<17> TypeUnknown;

  typedef etl::type_id_pair<Type1,  Type1::ID>  TypeId1;
  typedef etl::type_id_pair<Type2,  Type2::ID>  TypeId2;
  typedef etl::type_id_pair<Type3,  Type3::ID>  TypeId3;
  typedef etl::type_id_pair<Type4,  Type4::ID>  TypeId4;
  typedef etl::type_id_pair<Type5,  Type5::ID>  TypeId5;
  typedef etl::type_id_pair<Type6,  Type6::ID>  TypeId6;
  typedef etl::type_id_pair<Type7,  Type7::ID>  TypeId7;
  typedef etl::type_id_pair<Type8,  Type8::ID>  TypeId8;
  typedef etl::type_id_pair<Type9,  Type9::ID>  TypeId9;
  typedef etl::type_id_pair<Type10, Type10::ID> TypeId10;
  typedef etl::type_id_pair<Type11, Type11::ID> TypeId11;
  typedef etl::type_id_pair<Type12, Type12::ID> TypeId12;
  typedef etl::type_id_pair<Type13, Type13::ID> TypeId13;
  typedef etl::type_id_pair<Type14, Type14::ID> TypeId14;
  typedef etl::type_id_pair<Type15, Type15::ID> TypeId15;
  typedef etl::type_id_pair<Type16, Type16::ID> TypeId16;

  typedef etl::type_type_pair<Type<1>,  Type<2>>  TypeType12;
  typedef etl::type_type_pair<Type<2>,  Type<1>>  TypeType21;
  typedef etl::type_type_pair<Type<3>,  Type<4>>  TypeType34;
  typedef etl::type_type_pair<Type<4>,  Type<3>>  TypeType43;
  typedef etl::type_type_pair<Type<5>,  Type<6>>  TypeType56;
  typedef etl::type_type_pair<Type<6>,  Type<5>>  TypeType65;
  typedef etl::type_type_pair<Type<7>,  Type<8>>  TypeType78;
  typedef etl::type_type_pair<Type<8>,  Type<7>>  TypeType87;
  typedef etl::type_type_pair<Type<9>,  Type<10>> TypeType910;
  typedef etl::type_type_pair<Type<10>, Type<9>>  TypeType109;
  typedef etl::type_type_pair<Type<11>, Type<12>> TypeType1112;
  typedef etl::type_type_pair<Type<12>, Type<11>> TypeType1211;
  typedef etl::type_type_pair<Type<13>, Type<14>> TypeType1314;
  typedef etl::type_type_pair<Type<14>, Type<13>> TypeType1413;
  typedef etl::type_type_pair<Type<15>, Type<16>> TypeType1516;
  typedef etl::type_type_pair<Type<16>, Type<15>> TypeType1615;

  // Type from Id / Id from Type.
  typedef etl::type_id_lookup<TypeId16, TypeId1, TypeId15, TypeId2, TypeId14, TypeId3, TypeId13, TypeId4,
                              TypeId12, TypeId5, TypeId11, TypeId6, TypeId10, TypeId7, TypeId9,  TypeId8> Type_Id_Lookup16;

  typedef etl::type_id_lookup<TypeId16, TypeId1, TypeId15, TypeId2, TypeId14, TypeId3, TypeId13, TypeId4> Type_Id_Lookup8;

  typedef etl::type_id_lookup<TypeId16> Type_Id_Lookup1;

  typedef etl::type_type_lookup<TypeType12,  TypeType21,  TypeType34,   TypeType43,   TypeType56,   TypeType65,   TypeType78,   TypeType87,
                                TypeType910, TypeType109, TypeType1112, TypeType1211, TypeType1314, TypeType1413, TypeType1516, TypeType1615> Type_Type_Lookup16;

  typedef etl::type_type_lookup<TypeType12, TypeType21, TypeType34, TypeType43, TypeType56, TypeType65, TypeType78, TypeType87> Type_Type_Lookup8;

  typedef etl::type_type_lookup<TypeType12> Type_Type_Lookup1;

  SUITE(test_type_lookup)
  {
    //*************************************************************************
    TEST(test_type_from_id_16)
    {
      CHECK((etl::is_same<Type1,  typename Type_Id_Lookup16::type_from_id<Type1::ID>::type>::value));
      CHECK((etl::is_same<Type2,  typename Type_Id_Lookup16::type_from_id<Type2::ID>::type>::value));
      CHECK((etl::is_same<Type3,  typename Type_Id_Lookup16::type_from_id<Type3::ID>::type>::value));
      CHECK((etl::is_same<Type4,  typename Type_Id_Lookup16::type_from_id<Type4::ID>::type>::value));
      CHECK((etl::is_same<Type5,  typename Type_Id_Lookup16::type_from_id<Type5::ID>::type>::value));
      CHECK((etl::is_same<Type6,  typename Type_Id_Lookup16::type_from_id<Type6::ID>::type>::value));
      CHECK((etl::is_same<Type7,  typename Type_Id_Lookup16::type_from_id<Type7::ID>::type>::value));
      CHECK((etl::is_same<Type8,  typename Type_Id_Lookup16::type_from_id<Type8::ID>::type>::value));
      CHECK((etl::is_same<Type9,  typename Type_Id_Lookup16::type_from_id<Type9::ID>::type>::value));
      CHECK((etl::is_same<Type10, typename Type_Id_Lookup16::type_from_id<Type10::ID>::type>::value));
      CHECK((etl::is_same<Type11, typename Type_Id_Lookup16::type_from_id<Type11::ID>::type>::value));
      CHECK((etl::is_same<Type12, typename Type_Id_Lookup16::type_from_id<Type12::ID>::type>::value));
      CHECK((etl::is_same<Type13, typename Type_Id_Lookup16::type_from_id<Type13::ID>::type>::value));
      CHECK((etl::is_same<Type14, typename Type_Id_Lookup16::type_from_id<Type14::ID>::type>::value));
      CHECK((etl::is_same<Type15, typename Type_Id_Lookup16::type_from_id<Type15::ID>::type>::value));
      CHECK((etl::is_same<Type16, typename Type_Id_Lookup16::type_from_id<Type16::ID>::type>::value));
    }

#if !defined(ETL_TYPE_SELECT_FORCE_CPP03_IMPLEMENTATION)
    //*************************************************************************
    TEST(test_type_from_id_t_16)
    {
      CHECK((etl::is_same<Type1, typename Type_Id_Lookup16::type_from_id_t<Type1::ID>>::value));
      CHECK((etl::is_same<Type2, typename Type_Id_Lookup16::type_from_id_t<Type2::ID>>::value));
      CHECK((etl::is_same<Type3, typename Type_Id_Lookup16::type_from_id_t<Type3::ID>>::value));
      CHECK((etl::is_same<Type4, typename Type_Id_Lookup16::type_from_id_t<Type4::ID>>::value));
      CHECK((etl::is_same<Type5, typename Type_Id_Lookup16::type_from_id_t<Type5::ID>>::value));
      CHECK((etl::is_same<Type6, typename Type_Id_Lookup16::type_from_id_t<Type6::ID>>::value));
      CHECK((etl::is_same<Type7, typename Type_Id_Lookup16::type_from_id_t<Type7::ID>>::value));
      CHECK((etl::is_same<Type8, typename Type_Id_Lookup16::type_from_id_t<Type8::ID>>::value));
      CHECK((etl::is_same<Type9, typename Type_Id_Lookup16::type_from_id_t<Type9::ID>>::value));
      CHECK((etl::is_same<Type10, typename Type_Id_Lookup16::type_from_id_t<Type10::ID>>::value));
      CHECK((etl::is_same<Type11, typename Type_Id_Lookup16::type_from_id_t<Type11::ID>>::value));
      CHECK((etl::is_same<Type12, typename Type_Id_Lookup16::type_from_id_t<Type12::ID>>::value));
      CHECK((etl::is_same<Type13, typename Type_Id_Lookup16::type_from_id_t<Type13::ID>>::value));
      CHECK((etl::is_same<Type14, typename Type_Id_Lookup16::type_from_id_t<Type14::ID>>::value));
      CHECK((etl::is_same<Type15, typename Type_Id_Lookup16::type_from_id_t<Type15::ID>>::value));
      CHECK((etl::is_same<Type16, typename Type_Id_Lookup16::type_from_id_t<Type16::ID>>::value));
    }
#endif

    //*************************************************************************
    TEST(test_type_from_id_8)
    {
      CHECK((etl::is_same<Type1,  typename Type_Id_Lookup8::type_from_id<Type1::ID>::type>::value));
      CHECK((etl::is_same<Type2,  typename Type_Id_Lookup8::type_from_id<Type2::ID>::type>::value));
      CHECK((etl::is_same<Type3,  typename Type_Id_Lookup8::type_from_id<Type3::ID>::type>::value));
      CHECK((etl::is_same<Type4,  typename Type_Id_Lookup8::type_from_id<Type4::ID>::type>::value));
      CHECK((etl::is_same<Type13, typename Type_Id_Lookup8::type_from_id<Type13::ID>::type>::value));
      CHECK((etl::is_same<Type14, typename Type_Id_Lookup8::type_from_id<Type14::ID>::type>::value));
      CHECK((etl::is_same<Type15, typename Type_Id_Lookup8::type_from_id<Type15::ID>::type>::value));
      CHECK((etl::is_same<Type16, typename Type_Id_Lookup8::type_from_id<Type16::ID>::type>::value));
    }

#if !defined(ETL_TYPE_SELECT_FORCE_CPP03_IMPLEMENTATION)
    //*************************************************************************
    TEST(test_type_from_id_t_8)
    {
      CHECK((etl::is_same<Type1, typename Type_Id_Lookup8::type_from_id_t<Type1::ID>>::value));
      CHECK((etl::is_same<Type2, typename Type_Id_Lookup8::type_from_id_t<Type2::ID>>::value));
      CHECK((etl::is_same<Type3, typename Type_Id_Lookup8::type_from_id_t<Type3::ID>>::value));
      CHECK((etl::is_same<Type4, typename Type_Id_Lookup8::type_from_id_t<Type4::ID>>::value));
      CHECK((etl::is_same<Type13, typename Type_Id_Lookup8::type_from_id_t<Type13::ID>>::value));
      CHECK((etl::is_same<Type14, typename Type_Id_Lookup8::type_from_id_t<Type14::ID>>::value));
      CHECK((etl::is_same<Type15, typename Type_Id_Lookup8::type_from_id_t<Type15::ID>>::value));
      CHECK((etl::is_same<Type16, typename Type_Id_Lookup8::type_from_id_t<Type16::ID>>::value));
    }
#endif

    //*************************************************************************
    TEST(test_type_from_id_1)
    {
      CHECK((etl::is_same<Type16, typename Type_Id_Lookup1::type_from_id<Type16::ID>::type>::value));
    }

#if !defined(ETL_TYPE_SELECT_FORCE_CPP03_IMPLEMENTATION)
    //*************************************************************************
    TEST(test_type_from_id_t_1)
    {
      CHECK((etl::is_same<Type16, typename Type_Id_Lookup1::type_from_id_t<Type16::ID>>::value));
    }
#endif

    //*************************************************************************
    TEST(test_id_from_type_16)
    {
      CHECK_EQUAL((unsigned int) Type1::ID,  (unsigned int) Type_Id_Lookup16::id_from_type<Type1>::value);
      CHECK_EQUAL((unsigned int) Type2::ID,  (unsigned int) Type_Id_Lookup16::id_from_type<Type2>::value);
      CHECK_EQUAL((unsigned int) Type3::ID,  (unsigned int) Type_Id_Lookup16::id_from_type<Type3>::value);
      CHECK_EQUAL((unsigned int) Type4::ID,  (unsigned int) Type_Id_Lookup16::id_from_type<Type4>::value);
      CHECK_EQUAL((unsigned int) Type5::ID,  (unsigned int) Type_Id_Lookup16::id_from_type<Type5>::value);
      CHECK_EQUAL((unsigned int) Type6::ID,  (unsigned int) Type_Id_Lookup16::id_from_type<Type6>::value);
      CHECK_EQUAL((unsigned int) Type7::ID,  (unsigned int) Type_Id_Lookup16::id_from_type<Type7>::value);
      CHECK_EQUAL((unsigned int) Type8::ID,  (unsigned int) Type_Id_Lookup16::id_from_type<Type8>::value);
      CHECK_EQUAL((unsigned int) Type9::ID,  (unsigned int) Type_Id_Lookup16::id_from_type<Type9>::value);
      CHECK_EQUAL((unsigned int) Type10::ID, (unsigned int) Type_Id_Lookup16::id_from_type<Type10>::value);
      CHECK_EQUAL((unsigned int) Type11::ID, (unsigned int) Type_Id_Lookup16::id_from_type<Type11>::value);
      CHECK_EQUAL((unsigned int) Type12::ID, (unsigned int) Type_Id_Lookup16::id_from_type<Type12>::value);
      CHECK_EQUAL((unsigned int) Type13::ID, (unsigned int) Type_Id_Lookup16::id_from_type<Type13>::value);
      CHECK_EQUAL((unsigned int) Type14::ID, (unsigned int) Type_Id_Lookup16::id_from_type<Type14>::value);
      CHECK_EQUAL((unsigned int) Type15::ID, (unsigned int) Type_Id_Lookup16::id_from_type<Type15>::value);
      CHECK_EQUAL((unsigned int) Type16::ID, (unsigned int) Type_Id_Lookup16::id_from_type<Type16>::value);

      CHECK_EQUAL((unsigned int)Type1::ID,  (unsigned int)Type_Id_Lookup16::get_id_from_type<Type1>());
      CHECK_EQUAL((unsigned int)Type1::ID,  (unsigned int)Type_Id_Lookup16::get_id_from_type(Type1()));
      CHECK_EQUAL((unsigned int)Type2::ID,  (unsigned int)Type_Id_Lookup16::get_id_from_type<Type2>());
      CHECK_EQUAL((unsigned int)Type2::ID,  (unsigned int)Type_Id_Lookup16::get_id_from_type(Type2()));
      CHECK_EQUAL((unsigned int)Type3::ID,  (unsigned int)Type_Id_Lookup16::get_id_from_type<Type3>());
      CHECK_EQUAL((unsigned int)Type3::ID,  (unsigned int)Type_Id_Lookup16::get_id_from_type(Type3()));
      CHECK_EQUAL((unsigned int)Type4::ID,  (unsigned int)Type_Id_Lookup16::get_id_from_type<Type4>());
      CHECK_EQUAL((unsigned int)Type4::ID,  (unsigned int)Type_Id_Lookup16::get_id_from_type(Type4()));
      CHECK_EQUAL((unsigned int)Type5::ID,  (unsigned int)Type_Id_Lookup16::get_id_from_type<Type5>());
      CHECK_EQUAL((unsigned int)Type5::ID,  (unsigned int)Type_Id_Lookup16::get_id_from_type(Type5()));
      CHECK_EQUAL((unsigned int)Type6::ID,  (unsigned int)Type_Id_Lookup16::get_id_from_type<Type6>());
      CHECK_EQUAL((unsigned int)Type6::ID,  (unsigned int)Type_Id_Lookup16::get_id_from_type(Type6()));
      CHECK_EQUAL((unsigned int)Type7::ID,  (unsigned int)Type_Id_Lookup16::get_id_from_type<Type7>());
      CHECK_EQUAL((unsigned int)Type7::ID,  (unsigned int)Type_Id_Lookup16::get_id_from_type(Type7()));
      CHECK_EQUAL((unsigned int)Type8::ID,  (unsigned int)Type_Id_Lookup16::get_id_from_type<Type8>());
      CHECK_EQUAL((unsigned int)Type8::ID,  (unsigned int)Type_Id_Lookup16::get_id_from_type(Type8()));
      CHECK_EQUAL((unsigned int)Type9::ID,  (unsigned int)Type_Id_Lookup16::get_id_from_type<Type9>());
      CHECK_EQUAL((unsigned int)Type9::ID,  (unsigned int)Type_Id_Lookup16::get_id_from_type(Type9()));
      CHECK_EQUAL((unsigned int)Type10::ID, (unsigned int)Type_Id_Lookup16::get_id_from_type<Type10>());
      CHECK_EQUAL((unsigned int)Type10::ID, (unsigned int)Type_Id_Lookup16::get_id_from_type(Type10()));
      CHECK_EQUAL((unsigned int)Type11::ID, (unsigned int)Type_Id_Lookup16::get_id_from_type<Type11>());
      CHECK_EQUAL((unsigned int)Type11::ID, (unsigned int)Type_Id_Lookup16::get_id_from_type(Type11()));
      CHECK_EQUAL((unsigned int)Type12::ID, (unsigned int)Type_Id_Lookup16::get_id_from_type<Type12>());
      CHECK_EQUAL((unsigned int)Type12::ID, (unsigned int)Type_Id_Lookup16::get_id_from_type(Type12()));
      CHECK_EQUAL((unsigned int)Type13::ID, (unsigned int)Type_Id_Lookup16::get_id_from_type<Type13>());
      CHECK_EQUAL((unsigned int)Type13::ID, (unsigned int)Type_Id_Lookup16::get_id_from_type(Type13()));
      CHECK_EQUAL((unsigned int)Type14::ID, (unsigned int)Type_Id_Lookup16::get_id_from_type<Type14>());
      CHECK_EQUAL((unsigned int)Type14::ID, (unsigned int)Type_Id_Lookup16::get_id_from_type(Type14()));
      CHECK_EQUAL((unsigned int)Type15::ID, (unsigned int)Type_Id_Lookup16::get_id_from_type<Type15>());
      CHECK_EQUAL((unsigned int)Type15::ID, (unsigned int)Type_Id_Lookup16::get_id_from_type(Type15()));
      CHECK_EQUAL((unsigned int)Type16::ID, (unsigned int)Type_Id_Lookup16::get_id_from_type<Type16>());
      CHECK_EQUAL((unsigned int)Type16::ID, (unsigned int)Type_Id_Lookup16::get_id_from_type(Type16()));
    }

#if ETL_USING_CPP17 && !defined(ETL_TYPE_SELECT_FORCE_CPP03_IMPLEMENTATION)
    //*************************************************************************
    TEST(test_id_from_type_v_16)
    {
      CHECK_EQUAL((unsigned int)Type1::ID, (unsigned int)Type_Id_Lookup16::id_from_type_v<Type1>);
      CHECK_EQUAL((unsigned int)Type2::ID, (unsigned int)Type_Id_Lookup16::id_from_type_v<Type2>);
      CHECK_EQUAL((unsigned int)Type3::ID, (unsigned int)Type_Id_Lookup16::id_from_type_v<Type3>);
      CHECK_EQUAL((unsigned int)Type4::ID, (unsigned int)Type_Id_Lookup16::id_from_type_v<Type4>);
      CHECK_EQUAL((unsigned int)Type5::ID, (unsigned int)Type_Id_Lookup16::id_from_type_v<Type5>);
      CHECK_EQUAL((unsigned int)Type6::ID, (unsigned int)Type_Id_Lookup16::id_from_type_v<Type6>);
      CHECK_EQUAL((unsigned int)Type7::ID, (unsigned int)Type_Id_Lookup16::id_from_type_v<Type7>);
      CHECK_EQUAL((unsigned int)Type8::ID, (unsigned int)Type_Id_Lookup16::id_from_type_v<Type8>);
      CHECK_EQUAL((unsigned int)Type9::ID, (unsigned int)Type_Id_Lookup16::id_from_type_v<Type9>);
      CHECK_EQUAL((unsigned int)Type10::ID, (unsigned int)Type_Id_Lookup16::id_from_type_v<Type10>);
      CHECK_EQUAL((unsigned int)Type11::ID, (unsigned int)Type_Id_Lookup16::id_from_type_v<Type11>);
      CHECK_EQUAL((unsigned int)Type12::ID, (unsigned int)Type_Id_Lookup16::id_from_type_v<Type12>);
      CHECK_EQUAL((unsigned int)Type13::ID, (unsigned int)Type_Id_Lookup16::id_from_type_v<Type13>);
      CHECK_EQUAL((unsigned int)Type14::ID, (unsigned int)Type_Id_Lookup16::id_from_type_v<Type14>);
      CHECK_EQUAL((unsigned int)Type15::ID, (unsigned int)Type_Id_Lookup16::id_from_type_v<Type15>);
      CHECK_EQUAL((unsigned int)Type16::ID, (unsigned int)Type_Id_Lookup16::id_from_type_v<Type16>);
    }
#endif

    //*************************************************************************
    TEST(test_id_from_type_8)
    {
      CHECK_EQUAL((unsigned int) Type1::ID,  (unsigned int) Type_Id_Lookup8::id_from_type<Type1>::value);
      CHECK_EQUAL((unsigned int) Type2::ID,  (unsigned int) Type_Id_Lookup8::id_from_type<Type2>::value);
      CHECK_EQUAL((unsigned int) Type3::ID,  (unsigned int) Type_Id_Lookup8::id_from_type<Type3>::value);
      CHECK_EQUAL((unsigned int) Type4::ID,  (unsigned int) Type_Id_Lookup8::id_from_type<Type4>::value);
      CHECK_EQUAL((unsigned int) Type13::ID, (unsigned int) Type_Id_Lookup8::id_from_type<Type13>::value);
      CHECK_EQUAL((unsigned int) Type14::ID, (unsigned int) Type_Id_Lookup8::id_from_type<Type14>::value);
      CHECK_EQUAL((unsigned int) Type15::ID, (unsigned int) Type_Id_Lookup8::id_from_type<Type15>::value);
      CHECK_EQUAL((unsigned int) Type16::ID, (unsigned int) Type_Id_Lookup8::id_from_type<Type16>::value);

      CHECK_EQUAL((unsigned int)Type1::ID,  (unsigned int)Type_Id_Lookup8::get_id_from_type<Type1>());
      CHECK_EQUAL((unsigned int)Type1::ID,  (unsigned int)Type_Id_Lookup8::get_id_from_type(Type1()));
      CHECK_EQUAL((unsigned int)Type2::ID,  (unsigned int)Type_Id_Lookup8::get_id_from_type<Type2>());
      CHECK_EQUAL((unsigned int)Type2::ID,  (unsigned int)Type_Id_Lookup8::get_id_from_type(Type2()));
      CHECK_EQUAL((unsigned int)Type3::ID,  (unsigned int)Type_Id_Lookup8::get_id_from_type<Type3>());
      CHECK_EQUAL((unsigned int)Type3::ID,  (unsigned int)Type_Id_Lookup8::get_id_from_type(Type3()));
      CHECK_EQUAL((unsigned int)Type4::ID,  (unsigned int)Type_Id_Lookup8::get_id_from_type<Type4>());
      CHECK_EQUAL((unsigned int)Type4::ID,  (unsigned int)Type_Id_Lookup8::get_id_from_type(Type4()));
      CHECK_EQUAL((unsigned int)Type13::ID, (unsigned int)Type_Id_Lookup8::get_id_from_type<Type13>());
      CHECK_EQUAL((unsigned int)Type13::ID, (unsigned int)Type_Id_Lookup8::get_id_from_type(Type13()));
      CHECK_EQUAL((unsigned int)Type14::ID, (unsigned int)Type_Id_Lookup8::get_id_from_type<Type14>());
      CHECK_EQUAL((unsigned int)Type14::ID, (unsigned int)Type_Id_Lookup8::get_id_from_type(Type14()));
      CHECK_EQUAL((unsigned int)Type15::ID, (unsigned int)Type_Id_Lookup8::get_id_from_type<Type15>());
      CHECK_EQUAL((unsigned int)Type15::ID, (unsigned int)Type_Id_Lookup8::get_id_from_type(Type15()));
      CHECK_EQUAL((unsigned int)Type16::ID, (unsigned int)Type_Id_Lookup8::get_id_from_type<Type16>());
      CHECK_EQUAL((unsigned int)Type16::ID, (unsigned int)Type_Id_Lookup8::get_id_from_type(Type16()));
    }

#if ETL_USING_CPP17 && !defined(ETL_TYPE_SELECT_FORCE_CPP03_IMPLEMENTATION)
    //*************************************************************************
    TEST(test_id_from_type_v_8)
    {
      CHECK_EQUAL((unsigned int)Type1::ID, (unsigned int)Type_Id_Lookup8::id_from_type_v<Type1>);
      CHECK_EQUAL((unsigned int)Type2::ID, (unsigned int)Type_Id_Lookup8::id_from_type_v<Type2>);
      CHECK_EQUAL((unsigned int)Type3::ID, (unsigned int)Type_Id_Lookup8::id_from_type_v<Type3>);
      CHECK_EQUAL((unsigned int)Type4::ID, (unsigned int)Type_Id_Lookup8::id_from_type_v<Type4>);
      CHECK_EQUAL((unsigned int)Type13::ID, (unsigned int)Type_Id_Lookup8::id_from_type_v<Type13>);
      CHECK_EQUAL((unsigned int)Type14::ID, (unsigned int)Type_Id_Lookup8::id_from_type_v<Type14>);
      CHECK_EQUAL((unsigned int)Type15::ID, (unsigned int)Type_Id_Lookup8::id_from_type_v<Type15>);
      CHECK_EQUAL((unsigned int)Type16::ID, (unsigned int)Type_Id_Lookup8::id_from_type_v<Type16>);
    }
#endif

    //*************************************************************************
    TEST(test_id_from_type_1)
    {
      CHECK_EQUAL((unsigned int) Type16::ID, (unsigned int) Type_Id_Lookup1::id_from_type<Type16>::value);

      CHECK_EQUAL((unsigned int)Type16::ID, (unsigned int)Type_Id_Lookup1::get_id_from_type<Type16>());
      CHECK_EQUAL((unsigned int)Type16::ID, (unsigned int)Type_Id_Lookup1::get_id_from_type(Type16()));
    }

#if ETL_USING_CPP17 && !defined(ETL_TYPE_SELECT_FORCE_CPP03_IMPLEMENTATION)
    //*************************************************************************
    TEST(test_id_from_type_v_1)
    {
      CHECK_EQUAL((unsigned int)Type16::ID, (unsigned int)Type_Id_Lookup1::id_from_type_v<Type16>);
    }
#endif

    //*************************************************************************
    TEST(test_type_from_type_16)
    {
      CHECK((etl::is_same<Type2,  typename Type_Type_Lookup16::type_from_type<Type1>::type>::value));
      CHECK((etl::is_same<Type1,  typename Type_Type_Lookup16::type_from_type<Type2>::type>::value));
      CHECK((etl::is_same<Type4,  typename Type_Type_Lookup16::type_from_type<Type3>::type>::value));
      CHECK((etl::is_same<Type3,  typename Type_Type_Lookup16::type_from_type<Type4>::type>::value));
      CHECK((etl::is_same<Type6,  typename Type_Type_Lookup16::type_from_type<Type5>::type>::value));
      CHECK((etl::is_same<Type5,  typename Type_Type_Lookup16::type_from_type<Type6>::type>::value));
      CHECK((etl::is_same<Type8,  typename Type_Type_Lookup16::type_from_type<Type7>::type>::value));
      CHECK((etl::is_same<Type7,  typename Type_Type_Lookup16::type_from_type<Type8>::type>::value));
      CHECK((etl::is_same<Type10, typename Type_Type_Lookup16::type_from_type<Type9>::type>::value));
      CHECK((etl::is_same<Type9,  typename Type_Type_Lookup16::type_from_type<Type10>::type>::value));
      CHECK((etl::is_same<Type12, typename Type_Type_Lookup16::type_from_type<Type11>::type>::value));
      CHECK((etl::is_same<Type11, typename Type_Type_Lookup16::type_from_type<Type12>::type>::value));
      CHECK((etl::is_same<Type14, typename Type_Type_Lookup16::type_from_type<Type13>::type>::value));
      CHECK((etl::is_same<Type13, typename Type_Type_Lookup16::type_from_type<Type14>::type>::value));
      CHECK((etl::is_same<Type16, typename Type_Type_Lookup16::type_from_type<Type15>::type>::value));
      CHECK((etl::is_same<Type15, typename Type_Type_Lookup16::type_from_type<Type16>::type>::value));
    }

#if !defined(ETL_TYPE_SELECT_FORCE_CPP03_IMPLEMENTATION)
    //*************************************************************************
    TEST(test_type_from_type_t_16)
    {
      CHECK((etl::is_same<Type2, typename Type_Type_Lookup16::type_from_type_t<Type1>>::value));
      CHECK((etl::is_same<Type1, typename Type_Type_Lookup16::type_from_type_t<Type2>>::value));
      CHECK((etl::is_same<Type4, typename Type_Type_Lookup16::type_from_type_t<Type3>>::value));
      CHECK((etl::is_same<Type3, typename Type_Type_Lookup16::type_from_type_t<Type4>>::value));
      CHECK((etl::is_same<Type6, typename Type_Type_Lookup16::type_from_type_t<Type5>>::value));
      CHECK((etl::is_same<Type5, typename Type_Type_Lookup16::type_from_type_t<Type6>>::value));
      CHECK((etl::is_same<Type8, typename Type_Type_Lookup16::type_from_type_t<Type7>>::value));
      CHECK((etl::is_same<Type7, typename Type_Type_Lookup16::type_from_type_t<Type8>>::value));
      CHECK((etl::is_same<Type10, typename Type_Type_Lookup16::type_from_type_t<Type9>>::value));
      CHECK((etl::is_same<Type9, typename Type_Type_Lookup16::type_from_type_t<Type10>>::value));
      CHECK((etl::is_same<Type12, typename Type_Type_Lookup16::type_from_type_t<Type11>>::value));
      CHECK((etl::is_same<Type11, typename Type_Type_Lookup16::type_from_type_t<Type12>>::value));
      CHECK((etl::is_same<Type14, typename Type_Type_Lookup16::type_from_type_t<Type13>>::value));
      CHECK((etl::is_same<Type13, typename Type_Type_Lookup16::type_from_type_t<Type14>>::value));
      CHECK((etl::is_same<Type16, typename Type_Type_Lookup16::type_from_type_t<Type15>>::value));
      CHECK((etl::is_same<Type15, typename Type_Type_Lookup16::type_from_type_t<Type16>>::value));
    }
#endif

    //*************************************************************************
    TEST(test_type_from_type_8)
    {
      CHECK((etl::is_same<Type2, typename Type_Type_Lookup8::type_from_type<Type1>::type>::value));
      CHECK((etl::is_same<Type1, typename Type_Type_Lookup8::type_from_type<Type2>::type>::value));
      CHECK((etl::is_same<Type4, typename Type_Type_Lookup8::type_from_type<Type3>::type>::value));
      CHECK((etl::is_same<Type3, typename Type_Type_Lookup8::type_from_type<Type4>::type>::value));
      CHECK((etl::is_same<Type6, typename Type_Type_Lookup8::type_from_type<Type5>::type>::value));
      CHECK((etl::is_same<Type5, typename Type_Type_Lookup8::type_from_type<Type6>::type>::value));
      CHECK((etl::is_same<Type8, typename Type_Type_Lookup8::type_from_type<Type7>::type>::value));
      CHECK((etl::is_same<Type7, typename Type_Type_Lookup8::type_from_type<Type8>::type>::value));
    }

#if !defined(ETL_TYPE_SELECT_FORCE_CPP03_IMPLEMENTATION)
    //*************************************************************************
    TEST(test_type_from_type_t_8)
    {
      CHECK((etl::is_same<Type2, typename Type_Type_Lookup8::type_from_type_t<Type1>>::value));
      CHECK((etl::is_same<Type1, typename Type_Type_Lookup8::type_from_type_t<Type2>>::value));
      CHECK((etl::is_same<Type4, typename Type_Type_Lookup8::type_from_type_t<Type3>>::value));
      CHECK((etl::is_same<Type3, typename Type_Type_Lookup8::type_from_type_t<Type4>>::value));
      CHECK((etl::is_same<Type6, typename Type_Type_Lookup8::type_from_type_t<Type5>>::value));
      CHECK((etl::is_same<Type5, typename Type_Type_Lookup8::type_from_type_t<Type6>>::value));
      CHECK((etl::is_same<Type8, typename Type_Type_Lookup8::type_from_type_t<Type7>>::value));
      CHECK((etl::is_same<Type7, typename Type_Type_Lookup8::type_from_type_t<Type8>>::value));
    }
#endif

    //*************************************************************************
    TEST(test_type_from_type_1)
    {
      CHECK((etl::is_same<Type2, typename Type_Type_Lookup1::type_from_type<Type1>::type>::value));
    }

#if !defined(ETL_TYPE_SELECT_FORCE_CPP03_IMPLEMENTATION)
    //*************************************************************************
    TEST(test_type_from_type_t_1)
    {
      CHECK((etl::is_same<Type2, typename Type_Type_Lookup1::type_from_type_t<Type1>>::value));
    }
#endif
  };
}
