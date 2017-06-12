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

    null_message_router()
      : value(TReturn())
    {
    }

    null_message_router(const TReturn& value)
      : value(value)
    {
    }

    null_message_router(const null_message_router& other)
      : value(other.value)
    {
    }

    null_message_router& operator ==(const null_message_router& other)
      : value(other.value)
    {
    }

    TReturn receive(imessage_router<TReturn>& source, const imessage& message)
    {
      return value;
    }

  private:

    TReturn value;
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
      cog.outl("// The definition for all %s message types, returning TReturn." % Handlers)
      cog.outl("//***************************************************************************")
      cog.outl("template <typename TProcessor, typename TReturn,")
      cog.out("          ")
      for n in range(1, int(Handlers)):
          cog.out("typename T%s = void, " % n)
          if n % 4 == 0:
              cog.outl("")
              cog.out("          ")
      cog.outl("typename T%s = void>" % Handlers)
      cog.outl("class message_router")
      cog.outl("  : public imessage_router<TReturn>")
      cog.outl("{")
      cog.outl("public:")
      cog.outl("")
      cog.outl("  TReturn receive(imessage_router<TReturn>& source, const imessage& msg)")
      cog.outl("  {")
      cog.outl("    const id_t id = msg.get_message_id();")
      cog.outl("")
      cog.outl("    switch (id)")
      cog.outl("    {")
      for n in range(1, int(Handlers) + 1):
          cog.out("      case T%d::ID:" % n)
          cog.out(" return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T%d&>(msg));" % n)
          cog.outl(" break;")
      cog.out("      default:")
      cog.out(" return static_cast<TProcessor*>(this)->on_receive_unknown(source, msg);")
      cog.outl(" break;")
      cog.outl("    }")
      cog.outl("  }")
      cog.outl("};")
      cog.outl("")

      ################################################
      # The first definition for all of the messages, with void return type.
      ################################################
      cog.outl("//***************************************************************************")
      cog.outl("// The definition for all %s message types, returning void." % Handlers)
      cog.outl("//***************************************************************************")
      cog.outl("template <typename TProcessor,")
      cog.out("          ")
      for n in range(1, int(Handlers)):
          cog.out("typename T%s, " % n)
          if n % 4 == 0:
              cog.outl("")
              cog.out("          ")
      cog.outl("typename T%s>" % Handlers)
      cog.out("class message_router<TProcessor, void, ")
      for t in range(1, int(Handlers)):
          cog.out("T%d, " % t)
          if t % 16 == 0:
              cog.outl("")
              cog.out("               ")
      cog.outl("T%s>" % Handlers)
      cog.outl("  : public imessage_router<void>")
      cog.outl("{")
      cog.outl("public:")
      cog.outl("")
      cog.outl("  void receive(imessage_router<void>& source, const imessage& msg)")
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
              cog.outl("// Specialisation for %d message type, returning TReturn." % n)
          else:
              cog.outl("// Specialisation for %d message types, returning TReturn." % n)
          cog.outl("//***************************************************************************")
          cog.outl("template <typename TProcessor, typename TReturn, ")
          cog.out("          ")
          for t in range(1, n):
              cog.out("typename T%d, " % t)
              if t % 4 == 0:
                  cog.outl("")
                  cog.out("          ")
          cog.outl("typename T%d>" % n)
          cog.out("class message_router<TProcessor, TReturn, ")
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
          cog.outl(" : public imessage_router<TReturn>")
          cog.outl("{")
          cog.outl("public:")
          cog.outl("")
          cog.outl("  return_type receive(imessage_router<TReturn>& source, const imessage& msg)")
          cog.outl("  {")
          cog.outl("    const size_t id = msg.get_message_id();")
          cog.outl("")
          cog.outl("    switch (id)")
          cog.outl("    {")
          for t in range(1, n + 1):
              cog.out("      case T%d::ID:" % t)
              cog.out(" return static_cast<TProcessor*>(this)->on_receive(source, static_cast<const T%d&>(msg));" % t)
              cog.outl(" break;")
          cog.out("      default:")
          cog.out(" return static_cast<TProcessor*>(this)->on_receive_unknown(source, msg);")
          cog.outl(" break;")
          cog.outl("    }")
          cog.outl("  }")
          cog.outl("};")

          cog.outl("")
          cog.outl("//***************************************************************************")
          if n == 1:
              cog.outl("// Specialisation for %d message type, returning void." % n)
          else:
              cog.outl("// Specialisation for %d message types, returning void." % n)
          cog.outl("//***************************************************************************")
          cog.outl("template <typename TProcessor, ")
          cog.out("          ")
          for t in range(1, n):
              cog.out("typename T%d, " % t)
              if t % 4 == 0:
                  cog.outl("")
                  cog.out("          ")
          cog.outl("typename T%d>" % n)
          cog.out("class message_router<TProcessor, void, ")
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
          cog.outl(" : public imessage_router<void>")
          cog.outl("{")
          cog.outl("public:")
          cog.outl("")
          cog.outl("  void receive(imessage_router<void>& source, const imessage& msg)")
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
