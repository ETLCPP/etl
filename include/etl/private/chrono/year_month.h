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
    class year_month
    {
    public:

      //*************************************************************************
      /// Default constructor.
      //*************************************************************************
      ETL_CONSTEXPR year_month()
        : y()
        , m()
      {
      }

      //*************************************************************************
      /// Construct from month and day.
      //*************************************************************************
      ETL_CONSTEXPR14 year_month(const etl::chrono::year&  y_, 
                                 const etl::chrono::month& m_) ETL_NOEXCEPT
        : y(y_)
        , m(m_)
      {
      }

      //*************************************************************************
      /// Returns the year.
      //*************************************************************************
      ETL_NODISCARD
      ETL_CONSTEXPR14 etl::chrono::year year() const ETL_NOEXCEPT
      {
        return y;
      }

      //*************************************************************************
      /// Returns the month.
      //*************************************************************************
      ETL_NODISCARD
      ETL_CONSTEXPR14 etl::chrono::month month() const ETL_NOEXCEPT
      {
        return m;
      }

      //*************************************************************************
      /// Returns true if the month/day is valid.
      //*************************************************************************
      ETL_NODISCARD
      ETL_CONSTEXPR14 bool ok() const ETL_NOEXCEPT
      {       
        return y.ok() && m.ok();
      }

      //***********************************************************************
      /// Compare year_month with another.
      /// if month < other.month, returns -1
      /// else if month > other.month, returns 1
      /// else if day < other.day, returns -1
      /// else if day > other.day, returns 1
      /// else returns 0
      //***********************************************************************
      ETL_NODISCARD
      ETL_CONSTEXPR14 int compare(const year_month& other) const ETL_NOEXCEPT 
      {
        if (y < other.y) return -1;
        if (y > other.y) return 1;
        if (m < other.m) return -1;
        if (m > other.m) return 1;
      
        return 0;
      }

    private:

      etl::chrono::year  y;
      etl::chrono::month m;
    };

    //*************************************************************************
    /// Adds etl::chrono::years
    //*************************************************************************
    inline ETL_CONSTEXPR14 etl::chrono::year_month operator +(const etl::chrono::year_month& ym,
                                                              const etl::chrono::years&      dy) ETL_NOEXCEPT
    {
      return etl::chrono::year_month(ym.year() + dy, ym.month());
    }

    //*************************************************************************
    /// Adds etl::chrono::years
    //*************************************************************************
    inline ETL_CONSTEXPR14 etl::chrono::year_month operator +(const etl::chrono::years&      dy,
                                                              const etl::chrono::year_month& ym) ETL_NOEXCEPT
    {
      return etl::chrono::year_month(ym.year() + dy, ym.month());
    }

    //*************************************************************************
    /// Adds etl::chrono::months
    //*************************************************************************
    inline ETL_CONSTEXPR14 etl::chrono::year_month operator +(const etl::chrono::year_month& ym,
                                                              const etl::chrono::months&     dm) ETL_NOEXCEPT
    {
      return etl::chrono::year_month(ym.year(), ym.month() + dm);
    }

    //*************************************************************************
    /// Adds etl::chrono::months
    //*************************************************************************
    inline ETL_CONSTEXPR14 etl::chrono::year_month operator +(const etl::chrono::months& dm,
                                                              const etl::chrono::year_month& ym) ETL_NOEXCEPT
    {
      return etl::chrono::year_month(ym.year(), ym.month() + dm);
    }

    //*************************************************************************
    /// Subtracts etl::chrono::years
    //*************************************************************************
    inline ETL_CONSTEXPR14 etl::chrono::year_month operator -(const etl::chrono::year_month& ym,
                                                              const etl::chrono::years&      dy) ETL_NOEXCEPT
    {
      return etl::chrono::year_month(ym.year() - dy, ym.month());
    }

    //*************************************************************************
    /// Subtracts etl::chrono::months
    //*************************************************************************
    inline ETL_CONSTEXPR14 etl::chrono::year_month operator -(const etl::chrono::year_month& ym,
                                                              const etl::chrono::months&     dm) ETL_NOEXCEPT
    {
      return etl::chrono::year_month(ym.year(), ym.month() - dm);
    }

    //*************************************************************************
    /// Subtracts etl::chrono::year_month
    //*************************************************************************
    inline ETL_CONSTEXPR14 etl::chrono::months operator -(const etl::chrono::year_month& ym1,
                                                          const etl::chrono::year_month& ym2) ETL_NOEXCEPT
    {
      return etl::chrono::months(static_cast<int>(((int(ym1.year()) - int(ym2.year())) * 12) + (unsigned(ym1.month()) - unsigned(ym2.month()))));
    }

    //*************************************************************************
    /// Equality operator.
    //*************************************************************************
    inline ETL_CONSTEXPR14 bool operator ==(const etl::chrono::year_month& lhs, 
                                            const etl::chrono::year_month& rhs) ETL_NOEXCEPT
    {
      return (lhs.year() == rhs.year()) && (lhs.month() == rhs.month());
    }

    //*************************************************************************
    /// Equality operator.
    //*************************************************************************
    inline ETL_CONSTEXPR14 bool operator !=(const etl::chrono::year_month& lhs, 
                                            const etl::chrono::year_month& rhs) ETL_NOEXCEPT
    {
      return !(lhs == rhs);
    }

    //*************************************************************************
    /// Less-than operator.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR14
    inline bool operator <(const etl::chrono::year_month& lhs,
                           const etl::chrono::year_month& rhs) ETL_NOEXCEPT
    {
      if (lhs.year() < rhs.year())
      {
        return true;
      }
      else if (lhs.year() == rhs.year())
      {
        return lhs.month() < rhs.month();
      }
      else
      {
        return false;
      }
    }

    //*************************************************************************
    /// Less-than-equal operator.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR14
    inline bool operator <=(const etl::chrono::year_month& lhs,
                            const etl::chrono::year_month& rhs) ETL_NOEXCEPT
    {
      return !(rhs < lhs);
    }

    //*************************************************************************
    /// Greater-than operator.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR14
    inline bool operator >(const etl::chrono::year_month& lhs,
                           const etl::chrono::year_month& rhs) ETL_NOEXCEPT
    {
      return rhs < lhs;
    }

    //*************************************************************************
    /// Greater-than-equal operator.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR14
    inline bool operator >=(const etl::chrono::year_month& lhs,
                            const etl::chrono::year_month& rhs) ETL_NOEXCEPT
    {
      return !(lhs < rhs);
    }

    //***********************************************************************
    /// Spaceship operator
    //***********************************************************************
