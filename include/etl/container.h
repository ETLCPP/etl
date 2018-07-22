///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

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

#ifndef ETL_CONTAINER_INCLUDED
#define ETL_CONTAINER_INCLUDED

#include <stddef.h>

#include "platform.h"

#include "stl/iterator.h"

///\defgroup container container
///\ingroup utilities

namespace etl
{
	//*****************************************************************************
	/// Get the 'begin' iterator.
  ///\ingroup container
	//*****************************************************************************
	template<typename TContainer>
  ETL_CONSTEXPR typename TContainer::iterator begin(TContainer& container)
	{
		return container.begin();
	}

	//*****************************************************************************
	/// Get the 'begin' const_iterator for a container.
  ///\ingroup container
	//*****************************************************************************
	template<typename TContainer>
  ETL_CONSTEXPR typename TContainer::const_iterator begin(const TContainer& container)
	{
		return container.begin();
	}

  //*****************************************************************************
  /// Get the 'begin' const_iterator for a container.
  ///\ingroup container
  //*****************************************************************************
  template<typename TContainer>
  ETL_CONSTEXPR typename TContainer::const_iterator cbegin(const TContainer& container)
  {
    return container.cbegin();
  }

  //*****************************************************************************
  /// Get the 'begin' reverse_iterator for a container.
  ///\ingroup container
  //*****************************************************************************
  template<typename TContainer>
  ETL_CONSTEXPR typename TContainer::reverse_iterator rbegin(const TContainer& container)
  {
    return container.rbegin();
  }

  //*****************************************************************************
  /// Get the 'begin' reverse_iterator for a container.
  ///\ingroup container
  //*****************************************************************************
  template<typename TContainer>
  ETL_CONSTEXPR typename TContainer::reverse_iterator crbegin(const TContainer& container)
  {
    return container.crbegin();
  }

	//*****************************************************************************
	/// Get the 'end' iterator for a container.
  ///\ingroup container
	//*****************************************************************************
	template<typename TContainer>
  ETL_CONSTEXPR typename TContainer::iterator end(TContainer& container)
	{
		return container.end();
	}

	//*****************************************************************************
	/// Get the 'end' const_iterator for a container.
  ///\ingroup container
	//*****************************************************************************
	template<typename TContainer>
  ETL_CONSTEXPR typename TContainer::const_iterator end(const TContainer& container)
	{
		return container.end();
	}

  //*****************************************************************************
  /// Get the 'end' const_iterator for a container.
  ///\ingroup container
  //*****************************************************************************
  template<typename TContainer>
  ETL_CONSTEXPR typename TContainer::const_iterator cend(const TContainer& container)
  {
    return container.cend();
  }

  //*****************************************************************************
  /// Get the 'end' reverse_iterator for a container.
  ///\ingroup container
  //*****************************************************************************
  template<typename TContainer>
  ETL_CONSTEXPR typename TContainer::const_iterator rend(const TContainer& container)
  {
    return container.rend();
  }

  //*****************************************************************************
  /// Get the 'end' reverse_iterator for a container.
  ///\ingroup container
  //*****************************************************************************
  template<typename TContainer>
  ETL_CONSTEXPR typename TContainer::reverse_iterator crend(const TContainer& container)
  {
    return container.crend();
  }

	//*****************************************************************************
	/// Get the 'begin' pointer for an array.
  ///\ingroup container
	//*****************************************************************************
	template<typename TValue, const size_t ARRAY_SIZE>
  ETL_CONSTEXPR TValue* begin(TValue(&data)[ARRAY_SIZE])
	{
		return &data[0];
	}

  //*****************************************************************************
  /// Get the 'begin' const iterator for an array.
  ///\ingroup container
  //*****************************************************************************
  template<typename TValue, const size_t ARRAY_SIZE>
  ETL_CONSTEXPR const TValue* begin(const TValue(&data)[ARRAY_SIZE])
  {
    return &data[0];
  }

  //*****************************************************************************
  /// Get the 'begin' const iterator for an array.
  ///\ingroup container
  //*****************************************************************************
  template<typename TValue, const size_t ARRAY_SIZE>
  ETL_CONSTEXPR const TValue* cbegin(const TValue(&data)[ARRAY_SIZE])
  {
    return &data[0];
  }

