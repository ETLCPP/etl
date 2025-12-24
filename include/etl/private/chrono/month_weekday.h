///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2025 John Wellbelove

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
    class month_weekday
    {
    public:

      //*************************************************************************
      /// Construct from month and weekday_indexed.
      //*************************************************************************
      ETL_CONSTEXPR14 month_weekday(const etl::chrono::month&           m_, 
                                    const etl::chrono::weekday_indexed& wdi_) ETL_NOEXCEPT
        : m(m_)
        , wdi(wdi_)
      {
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
      /// Returns the weekday_indexed.
      //*************************************************************************
      ETL_NODISCARD
      ETL_CONSTEXPR14 etl::chrono::weekday_indexed weekday_indexed() const ETL_NOEXCEPT
      {
        return wdi;
      }

      //*************************************************************************
      /// Returns true if the month/day is valid.
      //*************************************************************************
      ETL_NODISCARD
      ETL_CONSTEXPR14 bool ok() const ETL_NOEXCEPT
      {       
        return m.ok() && wdi.ok();
      }

    private:

      etl::chrono::month m;
      etl::chrono::weekday_indexed wdi;
    };

    //*************************************************************************
    /// Equality operator.
    //*************************************************************************
    inline ETL_CONSTEXPR14 bool operator ==(const etl::chrono::month_weekday& lhs, 
                                            const etl::chrono::month_weekday& rhs) ETL_NOEXCEPT
    {
      return (lhs.month() == rhs.month()) &&
             (lhs.weekday_indexed() == rhs.weekday_indexed());
    }

    //*************************************************************************
    /// Equality operator.
    //*************************************************************************
    inline ETL_CONSTEXPR14 bool operator !=(const etl::chrono::month_weekday& lhs, 
                                            const etl::chrono::month_weekday& rhs) ETL_NOEXCEPT
    {
      return !(lhs == rhs);
    }

    //*************************************************************************
    /// Construct from month and weekday_last.
    //*************************************************************************
    class month_weekday_last
    {
    public:

      //*************************************************************************
      /// Construct from month and weekday_indexed.
      //*************************************************************************
      ETL_CONSTEXPR14 month_weekday_last(const etl::chrono::month&        m_, 
                                       const etl::chrono::weekday_last& wdl_) ETL_NOEXCEPT
        : m(m_)
        , wdl(wdl_)
      {
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
      /// Returns the weekday_indexed.
      //*************************************************************************
      ETL_NODISCARD
      ETL_CONSTEXPR14 etl::chrono::weekday_last weekday_last() const ETL_NOEXCEPT
      {
        return wdl;
      }

      //*************************************************************************
      /// Returns true if the month/day is valid.
      //*************************************************************************
      ETL_NODISCARD
      ETL_CONSTEXPR14 bool ok() const ETL_NOEXCEPT
      {       
        return m.ok() && wdl.ok();
      }

    private:

      etl::chrono::month m;
      etl::chrono::weekday_last wdl;
    };  

    //*************************************************************************
    /// Equality operator.
    //*************************************************************************
    inline ETL_CONSTEXPR14 bool operator ==(const etl::chrono::month_weekday_last& lhs, 
                                            const etl::chrono::month_weekday_last& rhs) ETL_NOEXCEPT
    {
      return (lhs.month() == rhs.month()) && (lhs.weekday_last() == rhs.weekday_last());
    }

    //*************************************************************************
    /// Equality operator.
    //*************************************************************************
    inline ETL_CONSTEXPR14 bool operator !=(const etl::chrono::month_weekday_last& lhs, 
                                            const etl::chrono::month_weekday_last& rhs) ETL_NOEXCEPT
    {
      return !(lhs == rhs);
    }
  }

  //*************************************************************************
  /// Hash function for etl::chrono::month_weekday
  //*************************************************************************
#if ETL_USING_8BIT_TYPES
  template <>
  struct hash<etl::chrono::month_weekday>
  {
    size_t operator()(const etl::chrono::month_weekday& mw) const
    {         
      etl::chrono::month::rep a = static_cast<etl::chrono::month::rep>(static_cast<unsigned>(mw.month()));
      unsigned int            b = mw.weekday_indexed().weekday().c_encoding();
      unsigned int            c = mw.weekday_indexed().index();

      uint8_t buffer[sizeof(a) + sizeof(b) + sizeof(c)];

      memcpy(buffer,             &a, sizeof(a));
      memcpy(buffer + sizeof(a), &b, sizeof(b));
      memcpy(buffer + sizeof(b), &b, sizeof(c));

      return etl::private_hash::generic_hash<size_t>(buffer, buffer + sizeof(a) + sizeof(b) + sizeof(c));
    }
  };
#endif

  //*************************************************************************
  /// Hash function for etl::chrono::month_weekday_last
  //*************************************************************************
#if ETL_USING_8BIT_TYPES
  template <>
  struct hash<etl::chrono::month_weekday_last>
  {
    size_t operator()(const etl::chrono::month_weekday_last& mw) const
    {
      etl::chrono::month::rep a = static_cast<etl::chrono::month::rep>(static_cast<unsigned>(mw.month()));
      unsigned int            b = mw.weekday_last().weekday().c_encoding();

      uint8_t buffer[sizeof(a) + sizeof(b)];

      memcpy(buffer,             &a, sizeof(a));
      memcpy(buffer + sizeof(a), &b, sizeof(b));

      return etl::private_hash::generic_hash<size_t>(buffer, buffer + sizeof(a) + sizeof(b));
    }
  };
#endif
}

