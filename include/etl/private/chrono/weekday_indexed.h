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
        , i(index_)
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
      unsigned             i;
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
    /// weekday index operator from index
    //***********************************************************************
    ETL_CONSTEXPR etl::chrono::weekday_indexed etl::chrono::weekday::operator[](unsigned index) const ETL_NOEXCEPT
    {
      return etl::chrono::weekday_indexed(*this, index);
    }
  }

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
}
