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

#include "etl/successor.h"

namespace
{
  //*******************************************************
  struct SuccessorBase : etl::successor<SuccessorBase>
  {
    SuccessorBase()
    {
    }

    SuccessorBase(SuccessorBase& successors...)
      : successor(successors)
    {
    }

    virtual void Test(int) = 0;
  };

  //*******************************************************
  struct SuccessorSameBase1 : SuccessorBase
  {
    SuccessorSameBase1()
      : value(0)
    {
    }

    SuccessorSameBase1(SuccessorBase& successors...)
      : SuccessorBase(successors)
      , value(0)
    {
    }

    void Test(int i) override
    {
      if (i > 1)
      {
        get_successor().Test(i);
      }
      else
      {
        value = i;
      }
    }

    int value;
  };

  //*******************************************************
  struct SuccessorSameBase2 : SuccessorBase
  {
    SuccessorSameBase2()
      : value(0)
    {
    }

    void Test(int i) override
    {
      if (i > 2)
      {
        get_successor().Test(i);
      }
      else
      {
        value = i;
      }
    }

    int value;
  };

  //*******************************************************
  struct SuccessorSameBase3 : SuccessorBase
  {
    SuccessorSameBase3()
      : value(0)
    {
    }

    void Test(int i) override
    {
      if (i > 3)
      {
        get_successor().Test(i);
      }
      else
      {
        value = i;
      }
    }

    int value;
  };

  //*******************************************************
  struct SuccessorSameBase4 : SuccessorBase
  {
    SuccessorSameBase4()
      : value(0)
    {
    }

    void Test(int i) override
    {
      if (i > 4)
      {
        get_successor().Test(i);
      }
      else
      {
        value = i;
      }
    }

    int value;
  };

  //*******************************************************
  struct SuccessorSameBase5 : SuccessorBase
  {
    SuccessorSameBase5()
      : value(0)
    {
    }

    void Test(int i) override
    {
      value = i;
    }

    int value;
  };

  //*******************************************************
  struct SuccessorNoBase3
  {
    SuccessorNoBase3()
      : value(0)
    {
    }

    void Test(int i)
    {
      value = i;
    }

    int value;
  };

  //*******************************************************
  struct SuccessorNoBase2 : etl::successor<SuccessorNoBase3>
  {
    SuccessorNoBase2()
      : value(0)
    {
    }

    void Test(int i)
    {
      if (i > 2)
      {
        get_successor().Test(i);
      }
      else
      {
        value = i;
      }
    }

    int value;
  };

  //*******************************************************
  struct SuccessorNoBase1 : etl::successor<SuccessorNoBase2>
  {
    SuccessorNoBase1()
      : value(0)
    {
    }

    void Test(int i)
    {
      if (i > 1)
      {
        get_successor().Test(i);
      }
      else
      {
        value = i;
      }
    }

    int value;
  };