#if ETL_USING_CPP20
    [[nodiscard]] inline constexpr auto operator <=>(const etl::chrono::year_month& lhs, 
                                                     const etl::chrono::year_month& rhs) ETL_NOEXCEPT
    {
      auto cmp = lhs.year()<=> rhs.year();

      if (cmp != 0)
      {
        return cmp;
      }
      else
      {
        return lhs.month() <=> rhs.month();
      }
    }
#endif
  }

  //*************************************************************************
  /// Hash function for etl::chrono::year_month
  //*************************************************************************
#if ETL_USING_8BIT_TYPES
  template <>
  struct hash<etl::chrono::year_month>
  {
    size_t operator()(const etl::chrono::year_month& ym) const
    {    
      etl::chrono::year::rep  y = static_cast<etl::chrono::year::rep>(static_cast<unsigned>(ym.year()));
      etl::chrono::month::rep m = static_cast<etl::chrono::month::rep>(static_cast<unsigned>(ym.month()));

      uint8_t buffer[sizeof(y) + sizeof(m)];

      memcpy(buffer,             &y, sizeof(y));
      memcpy(buffer + sizeof(y), &m, sizeof(m));

      return etl::private_hash::generic_hash<size_t>(buffer, buffer + sizeof(y) + sizeof(m));
    }
  };
#endif
}

