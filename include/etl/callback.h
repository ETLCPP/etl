///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2015 jwellbelove

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

#ifndef __ETL_CALLBACK__
#define __ETL_CALLBACK__

namespace etl
{
  //***************************************************************************
  /// A callback class designed to be multiply inherited by other client classes.
  /// The class is parametrised with a callback parameter type and a unique id.
  /// The unique id allows multiple callbacks with the same parameter type.
  ///\tparam TParameter The callback parameter type.
  ///\tparam ID The unique id for this callback.
  //***************************************************************************
  template <typename TParameter, const int ID>
  class callback
  {
  private:

    // Creates a parameter type unique to this ID.
    template <typename T, const int I>
    struct parameter
    {
        parameter(T value_)
            : value(value_)
        {
        }

        typedef T value_type;

        T value;

    private:

        parameter();
    };

    // Specialisation for void.
    template <const int I>
    struct parameter<void, I>
    {
        typedef void value_type;
    };

  public:

    typedef parameter<TParameter, ID> type;

    virtual void etl_callback(type p = type()) = 0;
  };
}

#endif
