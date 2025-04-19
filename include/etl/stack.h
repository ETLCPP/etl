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

#ifndef ETL_STACK_INCLUDED
#define ETL_STACK_INCLUDED

#include "platform.h"
#include "algorithm.h"
#include "utility.h"
#include "iterator.h"
#include "alignment.h"
#include "array.h"
#include "exception.h"
#include "error_handler.h"
#include "debug_count.h"
#include "type_traits.h"
#include "placement_new.h"

#include <stddef.h>
#include <stdint.h>

//*****************************************************************************
///\defgroup stack stack
/// A Last-in / first-out stack with the capacity defined at compile time,
/// written in the STL style.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //***************************************************************************
  ///\ingroup stack
  /// The base class for stack exceptions.
  //***************************************************************************
  class stack_exception : public exception
  {
  public:

    stack_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  ///\ingroup stack
  /// The exception thrown when the stack is full.
  //***************************************************************************
  class stack_full : public stack_exception
  {
  public:

    stack_full(string_type file_name_, numeric_type line_number_)
      : stack_exception(ETL_ERROR_TEXT("stack:full", ETL_STACK_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  ///\ingroup stack
  /// The exception thrown when the stack is empty.
  //***************************************************************************
  class stack_empty : public stack_exception
  {
  public:

    stack_empty(string_type file_name_, numeric_type line_number_)
      : stack_exception(ETL_ERROR_TEXT("stack:empty", ETL_STACK_FILE_ID"B"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  ///\ingroup stack
  /// A fixed capacity stack written in the STL style.
  /// \warning This stack cannot be used for concurrent access from multiple threads.
  //***************************************************************************
  class stack_base
  {
  public:

    typedef size_t size_type; ///< The type used for determining the size of stack.

    //*************************************************************************
    /// Checks to see if the stack is empty.
    /// \return <b>true</b> if the stack is empty, otherwise <b>false</b>
    //*************************************************************************
    bool empty() const
    {
      return current_size == 0;
    }

    //*************************************************************************
    /// Checks to see if the stack is full.
    /// \return <b>true</b> if the stack is full, otherwise <b>false</b>
    //*************************************************************************
    bool full() const
    {
      return current_size == CAPACITY;
    }

    //*************************************************************************
    /// Returns the current number of items top the stack.
    //*************************************************************************
    size_type size() const
    {
      return current_size;
    }

    //*************************************************************************
    /// Returns the maximum number of items that can be stacked.
    //*************************************************************************
    size_type max_size() const
    {
      return CAPACITY;
    }

    //*************************************************************************
    /// Returns the remaining capacity.
    ///\return The remaining capacity.
    //*************************************************************************
    size_t available() const
    {
      return max_size() - size();
    }

  protected:

    //*************************************************************************
    /// The constructor that is called from derived classes.
    //*************************************************************************
    stack_base(size_type max_size_)
      : top_index(0),
        current_size(0),
        CAPACITY(max_size_)
    {
    }

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~stack_base()
    {
    }

    //*************************************************************************
    /// Increments the indexes value to record a stack addition.
    //*************************************************************************
    void add_in()
    {
      top_index = current_size++;
      ETL_INCREMENT_DEBUG_COUNT;
    }

    //*************************************************************************
    /// Decrements the indexes value to record a queue deletion.
    //*************************************************************************
    void del_out()
    {
      --top_index;
      --current_size;
      ETL_DECREMENT_DEBUG_COUNT;
    }

    //*************************************************************************
    /// Clears all of the indexes.
    //*************************************************************************
    void index_clear()
    {
      top_index = 0;
      current_size = 0;
      ETL_RESET_DEBUG_COUNT;
    }

    size_type top_index;      ///< The index of the top of the stack.
    size_type current_size;   ///< The number of items in the stack.
    const size_type CAPACITY; ///< The maximum number of items in the stack.
    ETL_DECLARE_DEBUG_COUNT;  ///< For internal debugging purposes.
  };

  //***************************************************************************
  ///\ingroup stack
  ///\brief This is the base for all stacks that contain a particular type.
  ///\details Normally a reference to this type will be taken from a derived stack.
  ///\code
  /// etl::stack<int, 10> myStack;
  /// etl::istack<int>& iStack = myStack;
  ///\endcode
  /// \warning This stack cannot be used for concurrent access from multiple threads.
  /// \tparam T The type of value that the stack holds.
  //***************************************************************************
  template <typename T>
  class istack : public etl::stack_base
  {
  public:

    typedef T                     value_type;      ///< The type stored in the stack.
    typedef T&                    reference;       ///< A reference to the type used in the stack.
    typedef const T&              const_reference; ///< A const reference to the type used in the stack.
#if ETL_USING_CPP11
    typedef T&&                   rvalue_reference;///< An rvalue reference to the type used in the stack.
#endif
    typedef T*                    pointer;         ///< A pointer to the type used in the stack.
    typedef const T*              const_pointer;   ///< A const pointer to the type used in the stack.
    typedef stack_base::size_type size_type;       ///< The type used for determining the size of the stack.

  private:

    typedef typename etl::stack_base              base_t;

  public:

    //*************************************************************************
    /// Gets a reference to the value at the top of the stack.<br>
    /// \return A reference to the value at the top of the stack.
    //*************************************************************************
    reference top()
    {
      return p_buffer[top_index];
    }

    //*************************************************************************
    /// Adds a value to the stack.
    /// If asserts or exceptions are enabled, throws an etl::stack_full if the stack is already full.
    ///\param value The value to push to the stack.
    //*************************************************************************
    void push(const_reference value)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(stack_full));
#endif
      base_t::add_in();
      ::new (&p_buffer[top_index]) T(value);
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Adds a value to the stack.
    /// If asserts or exceptions are enabled, throws an etl::stack_full if the stack is already full.
    ///\param value The value to push to the stack.
    //*************************************************************************
    void push(rvalue_reference value)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(stack_full));
#endif
      base_t::add_in();
      ::new (&p_buffer[top_index]) T(etl::move(value));
    }
#endif

#if ETL_USING_CPP11 && ETL_NOT_USING_STLPORT
    //*************************************************************************
    /// Constructs a value in the stack place'.
    /// If asserts or exceptions are enabled, throws an etl::stack_full if the stack is already full.
    ///\param value The value to push to the stack.
    //*************************************************************************
    template <typename ... Args>
    reference emplace(Args && ... args)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(stack_full));
#endif
      base_t::add_in();
      ::new (&p_buffer[top_index]) T(etl::forward<Args>(args)...);

      return p_buffer[top_index];
    }
#else
    //*************************************************************************
    /// Constructs a value in the stack place'.
    /// If asserts or exceptions are enabled, throws an etl::stack_full if the stack is already full.
    ///\param value The value to push to the stack.
    //*************************************************************************
    reference emplace()
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(stack_full));
#endif
      base_t::add_in();
      ::new (&p_buffer[top_index]) T();

      return p_buffer[top_index];
    }

    //*************************************************************************
    /// Constructs a value in the stack place'.
    /// If asserts or exceptions are enabled, throws an etl::stack_full if the stack is already full.
    ///\param value The value to push to the stack.
    //*************************************************************************
    template <typename T1>
    reference emplace(const T1& value1)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(stack_full));
