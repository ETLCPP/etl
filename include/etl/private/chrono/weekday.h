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
    class weekday;
    class weekday_indexed;
    class weekday_last;
    struct last_spec;

    ETL_CONSTEXPR etl::chrono::weekday operator +(const etl::chrono::weekday& m, const etl::chrono::days& ds)   ETL_NOEXCEPT;
    ETL_CONSTEXPR etl::chrono::weekday operator +(const etl::chrono::days& ds,   const etl::chrono::weekday& m) ETL_NOEXCEPT;
    ETL_CONSTEXPR etl::chrono::weekday operator -(const etl::chrono::weekday& m, const etl::chrono::days& ds)   ETL_NOEXCEPT;

    //***********************************************************************
    /// weekday
    //***********************************************************************
    class weekday
    {
    public:

      //***********************************************************************
      /// Default constructor
      //***********************************************************************
      ETL_CONSTEXPR weekday() ETL_NOEXCEPT
        : value(255U)
      {
      }

      //***********************************************************************
      /// Construct from unsigned
      //***********************************************************************
      ETL_CONSTEXPR explicit weekday(unsigned value_) ETL_NOEXCEPT
        : value(value_ == 7U ? 0U :value_)
      {
      }

      //***********************************************************************
      /// Copy constructor
      //***********************************************************************
      ETL_CONSTEXPR weekday(const etl::chrono::weekday& other) ETL_NOEXCEPT
        : value(other.value)
      {
      }

      //***********************************************************************
      /// Assignment operator
      //***********************************************************************
      ETL_CONSTEXPR etl::chrono::weekday& operator =(const etl::chrono::weekday& rhs) ETL_NOEXCEPT
      {
        value = rhs.value;

        return *this;
      }

      //***********************************************************************
      /// Pre-increment operator
      //***********************************************************************
      ETL_CONSTEXPR etl::chrono::weekday& operator ++() ETL_NOEXCEPT
      {
        *this += etl::chrono::days(1);

        return *this;
      }

      //***********************************************************************
      /// Post-increment operator
      //***********************************************************************
      ETL_CONSTEXPR14 etl::chrono::weekday operator ++(int) ETL_NOEXCEPT
      {
        const etl::chrono::weekday temp = *this;
        
        *this += etl::chrono::days(1);

        return temp;
      }

      //***********************************************************************
      /// Pre-decrement operator
      //***********************************************************************
      ETL_CONSTEXPR etl::chrono::weekday& operator --() ETL_NOEXCEPT
      {
        *this -= etl::chrono::days(1);

        return *this;
      }

      //***********************************************************************
      /// Post-decrement operator
      //***********************************************************************
      ETL_CONSTEXPR14 etl::chrono::weekday operator --(int) ETL_NOEXCEPT
      {
        etl::chrono::weekday temp = *this;

        *this -= etl::chrono::days(1);

        return temp;
      }

      //***********************************************************************
      /// Plus-equals operator adding etl::chrono::days
      //***********************************************************************
      ETL_CONSTEXPR etl::chrono::weekday& operator +=(const etl::chrono::days& ds) ETL_NOEXCEPT
      {
        *this = *this + ds;

        return *this;
      }

      //***********************************************************************
      /// Minus-equals operator subtracting etl::chrono::days
      //***********************************************************************
      ETL_CONSTEXPR etl::chrono::weekday& operator -=(const etl::chrono::days& ds) ETL_NOEXCEPT
      {
        *this = *this - ds;

        return *this;
      }

      //***********************************************************************
      /// Returns <b>true</b> if the weekday is within the valid 1 to 31 range
      //***********************************************************************
      ETL_CONSTEXPR bool ok() const ETL_NOEXCEPT
      {
        return (c_encoding() <= 6U);
      }

      //***********************************************************************
      /// The minimum weekday value for which ok() will return <b>true</b>
      /// C encoding.
      //***********************************************************************
      static ETL_CONSTEXPR unsigned min() ETL_NOEXCEPT
      {
        return 0;
      }

      //***********************************************************************
      /// The maximum weekday value for which ok() will return <b>true</b>
      /// C encoding.
      //***********************************************************************
      static ETL_CONSTEXPR unsigned max() ETL_NOEXCEPT
      {
        return 6;
      }

      //***********************************************************************
      /// Get the C encoding of the weekday
      //***********************************************************************
      ETL_CONSTEXPR unsigned c_encoding() const ETL_NOEXCEPT
      {
        return value;
      }

      //***********************************************************************
      /// Get the ISO encoding of the weekday
      //***********************************************************************
      ETL_CONSTEXPR unsigned iso_encoding() const ETL_NOEXCEPT
      {
        return (value == 0U) ? 7U : value;
      }

      //***********************************************************************
      /// Index operator from index
      //***********************************************************************
      ETL_CONSTEXPR etl::chrono::weekday_indexed operator[](unsigned index) const ETL_NOEXCEPT;
        
      //***********************************************************************
      /// Index operator from etl::chrono::last_spec
      //***********************************************************************
      ETL_CONSTEXPR etl::chrono::weekday_last operator[](etl::chrono::last_spec last) const ETL_NOEXCEPT;

      //***********************************************************************
      /// Returns <b>true</b> if the day is a weekend.
      //***********************************************************************
      ETL_CONSTEXPR14 bool is_weekend() ETL_NOEXCEPT
      {
        return (c_encoding() == 0U) || (c_encoding() == 6U);
      }

    private:

      // The weekday value in C encoding.
      unsigned char value;
    };

    //***********************************************************************
    /// Equality operator
    //***********************************************************************
    ETL_CONSTEXPR bool operator ==(const etl::chrono::weekday& wd1, const etl::chrono::weekday& wd2) ETL_NOEXCEPT
    {
      return (wd1.c_encoding() == wd2.c_encoding());
    }

    //***********************************************************************
    /// Inequality operator
    //***********************************************************************
    ETL_CONSTEXPR bool operator !=(const etl::chrono::weekday& wd1, const etl::chrono::weekday& wd2) ETL_NOEXCEPT
    {
      return !(wd1 == wd2);
    }

    //***********************************************************************
    /// Add etl::chrono::days to etl::chrono::weekday
    ///\return etl::chrono::weekday
    //***********************************************************************
    ETL_CONSTEXPR14 etl::chrono::weekday operator +(const etl::chrono::weekday& wd, const etl::chrono::days& ds) ETL_NOEXCEPT
    {
      int delta = ds.count() % 7;

      unsigned int value = wd.c_encoding();

      // Adjust to allow a limited +-7 weekday delta
      value %= 7U;
      value += 7U;
      value += delta;
      value %= 7U;

      return etl::chrono::weekday(value);
    }

    //***********************************************************************
    /// Add etl::chrono::weekday to etl::chrono::days
    ///\return etl::chrono::weekday
    //***********************************************************************
    ETL_CONSTEXPR etl::chrono::weekday operator +(const etl::chrono::days& ds, const etl::chrono::weekday& wd) ETL_NOEXCEPT
    {
      return wd + ds;
    }

    //***********************************************************************
    /// Subtract etl::chrono::days from etl::chrono::weekday
    ///\return etl::chrono::weekday
    //***********************************************************************
    ETL_CONSTEXPR etl::chrono::weekday operator -(const etl::chrono::weekday& m, const etl::chrono::days& ds) ETL_NOEXCEPT
    {
      return m + etl::chrono::days(-ds.count());
    }

    //***********************************************************************
    /// Subtract etl::chrono::weekday from etl::chrono::weekday
    ///\return etl::chrono::days
    //***********************************************************************
    ETL_CONSTEXPR14 etl::chrono::days operator -(const etl::chrono::weekday& wd1, const etl::chrono::weekday& wd2) ETL_NOEXCEPT
    {
      if (wd1.ok() && wd2.ok())
      {
        int diff = static_cast<int>(wd1.c_encoding()) - static_cast<int>(wd2.c_encoding());
        
        return etl::chrono::days((diff + 7) % 7);
      }

      return etl::chrono::days(0);
    }

