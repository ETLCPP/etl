/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2026 BMW AG

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

#include "etl/future.h"

#include <type_traits>

#if ETL_HAS_ATOMIC

namespace
{
  //---------------------------------------------------------------------------
  // A simple struct to test non-primitive types in future/promise.
  //---------------------------------------------------------------------------
  struct Point
  {
    int x;
    int y;

    bool operator==(const Point& other) const
    {
      return x == other.x && y == other.y;
    }
  };

  //---------------------------------------------------------------------------
  // Counts the number of live objects, to check construction/destruction.
  //---------------------------------------------------------------------------
  struct Counted
  {
    static int live;

    explicit Counted(int value_)
      : value(value_)
    {
      ++live;
    }

    Counted(const Counted& other)
      : value(other.value)
    {
      ++live;
    }

    ~Counted()
    {
      --live;
    }

    int value;
  };

  int Counted::live = 0;

  //---------------------------------------------------------------------------
  // A move-only type that records whether it has been moved from.
  //---------------------------------------------------------------------------
  struct MoveOnly
  {
    explicit MoveOnly(int value_)
      : value(value_)
      , moved_from(false)
    {
    }

    MoveOnly(MoveOnly&& other)
      : value(other.value)
      , moved_from(false)
    {
      other.moved_from = true;
    }

    MoveOnly(const MoveOnly&)            = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;

    int  value;
    bool moved_from;
  };