#endif
      base_t::add_in();
      ::new (&p_buffer[top_index]) T(value1);

      return p_buffer[top_index];
    }

    //*************************************************************************
    /// Constructs a value in the stack place'.
    /// If asserts or exceptions are enabled, throws an etl::stack_full if the stack is already full.
    ///\param value The value to push to the stack.
    //*************************************************************************
    template <typename T1, typename T2>
    reference emplace(const T1& value1, const T2& value2)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(stack_full));
#endif
      base_t::add_in();
      ::new (&p_buffer[top_index]) T(value1, value2);

      return p_buffer[top_index];
    }

    //*************************************************************************
    /// Constructs a value in the stack place'.
    /// If asserts or exceptions are enabled, throws an etl::stack_full if the stack is already full.
    ///\param value The value to push to the stack.
    //*************************************************************************
    template <typename T1, typename T2, typename T3>
    reference emplace(const T1& value1, const T2& value2, const T3& value3)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(stack_full));
#endif
      base_t::add_in();
      ::new (&p_buffer[top_index]) T(value1, value2, value3);

      return p_buffer[top_index];
    }

    //*************************************************************************
    /// Constructs a value in the stack place'.
    /// If asserts or exceptions are enabled, throws an etl::stack_full if the stack is already full.
    ///\param value The value to push to the stack.
    //*************************************************************************
    template <typename T1, typename T2, typename T3, typename T4>
    reference emplace(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(stack_full));
#endif
      base_t::add_in();
      ::new (&p_buffer[top_index]) T(value1, value2, value3, value4);

      return p_buffer[top_index];
    }
