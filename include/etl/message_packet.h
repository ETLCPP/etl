/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2020 John Wellbelove

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

#ifndef ETL_MESSAGE_PACKET_INCLUDED
#define ETL_MESSAGE_PACKET_INCLUDED

#include "platform.h"

#include "alignment.h"
#include "error_handler.h"
#include "largest.h"
#include "message.h"
#include "static_assert.h"
#include "type_list.h"
#include "utility.h"

namespace etl
{
#if ETL_USING_CPP17 && !defined(ETL_MESSAGE_PACKET_FORCE_CPP03_IMPLEMENTATION)
  //***************************************************************************
  // The definition for all message types.
  //***************************************************************************
  template <typename... TMessageTypes>
  class message_packet
  {
  private:

    template <typename T>
    static constexpr bool IsMessagePacket = etl::is_same_v<etl::remove_const_t<etl::remove_reference_t<T>>, etl::message_packet<TMessageTypes...>>;

    template <typename T>
    static constexpr bool IsInMessageList = etl::is_one_of_v<etl::remove_const_t<etl::remove_reference_t<T>>, TMessageTypes...>;

    template <typename T>
    static constexpr bool IsIMessage = etl::is_same_v<etl::remove_const_t<etl::remove_reference_t<T>>, etl::imessage>;

  public:

    using message_types = etl::type_list<TMessageTypes...>;

      //********************************************
  #include "private/diagnostic_uninitialized_push.h"
    constexpr message_packet() noexcept
      : valid(false)
    {
    }
  #include "private/diagnostic_pop.h"

      //********************************************
      ///
      //********************************************
  #include "private/diagnostic_uninitialized_push.h"
    template <typename T, typename = typename etl::enable_if< IsIMessage<T> || IsInMessageList<T>, int>::type>
    explicit message_packet(T&& msg)
      : valid(true)
    {
      if constexpr (IsIMessage<T>)
      {
        if (accepts(msg))
        {
          add_new_message(etl::forward<T>(msg));
          valid = true;
        }
        else
        {
          valid = false;
        }

        ETL_ASSERT(valid, ETL_ERROR(unhandled_message_exception));
      }
      else if constexpr (IsInMessageList<T>)
      {
        add_new_message_type<T>(etl::forward<T>(msg));
      }
      else
      {
        ETL_STATIC_ASSERT(IsInMessageList<T>, "Message not in packet type list");
      }
    }
  #include "private/diagnostic_pop.h"

    //**********************************************
    message_packet(const message_packet& other)
    {
      valid = other.is_valid();

      if (valid)
      {
        add_new_message(other.get());
      }
    }

  #if ETL_USING_CPP11
    //**********************************************
    message_packet(message_packet&& other)
    {
      valid = other.is_valid();

      if (valid)
      {
        add_new_message(etl::move(other.get()));
      }
    }
  #endif

    //**********************************************
    void copy(const message_packet& other)
    {
      valid = other.is_valid();

      if (valid)
      {
        add_new_message(other.get());
      }
    }

    //**********************************************
    void copy(message_packet&& other)
    {
      valid = other.is_valid();

      if (valid)
      {
        add_new_message(etl::move(other.get()));
      }
    }

      //**********************************************
  #include "private/diagnostic_uninitialized_push.h"
    message_packet& operator=(const message_packet& rhs)
    {
      delete_current_message();
      valid = rhs.is_valid();
      if (valid)
      {
        add_new_message(rhs.get());
      }

      return *this;
    }
  #include "private/diagnostic_pop.h"

      //**********************************************
  #include "private/diagnostic_uninitialized_push.h"
    message_packet& operator=(message_packet&& rhs)
    {
      delete_current_message();
      valid = rhs.is_valid();
      if (valid)
      {
        add_new_message(etl::move(rhs.get()));
      }

      return *this;
    }
  #include "private/diagnostic_pop.h"

    //********************************************
    ~message_packet()
    {
      delete_current_message();
    }

    //********************************************
    etl::imessage& get() ETL_NOEXCEPT
    {
      return *static_cast<etl::imessage*>(data);
    }

    //********************************************
    const etl::imessage& get() const ETL_NOEXCEPT
    {
      return *static_cast<const etl::imessage*>(data);
    }

    //********************************************
    bool is_valid() const
    {
      return valid;
    }

    //**********************************************
    static ETL_CONSTEXPR bool accepts(etl::message_id_t id)
    {
      return (accepts_message<TMessageTypes::ID>(id) || ...);
    }

    //**********************************************
    static ETL_CONSTEXPR bool accepts(const etl::imessage& msg)
    {
      return accepts(msg.get_message_id());
    }

    //**********************************************
    template <etl::message_id_t Id>
    static ETL_CONSTEXPR bool accepts()
    {
      return (accepts_message<TMessageTypes::ID, Id>() || ...);
    }

    //**********************************************
    template <typename TMessage>
    static ETL_CONSTEXPR typename etl::enable_if<etl::is_base_of<etl::imessage, TMessage>::value, bool>::type accepts()
    {
      return accepts<TMessage::ID>();
    }

    enum
    {
      SIZE      = etl::largest<TMessageTypes...>::size,
      ALIGNMENT = etl::largest<TMessageTypes...>::alignment
    };

  private:

    //**********************************************
    template <etl::message_id_t Id1, etl::message_id_t Id2>
    static bool accepts_message()
    {
      return Id1 == Id2;
    }

    //**********************************************
    template <etl::message_id_t Id1>
    static bool accepts_message(etl::message_id_t id2)
    {
      return Id1 == id2;
    }

