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

#if !defined(__ETL_IN_IVECTOR_H__) && !defined(__ETL_IN_PVOIDVECTOR__)
#error This header is a private element of etl::vector, etl::ivector & etl::pvoidvector
#endif

#ifndef __ETL_VECTOR_BASE__
#define __ETL_VECTOR_BASE__

#include <stddef.h>

#include "../exception.h"
#include "../error_handler.h"
#include "../debug_count.h"

#define ETL_FILE "17"

namespace etl
{
  //***************************************************************************
  ///\ingroup vector
  /// Exception base for vectors
  //***************************************************************************
  class vector_exception : public exception
  {
  public:

    vector_exception(string_type what, string_type file_name, numeric_type line_number)
      : exception(what, file_name, line_number)
    {
    }
  };

  //***************************************************************************
  ///\ingroup vector
  /// Vector full exception.
  //***************************************************************************
  class vector_full : public vector_exception
  {
  public:

    vector_full(string_type file_name, numeric_type line_number)
      : vector_exception(ETL_ERROR_TEXT("vector:full", ETL_FILE"A"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  ///\ingroup vector
  /// Vector empty exception.
  //***************************************************************************
  class vector_empty : public vector_exception
  {
  public:

    vector_empty(string_type file_name, numeric_type line_number)
      : vector_exception(ETL_ERROR_TEXT("vector:empty", ETL_FILE"B"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  ///\ingroup vector
  /// Vector out of bounds exception.
  //***************************************************************************
  class vector_out_of_bounds : public vector_exception
  {
  public:

    vector_out_of_bounds(string_type file_name, numeric_type line_number)
      : vector_exception(ETL_ERROR_TEXT("vector:bounds", ETL_FILE"C"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  ///\ingroup vector
  /// The base class for all templated vector types.
  //***************************************************************************
  class vector_base
  {
  public:

    typedef size_t size_type;

    //*************************************************************************
    /// Returns the capacity of the vector.
    ///\return The capacity of the vector.
    //*************************************************************************
    size_type capacity() const
    {
      return MAX_SIZE;
    }

    //*************************************************************************
    /// Returns the maximum possible size of the vector.
    ///\return The maximum size of the vector.
    //*************************************************************************
    size_type max_size() const
    {
      return MAX_SIZE;
    }

  protected:

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    vector_base(size_t max_size)
      : MAX_SIZE(max_size)
    {
    }

    const size_type MAX_SIZE;         ///<The maximum number of elements in the vector.
    etl::debug_count construct_count; ///< Internal debugging.
  };
}

#undef ETL_FILE

#endif
