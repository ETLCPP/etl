///\file

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

#ifndef ETL_VISITOR_INCLUDED
#define ETL_VISITOR_INCLUDED

#include "platform.h"
#include "type_traits.h"

//*****************************************************************************
///\defgroup visitor visitor
/// A set of template classes for easy implementation of the visitor pattern.<br>
/// The visitor design pattern is a way of separating an algorithm from an object
/// structure on which it operates. A practical result of this separation is the
/// ability to add new operations to existing object structures without modifying
/// those structures. It is one way to easily follow the open/closed principle.
/// In essence, the visitor allows one to add new virtual functions to a family
/// of classes without modifying the classes themselves; instead, one creates a
/// visitor class that implements all of the appropriate specialisations of the
/// virtual function. The visitor takes the instance as input, and implements
/// the goal through double dispatch.<br>
/// \ingroup patterns
//*****************************************************************************

namespace etl
{
#if ETL_USING_CPP11 && !defined(ETL_VISITOR_FORCE_CPP03_IMPLEMENTATION)

  //*****************************************************************
  /// The visitable class for N types.
  ///\ingroup visitor
  //*****************************************************************
  template <typename T1, typename... Types>
  class visitable : public visitable<T1>, public visitable<Types...>
  {
  public:

    using visitable<T1>::accept;
    using visitable<Types...>::accept;
  };

  //*****************************************************************
  /// The specialised visitable class for 1 type.
  ///\ingroup visitor
  //*****************************************************************
  template <typename T1>
  class visitable<T1>
  {
  public:

    virtual ~visitable() = default;

    virtual void accept(T1&) = 0;
  };

#else

  //*****************************************************************
  /// The visitable base class for four visitor types.
  /// Derive visitable classes from this.
  ///\ingroup visitor
  //*****************************************************************
  template <typename T1, typename T2 = void, typename T3 = void, typename T4 = void>
  class visitable
  {
  public:

    virtual ~visitable() {}

    virtual void accept(T1&) = 0;
    virtual void accept(T2&) = 0;
    virtual void accept(T3&) = 0;
    virtual void accept(T4&) = 0;
  };

  //*****************************************************************
  /// The visitable base class for three visitor types.
  /// Derive visitable classes from this.
  ///\ingroup visitor
  //*****************************************************************
  template <typename T1, typename T2, typename T3>
  class visitable<T1, T2, T3>
  {
  public:

    virtual ~visitable() {}

    virtual void accept(T1&) = 0;
    virtual void accept(T2&) = 0;
    virtual void accept(T3&) = 0;
  };

  //*****************************************************************
  /// The visitable base class for two visitor types.
  /// Derive visitable classes from this.
  ///\ingroup visitor
  //*****************************************************************
  template <typename T1, typename T2>
  class visitable<T1, T2>
  {
  public:

    virtual ~visitable() {}

    virtual void accept(T1&) = 0;
    virtual void accept(T2&) = 0;
  };

  //*****************************************************************
  /// The visitable base class for one visitor type.
  /// Derive visitable classes from this.
  ///\ingroup visitor
  //*****************************************************************
  template <typename T1>
  class visitable<T1>
  {
  public:

    virtual ~visitable() {}

    virtual void accept(T1&) = 0;
  };

#endif

  //*****************************************************************
  /// The tag to identify an etl::visitor.
  ///\ingroup visitor
  //*****************************************************************
  struct visitor_tag
  {
  };

#if ETL_USING_CPP11 && !defined(ETL_VISITOR_FORCE_CPP03_IMPLEMENTATION)

  //*****************************************************************
  /// The visitor class for N types.
  ///\ingroup visitor
  //*****************************************************************
  template <typename T1, typename... Types>
  class visitor : public visitor<T1>, public visitor<Types...>
  {
  public:

    using visitor<T1>::visit;
    using visitor<Types...>::visit;
  };

  //*****************************************************************
  /// The specialised visitor class for 1 type.
  ///\ingroup visitor
  //*****************************************************************
  template <typename T1>
  class visitor<T1> : public virtual visitor_tag
  {
  public:

    virtual ~visitor() = default;

    virtual void visit(T1) = 0;
  };

#else

  //*****************************************************************
  /// The visitor base class for sixteen types.
  /// Derive visitors from this.
  ///\ingroup visitor
  //*****************************************************************
  template <typename T1,         typename T2  = void, typename T3  = void, typename T4  = void,
            typename T5  = void, typename T6  = void, typename T7  = void, typename T8  = void,
            typename T9  = void, typename T10 = void, typename T11 = void, typename T12 = void,
            typename T13 = void, typename T14 = void, typename T15 = void, typename T16 = void>
  class visitor : public visitor_tag
  {
  public:

    virtual ~visitor() {}

    virtual void visit(T1) = 0;
    virtual void visit(T2) = 0;
    virtual void visit(T3) = 0;
    virtual void visit(T4) = 0;
    virtual void visit(T5) = 0;
    virtual void visit(T6) = 0;
    virtual void visit(T7) = 0;
    virtual void visit(T8) = 0;
    virtual void visit(T9) = 0;
    virtual void visit(T10) = 0;
    virtual void visit(T11) = 0;
    virtual void visit(T12) = 0;
    virtual void visit(T13) = 0;
    virtual void visit(T14) = 0;
    virtual void visit(T15) = 0;
    virtual void visit(T16) = 0;
  };

  //*****************************************************************
  /// The visitor base class for fifteen types.
  /// Derive visitors from this.
  ///\ingroup visitor
  //*****************************************************************
  template <typename T1,  typename T2,  typename T3,  typename T4,
            typename T5,  typename T6,  typename T7,  typename T8,
            typename T9,  typename T10, typename T11, typename T12,
            typename T13, typename T14, typename T15>
  class visitor<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> : public visitor_tag
  {
  public:

    virtual ~visitor() {}

    virtual void visit(T1) = 0;
    virtual void visit(T2) = 0;
    virtual void visit(T3) = 0;
    virtual void visit(T4) = 0;
    virtual void visit(T5) = 0;
    virtual void visit(T6) = 0;
    virtual void visit(T7) = 0;
    virtual void visit(T8) = 0;
    virtual void visit(T9) = 0;
    virtual void visit(T10) = 0;
    virtual void visit(T11) = 0;
    virtual void visit(T12) = 0;
    virtual void visit(T13) = 0;
    virtual void visit(T14) = 0;
    virtual void visit(T15) = 0;
  };

  //*****************************************************************
  /// The visitor base class for fourteen types.
  /// Derive visitors from this.
  ///\ingroup visitor
  //*****************************************************************
  template <typename T1,  typename T2,  typename T3,  typename T4,
            typename T5,  typename T6,  typename T7,  typename T8,
            typename T9,  typename T10, typename T11, typename T12,
            typename T13, typename T14>
  class visitor<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> : public visitor_tag
  {
  public:

    virtual ~visitor() {}

    virtual void visit(T1) = 0;
    virtual void visit(T2) = 0;
    virtual void visit(T3) = 0;
    virtual void visit(T4) = 0;
    virtual void visit(T5) = 0;
    virtual void visit(T6) = 0;
    virtual void visit(T7) = 0;
    virtual void visit(T8) = 0;
    virtual void visit(T9) = 0;
    virtual void visit(T10) = 0;
    virtual void visit(T11) = 0;
    virtual void visit(T12) = 0;
    virtual void visit(T13) = 0;
    virtual void visit(T14) = 0;
  };

  //*****************************************************************
  /// The visitor base class for thirteen types.
  /// Derive visitors from this.
  ///\ingroup visitor
  //*****************************************************************
  template <typename T1, typename T2,  typename T3,  typename T4,
            typename T5, typename T6,  typename T7,  typename T8,
            typename T9, typename T10, typename T11, typename T12,
            typename T13>
  class visitor<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> : public visitor_tag
  {
  public:

    virtual ~visitor() {}

