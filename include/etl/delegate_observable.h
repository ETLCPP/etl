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

#ifndef ETL_DELEGATE_OBSERVABLE_INCLUDED
#define ETL_DELEGATE_OBSERVABLE_INCLUDED

#include "platform.h"
#include "delegate.h"
#include "array.h"

namespace etl
{
  //*********************************************************************
  /// An obserable type that uses delegates to notify observers.
  ///\tparam TNotification The notification type sent to the observers.
  ///\tparam Max_Observers The maximum number of observers that can be accommodated.
  ///\ingroup observer
  //*********************************************************************
  template <typename TNotification, size_t Max_Observers>
  class delegate_observable
  {
  public:
    
    /// The type of the observers.
    typedef etl::delegate<void(TNotification)> delegate_type;

  private:

    typedef etl::array<delegate_type, Max_Observers> DelegateList;

  public:

    /// The type for sizes.
    typedef size_t        size_type;
    
    /// The type of the notification.
    typedef TNotification notification_type;

    //*****************************************************************
    /// Default constructor.
    //*****************************************************************
    ETL_CONSTEXPR14 delegate_observable()
      : delegate_list()
      , delegate_count(0)
    {
    }

#if ETL_USING_CPP11
    //*****************************************************************
    /// Construct from a list of observers.
    //*****************************************************************
    template <typename... TDelegate>
    ETL_CONSTEXPR14 delegate_observable(TDelegate&&... delegates)
      : delegate_list{ etl::forward<TDelegate>(delegates)... }
      , delegate_count(sizeof...(delegates))
    {
      ETL_STATIC_ASSERT(Max_Observers >= sizeof...(delegates), "Number of delegates exceeds maximum observers");
      ETL_STATIC_ASSERT((etl::are_all_same<delegate_type, etl::decay_t<TDelegate>...>::value), "All delegates must be delegate_type");
    }

    //*****************************************************************
    /// Construct from notification type and a list of observers.
    /// Variant for template deduction guide.
    /// The notification value is ignored. It is just here to allow deduction 
    /// of the notification type for the template deduction guide.
    //*****************************************************************
    template <typename... TDelegate>
    ETL_CONSTEXPR14 delegate_observable(notification_type, TDelegate&&... delegates)
      : delegate_observable(etl::forward<TDelegate>(delegates)...)
    {
    }
#endif

    //*****************************************************************
    /// Add an observer to the list.
    ///\param observer A reference to the observer.
    ///\return <b>true</b> if the observer was added or already exists, <b>false</b> if not.
    //*****************************************************************
    ETL_CONSTEXPR14 bool add_observer(delegate_type observer)
    {
      for (size_t i = 0; i < Max_Observers; ++i)
      {
        if (delegate_list[i] == observer)
        {
          // Already there, so just return.
          return true;
        }
      }

      // If we get here, then we need to add it.
      for (size_t i = 0; i < Max_Observers; ++i)
      {
        if (!delegate_list[i].is_valid())
        {
          // Found an empty slot, so add it.
          delegate_list[i] = observer;
          ++delegate_count;
          return true;
        }
      }

      return false;
    }

    //*****************************************************************
    /// Remove a particular observer from the list.
    ///\param observer A reference to the observer.
    ///\return <b>true</b> if the observer was removed, <b>false</b> if not.
    //*****************************************************************
    ETL_CONSTEXPR14 bool remove_observer(const delegate_type& observer)
    {
      for (size_t i = 0; i < Max_Observers; ++i)
      {
        if (delegate_list[i] == observer)
        {
          // Clear it.
          delegate_list[i].clear();
          --delegate_count;
          return true;
        }
      }
       
      return false;
    }

    //*****************************************************************
    /// Clear all observers.
    //*****************************************************************
    ETL_CONSTEXPR14 void clear_observers()
    {
      for (size_t i = 0; i < Max_Observers; ++i)
      {
        delegate_list[i].clear();
      }

      delegate_count = 0;
    }

    //*****************************************************************
    /// Returns the number of observers.
    //*****************************************************************
    ETL_CONSTEXPR14 size_type number_of_observers() const
    {
      return delegate_count;
    }

    //*****************************************************************
    /// Notify all of the observers, sending them the notification.
    ///\tparam TNotification The notification type.
    ///\param n The notification.
    //*****************************************************************
    ETL_CONSTEXPR14 void notify_observers(notification_type n) const
    {
      if (delegate_count != 0)
      {
        for (size_t i = 0; i < Max_Observers; ++i)
        {
          delegate_list[i].call_if(n);
        }
      }
    }

  private:

    /// The list of observers.
    DelegateList delegate_list;
    
    /// The number of active delegates.
    size_t delegate_count;
  };

  //*************************************************************************
  /// Template deduction guides.
  //*************************************************************************
#if ETL_USING_CPP17
  template <typename TNotification, typename... TDelegates>
  delegate_observable(TNotification, TDelegates...) -> delegate_observable<TNotification, sizeof...(TDelegates)>;
#endif
}

#endif
