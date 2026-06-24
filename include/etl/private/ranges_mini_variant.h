///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2026 BMW AG

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

#ifndef ETL_RANGES_MINI_VARIANT_INCLUDED
#define ETL_RANGES_MINI_VARIANT_INCLUDED

#include "../platform.h"
#include "../error_handler.h"
#include "../utility.h"

#if ETL_USING_CPP17

namespace etl
{
  namespace ranges
  {
    namespace private_ranges
    {
      //*********************************************************************
      /// mini_variant
      /// A minimal, self-contained variant used internally by ranges,
      /// to prevent cyclic dependencies from algorithm.h and ranges.h.
      //*********************************************************************

      // Helper: get the I-th type from a parameter pack.
      template <size_t I, typename... Ts>
      struct type_at_index;

      template <size_t I, typename Head, typename... Tail>
      struct type_at_index<I, Head, Tail...> : type_at_index<I - 1, Tail...>
      {
      };

      template <typename Head, typename... Tail>
      struct type_at_index<0, Head, Tail...>
      {
        using type = Head;
      };

      template <size_t I, typename... Ts>
      using type_at_index_t = typename type_at_index<I, Ts...>::type;

      // Helper: maximum of sizeof... values
      template <typename... Ts>
      struct max_size;

      template <typename T>
      struct max_size<T>
      {
        static constexpr size_t value = sizeof(T);
      };

      template <typename T, typename... Ts>
      struct max_size<T, Ts...>
      {
        static constexpr size_t value = (sizeof(T) > max_size<Ts...>::value) ? sizeof(T) : max_size<Ts...>::value;
      };

      // Helper: maximum of alignof... values
      template <typename... Ts>
      struct max_align;

      template <typename T>
      struct max_align<T>
      {
        static constexpr size_t value = alignof(T);
      };

      template <typename T, typename... Ts>
      struct max_align<T, Ts...>
      {
        static constexpr size_t value = (alignof(T) > max_align<Ts...>::value) ? alignof(T) : max_align<Ts...>::value;
      };

      // Index value representing "no active alternative"
      inline constexpr size_t mini_variant_npos = ~size_t(0);

      // Detection trait: is a single type equality-comparable?
      template <typename T, typename = void>
      struct is_equality_comparable : etl::false_type
      {
      };

      template <typename T>
      struct is_equality_comparable< T, etl::void_t<decltype(etl::declval<const T&>() == etl::declval<const T&>())>> : etl::true_type
      {
      };

      // Conjunction: all types in the pack are equality-comparable
      template <typename... Ts>
      struct all_equality_comparable : etl::bool_constant<(is_equality_comparable<Ts>::value && ...)>
      {
      };

      // Detection trait: is a single type nothrow-move-constructible?
      template <typename T>
      struct is_nothrow_move_constructible
      {
      private:

        template <typename U>
        static auto test(int) -> etl::bool_constant<noexcept(U(etl::declval<U&&>()))>;

        template <typename>
        static etl::false_type test(...);

      public:

        static constexpr bool value = decltype(test<T>(0))::value;
      };

      // Conjunction: all types in the pack are nothrow-move-constructible
      template <typename... Ts>
      struct all_nothrow_move_constructible : etl::bool_constant<(is_nothrow_move_constructible<Ts>::value && ...)>
      {
      };

      // Detection trait: is a single type nothrow-destructible?
      template <typename T>
      struct is_nothrow_destructible
      {
      private:

        template <typename U>
        static auto test(int) -> etl::bool_constant<noexcept(etl::declval<U&>().~U())>;

        template <typename>
        static etl::false_type test(...);

      public:

        static constexpr bool value = decltype(test<T>(0))::value;
      };

      // Conjunction: all types in the pack are nothrow-destructible
      template <typename... Ts>
      struct all_nothrow_destructible : etl::bool_constant<(is_nothrow_destructible<Ts>::value && ...)>
      {
      };

