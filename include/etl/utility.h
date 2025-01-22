///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2016 John Wellbelove

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

#ifndef ETL_UTILITY_INCLUDED
#define ETL_UTILITY_INCLUDED

#include "platform.h"
#include "type_traits.h"

#if defined(ETL_IN_UNIT_TEST) || ETL_USING_STL
  #if ETL_USING_CPP11
    #include <utility>
  #else
    #include <algorithm>
  #endif
#endif

///\defgroup utility utility
///\ingroup utilities

namespace etl
{
#if ETL_USING_CPP11
  //******************************************************************************
  template <typename T>
  constexpr typename etl::remove_reference<T>::type&& move(T&& t) ETL_NOEXCEPT
  {
    return static_cast<typename etl::remove_reference<T>::type&&>(t);
  }

  //******************************************************************************
  template <typename T>
  constexpr T&& forward(typename etl::remove_reference<T>::type& t) ETL_NOEXCEPT
  {
    return static_cast<T&&>(t);
  }

  template <typename T>
  constexpr T&& forward(typename etl::remove_reference<T>::type&& t) ETL_NOEXCEPT
  {
    ETL_STATIC_ASSERT(!etl::is_lvalue_reference<T>::value, "Invalid rvalue to lvalue conversion");
    return static_cast<T&&>(t);
  }

  //******************************************************************************
  /// See std::forward_like https://en.cppreference.com/w/cpp/utility/forward_like
  /// Returns a reference to x which has similar properties to T&&.
  ///\return
  /// If etl::remove_reference_t<T> is const then returns a const reference if U is an lvalue, otherwise a const rvalue reference.
  /// If etl::remove_reference_t<T> is not const then returns a reference if U is an lvalue, otherwise an rvalue reference.
  //******************************************************************************
  //***********************************
  /// T is const & lvalue.
  //***********************************
  template <typename T, typename U>
  ETL_NODISCARD
  ETL_CONSTEXPR
  etl::enable_if_t<etl::is_const<etl::remove_reference_t<T>>::value && etl::is_lvalue_reference<T>::value, const etl::remove_reference_t<U>&>
    forward_like(U&& u) ETL_NOEXCEPT
  {
    return static_cast<const etl::remove_reference_t<U>&>(u);
  }

  //***********************************
  /// T is const & rvalue.
  //***********************************
  template <typename T, typename U>
  ETL_NODISCARD
  ETL_CONSTEXPR
  etl::enable_if_t<etl::is_const<etl::remove_reference_t<T>>::value && !etl::is_lvalue_reference<T>::value, const etl::remove_reference_t<U>&&>
    forward_like(U&& u) ETL_NOEXCEPT
  {
    return static_cast<const etl::remove_reference_t<U>&&>(u);
  }

  //***********************************
  /// T is not const & lvalue.
  //***********************************
  template <typename T, typename U>
  ETL_NODISCARD
  ETL_CONSTEXPR
  etl::enable_if_t<!etl::is_const<etl::remove_reference_t<T>>::value && etl::is_lvalue_reference<T>::value, etl::remove_reference_t<U>&>
    forward_like(U&& u) ETL_NOEXCEPT
  {
    return static_cast<etl::remove_reference_t<U>&>(u);
  }

  //***********************************
  /// T is not const & rvalue.
  //***********************************
  template <typename T, typename U>
  ETL_NODISCARD
  ETL_CONSTEXPR
  etl::enable_if_t<!etl::is_const<etl::remove_reference_t<T>>::value && !etl::is_lvalue_reference<T>::value, etl::remove_reference_t<U>&&>
    forward_like(U&& u) ETL_NOEXCEPT
  {
    return static_cast<etl::remove_reference_t<U>&&>(u);
  }

  //***********************************
  // Defines the type that forward_like would cast to.
  //***********************************
  template <typename T, typename U>
  using forward_like_t = decltype(etl::forward_like<T>(etl::declval<U&>()));
#endif

  // We can't have std::swap and etl::swap templates coexisting in the unit tests
  // as the compiler will be unable to decide which one to use, due to ADL.
#if ETL_NOT_USING_STL && !defined(ETL_IN_UNIT_TEST)
  //***************************************************************************
  // swap
  template <typename T>
  ETL_CONSTEXPR14 void swap(T& a, T& b) ETL_NOEXCEPT
  {
    T temp(ETL_MOVE(a));
    a = ETL_MOVE(b);
    b = ETL_MOVE(temp);
  }

