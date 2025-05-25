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
    class month;

    ETL_CONSTEXPR14 etl::chrono::month operator +(const etl::chrono::month& m,   const etl::chrono::months& ms) ETL_NOEXCEPT;
    ETL_CONSTEXPR14 etl::chrono::month operator +(const etl::chrono::months& ms, const etl::chrono::month& m)   ETL_NOEXCEPT;
    ETL_CONSTEXPR14 etl::chrono::month operator -(const etl::chrono::month& m,   const etl::chrono::months& ms) ETL_NOEXCEPT;

    namespace private_chrono
    {
      static ETL_CONSTANT unsigned char days_in_month[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    }

    //***********************************************************************
    /// month
    //***********************************************************************
    class month
    {
    public:

      using rep = uint_least8_t;

      //***********************************************************************
      /// Default constructor
      //***********************************************************************
      ETL_CONSTEXPR month() ETL_NOEXCEPT
        : value(0)
      {
      }

      //***********************************************************************
      /// Construct from unsigned
      //***********************************************************************
      ETL_CONSTEXPR explicit month(unsigned value_) ETL_NOEXCEPT
        : value(value_)
      {
      }

      //***********************************************************************
      /// Copy constructor
      //***********************************************************************
      ETL_CONSTEXPR14 month(const etl::chrono::month& other) ETL_NOEXCEPT
        : value(other.value)
      {
      }

      //***********************************************************************
      /// Assignment operator
      //***********************************************************************
      ETL_CONSTEXPR14 etl::chrono::month& operator =(const etl::chrono::month& rhs) ETL_NOEXCEPT
      {
        value = rhs.value;

        return *this;
      }

      //***********************************************************************
      /// Pre-increment operator
      //***********************************************************************
      ETL_CONSTEXPR14 etl::chrono::month& operator ++() ETL_NOEXCEPT
      {
        *this += etl::chrono::months(1);

        return *this;
      }

      //***********************************************************************
      /// Post-increment operator
      //***********************************************************************
      ETL_CONSTEXPR14 etl::chrono::month operator ++(int) ETL_NOEXCEPT
      {
        const etl::chrono::month temp = *this;
        
        *this += etl::chrono::months(1);

        return temp;
      }

      //***********************************************************************
      /// Pre-decrement operator
      //***********************************************************************
      ETL_CONSTEXPR14 etl::chrono::month& operator --() ETL_NOEXCEPT
      {
        *this -= etl::chrono::months(1);

        return *this;
      }

      //***********************************************************************
      /// Post-decrement operator
      //***********************************************************************
      ETL_CONSTEXPR14 etl::chrono::month operator --(int) ETL_NOEXCEPT
      {
        etl::chrono::month temp = *this;

        *this -= etl::chrono::months(1);

        return temp;
      }

      //***********************************************************************
      /// Plus-equals operator adding etl::chrono::months
      //***********************************************************************
      ETL_CONSTEXPR14 etl::chrono::month& operator +=(const etl::chrono::months& ms) ETL_NOEXCEPT
      {
        *this = *this + ms;

        return *this;
      }

      //***********************************************************************
      /// Minus-equals operator subtracting etl::chrono::months
      //***********************************************************************
      ETL_CONSTEXPR14 etl::chrono::month& operator -=(const etl::chrono::months& ms) ETL_NOEXCEPT
      {
        *this = *this - ms;

        return *this;
      }

      //***********************************************************************
      /// Returns <b>true</b> if the month is within the valid 1 to 31 range
      //***********************************************************************
      ETL_NODISCARD
      ETL_CONSTEXPR14 bool ok() const ETL_NOEXCEPT
      {
        return (value >= 1U) && (value <= 12U);
      }

      //***********************************************************************
      /// Compare month with another.
      /// if month < other, returns -1
      /// else if month > other, returns 1
      /// else returns 0
      //***********************************************************************
      ETL_NODISCARD
      ETL_CONSTEXPR14 int compare(const month& other) const ETL_NOEXCEPT 
      {
        if (value < other.value) return -1;
        if (value > other.value) return 1;

        return 0;
      }

      //***********************************************************************
      /// The minimum month value for which ok() will return <b>true</b>
      //***********************************************************************
      ETL_NODISCARD
      static ETL_CONSTEXPR14 etl::chrono::month min() ETL_NOEXCEPT
      {
        return etl::chrono::month(1);
      }

      //***********************************************************************
      /// The maximum month value for which ok() will return <b>true</b>
      //***********************************************************************
      ETL_NODISCARD
      static ETL_CONSTEXPR14 etl::chrono::month max() ETL_NOEXCEPT
      {
        return etl::chrono::month(12);
      }

      //***********************************************************************
      /// Conversion operator to unsigned int
      //***********************************************************************
      ETL_CONSTEXPR14 operator unsigned() const ETL_NOEXCEPT
      {
        return static_cast<unsigned>(value);
      }

    private:

      rep value;
    };

    //***********************************************************************
    /// Equality operator
    //***********************************************************************
    inline ETL_CONSTEXPR14 bool operator ==(const etl::chrono::month& d1, const etl::chrono::month& d2) ETL_NOEXCEPT
    {
      return (static_cast<unsigned>(d1) == static_cast<unsigned>(d2));
    }

    //***********************************************************************
    /// Inequality operator
    //***********************************************************************
    inline ETL_CONSTEXPR14 bool operator !=(const etl::chrono::month& d1, const etl::chrono::month& d2) ETL_NOEXCEPT
    {
      return !(d1 == d2);
    }

    //***********************************************************************
    /// Less-than operator
    //***********************************************************************
    inline ETL_CONSTEXPR14 bool operator <(const etl::chrono::month& d1, const etl::chrono::month& d2) ETL_NOEXCEPT
    {
      return (static_cast<unsigned>(d1) < static_cast<unsigned>(d2));
    }

    //***********************************************************************
    /// Less-than-or-equal operator
    //***********************************************************************
    inline ETL_CONSTEXPR14 bool operator <=(const etl::chrono::month& d1, const etl::chrono::month& d2) ETL_NOEXCEPT
    {
      return (static_cast<unsigned>(d1) <= static_cast<unsigned>(d2));
    }

    //***********************************************************************
    /// Greater-than operator
    //***********************************************************************
    inline ETL_CONSTEXPR14 bool operator >(const etl::chrono::month& d1, const etl::chrono::month& d2) ETL_NOEXCEPT
    {
      return (static_cast<unsigned>(d1) > static_cast<unsigned>(d2));
    }

    //***********************************************************************
    /// Greater-than-or-equal operator
    //***********************************************************************
    inline ETL_CONSTEXPR14 bool operator >=(const etl::chrono::month& d1, const etl::chrono::month& d2) ETL_NOEXCEPT
    {
      return (static_cast<unsigned>(d1) >= static_cast<unsigned>(d2));
    }

    //***********************************************************************
    /// Spaceship operator
    //***********************************************************************
#if ETL_USING_CPP20
    [[nodiscard]] inline constexpr auto operator <=>(const etl::chrono::month& d1, const etl::chrono::month& d2) noexcept
    {
      return (static_cast<unsigned>(d1) <=> static_cast<unsigned>(d2));
    }
#endif

    //***********************************************************************
    /// Add etl::chrono::months to etl::chrono::month
    ///\return etl::chrono::month
    //***********************************************************************
    inline ETL_CONSTEXPR14 etl::chrono::month operator +(const etl::chrono::month& m, const etl::chrono::months& ms) ETL_NOEXCEPT
    {
      unsigned int value = static_cast<unsigned int>(m);

      value = value % 12U;

      if (value == 0U)
      {
        value = 12U;
      }

      int delta = ms.count() % 12;

      // Adjust to allow a limited +-11 month delta
      value += 11U;
      value += delta;
      value %= 12U;
      ++value;

      return etl::chrono::month(value);
    }

    //***********************************************************************
    /// Add etl::chrono::month to etl::chrono::months
    ///\return etl::chrono::month
    //***********************************************************************
    inline ETL_CONSTEXPR14 etl::chrono::month operator +(const etl::chrono::months& ms, const etl::chrono::month& m) ETL_NOEXCEPT
    {
      return m + ms;
    }

    //***********************************************************************
    /// Subtract etl::chrono::months from etl::chrono::month
    ///\return etl::chrono::month
    //***********************************************************************
    inline ETL_CONSTEXPR14 etl::chrono::month operator -(const etl::chrono::month& m, const etl::chrono::months& ms) ETL_NOEXCEPT
    {
      return m + etl::chrono::months(-ms.count());
    }

    //***********************************************************************
    /// Subtract etl::chrono::month from etl::chrono::month
    ///\return etl::chrono::months
    //***********************************************************************
    inline ETL_CONSTEXPR14 etl::chrono::months operator -(const etl::chrono::month& m1, const etl::chrono::month& m2) ETL_NOEXCEPT
    {
      if (m1.ok() && m2.ok())
      {
        // Calculate the signed difference.
        int difference = static_cast<int>(static_cast<unsigned>(m1)) - static_cast<int>(static_cast<unsigned>(m2));

        // Adjust for wrap-around.
        if (difference < 0)
        {
          difference += 12;
        }

        etl::chrono::months ms(difference);

        // Check for validity.
        if (m1 == (m2 + ms))
        {
          return ms;
        }
      }

      return etl::chrono::months();
    }

#if ETL_USING_CPP17
    inline constexpr etl::chrono::month January{ 1 };
    inline constexpr etl::chrono::month February{ 2 };
    inline constexpr etl::chrono::month March{ 3 };
    inline constexpr etl::chrono::month April{ 4 };
    inline constexpr etl::chrono::month May{ 5 };
    inline constexpr etl::chrono::month June{ 6 };
    inline constexpr etl::chrono::month July{ 7 };
    inline constexpr etl::chrono::month August{ 8 };
    inline constexpr etl::chrono::month September{ 9 };
    inline constexpr etl::chrono::month October{ 10 };
    inline constexpr etl::chrono::month November{ 11 };
    inline constexpr etl::chrono::month December{ 12 };
#else     
    static ETL_CONSTANT etl::chrono::month January{ 1 };
    static ETL_CONSTANT etl::chrono::month February{ 2 };
    static ETL_CONSTANT etl::chrono::month March{ 3 };
    static ETL_CONSTANT etl::chrono::month April{ 4 };
    static ETL_CONSTANT etl::chrono::month May{ 5 };
    static ETL_CONSTANT etl::chrono::month June{ 6 };
    static ETL_CONSTANT etl::chrono::month July{ 7 };
    static ETL_CONSTANT etl::chrono::month August{ 8 };
    static ETL_CONSTANT etl::chrono::month September{ 9 };
    static ETL_CONSTANT etl::chrono::month October{ 10 };
    static ETL_CONSTANT etl::chrono::month November{ 11 };
    static ETL_CONSTANT etl::chrono::month December{ 12 };
#endif
  }

  //*************************************************************************
  /// Hash function for etl::chrono::month
  //*************************************************************************
#if ETL_USING_8BIT_TYPES
  template <>
  struct hash<etl::chrono::month>
  {
    size_t operator()(const etl::chrono::month& m) const
    {
      etl::chrono::month::rep value = static_cast<etl::chrono::month::rep>(static_cast<unsigned>(m));
      const uint8_t* p = reinterpret_cast<const uint8_t*>(&value);

      return etl::private_hash::generic_hash<size_t>(p, p + sizeof(value));
    }
  };
#endif


}
