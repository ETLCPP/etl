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

#ifndef ETL_MESSAGE_INCLUDED
#define ETL_MESSAGE_INCLUDED

#include "platform.h"
#include "error_handler.h"
#include "exception.h"
#include "message_types.h"
#include "type_traits.h"
#include "static_assert.h"

#include <stdint.h>

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
      : message_exception(ETL_ERROR_TEXT("message:unknown", ETL_MESSAGE_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

#if ETL_HAS_VIRTUAL_MESSAGES
  //***************************************************************************
  /// Message interface.
  /// Virtual.
  //***************************************************************************
  class imessage
  {
  public:

    //***********************************
    virtual ~imessage() ETL_NOEXCEPT
    {
    }

    //***********************************
    ETL_NODISCARD virtual etl::message_id_t get_message_id() const ETL_NOEXCEPT = 0;
  };

  //***************************************************************************
  /// Message type.
  /// Virtual.
  //***************************************************************************
  template <etl::message_id_t ID_, typename TBase = etl::imessage>
  class message : public TBase
  {
  public:

    //***********************************
    ETL_NODISCARD virtual etl::message_id_t get_message_id() const ETL_NOEXCEPT ETL_OVERRIDE
    {
      return ID;
    }

    //***********************************
    static ETL_CONSTANT etl::message_id_t ID = ID_;
  };

#else

  //***************************************************************************
  /// Message interface.
  /// Non-virtual.
  //***************************************************************************
  class imessage
  {
  public:

    //***********************************
    ETL_NODISCARD etl::message_id_t get_message_id() const ETL_NOEXCEPT
    {
      return id;
    }

  protected:

    //***********************************
    imessage(etl::message_id_t id_) ETL_NOEXCEPT
      : id(id_)
    {
    }

    //***********************************
    imessage(const imessage& other) ETL_NOEXCEPT
      : id(other.id)
    {
    }

    //***********************************
    imessage& operator =(const imessage& rhs)  ETL_NOEXCEPT
    {
      id = rhs.id;
      return *this;
    }

    //***********************************
    etl::message_id_t id;

  private:

    imessage() ETL_DELETE;
  };

  //***************************************************************************
  /// Message type.
  /// Non-virtual.
  //***************************************************************************
  template <etl::message_id_t ID_, typename TBase = etl::imessage>
  class message : public TBase
  {
  public:

    ETL_STATIC_ASSERT((etl::is_base_of<etl::imessage, TBase>::value), "TBase is not derived from etl::imessage");

    //***********************************
    message() ETL_NOEXCEPT
      : TBase(ID)
    {
    }

    //***********************************
    message(const message&) ETL_NOEXCEPT
      : TBase(ID)
    {
    }

    //***********************************
    message& operator =(const message&) ETL_NOEXCEPT
    {
      return *this;
    }

    //***********************************
    static ETL_CONSTANT etl::message_id_t ID = ID_;
  };
#endif

  //***************************************************************************
  /// The message's static ID.
  //***************************************************************************
  template <etl::message_id_t ID_, typename TBase>
  ETL_CONSTANT etl::message_id_t etl::message<ID_, TBase>::ID;
}

#endif
