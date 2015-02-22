///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl

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

#ifndef __ETL_INSTANCE_COUNT__
#define __ETL_INSTANCE_COUNT__

///\defgroup instance_count instance count
///\ingroup utilities

namespace etl
{
  //***************************************************************************
  /// Inherit from this to count instances of a type.
  ///\ingroup reference
  //***************************************************************************
  template <typename T>
  class instance_count
  {
  public:

    //*************************************************************************
    /// Construct and add 1.
    //*************************************************************************
    instance_count()
    {
      ++how_many;
    }

    //*************************************************************************
    /// Destruct and subtract 1.
    //*************************************************************************
    virtual ~instance_count()
    {
      --how_many;
    }

    //*************************************************************************
    /// Get how many instances we have.
    //*************************************************************************
    inline static size_t get_instance_count()
    {
      return how_many;
    }

  private:

    /// The count for this type.
    static size_t how_many;
  };

  /// Initialisation.
  template <typename T> size_t instance_count<T>::how_many = 0;
}

#endif