      template <typename... Ts>
      class mini_variant
      {
        static_assert(sizeof...(Ts) > 0, "mini_variant requires at least one type");

        static constexpr size_t storage_size  = max_size<Ts...>::value;
        static constexpr size_t storage_align = max_align<Ts...>::value;

        alignas(storage_align) unsigned char _storage[storage_size];
        size_t _index;

        // ---- Destruction dispatch table ----
        using destroy_fn = void (*)(void*);

        template <size_t I>
        static void destroy_impl(void* ptr)
        {
          using T = type_at_index_t<I, Ts...>;
          static_cast<T*>(ptr)->~T();
        }

        template <size_t... Is>
        static const destroy_fn* make_destroy_table(etl::index_sequence<Is...>)
        {
          static const destroy_fn table[] = {&destroy_impl<Is>...};
          return table;
        }

        static const destroy_fn* destroy_table()
        {
          static const destroy_fn* t = make_destroy_table(etl::make_index_sequence<sizeof...(Ts)>{});
          return t;
        }

        // ---- Copy dispatch table ----
        using copy_fn = void (*)(void* /*dst*/, const void* /*src*/);

        template <size_t I>
        static void copy_impl(void* dst, const void* src)
        {
          using T = type_at_index_t<I, Ts...>;
          ::new (dst) T(*static_cast<const T*>(src));
        }

        template <size_t... Is>
        static const copy_fn* make_copy_table(etl::index_sequence<Is...>)
        {
          static const copy_fn table[] = {&copy_impl<Is>...};
          return table;
        }

        static const copy_fn* copy_table()
        {
          static const copy_fn* t = make_copy_table(etl::make_index_sequence<sizeof...(Ts)>{});
          return t;
        }

        // ---- Move dispatch table ----
        using move_fn = void (*)(void* /*dst*/, void* /*src*/);

        template <size_t I>
        static void move_impl(void* dst, void* src)
        {
          using T = type_at_index_t<I, Ts...>;
          ::new (dst) T(etl::move(*static_cast<T*>(src)));
        }

        template <size_t... Is>
        static const move_fn* make_move_table(etl::index_sequence<Is...>)
        {
          static const move_fn table[] = {&move_impl<Is>...};
          return table;
        }

        static const move_fn* move_table()
        {
          static const move_fn* t = make_move_table(etl::make_index_sequence<sizeof...(Ts)>{});
          return t;
        }

        // ---- Equality dispatch table ----
        using equal_fn = bool (*)(const void* /*lhs*/, const void* /*rhs*/);

        template <size_t I>
        static bool equal_impl(const void* lhs, const void* rhs)
        {
          using T = type_at_index_t<I, Ts...>;
          return *static_cast<const T*>(lhs) == *static_cast<const T*>(rhs);
        }

        template <size_t... Is>
        static const equal_fn* make_equal_table(etl::index_sequence<Is...>)
        {
          static const equal_fn table[] = {&equal_impl<Is>...};
          return table;
        }

        static const equal_fn* equal_table()
        {
          static const equal_fn* t = make_equal_table(etl::make_index_sequence<sizeof...(Ts)>{});
          return t;
        }

        void destroy_current()
        {
          if (_index != mini_variant_npos)
          {
            destroy_table()[_index](&_storage);
            _index = mini_variant_npos;
          }
        }

      public:

        mini_variant()
          : _index{mini_variant_npos}
        {
        }

        mini_variant(const mini_variant& other)
          : _index{mini_variant_npos}
        {
          if (other._index != mini_variant_npos)
          {
            copy_table()[other._index](&_storage, &other._storage);
            _index = other._index;
          }
        }

        mini_variant& operator=(const mini_variant& other)
        {
          if (this != &other)
          {
            destroy_current();
            if (other._index != mini_variant_npos)
            {
              copy_table()[other._index](&_storage, &other._storage);
              _index = other._index;
            }
          }
          return *this;
        }

