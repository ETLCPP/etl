// 
//  The MIT License(MIT)
//  
//  Embedded Template Library.
//  https://github.com/ETLCPP/etl
//  https://www.etlcpp.com
//  
//  Copyright(c) 2019 Pontus Astrom, 2004 Stefan Heinzmann
//  
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files(the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions :
//  
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
//________________________________________________________________________________________

//________________________________________________________________________________________
//
// THIS FILE IS AUTO-GENERATED. DO NOT EDIT. CHANGES WILL BE OVERWRITTEN !
//________________________________________________________________________________________

//________________________________________________________________________________________
//
//  To generate to header file, run this at the command line.
//  Note: You will need Python and COG installed.
//
//  python -m cogapp -d -e -ohsm.h -DHandlers=<n> hsm_generator.h
//  Where <n> is the number of messages to support.
//
//  e.g.
//  To generate handlers for up to 16 events...
//  python -m cogapp -d -e -ohsm.h -DHandlers=16 hsm_generator.h
//
//  See generate.bat
//________________________________________________________________________________________

//
//  \brief  Definitions, classes for designing hierarchical state machines
//  \author Stefan Heinzmann, Pontus Astrom
//  \date   2004-12-01, modified 2019-08-06
//
//  The implementation is mostly taken from
//  http://accu.org/index.php/journals/252 with small modifications to
//  fix a few bugs, and add some additional UML statechart
//  support. For instance, with this version it is possible to have a composite
//  state as target for a transaction.

#ifndef ETL_HSM_INCLUDED
#define ETL_HSM_INCLUDED

#include "message_router.h"

namespace etl {
namespace hsm {

//________________________________________________________________________________________
//
//  Function for detecting if integer_sequence contains given value
//________________________________________________________________________________________

template <etl::message_id_t ... Is>
bool is_in(etl::message_id_t i, std::integer_sequence< etl::message_id_t, Is... >)
{
    return ((i == Is) || ...);
}

//________________________________________________________________________________________
//
// top
//________________________________________________________________________________________

template<typename E>
struct top
{
    typedef E Extended;
    virtual void process_event(etl::imessage const &, Extended &) const = 0;
    virtual bool accepts_event(etl::message_id_t) const = 0;
    virtual unsigned get_id() const = 0;
};

//________________________________________________________________________________________
//
//  composite declaration for 16 messages
//________________________________________________________________________________________

template<typename E, unsigned ID, typename P, 
         typename M00 = void, typename M01 = void, typename M02 = void,
         typename M03 = void, typename M04 = void, typename M05 = void,
         typename M06 = void, typename M07 = void, typename M08 = void,
         typename M09 = void, typename M10 = void, typename M11 = void,
         typename M12 = void, typename M13 = void, typename M14 = void,
         typename M15 = void>
class composite;

template<typename E, unsigned ID, typename P = composite<E, 0, top<E> >, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07,
         typename M08, typename M09, typename M10, typename M11,
         typename M12, typename M13, typename M14, typename M15>
class composite : public P
{
    public:

    typedef E Extended;
    typedef P Parent;
    typedef composite<E, ID, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11, M12, M13, M14, M15> Self;

    static void handle_entry(Extended &) {}
    static void handle_exit (Extended &) {}
    static void handle_init (Extended &);

    static bool me_accepts_event(etl::message_id_t id)
    {
        switch( id )
        {
            case M00::ID:
            case M01::ID:
            case M02::ID:
            case M03::ID:
            case M04::ID:
            case M05::ID:
            case M06::ID:
            case M07::ID:
            case M08::ID:
            case M09::ID:
            case M10::ID:
            case M11::ID:
            case M12::ID:
            case M13::ID:
            case M14::ID:
            case M15::ID:
                return true;
            default:
                return Parent::accepts_event( id );
        };
    }

    template<typename SIMPLE>
    void handle_event(etl::imessage const & m, Extended & e, SIMPLE const & s) const
    {
        switch ( m.get_message_id() )
        {
            case M00::ID: on_event<SIMPLE>( static_cast<M00 const &>( m ), e, s ); return;
            case M01::ID: on_event<SIMPLE>( static_cast<M01 const &>( m ), e, s ); return;
            case M02::ID: on_event<SIMPLE>( static_cast<M02 const &>( m ), e, s ); return;
            case M03::ID: on_event<SIMPLE>( static_cast<M03 const &>( m ), e, s ); return;
            case M04::ID: on_event<SIMPLE>( static_cast<M04 const &>( m ), e, s ); return;
            case M05::ID: on_event<SIMPLE>( static_cast<M05 const &>( m ), e, s ); return;
            case M06::ID: on_event<SIMPLE>( static_cast<M06 const &>( m ), e, s ); return;
            case M07::ID: on_event<SIMPLE>( static_cast<M07 const &>( m ), e, s ); return;
            case M08::ID: on_event<SIMPLE>( static_cast<M08 const &>( m ), e, s ); return;
            case M09::ID: on_event<SIMPLE>( static_cast<M09 const &>( m ), e, s ); return;
            case M10::ID: on_event<SIMPLE>( static_cast<M10 const &>( m ), e, s ); return;
            case M11::ID: on_event<SIMPLE>( static_cast<M11 const &>( m ), e, s ); return;
            case M12::ID: on_event<SIMPLE>( static_cast<M12 const &>( m ), e, s ); return;
            case M13::ID: on_event<SIMPLE>( static_cast<M13 const &>( m ), e, s ); return;
            case M14::ID: on_event<SIMPLE>( static_cast<M14 const &>( m ), e, s ); return;
            case M15::ID: on_event<SIMPLE>( static_cast<M15 const &>( m ), e, s ); return;
        }
        Parent::handle_event(m, e, s);
    }

    private:

    template<typename SIMPLE> void on_event( M00 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M01 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M02 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M03 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M04 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M05 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M06 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M07 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M08 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M09 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M10 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M11 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M12 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M13 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M14 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M15 const &, Extended &, SIMPLE const & ) const;
};

//________________________________________________________________________________________
//
//  composite specialisation for top-state which has class top as parent
//________________________________________________________________________________________

template<typename E>
class composite<E, 0> : public top<E>
{
    public:

    typedef E Extended;
    typedef top<E> Parent;
    typedef composite<E, 0, top<E> > Self;

    static void handle_entry(Extended &) {}
    static void handle_exit (Extended &) {}
    static void handle_init (Extended &);

    static bool me_accepts_event(etl::message_id_t)
    {
        return false;
    }

    template<typename SIMPLE>
    void handle_event(etl::imessage const & m, Extended & e, SIMPLE const & ) const
    {
        if ( e.has_successor() )
        {
            e.get_successor().receive(m);
        }
        else
        {
            e.on_receive_unknown(m);
        }
    }
};

//________________________________________________________________________________________
//
//  composite specialisation for 15 messages
//________________________________________________________________________________________

template<typename E, unsigned ID, typename P, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07,
         typename M08, typename M09, typename M10, typename M11,
         typename M12, typename M13, typename M14>
class composite<E, ID, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11, M12, M13, M14> : public P
{
    public:

    typedef E Extended;
    typedef P Parent;
    typedef composite<E, ID, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11, M12, M13, M14> Self;

    static void handle_entry(Extended &) {}
    static void handle_exit (Extended &) {}
    static void handle_init (Extended &);

    static bool me_accepts_event(etl::message_id_t id)
    {
        switch( id )
        {
            case M00::ID:
            case M01::ID:
            case M02::ID:
            case M03::ID:
            case M04::ID:
            case M05::ID:
            case M06::ID:
            case M07::ID:
            case M08::ID:
            case M09::ID:
            case M10::ID:
            case M11::ID:
            case M12::ID:
            case M13::ID:
            case M14::ID:
                return true;
            default:
                return Parent::accepts_event( id );
        };
    }

    template<typename SIMPLE>
    void handle_event(etl::imessage const & m, Extended & e, SIMPLE const & s) const
    {
        switch ( m.get_message_id() )
        {
            case M00::ID: on_event<SIMPLE>( static_cast<M00 const &>( m ), e, s ); return;
            case M01::ID: on_event<SIMPLE>( static_cast<M01 const &>( m ), e, s ); return;
            case M02::ID: on_event<SIMPLE>( static_cast<M02 const &>( m ), e, s ); return;
            case M03::ID: on_event<SIMPLE>( static_cast<M03 const &>( m ), e, s ); return;
            case M04::ID: on_event<SIMPLE>( static_cast<M04 const &>( m ), e, s ); return;
            case M05::ID: on_event<SIMPLE>( static_cast<M05 const &>( m ), e, s ); return;
            case M06::ID: on_event<SIMPLE>( static_cast<M06 const &>( m ), e, s ); return;
            case M07::ID: on_event<SIMPLE>( static_cast<M07 const &>( m ), e, s ); return;
            case M08::ID: on_event<SIMPLE>( static_cast<M08 const &>( m ), e, s ); return;
            case M09::ID: on_event<SIMPLE>( static_cast<M09 const &>( m ), e, s ); return;
            case M10::ID: on_event<SIMPLE>( static_cast<M10 const &>( m ), e, s ); return;
            case M11::ID: on_event<SIMPLE>( static_cast<M11 const &>( m ), e, s ); return;
            case M12::ID: on_event<SIMPLE>( static_cast<M12 const &>( m ), e, s ); return;
            case M13::ID: on_event<SIMPLE>( static_cast<M13 const &>( m ), e, s ); return;
            case M14::ID: on_event<SIMPLE>( static_cast<M14 const &>( m ), e, s ); return;
        }
        Parent::handle_event(m, e, s);
    }

    private:

    template<typename SIMPLE> void on_event( M00 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M01 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M02 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M03 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M04 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M05 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M06 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M07 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M08 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M09 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M10 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M11 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M12 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M13 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M14 const &, Extended &, SIMPLE const & ) const;
};

//________________________________________________________________________________________
//
//  composite specialisation for 14 messages
//________________________________________________________________________________________

template<typename E, unsigned ID, typename P, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07,
         typename M08, typename M09, typename M10, typename M11,
         typename M12, typename M13>
class composite<E, ID, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11, M12, M13> : public P
{
    public:

    typedef E Extended;
    typedef P Parent;
    typedef composite<E, ID, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11, M12, M13> Self;

    static void handle_entry(Extended &) {}
    static void handle_exit (Extended &) {}
    static void handle_init (Extended &);

    static bool me_accepts_event(etl::message_id_t id)
    {
        switch( id )
        {
            case M00::ID:
            case M01::ID:
            case M02::ID:
            case M03::ID:
            case M04::ID:
            case M05::ID:
            case M06::ID:
            case M07::ID:
            case M08::ID:
            case M09::ID:
            case M10::ID:
            case M11::ID:
            case M12::ID:
            case M13::ID:
                return true;
            default:
                return Parent::accepts_event( id );
        };
    }

    template<typename SIMPLE>
    void handle_event(etl::imessage const & m, Extended & e, SIMPLE const & s) const
    {
        switch ( m.get_message_id() )
        {
            case M00::ID: on_event<SIMPLE>( static_cast<M00 const &>( m ), e, s ); return;
            case M01::ID: on_event<SIMPLE>( static_cast<M01 const &>( m ), e, s ); return;
            case M02::ID: on_event<SIMPLE>( static_cast<M02 const &>( m ), e, s ); return;
            case M03::ID: on_event<SIMPLE>( static_cast<M03 const &>( m ), e, s ); return;
            case M04::ID: on_event<SIMPLE>( static_cast<M04 const &>( m ), e, s ); return;
            case M05::ID: on_event<SIMPLE>( static_cast<M05 const &>( m ), e, s ); return;
            case M06::ID: on_event<SIMPLE>( static_cast<M06 const &>( m ), e, s ); return;
            case M07::ID: on_event<SIMPLE>( static_cast<M07 const &>( m ), e, s ); return;
            case M08::ID: on_event<SIMPLE>( static_cast<M08 const &>( m ), e, s ); return;
            case M09::ID: on_event<SIMPLE>( static_cast<M09 const &>( m ), e, s ); return;
            case M10::ID: on_event<SIMPLE>( static_cast<M10 const &>( m ), e, s ); return;
            case M11::ID: on_event<SIMPLE>( static_cast<M11 const &>( m ), e, s ); return;
            case M12::ID: on_event<SIMPLE>( static_cast<M12 const &>( m ), e, s ); return;
            case M13::ID: on_event<SIMPLE>( static_cast<M13 const &>( m ), e, s ); return;
        }
        Parent::handle_event(m, e, s);
    }

    private:

    template<typename SIMPLE> void on_event( M00 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M01 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M02 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M03 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M04 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M05 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M06 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M07 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M08 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M09 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M10 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M11 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M12 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M13 const &, Extended &, SIMPLE const & ) const;
};

//________________________________________________________________________________________
//
//  composite specialisation for 13 messages
//________________________________________________________________________________________

template<typename E, unsigned ID, typename P, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07,
         typename M08, typename M09, typename M10, typename M11,
         typename M12>
class composite<E, ID, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11, M12> : public P
{
    public:

    typedef E Extended;
    typedef P Parent;
    typedef composite<E, ID, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11, M12> Self;

    static void handle_entry(Extended &) {}
    static void handle_exit (Extended &) {}
    static void handle_init (Extended &);

    static bool me_accepts_event(etl::message_id_t id)
    {
        switch( id )
        {
            case M00::ID:
            case M01::ID:
            case M02::ID:
            case M03::ID:
            case M04::ID:
            case M05::ID:
            case M06::ID:
            case M07::ID:
            case M08::ID:
            case M09::ID:
            case M10::ID:
            case M11::ID:
            case M12::ID:
                return true;
            default:
                return Parent::accepts_event( id );
        };
    }

    template<typename SIMPLE>
    void handle_event(etl::imessage const & m, Extended & e, SIMPLE const & s) const
    {
        switch ( m.get_message_id() )
        {
            case M00::ID: on_event<SIMPLE>( static_cast<M00 const &>( m ), e, s ); return;
            case M01::ID: on_event<SIMPLE>( static_cast<M01 const &>( m ), e, s ); return;
            case M02::ID: on_event<SIMPLE>( static_cast<M02 const &>( m ), e, s ); return;
            case M03::ID: on_event<SIMPLE>( static_cast<M03 const &>( m ), e, s ); return;
            case M04::ID: on_event<SIMPLE>( static_cast<M04 const &>( m ), e, s ); return;
            case M05::ID: on_event<SIMPLE>( static_cast<M05 const &>( m ), e, s ); return;
            case M06::ID: on_event<SIMPLE>( static_cast<M06 const &>( m ), e, s ); return;
            case M07::ID: on_event<SIMPLE>( static_cast<M07 const &>( m ), e, s ); return;
            case M08::ID: on_event<SIMPLE>( static_cast<M08 const &>( m ), e, s ); return;
            case M09::ID: on_event<SIMPLE>( static_cast<M09 const &>( m ), e, s ); return;
            case M10::ID: on_event<SIMPLE>( static_cast<M10 const &>( m ), e, s ); return;
            case M11::ID: on_event<SIMPLE>( static_cast<M11 const &>( m ), e, s ); return;
            case M12::ID: on_event<SIMPLE>( static_cast<M12 const &>( m ), e, s ); return;
        }
        Parent::handle_event(m, e, s);
    }

    private:

    template<typename SIMPLE> void on_event( M00 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M01 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M02 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M03 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M04 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M05 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M06 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M07 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M08 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M09 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M10 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M11 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M12 const &, Extended &, SIMPLE const & ) const;
};

//________________________________________________________________________________________
//
//  composite specialisation for 12 messages
//________________________________________________________________________________________

template<typename E, unsigned ID, typename P, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07,
         typename M08, typename M09, typename M10, typename M11>
class composite<E, ID, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11> : public P
{
    public:

    typedef E Extended;
    typedef P Parent;
    typedef composite<E, ID, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11> Self;

    static void handle_entry(Extended &) {}
    static void handle_exit (Extended &) {}
    static void handle_init (Extended &);

    static bool me_accepts_event(etl::message_id_t id)
    {
        switch( id )
        {
            case M00::ID:
            case M01::ID:
            case M02::ID:
            case M03::ID:
            case M04::ID:
            case M05::ID:
            case M06::ID:
            case M07::ID:
            case M08::ID:
            case M09::ID:
            case M10::ID:
            case M11::ID:
                return true;
            default:
                return Parent::accepts_event( id );
        };
    }

    template<typename SIMPLE>
    void handle_event(etl::imessage const & m, Extended & e, SIMPLE const & s) const
    {
        switch ( m.get_message_id() )
        {
            case M00::ID: on_event<SIMPLE>( static_cast<M00 const &>( m ), e, s ); return;
            case M01::ID: on_event<SIMPLE>( static_cast<M01 const &>( m ), e, s ); return;
            case M02::ID: on_event<SIMPLE>( static_cast<M02 const &>( m ), e, s ); return;
            case M03::ID: on_event<SIMPLE>( static_cast<M03 const &>( m ), e, s ); return;
            case M04::ID: on_event<SIMPLE>( static_cast<M04 const &>( m ), e, s ); return;
            case M05::ID: on_event<SIMPLE>( static_cast<M05 const &>( m ), e, s ); return;
            case M06::ID: on_event<SIMPLE>( static_cast<M06 const &>( m ), e, s ); return;
            case M07::ID: on_event<SIMPLE>( static_cast<M07 const &>( m ), e, s ); return;
            case M08::ID: on_event<SIMPLE>( static_cast<M08 const &>( m ), e, s ); return;
            case M09::ID: on_event<SIMPLE>( static_cast<M09 const &>( m ), e, s ); return;
            case M10::ID: on_event<SIMPLE>( static_cast<M10 const &>( m ), e, s ); return;
            case M11::ID: on_event<SIMPLE>( static_cast<M11 const &>( m ), e, s ); return;
        }
        Parent::handle_event(m, e, s);
    }

    private:

    template<typename SIMPLE> void on_event( M00 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M01 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M02 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M03 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M04 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M05 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M06 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M07 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M08 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M09 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M10 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M11 const &, Extended &, SIMPLE const & ) const;
};

//________________________________________________________________________________________
//
//  composite specialisation for 11 messages
//________________________________________________________________________________________

template<typename E, unsigned ID, typename P, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07,
         typename M08, typename M09, typename M10>
class composite<E, ID, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10> : public P
{
    public:

    typedef E Extended;
    typedef P Parent;
    typedef composite<E, ID, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10> Self;

    static void handle_entry(Extended &) {}
    static void handle_exit (Extended &) {}
    static void handle_init (Extended &);

    static bool me_accepts_event(etl::message_id_t id)
    {
        switch( id )
        {
            case M00::ID:
            case M01::ID:
            case M02::ID:
            case M03::ID:
            case M04::ID:
            case M05::ID:
            case M06::ID:
            case M07::ID:
            case M08::ID:
            case M09::ID:
            case M10::ID:
                return true;
            default:
                return Parent::accepts_event( id );
        };
    }

    template<typename SIMPLE>
    void handle_event(etl::imessage const & m, Extended & e, SIMPLE const & s) const
    {
        switch ( m.get_message_id() )
        {
            case M00::ID: on_event<SIMPLE>( static_cast<M00 const &>( m ), e, s ); return;
            case M01::ID: on_event<SIMPLE>( static_cast<M01 const &>( m ), e, s ); return;
            case M02::ID: on_event<SIMPLE>( static_cast<M02 const &>( m ), e, s ); return;
            case M03::ID: on_event<SIMPLE>( static_cast<M03 const &>( m ), e, s ); return;
            case M04::ID: on_event<SIMPLE>( static_cast<M04 const &>( m ), e, s ); return;
            case M05::ID: on_event<SIMPLE>( static_cast<M05 const &>( m ), e, s ); return;
            case M06::ID: on_event<SIMPLE>( static_cast<M06 const &>( m ), e, s ); return;
            case M07::ID: on_event<SIMPLE>( static_cast<M07 const &>( m ), e, s ); return;
            case M08::ID: on_event<SIMPLE>( static_cast<M08 const &>( m ), e, s ); return;
            case M09::ID: on_event<SIMPLE>( static_cast<M09 const &>( m ), e, s ); return;
            case M10::ID: on_event<SIMPLE>( static_cast<M10 const &>( m ), e, s ); return;
        }
        Parent::handle_event(m, e, s);
    }

    private:

    template<typename SIMPLE> void on_event( M00 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M01 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M02 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M03 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M04 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M05 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M06 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M07 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M08 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M09 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M10 const &, Extended &, SIMPLE const & ) const;
};

//________________________________________________________________________________________
//
//  composite specialisation for 10 messages
//________________________________________________________________________________________

template<typename E, unsigned ID, typename P, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07,
         typename M08, typename M09>
class composite<E, ID, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09> : public P
{
    public:

    typedef E Extended;
    typedef P Parent;
    typedef composite<E, ID, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09> Self;

    static void handle_entry(Extended &) {}
    static void handle_exit (Extended &) {}
    static void handle_init (Extended &);

    static bool me_accepts_event(etl::message_id_t id)
    {
        switch( id )
        {
            case M00::ID:
            case M01::ID:
            case M02::ID:
            case M03::ID:
            case M04::ID:
            case M05::ID:
            case M06::ID:
            case M07::ID:
            case M08::ID:
            case M09::ID:
                return true;
            default:
                return Parent::accepts_event( id );
        };
    }

    template<typename SIMPLE>
    void handle_event(etl::imessage const & m, Extended & e, SIMPLE const & s) const
    {
        switch ( m.get_message_id() )
        {
            case M00::ID: on_event<SIMPLE>( static_cast<M00 const &>( m ), e, s ); return;
            case M01::ID: on_event<SIMPLE>( static_cast<M01 const &>( m ), e, s ); return;
            case M02::ID: on_event<SIMPLE>( static_cast<M02 const &>( m ), e, s ); return;
            case M03::ID: on_event<SIMPLE>( static_cast<M03 const &>( m ), e, s ); return;
            case M04::ID: on_event<SIMPLE>( static_cast<M04 const &>( m ), e, s ); return;
            case M05::ID: on_event<SIMPLE>( static_cast<M05 const &>( m ), e, s ); return;
            case M06::ID: on_event<SIMPLE>( static_cast<M06 const &>( m ), e, s ); return;
            case M07::ID: on_event<SIMPLE>( static_cast<M07 const &>( m ), e, s ); return;
            case M08::ID: on_event<SIMPLE>( static_cast<M08 const &>( m ), e, s ); return;
            case M09::ID: on_event<SIMPLE>( static_cast<M09 const &>( m ), e, s ); return;
        }
        Parent::handle_event(m, e, s);
    }

    private:

    template<typename SIMPLE> void on_event( M00 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M01 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M02 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M03 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M04 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M05 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M06 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M07 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M08 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M09 const &, Extended &, SIMPLE const & ) const;
};

//________________________________________________________________________________________
//
//  composite specialisation for 9 messages
//________________________________________________________________________________________

template<typename E, unsigned ID, typename P, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07,
         typename M08>
class composite<E, ID, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08> : public P
{
    public:

    typedef E Extended;
    typedef P Parent;
    typedef composite<E, ID, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08> Self;

    static void handle_entry(Extended &) {}
    static void handle_exit (Extended &) {}
    static void handle_init (Extended &);

    static bool me_accepts_event(etl::message_id_t id)
    {
        switch( id )
        {
            case M00::ID:
            case M01::ID:
            case M02::ID:
            case M03::ID:
            case M04::ID:
            case M05::ID:
            case M06::ID:
            case M07::ID:
            case M08::ID:
                return true;
            default:
                return Parent::accepts_event( id );
        };
    }

    template<typename SIMPLE>
    void handle_event(etl::imessage const & m, Extended & e, SIMPLE const & s) const
    {
        switch ( m.get_message_id() )
        {
            case M00::ID: on_event<SIMPLE>( static_cast<M00 const &>( m ), e, s ); return;
            case M01::ID: on_event<SIMPLE>( static_cast<M01 const &>( m ), e, s ); return;
            case M02::ID: on_event<SIMPLE>( static_cast<M02 const &>( m ), e, s ); return;
            case M03::ID: on_event<SIMPLE>( static_cast<M03 const &>( m ), e, s ); return;
            case M04::ID: on_event<SIMPLE>( static_cast<M04 const &>( m ), e, s ); return;
            case M05::ID: on_event<SIMPLE>( static_cast<M05 const &>( m ), e, s ); return;
            case M06::ID: on_event<SIMPLE>( static_cast<M06 const &>( m ), e, s ); return;
            case M07::ID: on_event<SIMPLE>( static_cast<M07 const &>( m ), e, s ); return;
            case M08::ID: on_event<SIMPLE>( static_cast<M08 const &>( m ), e, s ); return;
        }
        Parent::handle_event(m, e, s);
    }

    private:

    template<typename SIMPLE> void on_event( M00 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M01 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M02 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M03 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M04 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M05 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M06 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M07 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M08 const &, Extended &, SIMPLE const & ) const;
};

//________________________________________________________________________________________
//
//  composite specialisation for 8 messages
//________________________________________________________________________________________

template<typename E, unsigned ID, typename P, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07>
class composite<E, ID, P, 
         M00, M01, M02, M03, M04, M05, M06, M07> : public P
{
    public:

    typedef E Extended;
    typedef P Parent;
    typedef composite<E, ID, P, 
         M00, M01, M02, M03, M04, M05, M06, M07> Self;

    static void handle_entry(Extended &) {}
    static void handle_exit (Extended &) {}
    static void handle_init (Extended &);

    static bool me_accepts_event(etl::message_id_t id)
    {
        switch( id )
        {
            case M00::ID:
            case M01::ID:
            case M02::ID:
            case M03::ID:
            case M04::ID:
            case M05::ID:
            case M06::ID:
            case M07::ID:
                return true;
            default:
                return Parent::accepts_event( id );
        };
    }

    template<typename SIMPLE>
    void handle_event(etl::imessage const & m, Extended & e, SIMPLE const & s) const
    {
        switch ( m.get_message_id() )
        {
            case M00::ID: on_event<SIMPLE>( static_cast<M00 const &>( m ), e, s ); return;
            case M01::ID: on_event<SIMPLE>( static_cast<M01 const &>( m ), e, s ); return;
            case M02::ID: on_event<SIMPLE>( static_cast<M02 const &>( m ), e, s ); return;
            case M03::ID: on_event<SIMPLE>( static_cast<M03 const &>( m ), e, s ); return;
            case M04::ID: on_event<SIMPLE>( static_cast<M04 const &>( m ), e, s ); return;
            case M05::ID: on_event<SIMPLE>( static_cast<M05 const &>( m ), e, s ); return;
            case M06::ID: on_event<SIMPLE>( static_cast<M06 const &>( m ), e, s ); return;
            case M07::ID: on_event<SIMPLE>( static_cast<M07 const &>( m ), e, s ); return;
        }
        Parent::handle_event(m, e, s);
    }

    private:

    template<typename SIMPLE> void on_event( M00 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M01 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M02 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M03 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M04 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M05 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M06 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M07 const &, Extended &, SIMPLE const & ) const;
};

//________________________________________________________________________________________
//
//  composite specialisation for 7 messages
//________________________________________________________________________________________

template<typename E, unsigned ID, typename P, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06>
class composite<E, ID, P, 
         M00, M01, M02, M03, M04, M05, M06> : public P
{
    public:

    typedef E Extended;
    typedef P Parent;
    typedef composite<E, ID, P, 
         M00, M01, M02, M03, M04, M05, M06> Self;

    static void handle_entry(Extended &) {}
    static void handle_exit (Extended &) {}
    static void handle_init (Extended &);

    static bool me_accepts_event(etl::message_id_t id)
    {
        switch( id )
        {
            case M00::ID:
            case M01::ID:
            case M02::ID:
            case M03::ID:
            case M04::ID:
            case M05::ID:
            case M06::ID:
                return true;
            default:
                return Parent::accepts_event( id );
        };
    }

    template<typename SIMPLE>
    void handle_event(etl::imessage const & m, Extended & e, SIMPLE const & s) const
    {
        switch ( m.get_message_id() )
        {
            case M00::ID: on_event<SIMPLE>( static_cast<M00 const &>( m ), e, s ); return;
            case M01::ID: on_event<SIMPLE>( static_cast<M01 const &>( m ), e, s ); return;
            case M02::ID: on_event<SIMPLE>( static_cast<M02 const &>( m ), e, s ); return;
            case M03::ID: on_event<SIMPLE>( static_cast<M03 const &>( m ), e, s ); return;
            case M04::ID: on_event<SIMPLE>( static_cast<M04 const &>( m ), e, s ); return;
            case M05::ID: on_event<SIMPLE>( static_cast<M05 const &>( m ), e, s ); return;
            case M06::ID: on_event<SIMPLE>( static_cast<M06 const &>( m ), e, s ); return;
        }
        Parent::handle_event(m, e, s);
    }

    private:

    template<typename SIMPLE> void on_event( M00 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M01 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M02 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M03 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M04 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M05 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M06 const &, Extended &, SIMPLE const & ) const;
};

//________________________________________________________________________________________
//
//  composite specialisation for 6 messages
//________________________________________________________________________________________

template<typename E, unsigned ID, typename P, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05>
class composite<E, ID, P, 
         M00, M01, M02, M03, M04, M05> : public P
{
    public:

    typedef E Extended;
    typedef P Parent;
    typedef composite<E, ID, P, 
         M00, M01, M02, M03, M04, M05> Self;

    static void handle_entry(Extended &) {}
    static void handle_exit (Extended &) {}
    static void handle_init (Extended &);

    static bool me_accepts_event(etl::message_id_t id)
    {
        switch( id )
        {
            case M00::ID:
            case M01::ID:
            case M02::ID:
            case M03::ID:
            case M04::ID:
            case M05::ID:
                return true;
            default:
                return Parent::accepts_event( id );
        };
    }

    template<typename SIMPLE>
    void handle_event(etl::imessage const & m, Extended & e, SIMPLE const & s) const
    {
        switch ( m.get_message_id() )
        {
            case M00::ID: on_event<SIMPLE>( static_cast<M00 const &>( m ), e, s ); return;
            case M01::ID: on_event<SIMPLE>( static_cast<M01 const &>( m ), e, s ); return;
            case M02::ID: on_event<SIMPLE>( static_cast<M02 const &>( m ), e, s ); return;
            case M03::ID: on_event<SIMPLE>( static_cast<M03 const &>( m ), e, s ); return;
            case M04::ID: on_event<SIMPLE>( static_cast<M04 const &>( m ), e, s ); return;
            case M05::ID: on_event<SIMPLE>( static_cast<M05 const &>( m ), e, s ); return;
        }
        Parent::handle_event(m, e, s);
    }

    private:

    template<typename SIMPLE> void on_event( M00 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M01 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M02 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M03 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M04 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M05 const &, Extended &, SIMPLE const & ) const;
};

//________________________________________________________________________________________
//
//  composite specialisation for 5 messages
//________________________________________________________________________________________

template<typename E, unsigned ID, typename P, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04>
class composite<E, ID, P, 
         M00, M01, M02, M03, M04> : public P
{
    public:

    typedef E Extended;
    typedef P Parent;
    typedef composite<E, ID, P, 
         M00, M01, M02, M03, M04> Self;

    static void handle_entry(Extended &) {}
    static void handle_exit (Extended &) {}
    static void handle_init (Extended &);

    static bool me_accepts_event(etl::message_id_t id)
    {
        switch( id )
        {
            case M00::ID:
            case M01::ID:
            case M02::ID:
            case M03::ID:
            case M04::ID:
                return true;
            default:
                return Parent::accepts_event( id );
        };
    }

    template<typename SIMPLE>
    void handle_event(etl::imessage const & m, Extended & e, SIMPLE const & s) const
    {
        switch ( m.get_message_id() )
        {
            case M00::ID: on_event<SIMPLE>( static_cast<M00 const &>( m ), e, s ); return;
            case M01::ID: on_event<SIMPLE>( static_cast<M01 const &>( m ), e, s ); return;
            case M02::ID: on_event<SIMPLE>( static_cast<M02 const &>( m ), e, s ); return;
            case M03::ID: on_event<SIMPLE>( static_cast<M03 const &>( m ), e, s ); return;
            case M04::ID: on_event<SIMPLE>( static_cast<M04 const &>( m ), e, s ); return;
        }
        Parent::handle_event(m, e, s);
    }

    private:

    template<typename SIMPLE> void on_event( M00 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M01 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M02 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M03 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M04 const &, Extended &, SIMPLE const & ) const;
};

//________________________________________________________________________________________
//
//  composite specialisation for 4 messages
//________________________________________________________________________________________

template<typename E, unsigned ID, typename P, 
         typename M00, typename M01, typename M02, typename M03>
class composite<E, ID, P, 
         M00, M01, M02, M03> : public P
{
    public:

    typedef E Extended;
    typedef P Parent;
    typedef composite<E, ID, P, 
         M00, M01, M02, M03> Self;

    static void handle_entry(Extended &) {}
    static void handle_exit (Extended &) {}
    static void handle_init (Extended &);

    static bool me_accepts_event(etl::message_id_t id)
    {
        switch( id )
        {
            case M00::ID:
            case M01::ID:
            case M02::ID:
            case M03::ID:
                return true;
            default:
                return Parent::accepts_event( id );
        };
    }

    template<typename SIMPLE>
    void handle_event(etl::imessage const & m, Extended & e, SIMPLE const & s) const
    {
        switch ( m.get_message_id() )
        {
            case M00::ID: on_event<SIMPLE>( static_cast<M00 const &>( m ), e, s ); return;
            case M01::ID: on_event<SIMPLE>( static_cast<M01 const &>( m ), e, s ); return;
            case M02::ID: on_event<SIMPLE>( static_cast<M02 const &>( m ), e, s ); return;
            case M03::ID: on_event<SIMPLE>( static_cast<M03 const &>( m ), e, s ); return;
        }
        Parent::handle_event(m, e, s);
    }

    private:

    template<typename SIMPLE> void on_event( M00 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M01 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M02 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M03 const &, Extended &, SIMPLE const & ) const;
};

//________________________________________________________________________________________
//
//  composite specialisation for 3 messages
//________________________________________________________________________________________

template<typename E, unsigned ID, typename P, 
         typename M00, typename M01, typename M02>
class composite<E, ID, P, 
         M00, M01, M02> : public P
{
    public:

    typedef E Extended;
    typedef P Parent;
    typedef composite<E, ID, P, 
         M00, M01, M02> Self;

    static void handle_entry(Extended &) {}
    static void handle_exit (Extended &) {}
    static void handle_init (Extended &);

    static bool me_accepts_event(etl::message_id_t id)
    {
        switch( id )
        {
            case M00::ID:
            case M01::ID:
            case M02::ID:
                return true;
            default:
                return Parent::accepts_event( id );
        };
    }

    template<typename SIMPLE>
    void handle_event(etl::imessage const & m, Extended & e, SIMPLE const & s) const
    {
        switch ( m.get_message_id() )
        {
            case M00::ID: on_event<SIMPLE>( static_cast<M00 const &>( m ), e, s ); return;
            case M01::ID: on_event<SIMPLE>( static_cast<M01 const &>( m ), e, s ); return;
            case M02::ID: on_event<SIMPLE>( static_cast<M02 const &>( m ), e, s ); return;
        }
        Parent::handle_event(m, e, s);
    }

    private:

    template<typename SIMPLE> void on_event( M00 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M01 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M02 const &, Extended &, SIMPLE const & ) const;
};

//________________________________________________________________________________________
//
//  composite specialisation for 2 messages
//________________________________________________________________________________________

template<typename E, unsigned ID, typename P, 
         typename M00, typename M01>
class composite<E, ID, P, 
         M00, M01> : public P
{
    public:

    typedef E Extended;
    typedef P Parent;
    typedef composite<E, ID, P, 
         M00, M01> Self;

    static void handle_entry(Extended &) {}
    static void handle_exit (Extended &) {}
    static void handle_init (Extended &);

    static bool me_accepts_event(etl::message_id_t id)
    {
        switch( id )
        {
            case M00::ID:
            case M01::ID:
                return true;
            default:
                return Parent::accepts_event( id );
        };
    }

    template<typename SIMPLE>
    void handle_event(etl::imessage const & m, Extended & e, SIMPLE const & s) const
    {
        switch ( m.get_message_id() )
        {
            case M00::ID: on_event<SIMPLE>( static_cast<M00 const &>( m ), e, s ); return;
            case M01::ID: on_event<SIMPLE>( static_cast<M01 const &>( m ), e, s ); return;
        }
        Parent::handle_event(m, e, s);
    }

    private:

    template<typename SIMPLE> void on_event( M00 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M01 const &, Extended &, SIMPLE const & ) const;
};

//________________________________________________________________________________________
//
//  composite specialisation for 1 messages
//________________________________________________________________________________________

template<typename E, unsigned ID, typename P, 
         typename M00>
class composite<E, ID, P, 
         M00> : public P
{
    public:

    typedef E Extended;
    typedef P Parent;
    typedef composite<E, ID, P, 
         M00> Self;

    static void handle_entry(Extended &) {}
    static void handle_exit (Extended &) {}
    static void handle_init (Extended &);

    static bool me_accepts_event(etl::message_id_t id)
    {
        switch( id )
        {
            case M00::ID:
                return true;
            default:
                return Parent::accepts_event( id );
        };
    }

    template<typename SIMPLE>
    void handle_event(etl::imessage const & m, Extended & e, SIMPLE const & s) const
    {
        switch ( m.get_message_id() )
        {
            case M00::ID: on_event<SIMPLE>( static_cast<M00 const &>( m ), e, s ); return;
        }
        Parent::handle_event(m, e, s);
    }

    private:

    template<typename SIMPLE> void on_event( M00 const &, Extended &, SIMPLE const & ) const;
};

//________________________________________________________________________________________
//
//  simple declaration for 16 messages
//________________________________________________________________________________________

template<typename E, unsigned ID, typename A, typename P, 
         typename M00 = void, typename M01 = void, typename M02 = void,
         typename M03 = void, typename M04 = void, typename M05 = void,
         typename M06 = void, typename M07 = void, typename M08 = void,
         typename M09 = void, typename M10 = void, typename M11 = void,
         typename M12 = void, typename M13 = void, typename M14 = void,
         typename M15 = void>
class simple;

template<typename E, unsigned ID, typename A, typename P = composite<E, 0, top<E> >, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07,
         typename M08, typename M09, typename M10, typename M11,
         typename M12, typename M13, typename M14, typename M15>
class simple : public P
{
    public:

    typedef E Extended;
    typedef P Parent;
    typedef A Allowed_ids;
    typedef simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11, M12, M13, M14, M15> Self;

    static void handle_entry(Extended &) {}
    static void handle_exit (Extended &) {}
    static void handle_init (Extended & arg)
    {
        arg.set_state(obj);
    }

    static const simple obj;

    virtual unsigned get_id() const override
    {
        return ID;
    }

    virtual void process_event(etl::imessage const & m, Extended & e) const override
    {
        handle_event(m, e, *this);
    }

    static bool me_accepts_event(etl::message_id_t id)
    {
        switch( id )
        {
            case M00::ID:
            case M01::ID:
            case M02::ID:
            case M03::ID:
            case M04::ID:
            case M05::ID:
            case M06::ID:
            case M07::ID:
            case M08::ID:
            case M09::ID:
            case M10::ID:
            case M11::ID:
            case M12::ID:
            case M13::ID:
            case M14::ID:
            case M15::ID:
                return true;
            default:
                return Parent::accepts_event( id );
        };
    }

    bool accepts_event(etl::message_id_t arg) const override
    {                                                                      
         return is_in( arg, Allowed_ids {} );                                           
    }                                                                      

    template<typename SIMPLE>
    void handle_event(etl::imessage const & m, Extended & e, SIMPLE const & s) const
    {
        switch ( m.get_message_id() )
        {
            case M00::ID: on_event<SIMPLE>( static_cast<M00 const &>( m ), e, s ); return;
            case M01::ID: on_event<SIMPLE>( static_cast<M01 const &>( m ), e, s ); return;
            case M02::ID: on_event<SIMPLE>( static_cast<M02 const &>( m ), e, s ); return;
            case M03::ID: on_event<SIMPLE>( static_cast<M03 const &>( m ), e, s ); return;
            case M04::ID: on_event<SIMPLE>( static_cast<M04 const &>( m ), e, s ); return;
            case M05::ID: on_event<SIMPLE>( static_cast<M05 const &>( m ), e, s ); return;
            case M06::ID: on_event<SIMPLE>( static_cast<M06 const &>( m ), e, s ); return;
            case M07::ID: on_event<SIMPLE>( static_cast<M07 const &>( m ), e, s ); return;
            case M08::ID: on_event<SIMPLE>( static_cast<M08 const &>( m ), e, s ); return;
            case M09::ID: on_event<SIMPLE>( static_cast<M09 const &>( m ), e, s ); return;
            case M10::ID: on_event<SIMPLE>( static_cast<M10 const &>( m ), e, s ); return;
            case M11::ID: on_event<SIMPLE>( static_cast<M11 const &>( m ), e, s ); return;
            case M12::ID: on_event<SIMPLE>( static_cast<M12 const &>( m ), e, s ); return;
            case M13::ID: on_event<SIMPLE>( static_cast<M13 const &>( m ), e, s ); return;
            case M14::ID: on_event<SIMPLE>( static_cast<M14 const &>( m ), e, s ); return;
            case M15::ID: on_event<SIMPLE>( static_cast<M15 const &>( m ), e, s ); return;
        }
        Parent::handle_event(m, e, s);
    }

    private:

    template<typename SIMPLE> void on_event( M00 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M01 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M02 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M03 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M04 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M05 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M06 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M07 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M08 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M09 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M10 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M11 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M12 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M13 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M14 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M15 const &, Extended &, SIMPLE const & ) const;
};

template<typename E, unsigned ID, typename A, typename P, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07,
         typename M08, typename M09, typename M10, typename M11,
         typename M12, typename M13, typename M14, typename M15>
const simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11, M12, M13, M14, M15>
      simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11, M12, M13, M14, M15>::obj;

//________________________________________________________________________________________
//
//  simple specialisation for 15 messages
//________________________________________________________________________________________

template<typename E, unsigned ID, typename A, typename P, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07,
         typename M08, typename M09, typename M10, typename M11,
         typename M12, typename M13, typename M14>
class simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11, M12, M13, M14> : public P
{
    public:

    typedef E Extended;
    typedef P Parent;
    typedef A Allowed_ids;
    typedef simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11, M12, M13, M14> Self;

    static void handle_entry(Extended &) {}
    static void handle_exit (Extended &) {}
    static void handle_init (Extended & arg)
    {
        arg.set_state(obj);
    }

    static const simple obj;

    virtual unsigned get_id() const override
    {
        return ID;
    }

    virtual void process_event(etl::imessage const & m, Extended & e) const override
    {
        handle_event(m, e, *this);
    }

    static bool me_accepts_event(etl::message_id_t id)
    {
        switch( id )
        {
            case M00::ID:
            case M01::ID:
            case M02::ID:
            case M03::ID:
            case M04::ID:
            case M05::ID:
            case M06::ID:
            case M07::ID:
            case M08::ID:
            case M09::ID:
            case M10::ID:
            case M11::ID:
            case M12::ID:
            case M13::ID:
            case M14::ID:
                return true;
            default:
                return Parent::accepts_event( id );
        };
    }

    bool accepts_event(etl::message_id_t arg) const override
    {                                                                      
         return is_in( arg, Allowed_ids {} );                                           
    }                                                                      

    template<typename SIMPLE>
    void handle_event(etl::imessage const & m, Extended & e, SIMPLE const & s) const
    {
        switch ( m.get_message_id() )
        {
            case M00::ID: on_event<SIMPLE>( static_cast<M00 const &>( m ), e, s ); return;
            case M01::ID: on_event<SIMPLE>( static_cast<M01 const &>( m ), e, s ); return;
            case M02::ID: on_event<SIMPLE>( static_cast<M02 const &>( m ), e, s ); return;
            case M03::ID: on_event<SIMPLE>( static_cast<M03 const &>( m ), e, s ); return;
            case M04::ID: on_event<SIMPLE>( static_cast<M04 const &>( m ), e, s ); return;
            case M05::ID: on_event<SIMPLE>( static_cast<M05 const &>( m ), e, s ); return;
            case M06::ID: on_event<SIMPLE>( static_cast<M06 const &>( m ), e, s ); return;
            case M07::ID: on_event<SIMPLE>( static_cast<M07 const &>( m ), e, s ); return;
            case M08::ID: on_event<SIMPLE>( static_cast<M08 const &>( m ), e, s ); return;
            case M09::ID: on_event<SIMPLE>( static_cast<M09 const &>( m ), e, s ); return;
            case M10::ID: on_event<SIMPLE>( static_cast<M10 const &>( m ), e, s ); return;
            case M11::ID: on_event<SIMPLE>( static_cast<M11 const &>( m ), e, s ); return;
            case M12::ID: on_event<SIMPLE>( static_cast<M12 const &>( m ), e, s ); return;
            case M13::ID: on_event<SIMPLE>( static_cast<M13 const &>( m ), e, s ); return;
            case M14::ID: on_event<SIMPLE>( static_cast<M14 const &>( m ), e, s ); return;
        }
        Parent::handle_event(m, e, s);
    }

    private:

    template<typename SIMPLE> void on_event( M00 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M01 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M02 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M03 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M04 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M05 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M06 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M07 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M08 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M09 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M10 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M11 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M12 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M13 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M14 const &, Extended &, SIMPLE const & ) const;
};

template<typename E, unsigned ID, typename A, typename P, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07,
         typename M08, typename M09, typename M10, typename M11,
         typename M12, typename M13, typename M14>
const simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11, M12, M13, M14>
      simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11, M12, M13, M14>::obj;

//________________________________________________________________________________________
//
//  simple specialisation for 14 messages
//________________________________________________________________________________________

template<typename E, unsigned ID, typename A, typename P, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07,
         typename M08, typename M09, typename M10, typename M11,
         typename M12, typename M13>
class simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11, M12, M13> : public P
{
    public:

    typedef E Extended;
    typedef P Parent;
    typedef A Allowed_ids;
    typedef simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11, M12, M13> Self;

    static void handle_entry(Extended &) {}
    static void handle_exit (Extended &) {}
    static void handle_init (Extended & arg)
    {
        arg.set_state(obj);
    }

    static const simple obj;

    virtual unsigned get_id() const override
    {
        return ID;
    }

    virtual void process_event(etl::imessage const & m, Extended & e) const override
    {
        handle_event(m, e, *this);
    }

    static bool me_accepts_event(etl::message_id_t id)
    {
        switch( id )
        {
            case M00::ID:
            case M01::ID:
            case M02::ID:
            case M03::ID:
            case M04::ID:
            case M05::ID:
            case M06::ID:
            case M07::ID:
            case M08::ID:
            case M09::ID:
            case M10::ID:
            case M11::ID:
            case M12::ID:
            case M13::ID:
                return true;
            default:
                return Parent::accepts_event( id );
        };
    }

    bool accepts_event(etl::message_id_t arg) const override
    {                                                                      
         return is_in( arg, Allowed_ids {} );                                           
    }                                                                      

    template<typename SIMPLE>
    void handle_event(etl::imessage const & m, Extended & e, SIMPLE const & s) const
    {
        switch ( m.get_message_id() )
        {
            case M00::ID: on_event<SIMPLE>( static_cast<M00 const &>( m ), e, s ); return;
            case M01::ID: on_event<SIMPLE>( static_cast<M01 const &>( m ), e, s ); return;
            case M02::ID: on_event<SIMPLE>( static_cast<M02 const &>( m ), e, s ); return;
            case M03::ID: on_event<SIMPLE>( static_cast<M03 const &>( m ), e, s ); return;
            case M04::ID: on_event<SIMPLE>( static_cast<M04 const &>( m ), e, s ); return;
            case M05::ID: on_event<SIMPLE>( static_cast<M05 const &>( m ), e, s ); return;
            case M06::ID: on_event<SIMPLE>( static_cast<M06 const &>( m ), e, s ); return;
            case M07::ID: on_event<SIMPLE>( static_cast<M07 const &>( m ), e, s ); return;
            case M08::ID: on_event<SIMPLE>( static_cast<M08 const &>( m ), e, s ); return;
            case M09::ID: on_event<SIMPLE>( static_cast<M09 const &>( m ), e, s ); return;
            case M10::ID: on_event<SIMPLE>( static_cast<M10 const &>( m ), e, s ); return;
            case M11::ID: on_event<SIMPLE>( static_cast<M11 const &>( m ), e, s ); return;
            case M12::ID: on_event<SIMPLE>( static_cast<M12 const &>( m ), e, s ); return;
            case M13::ID: on_event<SIMPLE>( static_cast<M13 const &>( m ), e, s ); return;
        }
        Parent::handle_event(m, e, s);
    }

    private:

    template<typename SIMPLE> void on_event( M00 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M01 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M02 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M03 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M04 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M05 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M06 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M07 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M08 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M09 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M10 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M11 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M12 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M13 const &, Extended &, SIMPLE const & ) const;
};

template<typename E, unsigned ID, typename A, typename P, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07,
         typename M08, typename M09, typename M10, typename M11,
         typename M12, typename M13>
const simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11, M12, M13>
      simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11, M12, M13>::obj;

//________________________________________________________________________________________
//
//  simple specialisation for 13 messages
//________________________________________________________________________________________

template<typename E, unsigned ID, typename A, typename P, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07,
         typename M08, typename M09, typename M10, typename M11,
         typename M12>
class simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11, M12> : public P
{
    public:

    typedef E Extended;
    typedef P Parent;
    typedef A Allowed_ids;
    typedef simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11, M12> Self;

    static void handle_entry(Extended &) {}
    static void handle_exit (Extended &) {}
    static void handle_init (Extended & arg)
    {
        arg.set_state(obj);
    }

    static const simple obj;

    virtual unsigned get_id() const override
    {
        return ID;
    }

    virtual void process_event(etl::imessage const & m, Extended & e) const override
    {
        handle_event(m, e, *this);
    }

    static bool me_accepts_event(etl::message_id_t id)
    {
        switch( id )
        {
            case M00::ID:
            case M01::ID:
            case M02::ID:
            case M03::ID:
            case M04::ID:
            case M05::ID:
            case M06::ID:
            case M07::ID:
            case M08::ID:
            case M09::ID:
            case M10::ID:
            case M11::ID:
            case M12::ID:
                return true;
            default:
                return Parent::accepts_event( id );
        };
    }

    bool accepts_event(etl::message_id_t arg) const override
    {                                                                      
         return is_in( arg, Allowed_ids {} );                                           
    }                                                                      

    template<typename SIMPLE>
    void handle_event(etl::imessage const & m, Extended & e, SIMPLE const & s) const
    {
        switch ( m.get_message_id() )
        {
            case M00::ID: on_event<SIMPLE>( static_cast<M00 const &>( m ), e, s ); return;
            case M01::ID: on_event<SIMPLE>( static_cast<M01 const &>( m ), e, s ); return;
            case M02::ID: on_event<SIMPLE>( static_cast<M02 const &>( m ), e, s ); return;
            case M03::ID: on_event<SIMPLE>( static_cast<M03 const &>( m ), e, s ); return;
            case M04::ID: on_event<SIMPLE>( static_cast<M04 const &>( m ), e, s ); return;
            case M05::ID: on_event<SIMPLE>( static_cast<M05 const &>( m ), e, s ); return;
            case M06::ID: on_event<SIMPLE>( static_cast<M06 const &>( m ), e, s ); return;
            case M07::ID: on_event<SIMPLE>( static_cast<M07 const &>( m ), e, s ); return;
            case M08::ID: on_event<SIMPLE>( static_cast<M08 const &>( m ), e, s ); return;
            case M09::ID: on_event<SIMPLE>( static_cast<M09 const &>( m ), e, s ); return;
            case M10::ID: on_event<SIMPLE>( static_cast<M10 const &>( m ), e, s ); return;
            case M11::ID: on_event<SIMPLE>( static_cast<M11 const &>( m ), e, s ); return;
            case M12::ID: on_event<SIMPLE>( static_cast<M12 const &>( m ), e, s ); return;
        }
        Parent::handle_event(m, e, s);
    }

    private:

    template<typename SIMPLE> void on_event( M00 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M01 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M02 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M03 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M04 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M05 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M06 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M07 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M08 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M09 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M10 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M11 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M12 const &, Extended &, SIMPLE const & ) const;
};

template<typename E, unsigned ID, typename A, typename P, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07,
         typename M08, typename M09, typename M10, typename M11,
         typename M12>
const simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11, M12>
      simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11, M12>::obj;

//________________________________________________________________________________________
//
//  simple specialisation for 12 messages
//________________________________________________________________________________________

template<typename E, unsigned ID, typename A, typename P, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07,
         typename M08, typename M09, typename M10, typename M11>
class simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11> : public P
{
    public:

    typedef E Extended;
    typedef P Parent;
    typedef A Allowed_ids;
    typedef simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11> Self;

    static void handle_entry(Extended &) {}
    static void handle_exit (Extended &) {}
    static void handle_init (Extended & arg)
    {
        arg.set_state(obj);
    }

    static const simple obj;

    virtual unsigned get_id() const override
    {
        return ID;
    }

    virtual void process_event(etl::imessage const & m, Extended & e) const override
    {
        handle_event(m, e, *this);
    }

    static bool me_accepts_event(etl::message_id_t id)
    {
        switch( id )
        {
            case M00::ID:
            case M01::ID:
            case M02::ID:
            case M03::ID:
            case M04::ID:
            case M05::ID:
            case M06::ID:
            case M07::ID:
            case M08::ID:
            case M09::ID:
            case M10::ID:
            case M11::ID:
                return true;
            default:
                return Parent::accepts_event( id );
        };
    }

    bool accepts_event(etl::message_id_t arg) const override
    {                                                                      
         return is_in( arg, Allowed_ids {} );                                           
    }                                                                      

    template<typename SIMPLE>
    void handle_event(etl::imessage const & m, Extended & e, SIMPLE const & s) const
    {
        switch ( m.get_message_id() )
        {
            case M00::ID: on_event<SIMPLE>( static_cast<M00 const &>( m ), e, s ); return;
            case M01::ID: on_event<SIMPLE>( static_cast<M01 const &>( m ), e, s ); return;
            case M02::ID: on_event<SIMPLE>( static_cast<M02 const &>( m ), e, s ); return;
            case M03::ID: on_event<SIMPLE>( static_cast<M03 const &>( m ), e, s ); return;
            case M04::ID: on_event<SIMPLE>( static_cast<M04 const &>( m ), e, s ); return;
            case M05::ID: on_event<SIMPLE>( static_cast<M05 const &>( m ), e, s ); return;
            case M06::ID: on_event<SIMPLE>( static_cast<M06 const &>( m ), e, s ); return;
            case M07::ID: on_event<SIMPLE>( static_cast<M07 const &>( m ), e, s ); return;
            case M08::ID: on_event<SIMPLE>( static_cast<M08 const &>( m ), e, s ); return;
            case M09::ID: on_event<SIMPLE>( static_cast<M09 const &>( m ), e, s ); return;
            case M10::ID: on_event<SIMPLE>( static_cast<M10 const &>( m ), e, s ); return;
            case M11::ID: on_event<SIMPLE>( static_cast<M11 const &>( m ), e, s ); return;
        }
        Parent::handle_event(m, e, s);
    }

    private:

    template<typename SIMPLE> void on_event( M00 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M01 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M02 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M03 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M04 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M05 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M06 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M07 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M08 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M09 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M10 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M11 const &, Extended &, SIMPLE const & ) const;
};

template<typename E, unsigned ID, typename A, typename P, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07,
         typename M08, typename M09, typename M10, typename M11>
const simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11>
      simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11>::obj;

//________________________________________________________________________________________
//
//  simple specialisation for 11 messages
//________________________________________________________________________________________

template<typename E, unsigned ID, typename A, typename P, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07,
         typename M08, typename M09, typename M10>
class simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10> : public P
{
    public:

    typedef E Extended;
    typedef P Parent;
    typedef A Allowed_ids;
    typedef simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10> Self;

    static void handle_entry(Extended &) {}
    static void handle_exit (Extended &) {}
    static void handle_init (Extended & arg)
    {
        arg.set_state(obj);
    }

    static const simple obj;

    virtual unsigned get_id() const override
    {
        return ID;
    }

    virtual void process_event(etl::imessage const & m, Extended & e) const override
    {
        handle_event(m, e, *this);
    }

    static bool me_accepts_event(etl::message_id_t id)
    {
        switch( id )
        {
            case M00::ID:
            case M01::ID:
            case M02::ID:
            case M03::ID:
            case M04::ID:
            case M05::ID:
            case M06::ID:
            case M07::ID:
            case M08::ID:
            case M09::ID:
            case M10::ID:
                return true;
            default:
                return Parent::accepts_event( id );
        };
    }

    bool accepts_event(etl::message_id_t arg) const override
    {                                                                      
         return is_in( arg, Allowed_ids {} );                                           
    }                                                                      

    template<typename SIMPLE>
    void handle_event(etl::imessage const & m, Extended & e, SIMPLE const & s) const
    {
        switch ( m.get_message_id() )
        {
            case M00::ID: on_event<SIMPLE>( static_cast<M00 const &>( m ), e, s ); return;
            case M01::ID: on_event<SIMPLE>( static_cast<M01 const &>( m ), e, s ); return;
            case M02::ID: on_event<SIMPLE>( static_cast<M02 const &>( m ), e, s ); return;
            case M03::ID: on_event<SIMPLE>( static_cast<M03 const &>( m ), e, s ); return;
            case M04::ID: on_event<SIMPLE>( static_cast<M04 const &>( m ), e, s ); return;
            case M05::ID: on_event<SIMPLE>( static_cast<M05 const &>( m ), e, s ); return;
            case M06::ID: on_event<SIMPLE>( static_cast<M06 const &>( m ), e, s ); return;
            case M07::ID: on_event<SIMPLE>( static_cast<M07 const &>( m ), e, s ); return;
            case M08::ID: on_event<SIMPLE>( static_cast<M08 const &>( m ), e, s ); return;
            case M09::ID: on_event<SIMPLE>( static_cast<M09 const &>( m ), e, s ); return;
            case M10::ID: on_event<SIMPLE>( static_cast<M10 const &>( m ), e, s ); return;
        }
        Parent::handle_event(m, e, s);
    }

    private:

    template<typename SIMPLE> void on_event( M00 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M01 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M02 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M03 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M04 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M05 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M06 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M07 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M08 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M09 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M10 const &, Extended &, SIMPLE const & ) const;
};

template<typename E, unsigned ID, typename A, typename P, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07,
         typename M08, typename M09, typename M10>
const simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10>
      simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10>::obj;

//________________________________________________________________________________________
//
//  simple specialisation for 10 messages
//________________________________________________________________________________________

template<typename E, unsigned ID, typename A, typename P, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07,
         typename M08, typename M09>
class simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09> : public P
{
    public:

    typedef E Extended;
    typedef P Parent;
    typedef A Allowed_ids;
    typedef simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09> Self;

    static void handle_entry(Extended &) {}
    static void handle_exit (Extended &) {}
    static void handle_init (Extended & arg)
    {
        arg.set_state(obj);
    }

    static const simple obj;

    virtual unsigned get_id() const override
    {
        return ID;
    }

    virtual void process_event(etl::imessage const & m, Extended & e) const override
    {
        handle_event(m, e, *this);
    }

    static bool me_accepts_event(etl::message_id_t id)
    {
        switch( id )
        {
            case M00::ID:
            case M01::ID:
            case M02::ID:
            case M03::ID:
            case M04::ID:
            case M05::ID:
            case M06::ID:
            case M07::ID:
            case M08::ID:
            case M09::ID:
                return true;
            default:
                return Parent::accepts_event( id );
        };
    }

    bool accepts_event(etl::message_id_t arg) const override
    {                                                                      
         return is_in( arg, Allowed_ids {} );                                           
    }                                                                      

    template<typename SIMPLE>
    void handle_event(etl::imessage const & m, Extended & e, SIMPLE const & s) const
    {
        switch ( m.get_message_id() )
        {
            case M00::ID: on_event<SIMPLE>( static_cast<M00 const &>( m ), e, s ); return;
            case M01::ID: on_event<SIMPLE>( static_cast<M01 const &>( m ), e, s ); return;
            case M02::ID: on_event<SIMPLE>( static_cast<M02 const &>( m ), e, s ); return;
            case M03::ID: on_event<SIMPLE>( static_cast<M03 const &>( m ), e, s ); return;
            case M04::ID: on_event<SIMPLE>( static_cast<M04 const &>( m ), e, s ); return;
            case M05::ID: on_event<SIMPLE>( static_cast<M05 const &>( m ), e, s ); return;
            case M06::ID: on_event<SIMPLE>( static_cast<M06 const &>( m ), e, s ); return;
            case M07::ID: on_event<SIMPLE>( static_cast<M07 const &>( m ), e, s ); return;
            case M08::ID: on_event<SIMPLE>( static_cast<M08 const &>( m ), e, s ); return;
            case M09::ID: on_event<SIMPLE>( static_cast<M09 const &>( m ), e, s ); return;
        }
        Parent::handle_event(m, e, s);
    }

    private:

    template<typename SIMPLE> void on_event( M00 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M01 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M02 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M03 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M04 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M05 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M06 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M07 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M08 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M09 const &, Extended &, SIMPLE const & ) const;
};

template<typename E, unsigned ID, typename A, typename P, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07,
         typename M08, typename M09>
const simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09>
      simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09>::obj;

//________________________________________________________________________________________
//
//  simple specialisation for 9 messages
//________________________________________________________________________________________

template<typename E, unsigned ID, typename A, typename P, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07,
         typename M08>
class simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08> : public P
{
    public:

    typedef E Extended;
    typedef P Parent;
    typedef A Allowed_ids;
    typedef simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08> Self;

    static void handle_entry(Extended &) {}
    static void handle_exit (Extended &) {}
    static void handle_init (Extended & arg)
    {
        arg.set_state(obj);
    }

    static const simple obj;

    virtual unsigned get_id() const override
    {
        return ID;
    }

    virtual void process_event(etl::imessage const & m, Extended & e) const override
    {
        handle_event(m, e, *this);
    }

    static bool me_accepts_event(etl::message_id_t id)
    {
        switch( id )
        {
            case M00::ID:
            case M01::ID:
            case M02::ID:
            case M03::ID:
            case M04::ID:
            case M05::ID:
            case M06::ID:
            case M07::ID:
            case M08::ID:
                return true;
            default:
                return Parent::accepts_event( id );
        };
    }

    bool accepts_event(etl::message_id_t arg) const override
    {                                                                      
         return is_in( arg, Allowed_ids {} );                                           
    }                                                                      

    template<typename SIMPLE>
    void handle_event(etl::imessage const & m, Extended & e, SIMPLE const & s) const
    {
        switch ( m.get_message_id() )
        {
            case M00::ID: on_event<SIMPLE>( static_cast<M00 const &>( m ), e, s ); return;
            case M01::ID: on_event<SIMPLE>( static_cast<M01 const &>( m ), e, s ); return;
            case M02::ID: on_event<SIMPLE>( static_cast<M02 const &>( m ), e, s ); return;
            case M03::ID: on_event<SIMPLE>( static_cast<M03 const &>( m ), e, s ); return;
            case M04::ID: on_event<SIMPLE>( static_cast<M04 const &>( m ), e, s ); return;
            case M05::ID: on_event<SIMPLE>( static_cast<M05 const &>( m ), e, s ); return;
            case M06::ID: on_event<SIMPLE>( static_cast<M06 const &>( m ), e, s ); return;
            case M07::ID: on_event<SIMPLE>( static_cast<M07 const &>( m ), e, s ); return;
            case M08::ID: on_event<SIMPLE>( static_cast<M08 const &>( m ), e, s ); return;
        }
        Parent::handle_event(m, e, s);
    }

    private:

    template<typename SIMPLE> void on_event( M00 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M01 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M02 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M03 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M04 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M05 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M06 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M07 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M08 const &, Extended &, SIMPLE const & ) const;
};

template<typename E, unsigned ID, typename A, typename P, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07,
         typename M08>
const simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08>
      simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08>::obj;

//________________________________________________________________________________________
//
//  simple specialisation for 8 messages
//________________________________________________________________________________________

template<typename E, unsigned ID, typename A, typename P, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07>
class simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06, M07> : public P
{
    public:

    typedef E Extended;
    typedef P Parent;
    typedef A Allowed_ids;
    typedef simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06, M07> Self;

    static void handle_entry(Extended &) {}
    static void handle_exit (Extended &) {}
    static void handle_init (Extended & arg)
    {
        arg.set_state(obj);
    }

    static const simple obj;

    virtual unsigned get_id() const override
    {
        return ID;
    }

    virtual void process_event(etl::imessage const & m, Extended & e) const override
    {
        handle_event(m, e, *this);
    }

    static bool me_accepts_event(etl::message_id_t id)
    {
        switch( id )
        {
            case M00::ID:
            case M01::ID:
            case M02::ID:
            case M03::ID:
            case M04::ID:
            case M05::ID:
            case M06::ID:
            case M07::ID:
                return true;
            default:
                return Parent::accepts_event( id );
        };
    }

    bool accepts_event(etl::message_id_t arg) const override
    {                                                                      
         return is_in( arg, Allowed_ids {} );                                           
    }                                                                      

    template<typename SIMPLE>
    void handle_event(etl::imessage const & m, Extended & e, SIMPLE const & s) const
    {
        switch ( m.get_message_id() )
        {
            case M00::ID: on_event<SIMPLE>( static_cast<M00 const &>( m ), e, s ); return;
            case M01::ID: on_event<SIMPLE>( static_cast<M01 const &>( m ), e, s ); return;
            case M02::ID: on_event<SIMPLE>( static_cast<M02 const &>( m ), e, s ); return;
            case M03::ID: on_event<SIMPLE>( static_cast<M03 const &>( m ), e, s ); return;
            case M04::ID: on_event<SIMPLE>( static_cast<M04 const &>( m ), e, s ); return;
            case M05::ID: on_event<SIMPLE>( static_cast<M05 const &>( m ), e, s ); return;
            case M06::ID: on_event<SIMPLE>( static_cast<M06 const &>( m ), e, s ); return;
            case M07::ID: on_event<SIMPLE>( static_cast<M07 const &>( m ), e, s ); return;
        }
        Parent::handle_event(m, e, s);
    }

    private:

    template<typename SIMPLE> void on_event( M00 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M01 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M02 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M03 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M04 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M05 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M06 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M07 const &, Extended &, SIMPLE const & ) const;
};

template<typename E, unsigned ID, typename A, typename P, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07>
const simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06, M07>
      simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06, M07>::obj;

//________________________________________________________________________________________
//
//  simple specialisation for 7 messages
//________________________________________________________________________________________

template<typename E, unsigned ID, typename A, typename P, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06>
class simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06> : public P
{
    public:

    typedef E Extended;
    typedef P Parent;
    typedef A Allowed_ids;
    typedef simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06> Self;

    static void handle_entry(Extended &) {}
    static void handle_exit (Extended &) {}
    static void handle_init (Extended & arg)
    {
        arg.set_state(obj);
    }

    static const simple obj;

    virtual unsigned get_id() const override
    {
        return ID;
    }

    virtual void process_event(etl::imessage const & m, Extended & e) const override
    {
        handle_event(m, e, *this);
    }

    static bool me_accepts_event(etl::message_id_t id)
    {
        switch( id )
        {
            case M00::ID:
            case M01::ID:
            case M02::ID:
            case M03::ID:
            case M04::ID:
            case M05::ID:
            case M06::ID:
                return true;
            default:
                return Parent::accepts_event( id );
        };
    }

    bool accepts_event(etl::message_id_t arg) const override
    {                                                                      
         return is_in( arg, Allowed_ids {} );                                           
    }                                                                      

    template<typename SIMPLE>
    void handle_event(etl::imessage const & m, Extended & e, SIMPLE const & s) const
    {
        switch ( m.get_message_id() )
        {
            case M00::ID: on_event<SIMPLE>( static_cast<M00 const &>( m ), e, s ); return;
            case M01::ID: on_event<SIMPLE>( static_cast<M01 const &>( m ), e, s ); return;
            case M02::ID: on_event<SIMPLE>( static_cast<M02 const &>( m ), e, s ); return;
            case M03::ID: on_event<SIMPLE>( static_cast<M03 const &>( m ), e, s ); return;
            case M04::ID: on_event<SIMPLE>( static_cast<M04 const &>( m ), e, s ); return;
            case M05::ID: on_event<SIMPLE>( static_cast<M05 const &>( m ), e, s ); return;
            case M06::ID: on_event<SIMPLE>( static_cast<M06 const &>( m ), e, s ); return;
        }
        Parent::handle_event(m, e, s);
    }

    private:

    template<typename SIMPLE> void on_event( M00 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M01 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M02 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M03 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M04 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M05 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M06 const &, Extended &, SIMPLE const & ) const;
};

template<typename E, unsigned ID, typename A, typename P, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06>
const simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06>
      simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05, M06>::obj;

//________________________________________________________________________________________
//
//  simple specialisation for 6 messages
//________________________________________________________________________________________

template<typename E, unsigned ID, typename A, typename P, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05>
class simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05> : public P
{
    public:

    typedef E Extended;
    typedef P Parent;
    typedef A Allowed_ids;
    typedef simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05> Self;

    static void handle_entry(Extended &) {}
    static void handle_exit (Extended &) {}
    static void handle_init (Extended & arg)
    {
        arg.set_state(obj);
    }

    static const simple obj;

    virtual unsigned get_id() const override
    {
        return ID;
    }

    virtual void process_event(etl::imessage const & m, Extended & e) const override
    {
        handle_event(m, e, *this);
    }

    static bool me_accepts_event(etl::message_id_t id)
    {
        switch( id )
        {
            case M00::ID:
            case M01::ID:
            case M02::ID:
            case M03::ID:
            case M04::ID:
            case M05::ID:
                return true;
            default:
                return Parent::accepts_event( id );
        };
    }

    bool accepts_event(etl::message_id_t arg) const override
    {                                                                      
         return is_in( arg, Allowed_ids {} );                                           
    }                                                                      

    template<typename SIMPLE>
    void handle_event(etl::imessage const & m, Extended & e, SIMPLE const & s) const
    {
        switch ( m.get_message_id() )
        {
            case M00::ID: on_event<SIMPLE>( static_cast<M00 const &>( m ), e, s ); return;
            case M01::ID: on_event<SIMPLE>( static_cast<M01 const &>( m ), e, s ); return;
            case M02::ID: on_event<SIMPLE>( static_cast<M02 const &>( m ), e, s ); return;
            case M03::ID: on_event<SIMPLE>( static_cast<M03 const &>( m ), e, s ); return;
            case M04::ID: on_event<SIMPLE>( static_cast<M04 const &>( m ), e, s ); return;
            case M05::ID: on_event<SIMPLE>( static_cast<M05 const &>( m ), e, s ); return;
        }
        Parent::handle_event(m, e, s);
    }

    private:

    template<typename SIMPLE> void on_event( M00 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M01 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M02 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M03 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M04 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M05 const &, Extended &, SIMPLE const & ) const;
};

template<typename E, unsigned ID, typename A, typename P, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05>
const simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05>
      simple<E, ID, A, P, 
         M00, M01, M02, M03, M04, M05>::obj;

//________________________________________________________________________________________
//
//  simple specialisation for 5 messages
//________________________________________________________________________________________

template<typename E, unsigned ID, typename A, typename P, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04>
class simple<E, ID, A, P, 
         M00, M01, M02, M03, M04> : public P
{
    public:

    typedef E Extended;
    typedef P Parent;
    typedef A Allowed_ids;
    typedef simple<E, ID, A, P, 
         M00, M01, M02, M03, M04> Self;

    static void handle_entry(Extended &) {}
    static void handle_exit (Extended &) {}
    static void handle_init (Extended & arg)
    {
        arg.set_state(obj);
    }

    static const simple obj;

    virtual unsigned get_id() const override
    {
        return ID;
    }

    virtual void process_event(etl::imessage const & m, Extended & e) const override
    {
        handle_event(m, e, *this);
    }

    static bool me_accepts_event(etl::message_id_t id)
    {
        switch( id )
        {
            case M00::ID:
            case M01::ID:
            case M02::ID:
            case M03::ID:
            case M04::ID:
                return true;
            default:
                return Parent::accepts_event( id );
        };
    }

    bool accepts_event(etl::message_id_t arg) const override
    {                                                                      
         return is_in( arg, Allowed_ids {} );                                           
    }                                                                      

    template<typename SIMPLE>
    void handle_event(etl::imessage const & m, Extended & e, SIMPLE const & s) const
    {
        switch ( m.get_message_id() )
        {
            case M00::ID: on_event<SIMPLE>( static_cast<M00 const &>( m ), e, s ); return;
            case M01::ID: on_event<SIMPLE>( static_cast<M01 const &>( m ), e, s ); return;
            case M02::ID: on_event<SIMPLE>( static_cast<M02 const &>( m ), e, s ); return;
            case M03::ID: on_event<SIMPLE>( static_cast<M03 const &>( m ), e, s ); return;
            case M04::ID: on_event<SIMPLE>( static_cast<M04 const &>( m ), e, s ); return;
        }
        Parent::handle_event(m, e, s);
    }

    private:

    template<typename SIMPLE> void on_event( M00 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M01 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M02 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M03 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M04 const &, Extended &, SIMPLE const & ) const;
};

template<typename E, unsigned ID, typename A, typename P, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04>
const simple<E, ID, A, P, 
         M00, M01, M02, M03, M04>
      simple<E, ID, A, P, 
         M00, M01, M02, M03, M04>::obj;

//________________________________________________________________________________________
//
//  simple specialisation for 4 messages
//________________________________________________________________________________________

template<typename E, unsigned ID, typename A, typename P, 
         typename M00, typename M01, typename M02, typename M03>
class simple<E, ID, A, P, 
         M00, M01, M02, M03> : public P
{
    public:

    typedef E Extended;
    typedef P Parent;
    typedef A Allowed_ids;
    typedef simple<E, ID, A, P, 
         M00, M01, M02, M03> Self;

    static void handle_entry(Extended &) {}
    static void handle_exit (Extended &) {}
    static void handle_init (Extended & arg)
    {
        arg.set_state(obj);
    }

    static const simple obj;

    virtual unsigned get_id() const override
    {
        return ID;
    }

    virtual void process_event(etl::imessage const & m, Extended & e) const override
    {
        handle_event(m, e, *this);
    }

    static bool me_accepts_event(etl::message_id_t id)
    {
        switch( id )
        {
            case M00::ID:
            case M01::ID:
            case M02::ID:
            case M03::ID:
                return true;
            default:
                return Parent::accepts_event( id );
        };
    }

    bool accepts_event(etl::message_id_t arg) const override
    {                                                                      
         return is_in( arg, Allowed_ids {} );                                           
    }                                                                      

    template<typename SIMPLE>
    void handle_event(etl::imessage const & m, Extended & e, SIMPLE const & s) const
    {
        switch ( m.get_message_id() )
        {
            case M00::ID: on_event<SIMPLE>( static_cast<M00 const &>( m ), e, s ); return;
            case M01::ID: on_event<SIMPLE>( static_cast<M01 const &>( m ), e, s ); return;
            case M02::ID: on_event<SIMPLE>( static_cast<M02 const &>( m ), e, s ); return;
            case M03::ID: on_event<SIMPLE>( static_cast<M03 const &>( m ), e, s ); return;
        }
        Parent::handle_event(m, e, s);
    }

    private:

    template<typename SIMPLE> void on_event( M00 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M01 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M02 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M03 const &, Extended &, SIMPLE const & ) const;
};

template<typename E, unsigned ID, typename A, typename P, 
         typename M00, typename M01, typename M02, typename M03>
const simple<E, ID, A, P, 
         M00, M01, M02, M03>
      simple<E, ID, A, P, 
         M00, M01, M02, M03>::obj;

//________________________________________________________________________________________
//
//  simple specialisation for 3 messages
//________________________________________________________________________________________

template<typename E, unsigned ID, typename A, typename P, 
         typename M00, typename M01, typename M02>
class simple<E, ID, A, P, 
         M00, M01, M02> : public P
{
    public:

    typedef E Extended;
    typedef P Parent;
    typedef A Allowed_ids;
    typedef simple<E, ID, A, P, 
         M00, M01, M02> Self;

    static void handle_entry(Extended &) {}
    static void handle_exit (Extended &) {}
    static void handle_init (Extended & arg)
    {
        arg.set_state(obj);
    }

    static const simple obj;

    virtual unsigned get_id() const override
    {
        return ID;
    }

    virtual void process_event(etl::imessage const & m, Extended & e) const override
    {
        handle_event(m, e, *this);
    }

    static bool me_accepts_event(etl::message_id_t id)
    {
        switch( id )
        {
            case M00::ID:
            case M01::ID:
            case M02::ID:
                return true;
            default:
                return Parent::accepts_event( id );
        };
    }

    bool accepts_event(etl::message_id_t arg) const override
    {                                                                      
         return is_in( arg, Allowed_ids {} );                                           
    }                                                                      

    template<typename SIMPLE>
    void handle_event(etl::imessage const & m, Extended & e, SIMPLE const & s) const
    {
        switch ( m.get_message_id() )
        {
            case M00::ID: on_event<SIMPLE>( static_cast<M00 const &>( m ), e, s ); return;
            case M01::ID: on_event<SIMPLE>( static_cast<M01 const &>( m ), e, s ); return;
            case M02::ID: on_event<SIMPLE>( static_cast<M02 const &>( m ), e, s ); return;
        }
        Parent::handle_event(m, e, s);
    }

    private:

    template<typename SIMPLE> void on_event( M00 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M01 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M02 const &, Extended &, SIMPLE const & ) const;
};

template<typename E, unsigned ID, typename A, typename P, 
         typename M00, typename M01, typename M02>
const simple<E, ID, A, P, 
         M00, M01, M02>
      simple<E, ID, A, P, 
         M00, M01, M02>::obj;

//________________________________________________________________________________________
//
//  simple specialisation for 2 messages
//________________________________________________________________________________________

template<typename E, unsigned ID, typename A, typename P, 
         typename M00, typename M01>
class simple<E, ID, A, P, 
         M00, M01> : public P
{
    public:

    typedef E Extended;
    typedef P Parent;
    typedef A Allowed_ids;
    typedef simple<E, ID, A, P, 
         M00, M01> Self;

    static void handle_entry(Extended &) {}
    static void handle_exit (Extended &) {}
    static void handle_init (Extended & arg)
    {
        arg.set_state(obj);
    }

    static const simple obj;

    virtual unsigned get_id() const override
    {
        return ID;
    }

    virtual void process_event(etl::imessage const & m, Extended & e) const override
    {
        handle_event(m, e, *this);
    }

    static bool me_accepts_event(etl::message_id_t id)
    {
        switch( id )
        {
            case M00::ID:
            case M01::ID:
                return true;
            default:
                return Parent::accepts_event( id );
        };
    }

    bool accepts_event(etl::message_id_t arg) const override
    {                                                                      
         return is_in( arg, Allowed_ids {} );                                           
    }                                                                      

    template<typename SIMPLE>
    void handle_event(etl::imessage const & m, Extended & e, SIMPLE const & s) const
    {
        switch ( m.get_message_id() )
        {
            case M00::ID: on_event<SIMPLE>( static_cast<M00 const &>( m ), e, s ); return;
            case M01::ID: on_event<SIMPLE>( static_cast<M01 const &>( m ), e, s ); return;
        }
        Parent::handle_event(m, e, s);
    }

    private:

    template<typename SIMPLE> void on_event( M00 const &, Extended &, SIMPLE const & ) const;
    template<typename SIMPLE> void on_event( M01 const &, Extended &, SIMPLE const & ) const;
};

template<typename E, unsigned ID, typename A, typename P, 
         typename M00, typename M01>
const simple<E, ID, A, P, 
         M00, M01>
      simple<E, ID, A, P, 
         M00, M01>::obj;

//________________________________________________________________________________________
//
//  simple specialisation for 1 messages
//________________________________________________________________________________________

template<typename E, unsigned ID, typename A, typename P, 
         typename M00>
class simple<E, ID, A, P, 
         M00> : public P
{
    public:

    typedef E Extended;
    typedef P Parent;
    typedef A Allowed_ids;
    typedef simple<E, ID, A, P, 
         M00> Self;

    static void handle_entry(Extended &) {}
    static void handle_exit (Extended &) {}
    static void handle_init (Extended & arg)
    {
        arg.set_state(obj);
    }

    static const simple obj;

    virtual unsigned get_id() const override
    {
        return ID;
    }

    virtual void process_event(etl::imessage const & m, Extended & e) const override
    {
        handle_event(m, e, *this);
    }

    static bool me_accepts_event(etl::message_id_t id)
    {
        switch( id )
        {
            case M00::ID:
                return true;
            default:
                return Parent::accepts_event( id );
        };
    }

    bool accepts_event(etl::message_id_t arg) const override
    {                                                                      
         return is_in( arg, Allowed_ids {} );                                           
    }                                                                      

    template<typename SIMPLE>
    void handle_event(etl::imessage const & m, Extended & e, SIMPLE const & s) const
    {
        switch ( m.get_message_id() )
        {
            case M00::ID: on_event<SIMPLE>( static_cast<M00 const &>( m ), e, s ); return;
        }
        Parent::handle_event(m, e, s);
    }

    private:

    template<typename SIMPLE> void on_event( M00 const &, Extended &, SIMPLE const & ) const;
};

template<typename E, unsigned ID, typename A, typename P, 
         typename M00>
const simple<E, ID, A, P, 
         M00>
      simple<E, ID, A, P, 
         M00>::obj;

//________________________________________________________________________________________
//
//  extended declaration for 16 messages
//________________________________________________________________________________________

template<typename TDerived, 
         typename M00 = void, typename M01 = void, typename M02 = void,
         typename M03 = void, typename M04 = void, typename M05 = void,
         typename M06 = void, typename M07 = void, typename M08 = void,
         typename M09 = void, typename M10 = void, typename M11 = void,
         typename M12 = void, typename M13 = void, typename M14 = void,
         typename M15 = void>
class extended;

template<typename TDerived, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07,
         typename M08, typename M09, typename M10, typename M11,
         typename M12, typename M13, typename M14, typename M15>
class extended : public etl::imessage_router
{
public:
    typedef TDerived derived;
    typedef top<derived> state;
    typedef etl::message_packet<
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11, M12, M13, M14, M15> message_packet;

    extended(etl::message_router_id_t id)
        : etl::imessage_router(id)
    {
        ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER,
                   ETL_ERROR(etl::message_router_illegal_id));
    }
    extended(etl::message_router_id_t id, etl::imessage_router& successor_)
        : etl::imessage_router(id, successor_)
    {
        ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER,
                   ETL_ERROR(etl::message_router_illegal_id));
    }
    // Non-mutating methods
    bool          is_started    () const { return _state != nullptr; };
    state const * get_state     () const { return _state; };
    bool          is_null_router() const ETL_OVERRIDE { return false; }
    bool          is_producer   () const ETL_OVERRIDE { return true;  }
    bool          is_consumer   () const ETL_OVERRIDE { return true;  }

    // Mutating methods
    void set_state(state const & arg) { _state = &arg; };
    void receive(etl::imessage const & message) override
    {
        get_state()->process_event(message, static_cast<derived&>(* this));
    }

    virtual void on_receive_unknown(etl::imessage const &) {}

    using imessage_router::accepts;
    bool accepts(etl::message_id_t id) const ETL_OVERRIDE
    {
        switch (id)
        {
        case M00::ID:
        case M01::ID:
        case M02::ID:
        case M03::ID:
        case M04::ID:
        case M05::ID:
        case M06::ID:
        case M07::ID:
        case M08::ID:
        case M09::ID:
        case M10::ID:
        case M11::ID:
        case M12::ID:
        case M13::ID:
        case M14::ID:
        case M15::ID:
            return true;
        default:
            return false;
        }
    }

private:
    const state* _state { nullptr };
};

//________________________________________________________________________________________
//
//  extended specialisation for 15 messages
//________________________________________________________________________________________

template<typename TDerived, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07,
         typename M08, typename M09, typename M10, typename M11,
         typename M12, typename M13, typename M14>
class extended<TDerived, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11, M12, M13, M14> : public etl::imessage_router
{
public:
    typedef TDerived derived;
    typedef top<derived> state;
    typedef etl::message_packet<
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11, M12, M13, M14> message_packet;

    extended(etl::message_router_id_t id)
        : etl::imessage_router(id)
    {
        ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER,
                   ETL_ERROR(etl::message_router_illegal_id));
    }
    extended(etl::message_router_id_t id, etl::imessage_router& successor_)
        : etl::imessage_router(id, successor_)
    {
        ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER,
                   ETL_ERROR(etl::message_router_illegal_id));
    }
    // Non-mutating methods
    bool          is_started    () const { return _state != nullptr; };
    state const * get_state     () const { return _state; };
    bool          is_null_router() const ETL_OVERRIDE { return false; }
    bool          is_producer   () const ETL_OVERRIDE { return true;  }
    bool          is_consumer   () const ETL_OVERRIDE { return true;  }

    // Mutating methods
    void set_state(state const & arg) { _state = &arg; };
    void receive(etl::imessage const & message) override
    {
        get_state()->process_event(message, static_cast<derived&>(* this));
    }

    virtual void on_receive_unknown(etl::imessage const &) {}

    using imessage_router::accepts;
    bool accepts(etl::message_id_t id) const ETL_OVERRIDE
    {
        switch (id)
        {
        case M00::ID:
        case M01::ID:
        case M02::ID:
        case M03::ID:
        case M04::ID:
        case M05::ID:
        case M06::ID:
        case M07::ID:
        case M08::ID:
        case M09::ID:
        case M10::ID:
        case M11::ID:
        case M12::ID:
        case M13::ID:
        case M14::ID:
            return true;
        default:
            return false;
        }
    }

