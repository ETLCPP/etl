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

#ifndef __ETL_MESSAGE_ROUTER__
#define __ETL_MESSAGE_ROUTER__

#include <stdint.h>

namespace etl
{
  /// Allow alternative type for message id.
#if !defined(ETL_MESSAGE_ID_TYPE)
    typedef uint_least8_t message_id_t;
#else
    typedef ETL_MESSAGE_ID_TYPE message_id_t;
#endif

  //***************************************************************************
  class imessage
  {
  public:

    virtual ~imessage() {}
    virtual etl::message_id_t get_message_id() const = 0;
  };

  //***************************************************************************
  template <const size_t ID_>
  class message : public imessage
  {
  public:

    enum
    {
      ID = ID_
    };

    //*******************************************
    etl::message_id_t get_message_id() const
    {
      return etl::message_id_t(ID);
    }
  };

  //***************************************************************************
  class imessage_router
  {
  public:

    virtual ~imessage_router() {}
    virtual void receive(const etl::imessage& message) = 0;
    virtual void receive(imessage_router& source, const etl::imessage& message) = 0;
    virtual bool accepts(etl::message_id_t id) const = 0;

    //*******************************************
    bool accepts(const etl::imessage& msg) const
    {
      return accepts(msg.get_message_id());
    }

    //*******************************************
    void send_message(imessage_router& destination,
                      const etl::imessage& message)
    {
      destination.receive(*this, message);
    }
  };

  //***************************************************************************
  /// This router can be used either as a sink for messages
  /// or as a producer-only of messages such an interrupt routine.
  //***************************************************************************
  class null_message_router : public imessage_router
  {
  public:

    //*******************************************
    void receive(const etl::imessage& message)
    {
    }

    //*******************************************
    void receive(etl::imessage_router& source, const etl::imessage& message)
    {
    }

    //*******************************************
    bool accepts(etl::message_id_t id) const
    {
      return false;
    }

    //*******************************************
    static null_message_router& get()
    {
      static null_message_router instance;
      return instance;
    }
  };

  //***************************************************************************
  /// Send a message to a router.
  /// Sets the 'sender' to etl::null_message_router type.
  //***************************************************************************
  inline static void send_message(etl::imessage_router& destination,
                                  const etl::imessage&  message)
  {
    destination.receive(message);
  }

  //***************************************************************************
  /// Send a message to a router.
  //***************************************************************************
  inline static void send_message(etl::imessage_router& source,
                                  etl::imessage_router& destination,
                                  const etl::imessage&  message)
  {
    destination.receive(source, message);
  }

  //***************************************************************************
  // To generate to header file, run this at the command line.
  // Note: You will need Python and COG installed.
  //
  // python -m cogapp -d -e -omessage_router.h -DHandlers=<n> message_router_generator.h
  // Where <n> is the number of messages to support.
  //
  // e.g.
  // To generate handlers for up to 16 messages...
  // python -m cogapp -d -e -omessage_router.h -DHandlers=16 message_router_generator.h
  //
  // See CreateMessageProcessor.bat
  //***************************************************************************

  //***************************************************************************
  // The code below has been auto generated. Do not manually edit.
  //***************************************************************************

  //***************************************************************************
  // The definition for all 16 message types.
  //***************************************************************************
  template <typename TProcessor,
            typename T1, typename T2 = void, typename T3 = void, typename T4 = void,
            typename T5 = void, typename T6 = void, typename T7 = void, typename T8 = void,
            typename T9 = void, typename T10 = void, typename T11 = void, typename T12 = void,
            typename T13 = void, typename T14 = void, typename T15 = void, typename T16 = void>
  class message_router  : public imessage_router
  {
  public:

    void receive(const etl::imessage& msg)
    {
      receive(etl::null_message_router::get(), msg);
    }

