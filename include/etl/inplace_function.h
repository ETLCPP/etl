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
FITNESS FOR TArgs PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#ifndef ETL_INPLACE_FUNCTION_INCLUDED
#define ETL_INPLACE_FUNCTION_INCLUDED

#include "platform.h"
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
  #define ETL_DEFAULT_INPLACE_FUNCTION_SIZE 16
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
    /// VTable for inplace_function
    //*************************************************************************
    template <typename TReturn, typename... TArgs>
    struct inplace_function_vtable
    {
      using invoke_type  = TReturn(*)(void*, TArgs...);
      using destroy_type = void(*)(void*);
      using move_type    = void(*)(void* src,       void* dst);
      using copy_type    = void(*)(const void* src, void* dst);
      using equals_type  = bool(*)(const void*, const void*);

      invoke_type  invoke  = nullptr;
      destroy_type destroy = nullptr;
      move_type    move    = nullptr;
      copy_type    copy    = nullptr;
      equals_type  equals  = nullptr;

      //*******************************************
      /// Target for non-const member function
      //*******************************************
      template <typename TObject>
      struct member_target
      {
        TReturn(TObject::* member)(TArgs...);
        TObject* obj;
      };

      //*******************************************
      /// Target for const member function
      //*******************************************
      template <typename TObject>
      struct const_member_target
      {
        TReturn(TObject::* member)(TArgs...) const;
        const TObject* obj;
      };

      //*******************************************
      // Copy construct from src storage.
      //*******************************************
      template <typename T>
      static void copy_construct(const void* src, void* dst)
      {
        ::new (dst) T(*static_cast<const T*>(src));
      }

      //*******************************************
      // Move construct from src storage.
      //*******************************************
      template <typename T, bool DestroySrc>
      static void move_construct(void* dst, void* src)
      {
        ::new (dst) T(etl::move(*static_cast<T*>(src)));

        if (DestroySrc)
        {
          static_cast<T*>(src)->~T();
        }
      }

      //*******************************************
      // Create a vtable for free function pointer
      //*******************************************
      static const inplace_function_vtable* for_function_ptr()
      {
        using function_type = TReturn(*)(TArgs...);

        static const inplace_function_vtable vtable
        {
          // invoke
          [](void* p, TArgs... a) -> TReturn { auto* fn = static_cast<function_type*>(p); return (*fn)(etl::forward<TArgs>(a)...); },
          // destroy
          nullptr,
          // move (same as copy for function pointer)
          &move_construct<function_type, false>,
          // copy
          &copy_construct<function_type>,
          // equals (compare stored function pointer values)
          [](const void* lhs, const void* rhs) -> bool
          {
            auto* l = static_cast<const function_type*>(lhs);
            auto* r = static_cast<const function_type*>(rhs);
            return *l == *r;
          }
        };

        return &vtable;
      }

      //*******************************************
      // Create a vtable for non-const member
      //*******************************************
      template <typename T>
      static const inplace_function_vtable* for_member()
      {
        using target_t = member_target<T>;

        static const inplace_function_vtable vtable
        {
          // invoke
          [](void* p, TArgs... a) -> TReturn { auto* s = static_cast<target_t*>(p); return (s->obj->*s->member)(etl::forward<TArgs>(a)...); },
          // destroy
          nullptr,
          // move (same as copy; trivially copyable)
          &move_construct<target_t, false>,
          // copy
          &copy_construct<target_t>,
          // equals (same object address and same member pointer)
          [](const void* lhs, const void* rhs) -> bool
          {
            auto* l = static_cast<const target_t*>(lhs);
            auto* r = static_cast<const target_t*>(rhs);
            return (l->obj == r->obj) && (l->member == r->member);
          }
        };

        return &vtable;
      }

      //*******************************************
      // Create a vtable for const member
      //*******************************************
      template <typename T>
      static const inplace_function_vtable* for_const_member()
      {
        using target_t = const_member_target<T>;

        static const inplace_function_vtable vtable
        {
          // invoke
          [](void* p, TArgs... a) -> TReturn { auto* s = static_cast<target_t*>(p); return (s->obj->*s->member)(etl::forward<TArgs>(a)...); },
          // destroy
          nullptr,
          // move (same as copy; trivially copyable)
          &move_construct<target_t, false>,
          // copy
          &copy_construct<target_t>,
          [](const void* lhs, const void* rhs) -> bool
          {
            auto* l = static_cast<const target_t*>(lhs);
            auto* r = static_cast<const target_t*>(rhs);
            return (l->obj == r->obj) && (l->member == r->member);
          }
        };

        return &vtable;
      }

      //*******************************************
      // Create a vtable for functor/lambda type
      //*******************************************
      template <typename T>
      static const inplace_function_vtable* for_functor()
      {
        // Choose destroy and move stubs based on trivial destructibility.
        constexpr bool destroy_src_on_move = !etl::is_trivially_destructible<T>::value;

        destroy_type destroy_ptr = etl::is_trivially_destructible<T>::value
          ? nullptr
          : static_cast<destroy_type>([](void* p) { static_cast<T*>(p)->~T(); });

        static const inplace_function_vtable vtable
        {
          // invoke
          [](void* p, TArgs... a) -> TReturn { return (*static_cast<T*>(p))(etl::forward<TArgs>(a)...); },
          // destroy
          destroy_ptr,
          // move
          &move_construct<T, destroy_src_on_move>,
          // copy
          &copy_construct<T>,
          nullptr  // No equality for non-const functors
        };

        return &vtable;
      }

      //*******************************************
      // Create a vtable for const functor/lambda type
      //*******************************************
      template <typename T>
      static const inplace_function_vtable* for_const_functor()
      {
        // Choose destroy and move stubs based on trivial destructibility.
        constexpr bool destroy_src_on_move = !etl::is_trivially_destructible<T>::value;

        destroy_type destroy_ptr = etl::is_trivially_destructible<T>::value
          ? nullptr
          : static_cast<destroy_type>([](void* p) { static_cast<T*>(p)->~T(); });

        static const inplace_function_vtable vtable
        {
          // invoke
          [](void* p, TArgs... a) -> TReturn { return (*static_cast<const T*>(p))(etl::forward<TArgs>(a)...); },
          // destroy
          destroy_ptr,
          // move
          &move_construct<T, destroy_src_on_move>,
          // copy
          &copy_construct<T>,
          nullptr  // No equality for const functors
        };

        return &vtable;
      }

      //*******************************************
      // Create a vtable for compile-time bound free function
      //*******************************************
      template <TReturn(*Function)(TArgs...)>
      static const inplace_function_vtable* for_compile_time_function()
      {
        static const inplace_function_vtable vtable
        {
          // invoke ignores object and calls the known function.
          [](void*, TArgs... a) -> TReturn { return Function(etl::forward<TArgs>(a)...); },
          nullptr, 
          nullptr, 
          nullptr,
          // Same vtable instance => same target identity
          [](const void*, const void*) -> bool { return true; }
        };

        return &vtable;
      }

      //*******************************************
      // Create a vtable for compile-time bound non-const member function + object
      //*******************************************
      template <typename T, TReturn(T::* Method)(TArgs...), T* Object>
      static const inplace_function_vtable* for_compile_time_member()
      {
        static const inplace_function_vtable vtable
        {
          [](void*, TArgs... a) -> TReturn { return (Object->*Method)(etl::forward<TArgs>(a)...); },
          nullptr, 
          nullptr, 
          nullptr,
          // Same vtable instance => same target identity
          [](const void*, const void*) -> bool { return true; }
        };

        return &vtable;
      }

      //*******************************************
      // Create a vtable for compile-time bound const member function + object
      //*******************************************
      template <typename T, TReturn(T::* Method)(TArgs...) const, const T* Object>
      static const inplace_function_vtable* for_compile_time_const_member()
      {
        static const inplace_function_vtable vtable
        {
          [](void*, TArgs... a) -> TReturn { return (Object->*Method)(etl::forward<TArgs>(a)...); },
          nullptr, 
          nullptr, 
          nullptr,
          // Same vtable instance => same target identity
          [](const void*, const void*) -> bool { return true; }
        };

        return &vtable;
      }

      //*******************************************
      // Create a vtable for compile-time bound operator() + object
      //*******************************************
      template <typename T, T* Object>
      static const inplace_function_vtable* for_compile_time_operator()
      {
        static const inplace_function_vtable vtable
        {
          [](void*, TArgs... a) -> TReturn { return (*Object).operator()(etl::forward<TArgs>(a)...); },
          nullptr, 
          nullptr, 
          nullptr,
          // Same vtable instance => same target identity
          [](const void*, const void*) -> bool { return true; }
        };

        return &vtable;
      }
    };
  }

  //*************************************************************************
  template <typename TSignature, 
            size_t Object_Size      = ETL_DEFAULT_INPLACE_FUNCTION_SIZE, 
            size_t Object_Alignment = ETL_DEFAULT_INPLACE_FUNCTION_ALIGNMENT>
  class inplace_function;

  //*************************************************************************
  // Is not an etl::inplace_function
  template <typename T>
  struct is_inplace_function : etl::false_type {};

  // Is an etl::inplace_function
  template <typename TReturn, typename... TArgs, size_t Object_Size, size_t Object_Alignment>
  struct is_inplace_function<etl::inplace_function<TReturn(TArgs...), Object_Size, Object_Alignment>> : etl::true_type {};

  //*************************************************************************
  /// inplace_function
  template <typename TReturn, typename... TArgs, size_t Object_Size, size_t Object_Alignment>
  class inplace_function<TReturn(TArgs...), Object_Size, Object_Alignment>
  {
    using this_type      = inplace_function<TReturn(TArgs...), Object_Size, Object_Alignment>;
    using function_type  = TReturn(*)(TArgs...);
    using storage_type   = etl::uninitialized_buffer<Object_Size, 1, Object_Alignment>;
    using vtable_type    = private_inplace_function::inplace_function_vtable<TReturn, TArgs...>;

  public:

    using return_type    = TReturn;
    using argument_types = etl::type_list<TArgs...>;

    //*************************************************************************
    /// Invokability trait bound to this inplace_function's signature
    /// Usage: <code>inplace_function<R(Args...)>::etl::is_invocable<Callable>::value</code>
    //*************************************************************************
    template <typename TCallable>
    using is_invocable = etl::is_invocable<TCallable, TReturn(TArgs...)>;

#if ETL_USING_CPP17
    template <typename TCallable>
    static constexpr bool is_invocable_v = etl::is_invocable<TCallable>::value;
#endif

    //*************************************************************************
    /// Default constructor
    //*************************************************************************
    inplace_function() noexcept = default;

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    inplace_function(const inplace_function& other)
    {
      clone_from(other);
    }

    //*************************************************************************
    /// Copy constructor for different object buffer sizes
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
    //*************************************************************************
    inplace_function(inplace_function&& other) noexcept
    {
      move_from(other);
    }

    //*************************************************************************
    /// Move constructor for different object buffer sizes
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
    ~inplace_function()
    {
      clear();
    }

    //*************************************************************************
    /// Construct from function pointer.
    //*************************************************************************
    inplace_function(function_type f)
    {
      set(f);
    }

    //*************************************************************************
    /// Construct from object + non-const member function (runtime).
    /// Stores {object*, member ptr} in storage and dispatches via stub.
    //*************************************************************************
    template <typename TObject>
    inplace_function(TReturn (TObject::*method)(TArgs...), TObject& obj)
    {
      set(method, obj);
    }

    //*************************************************************************
    /// Construct from object + const member function (runtime).
    /// Stores {object*, member ptr} in storage and dispatches via stub.
    //*************************************************************************
    template <typename TObject>
    inplace_function(TReturn(TObject::* method)(TArgs...) const, const TObject& obj)
    {
      set(method, obj);
    }

    //*************************************************************************
    /// Construct from lambda/functor (runtime, storage-owned).
    /// Stores the callable into storage and dispatches via a typed stub.
    //*************************************************************************
    template <typename TLambda,
              typename T = typename etl::decay<TLambda>::type,
              typename = etl::enable_if_t<etl::is_class<T>::value && !is_inplace_function<T>::value, void>>
    inplace_function(TLambda& lambda)
    {
      set(lambda);
    }

    //*************************************************************************
    /// Construct from const lambda/functor (runtime, storage-owned).
    /// Stores the callable into storage and dispatches via a typed stub.
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
    /// Stores the pointer into storage and dispatches via stub.
    //*************************************************************************
    void set(function_type f)
    {
      // Validate that 'f' is invocable with (TArgs...) and returns TReturn
      static_assert(etl::is_invocable_r<TReturn, function_type, TArgs...>::value,
                    "etl::inplace_function: function pointer is not compatible with the inplace_function signature");

      static_assert(Object_Size      >= sizeof(function_type),  "etl::inplace_function: storage size too small");
      static_assert(Object_Alignment >= alignof(function_type), "etl::inplace_function: storage alignment too small");

      clear();

      // Construct the object in the storage.
      ::new (storage_ptr()) function_type(f);

      vtable = vtable_type::for_function_ptr();
    }

    //*************************************************************************
    /// Create from object + member function (runtime).
    /// Stores {object*, member ptr} in storage and dispatches via stub.
    //*************************************************************************
    template <typename TObject>
    void set(TReturn(TObject::* method)(TArgs...), TObject& obj)
    {
      // Validate that 'method' is invocable with (TObject&, TArgs...) and returns TReturn
      static_assert(etl::is_invocable_r<TReturn, decltype(method), TObject&, TArgs...>::value,
                    "etl::inplace_function: bound member function is not compatible with the inplace_function signature");

      using target_t = typename vtable_type::template member_target<TObject>;

      static_assert(Object_Size      >= sizeof(target_t),  "etl::inplace_function: storage size too small");
      static_assert(Object_Alignment >= alignof(target_t), "etl::inplace_function: storage alignment too small");

      clear();

      // Construct the object in the storage.
      ::new (storage_ptr()) target_t{ method, &obj };
            
      vtable = vtable_type::template for_member<TObject>();
    }

    //*************************************************************************
    /// Create from object + const member function (runtime).
    /// Stores {object*, member ptr} in storage and dispatches via stub.
    //*************************************************************************
    template <typename TObject>
    void set(TReturn(TObject::* method)(TArgs...) const, const TObject& obj)
    {
      // Validate that 'method' is invocable with (TObject&, TArgs...) and returns TReturn
      static_assert(etl::is_invocable_r<TReturn, decltype(method), TObject&, TArgs...>::value,
                    "etl::inplace_function: bound member function is not compatible with the inplace_function signature");

      using target_t = typename vtable_type::template const_member_target<TObject>;

      static_assert(Object_Size      >= sizeof(target_t),  "etl::inplace_function: storage size too small");
      static_assert(Object_Alignment >= alignof(target_t), "etl::inplace_function: storage alignment too small");

      clear();

      // Construct the object in the storage.
      ::new (storage_ptr()) target_t{ method, &obj };

      
      vtable = vtable_type::template for_const_member<TObject>();
    }

    //*************************************************************************
    /// Create from lambda/functor (runtime, storage-owned).
    /// Stores the callable into storage and dispatches via a typed stub.
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
    /// Create from const lambda/functor (runtime, storage-owned).
    /// Stores the callable into storage and dispatches via a typed stub.
    //*************************************************************************
    template <typename TLambda,
              typename T = typename etl::decay<TLambda>::type,
              typename = etl::enable_if_t<etl::is_class<T>::value && !is_inplace_function<T>::value, void>>
    void set(const TLambda& lambda)
    {
      static_assert(etl::is_invocable<T>::value,
                    "etl::inplace_function: bound lambda/functor is not compatible with the inplace_function signature");

      //// For const functors, ensure operator() is const so invocation via const T* is valid.
      //static_assert(etl::function_traits<decltype(&T::operator())>::is_const,
      //              "etl::inplace_function: binding const functor requires const operator()");

      static_assert(Object_Size      >= sizeof(T),  "etl::inplace_function: Object size too small");
      static_assert(Object_Alignment >= alignof(T), "etl::inplace_function: Object alignment too small");

      clear();

      // Construct the object in the storage.
      ::new (storage_ptr()) T(lambda);
      
      vtable = vtable_type::template for_const_functor<T>();
    }

    //*************************************************************************
    /// Set from a compile-time bound free function (no payload stored).
    /// Function must have external linkage.
    /// Usage: ipf.template set<&FreeFn>();
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
    // Set from a compile-time bound non-const member + instance reference.
    // Instance must have external linkage when used as an NTTP.
    //*************************************************************************
    template <typename TObject, TReturn(TObject::*Method)(TArgs...), TObject& Instance>
    void set()
    {
      static_assert(etl::is_invocable<decltype(Method)>::value,
                    "etl::inplace_function: bound member function is not compatible with the inplace_function signature");

      clear();
      vtable = vtable_type::template for_compile_time_member<TObject, Method, &Instance>();
    }

    //*************************************************************************
    // Set from a compile-time bound const member + instance reference.
    //*************************************************************************
    template <typename TObject, TReturn(TObject::*Method)(TArgs...) const, const TObject& Instance>
    void set()
    {
      static_assert(etl::is_invocable<decltype(Method)>::value,
                    "etl::inplace_function: bound member function is not compatible with the inplace_function signature");

      clear();
      vtable = vtable_type::template for_compile_time_const_member<TObject, Method, &Instance>();
    }

    //*************************************************************************
    // operator() + instance reference (compile-time, non-const)
    //*************************************************************************
    template <typename TObject,
              TObject& Instance,
              typename T = etl::decay_t<TObject>,
              typename = etl::enable_if_t<etl::is_class<T>::value &&
                                          etl::has_call_operator<T>::value &&
                                          !etl::function_traits<decltype(&T::operator())>::is_const>>
    void set()
    {
      static_assert(etl::is_invocable<T>::value,
                    "etl::inplace_function: bound lambda/functor is not compatible with the inplace_function signature");

      clear();
      vtable = vtable_type::template for_compile_time_operator<TObject, &Instance>();
    }

    //*************************************************************************
    // operator() + instance reference (compile-time, const)
    //*************************************************************************
    template <typename TObject,
              const TObject& Instance,
              typename T = etl::decay_t<TObject>,
              typename = etl::enable_if_t<etl::is_class<T>::value &&
                                          etl::has_call_operator<T>::value &&
                                          etl::function_traits<decltype(&T::operator())>::is_const>>
    void set()
    {
      static_assert(etl::is_invocable<T>::value,
                    "etl::inplace_function: bound lambda/functor is not compatible with the inplace_function signature");

      clear();
      vtable = vtable_type::template for_compile_time_operator<const TObject, &Instance>();
    }

    //*************************************************************************
    /// Free function (external linkage)
    //*************************************************************************
    template <TReturn(*Function)(TArgs...)>
    static this_type create()
    {
      return this_type(vtable_type::template for_compile_time_function<Function>());
    }

    //*************************************************************************
    // Non-const member + instance reference (compile time)
    //*************************************************************************
    template <typename TObject, TReturn(TObject::*Method)(TArgs...), TObject& Instance>
    static this_type create()
    {
      return this_type(vtable_type::template for_compile_time_member<TObject, Method, &Instance>());
    }

    //*************************************************************************
    // Const member + instance reference (compile time)
    //*************************************************************************
    template <typename TObject, TReturn(TObject::*Method)(TArgs...) const, const TObject& Instance>
    static this_type create()
    {
      return this_type(vtable_type::template for_compile_time_const_member<TObject, Method, &Instance>());
    }

    //*************************************************************************
    // operator() + instance reference (compile time, non-const)
    //*************************************************************************
    template <typename TObject, TObject& Instance>
    static this_type create()
    {
      return this_type(vtable_type::template for_compile_time_operator<TObject, &Instance>());
    }

    //*************************************************************************
    /// Assignment operator
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
    //*************************************************************************
    inplace_function& operator =(etl::nullptr_t) noexcept
    {
      clear();

      return *this;
    }

    //*************************************************************************
    /// Assignment from function pointer
    //*************************************************************************
    inplace_function& operator =(function_type f)
    {
      set(f);

      return *this;
    }

    //*************************************************************************
    // Assignment from lambda/functor (runtime, storage-owned).
    // Enabled only for class types that are not etl::inplace_function.
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
    /// Equality operator
    //*************************************************************************
    template <size_t Other_Object_Size, size_t Other_Object_Alignment>
    bool operator ==(const etl::inplace_function<TReturn(TArgs...), Other_Object_Size, Other_Object_Alignment>& rhs) const
    {
      // If both empty then equal, else if one empty then not equal.
      if (vtable == nullptr || rhs.vtable == nullptr)
      {
        return vtable == rhs.vtable;
      }

      // If different target family/identity then not equal
      if (vtable != rhs.vtable)
      {
        return false;
      }

      // Same vtable, compare payloads if handler exists
      return vtable->equals ? vtable->equals(&storage, &rhs.storage) : false;
    }

    //*************************************************************************
    /// Inequality operator
    //*************************************************************************
    template <size_t Other_Object_Size, size_t Other_Object_Alignment>
    bool operator !=(const etl::inplace_function<TReturn(TArgs...), Other_Object_Size, Other_Object_Alignment>& rhs) const
    {
      return !(*this == rhs);
    }

    //*************************************************************************
    /// Swap with another inplace_function
    //*************************************************************************
    void swap(inplace_function& other)
    {
      if (this != &other)
      {
        inplace_function temp = etl::move(*this);
        *this                 = etl::move(other);
        other                 = etl::move(temp);
      }
    }

    //*************************************************************************
    /// Checks if the inplace_function is valid (has a target).
    //*************************************************************************
    ETL_NODISCARD
    bool is_valid() const noexcept
    {
      return (vtable != nullptr);
    }

    //*************************************************************************
    /// Boolean conversion operator.
    //*************************************************************************
    ETL_NODISCARD
    explicit operator bool() const noexcept
    {
      return is_valid();
    }

    //*************************************************************************
    /// Execute the inplace_function.
    //*************************************************************************
    TReturn operator()(TArgs... args) const
    {
      ETL_ASSERT(is_valid(), ETL_ERROR(inplace_function_uninitialized));

      return vtable->invoke(storage_ptr(), etl::forward<TArgs>(args)...);
    }

    //*************************************************************************
    /// Execute the is_inplace_function if valid.
    /// 'void' return.
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
    /// Execute the is_inplace_function if valid.
    /// Non 'void' return.
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
    /// Execute the is_inplace_function if valid or call alternative.
    /// Run time alternative.
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
    /// Execute the is_inplace_function if valid or call alternative.
    /// Compile time alternative.
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
    //*************************************************************************
    ETL_NODISCARD
    static constexpr size_t size() noexcept
    {
      return Object_Size;
    }

    //*************************************************************************
    /// Get the storage alignment
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
    // Get pointer to storage as a void*
    //*************************************************************************
    void* storage_ptr() const noexcept
    {
      return const_cast<void*>(static_cast<const void*>(&storage));
    }

#if ETL_IS_DEBUG_BUILD
    bool is_initialised;
#endif

    const vtable_type* vtable = nullptr;
    storage_type       storage;
  };

  //*************************************************************************
  /// Declare an inplace_function from a single type.
  //*************************************************************************
  template <typename TSignature, typename TStorage>
  using inplace_function_for = etl::inplace_function<TSignature,
                                                     sizeof(etl::decay_t<TStorage>),
                                                     alignof(etl::decay_t<TStorage>)>;

  //*************************************************************************
  /// Declare an inplace_function from multiple candidates. Picks the largest size/alignment.
  //*************************************************************************
  template <typename TSignature, typename T0, typename... TRest>
  using inplace_function_for_any = etl::inplace_function<TSignature,
                                                         etl::largest<etl::decay_t<T0>, etl::decay_t<TRest>...>::size,
                                                         etl::largest<etl::decay_t<T0>, etl::decay_t<TRest>...>::alignment>;

  //*************************************************************************
  /// Helper to build an inplace_function from a free function pointer known at compile time.
  /// Usage: auto ipf = etl::make_inplace_function<YourFreeFunction>();
  //*************************************************************************
  template <typename TReturn, typename... TArgs>
  ETL_NODISCARD
  etl::inplace_function<TReturn(TArgs...), sizeof(TReturn(*)(TArgs...)), alignof(TReturn(*)(TArgs...))>
    make_inplace_function(TReturn(*function)(TArgs...))
  {
    using function_type = TReturn(*)(TArgs...);

    return etl::inplace_function<TReturn(TArgs...), sizeof(function_type), alignof(function_type)>(function);
  }

  //*************************************************************************
  /// Helper to build an inplace_function bound to a non-const member function.
  /// Usage: auto ipf = etl::make_inplace_function(obj, &Type::Method);
  //*************************************************************************
  template <typename TObject, typename TReturn, typename... TArgs>
  ETL_NODISCARD
  etl::inplace_function<TReturn(TArgs...),
                               sizeof(typename etl::private_inplace_function::inplace_function_vtable<TReturn, TArgs...>::template member_target<TObject>),
                               alignof(typename etl::private_inplace_function::inplace_function_vtable<TReturn, TArgs...>::template member_target<TObject>)>
    make_inplace_function(TReturn (TObject::*method)(TArgs...), TObject& obj)
  {
    using target_t = typename etl::private_inplace_function::inplace_function_vtable<TReturn, TArgs...>::template member_target<TObject>;

    return etl::inplace_function<TReturn(TArgs...), sizeof(target_t), alignof(target_t)>(method, obj);
  }

  //*************************************************************************
  /// Helper to build an inplace_function bound to a const member function.
  /// Usage: auto ipf = etl::make_inplace_function(obj, &Type::Method)  // obj is const
  //*************************************************************************
  template <typename TObject, typename TReturn, typename... TArgs>
  ETL_NODISCARD
  etl::inplace_function<TReturn(TArgs...),
                               sizeof(typename etl::private_inplace_function::inplace_function_vtable<TReturn, TArgs...>::template const_member_target<TObject>),
                               alignof(typename etl::private_inplace_function::inplace_function_vtable<TReturn, TArgs...>::template const_member_target<TObject>)>
    make_inplace_function(TReturn (TObject::*method)(TArgs...) const, const TObject& obj)
  {
    using target_t = typename etl::private_inplace_function::inplace_function_vtable<TReturn, TArgs...>::template const_member_target<TObject>;

    return etl::inplace_function<TReturn(TArgs...), sizeof(target_t), alignof(target_t)>(method, obj);
  }

  //*************************************************************************
  // Helper to build an inplace_function from a lambda/functor (non-generic, non-overloaded).
  // Deduces R(Args...) from &T::operator().
  // Prefer this when operator() is not overloaded/templated.
  //*************************************************************************
  template <typename TLambda,
            typename T = typename etl::decay<TLambda>::type,
            typename = typename etl::enable_if_t<etl::is_class<T>::value && !is_inplace_function<T>::value, void>,
            typename TSignature = typename etl::function_traits<T>::function_type>
  ETL_NODISCARD
  etl::inplace_function<TSignature, sizeof(T), alignof(T)>
    make_inplace_function(TLambda&& lambda)
  {
    return etl::inplace_function<TSignature, sizeof(T), alignof(T)>(etl::forward<TLambda>(lambda));
  }

  //*************************************************************************
  // Storage-deducing maker (non-class only) to avoid colliding with functor maker
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
  // Make a inplace_function from a function at compile time.
  // Only participates for free function pointers (not member function pointers).
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
  /// Make a inplace_function from a member function at compile time.
  //*************************************************************************
  template <typename TObject, auto Method, TObject& Instance, typename = etl::enable_if_t<!etl::function_traits<decltype(Method)>::is_const>>
  ETL_NODISCARD
  auto make_inplace_function()
  {
    using function_type = typename etl::function_traits<decltype(Method)>::function_type;

    return etl::inplace_function<function_type, 1, 1>::template create<TObject, Method, Instance>();
  }

  //*************************************************************************
  /// Make a inplace_function from a const member function at compile time.
  //*************************************************************************
  template <typename TObject, auto Method, const TObject& Instance, typename = etl::enable_if_t<etl::function_traits<decltype(Method)>::is_const>>
  ETL_NODISCARD
  auto make_inplace_function()
  {
    using function_type = typename etl::function_traits<decltype(Method)>::function_type;

    return etl::inplace_function<function_type, 1, 1>::template create<TObject, Method, Instance>();
  }

  //*************************************************************************
  /// Make a inplace_function from operator() at compile time.
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
  //*************************************************************************
  template <typename TSignature, size_t Object_Size, size_t Object_Alignment>
  void swap(etl::inplace_function<TSignature, Object_Size, Object_Alignment>& lhs, etl::inplace_function<TSignature, Object_Size, Object_Alignment>& rhs) noexcept
  {
    lhs.swap(rhs);
  }

  //*************************************************************************
  /// Check inplace_function for equality with nullptr.
  //*************************************************************************
  template <typename TSignature, size_t Object_Size, size_t Object_Alignment>
  ETL_NODISCARD
  bool operator ==(etl::inplace_function<TSignature, Object_Size, Object_Alignment>& lhs, etl::nullptr_t)
  {
    return lhs == nullptr;
  }

  //*************************************************************************
  /// Check inplace_function for equality with nullptr.
  //*************************************************************************
  template <typename TSignature, size_t Object_Size, size_t Object_Alignment>
  ETL_NODISCARD
  bool operator ==(etl::nullptr_t, etl::inplace_function<TSignature, Object_Size, Object_Alignment>& rhs)
  {
    return rhs == nullptr;
  }

  //*************************************************************************
  /// Check inplace_function for inequality with nullptr.
  //*************************************************************************
  template <typename TSignature, size_t Object_Size, size_t Object_Alignment>
  ETL_NODISCARD
  bool operator !=(etl::inplace_function<TSignature, Object_Size, Object_Alignment>& lhs, etl::nullptr_t)
  {
    return !(lhs == nullptr);
  }

  //*************************************************************************
  /// Check inplace_function for inequality with nullptr.
  //*************************************************************************
  template <typename TSignature, size_t Object_Size, size_t Object_Alignment>
  ETL_NODISCARD
  bool operator !=(etl::nullptr_t, etl::inplace_function<TSignature, Object_Size, Object_Alignment>& rhs)
  {
    return !(rhs == nullptr);
  }
}

#endif