  template< class T, size_t N >
  ETL_CONSTEXPR14 void swap(T(&a)[N], T(&b)[N]) ETL_NOEXCEPT
  {
    for (size_t i = 0UL; i < N; ++i)
    {
      swap(a[i], b[i]);
    }
  }
#endif

  //***************************************************************************
  ///\brief pair holds two objects of arbitrary type
  ///
  ///\tparam T1, T2 The types of the elements that the pair stores
  //***************************************************************************
  template <typename T1, typename T2>
  struct pair
  {
    typedef T1 first_type;   ///< @c first_type is the first bound type
    typedef T2 second_type;  ///< @c second_type is the second bound type

    T1 first;   ///< @c first is a copy of the first object
    T2 second;  ///< @c second is a copy of the second object

    //***************************************************************************
    ///\brief Default constructor
    ///
    /// The default constructor creates @c first and @c second using their respective default constructors.
    //***************************************************************************
    ETL_CONSTEXPR pair()
      : first(T1())
      , second(T2())
    {
    }

    //***************************************************************************
    ///\brief Constructor from parameters
    ///
    /// Two objects may be passed to a @c pair constructor to be copied.
    //***************************************************************************
    ETL_CONSTEXPR14 pair(const T1& a, const T2& b)
      : first(a)
      , second(b)
    {
    }

#if ETL_USING_CPP11
    //***************************************************************************
    ///\brief Move constructor from parameters.
    //***************************************************************************
    template <typename U1, typename U2>
    ETL_CONSTEXPR14 pair(U1&& a, U2&& b)
      : first(etl::forward<U1>(a))
      , second(etl::forward<U2>(b))
    {
    }
#endif

    //***************************************************************************
    ///\brief Copy constructor
    ///
    /// There is also a templated copy constructor for the @c pair class itself.
    //***************************************************************************
    template <typename U1, typename U2>
    ETL_CONSTEXPR14 pair(const pair<U1, U2>& other)
      : first(other.first)
      , second(other.second)
    {
    }

    /// Copy constructor
    pair(const pair<T1, T2>& other)
      : first(other.first)
      , second(other.second)
    {
    }

#if ETL_USING_CPP11
    /// Move constructor
    template <typename U1, typename U2>
    ETL_CONSTEXPR14 pair(pair<U1, U2>&& other)
      : first(etl::forward<U1>(other.first))
      , second(etl::forward<U2>(other.second))
    {
    }
#endif

#if defined(ETL_IN_UNIT_TEST) || ETL_USING_STL
    /// Converting to std::pair
    template <typename U1, typename U2>
    operator std::pair<U1, U2>()
    {
      return std::make_pair(first, second);
    }

    /// Constructing from std::pair
    template <typename U1, typename U2>
    pair(const std::pair<U1, U2>& other)
      : first(other.first)
      , second(other.second)
    {
    }

#if ETL_USING_CPP11
    /// Constructing to etl::pair
    template <typename U1, typename U2>
    pair(std::pair<U1, U2>&& other)
      : first(etl::forward<U1>(other.first))
      , second(etl::forward<U2>(other.second))
    {
    }
#endif
#endif

    void swap(pair<T1, T2>& other)
    {
      using ETL_OR_STD::swap;

      swap(first, other.first);
      swap(second, other.second);
    }

    pair<T1, T2>& operator =(const pair<T1, T2>& other)
    {
      first = other.first;
      second = other.second;

      return *this;
    }

    template <typename U1, typename U2>
    pair<U1, U2>& operator =(const pair<U1, U2>& other)
    {
      first = other.first;
      second = other.second;

      return *this;
    }

#if ETL_USING_CPP11
    pair<T1, T2>& operator =(pair<T1, T2>&& other)
    {
      first = etl::forward<T1>(other.first);
      second = etl::forward<T2>(other.second);

      return *this;
    }

    template <typename U1, typename U2>
    pair<U1, U2>& operator =(pair<U1, U2>&& other)
    {
      first = etl::forward<U1>(other.first);
      second = etl::forward<U2>(other.second);

      return *this;
    }
#endif
  };

