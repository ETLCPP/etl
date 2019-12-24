/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2019 Pontus Astrom

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

#include "UnitTest++.h"

#include "etl/nullptr.h"

#include "etl/hsm.h"
#include "etl/enum_type.h"

#include <sstream>

namespace test
{
  const etl::message_router_id_t state_machine_id = 0;

  namespace message
  {
    struct Id
    {
	     enum enum_type
	     {
		     A, B, C, D, E, F, G, H
	     };

	     ETL_DECLARE_ENUM_TYPE(Id, etl::message_id_t)
	     ETL_ENUM_TYPE(A, "A")
	     ETL_ENUM_TYPE(B, "B")
	     ETL_ENUM_TYPE(C, "C")
	     ETL_ENUM_TYPE(D, "D")
	     ETL_ENUM_TYPE(E, "E")
	     ETL_ENUM_TYPE(F, "F")
	     ETL_ENUM_TYPE(G, "G")
	     ETL_ENUM_TYPE(H, "H")
	     ETL_END_ENUM_TYPE
    };

    class A : public etl::message<Id::A>{};
    class B : public etl::message<Id::B>{};
    class C : public etl::message<Id::C>{};
    class D : public etl::message<Id::D>{};
    class E : public etl::message<Id::E>{};
    class F : public etl::message<Id::F>{};
    class G : public etl::message<Id::G>{};
    class H : public etl::message<Id::H>{};
  }

  /******************************************************************************
   ** State_machine declaration
   ******************************************************************************/
  class State_machine : public etl::hsm<State_machine>
  {
  public:
	   State_machine(etl::message_router_id_t id)
		   : etl::hsm<State_machine>(id) { };

	   // Non-mutable
	   unsigned get_event_cnt() const
      {
         return _event_cnt;
      };

	   int foo() const
	   {
         return _foo;
      };

	   std::string get_transition() const
	   {
         return _os.str();
      };

	   // Mutable
	   void inc_event_cnt()
	   {
         _event_cnt++;
      };

	   void foo(int i)
	   {
         _foo = i;
      };

	   std::ostringstream & set_transition()
	   {
         return _os;
      }

	   void clear_transition()
	   {
         _os.str(std::string());
      }

	   virtual void receive(etl::imessage_router & source,
                           etl::imessage const & message) override final
      {
         get_state()->process_event(source, message, * this);
      };

	   virtual void receive(etl::imessage const & message) override final
      {
         static etl::null_message_router nmr;
         receive(nmr, message);
      };

	   virtual void receive(imessage_router & source,
                           etl::message_router_id_t destination_router_id,
                           etl::imessage const & message) override final
  	   {
         if ((destination_router_id == get_message_router_id())
             || (destination_router_id == imessage_router::ALL_MESSAGE_ROUTERS))
         {
            receive(source, message);
         }
      };

  private:
	   int                _foo       {0};
	   unsigned           _event_cnt {0};
	   std::ostringstream _os;
  };

  /******************************************************************************
   ** State definition
   ******************************************************************************/
  namespace state
  {
    /**
     * Definition of the state hierarchy. The numbers are unique unsigned
     * values used to uniquify state types in the case two states have the
     * same parent and message set. */
    typedef etl::hsm_state::composite<State_machine, 0> Top;
    typedef etl::hsm_state::composite<State_machine, 1, Top, message::E> S0;
    typedef etl::hsm_state::composite<State_machine, 2, S0,  message::A, message::B, message::C, message::D, message::F> S1;
    typedef etl::hsm_state::leaf     <State_machine, 3, S1,  message::G, message::H> S11;
    typedef etl::hsm_state::composite<State_machine, 4, S0,  message::C, message::F> S2;
    typedef etl::hsm_state::composite<State_machine, 5, S2,  message::B, message::H> S21;
    typedef etl::hsm_state::leaf     <State_machine, 6, S21, message::D, message::G> S211;
  }
}

/******************************************************************************
 * Top
 ******************************************************************************/
template<>
inline void test::state::Top::handle_init(test::State_machine & arg)
{
	arg.set_transition() << "Init(Top) - ";
	etl::hsm_state::Init<test::state::S0> tmp(arg);
}

/******************************************************************************
 * S0
 ******************************************************************************/
template<>
inline void test::state::S0::handle_entry(test::State_machine & arg)
{
	arg.set_transition() << "Entry(S0) - ";
}

template<>
inline void test::state::S0::handle_init(test::State_machine & arg)
{
	arg.set_transition() << "Init(S0) - ";
	etl::hsm_state::Init<test::state::S1> tmp(arg);
}

template<>
inline void test::state::S0::handle_exit(test::State_machine & arg)
{
	arg.set_transition() << "Exit(S0) - ";
}

