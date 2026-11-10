///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2017 John Wellbelove

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

#ifndef ETL_COMPARE_INCLUDED
#define ETL_COMPARE_INCLUDED

#include "platform.h"
#include "functional.h"
#include "parameter_type.h"

#if ETL_USING_CPP20 && ETL_USING_STL
  #include <compare>
#endif

//*****************************************************************************
///\defgroup compare compare
/// Comparisons only using less than operator
///\ingroup utilities
//*****************************************************************************
namespace etl
{
#if ETL_USING_CPP20
  #if ETL_USING_STL
  //***************************************************************************
  /// The STL supplies the C++20 comparison categories, so simply expose them
  /// through the etl namespace. This allows ETL code and user code to refer to
  /// etl::strong_ordering (etc.) regardless of the STL configuration.
  //***************************************************************************
  using std::partial_ordering;
  using std::strong_ordering;
  using std::weak_ordering;
  #else
  namespace private_compare
  {
    //*************************************************************************
    /// A type that can only be constructed from the literal 0.
    /// Used to constrain the comparison-category operators to comparisons
    /// against the literal 0, as required by the standard.
    //*************************************************************************
    struct only_literal_zero
    {
      ETL_CONSTEXPR only_literal_zero(only_literal_zero*) ETL_NOEXCEPT {}
    };

    typedef signed char ordering_value_type;

    inline constexpr ordering_value_type ordering_less       = -1;
    inline constexpr ordering_value_type ordering_equivalent = 0;
    inline constexpr ordering_value_type ordering_greater    = 1;
    inline constexpr ordering_value_type ordering_unordered  = 2;
  } // namespace private_compare

  //***************************************************************************
  /// Minimal partial-ordering category for C++20 three-way comparisons when the
  /// STL is unavailable.
  /// Unlike the other categories this one has an 'unordered' state, for which
  /// every comparison against 0 is false.
  //***************************************************************************
  class partial_ordering
  {
  public:

    static const partial_ordering less;
    static const partial_ordering equivalent;
    static const partial_ordering greater;
    static const partial_ordering unordered;

    friend ETL_CONSTEXPR bool operator==(partial_ordering lhs, partial_ordering rhs) ETL_NOEXCEPT = default;

    friend ETL_CONSTEXPR bool operator==(partial_ordering v, private_compare::only_literal_zero) ETL_NOEXCEPT
    {
      return v.value == private_compare::ordering_equivalent;
    }
    friend ETL_CONSTEXPR bool operator<(partial_ordering v, private_compare::only_literal_zero) ETL_NOEXCEPT
    {
      return v.value == private_compare::ordering_less;
    }
    friend ETL_CONSTEXPR bool operator>(partial_ordering v, private_compare::only_literal_zero) ETL_NOEXCEPT
    {
      return v.value == private_compare::ordering_greater;
    }
    friend ETL_CONSTEXPR bool operator<=(partial_ordering v, private_compare::only_literal_zero) ETL_NOEXCEPT
    {
      return (v.value == private_compare::ordering_less) || (v.value == private_compare::ordering_equivalent);
    }
    friend ETL_CONSTEXPR bool operator>=(partial_ordering v, private_compare::only_literal_zero) ETL_NOEXCEPT
    {
      return (v.value == private_compare::ordering_greater) || (v.value == private_compare::ordering_equivalent);
    }

    friend ETL_CONSTEXPR bool operator<(private_compare::only_literal_zero, partial_ordering v) ETL_NOEXCEPT
    {
      return v > 0;
    }
    friend ETL_CONSTEXPR bool operator>(private_compare::only_literal_zero, partial_ordering v) ETL_NOEXCEPT
    {
      return v < 0;
    }
    friend ETL_CONSTEXPR bool operator<=(private_compare::only_literal_zero, partial_ordering v) ETL_NOEXCEPT
    {
      return v >= 0;
    }
    friend ETL_CONSTEXPR bool operator>=(private_compare::only_literal_zero, partial_ordering v) ETL_NOEXCEPT
    {
      return v <= 0;
    }

  private:

    explicit ETL_CONSTEXPR partial_ordering(private_compare::ordering_value_type value_) ETL_NOEXCEPT
      : value(value_)
    {
    }

    friend class weak_ordering;
    friend class strong_ordering;

    private_compare::ordering_value_type value;
  };

  inline constexpr partial_ordering partial_ordering::less(private_compare::ordering_less);
  inline constexpr partial_ordering partial_ordering::equivalent(private_compare::ordering_equivalent);
  inline constexpr partial_ordering partial_ordering::greater(private_compare::ordering_greater);
  inline constexpr partial_ordering partial_ordering::unordered(private_compare::ordering_unordered);

  //***************************************************************************
  /// Minimal weak-ordering category for C++20 three-way comparisons when the
  /// STL is unavailable.
  //***************************************************************************
  class weak_ordering
  {
  public:

    static const weak_ordering less;
    static const weak_ordering equivalent;
    static const weak_ordering greater;

    ETL_CONSTEXPR operator partial_ordering() const ETL_NOEXCEPT
    {
      return partial_ordering(value);
    }

    friend ETL_CONSTEXPR bool operator==(weak_ordering lhs, weak_ordering rhs) ETL_NOEXCEPT = default;

    friend ETL_CONSTEXPR bool operator==(weak_ordering v, private_compare::only_literal_zero) ETL_NOEXCEPT
    {
      return v.value == private_compare::ordering_equivalent;
    }
    friend ETL_CONSTEXPR bool operator<(weak_ordering v, private_compare::only_literal_zero) ETL_NOEXCEPT
    {
      return v.value < private_compare::ordering_equivalent;
    }
    friend ETL_CONSTEXPR bool operator>(weak_ordering v, private_compare::only_literal_zero) ETL_NOEXCEPT
    {
      return v.value > private_compare::ordering_equivalent;
    }
    friend ETL_CONSTEXPR bool operator<=(weak_ordering v, private_compare::only_literal_zero) ETL_NOEXCEPT
    {
      return v.value <= private_compare::ordering_equivalent;
    }
    friend ETL_CONSTEXPR bool operator>=(weak_ordering v, private_compare::only_literal_zero) ETL_NOEXCEPT
    {
      return v.value >= private_compare::ordering_equivalent;
    }

    friend ETL_CONSTEXPR bool operator<(private_compare::only_literal_zero, weak_ordering v) ETL_NOEXCEPT
    {
      return v > 0;
    }
    friend ETL_CONSTEXPR bool operator>(private_compare::only_literal_zero, weak_ordering v) ETL_NOEXCEPT
    {
      return v < 0;
    }
    friend ETL_CONSTEXPR bool operator<=(private_compare::only_literal_zero, weak_ordering v) ETL_NOEXCEPT
    {
      return v >= 0;
    }
    friend ETL_CONSTEXPR bool operator>=(private_compare::only_literal_zero, weak_ordering v) ETL_NOEXCEPT
    {
      return v <= 0;
    }

  private:

    explicit ETL_CONSTEXPR weak_ordering(private_compare::ordering_value_type value_) ETL_NOEXCEPT
      : value(value_)
    {
    }

    friend class strong_ordering;

    private_compare::ordering_value_type value;
  };

  inline constexpr weak_ordering weak_ordering::less(private_compare::ordering_less);
  inline constexpr weak_ordering weak_ordering::equivalent(private_compare::ordering_equivalent);
  inline constexpr weak_ordering weak_ordering::greater(private_compare::ordering_greater);

  //***************************************************************************
  /// Minimal strong-ordering category for C++20 three-way comparisons when the
  /// STL is unavailable.
  //***************************************************************************
  class strong_ordering
  {
  public:

    static const strong_ordering less;
    static const strong_ordering equal;
    static const strong_ordering equivalent;
    static const strong_ordering greater;

    ETL_CONSTEXPR operator partial_ordering() const ETL_NOEXCEPT
    {
      return partial_ordering(value);
    }

    ETL_CONSTEXPR operator weak_ordering() const ETL_NOEXCEPT
    {
      return weak_ordering(value);
    }

    friend ETL_CONSTEXPR bool operator==(strong_ordering lhs, strong_ordering rhs) ETL_NOEXCEPT = default;

    friend ETL_CONSTEXPR bool operator==(strong_ordering v, private_compare::only_literal_zero) ETL_NOEXCEPT
    {
      return v.value == private_compare::ordering_equivalent;
    }
    friend ETL_CONSTEXPR bool operator<(strong_ordering v, private_compare::only_literal_zero) ETL_NOEXCEPT
    {
      return v.value < private_compare::ordering_equivalent;
    }
    friend ETL_CONSTEXPR bool operator>(strong_ordering v, private_compare::only_literal_zero) ETL_NOEXCEPT
    {
      return v.value > private_compare::ordering_equivalent;
    }
    friend ETL_CONSTEXPR bool operator<=(strong_ordering v, private_compare::only_literal_zero) ETL_NOEXCEPT
    {
      return v.value <= private_compare::ordering_equivalent;
    }
    friend ETL_CONSTEXPR bool operator>=(strong_ordering v, private_compare::only_literal_zero) ETL_NOEXCEPT
    {
      return v.value >= private_compare::ordering_equivalent;
    }

