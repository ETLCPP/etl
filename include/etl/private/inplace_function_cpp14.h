///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2025 BMW AG

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

#ifndef ETL_INPLACE_FUNCTION_CPP14_INCLUDED
#define ETL_INPLACE_FUNCTION_CPP14_INCLUDED

#include "../platform.h"
#include "../type_traits.h"
#include "../utility.h"

#ifndef ETL_INPLACE_FUNCTION_DEFAULT_CAPACITY
  #define ETL_INPLACE_FUNCTION_DEFAULT_CAPACITY 32
#endif
namespace etl
{
  namespace private_inplace_function
  {
    template <typename T>
    struct wrapper
    {
      using type = T;
    };
    template <typename R, typename... Args>
    struct vtable_t ETL_FINAL
    {
      using storage_ptr_t = void*;
      using invoke_func_t = R (*)(storage_ptr_t, Args&&...);
      using copy_or_move_func_t = void (*)(storage_ptr_t, storage_ptr_t);
      using dtor_func_t = void (*)(storage_ptr_t);

      const invoke_func_t       invoke_func{[](storage_ptr_t, Args&&...) -> R
                                      { return R(); }};
      const copy_or_move_func_t copy_func{[](storage_ptr_t, storage_ptr_t) -> void {}};
      const copy_or_move_func_t move_func{[](storage_ptr_t, storage_ptr_t) -> void {}};
      const dtor_func_t         dtor_func{[](storage_ptr_t) -> void {}};

      ~vtable_t() = default;

      ETL_CONSTEXPR vtable_t() ETL_NOEXCEPT = default;

      template <typename T>
      explicit ETL_CONSTEXPR vtable_t(wrapper<T>) ETL_NOEXCEPT
        : invoke_func{[](storage_ptr_t storage_ptr, Args&&... args) -> R
                      { return (*static_cast<T*>(storage_ptr))(
                          etl::forward<Args&&>(args)...); }},
          copy_func{[](storage_ptr_t dst_ptr, storage_ptr_t src_ptr) -> void
                    { ::new (dst_ptr) T{*static_cast<T*>(src_ptr)}; }},
          move_func{[](storage_ptr_t dst_ptr, storage_ptr_t src_ptr) -> void
                    {
                      ::new (dst_ptr) T{etl::move(*static_cast<T*>(src_ptr))};
                      static_cast<T*>(src_ptr)->~T();
                    }},
          dtor_func{[](storage_ptr_t src_ptr) -> void
                    { static_cast<T*>(src_ptr)->~T(); }}
      {
      }

    private:
      vtable_t(const vtable_t&) ETL_DELETE;
      vtable_t(vtable_t&&) ETL_DELETE;

      vtable_t& operator=(const vtable_t&) ETL_DELETE;
      vtable_t& operator=(vtable_t&&) ETL_DELETE;
    };

    template <class R, class... Args>
    ETL_INLINE_VAR ETL_CONSTEXPR17
        vtable_t<R, Args...>
        default_vtable{};
  }  // namespace private_inplace_function

  template <
    typename Signature,
    size_t Capacity = ETL_INPLACE_FUNCTION_DEFAULT_CAPACITY>
  class inplace_function;

  namespace private_inplace_function
  {
    template <typename>
    struct is_inplace_function : etl::false_type
    {
    };
    template <typename Sig, size_t Cap>
    struct is_inplace_function<inplace_function<Sig, Cap>> : etl::true_type
    {
    };
  }  // namespace private_inplace_function

  template <
    typename R,
    typename... Args,
    size_t Capacity>
  class inplace_function<R(Args...), Capacity> ETL_FINAL
  {
    using vtable_t = private_inplace_function::vtable_t<R, Args...>;
    using vtable_ptr_t = const vtable_t*;

    template <typename, size_t>
    friend class inplace_function;

  public:
    using capacity = etl::integral_constant<size_t, Capacity>;

    inplace_function() ETL_NOEXCEPT = default;

    template <
      typename T,
      typename C = etl::decay_t<T>,
      typename = etl::enable_if_t<
        !private_inplace_function::is_inplace_function<C>::value && etl::is_invocable_r<R, C&, Args...>::value>>
    inplace_function(T&& closure)
    {
      ETL_STATIC_ASSERT(etl::is_copy_constructible<C>::value,
                        "cannot be constructed from non-copyable types");

      ETL_STATIC_ASSERT(sizeof(C) <= Capacity,
                        "internal storage too small");

      static const vtable_t vt{private_inplace_function::wrapper<C>{}};

      vtable_ptr = &vt;
      ::new (storage) C{etl::forward<T>(closure)};
    }

    template <size_t Cap>
    inplace_function(const inplace_function<R(Args...), Cap>& other)
      : inplace_function(other.vtable_ptr, other.vtable_ptr->copy_func, other.storage)
    {
      ETL_STATIC_ASSERT(Capacity >= Cap,
                        "internal storage too small");
    }

    template <size_t Cap>
    inplace_function(inplace_function<R(Args...), Cap>&& other) ETL_NOEXCEPT
      : inplace_function(other.vtable_ptr, other.vtable_ptr->move_func, other.storage)
    {
      ETL_STATIC_ASSERT(Capacity >= Cap,
                        "internal storage too small");
    }

    inplace_function(const inplace_function& other)
      : vtable_ptr{other.vtable_ptr}
    {
      vtable_ptr->copy_func(
        storage,
        other.storage);
    }

    inplace_function(inplace_function&& other) ETL_NOEXCEPT
      : vtable_ptr{other.vtable_ptr}
    {
      vtable_ptr->move_func(
        storage,
        other.storage);
    }

    inplace_function& operator=(etl::nullptr_t) ETL_NOEXCEPT
    {
      vtable_ptr->dtor_func(&storage);
      vtable_ptr = ETL_NULLPTR;
      return *this;
    }

    inplace_function& operator=(inplace_function other) ETL_NOEXCEPT
    {
      vtable_ptr->dtor_func(storage);
      vtable_ptr = other.vtable_ptr;
      vtable_ptr->move_func(
        storage,
        other.storage);
      return *this;
    }

    ~inplace_function()
    {
      vtable_ptr->dtor_func(storage);
    }

    R operator()(Args... args) const
    {
      return vtable_ptr->invoke_func(
        storage,
        etl::forward<Args>(args)...);
    }

    explicit ETL_CONSTEXPR operator bool() const ETL_NOEXCEPT
    {
      return vtable_ptr != &private_inplace_function::default_vtable<R, Args...>;
    }

  private:
    vtable_ptr_t vtable_ptr{&private_inplace_function::default_vtable<R, Args...>};
    mutable char storage[Capacity];

    inplace_function(
      vtable_ptr_t                           vtable,
      typename vtable_t::copy_or_move_func_t copy_or_move_func,
      typename vtable_t::storage_ptr_t       storage_ptr)
      : vtable_ptr{vtable}
    {
      copy_or_move_func(storage, storage_ptr);
    }
  };

}  // namespace etl

#endif