template<>
template<typename LEAF>
void test::state::S0::
on_event(etl::imessage_router &, test::message::E const &, test::State_machine & h, LEAF const & l) const
{
	/* Guard condition (no guard here)*/
	/* optional Transition if a state change shall be
	 * affected. Transition::Transition() performs UML exit/ actions.*/
	etl::hsm_state::transition<LEAF, Self, test::state::S211> t(h);
	/* Transition action goes here */
	h.set_transition() << "Tran(S0, S211, E) - ";
}

/******************************************************************************
 * S1
 ******************************************************************************/
template<>
inline void test::state::S1::handle_entry(test::State_machine & arg)
{
	arg.set_transition() << "Entry(S1) - ";
}

template<>
inline void test::state::S1::handle_init(test::State_machine & arg)
{
	arg.set_transition() << "Init(S1) - ";
	etl::hsm_state::Init<test::state::S11> tmp(arg);
}

template<>
inline void test::state::S1::handle_exit(test::State_machine & arg)
{
	arg.set_transition() << "Exit(S1) - ";
}

template<>
template<typename LEAF>
inline void test::state::S1::
on_event(etl::imessage_router &, test::message::A const &, test::State_machine & h,	const LEAF & l) const
{
	etl::hsm_state::transition<LEAF, Self, test::state::S1> t(h);
	h.set_transition() << "Tran(S1, S1, A) - ";
}

template<>
template<typename LEAF>
inline void test::state::S1::
on_event(etl::imessage_router &, test::message::B const &, test::State_machine & h,	const LEAF & l) const
{
	etl::hsm_state::transition<LEAF, Self, test::state::S11> t(h);
	h.set_transition() << "Tran(S1, S11, B) - ";
}

template<>
template<typename LEAF>
inline void test::state::S1::
on_event(etl::imessage_router &, test::message::C const &, test::State_machine & h, const LEAF & l) const
{
	etl::hsm_state::transition<LEAF, Self, test::state::S2> t(h);
	h.set_transition() << "Tran(S1, S2, C) - ";
}

template<>
template<typename LEAF>
inline void test::state::S1::
on_event(etl::imessage_router &, test::message::D const &, test::State_machine & h, const LEAF & l) const
{
	etl::hsm_state::transition<LEAF, Self, test::state::S0> t(h);
	h.set_transition() << "Tran(S1, S0, D) - ";
}

template<>
template<typename LEAF>
inline void test::state::S1::
on_event(etl::imessage_router &, test::message::F const &, test::State_machine & h,	const LEAF & l) const
{
	etl::hsm_state::transition<LEAF, Self, test::state::S211> t(h);
	h.set_transition() << "Tran(S1, S211, F) - ";
}

/******************************************************************************
 * S11
 ******************************************************************************/
template<>
inline void test::state::S11::handle_entry(test::State_machine & arg)
{
	arg.set_transition() << "Entry(S11) - ";
}

template<>
inline void test::state::S11::handle_exit(test::State_machine & arg)
{
	arg.set_transition() << "Exit(S11) - ";
}

template<>
template<typename LEAF>
inline void test::state::S11::
on_event(etl::imessage_router &, test::message::G const &, test::State_machine & h, LEAF const & l) const
{
	etl::hsm_state::transition<LEAF, Self, test::state::S211> t(h);
	h.set_transition() << "Tran(S11, S211, G) - ";
}

template<>
template<typename LEAF>
inline void test::state::S11::
on_event(etl::imessage_router &, test::message::H const &, test::State_machine & h, LEAF const & l) const
{
	if (h.foo())
	{
		h.foo(0);
		h.set_transition() << "h[foo]/foo=0 - ";
	}
}

/******************************************************************************
 * S2
 ******************************************************************************/
template<>
inline void test::state::S2::handle_init(test::State_machine & arg)
{
	etl::hsm_state::Init<test::state::S21> tmp(arg);
	arg.set_transition() << "Init(S2) - ";
}

template<>
inline void test::state::S2::handle_entry(test::State_machine & arg)
{
	arg.set_transition() << "Entry(S2) - ";
}

template<>
inline void test::state::S2::handle_exit(test::State_machine & arg)
{
	arg.set_transition() << "Exit(S2) - ";
}

template<>
template<typename LEAF>
inline void test::state::S2::
on_event(etl::imessage_router &, test::message::C const &, test::State_machine & h,	const LEAF & l) const
{
	etl::hsm_state::transition<LEAF, Self, test::state::S1> t(h);
	h.set_transition() << "Tran(S2, S1, C) - ";
}

template<>
template<typename LEAF>
inline void test::state::S2::
on_event(etl::imessage_router &, test::message::F const &, test::State_machine & h,	const LEAF & l) const
{
	etl::hsm_state::transition<LEAF, Self, test::state::S11> t(h);
	h.set_transition() << "Tran(S2, S11, F) - ";
}

