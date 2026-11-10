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

#ifndef ETL_PRIVATE_META_GCC_H_INCLUDED
#define ETL_PRIVATE_META_GCC_H_INCLUDED

#include "../../platform.h"
#include "../../initializer_list.h"
#include "../../placement_new.h"
#include "../../string_view.h"
#include "../../utility.h"

///\defgroup meta meta
///\ingroup etl

// ===========================================================================
// Minimal STL shims for ETL_NO_STL mode.
//
// GCC 16's reflection builtins are declared in terms of a handful of types
// that must live in namespace std.  In an ETL_NO_STL build we must NOT pull
// in the corresponding standard library headers, so just enough of each type
// is defined here for consteval reflection use - no heap, no RTTI.
//
// Each shim is skipped when the real header has already been included, which
// is detected with that header's own include guard macro.  This only tells us
// what has been included SO FAR, so the shims must come after every other
// include in this file; they do, the ETL includes above are the only ones.
//
// std::allocator is deliberately named as such: GCC's constexpr evaluator has
// hardcoded knowledge of std::allocator<T>::allocate and only permits an
// allocation to persist across a constant evaluation when it is made through
// that exact name.  A differently named allocator does not work.
//
// std::initializer_list and placement new are not shimmed here: ETL already
// supplies both, for the STL and non-STL configurations alike, via
// initializer_list.h and placement_new.h included above.
// ===========================================================================

// The shims below construct objects in place during constant evaluation, which
// requires the standard placement new from <new>.  ETL's own fallback
// definition in placement_new.h is not usable in a constant expression.
#if !ETL_USING_STD_NEW
  #error etl::meta requires the <new> header - placement new must be usable in a constant expression.
#endif

// --- std::strong_ordering (minimal three-way comparison result) -----------
#if !defined(_COMPARE)
namespace std
{
  struct strong_ordering
  {
    signed char _v;
    constexpr explicit strong_ordering(signed char v) noexcept
      : _v(v)
    {
    }

    static const strong_ordering less;
    static const strong_ordering equal;
    static const strong_ordering equivalent;
    static const strong_ordering greater;

    friend constexpr bool operator==(strong_ordering a, strong_ordering b) noexcept
    {
      return a._v == b._v;
    }
    friend constexpr bool operator==(strong_ordering a, decltype(nullptr)) noexcept
    {
      return a._v == 0;
    }
    friend constexpr bool operator<(strong_ordering a, decltype(nullptr)) noexcept
    {
      return a._v < 0;
    }
    friend constexpr bool operator>(strong_ordering a, decltype(nullptr)) noexcept
    {
      return a._v > 0;
    }
    friend constexpr bool operator<=(strong_ordering a, decltype(nullptr)) noexcept
    {
      return a._v <= 0;
    }
    friend constexpr bool operator>=(strong_ordering a, decltype(nullptr)) noexcept
    {
      return a._v >= 0;
    }
  };
  inline constexpr strong_ordering strong_ordering::less{-1};
  inline constexpr strong_ordering strong_ordering::equal{0};
  inline constexpr strong_ordering strong_ordering::equivalent{0};
  inline constexpr strong_ordering strong_ordering::greater{1};
} // namespace std
#endif // _COMPARE

// --- std::allocator (constexpr allocation recognised by GCC) -------------
// GCC's constexpr evaluator has hardcoded knowledge of std::allocator —
// only allocations through it can persist across constant evaluations.
#if !defined(_GLIBCXX_MEMORY) && !defined(_ALLOCATOR_H) && !defined(_MEMORYFWD_H)
namespace std
{
  template <class T>
  struct allocator
  {
    using value_type = T;
    using size_type  = decltype(sizeof(0));

    constexpr allocator() noexcept = default;

    constexpr T* allocate(size_type n)
    {
      return static_cast<T*>(::operator new(n * sizeof(T)));
    }
    constexpr void deallocate(T* p, size_type)
    {
      ::operator delete(p);
    }
  };
} // namespace std
#endif // _GLIBCXX_MEMORY

// --- std::optional (consteval-only shim) ---------------------------------
#if !defined(_GLIBCXX_OPTIONAL)
namespace std
{
  struct nullopt_t
  {
    constexpr explicit nullopt_t(int) {}
  };
  inline constexpr nullopt_t nullopt{0};

