///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2015 jwellbelove

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

#ifndef ETL_OPTIONAL_INCLUDED
#define ETL_OPTIONAL_INCLUDED

#include <new>

#include "platform.h"
#include "alignment.h"
#include "type_traits.h"
#include "exception.h"
#include "error_handler.h"

namespace etl
{
  //*****************************************************************************
  /// A null option type.
  ///\ingroup utilities
  //*****************************************************************************
  class nullopt_t
  {
  public:

    // Convertible to any type of null non-member pointer.
    template<class T>
    operator T*() const
    {
      return 0;
    }

  private:

    // Can't take address of nullopt.
    void operator&() const;
  };

  //*****************************************************************************
  /// A null option.
  ///\ingroup utilities
  //*****************************************************************************
  const nullopt_t nullopt = {};

  //***************************************************************************
  /// Exception for optional.
  ///\ingroup list
  //***************************************************************************
  class optional_exception : public exception
  {
  public:

    optional_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Invalid exception for optional.
  ///\ingroup list
  //***************************************************************************
  class optional_invalid : public optional_exception
  {
  public:

    optional_invalid(string_type file_name_, numeric_type line_number_)
      : optional_exception("optional: invalid", file_name_, line_number_)
    {
    }
  };

  //*****************************************************************************
  /// An optional type.
  /// If the optional type is not initialised then a type is not constructed.
  /// See http://en.cppreference.com/w/cpp/utility/optional
  ///\tparam The type to store.
  ///\ingroup utilities
  //*****************************************************************************
  template <typename T>
  class optional
  {
  public:

    //***************************************************************************
    /// Constructor.
    //***************************************************************************
    optional()
      : valid(false)
    {
    }

    //***************************************************************************
    /// Constructor with nullopt.
    //***************************************************************************
    optional(etl::nullopt_t)
      : valid(false)
    {
    }

    //***************************************************************************
    /// Copy constructor.
    //***************************************************************************
    optional(const optional& other)
      : valid(bool(other))
    {
      if (valid)
      {
       ::new (storage.template get_address<T>()) T(other.value());
      }
    }

    //***************************************************************************
    /// Constructor from value type.
    //***************************************************************************
    optional(const T& value_)
    {
     ::new (storage.template get_address<T>()) T(value_);
      valid = true;
    }

    //***************************************************************************
    /// Destructor.
    //***************************************************************************
    ~optional()
    {
      if (valid)
      {
        storage.template get_reference<T>().~T();
      }
    }

    //***************************************************************************
    /// Assignment operator from nullopt.
    //***************************************************************************
    optional& operator =(etl::nullopt_t)
    {
      if (valid)
      {
        storage.template get_reference<T>().~T();
        valid = false;
      }

      return *this;
    }

    //***************************************************************************
    /// Assignment operator from optional.
    //***************************************************************************
    optional& operator =(const optional& other)
    {
      if (this != &other)
      {
        if (valid && !bool(other))
        {
          storage.template get_reference<T>().~T();
          valid = false;
        }
        else if (bool(other))
        {
          if (valid)
          {
            storage.template get_reference<T>() = other.value();
          }
          else
          {
           ::new (storage.template get_address<T>()) T(other.value());
            valid = true;
          }
        }
      }

      return *this;
    }

    //***************************************************************************
    /// Assignment operator from value type.
    //***************************************************************************
    optional& operator =(const T& value_)
    {
      if (valid)
      {
        storage.template get_reference<T>() = value_;
      }
      else
      {
       ::new (storage.template get_address<T>()) T(value_);
        valid = true;
      }

      return *this;
    }

    //***************************************************************************
    /// Pointer operator.
    //***************************************************************************
    T* operator ->()
    {
#if defined(ETL_DEBUG)
      ETL_ASSERT(valid, ETL_ERROR(optional_invalid));
#endif

      return storage.template get_address<T>();
    }

    //***************************************************************************
    /// Pointer operator.
    //***************************************************************************
    const T* operator ->() const
    {
#if defined(ETL_DEBUG)
      ETL_ASSERT(valid, ETL_ERROR(optional_invalid));
#endif

      return storage.template get_address<T>();
    }