    void receive(etl::imessage_router& source, const etl::imessage& msg)
    {
      const etl::message_id_t id = msg.get_message_id();

      switch (id)
      {
        case T1::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T1&>(msg)); break;
        case T2::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T2&>(msg)); break;
        case T3::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T3&>(msg)); break;
        case T4::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T4&>(msg)); break;
        case T5::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T5&>(msg)); break;
        case T6::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T6&>(msg)); break;
        case T7::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T7&>(msg)); break;
        case T8::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T8&>(msg)); break;
        case T9::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T9&>(msg)); break;
        case T10::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T10&>(msg)); break;
        case T11::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T11&>(msg)); break;
        case T12::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T12&>(msg)); break;
        case T13::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T13&>(msg)); break;
        case T14::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T14&>(msg)); break;
        case T15::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T15&>(msg)); break;
        case T16::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T16&>(msg)); break;
        default:  static_cast<TProcessor*>(this)->on_receive_unknown(source, msg); break;
      }
    }

    bool accepts(etl::message_id_t id) const
    {
      switch (id)
      {
        case T1::ID: case T2::ID: case T3::ID: case T4::ID: case T5::ID: case T6::ID: case T7::ID: case T8::ID:
        case T9::ID: case T10::ID: case T11::ID: case T12::ID: case T13::ID: case T14::ID: case T15::ID: case T16::ID:
          return true; break;
        default:
          return false; break;
      }
    }
  };

  //***************************************************************************
  // Specialisation for 15 message types.
  //***************************************************************************
  template <typename TProcessor,
            typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7, typename T8,
            typename T9, typename T10, typename T11, typename T12,
            typename T13, typename T14, typename T15>
  class message_router<TProcessor, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, void>
   : public imessage_router
  {
  public:

    void receive(const etl::imessage& msg)
    {
      receive(etl::null_message_router::get(), msg);
    }

    void receive(etl::imessage_router& source, const etl::imessage& msg)
    {
      const size_t id = msg.get_message_id();

      switch (id)
      {
        case T1::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T1&>(msg)); break;
        case T2::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T2&>(msg)); break;
        case T3::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T3&>(msg)); break;
        case T4::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T4&>(msg)); break;
        case T5::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T5&>(msg)); break;
        case T6::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T6&>(msg)); break;
        case T7::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T7&>(msg)); break;
        case T8::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T8&>(msg)); break;
        case T9::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T9&>(msg)); break;
        case T10::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T10&>(msg)); break;
        case T11::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T11&>(msg)); break;
        case T12::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T12&>(msg)); break;
        case T13::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T13&>(msg)); break;
        case T14::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T14&>(msg)); break;
        case T15::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T15&>(msg)); break;
        default: static_cast<TProcessor*>(this)->on_receive_unknown(source, msg); break;
      }
    }

    bool accepts(etl::message_id_t id) const
    {
      switch (id)
      {
        case T1::ID: case T2::ID: case T3::ID: case T4::ID: case T5::ID: case T6::ID: case T7::ID: case T8::ID:
        case T9::ID: case T10::ID: case T11::ID: case T12::ID: case T13::ID: case T14::ID: case T15::ID:
          return true; break;
        default:
          return false; break;
      }
    }
  };

  //***************************************************************************
  // Specialisation for 14 message types.
  //***************************************************************************
  template <typename TProcessor,
            typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7, typename T8,
            typename T9, typename T10, typename T11, typename T12,
            typename T13, typename T14>
  class message_router<TProcessor, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, void, void>
   : public imessage_router
  {
  public:

    void receive(const etl::imessage& msg)
    {
      receive(etl::null_message_router::get(), msg);
    }

    void receive(etl::imessage_router& source, const etl::imessage& msg)
    {
      const size_t id = msg.get_message_id();

      switch (id)
      {
        case T1::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T1&>(msg)); break;
        case T2::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T2&>(msg)); break;
        case T3::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T3&>(msg)); break;
        case T4::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T4&>(msg)); break;
        case T5::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T5&>(msg)); break;
        case T6::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T6&>(msg)); break;
        case T7::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T7&>(msg)); break;
        case T8::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T8&>(msg)); break;
        case T9::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T9&>(msg)); break;
        case T10::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T10&>(msg)); break;
        case T11::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T11&>(msg)); break;
        case T12::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T12&>(msg)); break;
        case T13::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T13&>(msg)); break;
        case T14::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T14&>(msg)); break;
        default: static_cast<TProcessor*>(this)->on_receive_unknown(source, msg); break;
      }
    }

    bool accepts(etl::message_id_t id) const
    {
      switch (id)
      {
        case T1::ID: case T2::ID: case T3::ID: case T4::ID: case T5::ID: case T6::ID: case T7::ID: case T8::ID:
        case T9::ID: case T10::ID: case T11::ID: case T12::ID: case T13::ID: case T14::ID:
          return true; break;
        default:
          return false; break;
      }
    }
  };

  //***************************************************************************
  // Specialisation for 13 message types.
  //***************************************************************************
  template <typename TProcessor,
            typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7, typename T8,
            typename T9, typename T10, typename T11, typename T12,
            typename T13>
  class message_router<TProcessor, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, void, void, void>
   : public imessage_router
  {
  public:

    void receive(const etl::imessage& msg)
    {
      receive(etl::null_message_router::get(), msg);
    }

    void receive(etl::imessage_router& source, const etl::imessage& msg)
    {
      const size_t id = msg.get_message_id();

      switch (id)
      {
        case T1::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T1&>(msg)); break;
        case T2::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T2&>(msg)); break;
        case T3::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T3&>(msg)); break;
        case T4::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T4&>(msg)); break;
        case T5::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T5&>(msg)); break;
        case T6::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T6&>(msg)); break;
        case T7::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T7&>(msg)); break;
        case T8::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T8&>(msg)); break;
        case T9::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T9&>(msg)); break;
        case T10::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T10&>(msg)); break;
        case T11::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T11&>(msg)); break;
        case T12::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T12&>(msg)); break;
        case T13::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T13&>(msg)); break;
        default: static_cast<TProcessor*>(this)->on_receive_unknown(source, msg); break;
      }
    }

    bool accepts(etl::message_id_t id) const
    {
      switch (id)
      {
        case T1::ID: case T2::ID: case T3::ID: case T4::ID: case T5::ID: case T6::ID: case T7::ID: case T8::ID:
        case T9::ID: case T10::ID: case T11::ID: case T12::ID: case T13::ID:
          return true; break;
        default:
          return false; break;
      }
    }
  };

  //***************************************************************************
  // Specialisation for 12 message types.
  //***************************************************************************
  template <typename TProcessor,
            typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7, typename T8,
            typename T9, typename T10, typename T11, typename T12>
  class message_router<TProcessor, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, void, void, void, void>
   : public imessage_router
  {
  public:

    void receive(const etl::imessage& msg)
    {
      receive(etl::null_message_router::get(), msg);
    }

    void receive(etl::imessage_router& source, const etl::imessage& msg)
    {
      const size_t id = msg.get_message_id();

      switch (id)
      {
        case T1::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T1&>(msg)); break;
        case T2::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T2&>(msg)); break;
        case T3::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T3&>(msg)); break;
        case T4::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T4&>(msg)); break;
        case T5::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T5&>(msg)); break;
        case T6::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T6&>(msg)); break;
        case T7::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T7&>(msg)); break;
        case T8::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T8&>(msg)); break;
        case T9::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T9&>(msg)); break;
        case T10::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T10&>(msg)); break;
        case T11::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T11&>(msg)); break;
        case T12::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T12&>(msg)); break;
        default: static_cast<TProcessor*>(this)->on_receive_unknown(source, msg); break;
      }
    }

    bool accepts(etl::message_id_t id) const
    {
      switch (id)
      {
        case T1::ID: case T2::ID: case T3::ID: case T4::ID: case T5::ID: case T6::ID: case T7::ID: case T8::ID:
        case T9::ID: case T10::ID: case T11::ID: case T12::ID:
          return true; break;
        default:
          return false; break;
      }
    }
  };

  //***************************************************************************
  // Specialisation for 11 message types.
  //***************************************************************************
  template <typename TProcessor,
            typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7, typename T8,
            typename T9, typename T10, typename T11>
  class message_router<TProcessor, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, void, void, void, void, void>
   : public imessage_router
  {
  public:

    void receive(const etl::imessage& msg)
    {
      receive(etl::null_message_router::get(), msg);
    }

    void receive(etl::imessage_router& source, const etl::imessage& msg)
    {
      const size_t id = msg.get_message_id();

      switch (id)
      {
        case T1::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T1&>(msg)); break;
        case T2::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T2&>(msg)); break;
        case T3::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T3&>(msg)); break;
        case T4::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T4&>(msg)); break;
        case T5::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T5&>(msg)); break;
        case T6::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T6&>(msg)); break;
        case T7::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T7&>(msg)); break;
        case T8::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T8&>(msg)); break;
        case T9::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T9&>(msg)); break;
        case T10::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T10&>(msg)); break;
        case T11::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T11&>(msg)); break;
        default: static_cast<TProcessor*>(this)->on_receive_unknown(source, msg); break;
      }
    }

    bool accepts(etl::message_id_t id) const
    {
      switch (id)
      {
        case T1::ID: case T2::ID: case T3::ID: case T4::ID: case T5::ID: case T6::ID: case T7::ID: case T8::ID:
        case T9::ID: case T10::ID: case T11::ID:
          return true; break;
        default:
          return false; break;
      }
    }
  };

  //***************************************************************************
  // Specialisation for 10 message types.
  //***************************************************************************
  template <typename TProcessor,
            typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7, typename T8,
            typename T9, typename T10>
  class message_router<TProcessor, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, void, void, void, void, void, void>
   : public imessage_router
  {
  public:

    void receive(const etl::imessage& msg)
    {
      receive(etl::null_message_router::get(), msg);
    }

    void receive(etl::imessage_router& source, const etl::imessage& msg)
    {
      const size_t id = msg.get_message_id();

      switch (id)
      {
        case T1::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T1&>(msg)); break;
        case T2::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T2&>(msg)); break;
        case T3::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T3&>(msg)); break;
        case T4::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T4&>(msg)); break;
        case T5::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T5&>(msg)); break;
        case T6::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T6&>(msg)); break;
        case T7::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T7&>(msg)); break;
        case T8::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T8&>(msg)); break;
        case T9::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T9&>(msg)); break;
        case T10::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T10&>(msg)); break;
        default: static_cast<TProcessor*>(this)->on_receive_unknown(source, msg); break;
      }
    }

    bool accepts(etl::message_id_t id) const
    {
      switch (id)
      {
        case T1::ID: case T2::ID: case T3::ID: case T4::ID: case T5::ID: case T6::ID: case T7::ID: case T8::ID:
        case T9::ID: case T10::ID:
          return true; break;
        default:
          return false; break;
      }
    }
  };

  //***************************************************************************
  // Specialisation for 9 message types.
  //***************************************************************************
  template <typename TProcessor,
            typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7, typename T8,
            typename T9>
  class message_router<TProcessor, T1, T2, T3, T4, T5, T6, T7, T8, T9, void, void, void, void, void, void, void>
   : public imessage_router
  {
  public:

    void receive(const etl::imessage& msg)
    {
      receive(etl::null_message_router::get(), msg);
    }

    void receive(etl::imessage_router& source, const etl::imessage& msg)
    {
      const size_t id = msg.get_message_id();

      switch (id)
      {
        case T1::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T1&>(msg)); break;
        case T2::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T2&>(msg)); break;
        case T3::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T3&>(msg)); break;
        case T4::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T4&>(msg)); break;
        case T5::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T5&>(msg)); break;
        case T6::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T6&>(msg)); break;
        case T7::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T7&>(msg)); break;
        case T8::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T8&>(msg)); break;
        case T9::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T9&>(msg)); break;
        default: static_cast<TProcessor*>(this)->on_receive_unknown(source, msg); break;
      }
    }

    bool accepts(etl::message_id_t id) const
    {
      switch (id)
      {
        case T1::ID: case T2::ID: case T3::ID: case T4::ID: case T5::ID: case T6::ID: case T7::ID: case T8::ID:
        case T9::ID:
          return true; break;
        default:
          return false; break;
      }
    }
  };

  //***************************************************************************
  // Specialisation for 8 message types.
  //***************************************************************************
  template <typename TProcessor,
            typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7, typename T8>
  class message_router<TProcessor, T1, T2, T3, T4, T5, T6, T7, T8, void, void, void, void, void, void, void, void>
   : public imessage_router
  {
  public:

    void receive(const etl::imessage& msg)
    {
      receive(etl::null_message_router::get(), msg);
    }

    void receive(etl::imessage_router& source, const etl::imessage& msg)
    {
      const size_t id = msg.get_message_id();

      switch (id)
      {
        case T1::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T1&>(msg)); break;
        case T2::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T2&>(msg)); break;
        case T3::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T3&>(msg)); break;
        case T4::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T4&>(msg)); break;
        case T5::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T5&>(msg)); break;
        case T6::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T6&>(msg)); break;
        case T7::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T7&>(msg)); break;
        case T8::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T8&>(msg)); break;
        default: static_cast<TProcessor*>(this)->on_receive_unknown(source, msg); break;
      }
    }

    bool accepts(etl::message_id_t id) const
    {
      switch (id)
      {
        case T1::ID: case T2::ID: case T3::ID: case T4::ID: case T5::ID: case T6::ID: case T7::ID: case T8::ID:

          return true; break;
        default:
          return false; break;
      }
    }
  };

  //***************************************************************************
  // Specialisation for 7 message types.
  //***************************************************************************
  template <typename TProcessor,
            typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7>
  class message_router<TProcessor, T1, T2, T3, T4, T5, T6, T7, void, void, void, void, void, void, void, void, void>
   : public imessage_router
  {
  public:

    void receive(const etl::imessage& msg)
    {
      receive(etl::null_message_router::get(), msg);
    }

    void receive(etl::imessage_router& source, const etl::imessage& msg)
    {
      const size_t id = msg.get_message_id();

      switch (id)
      {
        case T1::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T1&>(msg)); break;
        case T2::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T2&>(msg)); break;
        case T3::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T3&>(msg)); break;
        case T4::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T4&>(msg)); break;
        case T5::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T5&>(msg)); break;
        case T6::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T6&>(msg)); break;
        case T7::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T7&>(msg)); break;
        default: static_cast<TProcessor*>(this)->on_receive_unknown(source, msg); break;
      }
    }

    bool accepts(etl::message_id_t id) const
    {
      switch (id)
      {
        case T1::ID: case T2::ID: case T3::ID: case T4::ID: case T5::ID: case T6::ID: case T7::ID:
          return true; break;
        default:
          return false; break;
      }
    }
  };

  //***************************************************************************
  // Specialisation for 6 message types.
  //***************************************************************************
  template <typename TProcessor,
            typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6>
  class message_router<TProcessor, T1, T2, T3, T4, T5, T6, void, void, void, void, void, void, void, void, void, void>
   : public imessage_router
  {
  public:

    void receive(const etl::imessage& msg)
    {
      receive(etl::null_message_router::get(), msg);
    }

    void receive(etl::imessage_router& source, const etl::imessage& msg)
    {
      const size_t id = msg.get_message_id();

      switch (id)
      {
        case T1::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T1&>(msg)); break;
        case T2::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T2&>(msg)); break;
        case T3::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T3&>(msg)); break;
        case T4::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T4&>(msg)); break;
        case T5::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T5&>(msg)); break;
        case T6::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T6&>(msg)); break;
        default: static_cast<TProcessor*>(this)->on_receive_unknown(source, msg); break;
      }
    }

    bool accepts(etl::message_id_t id) const
    {
      switch (id)
      {
        case T1::ID: case T2::ID: case T3::ID: case T4::ID: case T5::ID: case T6::ID:
          return true; break;
        default:
          return false; break;
      }
    }
  };

  //***************************************************************************
  // Specialisation for 5 message types.
  //***************************************************************************
  template <typename TProcessor,
            typename T1, typename T2, typename T3, typename T4,
            typename T5>
  class message_router<TProcessor, T1, T2, T3, T4, T5, void, void, void, void, void, void, void, void, void, void, void>
   : public imessage_router
  {
  public:

    void receive(const etl::imessage& msg)
    {
      receive(etl::null_message_router::get(), msg);
    }

    void receive(etl::imessage_router& source, const etl::imessage& msg)
    {
      const size_t id = msg.get_message_id();

      switch (id)
      {
        case T1::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T1&>(msg)); break;
        case T2::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T2&>(msg)); break;
        case T3::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T3&>(msg)); break;
        case T4::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T4&>(msg)); break;
        case T5::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T5&>(msg)); break;
        default: static_cast<TProcessor*>(this)->on_receive_unknown(source, msg); break;
      }
    }

    bool accepts(etl::message_id_t id) const
    {
      switch (id)
      {
        case T1::ID: case T2::ID: case T3::ID: case T4::ID: case T5::ID:
          return true; break;
        default:
          return false; break;
      }
    }
  };

  //***************************************************************************
  // Specialisation for 4 message types.
  //***************************************************************************
  template <typename TProcessor,
            typename T1, typename T2, typename T3, typename T4>
  class message_router<TProcessor, T1, T2, T3, T4, void, void, void, void, void, void, void, void, void, void, void, void>
   : public imessage_router
  {
  public:

    void receive(const etl::imessage& msg)
    {
      receive(etl::null_message_router::get(), msg);
    }

    void receive(etl::imessage_router& source, const etl::imessage& msg)
    {
      const size_t id = msg.get_message_id();

      switch (id)
      {
        case T1::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T1&>(msg)); break;
        case T2::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T2&>(msg)); break;
        case T3::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T3&>(msg)); break;
        case T4::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T4&>(msg)); break;
        default: static_cast<TProcessor*>(this)->on_receive_unknown(source, msg); break;
      }
    }

    bool accepts(etl::message_id_t id) const
    {
      switch (id)
      {
        case T1::ID: case T2::ID: case T3::ID: case T4::ID:
          return true; break;
        default:
          return false; break;
      }
    }
  };

  //***************************************************************************
  // Specialisation for 3 message types.
  //***************************************************************************
  template <typename TProcessor,
            typename T1, typename T2, typename T3>
  class message_router<TProcessor, T1, T2, T3, void, void, void, void, void, void, void, void, void, void, void, void, void>
   : public imessage_router
  {
  public:

    void receive(const etl::imessage& msg)
    {
      receive(etl::null_message_router::get(), msg);
    }

    void receive(etl::imessage_router& source, const etl::imessage& msg)
    {
      const size_t id = msg.get_message_id();

      switch (id)
      {
        case T1::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T1&>(msg)); break;
        case T2::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T2&>(msg)); break;
        case T3::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T3&>(msg)); break;
        default: static_cast<TProcessor*>(this)->on_receive_unknown(source, msg); break;
      }
    }

    bool accepts(etl::message_id_t id) const
    {
      switch (id)
      {
        case T1::ID: case T2::ID: case T3::ID:
          return true; break;
        default:
          return false; break;
      }
    }

    class message_packet
    {
    public:

      explicit message_packet(const imessage& msg)
      {
        const etl::message_id_t id = msg.get_message_id();

        switch (id)
        {
          case T1::ID: new (&storage) T1(static_cast<const T1&>(msg)); break;
          case T2::ID: new (&storage) T2(static_cast<const T2&>(msg)); break;
          case T3::ID: new (&storage) T3(static_cast<const T3&>(msg)); break;
          default: break;
        }
      }

      ~message_packet()
      {
        static_cast<const etl::imessage*>(storage).~imessage();
      }

      message_packet(const message_packet& other)
      {
        const etl::message_id_t id = other.get_imessage().get_message_id();

        switch (id)
        {
          case T1::ID: new (&storage) T1(static_cast<const T1&>(msg)); break;
          case T2::ID: new (&storage) T2(static_cast<const T2&>(msg)); break;
          case T3::ID: new (&storage) T3(static_cast<const T3&>(msg)); break;
          default: break;
        }
      }

      message_packet& operator = (const message_packet& other)
      {
        if (this != &other)
        {
          static_cast<const etl::imessage*>(storage).~imessage();
          new (this) message_packet(other);
        }
      }

      const etl::imessage& get_imessage() const
      {
        return *reiniterpret_cast<const imessage*>(storage);
      }

      operator const imessage&() const
      {
        return get_imessage();
      }

    private:

      etl::aligned_storage<etl::largest<T1, T2, T3>::size, etl::largest_alignment<T1, T2, T3>::value>::type storage;
    };
  };

  //***************************************************************************
  // Specialisation for 2 message types.
  //***************************************************************************
  template <typename TProcessor,
            typename T1, typename T2>
  class message_router<TProcessor, T1, T2, void, void, void, void, void, void, void, void, void, void, void, void, void, void>
   : public imessage_router
  {
  public:

    void receive(const etl::imessage& msg)
    {
      receive(etl::null_message_router::get(), msg);
    }

    void receive(etl::imessage_router& source, const etl::imessage& msg)
    {
      const size_t id = msg.get_message_id();

      switch (id)
      {
        case T1::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T1&>(msg)); break;
        case T2::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T2&>(msg)); break;
        default: static_cast<TProcessor*>(this)->on_receive_unknown(source, msg); break;
      }
    }

    bool accepts(etl::message_id_t id) const
    {
      switch (id)
      {
        case T1::ID: case T2::ID:
          return true; break;
        default:
          return false; break;
      }
    }
  };

  //***************************************************************************
  // Specialisation for 1 message type.
  //***************************************************************************
  template <typename TProcessor,
            typename T1>
  class message_router<TProcessor, T1, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void>
   : public imessage_router
  {
  public:

    void receive(const etl::imessage& msg)
    {
      receive(etl::null_message_router::get(), msg);
    }

    void receive(etl::imessage_router& source, const etl::imessage& msg)
    {
      const size_t id = msg.get_message_id();

      switch (id)
      {
        case T1::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T1&>(msg)); break;
        default: static_cast<TProcessor*>(this)->on_receive_unknown(source, msg); break;
      }
    }

    bool accepts(etl::message_id_t id) const
    {
      switch (id)
      {
        case T1::ID:
          return true; break;
        default:
          return false; break;
      }
    }
  };
}

#endif