  template <class T>
  class optional
  {
    // A union member, not a byte buffer: reinterpret_cast on raw storage is
    // not allowed in a constant expression, and this shim is consteval-only.
    union storage_type
    {
      struct empty_type
      {
      } _M_empty;
      T _M_value;

      constexpr storage_type() noexcept
        : _M_empty()
      {
      }
      constexpr ~storage_type() {}
    };

    storage_type _M_storage{};
    bool         _M_has = false;

    constexpr T* _M_ptr() noexcept
    {
      return &_M_storage._M_value;
    }
    constexpr const T* _M_ptr() const noexcept
    {
      return &_M_storage._M_value;
    }

    constexpr void _M_construct(const T& v)
    {
      ::new (static_cast<void*>(&_M_storage._M_value)) T(v);
      _M_has = true;
    }
    constexpr void _M_construct(T&& v)
    {
      ::new (static_cast<void*>(&_M_storage._M_value)) T(static_cast<T&&>(v));
      _M_has = true;
    }

  public:

    using value_type = T;

    constexpr optional() noexcept = default;
    constexpr optional(nullopt_t) noexcept {}
    constexpr optional(const T& v)
    {
      _M_construct(v);
    }
    constexpr optional(T&& v)
    {
      _M_construct(static_cast<T&&>(v));
    }

    constexpr optional(const optional& other)
    {
      if (other._M_has)
      {
        _M_construct(*other);
      }
    }

    constexpr optional(optional&& other)
    {
      if (other._M_has)
      {
        _M_construct(static_cast<T&&>(*other._M_ptr()));
      }
    }

    constexpr optional& operator=(const optional& other)
    {
      if (this != &other)
      {
        reset();
        if (other._M_has)
        {
          _M_construct(*other);
        }
      }
      return *this;
    }

    constexpr optional& operator=(optional&& other)
    {
      if (this != &other)
      {
        reset();
        if (other._M_has)
        {
          _M_construct(static_cast<T&&>(*other._M_ptr()));
        }
      }
      return *this;
    }

    constexpr ~optional()
    {
      reset();
    }

    constexpr void reset() noexcept
    {
      if (_M_has)
      {
        _M_ptr()->~T();
        _M_has = false;
      }
    }

    constexpr bool has_value() const noexcept
    {
      return _M_has;
    }
    constexpr explicit operator bool() const noexcept
    {
      return _M_has;
    }
    constexpr T& operator*()
    {
      return *_M_ptr();
    }
    constexpr const T& operator*() const
    {
      return *_M_ptr();
    }
    constexpr T* operator->()
    {
      return _M_ptr();
    }
    constexpr const T* operator->() const
    {
      return _M_ptr();
    }
  };
} // namespace std
#endif // _GLIBCXX_OPTIONAL

// --- std::vector (consteval-capable shim using std::allocator) -----------
#if !defined(_GLIBCXX_VECTOR)
namespace std
{
  template <class T, class Alloc = std::allocator<T>>
  class vector
  {
    Alloc               _M_alloc{};
    T*                  _M_begin = nullptr;
    decltype(sizeof(0)) _M_size  = 0;
    decltype(sizeof(0)) _M_cap   = 0;

    constexpr void _M_reserve_exact(decltype(sizeof(0)) n)
    {
      _M_begin = n ? _M_alloc.allocate(n) : nullptr;
      _M_cap   = n;
      _M_size  = 0;
    }

    constexpr void _M_clear() noexcept
    {
      for (decltype(sizeof(0)) i = 0; i < _M_size; ++i)
      {
        (_M_begin + i)->~T();
      }
      if (_M_begin)
      {
        _M_alloc.deallocate(_M_begin, _M_cap);
      }
      _M_begin = nullptr;
      _M_size  = 0;
      _M_cap   = 0;
    }

  public:

    using value_type     = T;
    using size_type      = decltype(sizeof(0));
    using iterator       = T*;
    using const_iterator = const T*;

    constexpr vector() noexcept = default;
    constexpr vector(std::initializer_list<T> il)
    {
      _M_reserve_exact(il.size());
      for (auto& e : il)
      {
        ::new (static_cast<void*>(_M_begin + _M_size++)) T(e);
      }
    }