#endif

    //*************************************************************************
    /// Gets a const reference to the value at the top of the stack.<br>
    /// \return A const reference to the value at the top of the stack.
    //*************************************************************************
    const_reference top() const
    {
      return p_buffer[top_index];
    }

    //*************************************************************************
    /// Clears the stack to the empty state.
    //*************************************************************************
    void clear()
    {
      if ETL_IF_CONSTEXPR(etl::is_trivially_destructible<T>::value)
      {
        base_t::index_clear();
      }
      else
      {
        while (current_size > 0)
        {
          p_buffer[top_index].~T();
          base_t::del_out();
        }
      }
    }

    //*************************************************************************
    /// Removes the oldest item from the top of the stack.
    //*************************************************************************
    void pop()
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT_OR_RETURN(!empty(), ETL_ERROR(stack_empty));
#endif
      p_buffer[top_index].~T();
      base_t::del_out();
    }

    //*************************************************************************
    /// Removes the oldest item from the top of the stack and puts it in the destination.
    //*************************************************************************
    void pop_into(reference destination)
    {
      destination = ETL_MOVE(top());
      pop();
    }

    //*************************************************************************
    /// Removes the oldest item from the top of the stack and pushes it to the
    /// destination container.
    /// NOTE: The destination must support a push(T) member function.
    //*************************************************************************
    template <typename TContainer>
    void pop_into(TContainer& destination)
    {
      destination.push(ETL_MOVE(top()));
      pop();
    }

    //*************************************************************************
    /// Reverses the stack.
    //*************************************************************************
    void reverse()
    {
      etl::reverse(p_buffer, p_buffer + current_size);
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    istack& operator = (const istack& rhs)
    {
      if (&rhs != this)
      {
        clear();
        clone(rhs);
      }

      return *this;
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    istack& operator = (istack&& rhs)
    {
      if (&rhs != this)
      {
        clone(etl::move(rhs));
      }

      return *this;
    }
#endif

  protected:

    //*************************************************************************
    /// Make this a clone of the supplied stack
    //*************************************************************************
    void clone(const istack& other)
    {
      clear();

      size_t index = 0UL;

      for (size_t i = 0UL; i < other.size(); ++i)
      {
        push(other.p_buffer[index++]);
      }
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Make this a clone of the supplied stack
    //*************************************************************************
    void clone(istack&& other)
    {
      clear();

      size_t index = 0UL;

      for (size_t i = 0UL; i < other.size(); ++i)
      {
        push(etl::move(other.p_buffer[index++]));
      }
    }
#endif

    //*************************************************************************
    /// The constructor that is called from derived classes.
    //*************************************************************************
    istack(T* p_buffer_, size_type max_size_)
      : stack_base(max_size_),
        p_buffer(p_buffer_)
    {
    }

  private:

    // Disable copy construction.
    istack(const istack&);

    T* p_buffer; ///< The internal buffer.

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
#if defined(ETL_POLYMORPHIC_STACK) || defined(ETL_POLYMORPHIC_CONTAINERS)
  public:
    virtual ~istack()
    {
    }
#else
  protected:
    ~istack()
    {
    }
#endif
  };

  //***************************************************************************
  ///\ingroup stack
  /// A fixed capacity stack.
  /// This stack does not support concurrent access by different threads.
  /// \tparam T        The type this stack should support.
  /// \tparam MAX_SIZE The maximum capacity of the stack.
  //***************************************************************************
  template <typename T, const size_t SIZE>
  class stack : public etl::istack<T>
  {
  public:
    typedef typename etl::aligned_storage<sizeof(T), etl::alignment_of<T>::value>::type container_type;

    static ETL_CONSTANT size_t MAX_SIZE = SIZE;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    stack()
      : etl::istack<T>(reinterpret_cast<T*>(&buffer[0]), SIZE)
    {
    }

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    stack(const stack& rhs)
      : etl::istack<T>(reinterpret_cast<T*>(&buffer[0]), SIZE)
    {
      etl::istack<T>::clone(rhs);
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    stack(stack&& rhs)
      : etl::istack<T>(reinterpret_cast<T*>(&buffer[0]), SIZE)
    {
      etl::istack<T>::clone(etl::move(rhs));
    }
#endif

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~stack()
    {
      etl::istack<T>::clear();
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    stack& operator = (const stack& rhs)
    {
      if (&rhs != this)
      {
        etl::istack<T>::clone(rhs);
      }

      return *this;
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Move assignment operator.
    //*************************************************************************
    stack& operator = (stack&& rhs)
    {
      if (&rhs != this)
      {
        etl::istack<T>::clone(etl::move(rhs));
      }

      return *this;
    }
#endif

  private:

    /// The uninitialised buffer of T used in the stack.
    container_type buffer[SIZE];
  };

  template <typename T, const size_t SIZE>
  ETL_CONSTANT size_t stack<T, SIZE>::MAX_SIZE;
}

#endif
