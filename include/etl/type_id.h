///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2026 John Wellbelove

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

#ifndef ETL_TYPE_ID_INCLUDED
#define ETL_TYPE_ID_INCLUDED

#include "platform.h"
#include "type_traits.h"

namespace etl
{
  namespace private_type_id 
  {
    template <typename T>
    struct type_id_anchor 
    {
      static char value;
    };

    template <typename T>
    char type_id_anchor<T>::value = 0;
  }

  //*************************************************************************
  /// A type identifier.
  /// Provides a way to identify types at runtime without using RTTI.
  //*************************************************************************
  class type_id 
  {
  public:

    //*************************************************************************
    /// Get the type id for the specified type.
    /// \return A type_id instance.
    //*************************************************************************
    template <typename T>
    static ETL_CONSTEXPR type_id get() ETL_NOEXCEPT 
    {
      using type = typename etl::remove_cvref<T>::type;

      return type_id(&private_type_id::type_id_anchor<type>::value);
    }

    //*************************************************************************
    /// The invalid type id.
    /// \return A type_id instance representing an invalid type id.
    //*************************************************************************
    static ETL_CONSTEXPR type_id invalid_id() ETL_NOEXCEPT
    {
      return type_id();
    }

    //*************************************************************************
    /// Default constructor.
    /// Sets the value of the type id to Invalid_Type_Id
    //*************************************************************************
    ETL_CONSTEXPR type_id() ETL_NOEXCEPT
      : id(ETL_NULLPTR)
    {
    }

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    type_id(const type_id& other) ETL_NOEXCEPT
      : id(other.id)
    {
    }

    //*************************************************************************
    /// Assignment operator
    /// \param other The other type_id
    /// \return Reference to the type_id
    //*************************************************************************
    type_id& operator =(const type_id& other) ETL_NOEXCEPT 
    {
      id = other.id;
      return *this;
    }

    //*************************************************************************
    /// Equality operator
    /// \param rhs The other type_id to compare with
    /// \return <b>true</b> if the type_id is equal
    //*************************************************************************
    ETL_CONSTEXPR bool operator ==(const type_id& rhs) const ETL_NOEXCEPT 
    {
      return id == rhs.id;
    }

    //*************************************************************************
    /// Inequality operator
    /// \param rhs The other type_id to compare with
    /// \return <b>true</b> if the type_id is not equal

    //*************************************************************************
    ETL_CONSTEXPR bool operator !=(const type_id& rhs) const ETL_NOEXCEPT 
    {
      return !(id == rhs.id);
    }

    //*************************************************************************
    /// Less-than operator
    /// \param rhs The other type_id to compare with
    /// \return <b>true</b> if the this type_id is less than rhs
    //*************************************************************************
    bool operator <(const type_id& rhs) const ETL_NOEXCEPT 
    {
      return id < rhs.id;
    }

    //*************************************************************************
    /// Conversion operator to intptr_t
    /// Explicit for C++11 and above
    //*************************************************************************
    ETL_EXPLICIT operator intptr_t() const ETL_NOEXCEPT 
    {
      return reinterpret_cast<intptr_t>(id);
    }

    //*************************************************************************
    /// Conversion operator to uintptr_t
    /// Explicit for C++11 and above
    ///    //*************************************************************************
    ETL_EXPLICIT operator uintptr_t() const ETL_NOEXCEPT 
    {
      return reinterpret_cast<uintptr_t>(id);
    }

    //*************************************************************************
    /// Conversion operator to bool
    /// Explicit for C++11 and above
    /// \return <b>false</b> if the type_id is equal to <code>etl::type_id::Invalid_Type_Id</code>
    //*************************************************************************
    ETL_EXPLICIT ETL_CONSTEXPR operator bool() const ETL_NOEXCEPT 
    {
      return id != ETL_NULLPTR;
    }

  private:

    // Private construct from a void*
    ETL_CONSTEXPR type_id(void* id_) ETL_NOEXCEPT
      : id(id_)
    {
    }

    // The id as a void*
    const void* id;
  };
}

#endif
