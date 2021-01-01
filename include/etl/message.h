/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2017 jwellbelove

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

#ifndef ETL_MESSAGE_INCLUDED
#define ETL_MESSAGE_INCLUDED

#include <stdint.h>

#include "platform.h"
#include "error_handler.h"
#include "exception.h"
#include "message_types.h"
#include "reference_counted_object.h"

#undef ETL_FILE
#define ETL_FILE "38"

namespace etl
{
  //***************************************************************************
  class message_exception : public etl::exception
  {
  public:

    message_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  class unhandled_message_exception : public etl::message_exception
  {
  public:

    unhandled_message_exception(string_type file_name_, numeric_type line_number_)
      : message_exception(ETL_ERROR_TEXT("message:unknown", ETL_FILE"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  // Message interface.
  //***************************************************************************
  class imessage
  {
  public:

    virtual ~imessage()
    {
    }

    ETL_NODISCARD virtual etl::message_id_t get_message_id() const ETL_NOEXCEPT = 0;
  };

  //***************************************************************************
  // Message type.
  //***************************************************************************
  template <etl::message_id_t ID_>
  class message : public imessage
  {
  public:

    enum
    {
      ID = ID_
    };

    ETL_NODISCARD etl::message_id_t get_message_id() const ETL_NOEXCEPT ETL_OVERRIDE
    {
      return ID;
    }
  };
  
  class ireference_counted_message
  {
  public:

    virtual etl::imessage* get_message() = 0;
    virtual const etl::imessage* get_message() const = 0;
    virtual ~ireference_counted_message() {};
    virtual void set_reference_count(uint32_t value) = 0;
    virtual void increment_reference_count() = 0;
    ETL_NODISCARD virtual uint32_t decrement_reference_count() = 0;
    ETL_NODISCARD virtual uint32_t get_reference_count() const = 0;
  };

  //*******************************************************
  template <typename TMessage, typename TCounter>
  class reference_counted_message : virtual public ireference_counted_message
  {
  public:

    reference_counted_message(const TMessage& msg_)
      : msg(msg_)
    {
      reference_count = 0;
    }

    virtual TMessage* get_message() ETL_OVERRIDE
    {
      return &msg;
    }

    virtual const TMessage* get_message() const ETL_OVERRIDE
    {
      return &msg;
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
    TMessage msg;
  };
}

#undef ETL_FILE

#endif
