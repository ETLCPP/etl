/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2025 John Wellbelove

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

//***************************************************************************
// This file is included in variant_variadic.h for C++11 and C++14, as they
// do not support template fold expressions.
//***************************************************************************

namespace private_variant
{
  //***************************************************************************
  // Selects a do_visitor inplementation that is configured for the number of types.
  //***************************************************************************
  template <size_t NTypes>
  struct select_do_visitor;

  //***************************************************************************
  template <>
  struct select_do_visitor<1>
  {
    template <typename TVariant, typename TVisitor>
    static void do_visitor(TVariant& the_variant, TVisitor& visitor)
    {
      switch (the_variant.index())
      {
        case 0: { visitor.visit(etl::get<0>(the_variant)); break; }
        default: break;
      }
    }
  };

  //***************************************************************************
  template <>
  struct select_do_visitor<2>
  {
    template <typename TVariant, typename TVisitor>
    static void do_visitor(TVariant& the_variant, TVisitor& visitor)
    {
      switch (the_variant.index())
      {
        case 0: { visitor.visit(etl::get<0>(the_variant)); break; }
        case 1: { visitor.visit(etl::get<1>(the_variant)); break; }
        default: break;
      }
    }
  };

  //***************************************************************************
  template <>
  struct select_do_visitor<3>
  {
    template <typename TVariant, typename TVisitor>
    static void do_visitor(TVariant& the_variant, TVisitor& visitor)
    {
      switch (the_variant.index())
      {
        case 0: { visitor.visit(etl::get<0>(the_variant)); break; }
        case 1: { visitor.visit(etl::get<1>(the_variant)); break; }
        case 2: { visitor.visit(etl::get<2>(the_variant)); break; }
        default: break;
      }
    };
  };

  //***************************************************************************
  template <>
  struct select_do_visitor<4>
  {
    template <typename TVariant, typename TVisitor>
    static void do_visitor(TVariant& the_variant, TVisitor& visitor)
    {
      switch (the_variant.index())
      {
        case 0: { visitor.visit(etl::get<0>(the_variant)); break; }
        case 1: { visitor.visit(etl::get<1>(the_variant)); break; }
        case 2: { visitor.visit(etl::get<2>(the_variant)); break; }
        case 3: { visitor.visit(etl::get<3>(the_variant)); break; }
        default: break;
      }
    };
  };

  //***************************************************************************
  template <>
  struct select_do_visitor<5>
  {
    template <typename TVariant, typename TVisitor>
    static void do_visitor(TVariant& the_variant, TVisitor& visitor)
    {
      switch (the_variant.index())
      {
        case 0: { visitor.visit(etl::get<0>(the_variant)); break; }
        case 1: { visitor.visit(etl::get<1>(the_variant)); break; }
        case 2: { visitor.visit(etl::get<2>(the_variant)); break; }
        case 3: { visitor.visit(etl::get<3>(the_variant)); break; }
        case 4: { visitor.visit(etl::get<4>(the_variant)); break; }
        default: break;
      }
    };
  };

  //***************************************************************************
  template <>
  struct select_do_visitor<6>
  {
    template <typename TVariant, typename TVisitor>
    static void do_visitor(TVariant& the_variant, TVisitor& visitor)
    {
      switch (the_variant.index())
      {
        case 0: { visitor.visit(etl::get<0>(the_variant)); break; }
        case 1: { visitor.visit(etl::get<1>(the_variant)); break; }
        case 2: { visitor.visit(etl::get<2>(the_variant)); break; }
        case 3: { visitor.visit(etl::get<3>(the_variant)); break; }
        case 4: { visitor.visit(etl::get<4>(the_variant)); break; }
        case 5: { visitor.visit(etl::get<5>(the_variant)); break; }
        default: break;
      }
    };
  };

  //***************************************************************************
  template <>
  struct select_do_visitor<7>
  {
    template <typename TVariant, typename TVisitor>
    static void do_visitor(TVariant& the_variant, TVisitor& visitor)
    {
      switch (the_variant.index())
      {
        case 0: { visitor.visit(etl::get<0>(the_variant)); break; }
        case 1: { visitor.visit(etl::get<1>(the_variant)); break; }
        case 2: { visitor.visit(etl::get<2>(the_variant)); break; }
        case 3: { visitor.visit(etl::get<3>(the_variant)); break; }
        case 4: { visitor.visit(etl::get<4>(the_variant)); break; }
        case 5: { visitor.visit(etl::get<5>(the_variant)); break; }
        case 6: { visitor.visit(etl::get<6>(the_variant)); break; }
        default: break;
      }
    };
  };

  //***************************************************************************
  template <>
  struct select_do_visitor<8>
  {
    template <typename TVariant, typename TVisitor>
    static void do_visitor(TVariant& the_variant, TVisitor& visitor)
    {
      switch (the_variant.index())
      {
        case 0: { visitor.visit(etl::get<0>(the_variant)); break; }
        case 1: { visitor.visit(etl::get<1>(the_variant)); break; }
        case 2: { visitor.visit(etl::get<2>(the_variant)); break; }
        case 3: { visitor.visit(etl::get<3>(the_variant)); break; }
        case 4: { visitor.visit(etl::get<4>(the_variant)); break; }
        case 5: { visitor.visit(etl::get<5>(the_variant)); break; }
        case 6: { visitor.visit(etl::get<6>(the_variant)); break; }
        case 7: { visitor.visit(etl::get<7>(the_variant)); break; }
        default: break;
      }
    };
  };
#if !defined(ETL_VARIANT_CPP11_MAX_8_TYPES)
  //***************************************************************************
  template <>
  struct select_do_visitor<9>
  {
    template <typename TVariant, typename TVisitor>
    static void do_visitor(TVariant& the_variant, TVisitor& visitor)
    {
      switch (the_variant.index())
      {
        case 0: { visitor.visit(etl::get<0>(the_variant)); break; }
        case 1: { visitor.visit(etl::get<1>(the_variant)); break; }
        case 2: { visitor.visit(etl::get<2>(the_variant)); break; }
        case 3: { visitor.visit(etl::get<3>(the_variant)); break; }
        case 4: { visitor.visit(etl::get<4>(the_variant)); break; }
        case 5: { visitor.visit(etl::get<5>(the_variant)); break; }
        case 6: { visitor.visit(etl::get<6>(the_variant)); break; }
        case 7: { visitor.visit(etl::get<7>(the_variant)); break; }
        case 8: { visitor.visit(etl::get<8>(the_variant)); break; }
        default: break;
      }
    };
  };

