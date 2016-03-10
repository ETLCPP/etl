/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

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

#include <UnitTest++/UnitTest++.h>
#if defined(ETL_PLATFORM_WINDOWS)
#include <Windows.h>
#endif
#include <sstream>
#include <string>

#include "../src/error_handler.h"
#include "../src/exception.h"

bool error_received;

//*****************************************************************************
// An exception.
//*****************************************************************************
class test_exception : public etl::exception
{
public:

  test_exception(string_type file_name, numeric_type line_number)
    : exception(ETL_ERROR_TEXT("test_exception", "123"), file_name, line_number)
  {
    error_received = false;
  }
};

//*****************************************************************************
// A free error handler function.
//*****************************************************************************
void receive_error(const etl::exception& e)
{
  error_received = true;
  std::ostringstream oss;
  oss << "Error '" << e.what() << "' in " << e.file_name() << " at line " << e.line_number() << "\n";

#if defined(ETL_PLATFORM_WINDOWS) && defined(ETL_COMPILER_MICROSOFT)
  std::string stext = oss.str();

  WCHAR text[200];
  MultiByteToWideChar(0, 0, stext.c_str(), stext.size() + 1, text, 200);
  LPCWSTR ltext = text;

  OutputDebugString(ltext);
#endif
}

//*****************************************************************************
class test_class
{
public:

  //***************************************************************************
  // A member error handler function.
  //***************************************************************************
  void receive_error(const etl::exception& e)
  {
    error_received = true;
    std::ostringstream oss;
    oss << "Error '" << e.what() << "' in " << e.file_name() << " at line " << e.line_number() << "\n";

#if defined(ETL_PLATFORM_WINDOWS) && defined(ETL_COMPILER_MICROSOFT)
    std::string stext = oss.str();

    WCHAR text[200];
    MultiByteToWideChar(0, 0, stext.c_str(), stext.size() + 1, text, 200);
    LPCWSTR ltext = text;

    OutputDebugString(ltext);
#endif
  }
};

namespace
{
  SUITE(test_error_handler)
  {
    //*************************************************************************
    TEST(test_free_handler_function)
    {
      // Create the function callback object.
      etl::error_handler::free_function error_callback(receive_error);

      // Tell the error handler about it.
      etl::error_handler::set_callback(error_callback);

      // Log an error.
      etl::error_handler::error(ETL_ERROR(test_exception));

      CHECK(error_received);
    }

    //*************************************************************************
    TEST(test_member_handler_function)
    {
      // Create the class that contains the handler.
      test_class test;

      // Create the function callback object.
      etl::error_handler::member_function<test_class> error_callback(test, &test_class::receive_error);

      // Tell the error handler about it.
      etl::error_handler::set_callback(error_callback);

      // Log an error.
      etl::error_handler::error(ETL_ERROR(test_exception));

      CHECK(error_received);
    }
  };
}
