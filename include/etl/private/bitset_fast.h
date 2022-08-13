///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2022 John Wellbelove

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

//***************************************************************************
/// Optimised specialisations.
//***************************************************************************

namespace etl
{
  namespace private_bitset
  {
    template <size_t Total_Bits, typename TElement>
    class bitset_fast
    {
    public:

      enum
      {
        npos = etl::integral_limits<size_t>::max
      };

      typedef TElement element_type;

      ETL_CONSTEXPR14 bitset_fast()
        : data(static_cast<element_type>(0))
      {
      }

      ETL_CONSTEXPR14 bitset_fast(unsigned long long v)
        : data(static_cast<element_type>(v))
      {
      }

      ETL_CONSTEXPR14 bitset_fast(const char* text)
        : data(0)
      {
      }

      ETL_CONSTEXPR14 bitset_fast(const wchar_t* text)
        : data(0)
      {
      }

      ETL_CONSTEXPR14 bitset_fast(const char16_t* text)
        : data(0)
      {
      }

      ETL_CONSTEXPR14 bitset_fast(const char32_t* text)
        : data(0)
      {
      }

      ETL_CONSTEXPR14 bitset_fast(const bitset_fast<Total_Bits, TElement>& other)
        : data(other.data)
      {
      }

      template <typename T>
      ETL_CONSTEXPR14 T value() const
      {
        return T(data);
      }

    private:

      element_type data;
    };
  }
}
