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

  /// Allow alternative type for message id.
#if !defined(ETL_MESSAGE_ID_TYPE)
    typedef uint_least8_t id_t;
#else
    typedef ETL_MESSAGE_ID_TYPE id_t;
#endif

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
      return id_t(ID);
    }
  };

  //***************************************************************************
  class imessage_router
  {
  public:
    virtual ~imessage_router() {}
    virtual void receive(const imessage& message) = 0;
    virtual void receive(imessage_router& source, const imessage& message) = 0;

    void send_message(imessage_router& destination,
                      const imessage&  message)
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

    void receive(const imessage& message)
    {
    }

    void receive(imessage_router& source, const imessage& message)
    {
    }
  };

  //***************************************************************************
  /// Send a message to a router.
  /// Sets the 'sender' to etl::null_message_router type.
  //***************************************************************************
  inline static void send_message(imessage_router& destination, 
                                  const imessage&  message)
  {
    destination.receive(message);
  }

  //***************************************************************************
  /// Send a message to a router.
  //***************************************************************************
  inline static void send_message(imessage_router& source, 
                                  imessage_router& destination, 
                                  const imessage&  message)
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

  /*[[[cog
      import cog
      cog.outl("//***************************************************************************")
      cog.outl("// The code below has been auto generated. Do not manually edit.")
      cog.outl("//***************************************************************************")
      cog.outl("")
      ################################################
      # The first definition for all of the messages.
      ################################################
      cog.outl("//***************************************************************************")
      cog.outl("// The definition for all %s message types." % Handlers)
      cog.outl("//***************************************************************************")
      cog.outl("template <typename TProcessor,")
      cog.out("          ")
      cog.out("typename T1, ")
      for n in range(2, int(Handlers)):
          cog.out("typename T%s = void, " % n)
          if n % 4 == 0:
              cog.outl("")
              cog.out("          ")
      cog.outl("typename T%s = void>" % Handlers)
      cog.out("class message_router")
      cog.outl("  : public imessage_router")
      cog.outl("{")
      cog.outl("public:")
      cog.outl("")
      cog.outl("  void receive(const imessage& msg)")
      cog.outl("  {")
      cog.outl("    receive(etl::null_message_router(), msg);")
      cog.outl("  }")
      cog.outl("")
      cog.outl("  void receive(imessage_router& source, const imessage& msg)")
      cog.outl("  {")
      cog.outl("    const id_t id = msg.get_message_id();")
      cog.outl("")
      cog.outl("    switch (id)")
      cog.outl("    {")
      for n in range(1, int(Handlers) + 1):
          cog.out("      case T%d::ID:" % n)
          cog.out(" static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T%d&>(msg));" % n)
          cog.outl(" break;")
      cog.out("      default:")
      cog.out("  static_cast<TProcessor*>(this)->on_receive_unknown(source, msg);")
      cog.outl(" break;")
      cog.outl("    }")
      cog.outl("  }")
      cog.outl("};")

      ####################################
      # All of the other specialisations.
      ####################################
      for n in range(int(Handlers) - 1, 0, -1):
          cog.outl("")
          cog.outl("//***************************************************************************")
          if n == 1:
              cog.outl("// Specialisation for %d message type." % n)
          else:
              cog.outl("// Specialisation for %d message types." % n)
          cog.outl("//***************************************************************************")
          cog.outl("template <typename TProcessor, ")
          cog.out("          ")
          for t in range(1, n):
              cog.out("typename T%d, " % t)
              if t % 4 == 0:
                  cog.outl("")
                  cog.out("          ")
          cog.outl("typename T%d>" % n)
          cog.out("class message_router<TProcessor, ")
          for t in range(1, n + 1):
              cog.out("T%d, " % t)
              if t % 16 == 0:
                  cog.outl("")
                  cog.out("               ")
          for t in range(n + 1, int(Handlers)):
              cog.out("void, ")
              if t % 16 == 0:
                  cog.outl("")
                  cog.out("               ")
          cog.outl("void>")
          cog.outl(" : public imessage_router")
          cog.outl("{")
          cog.outl("public:")
          cog.outl("")
          cog.outl("  void receive(const imessage& msg)")
          cog.outl("  {")
          cog.outl("    receive(etl::null_message_router(), msg);")
          cog.outl("  }")
          cog.outl("")
          cog.outl("  void receive(imessage_router& source, const imessage& msg)")
          cog.outl("  {")
          cog.outl("    const size_t id = msg.get_message_id();")
          cog.outl("")
          cog.outl("    switch (id)")
          cog.outl("    {")
          for t in range(1, n + 1):
              cog.out("      case T%d::ID:" % t)
              cog.out(" static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T%d&>(msg));" % t)
              cog.outl(" break;")
          cog.out("      default:")
          cog.out(" static_cast<TProcessor*>(this)->on_receive_unknown(source, msg);")
          cog.outl(" break;")
          cog.outl("    }")
          cog.outl("  }")
          cog.outl("};")
  ]]]*/
  /*[[[end]]]*/
}

#endif
