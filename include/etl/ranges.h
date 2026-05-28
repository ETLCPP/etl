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

#ifndef ETL_RANGES_INCLUDED
#define ETL_RANGES_INCLUDED

#include "platform.h"

#include "error_handler.h"
#include "invoke.h"
#include "iterator.h"
#include "limits.h"
#include "tuple.h"
#include "type_traits.h"
#include "private/ranges_mini_variant.h"

#if ETL_USING_CPP17

namespace etl
{
  namespace ranges
  {
    //*************************************************************************
    /// Range adaptors.
    //*************************************************************************

    namespace private_ranges
    {
      template <typename T, typename Enable = void>
      struct iterator_trait;

      template <typename T>
      struct iterator_trait<T, etl::enable_if_t<etl::is_class_v<T>>>
      {
        using iterator       = typename etl::conditional_t< etl::is_const_v<T>, typename T::const_iterator, typename T::iterator>;
        using const_iterator = typename T::const_iterator;

        using value_type      = typename etl::iterator_traits<const_iterator>::value_type;
        using difference_type = typename etl::iterator_traits<const_iterator>::difference_type;
        using pointer         = typename etl::iterator_traits<const_iterator>::pointer;
        using reference       = typename etl::iterator_traits<const_iterator>::reference;
      };

      template <typename T>
      struct iterator_trait< T, etl::enable_if_t<etl::is_reference_v<T> && !etl::is_array_v<etl::remove_reference_t<T>>>>
      {
        using iterator =
          typename etl::conditional_t< etl::is_const_v<etl::remove_reference_t<T>>, typename etl::remove_reference<T>::type::const_iterator,
                                       typename etl::remove_reference<T>::type::iterator>;
        using const_iterator = typename etl::remove_reference<T>::type::const_iterator;

        using value_type      = typename etl::iterator_traits<iterator>::value_type;
        using difference_type = typename etl::iterator_traits<iterator>::difference_type;
        using pointer         = typename etl::iterator_traits<iterator>::pointer;
        using reference       = typename etl::iterator_traits<iterator>::reference;
      };

      template <typename T>
      struct iterator_trait< T, etl::enable_if_t<etl::is_array_v<etl::remove_reference_t<T>>>>
      {
        using value_type      = typename etl::remove_all_extents<etl::remove_reference_t<T>>::type;
        using iterator        = value_type*;
        using const_iterator  = const value_type*;
        using difference_type = ptrdiff_t;
        using pointer         = const value_type*;
        using reference       = const value_type&;
      };
    } // namespace private_ranges

    template <class D>
    class view_interface
    {
    public:

      view_interface() = default;

      constexpr bool empty() const
      {
        return cbegin() == cend();
      }

      auto cbegin() const
      {
        return static_cast<const D*>(this)->begin();
      }

      auto cend() const
      {
        return static_cast<const D*>(this)->end();
      }

      operator bool() const
      {
        return !empty();
      }

      size_t size() const
      {
        return static_cast<size_t>(etl::distance(cbegin(), cend()));
      }

      constexpr decltype(auto) front()
      {
        return *(static_cast<D*>(this)->begin());
      }

      constexpr decltype(auto) front() const
      {
        return *cbegin();
      }

      template < typename D2                                                                                                         = D,
                 etl::enable_if_t< etl::is_bidirectional_iterator_concept< decltype(etl::declval<const D2&>().begin())>::value, int> = 0>
      constexpr decltype(auto) back()
      {
        return *(static_cast<D*>(this)->end() - 1);
      }

      template < typename D2                                                                                                         = D,
                 etl::enable_if_t< etl::is_bidirectional_iterator_concept< decltype(etl::declval<const D2&>().begin())>::value, int> = 0>
      constexpr decltype(auto) back() const
      {
        return *etl::prev(cend());
      }

      constexpr decltype(auto) operator[](size_t i)
      {
        auto it{static_cast<D*>(this)->begin()};
        etl::advance(it, i);
        return *it;
      }

      template < typename D2 = D, etl::enable_if_t<etl::is_random_access_iterator< decltype(etl::declval<const D2&>().begin())>::value, int> = 0>
      constexpr decltype(auto) operator[](size_t i)
      {
        return static_cast<D*>(this)->begin()[i];
      }

      template < typename D2 = D, etl::enable_if_t<etl::is_random_access_iterator< decltype(etl::declval<const D2&>().begin())>::value, int> = 0>
      constexpr decltype(auto) operator[](size_t i) const
      {
        return cbegin()[i];
      }
    };

    template <class I>
    class range_iterator
    {
    public:

      auto get() const
      {
        return **(static_cast<const I*>(this));
      }
    };

    template <class I, class S = I>
    class subrange : public etl::ranges::view_interface<subrange<I, S>>
    {
    public:

      subrange(I i, S s)
        : _begin{i}
        , _end{s}
      {
      }

      constexpr I begin() const
      {
        return _begin;
      }

      constexpr S end() const
      {
        return _end;
      }

      constexpr subrange& advance(etl::iter_difference_t<I> n)
      {
        etl::advance(_begin, n);
        return *this;
      }

      constexpr subrange prev(etl::iter_difference_t<I> n = 1)
      {
        auto result = subrange{_begin, _end};
        result.advance(-n);
        return result;
      }

      constexpr subrange next(etl::iter_difference_t<I> n = 1)
      {
        auto result = subrange{_begin, _end};
        result.advance(n);
        return result;
      }

    private:

      I _begin;
      S _end;
    };

    template <class I, class S>
    subrange(I, S) -> subrange<I, S>;

    template <class T>
    class empty_view : public etl::ranges::view_interface<empty_view<T>>
    {
    public:

      using iterator = T*;

      constexpr empty_view() = default;

      static constexpr iterator begin() noexcept
      {
        return nullptr;
      }

      static constexpr iterator end() noexcept
      {
        return nullptr;
      }

      static constexpr T* data() noexcept
      {
        return nullptr;
      }

      static constexpr size_t size() noexcept
      {
        return 0;
      }

      static constexpr bool empty() noexcept
      {
        return true;
      }
    };

    struct dangling
    {
      constexpr dangling() noexcept = default;

      template <class... Args>
      constexpr dangling(Args&&...) noexcept
      {
      }
    };

    template <class T>
    constexpr bool enable_borrowed_range = false;

    template <class T>
    struct is_borrowed_range
    {
      static constexpr bool value = etl::is_range_v<T> || etl::ranges::enable_borrowed_range<T>;
    };

    template <typename T>
    inline constexpr bool is_borrowed_range_v = is_borrowed_range<T>::value;

    template <class T>
    using borrowed_iterator_t = etl::conditional_t<etl::ranges::is_borrowed_range_v<T>, etl::ranges::iterator_t<T>, etl::ranges::dangling>;

    template <class T>
    using borrowed_subrange_t =
      etl::conditional_t<etl::ranges::is_borrowed_range_v<T>, etl::ranges::subrange<etl::ranges::iterator_t<T>>, etl::ranges::dangling>;

    namespace views
    {
      template <class T>
      constexpr empty_view<T> empty{};
    }

    template <class T>
    class single_view : public etl::ranges::view_interface<single_view<T>>
    {
    public:

      using value_type     = T;
      using iterator       = value_type*;
      using const_iterator = const value_type*;

      constexpr single_view(const T& t) noexcept
        : _value(t)
      {
      }

      constexpr single_view(T&& t) noexcept
        : _value(etl::move(t))
      {
      }

      constexpr single_view(const single_view<T>& other)
        : _value(other._value)
      {
      }

      constexpr single_view(single_view<T>&& other)
        : _value(etl::move(other._value))
      {
      }

      constexpr single_view& operator=(const single_view<T>& other)
      {
        _value = other._value;
        return *this;
      }

      constexpr single_view& operator=(single_view<T>&& other)
      {
        _value = etl::move(other._value);
        return *this;
      }

      constexpr iterator begin() noexcept
      {
        return data();
      }

      constexpr const_iterator begin() const noexcept
      {
        return data();
      }

      constexpr iterator end() noexcept
      {
        return data() + 1;
      }

      constexpr const_iterator end() const noexcept
      {
        return data() + 1;
      }

      constexpr const T* data() const noexcept
      {
        return &_value;
      }

      constexpr T* data() noexcept
      {
        return &_value;
      }

      constexpr size_t size() const noexcept
      {
        return 1;
      }

      constexpr bool empty() const noexcept
      {
        return false;
      }

    private:

      value_type _value;
    };

    template <class T>
    single_view(T) -> single_view<T>;

    namespace views
    {
      namespace private_views
      {
        struct single
        {
          template <typename T>
          constexpr auto operator()(T&& t) const
          {
            return etl::ranges::single_view(t);
          }
        };
      } // namespace private_views

      inline constexpr private_views::single single;
    } // namespace views

    template <typename T>
    struct iota_iterator : public range_iterator<iota_iterator<T>>
    {
    public:

      using value_type      = T;
      using difference_type = ptrdiff_t;
      using pointer         = T*;
      using reference       = T&;

      using iterator_category = ETL_OR_STD::random_access_iterator_tag;

      constexpr explicit iota_iterator(T i)
        : _i{i}
      {
      }

      constexpr iota_iterator(const iota_iterator& other)
        : _i{other._i}
      {
      }

      iota_iterator& operator++()
      {
        ++_i;
        return *this;
      }

      iota_iterator operator++(int)
      {
        iota_iterator tmp = *this;
        _i++;
        return tmp;
      }

      iota_iterator& operator--()
      {
        --_i;
        return *this;
      }

      iota_iterator operator--(int)
      {
        iota_iterator tmp = *this;
        _i--;
        return tmp;
      }

      iota_iterator& operator+=(difference_type n)
      {
        _i += n;
        return *this;
      }

      iota_iterator operator+(difference_type n) const
      {
        return iota_iterator{static_cast<value_type>(_i + n)};
      }

      iota_iterator operator-(difference_type n) const
      {
        return iota_iterator{static_cast<value_type>(_i - n)};
      }

      difference_type operator-(iota_iterator other) const
      {
        return _i - other._i;
      }

      iota_iterator& operator=(const iota_iterator& other)
      {
        _i = other._i;
        return *this;
      }

      constexpr bool operator==(const iota_iterator& other) const
      {
        return _i == other._i;
      }

      constexpr bool operator!=(const iota_iterator& other) const
      {
        return _i != other._i;
      }

      constexpr value_type operator*() const
      {
        return _i;
      }

      constexpr value_type operator*()
      {
        return _i;
      }

    private:

      value_type _i;
    };

    template <class T>
    class iota_view : public etl::ranges::view_interface<iota_view<T>>
    {
    public:

      using iterator       = iota_iterator<T>;
      using const_iterator = iota_iterator<T>;

      iota_view() = default;

      constexpr explicit iota_view(T value, T bound = etl::numeric_limits<T>::max())
        : _value(value)
        , _bound(bound)
      {
      }

      constexpr iterator begin() const noexcept
      {
        return iterator(_value);
      }

      constexpr iterator end() const noexcept
      {
        return iterator(_bound);
      }

      constexpr size_t size() const noexcept
      {
        if (_bound == etl::numeric_limits<T>::max())
        {
          return etl::numeric_limits<T>::max();
        }
        return static_cast<size_t>(_bound - _value);
      }

      constexpr bool empty() const noexcept
      {
        return _value == _bound;
      }

    private:

      T _value;
      T _bound;
    };

    template <class T>
    iota_view(T) -> iota_view<T>;

    namespace views
    {
      namespace private_views
      {
        struct iota
        {
          template <typename T, typename B>
          constexpr auto operator()(T&& t, B&& b) const
          {
            return etl::ranges::iota_view(t, b);
          }
        };
      } // namespace private_views

      inline constexpr private_views::iota iota;
    } // namespace views

    template <typename T, typename B = T>
    struct repeat_iterator : public range_iterator<repeat_iterator<T, B>>
    {
    public:

      using value_type      = T;
      using difference_type = ptrdiff_t;
      using pointer         = T*;
      using reference       = T&;

      using iterator_category = ETL_OR_STD::random_access_iterator_tag;

      constexpr explicit repeat_iterator(T value, B i = etl::numeric_limits<B>::max())
        : _value{value}
        , _i{i}
      {
      }

      constexpr repeat_iterator(const repeat_iterator& other) = default;

      repeat_iterator& operator++()
      {
        --_i;
        return *this;
      }

      repeat_iterator operator++(int)
      {
        repeat_iterator tmp(*this);
        _i--;
        return tmp;
      }

      repeat_iterator& operator--()
      {
        ++_i;
        return *this;
      }

      repeat_iterator operator--(int)
      {
        repeat_iterator tmp(*this);
        _i++;
        return tmp;
      }

      repeat_iterator& operator+=(difference_type n)
      {
        _i -= static_cast<B>(n);
        return *this;
      }

      repeat_iterator operator+(difference_type n) const
      {
        return repeat_iterator{_value, static_cast<B>(_i - static_cast<B>(n))};
      }

      repeat_iterator operator-(difference_type n) const
      {
        return repeat_iterator{_value, static_cast<B>(_i + static_cast<B>(n))};
      }

      difference_type operator-(repeat_iterator other) const
      {
        return other._i - _i;
      }

      repeat_iterator& operator=(const repeat_iterator& other)
      {
        _i     = other._i;
        _value = other._value;
        return *this;
      }

      constexpr bool operator==(const repeat_iterator& other) const
      {
        return _i == other._i;
      }

      constexpr bool operator!=(const repeat_iterator& other) const
      {
        return _i != other._i;
      }

      constexpr value_type operator*() const
      {
        return _value;
      }

      constexpr value_type operator*()
      {
        return _value;
      }

    private:

      value_type _value;
      B          _i;
    };

    template <class T, class B = T>
    class repeat_view : public etl::ranges::view_interface<repeat_view<T>>
    {
    public:

      using iterator       = repeat_iterator<T, B>;
      using const_iterator = repeat_iterator<T, B>;

      repeat_view() = default;

      constexpr explicit repeat_view(T value, B bound = etl::numeric_limits<B>::max())
        : _value(value)
        , _bound(bound)
      {
      }

      constexpr iterator begin() const noexcept
      {
        return iterator(_value, _bound);
      }

      constexpr iterator end() const noexcept
      {
        return iterator(_value, 0);
      }

      constexpr size_t size() const noexcept
      {
        return static_cast<size_t>(_bound);
      }

      constexpr bool empty() const noexcept
      {
        return _bound == 0;
      }

    private:

      T _value;
      B _bound;
    };

    template <class T, class B = T>
    repeat_view(T, B = B()) -> repeat_view<T, B>;

    namespace views
    {
      namespace private_views
      {
        struct repeat
        {
          template <typename T, typename B>
          constexpr auto operator()(T&& t, B&& b) const
          {
            return etl::ranges::repeat_view(t, b);
          }
        };
      } // namespace private_views

      inline constexpr private_views::repeat repeat;
    } // namespace views

    // Non-template base so ADL finds exactly one operator| definition.
    class range_adapter_closure_base
    {
      // Found via ADL on the RHS (any type that derives from
      // range_adapter_closure_base).  Placing the operator here instead of at
      // global/namespace scope avoids conflicts with std::ranges::operator|.
      template <class Range, class Closure,
                typename = etl::enable_if_t< etl::is_base_of_v<range_adapter_closure_base, etl::decay_t<Closure>>
                                             && etl::is_invocable_v<etl::decay_t<Closure>, Range>>>
      friend auto operator|(Range&& r, Closure&& c)
      {
        return etl::forward<Closure>(c)(etl::forward<Range>(r));
      }
    };

    template <class Derived>
    class range_adapter_closure : public range_adapter_closure_base
    {
    };

    template <class Range>
    class ref_view : public etl::ranges::view_interface<ref_view<Range>>
    {
    public:

      using iterator       = typename etl::ranges::private_ranges::iterator_trait<Range>::iterator;
      using const_iterator = typename etl::ranges::private_ranges::iterator_trait< Range>::const_iterator;
      using pointer        = typename etl::ranges::private_ranges::iterator_trait<Range>::pointer;

      ref_view(Range& r)
        : _r{&r}
      {
      }

      constexpr Range& base() const
      {
        return *_r;
      }

      constexpr iterator begin() const
      {
        return iterator(ETL_OR_STD::begin(*_r));
      }

      constexpr iterator end() const
      {
        return iterator(ETL_OR_STD::end(*_r));
      }

      constexpr bool empty() const
      {
        return begin() == end();
      }

      constexpr size_t size() const
      {
        return static_cast<size_t>(etl::distance(begin(), end()));
      }

      constexpr pointer data() const
      {
        return &(*begin());
      }

    private:

      Range* _r;
    };

    template <class Range>
    ref_view(Range&) -> ref_view<Range>;

    struct ref_range_adapter_closure : public range_adapter_closure<ref_range_adapter_closure>
    {
      template <typename Range>
      using target_view_type = ref_view<Range>;

      ref_range_adapter_closure() = default;

      template <typename Range>
      ref_view<Range> operator()(Range& r)
      {
        return ref_view<Range>(r);
      }
    };

    namespace views
    {
      namespace private_views
      {
        struct ref
        {
          template <class Range>
          constexpr auto operator()(Range& r) const
          {
            return ranges::ref_view(r);
          }

          constexpr auto operator()() const
          {
            return ranges::ref_range_adapter_closure();
          }
        };
      } // namespace private_views

      inline constexpr private_views::ref ref;
    } // namespace views

    template <class Range>
    class owning_view : public etl::ranges::view_interface<owning_view<Range>>
    {
    public:

      using iterator       = typename etl::ranges::private_ranges::iterator_trait<Range>::iterator;
      using const_iterator = typename etl::ranges::private_ranges::iterator_trait< Range>::const_iterator;
      using pointer        = typename etl::ranges::private_ranges::iterator_trait<Range>::pointer;

      owning_view() = default;

      owning_view(owning_view&& other) = default;

      constexpr owning_view(Range&& r)
        : _r(etl::move(r))
      {
      }

      owning_view& operator=(const owning_view&) = delete;

      owning_view& operator=(owning_view&& other)
      {
        _r = etl::move(other._r);
        return *this;
      }

      constexpr Range& base() noexcept
      {
        return _r;
      }

      constexpr iterator begin() const
      {
        return iterator(ETL_OR_STD::begin(_r));
      }

      constexpr iterator end() const
      {
        return iterator(ETL_OR_STD::end(_r));
      }

      constexpr bool empty() const
      {
        return begin() == end();
      }

      constexpr size_t size() const
      {
        return static_cast<size_t>(etl::distance(begin(), end()));
      }

      constexpr pointer data()
      {
        return &(*begin());
      }

    private:

      Range _r;
    };

    template <class Range>
    owning_view(Range&&) -> owning_view<Range>;

    struct owning_range_adapter_closure : public range_adapter_closure<owning_range_adapter_closure>
    {
      template <typename Range, typename CleanRange = etl::remove_reference_t<Range>>
      using target_view_type = owning_view<CleanRange>;

      owning_range_adapter_closure() = default;

      template <typename Range, typename CleanRange = etl::remove_reference_t<Range>>
      owning_view<CleanRange> operator()(Range&& r)
      {
        return owning_view<CleanRange>(etl::move(r));
      }
    };

    namespace views
    {
      namespace private_views
      {
        struct owning
        {
          template <class Range>
          constexpr auto operator()(Range&& r) const
          {
            return ranges::owning_view(etl::forward<Range>(r));
          }