    constexpr vector(const vector& other)
    {
      _M_reserve_exact(other._M_size);
      for (size_type i = 0; i < other._M_size; ++i)
      {
        ::new (static_cast<void*>(_M_begin + _M_size++)) T(other._M_begin[i]);
      }
    }

    constexpr vector(vector&& other) noexcept
      : _M_begin(etl::exchange(other._M_begin, nullptr))
      , _M_size(etl::exchange(other._M_size, size_type(0)))
      , _M_cap(etl::exchange(other._M_cap, size_type(0)))
    {
    }

    constexpr vector& operator=(const vector& other)
    {
      if (this != &other)
      {
        _M_clear();
        _M_reserve_exact(other._M_size);
        for (size_type i = 0; i < other._M_size; ++i)
        {
          ::new (static_cast<void*>(_M_begin + _M_size++)) T(other._M_begin[i]);
        }
      }
      return *this;
    }

    constexpr vector& operator=(vector&& other) noexcept
    {
      if (this != &other)
      {
        _M_clear();
        _M_begin = etl::exchange(other._M_begin, nullptr);
        _M_size  = etl::exchange(other._M_size, size_type(0));
        _M_cap   = etl::exchange(other._M_cap, size_type(0));
      }
      return *this;
    }

    constexpr ~vector()
    {
      _M_clear();
    }

    constexpr size_type size() const noexcept
    {
      return _M_size;
    }
    constexpr bool empty() const noexcept
    {
      return _M_size == 0;
    }
    constexpr T* begin() noexcept
    {
      return _M_begin;
    }
    constexpr const T* begin() const noexcept
    {
      return _M_begin;
    }
    constexpr T* end() noexcept
    {
      return _M_begin + _M_size;
    }
    constexpr const T* end() const noexcept
    {
      return _M_begin + _M_size;
    }
    constexpr T& operator[](size_type i)
    {
      return _M_begin[i];
    }
    constexpr const T& operator[](size_type i) const
    {
      return _M_begin[i];
    }
    constexpr void push_back(const T& v)
    {
      if (_M_size == _M_cap)
      {
        size_type nc = _M_cap ? _M_cap * 2 : 4;
        T*        nb = _M_alloc.allocate(nc);
        for (size_type i = 0; i < _M_size; ++i)
        {
          ::new (static_cast<void*>(nb + i)) T(static_cast<T&&>(_M_begin[i]));
          (_M_begin + i)->~T();
        }
        if (_M_begin)
          _M_alloc.deallocate(_M_begin, _M_cap);
        _M_begin = nb;
        _M_cap   = nc;
      }
      ::new (static_cast<void*>(_M_begin + _M_size++)) T(v);
    }
  };
} // namespace std
#endif // _GLIBCXX_VECTOR

// --- std::basic_string / std::string / std::u8string (consteval shim) ----
#if !defined(_GLIBCXX_STRING)
namespace std
{
  template <class CharT>
  class basic_string
  {
    std::allocator<CharT> _M_alloc{};
    CharT*                _M_data = nullptr;
    decltype(sizeof(0))   _M_size = 0;

    constexpr void _M_assign(const CharT* s, decltype(sizeof(0)) n)
    {
      _M_size = n;
      _M_data = _M_alloc.allocate(_M_size + 1);
      for (decltype(sizeof(0)) i = 0; i < _M_size; ++i)
      {
        _M_data[i] = s[i];
      }
      _M_data[_M_size] = CharT();
    }

    constexpr void _M_release() noexcept
    {
      if (_M_data)
      {
        _M_alloc.deallocate(_M_data, _M_size + 1);
      }
      _M_data = nullptr;
      _M_size = 0;
    }

    constexpr void _M_copy_from(const basic_string& other)
    {
      if (other._M_data)
      {
        _M_assign(other._M_data, other._M_size);
      }
    }

  public:

    using value_type = CharT;
    using size_type  = decltype(sizeof(0));

    constexpr basic_string() noexcept = default;

    constexpr basic_string(const CharT* s)
    {
      size_type n = 0;
      while (s[n]) ++n;
      _M_assign(s, n);
    }

    constexpr basic_string(const basic_string& other)
    {
      _M_copy_from(other);
    }

