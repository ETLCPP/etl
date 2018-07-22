///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2016 jwellbelove

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

#ifndef ETL_CHAR_TRAITS_INCLUDED
#define ETL_CHAR_TRAITS_INCLUDED

#include "platform.h"
#include "stdint.h"
#include "algorithm.h"

#include "stl/algorithm.h"
#include "stl/iterator.h"

//*****************************************************************************
///\defgroup char_traits char_traits
/// Character traits
///\ingroup string
//*****************************************************************************

// Define the large character types if necessary.
#if (ETL_NO_LARGE_CHAR_SUPPORT)
typedef int16_t char16_t;
typedef int32_t char32_t;
#endif

namespace etl
{
  template<typename T> struct char_traits_types;
  
  template<> struct char_traits_types<char>
  {
    typedef char      char_type;
    typedef int       int_type;
    typedef long long off_type;
    typedef size_t    pos_type;
    typedef char      state_type;
  };
  
  template<> struct char_traits_types<wchar_t>
  {
    typedef wchar_t   char_type;
    typedef wchar_t   int_type;
    typedef long long off_type;
    typedef size_t    pos_type;
    typedef char      state_type;
  };
  
  template<> struct char_traits_types<char16_t>
  {
    typedef char16_t       char_type;
    typedef uint_least16_t int_type;
    typedef long long      off_type;
    typedef size_t         pos_type;
    typedef char           state_type; 
  };
  
  template<> struct char_traits_types<char32_t>
  {
    typedef char32_t       char_type;
    typedef uint_least32_t int_type;
    typedef long long      off_type;
    typedef size_t         pos_type;
    typedef char           state_type;  
  };
  
  //***************************************************************************
  /// Character traits for any character type.
  //***************************************************************************
  template<typename T>
  struct char_traits : public char_traits_types<T>
  {   
    typedef typename char_traits_types<T>::char_type  char_type;
    typedef typename char_traits_types<T>::int_type   int_type;
    typedef typename char_traits_types<T>::off_type   off_type;
    typedef typename char_traits_types<T>::pos_type   pos_type;
    typedef typename char_traits_types<T>::state_type state_type;
    
    //*************************************************************************
    static bool eq(char_type a, char_type b)
    {
      return a == b;
    }
    
    //*************************************************************************
    static bool lt(char_type a, char_type b)
    {
      return a < b;
    }     
    
    //*************************************************************************
    static size_t length(const char_type* str)
    {
      size_t count = 0;
          
      if (str != 0)
      {
        while (*str++ != 0)
        {
          ++count;
        }
      }
      
      return count;
    }
    
    //*************************************************************************
    static void assign(char_type& r, const char_type& c)
    {
      r = c;
    }
    
    //*************************************************************************
    static char_type* assign(char_type* p, size_t n, char_type c)
    {
      if (p != 0)
      {
        std::fill_n(p, n, c);
      }
      
      return p;
    }
    
    //*************************************************************************
    static char_type* move(char_type* dest, const char_type* src, size_t count)
    {
      if ((dest < src) || (dest > (src + count)))
      {
        etl::copy_n(src, src + count, dest);
      }
      else
      {
        etl::copy_n(std::reverse_iterator<char_type*>(src + count),
                    count,
                    std::reverse_iterator<char_type*>(dest + count));
      }
      
      return dest;
    }
    
    //*************************************************************************
    static char_type* copy(char_type* dest, const char_type* src, size_t count)
    {
      etl::copy_n(src, src + count, dest);
      
      return dest;
    }
    
    //*************************************************************************
    static int compare(const char_type* s1, const char_type* s2, size_t count)
    {       
      for (size_t i = 0; i < count; ++i)
      {
        if (*s1 < *s2)
        {
          return -1;
        }
          else if (*s1 > *s2)
        {
          return 1;
        }
        
        ++s1;
        ++s2;
      }
        
      return 0;
    }
    
    //*************************************************************************
    static const char_type* find(const char_type* p, size_t count, const char_type& ch)
    {     
      for (size_t i = 0; i < count; ++i)
      {
        if (*p == ch)
        {
          return p;
        }
        
        ++p;
      }
      
      return 0;
    }
    
    //*************************************************************************
    static char_type to_char_type(int_type c)
    {
      return static_cast<char_type>(c);
    }
    
    //*************************************************************************
    static int_type to_int_type(char_type c)
    {
      return static_cast<int_type>(c);
    }
    
    //*************************************************************************
    static bool eq_int_type(int_type c1, int_type c2)
    {
      return (c1 == c2);
    }
    
    //*************************************************************************
    static int_type eof()
    {
      return -1;
    }
    
    //*************************************************************************
    static int_type not_eof(int_type e)
    {
      return (e == eof()) ? eof() - 1 : e;
    }
  };
}

#endif