    friend ETL_CONSTEXPR bool operator<(private_compare::only_literal_zero, strong_ordering v) ETL_NOEXCEPT
    {
      return v > 0;
    }
    friend ETL_CONSTEXPR bool operator>(private_compare::only_literal_zero, strong_ordering v) ETL_NOEXCEPT
    {
      return v < 0;
    }
    friend ETL_CONSTEXPR bool operator<=(private_compare::only_literal_zero, strong_ordering v) ETL_NOEXCEPT
    {
      return v >= 0;
    }
    friend ETL_CONSTEXPR bool operator>=(private_compare::only_literal_zero, strong_ordering v) ETL_NOEXCEPT
    {
      return v <= 0;
    }

  private:

    explicit ETL_CONSTEXPR strong_ordering(private_compare::ordering_value_type value_) ETL_NOEXCEPT
      : value(value_)
    {
    }

    private_compare::ordering_value_type value;
  };

  inline constexpr strong_ordering strong_ordering::less(private_compare::ordering_less);
  inline constexpr strong_ordering strong_ordering::equal(private_compare::ordering_equivalent);
  inline constexpr strong_ordering strong_ordering::equivalent(private_compare::ordering_equivalent);
  inline constexpr strong_ordering strong_ordering::greater(private_compare::ordering_greater);
  #endif

  //***************************************************************************
  /// Converts a three-way comparison result to an etl::strong_ordering.
  /// Negative values map to 'less', positive values to 'greater' and zero to
  /// 'equal'.
  //***************************************************************************
  ETL_NODISCARD ETL_CONSTEXPR inline etl::strong_ordering make_strong_ordering(int value) ETL_NOEXCEPT
  {
    return (value < 0) ? etl::strong_ordering::less : (value > 0) ? etl::strong_ordering::greater : etl::strong_ordering::equal;
  }

  //***************************************************************************
  /// Converts a three-way comparison result to an etl::weak_ordering.
  /// Negative values map to 'less', positive values to 'greater' and zero to
  /// 'equivalent'.
  //***************************************************************************
  ETL_NODISCARD ETL_CONSTEXPR inline etl::weak_ordering make_weak_ordering(int value) ETL_NOEXCEPT
  {
    return (value < 0) ? etl::weak_ordering::less : (value > 0) ? etl::weak_ordering::greater : etl::weak_ordering::equivalent;
  }

  //***************************************************************************
  /// Converts a three-way comparison result to an etl::partial_ordering.
  /// Negative values map to 'less', positive values to 'greater' and zero to
  /// 'equivalent'.
  /// Note that 'unordered' cannot be expressed by an integer comparison result,
  /// so use etl::partial_ordering::unordered directly for that case.
  //***************************************************************************
  ETL_NODISCARD ETL_CONSTEXPR inline etl::partial_ordering make_partial_ordering(int value) ETL_NOEXCEPT
  {
    return (value < 0) ? etl::partial_ordering::less : (value > 0) ? etl::partial_ordering::greater : etl::partial_ordering::equivalent;
  }
#endif

  //***************************************************************************
  /// Defines <=, >, >=, ==, !=, <=> in terms of <
  /// Default implementation of TLess is etl::less
  //***************************************************************************
  template <typename T, typename TLess = etl::less<T> >
  struct compare
  {
    enum cmp_result
    {
      Less    = -1,
      Equal   = 0,
      Greater = 1
    };

    typedef typename etl::parameter_type<T>::type first_argument_type;
    typedef typename etl::parameter_type<T>::type second_argument_type;

    static ETL_CONSTEXPR bool lt(first_argument_type lhs, second_argument_type rhs)
    {
      return TLess()(lhs, rhs);
    }

    static ETL_CONSTEXPR bool gt(first_argument_type lhs, second_argument_type rhs)
    {
      return TLess()(rhs, lhs);
    }

    static ETL_CONSTEXPR bool lte(first_argument_type lhs, second_argument_type rhs)
    {
      return !gt(lhs, rhs);
    }

    static ETL_CONSTEXPR bool gte(first_argument_type lhs, second_argument_type rhs)
    {
      return !lt(lhs, rhs);
    }

    static ETL_CONSTEXPR bool eq(first_argument_type lhs, second_argument_type rhs)
    {
      return gte(lhs, rhs) && lte(lhs, rhs);
    }

    static ETL_CONSTEXPR bool ne(first_argument_type lhs, second_argument_type rhs)
    {
      return !eq(lhs, rhs);
    }

    static ETL_CONSTEXPR cmp_result cmp(first_argument_type lhs, second_argument_type rhs)
    {
      return lt(lhs, rhs) ? Less : gt(lhs, rhs) ? Greater : Equal;
    }
  };

  //***************************************************************************
  /// Default implementation of TLess is etl::less
  //***************************************************************************
#if ETL_USING_CPP11
  template <typename T, typename TLess = etl::less<T> >
#else
  template <typename T, typename TLess>
#endif
  ETL_CONSTEXPR14 int three_way_compare(const T& lhs, const T& rhs)
  {
    return etl::compare<T, TLess>::cmp(lhs, rhs);
  }
} // namespace etl

#endif
