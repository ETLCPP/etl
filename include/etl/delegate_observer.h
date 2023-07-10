///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2023 John Wellbelove

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

#ifndef ETL_DELEGATE_OBSERVER_INCLUDED
#define ETL_DELEGATE_OBSERVER_INCLUDED

#include "algorithm.h"

#include "platform.h"
#include "delegate.h"
#include "vector.h"
#include "exception.h"
#include "error_handler.h"
#include "utility.h"

namespace etl
{
  //***************************************************************************
  ///\ingroup observer
  /// The base class for delegate observer exceptions.
  //***************************************************************************
  class delegate_observer_exception : public exception
  {
  public:

    delegate_observer_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  ///\ingroup observer
  /// The exception thrown when the delegate observer list is full.
  //***************************************************************************
  class delegate_observer_list_full : public delegate_observer_exception
  {
  public:

    delegate_observer_list_full(string_type file_name_, numeric_type line_number_)
      : delegate_observer_exception(ETL_ERROR_TEXT("delegate_observable:full", ETL_DELEGATE_OBSERVER_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  //*********************************************************************
  /// The object that is being observed.
  ///\tparam MAX_OBSERVERS The maximum number of observers that can be accommodated.
  ///\ingroup observer
  //*********************************************************************
  template <typename TNotification, const size_t MAX_OBSERVERS>
  class delegate_observable
  {
  public:
    
    typedef etl::delegate<void(TNotification)> observer_type;

  private:

    //***********************************
    // Item stored in the observer list.
    //***********************************
    struct observer_item
    {
      observer_item(observer_type observer_)
        : observer(observer_)
        , enabled(true)
      {
      }

      observer_type observer;
      bool          enabled;
    };

    //***********************************
    // How to compare an observer with an observer list item.
    //***********************************
    struct compare_observers
    {
      compare_observers(observer_type& observer_)
        : observer(&observer_)
      {
      }

      bool operator ()(const observer_item& item) const
      {
        return observer == item.observer;
      }

      observer_type observer;
    };

    typedef etl::vector<observer_item, MAX_OBSERVERS> Observer_List;

  public:

    typedef size_t        size_type;
    typedef TNotification notification_type;

    //*****************************************************************
    /// Add an observer to the list.
    /// If asserts or exceptions are enabled then an etl::observable_observer_list_full
    /// is emitted if the observer list is already full.
    ///\param observer A reference to the observer.
    //*****************************************************************
    void add_observer(observer_type& observer)
    {
		  // See if we already have it in our list.
      typename Observer_List::iterator i_observer_item = find_observer(observer);

		  // Not there?
      if (i_observer_item == observer_list.end())
      {
        // Is there enough room?
        ETL_ASSERT_OR_RETURN(!observer_list.full(), ETL_ERROR(etl::observer_list_full));

        // Add it.
        observer_list.push_back(observer_item(observer));
      }
    }

    //*****************************************************************
    /// Remove a particular observer from the list.
    ///\param observer A reference to the observer.
    ///\return <b>true</b> if the observer was removed, <b>false</b> if not.
    //*****************************************************************
    bool remove_observer(observer_type& observer)
    {
      // See if we have it in our list.
      typename Observer_List::iterator i_observer_item = find_observer(observer);

      // Found it?
      if (i_observer_item != observer_list.end())
      {
        // Erase it.
        observer_list.erase(i_observer_item);
        return true;
      }
      else
      {
        return false;
      }
    }

    //*****************************************************************
    /// Enable an observer
    ///\param observer A reference to the observer.
    ///\param state    <b>true</b> to enable, <b>false</b> to disable. Default is enable.
    //*****************************************************************
    void enable_observer(observer_type& observer, bool state = true)
    {
      // See if we have it in our list.
      typename Observer_List::iterator i_observer_item = find_observer(observer);

      // Found it?
      if (i_observer_item != observer_list.end())
      {
        i_observer_item->enabled = state;
      }
    }

    //*****************************************************************
    /// Disable an observer
    //*****************************************************************
    void disable_observer(observer_type& observer)
    {
      // See if we have it in our list.
      typename Observer_List::iterator i_observer_item = find_observer(observer);

      // Found it?
      if (i_observer_item != observer_list.end())
      {
        i_observer_item->enabled = false;
      }
    }

    //*****************************************************************
    /// Clear all observers from the list.
    //*****************************************************************
    void clear_observers()
    {
      observer_list.clear();
    }

    //*****************************************************************
    /// Returns the number of observers.
    //*****************************************************************
    size_type number_of_observers() const
    {
      return observer_list.size();
    }

    //*****************************************************************
    /// Notify all of the observers, sending them the notification.
    ///\tparam TNotification The notification type.
    ///\param n The notification.
    //*****************************************************************
    void notify_observers(TNotification n)
    {
      typename Observer_List::iterator i_observer_item = observer_list.begin();

      while (i_observer_item != observer_list.end())
      {
        if (i_observer_item->enabled)
        {
          i_observer_item->p_observer->notification(n);
        }

        ++i_observer_item;
      }
    }

  protected:

    ~observable()
    {
    }

  private:

    //*****************************************************************
    /// Find an observer in the list.
    /// Returns the end of the list if not found.
    //*****************************************************************
    typename Observer_List::iterator find_observer(observer_type& observer_)
    {
      return etl::find_if(observer_list.begin(), observer_list.end(), compare_observers(observer_));
    }

    /// The list of observers.
    Observer_List observer_list;
  };
}

#endif