  //***************************************************************************
  template <>
  struct select_do_visitor<10>
  {
    template <typename TVariant, typename TVisitor>
    static void do_visitor(TVariant& the_variant, TVisitor& visitor)
    {
      switch (the_variant.index())
      {
        case 0: { visitor.visit(etl::get<0>(the_variant)); break; }
        case 1: { visitor.visit(etl::get<1>(the_variant)); break; }
        case 2: { visitor.visit(etl::get<2>(the_variant)); break; }
        case 3: { visitor.visit(etl::get<3>(the_variant)); break; }
        case 4: { visitor.visit(etl::get<4>(the_variant)); break; }
        case 5: { visitor.visit(etl::get<5>(the_variant)); break; }
        case 6: { visitor.visit(etl::get<6>(the_variant)); break; }
        case 7: { visitor.visit(etl::get<7>(the_variant)); break; }
        case 8: { visitor.visit(etl::get<8>(the_variant)); break; }
        case 9: { visitor.visit(etl::get<9>(the_variant)); break; }
        default: break;
      }
    };
  };

  //***************************************************************************
  template <>
  struct select_do_visitor<11>
  {
    template <typename TVariant, typename TVisitor>
    static void do_visitor(TVariant& the_variant, TVisitor& visitor)
    {
      switch (the_variant.index())
      {
        case 0:  { visitor.visit(etl::get<0>(the_variant)); break; }
        case 1:  { visitor.visit(etl::get<1>(the_variant)); break; }
        case 2:  { visitor.visit(etl::get<2>(the_variant)); break; }
        case 3:  { visitor.visit(etl::get<3>(the_variant)); break; }
        case 4:  { visitor.visit(etl::get<4>(the_variant)); break; }
        case 5:  { visitor.visit(etl::get<5>(the_variant)); break; }
        case 6:  { visitor.visit(etl::get<6>(the_variant)); break; }
        case 7:  { visitor.visit(etl::get<7>(the_variant)); break; }
        case 8:  { visitor.visit(etl::get<8>(the_variant)); break; }
        case 9:  { visitor.visit(etl::get<9>(the_variant)); break; }
        case 10: { visitor.visit(etl::get<10>(the_variant)); break; }
        default: break;
      }
    };
  };

  //***************************************************************************
  template <>
  struct select_do_visitor<12>
  {
    template <typename TVariant, typename TVisitor>
    static void do_visitor(TVariant& the_variant, TVisitor& visitor)
    {
      switch (the_variant.index())
      {
        case 0:  { visitor.visit(etl::get<0>(the_variant)); break; }
        case 1:  { visitor.visit(etl::get<1>(the_variant)); break; }
        case 2:  { visitor.visit(etl::get<2>(the_variant)); break; }
        case 3:  { visitor.visit(etl::get<3>(the_variant)); break; }
        case 4:  { visitor.visit(etl::get<4>(the_variant)); break; }
        case 5:  { visitor.visit(etl::get<5>(the_variant)); break; }
        case 6:  { visitor.visit(etl::get<6>(the_variant)); break; }
        case 7:  { visitor.visit(etl::get<7>(the_variant)); break; }
        case 8:  { visitor.visit(etl::get<8>(the_variant)); break; }
        case 9:  { visitor.visit(etl::get<9>(the_variant)); break; }
        case 10: { visitor.visit(etl::get<10>(the_variant)); break; }
        case 11: { visitor.visit(etl::get<11>(the_variant)); break; }
        default: break;
      }
    };
  };

  //***************************************************************************
  template <>
  struct select_do_visitor<13>
  {
    template <typename TVariant, typename TVisitor>
    static void do_visitor(TVariant& the_variant, TVisitor& visitor)
    {
      switch (the_variant.index())
      {
        case 0:  { visitor.visit(etl::get<0>(the_variant)); break; }
        case 1:  { visitor.visit(etl::get<1>(the_variant)); break; }
        case 2:  { visitor.visit(etl::get<2>(the_variant)); break; }
        case 3:  { visitor.visit(etl::get<3>(the_variant)); break; }
        case 4:  { visitor.visit(etl::get<4>(the_variant)); break; }
        case 5:  { visitor.visit(etl::get<5>(the_variant)); break; }
        case 6:  { visitor.visit(etl::get<6>(the_variant)); break; }
        case 7:  { visitor.visit(etl::get<7>(the_variant)); break; }
        case 8:  { visitor.visit(etl::get<8>(the_variant)); break; }
        case 9:  { visitor.visit(etl::get<9>(the_variant)); break; }
        case 10: { visitor.visit(etl::get<10>(the_variant)); break; }
        case 11: { visitor.visit(etl::get<11>(the_variant)); break; }
        case 12: { visitor.visit(etl::get<12>(the_variant)); break; }
        default: break;
      }
    };
  };

  //***************************************************************************
  template <>
  struct select_do_visitor<14>
  {
    template <typename TVariant, typename TVisitor>
    static void do_visitor(TVariant& the_variant, TVisitor& visitor)
    {
      switch (the_variant.index())
      {
        case 0:  { visitor.visit(etl::get<0>(the_variant)); break; }
        case 1:  { visitor.visit(etl::get<1>(the_variant)); break; }
        case 2:  { visitor.visit(etl::get<2>(the_variant)); break; }
        case 3:  { visitor.visit(etl::get<3>(the_variant)); break; }
        case 4:  { visitor.visit(etl::get<4>(the_variant)); break; }
        case 5:  { visitor.visit(etl::get<5>(the_variant)); break; }
        case 6:  { visitor.visit(etl::get<6>(the_variant)); break; }
        case 7:  { visitor.visit(etl::get<7>(the_variant)); break; }
        case 8:  { visitor.visit(etl::get<8>(the_variant)); break; }
        case 9:  { visitor.visit(etl::get<9>(the_variant)); break; }
        case 10: { visitor.visit(etl::get<10>(the_variant)); break; }
        case 11: { visitor.visit(etl::get<11>(the_variant)); break; }
        case 12: { visitor.visit(etl::get<12>(the_variant)); break; }
        case 13: { visitor.visit(etl::get<13>(the_variant)); break; }
        default: break;
      }
    };
  };

