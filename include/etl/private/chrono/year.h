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

#ifndef ETL_IN_CHRONO_H
  #error DO NOT DIRECTLY INCLUDE THIS FILE. USE CHRONO.H
#endif

namespace etl
{
  namespace chrono
  {
    //***********************************************************************
    /// year
    //***********************************************************************
    class year
    {
    public:

      //***********************************************************************
      /// Default constructor
      //***********************************************************************
      ETL_CONSTEXPR year() ETL_NOEXCEPT
        : value(0)
      {
      }

      //***********************************************************************
      /// Construct from unsigned
      //***********************************************************************
      ETL_CONSTEXPR explicit year(unsigned value_) ETL_NOEXCEPT
        : value(value_)
      {
      }

      //***********************************************************************
      /// Copy constructor
      //***********************************************************************
      ETL_CONSTEXPR year(const etl::chrono::year& other) ETL_NOEXCEPT
        : value(other.value)
      {
      }

      //***********************************************************************
      /// Assignment operator
      //***********************************************************************
      ETL_CONSTEXPR etl::chrono::year& operator =(const etl::chrono::year& rhs) ETL_NOEXCEPT
      {
        value = rhs.value;

        return *this;
      }

      //***********************************************************************
      /// Pre-increment operator
      //***********************************************************************
      ETL_CONSTEXPR etl::chrono::year& operator ++() ETL_NOEXCEPT
      {
        ++value;

        return *this;
      }

      //***********************************************************************
      /// Post-increment operator
      //***********************************************************************
      ETL_CONSTEXPR14 etl::chrono::year operator ++(int) ETL_NOEXCEPT
      {
        const etl::chrono::year temp = *this;
        ++value;

        return temp;
      }

      //***********************************************************************
      /// Pre-decrement operator
      //***********************************************************************
      ETL_CONSTEXPR etl::chrono::year& operator --() ETL_NOEXCEPT
      {
        --value;

        return *this;
      }

      //***********************************************************************
      /// Post-decrement operator
      //***********************************************************************
      ETL_CONSTEXPR14 etl::chrono::year operator --(int) ETL_NOEXCEPT
      {
        const etl::chrono::year temp = *this;
        --value;

        return temp;
      }

      //***********************************************************************
      /// Plus-equals operator adding etl::chrono::years
      //***********************************************************************
      ETL_CONSTEXPR etl::chrono::year& operator +=(const etl::chrono::years& ys) ETL_NOEXCEPT
      {
        value += static_cast<unsigned char>(ys.count());

        return *this;
      }

      //***********************************************************************
      /// Minus-equals operator subtracting etl::chrono::years
      //***********************************************************************
      ETL_CONSTEXPR etl::chrono::year& operator -=(const etl::chrono::years& ys) ETL_NOEXCEPT
      {
        value -= static_cast<unsigned char>(ys.count());

        return *this;
      }

      //***********************************************************************
      /// Returns <b>true</b> if the year is within the valid -32767 to 32767 range
      //***********************************************************************
      ETL_CONSTEXPR bool ok() const ETL_NOEXCEPT
      {
        return (value != etl::integral_limits<int16_t>::min);
      }

      //***********************************************************************
      /// The minimum year value for which ok() will return <b>true</b>
      //***********************************************************************
      static ETL_CONSTEXPR etl::chrono::year min() ETL_NOEXCEPT
      {
        return etl::chrono::year(-32767);
      }

      //***********************************************************************
      /// The maximum year value for which ok() will return <b>true</b>
      //***********************************************************************
      static ETL_CONSTEXPR etl::chrono::year max() ETL_NOEXCEPT
      {
        return etl::chrono::year(32767);
      }

      //***********************************************************************
      /// Returns <b>true</b> if the year is a leap year
      //***********************************************************************
      ETL_CONSTEXPR bool is_leap() const ETL_NOEXCEPT
      {
        return ((value % 4) == 0) &&    // Divisible by 4
               (((value % 100) != 0) || // but not divisible by 100
                ((value % 400) == 0));  // unless divisible by 400
      }

      //***********************************************************************
      /// Conversion operator to unsigned int
      //***********************************************************************
      ETL_CONSTEXPR operator int() const ETL_NOEXCEPT
      {
        return static_cast<int>(value);
      }

    private:

      int16_t value;
    };

    //***********************************************************************
    /// Equality operator
    //***********************************************************************
    ETL_CONSTEXPR bool operator ==(const etl::chrono::year& y1, const etl::chrono::year& y2) ETL_NOEXCEPT
    {
      return (static_cast<unsigned>(y1) == static_cast<unsigned>(y2));
    }

