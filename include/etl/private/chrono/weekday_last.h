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
    /// weekday_last
    //***********************************************************************
    class weekday_last
    {
    public:

      //***********************************************************************
      /// Default constructor
      //***********************************************************************
      ETL_CONSTEXPR weekday_last() ETL_NOEXCEPT
      {
      }

      //***********************************************************************
      /// Construct from unsigned
      //***********************************************************************
      ETL_CONSTEXPR weekday_last(const etl::chrono::weekday& wd_) ETL_NOEXCEPT
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
    /// Inequality operator
    //***********************************************************************
    ETL_CONSTEXPR bool operator !=(const etl::chrono::weekday_last& wd1, const etl::chrono::weekday_last& wd2) ETL_NOEXCEPT
    {
      return !(wd1 == wd2);
    }

    //***********************************************************************
    /// weekday index operator from index
    //***********************************************************************
    ETL_CONSTEXPR etl::chrono::weekday_last etl::chrono::weekday::operator[](etl::chrono::last_spec) const ETL_NOEXCEPT
    {
      return etl::chrono::weekday_last(*this);
    }
  }

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
