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
      ETL_CONSTEXPR month_weekday(const etl::chrono::month&           m_, 
                                  const etl::chrono::weekday_indexed& wdi_) ETL_NOEXCEPT
        : m(m_)
        , wdi(wdi_)
      {
      }

      //*************************************************************************
      /// Returns the month.
      //*************************************************************************
      ETL_CONSTEXPR etl::chrono::month month() const ETL_NOEXCEPT
      {
        return m;
      }

      //*************************************************************************
      /// Returns the weekday_indexed.
      //*************************************************************************
      ETL_CONSTEXPR etl::chrono::weekday_indexed weekday_indexed() const ETL_NOEXCEPT
      {
        return wdi;
      }

      //*************************************************************************
      /// Returns true if the month/day is valid.
      //*************************************************************************
      ETL_CONSTEXPR bool ok() const ETL_NOEXCEPT
      {       
        return m.ok() && wdi.ok();
      }

      //*************************************************************************
      /// Equality operator.
      //*************************************************************************
      friend ETL_CONSTEXPR14 bool operator ==(const etl::chrono::month_weekday& lhs, 
                                              const etl::chrono::month_weekday& rhs) ETL_NOEXCEPT
      {
        return (lhs.m == rhs.m) && (lhs.wdi == rhs.wdi);
      }

      //*************************************************************************
      /// Equality operator.
      //*************************************************************************
      friend ETL_CONSTEXPR14 bool operator !=(const etl::chrono::month_weekday& lhs, 
                                              const etl::chrono::month_weekday& rhs) ETL_NOEXCEPT
      {
        return !(lhs == rhs);
      }

    private:

      etl::chrono::month m;
      etl::chrono::weekday_indexed wdi;
    };

    //*************************************************************************
    /// Construct from month and weekday_last.
    //*************************************************************************
    class month_weekday_last
    {
    public:

      //*************************************************************************
      /// Construct from month and weekday_indexed.
      //*************************************************************************
      ETL_CONSTEXPR month_weekday_last(const etl::chrono::month&        m_, 
                                       const etl::chrono::weekday_last& wdl_) ETL_NOEXCEPT
        : m(m_)
        , wdl(wdl_)
      {
      }

      //*************************************************************************
      /// Returns the month.
      //*************************************************************************
      ETL_CONSTEXPR etl::chrono::month month() const ETL_NOEXCEPT
      {
        return m;
      }

      //*************************************************************************
      /// Returns the weekday_indexed.
      //*************************************************************************
      ETL_CONSTEXPR etl::chrono::weekday_last weekday_last() const ETL_NOEXCEPT
      {
        return wdl;
      }

      //*************************************************************************
      /// Returns true if the month/day is valid.
      //*************************************************************************
      ETL_CONSTEXPR bool ok() const ETL_NOEXCEPT
      {       
        return m.ok() && wdl.ok();
      }

      //*************************************************************************
      /// Equality operator.
      //*************************************************************************
      friend ETL_CONSTEXPR14 bool operator ==(const etl::chrono::month_weekday_last& lhs, 
                                              const etl::chrono::month_weekday_last& rhs) ETL_NOEXCEPT
      {
        return (lhs.m == rhs.m) && (lhs.wdl == rhs.wdl);
      }

      //*************************************************************************
      /// Equality operator.
      //*************************************************************************
      friend ETL_CONSTEXPR14 bool operator !=(const etl::chrono::month_weekday_last& lhs, 
                                              const etl::chrono::month_weekday_last& rhs) ETL_NOEXCEPT
      {
        return !(lhs == rhs);
      }

    private:

      etl::chrono::month m;
      etl::chrono::weekday_last wdl;
    };  
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
      uint8_t buffer[sizeof(unsigned int) + sizeof(unsigned int)];
      
      unsigned int a = mw.month();
      unsigned int b = mw.weekday_indexed().weekday().c_encoding();
      unsigned int c = mw.weekday_indexed().index();

      memcpy(buffer,             &a, sizeof(a));
      memcpy(buffer + sizeof(a), &b, sizeof(b));
      memcpy(buffer + sizeof(b), &b, sizeof(c));

      return etl::private_hash::generic_hash<size_t>(buffer, buffer + sizeof(unsigned int) + sizeof(unsigned int) + sizeof(unsigned int));
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
      uint8_t buffer[sizeof(unsigned int) + sizeof(unsigned int)];

      unsigned int a = mw.month();
      unsigned int b = mw.weekday_last().weekday().c_encoding();

      memcpy(buffer,             &a, sizeof(a));
      memcpy(buffer + sizeof(a), &b, sizeof(b));

      return etl::private_hash::generic_hash<size_t>(buffer, buffer + sizeof(unsigned int) + sizeof(unsigned int));
    }
  };
#endif
}

