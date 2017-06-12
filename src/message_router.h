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
  //***************************************************************************
  class imessage
  {
  public:

    typedef size_t id_t;

    virtual ~imessage() {}
    virtual id_t get_message_id() const = 0;
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

    id_t get_message_id() const
    {
      return ID;
    }
  };

  //***************************************************************************
  template <typename TReturn>
  class imessage_router
  {
  public:

    typedef TReturn return_type;

    virtual ~imessage_router() {}
    virtual TReturn receive(imessage_router& source, const imessage& message) = 0;

    TReturn send_to(imessage_router<TReturn>& destination, const imessage& message)
    {
      return destination.receive(*this, message);
    }
  };

  //***************************************************************************
  template <>
  class imessage_router<void>
  {
  public:

    typedef void return_type;

    virtual ~imessage_router() {}
    virtual void receive(imessage_router<void>& source, const imessage& message) = 0;

    void send_to(imessage_router<void>& destination, const imessage& message)
    {
      destination.receive(*this, message);
    }
  };

  //***************************************************************************
  template <typename TReturn>
  class null_message_router : public imessage_router<TReturn>
  {
  public:

    TReturn receive(imessage_router<TReturn>& source, const imessage& message)
    {
      return TReturn();
    }
  };

  //***************************************************************************
  template <>
  class null_message_router<void> : public imessage_router<void>
  {
  public:

    void receive(imessage_router<void>& source, const imessage& message)
    {
    }
  };

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
  // The definition for all 16 message types, returning TReturn.
  //***************************************************************************
  template <typename TProcessor, typename TReturn,
            typename T1 = void, typename T2 = void, typename T3 = void, typename T4 = void, 
            typename T5 = void, typename T6 = void, typename T7 = void, typename T8 = void, 
            typename T9 = void, typename T10 = void, typename T11 = void, typename T12 = void, 
            typename T13 = void, typename T14 = void, typename T15 = void, typename T16 = void>
  class message_router
    : public imessage_router<TReturn>
  {
  public:

    TReturn receive(imessage_router<TReturn>& source, const imessage& msg)
    {
      const id_t id = msg.get_message_id();

      switch (id)
      {
        case T1::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T1&>(msg)); break;
        case T2::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T2&>(msg)); break;
        case T3::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T3&>(msg)); break;
        case T4::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T4&>(msg)); break;
        case T5::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T5&>(msg)); break;
        case T6::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T6&>(msg)); break;
        case T7::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T7&>(msg)); break;
        case T8::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T8&>(msg)); break;
        case T9::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T9&>(msg)); break;
        case T10::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T10&>(msg)); break;
        case T11::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T11&>(msg)); break;
        case T12::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T12&>(msg)); break;
        case T13::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T13&>(msg)); break;
        case T14::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T14&>(msg)); break;
        case T15::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T15&>(msg)); break;
        case T16::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T16&>(msg)); break;
        default: return static_cast<TProcessor*>(this)->on_receive_unknown(source, msg); break;
      }
    }
  };

  //***************************************************************************
  // The definition for all 16 message types, returning void.
  //***************************************************************************
  template <typename TProcessor,
            typename T1, typename T2, typename T3, typename T4, 
            typename T5, typename T6, typename T7, typename T8, 
            typename T9, typename T10, typename T11, typename T12, 
            typename T13, typename T14, typename T15, typename T16>
  class message_router<TProcessor, void, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16>
    : public imessage_router<void>
  {
  public:

    void receive(imessage_router<void>& source, const imessage& msg)
    {
      const id_t id = msg.get_message_id();

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
  };

  //***************************************************************************
  // Specialisation for 15 message types, returning TReturn.
  //***************************************************************************
  template <typename TProcessor, typename TReturn, 
            typename T1, typename T2, typename T3, typename T4, 
            typename T5, typename T6, typename T7, typename T8, 
            typename T9, typename T10, typename T11, typename T12, 
            typename T13, typename T14, typename T15>
  class message_router<TProcessor, TReturn, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, void>
   : public imessage_router<TReturn>
  {
  public:

    return_type receive(imessage_router<TReturn>& source, const imessage& msg)
    {
      const size_t id = msg.get_message_id();

      switch (id)
      {
        case T1::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T1&>(msg)); break;
        case T2::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T2&>(msg)); break;
        case T3::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T3&>(msg)); break;
        case T4::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T4&>(msg)); break;
        case T5::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T5&>(msg)); break;
        case T6::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T6&>(msg)); break;
        case T7::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T7&>(msg)); break;
        case T8::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T8&>(msg)); break;
        case T9::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T9&>(msg)); break;
        case T10::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T10&>(msg)); break;
        case T11::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T11&>(msg)); break;
        case T12::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T12&>(msg)); break;
        case T13::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T13&>(msg)); break;
        case T14::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T14&>(msg)); break;
        case T15::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T15&>(msg)); break;
        default: return static_cast<TProcessor*>(this)->on_receive_unknown(source, msg); break;
      }
    }
  };

  //***************************************************************************
  // Specialisation for 15 message types, returning void.
  //***************************************************************************
  template <typename TProcessor, 
            typename T1, typename T2, typename T3, typename T4, 
            typename T5, typename T6, typename T7, typename T8, 
            typename T9, typename T10, typename T11, typename T12, 
            typename T13, typename T14, typename T15>
  class message_router<TProcessor, void, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, void>
   : public imessage_router<void>
  {
  public:

    void receive(imessage_router<void>& source, const imessage& msg)
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
  };

  //***************************************************************************
  // Specialisation for 14 message types, returning TReturn.
  //***************************************************************************
  template <typename TProcessor, typename TReturn, 
            typename T1, typename T2, typename T3, typename T4, 
            typename T5, typename T6, typename T7, typename T8, 
            typename T9, typename T10, typename T11, typename T12, 
            typename T13, typename T14>
  class message_router<TProcessor, TReturn, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, void, void>
   : public imessage_router<TReturn>
  {
  public:

    return_type receive(imessage_router<TReturn>& source, const imessage& msg)
    {
      const size_t id = msg.get_message_id();

      switch (id)
      {
        case T1::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T1&>(msg)); break;
        case T2::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T2&>(msg)); break;
        case T3::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T3&>(msg)); break;
        case T4::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T4&>(msg)); break;
        case T5::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T5&>(msg)); break;
        case T6::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T6&>(msg)); break;
        case T7::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T7&>(msg)); break;
        case T8::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T8&>(msg)); break;
        case T9::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T9&>(msg)); break;
        case T10::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T10&>(msg)); break;
        case T11::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T11&>(msg)); break;
        case T12::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T12&>(msg)); break;
        case T13::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T13&>(msg)); break;
        case T14::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T14&>(msg)); break;
        default: return static_cast<TProcessor*>(this)->on_receive_unknown(source, msg); break;
      }
    }
  };

  //***************************************************************************
  // Specialisation for 14 message types, returning void.
  //***************************************************************************
  template <typename TProcessor, 
            typename T1, typename T2, typename T3, typename T4, 
            typename T5, typename T6, typename T7, typename T8, 
            typename T9, typename T10, typename T11, typename T12, 
            typename T13, typename T14>
  class message_router<TProcessor, void, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, void, void>
   : public imessage_router<void>
  {
  public:

    void receive(imessage_router<void>& source, const imessage& msg)
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
  };

  //***************************************************************************
  // Specialisation for 13 message types, returning TReturn.
  //***************************************************************************
  template <typename TProcessor, typename TReturn, 
            typename T1, typename T2, typename T3, typename T4, 
            typename T5, typename T6, typename T7, typename T8, 
            typename T9, typename T10, typename T11, typename T12, 
            typename T13>
  class message_router<TProcessor, TReturn, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, void, void, void>
   : public imessage_router<TReturn>
  {
  public:

    return_type receive(imessage_router<TReturn>& source, const imessage& msg)
    {
      const size_t id = msg.get_message_id();

      switch (id)
      {
        case T1::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T1&>(msg)); break;
        case T2::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T2&>(msg)); break;
        case T3::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T3&>(msg)); break;
        case T4::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T4&>(msg)); break;
        case T5::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T5&>(msg)); break;
        case T6::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T6&>(msg)); break;
        case T7::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T7&>(msg)); break;
        case T8::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T8&>(msg)); break;
        case T9::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T9&>(msg)); break;
        case T10::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T10&>(msg)); break;
        case T11::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T11&>(msg)); break;
        case T12::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T12&>(msg)); break;
        case T13::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T13&>(msg)); break;
        default: return static_cast<TProcessor*>(this)->on_receive_unknown(source, msg); break;
      }
    }
  };

  //***************************************************************************
  // Specialisation for 13 message types, returning void.
  //***************************************************************************
  template <typename TProcessor, 
            typename T1, typename T2, typename T3, typename T4, 
            typename T5, typename T6, typename T7, typename T8, 
            typename T9, typename T10, typename T11, typename T12, 
            typename T13>
  class message_router<TProcessor, void, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, void, void, void>
   : public imessage_router<void>
  {
  public:

    void receive(imessage_router<void>& source, const imessage& msg)
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
  };

  //***************************************************************************
  // Specialisation for 12 message types, returning TReturn.
  //***************************************************************************
  template <typename TProcessor, typename TReturn, 
            typename T1, typename T2, typename T3, typename T4, 
            typename T5, typename T6, typename T7, typename T8, 
            typename T9, typename T10, typename T11, typename T12>
  class message_router<TProcessor, TReturn, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, void, void, void, void>
   : public imessage_router<TReturn>
  {
  public:

    return_type receive(imessage_router<TReturn>& source, const imessage& msg)
    {
      const size_t id = msg.get_message_id();

      switch (id)
      {
        case T1::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T1&>(msg)); break;
        case T2::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T2&>(msg)); break;
        case T3::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T3&>(msg)); break;
        case T4::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T4&>(msg)); break;
        case T5::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T5&>(msg)); break;
        case T6::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T6&>(msg)); break;
        case T7::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T7&>(msg)); break;
        case T8::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T8&>(msg)); break;
        case T9::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T9&>(msg)); break;
        case T10::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T10&>(msg)); break;
        case T11::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T11&>(msg)); break;
        case T12::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T12&>(msg)); break;
        default: return static_cast<TProcessor*>(this)->on_receive_unknown(source, msg); break;
      }
    }
  };

  //***************************************************************************
  // Specialisation for 12 message types, returning void.
  //***************************************************************************
  template <typename TProcessor, 
            typename T1, typename T2, typename T3, typename T4, 
            typename T5, typename T6, typename T7, typename T8, 
            typename T9, typename T10, typename T11, typename T12>
  class message_router<TProcessor, void, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, void, void, void, void>
   : public imessage_router<void>
  {
  public:

    void receive(imessage_router<void>& source, const imessage& msg)
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
  };

  //***************************************************************************
  // Specialisation for 11 message types, returning TReturn.
  //***************************************************************************
  template <typename TProcessor, typename TReturn, 
            typename T1, typename T2, typename T3, typename T4, 
            typename T5, typename T6, typename T7, typename T8, 
            typename T9, typename T10, typename T11>
  class message_router<TProcessor, TReturn, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, void, void, void, void, void>
   : public imessage_router<TReturn>
  {
  public:

    return_type receive(imessage_router<TReturn>& source, const imessage& msg)
    {
      const size_t id = msg.get_message_id();

      switch (id)
      {
        case T1::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T1&>(msg)); break;
        case T2::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T2&>(msg)); break;
        case T3::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T3&>(msg)); break;
        case T4::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T4&>(msg)); break;
        case T5::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T5&>(msg)); break;
        case T6::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T6&>(msg)); break;
        case T7::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T7&>(msg)); break;
        case T8::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T8&>(msg)); break;
        case T9::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T9&>(msg)); break;
        case T10::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T10&>(msg)); break;
        case T11::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T11&>(msg)); break;
        default: return static_cast<TProcessor*>(this)->on_receive_unknown(source, msg); break;
      }
    }
  };

  //***************************************************************************
  // Specialisation for 11 message types, returning void.
  //***************************************************************************
  template <typename TProcessor, 
            typename T1, typename T2, typename T3, typename T4, 
            typename T5, typename T6, typename T7, typename T8, 
            typename T9, typename T10, typename T11>
  class message_router<TProcessor, void, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, void, void, void, void, void>
   : public imessage_router<void>
  {
  public:

    void receive(imessage_router<void>& source, const imessage& msg)
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
  };

  //***************************************************************************
  // Specialisation for 10 message types, returning TReturn.
  //***************************************************************************
  template <typename TProcessor, typename TReturn, 
            typename T1, typename T2, typename T3, typename T4, 
            typename T5, typename T6, typename T7, typename T8, 
            typename T9, typename T10>
  class message_router<TProcessor, TReturn, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, void, void, void, void, void, void>
   : public imessage_router<TReturn>
  {
  public:

    return_type receive(imessage_router<TReturn>& source, const imessage& msg)
    {
      const size_t id = msg.get_message_id();

      switch (id)
      {
        case T1::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T1&>(msg)); break;
        case T2::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T2&>(msg)); break;
        case T3::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T3&>(msg)); break;
        case T4::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T4&>(msg)); break;
        case T5::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T5&>(msg)); break;
        case T6::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T6&>(msg)); break;
        case T7::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T7&>(msg)); break;
        case T8::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T8&>(msg)); break;
        case T9::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T9&>(msg)); break;
        case T10::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T10&>(msg)); break;
        default: return static_cast<TProcessor*>(this)->on_receive_unknown(source, msg); break;
      }
    }
  };

  //***************************************************************************
  // Specialisation for 10 message types, returning void.
  //***************************************************************************
  template <typename TProcessor, 
            typename T1, typename T2, typename T3, typename T4, 
            typename T5, typename T6, typename T7, typename T8, 
            typename T9, typename T10>
  class message_router<TProcessor, void, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, void, void, void, void, void, void>
   : public imessage_router<void>
  {
  public:

    void receive(imessage_router<void>& source, const imessage& msg)
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
  };

  //***************************************************************************
  // Specialisation for 9 message types, returning TReturn.
  //***************************************************************************
  template <typename TProcessor, typename TReturn, 
            typename T1, typename T2, typename T3, typename T4, 
            typename T5, typename T6, typename T7, typename T8, 
            typename T9>
  class message_router<TProcessor, TReturn, T1, T2, T3, T4, T5, T6, T7, T8, T9, void, void, void, void, void, void, void>
   : public imessage_router<TReturn>
  {
  public:

    return_type receive(imessage_router<TReturn>& source, const imessage& msg)
    {
      const size_t id = msg.get_message_id();

      switch (id)
      {
        case T1::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T1&>(msg)); break;
        case T2::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T2&>(msg)); break;
        case T3::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T3&>(msg)); break;
        case T4::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T4&>(msg)); break;
        case T5::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T5&>(msg)); break;
        case T6::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T6&>(msg)); break;
        case T7::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T7&>(msg)); break;
        case T8::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T8&>(msg)); break;
        case T9::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T9&>(msg)); break;
        default: return static_cast<TProcessor*>(this)->on_receive_unknown(source, msg); break;
      }
    }
  };

  //***************************************************************************
  // Specialisation for 9 message types, returning void.
  //***************************************************************************
  template <typename TProcessor, 
            typename T1, typename T2, typename T3, typename T4, 
            typename T5, typename T6, typename T7, typename T8, 
            typename T9>
  class message_router<TProcessor, void, T1, T2, T3, T4, T5, T6, T7, T8, T9, void, void, void, void, void, void, void>
   : public imessage_router<void>
  {
  public:

    void receive(imessage_router<void>& source, const imessage& msg)
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
  };

  //***************************************************************************
  // Specialisation for 8 message types, returning TReturn.
  //***************************************************************************
  template <typename TProcessor, typename TReturn, 
            typename T1, typename T2, typename T3, typename T4, 
            typename T5, typename T6, typename T7, typename T8>
  class message_router<TProcessor, TReturn, T1, T2, T3, T4, T5, T6, T7, T8, void, void, void, void, void, void, void, void>
   : public imessage_router<TReturn>
  {
  public:

    return_type receive(imessage_router<TReturn>& source, const imessage& msg)
    {
      const size_t id = msg.get_message_id();

      switch (id)
      {
        case T1::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T1&>(msg)); break;
        case T2::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T2&>(msg)); break;
        case T3::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T3&>(msg)); break;
        case T4::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T4&>(msg)); break;
        case T5::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T5&>(msg)); break;
        case T6::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T6&>(msg)); break;
        case T7::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T7&>(msg)); break;
        case T8::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T8&>(msg)); break;
        default: return static_cast<TProcessor*>(this)->on_receive_unknown(source, msg); break;
      }
    }
  };

  //***************************************************************************
  // Specialisation for 8 message types, returning void.
  //***************************************************************************
  template <typename TProcessor, 
            typename T1, typename T2, typename T3, typename T4, 
            typename T5, typename T6, typename T7, typename T8>
  class message_router<TProcessor, void, T1, T2, T3, T4, T5, T6, T7, T8, void, void, void, void, void, void, void, void>
   : public imessage_router<void>
  {
  public:

    void receive(imessage_router<void>& source, const imessage& msg)
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
  };

  //***************************************************************************
  // Specialisation for 7 message types, returning TReturn.
  //***************************************************************************
  template <typename TProcessor, typename TReturn, 
            typename T1, typename T2, typename T3, typename T4, 
            typename T5, typename T6, typename T7>
  class message_router<TProcessor, TReturn, T1, T2, T3, T4, T5, T6, T7, void, void, void, void, void, void, void, void, void>
   : public imessage_router<TReturn>
  {
  public:

    return_type receive(imessage_router<TReturn>& source, const imessage& msg)
    {
      const size_t id = msg.get_message_id();

      switch (id)
      {
        case T1::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T1&>(msg)); break;
        case T2::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T2&>(msg)); break;
        case T3::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T3&>(msg)); break;
        case T4::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T4&>(msg)); break;
        case T5::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T5&>(msg)); break;
        case T6::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T6&>(msg)); break;
        case T7::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T7&>(msg)); break;
        default: return static_cast<TProcessor*>(this)->on_receive_unknown(source, msg); break;
      }
    }
  };

  //***************************************************************************
  // Specialisation for 7 message types, returning void.
  //***************************************************************************
  template <typename TProcessor, 
            typename T1, typename T2, typename T3, typename T4, 
            typename T5, typename T6, typename T7>
  class message_router<TProcessor, void, T1, T2, T3, T4, T5, T6, T7, void, void, void, void, void, void, void, void, void>
   : public imessage_router<void>
  {
  public:

    void receive(imessage_router<void>& source, const imessage& msg)
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
  };

  //***************************************************************************
  // Specialisation for 6 message types, returning TReturn.
  //***************************************************************************
  template <typename TProcessor, typename TReturn, 
            typename T1, typename T2, typename T3, typename T4, 
            typename T5, typename T6>
  class message_router<TProcessor, TReturn, T1, T2, T3, T4, T5, T6, void, void, void, void, void, void, void, void, void, void>
   : public imessage_router<TReturn>
  {
  public:

    return_type receive(imessage_router<TReturn>& source, const imessage& msg)
    {
      const size_t id = msg.get_message_id();

      switch (id)
      {
        case T1::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T1&>(msg)); break;
        case T2::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T2&>(msg)); break;
        case T3::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T3&>(msg)); break;
        case T4::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T4&>(msg)); break;
        case T5::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T5&>(msg)); break;
        case T6::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T6&>(msg)); break;
        default: return static_cast<TProcessor*>(this)->on_receive_unknown(source, msg); break;
      }
    }
  };

  //***************************************************************************
  // Specialisation for 6 message types, returning void.
  //***************************************************************************
  template <typename TProcessor, 
            typename T1, typename T2, typename T3, typename T4, 
            typename T5, typename T6>
  class message_router<TProcessor, void, T1, T2, T3, T4, T5, T6, void, void, void, void, void, void, void, void, void, void>
   : public imessage_router<void>
  {
  public:

    void receive(imessage_router<void>& source, const imessage& msg)
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
  };

  //***************************************************************************
  // Specialisation for 5 message types, returning TReturn.
  //***************************************************************************
  template <typename TProcessor, typename TReturn, 
            typename T1, typename T2, typename T3, typename T4, 
            typename T5>
  class message_router<TProcessor, TReturn, T1, T2, T3, T4, T5, void, void, void, void, void, void, void, void, void, void, void>
   : public imessage_router<TReturn>
  {
  public:

    return_type receive(imessage_router<TReturn>& source, const imessage& msg)
    {
      const size_t id = msg.get_message_id();

      switch (id)
      {
        case T1::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T1&>(msg)); break;
        case T2::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T2&>(msg)); break;
        case T3::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T3&>(msg)); break;
        case T4::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T4&>(msg)); break;
        case T5::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T5&>(msg)); break;
        default: return static_cast<TProcessor*>(this)->on_receive_unknown(source, msg); break;
      }
    }
  };

  //***************************************************************************
  // Specialisation for 5 message types, returning void.
  //***************************************************************************
  template <typename TProcessor, 
            typename T1, typename T2, typename T3, typename T4, 
            typename T5>
  class message_router<TProcessor, void, T1, T2, T3, T4, T5, void, void, void, void, void, void, void, void, void, void, void>
   : public imessage_router<void>
  {
  public:

    void receive(imessage_router<void>& source, const imessage& msg)
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
  };

  //***************************************************************************
  // Specialisation for 4 message types, returning TReturn.
  //***************************************************************************
  template <typename TProcessor, typename TReturn, 
            typename T1, typename T2, typename T3, typename T4>
  class message_router<TProcessor, TReturn, T1, T2, T3, T4, void, void, void, void, void, void, void, void, void, void, void, void>
   : public imessage_router<TReturn>
  {
  public:

    return_type receive(imessage_router<TReturn>& source, const imessage& msg)
    {
      const size_t id = msg.get_message_id();

      switch (id)
      {
        case T1::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T1&>(msg)); break;
        case T2::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T2&>(msg)); break;
        case T3::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T3&>(msg)); break;
        case T4::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T4&>(msg)); break;
        default: return static_cast<TProcessor*>(this)->on_receive_unknown(source, msg); break;
      }
    }
  };

  //***************************************************************************
  // Specialisation for 4 message types, returning void.
  //***************************************************************************
  template <typename TProcessor, 
            typename T1, typename T2, typename T3, typename T4>
  class message_router<TProcessor, void, T1, T2, T3, T4, void, void, void, void, void, void, void, void, void, void, void, void>
   : public imessage_router<void>
  {
  public:

    void receive(imessage_router<void>& source, const imessage& msg)
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
  };

  //***************************************************************************
  // Specialisation for 3 message types, returning TReturn.
  //***************************************************************************
  template <typename TProcessor, typename TReturn, 
            typename T1, typename T2, typename T3>
  class message_router<TProcessor, TReturn, T1, T2, T3, void, void, void, void, void, void, void, void, void, void, void, void, void>
   : public imessage_router<TReturn>
  {
  public:

    return_type receive(imessage_router<TReturn>& source, const imessage& msg)
    {
      const size_t id = msg.get_message_id();

      switch (id)
      {
        case T1::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T1&>(msg)); break;
        case T2::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T2&>(msg)); break;
        case T3::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T3&>(msg)); break;
        default: return static_cast<TProcessor*>(this)->on_receive_unknown(source, msg); break;
      }
    }
  };

  //***************************************************************************
  // Specialisation for 3 message types, returning void.
  //***************************************************************************
  template <typename TProcessor, 
            typename T1, typename T2, typename T3>
  class message_router<TProcessor, void, T1, T2, T3, void, void, void, void, void, void, void, void, void, void, void, void, void>
   : public imessage_router<void>
  {
  public:

    void receive(imessage_router<void>& source, const imessage& msg)
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
  };

  //***************************************************************************
  // Specialisation for 2 message types, returning TReturn.
  //***************************************************************************
  template <typename TProcessor, typename TReturn, 
            typename T1, typename T2>
  class message_router<TProcessor, TReturn, T1, T2, void, void, void, void, void, void, void, void, void, void, void, void, void, void>
   : public imessage_router<TReturn>
  {
  public:

    return_type receive(imessage_router<TReturn>& source, const imessage& msg)
    {
      const size_t id = msg.get_message_id();

      switch (id)
      {
        case T1::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T1&>(msg)); break;
        case T2::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T2&>(msg)); break;
        default: return static_cast<TProcessor*>(this)->on_receive_unknown(source, msg); break;
      }
    }
  };

  //***************************************************************************
  // Specialisation for 2 message types, returning void.
  //***************************************************************************
  template <typename TProcessor, 
            typename T1, typename T2>
  class message_router<TProcessor, void, T1, T2, void, void, void, void, void, void, void, void, void, void, void, void, void, void>
   : public imessage_router<void>
  {
  public:

    void receive(imessage_router<void>& source, const imessage& msg)
    {
      const size_t id = msg.get_message_id();

      switch (id)
      {
        case T1::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T1&>(msg)); break;
        case T2::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T2&>(msg)); break;
        default: static_cast<TProcessor*>(this)->on_receive_unknown(source, msg); break;
      }
    }
  };

  //***************************************************************************
  // Specialisation for 1 message type, returning TReturn.
  //***************************************************************************
  template <typename TProcessor, typename TReturn, 
            typename T1>
  class message_router<TProcessor, TReturn, T1, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void>
   : public imessage_router<TReturn>
  {
  public:

    return_type receive(imessage_router<TReturn>& source, const imessage& msg)
    {
      const size_t id = msg.get_message_id();

      switch (id)
      {
        case T1::ID: return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T1&>(msg)); break;
        default: return static_cast<TProcessor*>(this)->on_receive_unknown(source, msg); break;
      }
    }
  };

  //***************************************************************************
  // Specialisation for 1 message type, returning void.
  //***************************************************************************
  template <typename TProcessor, 
            typename T1>
  class message_router<TProcessor, void, T1, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void>
   : public imessage_router<void>
  {
  public:

    void receive(imessage_router<void>& source, const imessage& msg)
    {
      const size_t id = msg.get_message_id();

      switch (id)
      {
        case T1::ID: static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T1&>(msg)); break;
        default: static_cast<TProcessor*>(this)->on_receive_unknown(source, msg); break;
      }
    }
  };
}

#endif