  //***************************************************************************
  template <>
  struct select_do_visitor<15>
  {
    template <typename TVariant, typename TVisitor>
    static void do_visitor(TVariant& the_variant, TVisitor& visitor)
    {
      switch (the_variant.index())
      {
        case 0:  { visitor.visit(etl::get<0>(the_variant)); break; }
        case 1:  { visitor.visit(etl::get<1>(the_variant)); break; }
        case 2:  { visitor.visit(etl::get<2>(the_variant)); break; }
        case 3:  { visitor.visit(etl::get<3>(the_variant)); break; }
        case 4:  { visitor.visit(etl::get<4>(the_variant)); break; }
        case 5:  { visitor.visit(etl::get<5>(the_variant)); break; }
        case 6:  { visitor.visit(etl::get<6>(the_variant)); break; }
        case 7:  { visitor.visit(etl::get<7>(the_variant)); break; }
        case 8:  { visitor.visit(etl::get<8>(the_variant)); break; }
        case 9:  { visitor.visit(etl::get<9>(the_variant)); break; }
        case 10: { visitor.visit(etl::get<10>(the_variant)); break; }
        case 11: { visitor.visit(etl::get<11>(the_variant)); break; }
        case 12: { visitor.visit(etl::get<12>(the_variant)); break; }
        case 13: { visitor.visit(etl::get<13>(the_variant)); break; }
        case 14: { visitor.visit(etl::get<14>(the_variant)); break; }
        default: break;
      }
    };
  };

  //***************************************************************************
  template <>
  struct select_do_visitor<16>
  {
    template <typename TVariant, typename TVisitor>
    static void do_visitor(TVariant& the_variant, TVisitor& visitor)
    {
      switch (the_variant.index())
      {
        case 0:  { visitor.visit(etl::get<0>(the_variant)); break; }
        case 1:  { visitor.visit(etl::get<1>(the_variant)); break; }
        case 2:  { visitor.visit(etl::get<2>(the_variant)); break; }
        case 3:  { visitor.visit(etl::get<3>(the_variant)); break; }
        case 4:  { visitor.visit(etl::get<4>(the_variant)); break; }
        case 5:  { visitor.visit(etl::get<5>(the_variant)); break; }
        case 6:  { visitor.visit(etl::get<6>(the_variant)); break; }
        case 7:  { visitor.visit(etl::get<7>(the_variant)); break; }
        case 8:  { visitor.visit(etl::get<8>(the_variant)); break; }
        case 9:  { visitor.visit(etl::get<9>(the_variant)); break; }
        case 10: { visitor.visit(etl::get<10>(the_variant)); break; }
        case 11: { visitor.visit(etl::get<11>(the_variant)); break; }
        case 12: { visitor.visit(etl::get<12>(the_variant)); break; }
        case 13: { visitor.visit(etl::get<13>(the_variant)); break; }
        case 14: { visitor.visit(etl::get<14>(the_variant)); break; }
        case 15: { visitor.visit(etl::get<15>(the_variant)); break; }
        default: break;
      }
    };
  };
#if !defined(ETL_VARIANT_CPP11_MAX_16_TYPES)
  //***************************************************************************
  template <>
  struct select_do_visitor<17>
  {
    template <typename TVariant, typename TVisitor>
    static void do_visitor(TVariant& the_variant, TVisitor& visitor)
    {
      switch (the_variant.index())
      {
        case 0:  { visitor.visit(etl::get<0>(the_variant)); break; }
        case 1:  { visitor.visit(etl::get<1>(the_variant)); break; }
        case 2:  { visitor.visit(etl::get<2>(the_variant)); break; }
        case 3:  { visitor.visit(etl::get<3>(the_variant)); break; }
        case 4:  { visitor.visit(etl::get<4>(the_variant)); break; }
        case 5:  { visitor.visit(etl::get<5>(the_variant)); break; }
        case 6:  { visitor.visit(etl::get<6>(the_variant)); break; }
        case 7:  { visitor.visit(etl::get<7>(the_variant)); break; }
        case 8:  { visitor.visit(etl::get<8>(the_variant)); break; }
        case 9:  { visitor.visit(etl::get<9>(the_variant)); break; }
        case 10: { visitor.visit(etl::get<10>(the_variant)); break; }
        case 11: { visitor.visit(etl::get<11>(the_variant)); break; }
        case 12: { visitor.visit(etl::get<12>(the_variant)); break; }
        case 13: { visitor.visit(etl::get<13>(the_variant)); break; }
        case 14: { visitor.visit(etl::get<14>(the_variant)); break; }
        case 15: { visitor.visit(etl::get<15>(the_variant)); break; }
        case 16: { visitor.visit(etl::get<16>(the_variant)); break; }
        default: break;
      }
    };
  };

  //***************************************************************************
  template <>
  struct select_do_visitor<18>
  {
    template <typename TVariant, typename TVisitor>
    static void do_visitor(TVariant& the_variant, TVisitor& visitor)
    {
      switch (the_variant.index())
      {
        case 0:  { visitor.visit(etl::get<0>(the_variant)); break; }
        case 1:  { visitor.visit(etl::get<1>(the_variant)); break; }
        case 2:  { visitor.visit(etl::get<2>(the_variant)); break; }
        case 3:  { visitor.visit(etl::get<3>(the_variant)); break; }
        case 4:  { visitor.visit(etl::get<4>(the_variant)); break; }
        case 5:  { visitor.visit(etl::get<5>(the_variant)); break; }
        case 6:  { visitor.visit(etl::get<6>(the_variant)); break; }
        case 7:  { visitor.visit(etl::get<7>(the_variant)); break; }
        case 8:  { visitor.visit(etl::get<8>(the_variant)); break; }
        case 9:  { visitor.visit(etl::get<9>(the_variant)); break; }
        case 10: { visitor.visit(etl::get<10>(the_variant)); break; }
        case 11: { visitor.visit(etl::get<11>(the_variant)); break; }
        case 12: { visitor.visit(etl::get<12>(the_variant)); break; }
        case 13: { visitor.visit(etl::get<13>(the_variant)); break; }
        case 14: { visitor.visit(etl::get<14>(the_variant)); break; }
        case 15: { visitor.visit(etl::get<15>(the_variant)); break; }
        case 16: { visitor.visit(etl::get<16>(the_variant)); break; }
        case 17: { visitor.visit(etl::get<17>(the_variant)); break; }
        default: break;
      }
    };
  };