  //*****************************************************************************
  /// Get the 'begin' reverse_iterator for an array.
  ///\ingroup container
  //*****************************************************************************
  template<typename TValue, const size_t ARRAY_SIZE>
  ETL_CONSTEXPR std::reverse_iterator<TValue*> rbegin(const TValue(&data)[ARRAY_SIZE])
  {
    return std::reverse_iterator<TValue*>(&data[ARRAY_SIZE]);
  }

  //*****************************************************************************
  /// Get the 'begin' const reverse_iterator for an array.
  ///\ingroup container
  //*****************************************************************************
  template<typename TValue, const size_t ARRAY_SIZE>
  ETL_CONSTEXPR std::reverse_iterator<const TValue*> crbegin(const TValue(&data)[ARRAY_SIZE])
  {
    return std::reverse_iterator<const TValue*>(&data[ARRAY_SIZE]);
  }

	//*****************************************************************************
	/// Get the 'end' iterator for an array.
  ///\ingroup container
	//*****************************************************************************
	template<typename TValue, const size_t ARRAY_SIZE>
  ETL_CONSTEXPR TValue* end(TValue(&data)[ARRAY_SIZE])
	{
		return &data[ARRAY_SIZE];
	}

  //*****************************************************************************
  /// Get the 'end' const iterator for an array.
  ///\ingroup container
  //*****************************************************************************
  template<typename TValue, const size_t ARRAY_SIZE>
  ETL_CONSTEXPR const TValue* end(const TValue(&data)[ARRAY_SIZE])
  {
    return &data[ARRAY_SIZE];
  }

  //*****************************************************************************
  /// Get the 'end' const iterator for an array.
  ///\ingroup container
  //*****************************************************************************
  template<typename TValue, const size_t ARRAY_SIZE>
  ETL_CONSTEXPR const TValue* cend(const TValue(&data)[ARRAY_SIZE])
  {
    return &data[ARRAY_SIZE];
  }

  //*****************************************************************************
  /// Get the 'end' reverse_iterator for an array.
  ///\ingroup container
  //*****************************************************************************
  template<typename TValue, const size_t ARRAY_SIZE>
  ETL_CONSTEXPR std::reverse_iterator<TValue*> rend(const TValue(&data)[ARRAY_SIZE])
  {
    return std::reverse_iterator<TValue*>(&data[0]);
  }

  //*****************************************************************************
  /// Get the 'end' const reverse_iterator for an array.
  ///\ingroup container
  //*****************************************************************************
  template<typename TValue, const size_t ARRAY_SIZE>
  ETL_CONSTEXPR std::reverse_iterator<const TValue*> crend(const TValue(&data)[ARRAY_SIZE])
  {
    return std::reverse_iterator<const TValue*>(&data[0]);
  }

  //*****************************************************************************
	/// Get the next iterator.
  ///\ingroup container
	//*****************************************************************************
  template<class TIterator>
  TIterator next(TIterator iterator, ptrdiff_t n = 1)
  {
      std::advance(iterator, n);
      return iterator;
  }

  //*****************************************************************************
	/// Get the previous iterator.
  ///\ingroup container
	//*****************************************************************************
  template<class TIterator>
  TIterator prev(TIterator iterator, ptrdiff_t n = 1)
  {
      std::advance(iterator, -n);
      return iterator;
  }

  ///**************************************************************************
  /// Get the size of a container.
  /// Expects the container to have defined 'size_type'.
  ///\ingroup container
  ///**************************************************************************
  template<typename TContainer>
  ETL_CONSTEXPR typename TContainer::size_type size(const TContainer& container)
  {
    return container.size();
  }

  ///**************************************************************************
  /// Get the size of an array in elements at run time, or compile time if C++11 or above.
  ///\ingroup container
  ///**************************************************************************
  template<typename TValue, const size_t ARRAY_SIZE>
  ETL_CONSTEXPR size_t size(TValue(&)[ARRAY_SIZE])
  {
    return ARRAY_SIZE;
  }

  ///**************************************************************************
  /// Get the size of an array in elements at compile time for C++03
  ///\code
  /// sizeof(array_size(array))
  ///\endcode
  ///\ingroup container
  ///**************************************************************************
  template <typename T, const size_t ARRAY_SIZE>
  char(&array_size(T(&array)[ARRAY_SIZE]))[ARRAY_SIZE];
}

#if ETL_CPP11_SUPPORTED
  #define ETL_ARRAY_SIZE(a) (etl::size(a))
#else
  #define ETL_ARRAY_SIZE(a) sizeof(etl::array_size(a))
#endif

#endif