          constexpr auto operator()() const
          {
            return ranges::owning_range_adapter_closure();
          }
        };
      } // namespace private_views

      inline constexpr private_views::owning owning;
    } // namespace views

    namespace views
    {
      namespace private_views
      {
        struct all : public range_adapter_closure_base
        {
          template < class Range,
                     etl::enable_if_t<etl::is_base_of_v< etl::ranges::view_interface<etl::decay_t<Range>>, etl::decay_t<Range>>, int> = 0>
          constexpr etl::decay_t<Range> operator()(Range&& r) const
          {
            return r;
          }

          template < class Range,
                     etl::enable_if_t<!etl::is_base_of_v< etl::ranges::view_interface<etl::decay_t<Range>>, etl::decay_t<Range>>, int> = 0>
          constexpr auto operator()(Range&& r) const
          {
            if constexpr (etl::is_lvalue_reference_v<Range>)
            {
              return etl::ranges::ref_view(etl::forward<Range>(r));
            }
            else
            {
              return etl::ranges::owning_view(etl::forward<Range>(r));
            }
          }
        };
      } // namespace private_views

      inline constexpr private_views::all all;

      template <typename R>
      using all_t = decltype(views::all(etl::declval<R>()));
    } // namespace views

    template <class Range, class Pred>
    class filter_iterator
    {
    public:

      using trait = typename etl::ranges::private_ranges::iterator_trait<Range>;

      using iterator        = typename trait::iterator;
      using const_iterator  = typename trait::const_iterator;
      using value_type      = typename trait::value_type;
      using difference_type = typename trait::difference_type;
      using pointer         = typename trait::pointer;
      using reference       = typename trait::reference;

      using iterator_category = ETL_OR_STD::bidirectional_iterator_tag;

      filter_iterator(const_iterator it, const_iterator it_end, const Pred& p)
        : _it{it}
        , _it_begin{it}
        , _it_end{it_end}
        , _p{p}
      {
        while (_it != _it_end && !_p(*_it))
        {
          ++_it;
        }
      }

      filter_iterator(const filter_iterator& other)
        : _it{other._it}
        , _it_begin{other._it_begin}
        , _it_end{other._it_end}
        , _p{other._p}
      {
        while (_it != _it_end && !_p(*_it))
        {
          ++_it;
        }
      }

      filter_iterator& operator++()
      {
        ++_it;
        while (_it != _it_end && !_p(*_it))
        {
          ++_it;
        }
        return *this;
      }

      filter_iterator operator++(int)
      {
        filter_iterator tmp = *this;

        _it++;
        while (_it != _it_end && !_p(*_it))
        {
          _it++;
        }

        return tmp;
      }

      filter_iterator& operator--()
      {
        --_it;
        while (_it != _it_begin && !_p(*_it))
        {
          --_it;
        }
        return *this;
      }

      filter_iterator operator--(int)
      {
        filter_iterator tmp = *this;

        _it--;
        while (_it != _it_begin && !_p(*_it))
        {
          _it--;
        }

        return tmp;
      }

      filter_iterator& operator+=(size_t n)
      {
        for (size_t i = 0; i < n; i++)
        {
          if (_it != _it_end)
          {
            ++(*this);
          }
        }

        return *this;
      }

      filter_iterator& operator-=(size_t n)
      {
        for (size_t i = 0; i < n; i++)
        {
          if (_it != _it_begin)
          {
            --(*this);
          }
        }

        return *this;
      }

      filter_iterator& operator=(const filter_iterator& other)
      {
        _it       = other._it;
        _it_begin = other._it_begin;
        _it_end   = other._it_end;
        ETL_ASSERT(&_p == &other._p, ETL_ERROR_GENERIC("Predicates need to be the same"));
        return *this;
      }

      value_type operator*()
      {
        return *_it;
      }

      bool operator==(const filter_iterator& other) const
      {
        return other._it == _it;
      }

      bool operator!=(const filter_iterator& other) const
      {
        return !(*this == other);
      }

    private:

      const_iterator _it;
      const_iterator _it_begin;
      const_iterator _it_end;
      const Pred&    _p;
    };

    template <class Range, class Pred>
    constexpr typename filter_iterator<Range, Pred>::difference_type operator-(const filter_iterator<Range, Pred>& lhs,
                                                                               const filter_iterator<Range, Pred>& rhs)
    {
      typename filter_iterator<Range, Pred>::difference_type result{0};
      filter_iterator<Range, Pred>                           it_up{rhs};
      while (it_up != lhs)
      {
        ++it_up;
        ++result;
      }
      return result;
    }

    template <class Range, typename Pred>
    class filter_view : public etl::ranges::view_interface<filter_view<Range, Pred>>
    {
    public:

      using iterator       = filter_iterator<Range, Pred>;
      using const_iterator = filter_iterator<Range, Pred>;

      filter_view(Range&& r, const Pred& pred)
        : _pred{pred}
        , _r{etl::move(r)}
      {
      }

      constexpr Range& base() const&
      {
        return _r;
      }

      constexpr const Pred& pred() const
      {
        return _pred;
      }

      constexpr const_iterator begin() const
      {
        return const_iterator(ETL_OR_STD::cbegin(_r), ETL_OR_STD::cend(_r), _pred);
      }

      constexpr const_iterator end() const
      {
        return const_iterator(ETL_OR_STD::cend(_r), ETL_OR_STD::cend(_r), _pred);
      }

    private:

      const Pred _pred;
      Range      _r;
    };

    template <class Range, typename Pred>
    filter_view(Range&&, Pred) -> filter_view<views::all_t<Range>, Pred>;

    template <typename Pred>
    struct filter_range_adapter_closure : public range_adapter_closure<filter_range_adapter_closure<Pred>>
    {
      template <typename Range>
      using target_view_type = filter_view<Range, Pred>;

      filter_range_adapter_closure(const Pred& p)
        : _p{p}
      {
      }

      template <typename Range>
      constexpr auto operator()(Range&& r)
      {
        return filter_view(views::all(etl::forward<Range>(r)), _p);
      }

      const Pred _p;
    };

    namespace views
    {
      namespace private_views
      {
        struct filter
        {
          template <class Range, typename Pred>
          constexpr auto operator()(Range&& r, const Pred& p) const
          {
            return filter_view(views::all(etl::forward<Range>(r)), p);
          }

          template <typename Pred>
          constexpr auto operator()(const Pred& p) const
          {
            return ranges::filter_range_adapter_closure<Pred>(p);
          }
        };
      } // namespace private_views

      inline constexpr private_views::filter filter;
    } // namespace views

    template <class Range, class Fun>
    class transform_iterator
    {
    public:

      using trait = typename etl::ranges::private_ranges::iterator_trait<Range>;

      using iterator        = typename trait::iterator;
      using const_iterator  = typename trait::const_iterator;
      using value_type      = typename trait::value_type;
      using difference_type = typename trait::difference_type;
      using pointer         = typename trait::pointer;
      using reference       = typename trait::reference;

      using iterator_category = ETL_OR_STD::forward_iterator_tag;

      transform_iterator(const_iterator it, const Fun& f)
        : _it(it)
        , _f(f)
      {
      }

      transform_iterator(const transform_iterator& other)
        : _it{other._it}
        , _f{other._f}
      {
      }

      transform_iterator& operator++()
      {
        ++_it;
        return *this;
      }

      transform_iterator operator++(int)
      {
        transform_iterator tmp = *this;
        _it++;
        return tmp;
      }

      transform_iterator& operator=(const transform_iterator& other)
      {
        _it = other._it;
        ETL_ASSERT(&_f == &other._f, ETL_ERROR_GENERIC("Transform functions need to be the same"));
        return *this;
      }

      value_type operator*()
      {
        return static_cast<value_type>(_f(*_it));
      }

      bool operator==(const transform_iterator& other) const
      {
        return other._it == _it;
      }

      bool operator!=(const transform_iterator& other) const
      {
        return !(*this == other);
      }

    private:

      const_iterator _it;
      const Fun&     _f;
    };

    template <class Range, typename Fun>
    class transform_view : public etl::ranges::view_interface<transform_view<Range, Fun>>
    {
    public:

      using iterator       = transform_iterator<Range, Fun>;
      using const_iterator = transform_iterator<Range, Fun>;

      transform_view(Range&& r, const Fun& fun)
        : _fun{fun}
        , _r{etl::move(r)}
      {
      }

      constexpr Range& base() const&
      {
        return _r;
      }

      constexpr const_iterator begin() const
      {
        return const_iterator(ETL_OR_STD::begin(_r), _fun);
      }

      constexpr const_iterator end() const
      {
        return const_iterator(ETL_OR_STD::end(_r), _fun);
      }

      constexpr size_t size() const
      {
        return static_cast<size_t>(etl::distance(ETL_OR_STD::cbegin(_r), ETL_OR_STD::cend(_r)));
      }

    private:

      const Fun _fun;
      Range     _r;
    };

    template <class Range, typename Fun>
    transform_view(Range&&, Fun) -> transform_view<views::all_t<Range>, Fun>;

    template <typename Fun>
    struct transform_range_adapter_closure : public range_adapter_closure<transform_range_adapter_closure<Fun>>
    {
      template <typename Range>
      using target_view_type = transform_view<Range, Fun>;

      transform_range_adapter_closure(const Fun& f)
        : _f{f}
      {
      }

      template <typename Range>
      constexpr auto operator()(Range&& r)
      {
        return transform_view(views::all(etl::forward<Range>(r)), _f);
      }

      const Fun _f;
    };

    namespace views
    {
      namespace private_views
      {
        struct transform
        {
          template <class Range, typename Fun>
          constexpr auto operator()(Range&& r, const Fun& f) const
          {
            return transform_view(views::all(etl::forward<Range>(r)), f);
          }

          template <typename Fun>
          constexpr auto operator()(const Fun& f) const
          {
            return ranges::transform_range_adapter_closure<Fun>(f);
          }
        };
      } // namespace private_views

      inline constexpr private_views::transform transform;
    } // namespace views

    template <class Range>
    class as_rvalue_view : public etl::ranges::view_interface<as_rvalue_view<Range>>
    {
    public:

      using iterator = typename etl::move_iterator< typename etl::ranges::private_ranges::iterator_trait<Range>::iterator>;

      as_rvalue_view(const as_rvalue_view& other) = default;

      as_rvalue_view(Range&& r)
        : _r{etl::move(r)}
      {
      }

      constexpr Range& base() const
      {
        return _r;
      }

      constexpr iterator begin() const
      {
        return iterator(ETL_OR_STD::begin(_r));
      }

      constexpr iterator end() const
      {
        return iterator(ETL_OR_STD::end(_r));
      }

      constexpr size_t size() const
      {
        return static_cast<size_t>(etl::distance(ETL_OR_STD::cbegin(_r), ETL_OR_STD::cend(_r)));
      }

    private:

      Range _r;
    };

    template <class Range>
    as_rvalue_view(Range&&) -> as_rvalue_view<views::all_t<Range>>;

    struct as_rvalue_range_adapter_closure : public range_adapter_closure<as_rvalue_range_adapter_closure>
    {
      template <typename Range>
      using target_view_type = as_rvalue_view<Range>;

      as_rvalue_range_adapter_closure() = default;

      template <typename Range>
      constexpr auto operator()(Range&& r)
      {
        return as_rvalue_view(views::all(etl::forward<Range>(r)));
      }
    };

    namespace views
    {
      namespace private_views
      {
        struct as_rvalue : public range_adapter_closure_base
        {
          template <class Range>
          constexpr auto operator()(Range&& r) const
          {
            return as_rvalue_view(views::all(etl::forward<Range>(r)));
          }

          constexpr auto operator()() const
          {
            return ranges::as_rvalue_range_adapter_closure();
          }
        };
      } // namespace private_views

      inline constexpr private_views::as_rvalue as_rvalue;
    } // namespace views

    template <class Range>
    class as_const_view : public etl::ranges::view_interface<as_const_view<Range>>
    {
    public:

      using iterator       = typename etl::ranges::private_ranges::iterator_trait< Range>::const_iterator;
      using const_iterator = iterator;

      as_const_view(const as_const_view& other) = default;

      as_const_view(Range&& r)
        : _r{etl::move(r)}
      {
      }

      constexpr Range& base() const
      {
        return _r;
      }

      constexpr iterator begin() const
      {
        return ETL_OR_STD::cbegin(_r);
      }

      constexpr iterator end() const
      {
        return ETL_OR_STD::cend(_r);
      }

      constexpr size_t size() const
      {
        return static_cast<size_t>(etl::distance(ETL_OR_STD::cbegin(_r), ETL_OR_STD::cend(_r)));
      }

    private:

      mutable Range _r;
    };

    template <class Range>
    as_const_view(Range&&) -> as_const_view<views::all_t<Range>>;

    struct as_const_range_adapter_closure : public range_adapter_closure<as_const_range_adapter_closure>
    {
      template <typename Range>
      using target_view_type = as_const_view<Range>;

      as_const_range_adapter_closure() = default;

      template <typename Range>
      constexpr auto operator()(Range&& r)
      {
        return as_const_view(views::all(etl::forward<Range>(r)));
      }
    };

    namespace views
    {
      namespace private_views
      {
        struct as_const : public range_adapter_closure_base
        {
          template <class Range>
          constexpr auto operator()(Range&& r) const
          {
            return as_const_view(views::all(etl::forward<Range>(r)));
          }

          constexpr auto operator()() const
          {
            return ranges::as_const_range_adapter_closure();
          }
        };
      } // namespace private_views

      inline constexpr private_views::as_const as_const;
    } // namespace views

    //*************************************************************************
    /// cache_latest_cache
    /// A lightweight cache for a single value, used internally by
    /// cache_latest_view to avoid depending on etl::optional.
    //*************************************************************************
    namespace private_ranges
    {
      template <typename T>
      struct cache_latest_cache
      {
        cache_latest_cache()
          : _has_value{false}
          , _value{}
        {
        }

        cache_latest_cache(const cache_latest_cache&)            = delete;
        cache_latest_cache& operator=(const cache_latest_cache&) = delete;

        bool has_value() const
        {
          return _has_value;
        }

        void set(const T& v)
        {
          _value     = v;
          _has_value = true;
        }

        void reset()
        {
          _has_value = false;
        }

        T& value()
        {
          return _value;
        }

        bool _has_value;
        T    _value;
      };
    } // namespace private_ranges

    //*************************************************************************
    /// cache_latest_iterator
    //*************************************************************************
    template <class Range>
    class cache_latest_iterator
    {
    public:

      using trait = typename etl::ranges::private_ranges::iterator_trait<Range>;

      using iterator        = typename trait::iterator;
      using const_iterator  = typename trait::const_iterator;
      using value_type      = typename trait::value_type;
      using difference_type = typename trait::difference_type;
      using pointer         = value_type*;
      using reference       = value_type&;

      using iterator_category = ETL_OR_STD::forward_iterator_tag;

      cache_latest_iterator() = default;

      cache_latest_iterator(const_iterator it, private_ranges::cache_latest_cache<value_type>* cache)
        : _it(it)
        , _cache(cache)
      {
      }

      cache_latest_iterator(const cache_latest_iterator& other)
        : _it(other._it)
        , _cache(other._cache)
      {
      }

      cache_latest_iterator& operator++()
      {
        ++_it;
        if (_cache)
        {
          _cache->reset();
        }
        return *this;
      }

      cache_latest_iterator operator++(int)
      {
        cache_latest_iterator tmp = *this;
        ++(*this);
        return tmp;
      }

      cache_latest_iterator& operator=(const cache_latest_iterator& other)
      {
        _it    = other._it;
        _cache = other._cache;
        return *this;
      }

      reference operator*() const
      {
        if (_cache && !_cache->has_value())
        {
          _cache->set(*_it);
        }
        return _cache->value();
      }

      pointer operator->() const
      {
        return &(**this);
      }

      bool operator==(const cache_latest_iterator& other) const
      {
        return other._it == _it;
      }

      bool operator!=(const cache_latest_iterator& other) const
      {
        return !(*this == other);
      }

    private:

      mutable const_iterator                          _it;
      private_ranges::cache_latest_cache<value_type>* _cache;
    };

    //*************************************************************************
    /// cache_latest_view
    /// A range adaptor that caches the most recently accessed element of the
    /// underlying range. Useful when dereferencing the underlying iterator is
    /// expensive and the result is needed more than once.
    //*************************************************************************
    template <class Range>
    class cache_latest_view : public etl::ranges::view_interface<cache_latest_view<Range>>
    {
    public:

      using iterator       = cache_latest_iterator<Range>;
      using const_iterator = cache_latest_iterator<Range>;
      using value_type     = typename etl::ranges::private_ranges::iterator_trait<Range>::value_type;

      cache_latest_view(Range&& r)
        : _r{etl::move(r)}
        , _cache{}
      {
      }

      cache_latest_view(const cache_latest_view& other)
        : _r{other._r}
        , _cache{}
      {
      }

      constexpr Range& base() const&
      {
        return _r;
      }

      constexpr iterator begin() const
      {
        _cache.reset();
        return iterator(ETL_OR_STD::begin(_r), &_cache);
      }

      constexpr iterator end() const
      {
        return iterator(ETL_OR_STD::end(_r), &_cache);
      }

      constexpr size_t size() const
      {
        return static_cast<size_t>(etl::distance(ETL_OR_STD::cbegin(_r), ETL_OR_STD::cend(_r)));
      }

    private:

      mutable Range                                          _r;
      mutable private_ranges::cache_latest_cache<value_type> _cache;
    };

    template <class Range>
    cache_latest_view(Range&&) -> cache_latest_view<views::all_t<Range>>;

    struct cache_latest_range_adapter_closure : public range_adapter_closure<cache_latest_range_adapter_closure>
    {
      template <typename Range>
      using target_view_type = cache_latest_view<Range>;

      cache_latest_range_adapter_closure() = default;

      template <typename Range>
      constexpr auto operator()(Range&& r)
      {
        return cache_latest_view(views::all(etl::forward<Range>(r)));
      }
    };

    namespace views
    {
      namespace private_views
      {
        struct cache_latest : public range_adapter_closure_base
        {
          template <class Range>
          constexpr auto operator()(Range&& r) const
          {
            return cache_latest_view(views::all(etl::forward<Range>(r)));
          }

          constexpr auto operator()() const
          {
            return ranges::cache_latest_range_adapter_closure();
          }
        };
      } // namespace private_views

      inline constexpr private_views::cache_latest cache_latest;
    } // namespace views

    template <class Range>
    class reverse_view : public etl::ranges::view_interface<reverse_view<Range>>
    {
    public:

      using iterator        = ETL_OR_STD::reverse_iterator< typename etl::ranges::private_ranges::iterator_trait<Range>::iterator>;
      using const_iterator  = ETL_OR_STD::reverse_iterator< typename etl::ranges::private_ranges::iterator_trait<Range>::const_iterator>;
      using difference_type = typename etl::ranges::private_ranges::iterator_trait< Range>::difference_type;

      constexpr reverse_view(Range&& r)
        : _r{etl::move(r)}
      {
      }

      reverse_view(const reverse_view& other) = default;

      constexpr Range base() const&
      {
        return _r;
      }

      constexpr iterator begin() const
      {
        return iterator(ETL_OR_STD::end(_r));
      }

      constexpr iterator end() const
      {
        return iterator(ETL_OR_STD::begin(_r));
      }

      constexpr size_t size() const
      {
        return static_cast<size_t>(etl::distance(ETL_OR_STD::begin(_r), ETL_OR_STD::end(_r)));
      }