    constexpr basic_string(basic_string&& other) noexcept
      : _M_data(etl::exchange(other._M_data, nullptr))
      , _M_size(etl::exchange(other._M_size, size_type(0)))
    {
    }

    constexpr basic_string& operator=(const basic_string& other)
    {
      if (this != &other)
      {
        _M_release();
        _M_copy_from(other);
      }
      return *this;
    }

    constexpr basic_string& operator=(basic_string&& other) noexcept
    {
      if (this != &other)
      {
        _M_release();
        _M_data = etl::exchange(other._M_data, nullptr);
        _M_size = etl::exchange(other._M_size, size_type(0));
      }
      return *this;
    }

    constexpr ~basic_string()
    {
      _M_release();
    }

    constexpr const CharT* data() const noexcept
    {
      return _M_data;
    }
    constexpr size_type size() const noexcept
    {
      return _M_size;
    }
    constexpr bool empty() const noexcept
    {
      return _M_size == 0;
    }
  };

  using string   = basic_string<char>;
  using u8string = basic_string<char8_t>;
} // namespace std
#endif // _GLIBCXX_STRING

// The u8string typedef is missing when the real <string> predates char8_t.
#if defined(_GLIBCXX_STRING) && !defined(__cpp_lib_char8_t)
namespace std
{
  using u8string = basic_string<char8_t>;
}
#endif

// --- std::constructible_from concept shim --------------------------------
#if !defined(_GLIBCXX_CONCEPTS)
namespace std
{
  template <class T, class... Args>
  concept constructible_from = __is_constructible(T, Args...);
}
#endif // _GLIBCXX_CONCEPTS

// ===========================================================================
// GCC 16 NO_STL reflection implementation.
//
// GCC 16's reflection builtins are only recognised when declared inside
// namespace std::meta.  We declare them there with ETL-friendly return
// types (etl::string_view instead of std::string_view, std::vector<info>
// for list-returning builtins), then using-import every name into
// etl::meta so user code sees etl::meta::foo().
// ===========================================================================

namespace std
{
  namespace meta
  {
    using info = decltype(^^int);

    //*************************************************************************
    /// Simple aggregate that GCC can aggregate-initialize for string results.
    /// etl::string_view has constructors so it's not an aggregate — GCC ICEs.
    /// GCC's builtins aggregate-initialize this as { pointer, length }.
    //*************************************************************************
    struct __string_result
    {
      const char*   _data = nullptr;
      unsigned long _size = 0;
    };

    //*************************************************************************
    /// Access control context.
    //*************************************************************************
    struct access_context
    {
    private:

      consteval access_context(info scope, info dc) noexcept
        : _M_scope{scope}
        , _M_designating_class{dc}
      {
      }

    public:

      access_context()                                = delete;
      consteval access_context(const access_context&) = default;
      consteval access_context(access_context&&)      = default;

      consteval info scope() const
      {
        return _M_scope;
      }
      consteval info designating_class() const
      {
        return _M_designating_class;
      }

      static consteval access_context current() noexcept;
      static consteval access_context unprivileged() noexcept
      {
        return access_context{^^::, info{}};
      }
      static consteval access_context unchecked() noexcept
      {
        return access_context{info{}, info{}};
      }
      consteval access_context via(info cls) const
      {
        return access_context{_M_scope, cls};
      }

      info _M_scope;
      info _M_designating_class;
    };

    //*************************************************************************
    /// Options for data_member_spec (simplified, no std::optional/string).
    //*************************************************************************
    struct data_member_options
    {
      struct _Name
      {
        template <class _Tp>
          requires std::constructible_from<std::u8string, _Tp>
        consteval _Name(_Tp&& __n)
          : _M_is_u8(true)
          , _M_u8s(static_cast<_Tp&&>(__n))
        {
        }

        template <class _Tp>
          requires std::constructible_from<std::string, _Tp>
        consteval _Name(_Tp&& __n)
          : _M_is_u8(false)
          , _M_s(static_cast<_Tp&&>(__n))
        {
        }

      private:

        bool          _M_is_u8;
        std::u8string _M_u8s;
        std::string   _M_s;
        info          _M_unused = {};
      };