    //***********************************************************************
    /// Inequality operator
    //***********************************************************************
    ETL_CONSTEXPR bool operator !=(const etl::chrono::year& y1, const etl::chrono::year& y2) ETL_NOEXCEPT
    {
      return !(y1 == y2);
    }

    //***********************************************************************
    /// Less-than operator
    //***********************************************************************
    ETL_CONSTEXPR bool operator <(const etl::chrono::year& y1, const etl::chrono::year& y2) ETL_NOEXCEPT
    {
      return (static_cast<unsigned>(y1) < static_cast<unsigned>(y2));
    }

    //***********************************************************************
    /// Less-than-or-equal operator
    //***********************************************************************
    ETL_CONSTEXPR bool operator <=(const etl::chrono::year& y1, const etl::chrono::year& y2) ETL_NOEXCEPT
    {
      return (static_cast<unsigned>(y1) <= static_cast<unsigned>(y2));
    }

    //***********************************************************************
    /// Greater-than operator
    //***********************************************************************
    ETL_CONSTEXPR bool operator >(const etl::chrono::year& y1, const etl::chrono::year& y2) ETL_NOEXCEPT
    {
      return (static_cast<unsigned>(y1) > static_cast<unsigned>(y2));
    }

    //***********************************************************************
    /// Greater-than-or-equal operator
    //***********************************************************************
    ETL_CONSTEXPR bool operator >=(const etl::chrono::year& y1, const etl::chrono::year& y2) ETL_NOEXCEPT
    {
      return (static_cast<unsigned>(y1) >= static_cast<unsigned>(y2));
    }

    //***********************************************************************
    /// Spaceship operator
    //***********************************************************************
#if ETL_USING_CPP20
    [[nodiscard]] constexpr auto operator <=>(const etl::chrono::year& y1, const etl::chrono::year& y2) noexcept
    {
      return (static_cast<unsigned>(y1) <=> static_cast<unsigned>(y2));
    }
#endif

    //***********************************************************************
    /// Add etl::chrono::years to etl::chrono::year
    ///\return etl::chrono::year
    //***********************************************************************
    ETL_CONSTEXPR etl::chrono::year operator +(const etl::chrono::year& y, const etl::chrono::years& ys) ETL_NOEXCEPT
    {
      etl::chrono::year result(y);

      result += ys;

      return result;
    }

    //***********************************************************************
    /// Add etl::chrono::year to etl::chrono::years
    ///\return etl::chrono::year
    //***********************************************************************
    ETL_CONSTEXPR etl::chrono::year operator +(const etl::chrono::years& ys, const etl::chrono::year& y) ETL_NOEXCEPT
    {
      etl::chrono::year result(y);

      result += ys;

      return result;
    }

    //***********************************************************************
    /// Subtract etl::chrono::years from etl::chrono::year
    ///\return etl::chrono::year
    //***********************************************************************
    ETL_CONSTEXPR etl::chrono::year operator -(const etl::chrono::year& y, const etl::chrono::years& ys) ETL_NOEXCEPT
    {
      etl::chrono::year result(y);

      result -= ys;

      return result;
    }

    //***********************************************************************
    /// Subtract etl::chrono::year from etl::chrono::years
    ///\return etl::chrono::years
    //***********************************************************************
    ETL_CONSTEXPR etl::chrono::year operator -(const etl::chrono::years& ys, const etl::chrono::year& y) ETL_NOEXCEPT
    {
      etl::chrono::year result(y);

      result -= ys;

      return result;
    }

    //***********************************************************************
    /// Subtract etl::chrono::year from etl::chrono::year
    ///\return etl::chrono::years
    //***********************************************************************
    ETL_CONSTEXPR etl::chrono::years operator -(const etl::chrono::year& y1, const etl::chrono::year& y2) ETL_NOEXCEPT
    {
      return etl::chrono::years(static_cast<int>(static_cast<unsigned>(y1)) - 
                                static_cast<int>(static_cast<unsigned>(y2)));
    }
  }

  //*************************************************************************
  /// Hash function for etl::chrono::year
  //*************************************************************************
#if ETL_USING_8BIT_TYPES
  template <>
  struct hash<etl::chrono::year>
  {
    size_t operator()(const etl::chrono::year& y) const
    {
      int value = y;
      const uint8_t* p = reinterpret_cast<const uint8_t*>(&value);

      return etl::private_hash::generic_hash<size_t>(p, p + sizeof(int));
    }
  };
#endif
}

#if ETL_HAS_CHRONO_LITERALS_YEAR
namespace etl
{
  namespace literals
  {
    namespace chrono_literals
    {
      //***********************************************************************
      /// Literal for years
      //***********************************************************************
      inline constexpr etl::chrono::year operator ""_year(unsigned long long y) noexcept
      {
        return etl::chrono::year(static_cast<int16_t>(y));
      }
    }
  }
}
#endif
