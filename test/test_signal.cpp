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
  using callback_type = void(std::ostream&);
  using signal_type   = etl::signal<callback_type, 5U>;
  using slot_type     = signal_type::slot_type;
  using span_type     = signal_type::span_type;

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
  ///\brief Free function that outputs "free"
  ///
  ///\param out: output
  //*************************************************************************
  void output_extra(std::ostream& out) 
  {
    output(out, "extra");
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

  example_class example;

  //*************************************************************************
  ///\brief Makes an empty slot
  ///
  ///\return constexpr slot_type
  //*************************************************************************
  ETL_CONSTEXPR14 slot_type make_empty_slot() 
  {
    return slot_type();
  }

  //*************************************************************************
  ///\brief Makes the free function slot
  ///
  ///\return constexpr slot_type
  //*************************************************************************
  ETL_CONSTEXPR14 slot_type make_free_slot() 
  {
    return slot_type::create<output_free>();
  }

  //*************************************************************************
  ///\brief Makes the lambda slot
  ///
  ///\return constexpr slot_type
  //*************************************************************************
  ETL_CONSTEXPR14 slot_type make_lambda_slot() 
  {
    return slot_type{output_lambda};
  }

  //*************************************************************************
  ///\brief Makes the static method slot
  ///
  ///\return constexpr slot_type
  //*************************************************************************
  ETL_CONSTEXPR14 slot_type make_static_slot() 
  {
    return slot_type::create<&example_class::static_method>();
  }

  //*************************************************************************
  ///\brief Makes the class method slot
  ///
  ///\return constexpr slot_type
  //*************************************************************************
  ETL_CONSTEXPR14 slot_type make_instance_slot() 
  {
    return slot_type::create<example_class, example, &example_class::method>();
  }

  //*************************************************************************
  ///\brief Makes the functor slot
  ///
  ///\return constexpr slot_type
  //*************************************************************************
  ETL_CONSTEXPR14 slot_type make_functor_slot() 
  {
    return slot_type::create<example_class, example>();
  }

  //*************************************************************************
  ///\brief Makes the free function slot
  ///
  ///\return constexpr slot_type
  //*************************************************************************
  ETL_CONSTEXPR14 slot_type make_extra_slot() 
  {
    return slot_type::create<output_extra>();
  }

  //*************************************************************************
  ///\brief Makes the incorrect slot type
  /// Uncomment for test_construct_with_incorrect_slot_type
  ///\return An incorrect slot
  //*************************************************************************
  //ETL_CONSTEXPR14 etl::delegate<int(std::ostream&)> make_incorrect_slot()
  //{
  //  return etl::delegate<int(std::ostream&)>();
  //}

  //***************************************************************************
  SUITE(signal_test)
  {
    //***************************************************************************
    TEST(test_default_construct)
    {
      const signal_type test_object;
      CHECK_EQUAL(0U, test_object.size());
      CHECK_EQUAL(5U, test_object.max_size());
      CHECK_TRUE(test_object.empty());
      CHECK_FALSE(test_object.full());
    }

#if ETL_USING_CPP14
    //***************************************************************************
    TEST(test_constexpr_construct)
    {
      static constexpr signal_type signal
      {
        make_free_slot(), 
        make_lambda_slot(), 
        make_static_slot(), 
        make_instance_slot(), 
        make_functor_slot()
      };

      CHECK_EQUAL(5U, signal.size());
      CHECK_EQUAL(0, signal.available());
      CHECK_EQUAL(5U, signal.max_size());
      CHECK_FALSE(signal.empty());
      CHECK_TRUE(signal.full());
    }
#endif

    //***************************************************************************
    TEST(test_construct_from_slot_list)
    {
      const auto free_slot     = make_free_slot();
      const auto lambda_slot   = make_lambda_slot();
      const auto static_slot   = make_static_slot();
      const auto instance_slot = make_instance_slot();
      const auto functor_slot  = make_functor_slot();

      const signal_type signal{ free_slot, lambda_slot, static_slot, instance_slot, functor_slot };

      CHECK_EQUAL(5, signal.size());
      CHECK_EQUAL(0, signal.available());
      CHECK_EQUAL(5, signal.max_size());
      CHECK_FALSE(signal.empty());
      CHECK_TRUE(signal.full());
    }

    //***************************************************************************
    TEST(test_construct_from_too_many_slots)
    {
      // Uncomment to trigger static_assert "Number of slots exceeds capacity"
      //const auto free_slot     = make_free_slot();
      //const auto lambda_slot   = make_lambda_slot();
      //const auto static_slot   = make_static_slot();
      //const auto instance_slot = make_instance_slot();
      //const auto functor_slot  = make_functor_slot();
      //const auto extra_slot    = make_extra_slot();

      //signal_type signal{ free_slot, lambda_slot, static_slot, instance_slot, functor_slot, extra_slot };
    }

#if ETL_USING_CPP14
    //***************************************************************************
    TEST(test_constexpr_construct_from_slot_list)
    {
      static constexpr auto free_slot     = make_free_slot();
      static constexpr auto lambda_slot   = make_lambda_slot();
      static constexpr auto static_slot   = make_static_slot();
      static constexpr auto instance_slot = make_instance_slot();
      static constexpr auto functor_slot  = make_functor_slot();

      static constexpr signal_type signal{ free_slot, lambda_slot, static_slot, instance_slot, functor_slot };

      CHECK_EQUAL(5, signal.size());
      CHECK_EQUAL(0, signal.available());
      CHECK_EQUAL(5, signal.max_size());
      CHECK_FALSE(signal.empty());
      CHECK_TRUE(signal.full());
    }
#endif

    //***************************************************************************
    TEST(test_construct_with_incorrect_slot_type)
    {
      // Uncomment to trigger static_assert "All slots must be slot_type"
      //const auto free_slot      = make_free_slot();
      //const auto lambda_slot    = make_lambda_slot();
      //const auto incorrect_slot = make_incorrect_slot(); // Incorrect type
      //const auto instance_slot  = make_instance_slot();
      //const auto functor_slot   = make_functor_slot();

      //signal_type signal{ free_slot, lambda_slot, incorrect_slot, instance_slot, functor_slot };
    }

    //***************************************************************************
    TEST(test_connect)
    {
      signal_type signal;

      const auto free_slot = make_free_slot();
      CHECK_TRUE(signal.connect(free_slot));
      CHECK_EQUAL(1U, signal.size());
      CHECK_EQUAL(4U, signal.available());
      signal.connect(free_slot); // Try to connect it again - nothing should change.
      CHECK_EQUAL(1U, signal.size());
      CHECK_EQUAL(4U, signal.available());
      CHECK_FALSE(signal.empty());
      CHECK_FALSE(signal.full());
      CHECK_TRUE(signal.connected(free_slot));

      const auto lambda_slot = make_lambda_slot();
      CHECK_TRUE(signal.connect(lambda_slot));
      CHECK_EQUAL(2U, signal.size());
      CHECK_EQUAL(3U, signal.available());
      CHECK_FALSE(signal.empty());
      CHECK_FALSE(signal.full());
      CHECK_TRUE(signal.connected(lambda_slot));

      const auto static_slot = make_static_slot();
      CHECK_TRUE(signal.connect(static_slot));
      CHECK_EQUAL(3U, signal.size());
      CHECK_EQUAL(2U, signal.available());
      CHECK_FALSE(signal.empty());
      CHECK_FALSE(signal.full());
      CHECK_TRUE(signal.connected(static_slot));

      const auto instance_slot = make_instance_slot();
      CHECK_TRUE(signal.connect(instance_slot));
      CHECK_EQUAL(4U, signal.size());
      CHECK_EQUAL(1U, signal.available());
      CHECK_FALSE(signal.empty());
      CHECK_FALSE(signal.full());
      CHECK_TRUE(signal.connected(instance_slot));

      const auto functor_slot = make_functor_slot();
      CHECK_TRUE(signal.connect(functor_slot));
      CHECK_EQUAL(5U, signal.size());
      CHECK_EQUAL(0U, signal.available());
      CHECK_FALSE(signal.empty());
      CHECK_TRUE(signal.full());
      CHECK_TRUE(signal.connected(functor_slot));
    }

#if ETL_USING_CPP17
    //***************************************************************************
    TEST(test_connect_from_initializer_list)
    {     
      const auto free_slot     = make_free_slot();
      const auto lambda_slot   = make_lambda_slot();
      const auto static_slot   = make_static_slot();
      const auto instance_slot = make_instance_slot();
      const auto functor_slot  = make_functor_slot();

      signal_type signal;
      CHECK_TRUE(signal.connect({ free_slot, lambda_slot, static_slot, instance_slot, functor_slot }));

      CHECK_EQUAL(5U, signal.size());
      CHECK_TRUE(signal.connected(free_slot));
      CHECK_TRUE(signal.connected(lambda_slot));
      CHECK_TRUE(signal.connected(static_slot));
      CHECK_TRUE(signal.connected(instance_slot));
      CHECK_TRUE(signal.connected(functor_slot));
      CHECK_EQUAL(0U, signal.available());
      CHECK_FALSE(signal.empty());
      CHECK_TRUE(signal.full());
    }
#endif

#if ETL_USING_CPP17
    //***************************************************************************
    TEST(test_connect_from_initializer_list_too_many_slots)
    {
      const auto free_slot     = make_free_slot();
      const auto lambda_slot   = make_lambda_slot();
      const auto static_slot   = make_static_slot();
      const auto instance_slot = make_instance_slot();
      const auto functor_slot  = make_functor_slot();
      const auto extra_slot    = make_extra_slot();

      signal_type signal;
      CHECK_THROW(signal.connect({ free_slot, lambda_slot, static_slot, instance_slot, functor_slot, extra_slot }), etl::signal_full);
    }
#endif

    //***************************************************************************
    TEST(test_connect_from_span)
    {
      const auto free_slot     = make_free_slot();
      const auto lambda_slot   = make_lambda_slot();
      const auto static_slot   = make_static_slot();
      const auto instance_slot = make_instance_slot();
      const auto functor_slot  = make_functor_slot();

      const slot_type slot_list[] = { free_slot, lambda_slot, static_slot, instance_slot, functor_slot };

      signal_type signal;
      CHECK_TRUE(signal.connect(slot_list));
    }

    //***************************************************************************
    TEST(test_connect_from_span_too_many_slots)
    {
      const auto free_slot     = make_free_slot();
      const auto lambda_slot   = make_lambda_slot();
      const auto static_slot   = make_static_slot();
      const auto instance_slot = make_instance_slot();
      const auto functor_slot  = make_functor_slot();
      const auto extra_slot    = make_extra_slot();

      const slot_type slot_list[] = { free_slot, lambda_slot, static_slot, instance_slot, functor_slot, extra_slot };

      signal_type signal;
      CHECK_THROW(signal.connect(slot_list), etl::signal_full);
    }

    //***************************************************************************
    TEST(test_disconnect)
    {
      const auto free_slot     = make_free_slot();
      const auto lambda_slot   = make_lambda_slot();
      const auto static_slot   = make_static_slot();
      const auto instance_slot = make_instance_slot();
      const auto functor_slot  = make_functor_slot();

      const slot_type slot_list[] = { free_slot, lambda_slot, static_slot, instance_slot, functor_slot };

      signal_type signal;
      CHECK_TRUE(signal.connect(slot_list));

      signal.disconnect(free_slot);
      CHECK_EQUAL(4U, signal.size());
      CHECK_EQUAL(1U, signal.available());
      CHECK_FALSE(signal.connected(free_slot));
      CHECK_FALSE(signal.empty());
      CHECK_FALSE(signal.full());

      // Try to remove it again - nothing should change.
      signal.disconnect(free_slot);
      CHECK_EQUAL(4U, signal.size());
      CHECK_EQUAL(1U, signal.available());
      CHECK_FALSE(signal.connected(free_slot));
      CHECK_FALSE(signal.empty());
      CHECK_FALSE(signal.full());

      signal.disconnect(lambda_slot);
      CHECK_EQUAL(3U, signal.size());
      CHECK_EQUAL(2U, signal.available());
      CHECK_FALSE(signal.connected(lambda_slot));
      CHECK_FALSE(signal.empty());
      CHECK_FALSE(signal.full());

      signal.disconnect(static_slot);
      CHECK_EQUAL(2U, signal.size());
      CHECK_EQUAL(3U, signal.available());
      CHECK_FALSE(signal.connected(static_slot));
      CHECK_FALSE(signal.empty());
      CHECK_FALSE(signal.full());

      signal.disconnect(instance_slot);
      CHECK_EQUAL(1U, signal.size());
      CHECK_EQUAL(4U, signal.available());
      CHECK_FALSE(signal.connected(instance_slot));
      CHECK_FALSE(signal.empty());
      CHECK_FALSE(signal.full());

      signal.disconnect(functor_slot);
      CHECK_EQUAL(0U, signal.size());
      CHECK_EQUAL(5U, signal.available());
      CHECK_FALSE(signal.connected(functor_slot));
      CHECK_TRUE(signal.empty());
      CHECK_FALSE(signal.full());
    }
  }

#if ETL_USING_CPP17
  //***************************************************************************
  TEST(test_disconnect_from_initializer_list)
  {
    const auto free_slot     = make_free_slot();
    const auto lambda_slot   = make_lambda_slot();
    const auto static_slot   = make_static_slot();
    const auto instance_slot = make_instance_slot();
    const auto functor_slot  = make_functor_slot();

    signal_type signal;
    CHECK_TRUE(signal.connect({ free_slot, lambda_slot, static_slot, instance_slot, functor_slot }));

    signal.disconnect({ lambda_slot, instance_slot });
    CHECK_EQUAL(3U, signal.size());
    CHECK_FALSE(signal.empty());
    CHECK_TRUE(signal.connected(free_slot));
    CHECK_FALSE(signal.connected(lambda_slot));
    CHECK_TRUE(signal.connected(static_slot));
    CHECK_FALSE(signal.connected(instance_slot));
    CHECK_TRUE(signal.connected(functor_slot));
  }
#endif

  //***************************************************************************
  TEST(test_disconnect_from_span_list)
  {
    const auto free_slot     = make_free_slot();
    const auto lambda_slot   = make_lambda_slot();
    const auto static_slot   = make_static_slot();
    const auto instance_slot = make_instance_slot();
    const auto functor_slot  = make_functor_slot();

    signal_type signal;
    CHECK_TRUE(signal.connect(free_slot));
    CHECK_TRUE(signal.connect(lambda_slot));
    CHECK_TRUE(signal.connect(static_slot));
    CHECK_TRUE(signal.connect(instance_slot));
    CHECK_TRUE(signal.connect(functor_slot));

    const slot_type slot_list[] = { lambda_slot, instance_slot };

    signal.disconnect(span_type(slot_list));
    CHECK_EQUAL(3U, signal.size());
    CHECK_FALSE(signal.empty());
    CHECK_TRUE(signal.connected(free_slot));
    CHECK_FALSE(signal.connected(lambda_slot));
    CHECK_TRUE(signal.connected(static_slot));
    CHECK_FALSE(signal.connected(instance_slot));
    CHECK_TRUE(signal.connected(functor_slot));
  }

  //***************************************************************************
  TEST(test_disconnect_all)
  {  
    const auto free_slot     = make_free_slot();
    const auto lambda_slot   = make_lambda_slot();
    const auto static_slot   = make_static_slot();
    const auto instance_slot = make_instance_slot();
    const auto functor_slot  = make_functor_slot();

    const slot_type slot_list[] = { free_slot, lambda_slot, static_slot, instance_slot, functor_slot };

    signal_type signal;
    CHECK_TRUE(signal.connect(slot_list));

    signal.disconnect_all();
    CHECK_EQUAL(0U, signal.size());
    CHECK_TRUE(signal.empty());
    CHECK_FALSE(signal.connected(free_slot));
    CHECK_FALSE(signal.connected(lambda_slot));
    CHECK_FALSE(signal.connected(static_slot));
    CHECK_FALSE(signal.connected(instance_slot));
    CHECK_FALSE(signal.connected(functor_slot));
  }

  //***************************************************************************
  TEST(test_call_empty)
  {
    std::stringstream ss;
    signal_type signal;
    signal(ss);
    CHECK_EQUAL(std::string{""}, ss.str());
  }

  //***************************************************************************
  TEST(test_call)
  {
    const auto free_slot     = make_free_slot();
    const auto lambda_slot   = make_lambda_slot();
    const auto static_slot   = make_static_slot();
    const auto instance_slot = make_instance_slot();
    const auto functor_slot  = make_functor_slot();

    signal_type signal{ free_slot, lambda_slot, static_slot, instance_slot, functor_slot };

    std::stringstream ss;
    signal(ss);

    // Expect all signals got called
    const std::string expected_string{"freelambdastaticmethodfunctor"};
    CHECK_EQUAL(expected_string, ss.str());
  }

#if ETL_USING_CPP14
  //***************************************************************************
  TEST(test_constexpr_call)
  {
    static constexpr auto free_slot     = make_free_slot();
    static constexpr auto lambda_slot   = make_lambda_slot();
    static constexpr auto static_slot   = make_static_slot();
    static constexpr auto instance_slot = make_instance_slot();
    static constexpr auto functor_slot  = make_functor_slot();

    static constexpr signal_type signal{ free_slot, lambda_slot, static_slot, instance_slot, functor_slot };

    std::stringstream ss;
    signal(ss);

    // Expect all slots got called
    const std::string expected_string{"freelambdastaticmethodfunctor"};
    CHECK_EQUAL(expected_string, ss.str());
  }
#endif

  //***************************************************************************
  TEST(test_call_empty_slots)
  {
    const auto free_slot     = make_free_slot();
    const auto lambda_slot   = make_empty_slot();
    const auto static_slot   = make_static_slot();
    const auto instance_slot = make_empty_slot();
    const auto functor_slot  = make_functor_slot();

    signal_type signal{ free_slot, lambda_slot, static_slot, instance_slot, functor_slot };

    std::stringstream ss;
    signal(ss);

    // Expect only valid slots got called
    const std::string expected_string{"freestaticfunctor"};
    CHECK_EQUAL(expected_string, ss.str());
  }

#if ETL_USING_CPP14
  //***************************************************************************
  TEST(test_call_constexpr_empty_slots)
  {
    static constexpr auto free_slot     = make_free_slot();
    static constexpr auto lambda_slot   = make_empty_slot();
    static constexpr auto static_slot   = make_static_slot();
    static constexpr auto instance_slot = make_empty_slot();
    static constexpr auto functor_slot  = make_functor_slot();

    static constexpr signal_type signal{ free_slot, lambda_slot, static_slot, instance_slot, functor_slot };

    std::stringstream ss;
    signal(ss);

    // Expect only valid slots got called
    const std::string expected_string{"freestaticfunctor"};
    CHECK_EQUAL(expected_string, ss.str());
  }
#endif
}
