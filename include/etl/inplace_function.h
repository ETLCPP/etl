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
#include "error_handler.h"
#include "exception.h"
#include "file_error_numbers.h"

namespace etl
{
  class inplace_function_exception : public etl::exception
  {
  public:

    inplace_function_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  class inplace_function_uninitialized : public inplace_function_exception
  {
  public:

    inplace_function_uninitialized(string_type file_name_, numeric_type line_number_)
      : inplace_function_exception(ETL_ERROR_TEXT("inplace_function:inplace_function_uninitialized", ETL_INPLACE_FUNCTION_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

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

    invoke_type  invoke  = ETL_NULLPTR;
    destroy_type destroy = ETL_NULLPTR;
    move_type    move    = ETL_NULLPTR;
    copy_type    copy    = ETL_NULLPTR;

    template <typename T>
    struct member_target
    {
      T* obj;
      TReturn (T::*member)(TArgs...);
    };

    template <typename T>
    struct const_member_target
    {
      const T* obj;
      TReturn (T::*member)(TArgs...) const;
    };

    //*******************************************
    // Create vtable for free function pointer
    //*******************************************
    static const inplace_function_vtable* for_function_ptr() 
    {
      using function_type = TReturn(*)(TArgs...);

      static const inplace_function_vtable vtable
      {
        // invoke
        [](void* p, TArgs... a) -> TReturn { auto* fn = static_cast<function_type*>(p); return (*fn)(etl::forward<TArgs>(a)...); },
        // destroy
        ETL_NULLPTR,
        // move
        [](void* dst, void* src) { ::new (dst) function_type(*static_cast<function_type*>(src)); },
        // copy
        [](void* dst, const void* src) { ::new (dst) function_type(*static_cast<const function_type*>(src)); }
      };

      return &vtable;
    }

    //*******************************************
    // Create vtable for non-const member
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
        ETL_NULLPTR,
        // move
        [](void* dst, void* src) { ::new (dst) target_t(etl::move(*static_cast<target_t*>(src))); },
        // copy
        [](void* dst, const void* src) { ::new (dst) target_t(*static_cast<const target_t*>(src)); }
      };

      return &vtable;
    }

    //*******************************************
    // Create vtable for const member
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
        ETL_NULLPTR,
        // move
        [](void* dst, void* src) { ::new (dst) target_t(etl::move(*static_cast<target_t*>(src))); },
        // copy
        [](void* dst, const void* src) { ::new (dst) target_t(*static_cast<const target_t*>(src)); }
      };

      return &vtable;
    }

    //*******************************************
    // Create vtable for functor/lambda type
    //*******************************************
    template <typename T>
    static const inplace_function_vtable* for_functor() 
    {
      // Choose destroy and move stubs based on trivial destructibility.
      destroy_type destroy_ptr = etl::is_trivially_destructible<T>::value ? ETL_NULLPTR 
                                                                        : [](void* p) { static_cast<T*>(p)->~T(); };

      move_type move_ptr = etl::is_trivially_destructible<T>::value ? [](void* dst, void* src) { ::new (dst) T(etl::move(*static_cast<T*>(src))); }
                                                                  : [](void* dst, void* src) { ::new (dst) T(etl::move(*static_cast<T*>(src))); static_cast<T*>(src)->~T(); };

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
  };

  //*************************************************************************
  template <typename TSignature, size_t Object_Size, size_t Object_Alignment>
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
    using this_type     = inplace_function<TReturn(TArgs...), Object_Size, Object_Alignment>;
    using function_type = TReturn(*)(TArgs...);
    using storage_type  = etl::uninitialized_buffer<Object_Size, 1, Object_Alignment>;
    using vtable_type   = inplace_function_vtable<TReturn, TArgs...>;

  public:

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
    // Create from function pointer.
    //*************************************************************************
    inplace_function(function_type f)
    {
      set(f);
    }

    //*************************************************************************
    // Create from object + non-const member function (runtime).
    // Stores {object*, member ptr} in SBO and dispatches via stub.
    //*************************************************************************
    template <typename TObject>
    inplace_function(TObject& obj, TReturn (TObject::*method)(TArgs...))
    {
      set(obj, method);
    }

    //*************************************************************************
    // Create from object + const member function (runtime).
    // Stores {object*, member ptr} in SBO and dispatches via stub.
    //*************************************************************************
    template <typename TObject>
    inplace_function(const TObject& obj, TReturn(TObject::* method)(TArgs...) const)
    {
      set(obj, method);
    }