  //***************************************************************************
  template <>
  struct select_do_visitor<19>
  {
    template <typename TVariant, typename TVisitor>
    static void do_visitor(TVariant& the_variant, TVisitor& visitor)
    {
      switch (the_variant.index())
      {
        case 0:  { visitor.visit(etl::get<0>(the_variant)); break; }
        case 1:  { visitor.visit(etl::get<1>(the_variant)); break; }
        case 2:  { visitor.visit(etl::get<2>(the_variant)); break; }
        case 3:  { visitor.visit(etl::get<3>(the_variant)); break; }
        case 4:  { visitor.visit(etl::get<4>(the_variant)); break; }
        case 5:  { visitor.visit(etl::get<5>(the_variant)); break; }
        case 6:  { visitor.visit(etl::get<6>(the_variant)); break; }
        case 7:  { visitor.visit(etl::get<7>(the_variant)); break; }
        case 8:  { visitor.visit(etl::get<8>(the_variant)); break; }
        case 9:  { visitor.visit(etl::get<9>(the_variant)); break; }
        case 10: { visitor.visit(etl::get<10>(the_variant)); break; }
        case 11: { visitor.visit(etl::get<11>(the_variant)); break; }
        case 12: { visitor.visit(etl::get<12>(the_variant)); break; }
        case 13: { visitor.visit(etl::get<13>(the_variant)); break; }
        case 14: { visitor.visit(etl::get<14>(the_variant)); break; }
        case 15: { visitor.visit(etl::get<15>(the_variant)); break; }
        case 16: { visitor.visit(etl::get<16>(the_variant)); break; }
        case 17: { visitor.visit(etl::get<17>(the_variant)); break; }
        case 18: { visitor.visit(etl::get<18>(the_variant)); break; }
        default: break;
      }
    };
  };

  //***************************************************************************
  template <>
  struct select_do_visitor<20>
  {
    template <typename TVariant, typename TVisitor>
    static void do_visitor(TVariant& the_variant, TVisitor& visitor)
    {
      switch (the_variant.index())
      {
        case 0:  { visitor.visit(etl::get<0>(the_variant)); break; }
        case 1:  { visitor.visit(etl::get<1>(the_variant)); break; }
        case 2:  { visitor.visit(etl::get<2>(the_variant)); break; }
        case 3:  { visitor.visit(etl::get<3>(the_variant)); break; }
        case 4:  { visitor.visit(etl::get<4>(the_variant)); break; }
        case 5:  { visitor.visit(etl::get<5>(the_variant)); break; }
        case 6:  { visitor.visit(etl::get<6>(the_variant)); break; }
        case 7:  { visitor.visit(etl::get<7>(the_variant)); break; }
        case 8:  { visitor.visit(etl::get<8>(the_variant)); break; }
        case 9:  { visitor.visit(etl::get<9>(the_variant)); break; }
        case 10: { visitor.visit(etl::get<10>(the_variant)); break; }
        case 11: { visitor.visit(etl::get<11>(the_variant)); break; }
        case 12: { visitor.visit(etl::get<12>(the_variant)); break; }
        case 13: { visitor.visit(etl::get<13>(the_variant)); break; }
        case 14: { visitor.visit(etl::get<14>(the_variant)); break; }
        case 15: { visitor.visit(etl::get<15>(the_variant)); break; }
        case 16: { visitor.visit(etl::get<16>(the_variant)); break; }
        case 17: { visitor.visit(etl::get<17>(the_variant)); break; }
        case 18: { visitor.visit(etl::get<18>(the_variant)); break; }
        case 19: { visitor.visit(etl::get<19>(the_variant)); break; }
        default: break;
      }
    };
  };

  //***************************************************************************
  template <>
  struct select_do_visitor<21>
  {
    template <typename TVariant, typename TVisitor>
    static void do_visitor(TVariant& the_variant, TVisitor& visitor)
    {
      switch (the_variant.index())
      {
        case 0:  { visitor.visit(etl::get<0>(the_variant)); break; }
        case 1:  { visitor.visit(etl::get<1>(the_variant)); break; }
        case 2:  { visitor.visit(etl::get<2>(the_variant)); break; }
        case 3:  { visitor.visit(etl::get<3>(the_variant)); break; }
        case 4:  { visitor.visit(etl::get<4>(the_variant)); break; }
        case 5:  { visitor.visit(etl::get<5>(the_variant)); break; }
        case 6:  { visitor.visit(etl::get<6>(the_variant)); break; }
        case 7:  { visitor.visit(etl::get<7>(the_variant)); break; }
        case 8:  { visitor.visit(etl::get<8>(the_variant)); break; }
        case 9:  { visitor.visit(etl::get<9>(the_variant)); break; }
        case 10: { visitor.visit(etl::get<10>(the_variant)); break; }
        case 11: { visitor.visit(etl::get<11>(the_variant)); break; }
        case 12: { visitor.visit(etl::get<12>(the_variant)); break; }
        case 13: { visitor.visit(etl::get<13>(the_variant)); break; }
        case 14: { visitor.visit(etl::get<14>(the_variant)); break; }
        case 15: { visitor.visit(etl::get<15>(the_variant)); break; }
        case 16: { visitor.visit(etl::get<16>(the_variant)); break; }
        case 17: { visitor.visit(etl::get<17>(the_variant)); break; }
        case 18: { visitor.visit(etl::get<18>(the_variant)); break; }
        case 19: { visitor.visit(etl::get<19>(the_variant)); break; }
        case 20: { visitor.visit(etl::get<20>(the_variant)); break; }
        default: break;
      }
    };
  };

