/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

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
  class imessage
  {
  public:

    imessage(etl::message_id_t id) 
      : message_id(id)
    {
    }

    const etl::message_id_t message_id;

#if defined(ETL_MESSAGES_ARE_VIRTUAL) || defined(ETL_POLYMORPHIC_MESSAGES)
    virtual ~imessage()
    {
    }
#else
    ~imessage()
    {
    }
#endif
  };

  //***************************************************************************
  template <const etl::message_id_t ID_>
  class message : public imessage
  {
  public:

    message()
      : imessage(ID_)
    {
    }

    enum
    {
      ID = ID_
    };
  };
}

#undef ETL_FILE

#endif