    virtual void visit(T1) = 0;
    virtual void visit(T2) = 0;
    virtual void visit(T3) = 0;
    virtual void visit(T4) = 0;
    virtual void visit(T5) = 0;
    virtual void visit(T6) = 0;
    virtual void visit(T7) = 0;
    virtual void visit(T8) = 0;
    virtual void visit(T9) = 0;
    virtual void visit(T10) = 0;
    virtual void visit(T11) = 0;
    virtual void visit(T12) = 0;
    virtual void visit(T13) = 0;
  };

  //*****************************************************************
  /// The visitor base class for twelve types.
  /// Derive visitors from this.
  ///\ingroup visitor
  //*****************************************************************
  template <typename T1, typename T2,  typename T3,  typename T4,
            typename T5, typename T6,  typename T7,  typename T8,
            typename T9, typename T10, typename T11, typename T12>
  class visitor<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> : public visitor_tag
  {
  public:

    virtual ~visitor() {}

    virtual void visit(T1) = 0;
    virtual void visit(T2) = 0;
    virtual void visit(T3) = 0;
    virtual void visit(T4) = 0;
    virtual void visit(T5) = 0;
    virtual void visit(T6) = 0;
    virtual void visit(T7) = 0;
    virtual void visit(T8) = 0;
    virtual void visit(T9) = 0;
    virtual void visit(T10) = 0;
    virtual void visit(T11) = 0;
    virtual void visit(T12) = 0;
  };

  //*****************************************************************
  /// The visitor base class for eleven types.
  /// Derive visitors from this.
  ///\ingroup visitor
  //*****************************************************************
  template <typename T1, typename T2,  typename T3, typename T4,
            typename T5, typename T6,  typename T7, typename T8,
            typename T9, typename T10, typename T11>
  class visitor<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> : public visitor_tag
  {
  public:

    virtual ~visitor() {}

    virtual void visit(T1) = 0;
    virtual void visit(T2) = 0;
    virtual void visit(T3) = 0;
    virtual void visit(T4) = 0;
    virtual void visit(T5) = 0;
    virtual void visit(T6) = 0;
    virtual void visit(T7) = 0;
    virtual void visit(T8) = 0;
    virtual void visit(T9) = 0;
    virtual void visit(T10) = 0;
    virtual void visit(T11) = 0;
  };

  //*****************************************************************
  /// The visitor base class for ten types.
  /// Derive visitors from this.
  ///\ingroup visitor
  //*****************************************************************
  template <typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7, typename T8,
            typename T9, typename T10>
  class visitor<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> : public visitor_tag
  {
  public:

    virtual ~visitor() {}

    virtual void visit(T1) = 0;
    virtual void visit(T2) = 0;
    virtual void visit(T3) = 0;
    virtual void visit(T4) = 0;
    virtual void visit(T5) = 0;
    virtual void visit(T6) = 0;
    virtual void visit(T7) = 0;
    virtual void visit(T8) = 0;
    virtual void visit(T9) = 0;
    virtual void visit(T10) = 0;
  };

  //*****************************************************************
  /// The visitor base class for nine types.
  /// Derive visitors from this.
  ///\ingroup visitor
  //*****************************************************************
  template <typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7, typename T8,
            typename T9>
  class visitor<T1, T2, T3, T4, T5, T6, T7, T8, T9> : public visitor_tag
  {
  public:

    virtual ~visitor() {}

    virtual void visit(T1) = 0;
    virtual void visit(T2) = 0;
    virtual void visit(T3) = 0;
    virtual void visit(T4) = 0;
    virtual void visit(T5) = 0;
    virtual void visit(T6) = 0;
    virtual void visit(T7) = 0;
    virtual void visit(T8) = 0;
    virtual void visit(T9) = 0;
  };

  //*****************************************************************
  /// The visitor base class for eight types.
  /// Derive visitors from this.
  ///\ingroup visitor
  //*****************************************************************
  template <typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7, typename T8>
  class visitor<T1, T2, T3, T4, T5, T6, T7, T8> : public visitor_tag
  {
  public:

    virtual ~visitor() {}