  //***************************************************************************
  template <>
  struct select_do_visitor<22>
  {
    template <typename TVariant, typename TVisitor>
    static void do_visitor(TVariant& the_variant, TVisitor& visitor)
    {
      switch (the_variant.index())
      {
        case 0:  { visitor.visit(etl::get<0>(the_variant)); break; }
        case 1:  { visitor.visit(etl::get<1>(the_variant)); break; }
        case 2:  { visitor.visit(etl::get<2>(the_variant)); break; }
        case 3:  { visitor.visit(etl::get<3>(the_variant)); break; }
        case 4:  { visitor.visit(etl::get<4>(the_variant)); break; }
        case 5:  { visitor.visit(etl::get<5>(the_variant)); break; }
        case 6:  { visitor.visit(etl::get<6>(the_variant)); break; }
        case 7:  { visitor.visit(etl::get<7>(the_variant)); break; }
        case 8:  { visitor.visit(etl::get<8>(the_variant)); break; }
        case 9:  { visitor.visit(etl::get<9>(the_variant)); break; }
        case 10: { visitor.visit(etl::get<10>(the_variant)); break; }
        case 11: { visitor.visit(etl::get<11>(the_variant)); break; }
        case 12: { visitor.visit(etl::get<12>(the_variant)); break; }
        case 13: { visitor.visit(etl::get<13>(the_variant)); break; }
        case 14: { visitor.visit(etl::get<14>(the_variant)); break; }
        case 15: { visitor.visit(etl::get<15>(the_variant)); break; }
        case 16: { visitor.visit(etl::get<16>(the_variant)); break; }
        case 17: { visitor.visit(etl::get<17>(the_variant)); break; }
        case 18: { visitor.visit(etl::get<18>(the_variant)); break; }
        case 19: { visitor.visit(etl::get<19>(the_variant)); break; }
        case 20: { visitor.visit(etl::get<20>(the_variant)); break; }
        case 21: { visitor.visit(etl::get<21>(the_variant)); break; }
        default: break;
      }
    };
  };

  //***************************************************************************
  template <>
  struct select_do_visitor<23>
  {
    template <typename TVariant, typename TVisitor>
    static void do_visitor(TVariant& the_variant, TVisitor& visitor)
    {
      switch (the_variant.index())
      {
        case 0:  { visitor.visit(etl::get<0>(the_variant)); break; }
        case 1:  { visitor.visit(etl::get<1>(the_variant)); break; }
        case 2:  { visitor.visit(etl::get<2>(the_variant)); break; }
        case 3:  { visitor.visit(etl::get<3>(the_variant)); break; }
        case 4:  { visitor.visit(etl::get<4>(the_variant)); break; }
        case 5:  { visitor.visit(etl::get<5>(the_variant)); break; }
        case 6:  { visitor.visit(etl::get<6>(the_variant)); break; }
        case 7:  { visitor.visit(etl::get<7>(the_variant)); break; }
        case 8:  { visitor.visit(etl::get<8>(the_variant)); break; }
        case 9:  { visitor.visit(etl::get<9>(the_variant)); break; }
        case 10: { visitor.visit(etl::get<10>(the_variant)); break; }
        case 11: { visitor.visit(etl::get<11>(the_variant)); break; }
        case 12: { visitor.visit(etl::get<12>(the_variant)); break; }
        case 13: { visitor.visit(etl::get<13>(the_variant)); break; }
        case 14: { visitor.visit(etl::get<14>(the_variant)); break; }
        case 15: { visitor.visit(etl::get<15>(the_variant)); break; }
        case 16: { visitor.visit(etl::get<16>(the_variant)); break; }
        case 17: { visitor.visit(etl::get<17>(the_variant)); break; }
        case 18: { visitor.visit(etl::get<18>(the_variant)); break; }
        case 19: { visitor.visit(etl::get<19>(the_variant)); break; }
        case 20: { visitor.visit(etl::get<20>(the_variant)); break; }
        case 21: { visitor.visit(etl::get<21>(the_variant)); break; }
        case 22: { visitor.visit(etl::get<22>(the_variant)); break; }
        default: break;
      }
    };
  };

  //***************************************************************************
  template <>
  struct select_do_visitor<24>
  {
    template <typename TVariant, typename TVisitor>
    static void do_visitor(TVariant& the_variant, TVisitor& visitor)
    {
      switch (the_variant.index())
      {
        case 0:  { visitor.visit(etl::get<0>(the_variant)); break; }
        case 1:  { visitor.visit(etl::get<1>(the_variant)); break; }
        case 2:  { visitor.visit(etl::get<2>(the_variant)); break; }
        case 3:  { visitor.visit(etl::get<3>(the_variant)); break; }
        case 4:  { visitor.visit(etl::get<4>(the_variant)); break; }
        case 5:  { visitor.visit(etl::get<5>(the_variant)); break; }
        case 6:  { visitor.visit(etl::get<6>(the_variant)); break; }
        case 7:  { visitor.visit(etl::get<7>(the_variant)); break; }
        case 8:  { visitor.visit(etl::get<8>(the_variant)); break; }
        case 9:  { visitor.visit(etl::get<9>(the_variant)); break; }
        case 10: { visitor.visit(etl::get<10>(the_variant)); break; }
        case 11: { visitor.visit(etl::get<11>(the_variant)); break; }
        case 12: { visitor.visit(etl::get<12>(the_variant)); break; }
        case 13: { visitor.visit(etl::get<13>(the_variant)); break; }
        case 14: { visitor.visit(etl::get<14>(the_variant)); break; }
        case 15: { visitor.visit(etl::get<15>(the_variant)); break; }
        case 16: { visitor.visit(etl::get<16>(the_variant)); break; }
        case 17: { visitor.visit(etl::get<17>(the_variant)); break; }
        case 18: { visitor.visit(etl::get<18>(the_variant)); break; }
        case 19: { visitor.visit(etl::get<19>(the_variant)); break; }
        case 20: { visitor.visit(etl::get<20>(the_variant)); break; }
        case 21: { visitor.visit(etl::get<21>(the_variant)); break; }
        case 22: { visitor.visit(etl::get<22>(the_variant)); break; }
        case 23: { visitor.visit(etl::get<23>(the_variant)); break; }
        default: break;
      }
    };
  };
#if !defined(ETL_VARIANT_CPP11_MAX_24_TYPES)
  //***************************************************************************
  template <>
  struct select_do_visitor<25>
  {
    template <typename TVariant, typename TVisitor>
    static void do_visitor(TVariant& the_variant, TVisitor& visitor)
    {
      switch (the_variant.index())
      {
        case 0:  { visitor.visit(etl::get<0>(the_variant)); break; }
        case 1:  { visitor.visit(etl::get<1>(the_variant)); break; }
        case 2:  { visitor.visit(etl::get<2>(the_variant)); break; }
        case 3:  { visitor.visit(etl::get<3>(the_variant)); break; }
        case 4:  { visitor.visit(etl::get<4>(the_variant)); break; }
        case 5:  { visitor.visit(etl::get<5>(the_variant)); break; }
        case 6:  { visitor.visit(etl::get<6>(the_variant)); break; }
        case 7:  { visitor.visit(etl::get<7>(the_variant)); break; }
        case 8:  { visitor.visit(etl::get<8>(the_variant)); break; }
        case 9:  { visitor.visit(etl::get<9>(the_variant)); break; }
        case 10: { visitor.visit(etl::get<10>(the_variant)); break; }
        case 11: { visitor.visit(etl::get<11>(the_variant)); break; }
        case 12: { visitor.visit(etl::get<12>(the_variant)); break; }
        case 13: { visitor.visit(etl::get<13>(the_variant)); break; }
        case 14: { visitor.visit(etl::get<14>(the_variant)); break; }
        case 15: { visitor.visit(etl::get<15>(the_variant)); break; }
        case 16: { visitor.visit(etl::get<16>(the_variant)); break; }
        case 17: { visitor.visit(etl::get<17>(the_variant)); break; }
        case 18: { visitor.visit(etl::get<18>(the_variant)); break; }
        case 19: { visitor.visit(etl::get<19>(the_variant)); break; }
        case 20: { visitor.visit(etl::get<20>(the_variant)); break; }
        case 21: { visitor.visit(etl::get<21>(the_variant)); break; }
        case 22: { visitor.visit(etl::get<22>(the_variant)); break; }
        case 23: { visitor.visit(etl::get<23>(the_variant)); break; }
        case 24: { visitor.visit(etl::get<24>(the_variant)); break; }
        default: break;
      }
    };
  };