    //***************************************************************************
    /// Dereference operator.
    //***************************************************************************
    T& operator *()
    {
#if defined(ETL_DEBUG)
      ETL_ASSERT(valid, ETL_ERROR(optional_invalid));
#endif

      return storage.template get_reference<T>();
    }

    //***************************************************************************
    /// Dereference operator.
    //***************************************************************************
    const T& operator *() const
    {
#if defined(ETL_DEBUG)
      ETL_ASSERT(valid, ETL_ERROR(optional_invalid));
#endif

      return storage.template get_reference<T>();
    }

    //***************************************************************************
    /// Bool conversion operator.
    //***************************************************************************
    explicit operator bool() const
    {
      return valid;
    }

    //***************************************************************************
    /// Get a reference to the value.
    //***************************************************************************
    T& value()
    {
#if defined(ETL_DEBUG)
      ETL_ASSERT(valid, ETL_ERROR(optional_invalid));
#endif

      return storage.template get_reference<T>();
    }

    //***************************************************************************
    /// Get a const reference to the value.
    //***************************************************************************
    const T& value() const
    {
#if defined(ETL_DEBUG)
      ETL_ASSERT(valid, ETL_ERROR(optional_invalid));
#endif

      return storage.template get_reference<T>();
    }

    //***************************************************************************
    /// Gets the value or a default if no valid.
    //***************************************************************************
    T value_or(T default_value) const
    {
      return valid ? value() : default_value;
    }

    //***************************************************************************
    /// Swaps this value with another.
    //***************************************************************************
    void swap(optional& other)
    {
      optional temp(*this);
      *this = other;
      other = temp;
    }

    //***************************************************************************
    /// Reset back to invalid.
    //***************************************************************************
    void reset()
    {
      if (valid)
      {
        storage.template get_reference<T>().~T();
        valid = false;
      }
    }

#if ETL_CPP11_SUPPORTED  && !defined(ETL_STLPORT) && !defined(ETL_OPTIONAL_FORCE_CPP03)
    //*************************************************************************
    /// Emplaces a value.
    ///\param args The arguments to construct with.
    //*************************************************************************
    template <typename ... Args>
    void emplace(Args && ... args)
    {
      if (valid)
      {
        // Destroy the old one.
        storage.template get_reference<T>().~T();
      }

      ::new (storage.template get_address<T>()) T(std::forward<Args>(args)...);
      valid = true;
    }
#else
    //*************************************************************************
    /// Emplaces a value.
    /// 1 parameter.
    //*************************************************************************
    template <typename T1>
    void emplace(const T1& value1)
    {
      if (valid)
      {
        // Destroy the old one.
        storage.template get_reference<T>().~T();
      }

      ::new (storage.template get_address<T>()) T(value1);
      valid = true;
    }

    //*************************************************************************
    /// Emplaces a value.
    /// 2 parameters.
    //*************************************************************************
    template <typename T1, typename T2>
    void emplace(const T1& value1, const T2& value2)
    {
      if (valid)
      {
        // Destroy the old one.
        storage.template get_reference<T>().~T();
      }

      ::new (storage.template get_address<T>()) T(value1, value2);
      valid = true;
    }

    //*************************************************************************
    /// Emplaces a value.
    /// 3 parameters.
    //*************************************************************************
    template <typename T1, typename T2, typename T3>
    void emplace(const T1& value1, const T2& value2, const T3& value3)
    {
      if (valid)
      {
        // Destroy the old one.
        storage.template get_reference<T>().~T();
      }

      ::new (storage.template get_address<T>()) T(value1, value2, value3);
      valid = true;
    }