  //***************************************************************************
  ///\brief A convenience wrapper for creating a @ref pair from two objects.
  ///
  ///\param a The first object.
  ///\param b The second object.
  ///
  ///\return A newly-constructed @ref pair object of the appropriate type.
  //***************************************************************************
#if ETL_USING_CPP11
  template <typename T1, typename T2>
  inline pair<T1, T2> make_pair(T1&& a, T2&& b)
  {
    return pair<T1, T2>(etl::forward<T1>(a), etl::forward<T2>(b));
  }
#else
  template <typename T1, typename T2>
  inline pair<T1, T2> make_pair(T1 a, T2 b)
  {
    return pair<T1, T2>(a, b);
  }
#endif

  //******************************************************************************
  template <typename T1, typename T2>
  inline void swap(pair<T1, T2>& a, pair<T1, T2>& b)
  {
    a.swap(b);
  }

  ///  Two pairs of the same type are equal iff their members are equal.
  template <typename T1, typename T2>
  inline bool operator ==(const pair<T1, T2>& a, const pair<T1, T2>& b)
  {
#include "private/diagnostic_float_equal_push.h"
    return (a.first == b.first) && !(a.second < b.second) && !(a.second > b.second);
#include "private/diagnostic_pop.h"
  }

  /// Uses @c operator== to find the result.
  template <typename T1, typename T2>
  inline bool operator !=(const pair<T1, T2>& a, const pair<T1, T2>& b)
  {
    return !(a == b);
  }

  template <typename T1, typename T2>
  inline bool operator <(const pair<T1, T2>& a, const pair<T1, T2>& b)
  {
    return (a.first < b.first) ||
      (!(b.first < a.first) && (a.second < b.second));
  }

  /// Uses @c operator< to find the result.
  template <typename T1, typename T2>
  inline bool operator >(const pair<T1, T2>& a, const pair<T1, T2>& b)
  {
    return (b < a);
  }

  /// Uses @c operator< to find the result.
  template <typename T1, typename T2>
  inline bool operator <=(const pair<T1, T2>& a, const pair<T1, T2>& b)
  {
    return !(b < a);
  }

  /// Uses @c operator< to find the result.
  template <typename T1, typename T2>
  inline bool operator >=(const pair<T1, T2>& a, const pair<T1, T2>& b)
  {
    return !(a < b);
  }

  //***************************************************************************
  ///\brief Functor to select @ref pair::first
  ///
  ///\ref select1st is a functor object that takes a single argument, a @ref pair, and returns the @ref pair::first element.
  ///
  ///\b Example
  ///\snippet test_utility.cpp test_select1st_example
  ///
  ///\tparam TPair The function object's argument type.
  ///
  ///\see select2nd
  //***************************************************************************
  template <typename TPair>
  struct select1st
  {
    typedef typename TPair::first_type type;  ///< type of member @ref pair::first.

    //***************************************************************************
    ///\brief Function call that return @c p.first.
    ///\return a reference to member @ref pair::first of the @c pair `p`
    //***************************************************************************
    type& operator()(TPair& p) const
    {
      return p.first;
    }

    //***************************************************************************
    ///\copydoc operator()(TPair&)const
    //
    const type& operator()(const TPair& p) const
    {
      return p.first;
    }
  };

  //***************************************************************************
  ///\brief Functor to select @ref pair::second
  ///
  ///\ref select2nd is a functor object that takes a single argument, a @ref pair, and returns the @ref pair::second element.
  ///
  ///\b Example
  ///\snippet test_utility.cpp test_select2nd_example
  ///
  ///\tparam TPair The function object's argument type.
  ///
  ///\see select1st
  //***************************************************************************
  template <typename TPair>
  struct select2nd
  {
    typedef typename TPair::second_type type;  ///< type of member @ref pair::second.

    //***************************************************************************
    ///\brief Function call. The return value is `p.second`.
    ///\return a reference to member `second` of the pair `p`.
    //***************************************************************************
    type& operator()(TPair& p) const
    {
      return p.second;
    }