  //***************************************************************************
  template <>
  struct select_do_visitor<26>
  {
    template <typename TVariant, typename TVisitor>
    static void do_visitor(TVariant& the_variant, TVisitor& visitor)
    {
      switch (the_variant.index())
      {
        case 0:  { visitor.visit(etl::get<0>(the_variant)); break; }
        case 1:  { visitor.visit(etl::get<1>(the_variant)); break; }
        case 2:  { visitor.visit(etl::get<2>(the_variant)); break; }
        case 3:  { visitor.visit(etl::get<3>(the_variant)); break; }
        case 4:  { visitor.visit(etl::get<4>(the_variant)); break; }
        case 5:  { visitor.visit(etl::get<5>(the_variant)); break; }
        case 6:  { visitor.visit(etl::get<6>(the_variant)); break; }
        case 7:  { visitor.visit(etl::get<7>(the_variant)); break; }
        case 8:  { visitor.visit(etl::get<8>(the_variant)); break; }
        case 9:  { visitor.visit(etl::get<9>(the_variant)); break; }
        case 10: { visitor.visit(etl::get<10>(the_variant)); break; }
        case 11: { visitor.visit(etl::get<11>(the_variant)); break; }
        case 12: { visitor.visit(etl::get<12>(the_variant)); break; }
        case 13: { visitor.visit(etl::get<13>(the_variant)); break; }
        case 14: { visitor.visit(etl::get<14>(the_variant)); break; }
        case 15: { visitor.visit(etl::get<15>(the_variant)); break; }
        case 16: { visitor.visit(etl::get<16>(the_variant)); break; }
        case 17: { visitor.visit(etl::get<17>(the_variant)); break; }
        case 18: { visitor.visit(etl::get<18>(the_variant)); break; }
        case 19: { visitor.visit(etl::get<19>(the_variant)); break; }
        case 20: { visitor.visit(etl::get<20>(the_variant)); break; }
        case 21: { visitor.visit(etl::get<21>(the_variant)); break; }
        case 22: { visitor.visit(etl::get<22>(the_variant)); break; }
        case 23: { visitor.visit(etl::get<23>(the_variant)); break; }
        case 24: { visitor.visit(etl::get<24>(the_variant)); break; }
        case 25: { visitor.visit(etl::get<25>(the_variant)); break; }
        default: break;
      }
    };
  };

  //***************************************************************************
  template <>
  struct select_do_visitor<27>
  {
    template <typename TVariant, typename TVisitor>
    static void do_visitor(TVariant& the_variant, TVisitor& visitor)
    {
      switch (the_variant.index())
      {
        case 0:  { visitor.visit(etl::get<0>(the_variant)); break; }
        case 1:  { visitor.visit(etl::get<1>(the_variant)); break; }
        case 2:  { visitor.visit(etl::get<2>(the_variant)); break; }
        case 3:  { visitor.visit(etl::get<3>(the_variant)); break; }
        case 4:  { visitor.visit(etl::get<4>(the_variant)); break; }
        case 5:  { visitor.visit(etl::get<5>(the_variant)); break; }
        case 6:  { visitor.visit(etl::get<6>(the_variant)); break; }
        case 7:  { visitor.visit(etl::get<7>(the_variant)); break; }
        case 8:  { visitor.visit(etl::get<8>(the_variant)); break; }
        case 9:  { visitor.visit(etl::get<9>(the_variant)); break; }
        case 10: { visitor.visit(etl::get<10>(the_variant)); break; }
        case 11: { visitor.visit(etl::get<11>(the_variant)); break; }
        case 12: { visitor.visit(etl::get<12>(the_variant)); break; }
        case 13: { visitor.visit(etl::get<13>(the_variant)); break; }
        case 14: { visitor.visit(etl::get<14>(the_variant)); break; }
        case 15: { visitor.visit(etl::get<15>(the_variant)); break; }
        case 16: { visitor.visit(etl::get<16>(the_variant)); break; }
        case 17: { visitor.visit(etl::get<17>(the_variant)); break; }
        case 18: { visitor.visit(etl::get<18>(the_variant)); break; }
        case 19: { visitor.visit(etl::get<19>(the_variant)); break; }
        case 20: { visitor.visit(etl::get<20>(the_variant)); break; }
        case 21: { visitor.visit(etl::get<21>(the_variant)); break; }
        case 22: { visitor.visit(etl::get<22>(the_variant)); break; }
        case 23: { visitor.visit(etl::get<23>(the_variant)); break; }
        case 24: { visitor.visit(etl::get<24>(the_variant)); break; }
        case 25: { visitor.visit(etl::get<25>(the_variant)); break; }
        case 26: { visitor.visit(etl::get<26>(the_variant)); break; }
        default: break;
      }
    };
  };

