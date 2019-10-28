/*
 * \brief  Test of HSM
 * \author Stefan Heinzmann, Pontus Astrom
 * \date   2019-08-08
 *
 * The implementation is mostly taken from
 * http://accu.org/index.php/journals/252 with modifications to
 * fix a few bugs and improve naming.
 * TODO: * Switch to GSL library instead of asserts for Expect and Ensure
 terminology
*/
//#include "UnitTest++.h"

#include "etl/nullptr.h"
const std::nullptr_t nullptr = {};

#include "etl/hsm.h"
#include "etl/enum_type.h"

#include <iostream>

namespace test {

etl::message_router_id_t const state_machine_id = 0;

namespace message {
// Messages
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

etl::imessage factory(char c)
{
	switch (c)
		{
		case 'a': return A();
		case 'b': return B();
		case 'c': return C();
		case 'd': return D();
		case 'e': return E();
		case 'f': return F();
		case 'g': return G();
		case 'h': return H();
		default : return A();
		}
};

} // namespace message

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

	// Mutablexo
	void inc_event_cnt()
   {
      _event_cnt++;
   };
	void foo(int i)
   {
      _foo = i;
	};

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
			 ||
			 (destination_router_id == imessage_router::ALL_MESSAGE_ROUTERS))
         {
            receive(source, message);
         }
   };
	
private:
	int      _foo       {0};
	unsigned _event_cnt {0};
};

/******************************************************************************
 ** State definition
 ******************************************************************************/
namespace state {
/**
 * Definition of the state hierarchy. The numbers are unique unsigned
 * values used to uniquify state types in the case two states have the
 * same parent and message set. */
typedef etl::state::Composite<State_machine, 0> Top;
typedef etl::state::Composite<State_machine, 1, Top, message::E> S0;
typedef etl::state::Composite<State_machine, 2, S0, message::A, message::B,
                              message::C, message::D, message::F> S1;
typedef etl::state::Leaf     <State_machine, 3, S1, message::G, message::H> S11;
typedef etl::state::Composite<State_machine, 4, S0, message::C, message::F> S2;
typedef etl::state::Composite<State_machine, 5, S2, message::B, message::H> S21;
typedef etl::state::Leaf     <State_machine, 6, S21, message::D, message::G> S211;

} // namespace state
} // namespace test

/******************************************************************************
 * Top
 ******************************************************************************/
template<>
inline void test::state::Top::handle_init(test::State_machine & arg)
{
	std::cout << "Init(Top) - ";
	etl::state::Init<test::state::S0> tmp(arg);
}

/******************************************************************************
 * S0
 ******************************************************************************/
template<>
inline void test::state::S0::handle_entry(test::State_machine &)
{
	std::cout << "Entry(S0) - ";
}

template<>
inline void test::state::S0::handle_init(test::State_machine & arg)
{
	std::cout << "Init(S0) - ";
	etl::state::Init<test::state::S1> tmp(arg);
}

template<>
inline void test::state::S0::handle_exit(test::State_machine &)
{
	std::cout << "Exit(S0) - ";
}

template<>
template<typename LEAF>
void test::state::S0::
on_event(etl::imessage_router &, test::message::E const &, test::State_machine & h, LEAF const & l) const
{
	/* Guard condition (no guard here)*/
	/* optional Transition if a state change shall be
	 * affected. Transition::Transition() performs UML exit/ actions.*/
	etl::state::Transition<LEAF, Self, test::state::S211> t(h);
	/* Transition action goes here */
	std::cout << "Tran(S0, S211, E) - ";
}

/******************************************************************************
 * S1
 ******************************************************************************/
template<>
inline void test::state::S1::handle_entry(test::State_machine &)
{
	std::cout << "Entry(S1) - ";
}

template<>
inline void test::state::S1::handle_init(test::State_machine & arg)
{
	std::cout << "Init(S1) - ";
	etl::state::Init<test::state::S11> tmp(arg);
}

template<>
inline void test::state::S1::handle_exit(test::State_machine &)
{
	std::cout << "Exit(S1) - ";
}

template<>
template<typename LEAF>
inline void test::state::S1::
on_event(etl::imessage_router &, test::message::A const &, test::State_machine & h,	const LEAF & l) const
{
	etl::state::Transition<LEAF, Self, test::state::S1> t(h);
	std::cout << "Tran(S1, S1, A) - ";
}

template<>
template<typename LEAF>
inline void test::state::S1::
on_event(etl::imessage_router &, test::message::B const &, test::State_machine & h,	const LEAF & l) const
{
	etl::state::Transition<LEAF, Self, test::state::S11> t(h);
	std::cout << "Tran(S1, S11, B) - ";
}

template<>
template<typename LEAF>
inline void test::state::S1::
on_event(etl::imessage_router &, test::message::C const &, test::State_machine & h, const LEAF & l) const
{
	etl::state::Transition<LEAF, Self, test::state::S2> t(h);
	std::cout << "Tran(S1, S2, C) - ";
}