private:
    const state* _state { nullptr };
};

//________________________________________________________________________________________
//
//  extended specialisation for 14 messages
//________________________________________________________________________________________

template<typename TDerived, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07,
         typename M08, typename M09, typename M10, typename M11,
         typename M12, typename M13>
class extended<TDerived, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11, M12, M13> : public etl::imessage_router
{
public:
    typedef TDerived derived;
    typedef top<derived> state;
    typedef etl::message_packet<
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11, M12, M13> message_packet;

    extended(etl::message_router_id_t id)
        : etl::imessage_router(id)
    {
        ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER,
                   ETL_ERROR(etl::message_router_illegal_id));
    }
    extended(etl::message_router_id_t id, etl::imessage_router& successor_)
        : etl::imessage_router(id, successor_)
    {
        ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER,
                   ETL_ERROR(etl::message_router_illegal_id));
    }
    // Non-mutating methods
    bool          is_started    () const { return _state != nullptr; };
    state const * get_state     () const { return _state; };
    bool          is_null_router() const ETL_OVERRIDE { return false; }
    bool          is_producer   () const ETL_OVERRIDE { return true;  }
    bool          is_consumer   () const ETL_OVERRIDE { return true;  }

    // Mutating methods
    void set_state(state const & arg) { _state = &arg; };
    void receive(etl::imessage const & message) override
    {
        get_state()->process_event(message, static_cast<derived&>(* this));
    }

    virtual void on_receive_unknown(etl::imessage const &) {}

    using imessage_router::accepts;
    bool accepts(etl::message_id_t id) const ETL_OVERRIDE
    {
        switch (id)
        {
        case M00::ID:
        case M01::ID:
        case M02::ID:
        case M03::ID:
        case M04::ID:
        case M05::ID:
        case M06::ID:
        case M07::ID:
        case M08::ID:
        case M09::ID:
        case M10::ID:
        case M11::ID:
        case M12::ID:
        case M13::ID:
            return true;
        default:
            return false;
        }
    }

