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

#ifndef ETL_PRIVATE_META_STL_H_INCLUDED
#define ETL_PRIVATE_META_STL_H_INCLUDED

#include <cstddef>
#include <initializer_list>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include <meta>
#include <source_location> // Workaround for GCC 16: <meta> depends on <source_location> but does not include it.

///\defgroup meta meta
///\ingroup etl

namespace etl
{
  namespace meta
  {
    //*************************************************************************
    // Standard <meta> path, used when the full standard library is available.
    //
    // A using-directive is sufficient here: qualified lookup for
    // etl::meta::foo also searches the namespaces nominated by the
    // using-directives of etl::meta ([namespace.qual]), so every std::meta
    // name - including the function templates such as extract<> - is
    // reachable as etl::meta::name.  This mirrors the non-STL implementation
    // in gcc.h, which uses the same mechanism.
    //*************************************************************************
    using namespace std::meta;

    // Enumerators of std::meta::operators (op_equals_equals, etc.) are scoped,
    // so they need a using-enum-declaration rather than the using-directive.
    using enum std::meta::operators;

    //*************************************************************************
    /// info_array — fixed-size, zero-allocation array of info values.
    ///
    /// GCC limitation: template-for over a std::vector<info> fails because the
    /// vector's heap allocation cannot persist in a constant expression.  The
    /// workaround is to copy the elements into a plain aggregate array (no
    /// allocation) and iterate that instead.
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
      for (size_t i = 0; i < N; ++i)
      {
        a.elems[i] = v[i];
      }
      return a;
    }

  } // namespace meta
} // namespace etl

#endif // ETL_PRIVATE_META_STL_H_INCLUDED