template<>
template<typename LEAF>
inline void test::state::S1::
on_event(etl::imessage_router &, test::message::D const &, test::State_machine & h, const LEAF & l) const
{
	etl::state::Transition<LEAF, Self, test::state::S0> t(h);
	std::cout << "Tran(S1, S0, D) - ";
}

template<>
template<typename LEAF>
inline void test::state::S1::
on_event(etl::imessage_router &, test::message::F const &, test::State_machine & h,	const LEAF & l) const
{
	etl::state::Transition<LEAF, Self, test::state::S211> t(h);
	std::cout << "Tran(S1, S211, F) - ";
}

/******************************************************************************
 * S11
 ******************************************************************************/
template<>
inline void test::state::S11::handle_entry(test::State_machine &)
{
	std::cout << "Entry(S11) - ";
}

template<>
inline void test::state::S11::handle_exit(test::State_machine &)
{
	std::cout << "Exit(S11) - ";
}

template<>
template<typename LEAF>
inline void test::state::S11::
on_event(etl::imessage_router &, test::message::G const &, test::State_machine & h, LEAF const & l) const
{
	etl::state::Transition<LEAF, Self, test::state::S211> t(h);
	std::cout << "Tran(S11, S211, G) - ";
}

template<>
template<typename LEAF>
inline void test::state::S11::
on_event(etl::imessage_router &, test::message::H const &, test::State_machine & h, LEAF const & l) const
{
	if (h.foo())
	{
		h.foo(0);
		std::cout << "h[foo]/foo=0 - ";
	}
}

/******************************************************************************
 * S2
 ******************************************************************************/
template<>
inline void test::state::S2::handle_init(test::State_machine & arg)
{
	etl::state::Init<test::state::S21> tmp(arg);
	std::cout << "Init(S2) - ";
}

template<>
inline void test::state::S2::handle_entry(test::State_machine &)
{
	std::cout << "Entry(S2) - ";
}

template<>
inline void test::state::S2::handle_exit(test::State_machine &)
{
	std::cout << "Exit(S2) - ";
}

template<>
template<typename LEAF>
inline void test::state::S2::
on_event(etl::imessage_router &, test::message::C const &, test::State_machine & h,	const LEAF & l) const
{
	etl::state::Transition<LEAF, Self, test::state::S1> t(h);
	std::cout << "Tran(S2, S1, C) - ";
}

template<>
template<typename LEAF>
inline void test::state::S2::
on_event(etl::imessage_router &, test::message::F const &, test::State_machine & h,	const LEAF & l) const
{
	etl::state::Transition<LEAF, Self, test::state::S11> t(h);
	std::cout << "Tran(S2, S11, F) - ";
}

/******************************************************************************
 * S21
 ******************************************************************************/
template<>
inline void test::state::S21::handle_entry(test::State_machine &)
{
	std::cout << "Entry(S21) - ";
}

template<>
inline void test::state::S21::handle_init(test::State_machine & arg)
{
	std::cout << "Init(S21) - ";
	etl::state::Init<test::state::S211> tmp(arg);
}

template<>
inline void test::state::S21::handle_exit(test::State_machine &)
{
	std::cout << "Exit(S21) - ";
}

template<>
template<typename LEAF>
inline void test::state::S21::
on_event(etl::imessage_router &, test::message::B const &, test::State_machine & h,	const LEAF & l) const
{
	etl::state::Transition<LEAF, Self, test::state::S211> t(h);
	std::cout << "Tran(S21, S211, B) - ";
}

template<>
template<typename LEAF>
inline void test::state::S21::
on_event(etl::imessage_router &, test::message::H const &, test::State_machine & h,	const LEAF & l) const
{
	if (!h.foo()) {
		etl::state::Transition<LEAF, Self, test::state::S21> t(h);
		std::cout << "h[!foo]/foo=1 - ";
		h.foo(1);
	}
}

/******************************************************************************
 * S211
 ******************************************************************************/
template<>
inline void test::state::S211::handle_entry(test::State_machine &)
{
	std::cout << "Entry(S211) - ";
}

template<>
inline void test::state::S211::handle_exit(test::State_machine &)
{
	std::cout << "Exit(S211) - ";
}

template<>
template<typename LEAF>
inline void test::state::S211::
on_event(etl::imessage_router &, test::message::D const &, test::State_machine & h,	const LEAF & l) const
{
	etl::state::Transition<LEAF, Self, test::state::S21> t(h);
	std::cout << "Tran(S211, S21, D) - ";
}

template<>
template<typename LEAF>
inline void test::state::S211::
on_event(etl::imessage_router &, test::message::G const &, test::State_machine & h,	const LEAF & l) const
{
	etl::state::Transition<LEAF, Self, test::state::S0> t(h);
	std::cout << "Tran(S211, S0, G) - ";
}

/******************************************************************************
 * main
 ******************************************************************************/
int main()
{
	test::State_machine hsm(test::state_machine_id);
	test::state::Top::handle_init(hsm);
	for(;;) {
		std::cout << "\nEvent: ";
		char c;
		std::cin >> c;
		if (c < 'a' || 'h' < c) {
			return 0;
		}
		hsm.receive(test::message::factory(c));
	}
}