    private:

      Range _r;
    };

    template <class Range>
    reverse_view(Range&&) -> reverse_view<views::all_t<Range>>;

    template <typename T>
    struct is_reverse_view : etl::false_type
    {
    };

    template <typename Range>
    struct is_reverse_view<reverse_view<Range>> : etl::true_type
    {
    };

    namespace views
    {
      namespace private_views
      {
        struct reverse : public range_adapter_closure_base
        {
          template <class Range>
          constexpr auto operator()(Range&& r) const
          {
            if constexpr (is_reverse_view<etl::remove_cv_t< etl::remove_reference_t<Range>>>::value)
            {
              return r.base();
            }
            else
            {
              return reverse_view(views::all(etl::forward<Range>(r)));
            }
          }
        };
      } // namespace private_views

      inline constexpr private_views::reverse reverse;
    } // namespace views

    template <class Range>
    class drop_view : public etl::ranges::view_interface<drop_view<Range>>
    {
    public:

      using iterator        = typename etl::ranges::private_ranges::iterator_trait<Range>::iterator;
      using const_iterator  = typename etl::ranges::private_ranges::iterator_trait< Range>::const_iterator;
      using difference_type = typename etl::ranges::private_ranges::iterator_trait< Range>::difference_type;

      constexpr drop_view(Range&& r, size_t drop_n)
        : _r{etl::move(r)}
        , _drop_n{drop_n}
        , _begin_cache{ETL_OR_STD::end(_r)}
        , _begin_cache_valid{false}
      {
      }

      drop_view(const drop_view& other) = default;

      constexpr Range base() const&
      {
        return _r;
      }

      constexpr iterator begin() const
      {
        if (!_begin_cache_valid)
        {
          _begin_cache       = drop_begin();
          _begin_cache_valid = true;
        }
        return _begin_cache;
      }

      constexpr iterator end() const
      {
        return iterator(ETL_OR_STD::end(_r));
      }

      constexpr size_t size() const
      {
        if (!_begin_cache_valid)
        {
          _begin_cache       = drop_begin();
          _begin_cache_valid = true;
        }
        return static_cast<size_t>(etl::distance(_begin_cache, ETL_OR_STD::end(_r)));
      }

    private:

      constexpr iterator drop_begin() const
      {
        iterator result{ETL_OR_STD::end(_r)};
        if (static_cast<difference_type>(_drop_n) < etl::distance(ETL_OR_STD::begin(_r), ETL_OR_STD::end(_r)))
        {
          result = ETL_OR_STD::begin(_r);
          etl::advance(result, _drop_n);
        }
        return result;
      }

      Range            _r;
      size_t           _drop_n;
      mutable iterator _begin_cache;
      mutable bool     _begin_cache_valid;
    };

    template <class Range>
    drop_view(Range&&) -> drop_view<views::all_t<Range>>;

    struct drop_range_adapter_closure : public range_adapter_closure<drop_range_adapter_closure>
    {
      template <typename Range>
      using target_view_type = drop_view<Range>;

      constexpr drop_range_adapter_closure(size_t drop_n)
        : _drop_n{drop_n}
      {
      }

      template <typename Range>
      constexpr auto operator()(Range&& r) const
      {
        return drop_view(views::all(etl::forward<Range>(r)), _drop_n);
      }

      const size_t _drop_n;
    };

    namespace views
    {
      namespace private_views
      {
        struct drop
        {
          template <class Range>
          constexpr auto operator()(Range&& r, size_t drop_n) const
          {
            return drop_view(views::all(etl::forward<Range>(r)), drop_n);
          }

          constexpr auto operator()(size_t drop_n) const
          {
            return ranges::drop_range_adapter_closure(drop_n);
          }
        };
      } // namespace private_views

      inline constexpr private_views::drop drop;
    } // namespace views

    template <class Range, class Pred>
    class drop_while_view : public etl::ranges::view_interface<drop_while_view<Range, Pred>>
    {
    public:

      using const_iterator  = typename etl::ranges::private_ranges::iterator_trait< Range>::const_iterator;
      using difference_type = typename etl::ranges::private_ranges::iterator_trait< Range>::difference_type;

      constexpr drop_while_view(Range&& r, Pred pred)
        : _r{etl::move(r)}
        , _pred{pred}
        , _begin_cache{}
        , _begin_cache_valid{false}
      {
      }

      constexpr const Range base() const&
      {
        return _r;
      }

      constexpr const Pred& pred() const
      {
        return _pred;
      }

      constexpr const_iterator begin() const
      {
        if (!_begin_cache_valid)
        {
          const_iterator result{ETL_OR_STD::cbegin(_r)};
          while (result != ETL_OR_STD::cend(_r) && _pred(*result))
          {
            ++result;
          }
          _begin_cache       = result;
          _begin_cache_valid = true;
        }
        return _begin_cache;
      }

      constexpr const_iterator end() const
      {
        return const_iterator(ETL_OR_STD::cend(_r));
      }

    private:

      Range                  _r;
      Pred                   _pred;
      mutable const_iterator _begin_cache;
      mutable bool           _begin_cache_valid;
    };

    template <class Range, class Pred>
    drop_while_view(Range&&, Pred) -> drop_while_view<views::all_t<Range>, Pred>;

    template <typename Pred>
    struct drop_while_range_adapter_closure : public range_adapter_closure<drop_while_range_adapter_closure<Pred>>
    {
      template <typename Range>
      using target_view_type = drop_while_view<Range, Pred>;

      constexpr drop_while_range_adapter_closure(Pred& pred)
        : _pred{pred}
      {
      }

      template <typename Range>
      constexpr auto operator()(Range&& r) const
      {
        return drop_while_view(views::all(etl::forward<Range>(r)), _pred);
      }

      Pred _pred;
    };

    namespace views
    {
      namespace private_views
      {
        struct drop_while
        {
          template <class Range, class Pred>
          constexpr auto operator()(Range&& r, Pred pred) const
          {
            return drop_while_view(views::all(etl::forward<Range>(r)), pred);
          }

          template <class Pred>
          constexpr auto operator()(Pred pred) const
          {
            return ranges::drop_while_range_adapter_closure(pred);
          }
        };
      } // namespace private_views

      inline constexpr private_views::drop_while drop_while;
    } // namespace views

    // Own implementation instead of using etl::min to avoid including
    // algorithm.h
    namespace private_views
    {
      template <typename T>
      constexpr T min(T a, T b)
      {
        return a < b ? a : b;
      }
    } // namespace private_views

    template <class Range>
    class take_view : public etl::ranges::view_interface<take_view<Range>>
    {
    public:

      using iterator        = typename etl::ranges::private_ranges::iterator_trait<Range>::iterator;
      using const_iterator  = typename etl::ranges::private_ranges::iterator_trait< Range>::const_iterator;
      using difference_type = typename etl::ranges::private_ranges::iterator_trait< Range>::difference_type;

      constexpr take_view(Range&& r, ranges::range_difference_t<Range> take_n)
        : _r{etl::move(r)}
        , _take_n{private_views::min<ranges::range_difference_t<Range>>(take_n, etl::distance(ETL_OR_STD::cbegin(r), ETL_OR_STD::cend(r)))}
      {
      }

      take_view(const take_view& other) = default;

      constexpr Range base() const&
      {
        return _r;
      }

      constexpr iterator begin() const
      {
        return iterator(ETL_OR_STD::begin(_r));
      }

      constexpr iterator end() const
      {
        iterator result{begin()};
        etl::advance(result, _take_n);
        return result;
      }

      constexpr ranges::range_difference_t<Range> size() const
      {
        return _take_n;
      }

    private:

      Range                             _r;
      ranges::range_difference_t<Range> _take_n;
    };

    template <class Range>
    take_view(Range&&, ranges::range_difference_t<Range>) -> take_view<views::all_t<Range>>;

    struct take_range_adapter_closure : public range_adapter_closure<take_range_adapter_closure>
    {
      template <typename Range>
      using target_view_type = take_view<Range>;

      template <class DifferenceType>
      constexpr take_range_adapter_closure(DifferenceType take_n)
        : _take_n{static_cast<size_t>(take_n)}
      {
      }

      template <typename Range>
      constexpr auto operator()(Range&& r) const
      {
        return take_view(views::all(etl::forward<Range>(r)), static_cast<ranges::range_difference_t<Range>>(_take_n));
      }

      const size_t _take_n;
    };

    namespace views
    {
      namespace private_views
      {
        struct take
        {
          template <class Range>
          constexpr auto operator()(Range&& r, ranges::range_difference_t<Range> take_n) const
          {
            return take_view(views::all(etl::forward<Range>(r)), take_n);
          }

          template <class DifferenceType>
          constexpr auto operator()(DifferenceType take_n) const
          {
            return ranges::take_range_adapter_closure(take_n);
          }
        };
      } // namespace private_views

      inline constexpr private_views::take take;
    } // namespace views

    template <class Range, class Pred>
    class take_while_view : public etl::ranges::view_interface<take_while_view<Range, Pred>>
    {
    public:

      using iterator        = typename etl::ranges::private_ranges::iterator_trait<Range>::iterator;
      using const_iterator  = typename etl::ranges::private_ranges::iterator_trait< Range>::const_iterator;
      using difference_type = typename etl::ranges::private_ranges::iterator_trait< Range>::difference_type;

      constexpr take_while_view(Range&& r, Pred pred)
        : _r{etl::move(r)}
        , _pred{etl::move(pred)}
        , _end_cache{}
        , _end_cache_valid{false}
      {
      }

      constexpr const Range base() const&
      {
        return _r;
      }

      constexpr const Pred& pred() const
      {
        return _pred;
      }

      constexpr auto begin() const
      {
        return ETL_OR_STD::begin(_r);
      }

      constexpr auto end() const
      {
        if (!_end_cache_valid)
        {
          iterator result{ETL_OR_STD::begin(_r)};
          while (result != ETL_OR_STD::end(_r) && _pred(*result))
          {
            ++result;
          }
          _end_cache       = result;
          _end_cache_valid = true;
        }
        return _end_cache;
      }

    private:

      Range            _r;
      Pred             _pred;
      mutable iterator _end_cache;
      mutable bool     _end_cache_valid;
    };

    template <class Range, class Pred>
    take_while_view(Range&&, Pred) -> take_while_view<views::all_t<Range>, Pred>;

    template <typename Pred>
    struct take_while_range_adapter_closure : public range_adapter_closure<take_while_range_adapter_closure<Pred>>
    {
      template <typename Range>
      using target_view_type = take_while_view<Range, Pred>;

      constexpr take_while_range_adapter_closure(Pred pred)
        : _pred{etl::move(pred)}
      {
      }

      template <typename Range>
      constexpr auto operator()(Range&& r) const
      {
        return take_while_view(views::all(etl::forward<Range>(r)), _pred);
      }

      Pred _pred;
    };

    namespace views
    {
      namespace private_views
      {
        struct take_while
        {
          template <class Range, class Pred>
          constexpr auto operator()(Range&& r, Pred&& pred) const
          {
            return take_while_view(views::all(etl::forward<Range>(r)), etl::forward<Pred>(pred));
          }

          template <class Pred>
          constexpr auto operator()(Pred&& pred) const
          {
            return ranges::take_while_range_adapter_closure(etl::forward<Pred>(pred));
          }
        };
      } // namespace private_views

      inline constexpr private_views::take_while take_while;
    } // namespace views

    template <class Range>
    class join_iterator
    {
    public:

      using trait = typename etl::ranges::private_ranges::iterator_trait<Range>;

      using iterator        = typename trait::iterator;
      using const_iterator  = typename trait::const_iterator;
      using difference_type = typename trait::difference_type;

      using iterator_category = ETL_OR_STD::forward_iterator_tag;

      using InnerRange     = decltype(*(ETL_OR_STD::begin(etl::declval<Range>())));
      using inner_trait    = typename etl::ranges::private_ranges::iterator_trait<InnerRange>;
      using inner_iterator = typename inner_trait::iterator;

      using value_type = typename inner_trait::value_type;
      using pointer    = typename inner_trait::pointer;
      using reference  = typename inner_trait::reference;

      join_iterator(iterator it, iterator it_end)
        : _it(it)
        , _it_end(it_end)
        , _inner_it(it != it_end ? ETL_OR_STD::begin(*it) : inner_iterator{})
        , _inner_it_end(it != it_end ? ETL_OR_STD::end(*it) : inner_iterator{})
      {
        adjust_iterator();
      }

      join_iterator(const join_iterator& other) = default;

      join_iterator& operator++()
      {
        if (_inner_it != _inner_it_end)
        {
          ++_inner_it;
        }

        adjust_iterator();

        return *this;
      }

      join_iterator operator++(int)
      {
        join_iterator tmp{*this};

        if (_inner_it != _inner_it_end)
        {
          _inner_it++;
        }

        adjust_iterator();

        return tmp;
      }

      join_iterator& operator=(const join_iterator& other)
      {
        _it           = other._it;
        _it_end       = other._it_end;
        _inner_it     = other._inner_it;
        _inner_it_end = other._inner_it_end;

        adjust_iterator();

        return *this;
      }

      reference operator*() const
      {
        return *_inner_it;
      }

      constexpr bool operator==(const join_iterator& other) const
      {
        return (_it == other._it && _inner_it == other._inner_it) || (_it == _it_end && other._it == other._it_end);
      }

      constexpr bool operator!=(const join_iterator& other) const
      {
        return !(*this == other);
      }

    private:

      void adjust_iterator()
      {
        while (_it != _it_end && _inner_it == _inner_it_end)
        {
          ++_it;
          if (_it != _it_end)
          {
            _inner_it     = ETL_OR_STD::begin((*_it));
            _inner_it_end = ETL_OR_STD::end((*_it));
          }
        }
      }

      iterator       _it;
      iterator       _it_end;
      inner_iterator _inner_it;
      inner_iterator _inner_it_end;
    };

    template <class Range>
    class join_view : public etl::ranges::view_interface<join_view<Range>>
    {
    public:

      using iterator       = join_iterator<Range>;
      using const_iterator = join_iterator<Range>;

      join_view(Range&& r)
        : _r{etl::move(r)}
      {
      }

      constexpr Range base() const&
      {
        return _r;
      }

      constexpr iterator begin() const
      {
        return iterator(ETL_OR_STD::begin(_r), ETL_OR_STD::end(_r));
      }

      constexpr iterator end() const
      {
        return iterator(ETL_OR_STD::end(_r), ETL_OR_STD::end(_r));
      }

    private:

      Range _r;
    };

    struct join_range_adapter_closure : public range_adapter_closure<join_range_adapter_closure>
    {
      template <typename Range>
      using target_view_type = join_view<Range>;

      join_range_adapter_closure() = default;

      template <typename Range>
      constexpr auto operator()(Range&& r)
      {
        return join_view(views::all(etl::forward<Range>(r)));
      }
    };

    template <class Range>
    explicit join_view(Range&&) -> join_view<views::all_t<Range>>;

    namespace views
    {
      namespace private_views
      {
        struct join : public range_adapter_closure_base
        {
          template <class Range>
          constexpr auto operator()(Range&& r) const
          {
            return join_view(views::all(etl::forward<Range>(r)));
          }

          constexpr auto operator()() const
          {
            return ranges::join_range_adapter_closure();
          }
        };
      } // namespace private_views

      inline constexpr private_views::join join;
    } // namespace views

    template <class Range, class Pattern>
    class join_with_iterator
    {
    public:

      using trait = typename etl::ranges::private_ranges::iterator_trait<Range>;

      using iterator        = typename trait::iterator;
      using const_iterator  = typename trait::const_iterator;
      using difference_type = typename trait::difference_type;

      using iterator_category = ETL_OR_STD::forward_iterator_tag;

      using InnerRange     = decltype(*(ETL_OR_STD::begin(etl::declval<Range>())));
      using inner_trait    = typename etl::ranges::private_ranges::iterator_trait<InnerRange>;
      using inner_iterator = typename inner_trait::iterator;

      using value_type = typename inner_trait::value_type;
      using pointer    = typename inner_trait::pointer;
      using reference  = typename inner_trait::reference;

      using pattern_trait          = typename etl::ranges::private_ranges::iterator_trait<Pattern>;
      using pattern_iterator       = typename pattern_trait::iterator;
      using pattern_const_iterator = typename pattern_trait::const_iterator;

      join_with_iterator(iterator it, iterator it_end, const Pattern& pattern)
        : _it(it)
        , _it_end(it_end)
        , _inner_it(it != it_end ? ETL_OR_STD::begin(*it) : inner_iterator{})
        , _inner_it_end(it != it_end ? ETL_OR_STD::end(*it) : inner_iterator{})
        , _pattern(pattern)
        , _pattern_it(pattern.cend())
        , _pattern_it_end(pattern.cend())
      {
        adjust_iterator();
      }

      join_with_iterator(const join_with_iterator& other) = default;

      join_with_iterator& operator++()
      {
        if (_pattern_it != _pattern_it_end)
        {
          ++_pattern_it;
        }
        else if (_inner_it != _inner_it_end)
        {
          ++_inner_it;
        }

        adjust_iterator();

        return *this;
      }

      join_with_iterator operator++(int)
      {
        join_with_iterator tmp{*this};

        if (_pattern_it != _pattern_it_end)
        {
          _pattern_it++;
        }
        else if (_inner_it != _inner_it_end)
        {
          _inner_it++;
        }

        adjust_iterator();

        return tmp;
      }

      join_with_iterator& operator=(const join_with_iterator& other)
      {
        _it             = other._it;
        _it_end         = other._it_end;
        _inner_it       = other._inner_it;
        _inner_it_end   = other._inner_it_end;
        _pattern_it     = other._pattern_it;
        _pattern_it_end = other._pattern_it_end;

        adjust_iterator();

        return *this;
      }

      value_type operator*() const
      {
        if (_pattern_it != _pattern_it_end)
        {
          return *_pattern_it;
        }
        return *_inner_it;
      }

      constexpr bool operator==(const join_with_iterator& other) const
      {
        return (_it == other._it && _inner_it == other._inner_it && _pattern_it == other._pattern_it) || (_it == _it_end);
      }

      constexpr bool operator!=(const join_with_iterator& other) const
      {
        return !(*this == other);
      }

    private:

      void adjust_iterator()
      {
        if (_it != _it_end && _inner_it == _inner_it_end && _pattern_it == _pattern_it_end)
        {
          ++_it;
          if (_it != _it_end)
          {
            _pattern_it     = ETL_OR_STD::cbegin(_pattern);
            _pattern_it_end = ETL_OR_STD::cend(_pattern);
            _inner_it       = ETL_OR_STD::begin(*_it);
            _inner_it_end   = ETL_OR_STD::end(*_it);
          }
        }
      }

      iterator               _it;
      iterator               _it_end;
      inner_iterator         _inner_it;
      inner_iterator         _inner_it_end;
      const Pattern&         _pattern;
      pattern_const_iterator _pattern_it;
      pattern_const_iterator _pattern_it_end;
    };

    template <class Range, class Pattern>
    class join_with_view : public etl::ranges::view_interface<join_with_view<Range, Pattern>>
    {
    public:

      using iterator       = join_with_iterator<Range, Pattern>;
      using const_iterator = join_with_iterator<Range, Pattern>;

      join_with_view(Range&& r, Pattern&& pattern)
        : _r{etl::move(r)}
        , _pattern{etl::move(pattern)}
      {
      }