/******************************************************************************
 * S21
 ******************************************************************************/
template<>
inline void test::state::S21::handle_entry(test::State_machine & arg)
{
	arg.set_transition() << "Entry(S21) - ";
}

template<>
inline void test::state::S21::handle_init(test::State_machine & arg)
{
	arg.set_transition() << "Init(S21) - ";
	etl::hsm_state::Init<test::state::S211> tmp(arg);
}

template<>
inline void test::state::S21::handle_exit(test::State_machine & arg)
{
	arg.set_transition() << "Exit(S21) - ";
}

template<>
template<typename LEAF>
inline void test::state::S21::
on_event(etl::imessage_router &, test::message::B const &, test::State_machine & h,	const LEAF & l) const
{
	etl::hsm_state::transition<LEAF, Self, test::state::S211> t(h);
	h.set_transition() << "Tran(S21, S211, B) - ";
}

template<>
template<typename LEAF>
inline void test::state::S21::
on_event(etl::imessage_router &, test::message::H const &, test::State_machine & h,	const LEAF & l) const
{
	if (!h.foo()) {
		etl::hsm_state::transition<LEAF, Self, test::state::S21> t(h);
		h.set_transition() << "h[!foo]/foo=1 - ";
		h.foo(1);
	}
}

/******************************************************************************
 * S211
 ******************************************************************************/
template<>
inline void test::state::S211::handle_entry(test::State_machine & arg)
{
	arg.set_transition() << "Entry(S211) - ";
}

template<>
inline void test::state::S211::handle_exit(test::State_machine & arg)
{
	arg.set_transition() << "Exit(S211) - ";
}

template<>
template<typename LEAF>
inline void test::state::S211::
on_event(etl::imessage_router &, test::message::D const &, test::State_machine & h,	const LEAF & l) const
{
	etl::hsm_state::transition<LEAF, Self, test::state::S21> t(h);
	h.set_transition() << "Tran(S211, S21, D) - ";
}

template<>
template<typename LEAF>
inline void test::state::S211::
on_event(etl::imessage_router &, test::message::G const &, test::State_machine & h,	const LEAF & l) const
{
	etl::hsm_state::transition<LEAF, Self, test::state::S0> t(h);
	h.set_transition() << "Tran(S211, S0, G) - ";
}

/******************************************************************************
 * Testsuite
 ******************************************************************************/
