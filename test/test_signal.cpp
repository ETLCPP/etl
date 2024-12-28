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

#include "UnitTest++/CheckMacros.h"
#include "unit_test_framework.h"

#include <sstream>
#include <string>

#include "etl/signal.h"

namespace
{

  constexpr auto total_output_methods_{5ULL
  };  // free funciton, lambda, static class method, class method, functor.

  using signal_type = etl::signal<void(std::ostream&), total_output_methods_>;
  using slot_type = signal_type::slot_type;

  //*************************************************************************
  ///\brief Generic output free function
  ///
  ///\param out: output stream
  ///\param str: input string
  //*************************************************************************
  void output(std::ostream& out, const std::string& str) 
  { 
    out << str; 
  }

  //*************************************************************************
  ///\brief Free function that outputs "free"
  ///
  ///\param out: output
  //*************************************************************************
  void output_free(std::ostream& out) 
  {
    output(out, "free");
  }

  //*************************************************************************
  ///\brief Lambda that outputs "lambda"
  /// 
  ///\param out: output
  //*************************************************************************
  auto output_lambda = [](std::ostream& out) { output(out, "lambda"); };

  class example_class
  {
  public:
    //*************************************************************************
    ///\brief Method that outputs "static"
    ///
    ///\param out: output
    //*************************************************************************
    static void static_method(std::ostream& out) 
    { 
      output(out, "static"); 
    }

    //*************************************************************************
    ///\brief Method that outputs "method"  
    ///
    ///\param out: output
    //*************************************************************************
    void method(std::ostream& out) 
    {
      output(out, "method");
    }

    //*************************************************************************
    ///\brief Functor method that outputs "functor"
    ///
    ///\param out: output
    //*************************************************************************
    void operator()(std::ostream& out) 
    {
      output(out, "functor");
    }
  };

  example_class example_;

  //*************************************************************************
  ///\brief Makes the free function slot
  ///
  ///\return constexpr slot_type
  //*************************************************************************
  constexpr slot_type make_free_slot() 
  {
    return slot_type::create<output_free>();
  }

  //*************************************************************************
  ///\brief Makes the lambda slot
  ///
  ///\return constexpr slot_type
  //*************************************************************************
  constexpr slot_type make_lambda_slot() 
  {
    return slot_type::create<output_lambda>();
  }

  //*************************************************************************
  ///\brief Makes the static method slot
  ///
  ///\return constexpr slot_type
  //*************************************************************************
  constexpr slot_type make_static_slot() 
  {
    return slot_type::create<&example_class::static_method>();
  }

  //*************************************************************************
  ///\brief Makes the class method slot
  ///
  ///\return constexpr slot_type
  //*************************************************************************
  constexpr slot_type make_instance_slot() 
  {
    return slot_type::create<example_class, example_, &example_class::method>();
  }

  //*************************************************************************
  ///\brief Makes the functor slot
  ///
  ///\return constexpr slot_type
  //*************************************************************************
  constexpr slot_type make_functor_slot() 
  {
    return slot_type::create<example_class, example_>();
  }

  constexpr signal_type constexpr_test_object_{
    make_free_slot(), 
    make_lambda_slot(), 
    make_static_slot(), 
    make_instance_slot(), 
    make_functor_slot()
  };