      constexpr Range base() const&
      {
        return _r;
      }

      constexpr iterator begin() const
      {
        return iterator(ETL_OR_STD::begin(_r), ETL_OR_STD::end(_r), _pattern);
      }

      constexpr iterator end() const
      {
        return iterator(ETL_OR_STD::end(_r), ETL_OR_STD::end(_r), _pattern);
      }

    private:

      Range   _r;
      Pattern _pattern;
    };

    // For range as separator
    template <class Range, class Pattern>
    join_with_view(Range&&, Pattern&&) -> join_with_view<views::all_t<Range>, views::all_t<Pattern>>;

    // For single value as separator
    template <class Range>
    join_with_view(Range&&, etl::ranges::range_value_t<etl::ranges::range_reference_t<Range>>)
      -> join_with_view<views::all_t<Range>, etl::ranges::single_view<etl::ranges::range_value_t< etl::ranges::range_reference_t<Range>>>>;

    namespace private_ranges
    {
      template <class Pattern>
      constexpr auto make_pattern(Pattern&& pattern)
      {
        if constexpr (etl::is_base_of_v<etl::ranges::view_interface<Pattern>, Pattern>)
        {
          return etl::forward<Pattern>(pattern);
        }
        else
        {
          return etl::ranges::single_view<Pattern>(etl::forward<Pattern>(pattern));
        }
      }

      template <class Pattern>
      constexpr auto make_pattern(const Pattern& pattern)
      {
        if constexpr (etl::is_array_v<etl::remove_reference_t<Pattern>> || etl::is_range_v<etl::remove_reference_t<Pattern>>)
        {
          return views::all(pattern);
        }
        else
        {
          return etl::ranges::single_view<etl::remove_reference_t<Pattern>>(pattern);
        }
      }
    } // namespace private_ranges

    template <class Pattern>
    struct join_with_range_adapter_closure : public range_adapter_closure<join_with_range_adapter_closure<Pattern>>
    {
      template <typename Range>
      using target_view_type = join_with_view<Range, Pattern>;

      join_with_range_adapter_closure(const Pattern& pattern)
        : _pattern(pattern)
      {
      }

      template <typename Range>
      constexpr auto operator()(Range&& r)
      {
        return join_with_view(views::all(etl::forward<Range>(r)), private_ranges::make_pattern<Pattern>(_pattern));
      }

      const Pattern& _pattern;
    };

    namespace views
    {
      namespace private_views
      {
        struct join_with
        {
          template <class Range, class Pattern>
          constexpr auto operator()(Range&& r, Pattern&& pattern) const
          {
            return join_with_view(views::all(etl::forward<Range>(r)),
                                  views::all(etl::ranges::private_ranges::make_pattern<Pattern>(etl::forward<Pattern>(pattern))));
          }

          template <class Pattern>
          constexpr auto operator()(const Pattern& pattern) const
          {
            return ranges::join_with_range_adapter_closure(pattern);
          }
        };
      } // namespace private_views

      inline constexpr private_views::join_with join_with;
    } // namespace views

    template <class Range, class Pattern>
    class split_iterator
    {
    public:

      using trait = typename etl::ranges::private_ranges::iterator_trait<Range>;

      using iterator        = typename trait::iterator;
      using const_iterator  = typename trait::const_iterator;
      using difference_type = typename trait::difference_type;

      using iterator_category = ETL_OR_STD::forward_iterator_tag;

      using pattern_trait          = typename etl::ranges::private_ranges::iterator_trait<Pattern>;
      using pattern_const_iterator = typename pattern_trait::const_iterator;

      using value_type = etl::ranges::subrange<const_iterator>;
      using pointer    = value_type*;
      using reference  = value_type;

      split_iterator(const_iterator it, const_iterator it_end, const Pattern& pattern, bool is_end = false)
        : _it(it)
        , _it_end(it_end)
        , _pattern(pattern)
        , _next(find_next())
        , _trailing_empty(!is_end && _it == _it_end)
      {
      }

      split_iterator(const split_iterator& other) = default;

      split_iterator& operator++()
      {
        _it = _next;

        if (_it != _it_end)
        {
          // Skip past the delimiter
          auto pat_size = etl::distance(ETL_OR_STD::cbegin(_pattern), ETL_OR_STD::cend(_pattern));
          for (difference_type i = 0; i < pat_size && _it != _it_end; ++i)
          {
            ++_it;
          }
          _next = find_next();
          // If we landed exactly at _it_end after skipping the delimiter,
          // there is one more trailing empty segment to emit.
          if (_it == _it_end && !_trailing_empty)
          {
            _trailing_empty = true;
          }
        }
        else
        {
          // We were at _it_end; this was the trailing empty segment.
          _trailing_empty = false;
        }

        return *this;
      }

      split_iterator operator++(int)
      {
        split_iterator tmp{*this};
        ++(*this);
        return tmp;
      }

      split_iterator& operator=(const split_iterator& other)
      {
        _it             = other._it;
        _it_end         = other._it_end;
        _next           = other._next;
        _trailing_empty = other._trailing_empty;
        return *this;
      }

      value_type operator*() const
      {
        return value_type(_it, _next);
      }

      constexpr bool operator==(const split_iterator& other) const
      {
        if (_it == _it_end && other._it == other._it_end)
        {
          return _trailing_empty == other._trailing_empty;
        }
        return _it == other._it;
      }

      constexpr bool operator!=(const split_iterator& other) const
      {
        return !(*this == other);
      }

    private:

      const_iterator find_next() const
      {
        auto pat_begin = ETL_OR_STD::cbegin(_pattern);
        auto pat_end   = ETL_OR_STD::cend(_pattern);
        auto pat_size  = etl::distance(pat_begin, pat_end);

        if (pat_size == 0)
        {
          // Empty pattern: split between each element
          auto result = _it;
          if (result != _it_end)
          {
            ++result;
          }
          return result;
        }

        for (auto search = _it; search != _it_end; ++search)
        {
          // Try to match the full pattern starting at 'search'
          auto s     = search;
          auto p     = pat_begin;
          bool match = true;
          while (p != pat_end)
          {
            if (s == _it_end || !(*s == *p))
            {
              match = false;
              break;
            }
            ++s;
            ++p;
          }
          if (match)
          {
            return search;
          }
        }

        return _it_end;
      }

      const_iterator _it;
      const_iterator _it_end;
      const Pattern& _pattern;
      const_iterator _next;
      // there is still one empty segment to emit after the last delimiter if
      // the last delimiter is at the end of the range
      bool _trailing_empty;
    };

    template <class Range, class Pattern>
    class split_view : public etl::ranges::view_interface<split_view<Range, Pattern>>
    {
    public:

      using iterator       = split_iterator<Range, Pattern>;
      using const_iterator = split_iterator<Range, Pattern>;

      split_view(Range&& r, Pattern&& pattern)
        : _r{etl::move(r)}
        , _pattern{etl::move(pattern)}
      {
      }

      constexpr Range& base() const&
      {
        return _r;
      }

      constexpr const Pattern& pattern() const
      {
        return _pattern;
      }

      constexpr iterator begin() const
      {
        return iterator(ETL_OR_STD::begin(_r), ETL_OR_STD::end(_r), _pattern);
      }

      constexpr iterator end() const
      {
        auto it = iterator(ETL_OR_STD::end(_r), ETL_OR_STD::end(_r), _pattern, true);
        return it;
      }

    private:

      Range   _r;
      Pattern _pattern;
    };

    // For range as delimiter (Pattern is a range, not a single value)
    template <class Range, class Pattern, etl::enable_if_t<etl::is_class_v<etl::decay_t<Pattern>>, int> = 0>
    split_view(Range&&, Pattern&&) -> split_view<views::all_t<Range>, views::all_t<Pattern>>;

    // For single value as delimiter (Pattern is not a range)
    template < class Range, class Pattern, etl::enable_if_t<!etl::is_class_v<etl::decay_t<Pattern>>, int> = 0>
    split_view(Range&&, Pattern&&) -> split_view<views::all_t<Range>, etl::ranges::single_view<etl::decay_t<Pattern>>>;

    template <class Pattern>
    struct split_range_adapter_closure : public range_adapter_closure<split_range_adapter_closure<Pattern>>
    {
      template <typename Range>
      using target_view_type = split_view<Range, Pattern>;

      split_range_adapter_closure(const Pattern& pattern)
        : _pattern(pattern)
      {
      }

      template <typename Range>
      constexpr auto operator()(Range&& r)
      {
        // If Pattern is a range, use views::all. If not, wrap in single_view.
        if constexpr (etl::is_class_v<etl::decay_t<Pattern>>)
        {
          return split_view(views::all(etl::forward<Range>(r)), views::all(_pattern));
        }
        else
        {
          return split_view(views::all(etl::forward<Range>(r)), etl::ranges::single_view(_pattern));
        }
      }

      const Pattern& _pattern;
    };

    namespace views
    {
      namespace private_views
      {
        struct split
        {
          // Range + Pattern (Pattern is a range)
          template < class Range, class Pattern, etl::enable_if_t<etl::is_class_v<etl::decay_t<Pattern>>, int> = 0>
          constexpr auto operator()(Range&& r, Pattern&& pattern) const
          {
            return split_view(views::all(etl::forward<Range>(r)), views::all(etl::forward<Pattern>(pattern)));
          }

          // Range + Pattern (Pattern is a single value)
          template < class Range, class Pattern, etl::enable_if_t<!etl::is_class_v<etl::decay_t<Pattern>>, int> = 0>
          constexpr auto operator()(Range&& r, Pattern&& pattern) const
          {
            return split_view(views::all(etl::forward<Range>(r)), etl::ranges::single_view(pattern));
          }

          // Pipe closure
          template <class Pattern>
          constexpr auto operator()(const Pattern& pattern) const
          {
            return ranges::split_range_adapter_closure<Pattern>(pattern);
          }
        };
      } // namespace private_views

      inline constexpr private_views::split split;
    } // namespace views

    //*************************************************************************
    /// lazy_split_view: lazily splits a range by a pattern.
    /// Unlike split_view (which eagerly computes subrange boundaries),
    /// lazy_split_view yields an inner range whose elements are discovered
    /// one at a time via a dedicated inner iterator. This makes it suitable
    /// for input ranges that do not support multi-pass iteration.
    //*************************************************************************

    template <class Range, class Pattern>
    class lazy_split_view;

    /// Inner range returned by dereferencing a lazy_split_iterator.
    /// Walking this range lazily consumes source elements until the
    /// delimiter pattern is found (or the source is exhausted).
    template <class Range, class Pattern>
    class lazy_split_inner_range
    {
    public:

      using trait               = typename etl::ranges::private_ranges::iterator_trait<Range>;
      using const_iterator_type = typename trait::const_iterator;
      using value_type          = typename trait::value_type;

      using pattern_trait          = typename etl::ranges::private_ranges::iterator_trait<Pattern>;
      using pattern_const_iterator = typename pattern_trait::const_iterator;

      class iterator
      {
      public:

        using value_type        = typename trait::value_type;
        using difference_type   = typename trait::difference_type;
        using pointer           = const value_type*;
        using reference         = const value_type&;
        using iterator_category = ETL_OR_STD::forward_iterator_tag;

        iterator() = default;

        iterator(const_iterator_type current, const_iterator_type segment_end, bool is_end)
          : _current_it(current)
          , _segment_end(segment_end)
          , _is_end(is_end || (current == segment_end))
        {
        }

        reference operator*() const
        {
          return *_current_it;
        }

        pointer operator->() const
        {
          return &(*_current_it);
        }

        iterator& operator++()
        {
          ++_current_it;
          if (_current_it == _segment_end)
          {
            _is_end = true;
          }
          return *this;
        }

        iterator operator++(int)
        {
          iterator tmp{*this};
          ++(*this);
          return tmp;
        }

        constexpr bool operator==(const iterator& other) const
        {
          if (_is_end && other._is_end)
          {
            return true;
          }
          if (_is_end != other._is_end)
          {
            return false;
          }
          return _current_it == other._current_it;
        }

        constexpr bool operator!=(const iterator& other) const
        {
          return !(*this == other);
        }

      private:

        const_iterator_type _current_it{};
        const_iterator_type _segment_end{};
        bool                _is_end = true;
      };

      using const_iterator = iterator;

      lazy_split_inner_range(const_iterator_type segment_begin, const_iterator_type segment_end)
        : _segment_begin(segment_begin)
        , _segment_end(segment_end)
      {
      }

      iterator begin() const
      {
        return iterator(_segment_begin, _segment_end, false);
      }

      iterator end() const
      {
        return iterator(_segment_end, _segment_end, true);
      }

      bool empty() const
      {
        return _segment_begin == _segment_end;
      }

    private:

      const_iterator_type _segment_begin;
      const_iterator_type _segment_end;
    };

    /// Outer iterator for lazy_split_view.
    /// Each dereference yields a lazy_split_inner_range that lazily
    /// walks the current segment up to the next pattern match.
    template <class Range, class Pattern>
    class lazy_split_iterator
    {
    public:

      using trait = typename etl::ranges::private_ranges::iterator_trait<Range>;

      using source_iterator = typename trait::iterator;
      using const_iterator  = typename trait::const_iterator;
      using difference_type = typename trait::difference_type;

      using iterator_category = ETL_OR_STD::forward_iterator_tag;

      using pattern_trait          = typename etl::ranges::private_ranges::iterator_trait<Pattern>;
      using pattern_const_iterator = typename pattern_trait::const_iterator;

      using value_type = lazy_split_inner_range<Range, Pattern>;
      using pointer    = value_type*;
      using reference  = value_type;

      lazy_split_iterator(const_iterator it, const_iterator it_end, const Pattern& pattern, bool is_end = false)
        : _it(it)
        , _it_end(it_end)
        , _pattern(pattern)
        , _next(find_next())
        , _trailing_empty(!is_end && _it == _it_end)
      {
      }

      lazy_split_iterator(const lazy_split_iterator& other) = default;

      lazy_split_iterator& operator=(const lazy_split_iterator& other)
      {
        _it             = other._it;
        _it_end         = other._it_end;
        _next           = other._next;
        _trailing_empty = other._trailing_empty;
        return *this;
      }

      lazy_split_iterator& operator++()
      {
        _it = _next;

        if (_it != _it_end)
        {
          // Skip past the matched delimiter
          auto pat_size = etl::distance(ETL_OR_STD::cbegin(_pattern), ETL_OR_STD::cend(_pattern));
          for (difference_type i = 0; i < pat_size && _it != _it_end; ++i)
          {
            ++_it;
          }
          _next = find_next();
          // If we landed exactly at _it_end after skipping the delimiter,
          // there is one more trailing empty segment to emit.
          if (_it == _it_end && !_trailing_empty)
          {
            _trailing_empty = true;
          }
        }
        else
        {
          // We were at _it_end; this was the trailing empty segment.
          _trailing_empty = false;
        }

        return *this;
      }

      lazy_split_iterator operator++(int)
      {
        lazy_split_iterator tmp{*this};
        ++(*this);
        return tmp;
      }

      /// Returns a lazy inner range — the caller iterates it
      /// element-by-element.
      value_type operator*() const
      {
        return value_type(_it, _next);
      }

      constexpr bool operator==(const lazy_split_iterator& other) const
      {
        if (_it == _it_end && other._it == other._it_end)
        {
          return _trailing_empty == other._trailing_empty;
        }
        return _it == other._it;
      }

      constexpr bool operator!=(const lazy_split_iterator& other) const
      {
        return !(*this == other);
      }

    private:

      /// Scans forward from _it looking for the pattern; returns the
      /// position of the first match (i.e. the end of the current segment).
      const_iterator find_next() const
      {
        auto pat_begin = ETL_OR_STD::cbegin(_pattern);
        auto pat_end   = ETL_OR_STD::cend(_pattern);
        auto pat_size  = etl::distance(pat_begin, pat_end);

        if (pat_size == 0)
        {
          // Empty pattern: split between each element
          auto result = _it;
          if (result != _it_end)
          {
            ++result;
          }
          return result;
        }

        for (auto search = _it; search != _it_end; ++search)
        {
          // Try to match the full pattern starting at 'search'
          auto s     = search;
          auto p     = pat_begin;
          bool match = true;
          while (p != pat_end)
          {
            if (s == _it_end || !(*s == *p))
            {
              match = false;
              break;
            }
            ++s;
            ++p;
          }
          if (match)
          {
            return search;
          }
        }

        return _it_end;
      }

      const_iterator _it;
      const_iterator _it_end;
      const Pattern& _pattern;
      const_iterator _next;
      bool           _trailing_empty;
    };

    template <class Range, class Pattern>
    class lazy_split_view : public etl::ranges::view_interface<lazy_split_view<Range, Pattern>>
    {
    public:

      using iterator       = lazy_split_iterator<Range, Pattern>;
      using const_iterator = lazy_split_iterator<Range, Pattern>;

      lazy_split_view(Range&& r, Pattern&& pattern)
        : _r{etl::move(r)}
        , _pattern{etl::move(pattern)}
      {
      }

      constexpr Range& base() const&
      {
        return _r;
      }

      constexpr const Pattern& pattern() const
      {
        return _pattern;
      }

      constexpr iterator begin() const
      {
        return iterator(ETL_OR_STD::begin(_r), ETL_OR_STD::end(_r), _pattern);
      }

      constexpr iterator end() const
      {
        return iterator(ETL_OR_STD::end(_r), ETL_OR_STD::end(_r), _pattern, true);
      }

    private:

      Range   _r;
      Pattern _pattern;
    };

    // Deduction guide: range delimiter (Pattern is a range)
    template <class Range, class Pattern, etl::enable_if_t<etl::is_class_v<etl::decay_t<Pattern>>, int> = 0>
    lazy_split_view(Range&&, Pattern&&) -> lazy_split_view<views::all_t<Range>, views::all_t<Pattern>>;

    // Deduction guide: single-value delimiter (Pattern is not a range)
    template < class Range, class Pattern, etl::enable_if_t<!etl::is_class_v<etl::decay_t<Pattern>>, int> = 0>
    lazy_split_view(Range&&, Pattern&&) -> lazy_split_view<views::all_t<Range>, etl::ranges::single_view<etl::decay_t<Pattern>>>;

    template <class Pattern>
    struct lazy_split_range_adapter_closure : public range_adapter_closure<lazy_split_range_adapter_closure<Pattern>>
    {
      template <typename Range>
      using target_view_type = lazy_split_view<Range, Pattern>;

      lazy_split_range_adapter_closure(const Pattern& pattern)
        : _pattern(pattern)
      {
      }

      template <typename Range>
      constexpr auto operator()(Range&& r)
      {
        if constexpr (etl::is_class_v<etl::decay_t<Pattern>>)
        {
          return lazy_split_view(views::all(etl::forward<Range>(r)), views::all(_pattern));
        }
        else
        {
          return lazy_split_view(views::all(etl::forward<Range>(r)), etl::ranges::single_view(_pattern));
        }
      }

      const Pattern& _pattern;
    };

    namespace views
    {
      namespace private_views
      {
        struct lazy_split
        {
          // Range + Pattern (Pattern is a range)
          template < class Range, class Pattern, etl::enable_if_t<etl::is_class_v<etl::decay_t<Pattern>>, int> = 0>
          constexpr auto operator()(Range&& r, Pattern&& pattern) const
          {
            return lazy_split_view(views::all(etl::forward<Range>(r)), views::all(etl::forward<Pattern>(pattern)));
          }

