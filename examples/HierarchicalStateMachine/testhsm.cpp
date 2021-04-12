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

#include "etl/hsm.h"
#include "etl/enum_type.h"

#include <iostream>

namespace test
{
    etl::message_router_id_t const router_id = 0;
  
namespace message
{
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
    
    A a;
    B b;
    C c;
    D d;
    E e;
    F f;
    G g;
    H h;

    etl::imessage & factory(char arg)
    {
        switch (arg)
        {
        case 'a': return a;
        case 'b': return b;
        case 'c': return c;
        case 'd': return d;
        case 'e': return e;
        case 'f': return f;
        case 'g': return g;
        case 'h': return h;
        default : return a;
        }
    };
    
} // namespace message

namespace state {

typedef std::integer_sequence< etl::message_id_t,
                               message::Id::A,
                               message::Id::B,
                               message::Id::C,
                               message::Id::D,
                               message::Id::E,
                               message::Id::F,
                               message::Id::G,
                               message::Id::H > allowed_message_ids_t;

//________________________________________________________________________________________
//    
//  Extended declaration
//________________________________________________________________________________________

class Extended : public etl::hsm::extended<Extended,
                                           message::A, message::B, message::C,
                                           message::D, message::E, message::F,
                                           message::G, message::H>
{
public:

    Extended(etl::message_router_id_t id)
        : etl::hsm::extended< Extended,
                              message::A, message::B, message::C, message::D, message::E,
                              message::F, message::G, message::H > (id)
    {
    }
    
    // Non-mutable
    unsigned get_event_cnt() const
    {
        return _event_cnt;
    }
    
    int foo() const
    {
        return _foo;
    }
    
    // Mutablexo
    void inc_event_cnt()
    {
        _event_cnt++;
    }

    void foo(int i)
    {
        _foo = i;
    }

private:

    int      _foo       {0};
    unsigned _event_cnt {0};
};

//________________________________________________________________________________________
//    
//  State definition
//________________________________________________________________________________________

    /**
     * Definition of the state hierarchy. The numbers are unique unsigned
     * values used to uniquify state types in the case two states have the
     * same parent and message set. */

    using Top  = etl::hsm::composite<Extended, 0>;

    using S0   = etl::hsm::composite<Extended, 1, Top, message::E>;

    using S1   = etl::hsm::composite<Extended, 2, S0,
                                     message::A, message::B, message::C, message::D,
                                     message::F>;

    using S11  = etl::hsm::simple   <Extended, 3, allowed_message_ids_t, S1,
                                     message::G>;

    using S2   = etl::hsm::composite<Extended, 4, S0, message::C, message::F>;

    using S21  = etl::hsm::composite<Extended, 5, S2, message::B, message::H>;

    using S211 = etl::hsm::simple   <Extended, 6, allowed_message_ids_t, S21,
                                     message::D, message::G>;

} // namespace state
} // namespace test

//________________________________________________________________________________________
//    
//  Top
//________________________________________________________________________________________

template<>
inline void test::state::Top::handle_init(test::state::Extended & arg)
{
  std::cout << "Init(Top) - ";
  etl::hsm::Init<test::state::S0> tmp(arg);
}

//________________________________________________________________________________________
//    
//  S0
//________________________________________________________________________________________

template<>
inline void test::state::S0::handle_entry(test::state::Extended &)
{
  std::cout << "Entry(S0) - ";
}

template<>
inline void test::state::S0::handle_init(test::state::Extended & arg)
{
  std::cout << "Init(S0) - ";
  etl::hsm::Init<test::state::S1> tmp(arg);
}

template<>
inline void test::state::S0::handle_exit(test::state::Extended &)
{
  std::cout << "Exit(S0) - ";
}

template<>
template<typename SIMPLE>
void test::state::S0::
on_event(test::message::E const &, test::state::Extended & h, SIMPLE const &) const
{
  /* Guard condition (no guard here)*/
  /* optional transition if a state change shall be
   * affected. transition::transition() performs UML exit/ actions.*/
  etl::hsm::transition<SIMPLE, Self, test::state::S211> t(h);
  /* transition action goes here */
  std::cout << "Tran(S0, S211, E) - ";
}

//________________________________________________________________________________________
//    
//  S1
//________________________________________________________________________________________

template<>
inline void test::state::S1::handle_entry(test::state::Extended &)
{
  std::cout << "Entry(S1) - ";
}

template<>
inline void test::state::S1::handle_init(test::state::Extended & arg)
{
  std::cout << "Init(S1) - ";
  etl::hsm::Init<test::state::S11> tmp(arg);
}

template<>
inline void test::state::S1::handle_exit(test::state::Extended &)
{
  std::cout << "Exit(S1) - ";
}

template<>
template<typename SIMPLE>
inline void test::state::S1::
on_event(test::message::A const &, test::state::Extended & h, const SIMPLE &) const
{
  etl::hsm::transition<SIMPLE, Self, test::state::S1> t(h);
  std::cout << "Tran(S1, S1, A) - ";
}

template<>
template<typename SIMPLE>
inline void test::state::S1::
on_event(test::message::B const &, test::state::Extended & h, const SIMPLE &) const
{
  etl::hsm::transition<SIMPLE, Self, test::state::S11> t(h);
  std::cout << "Tran(S1, S11, B) - ";
}

