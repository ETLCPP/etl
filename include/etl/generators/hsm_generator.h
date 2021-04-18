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

/*[[[cog
import cog
cog.outl('//________________________________________________________________________________________')
cog.outl('//')
cog.outl('// THIS FILE IS AUTO-GENERATED. DO NOT EDIT. CHANGES WILL BE OVERWRITTEN !')
cog.outl('//________________________________________________________________________________________')
cog.outl('')
]]]*/
/*[[[end]]]*/
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
    virtual void process_do(Extended &) const = 0;
    virtual void process_event(etl::imessage const &, Extended &) const = 0;
    virtual bool accepts_event(etl::message_id_t) const = 0;
    virtual unsigned get_id() const = 0;
};
/*[[[cog
import cog

def message_types(prefix, msgs, nfirstline, notherlines, suffix=''):
    sep = ','
    s = ''
    for msg_idx in range(0, msgs):
        if msg_idx == 0:
            sep = ''
        else:
            sep = ','

        linebreak = (msg_idx - nfirstline) % notherlines == 0
        if linebreak:
            s += '{}\n{:>9}{}{:02d}{}'.format(sep, '', prefix, msg_idx, suffix)
        else:
            s += '{} {}{:02d}{}'.format(sep, prefix, msg_idx, suffix)
    return s

def typename_M_eq_void(n):
    return message_types(prefix='typename M', msgs=n, nfirstline=0, notherlines=3,
                         suffix=' = void')

def typename_M(n):
    return message_types(prefix='typename M', msgs=n, nfirstline=0, notherlines=4)

def M(n):
    return message_types(prefix='M', msgs=n, nfirstline=0, notherlines=8)

def header_comment(text):
    cog.outl('')
    cog.outl('//________________________________________________________________________________________')
    cog.outl('//')
    cog.outl('//  {}'.format(text))
    cog.outl('//________________________________________________________________________________________')
    cog.outl('')

def create_class(classname, is_declaration, is_topspec, is_simple, n):
    if is_declaration:
        if is_simple:
            cog.outl(    'template<typename E, unsigned ID, typename A, typename P = composite<E, 0, top<E> >, {}>'.format(typename_M(n)))
        else:
            cog.outl(    'template<typename E, unsigned ID, typename P = composite<E, 0, top<E> >, {}>'.format(typename_M(n)))
        cog.outl(        'class {} : public P'.format(classname))
    else:
        if is_topspec:
            cog.outl(    'template<typename E>')
            cog.outl(    'class {}<E, 0> : public top<E>'.format(classname))
        else:
            if is_simple:
                cog.outl('template<typename E, unsigned ID, typename A, typename P, {}>'.format(typename_M(n)))
                cog.outl('class {}<E, ID, A, P, {}> : public P'.format(classname, M(n)))
            else:
                cog.outl('template<typename E, unsigned ID, typename P, {}>'.format(typename_M(n)))
                cog.outl('class {}<E, ID, P, {}> : public P'.format(classname, M(n)))
    cog.outl(            '{')
    cog.outl(            'public:')
    cog.outl(            '    typedef E Extended;')
    if is_topspec:
        cog.outl(        '    typedef top<E> Parent;')
        cog.outl(        '    typedef {}<E, 0, top<E> > Self;'.format(classname))
    else:
        cog.outl(        '    typedef P Parent;')
        if is_simple:
            cog.outl(    '    typedef A Allowed_ids;')
            cog.outl(    '    typedef {}<E, ID, A, P, {}> Self;'.format(classname, M(n)))
        else:
            cog.outl(    '    typedef {}<E, ID, P, {}> Self;'.format(classname, M(n)))
    cog.outl(            '')
    cog.outl(            '    static void handle_entry(Extended &) {}')
    cog.outl(            '    static void handle_exit (Extended &) {}')
    cog.outl(            '    static void handle_do   (Extended &) {}')
    if is_simple:
        cog.outl(        '    static void handle_init (Extended & arg)')
        cog.outl(        '    {')
        cog.outl(        '        arg.set_state(obj);')
        cog.outl(        '    }')
        cog.outl(        '')
        cog.outl(        '    static const {} obj;'.format(classname))
        cog.outl(        '')
        cog.outl(        '    virtual unsigned get_id() const override')
        cog.outl(        '    {')
        cog.outl(        '        return ID;')
        cog.outl(        '    }')
        cog.outl(        '')
        cog.outl(        '    virtual void process_do(Extended & e) const override')
        cog.outl(        '    {')
        cog.outl(        '        Parent::topdown_handle_do(e);')
        cog.outl(        '        handle_do(e);')
        cog.outl(        '    }')
        cog.outl(        '')
        cog.outl(        '    virtual void process_event(etl::imessage const & m, Extended & e) const override')
        cog.outl(        '    {')
        cog.outl(        '        if ( state_accepts_event( m.get_message_id() ))')
        cog.outl(        '        {')
        cog.outl(        '            process_do(e);')
        cog.outl(        '            handle_event(m, e, *this);')
        cog.outl(        '        }')
        cog.outl(        '    }')
    else:
        cog.outl(        '    static void handle_init (Extended &);')
    cog.outl(            '')
    if is_topspec:
        cog.outl(        '    static bool state_accepts_event(etl::message_id_t)')
        cog.outl(        '    {')
        cog.outl(        '        return false;')
    else:
        cog.outl(        '    static bool state_accepts_event(etl::message_id_t id)')
        cog.outl(        '    {')
        if n > 0:
            cog.outl(    '        switch( id )')
            cog.outl(    '        {')
            for t in range( n ):
                cog.outl('            case M{:02d}::ID:'.format(t))
            cog.outl(    '                return true;')
            cog.outl(    '            default:')
            cog.outl(    '                return Parent::state_accepts_event( id );')
            cog.outl(    '        };')
        else:
            cog.outl(    '        return Parent::state_accepts_event( id );')
    cog.outl(            '    }')
    if not is_simple:
        cog.outl(        '')
        cog.outl(        '    static void topdown_handle_do(Extended & e)')
        cog.outl(        '    {')
        if not is_topspec:
            cog.outl(    '        Parent::topdown_handle_do(e);')
        cog.outl(        '        handle_do(e);')
        cog.outl(        '    }')
    cog.outl(            '')
    if is_simple:
        cog.outl(        '    bool accepts_event(etl::message_id_t arg) const override')
        cog.outl(        '    {                                                                      ')
        cog.outl(        '         return is_in( arg, Allowed_ids {} );                                           ')
        cog.outl(        '    }                                                                      ')
        cog.outl(        '')
    cog.outl(            '    template<typename SIMPLE>')
    if is_topspec:
        cog.outl(        '    void handle_event(etl::imessage const & m, Extended & e, SIMPLE const & ) const')
    else:
        cog.outl(        '    void handle_event(etl::imessage const & m, Extended & e, SIMPLE const & s) const')
    cog.outl(            '    {')
    if n > 0:
        cog.outl(        '        switch ( m.get_message_id() )')
        cog.outl(        '        {')
        for t in range(n):
            cog.outl(    '            case M{:02d}::ID: handle_do(e); on_event<SIMPLE>( static_cast<M{:02d} const &>( m ), e, s ); return;'.format(t, t))
        cog.outl(        '        }')
    if is_topspec:
        cog.outl(        '        if ( e.has_successor() )')
        cog.outl(        '        {')
        cog.outl(        '            e.get_successor().receive(m);')
        cog.outl(        '        }')
        cog.outl(        '        else')
        cog.outl(        '        {')
        cog.outl(        '            e.on_receive_unknown(m);')
        cog.outl(        '        }')
    else:
        cog.outl(        '        Parent::handle_event(m, e, s);')
    cog.outl(            '    }')
    if n> 0:
        cog.outl(        '')
        cog.outl(        'private:')
    for t in range(n):
        cog.outl(        '    template<typename SIMPLE> void on_event( M{:02d} const &, Extended &, SIMPLE const & ) const;'.format(t))
    cog.outl(            '};')
    if is_simple:
        cog.outl(        '')
        cog.outl(        'template<typename E, unsigned ID, typename A, typename P, {}>'.format(typename_M(n)))
        cog.outl(        'const {c}<E, ID, A, P, {m}>'.format(c = classname, m = M(n)))
        cog.outl(        '      {c}<E, ID, A, P, {m}>::obj;'.format(c = classname, m = M(n)))

def create_extended(is_declaration, n):
    if is_declaration:
        cog.outl(        'template<typename TDerived, {}>'.format(typename_M(n)))
        cog.outl(        'class extended : public etl::imessage_router')
    else:
        cog.outl(        'template<typename TDerived, {}>'.format(typename_M(n)))
        cog.outl(        'class extended<TDerived, {}> : public etl::imessage_router'.format(M(n)))
    cog.outl(            '{')
    cog.outl(            'public:')
    cog.outl(            '    typedef TDerived derived;')
    cog.outl(            '    typedef top<derived> state;')
    cog.outl(            '    typedef etl::message_packet<{}> message_packet;'.format(M(n)))
    cog.outl(            '')
    cog.outl(            '    extended(etl::message_router_id_t id)')
    cog.outl(            '        : etl::imessage_router(id)')
    cog.outl(            '    {')
    cog.outl(            '        ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER,')
    cog.outl(            '                   ETL_ERROR(etl::message_router_illegal_id));')
    cog.outl(            '    }')
    cog.outl(            '    extended(etl::message_router_id_t id, etl::imessage_router& successor_)')
    cog.outl(            '        : etl::imessage_router(id, successor_)')
    cog.outl(            '    {')
    cog.outl(            '        ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER,')
    cog.outl(            '                   ETL_ERROR(etl::message_router_illegal_id));')
    cog.outl(            '    }')
    cog.outl(            '    // Non-mutating methods')
    cog.outl(            '    bool          is_started    () const { return _state != nullptr; };')
    cog.outl(            '    state const * get_state     () const { return _state; };')
    cog.outl(            '    bool          is_null_router() const ETL_OVERRIDE { return false; }')
    cog.outl(            '    bool          is_producer   () const ETL_OVERRIDE { return true;  }')
    cog.outl(            '    bool          is_consumer   () const ETL_OVERRIDE { return true;  }')
    cog.outl(            '')
    cog.outl(            '    // Mutating methods')
    cog.outl(            '    void set_state(state const & arg) { _state = &arg; };')
    cog.outl(            '    void receive(etl::imessage const & message) override')
    cog.outl(            '    {')
    cog.outl(            '        get_state()->process_event(message, static_cast<derived&>(* this));')
    cog.outl(            '    }')
    cog.outl(            '')
    cog.outl(            '    virtual void on_receive_unknown(etl::imessage const &) {}')
    cog.outl(            '')
    cog.outl(            '    using imessage_router::accepts;')
    cog.outl(            '    bool accepts(etl::message_id_t id) const ETL_OVERRIDE')
    cog.outl(            '    {')
    cog.outl(            '        switch (id)')
    cog.outl(            '        {')
    for t in range( n ):
        cog.outl(        '        case M{:02d}::ID:'.format(t))
    cog.outl(            '            return true;')
    cog.outl(            '        default:')
    cog.outl(            '            return false;')
    cog.outl(            '        }')
    cog.outl(            '    }')
    cog.outl(            '')
    cog.outl(            'private:')
    cog.outl(            '    const state* _state { nullptr };')
    cog.outl(            '};')

#_________________________________________________________________________________________
#
#  Creating classes here
#_________________________________________________________________________________________

h = int(Handlers)

header_comment(          'composite declaration for {} messages'.format(h))
cog.outl(                'template<typename E, unsigned ID, typename P, {}>'.format(typename_M_eq_void(h)))
cog.outl(                'class {};\n'.format('composite'))
create_class(            'composite', is_declaration=True, is_topspec=False, is_simple=False, n=h)

header_comment(          'composite specialisation for top-state which has class top as parent')
create_class(            'composite', is_declaration=False, is_topspec=True, is_simple=False, n=0)

for i in range(h - 1, 0, -1):
    header_comment(      'composite specialisation for {} messages'.format(i))
    create_class(        'composite', is_declaration=False, is_topspec=False, is_simple=False, n=i)

header_comment(          'simple declaration for {} messages'.format(h))
cog.outl(                'template<typename E, unsigned ID, typename A, typename P, {}>'.format(typename_M_eq_void(h)))
cog.outl(                'class {};\n'.format('simple'))

create_class(            'simple', is_declaration=True, is_topspec=False, is_simple=True, n=h)

for i in range(h - 1, 0, -1):
    header_comment(      'simple specialisation for {} messages'.format(i))
    create_class(        'simple', is_declaration=False, is_topspec=False, is_simple=True, n = i)


header_comment(          'extended declaration for {} messages'.format(h))
cog.outl(                'template<typename TDerived, {}>'.format(typename_M_eq_void(h)))
cog.outl(                'class {};\n'.format('extended'))

create_extended( is_declaration=True, n = h)

for i in range(h - 1, 0, -1):
    header_comment( 'extended specialisation for {} messages'.format(i))
    create_extended( is_declaration=False, n = i)

]]]*/
/*[[[end]]]*/

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
