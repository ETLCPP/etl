/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 jwellbelove

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

#ifndef ETL_MESSAGE_ROUTER_REGISTRY_INCLUDED
#define ETL_MESSAGE_ROUTER_REGISTRY_INCLUDED

#include <stdint.h>

#include "platform.h"
#include "file_error_numbers.h"
#include "message_router.h"
#include "flat_map.h"
#include "exception.h"
#include "error_handler.h"

#undef ETL_FILE
#define ETL_FILE ETL_MESSAGE_ROUTER_REGISTRY

namespace etl
{
  ////***************************************************************************
  ///// Base exception class for message router
  ////***************************************************************************
  //class message_router_registry_exception : public etl::exception
  //{
  //public:

  //  message_router_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
  //    : etl::exception(reason_, file_name_, line_number_)
  //  {
  //  }
  //};

  ////***************************************************************************
  ///// Router id is out of the legal range.
  ////***************************************************************************
  //class message_router_illegal_id : public etl::message_router_registry_exception
  //{
  //public:

  //  message_router_illegal_id(string_type file_name_, numeric_type line_number_)
  //    : message_router_exception(ETL_ERROR_TEXT("message router:illegal id", ETL_FILE"A"), file_name_, line_number_)
  //  {
  //  }
  //};

  //***************************************************************************
  /// This is the base of all message router registries.
  //***************************************************************************
  class imessage_router_registry
  {
  public:

    //********************************************
    etl::imessage_router* get_message_router(etl::message_router_id_t id) const
    {
      IRegistry::const_iterator itr = registry.find(id);

      if (itr != registry.end())
      {
        return itr->second;
      }
      else
      {
        return ETL_NULLPTR;
      }
    }

    //********************************************
    bool contains(etl::message_router_id_t id) const
    {
      return registry.find(id) != registry.end();
    }

    //********************************************
    bool register_message_router(etl::imessage_router& router)
    {
      if (!registry.full())
      {
        typename IRegistry::value_type element(router.get_message_router_id(), &router);

        registry.insert(element);
        return true;
      }
      else
      {
        return false;
      }
    }

    //********************************************
    void unregister_message_router(etl::message_router_id_t id)
    {
      registry.erase(id);
    }

  protected:

    typedef etl::iflat_map<etl::message_router_id_t, etl::imessage_router*> IRegistry;

    //********************************************
    imessage_router_registry(IRegistry& registry_)
      : registry(registry_)
    {
    }

  private:  

    IRegistry& registry;
  };

  //***************************************************************************
  /// Message router registry.
  //***************************************************************************
  template <size_t MaxRouters>
  class message_router_registry : public etl::imessage_router_registry
  {
  public:

    //********************************************
    message_router_registry()
      : message_router_registry(registry)
    {
    }

  private:

    typedef etl::flat_map<etl::message_router_id_t, etl::imessage_router*, MaxRouters> Registry;
    Registry registry;
  };
}

#undef ETL_FILE

#endif
