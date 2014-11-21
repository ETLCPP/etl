///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.

Copyright(c) 2014 jwellbelove

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

#ifndef __ETL_CYCLIC_VALUE__
#define __ETL_CYCLIC_VALUE__

#include <cstddef>

///\defgroup cyclic_value cyclic_value
/// Provides a value that cycles between two limits.
/// \ingroup utilities

#include <algorithm>

#include "static_assert.h"
#include "exception.h"

namespace etl
{
	//***************************************************************************
	/// Provides a value that cycles between two limits.
  /// Supports incrementing and decrementing.
	///\tparam T     The type of the variable.
	///\tparam FIRST The first value of the range.
	///\tparam LAST  The last value of the range.
  ///\ingroup cyclic_value
	//***************************************************************************
	template <typename T, const T FIRST = T(), const T LAST = T()>
	class cyclic_value
	{
	public:

		//*************************************************************************
		/// Constructor.
		/// Sets 'first' and 'last' to the template parameter values.
    /// The initial value is set to the first value.
		//*************************************************************************
		cyclic_value()
			: value(FIRST),
        first_value(FIRST),
			  last_value(LAST)
		{
		}

		//*************************************************************************
		/// Constructor.
    /// Sets the value to the first of the range.
    ///\param first The first value in the range.
		///\param last  The last value in the range.
		//*************************************************************************
		cyclic_value(const T& first_, const T& last_)
			: value(first_),
        first_value(first_),
			  last_value(last_)
		{
		}

		//*************************************************************************
		/// Sets the range.
		/// Sets the value to the first of the range.
    ///\param first The first value in the range.
		///\param last  The last value in the range.
  	//*************************************************************************
		void set(const T& first_, const T& last_)
		{
			first_value = first_;
			last_value  = last_;
			value       = first_;
		}

    //*************************************************************************
    /// Resets the value to the first in the range.
    //*************************************************************************
    void to_first()
    {
      value = first_value;
    }

    //*************************************************************************
    /// Resets the value to the last in the range.
    //*************************************************************************
    void to_last()
    {
      value = last_value;
    }

    //*************************************************************************
    /// Advances to value by a number of steps.
    ///\param n The number of steps to advance.
    //*************************************************************************
    void advance(int n)
    {
      if (n > 0)
      {
        for (int i = 0; i < n; ++i)
        {
          operator ++();
        }
      }
      else
      {
        for (int i = 0; i < -n; ++i)
        {
          operator --();
        }
      }
    }

		//*************************************************************************
		/// Conversion operator.
    /// \return The value of the underlying type.
		//*************************************************************************
		operator T()
		{
			return value;
		}

		//*************************************************************************
		/// Const conversion operator.
		/// \return The value of the underlying type.
		//*************************************************************************
		operator const T() const
		{
			return value;
		}

		//*************************************************************************
		/// ++ operator.
		//*************************************************************************
		cyclic_value& operator ++()
		{
			if (value == last_value)
			{
				value = first_value;
			}
			else
			{
        ++value;
			}

			return *this;
		}

		//*************************************************************************
		/// ++ operator.
		//*************************************************************************
		cyclic_value operator ++(int)
		{
			cyclic_value temp(*this);

			operator++();

			return temp;
		}

		//*************************************************************************
		/// -- operator.
		//*************************************************************************
		cyclic_value& operator --()
		{
			if (value == first_value)
			{
				value = last_value;
			}
			else
			{
				--value;
			}

			return *this;
		}

		//*************************************************************************
		/// -- operator.
		//*************************************************************************
		cyclic_value operator --(int)
		{
			cyclic_value temp(*this);

			operator--();

			return temp;
		}

    //*************************************************************************
		/// Gets the first value.
		//*************************************************************************
    const T& first() const
    {
      return first_value;
    }

  	//*************************************************************************
		/// Gets the last value.
		//*************************************************************************
    const T& last() const
    {
      return last_value;
    }

	private:

    T value;       ///< The current value.
    T first_value; ///< The first value in the range.
		T last_value;  ///< The last value in the range.
	};
}

#endif
