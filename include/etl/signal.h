///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2025 John Wellbelove, Mark Kitson

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

#ifndef ETL_SIGNAL_INCLUDED
#define ETL_SIGNAL_INCLUDED

#include <cstddef>

#include "platform.h"

#if ETL_NOT_USING_CPP11
  #error NOT SUPPORTED FOR C++03 OR BELOW
#endif

#if ETL_USING_CPP11

#include "exception.h"
#include "error_handler.h"
#include "delegate.h"
#include "algorithm.h"
#include "iterator.h"
#include "file_error_numbers.h"

//*****************************************************************************
///\defgroup signal signal
/// Container that handles storing and calling callbacks.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //***************************************************************************
  ///\ingroup signal
  /// The base class for signal exceptions.
  //***************************************************************************
  class signal_exception : public exception
  {
  public:
    signal_exception(string_type reason_, string_type file_name_, numeric_type line_number_) 
      : exception{reason_, file_name_, line_number_}
    {
    }
  };

  class signal_full : public signal_exception
  {
  public:
    signal_full(string_type file_name_, numeric_type line_number_)
      : signal_exception{ETL_ERROR_TEXT("signal:full", ETL_SIGNAL_FILE_ID"A"), file_name_, line_number_}
    {
    }
  };

  //***************************************************************************
  ///\ingroup signal
  ///
  ///\brief A lightweight signal class designed for efficient memory usage and 
  /// ability to store in ROM.
  ///
  ///\tparam TFunction: Callback signature.
  ///\tparam Length:    Maximum number of slots that can be connected to the signal.
  ///\tparam TSlot:     Function-object type or container type that can be invoked. Default etl::delegate.
  ///
  ///\todo Support for return types other than void (aggregate etc.)
  //***************************************************************************
  template <typename TFunction, size_t Length, typename TSlot = etl::delegate<TFunction>>
  class signal
  {
  public:

    using slot_type = TSlot;
    using size_type = size_t;

    //*************************************************************************
    /// Constructs the signal.
    //*************************************************************************
    template <typename... TArgs>
    ETL_CONSTEXPR14 explicit signal(TArgs&&... args) ETL_NOEXCEPT
      : slot_list{etl::forward<TArgs>(args)...}
      , end_of_list{slot_list + sizeof... (args)}
    {
    };

    //*************************************************************************
    ///\brief Invokes all the slots connected to the signal.
    /// Checks if the slot is valid to call.
    /// 
    ///\param args: Arguments to pass to the slots.
    //*************************************************************************
    template <typename... TArgs>
    void operator()(TArgs&&... args) const ETL_NOEXCEPT
    {
      for (const slot_type& s : *this)
      {
        if (valid(s))
        {
          s(etl::forward<TArgs>(args)...);
        }
      }
    }

    //*************************************************************************
    ///\brief Connects a slot to the signal.
    /// Ignores the slot if it has already been connected.
    ///
    ///\param slot: To connect.
    //*************************************************************************
    void connect(const slot_type& slot)
    {
      ETL_ASSERT_OR_RETURN(!full(), ETL_ERROR(signal_full));

      if (!connected(slot))
      {
        (*end_of_list) = slot;
        end_of_list    = etl::next(end_of_list);
      }
    }

    //*************************************************************************
    ///\brief Disconnects a slot from the signal.
    ///
    ///\param slot: To disconnect.
    //*************************************************************************
    void disconnect(const slot_type& slot) ETL_NOEXCEPT
    {
        const auto end_itr = end();
        const auto itr     = etl::find(begin(), end_itr, slot);
        
        if (itr != end_itr)
        {
          // Copy + replace idiom (expensive).  Shifts all elements after 'itr' one
          // position to the left.
          etl::copy(etl::next(itr), end_itr, itr);
          end_of_list = etl::prev(end_of_list);
        }
    }

    //*************************************************************************
    ///\brief Disconnects all slots from the signal.
    //*************************************************************************
    void disconnect_all() ETL_NOEXCEPT
    {
      end_of_list = begin();
    }

    //*************************************************************************
    ///\brief Checks if a slot is connected to the signal.
    ///
    ///\param slot: To check.
    ///\return true if the slot is connected.
    //*************************************************************************
    ETL_CONSTEXPR bool connected(const slot_type& slot) const ETL_NOEXCEPT
    {
      return etl::any_of(begin(), end(), [&slot](const slot_type& s) { return s == slot; });
    }

    //*************************************************************************
    ///\return true if the signal has no slots connected.
    //*************************************************************************
    ETL_CONSTEXPR bool empty() const ETL_NOEXCEPT
    {
      return begin() == end();
    }

    //*************************************************************************
    ///\return true if the signal has the maximum number of slots connected.
    //*************************************************************************
    ETL_CONSTEXPR bool full() const ETL_NOEXCEPT
    {
      return size() == max_size();
    }

    //*************************************************************************
    ///\return Total number of slots that can be connected.
    //*************************************************************************
    ETL_CONSTEXPR size_type max_size() const ETL_NOEXCEPT
    {
      return Length;
    }

    //*************************************************************************
    ///\return Total slots currently connected.
    //*************************************************************************
    ETL_CONSTEXPR size_type size() const ETL_NOEXCEPT
    {
      return etl::distance(begin(), end());
    }
  
  private:

    using iterator       = slot_type*;
    using const_iterator = const slot_type*;

    slot_type slot_list[Length];
    iterator  end_of_list;

    //*************************************************************************
    /// For a delegate slot type.
    //*************************************************************************
    template <typename TSlotType, typename... TArgs>
    static 
    typename etl::enable_if_t<etl::is_delegate<TSlotType>::value, bool>
      valid(const TSlotType& s)
    {
      return s.is_valid();
    }

    //*************************************************************************
    /// For a non-delegate slot type.
    //*************************************************************************
    template <typename TSlotType, typename... TArgs>
    static 
    typename etl::enable_if_t<!etl::is_delegate<TSlotType>::value, bool>
      valid(const TSlotType&)
    {
      return true;
    }

    //*************************************************************************
    ///\return Iterator to the beginning of the connected slots.
    //*************************************************************************
    ETL_CONSTEXPR14 iterator begin() ETL_NOEXCEPT
    {
      return slot_list;
    }

    //*************************************************************************
    ///\return Const Iterator to the beginning of the connected slots.
    //*************************************************************************
    ETL_CONSTEXPR14 const_iterator begin() const ETL_NOEXCEPT
    {
      return slot_list;
    }

    //*************************************************************************
    ///\return Const Iterator to the beginning of the connected slots.
    //*************************************************************************
    ETL_CONSTEXPR14 const_iterator cbegin() const ETL_NOEXCEPT
    {
      return slot_list;
    }

    //*************************************************************************
    ///\return Iterator to the end of the connected slots.
    //*************************************************************************
    ETL_CONSTEXPR14 iterator end() ETL_NOEXCEPT
    {
      return end_of_list;
    }

    //*************************************************************************
    ///\return Const Iterator to the end of the connected slots.
    //*************************************************************************
    ETL_CONSTEXPR14 const_iterator end() const ETL_NOEXCEPT
    {
      return end_of_list;
    }

    //*************************************************************************
    ///\return Const Iterator to the end of the connected slots.
    //*************************************************************************
    ETL_CONSTEXPR14 const_iterator cend() const ETL_NOEXCEPT
    {
      return end_of_list;
    }
  };
}

#endif
#endif