          // Range + Pattern (Pattern is a single value)
          template < class Range, class Pattern, etl::enable_if_t<!etl::is_class_v<etl::decay_t<Pattern>>, int> = 0>
          constexpr auto operator()(Range&& r, Pattern&& pattern) const
          {
            return lazy_split_view(views::all(etl::forward<Range>(r)), etl::ranges::single_view(pattern));
          }

          // Pipe closure
          template <class Pattern>
          constexpr auto operator()(const Pattern& pattern) const
          {
            return ranges::lazy_split_range_adapter_closure<Pattern>(pattern);
          }
        };
      } // namespace private_views

      inline constexpr private_views::lazy_split lazy_split;
    } // namespace views

    namespace views
    {
      namespace private_views
      {
        struct counted
        {
          template <class Iterator, class DifferenceType>
          constexpr auto operator()(Iterator&& it, DifferenceType&& count) const
          {
            using T = etl::decay_t<decltype(it)>;
            using D = etl::iter_difference_t<T>;

            // contiguous_iterator_tag not yet available
            // if constexpr(etl::is_same_v<typename
            // etl::iterator_traits<Iterator>::iterator_category,
            // ETL_OR_STD::contiguous_iterator_tag>)
            //{
            //  return etl::span(etl::to_address(it),
            //  static_cast<size_t>(static_cast<etl::iter_difference_t<T>>(count)));
            //}
            // else
            if constexpr (etl::is_same_v< typename etl::iterator_traits< Iterator>::iterator_category, ETL_OR_STD::random_access_iterator_tag>)
            {
              return etl::ranges::subrange(it, it + static_cast<D>(count));
            }
            else
            {
              return etl::ranges::subrange(etl::counted_iterator(it, count), etl::default_sentinel);
            }
          }
        };
      } // namespace private_views

      inline constexpr private_views::counted counted;
    } // namespace views

    template <class... Ranges>
    class concat_view;

    template <class... Ranges>
    class concat_iterator
    {
      static_assert(sizeof...(Ranges) > 0, "Type list must be non-empty");

    public:

      using types           = typename etl::type_list<Ranges...>;
      using first_range     = typename etl::type_list_type_at_index_t<types, 0>;
      using value_type      = typename etl::ranges::private_ranges::iterator_trait< first_range>::value_type;
      using reference       = typename etl::ranges::private_ranges::iterator_trait< first_range>::reference;
      using difference_type = ptrdiff_t;

      using iterator_variant_type = typename concat_view<Ranges...>::iterator_variant_type;

      concat_iterator(size_t index, concat_view<Ranges...>& view, iterator_variant_type current)
        : _ranges_index{index}
        , _view(view)
        , _current_it(current)
      {
      }

      concat_iterator(const concat_iterator& other) = default;

      constexpr reference operator*() const
      {
        return _view.get_value(_ranges_index, _current_it);
      }

      constexpr decltype(auto) operator[](difference_type pos) const
      {
        auto tmp = *this;
        if (pos > 0)
        {
          for (difference_type i = 0; i < pos; ++i)
          {
            tmp._view.advance(tmp._ranges_index, tmp._current_it, 1);
          }
        }
        if (pos < 0)
        {
          for (difference_type i = 0; i < -pos; ++i)
          {
            tmp._view.advance(tmp._ranges_index, tmp._current_it, -1);
          }
        }
        return *tmp;
      }

      constexpr concat_iterator& operator++()
      {
        _view.advance(_ranges_index, _current_it, 1);
        return *this;
      }

      constexpr concat_iterator operator++(int)
      {
        auto result = *this;
        _view.advance(_ranges_index, _current_it, 1);
        return result;
      }

      constexpr concat_iterator& operator--()
      {
        _view.advance(_ranges_index, _current_it, -1);
        return *this;
      }

      constexpr concat_iterator operator--(int)
      {
        auto result = *this;
        _view.advance(_ranges_index, _current_it, -1);
        return result;
      }

      constexpr concat_iterator& operator+=(difference_type n)
      {
        for (difference_type i = 0; i < n; ++i)
        {
          _view.advance(_ranges_index, _current_it, 1);
        }
        return *this;
      }

      constexpr concat_iterator& operator-=(difference_type n)
      {
        for (difference_type i = 0; i < n; ++i)
        {
          _view.advance(_ranges_index, _current_it, -1);
        }
        return *this;
      }

      friend constexpr bool operator==(const concat_iterator<Ranges...>& x, etl::default_sentinel_t)
      {
        return x._ranges_index == x._view.number_of_ranges - 1
               && etl::get<x._view.number_of_ranges - 1>(x._current_it) == etl::get<x._view.number_of_ranges - 1>(x._view).end();
      }

      friend constexpr bool operator==(const concat_iterator<Ranges...>& x, const concat_iterator<Ranges...>& y)
      {
        return x._ranges_index == y._ranges_index && x._current_it.index() == y._current_it.index() && x._current_it == y._current_it;
      }

      friend constexpr bool operator!=(const concat_iterator<Ranges...>& x, etl::default_sentinel_t)
      {
        return !(x == etl::default_sentinel);
      }

      friend constexpr bool operator!=(const concat_iterator<Ranges...>& x, const concat_iterator<Ranges...>& y)
      {
        return !(x == y);
      }

    private:

      size_t                        _ranges_index;
      const concat_view<Ranges...>& _view;
      iterator_variant_type         _current_it;
    };

    template <class... Ranges>
    class concat_view : public etl::ranges::view_interface<concat_view<Ranges...>>
    {
      static_assert(sizeof...(Ranges) > 0, "Type list must be non-empty");

    public:

      using types           = typename etl::type_list<Ranges...>;
      using first_range     = typename etl::type_list_type_at_index_t<types, 0>;
      using value_type      = typename etl::ranges::private_ranges::iterator_trait< first_range>::value_type;
      using reference       = typename etl::ranges::private_ranges::iterator_trait< first_range>::reference;
      using iterator        = concat_iterator<Ranges...>;
      using const_iterator  = concat_iterator<Ranges...>;
      using difference_type = typename etl::make_signed_t<size_t>;

      using iterator_variant_type =
        etl::ranges::private_ranges::mini_variant< typename etl::ranges::private_ranges::iterator_trait< Ranges>::iterator...>;

      using get_value_delegates_type = reference (*)(const iterator_variant_type& /*current*/);
      using advance_delegates_type   = void (*)(size_t& /*index*/, const etl::tuple<Ranges...>& /*r*/, iterator_variant_type& /*current*/,
                                              difference_type /*n*/);

      static constexpr const size_t number_of_ranges = sizeof...(Ranges);

      constexpr concat_view(Ranges&&... r)
        : _r{etl::move(r)...}
      {
        set_delegates();
      }

      concat_view(const concat_view& other) = default;

      constexpr iterator begin()
      {
        iterator_variant_type current;
        current.template emplace<0>(etl::get<0>(_r).begin());
        return iterator{0, *this, current};
      }

      constexpr iterator end()
      {
        iterator_variant_type current;
        current.template emplace<number_of_ranges - 1>(etl::get<number_of_ranges - 1>(_r).end());
        return iterator{number_of_ranges - 1, *this, current};
      }

      constexpr size_t size() const
      {
        return get_size();
      }

    private:

      template <class... Rs>
      friend class concat_iterator;

      template <size_t n = 0>
      constexpr size_t get_size() const
      {
        if constexpr (n < etl::tuple_size_v<decltype(_r)>)
        {
          return etl::get<n>(_r).size() + get_size<n + 1>();
        }
        else
        {
          return 0;
        }
      }

      // helper to advance iterator index+iterator variant
      void advance(size_t& index, iterator_variant_type& current, difference_type n) const
      {
        advance_delegates[index](index, _r, current, n);
      }

      template <size_t i = 0>
      void set_delegates()
      {
        if constexpr (i < number_of_ranges)
        {
          advance_delegates[i] = [](size_t& index, const etl::tuple<Ranges...>& r, iterator_variant_type& current, difference_type n)
          {
            if (n > 0)
            {
              auto  end = etl::get<i>(r).end();
              auto& it  = etl::get<i>(current);
              if (it != end)
              {
                ++it;
              }
              if (it == end)
              {
                if constexpr (i + 1 < number_of_ranges)
                {
                  current.template emplace<i + 1>(etl::get<i + 1>(r).begin());
                  index = i + 1;
                }
                else
                {
                  // at end of last range
                  ETL_ASSERT(it == end && i + 1 == number_of_ranges, ETL_ERROR_GENERIC("Wrong iterator state at end"));
                }
              }
            }
            if (n < 0)
            {
              auto  begin = etl::get<i>(r).begin();
              auto& it    = etl::get<i>(current);
              if (it == begin)
              {
                if constexpr (i > 0)
                {
                  current.template emplace<i - 1>(etl::get<i - 1>(r).end());
                  index = i - 1;

                  auto  begin2 = etl::get<i - 1>(r).begin();
                  auto& it2    = etl::get<i - 1>(current);
                  if (it2 != begin2)
                  {
                    --it2;
                  }
                }
                else
                {
                  // at beginning of first range
                  ETL_ASSERT(it == begin && i == 0, ETL_ERROR_GENERIC("Wrong iterator state at begin"));
                }
              }
              else
              {
                it--;
              }
            }
          };

          get_value_delegates[i] = [](const iterator_variant_type& current) -> reference
          {
            return *etl::get<i>(current);
          };

          set_delegates<i + 1>();
        }
      }

      reference get_value(size_t index, const iterator_variant_type& current) const
      {
        return get_value_delegates[index](current);
      }

      etl::tuple<Ranges...>    _r;
      get_value_delegates_type get_value_delegates[number_of_ranges];
      advance_delegates_type   advance_delegates[number_of_ranges];
    };

    template <class... Ranges>
    concat_view(Ranges&&...) -> concat_view<views::all_t<Ranges>...>;

    namespace views
    {
      namespace private_views
      {
        struct concat
        {
          template <class... Ranges>
          constexpr auto operator()(Ranges&&... r) const
          {
            return concat_view(views::all(etl::forward<Ranges>(r))...);
          }
        };
      } // namespace private_views

      inline constexpr private_views::concat concat;
    } // namespace views

    //*************************************************************************
    /// zip_iterator
    /// An iterator adaptor that iterates over multiple ranges simultaneously,
    /// producing etl::tuple of references to the current elements.
    //*************************************************************************
    template <class... Ranges>
    class zip_iterator
    {
      static_assert(sizeof...(Ranges) > 0, "Type list must be non-empty");

    public:

      using iterators_type  = etl::tuple< typename etl::ranges::private_ranges::iterator_trait< Ranges>::const_iterator...>;
      using value_type      = etl::tuple<typename etl::ranges::private_ranges::iterator_trait< Ranges>::value_type...>;
      using difference_type = ptrdiff_t;
      using pointer         = const value_type*;
      using reference       = value_type;

      using iterator_category = ETL_OR_STD::forward_iterator_tag;

      constexpr zip_iterator(iterators_type iters)
        : _iters(iters)
      {
      }

      constexpr zip_iterator(const zip_iterator& other) = default;

      constexpr zip_iterator& operator=(const zip_iterator& other) = default;

      constexpr zip_iterator& operator++()
      {
        increment(etl::make_index_sequence<sizeof...(Ranges)>{});
        return *this;
      }

      constexpr zip_iterator operator++(int)
      {
        zip_iterator tmp = *this;
        ++(*this);
        return tmp;
      }

      constexpr value_type operator*() const
      {
        return deref(etl::make_index_sequence<sizeof...(Ranges)>{});
      }

      friend constexpr bool operator==(const zip_iterator& lhs, const zip_iterator& rhs)
      {
        return lhs.any_equal(rhs, etl::make_index_sequence<sizeof...(Ranges)>{});
      }

      friend constexpr bool operator!=(const zip_iterator& lhs, const zip_iterator& rhs)
      {
        return !(lhs == rhs);
      }

    private:

      template <size_t... Is>
      constexpr void increment(etl::index_sequence<Is...>)
      {
        ((++etl::get<Is>(_iters)), ...);
      }

      template <size_t... Is>
      constexpr value_type deref(etl::index_sequence<Is...>) const
      {
        return value_type(*etl::get<Is>(_iters)...);
      }

      // zip terminates when ANY iterator reaches its end (shortest range
      // semantics)
      template <size_t... Is>
      constexpr bool any_equal(const zip_iterator& other, etl::index_sequence<Is...>) const
      {
        return ((etl::get<Is>(_iters) == etl::get<Is>(other._iters)) || ...);
      }

      iterators_type _iters;
    };

    //*************************************************************************
    /// zip_view
    /// A range adaptor that takes multiple ranges and produces a view of
    /// tuples, where the i-th tuple contains the i-th elements from all
    /// input ranges. The view has the length of the shortest input range.
    //*************************************************************************
    template <class... Ranges>
    class zip_view : public etl::ranges::view_interface<zip_view<Ranges...>>
    {
      static_assert(sizeof...(Ranges) > 0, "Type list must be non-empty");

    public:

      using iterator       = zip_iterator<Ranges...>;
      using const_iterator = zip_iterator<Ranges...>;

      constexpr zip_view(Ranges&&... r)
        : _r{etl::move(r)...}
      {
      }

      zip_view(const zip_view& other) = default;

      constexpr const_iterator begin() const
      {
        return make_begin(etl::make_index_sequence<sizeof...(Ranges)>{});
      }

      constexpr const_iterator end() const
      {
        return make_end(etl::make_index_sequence<sizeof...(Ranges)>{});
      }

      constexpr size_t size() const
      {
        return get_min_size(etl::make_index_sequence<sizeof...(Ranges)>{});
      }

    private:

      template <size_t... Is>
      constexpr const_iterator make_begin(etl::index_sequence<Is...>) const
      {
        return const_iterator(typename const_iterator::iterators_type(ETL_OR_STD::begin(etl::get<Is>(_r))...));
      }

      template <size_t... Is>
      constexpr const_iterator make_end(etl::index_sequence<Is...>) const
      {
        return const_iterator(typename const_iterator::iterators_type(ETL_OR_STD::end(etl::get<Is>(_r))...));
      }

      template <size_t... Is>
      constexpr size_t get_min_size(etl::index_sequence<Is...>) const
      {
        size_t sizes[] = {static_cast<size_t>(etl::distance(ETL_OR_STD::cbegin(etl::get<Is>(_r)), ETL_OR_STD::cend(etl::get<Is>(_r))))...};
        size_t min_val = sizes[0];
        for (size_t i = 1; i < sizeof...(Ranges); ++i)
        {
          if (sizes[i] < min_val)
          {
            min_val = sizes[i];
          }
        }
        return min_val;
      }

      mutable etl::tuple<Ranges...> _r;
    };

    template <class... Ranges>
    zip_view(Ranges&&...) -> zip_view<views::all_t<Ranges>...>;

    namespace views
    {
      namespace private_views
      {
        struct zip
        {
          template <class... Ranges>
          constexpr auto operator()(Ranges&&... r) const
          {
            return zip_view(views::all(etl::forward<Ranges>(r))...);
          }
        };
      } // namespace private_views

      inline constexpr private_views::zip zip;
    } // namespace views

    //*************************************************************************
    /// zip_transform_iterator
    /// An iterator that zips multiple ranges together and applies a
    /// transformation function to the elements, producing transformed values.
    //*************************************************************************
    template <class Fun, class... Ranges>
    class zip_transform_iterator
    {
      static_assert(sizeof...(Ranges) > 0, "Type list must be non-empty");

    public:

      using iterators_type  = etl::tuple< typename etl::ranges::private_ranges::iterator_trait< Ranges>::const_iterator...>;
      using value_type      = etl::invoke_result_t< Fun, typename etl::ranges::private_ranges::iterator_trait<Ranges>::value_type...>;
      using difference_type = ptrdiff_t;
      using pointer         = const value_type*;
      using reference       = value_type;

      using iterator_category = ETL_OR_STD::forward_iterator_tag;

      constexpr zip_transform_iterator(Fun f, iterators_type iters)
        : _f(f)
        , _iters(iters)
      {
      }

      constexpr zip_transform_iterator(const zip_transform_iterator& other) = default;

      constexpr zip_transform_iterator& operator=(const zip_transform_iterator& other) = default;

      constexpr zip_transform_iterator& operator++()
      {
        increment(etl::make_index_sequence<sizeof...(Ranges)>{});
        return *this;
      }

      constexpr zip_transform_iterator operator++(int)
      {
        zip_transform_iterator tmp = *this;
        ++(*this);
        return tmp;
      }

      constexpr value_type operator*() const
      {
        return deref(etl::make_index_sequence<sizeof...(Ranges)>{});
      }

      friend constexpr bool operator==(const zip_transform_iterator& lhs, const zip_transform_iterator& rhs)
      {
        return lhs.any_equal(rhs, etl::make_index_sequence<sizeof...(Ranges)>{});
      }

      friend constexpr bool operator!=(const zip_transform_iterator& lhs, const zip_transform_iterator& rhs)
      {
        return !(lhs == rhs);
      }

    private:

      template <size_t... Is>
      constexpr void increment(etl::index_sequence<Is...>)
      {
        ((++etl::get<Is>(_iters)), ...);
      }

      template <size_t... Is>
      constexpr value_type deref(etl::index_sequence<Is...>) const
      {
        return etl::invoke(_f, *etl::get<Is>(_iters)...);
      }

      // zip terminates when ANY iterator reaches its end (shortest range
      // semantics)
      template <size_t... Is>
      constexpr bool any_equal(const zip_transform_iterator& other, etl::index_sequence<Is...>) const
      {
        return ((etl::get<Is>(_iters) == etl::get<Is>(other._iters)) || ...);
      }

      Fun            _f;
      iterators_type _iters;
    };

    //*************************************************************************
    /// zip_transform_view
    /// A range adaptor that takes a transformation function and multiple
    /// ranges, and produces a view whose elements are the result of applying
    /// the function to the corresponding elements from all input ranges.
    /// The view has the length of the shortest input range.
    //*************************************************************************
    template <class Fun, class... Ranges>
    class zip_transform_view : public etl::ranges::view_interface<zip_transform_view<Fun, Ranges...>>
    {
      static_assert(sizeof...(Ranges) > 0, "Type list must be non-empty");

    public:

      using iterator       = zip_transform_iterator<Fun, Ranges...>;
      using const_iterator = zip_transform_iterator<Fun, Ranges...>;

      constexpr zip_transform_view(Fun f, Ranges&&... r)
        : _f{f}
        , _r{etl::move(r)...}
      {
      }

      zip_transform_view(const zip_transform_view& other) = default;

      constexpr const_iterator begin() const
      {
        return make_begin(etl::make_index_sequence<sizeof...(Ranges)>{});
      }

      constexpr const_iterator end() const
      {
        return make_end(etl::make_index_sequence<sizeof...(Ranges)>{});
      }

      constexpr size_t size() const
      {
        return get_min_size(etl::make_index_sequence<sizeof...(Ranges)>{});
      }

    private:

      template <size_t... Is>
      constexpr const_iterator make_begin(etl::index_sequence<Is...>) const
      {
        return const_iterator(_f, typename const_iterator::iterators_type(ETL_OR_STD::begin(etl::get<Is>(_r))...));
      }

      template <size_t... Is>
      constexpr const_iterator make_end(etl::index_sequence<Is...>) const
      {
        return const_iterator(_f, typename const_iterator::iterators_type(ETL_OR_STD::end(etl::get<Is>(_r))...));
      }