    //*************************************************************************
    // Create from lambda/functor (runtime, SBO-owned).
    // Stores the callable into SBO and dispatches via a typed stub.
    //*************************************************************************
    template <typename TLambda,
              typename T = typename etl::decay<TLambda>::type,
              typename = etl::enable_if_t<etl::is_class<T>::value && !is_inplace_function<T>::value, void>>
    inplace_function(TLambda&& lambda)
    {
      set(etl::forward<TLambda>(lambda));
    }
    
    //*************************************************************************
    // Create from function pointer (runtime).
    // Stores the pointer into SBO and dispatches via stub.
    //*************************************************************************
    void set(function_type f)
    {
      static_assert(Object_Size      >= sizeof(function_type),   "etl::inplace_function: SBO size too small");
      static_assert(Object_Alignment >= alignof(function_type), "etl::inplace_function: SBO alignment too small");

      clear();

      auto* slot = reinterpret_cast<function_type*>(&storage);
      ::new (slot) function_type(f);

      vtable = inplace_function_vtable<TReturn, TArgs...>::for_function_ptr();

      object = &storage;
    }

    //*************************************************************************
    // Create from object + member function (runtime).
    // Stores {object*, member ptr} in SBO and dispatches via stub.
    //*************************************************************************
    template <typename TObject>
    void set(TObject& obj, TReturn(TObject::* method)(TArgs...))
    {
      using target_t = inplace_function_vtable<TReturn, TArgs...>::template member_target<TObject>;

      static_assert(Object_Size      >= sizeof(target_t),  "etl::inplace_function: SBO size too small");
      static_assert(Object_Alignment >= alignof(target_t), "etl::inplace_function: SBO alignment too small");

      clear();

      auto* slot = reinterpret_cast<target_t*>(&storage);
      ::new (slot) target_t{ &obj, method };

      object = &storage;
      vtable = inplace_function_vtable<TReturn, TArgs...>::template for_member<TObject>();
    }

    //*************************************************************************
    // Create from object + const member function (runtime).
    // Stores {object*, member ptr} in SBO and dispatches via stub.
    //*************************************************************************
    template <typename TObject>
    void set(const TObject& obj, TReturn(TObject::* method)(TArgs...) const)
    {
      using target_t = inplace_function_vtable<TReturn, TArgs...>::template const_member_target<TObject>;

      static_assert(Object_Size      >= sizeof(target_t),  "etl::inplace_function: SBO size too small");
      static_assert(Object_Alignment >= alignof(target_t), "etl::inplace_function: SBO alignment too small");

      clear();

      auto* slot = reinterpret_cast<target_t*>(&storage);
      ::new (slot) target_t{ &obj, method };

      object = &storage;
      vtable = inplace_function_vtable<TReturn, TArgs...>::template for_const_member<TObject>();
    }

    //*************************************************************************
    // Create from lambda/functor (runtime, SBO-owned).
    // Stores the callable into SBO and dispatches via a typed stub.
    //*************************************************************************
    template <typename TLambda,
              typename T = typename etl::decay<TLambda>::type,
              typename = etl::enable_if_t<etl::is_class<T>::value && !is_inplace_function<T>::value, void>>
    void set(TLambda&& lambda)
    {
      static_assert(Object_Size      >= sizeof(T),  "etl::inplace_function: Object size too small");
      static_assert(Object_Alignment >= alignof(T), "etl::inplace_function: Object alignment too small");

      void* slot = &storage;
      ::new (slot) T(etl::forward<TLambda>(lambda));

      object = &storage;
      vtable = inplace_function_vtable<TReturn, TArgs...>::template for_functor<T>();
    }

    //*************************************************************************
    inplace_function& operator =(etl::nullptr_t) noexcept
    {
      clear();
      return *this;
    }

    //*************************************************************************
    inplace_function& operator =(function_type f)
    {
      set(f);

      return *this;
    }

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
    bool operator ==(const inplace_function& rhs) const noexcept
    {
      return (vtable == rhs.vtable);
    }

    //*************************************************************************
    bool operator !=(const inplace_function& rhs) const noexcept
    {
      return !(*this == rhs);
    }

    //*************************************************************************
    bool is_valid() const noexcept
    {
      return (vtable != ETL_NULLPTR);
    }

    //*************************************************************************
    explicit operator bool() const noexcept
    {
      return is_valid();
    }

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

