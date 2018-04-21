///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2017 jwellbelove

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

#ifndef ETL_USER_TYPE_INCLUDED
#define ETL_USER_TYPE_INCLUDED

#include "platform.h"

///\defgroup user_type user_type
/// Smart enumerations.<br>
/// A method of declaring a user type that also contains a set of constants,
/// but are not constrained to just those values.
/// This contrasts with 'enum_type', where the values are expected to <b>only</b> contain
/// those defined as constants.
/// <b>Declaring the enumeration.</b>
///\code
///   ETL_DECLARE_USER_TYPE(CompassDirection, int)
///   ETL_USER_TYPE(North, 0)
///   ETL_USER_TYPE(South, 180)
///   ETL_USER_TYPE(East,  90)
///   ETL_USER_TYPE(West,  270)
///   ETL_END_USER_TYPE(CompassDirection)
///\endcode
/// <b>Using the enumeration.</b>
///\code
/// CompassDirection direction;          // Default construction.
///
/// direction = CompassDirection::North; // Assignment from an enumeration constant;
///
/// int value = int(direction);          // Explicit conversion to 'int'.
/// int value = direction.get();
///
/// int& value = direction.get();        // Bind to internal value.
/// const int& value = direction.get();
///
/// direction = CompassDirection(value); // Explicit conversion from 'int'.
///
/// direction = CompassDirection(3);     // Explicit conversion from a value.
///
/// ++direction;                         // Manipulate the value;
/// direction -= CompassDirection(20);
///
/// direction = value;                   // Implicit conversion from 'int'. **** Compilation error ****
///
///\endcode
///\ingroup utilities

//*****************************************************************************
// The declaration of the structure.
//*****************************************************************************
#define ETL_DECLARE_USER_TYPE(TypeName, ValueType) \
  struct TypeName \
  { \
    /* Non-volatile definitions.*/ \
    typedef ValueType value_type; \
    TypeName() {} \
    TypeName(const TypeName &other) : value(other.value) {} \
    TypeName& operator=(const TypeName &other) { value = other.value; return *this; } \
    explicit TypeName(ValueType value_) : value(value_) {} \
    operator ValueType() const { return value; } \
    ValueType& get() { return value; } \
    const ValueType& get() const { return value; } \
    TypeName& operator ++() { ++value; return *this; } \
    TypeName operator ++(int) { TypeName temp(*this); TypeName::operator ++(); return temp; } \
    TypeName& operator --() { --value; return *this; } \
    TypeName operator --(int) { TypeName temp(*this); TypeName::operator --(); return temp; } \
    TypeName& operator +=(const ValueType& rhs) { value += rhs; return *this; } \
    TypeName& operator -=(const ValueType& rhs) { value -= rhs; return *this; } \
    TypeName& operator *=(const ValueType& rhs) { value *= rhs; return *this; } \
    TypeName& operator /=(const ValueType& rhs) { value /= rhs; return *this; } \
    TypeName& operator %=(const ValueType& rhs) { value %= rhs; return *this; } \
    TypeName& operator &=(const ValueType& rhs) { value &= rhs; return *this; } \
    TypeName& operator |=(const ValueType& rhs) { value |= rhs; return *this; } \
    TypeName& operator ^=(const ValueType& rhs) { value ^= rhs; return *this; } \
    TypeName& operator <<=(ValueType distance) { value <<= distance; return *this; } \
    TypeName& operator >>=(ValueType distance) { value >>= distance; return *this; } \
  private: \
    ValueType value; \
  public: \
    enum enum_type \
    {

//*****************************************************************************
// The predefined constants.
//*****************************************************************************
#define ETL_USER_TYPE(enum_name, value) \
      enum_name = value,

//*****************************************************************************
// The final section of the structure.
//*****************************************************************************
#define ETL_END_USER_TYPE(TypeName) \
    }; \
    TypeName(enum_type value_) : value(static_cast<value_type>(value_)) {} \
  };


#endif