  //***************************************************************************
  template <>
  struct select_do_visitor<28>
  {
    template <typename TVariant, typename TVisitor>
    static void do_visitor(TVariant& the_variant, TVisitor& visitor)
    {
      switch (the_variant.index())
      {
        case 0:  { visitor.visit(etl::get<0>(the_variant)); break; }
        case 1:  { visitor.visit(etl::get<1>(the_variant)); break; }
        case 2:  { visitor.visit(etl::get<2>(the_variant)); break; }
        case 3:  { visitor.visit(etl::get<3>(the_variant)); break; }
        case 4:  { visitor.visit(etl::get<4>(the_variant)); break; }
        case 5:  { visitor.visit(etl::get<5>(the_variant)); break; }
        case 6:  { visitor.visit(etl::get<6>(the_variant)); break; }
        case 7:  { visitor.visit(etl::get<7>(the_variant)); break; }
        case 8:  { visitor.visit(etl::get<8>(the_variant)); break; }
        case 9:  { visitor.visit(etl::get<9>(the_variant)); break; }
        case 10: { visitor.visit(etl::get<10>(the_variant)); break; }
        case 11: { visitor.visit(etl::get<11>(the_variant)); break; }
        case 12: { visitor.visit(etl::get<12>(the_variant)); break; }
        case 13: { visitor.visit(etl::get<13>(the_variant)); break; }
        case 14: { visitor.visit(etl::get<14>(the_variant)); break; }
        case 15: { visitor.visit(etl::get<15>(the_variant)); break; }
        case 16: { visitor.visit(etl::get<16>(the_variant)); break; }
        case 17: { visitor.visit(etl::get<17>(the_variant)); break; }
        case 18: { visitor.visit(etl::get<18>(the_variant)); break; }
        case 19: { visitor.visit(etl::get<19>(the_variant)); break; }
        case 20: { visitor.visit(etl::get<20>(the_variant)); break; }
        case 21: { visitor.visit(etl::get<21>(the_variant)); break; }
        case 22: { visitor.visit(etl::get<22>(the_variant)); break; }
        case 23: { visitor.visit(etl::get<23>(the_variant)); break; }
        case 24: { visitor.visit(etl::get<24>(the_variant)); break; }
        case 25: { visitor.visit(etl::get<25>(the_variant)); break; }
        case 26: { visitor.visit(etl::get<26>(the_variant)); break; }
        case 27: { visitor.visit(etl::get<27>(the_variant)); break; }
        default: break;
      }
    };
  };

  //***************************************************************************
  template <>
  struct select_do_visitor<29>
  {
    template <typename TVariant, typename TVisitor>
    static void do_visitor(TVariant& the_variant, TVisitor& visitor)
    {
      switch (the_variant.index())
      {
        case 0:  { visitor.visit(etl::get<0>(the_variant)); break; }
        case 1:  { visitor.visit(etl::get<1>(the_variant)); break; }
        case 2:  { visitor.visit(etl::get<2>(the_variant)); break; }
        case 3:  { visitor.visit(etl::get<3>(the_variant)); break; }
        case 4:  { visitor.visit(etl::get<4>(the_variant)); break; }
        case 5:  { visitor.visit(etl::get<5>(the_variant)); break; }
        case 6:  { visitor.visit(etl::get<6>(the_variant)); break; }
        case 7:  { visitor.visit(etl::get<7>(the_variant)); break; }
        case 8:  { visitor.visit(etl::get<8>(the_variant)); break; }
        case 9:  { visitor.visit(etl::get<9>(the_variant)); break; }
        case 10: { visitor.visit(etl::get<10>(the_variant)); break; }
        case 11: { visitor.visit(etl::get<11>(the_variant)); break; }
        case 12: { visitor.visit(etl::get<12>(the_variant)); break; }
        case 13: { visitor.visit(etl::get<13>(the_variant)); break; }
        case 14: { visitor.visit(etl::get<14>(the_variant)); break; }
        case 15: { visitor.visit(etl::get<15>(the_variant)); break; }
        case 16: { visitor.visit(etl::get<16>(the_variant)); break; }
        case 17: { visitor.visit(etl::get<17>(the_variant)); break; }
        case 18: { visitor.visit(etl::get<18>(the_variant)); break; }
        case 19: { visitor.visit(etl::get<19>(the_variant)); break; }
        case 20: { visitor.visit(etl::get<20>(the_variant)); break; }
        case 21: { visitor.visit(etl::get<21>(the_variant)); break; }
        case 22: { visitor.visit(etl::get<22>(the_variant)); break; }
        case 23: { visitor.visit(etl::get<23>(the_variant)); break; }
        case 24: { visitor.visit(etl::get<24>(the_variant)); break; }
        case 25: { visitor.visit(etl::get<25>(the_variant)); break; }
        case 26: { visitor.visit(etl::get<26>(the_variant)); break; }
        case 27: { visitor.visit(etl::get<27>(the_variant)); break; }
        case 28: { visitor.visit(etl::get<28>(the_variant)); break; }
        default: break;
      }
    };
  };

  //***************************************************************************
  template <>
  struct select_do_visitor<30>
  {
    template <typename TVariant, typename TVisitor>
    static void do_visitor(TVariant& the_variant, TVisitor& visitor)
    {
      switch (the_variant.index())
      {
        case 0:  { visitor.visit(etl::get<0>(the_variant)); break; }
        case 1:  { visitor.visit(etl::get<1>(the_variant)); break; }
        case 2:  { visitor.visit(etl::get<2>(the_variant)); break; }
        case 3:  { visitor.visit(etl::get<3>(the_variant)); break; }
        case 4:  { visitor.visit(etl::get<4>(the_variant)); break; }
        case 5:  { visitor.visit(etl::get<5>(the_variant)); break; }
        case 6:  { visitor.visit(etl::get<6>(the_variant)); break; }
        case 7:  { visitor.visit(etl::get<7>(the_variant)); break; }
        case 8:  { visitor.visit(etl::get<8>(the_variant)); break; }
        case 9:  { visitor.visit(etl::get<9>(the_variant)); break; }
        case 10: { visitor.visit(etl::get<10>(the_variant)); break; }
        case 11: { visitor.visit(etl::get<11>(the_variant)); break; }
        case 12: { visitor.visit(etl::get<12>(the_variant)); break; }
        case 13: { visitor.visit(etl::get<13>(the_variant)); break; }
        case 14: { visitor.visit(etl::get<14>(the_variant)); break; }
        case 15: { visitor.visit(etl::get<15>(the_variant)); break; }
        case 16: { visitor.visit(etl::get<16>(the_variant)); break; }
        case 17: { visitor.visit(etl::get<17>(the_variant)); break; }
        case 18: { visitor.visit(etl::get<18>(the_variant)); break; }
        case 19: { visitor.visit(etl::get<19>(the_variant)); break; }
        case 20: { visitor.visit(etl::get<20>(the_variant)); break; }
        case 21: { visitor.visit(etl::get<21>(the_variant)); break; }
        case 22: { visitor.visit(etl::get<22>(the_variant)); break; }
        case 23: { visitor.visit(etl::get<23>(the_variant)); break; }
        case 24: { visitor.visit(etl::get<24>(the_variant)); break; }
        case 25: { visitor.visit(etl::get<25>(the_variant)); break; }
        case 26: { visitor.visit(etl::get<26>(the_variant)); break; }
        case 27: { visitor.visit(etl::get<27>(the_variant)); break; }
        case 28: { visitor.visit(etl::get<28>(the_variant)); break; }
        case 29: { visitor.visit(etl::get<29>(the_variant)); break; }
        default: break;
      }
    };
  };

