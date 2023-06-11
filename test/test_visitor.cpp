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

#include "etl/visitor.h"

//*****************************************************************************
// Pre-declare the data types.
//*****************************************************************************
class Square;
class Circle;
class Triangle;

//*****************************************************************************
// What classes do you want the visitors to handle?
// Square will be passed by reference.
// Circle will be passed by reference.
// Triangle will be passed by const reference.
//*****************************************************************************
typedef etl::visitor<Square&, Circle&, const Triangle&> DrawVisitorType;

//*****************************************************************************
// What classes do you want the visitors to handle?
// Square will be passed by reference.
// Triangle will be passed by const reference.
//*****************************************************************************
typedef etl::visitor<Square&, const Triangle&> LogVisitorType;

//*****************************************************************************
// Base shape.
//*****************************************************************************
class ShapeBase : public etl::visitable<DrawVisitorType, LogVisitorType>
{
};

//*****************************************************************************
// Square accepts draw & log visitors.
//*****************************************************************************
class Square : public ShapeBase
{
public:

  void accept(DrawVisitorType& visitor)
  {
    visitor.visit(*this);
  }

  void accept(LogVisitorType& visitor)
  {
    visitor.visit(*this);
  }
};

//*****************************************************************************
// Circle only accepts draw visitors.
//*****************************************************************************
class Circle : public ShapeBase
{
public:

  void accept(DrawVisitorType& visitor)
  {
    visitor.visit(*this);
  }

  void accept(LogVisitorType&)
  {
  }
};

//*****************************************************************************
// Triangle accepts draw & log visitors.
//*****************************************************************************
class Triangle : public ShapeBase
{
public:

  void accept(DrawVisitorType& visitor)
  {
    visitor.visit(*this);
  }

  void accept(LogVisitorType& visitor)
  {
    visitor.visit(*this);
  }
};


//*****************************************************************************
// Generic other shapes.
//*****************************************************************************
template <const int ID, typename TVisitor>
class Shape : public etl::visitable<TVisitor>
{
public:

  void accept(TVisitor& visitor)
  {
    visitor.visit(*this);
  }
};

//*****************************************************************************
class DrawVisitor : public DrawVisitorType
{
public:

  DrawVisitor()
    : square_called(false),
      circle_called(false),
      triangle_called(false)
  {
  }

	void visit(Square&)
	{
    square_called = true;
	}

	void visit(Circle&)
	{
    circle_called = true;
	}

	void visit(const Triangle&)
	{
    triangle_called = true;
	}

  bool square_called;
  bool circle_called;
  bool triangle_called;
};

//*****************************************************************************
class LogVisitor : public LogVisitorType
{
public:

  LogVisitor()
    : square_called(false),
      circle_called(false),
      triangle_called(false)
  {
  }

  void visit(Square&)
  {
    square_called = true;
  }

  // SHOULD NEVER BE CALLED.
  void visit(Circle&)
  {
    circle_called = true;
  }

  void visit(const Triangle&)
  {
    triangle_called = true;
  }

  bool square_called;
  bool circle_called;
  bool triangle_called;
};