      //********************************************
  #include "private/diagnostic_uninitialized_push.h"
    void delete_current_message()
    {
      if (valid)
      {
        etl::imessage* pmsg = static_cast<etl::imessage*>(data);

  #if ETL_HAS_VIRTUAL_MESSAGES
        pmsg->~imessage();
  #else
        delete_message(pmsg);
  #endif
      }
    }
  #include "private/diagnostic_pop.h"

  #if !ETL_HAS_VIRTUAL_MESSAGES
    //********************************************
    void delete_message(etl::imessage* pmsg)
    {
      (delete_message_type<TMessageTypes>(pmsg) || ...);
    }

    //********************************************
    template <typename TType>
    bool delete_message_type(etl::imessage* pmsg)
    {
      if (TType::ID == pmsg->get_message_id())
      {
        TType* p = static_cast<TType*>(pmsg);
        p->~TType();
        return true;
      }
      else
      {
        return false;
      }
    }
  #endif

    //********************************************
    void add_new_message(const etl::imessage& msg)
    {
      (add_new_message_type<TMessageTypes>(msg) || ...);
    }

    //********************************************
    void add_new_message(etl::imessage&& msg)
    {
      (add_new_message_type<TMessageTypes>(etl::move(msg)) || ...);
    }

  #include "private/diagnostic_uninitialized_push.h"
    //********************************************
    /// Only enabled for types that are in the typelist.
    //********************************************
    template <typename TMessage>
    etl::enable_if_t< etl::is_one_of_v<etl::remove_const_t<etl::remove_reference_t<TMessage>>, TMessageTypes...>, void>
      add_new_message_type(TMessage&& msg)
    {
      void* p = data;
      new (p) etl::remove_reference_t<TMessage>((etl::forward<TMessage>(msg)));
    }
  #include "private/diagnostic_pop.h"

  #include "private/diagnostic_uninitialized_push.h"
    //********************************************
    template <typename TType>
    bool add_new_message_type(const etl::imessage& msg)
    {
      if (TType::ID == msg.get_message_id())
      {
        void* p = data;
        new (p) TType(static_cast<const TType&>(msg));
        return true;
      }
      else
      {
        return false;
      }
    }
  #include "private/diagnostic_pop.h"

  #include "private/diagnostic_uninitialized_push.h"
    //********************************************
    template <typename TType>
    bool add_new_message_type(etl::imessage&& msg)
    {
      if (TType::ID == msg.get_message_id())
      {
        void* p = data;
        new (p) TType(static_cast<TType&&>(msg));
        return true;
      }
      else
      {
        return false;
      }
    }
  #include "private/diagnostic_pop.h"

    typename etl::aligned_storage<SIZE, ALIGNMENT>::type data;
    bool                                                 valid;
  };

  //***************************************************************************
  // The definition for no message types.
  //***************************************************************************
  template <>
  class message_packet<>
  {
  private:

    template <typename T>
    static constexpr bool IsMessagePacket = etl::is_same_v<etl::remove_const_t<etl::remove_reference_t<T>>, etl::message_packet<>>;

    template <typename T>
    static constexpr bool IsInMessageList = false;

    template <typename T>
    static constexpr bool IsIMessage = etl::is_same_v<etl::remove_const_t<etl::remove_reference_t<T>>, etl::imessage>;

  public:

    using message_types = etl::type_list<>;

      //********************************************
  #include "private/diagnostic_uninitialized_push.h"
    constexpr message_packet() noexcept {}
  #include "private/diagnostic_pop.h"

    //**********************************************
    message_packet(const message_packet& /*other*/) {}

  #if ETL_USING_CPP11
    //**********************************************
    message_packet(message_packet&& /*other*/) {}
  #endif

    //**********************************************
    void copy(const message_packet& /*other*/) {}

    //**********************************************
    void copy(message_packet&& /*other*/) {}

      //**********************************************
  #include "private/diagnostic_uninitialized_push.h"
    message_packet& operator=(const message_packet& /*rhs*/)
    {
      return *this;
    }
  #include "private/diagnostic_pop.h"

      //**********************************************
  #include "private/diagnostic_uninitialized_push.h"
    message_packet& operator=(message_packet&& /*rhs*/)
    {
      return *this;
    }
  #include "private/diagnostic_pop.h"

    //********************************************
    ~message_packet() {}

    //********************************************
    bool is_valid() const
    {
      return false;
    }

    //**********************************************
    static ETL_CONSTEXPR bool accepts(etl::message_id_t /*id*/)
    {
      return false;
    }

    //**********************************************
    static ETL_CONSTEXPR bool accepts(const etl::imessage& /*msg*/)
    {
      return false;
    }

    //**********************************************
    template <etl::message_id_t Id>
    static ETL_CONSTEXPR bool accepts()
    {
      return false;
    }

    //**********************************************
    template <typename TMessage>
    static ETL_CONSTEXPR typename etl::enable_if<etl::is_base_of<etl::imessage, TMessage>::value, bool>::type accepts()
    {
      return false;
    }

    enum
    {
      SIZE      = 0,
      ALIGNMENT = 1
    };
  };

  //***************************************************************************
  /// Helper to turn etl::type_list<TTypes...> into
  /// etl::message_packet<TTypes...>
  template <typename TList>
  struct message_packet_from_type_list;

  template <typename... TTypes>
  struct message_packet_from_type_list<etl::type_list<TTypes...>>
  {
    using type = etl::message_packet<TTypes...>;
  };

  template <typename TTypeList>
  using message_packet_from_type_list_t = typename message_packet_from_type_list<TTypeList>::type;

#else
  #include "private/message_packet_cpp03.h"
#endif
} // namespace etl

#endif
