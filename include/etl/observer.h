///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2014 jwellbelove

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

#ifndef ETL_OBSERVER_INCLUDED
#define ETL_OBSERVER_INCLUDED

//*****************************************************************************
///\defgroup observer observer
/// A templated implementation to simplify the creation of the observer pattern
/// and attempts to eliminate certain runtime errors by turning them into compile errors.
/// The pattern consists of two template classes.
/// \li <b>Observer</b><br>
/// This template may take up to eight notification types.
/// Each notification type will generate a pure virtual 'notification'
/// function. The class that inherits from this *must* define all
/// of the 'notification' function overloads otherwise the object will
/// remain 'abstract' and will not compile.
/// This ensures that no overload can be forgotten.<br>
///
/// \li <b>observable</b><br>
/// The class derived from this will be observed by the above class.
/// It keeps a list of registered observers and will notify all
/// of them with the notifications.
///\ingroup patterns
//*****************************************************************************

#include "algorithm.h"

#include "platform.h"
#include "vector.h"
#include "exception.h"
#include "error_handler.h"

#undef ETL_FILE
#define ETL_FILE "18"

namespace etl
{
  //***************************************************************************
  ///\ingroup observer
  /// The base class for observer exceptions.
  //***************************************************************************
  class observer_exception : public exception
  {
  public:

    observer_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  ///\ingroup observer
  /// The exception thrown when the observer list is full.
  //***************************************************************************
  class observer_list_full : public observer_exception
  {
  public:

    observer_list_full(string_type file_name_, numeric_type line_number_)
      : observer_exception(ETL_ERROR_TEXT("observer:full", ETL_FILE"A"), file_name_, line_number_)
    {
    }
  };

  //*********************************************************************
  /// The object that is being observed.
  ///\tparam TObserver     The observer type.
  ///\tparam MAX_OBSERVERS The maximum number of observers that can be accomodated.
  ///\ingroup observer
  //*********************************************************************
  template <typename TObserver, const size_t MAX_OBSERVERS>
  class observable
  {
  public:

    typedef size_t size_type;

    typedef etl::vector<TObserver*, MAX_OBSERVERS> Observer_List;

    //*****************************************************************
    /// Add an observer to the list.
    /// If asserts or exceptions are enabled then an etl::observable_observer_list_full
    /// is emitted if the observer list is already full.
    ///\param observer A reference to the observer.
    //*****************************************************************
    void add_observer(TObserver& observer)
    {
		  // See if we already have it in our list.
      typename Observer_List::const_iterator i_observer = std::find(observer_list.begin(),
                                                                    observer_list.end(),
                                                                    &observer);

		  // Not there?
      if (i_observer == observer_list.end())
      {
        // Is there enough room?
        ETL_ASSERT(!observer_list.full(), ETL_ERROR(etl::observer_list_full));

        // Add it.
        observer_list.push_back(&observer);
      }
    }