  SUITE(signal_test)
  {
    TEST(construct)
    {
      signal_type test_object_;
      CHECK_EQUAL(0U, test_object_.size());
      CHECK_EQUAL(total_output_methods_, test_object_.max_size());
      CHECK_TRUE(test_object_.empty());
      CHECK_FALSE(test_object_.full());

      CHECK_EQUAL(total_output_methods_, constexpr_test_object_.size());
      CHECK_EQUAL(total_output_methods_, constexpr_test_object_.max_size());
      CHECK_FALSE(constexpr_test_object_.empty());
      CHECK_TRUE(constexpr_test_object_.full());
    }

    TEST(connect)
    {
      signal_type test_object_;
      const auto free_slot = make_free_slot();
      test_object_.connect(free_slot);
      CHECK_EQUAL(1U, test_object_.size());
      CHECK_TRUE(test_object_.connected(free_slot));

      const auto lambda_slot = make_lambda_slot();
      test_object_.connect(lambda_slot);
      CHECK_EQUAL(2U, test_object_.size());
      CHECK_TRUE(test_object_.connected(lambda_slot));

      const auto static_slot = make_static_slot();
      test_object_.connect(static_slot);
      CHECK_EQUAL(3U, test_object_.size());
      CHECK_TRUE(test_object_.connected(static_slot));

      const auto instance_slot = make_instance_slot();
      test_object_.connect(instance_slot);
      CHECK_EQUAL(4U, test_object_.size());
      CHECK_TRUE(test_object_.connected(instance_slot));

      const auto functor_slot = make_functor_slot();
      test_object_.connect(functor_slot);
      CHECK_EQUAL(5U, test_object_.size());
      CHECK_TRUE(test_object_.connected(functor_slot));
      CHECK_TRUE(test_object_.full());
    }

    TEST(disconnect)
    {
      signal_type test_object_;
      const auto free_slot = make_free_slot();
      const auto lambda_slot = make_lambda_slot();
      const auto static_slot = make_static_slot();
      const auto instance_slot = make_instance_slot();
      const auto functor_slot = make_functor_slot();
      test_object_.connect(free_slot);
      test_object_.connect(lambda_slot);
      test_object_.connect(static_slot);
      test_object_.connect(instance_slot);
      test_object_.connect(functor_slot);

      test_object_.disconnect(free_slot);
      CHECK_EQUAL(4U, test_object_.size());
      CHECK_FALSE(test_object_.connected(free_slot));

      // Try to remove it again - nothing should change.
      test_object_.disconnect(free_slot);
      CHECK_EQUAL(4U, test_object_.size());
      CHECK_FALSE(test_object_.connected(free_slot));

      test_object_.disconnect(lambda_slot);
      CHECK_EQUAL(3U, test_object_.size());
      CHECK_FALSE(test_object_.connected(lambda_slot));

      test_object_.disconnect(static_slot);
      CHECK_EQUAL(2U, test_object_.size());
      CHECK_FALSE(test_object_.connected(static_slot));

      test_object_.disconnect(instance_slot);
      CHECK_EQUAL(1U, test_object_.size());
      CHECK_FALSE(test_object_.connected(instance_slot));

      test_object_.disconnect(functor_slot);
      CHECK_EQUAL(0U, test_object_.size());
      CHECK_FALSE(test_object_.connected(functor_slot));
    }
  }

  TEST(disconnect_all)
  {
    signal_type test_object_;
    const auto free_slot = make_free_slot();
    const auto lambda_slot = make_lambda_slot();
    const auto static_slot = make_static_slot();
    const auto instance_slot = make_instance_slot();
    const auto functor_slot = make_functor_slot();
    test_object_.connect(free_slot);
    test_object_.connect(lambda_slot);
    test_object_.connect(static_slot);
    test_object_.connect(instance_slot);
    test_object_.connect(functor_slot);

    test_object_.disconnect_all();
    CHECK_EQUAL(0U, test_object_.size());
    CHECK_TRUE(test_object_.empty());
    CHECK_FALSE(test_object_.connected(free_slot));
    CHECK_FALSE(test_object_.connected(lambda_slot));
    CHECK_FALSE(test_object_.connected(static_slot));
    CHECK_FALSE(test_object_.connected(instance_slot));
    CHECK_FALSE(test_object_.connected(functor_slot));
  }

  TEST(call_empty)
  {
    std::stringstream ss;
    signal_type test_object_;
    test_object_(ss);
    CHECK_EQUAL(std::string{""}, ss.str());
  }

  TEST(call)
  {
    signal_type test_object_;
    test_object_.connect(make_free_slot());
    test_object_.connect(make_lambda_slot());
    test_object_.connect(make_static_slot());
    test_object_.connect(make_instance_slot());
    test_object_.connect(make_functor_slot());

    std::stringstream ss;
    test_object_(ss);

    // expect all signals got called
    const std::string expected_string{"freelambdastaticmethodfunctor"};
    CHECK_EQUAL(expected_string, ss.str());

    std::stringstream ss2;
    constexpr_test_object_(ss2);
    CHECK_EQUAL(expected_string, ss2.str());
  }

}
