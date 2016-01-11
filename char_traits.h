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

#ifndef __ETL_CHAR_TRAITS__
#define __ETL_CHAR_TRAITS__

#include "stdint.h"
#include "algorithms.h"

//*****************************************************************************
///\defgroup char_traits char_traits
/// Character traits
///\ingroup string
//*****************************************************************************

namespace etl
{
  template<typename T> struct char_traits;
  
  //***************************************************************************
  /// Specialisation for char.
  //***************************************************************************
  template<> struct char_traits<char>
  {
    typedef char      char_type;
    typedef int       int_type;
    typedef long long off_type;
    typedef size_t    pos_type;
    typedef char      state_type;
    
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
    static size_t length(const char* str)
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
      r = a;
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
    static char_type* move(char_type* dest, const char_type* src, std::size_t count)
    {
      if ((dest < src) || (dest > (src + count))
      {
        etl::copy_n(src, src + count, dest);
      }
      else
      {
        etl::copy_n(std::reverse_iterator(src + count), 
                    std::reverse_iterator(src), 
                    std::reverse_iterator(dest + count));  
      }
    }
  };
  
  //***************************************************************************
  /// Specialisation for wchar_t
  //***************************************************************************
  template <> struct char_traits<wchar_t>
  {
    typedef wchar_t   char_type;
    typedef wchar_t   int_type;
    typedef long long off_type;
    typedef size_t    pos_type;
    typedef char      state_type;
      
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
    static size_t length(const wchar_t* str)
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
      r = a;
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
    static char_type* move(char_type* dest, const char_type* src, std::size_t count)
    {
      if ((dest < src) || (dest > (src + count))
      {
        etl::copy_n(src, src + count, dest);
      }
      else
      {
        etl::copy_n(std::reverse_iterator(src + count), 
                    std::reverse_iterator(src), 
                    std::reverse_iterator(dest + count));   
      }
    }
  };
  
  //***************************************************************************
  /// Specialisation for char16_t.
  //***************************************************************************
  template <> struct char_traits<char16_t>
  {  
    typedef char16_t       char_type;
    typedef uint_least16_t int_type;
    typedef long long      off_type;
    typedef size_t         pos_type;
    typedef char           state_type;      
      
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
    static size_t length(const char16_t* str)
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
      r = a;
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
    static char_type* move(char_type* dest, const char_type* src, std::size_t count)
    {
      if ((dest < src) || (dest > (src + count))
      {
        etl::copy_n(src, src + count, dest);
      }
      else
      {
        etl::copy_n(std::reverse_iterator(src + count), 
                    std::reverse_iterator(src), 
                    std::reverse_iterator(dest + count));   
      }
    }    
  }

  //***************************************************************************
  /// Specialisation for char32_t.
  //***************************************************************************
  template <> struct char_traits<char32_t>
  {
    typedef char16_t       char_type;
    typedef uint_least32_t int_type;
    typedef long long      off_type;
    typedef size_t         pos_type;
    typedef char           state_type;        
      
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
    static size_t length(const char32_t* str)
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
      r = a;
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
    static char_type* move(char_type* dest, const char_type* src, std::size_t count)
    {
      if ((dest < src) || (dest > (src + count))
      {
        etl::copy_n(src, src + count, dest);
      }
      else
      {
        etl::copy_n(std::reverse_iterator(src + count), 
                    std::reverse_iterator(src), 
                    std::reverse_iterator(dest + count));   
      }
    }
  }
}

#endif
