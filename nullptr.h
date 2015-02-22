///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl

Copyright(c) 2014 jwellbelove

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

#ifndef __ETL_NULLPTR__
#define __ETL_NULLPTR__

///\defgroup nullptr nullptr
/// A definition of nullptr for compilers that don't support it as standard.
///\ingroup utilities

#if (defined(_MSC_VER) && (_MSC_VER < 1600)) || defined(COMPILER_KEIL) || defined(COMPILER_IAR)
namespace std
{
  //*****************************************************************************
  /// A null pointer type.
  ///\ingroup nullptr
  //*****************************************************************************
  class nullptr_t
  {
  public:

    // Convertible to any type of null non-member pointer.
    template<class T>
    operator T*() const
    {
      return 0;
    }

    // Or any type of null member pointer.
    template<class C, class T>
    operator T C::*() const
    {
      return 0;
    }

  private:

    // Can't take address of nullptr.
    void operator&() const;
  };

  //*****************************************************************************
  /// A null pointer.
  ///\ingroup nullptr
  //*****************************************************************************
  const nullptr_t nullptr = {};
}

//*****************************************************************************
/// A null pointer type.
///\ingroup nullptr
//*****************************************************************************
class nullptr_t
{
public:

  // Convertible to any type of null non-member pointer.
  template<class T>
  operator T*() const
  {
    return 0;
  }

  // Or any type of null member pointer.
  template<class C, class T>
  operator T C::*() const
  {
    return 0;
  }

private:

  // Can't take address of nullptr.
  void operator&() const;
};

//*****************************************************************************
/// A null pointer.
///\ingroup nullptr
//*****************************************************************************
const nullptr_t nullptr = {};

#endif
#endif