      std::optional<_Name> name;
      std::optional<int>   alignment         = {};
      std::optional<int>   bit_width         = {};
      bool                 no_unique_address = false;
      std::vector<info>    annotations       = {};
    };

    // --- Naming & display ------------------------------------------------
    consteval bool            has_identifier(info);
    consteval __string_result identifier_of(info);
    consteval __string_result display_string_of(info);

    //*************************************************************************
    /// Exception type that GCC reflection builtins expect to exist.
    //*************************************************************************
    class exception
    {
    public:

      consteval exception() noexcept
        : _M_what("")
      {
      }
      consteval exception(const char* w) noexcept
        : _M_what(w)
      {
      }
      consteval exception(const exception&) noexcept = default;
      consteval const char* what() const noexcept
      {
        return _M_what;
      }
      const char* _M_what;
    };

    // TODO: query builtins

    // --- Core queries ----------------------------------------------------
    consteval info              type_of(info);
    consteval info              object_of(info);
    consteval info              constant_of(info);
    consteval bool              has_parent(info);
    consteval info              parent_of(info);
    consteval info              dealias(info);
    consteval bool              has_template_arguments(info);
    consteval info              template_of(info);
    consteval std::vector<info> template_arguments_of(info);
    consteval std::vector<info> parameters_of(info);
    consteval info              variable_of(info);
    consteval info              return_type_of(info);

    // TODO: predicate builtins

    // --- Access predicates -----------------------------------------------
    consteval bool is_accessible(info, access_context);
    consteval bool is_public(info);
    consteval bool is_protected(info);
    consteval bool is_private(info);

    // --- Virtual / override / final --------------------------------------
    consteval bool is_virtual(info);
    consteval bool is_pure_virtual(info);
    consteval bool is_override(info);
    consteval bool is_final(info);

    // --- Function / member qualifiers ------------------------------------
    consteval bool is_deleted(info);
    consteval bool is_defaulted(info);
    consteval bool is_explicit(info);
    consteval bool is_noexcept(info);
    consteval bool is_bit_field(info);
    consteval bool is_enumerator(info);
    consteval bool is_const(info);
    consteval bool is_volatile(info);
    consteval bool is_mutable_member(info);
    consteval bool is_lvalue_reference_qualified(info);
    consteval bool is_rvalue_reference_qualified(info);
    consteval bool is_user_declared(info);
    consteval bool is_user_provided(info);

    // --- Storage duration / linkage --------------------------------------
    consteval bool has_static_storage_duration(info);
    consteval bool has_thread_storage_duration(info);
    consteval bool has_automatic_storage_duration(info);
    consteval bool has_internal_linkage(info);
    consteval bool has_module_linkage(info);
    consteval bool has_external_linkage(info);
    consteval bool has_c_language_linkage(info);
    consteval bool has_linkage(info);

    // --- Entity kind predicates ------------------------------------------
    consteval bool is_type(info);
    consteval bool is_namespace(info);
    consteval bool is_namespace_alias(info);
    consteval bool is_function(info);
    consteval bool is_variable(info);
    consteval bool is_type_alias(info);
    consteval bool is_complete_type(info);
    consteval bool is_enumerable_type(info);

    // --- Function kind predicates ----------------------------------------
    consteval bool is_conversion_function(info);
    consteval bool is_operator_function(info);
    consteval bool is_literal_operator(info);
    consteval bool is_special_member_function(info);
    consteval bool is_constructor(info);
    consteval bool is_default_constructor(info);
    consteval bool is_copy_constructor(info);
    consteval bool is_move_constructor(info);
    consteval bool is_assignment(info);
    consteval bool is_copy_assignment(info);
    consteval bool is_move_assignment(info);
    consteval bool is_destructor(info);

    // --- Parameter predicates (P3096) ------------------------------------
    consteval bool is_function_parameter(info);
    consteval bool is_explicit_object_parameter(info);
    consteval bool has_default_argument(info);
    consteval bool has_ellipsis_parameter(info);

    // --- Template predicates ---------------------------------------------
    consteval bool is_template(info);
    consteval bool is_function_template(info);
    consteval bool is_variable_template(info);
    consteval bool is_class_template(info);
    consteval bool is_alias_template(info);
    consteval bool is_conversion_function_template(info);
    consteval bool is_operator_function_template(info);
    consteval bool is_literal_operator_template(info);
    consteval bool is_constructor_template(info);
    consteval bool is_concept(info);

