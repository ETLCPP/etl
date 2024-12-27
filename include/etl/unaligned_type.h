///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2022 John Wellbelove

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

#ifndef ETL_UNALIGNED_TYPE_INCLUDED
#define ETL_UNALIGNED_TYPE_INCLUDED

///\defgroup unaligned_types unaligned_types
/// Unaligned types utilities
///\ingroup utilities

#include "platform.h"
#include "type_traits.h"
#include "nullptr.h"
#include "endianness.h"
#include "error_handler.h"
#include "exception.h"
#include "iterator.h"
#include "algorithm.h"
#include "bit.h"

#include <string.h>

namespace etl
{
  //***************************************************************************
  /// The base class for unaligned_type exceptions.
  ///\ingroup pool
  //***************************************************************************
  struct unaligned_type_exception : public etl::exception
  {
  public:

    unaligned_type_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// The base class for unaligned_type buffer overflow.
  ///\ingroup pool
  //***************************************************************************
  class unaligned_type_buffer_size : public unaligned_type_exception
  {
  public:

    unaligned_type_buffer_size(string_type file_name_, numeric_type line_number_)
      : unaligned_type_exception(ETL_ERROR_TEXT("unaligned_type:buffer size", ETL_UNALIGNED_TYPE_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  namespace private_unaligned_type
  {

    //*************************************************************************
    /// unaligned_type_common
    /// Contains all functionality that doesn't require the type.
    /// Implements CRTP for StorageContainer
    //*************************************************************************
    template <size_t Size_, typename StorageContainer_>
    class unaligned_type_common
    {
    public:

      static ETL_CONSTANT size_t Size = Size_;

      typedef StorageContainer_                     storage_container_type;
      typedef unsigned char                         storage_value_type;
      typedef storage_value_type*                   pointer;
      typedef const storage_value_type*             const_pointer;
      typedef storage_value_type*                   iterator;
      typedef const storage_value_type*             const_iterator;
      typedef etl::reverse_iterator<iterator>       reverse_iterator;
      typedef etl::reverse_iterator<const_iterator> const_reverse_iterator;

      //*************************************************************************
      /// Default constructor
      //*************************************************************************
      ETL_CONSTEXPR unaligned_type_common()
      {
      }

      //*************************************************************************
      /// Size of the storage.
      //*************************************************************************
      ETL_CONSTEXPR size_t size() const
      {
        return Size;
      }

      //*************************************************************************
      /// Pointer to the beginning of the storage.
      //*************************************************************************
      pointer data()
      {
        return static_cast<storage_container_type*>(this)->data();
      }

      //*************************************************************************
      /// Const pointer to the beginning of the storage.
      //*************************************************************************
      ETL_CONSTEXPR const_pointer data() const
      {
        return static_cast<const storage_container_type*>(this)->data();
      }

      //*************************************************************************
      /// Iterator to the beginning of the storage.
      //*************************************************************************
      iterator begin()
      {
        return iterator(static_cast<storage_container_type*>(this)->data());
      }

      //*************************************************************************
      /// Const iterator to the beginning of the storage.
      //*************************************************************************
      ETL_CONSTEXPR const_iterator begin() const
      {
        return const_iterator(static_cast<const storage_container_type*>(this)->data());
      }

      //*************************************************************************
      /// Const iterator to the beginning of the storage.
      //*************************************************************************
      ETL_CONSTEXPR const_iterator cbegin() const
      {
        return const_iterator(static_cast<const storage_container_type*>(this)->data());
      }

      //*************************************************************************
      /// Reverse iterator to the beginning of the storage.
      //*************************************************************************
      reverse_iterator rbegin()
      {
        return reverse_iterator(static_cast<storage_container_type*>(this)->data() + Size);
      }

      //*************************************************************************
      /// Const reverse iterator to the beginning of the storage.
      //*************************************************************************
      ETL_CONSTEXPR14 const_reverse_iterator rbegin() const
      {
        return const_reverse_iterator(static_cast<const storage_container_type*>(this)->data() + Size);
      }

      //*************************************************************************
      /// Const reverse iterator to the beginning of the storage.
      //*************************************************************************
      ETL_CONSTEXPR14 const_reverse_iterator crbegin() const
      {
        return const_reverse_iterator(static_cast<const storage_container_type*>(this)->data() + Size);
      }

      //*************************************************************************
      /// Iterator to the end of the storage.
      //*************************************************************************
      iterator end()
      {
        return iterator(static_cast<storage_container_type*>(this)->data() + Size);
      }

      //*************************************************************************
      /// Const iterator to the end of the storage.
      //*************************************************************************
      ETL_CONSTEXPR const_iterator end() const
      {
        return const_iterator(static_cast<const storage_container_type*>(this)->data() + Size);
      }

      //*************************************************************************
      /// Const iterator to the end of the storage.
      //*************************************************************************
      ETL_CONSTEXPR const_iterator cend() const
      {
        return const_iterator(static_cast<const storage_container_type*>(this)->data() + Size);
      }

      //*************************************************************************
      /// Reverse iterator to the end of the storage.
      //*************************************************************************
      reverse_iterator rend()
      {
        return reverse_iterator(static_cast<storage_container_type*>(this)->data());
      }

      //*************************************************************************
      /// Const reverse iterator to the end of the storage.
      //*************************************************************************
      ETL_CONSTEXPR14 const_reverse_iterator rend() const
      {
        return const_reverse_iterator(static_cast<const storage_container_type*>(this)->data());
      }

      //*************************************************************************
      /// Const reverse iterator to the end of the storage.
      //*************************************************************************
      ETL_CONSTEXPR14 const_reverse_iterator crend() const
      {
        return const_reverse_iterator(static_cast<const storage_container_type*>(this)->data());
      }

      //*************************************************************************
      /// Index operator.
      //*************************************************************************
      storage_value_type& operator[](int i)
      {
        return static_cast<storage_container_type*>(this)[i];
      }

      //*************************************************************************
      /// Const index operator.
      //*************************************************************************
      ETL_CONSTEXPR const storage_value_type& operator[](int i) const
      {
        return static_cast<const storage_container_type*>(this)[i];
      }
    };

    template <size_t Size_, typename StorageContainer_>
    ETL_CONSTANT size_t unaligned_type_common<Size_, StorageContainer_>::Size;
  }

  //*************************************************************************
  /// unaligned_type_base
  ///\brief Allows an arithmetic type to be stored at an unaligned address.
  ///\tparam T      The arithmetic type.
  ///\tparam Endian The endianness of the arithmetic type.
  ///\tparam StorageContainer Specialization for internal or external storage via CRTP
  //*************************************************************************
  template <typename T, int Endian_, typename StorageContainer>
  class unaligned_type_base
  : public private_unaligned_type::unaligned_type_common<sizeof(T), StorageContainer>
  {
  public:

    ETL_STATIC_ASSERT(etl::is_integral<T>::value || etl::is_floating_point<T>::value, "Unaligned type must be integral or floating point");

    typedef T                                                                          value_type;
    typedef typename etl::remove_cv<T>::type                                           base_value_type;
    typedef private_unaligned_type::unaligned_type_common<sizeof(T), StorageContainer> base_type;
    typedef StorageContainer                                                           storage_container_type;

    typedef typename base_type::storage_value_type     storage_value_type;
    typedef typename base_type::pointer                pointer;
    typedef typename base_type::const_pointer          const_pointer;
    typedef typename base_type::iterator               iterator;
    typedef typename base_type::const_iterator         const_iterator;
    typedef typename base_type::reverse_iterator       reverse_iterator;
    typedef typename base_type::const_reverse_iterator const_reverse_iterator;

    static ETL_CONSTANT int Endian = Endian_;
    static ETL_CONSTANT size_t Size = private_unaligned_type::unaligned_type_common<sizeof(T), StorageContainer>::Size;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    ETL_CONSTEXPR unaligned_type_base()
    {
    }

    //*************************************************************************
    /// Assignment operator
    //*************************************************************************
    ETL_CONSTEXPR14 unaligned_type_base& operator =(T value)
    {
      unaligned_copy<T>::copy(value, static_cast<storage_container_type*>(this)->data());

      return *this;
    }

    //*************************************************************************
    /// Assignment operator from other endianness.
    //*************************************************************************
    template <int Endian_Other, typename StorageContainer_Other>
    ETL_CONSTEXPR14 unaligned_type_base& operator =(const unaligned_type_base<T, Endian_Other, StorageContainer_Other>& other)
    {
      unaligned_copy<T>::copy(other.data(), Endian_Other, static_cast<storage_container_type*>(this)->data());

      return *this;
    }

    //*************************************************************************
    /// Conversion operator
    //*************************************************************************
    ETL_CONSTEXPR14 operator T() const
    {
      base_value_type value = base_value_type();

      unaligned_copy<T>::copy(static_cast<const storage_container_type*>(this)->data(), value);

      return value;
    }

    //*************************************************************************
    /// Get the value.
    //*************************************************************************
    ETL_CONSTEXPR14 T value() const
    {
      base_value_type value = base_value_type();

      unaligned_copy<T>::copy(static_cast<const storage_container_type*>(this)->data(), value);

      return value;
    }

    //*************************************************************************
    /// Unaligned copy
    //*************************************************************************
    template <typename U, size_t Size = sizeof(U), typename Enable = void>
    struct unaligned_copy;

    //*******************************************
    /// Unaligned copy
    /// Size == 1
    //*******************************************
    template <typename U>
    struct unaligned_copy<U, 1U, void>
    {
      //*******************************
      static ETL_CONSTEXPR14 void copy(T value, pointer store)
      {
        store[0] = static_cast<storage_value_type>(value);
      }

      //*******************************
      static ETL_CONSTEXPR14 void copy(const_pointer store, base_value_type& value)
      {
        value = static_cast<T>(store[0]);
      }

      //*******************************
      static ETL_CONSTEXPR14 void copy(const_pointer src, int /*endian_src*/, unsigned char* dst)
      {
        dst[0] = src[0];
      }
    };

    //*******************************************
    /// Unaligned copy
    /// Size == 2
    //*******************************************
    template <typename U>
    struct unaligned_copy<U, 2U, typename etl::enable_if<etl::is_integral<U>::value>::type>
    {
      //*******************************
      static ETL_CONSTEXPR14 void copy(T value, unsigned char* store)
      {
        if (Endian == etl::endianness::value())
        {
          store[0] = static_cast<storage_value_type>(value);
          store[1] = static_cast<storage_value_type>(value >> (1U * CHAR_BIT));
        }
        else
        {
          store[1] = static_cast<storage_value_type>(value);
          store[0] = static_cast<storage_value_type>(value >> (1U * CHAR_BIT));
        }
      }

      //*******************************
      static ETL_CONSTEXPR14 void copy(const_pointer store, base_value_type& value)
      {
        if (Endian == etl::endianness::value())
        {
          value = static_cast<T>(static_cast<unsigned char>(store[0]));
          value |= static_cast<T>(static_cast<unsigned char>(store[1])) << (1U * CHAR_BIT);
        }
        else
        {
          value = static_cast<T>(static_cast<unsigned char>(store[1]));
          value |= static_cast<T>(static_cast<unsigned char>(store[0])) << (1U * CHAR_BIT);
        }
      }

      //*******************************
      static ETL_CONSTEXPR14 void copy(const_pointer src, int endian_src, unsigned char* dst)
      {
        if (Endian == endian_src)
        {
          dst[0] = src[0];
          dst[1] = src[1];
        }
        else
        {
          dst[0] = src[1];
          dst[1] = src[0];
        }
      }
    };

    //*******************************************
    /// Unaligned copy
    /// Size == 4
    /// Integrals
    //*******************************************
    template <typename U>
    struct unaligned_copy<U, 4U, typename etl::enable_if<etl::is_integral<U>::value>::type>
    {
      static ETL_CONSTEXPR14 void copy(T value, unsigned char* store)
      {
        if (Endian == etl::endianness::value())
        {
          store[0] = static_cast<storage_value_type>(value);
          store[1] = static_cast<storage_value_type>(value >> (1U * CHAR_BIT));
          store[2] = static_cast<storage_value_type>(value >> (2U * CHAR_BIT));
          store[3] = static_cast<storage_value_type>(value >> (3U * CHAR_BIT));
        }
        else
        {
          store[3] = static_cast<storage_value_type>(value);
          store[2] = static_cast<storage_value_type>(value >> (1U * CHAR_BIT));
          store[1] = static_cast<storage_value_type>(value >> (2U * CHAR_BIT));
          store[0] = static_cast<storage_value_type>(value >> (3U * CHAR_BIT));
        }
      }

      //*******************************
      static ETL_CONSTEXPR14 void copy(const_pointer store, base_value_type& value)
      {
        if (Endian == etl::endianness::value())
        {
          value =  static_cast<T>(static_cast<unsigned char>(store[0]));
          value |= static_cast<T>(static_cast<unsigned char>(store[1])) << (1U * CHAR_BIT);
          value |= static_cast<T>(static_cast<unsigned char>(store[2])) << (2U * CHAR_BIT);
          value |= static_cast<T>(static_cast<unsigned char>(store[3])) << (3U * CHAR_BIT);
        }
        else
        {
          value =  static_cast<T>(static_cast<unsigned char>(store[3]));
          value |= static_cast<T>(static_cast<unsigned char>(store[2])) << (1U * CHAR_BIT);
          value |= static_cast<T>(static_cast<unsigned char>(store[1])) << (2U * CHAR_BIT);
          value |= static_cast<T>(static_cast<unsigned char>(store[0])) << (3U * CHAR_BIT);
        }
      }

      //*******************************
      static ETL_CONSTEXPR14 void copy(const_pointer src, int endian_src, unsigned char* dst)
      {
        if (Endian == endian_src)
        {
          dst[0] = src[0];
          dst[1] = src[1];
          dst[2] = src[2];
          dst[3] = src[3];
        }
        else
        {
          dst[0] = src[3];
          dst[1] = src[2];
          dst[2] = src[1];
          dst[3] = src[0];
        }
      }
    };

    //*******************************************
    /// Unaligned copy
    /// Size == 4
    /// Floating point
    //*******************************************
    template <typename U>
    struct unaligned_copy<U, 4U, typename etl::enable_if<etl::is_floating_point<U>::value>::type>
    {
      static void copy(T value, unsigned char* store)
      {
        memcpy(store, &value, 4U);

        if (Endian != etl::endianness::value())
        {
          etl::reverse(store, store + 4U);
        }
      }

      //*******************************
      static void copy(const_pointer store, base_value_type& value)
      {
        unsigned char temp[4U];
        memcpy(temp, store, 4U);

        if (Endian != etl::endianness::value())
        {
          etl::reverse(temp, temp + 4U);
        }

        memcpy(&value, temp, 4U);
      }

      //*******************************
      static ETL_CONSTEXPR14 void copy(const_pointer src, int endian_src, unsigned char* dst)
      {
        memcpy(dst, src, 4U);

        if (Endian != endian_src)
        {
          etl::reverse(dst, dst + 4U);
        }
      }
    };

    //*******************************************
    /// Unaligned copy
    /// Size == 8
    /// Integrals
    //*******************************************
    template <typename U>
    struct unaligned_copy<U, 8U, typename etl::enable_if<etl::is_integral<U>::value>::type>
    {
      static ETL_CONSTEXPR14 void copy(T value, unsigned char* store)
      {
        if (Endian == etl::endianness::value())
        {
          store[0] = static_cast<storage_value_type>(value);
          store[1] = static_cast<storage_value_type>(value >> (1U * CHAR_BIT));
          store[2] = static_cast<storage_value_type>(value >> (2U * CHAR_BIT));
          store[3] = static_cast<storage_value_type>(value >> (3U * CHAR_BIT));
          store[4] = static_cast<storage_value_type>(value >> (4U * CHAR_BIT));
          store[5] = static_cast<storage_value_type>(value >> (5U * CHAR_BIT));
          store[6] = static_cast<storage_value_type>(value >> (6U * CHAR_BIT));
          store[7] = static_cast<storage_value_type>(value >> (7U * CHAR_BIT));
        }
        else
        {
          store[7] = static_cast<storage_value_type>(value);
          store[6] = static_cast<storage_value_type>(value >> (1U * CHAR_BIT));
          store[5] = static_cast<storage_value_type>(value >> (2U * CHAR_BIT));
          store[4] = static_cast<storage_value_type>(value >> (3U * CHAR_BIT));
          store[3] = static_cast<storage_value_type>(value >> (4U * CHAR_BIT));
          store[2] = static_cast<storage_value_type>(value >> (5U * CHAR_BIT));
          store[1] = static_cast<storage_value_type>(value >> (6U * CHAR_BIT));
          store[0] = static_cast<storage_value_type>(value >> (7U * CHAR_BIT));
        }
      }

      //*******************************
      static ETL_CONSTEXPR14 void copy(const_pointer store, base_value_type& value)
      {
        if (Endian == etl::endianness::value())
        {
          value = static_cast<T>(static_cast<unsigned char>(store[0]));
          value |= static_cast<T>(static_cast<unsigned char>(store[1])) << (1U * CHAR_BIT);
          value |= static_cast<T>(static_cast<unsigned char>(store[2])) << (2U * CHAR_BIT);
          value |= static_cast<T>(static_cast<unsigned char>(store[3])) << (3U * CHAR_BIT);
          value |= static_cast<T>(static_cast<unsigned char>(store[4])) << (4U * CHAR_BIT);
          value |= static_cast<T>(static_cast<unsigned char>(store[5])) << (5U * CHAR_BIT);
          value |= static_cast<T>(static_cast<unsigned char>(store[6])) << (6U * CHAR_BIT);
          value |= static_cast<T>(static_cast<unsigned char>(store[7])) << (7U * CHAR_BIT);
        }
        else
        {
          value = static_cast<T>(static_cast<unsigned char>(store[7]));
          value |= static_cast<T>(static_cast<unsigned char>(store[6])) << (1U * CHAR_BIT);
          value |= static_cast<T>(static_cast<unsigned char>(store[5])) << (2U * CHAR_BIT);
          value |= static_cast<T>(static_cast<unsigned char>(store[4])) << (3U * CHAR_BIT);
          value |= static_cast<T>(static_cast<unsigned char>(store[3])) << (4U * CHAR_BIT);
          value |= static_cast<T>(static_cast<unsigned char>(store[2])) << (5U * CHAR_BIT);
          value |= static_cast<T>(static_cast<unsigned char>(store[1])) << (6U * CHAR_BIT);
          value |= static_cast<T>(static_cast<unsigned char>(store[0])) << (7U * CHAR_BIT);
        }
      }

      //*******************************
      static ETL_CONSTEXPR14 void copy(const_pointer src, int endian_src, unsigned char* dst)
      {
        if (Endian == endian_src)
        {
          dst[0] = src[0];
          dst[1] = src[1];
          dst[2] = src[2];
          dst[3] = src[3];
          dst[4] = src[4];
          dst[5] = src[5];
          dst[6] = src[6];
          dst[7] = src[7];
        }
        else
        {
          dst[0] = src[7];
          dst[1] = src[6];
          dst[2] = src[5];
          dst[3] = src[4];
          dst[4] = src[3];
          dst[5] = src[2];
          dst[6] = src[1];
          dst[7] = src[0];
        }
      }
    };

    //*******************************************
    /// Unaligned copy
    /// Size == 8
    /// Floating point
    //*******************************************
    template <typename U>
    struct unaligned_copy<U, 8U, typename etl::enable_if<etl::is_floating_point<U>::value>::type>
    {
      static void copy(T value, unsigned char* store)
      {
        memcpy(store, &value, 8U);

        if (Endian != etl::endianness::value())
        {
          etl::reverse(store, store + 8U);
        }
      }

      //*******************************
      static void copy(const_pointer store, base_value_type& value)
      {
        unsigned char temp[8U];
        memcpy(temp, store, 8U);

        if (Endian != etl::endianness::value())
        {
          etl::reverse(temp, temp + 8U);
        }

        memcpy(&value, temp, 8U);
      }

      //*******************************
      static ETL_CONSTEXPR14 void copy(const_pointer src, int endian_src, unsigned char* dst)
      {
        memcpy(dst, src, 8U);

        if (Endian != endian_src)
        {
          etl::reverse(dst, dst + 8U);
        }
      }
    };

    //*******************************************
    /// Unaligned copy
    /// Size == 12
    /// Floating point
    //*******************************************
    template <typename U>
    struct unaligned_copy<U, 12U, typename etl::enable_if<etl::is_floating_point<U>::value>::type>
    {
      static void copy(T value, unsigned char* store)
      {
        memcpy(store, &value, 12U);

        if (Endian != etl::endianness::value())
        {
          etl::reverse(store, store + 12U);
        }
      }

      //*******************************
      static void copy(const_pointer store, base_value_type& value)
      {
        unsigned char temp[12U];
        memcpy(temp, store, 12U);

        if (Endian != etl::endianness::value())
        {
          etl::reverse(temp, temp + 12U);
        }

        memcpy(&value, temp, 12U);
      }

      //*******************************
      static ETL_CONSTEXPR14 void copy(const_pointer src, int endian_src, unsigned char* dst)
      {
        memcpy(dst, src, 12U);

        if (Endian != endian_src)
        {
          etl::reverse(dst, dst + 12U);
        }
      }
    };

    //*******************************************
    /// Unaligned copy
    /// Size == 16
    /// Floating point
    //*******************************************
    template <typename U>
    struct unaligned_copy<U, 16U, typename etl::enable_if<etl::is_floating_point<U>::value>::type>
    {
      static void copy(T value, unsigned char* store)
      {
        memcpy(store, &value, 16U);

        if (Endian != etl::endianness::value())
        {
          etl::reverse(store, store + 16U);
        }
      }

      //*******************************
      static void copy(const_pointer store, base_value_type& value)
      {
        unsigned char temp[16U];
        memcpy(temp, store, 16U);

        if (Endian != etl::endianness::value())
        {
          etl::reverse(temp, temp + 16U);
        }

        memcpy(&value, temp, 16U);
      }

      //*******************************
      static ETL_CONSTEXPR14 void copy(const_pointer src, int endian_src, unsigned char* dst)
      {
        memcpy(dst, src, 16U);

        if (Endian != endian_src)
        {
          etl::reverse(dst, dst + 16U);
        }
      }
    };
  };

  template <typename T, int Endian_, typename StorageContainer>
  ETL_CONSTANT int unaligned_type_base<T, Endian_, StorageContainer>::Endian;
    
  template <typename T, int Endian_, typename StorageContainer>
  ETL_CONSTANT size_t unaligned_type_base<T, Endian_, StorageContainer>::Size;

  //*************************************************************************
  /// unaligned_type
  ///\brief Allows an arithmetic type to be stored at an unaligned address.
  ///\tparam T      The arithmetic type.
  ///\tparam Endian The endianness of the arithmetic type.
  //*************************************************************************
  template<typename T, int Endian_>
  class unaligned_type
  : public unaligned_type_base<T, Endian_, unaligned_type<T, Endian_> >
  {
  public:

    typedef unaligned_type_base<T, Endian_, unaligned_type<T, Endian_> > base_type;
    typedef unsigned char                                                storage_value_type;

    static ETL_CONSTANT size_t Size = sizeof(T);

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    ETL_CONSTEXPR unaligned_type()
    : storage()
    {
    }

    //*************************************************************************
    /// Construct from a value.
    //*************************************************************************
    ETL_CONSTEXPR14 unaligned_type(T value)
    : storage()
    {
      base_type::template unaligned_copy<T>::copy(value, storage);
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    template <int Endian_Other, typename StorageContainer_Other>
    ETL_CONSTEXPR14 unaligned_type(const unaligned_type_base<T, Endian_Other, StorageContainer_Other>& other)
    : storage()
    {
      base_type::template unaligned_copy<T>::copy(other.data(), Endian_Other, storage);
    }

    //*************************************************************************
    /// Assignment from value.
    //*************************************************************************
    ETL_CONSTEXPR14 unaligned_type& operator =(T value)
    {
      base_type::template unaligned_copy<T>::copy(value, storage);

      return *this;
    }

    //*************************************************************************
    /// Copy assignment.
    //*************************************************************************
    template <int Endian_Other>
    ETL_CONSTEXPR14 unaligned_type& operator =(const unaligned_type<T, Endian_Other>& other)
    {
      base_type::template unaligned_copy<T>::copy(other.data(), Endian_Other, storage);

      return *this;
    }

    //*************************************************************************
    /// Const storage access.
    //*************************************************************************
    ETL_CONSTEXPR14 const storage_value_type* data() const
    {
      return storage;
    }

    //*************************************************************************
    /// Storage access.
    //*************************************************************************
    ETL_CONSTEXPR14 storage_value_type* data()
    {
      return storage;
    }

    //*************************************************************************
    /// Const index operator.
    //*************************************************************************
    ETL_CONSTEXPR const storage_value_type& operator[](int i) const
    {
      return storage[i];
    }

    //*************************************************************************
    /// Index operator.
    //*************************************************************************
    storage_value_type& operator[](int i)
    {
      return storage[i];
    }

  private:

    storage_value_type storage[sizeof(T)];
  };

  template <typename T, int Endian_>
  ETL_CONSTANT size_t unaligned_type<T, Endian_>::Size;

  //*************************************************************************
  /// Automatic const or non-const choice to support const and non-const storage
  //*************************************************************************
  template<typename T>
  struct storage_type
  {
    typedef typename etl::conditional<etl::is_const<T>::value, const unsigned char, unsigned char>::type type;
  };

  //*************************************************************************
  /// unaligned_type
  ///\brief Allows an arithmetic type to be stored at an unaligned external address.
  ///\note No initialization of pointed-to data is being done.
  ///      This needs to be done by application code.
  ///\tparam T      The arithmetic type.
  ///\tparam Endian The endianness of the arithmetic type.
  //*************************************************************************
  template<typename T, int Endian_>
  class unaligned_type_ext
  : public unaligned_type_base<T, Endian_, unaligned_type_ext<T, Endian_> >
  {
  public:

    typedef unaligned_type_base<T, Endian_, unaligned_type_ext<T, Endian_> > base_type;
    typedef typename storage_type<T>::type                                   storage_value_type;

    static ETL_CONSTANT size_t Size = sizeof(T);

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    ETL_CONSTEXPR unaligned_type_ext()
    : storage(ETL_NULLPTR)
    {
    }

    //*************************************************************************
    /// Constructor from const pointer.
    //*************************************************************************
    ETL_CONSTEXPR14 explicit unaligned_type_ext(const void* address, size_t buffer_size = sizeof(T))
    : storage(static_cast<const storage_value_type*>(address))
    {
      ETL_ASSERT(sizeof(T) <= buffer_size, ETL_ERROR(etl::unaligned_type_buffer_size));
    }

    //*************************************************************************
    /// Constructor from pointer.
    //*************************************************************************
    ETL_CONSTEXPR14 explicit unaligned_type_ext(void* address, size_t buffer_size = sizeof(T))
    : storage(static_cast<storage_value_type*>(address))
    {
      ETL_ASSERT(sizeof(T) <= buffer_size, ETL_ERROR(etl::unaligned_type_buffer_size));
    }

    //*************************************************************************
    /// Assignment from value.
    //*************************************************************************
    ETL_CONSTEXPR14 unaligned_type_ext& operator =(T value)
    {
      base_type::template unaligned_copy<T>::copy(value, storage);

      return *this;
    }

    //*************************************************************************
    /// Set external storage to new location with optional size limit of buffer.
    ///\param address     New external buffer address
    ///\param buffer_size Maximum external buffer size. Guarded by static assert.
    //*************************************************************************
    void set_storage(void* address, size_t buffer_size = sizeof(T))
    {
      ETL_ASSERT(sizeof(T) <= buffer_size, ETL_ERROR(etl::unaligned_type_buffer_size));

      storage = static_cast<storage_value_type*>(address);
    }

    //*************************************************************************
    /// Set external storage to new location with compile time size limit of buffer.
    ///\tparam BufferSize Maximum external buffer size. Guarded by static assert.
    ///\param address     New external buffer address
    //*************************************************************************
    template <size_t BufferSize>
    void set_storage(void* address)
    {
      ETL_STATIC_ASSERT(sizeof(T) <= BufferSize, "Buffer size to small for type");

      storage = static_cast<storage_value_type*>(address);
    }

    //*************************************************************************
    /// Set external storage to new location with optional size limit of buffer.
    /// Overload for const buffer.
    ///\param address     New external buffer address
    ///\param buffer_size Maximum external buffer size. Guarded by static assert.
    //*************************************************************************
    void set_storage(const void* address, size_t buffer_size = sizeof(T))
    {
      ETL_ASSERT(sizeof(T) <= buffer_size, ETL_ERROR(etl::unaligned_type_buffer_size));

      storage = static_cast<const storage_value_type*>(address);
    }

    //*************************************************************************
    /// Set external storage to new location with compile time size limit of buffer.
    /// Overload for const buffer.
    ///\tparam BufferSize Maximum external buffer size. Guarded by static assert.
    ///\param address     New external buffer address
    //*************************************************************************
    template <size_t BufferSize>
    void set_storage(const void* address)
    {
      ETL_STATIC_ASSERT(sizeof(T) <= BufferSize, "Buffer size to small for type");

      storage = static_cast<const storage_value_type*>(address);
    }

    //*************************************************************************
    /// Const storage access.
    //*************************************************************************
    ETL_CONSTEXPR14 const storage_value_type* data() const
    {
      return storage;
    }

    //*************************************************************************
    /// Storage access.
    //*************************************************************************
    ETL_CONSTEXPR14 storage_value_type* data()
    {
      return storage;
    }

    //*************************************************************************
    /// Const index operator.
    //*************************************************************************
    ETL_CONSTEXPR const storage_value_type& operator[](int i) const
    {
      return storage[i];
    }

    //*************************************************************************
    /// Index operator.
    //*************************************************************************
    storage_value_type& operator[](int i)
    {
      return storage[i];
    }

  private:

    storage_value_type* storage;
  };

  template <typename T, int Endian_>
  ETL_CONSTANT size_t unaligned_type_ext<T, Endian_>::Size;

  //*************************************************************************
  /// Convenience types with internal storage
  //*************************************************************************
#if ETL_HAS_CONSTEXPR_ENDIANNESS
  // Host order
  typedef unaligned_type<char,               etl::endianness::value()> host_char_t;
  typedef unaligned_type<signed char,        etl::endianness::value()> host_schar_t;
  typedef unaligned_type<unsigned char,      etl::endianness::value()> host_uchar_t;
  typedef unaligned_type<short,              etl::endianness::value()> host_short_t;
  typedef unaligned_type<unsigned short,     etl::endianness::value()> host_ushort_t;
  typedef unaligned_type<int,                etl::endianness::value()> host_int_t;
  typedef unaligned_type<unsigned int,       etl::endianness::value()> host_uint_t;
  typedef unaligned_type<long,               etl::endianness::value()> host_long_t;
  typedef unaligned_type<unsigned long,      etl::endianness::value()> host_ulong_t;
  typedef unaligned_type<long long,          etl::endianness::value()> host_long_long_t;
  typedef unaligned_type<unsigned long long, etl::endianness::value()> host_ulong_long_t;
#if ETL_USING_8BIT_TYPES
  typedef unaligned_type<int8_t,             etl::endianness::value()> host_int8_t;
  typedef unaligned_type<uint8_t,            etl::endianness::value()> host_uint8_t;
#endif
  typedef unaligned_type<int16_t,            etl::endianness::value()> host_int16_t;
  typedef unaligned_type<uint16_t,           etl::endianness::value()> host_uint16_t;
  typedef unaligned_type<int32_t,            etl::endianness::value()> host_int32_t;
  typedef unaligned_type<uint32_t,           etl::endianness::value()> host_uint32_t;
#if ETL_USING_64BIT_TYPES
  typedef unaligned_type<int64_t,            etl::endianness::value()> host_int64_t;
  typedef unaligned_type<uint64_t,           etl::endianness::value()> host_uint64_t;
#endif
  typedef unaligned_type<float,              etl::endianness::value()> host_float_t;
  typedef unaligned_type<double,             etl::endianness::value()> host_double_t;
  typedef unaligned_type<long double,        etl::endianness::value()> host_long_double_t;
#endif

  // Little Endian
  typedef unaligned_type<char,               etl::endian::little> le_char_t;
  typedef unaligned_type<signed char,        etl::endian::little> le_schar_t;
  typedef unaligned_type<unsigned char,      etl::endian::little> le_uchar_t;
  typedef unaligned_type<short,              etl::endian::little> le_short_t;
  typedef unaligned_type<unsigned short,     etl::endian::little> le_ushort_t;
  typedef unaligned_type<int,                etl::endian::little> le_int_t;
  typedef unaligned_type<unsigned int,       etl::endian::little> le_uint_t;
  typedef unaligned_type<long,               etl::endian::little> le_long_t;
  typedef unaligned_type<unsigned long,      etl::endian::little> le_ulong_t;
  typedef unaligned_type<long long,          etl::endian::little> le_long_long_t;
  typedef unaligned_type<unsigned long long, etl::endian::little> le_ulong_long_t;
#if ETL_USING_8BIT_TYPES
  typedef unaligned_type<int8_t,             etl::endian::little> le_int8_t;
  typedef unaligned_type<uint8_t,            etl::endian::little> le_uint8_t;
#endif
  typedef unaligned_type<int16_t,            etl::endian::little> le_int16_t;
  typedef unaligned_type<uint16_t,           etl::endian::little> le_uint16_t;
  typedef unaligned_type<int32_t,            etl::endian::little> le_int32_t;
  typedef unaligned_type<uint32_t,           etl::endian::little> le_uint32_t;
#if ETL_USING_64BIT_TYPES
  typedef unaligned_type<int64_t,            etl::endian::little> le_int64_t;
  typedef unaligned_type<uint64_t,           etl::endian::little> le_uint64_t;
#endif
  typedef unaligned_type<float,              etl::endian::little> le_float_t;
  typedef unaligned_type<double,             etl::endian::little> le_double_t;
  typedef unaligned_type<long double,        etl::endian::little> le_long_double_t;

  // Big Endian
  typedef unaligned_type<char,               etl::endian::big> be_char_t;
  typedef unaligned_type<signed char,        etl::endian::big> be_schar_t;
  typedef unaligned_type<unsigned char,      etl::endian::big> be_uchar_t;
  typedef unaligned_type<short,              etl::endian::big> be_short_t;
  typedef unaligned_type<unsigned short,     etl::endian::big> be_ushort_t;
  typedef unaligned_type<int,                etl::endian::big> be_int_t;
  typedef unaligned_type<unsigned int,       etl::endian::big> be_uint_t;
  typedef unaligned_type<long,               etl::endian::big> be_long_t;
  typedef unaligned_type<unsigned long,      etl::endian::big> be_ulong_t;
  typedef unaligned_type<long long,          etl::endian::big> be_long_long_t;
  typedef unaligned_type<unsigned long long, etl::endian::big> be_ulong_long_t;
#if ETL_USING_8BIT_TYPES
  typedef unaligned_type<int8_t,             etl::endian::big> be_int8_t;
  typedef unaligned_type<uint8_t,            etl::endian::big> be_uint8_t;
#endif
  typedef unaligned_type<int16_t,            etl::endian::big> be_int16_t;
  typedef unaligned_type<uint16_t,           etl::endian::big> be_uint16_t;
  typedef unaligned_type<int32_t,            etl::endian::big> be_int32_t;
  typedef unaligned_type<uint32_t,           etl::endian::big> be_uint32_t;
#if ETL_USING_64BIT_TYPES
  typedef unaligned_type<int64_t,            etl::endian::big> be_int64_t;
  typedef unaligned_type<uint64_t,           etl::endian::big> be_uint64_t;
#endif
  typedef unaligned_type<float,              etl::endian::big> be_float_t;
  typedef unaligned_type<double,             etl::endian::big> be_double_t;
  typedef unaligned_type<long double,        etl::endian::big> be_long_double_t;

  // Network Order
  typedef be_char_t        net_char_t;
  typedef be_schar_t       net_schar_t;
  typedef be_uchar_t       net_uchar_t;
  typedef be_short_t       net_short_t;
  typedef be_ushort_t      net_ushort_t;
  typedef be_int_t         net_int_t;
  typedef be_uint_t        net_uint_t;
  typedef be_long_t        net_long_t;
  typedef be_ulong_t       net_ulong_t;
  typedef be_long_long_t   net_long_long_t;
  typedef be_ulong_long_t  net_ulong_long_t;
#if ETL_USING_8BIT_TYPES
  typedef be_int8_t        net_int8_t;
  typedef be_uint8_t       net_uint8_t;
#endif
  typedef be_int16_t       net_int16_t;
  typedef be_uint16_t      net_uint16_t;
  typedef be_int32_t       net_int32_t;
  typedef be_uint32_t      net_uint32_t;
#if ETL_USING_64BIT_TYPES
  typedef be_int64_t       net_int64_t;
  typedef be_uint64_t      net_uint64_t;
#endif
  typedef be_float_t       net_float_t;
  typedef be_double_t      net_double_t;
  typedef be_long_double_t net_long_double_t;

#if ETL_USING_CPP11
  template <typename T, int Endian>
  using unaligned_type_t = typename etl::unaligned_type<T, Endian>::type;
#endif

#if ETL_USING_CPP17
  template <typename T, int Endian>
  constexpr size_t unaligned_type_v = etl::unaligned_type<T, Endian>::Size;
#endif

  //*************************************************************************
  /// Convenience types with external storage
  //*************************************************************************
#if ETL_HAS_CONSTEXPR_ENDIANNESS
  // Host order
  typedef unaligned_type_ext<char,               etl::endianness::value()> host_char_t_ext;
  typedef unaligned_type_ext<signed char,        etl::endianness::value()> host_schar_t_ext;
  typedef unaligned_type_ext<unsigned char,      etl::endianness::value()> host_uchar_t_ext;
  typedef unaligned_type_ext<short,              etl::endianness::value()> host_short_t_ext;
  typedef unaligned_type_ext<unsigned short,     etl::endianness::value()> host_ushort_t_ext;
  typedef unaligned_type_ext<int,                etl::endianness::value()> host_int_t_ext;
  typedef unaligned_type_ext<unsigned int,       etl::endianness::value()> host_uint_t_ext;
  typedef unaligned_type_ext<long,               etl::endianness::value()> host_long_t_ext;
  typedef unaligned_type_ext<unsigned long,      etl::endianness::value()> host_ulong_t_ext;
  typedef unaligned_type_ext<long long,          etl::endianness::value()> host_long_long_t_ext;
  typedef unaligned_type_ext<unsigned long long, etl::endianness::value()> host_ulong_long_t_ext;
#if ETL_USING_8BIT_TYPES
  typedef unaligned_type_ext<int8_t,             etl::endianness::value()> host_int8_t_ext;
  typedef unaligned_type_ext<uint8_t,            etl::endianness::value()> host_uint8_t_ext;
#endif
  typedef unaligned_type_ext<int16_t,            etl::endianness::value()> host_int16_t_ext;
  typedef unaligned_type_ext<uint16_t,           etl::endianness::value()> host_uint16_t_ext;
  typedef unaligned_type_ext<int32_t,            etl::endianness::value()> host_int32_t_ext;
  typedef unaligned_type_ext<uint32_t,           etl::endianness::value()> host_uint32_t_ext;
#if ETL_USING_64BIT_TYPES
  typedef unaligned_type_ext<int64_t,            etl::endianness::value()> host_int64_t_ext;
  typedef unaligned_type_ext<uint64_t,           etl::endianness::value()> host_uint64_t_ext;
#endif
  typedef unaligned_type_ext<float,              etl::endianness::value()> host_float_t_ext;
  typedef unaligned_type_ext<double,             etl::endianness::value()> host_double_t_ext;
  typedef unaligned_type_ext<long double,        etl::endianness::value()> host_long_double_t_ext;
#endif

  // Little Endian
  typedef unaligned_type_ext<char,               etl::endian::little> le_char_t_ext;
  typedef unaligned_type_ext<signed char,        etl::endian::little> le_schar_t_ext;
  typedef unaligned_type_ext<unsigned char,      etl::endian::little> le_uchar_t_ext;
  typedef unaligned_type_ext<short,              etl::endian::little> le_short_t_ext;
  typedef unaligned_type_ext<unsigned short,     etl::endian::little> le_ushort_t_ext;
  typedef unaligned_type_ext<int,                etl::endian::little> le_int_t_ext;
  typedef unaligned_type_ext<unsigned int,       etl::endian::little> le_uint_t_ext;
  typedef unaligned_type_ext<long,               etl::endian::little> le_long_t_ext;
  typedef unaligned_type_ext<unsigned long,      etl::endian::little> le_ulong_t_ext;
  typedef unaligned_type_ext<long long,          etl::endian::little> le_long_long_t_ext;
  typedef unaligned_type_ext<unsigned long long, etl::endian::little> le_ulong_long_t_ext;
#if ETL_USING_8BIT_TYPES
  typedef unaligned_type_ext<int8_t,             etl::endian::little> le_int8_t_ext;
  typedef unaligned_type_ext<uint8_t,            etl::endian::little> le_uint8_t_ext;
#endif
  typedef unaligned_type_ext<int16_t,            etl::endian::little> le_int16_t_ext;
  typedef unaligned_type_ext<uint16_t,           etl::endian::little> le_uint16_t_ext;
  typedef unaligned_type_ext<int32_t,            etl::endian::little> le_int32_t_ext;
  typedef unaligned_type_ext<uint32_t,           etl::endian::little> le_uint32_t_ext;
#if ETL_USING_64BIT_TYPES
  typedef unaligned_type_ext<int64_t,            etl::endian::little> le_int64_t_ext;
  typedef unaligned_type_ext<uint64_t,           etl::endian::little> le_uint64_t_ext;
#endif
  typedef unaligned_type_ext<float,              etl::endian::little> le_float_t_ext;
  typedef unaligned_type_ext<double,             etl::endian::little> le_double_t_ext;
  typedef unaligned_type_ext<long double,        etl::endian::little> le_long_double_t_ext;

  // Big Endian
  typedef unaligned_type_ext<char,               etl::endian::big> be_char_t_ext;
  typedef unaligned_type_ext<signed char,        etl::endian::big> be_schar_t_ext;
  typedef unaligned_type_ext<unsigned char,      etl::endian::big> be_uchar_t_ext;
  typedef unaligned_type_ext<short,              etl::endian::big> be_short_t_ext;
  typedef unaligned_type_ext<unsigned short,     etl::endian::big> be_ushort_t_ext;
  typedef unaligned_type_ext<int,                etl::endian::big> be_int_t_ext;
  typedef unaligned_type_ext<unsigned int,       etl::endian::big> be_uint_t_ext;
  typedef unaligned_type_ext<long,               etl::endian::big> be_long_t_ext;
  typedef unaligned_type_ext<unsigned long,      etl::endian::big> be_ulong_t_ext;
  typedef unaligned_type_ext<long long,          etl::endian::big> be_long_long_t_ext;
  typedef unaligned_type_ext<unsigned long long, etl::endian::big> be_ulong_long_t_ext;
#if ETL_USING_8BIT_TYPES
  typedef unaligned_type_ext<int8_t,             etl::endian::big> be_int8_t_ext;
  typedef unaligned_type_ext<uint8_t,            etl::endian::big> be_uint8_t_ext;
#endif
  typedef unaligned_type_ext<int16_t,            etl::endian::big> be_int16_t_ext;
  typedef unaligned_type_ext<uint16_t,           etl::endian::big> be_uint16_t_ext;
  typedef unaligned_type_ext<int32_t,            etl::endian::big> be_int32_t_ext;
  typedef unaligned_type_ext<uint32_t,           etl::endian::big> be_uint32_t_ext;
#if ETL_USING_64BIT_TYPES
  typedef unaligned_type_ext<int64_t,            etl::endian::big> be_int64_t_ext;
  typedef unaligned_type_ext<uint64_t,           etl::endian::big> be_uint64_t_ext;
#endif
  typedef unaligned_type_ext<float,              etl::endian::big> be_float_t_ext;
  typedef unaligned_type_ext<double,             etl::endian::big> be_double_t_ext;
  typedef unaligned_type_ext<long double,        etl::endian::big> be_long_double_t_ext;

  // Network Order
  typedef be_char_t_ext        net_char_t_ext;
  typedef be_schar_t_ext       net_schar_t_ext;
  typedef be_uchar_t_ext       net_uchar_t_ext;
  typedef be_short_t_ext       net_short_t_ext;
  typedef be_ushort_t_ext      net_ushort_t_ext;
  typedef be_int_t_ext         net_int_t_ext;
  typedef be_uint_t_ext        net_uint_t_ext;
  typedef be_long_t_ext        net_long_t_ext;
  typedef be_ulong_t_ext       net_ulong_t_ext;
  typedef be_long_long_t_ext   net_long_long_t_ext;
  typedef be_ulong_long_t_ext  net_ulong_long_t_ext;
#if ETL_USING_8BIT_TYPES
  typedef be_int8_t_ext        net_int8_t_ext;
  typedef be_uint8_t_ext       net_uint8_t_ext;
#endif
  typedef be_int16_t_ext       net_int16_t_ext;
  typedef be_uint16_t_ext      net_uint16_t_ext;
  typedef be_int32_t_ext       net_int32_t_ext;
  typedef be_uint32_t_ext      net_uint32_t_ext;
#if ETL_USING_64BIT_TYPES
  typedef be_int64_t_ext       net_int64_t_ext;
  typedef be_uint64_t_ext      net_uint64_t_ext;
#endif
  typedef be_float_t_ext       net_float_t_ext;
  typedef be_double_t_ext      net_double_t_ext;
  typedef be_long_double_t_ext net_long_double_t_ext;

#if ETL_USING_CPP11
  template <typename T, int Endian>
  using unaligned_type_ext_t = typename etl::unaligned_type_ext<T, Endian>::type;
#endif

#if ETL_USING_CPP17
  template <typename T, int Endian>
  constexpr size_t unaligned_type_ext_v = etl::unaligned_type_ext<T, Endian>::Size;
#endif

  //*************************************************************************
  /// Convenience const types with external storage
  //*************************************************************************
#if ETL_HAS_CONSTEXPR_ENDIANNESS
  // Host order
  typedef unaligned_type_ext<const char,               etl::endianness::value()> host_const_char_t_ext;
  typedef unaligned_type_ext<const signed char,        etl::endianness::value()> host_const_schar_t_ext;
  typedef unaligned_type_ext<const unsigned char,      etl::endianness::value()> host_const_uchar_t_ext;
  typedef unaligned_type_ext<const short,              etl::endianness::value()> host_const_short_t_ext;
  typedef unaligned_type_ext<const unsigned short,     etl::endianness::value()> host_const_ushort_t_ext;
  typedef unaligned_type_ext<const int,                etl::endianness::value()> host_const_int_t_ext;
  typedef unaligned_type_ext<const unsigned int,       etl::endianness::value()> host_const_uint_t_ext;
  typedef unaligned_type_ext<const long,               etl::endianness::value()> host_const_long_t_ext;
  typedef unaligned_type_ext<const unsigned long,      etl::endianness::value()> host_const_ulong_t_ext;
  typedef unaligned_type_ext<const long long,          etl::endianness::value()> host_const_long_long_t_ext;
  typedef unaligned_type_ext<const unsigned long long, etl::endianness::value()> host_const_ulong_long_t_ext;
#if ETL_USING_8BIT_TYPES
  typedef unaligned_type_ext<const int8_t,             etl::endianness::value()> host_const_int8_t_ext;
  typedef unaligned_type_ext<const uint8_t,            etl::endianness::value()> host_const_uint8_t_ext;
#endif
  typedef unaligned_type_ext<const int16_t,            etl::endianness::value()> host_const_int16_t_ext;
  typedef unaligned_type_ext<const uint16_t,           etl::endianness::value()> host_const_uint16_t_ext;
  typedef unaligned_type_ext<const int32_t,            etl::endianness::value()> host_const_int32_t_ext;
  typedef unaligned_type_ext<const uint32_t,           etl::endianness::value()> host_const_uint32_t_ext;
#if ETL_USING_64BIT_TYPES
  typedef unaligned_type_ext<const int64_t,            etl::endianness::value()> host_const_int64_t_ext;
  typedef unaligned_type_ext<const uint64_t,           etl::endianness::value()> host_const_uint64_t_ext;
#endif
  typedef unaligned_type_ext<const float,              etl::endianness::value()> host_const_float_t_ext;
  typedef unaligned_type_ext<const double,             etl::endianness::value()> host_const_double_t_ext;
  typedef unaligned_type_ext<const long double,        etl::endianness::value()> host_const_long_double_t_ext;
#endif

  // Little Endian
  typedef unaligned_type_ext<const char,               etl::endian::little> le_const_char_t_ext;
  typedef unaligned_type_ext<const signed char,        etl::endian::little> le_const_schar_t_ext;
  typedef unaligned_type_ext<const unsigned char,      etl::endian::little> le_const_uchar_t_ext;
  typedef unaligned_type_ext<const short,              etl::endian::little> le_const_short_t_ext;
  typedef unaligned_type_ext<const unsigned short,     etl::endian::little> le_const_ushort_t_ext;
  typedef unaligned_type_ext<const int,                etl::endian::little> le_const_int_t_ext;
  typedef unaligned_type_ext<const unsigned int,       etl::endian::little> le_const_uint_t_ext;
  typedef unaligned_type_ext<const long,               etl::endian::little> le_const_long_t_ext;
  typedef unaligned_type_ext<const unsigned long,      etl::endian::little> le_const_ulong_t_ext;
  typedef unaligned_type_ext<const long long,          etl::endian::little> le_const_long_long_t_ext;
  typedef unaligned_type_ext<const unsigned long long, etl::endian::little> le_const_ulong_long_t_ext;
#if ETL_USING_8BIT_TYPES
  typedef unaligned_type_ext<const int8_t,             etl::endian::little> le_const_int8_t_ext;
  typedef unaligned_type_ext<const uint8_t,            etl::endian::little> le_const_uint8_t_ext;
#endif
  typedef unaligned_type_ext<const int16_t,            etl::endian::little> le_const_int16_t_ext;
  typedef unaligned_type_ext<const uint16_t,           etl::endian::little> le_const_uint16_t_ext;
  typedef unaligned_type_ext<const int32_t,            etl::endian::little> le_const_int32_t_ext;
  typedef unaligned_type_ext<const uint32_t,           etl::endian::little> le_const_uint32_t_ext;
#if ETL_USING_64BIT_TYPES
  typedef unaligned_type_ext<const int64_t,            etl::endian::little> le_const_int64_t_ext;
  typedef unaligned_type_ext<const uint64_t,           etl::endian::little> le_const_uint64_t_ext;
#endif
  typedef unaligned_type_ext<const float,              etl::endian::little> le_const_float_t_ext;
  typedef unaligned_type_ext<const double,             etl::endian::little> le_const_double_t_ext;
  typedef unaligned_type_ext<const long double,        etl::endian::little> le_const_long_double_t_ext;

  // Big Endian
  typedef unaligned_type_ext<const char,               etl::endian::big> be_const_char_t_ext;
  typedef unaligned_type_ext<const signed char,        etl::endian::big> be_const_schar_t_ext;
  typedef unaligned_type_ext<const unsigned char,      etl::endian::big> be_const_uchar_t_ext;
  typedef unaligned_type_ext<const short,              etl::endian::big> be_const_short_t_ext;
  typedef unaligned_type_ext<const unsigned short,     etl::endian::big> be_const_ushort_t_ext;
  typedef unaligned_type_ext<const int,                etl::endian::big> be_const_int_t_ext;
  typedef unaligned_type_ext<const unsigned int,       etl::endian::big> be_const_uint_t_ext;
  typedef unaligned_type_ext<const long,               etl::endian::big> be_const_long_t_ext;
  typedef unaligned_type_ext<const unsigned long,      etl::endian::big> be_const_ulong_t_ext;
  typedef unaligned_type_ext<const long long,          etl::endian::big> be_const_long_long_t_ext;
  typedef unaligned_type_ext<const unsigned long long, etl::endian::big> be_const_ulong_long_t_ext;
#if ETL_USING_8BIT_TYPES
  typedef unaligned_type_ext<const int8_t,             etl::endian::big> be_const_int8_t_ext;
  typedef unaligned_type_ext<const uint8_t,            etl::endian::big> be_const_uint8_t_ext;
#endif
  typedef unaligned_type_ext<const int16_t,            etl::endian::big> be_const_int16_t_ext;
  typedef unaligned_type_ext<const uint16_t,           etl::endian::big> be_const_uint16_t_ext;
  typedef unaligned_type_ext<const int32_t,            etl::endian::big> be_const_int32_t_ext;
  typedef unaligned_type_ext<const uint32_t,           etl::endian::big> be_const_uint32_t_ext;
#if ETL_USING_64BIT_TYPES
  typedef unaligned_type_ext<const int64_t,            etl::endian::big> be_const_int64_t_ext;
  typedef unaligned_type_ext<const uint64_t,           etl::endian::big> be_const_uint64_t_ext;
#endif
  typedef unaligned_type_ext<const float,              etl::endian::big> be_const_float_t_ext;
  typedef unaligned_type_ext<const double,             etl::endian::big> be_const_double_t_ext;
  typedef unaligned_type_ext<const long double,        etl::endian::big> be_const_long_double_t_ext;

  // Network Order
  typedef be_const_char_t_ext        net_const_char_t_ext;
  typedef be_const_schar_t_ext       net_const_schar_t_ext;
  typedef be_const_uchar_t_ext       net_const_uchar_t_ext;
  typedef be_const_short_t_ext       net_const_short_t_ext;
  typedef be_const_ushort_t_ext      net_const_ushort_t_ext;
  typedef be_const_int_t_ext         net_const_int_t_ext;
  typedef be_const_uint_t_ext        net_const_uint_t_ext;
  typedef be_const_long_t_ext        net_const_long_t_ext;
  typedef be_const_ulong_t_ext       net_const_ulong_t_ext;
  typedef be_const_long_long_t_ext   net_const_long_long_t_ext;
  typedef be_const_ulong_long_t_ext  net_const_ulong_long_t_ext;
#if ETL_USING_8BIT_TYPES
  typedef be_const_int8_t_ext        net_const_int8_t_ext;
  typedef be_const_uint8_t_ext       net_const_uint8_t_ext;
#endif
  typedef be_const_int16_t_ext       net_const_int16_t_ext;
  typedef be_const_uint16_t_ext      net_const_uint16_t_ext;
  typedef be_const_int32_t_ext       net_const_int32_t_ext;
  typedef be_const_uint32_t_ext      net_const_uint32_t_ext;
#if ETL_USING_64BIT_TYPES
  typedef be_const_int64_t_ext       net_const_int64_t_ext;
  typedef be_const_uint64_t_ext      net_const_uint64_t_ext;
#endif
  typedef be_const_float_t_ext       net_const_float_t_ext;
  typedef be_const_double_t_ext      net_const_double_t_ext;
  typedef be_const_long_double_t_ext net_const_long_double_t_ext;
}

#endif