#if ETL_USING_CPP17
    inline constexpr etl::chrono::weekday Sunday{ 0U };
    inline constexpr etl::chrono::weekday Monday{ 1U };
    inline constexpr etl::chrono::weekday Tuesday{ 2U };
    inline constexpr etl::chrono::weekday Wednesday{ 3U };
    inline constexpr etl::chrono::weekday Thursday{ 4U };
    inline constexpr etl::chrono::weekday Friday{ 5U };
    inline constexpr etl::chrono::weekday Saturday{ 6U };
#else     
    static ETL_CONSTANT etl::chrono::weekday Sunday{ 0U };
    static ETL_CONSTANT etl::chrono::weekday Monday{ 1U };
    static ETL_CONSTANT etl::chrono::weekday Tuesday{ 2U };
    static ETL_CONSTANT etl::chrono::weekday Wednesday{ 3U };
    static ETL_CONSTANT etl::chrono::weekday Thursday{ 4U };
    static ETL_CONSTANT etl::chrono::weekday Friday{ 5U };
    static ETL_CONSTANT etl::chrono::weekday Saturday{ 6U };
#endif

    //***********************************************************************
    /// weekday_indexed
    //***********************************************************************
    class weekday_indexed
    {
    public:

      //***********************************************************************
      /// Default constructor
      //***********************************************************************
      ETL_CONSTEXPR weekday_indexed() ETL_NOEXCEPT
        : wd()
        , i()
      {
      }

      //***********************************************************************
      /// Construct from weekday and index
      //***********************************************************************
      ETL_CONSTEXPR weekday_indexed(const etl::chrono::weekday& wd_, unsigned index_) ETL_NOEXCEPT
        : wd(wd_)
        , i(static_cast<uint_least8_t>(index_))
      {
      }

      //***********************************************************************
      /// Copy constructor
      //***********************************************************************
      ETL_CONSTEXPR weekday_indexed(const etl::chrono::weekday_indexed& other) ETL_NOEXCEPT
        : wd(other.wd)
        , i(other.i)
      {
      }

      //***********************************************************************
      /// Assignment operator
      //***********************************************************************
      ETL_CONSTEXPR14 etl::chrono::weekday_indexed& operator =(const etl::chrono::weekday_indexed& rhs) ETL_NOEXCEPT
      {
        wd = rhs.wd;
        i  = rhs.i;

        return *this;
      }

      //***********************************************************************
      /// Get weekday
      //***********************************************************************
      ETL_NODISCARD ETL_CONSTEXPR etl::chrono::weekday weekday() const ETL_NOEXCEPT
      {
        return wd;
      }

      //***********************************************************************
      /// Get index
      //***********************************************************************
      ETL_NODISCARD ETL_CONSTEXPR unsigned index() const ETL_NOEXCEPT
      {
        return i;
      }

      //***********************************************************************
      /// Returns <b>true</b> if the weekday and index are valid
      //***********************************************************************
      ETL_NODISCARD ETL_CONSTEXPR bool ok() const ETL_NOEXCEPT
      {
        return wd.ok() && (i >= 1U) && (i <= 5U);
      }

    private:

      etl::chrono::weekday wd;
      uint_least8_t i;
    };

    //***********************************************************************
    /// Equality operator
    //***********************************************************************
    ETL_CONSTEXPR bool operator ==(const etl::chrono::weekday_indexed& wd1, const etl::chrono::weekday_indexed& wd2) ETL_NOEXCEPT
    {
      return (wd1.weekday() == wd2.weekday()) && 
        (wd1.index()   == wd2.index());
    }

    //***********************************************************************
    /// Inequality operator
    //***********************************************************************
    ETL_CONSTEXPR bool operator !=(const etl::chrono::weekday_indexed& wd1, const etl::chrono::weekday_indexed& wd2) ETL_NOEXCEPT
    {
      return !(wd1 == wd2);
    }

    //***********************************************************************
    /// weekday_last
    //***********************************************************************
    class weekday_last
    {
    public:

      //***********************************************************************
      /// Construct from unsigned
      //***********************************************************************
      ETL_CONSTEXPR explicit weekday_last(const etl::chrono::weekday& wd_) ETL_NOEXCEPT
        : wd(wd_)
      {
      }

      //***********************************************************************
      /// Copy constructor
      //***********************************************************************
      ETL_CONSTEXPR weekday_last(const etl::chrono::weekday_last& other) ETL_NOEXCEPT
        : wd(other.wd)
      {
      }

      //***********************************************************************
      /// Assignment operator
      //***********************************************************************
      ETL_CONSTEXPR14 etl::chrono::weekday_last& operator =(const etl::chrono::weekday_last& rhs) ETL_NOEXCEPT
      {
        wd = rhs.wd;

        return *this;
      }

      //***********************************************************************
      /// Get weekday
      //***********************************************************************
      ETL_NODISCARD ETL_CONSTEXPR etl::chrono::weekday weekday() const ETL_NOEXCEPT
      {
        return wd;
      }

      //***********************************************************************
      /// Returns <b>true</b> if the weekday is valid
      //***********************************************************************
      ETL_NODISCARD ETL_CONSTEXPR bool ok() const ETL_NOEXCEPT
      {
        return wd.ok();
      }

    private:

      etl::chrono::weekday wd;
    };

    //***********************************************************************
    /// Equality operator
    //***********************************************************************
    ETL_CONSTEXPR bool operator ==(const etl::chrono::weekday_last& wd1, const etl::chrono::weekday_last& wd2) ETL_NOEXCEPT
    {
      return (wd1.weekday() == wd2.weekday());
    }

    //***********************************************************************
    /// weekday index operator from index
    //***********************************************************************
    ETL_CONSTEXPR etl::chrono::weekday_indexed etl::chrono::weekday::operator[](unsigned index) const ETL_NOEXCEPT
    {
      return etl::chrono::weekday_indexed(*this, index);
    }

    //***********************************************************************
    /// Index operator from etl::chrono::last_spec
    //***********************************************************************
    ETL_CONSTEXPR etl::chrono::weekday_last etl::chrono::weekday::operator[](etl::chrono::last_spec last) const ETL_NOEXCEPT
    {
      return etl::chrono::weekday_last(*this);
    }
  }

  //*************************************************************************
  /// Hash function for etl::chrono::weekday
  //*************************************************************************
