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

#if ETL_NOT_USING_CPP11 && !defined(ETL_IN_UNIT_TEST)
#error NOT SUPPORTED FOR C++03 OR BELOW
#endif

#if ETL_USING_CPP11

#include "exception.h"
#include "error_handler.h"
#include "delegate.h"
#include "algorithm.h"
#include "iterator.h"
#include "type_traits.h"
#include "initializer_list.h"
#include "span.h"
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

  //***************************************************************************
  ///\ingroup signal
  /// Signal full exception.
  //***************************************************************************
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
  ///\tparam Size:      Maximum number of slots that can be connected to the signal.
  ///\tparam TSlot:     Function-object type or container type that can be invoked. Default etl::delegate<TFunction>.
  //***************************************************************************
  template <typename TFunction, size_t Size, typename TSlot = etl::delegate<TFunction>>
  class signal
  {
  public:

    using slot_type = TSlot;
    using size_type = size_t;
    using span_type = etl::span<const slot_type>;

    //*************************************************************************
    ///\brief  Construct the signal from a variadic list of slots.
    ///
    ///\param slots: Variadic list of slots.
    //*************************************************************************
    template <typename... TSlots>
    ETL_CONSTEXPR14 explicit signal(TSlots&&... slots) ETL_NOEXCEPT
      : slot_list{etl::forward<TSlots>(slots)...}
      , slot_list_end{slot_list + sizeof...(slots)}
    {
      static_assert((etl::are_all_same<slot_type, etl::decay_t<TSlots>...>::value), "All slots must be slot_type");
      static_assert(sizeof...(slots) <= Size, "Number of slots exceeds capacity");
    }

    //*************************************************************************
    ///\brief Connects a slot to the signal.
    /// Ignores the slot if it has already been connected.
    ///
    ///\param slot: The slot to connect.
    ///\return <b>false</b> if not all slots could be connected.
    //*************************************************************************
    bool connect(const slot_type& slot) ETL_NOEXCEPT_EXPR(ETL_NOT_USING_EXCEPTIONS)
    {
      if (!connected(slot))
      {
        ETL_ASSERT_OR_RETURN_VALUE(!full(), ETL_ERROR(signal_full), false);
        append_slot(slot);
      }

      return true;
    }

#if ETL_HAS_INITIALIZER_LIST && ETL_USING_CPP17
    //*************************************************************************
    ///\brief Connects slots to the signal.
    /// Ignores the slots if it has already been connected.
    ///
    ///\param slots: std::initializer_list of slots to connect.
    ///\return <b>false</b> if not all slots could be connected.
    //*************************************************************************
    bool connect(std::initializer_list<const slot_type> slots) ETL_NOEXCEPT_EXPR(ETL_NOT_USING_EXCEPTIONS)
    {
      for (const slot_type& slot : slots)
      {
        if (!connected(slot))
        {
          ETL_ASSERT_OR_RETURN_VALUE(!full(), ETL_ERROR(signal_full), false);
          append_slot(slot);
        }
      }

      return true;
    }
#endif

    //*************************************************************************
    ///\brief Connects slots to the signal.
    /// Ignores the slots if it has already been connected.
    ///
    ///\param slots: etl::span of slots to connect.
    ///\return <b>false</b> if not all slots could be connected.
    //*************************************************************************
    bool connect(const span_type slots) ETL_NOEXCEPT_EXPR(ETL_NOT_USING_EXCEPTIONS)
    {
      for (const slot_type& slot : slots)
      {
        if (!connected(slot))
        {
          ETL_ASSERT_OR_RETURN_VALUE(!full(), ETL_ERROR(signal_full), false);
          append_slot(slot);
        }
      }

      return true;
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
        // Shifts all elements after 'itr' one position to the left.
        etl::copy(etl::next(itr), end_itr, itr);
        slot_list_end = etl::prev(slot_list_end);
      }
    }

#if ETL_HAS_INITIALIZER_LIST && ETL_USING_CPP17
    //*************************************************************************
    ///\brief Disconnects multiple slots from the signal.
    ///
    ///\param slot: std::intializer_list of slots to disconnect.
    //*************************************************************************
    void disconnect(std::initializer_list<const slot_type> slots) ETL_NOEXCEPT
    {
      for (const slot_type& slot : slots)
      {
        disconnect(slot);
      }
    }