private:
    const state* _state { nullptr };
};

//________________________________________________________________________________________
//
//  extended specialisation for 13 messages
//________________________________________________________________________________________

template<typename TDerived, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07,
         typename M08, typename M09, typename M10, typename M11,
         typename M12>
class extended<TDerived, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11, M12> : public etl::imessage_router
{
public:
    typedef TDerived derived;
    typedef top<derived> state;
    typedef etl::message_packet<
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11, M12> message_packet;

    extended(etl::message_router_id_t id)
        : etl::imessage_router(id)
    {
        ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER,
                   ETL_ERROR(etl::message_router_illegal_id));
    }
    extended(etl::message_router_id_t id, etl::imessage_router& successor_)
        : etl::imessage_router(id, successor_)
    {
        ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER,
                   ETL_ERROR(etl::message_router_illegal_id));
    }
    // Non-mutating methods
    bool          is_started    () const { return _state != nullptr; };
    state const * get_state     () const { return _state; };
    bool          is_null_router() const ETL_OVERRIDE { return false; }
    bool          is_producer   () const ETL_OVERRIDE { return true;  }
    bool          is_consumer   () const ETL_OVERRIDE { return true;  }

    // Mutating methods
    void set_state(state const & arg) { _state = &arg; };
    void receive(etl::imessage const & message) override
    {
        get_state()->process_event(message, static_cast<derived&>(* this));
    }

    virtual void on_receive_unknown(etl::imessage const &) {}

    using imessage_router::accepts;
    bool accepts(etl::message_id_t id) const ETL_OVERRIDE
    {
        switch (id)
        {
        case M00::ID:
        case M01::ID:
        case M02::ID:
        case M03::ID:
        case M04::ID:
        case M05::ID:
        case M06::ID:
        case M07::ID:
        case M08::ID:
        case M09::ID:
        case M10::ID:
        case M11::ID:
        case M12::ID:
            return true;
        default:
            return false;
        }
    }

