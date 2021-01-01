///******************************************************************************
//The MIT License(MIT)
//
//Embedded Template Library.
//https://github.com/ETLCPP/etl
//https://www.etlcpp.com
//
//Copyright(c) 2020 jwellbelove
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.
//******************************************************************************/

#ifndef ETL_REFERENCE_COUNTED_OBJECT_INCLUDED
#define ETL_REFERENCE_COUNTED_OBJECT_INCLUDED

#include <stdint.h>

#include "platform.h"

namespace etl
{
  //***************************************************************************
  class ireference_counted_object
  {
  public:

    virtual ~ireference_counted_object() {};
    virtual void set_reference_count(uint32_t value) = 0;
    virtual void increment_reference_count() = 0;
    ETL_NODISCARD virtual uint32_t decrement_reference_count() = 0;
    ETL_NODISCARD virtual uint32_t get_reference_count() const = 0;
  };

  //***************************************************************************
  template <typename TObject, typename TCounter>
  class reference_counted_object : public ireference_counted_object
  {
  public:

    reference_counted_object(const TObject& object_)
      : object(object_)
    {
      reference_count = 0;
    }

    TObject* get_object()
    {
      return &object;
    }
    
    const TObject* get_object() const
    {
      return &object;
    }

    virtual void set_reference_count(uint32_t value) ETL_OVERRIDE
    {
      reference_count = value;
    }

    virtual void increment_reference_count() ETL_OVERRIDE
    {
      ++reference_count;
    }

    ETL_NODISCARD virtual uint32_t decrement_reference_count() ETL_OVERRIDE
    {
      return uint32_t(--reference_count);
    }

    ETL_NODISCARD virtual uint32_t get_reference_count() const ETL_OVERRIDE
    {
      return uint32_t(reference_count);
    }

  private:

    TCounter reference_count;
    TObject  object;
  };
}

#endif
