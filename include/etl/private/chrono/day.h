///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2023 John Wellbelove

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

#ifndef ETL_CHRONO_DAY_INCLUDED
#define ETL_CHRONO_DAY_INCLUDED

#include "../../platform.h"
#include "../../hash.h"

#include "duration.h"

#include <stdint.h>

namespace etl
{
  namespace chrono
  {
    //***********************************************************************
    /// day
    //***********************************************************************
    class day
    {
    public:

      //***********************************************************************
      /// Default constructor
      //***********************************************************************
      ETL_CONSTEXPR day()
        : value(0)
      {
      }

      //***********************************************************************
      /// Construct from unsigned
      //***********************************************************************
      ETL_CONSTEXPR explicit day(unsigned value_)
        : value(value_)
      {
      }

      //***********************************************************************
      /// Copy constructor
      //***********************************************************************
      ETL_CONSTEXPR day(const etl::chrono::day& other)
        : value(other.value)
      {
      }

      //***********************************************************************
      /// Assignment operator
      //***********************************************************************
      ETL_CONSTEXPR etl::chrono::day& operator =(const etl::chrono::day& rhs)
      {
        value = rhs.value;

        return *this;
      }

      //***********************************************************************
      /// Pre-increment operator
      //***********************************************************************
      ETL_CONSTEXPR etl::chrono::day& operator ++() ETL_NOEXCEPT
      {
        ++value;

        return *this;
      }

      //***********************************************************************
      /// Post-increment operator
      //***********************************************************************
      ETL_CONSTEXPR14 etl::chrono::day operator ++(int) ETL_NOEXCEPT
      {
        const etl::chrono::day temp = *this;
        ++value;

        return temp;
      }

      //***********************************************************************
      /// Pre-decrement operator
      //***********************************************************************
      ETL_CONSTEXPR etl::chrono::day& operator --() ETL_NOEXCEPT
      {
        --value;

        return *this;
      }

      //***********************************************************************
      /// Post-decrement operator
      //***********************************************************************
      ETL_CONSTEXPR14 etl::chrono::day operator --(int) ETL_NOEXCEPT
      {
        const etl::chrono::day temp = *this;
        --value;

        return temp;
      }

      //***********************************************************************
      /// Plus-equals operator adding etl::chrono::days
      //***********************************************************************
      ETL_CONSTEXPR etl::chrono::day& operator +=(const etl::chrono::days& ds) ETL_NOEXCEPT
      {
        value += static_cast<unsigned char>(ds.count());

        return *this;
      }

      //***********************************************************************
      /// Minus-equals operator subtracting etl::chrono::days
      //***********************************************************************
      ETL_CONSTEXPR etl::chrono::day& operator -=(const etl::chrono::days& ds) ETL_NOEXCEPT
      {
        value -= static_cast<unsigned char>(ds.count());

        return *this;
      }

      //***********************************************************************
      /// Returns <b>true</b> if the day is within the valid 1 to 31 range
      //***********************************************************************
      ETL_CONSTEXPR bool ok() const ETL_NOEXCEPT
      {
        return (value >= 1U) && (value <= 31U);
      }

      //***********************************************************************
      /// Conversion operator to unsigned int
      //***********************************************************************
      ETL_CONSTEXPR operator unsigned() const ETL_NOEXCEPT
      {
        return static_cast<unsigned>(value);
      }

      //***********************************************************************
      /// The minimum day value for which ok() will return <b>true</b>
      //***********************************************************************
      static ETL_NODISCARD ETL_CONSTEXPR etl::chrono::day min() ETL_NOEXCEPT
      {
        return etl::chrono::day(1);
      }

      //***********************************************************************
      /// The maximum day value for which ok() will return <b>true</b>
      //***********************************************************************
      static ETL_NODISCARD ETL_CONSTEXPR etl::chrono::day max() ETL_NOEXCEPT
      {
        return etl::chrono::day(31);
      }

    private:

      unsigned char value;
    };

    //***********************************************************************
    /// Add etl::chrono::days to etl::chrono::day
    ///\return etl::chrono::day
    //***********************************************************************
    ETL_CONSTEXPR etl::chrono::day operator +(const etl::chrono::day& d, const etl::chrono::days& ds) ETL_NOEXCEPT
    {
      etl::chrono::day result(d);

      result += ds;

      return result;
    }

