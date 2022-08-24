///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 jwellbelove

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

#ifndef ETL_SINGLETON_INCLUDED
#define ETL_SINGLETON_INCLUDED

#include "platform.h"

namespace etl
{
  //***************************************************************************
  /// Creates .
  //***************************************************************************
  template <typename TObject>
  class singleton
  {
  public:

    static TObject& get_instance()
    {
      return p_instance.get();
    }

  protected:

    static TObject* p_instance;

  private:

    singleton() ETL_DELETE;
    singleton(const singleton&) ETL_DELETE;
    singleton& operator =(const singleton&) ETL_DELETE;
  };


  template <typename TSingleton>
  class singleton_factory
  {
    virtual ~singleton_factory() {}

    template <typename... TArgs>
    TSingleton* create(TArgs args...)
    {
      if (is_created)
      {
        return TSingleton::mp_Instance.get();
      }


      is_created = true;
      static Singleton Tmp;
      _TSingleton::mp_Instance.reset(pTmp);

      return _TSingleton::mp_Instance.get();
    }

  };
}

#endif
