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

#ifndef __ETL_SMALLEST__
#define __ETL_SMALLEST__

#include <stdint.h>

#include "integral_limits.h"

///\defgroup smallest smallest
///\ingroup utilities

namespace etl
{
  //***************************************************************************
  /// Template to determine the smallest type and size.
  /// Supports up to 16 types.
  /// Defines 'value_type' which is the type of the smallest parameter.
  /// Defines 'size' which is the size of the smallest parameter.
  ///\ingroup smallest
  //***************************************************************************
  template <typename T1,         typename T2  = void, typename T3  = void, typename T4  = void,
            typename T5  = void, typename T6  = void, typename T7  = void, typename T8  = void,
            typename T9  = void, typename T10 = void, typename T11 = void, typename T12 = void,
            typename T13 = void, typename T14 = void, typename T15 = void, typename T16 = void>
  struct smallest_type
  {
  private:

    // Declaration.
    template <const bool Boolean, typename TrueType, typename FalseType>
    struct choose_type;

    // Specialisation for 'true'.
    // Defines 'type' as 'TrueType'.
    template <typename TrueType, typename FalseType>
    struct choose_type<true, TrueType, FalseType>
    {
      typedef TrueType type;
    };

    // Specialisation for 'false'. 
    // Defines 'type' as 'FalseType'.
    template <typename TrueType, typename FalseType>
    struct choose_type<false, TrueType, FalseType>
    {
      typedef FalseType type;
    };

  public:

    // Define 'smallest_other' as 'smallest_type' with all but the first parameter. 
    typedef typename smallest_type<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16>::type smallest_other;

    // Set 'type' to be the smallest of the first parameter and any of the others.
    // This is recursive.
    typedef typename choose_type<(sizeof(T1) < sizeof(smallest_other)),// Boolean
                                  T1,                                  // TrueType
                                  smallest_other>                      // FalseType
                                  ::type type;                         // The smallest type of the two.

    // The size of the smallest type.
    enum
    {
      size = sizeof(type)
    };
  };

  //***************************************************************************
  // Specialisation for one template parameter.
  //***************************************************************************
  template <typename T1>
  struct smallest_type <T1,   void, void, void,
                        void, void, void, void,
                        void, void, void, void,
                        void, void, void, void>
  {
    typedef T1 type;

    enum
    {
      size = sizeof(type)
    };
  };
  
  namespace __private_smallest__
  {
    //*************************************************************************
    // Determine the type to hold the number of bits based on the index.
    //*************************************************************************
    template <const int index>
    struct best_fit_uint_type;
    
    //*************************************************************************
    // Less than or equal to 8 bits.
    //*************************************************************************
    template <>
    struct best_fit_uint_type<0>
    {
      typedef uint_least8_t type;
    };

    //*************************************************************************
    // 9 to 16 bits.
    //*************************************************************************
    template <>
    struct best_fit_uint_type<1>
    {
      typedef uint_least16_t type;
    };

    //*************************************************************************
    // 17 to 31 bits.
    //*************************************************************************
    template <>
    struct best_fit_uint_type<2>
    {
      typedef uint_least32_t type;
    };

    //*************************************************************************
    // Greater than 32 bits.
    //*************************************************************************
    template <>
    struct best_fit_uint_type<3>
    {
      typedef uint_least64_t type;
    };
  }

  //***************************************************************************
  /// Template to determine the smallest unsigned int type that can contain a 
  /// value with the specified number of bits.
  /// Defines 'type' which is the type of the smallest unsigned integer.
  ///\ingroup smallest
  //***************************************************************************
  template <const size_t N>
  struct smallest_uint_for_bits
  {
  private:
    
    // Determines the index of the best unsigned type for the required number of bits.
    static const int TYPE_INDEX = ((N >  8) ? 1 : 0) + ((N > 16) ? 1 : 0) + ((N > 32) ? 1 : 0);

  public:

    typedef typename __private_smallest__::best_fit_uint_type<TYPE_INDEX>::type type;
  };
}

#endif