  private:

    //*************************************************************************
    /// clear
    //*************************************************************************
    void clear() noexcept 
    {
      if (is_valid())
      {
        if (vtable->destroy)
        {
          vtable->destroy(object);
        }

        vtable = ETL_NULLPTR;
        object = ETL_NULLPTR;
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

        other.vtable  = ETL_NULLPTR;
        other.object = ETL_NULLPTR;
      }
      else 
      { 
        object = other.object; 
        other.vtable  = ETL_NULLPTR; 
        other.object = ETL_NULLPTR;
      }
    }

    void*              object = ETL_NULLPTR;
    const vtable_type* vtable  = ETL_NULLPTR;
    storage_type       storage;
  };

  //*************************************************************************
  // Helper to build an inplace_function from a free function pointer known at compile time.
  // Usage: auto f = etl::make_inplace_function<YourFreeFunction>();
  //*************************************************************************
  template <typename TReturn, typename... TArgs>
  inline etl::inplace_function<TReturn(TArgs...), sizeof(TReturn(*)(TArgs...)), alignof(TReturn(*)(TArgs...))>
    make_inplace_function(TReturn(*function)(TArgs...))
  {
    using function_type = TReturn(*)(TArgs...);

    return etl::inplace_function<TReturn(TArgs...), sizeof(function_type), alignof(function_type)>(function);
  }

  //*************************************************************************
  // Helper to build an inplace_function bound to a non-const member function.
  // Usage: auto f = etl::make_inplace_function(obj, &Type::Method);
  //*************************************************************************
  template <typename TObject, typename TReturn, typename... TArgs>
  inline etl::inplace_function<TReturn(TArgs...),
                               sizeof(typename etl::inplace_function_vtable<TReturn, TArgs...>::template member_target<TObject>),
                               alignof(typename etl::inplace_function_vtable<TReturn, TArgs...>::template member_target<TObject>)>
    make_inplace_function(TObject& obj, TReturn (TObject::*method)(TArgs...))
  {
    using target_t = typename etl::inplace_function_vtable<TReturn, TArgs...>::template member_target<TObject>;

    return etl::inplace_function<TReturn(TArgs...), sizeof(target_t), alignof(target_t)>(obj, method);
  }

  //*************************************************************************
  // Helper to build an inplace_function bound to a const member function.
  // Usage: auto f = etl::make_inplace_function(obj, &Type::Method)  // obj is const
  //*************************************************************************
  template <typename TObject, typename TReturn, typename... TArgs>
  inline etl::inplace_function<TReturn(TArgs...),
                               sizeof(typename etl::inplace_function_vtable<TReturn, TArgs...>::template const_member_target<TObject>),
                               alignof(typename etl::inplace_function_vtable<TReturn, TArgs...>::template const_member_target<TObject>)>
    make_inplace_function(const TObject& obj, TReturn (TObject::*method)(TArgs...) const)
  {
    using target_t = typename etl::inplace_function_vtable<TReturn, TArgs...>::template const_member_target<TObject>;

    return etl::inplace_function<TReturn(TArgs...), sizeof(target_t), alignof(target_t)>(obj, method);
  }

  //*************************************************************************
  // TODO: Needs lambda make_inplace_function
  //*************************************************************************

  //*************************************************************************
  template <typename TSignature, size_t Object_Size, size_t Object_Alignment>
  void swap(etl::inplace_function<TSignature, Object_Size, Object_Alignment>& lhs, etl::inplace_function<TSignature, Object_Size, Object_Alignment>& rhs) noexcept
  {
    lhs.swap(rhs);
  }

  //*************************************************************************
  template <typename TSignature, size_t Object_Size1, size_t Object_Alignment1, size_t Object_Size2, size_t Object_Alignment2>
  bool operator ==(etl::inplace_function<TSignature, Object_Size1, Object_Alignment1>& lhs, etl::inplace_function<TSignature, Object_Size2, Object_Alignment2>& rhs)
  {
    return lhs.operator==(rhs);
  }

  //*************************************************************************
  template <typename TSignature, size_t Object_Size1, size_t Object_Alignment1, size_t Object_Size2, size_t Object_Alignment2>
  bool operator !=(etl::inplace_function<TSignature, Object_Size1, Object_Alignment1>& lhs, etl::inplace_function<TSignature, Object_Size2, Object_Alignment2>& rhs)
  {
    return !(lhs == rhs);
  }
}

#endif