#if ETL_USING_8BIT_TYPES
  template <>
  struct hash<etl::chrono::weekday>
  {
    size_t operator()(const etl::chrono::weekday& wd) const
    {
      unsigned value = wd.c_encoding();
      const uint8_t* p = reinterpret_cast<const uint8_t*>(&value);

      return etl::private_hash::generic_hash<size_t>(p, p + sizeof(unsigned));
    }
  };
#endif

  //*************************************************************************
  /// Hash function for etl::chrono::weekday_indexed
  //*************************************************************************
#if ETL_USING_8BIT_TYPES
  template <>
  struct hash<etl::chrono::weekday_indexed>
  {
    size_t operator()(const etl::chrono::weekday_indexed& wdi) const
    {
      return etl::hash<etl::chrono::weekday>()(wdi.weekday()) ^ etl::hash<unsigned>()(wdi.index());
    }
  };
#endif

  //*************************************************************************
  /// Hash function for etl::chrono::weekday_last
  //*************************************************************************
#if ETL_USING_8BIT_TYPES
  template <>
  struct hash<etl::chrono::weekday_last>
  {
    size_t operator()(const etl::chrono::weekday_last& wdl) const
    {
      return etl::hash<etl::chrono::weekday>()(wdl.weekday());
    }
  };
#endif
}

#if ETL_HAS_CHRONO_LITERALS_WEEKDAY
namespace etl
{
  namespace literals
  {
    namespace chrono_literals
    {
      //***********************************************************************
      /// Literal for weekdays
      //***********************************************************************
      inline constexpr etl::chrono::weekday operator ""_weekday(unsigned long long wd) noexcept
      {
        return etl::chrono::weekday(static_cast<unsigned char>(wd));
      }
    }
  }
}
#endif