      template <size_t... Is>
      constexpr size_t get_min_size(etl::index_sequence<Is...>) const
      {
        size_t sizes[] = {static_cast<size_t>(etl::distance(ETL_OR_STD::cbegin(etl::get<Is>(_r)), ETL_OR_STD::cend(etl::get<Is>(_r))))...};
        size_t min_val = sizes[0];
        for (size_t i = 1; i < sizeof...(Ranges); ++i)
        {
          if (sizes[i] < min_val)
          {
            min_val = sizes[i];
          }
        }
        return min_val;
      }

      Fun                           _f;
      mutable etl::tuple<Ranges...> _r;
    };

    template <class Fun, class... Ranges>
    zip_transform_view(Fun, Ranges&&...) -> zip_transform_view<Fun, views::all_t<Ranges>...>;

    namespace views
    {
      namespace private_views
      {
        struct zip_transform
        {
          template <class Fun, class... Ranges>
          constexpr auto operator()(Fun&& f, Ranges&&... r) const
          {
            return zip_transform_view(etl::forward<Fun>(f), views::all(etl::forward<Ranges>(r))...);
          }
        };
      } // namespace private_views

      inline constexpr private_views::zip_transform zip_transform;
    } // namespace views

    //*************************************************************************
    /// common_iterator
    /// A type-erasing iterator that wraps either an iterator I or a sentinel S,
    /// providing a common type for both. Used by common_view.
    //*************************************************************************
    template <class I, class S>
    class common_iterator
    {
    public:

      using value_type        = typename etl::iterator_traits<I>::value_type;
      using difference_type   = typename etl::iterator_traits<I>::difference_type;
      using pointer           = typename etl::iterator_traits<I>::pointer;
      using reference         = typename etl::iterator_traits<I>::reference;
      using iterator_category = ETL_OR_STD::input_iterator_tag;

      constexpr common_iterator()
        : _is_sentinel{false}
        , _it{}
        , _sentinel{}
      {
      }

      constexpr common_iterator(I it)
        : _is_sentinel{false}
        , _it{it}
        , _sentinel{}
      {
      }

      constexpr common_iterator(S s)
        : _is_sentinel{true}
        , _it{}
        , _sentinel{s}
      {
      }

      constexpr common_iterator(const common_iterator& other)
        : _is_sentinel{other._is_sentinel}
        , _it{other._it}
        , _sentinel{other._sentinel}
      {
      }

      constexpr common_iterator& operator=(const common_iterator& other)
      {
        _is_sentinel = other._is_sentinel;
        _it          = other._it;
        _sentinel    = other._sentinel;
        return *this;
      }

      constexpr decltype(auto) operator*() const
      {
        return *_it;
      }

      constexpr decltype(auto) operator*()
      {
        return *_it;
      }

      constexpr auto operator->() const
      {
        return &(*_it);
      }

      constexpr common_iterator& operator++()
      {
        ++_it;
        return *this;
      }

      constexpr common_iterator operator++(int)
      {
        common_iterator tmp = *this;
        ++_it;
        return tmp;
      }

      friend constexpr bool operator==(const common_iterator& lhs, const common_iterator& rhs)
      {
        if (lhs._is_sentinel && rhs._is_sentinel)
        {
          return true;
        }
        if (!lhs._is_sentinel && !rhs._is_sentinel)
        {
          return lhs._it == rhs._it;
        }
        if (lhs._is_sentinel)
        {
          return rhs._it == lhs._sentinel;
        }
        return lhs._it == rhs._sentinel;
      }

      friend constexpr bool operator!=(const common_iterator& lhs, const common_iterator& rhs)
      {
        return !(lhs == rhs);
      }

    private:

      bool _is_sentinel;
      I    _it;
      S    _sentinel;
    };

    //*************************************************************************
    /// Helper to detect if a range is a "common range"
    /// (i.e., begin() and end() return the same type)
    //*************************************************************************
    namespace private_ranges
    {
      template <typename Range, typename = void>
      struct is_common_range : etl::false_type
      {
      };

      template <typename Range>
      struct is_common_range<
        Range,
        etl::enable_if_t< etl::is_same_v< decltype(ETL_OR_STD::begin(etl::declval<Range&>())), decltype(ETL_OR_STD::end(etl::declval<Range&>())) > >>
        : etl::true_type
      {
      };
    } // namespace private_ranges

    //*************************************************************************
    /// common_view
    /// Adapts a view so that its iterator and sentinel types are the same.
    /// If the underlying range is already a common range, acts as a simple
    /// wrapper (pass-through). Otherwise, wraps with common_iterator.
    //*************************************************************************
    template <class Range, bool IsCommon = private_ranges::is_common_range<Range>::value>
    class common_view;

    // Specialization for ranges that are already common (begin/end same type)
    template <class Range>
    class common_view<Range, true> : public etl::ranges::view_interface<common_view<Range, true>>
    {
    public:

      using iterator        = decltype(ETL_OR_STD::begin(etl::declval<Range&>()));
      using const_iterator  = decltype(ETL_OR_STD::cbegin(etl::declval<const Range&>()));
      using difference_type = typename etl::iterator_traits<iterator>::difference_type;

      constexpr common_view(Range&& r)
        : _r{etl::move(r)}
      {
      }

      common_view(const common_view& other) = default;

      constexpr Range base() const&
      {
        return _r;
      }

      constexpr iterator begin() const
      {
        return iterator(ETL_OR_STD::begin(_r));
      }

      constexpr iterator end() const
      {
        return iterator(ETL_OR_STD::end(_r));
      }

      constexpr size_t size() const
      {
        return static_cast<size_t>(etl::distance(ETL_OR_STD::begin(_r), ETL_OR_STD::end(_r)));
      }

    private:

      Range _r;
    };

    // Specialization for ranges that are NOT common (begin/end differ)
    template <class Range>
    class common_view<Range, false> : public etl::ranges::view_interface<common_view<Range, false>>
    {
    public:

      using base_iterator   = decltype(ETL_OR_STD::begin(etl::declval<Range&>()));
      using base_sentinel   = decltype(ETL_OR_STD::end(etl::declval<Range&>()));
      using iterator        = common_iterator<base_iterator, base_sentinel>;
      using const_iterator  = iterator;
      using difference_type = typename etl::iterator_traits<base_iterator>::difference_type;

      constexpr common_view(Range&& r)
        : _r{etl::move(r)}
      {
      }

      common_view(const common_view& other) = default;

      constexpr Range base() const&
      {
        return _r;
      }

      constexpr iterator begin() const
      {
        return iterator(ETL_OR_STD::begin(_r));
      }

      constexpr iterator end() const
      {
        return iterator(ETL_OR_STD::end(_r));
      }

    private:

      Range _r;
    };

    template <class Range>
    common_view(Range&&) -> common_view<views::all_t<Range>>;

    struct common_range_adapter_closure : public range_adapter_closure<common_range_adapter_closure>
    {
      template <typename Range>
      using target_view_type = common_view<Range>;

      common_range_adapter_closure() = default;

      template <typename Range>
      constexpr auto operator()(Range&& r)
      {
        return common_view(views::all(etl::forward<Range>(r)));
      }
    };

    namespace views
    {
      namespace private_views
      {
        struct common : public range_adapter_closure_base
        {
          template <class Range>
          constexpr auto operator()(Range&& r) const
          {
            return common_view(views::all(etl::forward<Range>(r)));
          }

          constexpr auto operator()() const
          {
            return ranges::common_range_adapter_closure();
          }
        };
      } // namespace private_views

      inline constexpr private_views::common common;
    } // namespace views

    //*************************************************************************
    /// enumerate_iterator
    /// An iterator adaptor that pairs each element of the underlying range
    /// with its index, producing etl::tuple<size_t, value_type>.
    //*************************************************************************
    template <class Range>
    class enumerate_iterator
    {
    public:

      using trait = typename etl::ranges::private_ranges::iterator_trait<Range>;

      using base_iterator   = typename trait::const_iterator;
      using base_value_type = typename trait::value_type;
      using value_type      = etl::tuple<size_t, base_value_type>;
      using difference_type = typename trait::difference_type;
      using pointer         = const value_type*;
      using reference       = value_type;

      using iterator_category = ETL_OR_STD::forward_iterator_tag;

      enumerate_iterator(base_iterator it, size_t index)
        : _it(it)
        , _index(index)
      {
      }

      enumerate_iterator(const enumerate_iterator& other)
        : _it{other._it}
        , _index{other._index}
      {
      }

      enumerate_iterator& operator++()
      {
        ++_it;
        ++_index;
        return *this;
      }

      enumerate_iterator operator++(int)
      {
        enumerate_iterator tmp = *this;
        ++_it;
        ++_index;
        return tmp;
      }

      enumerate_iterator& operator=(const enumerate_iterator& other)
      {
        _it    = other._it;
        _index = other._index;
        return *this;
      }

      value_type operator*() const
      {
        return value_type(_index, *_it);
      }

      bool operator==(const enumerate_iterator& other) const
      {
        return other._it == _it;
      }

      bool operator!=(const enumerate_iterator& other) const
      {
        return !(*this == other);
      }

    private:

      base_iterator _it;
      size_t        _index;
    };

    //*************************************************************************
    /// enumerate_view
    /// A range adaptor that pairs each element of the underlying range
    /// with its index, producing a view of etl::tuple<size_t, value_type>.
    //*************************************************************************
    template <class Range>
    class enumerate_view : public etl::ranges::view_interface<enumerate_view<Range>>
    {
    public:

      using iterator       = enumerate_iterator<Range>;
      using const_iterator = enumerate_iterator<Range>;

      enumerate_view(Range&& r)
        : _r{etl::move(r)}
      {
      }

      enumerate_view(const enumerate_view& other) = default;

      constexpr Range& base() const&
      {
        return _r;
      }

      constexpr const_iterator begin() const
      {
        return const_iterator(ETL_OR_STD::begin(_r), 0);
      }

      constexpr const_iterator end() const
      {
        return const_iterator(ETL_OR_STD::end(_r), static_cast<size_t>(etl::distance(ETL_OR_STD::cbegin(_r), ETL_OR_STD::cend(_r))));
      }

      constexpr size_t size() const
      {
        return static_cast<size_t>(etl::distance(ETL_OR_STD::cbegin(_r), ETL_OR_STD::cend(_r)));
      }

    private:

      mutable Range _r;
    };

    template <class Range>
    enumerate_view(Range&&) -> enumerate_view<views::all_t<Range>>;

    struct enumerate_range_adapter_closure : public range_adapter_closure<enumerate_range_adapter_closure>
    {
      template <typename Range>
      using target_view_type = enumerate_view<Range>;

      enumerate_range_adapter_closure() = default;

      template <typename Range>
      constexpr auto operator()(Range&& r)
      {
        return enumerate_view(views::all(etl::forward<Range>(r)));
      }
    };

    namespace views
    {
      namespace private_views
      {
        struct enumerate : public range_adapter_closure_base
        {
          template <class Range>
          constexpr auto operator()(Range&& r) const
          {
            return enumerate_view(views::all(etl::forward<Range>(r)));
          }

          constexpr auto operator()() const
          {
            return ranges::enumerate_range_adapter_closure();
          }
        };
      } // namespace private_views

      inline constexpr private_views::enumerate enumerate;
    } // namespace views

    //*************************************************************************
    /// elements_iterator
    /// An iterator adaptor that extracts the Nth element from a tuple-like
    /// value type using etl::get or std::get (found via ADL).
    //*************************************************************************
    template <class Range, size_t N>
    class elements_iterator
    {
    public:

      using trait = typename etl::ranges::private_ranges::iterator_trait<Range>;

      using base_iterator   = typename trait::const_iterator;
      using base_value_type = typename trait::value_type;
      using value_type      = etl::tuple_element_t<N, base_value_type>;
      using difference_type = typename trait::difference_type;
      using pointer         = const value_type*;
      using reference       = const value_type&;

      using iterator_category = ETL_OR_STD::forward_iterator_tag;

      elements_iterator(base_iterator it)
        : _it(it)
      {
      }

      elements_iterator(const elements_iterator& other)
        : _it{other._it}
      {
      }

      elements_iterator& operator++()
      {
        ++_it;
        return *this;
      }

      elements_iterator operator++(int)
      {
        elements_iterator tmp = *this;
        _it++;
        return tmp;
      }

      elements_iterator& operator=(const elements_iterator& other)
      {
        _it = other._it;
        return *this;
      }

      decltype(auto) operator*() const
      {
        using etl::get;
        return get<N>(*_it);
      }

      bool operator==(const elements_iterator& other) const
      {
        return other._it == _it;
      }

      bool operator!=(const elements_iterator& other) const
      {
        return !(*this == other);
      }

    private:

      base_iterator _it;
    };

    //*************************************************************************
    /// elements_view
    /// A range adaptor that takes a view of tuple-like values and produces
    /// a view of the Nth element of each tuple-like value.
    //*************************************************************************
    template <class Range, size_t N>
    class elements_view : public etl::ranges::view_interface<elements_view<Range, N>>
    {
    public:

      using iterator       = elements_iterator<Range, N>;
      using const_iterator = elements_iterator<Range, N>;

      elements_view(Range&& r)
        : _r{etl::move(r)}
      {
      }

      elements_view(const elements_view& other) = default;

      constexpr Range& base() const&
      {
        return _r;
      }

      constexpr const_iterator begin() const
      {
        return const_iterator(ETL_OR_STD::begin(_r));
      }

      constexpr const_iterator end() const
      {
        return const_iterator(ETL_OR_STD::end(_r));
      }

      constexpr size_t size() const
      {
        return static_cast<size_t>(etl::distance(ETL_OR_STD::cbegin(_r), ETL_OR_STD::cend(_r)));
      }

    private:

      mutable Range _r;
    };

    template <class Range, size_t N>
    elements_view(Range&&, etl::integral_constant<size_t, N>) -> elements_view<views::all_t<Range>, N>;

    template <size_t N>
    struct elements_range_adapter_closure : public range_adapter_closure<elements_range_adapter_closure<N>>
    {
      template <typename Range>
      using target_view_type = elements_view<Range, N>;

      elements_range_adapter_closure() = default;

      template <typename Range>
      constexpr auto operator()(Range&& r)
      {
        return elements_view<views::all_t<Range>, N>(views::all(etl::forward<Range>(r)));
      }
    };

    /// keys_view is an alias for elements_view with N=0.
    template <class Range>
    using keys_view = elements_view<Range, 0>;

    /// values_view is an alias for elements_view with N=1.
    template <class Range>
    using values_view = elements_view<Range, 1>;

    namespace views
    {
      namespace private_views
      {
        template <size_t N>
        struct elements_fn : public range_adapter_closure_base
        {
          template <class Range>
          constexpr auto operator()(Range&& r) const
          {
            return elements_view<views::all_t<Range>, N>(views::all(etl::forward<Range>(r)));
          }

          constexpr auto operator()() const
          {
            return ranges::elements_range_adapter_closure<N>();
          }
        };
      } // namespace private_views

      template <size_t N>
      inline constexpr private_views::elements_fn<N> elements{};

      inline constexpr private_views::elements_fn<0> keys{};
      inline constexpr private_views::elements_fn<1> values{};
    } // namespace views

    //*************************************************************************
    /// Helper: create a tuple type that repeats T exactly N times.
    //*************************************************************************
    namespace private_ranges
    {
      template <typename T, size_t N, typename = etl::make_index_sequence<N>>
      struct repeat_tuple;

      template <typename T, size_t N, size_t... Is>
      struct repeat_tuple<T, N, etl::index_sequence<Is...>>
      {
        template <size_t>
        using always = T;

        using type = etl::tuple<always<Is>...>;
      };

      template <typename T, size_t N>
      using repeat_tuple_t = typename repeat_tuple<T, N>::type;

      /// Helper: compute invoke_result_t<Fun, T, T, ..., T> with T repeated N
      /// times.
      template <typename Fun, typename T, size_t N, typename = etl::make_index_sequence<N>>
      struct repeat_invoke_result;

      template <typename Fun, typename T, size_t N, size_t... Is>
      struct repeat_invoke_result<Fun, T, N, etl::index_sequence<Is...>>
      {
        template <size_t>
        using always = T;

        using type = etl::invoke_result_t<Fun, always<Is>...>;
      };

      template <typename Fun, typename T, size_t N>
      using repeat_invoke_result_t = typename repeat_invoke_result<Fun, T, N>::type;
    } // namespace private_ranges

    //*************************************************************************
    /// adjacent_iterator
    /// An iterator adaptor that produces tuples of N consecutive elements
    /// from the underlying range. Each increment advances all N internal
    /// iterators by one position.
    //*************************************************************************
    template <class Range, size_t N>
    class adjacent_iterator
    {
      static_assert(N > 0, "adjacent window size must be > 0");

    public:

      using trait = typename etl::ranges::private_ranges::iterator_trait<Range>;

      using base_iterator   = typename trait::const_iterator;
      using base_value_type = typename trait::value_type;
      using value_type      = private_ranges::repeat_tuple_t<base_value_type, N>;
      using difference_type = typename trait::difference_type;
      using pointer         = const value_type*;
      using reference       = value_type;

      using iterator_category = ETL_OR_STD::forward_iterator_tag;

      /// Construct from an array of N iterators (the sliding window).
      template <size_t... Is>
      constexpr adjacent_iterator(base_iterator first, base_iterator last, etl::index_sequence<Is...>)
        : _iters{advance_copy(first, last, Is)...}
        , _end{last}
      {
      }

      constexpr adjacent_iterator(const adjacent_iterator& other) = default;

      constexpr adjacent_iterator& operator=(const adjacent_iterator& other) = default;

      constexpr adjacent_iterator& operator++()
      {
        increment(etl::make_index_sequence<N>{});
        return *this;
      }

      constexpr adjacent_iterator operator++(int)
      {
        adjacent_iterator tmp = *this;
        ++(*this);
        return tmp;
      }

      constexpr value_type operator*() const
      {
        return deref(etl::make_index_sequence<N>{});
      }

      friend constexpr bool operator==(const adjacent_iterator& lhs, const adjacent_iterator& rhs)
      {
        // Compare the last iterator in the window (index N-1).
        // When it reaches end, the window is exhausted.
        return lhs._iters[N - 1] == rhs._iters[N - 1];
      }

      friend constexpr bool operator!=(const adjacent_iterator& lhs, const adjacent_iterator& rhs)
      {
        return !(lhs == rhs);
      }

    private:

      static constexpr base_iterator advance_copy(base_iterator it, base_iterator last, size_t n)
      {
        for (size_t i = 0; i < n && it != last; ++i)
        {
          ++it;
        }
        return it;
      }

      template <size_t... Is>
      constexpr void increment(etl::index_sequence<Is...>)
      {
        ((void)((_iters[Is] != _end) ? (void)++_iters[Is] : (void)0), ...);
      }

      template <size_t... Is>
      constexpr value_type deref(etl::index_sequence<Is...>) const
      {
        return value_type(*_iters[Is]...);
      }

      base_iterator _iters[N];
      base_iterator _end;
    };

    //*************************************************************************
    /// adjacent_view
    /// A range adaptor that takes a range and a compile-time window size N
    /// and produces a view of tuples, where the i-th tuple contains the
    /// elements at positions [i, i+1, ..., i+N-1] from the underlying range.
    /// The resulting view has (size - N + 1) elements, or is empty if the
    /// underlying range has fewer than N elements.
    //*************************************************************************
    template <class Range, size_t N>
    class adjacent_view : public etl::ranges::view_interface<adjacent_view<Range, N>>
    {
      static_assert(N > 0, "adjacent window size must be > 0");