private:
    const state* _state { nullptr };
};

//________________________________________________________________________________________
//
//  extended specialisation for 12 messages
//________________________________________________________________________________________

template<typename TDerived, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07,
         typename M08, typename M09, typename M10, typename M11>
class extended<TDerived, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11> : public etl::imessage_router
{
public:
    typedef TDerived derived;
    typedef top<derived> state;
    typedef etl::message_packet<
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10, M11> message_packet;

    extended(etl::message_router_id_t id)
        : etl::imessage_router(id)
    {
        ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER,
                   ETL_ERROR(etl::message_router_illegal_id));
    }
    extended(etl::message_router_id_t id, etl::imessage_router& successor_)
        : etl::imessage_router(id, successor_)
    {
        ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER,
                   ETL_ERROR(etl::message_router_illegal_id));
    }
    // Non-mutating methods
    bool          is_started    () const { return _state != nullptr; };
    state const * get_state     () const { return _state; };
    bool          is_null_router() const ETL_OVERRIDE { return false; }
    bool          is_producer   () const ETL_OVERRIDE { return true;  }
    bool          is_consumer   () const ETL_OVERRIDE { return true;  }

    // Mutating methods
    void set_state(state const & arg) { _state = &arg; };
    void receive(etl::imessage const & message) override
    {
        get_state()->process_event(message, static_cast<derived&>(* this));
    }

    virtual void on_receive_unknown(etl::imessage const &) {}

    using imessage_router::accepts;
    bool accepts(etl::message_id_t id) const ETL_OVERRIDE
    {
        switch (id)
        {
        case M00::ID:
        case M01::ID:
        case M02::ID:
        case M03::ID:
        case M04::ID:
        case M05::ID:
        case M06::ID:
        case M07::ID:
        case M08::ID:
        case M09::ID:
        case M10::ID:
        case M11::ID:
            return true;
        default:
            return false;
        }
    }