    //*************************************************************************
    /// Emplaces a value.
    /// 4 parameters.
    //*************************************************************************
    template <typename T1, typename T2, typename T3, typename T4>
    void emplace(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
    {
      if (valid)
      {
        // Destroy the old one.
        storage.template get_reference<T>().~T();
      }

      ::new (storage.template get_address<T>()) T(value1, value2, value3, value4);
      valid = true;
    }
#endif

  private:

    typename etl::aligned_storage_as<sizeof(T), T>::type storage;
    bool valid;
  };
}

//*************************************************************************
/// Swaps the values.
//*************************************************************************
template <typename T>
void swap(etl::optional<T>& lhs, etl::optional<T>& rhs)
{
  lhs.swap(rhs);
}

//***************************************************************************
/// Equality operator. cppreference 1
//***************************************************************************
template <typename T>
bool operator ==(const etl::optional<T>& lhs, const etl::optional<T>& rhs)
{
  if (bool(lhs) != bool(rhs))
  {
    return false;
  }
  else if (!bool(lhs) && !bool(rhs))
  {
    return true;
  }
  else
  {
    return lhs.value() == rhs.value();
  }
}

//***************************************************************************
/// Equality operator. cppreference 2
//***************************************************************************
template <typename T>
bool operator !=(const etl::optional<T>& lhs, const etl::optional<T>& rhs)
{
  return !(lhs == rhs);
}

//***************************************************************************
/// Less than operator. cppreference 3
//***************************************************************************
template <typename T>
bool operator <(const etl::optional<T>& lhs, const etl::optional<T>& rhs)
{
  if (!bool(rhs))
  {
    return false;
  }
  else if (!bool(lhs))
  {
    return true;
  }
  else
  {
    return lhs.value() < rhs.value();
  }
}

//***************************************************************************
/// Less than equal operator. cppreference 4
//***************************************************************************
template <typename T>
bool operator <=(const etl::optional<T>& lhs, const etl::optional<T>& rhs)
{
  if (!bool(lhs))
  {
    return true;
  }
  else if (!bool(rhs))
  {
    return false;
  }
  else
  {
    return lhs.value() <= rhs.value();
  }
}

//***************************************************************************
/// greater than operator. cppreference 5
//***************************************************************************
template <typename T>
bool operator >(const etl::optional<T>& lhs, const etl::optional<T>& rhs)
{
    if (!bool(lhs))
    {
      return false;
    }
    else if (!bool(rhs))
    {
      return true;
    }
    else
    {
      return lhs.value() > rhs.value();
    }
}

//***************************************************************************
/// greater than equal operator. cppreference 6
//***************************************************************************
template <typename T>
bool operator >=(const etl::optional<T>& lhs, const etl::optional<T>& rhs)
{
  if (!bool(rhs))
  {
    return true;
  }
  else if (!bool(lhs))
  {
    return false;
  }
  else
  {
    return lhs.value() >= rhs.value();
  }
}

//***************************************************************************
/// Equality operator. cppreference 7
//***************************************************************************
template <typename T>
bool operator ==(const etl::optional<T>& lhs, etl::nullopt_t)
{
  return !bool(lhs);
}

//***************************************************************************
/// Equality operator. cppreference 8
//***************************************************************************
template <typename T>
bool operator ==(etl::nullopt_t, const etl::optional<T>& rhs)
{
  return !bool(rhs);
}

//***************************************************************************
/// Inequality operator. cppreference 9
//***************************************************************************
template <typename T>
bool operator !=(const etl::optional<T>& lhs, etl::nullopt_t)
{
  return !(lhs == etl::nullopt);
}

//***************************************************************************
/// Inequality operator. cppreference 10
//***************************************************************************
template <typename T>
bool operator !=(etl::nullopt_t , const etl::optional<T>& rhs)
{
  return !(etl::nullopt == rhs);
}

//***************************************************************************
/// Less than operator. cppreference 11
//***************************************************************************
template <typename T>
bool operator <(const etl::optional<T>&, etl::nullopt_t)
{
  return false;
}

//***************************************************************************
/// Less than operator. cppreference 12
//***************************************************************************
template <typename T>
bool operator <(etl::nullopt_t, const etl::optional<T>& rhs)
{
  return bool(rhs);
}

//***************************************************************************
/// Less than equal operator. cppreference 13
//***************************************************************************
template <typename T>
bool operator <=(const etl::optional<T>& lhs, etl::nullopt_t)
{
  return !bool(lhs);
}

//***************************************************************************
/// Less than equal operator. cppreference 14
//***************************************************************************
template <typename T>
bool operator <=(etl::nullopt_t, const etl::optional<T>&)
{
  return true;
}

//***************************************************************************
/// Greater than operator. cppreference 15
//***************************************************************************
template <typename T>
bool operator >(const etl::optional<T>& lhs, etl::nullopt_t)
{
  return bool(lhs);
}

//***************************************************************************
/// Greater than operator. cppreference 16
//***************************************************************************
template <typename T>
bool operator >(etl::nullopt_t, const etl::optional<T>&)
{
  return false;
}

//***************************************************************************
/// Greater than equal operator. cppreference 17
//***************************************************************************
template <typename T>
bool operator >=(const etl::optional<T>&, etl::nullopt_t)
{
  return true;
}

//***************************************************************************
/// Greater than equal operator. cppreference 18
//***************************************************************************
template <typename T>
bool operator >=(etl::nullopt_t, const etl::optional<T>& rhs)
{
  return !bool(rhs);
}

//***************************************************************************
/// Equality operator. cppreference 19
//**************************************************************************
template <typename T, typename U>
bool operator ==(const etl::optional<T>& lhs, const U& rhs)
{
  return bool(lhs) ? lhs.value() == rhs : false;
}

//***************************************************************************
/// Inequality operator. cppreference 21
//**************************************************************************
template <typename T, typename U>
bool operator !=(const etl::optional<T>& lhs, const U& rhs)
{
  return !(lhs == rhs);
}

//***************************************************************************
/// Equality operator. cppreference 20
//**************************************************************************
template <typename T, typename U>
bool operator ==(const U& lhs, const etl::optional<T>& rhs)
{
  return bool(rhs) ? rhs.value() == lhs : false;
}

//***************************************************************************
/// Inequality operator. cppreference 22
//**************************************************************************
template <typename T, typename U>
bool operator !=(const U& lhs, const etl::optional<T>& rhs)
{
  return !(lhs == rhs);
}

//***************************************************************************
/// Less than operator. cppreference 23
//***************************************************************************
template <typename T, typename U>
bool operator <(const etl::optional<T>& lhs, const U& rhs)
{
  return bool(lhs) ? lhs.value() < rhs : true;
}

//***************************************************************************
/// Less than operator. cppreference 24
//***************************************************************************
template <typename T, typename U>
bool operator <(const U& lhs, const etl::optional<T>& rhs)
{
  return bool(rhs) ? lhs < rhs.value() : false;
}

//***************************************************************************
/// Less than equal operator. cppreference 25
//***************************************************************************
template <typename T, typename U>
bool operator <=(const etl::optional<T>& lhs, const U& rhs)
{
  return bool(lhs) ? lhs.value() <= rhs : true;
}

//***************************************************************************
/// Less than equal operator. cppreference 26
//***************************************************************************
template <typename T, typename U>
bool operator <=(const U& lhs, const etl::optional<T>& rhs)
{
  return bool(rhs) ? lhs <= rhs.value() : false;
}

//***************************************************************************
/// Greater than operator. cppreference 27
//***************************************************************************
template <typename T, typename U>
bool operator >(const etl::optional<T>& lhs, const U& rhs)
{
  return bool(lhs) ? lhs.value() > rhs  : false;
}

//***************************************************************************
/// Greater than operator. cppreference 28
//***************************************************************************
template <typename T, typename U>
bool operator >(const U& lhs, const etl::optional<T>& rhs)
{
  return bool(rhs) ? lhs > rhs.value() : true;
}

//***************************************************************************
/// Greater than equal operator. cppreference 29
//***************************************************************************
template <typename T, typename U>
bool operator >=(const etl::optional<T>& lhs, const U& rhs)
{
  return bool(lhs) ? lhs.value() >= rhs : false;
}

//***************************************************************************
/// Greater than equal operator. cppreference 30
//***************************************************************************
template <typename T, typename U>
bool operator >=(const U& lhs, const etl::optional<T>& rhs)
{
  return bool(rhs) ? lhs >= rhs.value() : true;
}

//***************************************************************************
/// Make an optional.
//***************************************************************************
template <typename T>
etl::optional<typename etl::decay<T>::type> make_optional(T& value)
{
  return etl::optional<typename etl::decay<T>::type>(value);
}

#endif