    public:

      using iterator       = adjacent_iterator<Range, N>;
      using const_iterator = adjacent_iterator<Range, N>;

      constexpr adjacent_view(Range&& r)
        : _r{etl::move(r)}
      {
      }

      adjacent_view(const adjacent_view& other) = default;

      constexpr Range& base() const&
      {
        return _r;
      }

      constexpr const_iterator begin() const
      {
        return const_iterator(ETL_OR_STD::begin(_r), ETL_OR_STD::end(_r), etl::make_index_sequence<N>{});
      }

      constexpr const_iterator end() const
      {
        // The end iterator has all N internal iterators at the end position.
        return const_iterator(ETL_OR_STD::end(_r), ETL_OR_STD::end(_r), etl::make_index_sequence<N>{});
      }

      constexpr size_t size() const
      {
        auto total = static_cast<size_t>(etl::distance(ETL_OR_STD::cbegin(_r), ETL_OR_STD::cend(_r)));
        return (total >= N) ? (total - N + 1) : 0;
      }

    private:

      mutable Range _r;
    };

    template <class Range, size_t N>
    adjacent_view(Range&&, etl::integral_constant<size_t, N>) -> adjacent_view<views::all_t<Range>, N>;

    template <size_t N>
    struct adjacent_range_adapter_closure : public range_adapter_closure<adjacent_range_adapter_closure<N>>
    {
      template <typename Range>
      using target_view_type = adjacent_view<Range, N>;

      adjacent_range_adapter_closure() = default;

      template <typename Range>
      constexpr auto operator()(Range&& r)
      {
        return adjacent_view<views::all_t<Range>, N>(views::all(etl::forward<Range>(r)));
      }
    };

    namespace views
    {
      namespace private_views
      {
        template <size_t N>
        struct adjacent_fn : public range_adapter_closure_base
        {
          template <class Range>
          constexpr auto operator()(Range&& r) const
          {
            return adjacent_view<views::all_t<Range>, N>(views::all(etl::forward<Range>(r)));
          }

          constexpr auto operator()() const
          {
            return ranges::adjacent_range_adapter_closure<N>();
          }
        };
      } // namespace private_views

      template <size_t N>
      inline constexpr private_views::adjacent_fn<N> adjacent{};

      /// pairwise is an alias for adjacent<2>.
      inline constexpr private_views::adjacent_fn<2> pairwise{};
    } // namespace views

    //*************************************************************************
    /// adjacent_transform_iterator
    /// An iterator adaptor that takes a sliding window of N consecutive
    /// elements from the underlying range and applies a transformation
    /// function to them, producing a single value per window position.
    //*************************************************************************
    template <class Range, class Fun, size_t N>
    class adjacent_transform_iterator
    {
      static_assert(N > 0, "adjacent window size must be > 0");

    public:

      using trait = typename etl::ranges::private_ranges::iterator_trait<Range>;

      using base_iterator   = typename trait::const_iterator;
      using base_value_type = typename trait::value_type;
      using value_type      = private_ranges::repeat_invoke_result_t<Fun, base_value_type, N>;
      using difference_type = typename trait::difference_type;
      using pointer         = const value_type*;
      using reference       = value_type;

      using iterator_category = ETL_OR_STD::forward_iterator_tag;

      /// Construct from a starting iterator, end sentinel, index sequence, and
      /// a function.
      template <size_t... Is>
      constexpr adjacent_transform_iterator(Fun f, base_iterator first, base_iterator last, etl::index_sequence<Is...>)
        : _f{f}
        , _iters{advance_copy(first, last, Is)...}
        , _end{last}
      {
      }

      constexpr adjacent_transform_iterator(const adjacent_transform_iterator& other) = default;

      constexpr adjacent_transform_iterator& operator=(const adjacent_transform_iterator& other) = default;

      constexpr adjacent_transform_iterator& operator++()
      {
        increment(etl::make_index_sequence<N>{});
        return *this;
      }

      constexpr adjacent_transform_iterator operator++(int)
      {
        adjacent_transform_iterator tmp = *this;
        ++(*this);
        return tmp;
      }

      constexpr value_type operator*() const
      {
        return deref(etl::make_index_sequence<N>{});
      }

      friend constexpr bool operator==(const adjacent_transform_iterator& lhs, const adjacent_transform_iterator& rhs)
      {
        // Compare the last iterator in the window (index N-1).
        // When it reaches end, the window is exhausted.
        return lhs._iters[N - 1] == rhs._iters[N - 1];
      }

      friend constexpr bool operator!=(const adjacent_transform_iterator& lhs, const adjacent_transform_iterator& rhs)
      {
        return !(lhs == rhs);
      }

    private:

      static constexpr base_iterator advance_copy(base_iterator it, base_iterator last, size_t n)
      {
        for (size_t i = 0; i < n && it != last; ++i)
        {
          ++it;
        }
        return it;
      }

      template <size_t... Is>
      constexpr void increment(etl::index_sequence<Is...>)
      {
        ((void)((_iters[Is] != _end) ? (void)++_iters[Is] : (void)0), ...);
      }

      template <size_t... Is>
      constexpr value_type deref(etl::index_sequence<Is...>) const
      {
        return etl::invoke(_f, *_iters[Is]...);
      }

      Fun           _f;
      base_iterator _iters[N];
      base_iterator _end;
    };

    //*************************************************************************
    /// adjacent_transform_view
    /// A range adaptor that takes a range, a compile-time window size N, and
    /// a transformation function, and produces a view whose elements are the
    /// result of applying the function to each sliding window of N consecutive
    /// elements from the underlying range.
    /// The resulting view has (size - N + 1) elements, or is empty if the
    /// underlying range has fewer than N elements.
    //*************************************************************************
    template <class Range, class Fun, size_t N>
    class adjacent_transform_view : public etl::ranges::view_interface< adjacent_transform_view<Range, Fun, N>>
    {
      static_assert(N > 0, "adjacent window size must be > 0");

    public:

      using iterator       = adjacent_transform_iterator<Range, Fun, N>;
      using const_iterator = adjacent_transform_iterator<Range, Fun, N>;

      constexpr adjacent_transform_view(Fun f, Range&& r)
        : _f{f}
        , _r{etl::move(r)}
      {
      }

      adjacent_transform_view(const adjacent_transform_view& other) = default;

      constexpr Range& base() const&
      {
        return _r;
      }

      constexpr const_iterator begin() const
      {
        return const_iterator(_f, ETL_OR_STD::begin(_r), ETL_OR_STD::end(_r), etl::make_index_sequence<N>{});
      }

      constexpr const_iterator end() const
      {
        // The end iterator has all N internal iterators at the end position.
        return const_iterator(_f, ETL_OR_STD::end(_r), ETL_OR_STD::end(_r), etl::make_index_sequence<N>{});
      }

      constexpr size_t size() const
      {
        auto total = static_cast<size_t>(etl::distance(ETL_OR_STD::cbegin(_r), ETL_OR_STD::cend(_r)));
        return (total >= N) ? (total - N + 1) : 0;
      }

    private:

      Fun           _f;
      mutable Range _r;
    };

    template <class Fun, class Range, size_t N>
    adjacent_transform_view(Fun, Range&&, etl::integral_constant<size_t, N>) -> adjacent_transform_view<views::all_t<Range>, Fun, N>;

    template <size_t N, typename Fun>
    struct adjacent_transform_range_adapter_closure : public range_adapter_closure< adjacent_transform_range_adapter_closure<N, Fun>>
    {
      template <typename Range>
      using target_view_type = adjacent_transform_view<Range, Fun, N>;

      adjacent_transform_range_adapter_closure(Fun f)
        : _f{f}
      {
      }

      template <typename Range>
      constexpr auto operator()(Range&& r)
      {
        return adjacent_transform_view<views::all_t<Range>, Fun, N>(_f, views::all(etl::forward<Range>(r)));
      }

      Fun _f;
    };

    namespace views
    {
      namespace private_views
      {
        template <size_t N>
        struct adjacent_transform_fn
        {
          template <class Range, typename Fun>
          constexpr auto operator()(Range&& r, Fun&& f) const
          {
            return adjacent_transform_view<views::all_t<Range>, etl::decay_t<Fun>, N>(etl::forward<Fun>(f), views::all(etl::forward<Range>(r)));
          }

          template <typename Fun>
          constexpr auto operator()(Fun&& f) const
          {
            return ranges::adjacent_transform_range_adapter_closure< N, etl::decay_t<Fun>>(etl::forward<Fun>(f));
          }
        };
      } // namespace private_views

      template <size_t N>
      inline constexpr private_views::adjacent_transform_fn<N> adjacent_transform{};

      /// pairwise_transform is an alias for adjacent_transform<2>.
      inline constexpr private_views::adjacent_transform_fn<2> pairwise_transform{};
    } // namespace views

    //*************************************************************************
    /// chunk_iterator: an iterator that yields subrange chunks of a range.
    //*************************************************************************
    template <class Range>
    class chunk_iterator
    {
    public:

      using trait = typename etl::ranges::private_ranges::iterator_trait<Range>;

      using inner_iterator       = typename trait::iterator;
      using const_inner_iterator = typename trait::const_iterator;
      using difference_type      = typename trait::difference_type;

      using iterator_category = ETL_OR_STD::forward_iterator_tag;

      using value_type = etl::ranges::subrange<const_inner_iterator>;
      using pointer    = value_type*;
      using reference  = value_type;

      chunk_iterator(const_inner_iterator it, const_inner_iterator it_end, difference_type chunk_size)
        : _it(it)
        , _it_end(it_end)
        , _chunk_size(chunk_size)
      {
      }

      chunk_iterator(const chunk_iterator& other) = default;

      chunk_iterator& operator=(const chunk_iterator& other) = default;

      chunk_iterator& operator++()
      {
        difference_type remaining = etl::distance(_it, _it_end);
        difference_type step      = (_chunk_size < remaining) ? _chunk_size : remaining;
        etl::advance(_it, step);
        return *this;
      }

      chunk_iterator operator++(int)
      {
        chunk_iterator tmp{*this};
        ++(*this);
        return tmp;
      }

      value_type operator*() const
      {
        difference_type      remaining = etl::distance(_it, _it_end);
        difference_type      step      = (_chunk_size < remaining) ? _chunk_size : remaining;
        const_inner_iterator chunk_end = _it;
        etl::advance(chunk_end, step);
        return value_type(_it, chunk_end);
      }

      constexpr bool operator==(const chunk_iterator& other) const
      {
        return _it == other._it;
      }

      constexpr bool operator!=(const chunk_iterator& other) const
      {
        return !(*this == other);
      }

    private:

      const_inner_iterator _it;
      const_inner_iterator _it_end;
      difference_type      _chunk_size;
    };

    //*************************************************************************
    /// chunk_view: splits a range into non-overlapping chunks of a given size.
    /// The last chunk may be smaller if the range size is not evenly divisible
    /// by the chunk size.
    //*************************************************************************
    template <class Range>
    class chunk_view : public etl::ranges::view_interface<chunk_view<Range>>
    {
    public:

      using iterator        = chunk_iterator<Range>;
      using const_iterator  = chunk_iterator<Range>;
      using difference_type = typename etl::ranges::private_ranges::iterator_trait< Range>::difference_type;

      constexpr chunk_view(Range&& r, difference_type chunk_size)
        : _r{etl::move(r)}
        , _chunk_size{chunk_size}
      {
      }

      chunk_view(const chunk_view& other) = default;

      constexpr Range base() const&
      {
        return _r;
      }

      constexpr iterator begin() const
      {
        return iterator(ETL_OR_STD::begin(_r), ETL_OR_STD::end(_r), _chunk_size);
      }

      constexpr iterator end() const
      {
        return iterator(ETL_OR_STD::end(_r), ETL_OR_STD::end(_r), _chunk_size);
      }

    private:

      Range           _r;
      difference_type _chunk_size;
    };

    template <class Range>
    chunk_view(Range&&, typename etl::ranges::private_ranges::iterator_trait< Range>::difference_type) -> chunk_view<views::all_t<Range>>;

    struct chunk_range_adapter_closure : public range_adapter_closure<chunk_range_adapter_closure>
    {
      template <typename Range>
      using target_view_type = chunk_view<Range>;

      template <class DifferenceType>
      constexpr chunk_range_adapter_closure(DifferenceType chunk_size)
        : _chunk_size{static_cast<size_t>(chunk_size)}
      {
      }

      template <typename Range>
      constexpr auto operator()(Range&& r) const
      {
        return chunk_view(views::all(etl::forward<Range>(r)), static_cast< typename chunk_view<views::all_t<Range>>::difference_type>(_chunk_size));
      }

      const size_t _chunk_size;
    };

    namespace views
    {
      namespace private_views
      {
        struct chunk
        {
          template <class Range>
          constexpr auto operator()(Range&& r, ranges::range_difference_t<Range> chunk_size) const
          {
            return chunk_view(views::all(etl::forward<Range>(r)), chunk_size);
          }

          template <class DifferenceType>
          constexpr auto operator()(DifferenceType chunk_size) const
          {
            return ranges::chunk_range_adapter_closure(chunk_size);
          }
        };
      } // namespace private_views

      inline constexpr private_views::chunk chunk;
    } // namespace views

    //*************************************************************************
    /// slide_iterator: an iterator that yields overlapping subrange windows
    /// of a given size from the underlying range.
    //*************************************************************************
    template <class Range>
    class slide_iterator
    {
    public:

      using trait = typename etl::ranges::private_ranges::iterator_trait<Range>;

      using inner_iterator       = typename trait::iterator;
      using const_inner_iterator = typename trait::const_iterator;
      using difference_type      = typename trait::difference_type;

      using iterator_category = ETL_OR_STD::forward_iterator_tag;

      using value_type = etl::ranges::subrange<const_inner_iterator>;
      using pointer    = value_type*;
      using reference  = value_type;

      slide_iterator(const_inner_iterator it, const_inner_iterator it_end, difference_type window_size)
        : _it(it)
        , _it_end(it_end)
        , _window_size(window_size)
      {
      }

      slide_iterator(const slide_iterator& other) = default;

      slide_iterator& operator=(const slide_iterator& other) = default;

      slide_iterator& operator++()
      {
        ++_it;
        return *this;
      }

      slide_iterator operator++(int)
      {
        slide_iterator tmp{*this};
        ++(*this);
        return tmp;
      }

      value_type operator*() const
      {
        const_inner_iterator window_end = _it;
        etl::advance(window_end, _window_size);
        return value_type(_it, window_end);
      }

      constexpr bool operator==(const slide_iterator& other) const
      {
        return _it == other._it;
      }

      constexpr bool operator!=(const slide_iterator& other) const
      {
        return !(*this == other);
      }

    private:

      const_inner_iterator _it;
      const_inner_iterator _it_end;
      difference_type      _window_size;
    };

    //*************************************************************************
    /// slide_view: produces a view of overlapping subranges (sliding windows)
    /// of a given size from the underlying range.
    /// For a range of size S and window size N, the resulting view has
    /// max(S - N + 1, 0) elements. Each element is a subrange of N
    /// consecutive elements from the underlying range.
    //*************************************************************************
    template <class Range>
    class slide_view : public etl::ranges::view_interface<slide_view<Range>>
    {
    public:

      using iterator        = slide_iterator<Range>;
      using const_iterator  = slide_iterator<Range>;
      using difference_type = typename etl::ranges::private_ranges::iterator_trait< Range>::difference_type;

      constexpr slide_view(Range&& r, difference_type window_size)
        : _r{etl::move(r)}
        , _window_size{window_size}
      {
      }

      slide_view(const slide_view& other) = default;

      constexpr Range base() const&
      {
        return _r;
      }

      constexpr iterator begin() const
      {
        return iterator(ETL_OR_STD::begin(_r), ETL_OR_STD::end(_r), _window_size);
      }

      constexpr iterator end() const
      {
        auto total = static_cast<difference_type>(etl::distance(ETL_OR_STD::begin(_r), ETL_OR_STD::end(_r)));
        if (total < _window_size)
        {
          // Empty view: begin == end
          return iterator(ETL_OR_STD::begin(_r), ETL_OR_STD::end(_r), _window_size);
        }
        auto end_it = ETL_OR_STD::begin(_r);
        etl::advance(end_it, total - _window_size + 1);
        return iterator(end_it, ETL_OR_STD::end(_r), _window_size);
      }

      constexpr size_t size() const
      {
        auto total = static_cast<size_t>(etl::distance(ETL_OR_STD::cbegin(_r), ETL_OR_STD::cend(_r)));
        return (total >= static_cast<size_t>(_window_size)) ? (total - static_cast<size_t>(_window_size) + 1) : 0;
      }

    private:

      Range           _r;
      difference_type _window_size;
    };

    template <class Range>
    slide_view(Range&&, typename etl::ranges::private_ranges::iterator_trait< Range>::difference_type) -> slide_view<views::all_t<Range>>;

    struct slide_range_adapter_closure : public range_adapter_closure<slide_range_adapter_closure>
    {
      template <typename Range>
      using target_view_type = slide_view<Range>;

      template <class DifferenceType>
      constexpr slide_range_adapter_closure(DifferenceType window_size)
        : _window_size{static_cast<size_t>(window_size)}
      {
      }

      template <typename Range>
      constexpr auto operator()(Range&& r) const
      {
        return slide_view(views::all(etl::forward<Range>(r)), static_cast< typename slide_view<views::all_t<Range>>::difference_type>(_window_size));
      }

      const size_t _window_size;
    };

    namespace views
    {
      namespace private_views
      {
        struct slide
        {
          template <class Range>
          constexpr auto operator()(Range&& r, ranges::range_difference_t<Range> window_size) const
          {
            return slide_view(views::all(etl::forward<Range>(r)), window_size);
          }

          template <class DifferenceType>
          constexpr auto operator()(DifferenceType window_size) const
          {
            return ranges::slide_range_adapter_closure(window_size);
          }
        };
      } // namespace private_views

      inline constexpr private_views::slide slide;
    } // namespace views

    //*************************************************************************
    /// chunk_by_iterator: an iterator that yields subrange chunks of a range,
    /// where chunk boundaries are determined by a binary predicate.
    /// A new chunk starts whenever the predicate returns false for an
    /// adjacent pair of elements.
    //*************************************************************************
    template <class Range, class Pred>
    class chunk_by_iterator
    {
    public:

      using trait = typename etl::ranges::private_ranges::iterator_trait<Range>;

      using inner_iterator       = typename trait::iterator;
      using const_inner_iterator = typename trait::const_iterator;
      using difference_type      = typename trait::difference_type;

      using iterator_category = ETL_OR_STD::forward_iterator_tag;

      using value_type = etl::ranges::subrange<const_inner_iterator>;
      using pointer    = value_type*;
      using reference  = value_type;

      chunk_by_iterator(const_inner_iterator it, const_inner_iterator it_end, const Pred& pred)
        : _it(it)
        , _it_end(it_end)
        , _pred(pred)
      {
        _chunk_end = find_next_chunk_end();
      }

      chunk_by_iterator(const chunk_by_iterator& other) = default;

      chunk_by_iterator& operator=(const chunk_by_iterator& other) = default;

      chunk_by_iterator& operator++()
      {
        _it        = _chunk_end;
        _chunk_end = find_next_chunk_end();
        return *this;
      }