    // --- Value / object / membership predicates --------------------------
    consteval bool is_value(info);
    consteval bool is_object(info);
    consteval bool is_structured_binding(info);
    consteval bool is_class_member(info);
    consteval bool is_namespace_member(info);
    consteval bool is_nonstatic_data_member(info);
    consteval bool is_static_member(info);
    consteval bool is_base(info);
    consteval bool has_default_member_initializer(info);

    // --- Accessibility queries -------------------------------------------
    consteval bool has_inaccessible_nonstatic_data_members(info, access_context);
    consteval bool has_inaccessible_bases(info, access_context);
    consteval bool has_inaccessible_subobjects(info, access_context);

    // TODO: member query builtins

    // --- Member / base / enumerator queries ------------------------------
    consteval std::vector<info> members_of(info, access_context);
    consteval std::vector<info> bases_of(info, access_context);
    consteval std::vector<info> static_data_members_of(info, access_context);
    consteval std::vector<info> nonstatic_data_members_of(info, access_context);
    consteval std::vector<info> subobjects_of(info, access_context);
    consteval std::vector<info> enumerators_of(info);

    // TODO: type-trait mirrors

    // --- Primary type categories -----------------------------------------
    consteval bool is_void_type(info);
    consteval bool is_null_pointer_type(info);
    consteval bool is_integral_type(info);
    consteval bool is_floating_point_type(info);
    consteval bool is_array_type(info);
    consteval bool is_pointer_type(info);
    consteval bool is_lvalue_reference_type(info);
    consteval bool is_rvalue_reference_type(info);
    consteval bool is_member_object_pointer_type(info);
    consteval bool is_member_function_pointer_type(info);
    consteval bool is_enum_type(info);
    consteval bool is_union_type(info);
    consteval bool is_class_type(info);
    consteval bool is_function_type(info);
    consteval bool is_reflection_type(info);

    // --- Composite type categories ---------------------------------------
    consteval bool is_reference_type(info);
    consteval bool is_arithmetic_type(info);
    consteval bool is_fundamental_type(info);
    consteval bool is_object_type(info);
    consteval bool is_scalar_type(info);
    consteval bool is_compound_type(info);
    consteval bool is_member_pointer_type(info);

    // --- Type properties -------------------------------------------------
    consteval bool is_const_type(info);
    consteval bool is_volatile_type(info);
    consteval bool is_trivially_copyable_type(info);
    consteval bool is_standard_layout_type(info);
    consteval bool is_empty_type(info);
    consteval bool is_polymorphic_type(info);
    consteval bool is_abstract_type(info);
    consteval bool is_final_type(info);
    consteval bool is_aggregate_type(info);
    consteval bool is_signed_type(info);
    consteval bool is_unsigned_type(info);
    consteval bool is_bounded_array_type(info);
    consteval bool is_unbounded_array_type(info);
    consteval bool is_scoped_enum_type(info);
    consteval bool is_structural_type(info);

    // --- Constructible / assignable / destructible -----------------------
    template <class Rg = std::initializer_list<info>>
    consteval bool is_constructible_type(info, Rg&&);
    consteval bool is_default_constructible_type(info);
    consteval bool is_copy_constructible_type(info);
    consteval bool is_move_constructible_type(info);
    consteval bool is_assignable_type(info, info);
    consteval bool is_copy_assignable_type(info);
    consteval bool is_move_assignable_type(info);
    consteval bool is_swappable_with_type(info, info);
    consteval bool is_swappable_type(info);
    consteval bool is_destructible_type(info);

    template <class Rg = std::initializer_list<info>>
    consteval bool is_trivially_constructible_type(info, Rg&&);
    consteval bool is_trivially_default_constructible_type(info);
    consteval bool is_trivially_copy_constructible_type(info);
    consteval bool is_trivially_move_constructible_type(info);
    consteval bool is_trivially_assignable_type(info, info);
    consteval bool is_trivially_copy_assignable_type(info);
    consteval bool is_trivially_move_assignable_type(info);
    consteval bool is_trivially_destructible_type(info);

