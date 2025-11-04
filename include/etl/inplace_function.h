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
      using move_type    = void(*)(void* dst, void* src);
      using copy_type    = void(*)(void* dst, const void* src);

      invoke_type  invoke  = nullptr;
      destroy_type destroy = destroy_type(nullptr);
      move_type    move    = nullptr;
      copy_type    copy    = nullptr;

      //***********************************
      /// Target for non-const member function
      //***********************************
      template <typename T>
      struct member_target
      {
        T* obj;
        TReturn(T::* member)(TArgs...);
      };

      //***********************************
      /// Target for const member function
      //***********************************
      template <typename T>
      struct const_member_target
      {
        const T* obj;
        TReturn(T::* member)(TArgs...) const;
      };

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
          destroy_type(nullptr),
          // move
          [](void* dst, void* src) { ::new (dst) function_type(*static_cast<function_type*>(src)); },
          // copy
          [](void* dst, const void* src) { ::new (dst) function_type(*static_cast<const function_type*>(src)); }
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
          destroy_type(nullptr),
          // move
          [](void* dst, void* src) { ::new (dst) target_t(etl::move(*static_cast<target_t*>(src))); },
          // copy
          [](void* dst, const void* src) { ::new (dst) target_t(*static_cast<const target_t*>(src)); }
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
          destroy_type(nullptr),
          // move
          [](void* dst, void* src) { ::new (dst) target_t(etl::move(*static_cast<target_t*>(src))); },
          // copy
          [](void* dst, const void* src) { ::new (dst) target_t(*static_cast<const target_t*>(src)); }
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
        destroy_type destroy_ptr = etl::is_trivially_destructible<T>::value
                                     ? destroy_type(nullptr)
                                     : static_cast<destroy_type>(+[](void* p) { static_cast<T*>(p)->~T(); });

        move_type move_ptr = etl::is_trivially_destructible<T>::value
                               ? static_cast<move_type>(+[](void* dst, void* src) {::new (dst) T(etl::move(*static_cast<T*>(src)));})
                               : static_cast<move_type>(+[](void* dst, void* src) {::new (dst) T(etl::move(*static_cast<T*>(src))); static_cast<T*>(src)->~T();});

        static const inplace_function_vtable vtable
        {
          // invoke
          [](void* p, TArgs... a) -> TReturn { return (*static_cast<T*>(p))(etl::forward<TArgs>(a)...); },
          // destroy
          destroy_ptr,
          // move
          move_ptr,
          // copy
          [](void* dst, const void* src) { ::new (dst) T(*static_cast<const T*>(src)); }
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
        destroy_type destroy_ptr = etl::is_trivially_destructible<T>::value
          ? destroy_type(nullptr)
          : static_cast<destroy_type>(+[](void* p) { static_cast<T*>(p)->~T(); });

        move_type move_ptr = etl::is_trivially_destructible<T>::value
          ? static_cast<move_type>(+[](void* dst, void* src) {::new (dst) T(etl::move(*static_cast<T*>(src)));})
          : static_cast<move_type>(+[](void* dst, void* src) {::new (dst) T(etl::move(*static_cast<T*>(src))); static_cast<T*>(src)->~T();});

        static const inplace_function_vtable vtable
        {
          // invoke
          [](void* p, TArgs... a) -> TReturn { return (*static_cast<const T*>(p))(etl::forward<TArgs>(a)...); },
          // destroy
          destroy_ptr,
          // move
          move_ptr,
          // copy
          [](void* dst, const void* src) { ::new (dst) T(*static_cast<const T*>(src)); }
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
          nullptr, // destroy
          nullptr, // move
          nullptr  // copy
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
          // invoke ignores object and uses the known object + member pointer.
          [](void*, TArgs... a) -> TReturn { return (Object->*Method)(etl::forward<TArgs>(a)...); },
          nullptr, // destroy
          nullptr, // move
          nullptr  // copy
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
          nullptr, // destroy
          nullptr, // move
          nullptr  // copy
        };

        return &vtable;
      }

      //*******************************************
      // Compile-time bound operator() + object
      //*******************************************
      template <typename T, T* Object>
      static const inplace_function_vtable* for_compile_time_operator()
      {
        static const inplace_function_vtable vtable
        {
          [](void*, TArgs... a) -> TReturn {
            return (*Object).operator()(etl::forward<TArgs>(a)...);
          },
          nullptr, // destroy
          nullptr, // move
          nullptr  // copy
        };

        return &vtable;
      }
    };

    //*******************************************
    // Compile-time maximum value calculation
    //*******************************************
    template <size_t A, size_t B>
    struct max_value : etl::integral_constant<size_t, (A > B ? A : B)> {};

    template <size_t A, size_t... Rest>
    struct max_value_pack : etl::integral_constant<size_t, max_value<A, max_value_pack<Rest...>::value>::value> {};

    template <size_t A>
    struct max_value_pack<A> : etl::integral_constant<size_t, A> {};
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
      static_assert(Object_Size      >= Other_Object_Size,      "etl::inplace_function: Object size too small");
      static_assert(Object_Alignment >= Other_Object_Alignment, "etl::inplace_function: Object alignment too small");

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
      static_assert(Object_Size      >= Other_Object_Size,      "etl::inplace_function: Object size too small");
      static_assert(Object_Alignment >= Other_Object_Alignment, "etl::inplace_function: Object alignment too small");

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
    inplace_function(TObject& obj, TReturn (TObject::*method)(TArgs...))
    {
      set(obj, method);
    }

    //*************************************************************************
    /// Construct from object + const member function (runtime).
    /// Stores {object*, member ptr} in storage and dispatches via stub.
    //*************************************************************************
    template <typename TObject>
    inplace_function(const TObject& obj, TReturn(TObject::* method)(TArgs...) const)
    {
      set(obj, method);
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
      static_assert(Object_Size      >= sizeof(function_type),  "etl::inplace_function: storage size too small");
      static_assert(Object_Alignment >= alignof(function_type), "etl::inplace_function: storage alignment too small");

      clear();

      auto* slot = reinterpret_cast<function_type*>(&storage);
      ::new (slot) function_type(f);

      vtable = vtable_type::for_function_ptr();
      object = &storage;
    }

    //*************************************************************************
    /// Create from object + member function (runtime).
    /// Stores {object*, member ptr} in storage and dispatches via stub.
    //*************************************************************************
    template <typename TObject>
    void set(TObject& obj, TReturn(TObject::* method)(TArgs...))
    {
      using target_t = vtable_type::template member_target<TObject>;

      static_assert(Object_Size      >= sizeof(target_t),  "etl::inplace_function: storage size too small");
      static_assert(Object_Alignment >= alignof(target_t), "etl::inplace_function: storage alignment too small");

      clear();

      auto* slot = reinterpret_cast<target_t*>(&storage);
      ::new (slot) target_t{ &obj, method };

      object = &storage;
      vtable = vtable_type::template for_member<TObject>();
    }

    //*************************************************************************
    /// Create from object + const member function (runtime).
    /// Stores {object*, member ptr} in storage and dispatches via stub.
    //*************************************************************************
    template <typename TObject>
    void set(const TObject& obj, TReturn(TObject::* method)(TArgs...) const)
    {
      using target_t = vtable_type::template const_member_target<TObject>;

      static_assert(Object_Size      >= sizeof(target_t),  "etl::inplace_function: storage size too small");
      static_assert(Object_Alignment >= alignof(target_t), "etl::inplace_function: storage alignment too small");

      clear();

      auto* slot = reinterpret_cast<target_t*>(&storage);
      ::new (slot) target_t{ &obj, method };

      object = &storage;
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
      static_assert(Object_Size      >= sizeof(T),  "etl::inplace_function: Object size too small");
      static_assert(Object_Alignment >= alignof(T), "etl::inplace_function: Object alignment too small");

      void* slot = &storage;
      ::new (slot) T(lambda);

      object = &storage;
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
      static_assert(Object_Size      >= sizeof(T),  "etl::inplace_function: Object size too small");
      static_assert(Object_Alignment >= alignof(T), "etl::inplace_function: Object alignment too small");

      void* slot = &storage;
      ::new (slot) T(lambda);

      object = &storage;
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
      clear();
      vtable = vtable_type::template for_compile_time_function<Function>();
      object = nullptr;
    }

    //*************************************************************************
    // Set from a compile-time bound non-const member + instance reference.
    // Instance must have external linkage when used as an NTTP.
    //*************************************************************************
    template <typename TObject, TReturn(TObject::*Method)(TArgs...), TObject& Instance>
    void set()
    {
      clear();
      vtable = vtable_type::template for_compile_time_member<TObject, Method, &Instance>();
      object = nullptr;
    }

    //*************************************************************************
    // Set from a compile-time bound const member + instance reference.
    //*************************************************************************
    template <typename TObject, TReturn(TObject::*Method)(TArgs...) const, const TObject& Instance>
    void set()
    {
      clear();
      vtable = vtable_type::template for_compile_time_const_member<TObject, Method, &Instance>();
      object = nullptr;
    }

    //*************************************************************************
    /// Free function (external linkage)
    //*************************************************************************
    template <TReturn(*Function)(TArgs...)>
    static this_type create()
    {
      return this_type(vtable_type::template for_compile_time_function<Function>(), nullptr);
    }

    //*************************************************************************
    // Non-const member + instance reference (compile time)
    //*************************************************************************
    template <typename TObject, TReturn(TObject::*Method)(TArgs...), TObject& Instance>
    static this_type create()
    {
      return this_type(vtable_type::template for_compile_time_member<TObject, Method, &Instance>(), nullptr);
    }

    //*************************************************************************
    // Const member + instance reference (compile time)
    //*************************************************************************
    template <typename TObject, TReturn(TObject::*Method)(TArgs...) const, const TObject& Instance>
    static this_type create()
    {
      return this_type(vtable_type::template for_compile_time_const_member<TObject, Method, &Instance>(), nullptr);
    }

    // operator() + instance reference (compile time, non-const)
    template <typename TObject, TObject& Instance>
    static this_type create()
    {
      return this_type(vtable_type::template for_compile_time_operator<TObject, &Instance>(), nullptr);
    }

    //// (optional) matching set() helpers
    //template <typename TObject, TObject& Instance>
    //void set()
    //{
    //  clear();
    //  vtable = vtable_type::template for_compile_time_operator<TObject, &Instance>();
    //  object = nullptr;
    //}

    //template <typename TObject, const TObject& Instance>
    //void set()
    //{
    //  clear();
    //  vtable = vtable_type::template for_compile_time_const_operator<TObject, &Instance>();
    //  object = nullptr;
    //}

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

  // ...

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

      return vtable->invoke(object, etl::forward<TArgs>(args)...);
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
        vtable->invoke(object, etl::forward<TArgs>(args)...);
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
        result = vtable->invoke(object, etl::forward<TArgs>(args)...);
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
        return vtable->invoke(object, etl::forward<TArgs>(args)...);
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
    template <TReturn(*Method)(TArgs...)>
    TReturn call_or(TArgs... args) const
    {
      if (is_valid())
      {
        return vtable->invoke(object, etl::forward<TArgs>(args)...);
      }
      else
      {
        return (Method)(etl::forward<TArgs>(args)...);
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
          vtable->destroy(object);
        }

        vtable = nullptr;
        object = nullptr;
      }
    }

    //*************************************************************************
    /// reset
    /// Alias for clear.
    //*************************************************************************
    void reset() noexcept
    {
      clear();
    }

  private:

    // Allow cross-size access to internals
    template <typename, size_t, size_t>
    friend class inplace_function;

    //*************************************************************************
    // Direct-initialization constructor for CT-bound vtables (no payload).
    //*************************************************************************
    explicit inplace_function(const vtable_type* vt, void* obj) noexcept
      : object(obj)
      , vtable(vt)
      , storage()
    {}

    ////*************************************************************************
    // clone_from
    ////*************************************************************************
    template <size_t Object_Size, size_t Object_Alignment>
    void clone_from(const etl::inplace_function<TReturn(TArgs...), Object_Size, Object_Alignment>& other) 
    {
      vtable = other.vtable;

      if (vtable && vtable->copy) 
      { 
        vtable->copy(&storage, &other.storage); 
        object = &storage; 
      }
      else 
      { 
        object = other.object; 
      }
    }

    ////*************************************************************************
    // move_from
    ////*************************************************************************
    template <size_t Object_Size, size_t Object_Alignment>
    void move_from(etl::inplace_function<TReturn(TArgs...), Object_Size, Object_Alignment>& other) 
    {
      vtable = other.vtable;

      if (vtable && vtable->move) 
      { 
        vtable->move(&storage, &other.storage); 
        object = &storage; 

        other.vtable = nullptr;
        other.object = nullptr;
      }
      else 
      { 
        object = other.object; 
        other.vtable = nullptr; 
        other.object = nullptr;
      }
    }

    void*              object = nullptr;
    const vtable_type* vtable = nullptr;
    storage_type       storage;
  };

  //*************************************************************************
  /// Helper to build an inplace_function from a free function pointer known at compile time.
  /// Usage: auto ipf = etl::make_inplace_function<YourFreeFunction>();
  //*************************************************************************
  template <typename TReturn, typename... TArgs>
  etl::inplace_function<TReturn(TArgs...), sizeof(TReturn(*)(TArgs...)), alignof(TReturn(*)(TArgs...))>
  ETL_NODISCARD
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
    make_inplace_function(TObject& obj, TReturn (TObject::*method)(TArgs...))
  {
    using target_t = typename etl::private_inplace_function::inplace_function_vtable<TReturn, TArgs...>::template member_target<TObject>;

    return etl::inplace_function<TReturn(TArgs...), sizeof(target_t), alignof(target_t)>(obj, method);
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
    make_inplace_function(const TObject& obj, TReturn (TObject::*method)(TArgs...) const)
  {
    using target_t = typename etl::private_inplace_function::inplace_function_vtable<TReturn, TArgs...>::template const_member_target<TObject>;

    return etl::inplace_function<TReturn(TArgs...), sizeof(target_t), alignof(target_t)>(obj, method);
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
  auto make_inplace_function(TType&& storage)
  {
    return inplace_function_for<TSignature, T>(etl::forward<TType>(storage));
  }

#if ETL_USING_CPP17
  //*************************************************************************
  // Make a inplace_function from a function at compile time.
  //*************************************************************************
  template <auto Function>
  ETL_NODISCARD
  auto make_inplace_function()
  {
    using function_type = typename etl::function_traits<decltype(Function)>::function_type;

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
  template <typename TObject, TObject& Instance>
  ETL_NODISCARD
  auto make_inplace_function()
  {
    using function_type = typename etl::function_traits<decltype(&TObject::operator())>::function_type;

    return etl::inplace_function<function_type, 1, 1>::template create<TObject, Instance>();
  }
#endif

  //*************************************************************************
  // Alias: Choose buffer from a single type
  //*************************************************************************
  template <typename TSignature, typename TStorage>
  using inplace_function_for = etl::inplace_function<TSignature,
                                                     sizeof(etl::decay_t<TStorage>),
                                                     alignof(etl::decay_t<TStorage>)>;

  //*************************************************************************
  // Alias for multiple candidates – picks the largest size/alignment
  //*************************************************************************
  template <typename TSignature, typename T0, typename... TRest>
  using inplace_function_for_any = etl::inplace_function<TSignature,
                                                         etl::private_inplace_function::max_value<sizeof(etl::decay_t<T0>),  sizeof(etl::decay_t<TRest>)...>::value,
                                                         etl::private_inplace_function::max_value<alignof(etl::decay_t<T0>), alignof(etl::decay_t<TRest>)...>::value>;

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