template<>
template<typename SIMPLE>
inline void test::state::S1::
on_event(test::message::C const &, test::state::Extended & h, const SIMPLE &) const
{
  etl::hsm::transition<SIMPLE, Self, test::state::S2> t(h);
  std::cout << "Tran(S1, S2, C) - ";
}

template<>
template<typename SIMPLE>
inline void test::state::S1::
on_event(test::message::D const &, test::state::Extended & h, const SIMPLE &) const
{
  etl::hsm::transition<SIMPLE, Self, test::state::S0> t(h);
  std::cout << "Tran(S1, S0, D) - ";
}

template<>
template<typename SIMPLE>
inline void test::state::S1::
on_event(test::message::F const &, test::state::Extended & h, const SIMPLE &) const
{
  etl::hsm::transition<SIMPLE, Self, test::state::S211> t(h);
  std::cout << "Tran(S1, S211, F) - ";
}

//________________________________________________________________________________________
//    
//  S11
//________________________________________________________________________________________

template<>
inline void test::state::S11::handle_entry(test::state::Extended &)
{
  std::cout << "Entry(S11) - ";
}

template<>
inline void test::state::S11::handle_exit(test::state::Extended &)
{
  std::cout << "Exit(S11) - ";
}

template<>
template<typename SIMPLE>
inline void test::state::S11::
on_event(test::message::G const &, test::state::Extended & h, SIMPLE const &) const
{
  etl::hsm::transition<SIMPLE, Self, test::state::S211> t(h);
  std::cout << "Tran(S11, S211, G) - ";
}

//________________________________________________________________________________________
//    
//  S2
//________________________________________________________________________________________

template<>
inline void test::state::S2::handle_init(test::state::Extended & arg)
{
  etl::hsm::Init<test::state::S21> tmp(arg);
  std::cout << "Init(S2) - ";
}

template<>
inline void test::state::S2::handle_entry(test::state::Extended &)
{
  std::cout << "Entry(S2) - ";
}

template<>
inline void test::state::S2::handle_exit(test::state::Extended &)
{
  std::cout << "Exit(S2) - ";
}

template<>
template<typename SIMPLE>
inline void test::state::S2::
on_event(test::message::C const &, test::state::Extended & h, const SIMPLE &) const
{
  etl::hsm::transition<SIMPLE, Self, test::state::S1> t(h);
  std::cout << "Tran(S2, S1, C) - ";
}

template<>
template<typename SIMPLE>
inline void test::state::S2::
on_event(test::message::F const &, test::state::Extended & h, const SIMPLE &) const
{
  etl::hsm::transition<SIMPLE, Self, test::state::S11> t(h);
  std::cout << "Tran(S2, S11, F) - ";
}

//________________________________________________________________________________________
//    
//  S21
//________________________________________________________________________________________

template<>
inline void test::state::S21::handle_entry(test::state::Extended &)
{
  std::cout << "Entry(S21) - ";
}

template<>
inline void test::state::S21::handle_init(test::state::Extended & arg)
{
  std::cout << "Init(S21) - ";
  etl::hsm::Init<test::state::S211> tmp(arg);
}

template<>
inline void test::state::S21::handle_exit(test::state::Extended &)
{
  std::cout << "Exit(S21) - ";
}

template<>
template<typename SIMPLE>
inline void test::state::S21::
on_event(test::message::B const &, test::state::Extended & h, const SIMPLE &) const
{
  etl::hsm::transition<SIMPLE, Self, test::state::S211> t(h);
  std::cout << "Tran(S21, S211, B) - ";
}

template<>
template<typename SIMPLE>
inline void test::state::S21::
on_event(test::message::H const &, test::state::Extended & h,	const SIMPLE & l) const
{
  if (!h.foo()) {
    etl::hsm::transition<SIMPLE, Self, test::state::S21> t(h);
    std::cout << "!foo/Tran(S21, S21, H) - ";
    h.foo(1);
  }
}

//________________________________________________________________________________________
//    
//  S211
//________________________________________________________________________________________

template<>
inline void test::state::S211::handle_entry(test::state::Extended &)
{
  std::cout << "Entry(S211) - ";
}

template<>
inline void test::state::S211::handle_exit(test::state::Extended &)
{
  std::cout << "Exit(S211) - ";
}

template<>
template<typename SIMPLE>
inline void test::state::S211::
on_event(test::message::D const &, test::state::Extended & h, const SIMPLE &) const
{
  etl::hsm::transition<SIMPLE, Self, test::state::S21> t(h);
  std::cout << "Tran(S211, S21, D) - ";
}

template<>
template<typename SIMPLE>
inline void test::state::S211::
on_event(test::message::G const &, test::state::Extended & h, const SIMPLE &) const
{
  etl::hsm::transition<SIMPLE, Self, test::state::S0> t(h);
  std::cout << "Tran(S211, S0, G) - ";
}

//________________________________________________________________________________________
//    
// main
//________________________________________________________________________________________

int main()
{
    test::state::Extended ext(test::router_id);
    test::state::Top::handle_init(ext);
    for(;;) {
        std::cout << "\nEvent: ";
        char c;
        std::cin >> c;
        if (c < 'a' || 'h' < c)
        {
            return 0;
        }
        ext.receive( test::message::factory(c));
    }
}
