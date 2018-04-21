///\file

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

#ifndef ETL_ENUM_TYPE_INCLUDED
#define ETL_ENUM_TYPE_INCLUDED

#include "platform.h"

///\defgroup enum_type enum_type
/// Smart enumerations.<br>
/// A method of declaring enumerations that allow grouping within a structure.
/// Avoids the problem of clashing names that can occur with standard enumerations.
/// One way to think of the code is as a type with built-in constants and an optional conversion to a string.<br><br>
/// <b>Declaring the enumeration.</b>
///\code
/// struct CompassDirection
/// {
///   enum enum_type
///   {
///     North = 0,
///     South = 180,
///     East  = 90,
///     West  = 270
///   };
///
///   ETL_DECLARE_ENUM_TYPE(CompassDirection, int)
///   ETL_ENUM_TYPE(North, "North")
///   ETL_ENUM_TYPE(South, "South")
///   ETL_ENUM_TYPE(East,  "East")
///   ETL_ENUM_TYPE(West,  "West")
///   ETL_END_ENUM_TYPE
/// };
///\endcode
/// <b>Using the enumeration.</b>
///\code
/// CompassDirection direction;          // Default construction.
///
/// direction = CompassDirection::North; // Assignment from an enumeration constant;
///
/// int value = direction;               // Implicit conversion to 'int'.
///
/// direction = CompassDirection(value); // Explicit conversion from 'int'.
///
/// direction = CompassDirection(3);     // Explicit conversion from an invalid value. This unfortunately cannot be avoided. Caveat emptor!
///
/// direction = value;                   // Implicit conversion from 'int'. **** Compilation error ****
///
/// std::cout << "Direction = " << direction.c_str(); // Prints "Direction = North"
///\endcode
/// If a conversion to a string is not required then the 'ETL_ENUM_TYPE' declaration may be omitted.
/// In that case the c_str() function will return a "?". This will also be the case for any
/// enumeration value that does not have an ETL_ENUM_TYPE entry.
///\ingroup utilities

//*****************************************************************************
// The declaration of the member functions and the first section of the 'c_str' function.
//*****************************************************************************
#define ETL_DECLARE_ENUM_TYPE(TypeName, ValueType) \
  typedef ValueType value_type; \
	TypeName() {} \
	TypeName(const TypeName &other) : value(other.value) {} \
	TypeName(enum_type value_) : value(value_) {} \
  TypeName& operator=(const TypeName &other) {value = other.value; return *this;} \
  explicit TypeName(value_type value_) : value(static_cast<enum_type>(value_)) {} \
	operator value_type() const {return static_cast<value_type>(value);} \
  value_type get_value() const {return static_cast<value_type>(value);} \
  enum_type get_enum() const {return value;} \
  const char* c_str() const \
  { \
    switch (value) \
    {

//*****************************************************************************
// A case in the 'c_str' function's switch statement.
//*****************************************************************************
#define ETL_ENUM_TYPE(value, name) \
      case value: \
        return name; \

//*****************************************************************************
// The final section of the 'c_str' function and the value declaration.
//*****************************************************************************
#define ETL_END_ENUM_TYPE \
      default: \
        return "?"; \
    } \
  } \
private: \
  enum_type value;

#endif
