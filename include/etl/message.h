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
  template <etl::message_id_t ID_, typename TParent = etl::imessage>
  class message : public TParent
  {
    ETL_STATIC_ASSERT((etl::is_base_of<etl::imessage, TParent>::value), "TParent is not derived from etl::imessage");

  public:

    static ETL_CONSTANT etl::message_id_t ID = ID_;

    ETL_NODISCARD etl::message_id_t get_message_id() const ETL_NOEXCEPT ETL_OVERRIDE
    {
      return ID;
    }
  };

  template <etl::message_id_t ID_, typename TParent>
  ETL_CONSTANT etl::message_id_t message<ID_, TParent>::ID;
}

#endif