  SUITE(test_future)
  {
    //*************************************************************************
    // shared_state<T> tests
    //*************************************************************************

    TEST(test_shared_state_int_default_not_ready)
    {
      etl::shared_state<int> state;
      CHECK(!state.is_ready());
    }

    TEST(test_shared_state_int_set_value_becomes_ready)
    {
      etl::shared_state<int> state;
      state.set_value(42);
      CHECK(state.is_ready());
      CHECK_EQUAL(42, state.get());
    }

    TEST(test_shared_state_int_get_const)
    {
      etl::shared_state<int> state;
      state.set_value(77);
      const etl::shared_state<int>& cref = state;
      CHECK_EQUAL(77, cref.get());
    }

    TEST(test_shared_state_int_reset)
    {
      etl::shared_state<int> state;
      state.set_value(10);
      CHECK(state.is_ready());
      state.reset();
      CHECK(!state.is_ready());
    }

    TEST(test_shared_state_int_reset_when_not_ready)
    {
      etl::shared_state<int> state;
      // Should be safe to reset even when not ready.
      state.reset();
      CHECK(!state.is_ready());
    }

    TEST(test_shared_state_double)
    {
      etl::shared_state<double> state;
      state.set_value(3.14);
      CHECK(state.is_ready());
      CHECK_CLOSE(3.14, state.get(), 0.001);
    }

    TEST(test_shared_state_struct)
    {
      etl::shared_state<Point> state;
      Point                    p{10, 20};
      state.set_value(p);
      CHECK(state.is_ready());
      CHECK_EQUAL(10, state.get().x);
      CHECK_EQUAL(20, state.get().y);
    }

    //*************************************************************************
    // shared_state<void> tests
    //*************************************************************************

    TEST(test_shared_state_void_default_not_ready)
    {
      etl::shared_state<void> state;
      CHECK(!state.is_ready());
    }

    TEST(test_shared_state_void_set_value_becomes_ready)
    {
      etl::shared_state<void> state;
      state.set_value();
      CHECK(state.is_ready());
    }

    TEST(test_shared_state_void_reset)
    {
      etl::shared_state<void> state;
      state.set_value();
      CHECK(state.is_ready());
      state.reset();
      CHECK(!state.is_ready());
    }

    //*************************************************************************
    // future<T> tests
    //*************************************************************************

    TEST(test_future_int_default_invalid)
    {
      etl::future<int> f;
      CHECK(!f.valid());
      CHECK(!f.is_ready());
    }

    TEST(test_future_int_from_state_valid_not_ready)
    {
      etl::shared_state<int> state;
      etl::future<int>       f(state);
      CHECK(f.valid());
      CHECK(!f.is_ready());
    }

    TEST(test_future_int_becomes_ready_after_set)
    {
      etl::shared_state<int> state;
      etl::future<int>       f(state);
      state.set_value(55);
      CHECK(f.is_ready());
      CHECK_EQUAL(55, f.get());
    }

    TEST(test_future_int_const_get)
    {
      etl::shared_state<int> state;
      state.set_value(88);
      const etl::future<int> f(state);
      CHECK(f.is_ready());
      CHECK_EQUAL(88, f.get());
    }

    TEST(test_future_double)
    {
      etl::shared_state<double> state;
      etl::future<double>       f(state);
      state.set_value(2.718);
      CHECK(f.is_ready());
      CHECK_CLOSE(2.718, f.get(), 0.001);
    }

    TEST(test_future_struct)
    {
      etl::shared_state<Point> state;
      etl::future<Point>       f(state);
      state.set_value(Point{3, 4});
      CHECK(f.is_ready());
      CHECK_EQUAL(3, f.get().x);
      CHECK_EQUAL(4, f.get().y);
    }

    //*************************************************************************
    // future<void> tests
    //*************************************************************************

    TEST(test_future_void_default_invalid)
    {
      etl::future<void> f;
      CHECK(!f.valid());
      CHECK(!f.is_ready());
    }

    TEST(test_future_void_from_state)
    {
      etl::shared_state<void> state;
      etl::future<void>       f(state);
      CHECK(f.valid());
      CHECK(!f.is_ready());
      state.set_value();
      CHECK(f.is_ready());
    }

    //*************************************************************************
    // promise<T> tests
    //*************************************************************************

    TEST(test_promise_int_default_invalid)
    {
      etl::promise<int> p;
      CHECK(!p.valid());
    }

    TEST(test_promise_int_set_value_fulfils_future)
    {
      etl::shared_state<int> state;
      etl::promise<int>      p(state);
      etl::future<int>       f = p.get_future();
      CHECK(f.valid());
      CHECK(!f.is_ready());
      p.set_value(123);
      CHECK(f.is_ready());
      CHECK_EQUAL(123, f.get());
    }

    TEST(test_promise_double)
    {
      etl::shared_state<double> state;
      etl::promise<double>      p(state);
      etl::future<double>       f = p.get_future();
      p.set_value(1.41421);
      CHECK(f.is_ready());
      CHECK_CLOSE(1.41421, f.get(), 0.00001);
    }

    TEST(test_promise_struct)
    {
      etl::shared_state<Point> state;
      etl::promise<Point>      p(state);
      etl::future<Point>       f = p.get_future();
      p.set_value(Point{-5, 7});
      CHECK(f.is_ready());
      CHECK_EQUAL(-5, f.get().x);
      CHECK_EQUAL(7, f.get().y);
    }

    //*************************************************************************
    // promise<void> tests
    //*************************************************************************

    TEST(test_promise_void_default_invalid)
    {
      etl::promise<void> p;
      CHECK(!p.valid());
    }

    TEST(test_promise_void_set_value_fulfils_future)
    {
      etl::shared_state<void> state;
      etl::promise<void>      p(state);
      etl::future<void>       f = p.get_future();
      CHECK(f.valid());
      CHECK(!f.is_ready());
      p.set_value();
      CHECK(f.is_ready());
    }

    //*************************************************************************
    // Integration / edge-case tests
    //*************************************************************************

    TEST(test_multiple_futures_same_state)
    {
      // Two futures can observe the same shared_state.
      etl::shared_state<int> state;
      etl::future<int>       f1(state);
      etl::future<int>       f2(state);
      CHECK(!f1.is_ready());
      CHECK(!f2.is_ready());
      state.set_value(999);
      CHECK(f1.is_ready());
      CHECK(f2.is_ready());
      CHECK_EQUAL(999, f1.get());
      CHECK_EQUAL(999, f2.get());
    }

    TEST(test_shared_state_reuse_after_reset)
    {
      etl::shared_state<int> state;
      etl::promise<int>      p(state);
      etl::future<int>       f = p.get_future();
      p.set_value(1);
      CHECK_EQUAL(1, f.get());

      // Reset and reuse.
      state.reset();
      CHECK(!f.is_ready());

      // Set a different value.
      state.set_value(2);
      CHECK(f.is_ready());
      CHECK_EQUAL(2, f.get());
    }

    TEST(test_move_semantics_set_value)
    {
      etl::shared_state<MoveOnly> state;
      etl::promise<MoveOnly>      p(state);
      etl::future<MoveOnly>       f = p.get_future();
      MoveOnly                    value(50);
      p.set_value(etl::move(value));
      CHECK(value.moved_from);
      CHECK(f.is_ready());
      CHECK_EQUAL(50, f.get().value);
      CHECK(!f.get().moved_from);
    }

    //*************************************************************************
    // Lifetime of the stored value
    //*************************************************************************

    TEST(test_shared_state_destroys_value)
    {
      Counted::live = 0;
      {
        etl::shared_state<Counted> state;
        CHECK_EQUAL(0, Counted::live);
        state.set_value(Counted(1));
        CHECK_EQUAL(1, Counted::live);
      }
      CHECK_EQUAL(0, Counted::live);
    }

    TEST(test_shared_state_not_ready_destroys_nothing)
    {
      Counted::live = 0;
      {
        etl::shared_state<Counted> state;
      }
      CHECK_EQUAL(0, Counted::live);
    }

    TEST(test_shared_state_reset_destroys_value_and_reuse)
    {
      Counted::live = 0;
      {
        etl::shared_state<Counted> state;
        state.set_value(Counted(1));
        CHECK_EQUAL(1, Counted::live);
        state.reset();
        CHECK_EQUAL(0, Counted::live);
        CHECK(!state.is_ready());
        state.set_value(Counted(2));
        CHECK_EQUAL(1, Counted::live);
        CHECK_EQUAL(2, state.get().value);
      }
      CHECK_EQUAL(0, Counted::live);
    }

    //*************************************************************************
    // Reference specialisations
    //*************************************************************************

    TEST(test_reference)
    {
      int                     value = 1;
      etl::shared_state<int&> state;
      etl::promise<int&>      p(state);
      etl::future<int&>       f = p.get_future();
      CHECK(!f.is_ready());
      p.set_value(value);
      CHECK(f.is_ready());
      CHECK(&f.get() == &value);
      f.get() = 2;
      CHECK_EQUAL(2, value);

      state.reset();
      CHECK(!f.is_ready());

      int other = 3;
      p.set_value(other);
      CHECK(&f.get() == &other);
    }

    //*************************************************************************
    // Types
    //*************************************************************************

    TEST(test_future_get_return_types)
    {
      CHECK((std::is_same<const int&, decltype(etl::declval<etl::future<int>&>().get())>::value));
      CHECK((std::is_same<const int&, decltype(etl::declval<const etl::future<int>&>().get())>::value));
      CHECK((std::is_same<int&, decltype(etl::declval<etl::future<int&>&>().get())>::value));
      CHECK((std::is_same<void, decltype(etl::declval<etl::future<void>&>().get())>::value));
    }

    TEST(test_future_is_copyable)
    {
      CHECK(std::is_copy_constructible<etl::future<int>>::value);
      CHECK(std::is_copy_assignable<etl::future<int>>::value);

      etl::shared_state<int> state;
      etl::future<int>       f1(state);
      etl::future<int>       f2(f1);
      state.set_value(5);
      CHECK_EQUAL(5, f1.get());
      CHECK_EQUAL(5, f2.get());
      CHECK_EQUAL(5, f2.get()); // get() does not consume the value.
    }

    TEST(test_promise_is_move_only)
    {
      CHECK(!std::is_copy_constructible<etl::promise<int>>::value);
      CHECK(!std::is_copy_assignable<etl::promise<int>>::value);
      CHECK(std::is_nothrow_move_constructible<etl::promise<int>>::value);
      CHECK(std::is_nothrow_move_assignable<etl::promise<int>>::value);

      CHECK(!std::is_copy_constructible<etl::promise<int&>>::value);
      CHECK(std::is_nothrow_move_constructible<etl::promise<int&>>::value);

      CHECK(!std::is_copy_constructible<etl::promise<void>>::value);
      CHECK(std::is_nothrow_move_constructible<etl::promise<void>>::value);
    }

    TEST(test_promise_move_construct)
    {
      etl::shared_state<int> state;
      etl::promise<int>      p1(state);
      etl::promise<int>      p2(etl::move(p1));
      CHECK(!p1.valid());
      CHECK(p2.valid());

      etl::future<int> f = p2.get_future();
      p2.set_value(7);
      CHECK_EQUAL(7, f.get());
    }

    TEST(test_promise_move_assign)
    {
      etl::shared_state<void> state;
      etl::promise<void>      p1(state);
      etl::promise<void>      p2;
      CHECK(!p2.valid());
      p2 = etl::move(p1);
      CHECK(!p1.valid());
      CHECK(p2.valid());

      etl::future<void> f = p2.get_future();
      p2.set_value();
      CHECK(f.is_ready());
    }

    //*************************************************************************
    // Errors
    //*************************************************************************

    TEST(test_shared_state_get_not_ready)
    {
      etl::shared_state<int>        state;
      const etl::shared_state<int>& cstate = state;
      etl::shared_state<int&>       rstate;
      CHECK_THROW((void)state.get(), etl::future_not_ready);
      CHECK_THROW((void)cstate.get(), etl::future_not_ready);
      CHECK_THROW((void)rstate.get(), etl::future_not_ready);
    }

    TEST(test_shared_state_set_value_twice)
    {
      etl::shared_state<int> state;
      state.set_value(1);
      CHECK_THROW(state.set_value(2), etl::promise_already_satisfied);
      CHECK_EQUAL(1, state.get());

      int                     a = 1;
      int                     b = 2;
      etl::shared_state<int&> rstate;
      rstate.set_value(a);
      CHECK_THROW(rstate.set_value(b), etl::promise_already_satisfied);
      CHECK(&rstate.get() == &a);

      etl::shared_state<void> vstate;
      vstate.set_value();
      CHECK_THROW(vstate.set_value(), etl::promise_already_satisfied);
    }

    TEST(test_promise_set_value_twice)
    {
      Counted::live = 0;
      {
        etl::shared_state<Counted> state;
        etl::promise<Counted>      p(state);
        p.set_value(Counted(1));
        CHECK_THROW(p.set_value(Counted(2)), etl::promise_already_satisfied);
        CHECK_EQUAL(1, state.get().value);
        CHECK_EQUAL(1, Counted::live);
      }
      CHECK_EQUAL(0, Counted::live);
    }

    TEST(test_future_get_not_ready)
    {
      etl::shared_state<int>  state;
      etl::future<int>        f(state);
      etl::shared_state<void> vstate;
      etl::future<void>       vf(vstate);
      CHECK_THROW((void)f.get(), etl::future_not_ready);
      CHECK_THROW(vf.get(), etl::future_not_ready);
    }

    TEST(test_future_get_no_state)
    {
      etl::future<int>  f;
      etl::future<int&> rf;
      etl::future<void> vf;
      CHECK_THROW((void)f.get(), etl::future_no_state);
      CHECK_THROW((void)rf.get(), etl::future_no_state);
      CHECK_THROW(vf.get(), etl::future_no_state);
    }

    TEST(test_promise_no_state)
    {
      int                value = 1;
      etl::promise<int>  p;
      etl::promise<int&> rp;
      etl::promise<void> vp;

      CHECK_THROW((void)p.get_future(), etl::future_no_state);
      CHECK_THROW(p.set_value(1), etl::future_no_state);
      CHECK_THROW((void)rp.get_future(), etl::future_no_state);
      CHECK_THROW(rp.set_value(value), etl::future_no_state);
      CHECK_THROW((void)vp.get_future(), etl::future_no_state);
      CHECK_THROW(vp.set_value(), etl::future_no_state);
    }

    TEST(test_promise_no_state_after_move)
    {
      etl::shared_state<int> state;
      etl::promise<int>      p1(state);
      etl::promise<int>      p2(etl::move(p1));
      CHECK_THROW(p1.set_value(1), etl::future_no_state);
      CHECK(!state.is_ready());
    }

    TEST(test_exceptions_derive_from_future_exception)
    {
      etl::future<int> f;
      CHECK_THROW((void)f.get(), etl::future_exception);

      etl::promise<int> p;
      CHECK_THROW(p.set_value(1), etl::future_exception);

      etl::shared_state<int> state;
      state.set_value(1);
      CHECK_THROW(state.set_value(2), etl::future_exception);
    }
  }
} // namespace

#endif // ETL_HAS_ATOMIC