SUITE(test_map_hsm)
{
	class HsmFixture
	{
	public:
		 HsmFixture() {}
		 ~HsmFixture() {}

		 test::State_machine hsm {test::state_machine_id};
	};

	//*************************************************************************
	TEST_FIXTURE(HsmFixture, test_hsm_basic)
	{
		CHECK(!hsm.is_started());
		test::state::Top::handle_init(hsm);
		CHECK(hsm.is_started());
		CHECK(hsm.get_state() == &test::state::S11::obj);
		CHECK(hsm.get_transition() == "Init(Top) - Entry(S0) - Init(S0) - Entry(S1) - Init(S1) - Entry(S11) - ");
		hsm.clear_transition();

		// In S11
		hsm.receive(test::message::A());
		CHECK(hsm.get_transition() == "Exit(S11) - Exit(S1) - Tran(S1, S1, A) - Entry(S1) - Init(S1) - Entry(S11) - ");
		CHECK(hsm.get_state() == &test::state::S11::obj);
		hsm.clear_transition();

		// In S11
		hsm.receive(test::message::B());
		CHECK(hsm.get_transition() == "Exit(S11) - Exit(S1) - Tran(S1, S11, B) - Entry(S1) - Entry(S11) - ");
		CHECK(hsm.get_state() == &test::state::S11::obj);
		hsm.clear_transition();

		// In S11
		hsm.receive(test::message::C());
		CHECK(hsm.get_transition() == "Exit(S11) - Exit(S1) - Tran(S1, S2, C) - Entry(S2) - Init(S2) - Entry(S21) - Init(S21) - Entry(S211) - ");
		CHECK(hsm.get_state() == &test::state::S211::obj);
		hsm.clear_transition();

		// In S211
		hsm.receive(test::message::A());
		CHECK(hsm.get_transition() == "");
		CHECK(hsm.get_state() == &test::state::S211::obj);
		hsm.clear_transition();

		// In S211
		hsm.receive(test::message::B());
		CHECK(hsm.get_transition() == "Exit(S211) - Exit(S21) - Tran(S21, S211, B) - Entry(S21) - Entry(S211) - ");
		CHECK(hsm.get_state() == &test::state::S211::obj);
		hsm.clear_transition();

		// In S211
		hsm.receive(test::message::C());
		CHECK(hsm.get_transition() == "Exit(S211) - Exit(S21) - Exit(S2) - Tran(S2, S1, C) - Entry(S1) - Init(S1) - Entry(S11) - ");
		CHECK(hsm.get_state() == &test::state::S11::obj);
		hsm.clear_transition();

		// In S11
		hsm.receive(test::message::D());
		CHECK(hsm.get_transition() == "Exit(S11) - Exit(S1) - Exit(S0) - Tran(S1, S0, D) - Entry(S0) - Init(S0) - Entry(S1) - Init(S1) - Entry(S11) - ");
		CHECK(hsm.get_state() == &test::state::S11::obj);
		hsm.clear_transition();

		// In S11
		hsm.receive(test::message::E());
		CHECK(hsm.get_transition() == "Exit(S11) - Exit(S1) - Exit(S0) - Tran(S0, S211, E) - Entry(S0) - Entry(S2) - Entry(S21) - Entry(S211) - ");
		CHECK(hsm.get_state() == &test::state::S211::obj);
		hsm.clear_transition();

		// In S211
		hsm.receive(test::message::D());
		CHECK(hsm.get_transition() == "Exit(S211) - Exit(S21) - Tran(S211, S21, D) - Entry(S21) - Init(S21) - Entry(S211) - ");
		CHECK(hsm.get_state() == &test::state::S211::obj);
		hsm.clear_transition();

		// In S211
		hsm.receive(test::message::E());
		CHECK(hsm.get_transition() == "Exit(S211) - Exit(S21) - Exit(S2) - Exit(S0) - Tran(S0, S211, E) - Entry(S0) - Entry(S2) - Entry(S21) - Entry(S211) - ");
		CHECK(hsm.get_state() == &test::state::S211::obj);
		hsm.clear_transition();

		// In S211
		hsm.receive(test::message::F());
		CHECK(hsm.get_transition() == "Exit(S211) - Exit(S21) - Exit(S2) - Tran(S2, S11, F) - Entry(S1) - Entry(S11) - ");
		CHECK(hsm.get_state() == &test::state::S11::obj);
		hsm.clear_transition();

		// In S11
		hsm.receive(test::message::F());
		CHECK(hsm.get_transition() == "Exit(S11) - Exit(S1) - Tran(S1, S211, F) - Entry(S2) - Entry(S21) - Entry(S211) - ");
		CHECK(hsm.get_state() == &test::state::S211::obj);
		hsm.clear_transition();

		// In S211
		hsm.receive(test::message::G());
		CHECK(hsm.get_transition() == "Exit(S211) - Exit(S21) - Exit(S2) - Exit(S0) - Tran(S211, S0, G) - Entry(S0) - Init(S0) - Entry(S1) - Init(S1) - Entry(S11) - ");
		CHECK(hsm.get_state() == &test::state::S11::obj);
		hsm.clear_transition();

		// In S11
		hsm.receive(test::message::G());
		CHECK(hsm.get_transition() == "Exit(S11) - Exit(S1) - Tran(S11, S211, G) - Entry(S2) - Entry(S21) - Entry(S211) - ");
		CHECK(hsm.get_state() == &test::state::S211::obj);
		hsm.clear_transition();

		// In S211
		hsm.receive(test::message::H());
		CHECK(hsm.get_transition() == "Exit(S211) - Exit(S21) - h[!foo]/foo=1 - Entry(S21) - Init(S21) - Entry(S211) - ");
		CHECK(hsm.get_state() == &test::state::S211::obj);
		hsm.clear_transition();

		// In S211
		hsm.receive(test::message::H());
		CHECK(hsm.get_transition() == "");
		CHECK(hsm.get_state() == &test::state::S211::obj);
		hsm.clear_transition();

		// In S211
		hsm.receive(test::message::C());
		CHECK(hsm.get_state() == &test::state::S11::obj);
		hsm.clear_transition();

		// In S11
		hsm.receive(test::message::H());
		CHECK(hsm.get_transition() == "h[foo]/foo=0 - ");
		CHECK(hsm.get_state() == &test::state::S11::obj);
		hsm.clear_transition();

		// In S11
		hsm.receive(test::message::H());
		CHECK(hsm.get_transition() == "");
		CHECK(hsm.get_state() == &test::state::S11::obj);
		hsm.clear_transition();
	}

	//*************************************************************************
	TEST_FIXTURE(HsmFixture, test_set_state)
	{
		CHECK(!hsm.is_started());
		test::state::Top::handle_init(hsm);
		CHECK(hsm.get_state() == &test::state::S11::obj);
		hsm.set_state(test::state::S211::obj);
		CHECK(hsm.get_state() == &test::state::S211::obj);
	}

};
