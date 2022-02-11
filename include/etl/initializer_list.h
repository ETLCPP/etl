///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2022 jwellbelove

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

#ifndef ETL_INITIALIZER_LIST_INCLUDED
#define ETL_INITIALIZER_LIST_INCLUDED

#include "platform.h"

#if ETL_CPP11_SUPPORTED

#include <stddef.h>

#if ETL_USING_STL

	#include <initializer_list>

#else

namespace std
{
#if defined(ETL_COMPILER_MICROSOFT)
  ///**************************************************************************
  /// A definition of initializer_list that is compatible with the Microsoft compiler
  ///**************************************************************************
  template<typename T>
  class initializer_list
  {
  public:

    using value_type      = T;
    using reference       = const T&;
    using const_reference = const T&;
    using size_type       = size_t;
    using iterator        = const T*;
    using const_iterator  = const T*;

    constexpr initializer_list() noexcept : m_first(nullptr), m_last(nullptr)
    {
    }

    constexpr initializer_list(const T* first, const T* last) noexcept
      : m_first(first), m_last(last) 
    {
    }

    constexpr const T* begin() const noexcept 
    { 
      return m_first; 
    }

    constexpr const T* end() const noexcept 
    { 
      return m_last; 
    }

    constexpr size_t size() const noexcept
    {
      return static_cast<size_t>(m_last - m_first);
    }

  private:

    const T* m_first;
    const T* m_last;
  };

  template<typename T>
  constexpr const T* begin(initializer_list<T> il) noexcept
  {
    return il.begin();
  }

  template<typename T>
  constexpr const T* end(initializer_list<T> il) noexcept
  {
    return il.end();
  }

#elif defined(ETL_COMPILER_GCC) || defined(ETL_COMPILER_CLANG) || defined(ETL_COMPILER_ARM6) || defined(ETL_COMPILER_ARM7)

  ///**************************************************************************
  /// A definition of initializer_list that is compatible with Clang, GCC and related compilers.
  ///**************************************************************************
  template<class T>
  class initializer_list
  {
  public:

    using value_type      = T;
    using reference       = const T&;
    using const_reference = const T&;
    using size_type       = size_t;
    using iterator        = const T*;
    using const_iterator  = const T*;

    constexpr initializer_list() noexcept : m_begin(nullptr), m_size(0)
    {
    }

    constexpr size_t size()  const noexcept 
    { 
      return m_size; 
    }

    constexpr const T* begin() const noexcept 
    { 
      return m_begin; 
    }

    constexpr const T* end()   const noexcept 
    { 
      return m_begin + m_size; 
    }

  private:

    constexpr initializer_list(const T* b, size_t s) noexcept
      : m_begin(b)
      , m_size(s)
    {
    }

    const T* m_begin;
    size_t   m_size;
  };

  template<class T>
  constexpr const T* begin(initializer_list<T> init) noexcept
  {
    return init.begin();
  }

  template<class T>
  constexpr const T* end(initializer_list<T> init) noexcept
  {
    return init.end();
  }
#else
  #error No definition for initializer_list is currently available.
#endif // Compiler tests
}
#endif // ETL_USING_STL
#else
  #error initializer_list is only supported for C++11 or above.
#endif // ETL_CPP11_SUPPORTED
#endif // ETL_INITIALIZER_LIST_INCLUDED