///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 John Wellbelove

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

#ifndef ETL_NULLPTR_INCLUDED
#define ETL_NULLPTR_INCLUDED

#include "platform.h"

#include <stddef.h>

namespace etl
{
#if ETL_CPP11_NOT_SUPPORTED
  class nullptr_t
  {
  public:
    template <class T>
    inline operator T*() const { return 0; }

    template <class C, class T>
    inline operator T C::* () const { return 0; }
    
    inline bool operator==(nullptr_t) const { return true; }
    inline bool operator!=(nullptr_t) const { return false; }
  private:
    void operator&() const ETL_DELETE; // cannot take the address of ETL_NULLPTR
  };
  
  static const nullptr_t _nullptr = nullptr_t();

  #define ETL_NULLPTR (etl::_nullptr)
#else
  // Use the new style nullptr.
  typedef decltype(nullptr) nullptr_t;
  #define ETL_NULLPTR nullptr
#endif
}

#endif