private:
    const state* _state { nullptr };
};

//________________________________________________________________________________________
//
//  extended specialisation for 11 messages
//________________________________________________________________________________________

template<typename TDerived, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07,
         typename M08, typename M09, typename M10>
class extended<TDerived, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10> : public etl::imessage_router
{
public:
    typedef TDerived derived;
    typedef top<derived> state;
    typedef etl::message_packet<
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09, M10> message_packet;

    extended(etl::message_router_id_t id)
        : etl::imessage_router(id)
    {
        ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER,
                   ETL_ERROR(etl::message_router_illegal_id));
    }
    extended(etl::message_router_id_t id, etl::imessage_router& successor_)
        : etl::imessage_router(id, successor_)
    {
        ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER,
                   ETL_ERROR(etl::message_router_illegal_id));
    }
    // Non-mutating methods
    bool          is_started    () const { return _state != nullptr; };
    state const * get_state     () const { return _state; };
    bool          is_null_router() const ETL_OVERRIDE { return false; }
    bool          is_producer   () const ETL_OVERRIDE { return true;  }
    bool          is_consumer   () const ETL_OVERRIDE { return true;  }

    // Mutating methods
    void set_state(state const & arg) { _state = &arg; };
    void receive(etl::imessage const & message) override
    {
        get_state()->process_event(message, static_cast<derived&>(* this));
    }

    virtual void on_receive_unknown(etl::imessage const &) {}

    using imessage_router::accepts;
    bool accepts(etl::message_id_t id) const ETL_OVERRIDE
    {
        switch (id)
        {
        case M00::ID:
        case M01::ID:
        case M02::ID:
        case M03::ID:
        case M04::ID:
        case M05::ID:
        case M06::ID:
        case M07::ID:
        case M08::ID:
        case M09::ID:
        case M10::ID:
            return true;
        default:
            return false;
        }
    }

