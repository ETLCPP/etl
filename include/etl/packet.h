///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2017 John Wellbelove

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

#ifndef ETL_PACKET_INCLUDED
#define ETL_PACKET_INCLUDED

#include "platform.h"
#include "static_assert.h"
#include "alignment.h"
#include "utility.h"
#include "placement_new.h"

//*****************************************************************************
///\defgroup packet packet
/// A class that can contain one a several related types.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //***************************************************************************
  /// A template class that can store any types derived from TBase that conform
  /// to the size and alignment requirements.
  ///\ingroup packet
  //***************************************************************************
  template <typename TBase, size_t SIZE, size_t ALIGNMENT>
  class packet
  {
  public:

    typedef TBase base_t;

#if ETL_USING_CPP11
    //***************************************************************************
    /// Constructor that static asserts any types that do not conform to the max size and alignment.
    //***************************************************************************
    template <typename T>
    explicit packet(T&& value)
    {
      typedef typename etl::types<T>::type type;

      ETL_STATIC_ASSERT((etl::is_base_of<TBase, type>::value), "Unsupported type");
      ETL_STATIC_ASSERT(sizeof(type) <= SIZE, "Unsupported size");
      ETL_STATIC_ASSERT(etl::alignment_of<type>::value <= ALIGNMENT, "Unsupported alignment");

      ::new (static_cast<type*>(data)) type(etl::forward<T>(value));
    }
#else
    //***************************************************************************
    /// Constructor that static asserts any types that do not conform to the max size and alignment.
    //***************************************************************************
    template <typename T>
    explicit packet(const T& value)
    {
      ETL_STATIC_ASSERT((etl::is_base_of<TBase, T>::value), "Unsupported type");
      ETL_STATIC_ASSERT(sizeof(T) <= SIZE, "Unsupported size");
      ETL_STATIC_ASSERT(etl::alignment_of<T>::value <= ALIGNMENT, "Unsupported alignment");

      ::new (static_cast<T*>(data)) T(value);
    }
#endif

    //***************************************************************************
    /// Destructor
    //***************************************************************************
    ~packet()
    {
      static_cast<TBase*>(data)->~TBase();
    }

#if ETL_USING_CPP11
    //***************************************************************************
    /// Assignment operator for type.
    ///\param value The value to assign.
    //***************************************************************************
    template <typename T>
    packet& operator =(T&& value)
    {
      typedef typename etl::types<T>::type type;

      ETL_STATIC_ASSERT((etl::is_base_of<TBase, type>::value), "Unsupported type");
      ETL_STATIC_ASSERT(sizeof(type) <= SIZE, "Unsupported size");
      ETL_STATIC_ASSERT(etl::alignment_of<type>::value <= ALIGNMENT, "Unsupported alignment");

      static_cast<TBase*>(data)->~TBase();
      ::new (static_cast<type*>(data)) type(etl::forward<T>(value));

      return *this;
    }
#else
    //***************************************************************************
    /// Assignment operator for type.
    ///\param value The value to assign.
    //***************************************************************************
    template <typename T>
    packet& operator =(const T& value)
    {
      ETL_STATIC_ASSERT((etl::is_base_of<TBase, T>::value), "Unsupported type");
      ETL_STATIC_ASSERT(sizeof(T) <= SIZE, "Unsupported size");
      ETL_STATIC_ASSERT(etl::alignment_of<T>::value <= ALIGNMENT, "Unsupported alignment");

      static_cast<TBase*>(data)->~TBase();
      ::new (static_cast<T*>(data)) T(value);

      return *this;
    }
#endif

    //***************************************************************************
    /// Get access to the contained object.
    //***************************************************************************
    TBase& get()
    {
      return *static_cast<TBase*>(data);
    }

    //***************************************************************************
    /// Get access to the contained object.
    //***************************************************************************
    const TBase& get() const
    {
      return *static_cast<const TBase*>(data);
    }

  private:

    packet(const packet& other);
    packet& operator =(const packet& other);

    //***************************************************************************
    /// The internal storage.
    /// Aligned on a suitable boundary, which should be good for all types.
    //***************************************************************************
    typename etl::aligned_storage<SIZE, ALIGNMENT>::type data;
  };
}

#endif