namespace
{
  SUITE(test_visitor)
  {
    //*************************************************************************
    TEST(test_two_visitors_three_visitables)
    {
      DrawVisitor draw_visitor;
      LogVisitor  log_visitor;

      Square   square;
      Circle   circle;
      Triangle triangle;

      ShapeBase* pShapeBase;

      CHECK_EQUAL(false, draw_visitor.square_called);
      CHECK_EQUAL(false, draw_visitor.circle_called);
      CHECK_EQUAL(false, draw_visitor.triangle_called);
      CHECK_EQUAL(false, log_visitor.square_called);
      CHECK_EQUAL(false, log_visitor.circle_called);
      CHECK_EQUAL(false, log_visitor.triangle_called);

      square.accept(draw_visitor);
      square.accept(log_visitor);

      CHECK_EQUAL(true,  draw_visitor.square_called);
      CHECK_EQUAL(false, draw_visitor.circle_called);
      CHECK_EQUAL(false, draw_visitor.triangle_called);
      CHECK_EQUAL(true,  log_visitor.square_called);
      CHECK_EQUAL(false, log_visitor.circle_called);
      CHECK_EQUAL(false, log_visitor.triangle_called);

      circle.accept(draw_visitor);

      CHECK_EQUAL(true,  draw_visitor.square_called);
      CHECK_EQUAL(true,  draw_visitor.circle_called);
      CHECK_EQUAL(false, draw_visitor.triangle_called);
      CHECK_EQUAL(true,  log_visitor.square_called);
      CHECK_EQUAL(false, log_visitor.circle_called);
      CHECK_EQUAL(false, log_visitor.triangle_called);

      pShapeBase = &triangle;
      pShapeBase->accept(draw_visitor);
      pShapeBase->accept(log_visitor);

      CHECK_EQUAL(true,  draw_visitor.square_called);
      CHECK_EQUAL(true,  draw_visitor.circle_called);
      CHECK_EQUAL(true,  draw_visitor.triangle_called);
      CHECK_EQUAL(true,  log_visitor.square_called);
      CHECK_EQUAL(false, log_visitor.circle_called);
      CHECK_EQUAL(true,  log_visitor.triangle_called);
    }

    //*************************************************************************
    TEST(test_1_visitor)
    {
      class AShape;
      class ShapeVisitor1 : public etl::visitor<AShape&>
      {
        void visit(AShape&) {}
      };

      class AShape : public etl::visitable<ShapeVisitor1>
      {
      public:

        void accept(ShapeVisitor1&) {}
      };

      // This test just needs to compile without errors.
      CHECK(true);
    }

    //*************************************************************************
    TEST(test_2_visitor)
    {
      class AShape;
      class ShapeVisitor1 : public etl::visitor<AShape&>
      {
        void visit(AShape&) {}
      };

      class ShapeVisitor2 : public etl::visitor<AShape&>
      {
        void visit(AShape&) {}
      };

      class AShape : public etl::visitable<ShapeVisitor1, ShapeVisitor2>
      {
      public:

        void accept(ShapeVisitor1&) {}
        void accept(ShapeVisitor2&) {}
      };

      // This test just needs to compile without errors.
      CHECK(true);
    }

    //*************************************************************************
    TEST(test_3_visitor)
    {
      class AShape;
      class ShapeVisitor1 : public etl::visitor<AShape&>
      {
        void visit(AShape&) {}
      };

      class ShapeVisitor2 : public etl::visitor<AShape&>
      {
        void visit(AShape&) {}
      };

      class ShapeVisitor3 : public etl::visitor<AShape&>
      {
        void visit(AShape&) {}
      };

      class AShape : public etl::visitable<ShapeVisitor1, ShapeVisitor2, ShapeVisitor3>
      {
      public:

        void accept(ShapeVisitor1&) {}
        void accept(ShapeVisitor2&) {}
        void accept(ShapeVisitor3&) {}
      };

      // This test just needs to compile without errors.
      CHECK(true);
    }

    //*************************************************************************
    TEST(test_4_visitor)
    {
      class AShape;
      class ShapeVisitor1 : public etl::visitor<AShape&>
      {
        void visit(AShape&) {}
      };

      class ShapeVisitor2 : public etl::visitor<AShape&>
      {
        void visit(AShape&) {}
      };

      class ShapeVisitor3 : public etl::visitor<AShape&>
      {
        void visit(AShape&) {}
      };

      class ShapeVisitor4 : public etl::visitor<AShape&>
      {
        void visit(AShape&) {}
      };

      class AShape : public etl::visitable<ShapeVisitor1, ShapeVisitor2, ShapeVisitor3, ShapeVisitor4>
      {
      public:

        void accept(ShapeVisitor1&) {}
        void accept(ShapeVisitor2&) {}
        void accept(ShapeVisitor3&) {}
        void accept(ShapeVisitor4&) {}
      };

      // This test just needs to compile without errors.
      CHECK(true);
    }

    //*************************************************************************
    TEST(test_1_visitable)
    {
      class ShapeVisitor : public etl::visitor <Shape<1, ShapeVisitor> >
      {
      public:

        void visit(Shape<1, ShapeVisitor>) {}
      };

      // This test just needs to compile without errors.
      CHECK(true);
    }

    //*************************************************************************
    TEST(test_2_visitable)
    {
      class ShapeVisitor : public etl::visitor <Shape<1, ShapeVisitor>, Shape<2, ShapeVisitor> >
      {
      public:

        void visit(Shape<1, ShapeVisitor>) {}
        void visit(Shape<2, ShapeVisitor>) {}
      };

      // This test just needs to compile without errors.
      CHECK(true);
    }

    //*************************************************************************
    TEST(test_3_visitable)
    {
      class ShapeVisitor : public etl::visitor <Shape<1, ShapeVisitor>, Shape<2, ShapeVisitor>, Shape<3, ShapeVisitor> >
      {
      public:

        void visit(Shape<1, ShapeVisitor>) {}
        void visit(Shape<2, ShapeVisitor>) {}
        void visit(Shape<3, ShapeVisitor>) {}
      };

      // This test just needs to compile without errors.
      CHECK(true);
    }

    //*************************************************************************
    TEST(test_4_visitable)
    {
      class ShapeVisitor : public etl::visitor <Shape<1, ShapeVisitor>, Shape<2, ShapeVisitor>, Shape<3, ShapeVisitor>, Shape<4, ShapeVisitor> >
      {
      public:

        void visit(Shape<1, ShapeVisitor>) {}
        void visit(Shape<2, ShapeVisitor>) {}
        void visit(Shape<3, ShapeVisitor>) {}
        void visit(Shape<4, ShapeVisitor>) {}
      };

      // This test just needs to compile without errors.
      CHECK(true);
    }

    //*************************************************************************
    TEST(test_5_visitable)
    {
      class ShapeVisitor : public etl::visitor <Shape<1, ShapeVisitor>, Shape<2, ShapeVisitor>, Shape<3, ShapeVisitor>, Shape<4, ShapeVisitor>,
                                                Shape<5, ShapeVisitor> >
      {
      public:

        void visit(Shape<1, ShapeVisitor>) {}
        void visit(Shape<2, ShapeVisitor>) {}
        void visit(Shape<3, ShapeVisitor>) {}
        void visit(Shape<4, ShapeVisitor>) {}
        void visit(Shape<5, ShapeVisitor>) {}
      };

      // This test just needs to compile without errors.
      CHECK(true);
    }

    //*************************************************************************
    TEST(test_6_visitable)
    {
      class ShapeVisitor : public etl::visitor <Shape<1, ShapeVisitor>, Shape<2, ShapeVisitor>, Shape<3, ShapeVisitor>, Shape<4, ShapeVisitor>,
                                                Shape<5, ShapeVisitor>, Shape<6, ShapeVisitor> >
      {
      public:

        void visit(Shape<1, ShapeVisitor>) {}
        void visit(Shape<2, ShapeVisitor>) {}
        void visit(Shape<3, ShapeVisitor>) {}
        void visit(Shape<4, ShapeVisitor>) {}
        void visit(Shape<5, ShapeVisitor>) {}
        void visit(Shape<6, ShapeVisitor>) {}
      };

      // This test just needs to compile without errors.
      CHECK(true);
    }

    //*************************************************************************
    TEST(test_7_visitable)
    {
      class ShapeVisitor : public etl::visitor <Shape<1, ShapeVisitor>, Shape<2, ShapeVisitor>, Shape<3, ShapeVisitor>, Shape<4, ShapeVisitor>,
                                                Shape<5, ShapeVisitor>, Shape<6, ShapeVisitor>, Shape<7, ShapeVisitor> >
      {
      public:

        void visit(Shape<1, ShapeVisitor>) {}
        void visit(Shape<2, ShapeVisitor>) {}
        void visit(Shape<3, ShapeVisitor>) {}
        void visit(Shape<4, ShapeVisitor>) {}
        void visit(Shape<5, ShapeVisitor>) {}
        void visit(Shape<6, ShapeVisitor>) {}
        void visit(Shape<7, ShapeVisitor>) {}
      };

      // This test just needs to compile without errors.
      CHECK(true);
    }

    //*************************************************************************
    TEST(test_8_visitable)
    {
      class ShapeVisitor : public etl::visitor <Shape<1, ShapeVisitor>, Shape<2, ShapeVisitor>, Shape<3, ShapeVisitor>, Shape<4, ShapeVisitor>,
                                                Shape<5, ShapeVisitor>, Shape<6, ShapeVisitor>, Shape<7, ShapeVisitor>, Shape<8, ShapeVisitor> >
      {
      public:

        void visit(Shape<1, ShapeVisitor>) {}
        void visit(Shape<2, ShapeVisitor>) {}
        void visit(Shape<3, ShapeVisitor>) {}
        void visit(Shape<4, ShapeVisitor>) {}
        void visit(Shape<5, ShapeVisitor>) {}
        void visit(Shape<6, ShapeVisitor>) {}
        void visit(Shape<7, ShapeVisitor>) {}
        void visit(Shape<8, ShapeVisitor>) {}
      };

      // This test just needs to compile without errors.
      CHECK(true);
    }

    //*************************************************************************
    TEST(test_9_visitable)
    {
      class ShapeVisitor : public etl::visitor <Shape<1, ShapeVisitor>, Shape<2, ShapeVisitor>, Shape<3, ShapeVisitor>, Shape<4, ShapeVisitor>,
                                                Shape<5, ShapeVisitor>, Shape<6, ShapeVisitor>, Shape<7, ShapeVisitor>, Shape<8, ShapeVisitor>,
                                                Shape<9, ShapeVisitor> >
      {
      public:

        void visit(Shape<1, ShapeVisitor>) {}
        void visit(Shape<2, ShapeVisitor>) {}
        void visit(Shape<3, ShapeVisitor>) {}
        void visit(Shape<4, ShapeVisitor>) {}
        void visit(Shape<5, ShapeVisitor>) {}
        void visit(Shape<6, ShapeVisitor>) {}
        void visit(Shape<7, ShapeVisitor>) {}
        void visit(Shape<8, ShapeVisitor>) {}
        void visit(Shape<9, ShapeVisitor>) {}
      };

      // This test just needs to compile without errors.
      CHECK(true);
    }

    //*************************************************************************
    TEST(test_10_visitable)
    {
      class ShapeVisitor : public etl::visitor <Shape<1, ShapeVisitor>, Shape<2, ShapeVisitor>, Shape<3, ShapeVisitor>, Shape<4, ShapeVisitor>,
                                                Shape<5, ShapeVisitor>, Shape<6, ShapeVisitor>, Shape<7, ShapeVisitor>, Shape<8, ShapeVisitor>,
                                                Shape<9, ShapeVisitor>, Shape<10, ShapeVisitor> >
      {
      public:

        void visit(Shape<1, ShapeVisitor>) {}
        void visit(Shape<2, ShapeVisitor>) {}
        void visit(Shape<3, ShapeVisitor>) {}
        void visit(Shape<4, ShapeVisitor>) {}
        void visit(Shape<5, ShapeVisitor>) {}
        void visit(Shape<6, ShapeVisitor>) {}
        void visit(Shape<7, ShapeVisitor>) {}
        void visit(Shape<8, ShapeVisitor>) {}
        void visit(Shape<9, ShapeVisitor>) {}
        void visit(Shape<10, ShapeVisitor>) {}
      };

      // This test just needs to compile without errors.
      CHECK(true);
    }


    //*************************************************************************
    TEST(test_11_visitable)
    {
      class ShapeVisitor : public etl::visitor <Shape<1, ShapeVisitor>, Shape<2, ShapeVisitor>, Shape<3, ShapeVisitor>, Shape<4, ShapeVisitor>,
                                                Shape<5, ShapeVisitor>, Shape<6, ShapeVisitor>, Shape<7, ShapeVisitor>, Shape<8, ShapeVisitor>,
                                                Shape<9, ShapeVisitor>, Shape<10, ShapeVisitor>, Shape<11, ShapeVisitor> >
      {
      public:

        void visit(Shape<1, ShapeVisitor>) {}
        void visit(Shape<2, ShapeVisitor>) {}
        void visit(Shape<3, ShapeVisitor>) {}
        void visit(Shape<4, ShapeVisitor>) {}
        void visit(Shape<5, ShapeVisitor>) {}
        void visit(Shape<6, ShapeVisitor>) {}
        void visit(Shape<7, ShapeVisitor>) {}
        void visit(Shape<8, ShapeVisitor>) {}
        void visit(Shape<9, ShapeVisitor>) {}
        void visit(Shape<10, ShapeVisitor>) {}
        void visit(Shape<11, ShapeVisitor>) {}
      };

      // This test just needs to compile without errors.
      CHECK(true);
    }

    //*************************************************************************
    TEST(test_12_visitable)
    {
      class ShapeVisitor : public etl::visitor <Shape<1, ShapeVisitor>, Shape<2, ShapeVisitor>, Shape<3, ShapeVisitor>, Shape<4, ShapeVisitor>,
                                                Shape<5, ShapeVisitor>, Shape<6, ShapeVisitor>, Shape<7, ShapeVisitor>, Shape<8, ShapeVisitor>,
                                                Shape<9, ShapeVisitor>, Shape<10, ShapeVisitor>, Shape<11, ShapeVisitor>, Shape<12, ShapeVisitor> >
      {
      public:

        void visit(Shape<1, ShapeVisitor>) {}
        void visit(Shape<2, ShapeVisitor>) {}
        void visit(Shape<3, ShapeVisitor>) {}
        void visit(Shape<4, ShapeVisitor>) {}
        void visit(Shape<5, ShapeVisitor>) {}
        void visit(Shape<6, ShapeVisitor>) {}
        void visit(Shape<7, ShapeVisitor>) {}
        void visit(Shape<8, ShapeVisitor>) {}
        void visit(Shape<9, ShapeVisitor>) {}
        void visit(Shape<10, ShapeVisitor>) {}
        void visit(Shape<11, ShapeVisitor>) {}
        void visit(Shape<12, ShapeVisitor>) {}
      };

      // This test just needs to compile without errors.
      CHECK(true);
    }

    //*************************************************************************
    TEST(test_13_visitable)
    {
      class ShapeVisitor : public etl::visitor <Shape<1, ShapeVisitor>, Shape<2, ShapeVisitor>, Shape<3, ShapeVisitor>, Shape<4, ShapeVisitor>,
                                                Shape<5, ShapeVisitor>, Shape<6, ShapeVisitor>, Shape<7, ShapeVisitor>, Shape<8, ShapeVisitor>,
                                                Shape<9, ShapeVisitor>, Shape<10, ShapeVisitor>, Shape<11, ShapeVisitor>, Shape<12, ShapeVisitor>,
                                                Shape<13, ShapeVisitor> >
      {
      public:

        void visit(Shape<1, ShapeVisitor>) {}
        void visit(Shape<2, ShapeVisitor>) {}
        void visit(Shape<3, ShapeVisitor>) {}
        void visit(Shape<4, ShapeVisitor>) {}
        void visit(Shape<5, ShapeVisitor>) {}
        void visit(Shape<6, ShapeVisitor>) {}
        void visit(Shape<7, ShapeVisitor>) {}
        void visit(Shape<8, ShapeVisitor>) {}
        void visit(Shape<9, ShapeVisitor>) {}
        void visit(Shape<10, ShapeVisitor>) {}
        void visit(Shape<11, ShapeVisitor>) {}
        void visit(Shape<12, ShapeVisitor>) {}
        void visit(Shape<13, ShapeVisitor>) {}
      };

      // This test just needs to compile without errors.
      CHECK(true);
    }

    //*************************************************************************
    TEST(test_14_visitable)
    {
      class ShapeVisitor : public etl::visitor <Shape<1, ShapeVisitor>, Shape<2, ShapeVisitor>, Shape<3, ShapeVisitor>, Shape<4, ShapeVisitor>,
                                                Shape<5, ShapeVisitor>, Shape<6, ShapeVisitor>, Shape<7, ShapeVisitor>, Shape<8, ShapeVisitor>,
                                                Shape<9, ShapeVisitor>, Shape<10, ShapeVisitor>, Shape<11, ShapeVisitor>, Shape<12, ShapeVisitor>,
                                                Shape<13, ShapeVisitor>, Shape<14, ShapeVisitor> >
      {
      public:

        void visit(Shape<1, ShapeVisitor>) {}
        void visit(Shape<2, ShapeVisitor>) {}
        void visit(Shape<3, ShapeVisitor>) {}
        void visit(Shape<4, ShapeVisitor>) {}
        void visit(Shape<5, ShapeVisitor>) {}
        void visit(Shape<6, ShapeVisitor>) {}
        void visit(Shape<7, ShapeVisitor>) {}
        void visit(Shape<8, ShapeVisitor>) {}
        void visit(Shape<9, ShapeVisitor>) {}
        void visit(Shape<10, ShapeVisitor>) {}
        void visit(Shape<11, ShapeVisitor>) {}
        void visit(Shape<12, ShapeVisitor>) {}
        void visit(Shape<13, ShapeVisitor>) {}
        void visit(Shape<14, ShapeVisitor>) {}
      };

      // This test just needs to compile without errors.
      CHECK(true);
    }

    //*************************************************************************
    TEST(test_15_visitable)
    {
      class ShapeVisitor : public etl::visitor <Shape<1, ShapeVisitor>, Shape<2, ShapeVisitor>, Shape<3, ShapeVisitor>, Shape<4, ShapeVisitor>,
                                                Shape<5, ShapeVisitor>, Shape<6, ShapeVisitor>, Shape<7, ShapeVisitor>, Shape<8, ShapeVisitor>,
                                                Shape<9, ShapeVisitor>, Shape<10, ShapeVisitor>, Shape<11, ShapeVisitor>, Shape<12, ShapeVisitor>,
                                                Shape<13, ShapeVisitor>, Shape<14, ShapeVisitor>, Shape<15, ShapeVisitor> >
      {
      public:

        void visit(Shape<1, ShapeVisitor>) {}
        void visit(Shape<2, ShapeVisitor>) {}
        void visit(Shape<3, ShapeVisitor>) {}
        void visit(Shape<4, ShapeVisitor>) {}
        void visit(Shape<5, ShapeVisitor>) {}
        void visit(Shape<6, ShapeVisitor>) {}
        void visit(Shape<7, ShapeVisitor>) {}
        void visit(Shape<8, ShapeVisitor>) {}
        void visit(Shape<9, ShapeVisitor>) {}
        void visit(Shape<10, ShapeVisitor>) {}
        void visit(Shape<11, ShapeVisitor>) {}
        void visit(Shape<12, ShapeVisitor>) {}
        void visit(Shape<13, ShapeVisitor>) {}
        void visit(Shape<14, ShapeVisitor>) {}
        void visit(Shape<15, ShapeVisitor>) {}
      };

      // This test just needs to compile without errors.
      CHECK(true);
    }

    //*************************************************************************
    TEST(test_16_visitable)
    {
      class ShapeVisitor : public etl::visitor <Shape<1, ShapeVisitor>, Shape<2, ShapeVisitor>, Shape<3, ShapeVisitor>, Shape<4, ShapeVisitor>,
                                                Shape<5, ShapeVisitor>, Shape<6, ShapeVisitor>, Shape<7, ShapeVisitor>, Shape<8, ShapeVisitor>,
                                                Shape<9, ShapeVisitor>, Shape<10, ShapeVisitor>, Shape<11, ShapeVisitor>, Shape<12, ShapeVisitor>,
                                                Shape<13, ShapeVisitor>, Shape<14, ShapeVisitor>, Shape<15, ShapeVisitor>, Shape<16, ShapeVisitor> >
      {
      public:

        void visit(Shape<1, ShapeVisitor>) {}
        void visit(Shape<2, ShapeVisitor>) {}
        void visit(Shape<3, ShapeVisitor>) {}
        void visit(Shape<4, ShapeVisitor>) {}
        void visit(Shape<5, ShapeVisitor>) {}
        void visit(Shape<6, ShapeVisitor>) {}
        void visit(Shape<7, ShapeVisitor>) {}
        void visit(Shape<8, ShapeVisitor>) {}
        void visit(Shape<9, ShapeVisitor>) {}
        void visit(Shape<10, ShapeVisitor>) {}
        void visit(Shape<11, ShapeVisitor>) {}
        void visit(Shape<12, ShapeVisitor>) {}
        void visit(Shape<13, ShapeVisitor>) {}
        void visit(Shape<14, ShapeVisitor>) {}
        void visit(Shape<15, ShapeVisitor>) {}
        void visit(Shape<16, ShapeVisitor>) {}
      };

      // This test just needs to compile without errors.
      CHECK(true);
    }

    //*************************************************************************
    struct NotVisitor {};
    
    TEST(test_is_visitor)
    {
#if ETL_USING_CPP17
      CHECK_TRUE(etl::is_visitor_v<DrawVisitor>);
      CHECK_TRUE(etl::is_visitor_v<LogVisitor>);
      CHECK_FALSE(etl::is_visitor_v<NotVisitor>);
#else
      CHECK_TRUE(etl::is_visitor<DrawVisitor>::value);
      CHECK_TRUE(etl::is_visitor<LogVisitor>::value);
      CHECK_FALSE(etl::is_visitor<NotVisitor>::value);
#endif
    }
  }
}