        mini_variant(mini_variant&& other) noexcept(all_nothrow_move_constructible<Ts...>::value && all_nothrow_destructible<Ts...>::value)
          : _index{mini_variant_npos}
        {
          if (other._index != mini_variant_npos)
          {
            move_table()[other._index](&_storage, &other._storage);
            _index = other._index;
            other.destroy_current();
          }
        }

        mini_variant& operator=(mini_variant&& other) noexcept(all_nothrow_move_constructible<Ts...>::value && all_nothrow_destructible<Ts...>::value)
        {
          if (this != &other)
          {
            destroy_current();
            if (other._index != mini_variant_npos)
            {
              move_table()[other._index](&_storage, &other._storage);
              _index = other._index;
              other.destroy_current();
            }
          }
          return *this;
        }

        ~mini_variant() noexcept(all_nothrow_destructible<Ts...>::value)
        {
          destroy_current();
        }

        template <size_t I, typename... Args>
        void emplace(Args&&... args)
        {
          static_assert(I < sizeof...(Ts), "Index out of range");
          using T = type_at_index_t<I, Ts...>;
          destroy_current();
          ::new (&_storage) T(etl::forward<Args>(args)...);
          _index = I;
        }

        constexpr size_t index() const
        {
          return _index;
        }

        template <size_t I>
        type_at_index_t<I, Ts...>& get_ref()
        {
          static_assert(I < sizeof...(Ts), "Index out of range");
          ETL_ASSERT(_index == I, ETL_ERROR_GENERIC("mini_variant: bad index"));
          using T = type_at_index_t<I, Ts...>;
          return *reinterpret_cast<T*>(&_storage);
        }

        template <size_t I>
        const type_at_index_t<I, Ts...>& get_ref() const
        {
          static_assert(I < sizeof...(Ts), "Index out of range");
          ETL_ASSERT(_index == I, ETL_ERROR_GENERIC("mini_variant: bad index"));
          using T = type_at_index_t<I, Ts...>;
          return *reinterpret_cast<const T*>(&_storage);
        }

        template <bool B = all_equality_comparable<Ts...>::value, etl::enable_if_t<B, int> = 0>
        friend bool operator==(const mini_variant& lhs, const mini_variant& rhs)
        {
          if (lhs._index != rhs._index)
          {
            return false;
          }
          if (lhs._index == mini_variant_npos)
          {
            return true;
          }
          return equal_table()[lhs._index](&lhs._storage, &rhs._storage);
        }

        template <bool B = all_equality_comparable<Ts...>::value, etl::enable_if_t<B, int> = 0>
        friend bool operator!=(const mini_variant& lhs, const mini_variant& rhs)
        {
          return !(lhs == rhs);
        }
      };
    } // namespace private_ranges

  } // namespace ranges

  template <size_t I, typename... Ts>
  typename ranges::private_ranges::type_at_index_t<I, Ts...>& get(ranges::private_ranges::mini_variant<Ts...>& v)
  {
    return v.template get_ref<I>();
  }

  template <size_t I, typename... Ts>
  const typename ranges::private_ranges::type_at_index_t<I, Ts...>& get(const ranges::private_ranges::mini_variant<Ts...>& v)
  {
    return v.template get_ref<I>();
  }

  template <size_t I, typename... Ts>
  typename ranges::private_ranges::type_at_index_t<I, Ts...>&& get(ranges::private_ranges::mini_variant<Ts...>&& v)
  {
    return etl::move(v.template get_ref<I>());
  }

  template <size_t I, typename... Ts>
  const typename ranges::private_ranges::type_at_index_t<I, Ts...>&& get(const ranges::private_ranges::mini_variant<Ts...>&& v)
  {
    return etl::move(v.template get_ref<I>());
  }

} // namespace etl

#endif // ETL_USING_CPP17
#endif
