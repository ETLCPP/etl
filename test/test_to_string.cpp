/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2019 jwellbelove

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

#include "UnitTest++.h"

#include <string>

#include "etl/to_string.h"
#include "etl/cstring.h"

#undef STR
#define STR(x) x

namespace
{
  /* A utility function to reverse a string  */
  void reverse(char str[], int length)
  {
    int start = 0;
    int end = length - 1;
    while (start < end)
    {
      std::swap(*(str + start), *(str + end));
      start++;
      end--;
    }
  }

  // Implementation of itoa()
  char* itoa(int num, char* str, int base)
  {
    int i = 0;
    bool isNegative = false;

    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0)
    {
      str[i++] = '0';
      str[i] = '\0';
      return str;
    }

    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
      isNegative = true;
      num = -num;
    }

    // Process individual digits
    while (num != 0)
    {
      int rem = num % base;
      str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
      num = num / base;
    }

    // If number is negative, append '-'
    if (isNegative)
      str[i++] = '-';

    str[i] = '\0'; // Append string terminator

    // Reverse the string
    reverse(str, i);

    return str;
  }

  SUITE(test_string_char)
  {
    //*************************************************************************
    TEST(test_x)
    {
      //char str[100];

      etl::string<10> str;

      etl::to_string(char(127), str, 10);

      str.clear();
      etl::to_string(char(-128), str, 10);
      //itoa(-1567, str, 10);
      //itoa(1567, str, 2);
      //itoa(1567, str, 8);
      //itoa(1567, str, 16);
    }
  };
}