    template <class Rg = std::initializer_list<info>>
    consteval bool is_nothrow_constructible_type(info, Rg&&);
    consteval bool is_nothrow_default_constructible_type(info);
    consteval bool is_nothrow_copy_constructible_type(info);
    consteval bool is_nothrow_move_constructible_type(info);
    consteval bool is_nothrow_assignable_type(info, info);
    consteval bool is_nothrow_copy_assignable_type(info);
    consteval bool is_nothrow_move_assignable_type(info);
    consteval bool is_nothrow_swappable_with_type(info, info);
    consteval bool is_nothrow_swappable_type(info);
    consteval bool is_nothrow_destructible_type(info);

    consteval bool is_implicit_lifetime_type(info);
    consteval bool has_virtual_destructor(info);
    consteval bool has_unique_object_representations(info);

    // --- Property queries ------------------------------------------------
    consteval size_t rank(info);
    consteval size_t extent(info, unsigned = 0);

    // --- Type relations --------------------------------------------------
    consteval bool is_same_type(info, info);
    consteval bool is_base_of_type(info, info);
    consteval bool is_convertible_type(info, info);
    consteval bool is_nothrow_convertible_type(info, info);
    consteval bool is_layout_compatible_type(info, info);

    // TODO: transforms + layout + extract

    // --- Type transformations --------------------------------------------
    consteval info                 add_const(info);
    consteval info                 add_volatile(info);
    consteval info                 add_cv(info);
    consteval info                 remove_const(info);
    consteval info                 remove_volatile(info);
    consteval info                 remove_cv(info);
    consteval info                 add_lvalue_reference(info);
    consteval info                 add_rvalue_reference(info);
    consteval info                 remove_reference(info);
    consteval info                 add_pointer(info);
    consteval info                 remove_pointer(info);
    consteval info                 remove_cvref(info);
    consteval info                 decay(info);
    consteval info                 make_signed(info);
    consteval info                 make_unsigned(info);
    consteval info                 remove_extent(info);
    consteval info                 remove_all_extents(info);
    consteval info                 underlying_type(info);
    consteval info                 unwrap_reference(info);
    consteval info                 unwrap_ref_decay(info);
    consteval std::strong_ordering type_order(info, info);
    template <class Rg = std::initializer_list<info>>
    consteval info common_type(Rg&&);

    // --- Layout queries --------------------------------------------------
    struct member_offset
    {
      ptrdiff_t           bytes;
      ptrdiff_t           bits;
      constexpr ptrdiff_t total_bits() const
      {
        return bytes * __CHAR_BIT__ + bits;
      }
      friend constexpr bool operator==(const member_offset& a, const member_offset& b) noexcept
      {
        return a.bytes == b.bytes && a.bits == b.bits;
      }
      friend constexpr bool operator!=(const member_offset& a, const member_offset& b) noexcept
      {
        return !(a == b);
      }
      friend constexpr bool operator<(const member_offset& a, const member_offset& b) noexcept
      {
        return a.total_bits() < b.total_bits();
      }
    };
    consteval member_offset offset_of(info);
    consteval size_t        size_of(info);
    consteval size_t        alignment_of(info);
    consteval size_t        bit_size_of(info);

    // --- Value extraction / injection ------------------------------------
    template <class T>
    consteval T extract(info);
    template <class T>
    consteval info reflect_constant(T);
    template <class T>
    consteval info reflect_object(T&);
    template <class T>
    consteval info reflect_function(T&);

    // --- Substitution ----------------------------------------------------
    template <class Rg = std::initializer_list<info>>
    consteval bool can_substitute(info, Rg&&);
    template <class Rg = std::initializer_list<info>>
    consteval info substitute(info, Rg&&);

    // --- reflect_invoke --------------------------------------------------
    // Not yet implemented in GCC 16 trunk.

    // --- define_aggregate / data_member_spec -----------------------------
    consteval info data_member_spec(info, data_member_options);
    consteval bool is_data_member_spec(info);
    template <class Rg = std::initializer_list<info>>
    consteval info define_aggregate(info, Rg&&);

    // --- Extra predicates ------------------------------------------------
    // is_structural_type is supported by GCC 16 and is declared with the
    // other type properties above. is_access_specified is not yet
    // implemented in GCC 16.