    //*****************************************************************
    /// Remove a particular observer from the list.
    ///\param observer A reference to the observer.
    ///\return <b>true</b> if the observer was removed, <b>false</b> if not.
    //*****************************************************************
    bool remove_observer(TObserver& observer)
    {
      // See if we have it in our list.
      typename Observer_List::iterator i_observer = std::find(observer_list.begin(),
                                                              observer_list.end(),
                                                              &observer);

      // Found it?
      if (i_observer != observer_list.end())
      {
        // Erase it.
        observer_list.erase(i_observer);
        return true;
      }
      else
      {
        return false;
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
    ///\tparam TNotification the notification type.
    ///\param n The notification.
    //*****************************************************************
#if ETL_CPP11_SUPPORTED && !defined(ETL_OBSERVER_FORCE_CPP03)
    template <typename... TNotification>
    void notify_observers(TNotification... n)
    {
      for (auto observer : observer_list)
      {
        observer->notification(n...);
      }
    }
#else
    template <typename TNotification>
    void notify_observers(TNotification n)
    {
      for (size_t i = 0; i < observer_list.size(); ++i)
      {
        observer_list[i]->notification(n);
      }
    }
#endif


  protected:

    ~observable()
    {
    }

  private:

    /// The list of observers.
    Observer_List observer_list;
  };

#if ETL_CPP11_SUPPORTED && !defined(ETL_OBSERVER_FORCE_CPP03)

  //*****************************************************************
  /// The observer class for N types.
  ///\ingroup observer
  //*****************************************************************
  template <typename T1, typename... Types>
  class observer : public observer<T1>, public observer<Types...>
  {
  public:

    using observer<T1>::notification;
    using observer<Types...>::notification;
  };

  //*****************************************************************
  /// The specialised observer class for 1 type.
  ///\ingroup observer
  //*****************************************************************
  template <typename T1>
  class observer<T1>
  {
  public:

    virtual void notification(T1) = 0;
  };

#else

  //*********************************************************************
  /// The observer interface for eight notification types.
  ///\ingroup observer
  //*********************************************************************
  template <typename T1,
            typename T2  = void,
            typename T3  = void,
            typename T4  = void,
            typename T5  = void,
            typename T6  = void,
            typename T7  = void,
            typename T8  = void>
  class observer
  {
  public:
    virtual ~observer() {}
    virtual void notification(T1) = 0;
    virtual void notification(T2) = 0;
    virtual void notification(T3) = 0;
    virtual void notification(T4) = 0;
    virtual void notification(T5) = 0;
    virtual void notification(T6) = 0;
    virtual void notification(T7) = 0;
    virtual void notification(T8) = 0;
  };

  //*********************************************************************
  /// The observer interface for seven notification types.
  ///\ingroup observer
  //*********************************************************************
  template <typename T1,
            typename T2,
            typename T3,
            typename T4,
            typename T5,
            typename T6,
            typename T7>
  class observer<T1, T2, T3, T4, T5, T6, T7>
  {
  public:

    virtual ~observer() {}
    virtual void notification(T1) = 0;
    virtual void notification(T2) = 0;
    virtual void notification(T3) = 0;
    virtual void notification(T4) = 0;
    virtual void notification(T5) = 0;
    virtual void notification(T6) = 0;
    virtual void notification(T7) = 0;
  };

  //*********************************************************************
  /// The observer interface for six notification types.
  ///\ingroup observer
  //*********************************************************************
  template <typename T1,
            typename T2,
            typename T3,
            typename T4,
            typename T5,
            typename T6>
  class observer<T1, T2, T3, T4, T5, T6>
  {
  public:

    virtual ~observer() {}
    virtual void notification(T1) = 0;
    virtual void notification(T2) = 0;
    virtual void notification(T3) = 0;
    virtual void notification(T4) = 0;
    virtual void notification(T5) = 0;
    virtual void notification(T6) = 0;
  };

  //*********************************************************************
  /// The observer interface for five notification types.
  ///\ingroup observer
  //*********************************************************************
  template <typename T1,
            typename T2,
            typename T3,
            typename T4,
            typename T5>
  class observer<T1, T2, T3, T4, T5>
  {
  public:

    virtual ~observer() {}
    virtual void notification(T1) = 0;
    virtual void notification(T2) = 0;
    virtual void notification(T3) = 0;
    virtual void notification(T4) = 0;
    virtual void notification(T5) = 0;
  };

  //*********************************************************************
  /// The observer interface for four notification types.
  ///\ingroup observer
  //*********************************************************************
  template <typename T1,
            typename T2,
            typename T3,
            typename T4>
  class observer<T1, T2, T3, T4>
  {
  public:

    virtual ~observer() {}
    virtual void notification(T1) = 0;
    virtual void notification(T2) = 0;
    virtual void notification(T3) = 0;
    virtual void notification(T4) = 0;
  };

  //*********************************************************************
  /// The observer interface for three notification types.
  ///\ingroup observer
  //*********************************************************************
  template <typename T1,
            typename T2,
            typename T3>
  class observer<T1, T2, T3>
  {
  public:

    virtual ~observer() {}
    virtual void notification(T1) = 0;
    virtual void notification(T2) = 0;
    virtual void notification(T3) = 0;
  };

  //*********************************************************************
  /// The observer interface for two notification types.
  ///\ingroup observer
  //*********************************************************************
  template <typename T1,
            typename T2>
  class observer<T1, T2>
  {
  public:

    virtual ~observer() {}
    virtual void notification(T1) = 0;
    virtual void notification(T2) = 0;
  };

  //*********************************************************************
  /// The observer interface for one notification type.
  ///\ingroup observer
  //*********************************************************************
  template <typename T1>
  class observer<T1>
  {
  public:

    virtual ~observer() {}
    virtual void notification(T1) = 0;
  };

#endif
}

#undef ETL_FILE

#endif