private:
    const state* _state { nullptr };
};

//________________________________________________________________________________________
//
//  extended specialisation for 10 messages
//________________________________________________________________________________________

template<typename TDerived, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07,
         typename M08, typename M09>
class extended<TDerived, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09> : public etl::imessage_router
{
public:
    typedef TDerived derived;
    typedef top<derived> state;
    typedef etl::message_packet<
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08, M09> message_packet;

    extended(etl::message_router_id_t id)
        : etl::imessage_router(id)
    {
        ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER,
                   ETL_ERROR(etl::message_router_illegal_id));
    }
    extended(etl::message_router_id_t id, etl::imessage_router& successor_)
        : etl::imessage_router(id, successor_)
    {
        ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER,
                   ETL_ERROR(etl::message_router_illegal_id));
    }
    // Non-mutating methods
    bool          is_started    () const { return _state != nullptr; };
    state const * get_state     () const { return _state; };
    bool          is_null_router() const ETL_OVERRIDE { return false; }
    bool          is_producer   () const ETL_OVERRIDE { return true;  }
    bool          is_consumer   () const ETL_OVERRIDE { return true;  }

    // Mutating methods
    void set_state(state const & arg) { _state = &arg; };
    void receive(etl::imessage const & message) override
    {
        get_state()->process_event(message, static_cast<derived&>(* this));
    }

    virtual void on_receive_unknown(etl::imessage const &) {}

    using imessage_router::accepts;
    bool accepts(etl::message_id_t id) const ETL_OVERRIDE
    {
        switch (id)
        {
        case M00::ID:
        case M01::ID:
        case M02::ID:
        case M03::ID:
        case M04::ID:
        case M05::ID:
        case M06::ID:
        case M07::ID:
        case M08::ID:
        case M09::ID:
            return true;
        default:
            return false;
        }
    }