#endif

    //*************************************************************************
    ///\brief Disconnects multiple slots from the signal.
    ///
    ///\param slot: etl::span of slots to disconnect.
    //*************************************************************************
    void disconnect(const span_type slots) ETL_NOEXCEPT
    {
      for (const slot_type& slot : slots)
      {
        disconnect(slot);
      }
    }

    //*************************************************************************
    ///\brief Disconnects all slots from the signal.
    //*************************************************************************
    void disconnect_all() ETL_NOEXCEPT
    {
      slot_list_end = begin();
    }

    //*************************************************************************
    ///\brief Checks if a slot is connected to the signal.
    ///
    ///\param slot: To check.
    ///\return <b>true</b> if the slot is connected.
    //*************************************************************************
    ETL_CONSTEXPR14 bool connected(const slot_type& slot) const ETL_NOEXCEPT
    {
      return etl::any_of(begin(), end(), [&slot](const slot_type& s) { return s == slot; });
    }

    //*************************************************************************
    ///\return <b>true</b> if the signal has no slots connected.
    //*************************************************************************
    ETL_CONSTEXPR14 bool empty() const ETL_NOEXCEPT
    {
      return begin() == end();
    }

    //*************************************************************************
    ///\return <b>true</b> if the signal has the maximum number of slots connected.
    //*************************************************************************
    ETL_CONSTEXPR14 bool full() const ETL_NOEXCEPT
    {
      return size() == max_size();
    }

    //*************************************************************************
    ///\return Total number of slots that can be connected.
    //*************************************************************************
    ETL_CONSTEXPR14 size_type max_size() const ETL_NOEXCEPT
    {
      return Size;
    }

    //*************************************************************************
    ///\return Total slots currently connected.
    //*************************************************************************
    ETL_CONSTEXPR14 size_type size() const ETL_NOEXCEPT
    {
      return etl::distance(begin(), end());
    }

    //*************************************************************************
    ///\return Total empty slots available.
    //*************************************************************************
    ETL_CONSTEXPR14 size_type available() const ETL_NOEXCEPT
    {
      return max_size() - size();
    }

    //*************************************************************************
    ///\brief Invokes all the slots connected to the signal.
    /// Checks if the slot is valid to call.
    /// 
    ///\param args: Arguments to pass to the slots.
    //*************************************************************************
    template <typename... TArgs>
    void operator()(TArgs&&... args) const ETL_NOEXCEPT
    {
      for (const slot_type& slot : *this)
      {
        if (slot_is_valid(slot))
        {
          slot(etl::forward<TArgs>(args)...);
        }
      }
    }

  private:

    using iterator       = slot_type*;
    using const_iterator = const slot_type*;

    slot_type slot_list[Size];
    iterator  slot_list_end;

    //*************************************************************************
    /// Appends a slot to the slot list.
    //*************************************************************************
    void append_slot(const slot_type& slot) ETL_NOEXCEPT
    {
      (*slot_list_end) = slot;
      slot_list_end    = etl::next(slot_list_end);
    }

    //*************************************************************************
    /// For a delegate slot type.
    //*************************************************************************
    template <typename TSlotType, typename... TArgs>
    static 
    typename etl::enable_if_t<etl::is_delegate<TSlotType>::value, bool>
      slot_is_valid(const TSlotType& s) ETL_NOEXCEPT
    {
      return s.is_valid();
    }

    //*************************************************************************
    /// For a non-delegate slot type.
    //*************************************************************************
    template <typename TSlotType, typename... TArgs>
    static 
    typename etl::enable_if_t<!etl::is_delegate<TSlotType>::value, bool>
      slot_is_valid(const TSlotType&) ETL_NOEXCEPT
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
    ///\return Iterator to the end of the connected slots.
    //*************************************************************************
    ETL_CONSTEXPR14 iterator end() ETL_NOEXCEPT
    {
      return slot_list_end;
    }

    //*************************************************************************
    ///\return Const Iterator to the end of the connected slots.
    //*************************************************************************
    ETL_CONSTEXPR14 const_iterator end() const ETL_NOEXCEPT
    {
      return slot_list_end;
    }
  };
}

#endif
#endif
