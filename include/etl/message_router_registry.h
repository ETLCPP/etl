/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 John Wellbelove

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

#include "platform.h"
#include "file_error_numbers.h"
#include "message_router.h"
#include "flat_multimap.h"
#include "exception.h"
#include "error_handler.h"
#include "iterator.h"
#include "memory.h"

#include <stdint.h>

namespace etl
{
  //***************************************************************************
  /// Base exception class for message router registry.
  //***************************************************************************
  class message_router_registry_exception : public etl::exception
  {
  public:

    message_router_registry_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : etl::exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// The registry is full.
  //***************************************************************************
  class message_router_registry_full : public etl::message_router_registry_exception
  {
  public:

    message_router_registry_full(string_type file_name_, numeric_type line_number_)
      : message_router_registry_exception(ETL_ERROR_TEXT("message router registry:full", ETL_MESSAGE_ROUTER_REGISTRY_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// This is the base of all message router registries.
  //***************************************************************************
  class imessage_router_registry
  {
  private:

    typedef etl::iflat_multimap<etl::message_router_id_t, etl::imessage_router*> IRegistry;

  public:

    class const_iterator;

    //********************************************
    /// Iterator
    //********************************************
    class iterator : public etl::iterator<ETL_OR_STD::forward_iterator_tag, etl::imessage_router*>
    {
    public:

      friend class imessage_router_registry;
      friend class const_iterator;

      //********************************************
      iterator()
      {
      }

      //********************************************
      iterator(const iterator& other)
        : itr(other.itr)
      {
      }

      //********************************************
      iterator& operator =(const iterator& other)
      {
        itr = other.itr;
        return *this;
      }

      //********************************************
      etl::imessage_router& operator *()
      {
        return *(itr->second);
      }

      //********************************************
      const etl::imessage_router& operator *() const
      {
        return *(itr->second);
      }

      //********************************************
      etl::imessage_router* operator ->()
      {
        return itr->second;
      }

      //********************************************
      const etl::imessage_router* operator ->() const
      {
        return itr->second;
      }

      //********************************************
      iterator& operator ++()
      {
        ++itr;
        return *this;
      }

      //********************************************
      iterator operator ++(int)
      {
        iterator temp(*this);
        ++itr;
        return temp;
      }

      //********************************************
      friend bool operator ==(const iterator& lhs, const iterator& rhs)
      {
        return lhs.itr == rhs.itr;
      }

      //********************************************
      friend bool operator !=(const iterator& lhs, const iterator& rhs)
      {
        return !(lhs == rhs);
      }

    private:

      //********************************************
      iterator(IRegistry::iterator itr_)
        : itr(itr_)
      {
      }

      IRegistry::iterator itr;
    };

    //********************************************
    /// Const Iterator
    //********************************************
    class const_iterator : etl::iterator<ETL_OR_STD::forward_iterator_tag, const etl::imessage_router*>
    {
    public:

      friend class imessage_router_registry;

      //********************************************
      const_iterator()
      {
      }

      //********************************************
      const_iterator(const imessage_router_registry::iterator& other)
        : itr(other.itr)
      {
      }

      //********************************************
      const_iterator(const const_iterator& other)
        : itr(other.itr)
      {
      }

      //********************************************
      const_iterator& operator =(const const_iterator& other)
      {
        itr = other.itr;
        return *this;
      }

      //********************************************
      const etl::imessage_router& operator *() const
      {
        return *(itr->second);
      }

      //********************************************
      const etl::imessage_router* operator ->() const
      {
        return itr->second;
      }

      //********************************************
      const_iterator& operator ++()
      {
        ++itr;
        return *this;
      }

      //********************************************
      const_iterator operator ++(int)
      {
        const_iterator temp(*this);
        ++itr;
        return temp;
      }

      //********************************************
      friend bool operator ==(const const_iterator& lhs, const const_iterator& rhs)
      {
        return lhs.itr == rhs.itr;
      }

      //********************************************
      friend bool operator !=(const const_iterator& lhs, const const_iterator& rhs)
      {
        return !(lhs == rhs);
      }

    private:

      //********************************************
      const_iterator(IRegistry::const_iterator itr_)
        : itr(itr_)
      {
      }

      IRegistry::const_iterator itr;
    };

    //********************************************
    /// Get the beginning of the registry.
    //********************************************
    iterator begin()
    {
      return iterator(registry.begin());
    }

    const_iterator begin() const
    {
      return const_iterator(registry.cbegin());
    }

    const_iterator cbegin() const
    {
      return const_iterator(registry.cbegin());
    }

    //********************************************
    /// Get the end of the registry.
    //********************************************
    iterator end()
    {
      return iterator(registry.end());
    }

    const_iterator end() const
    {
      return const_iterator(registry.cend());
    }

    const_iterator cend() const
    {
      return const_iterator(registry.cend());
    }

    //********************************************
    /// Get the first router in the registry with the specified ID.
    //********************************************
    etl::imessage_router* find(etl::message_router_id_t id)
    {
      IRegistry::iterator itr = registry.find(id);

      if (registry.find(id) != registry.end())
      {
        return itr->second;
      }
      else
      {
        return ETL_NULLPTR;
      }
    }

    const etl::imessage_router* find(etl::message_router_id_t id) const
    {
      IRegistry::const_iterator itr = registry.find(id);

      if (registry.find(id) != registry.end())
      {
        return itr->second;
      }
      else
      {
        return ETL_NULLPTR;
      }
    }

    //********************************************
    /// Get the lower bound in the registry with the specified ID.
    //********************************************
    iterator lower_bound(etl::message_router_id_t id)
    {
      return iterator(registry.lower_bound(id));
    }

    const_iterator lower_bound(etl::message_router_id_t id) const
    {
      return const_iterator(IRegistry::const_iterator(registry.lower_bound(id)));
    }

    //********************************************
    /// Get the upper bound in the registry with the specified ID.
    //********************************************
    iterator upper_bound(etl::message_router_id_t id)
    {
      return iterator(registry.upper_bound(id));
    }

    const_iterator upper_bound(etl::message_router_id_t id) const
    {
      return const_iterator(IRegistry::const_iterator(registry.upper_bound(id)));
    }

    //********************************************
    /// Registers a router, if not already registered.
    /// If the registry is full then an ETL assert is called.
    //********************************************
    void add(etl::imessage_router& router)
    {
      if (!registry.full() && !contains(router))
      {
        IRegistry::value_type element(router.get_message_router_id(), &router);

        registry.insert(element);
      }
      else
      {
        ETL_ASSERT_FAIL(ETL_ERROR(etl::message_router_registry_full));
      }
    }

    //********************************************
    /// Registers a router, if not already registered.
    /// If the registry is full then an ETL assert is called.
    //********************************************
    void add(etl::imessage_router* p_router)
    {
      if (p_router != ETL_NULLPTR)
      {
        add(*p_router);
      }
    }

    //********************************************
    /// Registers a list of routers, if not already registered.
    /// If the registry is full then an ETL assert is called.
    //********************************************
    template <typename TIterator>
    void add(TIterator first, const TIterator& last)
    {
      while (first != last)
      {
        add(*first);
        ++first;
      }
    }

    //********************************************
    /// Unregisters a router.
    //********************************************
    void remove(etl::message_router_id_t id)
    {
      registry.erase(id);
    }

    //********************************************
    /// Returns <b>true</b> if the registry contains a router that has the specified ID.
    /// Returns <b>false</b> if not found.
    //********************************************
    bool contains(const etl::message_router_id_t id) const
    {
      return find(id) != ETL_NULLPTR;


      //return registry.find(id) != registry.end();
    }

    //********************************************
    /// Returns <b>true</b> if the registry contains the router.
    /// Returns <b>false</b> if not found.
    //********************************************
    bool contains(const etl::imessage_router* const p_router) const
    {
      if (p_router == ETL_NULLPTR)
      {
        return false;
      }

      IRegistry::const_iterator irouter = registry.find(p_router->get_message_router_id());

      return  (irouter != registry.cend()) && (irouter->second == p_router);
    }

    //********************************************
    /// Returns <b>true</b> if the registry contains the router.
    /// Returns <b>false</b> if not found.
    //********************************************
    bool contains(const etl::imessage_router& router) const
    {
      IRegistry::const_iterator irouter = registry.find(router.get_message_router_id());

      return  (irouter != registry.cend()) && (irouter->second == &router);
    }

    //********************************************
    /// Returns the number of routers with the specified ID.
    //********************************************
    size_t count(const etl::message_router_id_t id) const
    {
      return registry.count(id);
    }

    //********************************************
    /// Returns <b>true</b> if the registry is empty, otherwise <b>false</b>.
    //********************************************
    bool empty() const
    {
      return registry.empty();
    }

    //********************************************
    /// Returns <b>true</b> if the registry is full, otherwise <b>false</b>.
    //********************************************
    bool full() const
    {
      return registry.full();
    }

    //********************************************
    /// Returns the size of the registry.
    //********************************************
    size_t size() const
    {
      return registry.size();
    }

    //********************************************
    /// Returns the available size of the registry.
    //********************************************
    size_t available() const
    {
      return registry.available();
    }

    //********************************************
    /// Returns the maximum size of the registry.
    //********************************************
    size_t max_size() const
    {
      return registry.max_size();
    }

  protected:

    //********************************************
    // Constructor.
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
    // Default constructor.
    //********************************************
    message_router_registry()
      : imessage_router_registry(registry)
    {
    }

    //********************************************
    /// Constructor.
    /// Constructs from an iterator range.
    //********************************************
    template <typename TIterator>
    message_router_registry(TIterator first, const TIterator& last)
       : imessage_router_registry(registry)
    {
      while (first != last)
      {
        this->add(*first);
        ++first;
      }
    }

#if ETL_HAS_INITIALIZER_LIST
    //********************************************
    // Initializer_list constructor.
    //********************************************
    message_router_registry(std::initializer_list<etl::imessage_router*> init)
      : imessage_router_registry(registry)
    {
      std::initializer_list<etl::imessage_router*>::const_iterator itr = init.begin();

      while (itr != init.end())
      {
        this->add(*itr);
        ++itr;
      }
    }
#endif

    //********************************************
    // Copy constructor.
    //********************************************
    message_router_registry(const message_router_registry& rhs)
      : imessage_router_registry(registry)
    {
      registry = rhs.registry;
    }

    //********************************************
    // Assignment operator.
    //********************************************
    message_router_registry& operator =(const message_router_registry& rhs)
    {
      registry = rhs.registry;

      return *this;
    }

  private:

    typedef etl::flat_multimap<etl::message_router_id_t, etl::imessage_router*, MaxRouters> Registry;
    Registry registry;
  };
}

#endif