private:
    const state* _state { nullptr };
};

//________________________________________________________________________________________
//
//  extended specialisation for 9 messages
//________________________________________________________________________________________

template<typename TDerived, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07,
         typename M08>
class extended<TDerived, 
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08> : public etl::imessage_router
{
public:
    typedef TDerived derived;
    typedef top<derived> state;
    typedef etl::message_packet<
         M00, M01, M02, M03, M04, M05, M06, M07,
         M08> message_packet;

    extended(etl::message_router_id_t id)
        : etl::imessage_router(id)
    {
        ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER,
                   ETL_ERROR(etl::message_router_illegal_id));
    }
    extended(etl::message_router_id_t id, etl::imessage_router& successor_)
        : etl::imessage_router(id, successor_)
    {
        ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER,
                   ETL_ERROR(etl::message_router_illegal_id));
    }
    // Non-mutating methods
    bool          is_started    () const { return _state != nullptr; };
    state const * get_state     () const { return _state; };
    bool          is_null_router() const ETL_OVERRIDE { return false; }
    bool          is_producer   () const ETL_OVERRIDE { return true;  }
    bool          is_consumer   () const ETL_OVERRIDE { return true;  }

    // Mutating methods
    void set_state(state const & arg) { _state = &arg; };
    void receive(etl::imessage const & message) override
    {
        get_state()->process_event(message, static_cast<derived&>(* this));
    }

    virtual void on_receive_unknown(etl::imessage const &) {}

    using imessage_router::accepts;
    bool accepts(etl::message_id_t id) const ETL_OVERRIDE
    {
        switch (id)
        {
        case M00::ID:
        case M01::ID:
        case M02::ID:
        case M03::ID:
        case M04::ID:
        case M05::ID:
        case M06::ID:
        case M07::ID:
        case M08::ID:
            return true;
        default:
            return false;
        }
    }

private:
    const state* _state { nullptr };
};

//________________________________________________________________________________________
//
//  extended specialisation for 8 messages
//________________________________________________________________________________________

template<typename TDerived, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06, typename M07>
class extended<TDerived, 
         M00, M01, M02, M03, M04, M05, M06, M07> : public etl::imessage_router
{
public:
    typedef TDerived derived;
    typedef top<derived> state;
    typedef etl::message_packet<
         M00, M01, M02, M03, M04, M05, M06, M07> message_packet;

    extended(etl::message_router_id_t id)
        : etl::imessage_router(id)
    {
        ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER,
                   ETL_ERROR(etl::message_router_illegal_id));
    }
    extended(etl::message_router_id_t id, etl::imessage_router& successor_)
        : etl::imessage_router(id, successor_)
    {
        ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER,
                   ETL_ERROR(etl::message_router_illegal_id));
    }
    // Non-mutating methods
    bool          is_started    () const { return _state != nullptr; };
    state const * get_state     () const { return _state; };
    bool          is_null_router() const ETL_OVERRIDE { return false; }
    bool          is_producer   () const ETL_OVERRIDE { return true;  }
    bool          is_consumer   () const ETL_OVERRIDE { return true;  }

    // Mutating methods
    void set_state(state const & arg) { _state = &arg; };
    void receive(etl::imessage const & message) override
    {
        get_state()->process_event(message, static_cast<derived&>(* this));
    }

    virtual void on_receive_unknown(etl::imessage const &) {}

    using imessage_router::accepts;
    bool accepts(etl::message_id_t id) const ETL_OVERRIDE
    {
        switch (id)
        {
        case M00::ID:
        case M01::ID:
        case M02::ID:
        case M03::ID:
        case M04::ID:
        case M05::ID:
        case M06::ID:
        case M07::ID:
            return true;
        default:
            return false;
        }
    }

private:
    const state* _state { nullptr };
};

//________________________________________________________________________________________
//
//  extended specialisation for 7 messages
//________________________________________________________________________________________

