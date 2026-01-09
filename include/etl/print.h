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

#ifndef ETL_PRINT_INCLUDED
#define ETL_PRINT_INCLUDED

#include "platform.h"

#include "format.h"

#if ETL_USING_CPP11

// to be implemented in a concrete project, typically printing to a serial console
// type int here is the convention from putchar(), actually storing char
extern "C" void etl_putchar(int c);

namespace etl
{
  namespace private_print
  {
    using char_type = etl::private_format::char_type;

    // No-op iterator that forwards all assignments to etl_putchar
    class print_iterator
    {
    public:
      class print_to
      {
      public:
        print_to& operator=(char_type c)
        {
          etl_putchar(static_cast<int>(c));
          return *this;
        }
      };

      print_iterator()
      {
      }

      print_iterator(const print_iterator&)
      {
      }

      print_iterator& operator=(const print_iterator&)
      {
        return *this;
      }

      print_to operator*()
      {
        return print_to();
      }

      print_iterator& operator++()
      {
        return *this;
      }

      print_iterator operator++(int)
      {
        return *this;
      }
    };
  }  // namespace private_print

  template <class... Args>
  void print(etl::format_string<Args...> fmt, Args&&... args)
  {
    private_print::print_iterator it;
    (void)format_to(it, etl::move(fmt), etl::forward<Args>(args)...);
  }

  inline void println()
  {
    etl_putchar(static_cast<int>('\n'));
  }

  template <class... Args>
  void println(etl::format_string<Args...> fmt, Args&&... args)
  {
    private_print::print_iterator it;
    (void)format_to(it, etl::move(fmt), etl::forward<Args>(args)...);
    println();
  }
}  // namespace etl

#endif

#endif