    virtual void visit(T1) = 0;
    virtual void visit(T2) = 0;
    virtual void visit(T3) = 0;
    virtual void visit(T4) = 0;
    virtual void visit(T5) = 0;
    virtual void visit(T6) = 0;
    virtual void visit(T7) = 0;
    virtual void visit(T8) = 0;
  };

  //*****************************************************************
  /// The visitor base class for seven types.
  /// Derive visitors from this.
  ///\ingroup visitor
  //*****************************************************************
  template <typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7>
  class visitor<T1, T2, T3, T4, T5, T6, T7> : public visitor_tag
  {
  public:

    virtual ~visitor() {}

    virtual void visit(T1) = 0;
    virtual void visit(T2) = 0;
    virtual void visit(T3) = 0;
    virtual void visit(T4) = 0;
    virtual void visit(T5) = 0;
    virtual void visit(T6) = 0;
    virtual void visit(T7) = 0;
  };

  //*****************************************************************
  /// The visitor base class for six types.
  /// Derive visitors from this.
  ///\ingroup visitor
  //*****************************************************************
  template <typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6>
  class visitor<T1, T2, T3, T4, T5, T6> : public visitor_tag
  {
  public:

    virtual ~visitor() {}

    virtual void visit(T1) = 0;
    virtual void visit(T2) = 0;
    virtual void visit(T3) = 0;
    virtual void visit(T4) = 0;
    virtual void visit(T5) = 0;
    virtual void visit(T6) = 0;
  };

  //*****************************************************************
  /// The visitor base class for five types.
  /// Derive visitors from this.
  ///\ingroup visitor
  //*****************************************************************
  template <typename T1, typename T2, typename T3, typename T4,
            typename T5>
  class visitor<T1, T2, T3, T4, T5> : public visitor_tag
  {
  public:

    virtual ~visitor() {}

    virtual void visit(T1) = 0;
    virtual void visit(T2) = 0;
    virtual void visit(T3) = 0;
    virtual void visit(T4) = 0;
    virtual void visit(T5) = 0;
  };

  //*****************************************************************
  /// The visitor base class for four types.
  /// Derive visitors from this.
  ///\ingroup visitor
  //*****************************************************************
  template <typename T1, typename T2, typename T3, typename T4>
  class visitor<T1, T2, T3, T4> : public visitor_tag
  {
  public:

    virtual ~visitor() {}

    virtual void visit(T1) = 0;
    virtual void visit(T2) = 0;
    virtual void visit(T3) = 0;
    virtual void visit(T4) = 0;
  };

  //*****************************************************************
  /// The visitor base class for three types.
  /// Derive visitors from this.
  ///\ingroup visitor
  //*****************************************************************
  template <typename T1, typename T2, typename T3>
  class visitor<T1, T2, T3> : public visitor_tag
  {
  public:

    virtual ~visitor() {}

    virtual void visit(T1) = 0;
    virtual void visit(T2) = 0;
    virtual void visit(T3) = 0;
  };

  //*****************************************************************
  /// The visitor base class for two types.
  /// Derive visitors from this.
  ///\ingroup visitor
  //*****************************************************************
  template <typename T1, typename T2>
  class visitor<T1, T2> : public visitor_tag
  {
  public:

    virtual ~visitor() {}

    virtual void visit(T1) = 0;
    virtual void visit(T2) = 0;
  };

  //*****************************************************************
  /// The visitor base class for one type.
  /// Derive visitors from this.
  ///\ingroup visitor
  //*****************************************************************
  template <typename T1>
  class visitor<T1> : public visitor_tag
  {
  public:

    virtual ~visitor() {}

    virtual void visit(T1) = 0;
  };

#endif

  //*****************************************************************
  /// Is the type an etl::visitor?
  ///\ingroup visitor
  //*****************************************************************
  template <typename T>
  struct is_visitor : public etl::bool_constant<etl::is_base_of<etl::visitor_tag, T>::value>
  {
  };

#if ETL_USING_CPP17
  //*****************************************************************
  /// Is the type an etl::visitor?
  ///\ingroup visitor
  //*****************************************************************
  template <typename T>
  constexpr bool is_visitor_v = etl::is_visitor<T>::value;
#endif
}

#endif