      chunk_by_iterator operator++(int)
      {
        chunk_by_iterator tmp{*this};
        ++(*this);
        return tmp;
      }

      value_type operator*() const
      {
        return value_type(_it, _chunk_end);
      }

      constexpr bool operator==(const chunk_by_iterator& other) const
      {
        return _it == other._it;
      }

      constexpr bool operator!=(const chunk_by_iterator& other) const
      {
        return !(*this == other);
      }

    private:

      const_inner_iterator find_next_chunk_end() const
      {
        if (_it == _it_end)
        {
          return _it_end;
        }

        const_inner_iterator it_prev = _it;
        const_inner_iterator it_curr = _it;
        ++it_curr;

        while (it_curr != _it_end)
        {
          if (!_pred(*it_prev, *it_curr))
          {
            return it_curr;
          }
          it_prev = it_curr;
          ++it_curr;
        }

        return _it_end;
      }

      const_inner_iterator _it;
      const_inner_iterator _it_end;
      const_inner_iterator _chunk_end;
      Pred                 _pred;
    };

    //*************************************************************************
    /// chunk_by_view: splits a range into subranges between adjacent elements
    /// for which the given binary predicate returns false.
    /// Each chunk is a maximal subrange of consecutive elements where the
    /// predicate holds for every adjacent pair.
    //*************************************************************************
    template <class Range, class Pred>
    class chunk_by_view : public etl::ranges::view_interface<chunk_by_view<Range, Pred>>
    {
    public:

      using iterator       = chunk_by_iterator<Range, Pred>;
      using const_iterator = chunk_by_iterator<Range, Pred>;

      chunk_by_view(Range&& r, const Pred& pred)
        : _r{etl::move(r)}
        , _pred{pred}
      {
      }

      chunk_by_view(const chunk_by_view& other) = default;

      constexpr Range base() const&
      {
        return _r;
      }

      constexpr const Pred& pred() const
      {
        return _pred;
      }

      constexpr const_iterator begin() const
      {
        return const_iterator(ETL_OR_STD::begin(_r), ETL_OR_STD::end(_r), _pred);
      }

      constexpr const_iterator end() const
      {
        return const_iterator(ETL_OR_STD::end(_r), ETL_OR_STD::end(_r), _pred);
      }

    private:

      Range      _r;
      const Pred _pred;
    };

    template <class Range, typename Pred>
    chunk_by_view(Range&&, Pred) -> chunk_by_view<views::all_t<Range>, Pred>;

    template <typename Pred>
    struct chunk_by_range_adapter_closure : public range_adapter_closure<chunk_by_range_adapter_closure<Pred>>
    {
      template <typename Range>
      using target_view_type = chunk_by_view<Range, Pred>;

      chunk_by_range_adapter_closure(const Pred& p)
        : _p{p}
      {
      }

      template <typename Range>
      constexpr auto operator()(Range&& r) const
      {
        return chunk_by_view(views::all(etl::forward<Range>(r)), _p);
      }

      const Pred _p;
    };

    namespace views
    {
      namespace private_views
      {
        struct chunk_by
        {
          template <class Range, typename Pred>
          constexpr auto operator()(Range&& r, const Pred& p) const
          {
            return chunk_by_view(views::all(etl::forward<Range>(r)), p);
          }

          template <typename Pred>
          constexpr auto operator()(const Pred& p) const
          {
            return ranges::chunk_by_range_adapter_closure<Pred>(p);
          }
        };
      } // namespace private_views

      inline constexpr private_views::chunk_by chunk_by;
    } // namespace views

    //*************************************************************************
    /// stride_iterator: an iterator adaptor that advances the underlying
    /// iterator by a fixed stride on each increment.
    //*************************************************************************
    template <class Range>
    class stride_iterator
    {
    public:

      using trait = typename etl::ranges::private_ranges::iterator_trait<Range>;

      using inner_iterator       = typename trait::iterator;
      using const_inner_iterator = typename trait::const_iterator;
      using difference_type      = typename trait::difference_type;

      using iterator_category = ETL_OR_STD::forward_iterator_tag;

      using value_type = typename trait::value_type;
      using pointer    = typename trait::pointer;
      using reference  = typename trait::reference;

      constexpr stride_iterator(const_inner_iterator it, const_inner_iterator it_end, difference_type stride_n)
        : _it(it)
        , _it_end(it_end)
        , _stride_n(stride_n)
      {
      }

      stride_iterator(const stride_iterator& other) = default;

      stride_iterator& operator=(const stride_iterator& other) = default;

      constexpr stride_iterator& operator++()
      {
        difference_type remaining = etl::distance(_it, _it_end);
        difference_type step      = (_stride_n < remaining) ? _stride_n : remaining;
        etl::advance(_it, step);
        return *this;
      }

      constexpr stride_iterator operator++(int)
      {
        stride_iterator tmp{*this};
        ++(*this);
        return tmp;
      }

      constexpr auto operator*() const
      {
        return *_it;
      }

      constexpr auto operator->() const
      {
        return &(*_it);
      }

      constexpr bool operator==(const stride_iterator& other) const
      {
        return _it == other._it;
      }

      constexpr bool operator!=(const stride_iterator& other) const
      {
        return !(*this == other);
      }

    private:

      mutable const_inner_iterator _it;
      const_inner_iterator         _it_end;
      difference_type              _stride_n;
    };

    //*************************************************************************
    /// stride_view: a range adaptor that yields every Nth element from the
    /// underlying range, starting with the first element.
    //*************************************************************************
    template <class Range>
    class stride_view : public etl::ranges::view_interface<stride_view<Range>>
    {
    public:

      using iterator        = stride_iterator<Range>;
      using const_iterator  = stride_iterator<Range>;
      using difference_type = typename etl::ranges::private_ranges::iterator_trait< Range>::difference_type;

      constexpr stride_view(Range&& r, difference_type stride_n)
        : _r{etl::move(r)}
        , _stride_n{stride_n}
      {
      }

      stride_view(const stride_view& other) = default;

      constexpr Range base() const&
      {
        return _r;
      }

      constexpr iterator begin() const
      {
        return iterator(ETL_OR_STD::begin(_r), ETL_OR_STD::end(_r), _stride_n);
      }

      constexpr iterator end() const
      {
        return iterator(ETL_OR_STD::end(_r), ETL_OR_STD::end(_r), _stride_n);
      }

    private:

      Range           _r;
      difference_type _stride_n;
    };

    template <class Range>
    stride_view(Range&&, typename etl::ranges::private_ranges::iterator_trait< Range>::difference_type) -> stride_view<views::all_t<Range>>;

    struct stride_range_adapter_closure : public range_adapter_closure<stride_range_adapter_closure>
    {
      template <typename Range>
      using target_view_type = stride_view<Range>;

      template <class DifferenceType>
      constexpr stride_range_adapter_closure(DifferenceType stride_n)
        : _stride_n{static_cast<size_t>(stride_n)}
      {
      }

      template <typename Range>
      constexpr auto operator()(Range&& r) const
      {
        return stride_view(views::all(etl::forward<Range>(r)), static_cast< typename stride_view<views::all_t<Range>>::difference_type>(_stride_n));
      }

      const size_t _stride_n;
    };

    namespace views
    {
      namespace private_views
      {
        struct stride
        {
          template <class Range>
          constexpr auto operator()(Range&& r, ranges::range_difference_t<Range> stride_n) const
          {
            return stride_view(views::all(etl::forward<Range>(r)), stride_n);
          }

          template <class DifferenceType>
          constexpr auto operator()(DifferenceType stride_n) const
          {
            return ranges::stride_range_adapter_closure(stride_n);
          }
        };
      } // namespace private_views

      inline constexpr private_views::stride stride;
    } // namespace views

    //*************************************************************************
    /// cartesian_product_iterator
    /// An iterator adaptor that iterates over the Cartesian product of
    /// multiple ranges, producing etl::tuple of values from each range.
    /// The iteration order is lexicographic with the last range varying
    /// fastest (like nested for loops).
    //*************************************************************************
    template <class... Ranges>
    class cartesian_product_iterator
    {
      static_assert(sizeof...(Ranges) > 0, "Type list must be non-empty");

    public:

      using iterators_type  = etl::tuple< typename etl::ranges::private_ranges::iterator_trait< Ranges>::const_iterator...>;
      using value_type      = etl::tuple<typename etl::ranges::private_ranges::iterator_trait< Ranges>::value_type...>;
      using difference_type = ptrdiff_t;
      using pointer         = const value_type*;
      using reference       = value_type;

      using iterator_category = ETL_OR_STD::forward_iterator_tag;

      constexpr cartesian_product_iterator(iterators_type current, iterators_type begins, iterators_type ends, bool is_end = false)
        : _current_it(current)
        , _begins(begins)
        , _ends(ends)
        , _is_end(is_end)
      {
      }

      constexpr cartesian_product_iterator(const cartesian_product_iterator& other) = default;

      constexpr cartesian_product_iterator& operator=(const cartesian_product_iterator& other) = default;

      constexpr cartesian_product_iterator& operator++()
      {
        increment();
        return *this;
      }

      constexpr cartesian_product_iterator operator++(int)
      {
        cartesian_product_iterator tmp = *this;
        ++(*this);
        return tmp;
      }

      constexpr value_type operator*() const
      {
        return deref(etl::make_index_sequence<sizeof...(Ranges)>{});
      }

      friend constexpr bool operator==(const cartesian_product_iterator& lhs, const cartesian_product_iterator& rhs)
      {
        return lhs._is_end == rhs._is_end && (lhs._is_end || lhs.all_equal(rhs, etl::make_index_sequence<sizeof...(Ranges)>{}));
      }

      friend constexpr bool operator!=(const cartesian_product_iterator& lhs, const cartesian_product_iterator& rhs)
      {
        return !(lhs == rhs);
      }

    private:

      // Increment with carry: increment the last range, and carry over to the
      // previous range when a range wraps around
      constexpr void increment()
      {
        if (_is_end)
          return;
        increment_at<sizeof...(Ranges) - 1>();
      }

      template <size_t I>
      constexpr etl::enable_if_t<(I > 0)> increment_at()
      {
        auto& it = etl::get<I>(_current_it);
        ++it;
        if (it == etl::get<I>(_ends))
        {
          it = etl::get<I>(_begins);
          increment_at<I - 1>();
        }
      }

      template <size_t I>
      constexpr etl::enable_if_t<(I == 0)> increment_at()
      {
        auto& it = etl::get<0>(_current_it);
        ++it;
        if (it == etl::get<0>(_ends))
        {
          _is_end = true;
        }
      }

      template <size_t... Is>
      constexpr value_type deref(etl::index_sequence<Is...>) const
      {
        return value_type(*etl::get<Is>(_current_it)...);
      }

      template <size_t... Is>
      constexpr bool all_equal(const cartesian_product_iterator& other, etl::index_sequence<Is...>) const
      {
        return ((etl::get<Is>(_current_it) == etl::get<Is>(other._current_it)) && ...);
      }

      iterators_type _current_it;
      iterators_type _begins;
      iterators_type _ends;
      bool           _is_end;
    };

    //*************************************************************************
    /// cartesian_product_view
    /// A range adaptor that computes the Cartesian product of multiple
    /// ranges. Given N ranges, produces a view of tuples where each tuple
    /// contains one element from each range. The total number of elements
    /// is the product of all input range sizes. The last range varies
    /// fastest (like nested for loops).
    //*************************************************************************
    template <class... Ranges>
    class cartesian_product_view : public etl::ranges::view_interface<cartesian_product_view<Ranges...>>
    {
      static_assert(sizeof...(Ranges) > 0, "Type list must be non-empty");

    public:

      using iterator       = cartesian_product_iterator<Ranges...>;
      using const_iterator = cartesian_product_iterator<Ranges...>;

      constexpr cartesian_product_view(Ranges&&... r)
        : _r{etl::move(r)...}
      {
      }

      cartesian_product_view(const cartesian_product_view& other) = default;

      constexpr const_iterator begin() const
      {
        if (any_empty(etl::make_index_sequence<sizeof...(Ranges)>{}))
        {
          return end();
        }
        return make_begin(etl::make_index_sequence<sizeof...(Ranges)>{});
      }

      constexpr const_iterator end() const
      {
        return make_end(etl::make_index_sequence<sizeof...(Ranges)>{});
      }

      constexpr size_t size() const
      {
        return get_product_size(etl::make_index_sequence<sizeof...(Ranges)>{});
      }

    private:

      template <size_t... Is>
      constexpr const_iterator make_begin(etl::index_sequence<Is...>) const
      {
        return const_iterator(typename const_iterator::iterators_type(ETL_OR_STD::begin(etl::get<Is>(_r))...),
                              typename const_iterator::iterators_type(ETL_OR_STD::begin(etl::get<Is>(_r))...),
                              typename const_iterator::iterators_type(ETL_OR_STD::end(etl::get<Is>(_r))...), false);
      }

      template <size_t... Is>
      constexpr const_iterator make_end(etl::index_sequence<Is...>) const
      {
        return const_iterator(typename const_iterator::iterators_type(ETL_OR_STD::end(etl::get<Is>(_r))...),
                              typename const_iterator::iterators_type(ETL_OR_STD::begin(etl::get<Is>(_r))...),
                              typename const_iterator::iterators_type(ETL_OR_STD::end(etl::get<Is>(_r))...), true);
      }

      template <size_t... Is>
      constexpr bool any_empty(etl::index_sequence<Is...>) const
      {
        return ((ETL_OR_STD::begin(etl::get<Is>(_r)) == ETL_OR_STD::end(etl::get<Is>(_r))) || ...);
      }

      template <size_t... Is>
      constexpr size_t get_product_size(etl::index_sequence<Is...>) const
      {
        size_t sizes[] = {static_cast<size_t>(etl::distance(ETL_OR_STD::cbegin(etl::get<Is>(_r)), ETL_OR_STD::cend(etl::get<Is>(_r))))...};
        size_t product = 1;
        for (size_t i = 0; i < sizeof...(Ranges); ++i)
        {
          product *= sizes[i];
        }
        return product;
      }

      mutable etl::tuple<Ranges...> _r;
    };

    template <class... Ranges>
    cartesian_product_view(Ranges&&...) -> cartesian_product_view<views::all_t<Ranges>...>;

    namespace views
    {
      namespace private_views
      {
        struct cartesian_product
        {
          template <class... Ranges>
          constexpr auto operator()(Ranges&&... r) const
          {
            return cartesian_product_view(views::all(etl::forward<Ranges>(r))...);
          }
        };
      } // namespace private_views

      inline constexpr private_views::cartesian_product cartesian_product;
    } // namespace views

    //*************************************************************************
    /// to_input_iterator
    /// An iterator wrapper that downgrades the iterator category to
    /// input_iterator_tag, preserving the underlying iterator's traversal
    /// behaviour but preventing algorithms from assuming stronger guarantees.
    //*************************************************************************
    template <class Range>
    class to_input_iterator
    {
    public:

      using trait = typename etl::ranges::private_ranges::iterator_trait<Range>;

      using iterator        = typename trait::iterator;
      using const_iterator  = typename trait::const_iterator;
      using value_type      = typename trait::value_type;
      using difference_type = typename trait::difference_type;
      using pointer         = typename trait::pointer;
      using reference       = typename trait::reference;

      using iterator_category = ETL_OR_STD::input_iterator_tag;

      to_input_iterator() = default;

      to_input_iterator(const_iterator it)
        : _it(it)
      {
      }

      to_input_iterator(const to_input_iterator& other) = default;

      to_input_iterator& operator=(const to_input_iterator& other) = default;

      to_input_iterator& operator++()
      {
        ++_it;
        return *this;
      }

      to_input_iterator operator++(int)
      {
        to_input_iterator tmp = *this;
        ++(*this);
        return tmp;
      }

      reference operator*() const
      {
        return *_it;
      }

      pointer operator->() const
      {
        return &(*_it);
      }

      bool operator==(const to_input_iterator& other) const
      {
        return _it == other._it;
      }

      bool operator!=(const to_input_iterator& other) const
      {
        return !(*this == other);
      }

    private:

      mutable const_iterator _it;
    };

    //*************************************************************************
    /// to_input_view
    /// A range adaptor that wraps a view and downgrades its iterator category
    /// to input_iterator_tag. The view preserves all the elements and order
    /// of the underlying range but prevents algorithms from relying on
    /// forward, bidirectional, or random-access traversal guarantees.
    //*************************************************************************
    template <class Range>
    class to_input_view : public etl::ranges::view_interface<to_input_view<Range>>
    {
    public:

      using iterator       = to_input_iterator<Range>;
      using const_iterator = iterator;

      to_input_view(const to_input_view& other) = default;

      to_input_view(Range&& r)
        : _r{etl::move(r)}
      {
      }

      constexpr Range& base() const
      {
        return _r;
      }

      constexpr iterator begin() const
      {
        return iterator(ETL_OR_STD::begin(_r));
      }

      constexpr iterator end() const
      {
        return iterator(ETL_OR_STD::end(_r));
      }

      constexpr size_t size() const
      {
        return static_cast<size_t>(etl::distance(ETL_OR_STD::cbegin(_r), ETL_OR_STD::cend(_r)));
      }

    private:

      mutable Range _r;
    };

    template <class Range>
    to_input_view(Range&&) -> to_input_view<views::all_t<Range>>;

    struct to_input_range_adapter_closure : public range_adapter_closure<to_input_range_adapter_closure>
    {
      template <typename Range>
      using target_view_type = to_input_view<Range>;

      to_input_range_adapter_closure() = default;

      template <typename Range>
      constexpr auto operator()(Range&& r)
      {
        return to_input_view(views::all(etl::forward<Range>(r)));
      }
    };

    namespace views
    {
      namespace private_views
      {
        struct to_input : public range_adapter_closure_base
        {
          template <class Range>
          constexpr auto operator()(Range&& r) const
          {
            return to_input_view(views::all(etl::forward<Range>(r)));
          }

          constexpr auto operator()() const
          {
            return ranges::to_input_range_adapter_closure();
          }
        };
      } // namespace private_views

      inline constexpr private_views::to_input to_input;
    } // namespace views

    //*************************************************************************
    /// elements_of
    /// Encapsulates a range. Specializations of elements_of act as a tag in
    /// overload sets to disambiguate when a range should be treated as a
    /// sequence rather than a single value.
    /// This is primarily used with coroutine generators to indicate that
    /// elements of a range should be yielded one at a time.
    //*************************************************************************
    template <class R>
    struct elements_of
    {
      R range;
    };

    template <class R>
    elements_of(R&&) -> elements_of<R>;

    namespace private_ranges
    {
      template <class C>
      struct to_range_adapter_closure : public range_adapter_closure<to_range_adapter_closure<C>>
      {
        template <class Range = void>
        using target_view_type = C;

        to_range_adapter_closure() = default;

        template <class Range>
        C operator()(const Range& r) const
        {
          using result_type = C;

          result_type result;

          for (auto i : r)
          {
            result.push_back(i);
          }

          return result;
        }

        template <class Range>
        C operator()(Range&& r)
        {
          using result_type = C;

          result_type result;

          for (auto&& i : r)
          {
            result.emplace_back(etl::move(i));
          }

          return result;
        }
      };
    } // namespace private_ranges

    template <class C>
    private_ranges::to_range_adapter_closure<C> to()
    {
      return private_ranges::to_range_adapter_closure<C>();
    }

  } // namespace ranges

  namespace views = ranges::views;
} // namespace etl

#endif

#endif
