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
      ETL_CONSTEXPR day()
        : value(0)
      {
      }

      //***********************************************************************
      ETL_CONSTEXPR day(unsigned value_)
        : value(value_)
      {
      }

      //***********************************************************************
      ETL_CONSTEXPR day(const etl::chrono::day& other)
        : value(other.value)
      {
      }

      //***********************************************************************
      ETL_CONSTEXPR etl::chrono::day& operator =(const etl::chrono::day& rhs)
      {
        value = rhs.value;

        return *this;
      }

      //***********************************************************************
      template <typename TToDuration, typename TValue2, typename TPeriod2>
      ETL_CONSTEXPR etl::chrono::day& operator =(const etl::chrono::duration<TValue2, TPeriod2>& rhs)
      {
        value = etl::chrono::duration_cast<TToDuration, TValue2, TPeriod2>(rhs);
      }

      //***********************************************************************
      ETL_CONSTEXPR etl::chrono::day& operator ++() ETL_NOEXCEPT
      {
        ++value;

        return *this;
      }

      //***********************************************************************
      ETL_CONSTEXPR14 etl::chrono::day operator ++(int) ETL_NOEXCEPT
      {
        const etl::chrono::day temp = *this;
        ++value;

        return temp;
      }

      //***********************************************************************
      ETL_CONSTEXPR etl::chrono::day& operator --() ETL_NOEXCEPT
      {
        --value;

        return *this;
      }

      //***********************************************************************
      ETL_CONSTEXPR14 etl::chrono::day operator --(int) ETL_NOEXCEPT
      {
        const etl::chrono::day temp = *this;
        --value;

        return temp;
      }

      //***********************************************************************
      ETL_CONSTEXPR etl::chrono::day& operator +=(const etl::chrono::days& ds) ETL_NOEXCEPT
      {
        value += static_cast<unsigned char>(ds.count());

        return *this;
      }

      //***********************************************************************
      ETL_CONSTEXPR etl::chrono::day& operator -=(const etl::chrono::days& ds) ETL_NOEXCEPT
      {
        value -= static_cast<unsigned char>(ds.count());

        return *this;
      }

      //***********************************************************************
      ETL_CONSTEXPR bool ok() const ETL_NOEXCEPT
      {
        return (value >= 1U) && (value <= 31U);
      }

      //***********************************************************************
      ETL_CONSTEXPR operator unsigned() const ETL_NOEXCEPT
      {
        return static_cast<unsigned>(value);
      }

    private:

      unsigned char value;
    };

    //***********************************************************************
    ETL_CONSTEXPR etl::chrono::day operator +(const etl::chrono::day& d, const etl::chrono::days& ds) ETL_NOEXCEPT
    {
      etl::chrono::day result(d);

      result += ds;

      return result;
    }

    //***********************************************************************
    ETL_CONSTEXPR etl::chrono::day operator +(const etl::chrono::days& ds, const etl::chrono::day& d) ETL_NOEXCEPT
    {
      etl::chrono::day result(d);

      result += ds;

      return result;
    }

    //***********************************************************************
    ETL_CONSTEXPR etl::chrono::day operator -(const etl::chrono::day& d, const etl::chrono::days& ds) ETL_NOEXCEPT
    {
      etl::chrono::day result(d);

      result -= ds;

      return result;
    }

    //***********************************************************************
    ETL_CONSTEXPR etl::chrono::days operator -(const etl::chrono::day& d1, const etl::chrono::day& d2) ETL_NOEXCEPT
    {
      return etl::chrono::days(static_cast<int>(static_cast<unsigned>(d1)) - 
                               static_cast<int>(static_cast<unsigned>(d2)));
    }

    //***********************************************************************
    ETL_CONSTEXPR bool operator ==(const etl::chrono::day& d1, const etl::chrono::day& d2) ETL_NOEXCEPT
    {
      return (static_cast<unsigned>(d1) == static_cast<unsigned>(d2));
    }

    //***********************************************************************
    ETL_CONSTEXPR bool operator !=(const etl::chrono::day& d1, const etl::chrono::day& d2) ETL_NOEXCEPT
    {
      return !(d1 == d2);
    }

    //***********************************************************************
    ETL_CONSTEXPR bool operator <(const etl::chrono::day& d1, const etl::chrono::day& d2) ETL_NOEXCEPT
    {
      return (static_cast<unsigned>(d1) < static_cast<unsigned>(d2));
    }

    //***********************************************************************
    ETL_CONSTEXPR bool operator <=(const etl::chrono::day& d1, const etl::chrono::day& d2) ETL_NOEXCEPT
    {
      return (static_cast<unsigned>(d1) <= static_cast<unsigned>(d2));
    }

    //***********************************************************************
    ETL_CONSTEXPR bool operator >(const etl::chrono::day& d1, const etl::chrono::day& d2) ETL_NOEXCEPT
    {
      return (static_cast<unsigned>(d1) > static_cast<unsigned>(d2));
    }

    //***********************************************************************
    ETL_CONSTEXPR bool operator >=(const etl::chrono::day& d1, const etl::chrono::day& d2) ETL_NOEXCEPT
    {
      return (static_cast<unsigned>(d1) >= static_cast<unsigned>(d2));
    }

    //***********************************************************************
#if ETL_USING_CPP20
    ETL_CONSTEXPR auto operator <=>(const etl::chrono::day& d1, const etl::chrono::day& d2)
    {
      return (static_cast<unsigned>(d1) <=> static_cast<unsigned>(d2));
    }
#endif
  }

  //*************************************************************************
  /// Hash function.
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
        return etl::chrono::day(d);
      }
    }
  }
}
#endif

#endif