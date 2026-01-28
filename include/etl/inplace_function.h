///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2025 John Wellbelove

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

#ifndef ETL_INPLACE_FUNCTION_INCLUDED
#define ETL_INPLACE_FUNCTION_INCLUDED

#include "platform.h"

#if ETL_USING_CPP11

#include "utility.h"
#include "optional.h"
#include "placement_new.h"
#include "type_traits.h"
#include "function_traits.h"
#include "error_handler.h"
#include "exception.h"
#include "invoke.h"
#include "file_error_numbers.h"

#if !defined(ETL_DEFAULT_INPLACE_FUNCTION_SIZE)
  #define ETL_DEFAULT_INPLACE_FUNCTION_SIZE 32
#endif

#if !defined(ETL_DEFAULT_INPLACE_FUNCTION_ALIGNMENT)
  #define ETL_DEFAULT_INPLACE_FUNCTION_ALIGNMENT alignof(void*)
#endif

namespace etl
{
  //*************************************************************************
  /// Exception base for inplace_function
  //*************************************************************************
  class inplace_function_exception : public etl::exception
  {
  public:

    inplace_function_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //*************************************************************************
  /// Inplace function uninitialized exception.
  //*************************************************************************
  class inplace_function_uninitialized : public inplace_function_exception
  {
  public:

    inplace_function_uninitialized(string_type file_name_, numeric_type line_number_)
      : inplace_function_exception(ETL_ERROR_TEXT("inplace_function:uninitialized", ETL_INPLACE_FUNCTION_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  namespace private_inplace_function
  {
    //*************************************************************************
    /// The vtable for inplace_function
    //*************************************************************************
    template <typename TReturn, typename... TArgs>
    struct inplace_function_vtable
    {
      using invoke_type  = TReturn(*)(void*, TArgs...);
      using destroy_type = void(*)(void*);
      using move_type    = void(*)(void*, void*);
      using copy_type    = void(*)(const void*, void*);

      invoke_type  invoke  = nullptr;
      destroy_type destroy = nullptr;
      move_type    move    = nullptr;
      copy_type    copy    = nullptr;

      //*****************************************
      // Constructor
      //*****************************************
      ETL_CONSTEXPR inplace_function_vtable(invoke_type  i,
                                            destroy_type d,
                                            move_type    m,
                                            copy_type    c)
        : invoke(i)
        , destroy(d)
        , move(m)
        , copy(c) {}

      //*****************************************
      // Payload records for member bindings
      //*****************************************
      // Non-const member function
      //*****************************************
      template <typename TObject>
      struct member_target
      {
        TReturn (TObject::* member)(TArgs...);
        TObject obj;
      };

      //*****************************************
      // Const member function
      //*****************************************
      template <typename TObject>
      struct const_member_target
      {
        TReturn(TObject::* member)(TArgs...) const;
        TObject obj;
      };

      //*****************************************
      // Storage helpers
      //*****************************************
      // Copy construct
      //*****************************************
      template <typename T>
      static void copy_construct(const void* src, void* dst)
      {
        ::new (dst) T(*static_cast<const T*>(src));
      }

      //*****************************************
      // Move construct
      //*****************************************
      template <typename T, bool DestroySrc>
      static void move_construct(void* src, void* dst)
      {
        ::new (dst) T(etl::move(*static_cast<T*>(src)));

        if (DestroySrc)
        {
          static_cast<T*>(src)->~T();
        }
      }

      //*****************************************
      // Stub functions
      //*****************************************
      // Destroy function
      //*****************************************
      template <typename T>
      static void destroy_stub(void* p)
      {
        static_cast<T*>(p)->~T();
      }

      //*****************************************
      // Free function - Returning value
      //*****************************************
      template <typename F, typename R = TReturn, etl::enable_if_t<!etl::is_void<R>::value, int> = 0>
      static R stub_function_ptr(void* p, TArgs... a)
      {
        return (*static_cast<F*>(p))(etl::forward<TArgs>(a)...);
      }

      //*****************************************
      // Free function - returning void
      //*****************************************
      template <typename F, typename R = TReturn, etl::enable_if_t<etl::is_void<R>::value, int> = 0>
      static void stub_function_ptr(void* p, TArgs... a)
      {
        (*static_cast<F*>(p))(etl::forward<TArgs>(a)...);
      }

      //*****************************************
      // Member target - Returning value
      //*****************************************
      template <typename Target, typename R = TReturn, etl::enable_if_t<!etl::is_void<R>::value, int> = 0>
      static R stub_member(void* p, TArgs... a)
      {
        auto* s = static_cast<Target*>(p);
        return (s->obj.*(s->member))(etl::forward<TArgs>(a)...);
      }

      //*****************************************
      // Member target - returning void
      //*****************************************
      template <typename Target, typename R = TReturn, etl::enable_if_t<etl::is_void<R>::value, int> = 0>
      static void stub_member(void* p, TArgs... a)
      {
        auto* s = static_cast<Target*>(p);
        (s->obj.*(s->member))(etl::forward<TArgs>(a)...);
      }

      //*****************************************
      // Functor / lambda stored in payload - returning value
      //*****************************************
      template <typename T, typename R = TReturn, etl::enable_if_t<!etl::is_void<R>::value, int> = 0>
      static R stub_functor(void* p, TArgs... a)
      {
        return (*static_cast<T*>(p))(etl::forward<TArgs>(a)...);
      }

      //*****************************************
      // Functor / lambda stored in payload - returning void
      //*****************************************
      template <typename T, typename R = TReturn, etl::enable_if_t<etl::is_void<R>::value, int> = 0>
      static void stub_functor(void* p, TArgs... a)
      {
        (*static_cast<T*>(p))(etl::forward<TArgs>(a)...);
      }

      //*****************************************
      // Const functor / lambda stored in payload - returning value
      //*****************************************
      template <typename T, typename R = TReturn, etl::enable_if_t<!etl::is_void<R>::value, int> = 0>
      static R stub_const_functor(void* p, TArgs... a)
      {
        return (*static_cast<const T*>(p))(etl::forward<TArgs>(a)...);
      }

      //*****************************************
      // Const functor / lambda stored in payload - returning void
      //*****************************************
      template <typename T, typename R = TReturn, etl::enable_if_t<etl::is_void<R>::value, int> = 0>
      static void stub_const_functor(void* p, TArgs... a)
      {
        (*static_cast<const T*>(p))(etl::forward<TArgs>(a)...);
      }

      //*****************************************
      // Compile-time bound free function - returning value
      //*****************************************
      template <TReturn(*Function)(TArgs...), typename R = TReturn, etl::enable_if_t<!etl::is_void<R>::value, int> = 0>
      static R stub_ct_function(void*, TArgs... a)
      {
        return Function(etl::forward<TArgs>(a)...);
      }

      //*****************************************
      // Compile-time bound free function - returning void
      //*****************************************
      template <TReturn(*Function)(TArgs...), typename R = TReturn, etl::enable_if_t<etl::is_void<R>::value, int> = 0>
      static void stub_ct_function(void*, TArgs... a)
      {
        Function(etl::forward<TArgs>(a)...);
      }

      //*****************************************
      // Compile-time bound member + object - returning value
      //*****************************************
      template <typename TObject, TReturn(TObject::* Method)(TArgs...), TObject* Object, typename R = TReturn, etl::enable_if_t<!etl::is_void<R>::value, int> = 0>
      static R stub_ct_member(void*, TArgs... a)
      {
        return (Object->*Method)(etl::forward<TArgs>(a)...);
      }

      //*****************************************
      // Compile-time bound member + object - returning void
      //*****************************************
      template <typename TObject, TReturn(TObject::* Method)(TArgs...), TObject* Object, typename R = TReturn, etl::enable_if_t<etl::is_void<R>::value, int> = 0>
      static void stub_ct_member(void*, TArgs... a)
      {
        (Object->*Method)(etl::forward<TArgs>(a)...);
      }

      //*****************************************
      // Compile-time bound const member + object - returning value
      //*****************************************
      template <typename TObject, TReturn(TObject::* Method)(TArgs...) const, const TObject* Object, typename R = TReturn, etl::enable_if_t<!etl::is_void<R>::value, int> = 0>
      static R stub_ct_const_member(void*, TArgs... a)
      {
        return (Object->*Method)(etl::forward<TArgs>(a)...);
      }

      //*****************************************
      // Compile-time bound const member + object - returning void
      //*****************************************
      template <typename TObject, TReturn(TObject::* Method)(TArgs...) const, const TObject* Object, typename R = TReturn, etl::enable_if_t<etl::is_void<R>::value, int> = 0>
      static void stub_ct_const_member(void*, TArgs... a)
      {
        (Object->*Method)(etl::forward<TArgs>(a)...);
      }

      //*****************************************
      // Compile-time bound operator() + object - returning value
      //*****************************************
      template <typename TObject, TObject* Object, typename R = TReturn, etl::enable_if_t<!etl::is_void<R>::value, int> = 0>
      static R stub_ct_operator(void*, TArgs... a)
      {
        return (*Object)(etl::forward<TArgs>(a)...);
      }

      //*****************************************
      // Compile-time bound operator() + object - returning void
      //*****************************************
      template <typename TObject, TObject* Object, typename R = TReturn, etl::enable_if_t<etl::is_void<R>::value, int> = 0>
      static void stub_ct_operator(void*, TArgs... a)
      {
        (*Object)(etl::forward<TArgs>(a)...);
      }

      //*****************************************
      // vtable factories
      //*****************************************
      // Free function pointer
      //*****************************************
      static const inplace_function_vtable* for_function_ptr()
      {
        using function_type = TReturn(*)(TArgs...);
        static const inplace_function_vtable vtable(&inplace_function_vtable::template stub_function_ptr<function_type>,
                                                    nullptr,
                                                    &inplace_function_vtable::template move_construct<function_type, false>,
                                                    &inplace_function_vtable::template copy_construct<function_type>);
        return &vtable;
      }

      //*****************************************
      // Member function pointer
      //*****************************************
      template <typename TObject>
      static const inplace_function_vtable* for_member()
      {
        using target_t = member_target<TObject>;
        constexpr bool destroy_src_on_move = !etl::is_trivially_destructible<TObject>::value;

        constexpr destroy_type destroy_ptr = etl::is_trivially_destructible<TObject>::value ? nullptr
                                                                                            : &inplace_function_vtable::template destroy_stub<target_t>;

        static const inplace_function_vtable vtable(&inplace_function_vtable::template stub_member<target_t>,
                                                    destroy_ptr,
                                                    &inplace_function_vtable::template move_construct<target_t, destroy_src_on_move>,
                                                    &inplace_function_vtable::template copy_construct<target_t>);
        return &vtable;
      }

      //*****************************************
      // Const member function pointer
      //*****************************************
      template <typename TObject>
      static const inplace_function_vtable* for_const_member()
      {
        using target_t = const_member_target<TObject>;
        constexpr bool destroy_src_on_move = !etl::is_trivially_destructible<TObject>::value;

        constexpr destroy_type destroy_ptr = etl::is_trivially_destructible<TObject>::value ? nullptr
                                                                                            : &inplace_function_vtable::template destroy_stub<target_t>;

        static const inplace_function_vtable vtable(&inplace_function_vtable::template stub_member<target_t>,
                                                    destroy_ptr,
                                                    &inplace_function_vtable::template move_construct<target_t, destroy_src_on_move>,
                                                    &inplace_function_vtable::template copy_construct<target_t>);
        return &vtable;
      }

      //*****************************************
      // Functor / lambda
      //*****************************************
      template <typename TObject>
      static const inplace_function_vtable* for_functor()
      {
        constexpr bool destroy_src_on_move = !etl::is_trivially_destructible<TObject>::value;

        constexpr destroy_type destroy_ptr = etl::is_trivially_destructible<TObject>::value ? nullptr
                                                                                            : &inplace_function_vtable::template destroy_stub<TObject>;
        
        static const inplace_function_vtable vtable(&inplace_function_vtable::template stub_functor<TObject>,
                                                    destroy_ptr,
                                                    &inplace_function_vtable::template move_construct<TObject, destroy_src_on_move>,
                                                    &inplace_function_vtable::template copy_construct<TObject>);
        return &vtable;
      }

      //*****************************************
      // Const functor / lambda
      //*****************************************
      template <typename TObject>
      static const inplace_function_vtable* for_const_functor()
      {
        constexpr bool destroy_src_on_move = !etl::is_trivially_destructible<TObject>::value;
        
        constexpr destroy_type destroy_ptr = etl::is_trivially_destructible<TObject>::value ? nullptr
                                                                                            : &inplace_function_vtable::template destroy_stub<TObject>;

        static const inplace_function_vtable vtable(&inplace_function_vtable::template stub_const_functor<TObject>,
                                                    destroy_ptr,
                                                    &inplace_function_vtable::template move_construct<TObject, destroy_src_on_move>,
                                                    &inplace_function_vtable::template copy_construct<TObject>);
        return &vtable;
      }

      //*****************************************
      // Compile-time bound free function
      //*****************************************
      template <TReturn(*Function)(TArgs...)>
      static const inplace_function_vtable* for_compile_time_function()
      {
        static const inplace_function_vtable vtable(&inplace_function_vtable::template stub_ct_function<Function>,
                                                    nullptr, 
                                                    nullptr, 
                                                    nullptr);
        return &vtable;
      }

      //*****************************************
      // Compile-time bound member function + object
      //*****************************************
      template <typename TObject, TReturn(TObject::* Method)(TArgs...), TObject* Object>
      static const inplace_function_vtable* for_compile_time_member()
      {
        static const inplace_function_vtable vtable(&inplace_function_vtable::template stub_ct_member<TObject, Method, Object>,
                                                    nullptr, 
                                                    nullptr, 
                                                    nullptr);
        return &vtable;
      }

      //*****************************************
      // Compile-time bound const member function + object
      //*****************************************
      template <typename TObject, TReturn(TObject::* Method)(TArgs...) const, const TObject* Object>
      static const inplace_function_vtable* for_compile_time_const_member()
      {
        static const inplace_function_vtable vtable(&inplace_function_vtable::template stub_ct_const_member<TObject, Method, Object>,
                                                    nullptr, 
                                                    nullptr, 
                                                    nullptr);
        return &vtable;
      }

      //*****************************************
      // Compile-time bound operator() + object
      //*****************************************
      template <typename TObject, TObject* Object>
      static const inplace_function_vtable* for_compile_time_operator()
      {
        static const inplace_function_vtable vtable(&inplace_function_vtable::template stub_ct_operator<TObject, Object>,
                                                    nullptr, 
                                                    nullptr, 
                                                    nullptr);
        return &vtable;
      }
    };
  }

  //*************************************************************************
  /// inplace_function
  //*************************************************************************
  template <typename TSignature, 
            size_t Object_Size      = ETL_DEFAULT_INPLACE_FUNCTION_SIZE, 
            size_t Object_Alignment = ETL_DEFAULT_INPLACE_FUNCTION_ALIGNMENT>
  class inplace_function;

  //*************************************************************************
  // Is not an etl::inplace_function
  //*************************************************************************
  template <typename T>
  struct is_inplace_function : etl::false_type {};

  //*************************************************************************
  // Is an etl::inplace_function
  //*************************************************************************
  template <typename TReturn, typename... TArgs, size_t Object_Size, size_t Object_Alignment>
  struct is_inplace_function<etl::inplace_function<TReturn(TArgs...), Object_Size, Object_Alignment>> : etl::true_type {};

  //*************************************************************************
  /// inplace_function
  /// A function wrapper with in-place storage for the callable object.
  /// \tparam TReturn The return type of the function signature.
  /// \tparam TArgs   The argument types of the function signature.
  /// \tparam Object_Size The size of the in-place storage for the callable object.
  /// \tparam Object_Alignment The alignment of the in-place storage for the callable object.
  //*************************************************************************
  template <typename TReturn, typename... TArgs, size_t Object_Size, size_t Object_Alignment>
  class inplace_function<TReturn(TArgs...), Object_Size, Object_Alignment>
  {
  private:

    using this_type      = inplace_function<TReturn(TArgs...), Object_Size, Object_Alignment>;
    using storage_type   = etl::uninitialized_buffer<Object_Size, 1, Object_Alignment>;
    using vtable_type    = private_inplace_function::inplace_function_vtable<TReturn, TArgs...>;
    using function_ptr   = TReturn(*)(TArgs...);

  public:

    using function_type  = TReturn(TArgs...);
    using return_type    = TReturn;
    using argument_types = etl::type_list<TArgs...>;

    //*************************************************************************
    /// Default constructor
    //*************************************************************************
    inplace_function() noexcept = default;

    //*************************************************************************
    /// Copy constructor
    ///\param other The other inplace_function to copy from.
    //*************************************************************************
    inplace_function(const inplace_function& other)
    {
      clone_from(other);
    }

    //*************************************************************************
    /// Copy constructor for different object buffer sizes
    /// Static asserts if the destination buffer is too small.
    /// Static assert if the destination alignment is too small.
    /// \param other The other inplace_function to copy from.
    ///*************************************************************************
    template <size_t Other_Object_Size, size_t Other_Object_Alignment>
    inplace_function(const etl::inplace_function<TReturn(TArgs...), Other_Object_Size, Other_Object_Alignment>& other)
    {
      static_assert(Object_Size      >= Other_Object_Size,      "etl::inplace_function: Destination object size too small");
      static_assert(Object_Alignment >= Other_Object_Alignment, "etl::inplace_function: Destination object alignment too small");

      clone_from(other);
    }

    //*************************************************************************
    /// Move constructor
    /// \param other The other inplace_function to move from.
    //*************************************************************************
    inplace_function(inplace_function&& other) noexcept
    {
      move_from(other);
    }

    //*************************************************************************
    /// Move constructor for different object buffer 
    /// Static asserts if the destination buffer is too small.
    /// Static assert if the destination alignment is too small.
    /// \param other The other inplace_function to move from.
    //*************************************************************************
    template <size_t Other_Object_Size, size_t Other_Object_Alignment>
    inplace_function(etl::inplace_function<TReturn(TArgs...), Other_Object_Size, Other_Object_Alignment>&& other) noexcept
    {
      static_assert(Object_Size      >= Other_Object_Size,      "etl::inplace_function: Destination object size too small");
      static_assert(Object_Alignment >= Other_Object_Alignment, "etl::inplace_function: Destination object alignment too small");

      move_from(other);
    }

    //*************************************************************************
    /// Destructor
    //*************************************************************************
    ~inplace_function() noexcept
    {
      clear();
    }

    //*************************************************************************
    /// Construct from function pointer.
    /// \param f The function pointer.
    //*************************************************************************
    inplace_function(function_ptr f)
    {
      set(f);
    }

    //*************************************************************************
    /// Construct from object + non-const member function (runtime).
    /// \tparam TObject The object type.
    /// \param method The member function pointer.
    /// \param obj    The object.
    //*************************************************************************
    template <typename TObject, typename TObjectArg>
    inplace_function(TReturn (TObject::*method)(TArgs...), TObjectArg&& obj)
    {
      set(method, etl::forward<TObjectArg>(obj));
    }

    //*************************************************************************
    /// Construct from object + const member function (runtime).
    /// \tparam TObject The object type.
    /// \param method The member function pointer.
    /// \param obj    The object.
    //*************************************************************************
    template <typename TObject, typename TObjectArg>
    inplace_function(TReturn (TObject::*method)(TArgs...) const, TObjectArg&& obj)
    {
      set(method, etl::forward<TObjectArg>(obj));
    }

    //*************************************************************************
    /// Construct from lambda/functor (runtime).
    /// \tparam TLambda The lambda/functor type.
    /// \param lambda  The lambda/functor.
    //*************************************************************************
    template <typename TLambda,
              typename T = typename etl::decay<TLambda>::type,
              typename = etl::enable_if_t<etl::is_class<T>::value && !is_inplace_function<T>::value, void>>
    inplace_function(TLambda& lambda)
    {
      set(lambda);
    }

    //*************************************************************************
    /// Construct from const lambda/functor (runtime).
    /// \tparam TLambda The lambda/functor type.
    /// \param lambda  The lambda/functor.
    //*************************************************************************
    template <typename TLambda,
              typename T = typename etl::decay<TLambda>::type,
              typename = etl::enable_if_t<etl::is_class<T>::value && !is_inplace_function<T>::value, void>>
    inplace_function(const TLambda& lambda)
    {
      set(lambda);
    }

    //*************************************************************************
    /// Create from function pointer (runtime).
    /// \param f The function pointer.
    //*************************************************************************
    void set(function_ptr f)
    {
      // Validate that 'f' is invocable with (TArgs...) and returns TReturn
      static_assert(etl::is_invocable_r<TReturn, function_ptr, TArgs...>::value,
                    "etl::inplace_function: function pointer is not compatible with the inplace_function signature");

      static_assert(Object_Size      >= sizeof(function_ptr),  "etl::inplace_function: storage size too small");
      static_assert(Object_Alignment >= alignof(function_ptr), "etl::inplace_function: storage alignment too small");

      clear();

      // Construct the object in the storage.
      ::new (storage_ptr()) function_ptr(f);

      vtable = vtable_type::for_function_ptr();
    }

    //*************************************************************************
    /// Create from object + member function (runtime).
    /// \tparam TObject The object type.
    /// \param method The member function pointer.
    /// \param obj    The object.
    //*************************************************************************
    template <typename TObject, typename TObjectArg>
    void set(TReturn(TObject::* method)(TArgs...), TObjectArg&& obj)
    {
      using D = etl::decay_t<TObjectArg>;
      static_assert(etl::is_invocable_r<TReturn, decltype(method), D&, TArgs...>::value,
                    "etl::inplace_function: bound member function is not compatible with the inplace_function signature");

      using target_t = typename vtable_type::template member_target<D>;

      static_assert(Object_Size      >= sizeof(target_t),  "etl::inplace_function: storage size too small");
      static_assert(Object_Alignment >= alignof(target_t), "etl::inplace_function: storage alignment too small");

      clear();
      ::new (storage_ptr()) target_t{ method, etl::forward<TObjectArg>(obj) };
      vtable = vtable_type::template for_member<D>();
    }

    //*************************************************************************
    /// Create from object + const member function (runtime).
    /// \tparam TObject The object type.
    /// \param method The member function pointer.
    /// \param obj    The object.
    //*************************************************************************
    template <typename TObject, typename TObjectArg>
    void set(TReturn(TObject::* method)(TArgs...) const, TObjectArg&& obj)
    {
      using D = etl::decay_t<TObjectArg>;
      static_assert(etl::is_invocable_r<TReturn, decltype(method), D&, TArgs...>::value,
                    "etl::inplace_function: bound member function is not compatible with the inplace_function signature");

      using target_t = typename vtable_type::template const_member_target<D>;

      static_assert(Object_Size      >= sizeof(target_t),  "etl::inplace_function: storage size too small");
      static_assert(Object_Alignment >= alignof(target_t), "etl::inplace_function: storage alignment too small");

      clear();
      ::new (storage_ptr()) target_t{ method, etl::forward<TObjectArg>(obj) };
      vtable = vtable_type::template for_const_member<D>();
    }

    //*************************************************************************
    /// Create from lambda/functor (runtime).
    /// \tparam TLambda The lambda/functor type.
    /// \param lambda  The lambda/functor.
    //*************************************************************************
    template <typename TLambda,
              typename T = typename etl::decay<TLambda>::type,
              typename = etl::enable_if_t<etl::is_class<T>::value && !is_inplace_function<T>::value, void>>
    void set(TLambda& lambda)
    {
      // Validate that 'method' is invocable with (TObject&, TArgs...) and returns TReturn
      static_assert(etl::is_invocable_r<TReturn, T, TArgs...>::value,
                    "etl::inplace_function: bound lambda/functor is not compatible with the inplace_function signature");

      static_assert(Object_Size      >= sizeof(T),  "etl::inplace_function: Object size too small");
      static_assert(Object_Alignment >= alignof(T), "etl::inplace_function: Object alignment too small");

      clear();

      // Construct the object in the storage.
      ::new (storage_ptr()) T(lambda);

      
      vtable = vtable_type::template for_functor<T>();
    }

    //*************************************************************************
    /// Create from const lambda/functor (runtime).
    /// \tparam TLambda The lambda/functor type.
    /// \param lambda  The lambda/functor.
    //*************************************************************************
    template <typename TLambda,
              typename T = typename etl::decay<TLambda>::type,
              typename = etl::enable_if_t<etl::is_class<T>::value && !is_inplace_function<T>::value, void>>
    void set(const TLambda& lambda)
    {
      static_assert(etl::is_invocable_r<TReturn, const T, TArgs...>::value,
                    "etl::inplace_function: bound lambda/functor is not compatible with the inplace_function signature");

      static_assert(Object_Size      >= sizeof(T),  "etl::inplace_function: Object size too small");
      static_assert(Object_Alignment >= alignof(T), "etl::inplace_function: Object alignment too small");

      clear();

      // Construct the object in the storage.
      ::new (storage_ptr()) T(lambda);
      
      vtable = vtable_type::template for_const_functor<T>();
    }

    //*************************************************************************
    /// Set from a compile-time bound free function.
    /// Function must have external linkage.
    /// Usage: ipf.template set<&FreeFn>();
    /// \tparam Function The function pointer.
    //*************************************************************************
    template <TReturn(*Function)(TArgs...)>
    void set()
    {
      static_assert(etl::is_invocable_r<TReturn, decltype(Function), TArgs...>::value,
                    "etl::inplace_function: function pointer is not compatible with the inplace_function signature");

      clear();
      vtable = vtable_type::template for_compile_time_function<Function>();
    }

    //*************************************************************************
    /// Set from a compile-time bound non-const member + instance reference.
    /// Instance must have external linkage when used as an NTTP.
    /// \tparam TObject The object type.
    /// \tparam Method  The member function pointer.
    /// \tparam Instance The instance reference.
    //*************************************************************************
    template <typename TObject, TReturn(TObject::*Method)(TArgs...), TObject& Instance>
    void set()
    {
      static_assert(etl::is_invocable_r<TReturn, decltype(Method), TObject&, TArgs...>::value,
                    "etl::inplace_function: bound member function is not compatible with the inplace_function signature");

      clear();
      vtable = vtable_type::template for_compile_time_member<TObject, Method, &Instance>();
    }

    //*************************************************************************
    /// Set from a compile-time bound const member + instance reference.
    /// Instance must have external linkage when used as an NTTP.
    /// \tparam TObject  The object type.
    /// \tparam Method   The member function pointer.
    /// \tparam Instance The instance reference.
    //*************************************************************************
    template <typename TObject, TReturn(TObject::*Method)(TArgs...) const, const TObject& Instance>
    void set()
    {
      static_assert(etl::is_invocable_r<TReturn, decltype(Method), const TObject&, TArgs...>::value,
                    "etl::inplace_function: bound member function is not compatible with the inplace_function signature");

      clear();
      vtable = vtable_type::template for_compile_time_const_member<TObject, Method, &Instance>();
    }

    //*************************************************************************
    /// operator() + instance reference (compile-time, non-const)
    /// \tparam TObject  The object type.
    /// \tparam Instance The instance reference.
    //*************************************************************************
    template <typename TObject,
              TObject& Instance,
              typename T = etl::decay_t<TObject>,
              typename = etl::enable_if_t<etl::is_class<T>::value &&
                                          etl::has_call_operator<T>::value &&
                                          !etl::function_traits<decltype(&T::operator())>::is_const>>
    void set()
    {
      static_assert(etl::is_invocable_r<TReturn, T, TArgs...>::value,
                    "etl::inplace_function: bound lambda/functor is not compatible with the inplace_function signature");

      clear();
      vtable = vtable_type::template for_compile_time_operator<TObject, &Instance>();
    }

    //*************************************************************************
    /// operator() + instance reference (compile-time, const)
    /// \tparam TObject  The object type.
    /// \tparam Instance The instance reference.
    //*************************************************************************
    template <typename TObject,
              const TObject& Instance,
              typename T = etl::decay_t<TObject>,
              typename = etl::enable_if_t<etl::is_class<T>::value &&
                                          etl::has_call_operator<T>::value &&
                                          etl::function_traits<decltype(&T::operator())>::is_const>>
    void set()
    {
      static_assert(etl::is_invocable_r<TReturn, const T, TArgs...>::value,
                    "etl::inplace_function: bound lambda/functor is not compatible with the inplace_function signature");

      clear();
      vtable = vtable_type::template for_compile_time_operator<const TObject, &Instance>();
    }

    //*************************************************************************
    /// Free function (external linkage)
    /// \tparam Function The function pointer.
    /// \return The constructed inplace_function.
    //*************************************************************************
    template <TReturn(*Function)(TArgs...)>
    static this_type create()
    {
      return this_type(vtable_type::template for_compile_time_function<Function>());
    }

    //*************************************************************************
    /// Non-const member + instance reference (compile time)
    /// \tparam TObject  The object type.
    /// \tparam Method   The member function pointer.
    /// \tparam Instance The instance reference.
    /// \return The constructed inplace_function.
    //*************************************************************************
    template <typename TObject, TReturn(TObject::*Method)(TArgs...), TObject& Instance>
    static this_type create()
    {
      return this_type(vtable_type::template for_compile_time_member<TObject, Method, &Instance>());
    }

    //*************************************************************************
    /// Const member + instance reference (compile time)
    /// \tparam TObject  The object type.
    /// \tparam Method   The member function pointer.
    /// \tparam Instance The instance reference.
    /// \return The constructed inplace_function.
    //*************************************************************************
    template <typename TObject, TReturn(TObject::*Method)(TArgs...) const, const TObject& Instance>
    static this_type create()
    {
      return this_type(vtable_type::template for_compile_time_const_member<TObject, Method, &Instance>());
    }

    //*************************************************************************
    /// operator() + instance reference (compile time, non-const)
    /// \tparam TObject  The object type.
    /// \tparam Instance The instance reference.
    /// \return The constructed inplace_function.
    //*************************************************************************
    template <typename TObject, TObject& Instance>
    static this_type create()
    {
      return this_type(vtable_type::template for_compile_time_operator<TObject, &Instance>());
    }

    //*************************************************************************
    /// Assignment operator
    /// \param rhs The other inplace_function to copy from.
    /// \return The current inplace_function.
    //*************************************************************************
    inplace_function& operator =(const inplace_function& rhs)
    {
      if (this != &rhs)
      {
        clear();
        clone_from(rhs);
      }

      return *this;
    }

    //*************************************************************************
    /// Move assignment operator
    /// \param rhs The other inplace_function to move from.
    /// \return The current inplace_function.
    //*************************************************************************
    inplace_function& operator =(inplace_function&& rhs) noexcept
    {
      if (this != &rhs)
      {
        clear();
        move_from(rhs);
      }

      return *this;
    }

    //*************************************************************************
    /// Assignment from nullptr
    /// Clears the inplace_function.
    /// \param nullptr_t Null pointer
    /// \return The current inplace_function.
    //*************************************************************************
    inplace_function& operator =(etl::nullptr_t) noexcept
    {
      clear();

      return *this;
    }

    //*************************************************************************
    /// Assignment from function pointer
    /// \param f The function pointer.
    /// \return The current inplace_function.
    //*************************************************************************
    inplace_function& operator =(function_ptr f)
    {
      set(f);

      return *this;
    }

    //*************************************************************************
    /// Assignment from lambda/functor (runtime).
    /// Enabled only for class types that are not etl::inplace_function.
    /// \tparam TLambda The lambda/functor type.
    /// \param lambda  The lambda/functor.
    /// \return The current inplace_function.
    //*************************************************************************
    template <typename TLambda, 
              typename T = typename etl::decay<TLambda>::type, 
              typename = etl::enable_if_t<etl::is_class<T>::value && !is_inplace_function<T>::value, void>>
    inplace_function& operator =(TLambda&& lambda)
    {
      set(etl::forward<TLambda>(lambda));

      return *this;
    }

    //*************************************************************************
    /// Swap with another inplace_function
    /// \param other The other inplace_function.
    //*************************************************************************
    void swap(inplace_function& other) noexcept
    {
      if (this == &other)
      {
        return;
      }

      do_swap(vtable,       storage_ptr(), 
              other.vtable, other.storage_ptr());
    }

    //*************************************************************************
    /// Checks if the inplace_function is valid (has a target).
    /// \return <b>true</b> if the inplace_function is valid, <b>false</b> if not.
    //*************************************************************************
    ETL_NODISCARD
    bool is_valid() const noexcept
    {
      return (vtable != nullptr);
    }

    //*************************************************************************
    /// Boolean conversion operator.
    /// Checks if the inplace_function is valid (has a target).
    /// \return <b>true</b> if the inplace_function is valid, <b>false</b> if not.
    //*************************************************************************
    ETL_NODISCARD
    explicit operator bool() const noexcept
    {
      return is_valid();
    }

    //*************************************************************************
    /// Execute the inplace_function.
    /// Triggers inplace_function_uninitialized assertion if not valid.
    /// \param args The arguments to pass to the inplace_function.
    /// \return The result of the invocation.
    //*************************************************************************
    TReturn operator()(TArgs... args) const
    {
      ETL_ASSERT(is_valid(), ETL_ERROR(inplace_function_uninitialized));

      return vtable->invoke(storage_ptr(), etl::forward<TArgs>(args)...);
    }

    //*************************************************************************
    /// Execute the inplace_function, if valid.
    /// 'void' return.
    /// \param args The arguments to pass to the inplace_function.
    /// \return <b>true</b> if executed, <b>false</b> if not valid.
    //*************************************************************************
    template <typename TRet = TReturn>
    typename etl::enable_if_t<etl::is_same<TRet, void>::value, bool>
      call_if(TArgs... args) const
    {
      if (is_valid())
      {
        vtable->invoke(storage_ptr(), etl::forward<TArgs>(args)...);
        return true;
      }
      else
      {
        return false;
      }
    }

    //*************************************************************************
    /// Execute the inplace_function if valid.
    /// Non 'void' return.
    /// \param args The arguments to pass to the inplace_function.
    /// \return etl::optional with result if executed, empty etl::optional if not valid.
    //*************************************************************************
    template <typename TRet = TReturn>
    typename etl::enable_if_t<!etl::is_same<TRet, void>::value, etl::optional<TReturn>>
      call_if(TArgs... args) const
    {
      etl::optional<TReturn> result;

      if (is_valid())
      {
        result = vtable->invoke(storage_ptr(), etl::forward<TArgs>(args)...);
      }

      return result;
    }

    //*************************************************************************
    /// Execute the inplace_function if valid or call alternative.
    /// Run time alternative.
    /// \tparam TAlternative The alternative callable.
    /// \param alternative The alternative callable.
    /// \param args The arguments to pass to the inplace_function or alternative.
    /// \return The result of the invocation.
    //*************************************************************************
    template <typename TAlternative>
    TReturn call_or(TAlternative&& alternative, TArgs... args) const
    {
      if (is_valid())
      {
        return vtable->invoke(storage_ptr(), etl::forward<TArgs>(args)...);
      }
      else
      {
        return etl::forward<TAlternative>(alternative)(etl::forward<TArgs>(args)...);
      }
    }

    //*************************************************************************
    /// Execute the inplace_function if valid or call alternative.
    /// Compile time alternative.
    /// \tparam Alternative The alternative callable.
    /// \param args The arguments to pass to the inplace_function or alternative.
    /// \return The result of the invocation.
    //*************************************************************************
    template <TReturn(*Alternative)(TArgs...)>
    TReturn call_or(TArgs... args) const
    {
      if (is_valid())
      {
        return vtable->invoke(storage_ptr(), etl::forward<TArgs>(args)...);
      }
      else
      {
        return (Alternative)(etl::forward<TArgs>(args)...);
      }
    }

    //*************************************************************************
    /// Clears the inplace_function
    /// Destroys any stored callable object.
    //*************************************************************************
    void clear() noexcept 
    {
      if (is_valid())
      {
        if (vtable->destroy)
        {
          vtable->destroy(storage_ptr());
        }

        vtable = nullptr;
      }
    }

    //*************************************************************************
    /// Get the storage size
    /// \return The storage size.
    //*************************************************************************
    ETL_NODISCARD
    static constexpr size_t size() noexcept
    {
      return Object_Size;
    }

    //*************************************************************************
    /// Get the storage alignment
    /// \return The storage alignment.
    //*************************************************************************
    ETL_NODISCARD
    static constexpr size_t alignment() noexcept
    {
      return Object_Alignment;
    }

  private:

    // Allow cross-size access to internals
    template <typename, size_t, size_t>
    friend class inplace_function;

    //*************************************************************************
    // Direct-initialization constructor for CT-bound vtables (no payload).
    //*************************************************************************
    explicit inplace_function(const vtable_type* vt) noexcept
      : vtable(vt)
      , storage()
    {}

    //*************************************************************************
    // clone_from
    //*************************************************************************
    template <size_t Other_Object_Size, size_t Other_Object_Alignment>
    void clone_from(const etl::inplace_function<TReturn(TArgs...), Other_Object_Size, Other_Object_Alignment>& other) 
    {
      vtable = other.vtable;

      if (vtable && vtable->copy) 
      { 
        vtable->copy(&other.storage, &storage);
      }
    }

    //*************************************************************************
    // move_from
    //*************************************************************************
    template <size_t Other_Object_Size, size_t Other_Object_Alignment>
    void move_from(etl::inplace_function<TReturn(TArgs...), Other_Object_Size, Other_Object_Alignment>& other) 
    {
      vtable = other.vtable;

      if (vtable && vtable->move) 
      { 
        vtable->move(&other.storage, &storage); 
      }

      other.vtable = nullptr; 
    }

    //*************************************************************************
    // Internal swap implementation
    //*************************************************************************
      static void do_swap(const vtable_type*& vt_a, void* storage_a,
                          const vtable_type*& vt_b, void* storage_b) noexcept
      {
        const bool a_valid = (vt_a != nullptr);
        const bool b_valid = (vt_b != nullptr);

        if (!a_valid && !b_valid)
        {
          return;
        }

        if (!a_valid)
        {
          // Only 'b' is valid
          if (vt_b->move)
          {
            vt_b->move(storage_b, storage_a);
          }
          else if (vt_b->copy)
          {
            vt_b->copy(storage_b, storage_a);
          }
          vt_a = vt_b;
          vt_b = nullptr;
          return;
        }

        // Only 'a' is valid
        if (!b_valid)
        {
          if (vt_a->move)
          {
            vt_a->move(storage_a, storage_b);
          }
          else if (vt_a->copy)
          {
            vt_a->copy(storage_a, storage_b);
          }
          vt_b = vt_a;
          vt_a = nullptr;
          return;
        }

        // Both valid.
        // If both have no payload (compile-time bound: no move/copy) just swap vtable pointers.
        if (!vt_a->move && !vt_a->copy &&
            !vt_b->move && !vt_b->copy)
        {
          const vtable_type* tmp = vt_a;
          vt_a = vt_b;
          vt_b = tmp;
          return;
        }

        // General case
        alignas(Object_Alignment) unsigned char temp[Object_Size];

        // a -> temp
        if (vt_a->move)
        {
          vt_a->move(storage_a, temp);
        }
        else if (vt_a->copy)
        {
          vt_a->copy(storage_a, temp);
        }

        // b -> a
        if (vt_b->move)
        {
          vt_b->move(storage_b, storage_a);
        }
        else if (vt_b->copy)
        {
          vt_b->copy(storage_b, storage_a);
        }

        // temp -> b
        if (vt_a->move)
        {
          vt_a->move(temp, storage_b);
        }
        else if (vt_a->copy)
        {
          vt_a->copy(temp, storage_b);
        }

        // Swap vtable pointers
        const vtable_type* tmp = vt_a;
        vt_a = vt_b;
        vt_b = tmp;
      }

    //*************************************************************************
    // Get pointer to storage as a void*
    //*************************************************************************
    void* storage_ptr() const noexcept
    {
      return const_cast<void*>(static_cast<const void*>(&storage));
    }

    const vtable_type* vtable = nullptr;
    storage_type       storage;
  };

  //*************************************************************************
  /// Declare an inplace_function from a single type.
  /// \tparam TSignature The function signature.
  /// \tparam TStorage   The storage type.
  //*************************************************************************
  template <typename TSignature, typename TStorage>
  using inplace_function_for = etl::inplace_function<TSignature,
                                                     sizeof(etl::decay_t<TStorage>),
                                                     alignof(etl::decay_t<TStorage>)>;

  //*************************************************************************
  /// Declare an inplace_function from multiple candidates. Picks the largest size/alignment.
  /// \tparam      TSignature The function signature.
  /// \tparam T... The stored candidates types.
  //*************************************************************************
  template <typename TSignature, typename T0, typename... TRest>
  using inplace_function_for_any = etl::inplace_function<TSignature,
                                                         etl::largest<etl::decay_t<T0>, etl::decay_t<TRest>...>::size,
                                                         etl::largest<etl::decay_t<T0>, etl::decay_t<TRest>...>::alignment>;

  //*************************************************************************
  /// Helper to build an inplace_function from a free function pointer.
  /// Usage: auto ipf = etl::make_inplace_function<YourFreeFunction>();
  /// \param function The function pointer.
  /// \return The constructed inplace_function.
  //*************************************************************************
  template <typename TReturn, typename... TArgs>
  ETL_NODISCARD
  etl::inplace_function<TReturn(TArgs...), sizeof(TReturn(*)(TArgs...)), alignof(TReturn(*)(TArgs...))>
    make_inplace_function(TReturn(*function)(TArgs...))
  {
    using function_ptr = TReturn(*)(TArgs...);

    return etl::inplace_function_for<TReturn(TArgs...), function_ptr>(function);
  }

  //*************************************************************************
  /// Helper to build an inplace_function bound to a non-const member function.
  /// Usage: auto ipf = etl::make_inplace_function(obj, &Type::Method);
  /// \param method The member function pointer.
  /// \param obj    The object.
  /// \return The constructed inplace_function.
  //*************************************************************************
  template <typename TObject,
            typename TReturn,
            typename... TArgs,
            typename TTarget = typename etl::private_inplace_function::inplace_function_vtable<TReturn, TArgs...>::template member_target<TObject>>
  ETL_NODISCARD
  etl::inplace_function<TReturn(TArgs...), sizeof(TTarget), alignof(TTarget)>
    make_inplace_function(TReturn (TObject::*method)(TArgs...), TObject& obj)
  {
    return etl::inplace_function_for<TReturn(TArgs...), TTarget>(method, obj);
  }

  //*************************************************************************
  /// Helper to build an inplace_function bound to a const member function.
  /// Usage: auto ipf = etl::make_inplace_function(obj, &Type::Method)  // obj is const
  /// \param method The member function pointer.
  /// \param obj    The object.
  /// \return The constructed inplace_function.
  //*************************************************************************
  template <typename TObject, 
            typename TReturn, 
            typename... TArgs,
            typename TTarget = typename etl::private_inplace_function::inplace_function_vtable<TReturn, TArgs...>::template const_member_target<TObject>>
  ETL_NODISCARD
  etl::inplace_function<TReturn(TArgs...), sizeof(TTarget), alignof(TTarget)>
    make_inplace_function(TReturn (TObject::*method)(TArgs...) const, const TObject& obj)
  {
    return etl::inplace_function_for<TReturn(TArgs...), TTarget>(method, obj);
  }

  //*************************************************************************
  /// Helper to build an inplace_function from a lambda/functor (non-generic, non-overloaded).
  /// Deduces R(Args...) from &T::operator().
  /// Prefer this when operator() is not overloaded/templated.
  /// \param lambda  The lambda/functor.
  /// \return The constructed inplace_function.
  //*************************************************************************
  template <typename TLambda,
            typename T = typename etl::decay<TLambda>::type,
            typename = typename etl::enable_if_t<etl::is_class<T>::value && !is_inplace_function<T>::value, void>,
            typename TSignature = typename etl::function_traits<T>::function_type>
  ETL_NODISCARD
  etl::inplace_function<TSignature, sizeof(T), alignof(T)>
    make_inplace_function(TLambda&& lambda)
  {
    return etl::inplace_function_for<TSignature, T>(etl::forward<TLambda>(lambda));
  }

  //*************************************************************************
  /// Storage-deducing maker (non-class only) to avoid colliding with functor maker
  /// \param function The function-like object.
  /// \return The constructed inplace_function.
  //*************************************************************************
  template <typename TSignature,
            typename TType,
            typename T = typename etl::decay<TType>::type,
            typename = typename etl::enable_if_t<!etl::is_class<T>::value, int>>
  ETL_NODISCARD 
  inplace_function_for<TSignature, T> make_inplace_function(TType&& function) 
  {
    // If T is a function type, use a function pointer for storage sizing.
    using storage_t = typename etl::conditional<etl::is_function<T>::value,
                                                typename etl::add_pointer<T>::type,
                                                T>::type;

    return inplace_function_for<TSignature, storage_t>(etl::forward<TType>(function));
  }

#if ETL_USING_CPP17
  //*************************************************************************
  /// Make an inplace_function from a function at compile time.
  /// Only participates for free function pointers (not member function pointers).
  /// \tparam Function The function pointer.
  /// \return The constructed inplace_function.
  //*************************************************************************
  template <auto Function,
            typename F = decltype(Function),
            typename = etl::enable_if_t<etl::is_pointer<F>::value && etl::is_function<etl::remove_pointer_t<F>>::value>>
  ETL_NODISCARD
  auto make_inplace_function()
  {
    using function_type = typename etl::function_traits<F>::function_type;

    return etl::inplace_function<function_type, 1, 1>::template create<Function>();
  }

  //*************************************************************************
  /// Make an inplace_function from a member function at compile time.
  /// \tparam TObject The object type.
  /// \tparam Method  The member function pointer.
  /// \tparam Instance The instance reference.
  /// \return The constructed inplace_function.
  //*************************************************************************
  template <typename TObject, 
            auto Method, 
            TObject& Instance,
            typename T = decltype(Method),
            typename = etl::enable_if_t<etl::is_member_function_pointer<T>::value>,
            typename = etl::enable_if_t<!etl::function_traits<T>::is_const>>
    ETL_NODISCARD
    auto make_inplace_function()
  {
    using function_type = typename etl::function_traits<decltype(Method)>::function_type;

    return etl::inplace_function<function_type, 1, 1>::template create<TObject, Method, Instance>();
  }

  //*************************************************************************
  /// Make an inplace_function from a const member function at compile time.
  /// \tparam TObject The object type.
  /// \tparam Method  The member function pointer.
  /// \tparam Instance The instance reference.
  /// \return The constructed inplace_function.
  //*************************************************************************
  template <typename TObject, 
            auto Method, 
            const TObject& Instance,
            typename T = decltype(Method),
            typename = etl::enable_if_t<etl::is_member_function_pointer<T>::value>,
            typename = etl::enable_if_t<etl::function_traits<T>::is_const>>
    ETL_NODISCARD
    auto make_inplace_function()
  {
    using function_type = typename etl::function_traits<T>::function_type;

    return etl::inplace_function<function_type, 1, 1>::template create<TObject, Method, Instance>();
  }

  //*************************************************************************
  /// Make an inplace_function from operator() at compile time.
  /// \tparam TObject The object type.
  /// \tparam Instance The instance reference.
  /// \return The constructed inplace_function.
  //*************************************************************************
  template <typename TObject,
            TObject& Instance,
            typename T = etl::decay_t<TObject>,
            typename = etl::enable_if_t<etl::is_class<T>::value && etl::has_call_operator<T>::value>>
  ETL_NODISCARD
  auto make_inplace_function()
  {
    using function_type = typename etl::function_traits<decltype(&TObject::operator())>::function_type;

    return etl::inplace_function<function_type, 1, 1>::template create<TObject, Instance>();
  }
#endif

  //*************************************************************************
  /// Swap two inplace_functions.
  /// \param lhs The first inplace_function.
  /// \param rhs The second inplace_function.
  //*************************************************************************
  template <typename TSignature, size_t Object_Size, size_t Object_Alignment>
  void swap(etl::inplace_function<TSignature, Object_Size, Object_Alignment>& lhs, etl::inplace_function<TSignature, Object_Size, Object_Alignment>& rhs) noexcept
  {
    lhs.swap(rhs);
  }

  //*************************************************************************
  /// Check inplace_function for equality with nullptr.
  /// \param lhs The inplace_function.
  /// \param rhs The nullptr.
  /// \returns true if equal.
  //*************************************************************************
  template <typename TSignature, size_t Object_Size, size_t Object_Alignment>
  ETL_NODISCARD
  bool operator ==(const etl::inplace_function<TSignature, Object_Size, Object_Alignment>& lhs, etl::nullptr_t)
  {
    return !lhs.is_valid();
  }

  //*************************************************************************
  /// Check inplace_function for equality with nullptr.
  /// \param lhs The nullptr.
  /// \param rhs The inplace_function.
  /// \returns true if equal.
  //*************************************************************************
  template <typename TSignature, size_t Object_Size, size_t Object_Alignment>
  ETL_NODISCARD
  bool operator ==(etl::nullptr_t, const etl::inplace_function<TSignature, Object_Size, Object_Alignment>& rhs)
  {
    return !rhs.is_valid();
  }

  //*************************************************************************
  /// Check inplace_function for inequality with nullptr.
  /// \param lhs The inplace_function.
  /// \param rhs The nullptr.
  /// \returns true if not equal.
  //*************************************************************************
  template <typename TSignature, size_t Object_Size, size_t Object_Alignment>
  ETL_NODISCARD
  bool operator !=(const etl::inplace_function<TSignature, Object_Size, Object_Alignment>& lhs, etl::nullptr_t)
  {
    return lhs.is_valid();
  }

  //*************************************************************************
  /// Check inplace_function for inequality with nullptr.
  /// \param lhs The nullptr.
  /// \param rhs The inplace_function.
  /// \returns true if not equal.
  //*************************************************************************
  template <typename TSignature, size_t Object_Size, size_t Object_Alignment>
  ETL_NODISCARD
  bool operator !=(etl::nullptr_t, const etl::inplace_function<TSignature, Object_Size, Object_Alignment>& rhs)
  {
    return rhs.is_valid();
  }
}

#endif
#endif