  //***************************************************************************
  template <>
  struct select_do_visitor<31>
  {
    template <typename TVariant, typename TVisitor>
    static void do_visitor(TVariant& the_variant, TVisitor& visitor)
    {
      switch (the_variant.index())
      {
        case 0:  { visitor.visit(etl::get<0>(the_variant)); break; }
        case 1:  { visitor.visit(etl::get<1>(the_variant)); break; }
        case 2:  { visitor.visit(etl::get<2>(the_variant)); break; }
        case 3:  { visitor.visit(etl::get<3>(the_variant)); break; }
        case 4:  { visitor.visit(etl::get<4>(the_variant)); break; }
        case 5:  { visitor.visit(etl::get<5>(the_variant)); break; }
        case 6:  { visitor.visit(etl::get<6>(the_variant)); break; }
        case 7:  { visitor.visit(etl::get<7>(the_variant)); break; }
        case 8:  { visitor.visit(etl::get<8>(the_variant)); break; }
        case 9:  { visitor.visit(etl::get<9>(the_variant)); break; }
        case 10: { visitor.visit(etl::get<10>(the_variant)); break; }
        case 11: { visitor.visit(etl::get<11>(the_variant)); break; }
        case 12: { visitor.visit(etl::get<12>(the_variant)); break; }
        case 13: { visitor.visit(etl::get<13>(the_variant)); break; }
        case 14: { visitor.visit(etl::get<14>(the_variant)); break; }
        case 15: { visitor.visit(etl::get<15>(the_variant)); break; }
        case 16: { visitor.visit(etl::get<16>(the_variant)); break; }
        case 17: { visitor.visit(etl::get<17>(the_variant)); break; }
        case 18: { visitor.visit(etl::get<18>(the_variant)); break; }
        case 19: { visitor.visit(etl::get<19>(the_variant)); break; }
        case 20: { visitor.visit(etl::get<20>(the_variant)); break; }
        case 21: { visitor.visit(etl::get<21>(the_variant)); break; }
        case 22: { visitor.visit(etl::get<22>(the_variant)); break; }
        case 23: { visitor.visit(etl::get<23>(the_variant)); break; }
        case 24: { visitor.visit(etl::get<24>(the_variant)); break; }
        case 25: { visitor.visit(etl::get<25>(the_variant)); break; }
        case 26: { visitor.visit(etl::get<26>(the_variant)); break; }
        case 27: { visitor.visit(etl::get<27>(the_variant)); break; }
        case 28: { visitor.visit(etl::get<28>(the_variant)); break; }
        case 29: { visitor.visit(etl::get<29>(the_variant)); break; }
        case 30: { visitor.visit(etl::get<30>(the_variant)); break; }
        default: break;
      }
    };
  };

  //***************************************************************************
  template <>
  struct select_do_visitor<32>
  {
    template <typename TVariant, typename TVisitor>
    static void do_visitor(TVariant& the_variant, TVisitor& visitor)
    {
      switch (the_variant.index())
      {
        case 0:  { visitor.visit(etl::get<0>(the_variant)); break; }
        case 1:  { visitor.visit(etl::get<1>(the_variant)); break; }
        case 2:  { visitor.visit(etl::get<2>(the_variant)); break; }
        case 3:  { visitor.visit(etl::get<3>(the_variant)); break; }
        case 4:  { visitor.visit(etl::get<4>(the_variant)); break; }
        case 5:  { visitor.visit(etl::get<5>(the_variant)); break; }
        case 6:  { visitor.visit(etl::get<6>(the_variant)); break; }
        case 7:  { visitor.visit(etl::get<7>(the_variant)); break; }
        case 8:  { visitor.visit(etl::get<8>(the_variant)); break; }
        case 9:  { visitor.visit(etl::get<9>(the_variant)); break; }
        case 10: { visitor.visit(etl::get<10>(the_variant)); break; }
        case 11: { visitor.visit(etl::get<11>(the_variant)); break; }
        case 12: { visitor.visit(etl::get<12>(the_variant)); break; }
        case 13: { visitor.visit(etl::get<13>(the_variant)); break; }
        case 14: { visitor.visit(etl::get<14>(the_variant)); break; }
        case 15: { visitor.visit(etl::get<15>(the_variant)); break; }
        case 16: { visitor.visit(etl::get<16>(the_variant)); break; }
        case 17: { visitor.visit(etl::get<17>(the_variant)); break; }
        case 18: { visitor.visit(etl::get<18>(the_variant)); break; }
        case 19: { visitor.visit(etl::get<19>(the_variant)); break; }
        case 20: { visitor.visit(etl::get<20>(the_variant)); break; }
        case 21: { visitor.visit(etl::get<21>(the_variant)); break; }
        case 22: { visitor.visit(etl::get<22>(the_variant)); break; }
        case 23: { visitor.visit(etl::get<23>(the_variant)); break; }
        case 24: { visitor.visit(etl::get<24>(the_variant)); break; }
        case 25: { visitor.visit(etl::get<25>(the_variant)); break; }
        case 26: { visitor.visit(etl::get<26>(the_variant)); break; }
        case 27: { visitor.visit(etl::get<27>(the_variant)); break; }
        case 28: { visitor.visit(etl::get<28>(the_variant)); break; }
        case 29: { visitor.visit(etl::get<29>(the_variant)); break; }
        case 30: { visitor.visit(etl::get<30>(the_variant)); break; }
        case 31: { visitor.visit(etl::get<31>(the_variant)); break; }
        default: break;
      }
    };
  };
#endif
#endif
#endif
}