  SUITE(test_successor)
  {
    //*************************************************************************
    TEST(test_successors_same_check_append_multiple_to_first)
    {
      SuccessorSameBase1 successor1;
      SuccessorSameBase2 successor2;
      SuccessorSameBase3 successor3;
      SuccessorSameBase4 successor4;
      SuccessorSameBase5 successor5;

      successor1.append_successor(successor2, successor3, successor4, successor5);

      CHECK(successor1.has_successor());
      CHECK(successor2.has_successor());
      CHECK(successor3.has_successor());
      CHECK(successor4.has_successor());
      CHECK(!successor5.has_successor());

      CHECK(&successor1.get_successor() == &successor2);
      CHECK(&successor2.get_successor() == &successor3);
      CHECK(&successor3.get_successor() == &successor4);

      CHECK_THROW(successor5.get_successor(), etl::successor_invalid);
    }

    //*************************************************************************
    TEST(test_successors_same_check_append_multiple)
    {
      SuccessorSameBase1 successor1;
      SuccessorSameBase2 successor2;
      SuccessorSameBase3 successor3;
      SuccessorSameBase4 successor4;
      SuccessorSameBase5 successor5;

      successor1.set_successor(successor2, successor3);
      successor1.append_successor(successor4, successor5);

      CHECK(successor1.has_successor());
      CHECK(successor2.has_successor());
      CHECK(successor3.has_successor());
      CHECK(successor4.has_successor());
      CHECK(!successor5.has_successor());

      CHECK(&successor1.get_successor() == &successor2);
      CHECK(&successor2.get_successor() == &successor3);
      CHECK(&successor3.get_successor() == &successor4);

      CHECK_THROW(successor5.get_successor(), etl::successor_invalid);
    }

    //*************************************************************************
    TEST(test_successors_same_check_append_chain)
    {
      SuccessorSameBase1 successor1;
      SuccessorSameBase2 successor2;
      SuccessorSameBase3 successor3;
      SuccessorSameBase4 successor4;
      SuccessorSameBase5 successor5;

      successor1.set_successor(successor2, successor3);
      successor4.set_successor(successor5);
      successor1.append_successor(successor4);

      CHECK(successor1.has_successor());
      CHECK(successor2.has_successor());
      CHECK(successor3.has_successor());
      CHECK(successor4.has_successor());
      CHECK(!successor5.has_successor());

      CHECK(&successor1.get_successor() == &successor2);
      CHECK(&successor2.get_successor() == &successor3);
      CHECK(&successor3.get_successor() == &successor4);

      CHECK_THROW(successor5.get_successor(), etl::successor_invalid);
    }

    //*************************************************************************
    TEST(test_successors_same_check)
    {
      SuccessorSameBase1 successor1;
      SuccessorSameBase2 successor2;
      SuccessorSameBase3 successor3;

      successor1.set_successor(successor2, successor3);

      CHECK(successor1.has_successor());
      CHECK(successor2.has_successor());
      CHECK(!successor3.has_successor());

      CHECK(&successor1.get_successor() == &successor2);
      CHECK(&successor2.get_successor() == &successor3);

      CHECK_THROW(successor3.get_successor(), etl::successor_invalid);
    }

    //*************************************************************************
    TEST(test_successors_same_check_calls)
    {
      SuccessorSameBase1 successor1;
      SuccessorSameBase2 successor2;
      SuccessorSameBase3 successor3;

      successor1.set_successor(successor2, successor3);

      successor1.Test(1);
      successor1.Test(2);
      successor1.Test(3);

      CHECK_EQUAL(1, successor1.value);
      CHECK_EQUAL(2, successor2.value);
      CHECK_EQUAL(3, successor3.value);
    }

    //*************************************************************************
    TEST(test_successors_different_setup)
    {
      SuccessorNoBase1 successor1;
      SuccessorNoBase2 successor2;
      SuccessorNoBase3 successor3;

      successor1.set_successor(successor2, successor3);

      CHECK(successor1.has_successor());
      CHECK(successor2.has_successor());
      // successor3 does not support a successor.

      CHECK(&successor1.get_successor() == &successor2);
      CHECK(&successor2.get_successor() == &successor3);
    }

    //*************************************************************************
    TEST(test_successors_different_check_calls)
    {
      SuccessorNoBase1 successor1;
      SuccessorNoBase2 successor2;
      SuccessorNoBase3 successor3;

      successor1.set_successor(successor2, successor3);

      successor1.Test(1);
      successor1.Test(2);
      successor1.Test(3);

      CHECK_EQUAL(1, successor1.value);
      CHECK_EQUAL(2, successor2.value);
      CHECK_EQUAL(3, successor3.value);
    }

    //*************************************************************************
    TEST(test_clear_successor)
    {
      SuccessorSameBase1 successor1;
      SuccessorSameBase2 successor2;
      SuccessorSameBase3 successor3;
      SuccessorSameBase4 successor4;
      SuccessorSameBase5 successor5;

      successor1.set_successor(successor2, successor3, successor4, successor5);

      successor2.clear_successor();

      CHECK(successor1.has_successor());
      CHECK(!successor2.has_successor());
      CHECK(successor3.has_successor());
      CHECK(successor4.has_successor());
      CHECK(!successor5.has_successor());
    }
    
    //*************************************************************************
    TEST(test_clear_successor_chain)
    {
      SuccessorSameBase1 successor1;
      SuccessorSameBase2 successor2;
      SuccessorSameBase3 successor3;
      SuccessorSameBase4 successor4;
      SuccessorSameBase5 successor5;

      successor1.set_successor(successor2, successor3, successor4, successor5);

      successor2.clear_successor_chain();

      CHECK(successor1.has_successor());
      CHECK(!successor2.has_successor());
      CHECK(!successor3.has_successor());
      CHECK(!successor4.has_successor());
      CHECK(!successor5.has_successor());
    }

    //*************************************************************************
    TEST(test_interative_successors)
    {
      class IProcessor : public etl::successor<IProcessor>
      {
      public:

        //*********************************
        virtual bool Check() = 0;

        //*********************************
        bool Process()
        {
          bool handled = false;

          IProcessor* p = this;

          while (handled == false)
          {
            handled = p->Check();

            // Were we unable to handle the call?
            if (handled == false)
            {
              // Do we have a successor?
              if (p->has_successor())
              {
                // Get the next successor.
                p = &(p->get_successor());
              }
              else
              {
                break;
              }
            }
          }

          return handled;
        }
      };

      //*********************************
      class Processor1 : public IProcessor
      {
      public:

        bool Check() override
        {
          return false;
        }
      };

      //*********************************
      class Processor2 : public IProcessor
      {
      public:

        bool Check() override
        {
          return false;
        }
      };

      //*********************************
      class Processor3 : public IProcessor
      {
      public:

        bool Check() override
        {
          return false;
        }
      };

      //*********************************
      class Processor4 : public IProcessor
      {
      public:

        bool Check() override
        {
          return true;
        }
      };

      Processor1 processor1a;
      Processor1 processor1b;
      Processor2 processor2a;
      Processor2 processor2b;
      Processor3 processor3;
      Processor4 processor4;

      processor1a.append_successor(processor2a, processor3);
      processor1b.append_successor(processor2b, processor4);

      CHECK(processor1a.Process() == false);
      CHECK(processor1b.Process() == true);
    }
  }
}