    //***********************************************************************
    /// Add etl::chrono::day to etl::chrono::days
    ///\return etl::chrono::day
    //***********************************************************************
    ETL_CONSTEXPR etl::chrono::day operator +(const etl::chrono::days& ds, const etl::chrono::day& d) ETL_NOEXCEPT
    {
      etl::chrono::day result(d);

      result += ds;

      return result;
    }

    //***********************************************************************
    /// Subtract etl::chrono::days from etl::chrono::day
    ///\return etl::chrono::day
    //***********************************************************************
    ETL_CONSTEXPR etl::chrono::day operator -(const etl::chrono::day& d, const etl::chrono::days& ds) ETL_NOEXCEPT
    {
      etl::chrono::day result(d);

      result -= ds;

      return result;
    }

    //***********************************************************************
    /// Subtract etl::chrono::day from etl::chrono::day
    ///\return etl::chrono::days
    //***********************************************************************
    ETL_CONSTEXPR etl::chrono::days operator -(const etl::chrono::day& d1, const etl::chrono::day& d2) ETL_NOEXCEPT
    {
      return etl::chrono::days(static_cast<int>(static_cast<unsigned>(d1)) - 
                               static_cast<int>(static_cast<unsigned>(d2)));
    }

    //***********************************************************************
    /// Equality operator
    //***********************************************************************
    ETL_CONSTEXPR bool operator ==(const etl::chrono::day& d1, const etl::chrono::day& d2) ETL_NOEXCEPT
    {
      return (static_cast<unsigned>(d1) == static_cast<unsigned>(d2));
    }

    //***********************************************************************
    /// Inequality operator
    //***********************************************************************
    ETL_CONSTEXPR bool operator !=(const etl::chrono::day& d1, const etl::chrono::day& d2) ETL_NOEXCEPT
    {
      return !(d1 == d2);
    }

    //***********************************************************************
    /// Less-than operator
    //***********************************************************************
    ETL_CONSTEXPR bool operator <(const etl::chrono::day& d1, const etl::chrono::day& d2) ETL_NOEXCEPT
    {
      return (static_cast<unsigned>(d1) < static_cast<unsigned>(d2));
    }

    //***********************************************************************
    /// Less-than-or-equal operator
    //***********************************************************************
    ETL_CONSTEXPR bool operator <=(const etl::chrono::day& d1, const etl::chrono::day& d2) ETL_NOEXCEPT
    {
      return (static_cast<unsigned>(d1) <= static_cast<unsigned>(d2));
    }

    //***********************************************************************
    /// Greater-than operator
    //***********************************************************************
    ETL_CONSTEXPR bool operator >(const etl::chrono::day& d1, const etl::chrono::day& d2) ETL_NOEXCEPT
    {
      return (static_cast<unsigned>(d1) > static_cast<unsigned>(d2));
    }

    //***********************************************************************
    /// Greater-than-or-equal operator
    //***********************************************************************
    ETL_CONSTEXPR bool operator >=(const etl::chrono::day& d1, const etl::chrono::day& d2) ETL_NOEXCEPT
    {
      return (static_cast<unsigned>(d1) >= static_cast<unsigned>(d2));
    }

    //***********************************************************************
    /// Spaceship operator
    //***********************************************************************
#if ETL_USING_CPP20
    constexpr auto operator <=>(const etl::chrono::day& d1, const etl::chrono::day& d2) noexcept
    {
      return (static_cast<unsigned>(d1) <=> static_cast<unsigned>(d2));
    }
#endif
  }

  //*************************************************************************
  /// Hash function for etl::chrono::day
  //*************************************************************************
#if ETL_USING_8BIT_TYPES
  template <>
  struct hash<etl::chrono::day>
  {
    size_t operator()(const etl::chrono::day& d) const
    {
      unsigned value = d;
      const uint8_t* p = reinterpret_cast<const uint8_t*>(&value);

      return etl::private_hash::generic_hash<size_t>(p, p + sizeof(unsigned));
    }
  };
#endif
}

#if ETL_USING_CPP11
namespace etl
{
  namespace literals
  {
    namespace chrono_literals
    {
      //***********************************************************************
      /// Literal for days
      //***********************************************************************
      ETL_CONSTEXPR etl::chrono::day operator ""_d(unsigned long long d) ETL_NOEXCEPT
      {
        return etl::chrono::day(static_cast<unsigned>(d));
      }
    }
  }
}
#endif

#endif