    // --- Operator queries (declared here so the builtin is in std::meta) -
    enum class operators
    {
      op_new = 1,
      op_delete,
      op_array_new,
      op_array_delete,
      op_co_await,
      op_parentheses,
      op_square_brackets,
      op_arrow,
      op_arrow_star,
      op_tilde,
      op_exclamation,
      op_plus,
      op_minus,
      op_star,
      op_slash,
      op_percent,
      op_caret,
      op_ampersand,
      op_equals,
      op_pipe,
      op_plus_equals,
      op_minus_equals,
      op_star_equals,
      op_slash_equals,
      op_percent_equals,
      op_caret_equals,
      op_ampersand_equals,
      op_pipe_equals,
      op_equals_equals,
      op_exclamation_equals,
      op_less,
      op_greater,
      op_less_equals,
      op_greater_equals,
      op_spaceship,
      op_ampersand_ampersand,
      op_pipe_pipe,
      op_less_less,
      op_greater_greater,
      op_less_less_equals,
      op_greater_greater_equals,
      op_plus_plus,
      op_minus_minus,
      op_comma
    };
    consteval operators       operator_of(info);
    consteval __string_result symbol_of(operators);

  } // namespace meta
} // namespace std

namespace etl
{
  namespace meta
  {
    // Import everything from std::meta into etl::meta
    using namespace std::meta;

    // Also pull in the operators enumerators for unqualified use
    using enum std::meta::operators;

    // Wrap string-returning builtins to return etl::string_view
    // (std::meta versions return __string_result aggregate because
    //  etl::string_view is not an aggregate and GCC ICEs on it)
    namespace private_meta
    {
      //*********************************************************************
      /// GCC initialises the string-returning builtins' result from a single
      /// NUL-terminated 'const char*', exactly as it would initialise a
      /// std::string_view from a character pointer.  Only __string_result's
      /// first member is written, so the length must be recovered here.
      //*********************************************************************
      consteval etl::string_view to_string_view(std::meta::__string_result s)
      {
        if (s._data == nullptr)
        {
          // Avoid pointer arithmetic on nullptr, which is ill-formed in a
          // constant expression.
          return etl::string_view();
        }

        unsigned long length = s._size;

        if (length == 0)
        {
          while (s._data[length] != '\0')
          {
            ++length;
          }
        }

        return etl::string_view(s._data, length);
      }
    } // namespace private_meta

    consteval etl::string_view identifier_of(info r)
    {
      return private_meta::to_string_view(std::meta::identifier_of(r));
    }

    consteval etl::string_view display_string_of(info r)
    {
      return private_meta::to_string_view(std::meta::display_string_of(r));
    }

    consteval etl::string_view symbol_of(operators op)
    {
      return private_meta::to_string_view(std::meta::symbol_of(op));
    }

    //*************************************************************************
    /// info_array — fixed-size, zero-allocation array of info values.
    ///
    /// GCC 16 limitation: template-for over a std::vector<info> fails
    /// because the vector's heap allocation cannot persist in a constant
    /// expression.  The workaround is to copy the elements into a plain
    /// aggregate array (no allocation) and iterate that instead.
    ///
    /// Usage pattern:
    ///   constexpr auto N   = std::meta::members_of(^^T, ctx).size();
    ///   constexpr auto arr = etl::meta::to_info_array<N>(
    ///                            std::meta::members_of(^^T, ctx));
    ///   template for (constexpr auto m : arr) { ... }
    //*************************************************************************
    template <size_t N>
    struct info_array
    {
      info                  elems[N > 0 ? N : 1]{};
      consteval const info* begin() const
      {
        return elems;
      }
      consteval const info* end() const
      {
        return elems + N;
      }
      consteval size_t size() const
      {
        return N;
      }
      consteval const info& operator[](size_t i) const
      {
        return elems[i];
      }
    };

    template <size_t N>
    consteval info_array<N> to_info_array(const std::vector<info>& v)
    {
      info_array<N> a{};
      for (size_t i = 0; i < N; ++i) a.elems[i] = v[i];
      return a;
    }

  } // namespace meta
} // namespace etl

#endif // ETL_PRIVATE_META_GCC_H_INCLUDED
