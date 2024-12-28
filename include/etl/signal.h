///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 John Wellbelove, Mark Kitson

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

#include "exception.h"
#include "error_handler.h"
#include "delegate.h"
#include "platform.h"
#include "algorithm.h"

#if ETL_USING_STL
  #include <algorithm>
  #include <iterator>

#if ETL_CPP11_NOT_SUPPORTED
    #include "algorithm.h"
#endif // ETL_CPP11_NOT_SUPPORTED
#else 
  #include "algorithm.h"
  #include "iterator.h"
#endif // ETL_USING_STL

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

  class signal_full final : public signal_exception
  {
  public:
    signal_full(string_type file_name_, numeric_type line_number_)
      : signal_exception{ETL_ERROR_TEXT("signal:full", "A"), file_name_, line_number_}
    {
    }
  };


  //***************************************************************************
  ///\ingroup signal
  ///
  ///\brief A lightweight signal class designed for efficient memory usage and 
  /// ability to store in ROM.
  ///
  ///\tparam T: Callback signature.
  ///\tparam LENGTH: Maximum number of slots that can be connected to the signal.
  ///\tparam TSlot: Function-object type or container type that can be invoked.
  ///
  ///\todo Support for return types other than void (aggregate etc.)
  //***************************************************************************
  template <typename T, size_t LENGTH, typename TSlot = etl::delegate<T>>
  class signal final
  {
  public:
    using slot_type = TSlot;
    using size_type = size_t;

    //*************************************************************************
    /// Constructs the signal.
    //*************************************************************************
    template <typename... TArgs>
    ETL_CONSTEXPR14 explicit signal(TArgs&&... args) ETL_NOEXCEPT
      : _slots{ETL_OR_STD::forward<TArgs>(args)...}
      , _end{std::next(_slots, sizeof...(args))}
    {
    };

    //*************************************************************************
    ///\brief Invokes all the slots connected to the signal.
    ///
    ///\param args: Arguments to pass to the slots.
    //*************************************************************************
    template <typename... TArgs>
    void operator()(TArgs&&... args) const ETL_NOEXCEPT
    {
      ETL_OR_STD::for_each(begin(), end(), [&args...](const slot_type& s) {
          s(ETL_OR_STD::forward<TArgs>(args)...);
      });
    }

    //*************************************************************************
    ///\brief Connects a slot to the signal.
    ///
    ///\param slot: To connect.
    //*************************************************************************
    void connect(const slot_type& slot)
    {
      ETL_ASSERT_OR_RETURN(!full(), ETL_ERROR(signal_full));
      (*_end) = slot;
      _end = ETL_OR_STD::next(_end);
    }

    //*************************************************************************
    ///\brief Disconnects a slot from the signal.
    ///
    ///\param slot: To disconnect.
    //*************************************************************************
    void disconnect(const slot_type& slot) ETL_NOEXCEPT
    {
        if(empty())
        {
          return;
        }

        const auto end_it = end();
        const auto it = ETL_OR_STD::find(begin(), end_it, slot);
        if(it == end_it)
        {
          return;
        }

        // Copy + replace idiom (expensive).  Shifts all elements after 'it' one
        // position to the left.
        ETL_OR_STD::copy(ETL_OR_STD::next(it), end_it, it);
        _end = ETL_OR_STD::prev(_end);
    }

    //*************************************************************************
    ///\brief Disconnects all slots from the signal.
    //*************************************************************************
    void disconnect_all() ETL_NOEXCEPT
    {
      _end = begin();
    }

    //*************************************************************************
    ///\brief Checks if a slot is connected to the signal.
    ///
    ///\param slot: To check.
    ///\return true if the slot is connected.
    //*************************************************************************
    ETL_CONSTEXPR bool connected(const slot_type& slot) const ETL_NOEXCEPT
    {
#ifdef ETL_CPP11_SUPPORTED
        return ETL_OR_STD::any_of(begin(), end(), [&slot](const slot_type& s){return s == slot;});
#else
        return any_of(begin(), end(), [&slot](const slot_type& s) {
            return s == slot;
        });
#endif // ETL_CPP11_SUPPORTED
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
      return LENGTH;
    }

    //*************************************************************************
    ///\return Total slots currently connected.
    //*************************************************************************
    ETL_CONSTEXPR size_type size() const ETL_NOEXCEPT
    {
      return ETL_OR_STD::distance(begin(), end());
    }
  
  private:
    using iterator = slot_type*;
    using const_iterator = const slot_type*;

    slot_type _slots[LENGTH];
    iterator _end;

    //*************************************************************************
    ///\return Iterator to the beginning of the connected slots.
    //*************************************************************************
    ETL_CONSTEXPR14 iterator begin() ETL_NOEXCEPT
    {
      return _slots;
    }

    //*************************************************************************
    ///\return Const Iterator to the beginning of the connected slots.
    //*************************************************************************
    ETL_CONSTEXPR14 const_iterator begin() const ETL_NOEXCEPT
    {
      return _slots;
    }

    //*************************************************************************
    ///\return Iterator to the end of the connected slots.
    //*************************************************************************
    ETL_CONSTEXPR14 iterator end() ETL_NOEXCEPT
    {
      return _end;
    }

    //*************************************************************************
    ///\return Const Iterator to the end of the connected slots.
    //*************************************************************************
    ETL_CONSTEXPR14 const_iterator end() const ETL_NOEXCEPT
    {
      return _end;
    }
  };
}

#endif // ETL_SIGNAL_INCLUDED