template<typename TDerived, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05, typename M06>
class extended<TDerived, 
         M00, M01, M02, M03, M04, M05, M06> : public etl::imessage_router
{
public:
    typedef TDerived derived;
    typedef top<derived> state;
    typedef etl::message_packet<
         M00, M01, M02, M03, M04, M05, M06> message_packet;

    extended(etl::message_router_id_t id)
        : etl::imessage_router(id)
    {
        ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER,
                   ETL_ERROR(etl::message_router_illegal_id));
    }
    extended(etl::message_router_id_t id, etl::imessage_router& successor_)
        : etl::imessage_router(id, successor_)
    {
        ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER,
                   ETL_ERROR(etl::message_router_illegal_id));
    }
    // Non-mutating methods
    bool          is_started    () const { return _state != nullptr; };
    state const * get_state     () const { return _state; };
    bool          is_null_router() const ETL_OVERRIDE { return false; }
    bool          is_producer   () const ETL_OVERRIDE { return true;  }
    bool          is_consumer   () const ETL_OVERRIDE { return true;  }

    // Mutating methods
    void set_state(state const & arg) { _state = &arg; };
    void receive(etl::imessage const & message) override
    {
        get_state()->process_event(message, static_cast<derived&>(* this));
    }

    virtual void on_receive_unknown(etl::imessage const &) {}

    using imessage_router::accepts;
    bool accepts(etl::message_id_t id) const ETL_OVERRIDE
    {
        switch (id)
        {
        case M00::ID:
        case M01::ID:
        case M02::ID:
        case M03::ID:
        case M04::ID:
        case M05::ID:
        case M06::ID:
            return true;
        default:
            return false;
        }
    }

private:
    const state* _state { nullptr };
};

//________________________________________________________________________________________
//
//  extended specialisation for 6 messages
//________________________________________________________________________________________

template<typename TDerived, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04, typename M05>
class extended<TDerived, 
         M00, M01, M02, M03, M04, M05> : public etl::imessage_router
{
public:
    typedef TDerived derived;
    typedef top<derived> state;
    typedef etl::message_packet<
         M00, M01, M02, M03, M04, M05> message_packet;

    extended(etl::message_router_id_t id)
        : etl::imessage_router(id)
    {
        ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER,
                   ETL_ERROR(etl::message_router_illegal_id));
    }
    extended(etl::message_router_id_t id, etl::imessage_router& successor_)
        : etl::imessage_router(id, successor_)
    {
        ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER,
                   ETL_ERROR(etl::message_router_illegal_id));
    }
    // Non-mutating methods
    bool          is_started    () const { return _state != nullptr; };
    state const * get_state     () const { return _state; };
    bool          is_null_router() const ETL_OVERRIDE { return false; }
    bool          is_producer   () const ETL_OVERRIDE { return true;  }
    bool          is_consumer   () const ETL_OVERRIDE { return true;  }

    // Mutating methods
    void set_state(state const & arg) { _state = &arg; };
    void receive(etl::imessage const & message) override
    {
        get_state()->process_event(message, static_cast<derived&>(* this));
    }

    virtual void on_receive_unknown(etl::imessage const &) {}

    using imessage_router::accepts;
    bool accepts(etl::message_id_t id) const ETL_OVERRIDE
    {
        switch (id)
        {
        case M00::ID:
        case M01::ID:
        case M02::ID:
        case M03::ID:
        case M04::ID:
        case M05::ID:
            return true;
        default:
            return false;
        }
    }

private:
    const state* _state { nullptr };
};

//________________________________________________________________________________________
//
//  extended specialisation for 5 messages
//________________________________________________________________________________________

template<typename TDerived, 
         typename M00, typename M01, typename M02, typename M03,
         typename M04>
class extended<TDerived, 
         M00, M01, M02, M03, M04> : public etl::imessage_router
{
public:
    typedef TDerived derived;
    typedef top<derived> state;
    typedef etl::message_packet<
         M00, M01, M02, M03, M04> message_packet;

    extended(etl::message_router_id_t id)
        : etl::imessage_router(id)
    {
        ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER,
                   ETL_ERROR(etl::message_router_illegal_id));
    }
    extended(etl::message_router_id_t id, etl::imessage_router& successor_)
        : etl::imessage_router(id, successor_)
    {
        ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER,
                   ETL_ERROR(etl::message_router_illegal_id));
    }
    // Non-mutating methods
    bool          is_started    () const { return _state != nullptr; };
    state const * get_state     () const { return _state; };
    bool          is_null_router() const ETL_OVERRIDE { return false; }
    bool          is_producer   () const ETL_OVERRIDE { return true;  }
    bool          is_consumer   () const ETL_OVERRIDE { return true;  }

    // Mutating methods
    void set_state(state const & arg) { _state = &arg; };
    void receive(etl::imessage const & message) override
    {
        get_state()->process_event(message, static_cast<derived&>(* this));
    }

    virtual void on_receive_unknown(etl::imessage const &) {}

    using imessage_router::accepts;
    bool accepts(etl::message_id_t id) const ETL_OVERRIDE
    {
        switch (id)
        {
        case M00::ID:
        case M01::ID:
        case M02::ID:
        case M03::ID:
        case M04::ID:
            return true;
        default:
            return false;
        }
    }

private:
    const state* _state { nullptr };
};

//________________________________________________________________________________________
//
//  extended specialisation for 4 messages
//________________________________________________________________________________________

template<typename TDerived, 
         typename M00, typename M01, typename M02, typename M03>
class extended<TDerived, 
         M00, M01, M02, M03> : public etl::imessage_router
{
public:
    typedef TDerived derived;
    typedef top<derived> state;
    typedef etl::message_packet<
         M00, M01, M02, M03> message_packet;

    extended(etl::message_router_id_t id)
        : etl::imessage_router(id)
    {
        ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER,
                   ETL_ERROR(etl::message_router_illegal_id));
    }
    extended(etl::message_router_id_t id, etl::imessage_router& successor_)
        : etl::imessage_router(id, successor_)
    {
        ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER,
                   ETL_ERROR(etl::message_router_illegal_id));
    }
    // Non-mutating methods
    bool          is_started    () const { return _state != nullptr; };
    state const * get_state     () const { return _state; };
    bool          is_null_router() const ETL_OVERRIDE { return false; }
    bool          is_producer   () const ETL_OVERRIDE { return true;  }
    bool          is_consumer   () const ETL_OVERRIDE { return true;  }

    // Mutating methods
    void set_state(state const & arg) { _state = &arg; };
    void receive(etl::imessage const & message) override
    {
        get_state()->process_event(message, static_cast<derived&>(* this));
    }

    virtual void on_receive_unknown(etl::imessage const &) {}

    using imessage_router::accepts;
    bool accepts(etl::message_id_t id) const ETL_OVERRIDE
    {
        switch (id)
        {
        case M00::ID:
        case M01::ID:
        case M02::ID:
        case M03::ID:
            return true;
        default:
            return false;
        }
    }

private:
    const state* _state { nullptr };
};

//________________________________________________________________________________________
//
//  extended specialisation for 3 messages
//________________________________________________________________________________________

template<typename TDerived, 
         typename M00, typename M01, typename M02>
class extended<TDerived, 
         M00, M01, M02> : public etl::imessage_router
{
public:
    typedef TDerived derived;
    typedef top<derived> state;
    typedef etl::message_packet<
         M00, M01, M02> message_packet;

    extended(etl::message_router_id_t id)
        : etl::imessage_router(id)
    {
        ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER,
                   ETL_ERROR(etl::message_router_illegal_id));
    }
    extended(etl::message_router_id_t id, etl::imessage_router& successor_)
        : etl::imessage_router(id, successor_)
    {
        ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER,
                   ETL_ERROR(etl::message_router_illegal_id));
    }
    // Non-mutating methods
    bool          is_started    () const { return _state != nullptr; };
    state const * get_state     () const { return _state; };
    bool          is_null_router() const ETL_OVERRIDE { return false; }
    bool          is_producer   () const ETL_OVERRIDE { return true;  }
    bool          is_consumer   () const ETL_OVERRIDE { return true;  }

    // Mutating methods
    void set_state(state const & arg) { _state = &arg; };
    void receive(etl::imessage const & message) override
    {
        get_state()->process_event(message, static_cast<derived&>(* this));
    }

    virtual void on_receive_unknown(etl::imessage const &) {}

    using imessage_router::accepts;
    bool accepts(etl::message_id_t id) const ETL_OVERRIDE
    {
        switch (id)
        {
        case M00::ID:
        case M01::ID:
        case M02::ID:
            return true;
        default:
            return false;
        }
    }

private:
    const state* _state { nullptr };
};

//________________________________________________________________________________________
//
//  extended specialisation for 2 messages
//________________________________________________________________________________________

template<typename TDerived, 
         typename M00, typename M01>
class extended<TDerived, 
         M00, M01> : public etl::imessage_router
{
public:
    typedef TDerived derived;
    typedef top<derived> state;
    typedef etl::message_packet<
         M00, M01> message_packet;

    extended(etl::message_router_id_t id)
        : etl::imessage_router(id)
    {
        ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER,
                   ETL_ERROR(etl::message_router_illegal_id));
    }
    extended(etl::message_router_id_t id, etl::imessage_router& successor_)
        : etl::imessage_router(id, successor_)
    {
        ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER,
                   ETL_ERROR(etl::message_router_illegal_id));
    }
    // Non-mutating methods
    bool          is_started    () const { return _state != nullptr; };
    state const * get_state     () const { return _state; };
    bool          is_null_router() const ETL_OVERRIDE { return false; }
    bool          is_producer   () const ETL_OVERRIDE { return true;  }
    bool          is_consumer   () const ETL_OVERRIDE { return true;  }

    // Mutating methods
    void set_state(state const & arg) { _state = &arg; };
    void receive(etl::imessage const & message) override
    {
        get_state()->process_event(message, static_cast<derived&>(* this));
    }

    virtual void on_receive_unknown(etl::imessage const &) {}

    using imessage_router::accepts;
    bool accepts(etl::message_id_t id) const ETL_OVERRIDE
    {
        switch (id)
        {
        case M00::ID:
        case M01::ID:
            return true;
        default:
            return false;
        }
    }

private:
    const state* _state { nullptr };
};

//________________________________________________________________________________________
//
//  extended specialisation for 1 messages
//________________________________________________________________________________________

template<typename TDerived, 
         typename M00>
class extended<TDerived, 
         M00> : public etl::imessage_router
{
public:
    typedef TDerived derived;
    typedef top<derived> state;
    typedef etl::message_packet<
         M00> message_packet;

    extended(etl::message_router_id_t id)
        : etl::imessage_router(id)
    {
        ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER,
                   ETL_ERROR(etl::message_router_illegal_id));
    }
    extended(etl::message_router_id_t id, etl::imessage_router& successor_)
        : etl::imessage_router(id, successor_)
    {
        ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER,
                   ETL_ERROR(etl::message_router_illegal_id));
    }
    // Non-mutating methods
    bool          is_started    () const { return _state != nullptr; };
    state const * get_state     () const { return _state; };
    bool          is_null_router() const ETL_OVERRIDE { return false; }
    bool          is_producer   () const ETL_OVERRIDE { return true;  }
    bool          is_consumer   () const ETL_OVERRIDE { return true;  }

    // Mutating methods
    void set_state(state const & arg) { _state = &arg; };
    void receive(etl::imessage const & message) override
    {
        get_state()->process_event(message, static_cast<derived&>(* this));
    }

    virtual void on_receive_unknown(etl::imessage const &) {}

    using imessage_router::accepts;
    bool accepts(etl::message_id_t id) const ETL_OVERRIDE
    {
        switch (id)
        {
        case M00::ID:
            return true;
        default:
            return false;
        }
    }

private:
    const state* _state { nullptr };
};

//________________________________________________________________________________________
//
// Init
//________________________________________________________________________________________

template<typename C>
struct Init
{
    typedef C Child;
    typedef typename C::Extended Extended;
    Init(Extended & arg) : _extended(arg) { };
    ~Init()
    {
        Child::handle_entry(_extended);
        Child::handle_init(_extended);
    }
    Extended & _extended;
};

namespace _private_
{

//________________________________________________________________________________________
//
// Is_child: See GOTW #71 for design
//________________________________________________________________________________________

template<typename C, typename P>
struct Is_child
{
    private:
    
    typedef C Child;
    typedef P Parent;

    class  Yes { char a[1]; };
    class  No  { char a[5]; };
    static Yes test(Parent *); // undefined
    static No  test(...);      // undefined
    
    public:

    enum { Res =  sizeof(test(static_cast<Child *>(0))) == sizeof(Yes) ? 1 : 0 };
};

template<bool> class Bool {};

} // namespace _private_

//________________________________________________________________________________________
//
// transition
//________________________________________________________________________________________

template<typename C, typename S, typename T>
struct transition
{
    typedef C Current;
    typedef S Source;
    typedef T Target;
    typedef typename Current::Extended Extended;
    typedef typename Current::Parent Current_parent;
    typedef typename Target::Parent Target_parent;

    enum // work out when to terminate template recursion
    {
        eTB_CB    = _private_::Is_child<Target_parent, Current_parent>::Res,
        eS_CB     = _private_::Is_child<Source       , Current_parent>::Res,
        eS_C      = _private_::Is_child<Source       , Current       >::Res,
        eC_S      = _private_::Is_child<Current      , Source        >::Res,
        exitStop  = eTB_CB && eS_C,
        entryStop = eS_C || (eS_CB && !eC_S)
    };

    transition(Extended & arg)
        : _extended(arg)
    {
        exit_actions(_extended, _private_::Bool<false>());
    }

    ~transition()
    {
        typedef transition<Target, Source, Target> Trans;
        Trans::entry_actions(_extended, _private_::Bool<false>());
        Target::handle_init(_extended);
    }

    // We use overloading to stop recursion. The more natural template
    // specialization method would require to specialize the inner
    // template without specializing the outer one, which is
    // forbidden.
    static void exit_actions (Extended &  , _private_::Bool<true> ) {}
    static void exit_actions (Extended & e, _private_::Bool<false>)
    {
        typedef transition<Current_parent, Source, Target> Trans;
        Current::handle_exit(e);
        Trans::exit_actions(e, _private_::Bool<exitStop>());
    };
    
    static void entry_actions(Extended &, _private_::Bool<true>) {}
    static void entry_actions(Extended & e, _private_::Bool<false>)
    {
        typedef transition<Current_parent, Source, Target> Trans;
        Trans::entry_actions(e, _private_::Bool<entryStop>());
        Current::handle_entry(e);
    };

    private:
    
    Extended & _extended;
};

} // namespace hsm
} // namespace etl

#endif // ETL_HSM_INCLUDED
