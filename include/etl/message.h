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

  class message_tag {};

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
  class message : public TBase, public etl::message_tag
  {
  public:

    ETL_STATIC_ASSERT((etl::is_base_of<etl::imessage, TBase>::value), "TBase is not derived from etl::imessage");

    typedef TBase base_type;

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
  class message : public TBase, public etl::message_tag
  {
  public:

    ETL_STATIC_ASSERT((etl::is_base_of<etl::imessage, TBase>::value), "TBase is not derived from etl::imessage");

    typedef TBase base_type;

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

  //***************************************************************************
  /// Is T an etl::imessage?
  //***************************************************************************
  template <typename T>
  struct is_imessage : public etl::bool_constant<etl::is_same<etl::imessage, typename etl::remove_cvref<T>::type>::value>
  {
  };

  //***************************************************************************
  /// Is T ultimately derived from etl::imessage?
  //***************************************************************************
  template <typename T>
  struct is_message : public etl::bool_constant<etl::is_base_of<etl::imessage, typename etl::remove_cvref<T>::type>::value>
  {
  };

  //***************************************************************************
  /// Is T an etl::message<> or derived from etl::message<>
  //***************************************************************************
  template <typename T>
  struct is_message_type : public etl::bool_constant<etl::is_base_of<etl::message_tag, typename etl::remove_cvref<T>::type>::value>
  {
  };

  //***************************************************************************
  /// Is T a base of etl::message<T>
  //***************************************************************************
  template <typename T>
  struct is_message_base : public etl::bool_constant<etl::is_message<T>::value && !etl::is_message_type<T>::value>
  {
  };

  //***************************************************************************
  /// Is T a user defined base of etl::message<T> and not an etl::imessage
  //***************************************************************************
  template <typename T>
  struct is_user_message_base : public etl::bool_constant<etl::is_message_base<T>::value && !etl::is_imessage<T>::value>
  {
  };

#if ETL_USING_CPP17
  //***************************************************************************
  /// Is T an etl::imessage?
  //***************************************************************************
  template <typename T>
  inline constexpr bool is_imessage_v = is_imessage<T>::value;

  //***************************************************************************
  /// Is T ultimately derived from etl::imessage?
  //***************************************************************************
  template <typename T>
  inline constexpr bool is_message_v = is_message<T>::value;

  //***************************************************************************
  /// Is T derived from etl::message<>
  //***************************************************************************
  template <typename T>
  inline constexpr bool is_message_type_v = is_message_type<T>::value;

  //***************************************************************************
  /// Is T a base of etl::message<T>
  //***************************************************************************
  template <typename T>
  inline constexpr bool is_message_base_v = is_message_base<T>::value;

  //***************************************************************************
  /// Is T a user defined base of etl::message<T>
  //***************************************************************************
  template <typename T>
  inline constexpr bool is_user_message_base_v = is_user_message_base<T>::value;
#endif
}

#endif