    //***************************************************************************
    ///\copydoc operator()(TPair&)const
    //***************************************************************************
    const type& operator()(const TPair& p) const
    {
      return p.second;
    }
  };

#if ETL_NOT_USING_STL || ETL_CPP14_NOT_SUPPORTED
  //***************************************************************************
  /// exchange (const)
  //***************************************************************************
  template <typename T>
  T exchange(T& object, const T& new_value)
  {
    T old_value = object;
    object = new_value;
    return old_value;
  }

  template <typename T, typename U>
  T exchange(T& object, const U& new_value)
  {
    T old_value = object;
    object = new_value;
    return old_value;
  }
#else
  //***************************************************************************
  /// exchange (const)
  //***************************************************************************
  template <typename T, typename U = T>
  T exchange(T& object, const U& new_value)
  {
    return std::exchange(object, new_value);
  }
#endif

  //***************************************************************************
  /// as_const
  //***************************************************************************
  template <typename T>
  typename etl::add_const<T>::type& as_const(T& t)
  {
    return t;
  }

  //***************************************************************************
  /// integer_sequence
  //***************************************************************************
#if ETL_USING_CPP11
  template <typename T, T... Integers>
  class integer_sequence
  {
  public:
  
    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Integral types only");

    using value_type = T;
  
    static ETL_CONSTEXPR size_t size() ETL_NOEXCEPT 
    { 
      return sizeof...(Integers);
    }
  };

  namespace private_index_sequence
  {
    template <size_t N, typename IndexSeq>
    struct make_index_sequence;

    template <size_t N, size_t... Indices>
    struct make_index_sequence<N, etl::integer_sequence<size_t, Indices...>>
    {
      using type = typename make_index_sequence<N - 1, etl::integer_sequence<size_t, N - 1, Indices...>>::type;
    };

    template <size_t... Indices>
    struct make_index_sequence<0, etl::integer_sequence<size_t, Indices...>>
    {
      using type = etl::integer_sequence<size_t, Indices...>;
    };
  }

  //***********************************
  template <size_t N>
  using make_index_sequence = typename private_index_sequence::make_index_sequence<N, etl::integer_sequence<size_t>>::type;

  //***********************************
  template <size_t... Indices>
  using index_sequence = etl::integer_sequence<size_t, Indices...>;
#endif

  //***************************************************************************
  /// 2D coordinate type.
  //***************************************************************************
  template <typename T>
  struct coordinate_2d
  {
    coordinate_2d()
      : x(T(0))
      , y(T(0))
    {
    }

    coordinate_2d(T x_, T y_)
      : x(x_)
      , y(y_)
    {
    }

    friend bool operator ==(const coordinate_2d& lhs, const coordinate_2d& rhs)
    {
      return (lhs.x == rhs.x) && (lhs.y == rhs.y);
    }

    friend bool operator !=(const coordinate_2d& lhs, const coordinate_2d& rhs)
    {
      return !(lhs == rhs);
    }

    T x;
    T y;
  };

  //***************************************************************************
  /// in_place disambiguation tags.
  //***************************************************************************
  
  //*************************
  struct in_place_t 
  {
    explicit ETL_CONSTEXPR in_place_t() {}
  };

#if ETL_USING_CPP17
  inline constexpr in_place_t in_place{};
#endif
  
  //*************************
  template <typename T> struct in_place_type_t 
  {
    explicit ETL_CONSTEXPR in_place_type_t() {}
  };

#if ETL_USING_CPP17
  template <typename T>
  inline constexpr in_place_type_t<T> in_place_type{};
#endif

  //*************************
  template <size_t I> struct in_place_index_t 
  {
    explicit ETL_CONSTEXPR in_place_index_t() {}
  };

#if ETL_USING_CPP17
  template <size_t I>
  inline constexpr in_place_index_t<I> in_place_index{};
#endif

#if ETL_USING_CPP11
  //*************************************************************************
  // A function wrapper for free/global functions.
  // Deprecated.
  // See etl::function_ptr_as_functor for a runtime time wrapper option.
  // See etl::function_as_functor for a compile time wrapper option.
  //*************************************************************************
  template <typename TReturn, typename... TParams>
  class ETL_DEPRECATED functor
  {
  public:

    //*********************************
    /// Constructor.
    //*********************************
    constexpr functor(TReturn(*ptr_)(TParams...))
      : ptr(ptr_)
    {
    }

    //*********************************
    /// Const function operator.
    //********************************* 
    constexpr TReturn operator()(TParams... args) const
    {
      return ptr(etl::forward<TParams>(args)...);
    }

  private:

    /// The pointer to the function.
    TReturn(*ptr)(TParams...);
  };

  //*****************************************************************************
  // Wrap a member function with a static free function.
  // Creates a static member function that calls the specified member function.
  // Deprecated
  // See etl::member_function_as_static
  //*****************************************************************************
  template <typename T>
  class member_function_wrapper;

  template <typename TReturn, typename... TParams>
  class ETL_DEPRECATED member_function_wrapper<TReturn(TParams...)>
  {
  public:

    template <typename T, T& Instance, TReturn(T::* Method)(TParams...)>
    static constexpr TReturn function(TParams... params)
    {
      return (Instance.*Method)(etl::forward<TParams>(params)...);
    }
  };

  //*****************************************************************************
  // Wrap a functor with a static free function.
  // Creates a static member function that calls the specified functor.
  // Deprecated
  // See etl::functor_as_static
  //*****************************************************************************
  template <typename T>
  class functor_wrapper;

  template <typename TReturn, typename... TParams>
  class functor_wrapper<TReturn(TParams...)>
  {
  public:

    template <typename TFunctor, TFunctor& Instance>
    static constexpr TReturn function(TParams... params)
    {
      return Instance(etl::forward<TParams>(params)...);
    }
  };
#endif

#if ETL_USING_CPP17
  //*****************************************************************************
  // Wraps a functor with a static free function at compile time.
  // Creates a static member 'call' that calls the specified functor.
  //*****************************************************************************
  template <auto& Instance>
  struct functor_as_static 
  {
    template <typename... TArgs>
    static constexpr auto call(TArgs&&... args)
    {
      return (Instance.operator())(etl::forward<TArgs>(args)...);
    }
  };

  //*****************************************************************************
  // Wraps a member function with a static free function at compile time.
  // Creates a static member 'call' that calls the specified member function.
  //*****************************************************************************
  template <auto Method, auto& Instance>
  struct member_function_as_static 
  {
    template <typename... TArgs>
    static constexpr auto call(TArgs&&... args)
    {
      return (Instance.*Method)(etl::forward<TArgs>(args)...);
    }
  };

  //*****************************************************************************
  // Wraps a member function with a functor at compile time.
  // Creates a functor that calls the specified member function.
  //*****************************************************************************
  template <auto Method, auto& Instance>
  class member_function_as_functor
  {
  public:

    template <typename... TArgs>
    constexpr auto operator()(TArgs&&... args) const -> decltype((Instance.*Method)(etl::forward<TArgs>(args)...))
    {
      return (Instance.*Method)(etl::forward<TArgs>(args)...);
    }
  };

  //*****************************************************************************
  // Wraps a function with a functor at compile time.
  // Creates a functor that calls the specified free function.
  //*****************************************************************************
  template <auto Function>
  class function_as_functor
  {
  public:

    template<typename... TArgs>
    constexpr auto operator()(TArgs&&... args) const -> decltype(Function(etl::forward<TArgs>(args)...))
    {
      return Function(etl::forward<TArgs>(args)...);
    }
  };
#endif

#if ETL_USING_CPP11
  //*****************************************************************************
  // Wraps a function pointer with a functor at run time.
  // Creates a functor that calls the specified free function.
  //*****************************************************************************
  template <typename T>
  class function_ptr_as_functor;

  template <typename TReturn, typename... TArgs>
  class function_ptr_as_functor<TReturn(TArgs...)>
  {
  public:

    //*********************************
    /// Constructor.
    //*********************************
    constexpr function_ptr_as_functor(TReturn(*ptr_)(TArgs...))
      : ptr(ptr_)
    {
    }

    //*********************************
    /// Const function operator.
    //*********************************
    constexpr TReturn operator()(TArgs... args) const
    {
      return ptr(etl::forward<TArgs>(args)...);
    }

  private:

    /// The pointer to the function.
    TReturn(*ptr)(TArgs...);
  };
#endif
}

#endif
