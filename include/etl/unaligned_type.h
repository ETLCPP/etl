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
#include "algorithm.h"
#include "array.h"
#include "binary.h"
#include "bit.h"
#include "endianness.h"
#include "exception.h"
#include "file_error_numbers.h"
#include "integral_limits.h"
#include "iterator.h"
#include "type_traits.h"

#include <stdint.h>
#include <string.h>

namespace etl
{
  struct unaligned_type_exception : public etl::exception
  {
  public:

    unaligned_type_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Buffer size exception.
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
    /// ETL_PACKED ensures that GCC does not complain when used in a packed
    /// object.
    //*************************************************************************
    template <size_t Size_, typename TDerivedType>
    ETL_PACKED_CLASS(unaligned_type_common)
    {
    public:

      typedef TDerivedType                          derived_type;
      typedef unsigned char                         storage_type;
      typedef storage_type*                         pointer;
      typedef const storage_type*                   const_pointer;
      typedef storage_type*                         iterator;
      typedef const storage_type*                   const_iterator;
      typedef etl::reverse_iterator<iterator>       reverse_iterator;
      typedef etl::reverse_iterator<const_iterator> const_reverse_iterator;

      //*************************************************************************
      /// Default constructor
      //*************************************************************************
      ETL_CONSTEXPR14 unaligned_type_common()
      {
      }

      //*************************************************************************
      /// Size of the storage.
      //*************************************************************************
      ETL_CONSTEXPR14 size_t size() const
      {
        return Size_;
      }

      //*************************************************************************
      /// Pointer to the beginning of the storage.
      //*************************************************************************
      ETL_CONSTEXPR14 pointer data()
      {
        return get_storage();
      }

      //*************************************************************************
      /// Const pointer to the beginning of the storage.
      //*************************************************************************
      ETL_CONSTEXPR14 const_pointer data() const
      {
        return get_storage();
      }

      //*************************************************************************
      /// Iterator to the beginning of the storage.
      //*************************************************************************
      iterator begin()
      {
        return iterator(get_storage());
      }

      //*************************************************************************
      /// Const iterator to the beginning of the storage.
      //*************************************************************************
      const_iterator begin() const
      {
        return const_iterator(get_storage());
      }

      //*************************************************************************
      /// Const iterator to the beginning of the storage.
      //*************************************************************************
      const_iterator cbegin() const
      {
        return const_iterator(get_storage());
      }

      //*************************************************************************
      /// Reverse iterator to the beginning of the storage.
      //*************************************************************************
      reverse_iterator rbegin()
      {
        return reverse_iterator(get_storage() + Size_);
      }

      //*************************************************************************
      /// Const reverse iterator to the beginning of the storage.
      //*************************************************************************
      const_reverse_iterator rbegin() const
      {
        return const_reverse_iterator(get_storage() + Size_);
      }

      //*************************************************************************
      /// Const reverse iterator to the beginning of the storage.
      //*************************************************************************
      const_reverse_iterator crbegin() const
      {
        return const_reverse_iterator(get_storage() + Size_);
      }

      //*************************************************************************
      /// Iterator to the end of the storage.
      //*************************************************************************
      iterator end()
      {
        return iterator(get_storage() + Size_);
      }

      //*************************************************************************
      /// Const iterator to the end of the storage.
      //*************************************************************************
      const_iterator end() const
      {
        return const_iterator(get_storage() + Size_);
      }

      //*************************************************************************
      /// Const iterator to the end of the storage.
      //*************************************************************************
      const_iterator cend() const
      {
        return const_iterator(get_storage() + Size_);
      }

      //*************************************************************************
      /// Reverse iterator to the end of the storage.
      //*************************************************************************
      reverse_iterator rend()
      {
        return reverse_iterator(get_storage());
      }

      //*************************************************************************
      /// Const reverse iterator to the end of the storage.
      //*************************************************************************
      const_reverse_iterator rend() const
      {
        return const_reverse_iterator(get_storage());
      }

      //*************************************************************************
      /// Const reverse iterator to the end of the storage.
      //*************************************************************************
      const_reverse_iterator crend() const
      {
        return const_reverse_iterator(get_storage());
      }

      //*************************************************************************
      /// Index operator.
      //*************************************************************************
      storage_type& operator[](int i)
      {
        return get_storage()[i];
      }

      //*************************************************************************
      /// Const index operator.
      //*************************************************************************
      const storage_type& operator[](int i) const
      {
        return get_storage()[i];
      }

    private:

      //*************************************************************************
      /// Get a pointer to the storage.
      //*************************************************************************
      ETL_CONSTEXPR14 pointer get_storage()
      {
        return static_cast<derived_type*>(this)->storage;
      }

      //*************************************************************************
      /// Get a const pointer to the storage.
      //*************************************************************************
      ETL_CONSTEXPR14 const_pointer get_storage() const
      {
        return static_cast<const derived_type*>(this)->storage;
      }
    };
    ETL_END_PACKED

    //*************************************************************************
    /// unaligned_type_storage
    /// Contains the fixed storage for a type.
    /// ETL_PACKED ensures that GCC does not complain when used in a packed
    /// object.
    //*************************************************************************
    template <size_t Size_>
    ETL_PACKED_CLASS(unaligned_type_storage)
      : public unaligned_type_common<Size_, unaligned_type_storage<Size_> >
    {
    public:

      friend class unaligned_type_common<Size_, unaligned_type_storage<Size_> >;

    protected:

      //*******************************
      ETL_CONSTEXPR14 unaligned_type_storage()
        : storage()
      {
      }

      unsigned char storage[Size_];
    };
    ETL_END_PACKED

    //*************************************************************************
    /// unaligned_type_storage_ext
    /// Contains a pointer to the fixed storage for a type.
    /// ETL_PACKED ensures that GCC does not complain when used in a packed
    /// object.
    //*************************************************************************
    template <size_t Size_>
    ETL_PACKED_CLASS(unaligned_type_storage_ext)
      : public unaligned_type_common<Size_, unaligned_type_storage_ext<Size_> >
    {
    public:

      friend class unaligned_type_common<Size_, unaligned_type_storage_ext<Size_> >;

    protected:

      //*******************************
      unaligned_type_storage_ext(unsigned char* storage_)
        : storage(storage_)
      {
      }

      //*******************************
      unaligned_type_storage_ext(const unaligned_type_storage_ext<Size_>& other)
        : storage(other.storage)
      {
      }

      //*******************************
      unaligned_type_storage_ext& operator=(const unaligned_type_storage_ext<Size_>& other)
      {
        storage = other.storage;

        return *this;
      }

      unsigned char* storage;
    };
    ETL_END_PACKED

    //*************************************************************************
    /// The shift, in bits, that positions byte 'index' of a value of 'Size'
    /// bytes, for the target endianness.
    //*************************************************************************
    template <size_t Size, int Endian>
    ETL_CONSTEXPR size_t byte_shift(size_t index)
    {
      return ((Endian == ETL_ENDIAN_LITTLE) ? index : (Size - 1U - index)) * 8U;
    }

#if ETL_USING_BUILTIN_BIT_CAST
    //*************************************************************************
    /// Unsigned integer type of a given byte size, used as a constexpr-capable
    /// 'proxy' for extracting/inserting the bytes of a floating point value.
    //*************************************************************************
    template <size_t Size_>
    struct uint_of_size;

    template <>
    struct uint_of_size<2U>
    {
      typedef uint16_t type;
    };

    template <>
    struct uint_of_size<4U>
    {
      typedef uint32_t type;
    };

    template <>
    struct uint_of_size<8U>
    {
      typedef uint64_t type;
    };

    //*************************************************************************
    /// Whether the bit_cast fast path may be used for a type of 'Size' bytes.
    /// It requires a same-size unsigned integer proxy type to exist.
    //*************************************************************************
    template <size_t Size>
    struct use_bit_cast : etl::conditional<(Size == 2U) || (Size == 4U) || (Size == 8U), etl::true_type, etl::false_type>
    {
    };
#endif

    //*************************************************************************
    /// Unaligned copy
    //*************************************************************************
    template <size_t Size_, int Endian_, bool Is_Integral>
    class unaligned_copy;

    //*************************************************************************
    /// Unaligned copy
    /// For integrals.
    /// Bytes are extracted/inserted directly in the target endianness using
    /// shifts, rather than a native-endianness memcpy + conditional reverse.
    /// This makes the conversion independent of the host's endianness and
    /// usable in a constexpr context (C++14 and later).
    //*************************************************************************
    template <size_t Size_, int Endian_>
    ETL_PACKED_CLASS(unaligned_copy)<Size_, Endian_, true>
    {
    public:

      typedef typename private_unaligned_type::unaligned_type_storage< Size_>::storage_type  storage_type;
      typedef typename private_unaligned_type::unaligned_type_storage<Size_>::pointer        pointer;
      typedef typename private_unaligned_type::unaligned_type_storage< Size_>::const_pointer const_pointer;

      //*******************************
      template <typename T>
      static ETL_CONSTEXPR14 void copy_value_to_store(const T& value, pointer store)
      {
        // Note: 'etl::unsigned_type' is used rather than 'etl::make_unsigned', as the
        // latter is not defined for 'bool'.
        typedef typename etl::unsigned_type<T>::type unsigned_t;

        unsigned_t uvalue = static_cast<unsigned_t>(value);

        for (size_t i = 0UL; i < Size_; ++i)
        {
          store[i] = static_cast<storage_type>(uvalue >> private_unaligned_type::byte_shift<Size_, Endian_>(i));
        }
      }

      //*******************************
      template <typename T>
      static ETL_CONSTEXPR14 void copy_store_to_value(const_pointer store, T & value)
      {
        typedef typename etl::unsigned_type<T>::type unsigned_t;

        unsigned_t uvalue = unsigned_t(0);

        for (size_t i = 0UL; i < Size_; ++i)
        {
          uvalue = static_cast<unsigned_t>(uvalue | (static_cast<unsigned_t>(store[i]) << private_unaligned_type::byte_shift<Size_, Endian_>(i)));
        }

        value = static_cast<T>(uvalue);
      }

      //*******************************
      /// \note 'src' and 'dst' must either be equal or refer to completely
      /// separate storage. Partially overlapping storage is undefined
      /// behaviour.
      //*******************************
      static ETL_CONSTEXPR14 void copy_store_to_store(const_pointer src, int endian_src, pointer dst)
      {
        if (Endian_ == endian_src)
        {
          if (src != dst)
          {
            for (size_t i = 0UL; i < Size_; ++i)
            {
              dst[i] = src[i];
            }
          }
        }
        else
        {
          // Swap mirrored bytes in pairs, so that the reversal is correct even
          // when 'src' and 'dst' are the same storage.
          for (size_t i = 0UL; i < (Size_ / 2UL); ++i)
          {
            const storage_type low  = src[i];
            const storage_type high = src[Size_ - 1UL - i];

            dst[i]               = high;
            dst[Size_ - 1UL - i] = low;
          }

          if ((Size_ % 2UL) != 0UL)
          {
            dst[Size_ / 2UL] = src[Size_ / 2UL];
          }
        }
      }
    };
    ETL_END_PACKED

    //*************************************************************************
    /// Unaligned copy
    /// For floating point.
    //*************************************************************************
    template <size_t Size_, int Endian_>
    ETL_PACKED_CLASS(unaligned_copy)<Size_, Endian_, false>
    {
    public:

      typedef typename private_unaligned_type::unaligned_type_storage< Size_>::storage_type  storage_type;
      typedef typename private_unaligned_type::unaligned_type_storage<Size_>::pointer        pointer;
      typedef typename private_unaligned_type::unaligned_type_storage< Size_>::const_pointer const_pointer;

      //*******************************
      // The bit_cast fast path is only valid when a same-size unsigned integer
      // proxy type exists (currently 2, 4 or 8 bytes, e.g. a 16-bit float, float,
      // or double). Other sizes (e.g. 80/96/128-bit 'long double') fall back to
      // memcpy.
      //*******************************
#if ETL_USING_BUILTIN_BIT_CAST
      template <typename T>
      static ETL_CONSTEXPR14 void do_copy_value_to_store(const T& value, pointer store, etl::true_type)
      {
        typedef typename private_unaligned_type::uint_of_size<sizeof(T)>::type uint_t;

        uint_t uvalue = etl::bit_cast<uint_t>(value);

        for (size_t i = 0UL; i < Size_; ++i)
        {
          store[i] = static_cast<storage_type>(uvalue >> private_unaligned_type::byte_shift<Size_, Endian_>(i));
        }
      }
#endif

      template <typename T>
      static void do_copy_value_to_store(const T& value, pointer store, etl::false_type)
      {
        memcpy(store, &value, Size_);

#if ETL_HAS_CONSTEXPR_ENDIANNESS
        if ETL_IF_CONSTEXPR (Endian_ != etl::endianness::value())
#else
        if (Endian_ != etl::endianness::value())
#endif
        {
          etl::reverse(store, store + Size_);
        }
      }

      //*******************************
      template <typename T>
      static
#if ETL_USING_BUILTIN_BIT_CAST
        ETL_CONSTEXPR14
#endif
        void
        copy_value_to_store(const T& value, pointer store)
      {
#if ETL_USING_BUILTIN_BIT_CAST
        typedef typename private_unaligned_type::use_bit_cast<sizeof(T)>::type use_bit_cast_t;
#else
        typedef etl::false_type use_bit_cast_t;
#endif
        do_copy_value_to_store(value, store, use_bit_cast_t());
      }

      //*******************************
#if ETL_USING_BUILTIN_BIT_CAST
      template <typename T>
      static ETL_CONSTEXPR14 void do_copy_store_to_value(const_pointer store, T & value, etl::true_type)
      {
        typedef typename private_unaligned_type::uint_of_size<sizeof(T)>::type uint_t;

        uint_t uvalue = uint_t(0);

        for (size_t i = 0UL; i < Size_; ++i)
        {
          uvalue = static_cast<uint_t>(uvalue | (static_cast<uint_t>(store[i]) << private_unaligned_type::byte_shift<Size_, Endian_>(i)));
        }

        value = etl::bit_cast<T>(uvalue);
      }
#endif

      template <typename T>
      static void do_copy_store_to_value(const_pointer store, T & value, etl::false_type)
      {
        memcpy(&value, store, Size_);

#if ETL_HAS_CONSTEXPR_ENDIANNESS
        if ETL_IF_CONSTEXPR (Endian_ != etl::endianness::value())
#else
        if (Endian_ != etl::endianness::value())
#endif
        {
          etl::reverse(reinterpret_cast<pointer>(&value), reinterpret_cast<pointer>(&value) + Size_);
        }
      }

      //*******************************
      template <typename T>
      static
#if ETL_USING_BUILTIN_BIT_CAST
        ETL_CONSTEXPR14
#endif
        void
        copy_store_to_value(const_pointer store, T & value)
      {
#if ETL_USING_BUILTIN_BIT_CAST
        typedef typename private_unaligned_type::use_bit_cast<sizeof(T)>::type use_bit_cast_t;
#else
        typedef etl::false_type use_bit_cast_t;
#endif
        do_copy_store_to_value(store, value, use_bit_cast_t());
      }

      //*******************************
      // This is pure byte manipulation (copy + optional reversal), with no
      // floating point arithmetic involved, so it is always constexpr-capable
      // (C++14 and later), regardless of bit_cast/builtin availability.
      /// \note 'src' and 'dst' must either be equal or refer to completely
      /// separate storage. Partially overlapping storage is undefined
      /// behaviour.
      //*******************************
      static ETL_CONSTEXPR14 void copy_store_to_store(const_pointer src, int endian_src, pointer dst)
      {
        if (Endian_ == endian_src)
        {
          if (src != dst)
          {
            for (size_t i = 0UL; i < Size_; ++i)
            {
              dst[i] = src[i];
            }
          }
        }
        else
        {
          // Swap mirrored bytes in pairs, so that the reversal is correct even
          // when 'src' and 'dst' are the same storage.
          for (size_t i = 0UL; i < (Size_ / 2UL); ++i)
          {
            const storage_type low  = src[i];
            const storage_type high = src[Size_ - 1UL - i];

            dst[i]               = high;
            dst[Size_ - 1UL - i] = low;
          }

          if ((Size_ % 2UL) != 0UL)
          {
            dst[Size_ / 2UL] = src[Size_ / 2UL];
          }
        }
      }
    };
    ETL_END_PACKED
  } // namespace private_unaligned_type

  //*************************************************************************
  /// unaligned_type
  ///\brief Allows an arithmetic type to be stored at an unaligned address.
  ///\tparam T      The arithmetic type.
  ///\tparam Endian The endianness of the arithmetic type.
  //*************************************************************************
  template <typename T, int Endian_>
  ETL_PACKED_CLASS(unaligned_type)
    : public private_unaligned_type::unaligned_type_storage<sizeof(T)>
  {
  public:

    ETL_STATIC_ASSERT(etl::is_integral<T>::value || etl::is_floating_point<T>::value, "Unaligned type must be integral or floating point");

    typedef T value_type;

    typedef private_unaligned_type::unaligned_copy< sizeof(T), Endian_, etl::is_floating_point<T>::value ? false : true> unaligned_copy;

    typedef typename private_unaligned_type::unaligned_type_storage<sizeof(T)>::storage_type           storage_type;
    typedef typename private_unaligned_type::unaligned_type_storage<sizeof(T)>::pointer                pointer;
    typedef typename private_unaligned_type::unaligned_type_storage<sizeof(T)>::const_pointer          const_pointer;
    typedef typename private_unaligned_type::unaligned_type_storage<sizeof(T)>::iterator               iterator;
    typedef typename private_unaligned_type::unaligned_type_storage<sizeof(T)>::const_iterator         const_iterator;
    typedef typename private_unaligned_type::unaligned_type_storage<sizeof(T)>::reverse_iterator       reverse_iterator;
    typedef typename private_unaligned_type::unaligned_type_storage<sizeof(T)>::const_reverse_iterator const_reverse_iterator;

    static ETL_CONSTANT int    Endian = Endian_;
    static ETL_CONSTANT size_t Size   = sizeof(T);

    //*************************************************************************
    /// Default constructor
    //*************************************************************************
    unaligned_type() {}

    //*************************************************************************
    /// Construct from a value.
    //*************************************************************************
    ETL_CONSTEXPR14 unaligned_type(T value)
    {
      unaligned_copy::copy_value_to_store(value, this->storage);
    }

    //*************************************************************************
    /// Construct from an address.
    //*************************************************************************
    unaligned_type(const void* address)
    {
      etl::copy_n(reinterpret_cast<const unsigned char*>(address), sizeof(T), this->storage);
    }

    //*************************************************************************
    /// Construct from an address and buffer size.
    /// \note 'buffer_size' must be greater than or equal to 'sizeof(T)'.
    /// This is a precondition, checked by ETL_ASSERT. If the checks are
    /// disabled (e.g. ETL_NO_CHECKS) then passing a smaller buffer size
    /// results in a read beyond the end of the buffer.
    //*************************************************************************
    unaligned_type(const void* address, size_t buffer_size)
    {
      ETL_ASSERT(sizeof(T) <= buffer_size, ETL_ERROR(etl::unaligned_type_buffer_size));

      etl::copy_n(reinterpret_cast<const unsigned char*>(address), sizeof(T), this->storage);
    }

    //*************************************************************************
    /// Construct from a byte buffer.
    /// Unlike the 'const void*' overload above, this does not require a
    /// reinterpret_cast, so it is usable in a constexpr context (C++14 and
    /// later), allowing compile time decoding of a byte buffer with an
    /// explicit endianness.
    //*************************************************************************
    ETL_CONSTEXPR14 unaligned_type(const unsigned char* address)
    {
      etl::copy_n(address, sizeof(T), this->storage);
    }

    //*************************************************************************
    /// Construct from a byte buffer and buffer size.
    /// See the note on the 'const unsigned char*' overload above regarding
    /// constexpr usability.
    /// \note 'buffer_size' must be greater than or equal to 'sizeof(T)'.
    /// This is a precondition, checked by ETL_ASSERT. If the checks are
    /// disabled (e.g. ETL_NO_CHECKS) then passing a smaller buffer size
    /// results in a read beyond the end of the buffer.
    //*************************************************************************
    ETL_CONSTEXPR14 unaligned_type(const unsigned char* address, size_t buffer_size)
    {
      ETL_ASSERT(sizeof(T) <= buffer_size, ETL_ERROR(etl::unaligned_type_buffer_size));

      etl::copy_n(address, sizeof(T), this->storage);
    }

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    ETL_CONSTEXPR14 unaligned_type(const unaligned_type<T, Endian>& other)
    {
      unaligned_copy::copy_store_to_store(other.data(), Endian, this->storage);
    }

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    template <int Endian_Other>
    ETL_CONSTEXPR14 unaligned_type(const unaligned_type<T, Endian_Other>& other)
    {
      unaligned_copy::copy_store_to_store(other.data(), Endian_Other, this->storage);
    }

    //*************************************************************************
    /// Assignment operator
    //*************************************************************************
    ETL_CONSTEXPR14 unaligned_type& operator=(T value)
    {
      unaligned_copy::copy_value_to_store(value, this->storage);

      return *this;
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    ETL_CONSTEXPR14 unaligned_type& operator=(const unaligned_type<T, Endian_>& other)
    {
      unaligned_copy::copy_store_to_store(other.data(), Endian_, this->storage);

      return *this;
    }

    //*************************************************************************
    /// Assignment operator from other endianness.
    //*************************************************************************
    template <int Endian_Other>
    ETL_CONSTEXPR14 unaligned_type& operator=(const unaligned_type<T, Endian_Other>& other)
    {
      unaligned_copy::copy_store_to_store(other.data(), Endian_Other, this->storage);

      return *this;
    }

    //*************************************************************************
    /// Conversion operator
    //*************************************************************************
    ETL_CONSTEXPR14 operator T() const
    {
      T value = T();

      unaligned_copy::copy_store_to_value(this->storage, value);

      return value;
    }

    //*************************************************************************
    /// Get the value.
    //*************************************************************************
    ETL_CONSTEXPR14 T value() const
    {
      T value = T();

      unaligned_copy::copy_store_to_value(this->storage, value);

      return value;
    }
  };
  ETL_END_PACKED

  template <typename T, int Endian_>
  ETL_CONSTANT int unaligned_type<T, Endian_>::Endian;

  template <typename T, int Endian_>
  ETL_CONSTANT size_t unaligned_type<T, Endian_>::Size;

  namespace private_unaligned_type
  {
    //*************************************************************************
    /// The unsigned integer type used to interface to an unaligned unsigned
    /// integer of 'Size' bytes.
    /// The smallest standard unsigned integer type that can hold all of the
    /// bytes is selected.
    //*************************************************************************
    template <size_t Size_>
    struct interface_uint;

    template <>
    struct interface_uint<1U>
    {
      typedef uint8_t type;
    };

    template <>
    struct interface_uint<2U>
    {
      typedef uint16_t type;
    };

    template <>
    struct interface_uint<3U>
    {
      typedef uint32_t type;
    };

    template <>
    struct interface_uint<4U>
    {
      typedef uint32_t type;
    };

#if ETL_USING_64BIT_TYPES
    template <>
    struct interface_uint<5U>
    {
      typedef uint64_t type;
    };

    template <>
    struct interface_uint<6U>
    {
      typedef uint64_t type;
    };

    template <>
    struct interface_uint<7U>
    {
      typedef uint64_t type;
    };

    template <>
    struct interface_uint<8U>
    {
      typedef uint64_t type;
    };
#endif

    //*************************************************************************
    /// The signed integer type used to interface to an unaligned signed
    /// integer of 'Size' bytes.
    /// The smallest standard signed integer type that can hold all of the
    /// bytes is selected.
    //*************************************************************************
    template <size_t Size_>
    struct interface_int;

    template <>
    struct interface_int<1U>
    {
      typedef int8_t type;
    };

    template <>
    struct interface_int<2U>
    {
      typedef int16_t type;
    };

    template <>
    struct interface_int<3U>
    {
      typedef int32_t type;
    };

    template <>
    struct interface_int<4U>
    {
      typedef int32_t type;
    };

#if ETL_USING_64BIT_TYPES
    template <>
    struct interface_int<5U>
    {
      typedef int64_t type;
    };

    template <>
    struct interface_int<6U>
    {
      typedef int64_t type;
    };

    template <>
    struct interface_int<7U>
    {
      typedef int64_t type;
    };

    template <>
    struct interface_int<8U>
    {
      typedef int64_t type;
    };
#endif

    //*************************************************************************
    /// Sign extends the low 'Size_ * 8' bits of an unsigned value into the
    /// full width of the signed interface type.
    /// Uses only well defined unsigned arithmetic, so it is usable in a
    /// constexpr context.
    //*************************************************************************
    template <size_t Size_, typename TSigned, typename TUnsigned>
    ETL_CONSTEXPR TSigned sign_extend(TUnsigned uvalue)
    {
      return static_cast<TSigned>(static_cast<TUnsigned>(static_cast<TUnsigned>(uvalue ^ (static_cast<TUnsigned>(1) << ((Size_ * 8U) - 1U)))
                                                         - (static_cast<TUnsigned>(1) << ((Size_ * 8U) - 1U))));
    }
  } // namespace private_unaligned_type

  //*************************************************************************
  /// unaligned_uint_type
  ///\brief Allows an unsigned integer of an arbitrary number of bytes to be
  ///       stored at an unaligned address.
  ///       Useful for types that have no native equivalent, such as a 24 bit
  ///       unsigned integer.
  ///       The value is interfaced to via the smallest standard unsigned
  ///       integer type that can hold it.
  ///\tparam Size_   The number of bytes of storage.
  ///\tparam Endian_ The endianness of the stored integer.
  ///\note  Values that do not fit in 'Size_' bytes are truncated, in the same
  ///       way as a narrowing static_cast.
  //*************************************************************************
  template <size_t Size_, int Endian_>
  ETL_PACKED_CLASS(unaligned_uint_type)
    : public private_unaligned_type::unaligned_type_storage<Size_>
  {
  public:

    typedef typename private_unaligned_type::interface_uint<Size_>::type value_type;

    typedef private_unaligned_type::unaligned_copy<Size_, Endian_, true> unaligned_copy;

    typedef typename private_unaligned_type::unaligned_type_storage<Size_>::storage_type           storage_type;
    typedef typename private_unaligned_type::unaligned_type_storage<Size_>::pointer                pointer;
    typedef typename private_unaligned_type::unaligned_type_storage<Size_>::const_pointer          const_pointer;
    typedef typename private_unaligned_type::unaligned_type_storage<Size_>::iterator               iterator;
    typedef typename private_unaligned_type::unaligned_type_storage<Size_>::const_iterator         const_iterator;
    typedef typename private_unaligned_type::unaligned_type_storage<Size_>::reverse_iterator       reverse_iterator;
    typedef typename private_unaligned_type::unaligned_type_storage<Size_>::const_reverse_iterator const_reverse_iterator;

    static ETL_CONSTANT int        Endian    = Endian_;
    static ETL_CONSTANT size_t     Size      = Size_;
    static ETL_CONSTANT value_type Min_Value = static_cast<value_type>(0);
    static ETL_CONSTANT value_type Max_Value = static_cast<value_type>(etl::integral_limits<value_type>::max >> ((sizeof(value_type) - Size_) * 8U));

    //*************************************************************************
    /// Default constructor
    //*************************************************************************
    unaligned_uint_type() {}

    //*************************************************************************
    /// Construct from a value.
    //*************************************************************************
    ETL_CONSTEXPR14 unaligned_uint_type(value_type value)
    {
      unaligned_copy::copy_value_to_store(value, this->storage);
    }

    //*************************************************************************
    /// Construct from an address.
    //*************************************************************************
    unaligned_uint_type(const void* address)
    {
      etl::copy_n(reinterpret_cast<const unsigned char*>(address), Size_, this->storage);
    }

    //*************************************************************************
    /// Construct from an address and buffer size.
    /// \note 'buffer_size' must be greater than or equal to 'Size_'.
    //*************************************************************************
    unaligned_uint_type(const void* address, size_t buffer_size)
    {
      ETL_ASSERT(Size_ <= buffer_size, ETL_ERROR(etl::unaligned_type_buffer_size));

      etl::copy_n(reinterpret_cast<const unsigned char*>(address), Size_, this->storage);
    }

    //*************************************************************************
    /// Construct from a byte buffer.
    /// Usable in a constexpr context (C++14 and later).
    //*************************************************************************
    ETL_CONSTEXPR14 unaligned_uint_type(const unsigned char* address)
    {
      etl::copy_n(address, Size_, this->storage);
    }

    //*************************************************************************
    /// Construct from a byte buffer and buffer size.
    /// \note 'buffer_size' must be greater than or equal to 'Size_'.
    //*************************************************************************
    ETL_CONSTEXPR14 unaligned_uint_type(const unsigned char* address, size_t buffer_size)
    {
      ETL_ASSERT(Size_ <= buffer_size, ETL_ERROR(etl::unaligned_type_buffer_size));

      etl::copy_n(address, Size_, this->storage);
    }

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    ETL_CONSTEXPR14 unaligned_uint_type(const unaligned_uint_type<Size_, Endian>& other)
    {
      unaligned_copy::copy_store_to_store(other.data(), Endian, this->storage);
    }

    //*************************************************************************
    /// Copy constructor from other endianness.
    //*************************************************************************
    template <int Endian_Other>
    ETL_CONSTEXPR14 unaligned_uint_type(const unaligned_uint_type<Size_, Endian_Other>& other)
    {
      unaligned_copy::copy_store_to_store(other.data(), Endian_Other, this->storage);
    }

    //*************************************************************************
    /// Assignment operator
    //*************************************************************************
    ETL_CONSTEXPR14 unaligned_uint_type& operator=(value_type value)
    {
      unaligned_copy::copy_value_to_store(value, this->storage);

      return *this;
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    ETL_CONSTEXPR14 unaligned_uint_type& operator=(const unaligned_uint_type<Size_, Endian_>& other)
    {
      unaligned_copy::copy_store_to_store(other.data(), Endian_, this->storage);

      return *this;
    }

    //*************************************************************************
    /// Assignment operator from other endianness.
    //*************************************************************************
    template <int Endian_Other>
    ETL_CONSTEXPR14 unaligned_uint_type& operator=(const unaligned_uint_type<Size_, Endian_Other>& other)
    {
      unaligned_copy::copy_store_to_store(other.data(), Endian_Other, this->storage);

      return *this;
    }

    //*************************************************************************
    /// Conversion operator
    //*************************************************************************
    ETL_CONSTEXPR14 operator value_type() const
    {
      value_type value = value_type();

      unaligned_copy::copy_store_to_value(this->storage, value);

      return value;
    }

    //*************************************************************************
    /// Get the value.
    //*************************************************************************
    ETL_CONSTEXPR14 value_type value() const
    {
      value_type value = value_type();

      unaligned_copy::copy_store_to_value(this->storage, value);

      return value;
    }
  };
  ETL_END_PACKED

  template <size_t Size_, int Endian_>
  ETL_CONSTANT int unaligned_uint_type<Size_, Endian_>::Endian;

  template <size_t Size_, int Endian_>
  ETL_CONSTANT size_t unaligned_uint_type<Size_, Endian_>::Size;

  template <size_t Size_, int Endian_>
  ETL_CONSTANT typename unaligned_uint_type<Size_, Endian_>::value_type unaligned_uint_type<Size_, Endian_>::Min_Value;

  template <size_t Size_, int Endian_>
  ETL_CONSTANT typename unaligned_uint_type<Size_, Endian_>::value_type unaligned_uint_type<Size_, Endian_>::Max_Value;

  //*************************************************************************
  /// unaligned_int_type
  ///\brief Allows a signed, two's complement, integer of an arbitrary number
  ///       of bytes to be stored at an unaligned address.
  ///       Useful for types that have no native equivalent, such as a 24 bit
  ///       signed integer.
  ///       The value is interfaced to via the smallest standard signed
  ///       integer type that can hold it, and is sign extended on reading.
  ///\tparam Size_   The number of bytes of storage.
  ///\tparam Endian_ The endianness of the stored integer.
  ///\note  Values that do not fit in 'Size_' bytes are truncated, in the same
  ///       way as a narrowing static_cast.
  //*************************************************************************
  template <size_t Size_, int Endian_>
  ETL_PACKED_CLASS(unaligned_int_type)
    : public private_unaligned_type::unaligned_type_storage<Size_>
  {
  public:

    typedef typename private_unaligned_type::interface_int<Size_>::type value_type;

    typedef private_unaligned_type::unaligned_copy<Size_, Endian_, true> unaligned_copy;

    typedef typename private_unaligned_type::unaligned_type_storage<Size_>::storage_type           storage_type;
    typedef typename private_unaligned_type::unaligned_type_storage<Size_>::pointer                pointer;
    typedef typename private_unaligned_type::unaligned_type_storage<Size_>::const_pointer          const_pointer;
    typedef typename private_unaligned_type::unaligned_type_storage<Size_>::iterator               iterator;
    typedef typename private_unaligned_type::unaligned_type_storage<Size_>::const_iterator         const_iterator;
    typedef typename private_unaligned_type::unaligned_type_storage<Size_>::reverse_iterator       reverse_iterator;
    typedef typename private_unaligned_type::unaligned_type_storage<Size_>::const_reverse_iterator const_reverse_iterator;

    static ETL_CONSTANT int        Endian = Endian_;
    static ETL_CONSTANT size_t     Size   = Size_;
    static ETL_CONSTANT value_type Max_Value =
      static_cast<value_type>(etl::integral_limits<typename etl::unsigned_type<value_type>::type>::max >> (((sizeof(value_type) - Size_) * 8U) + 1U));
    static ETL_CONSTANT value_type Min_Value = static_cast<value_type>(-Max_Value - 1);

    //*************************************************************************
    /// Default constructor
    //*************************************************************************
    unaligned_int_type() {}

    //*************************************************************************
    /// Construct from a value.
    //*************************************************************************
    ETL_CONSTEXPR14 unaligned_int_type(value_type value)
    {
      unaligned_copy::copy_value_to_store(value, this->storage);
    }

    //*************************************************************************
    /// Construct from an address.
    //*************************************************************************
    unaligned_int_type(const void* address)
    {
      etl::copy_n(reinterpret_cast<const unsigned char*>(address), Size_, this->storage);
    }

    //*************************************************************************
    /// Construct from an address and buffer size.
    /// \note 'buffer_size' must be greater than or equal to 'Size_'.
    //*************************************************************************
    unaligned_int_type(const void* address, size_t buffer_size)
    {
      ETL_ASSERT(Size_ <= buffer_size, ETL_ERROR(etl::unaligned_type_buffer_size));

      etl::copy_n(reinterpret_cast<const unsigned char*>(address), Size_, this->storage);
    }

    //*************************************************************************
    /// Construct from a byte buffer.
    /// Usable in a constexpr context (C++14 and later).
    //*************************************************************************
    ETL_CONSTEXPR14 unaligned_int_type(const unsigned char* address)
    {
      etl::copy_n(address, Size_, this->storage);
    }

    //*************************************************************************
    /// Construct from a byte buffer and buffer size.
    /// \note 'buffer_size' must be greater than or equal to 'Size_'.
    //*************************************************************************
    ETL_CONSTEXPR14 unaligned_int_type(const unsigned char* address, size_t buffer_size)
    {
      ETL_ASSERT(Size_ <= buffer_size, ETL_ERROR(etl::unaligned_type_buffer_size));

      etl::copy_n(address, Size_, this->storage);
    }

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    ETL_CONSTEXPR14 unaligned_int_type(const unaligned_int_type<Size_, Endian>& other)
    {
      unaligned_copy::copy_store_to_store(other.data(), Endian, this->storage);
    }

    //*************************************************************************
    /// Copy constructor from other endianness.
    //*************************************************************************
    template <int Endian_Other>
    ETL_CONSTEXPR14 unaligned_int_type(const unaligned_int_type<Size_, Endian_Other>& other)
    {
      unaligned_copy::copy_store_to_store(other.data(), Endian_Other, this->storage);
    }

    //*************************************************************************
    /// Assignment operator
    //*************************************************************************
    ETL_CONSTEXPR14 unaligned_int_type& operator=(value_type value)
    {
      unaligned_copy::copy_value_to_store(value, this->storage);

      return *this;
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    ETL_CONSTEXPR14 unaligned_int_type& operator=(const unaligned_int_type<Size_, Endian_>& other)
    {
      unaligned_copy::copy_store_to_store(other.data(), Endian_, this->storage);

      return *this;
    }

    //*************************************************************************
    /// Assignment operator from other endianness.
    //*************************************************************************
    template <int Endian_Other>
    ETL_CONSTEXPR14 unaligned_int_type& operator=(const unaligned_int_type<Size_, Endian_Other>& other)
    {
      unaligned_copy::copy_store_to_store(other.data(), Endian_Other, this->storage);

      return *this;
    }

    //*************************************************************************
    /// Conversion operator
    //*************************************************************************
    ETL_CONSTEXPR14 operator value_type() const
    {
      return value();
    }

    //*************************************************************************
    /// Get the value.
    //*************************************************************************
    ETL_CONSTEXPR14 value_type value() const
    {
      typedef typename etl::unsigned_type<value_type>::type unsigned_t;

      unsigned_t uvalue = unsigned_t(0);

      unaligned_copy::copy_store_to_value(this->storage, uvalue);

      return private_unaligned_type::sign_extend<Size_, value_type>(uvalue);
    }
  };
  ETL_END_PACKED

  //*************************************************************************
  /// unaligned_type_ext
  ///\brief Allows an arithmetic type to be stored at an unaligned address.
  ///       Uses an external buffer.
  ///\tparam T      The arithmetic type.
  ///\tparam Endian The endianness of the arithmetic type.
  //*************************************************************************
  template <typename T, int Endian_>
  ETL_PACKED_CLASS(unaligned_type_ext)
    : public private_unaligned_type::unaligned_type_storage_ext<sizeof(T)>
  {
  public:

    ETL_STATIC_ASSERT(etl::is_integral<T>::value || etl::is_floating_point<T>::value, "Unaligned type must be integral or floating point");

    template <typename U, int Endian_Other>
    friend class unaligned_type_ext;

    typedef T value_type;

    typedef private_unaligned_type::unaligned_copy< sizeof(T), Endian_, etl::is_floating_point<T>::value ? false : true> unaligned_copy;

    typedef typename private_unaligned_type::unaligned_type_storage_ext<sizeof(T)>::storage_type           storage_type;
    typedef typename private_unaligned_type::unaligned_type_storage_ext<sizeof(T)>::pointer                pointer;
    typedef typename private_unaligned_type::unaligned_type_storage_ext<sizeof(T)>::const_pointer          const_pointer;
    typedef typename private_unaligned_type::unaligned_type_storage_ext<sizeof(T)>::iterator               iterator;
    typedef typename private_unaligned_type::unaligned_type_storage_ext<sizeof(T)>::const_iterator         const_iterator;
    typedef typename private_unaligned_type::unaligned_type_storage_ext<sizeof(T)>::reverse_iterator       reverse_iterator;
    typedef typename private_unaligned_type::unaligned_type_storage_ext<sizeof(T)>::const_reverse_iterator const_reverse_iterator;

    static ETL_CONSTANT int    Endian = Endian_;
    static ETL_CONSTANT size_t Size   = sizeof(T);

    //*************************************************************************
    /// Construct from a storage pointer
    //*************************************************************************
    unaligned_type_ext(pointer storage_)
      : private_unaligned_type::unaligned_type_storage_ext<Size>(storage_)
    {
    }

    //*************************************************************************
    /// Construct from a value and storage pointer
    //*************************************************************************
    unaligned_type_ext(T value, pointer storage_)
      : private_unaligned_type::unaligned_type_storage_ext<Size>(storage_)
    {
      unaligned_copy::copy_value_to_store(value, this->storage);
    }

    //*************************************************************************
    /// Copy constructor with storage pointer
    /// \note 'storage_' must either be the same storage as 'other's, or refer
    /// to completely separate storage. Partially overlapping storage is
    /// undefined behaviour.
    //*************************************************************************
    template <int Endian_Other>
    unaligned_type_ext(const unaligned_type_ext<T, Endian_Other>& other, pointer storage_)
      : private_unaligned_type::unaligned_type_storage_ext<Size>(storage_)
    {
      unaligned_copy::copy_store_to_store(other.data(), Endian_Other, this->storage);
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Move constructor
    //*************************************************************************
    unaligned_type_ext(unaligned_type_ext<T, Endian> && other)
      : private_unaligned_type::unaligned_type_storage_ext<Size>(other.storage)
    {
      other.storage = ETL_NULLPTR;
    }

    //*************************************************************************
    /// Move constructor
    //*************************************************************************
    template <int Endian_Other>
    unaligned_type_ext(unaligned_type_ext<T, Endian_Other> && other)
      : private_unaligned_type::unaligned_type_storage_ext<Size>(other.storage)
    {
      // If we're constructing from a different endianness then we need to
      // reverse the data order.
      if (Endian != Endian_Other)
      {
        etl::reverse(this->begin(), this->end());
      }

      other.storage = ETL_NULLPTR;
    }
#endif

    //*************************************************************************
    /// Assignment operator
    //*************************************************************************
    unaligned_type_ext& operator=(T value)
    {
      unaligned_copy::copy_value_to_store(value, this->storage);

      return *this;
    }

    //*************************************************************************
    /// Copy assignment operator from other endianness.
    //*************************************************************************
    unaligned_type_ext& operator=(const unaligned_type_ext<T, Endian>& other)
    {
      unaligned_copy::copy_store_to_store(other.data(), Endian, this->storage);

      return *this;
    }

    //*************************************************************************
    /// Copy assignment operator from other endianness.
    /// \note The storage of 'other' must either be the same storage as this
    /// object's, or completely separate. Partially overlapping storage is
    /// undefined behaviour.
    //*************************************************************************
    template <int Endian_Other>
    unaligned_type_ext& operator=(const unaligned_type_ext<T, Endian_Other>& other)
    {
      unaligned_copy::copy_store_to_store(other.data(), Endian_Other, this->storage);

      return *this;
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Move assignment operator from other endianness.
    //*************************************************************************
    unaligned_type_ext& operator=(unaligned_type_ext<T, Endian>&& other)
    {
      this->storage = other.storage;
      other.storage = ETL_NULLPTR;

      return *this;
    }

    //*************************************************************************
    /// Move assignment operator from other endianness.
    //*************************************************************************
    template <int Endian_Other>
    unaligned_type_ext& operator=(unaligned_type_ext<T, Endian_Other>&& other)
    {
      this->storage = other.storage;

      // If we're assigning from a different endianness then we need to reverse
      // the data order.
      if (Endian != Endian_Other)
      {
        etl::reverse(this->begin(), this->end());
      }

      other.storage = ETL_NULLPTR;

      return *this;
    }
#endif

    //*************************************************************************
    /// Conversion operator
    //*************************************************************************
    operator T() const
    {
      T value = T();

      unaligned_copy::copy_store_to_value(this->storage, value);

      return value;
    }

    //*************************************************************************
    /// Get the value.
    //*************************************************************************
    T value() const
    {
      T value = T();

      unaligned_copy::copy_store_to_value(this->storage, value);

      return value;
    }

    //*************************************************************************
    /// Sets the storage for the type.
    //*************************************************************************
    void set_storage(pointer storage_)
    {
      this->storage = storage_;
    }

  private:

    unaligned_type_ext() ETL_DELETE;
  };
  ETL_END_PACKED

  template <typename T, int Endian_>
  ETL_CONSTANT int unaligned_type_ext<T, Endian_>::Endian;

  template <typename T, int Endian_>
  ETL_CONSTANT size_t unaligned_type_ext<T, Endian_>::Size;

  //*************************************************************************
  /// unaligned_uint_type_ext
  ///\brief Allows an unsigned integer of an arbitrary number of bytes to be
  ///       stored at an unaligned address. Uses an external buffer.
  ///       The value is interfaced to via the smallest standard unsigned
  ///       integer type that can hold it.
  ///\tparam Size_   The number of bytes of storage.
  ///\tparam Endian_ The endianness of the stored integer.
  ///\note  Values that do not fit in 'Size_' bytes are truncated, in the same
  ///       way as a narrowing static_cast.
  //*************************************************************************
  template <size_t Size_, int Endian_>
  ETL_PACKED_CLASS(unaligned_uint_type_ext)
    : public private_unaligned_type::unaligned_type_storage_ext<Size_>
  {
  public:

    template <size_t Size_Other, int Endian_Other>
    friend class unaligned_uint_type_ext;

    typedef typename private_unaligned_type::interface_uint<Size_>::type value_type;

    typedef private_unaligned_type::unaligned_copy<Size_, Endian_, true> unaligned_copy;

    typedef typename private_unaligned_type::unaligned_type_storage_ext<Size_>::storage_type           storage_type;
    typedef typename private_unaligned_type::unaligned_type_storage_ext<Size_>::pointer                pointer;
    typedef typename private_unaligned_type::unaligned_type_storage_ext<Size_>::const_pointer          const_pointer;
    typedef typename private_unaligned_type::unaligned_type_storage_ext<Size_>::iterator               iterator;
    typedef typename private_unaligned_type::unaligned_type_storage_ext<Size_>::const_iterator         const_iterator;
    typedef typename private_unaligned_type::unaligned_type_storage_ext<Size_>::reverse_iterator       reverse_iterator;
    typedef typename private_unaligned_type::unaligned_type_storage_ext<Size_>::const_reverse_iterator const_reverse_iterator;

    static ETL_CONSTANT int        Endian    = Endian_;
    static ETL_CONSTANT size_t     Size      = Size_;
    static ETL_CONSTANT value_type Min_Value = static_cast<value_type>(0);
    static ETL_CONSTANT value_type Max_Value = static_cast<value_type>(etl::integral_limits<value_type>::max >> ((sizeof(value_type) - Size_) * 8U));

    //*************************************************************************
    /// Construct from a storage pointer
    //*************************************************************************
    unaligned_uint_type_ext(pointer storage_)
      : private_unaligned_type::unaligned_type_storage_ext<Size_>(storage_)
    {
    }

    //*************************************************************************
    /// Construct from a value and storage pointer
    //*************************************************************************
    unaligned_uint_type_ext(value_type value, pointer storage_)
      : private_unaligned_type::unaligned_type_storage_ext<Size_>(storage_)
    {
      unaligned_copy::copy_value_to_store(value, this->storage);
    }

    //*************************************************************************
    /// Copy constructor with storage pointer
    /// \note 'storage_' must either be the same storage as 'other's, or refer
    /// to completely separate storage. Partially overlapping storage is
    /// undefined behaviour.
    //*************************************************************************
    template <int Endian_Other>
    unaligned_uint_type_ext(const unaligned_uint_type_ext<Size_, Endian_Other>& other, pointer storage_)
      : private_unaligned_type::unaligned_type_storage_ext<Size_>(storage_)
    {
      unaligned_copy::copy_store_to_store(other.data(), Endian_Other, this->storage);
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Move constructor
    //*************************************************************************
    unaligned_uint_type_ext(unaligned_uint_type_ext<Size_, Endian> && other)
      : private_unaligned_type::unaligned_type_storage_ext<Size_>(other.storage)
    {
      other.storage = ETL_NULLPTR;
    }

    //*************************************************************************
    /// Move constructor from other endianness
    //*************************************************************************
    template <int Endian_Other>
    unaligned_uint_type_ext(unaligned_uint_type_ext<Size_, Endian_Other> && other)
      : private_unaligned_type::unaligned_type_storage_ext<Size_>(other.storage)
    {
      // If we're constructing from a different endianness then we need to
      // reverse the data order.
      if (Endian != Endian_Other)
      {
        etl::reverse(this->begin(), this->end());
      }

      other.storage = ETL_NULLPTR;
    }
#endif

    //*************************************************************************
    /// Assignment operator
    //*************************************************************************
    unaligned_uint_type_ext& operator=(value_type value)
    {
      unaligned_copy::copy_value_to_store(value, this->storage);

      return *this;
    }

    //*************************************************************************
    /// Copy assignment operator.
    //*************************************************************************
    unaligned_uint_type_ext& operator=(const unaligned_uint_type_ext<Size_, Endian>& other)
    {
      unaligned_copy::copy_store_to_store(other.data(), Endian, this->storage);

      return *this;
    }

    //*************************************************************************
    /// Copy assignment operator from other endianness.
    /// \note The storage of 'other' must either be the same storage as this
    /// object's, or completely separate. Partially overlapping storage is
    /// undefined behaviour.
    //*************************************************************************
    template <int Endian_Other>
    unaligned_uint_type_ext& operator=(const unaligned_uint_type_ext<Size_, Endian_Other>& other)
    {
      unaligned_copy::copy_store_to_store(other.data(), Endian_Other, this->storage);

      return *this;
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Move assignment operator.
    //*************************************************************************
    unaligned_uint_type_ext& operator=(unaligned_uint_type_ext<Size_, Endian>&& other)
    {
      this->storage = other.storage;
      other.storage = ETL_NULLPTR;

      return *this;
    }

    //*************************************************************************
    /// Move assignment operator from other endianness.
    //*************************************************************************
    template <int Endian_Other>
    unaligned_uint_type_ext& operator=(unaligned_uint_type_ext<Size_, Endian_Other>&& other)
    {
      this->storage = other.storage;

      // If we're assigning from a different endianness then we need to reverse
      // the data order.
      if (Endian != Endian_Other)
      {
        etl::reverse(this->begin(), this->end());
      }

      other.storage = ETL_NULLPTR;

      return *this;
    }
#endif

    //*************************************************************************
    /// Conversion operator
    //*************************************************************************
    operator value_type() const
    {
      return value();
    }

    //*************************************************************************
    /// Get the value.
    //*************************************************************************
    value_type value() const
    {
      value_type v = value_type();

      unaligned_copy::copy_store_to_value(this->storage, v);

      return v;
    }

    //*************************************************************************
    /// Sets the storage for the type.
    //*************************************************************************
    void set_storage(pointer storage_)
    {
      this->storage = storage_;
    }

  private:

    unaligned_uint_type_ext() ETL_DELETE;
  };
  ETL_END_PACKED

  template <size_t Size_, int Endian_>
  ETL_CONSTANT int unaligned_uint_type_ext<Size_, Endian_>::Endian;

  template <size_t Size_, int Endian_>
  ETL_CONSTANT size_t unaligned_uint_type_ext<Size_, Endian_>::Size;

  template <size_t Size_, int Endian_>
  ETL_CONSTANT typename unaligned_uint_type_ext<Size_, Endian_>::value_type unaligned_uint_type_ext<Size_, Endian_>::Min_Value;

  template <size_t Size_, int Endian_>
  ETL_CONSTANT typename unaligned_uint_type_ext<Size_, Endian_>::value_type unaligned_uint_type_ext<Size_, Endian_>::Max_Value;

  //*************************************************************************
  /// unaligned_int_type_ext
  ///\brief Allows a signed, two's complement, integer of an arbitrary number
  ///       of bytes to be stored at an unaligned address. Uses an external
  ///       buffer.
  ///       The value is interfaced to via the smallest standard signed
  ///       integer type that can hold it, and is sign extended on reading.
  ///\tparam Size_   The number of bytes of storage.
  ///\tparam Endian_ The endianness of the stored integer.
  ///\note  Values that do not fit in 'Size_' bytes are truncated, in the same
  ///       way as a narrowing static_cast.
  //*************************************************************************
  template <size_t Size_, int Endian_>
  ETL_PACKED_CLASS(unaligned_int_type_ext)
    : public private_unaligned_type::unaligned_type_storage_ext<Size_>
  {
  public:

    template <size_t Size_Other, int Endian_Other>
    friend class unaligned_int_type_ext;

    typedef typename private_unaligned_type::interface_int<Size_>::type value_type;

    typedef private_unaligned_type::unaligned_copy<Size_, Endian_, true> unaligned_copy;

    typedef typename private_unaligned_type::unaligned_type_storage_ext<Size_>::storage_type           storage_type;
    typedef typename private_unaligned_type::unaligned_type_storage_ext<Size_>::pointer                pointer;
    typedef typename private_unaligned_type::unaligned_type_storage_ext<Size_>::const_pointer          const_pointer;
    typedef typename private_unaligned_type::unaligned_type_storage_ext<Size_>::iterator               iterator;
    typedef typename private_unaligned_type::unaligned_type_storage_ext<Size_>::const_iterator         const_iterator;
    typedef typename private_unaligned_type::unaligned_type_storage_ext<Size_>::reverse_iterator       reverse_iterator;
    typedef typename private_unaligned_type::unaligned_type_storage_ext<Size_>::const_reverse_iterator const_reverse_iterator;

    static ETL_CONSTANT int        Endian = Endian_;
    static ETL_CONSTANT size_t     Size   = Size_;
    static ETL_CONSTANT value_type Max_Value =
      static_cast<value_type>(etl::integral_limits<typename etl::unsigned_type<value_type>::type>::max >> (((sizeof(value_type) - Size_) * 8U) + 1U));
    static ETL_CONSTANT value_type Min_Value = static_cast<value_type>(-Max_Value - 1);

    //*************************************************************************
    /// Construct from a storage pointer
    //*************************************************************************
    unaligned_int_type_ext(pointer storage_)
      : private_unaligned_type::unaligned_type_storage_ext<Size_>(storage_)
    {
    }

    //*************************************************************************
    /// Construct from a value and storage pointer
    //*************************************************************************
    unaligned_int_type_ext(value_type value, pointer storage_)
      : private_unaligned_type::unaligned_type_storage_ext<Size_>(storage_)
    {
      unaligned_copy::copy_value_to_store(value, this->storage);
    }

    //*************************************************************************
    /// Copy constructor with storage pointer
    /// \note 'storage_' must either be the same storage as 'other's, or refer
    /// to completely separate storage. Partially overlapping storage is
    /// undefined behaviour.
    //*************************************************************************
    template <int Endian_Other>
    unaligned_int_type_ext(const unaligned_int_type_ext<Size_, Endian_Other>& other, pointer storage_)
      : private_unaligned_type::unaligned_type_storage_ext<Size_>(storage_)
    {
      unaligned_copy::copy_store_to_store(other.data(), Endian_Other, this->storage);
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Move constructor
    //*************************************************************************
    unaligned_int_type_ext(unaligned_int_type_ext<Size_, Endian> && other)
      : private_unaligned_type::unaligned_type_storage_ext<Size_>(other.storage)
    {
      other.storage = ETL_NULLPTR;
    }

    //*************************************************************************
    /// Move constructor from other endianness
    //*************************************************************************
    template <int Endian_Other>
    unaligned_int_type_ext(unaligned_int_type_ext<Size_, Endian_Other> && other)
      : private_unaligned_type::unaligned_type_storage_ext<Size_>(other.storage)
    {
      // If we're constructing from a different endianness then we need to
      // reverse the data order.
      if (Endian != Endian_Other)
      {
        etl::reverse(this->begin(), this->end());
      }

      other.storage = ETL_NULLPTR;
    }
#endif

    //*************************************************************************
    /// Assignment operator
    //*************************************************************************
    unaligned_int_type_ext& operator=(value_type value)
    {
      unaligned_copy::copy_value_to_store(value, this->storage);

      return *this;
    }

    //*************************************************************************
    /// Copy assignment operator.
    //*************************************************************************
    unaligned_int_type_ext& operator=(const unaligned_int_type_ext<Size_, Endian>& other)
    {
      unaligned_copy::copy_store_to_store(other.data(), Endian, this->storage);

      return *this;
    }

    //*************************************************************************
    /// Copy assignment operator from other endianness.
    /// \note The storage of 'other' must either be the same storage as this
    /// object's, or completely separate. Partially overlapping storage is
    /// undefined behaviour.
    //*************************************************************************
    template <int Endian_Other>
    unaligned_int_type_ext& operator=(const unaligned_int_type_ext<Size_, Endian_Other>& other)
    {
      unaligned_copy::copy_store_to_store(other.data(), Endian_Other, this->storage);

      return *this;
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Move assignment operator.
    //*************************************************************************
    unaligned_int_type_ext& operator=(unaligned_int_type_ext<Size_, Endian>&& other)
    {
      this->storage = other.storage;
      other.storage = ETL_NULLPTR;

      return *this;
    }

    //*************************************************************************
    /// Move assignment operator from other endianness.
    //*************************************************************************
    template <int Endian_Other>
    unaligned_int_type_ext& operator=(unaligned_int_type_ext<Size_, Endian_Other>&& other)
    {
      this->storage = other.storage;

      // If we're assigning from a different endianness then we need to reverse
      // the data order.
      if (Endian != Endian_Other)
      {
        etl::reverse(this->begin(), this->end());
      }

      other.storage = ETL_NULLPTR;

      return *this;
    }
#endif

    //*************************************************************************
    /// Conversion operator
    //*************************************************************************
    operator value_type() const
    {
      return value();
    }

    //*************************************************************************
    /// Get the value.
    //*************************************************************************
    value_type value() const
    {
      typedef typename etl::unsigned_type<value_type>::type unsigned_t;

      unsigned_t uvalue = unsigned_t(0);

      unaligned_copy::copy_store_to_value(this->storage, uvalue);

      return private_unaligned_type::sign_extend<Size_, value_type>(uvalue);
    }

    //*************************************************************************
    /// Sets the storage for the type.
    //*************************************************************************
    void set_storage(pointer storage_)
    {
      this->storage = storage_;
    }

  private:

    unaligned_int_type_ext() ETL_DELETE;
  };
  ETL_END_PACKED

  template <size_t Size_, int Endian_>
  ETL_CONSTANT int unaligned_int_type<Size_, Endian_>::Endian;

  template <size_t Size_, int Endian_>
  ETL_CONSTANT size_t unaligned_int_type<Size_, Endian_>::Size;

  template <size_t Size_, int Endian_>
  ETL_CONSTANT typename unaligned_int_type<Size_, Endian_>::value_type unaligned_int_type<Size_, Endian_>::Min_Value;

  template <size_t Size_, int Endian_>
  ETL_CONSTANT typename unaligned_int_type<Size_, Endian_>::value_type unaligned_int_type<Size_, Endian_>::Max_Value;

  // 24 bit integers.
  typedef unaligned_uint_type<3U, etl::endian::little> le_uint24_t;
  typedef unaligned_uint_type<3U, etl::endian::big>    be_uint24_t;
  typedef be_uint24_t                                  net_uint24_t;
  typedef unaligned_int_type<3U, etl::endian::little>  le_int24_t;
  typedef unaligned_int_type<3U, etl::endian::big>     be_int24_t;
  typedef be_int24_t                                   net_int24_t;
#if ETL_HAS_CONSTEXPR_ENDIANNESS
  typedef unaligned_uint_type<3U, etl::endianness::value()> host_uint24_t;
  typedef unaligned_int_type<3U, etl::endianness::value()>  host_int24_t;
#endif

#if ETL_USING_64BIT_TYPES
  // 40, 48 and 56 bit integers.
  typedef unaligned_uint_type<5U, etl::endian::little> le_uint40_t;
  typedef unaligned_uint_type<5U, etl::endian::big>    be_uint40_t;
  typedef be_uint40_t                                  net_uint40_t;
  typedef unaligned_int_type<5U, etl::endian::little>  le_int40_t;
  typedef unaligned_int_type<5U, etl::endian::big>     be_int40_t;
  typedef be_int40_t                                   net_int40_t;

  typedef unaligned_uint_type<6U, etl::endian::little> le_uint48_t;
  typedef unaligned_uint_type<6U, etl::endian::big>    be_uint48_t;
  typedef be_uint48_t                                  net_uint48_t;
  typedef unaligned_int_type<6U, etl::endian::little>  le_int48_t;
  typedef unaligned_int_type<6U, etl::endian::big>     be_int48_t;
  typedef be_int48_t                                   net_int48_t;

  typedef unaligned_uint_type<7U, etl::endian::little> le_uint56_t;
  typedef unaligned_uint_type<7U, etl::endian::big>    be_uint56_t;
  typedef be_uint56_t                                  net_uint56_t;
  typedef unaligned_int_type<7U, etl::endian::little>  le_int56_t;
  typedef unaligned_int_type<7U, etl::endian::big>     be_int56_t;
  typedef be_int56_t                                   net_int56_t;

  #if ETL_HAS_CONSTEXPR_ENDIANNESS
  typedef unaligned_uint_type<5U, etl::endianness::value()> host_uint40_t;
  typedef unaligned_uint_type<6U, etl::endianness::value()> host_uint48_t;
  typedef unaligned_uint_type<7U, etl::endianness::value()> host_uint56_t;
  typedef unaligned_int_type<5U, etl::endianness::value()>  host_int40_t;
  typedef unaligned_int_type<6U, etl::endianness::value()>  host_int48_t;
  typedef unaligned_int_type<7U, etl::endianness::value()>  host_int56_t;
  #endif
#endif

  template <size_t Size_, int Endian_>
  ETL_CONSTANT int unaligned_int_type_ext<Size_, Endian_>::Endian;

  template <size_t Size_, int Endian_>
  ETL_CONSTANT size_t unaligned_int_type_ext<Size_, Endian_>::Size;

  template <size_t Size_, int Endian_>
  ETL_CONSTANT typename unaligned_int_type_ext<Size_, Endian_>::value_type unaligned_int_type_ext<Size_, Endian_>::Min_Value;

  template <size_t Size_, int Endian_>
  ETL_CONSTANT typename unaligned_int_type_ext<Size_, Endian_>::value_type unaligned_int_type_ext<Size_, Endian_>::Max_Value;

  // 24 bit integers, external buffer.
  typedef unaligned_uint_type_ext<3U, etl::endian::little> le_uint24_ext_t;
  typedef unaligned_uint_type_ext<3U, etl::endian::big>    be_uint24_ext_t;
  typedef be_uint24_ext_t                                  net_uint24_ext_t;
  typedef unaligned_int_type_ext<3U, etl::endian::little>  le_int24_ext_t;
  typedef unaligned_int_type_ext<3U, etl::endian::big>     be_int24_ext_t;
  typedef be_int24_ext_t                                   net_int24_ext_t;
#if ETL_HAS_CONSTEXPR_ENDIANNESS
  typedef unaligned_uint_type_ext<3U, etl::endianness::value()> host_uint24_ext_t;
  typedef unaligned_int_type_ext<3U, etl::endianness::value()>  host_int24_ext_t;
#endif

#if ETL_USING_64BIT_TYPES
  // 40, 48 and 56 bit integers, external buffer.
  typedef unaligned_uint_type_ext<5U, etl::endian::little> le_uint40_ext_t;
  typedef unaligned_uint_type_ext<5U, etl::endian::big>    be_uint40_ext_t;
  typedef be_uint40_ext_t                                  net_uint40_ext_t;
  typedef unaligned_int_type_ext<5U, etl::endian::little>  le_int40_ext_t;
  typedef unaligned_int_type_ext<5U, etl::endian::big>     be_int40_ext_t;
  typedef be_int40_ext_t                                   net_int40_ext_t;

  typedef unaligned_uint_type_ext<6U, etl::endian::little> le_uint48_ext_t;
  typedef unaligned_uint_type_ext<6U, etl::endian::big>    be_uint48_ext_t;
  typedef be_uint48_ext_t                                  net_uint48_ext_t;
  typedef unaligned_int_type_ext<6U, etl::endian::little>  le_int48_ext_t;
  typedef unaligned_int_type_ext<6U, etl::endian::big>     be_int48_ext_t;
  typedef be_int48_ext_t                                   net_int48_ext_t;

  typedef unaligned_uint_type_ext<7U, etl::endian::little> le_uint56_ext_t;
  typedef unaligned_uint_type_ext<7U, etl::endian::big>    be_uint56_ext_t;
  typedef be_uint56_ext_t                                  net_uint56_ext_t;
  typedef unaligned_int_type_ext<7U, etl::endian::little>  le_int56_ext_t;
  typedef unaligned_int_type_ext<7U, etl::endian::big>     be_int56_ext_t;
  typedef be_int56_ext_t                                   net_int56_ext_t;

  #if ETL_HAS_CONSTEXPR_ENDIANNESS
  typedef unaligned_uint_type_ext<5U, etl::endianness::value()> host_uint40_ext_t;
  typedef unaligned_uint_type_ext<6U, etl::endianness::value()> host_uint48_ext_t;
  typedef unaligned_uint_type_ext<7U, etl::endianness::value()> host_uint56_ext_t;
  typedef unaligned_int_type_ext<5U, etl::endianness::value()>  host_int40_ext_t;
  typedef unaligned_int_type_ext<6U, etl::endianness::value()>  host_int48_ext_t;
  typedef unaligned_int_type_ext<7U, etl::endianness::value()>  host_int56_ext_t;
  #endif
#endif

#if ETL_HAS_CONSTEXPR_ENDIANNESS
  // Host order
  typedef unaligned_type<char, etl::endianness::value()>               host_char_t;
  typedef unaligned_type<signed char, etl::endianness::value()>        host_schar_t;
  typedef unaligned_type<unsigned char, etl::endianness::value()>      host_uchar_t;
  typedef unaligned_type<short, etl::endianness::value()>              host_short_t;
  typedef unaligned_type<unsigned short, etl::endianness::value()>     host_ushort_t;
  typedef unaligned_type<int, etl::endianness::value()>                host_int_t;
  typedef unaligned_type<unsigned int, etl::endianness::value()>       host_uint_t;
  typedef unaligned_type<long, etl::endianness::value()>               host_long_t;
  typedef unaligned_type<unsigned long, etl::endianness::value()>      host_ulong_t;
  typedef unaligned_type<long long, etl::endianness::value()>          host_long_long_t;
  typedef unaligned_type<unsigned long long, etl::endianness::value()> host_ulong_long_t;
  #if ETL_USING_8BIT_TYPES
  typedef unaligned_type<int8_t, etl::endianness::value()>  host_int8_t;
  typedef unaligned_type<uint8_t, etl::endianness::value()> host_uint8_t;
  #endif
  typedef unaligned_type<int16_t, etl::endianness::value()>  host_int16_t;
  typedef unaligned_type<uint16_t, etl::endianness::value()> host_uint16_t;
  typedef unaligned_type<int32_t, etl::endianness::value()>  host_int32_t;
  typedef unaligned_type<uint32_t, etl::endianness::value()> host_uint32_t;
  #if ETL_USING_64BIT_TYPES
  typedef unaligned_type<int64_t, etl::endianness::value()>  host_int64_t;
  typedef unaligned_type<uint64_t, etl::endianness::value()> host_uint64_t;
  #endif
  typedef unaligned_type<float, etl::endianness::value()>       host_float_t;
  typedef unaligned_type<double, etl::endianness::value()>      host_double_t;
  typedef unaligned_type<long double, etl::endianness::value()> host_long_double_t;
#endif

  // Little Endian
  typedef unaligned_type<char, etl::endian::little>               le_char_t;
  typedef unaligned_type<signed char, etl::endian::little>        le_schar_t;
  typedef unaligned_type<unsigned char, etl::endian::little>      le_uchar_t;
  typedef unaligned_type<short, etl::endian::little>              le_short_t;
  typedef unaligned_type<unsigned short, etl::endian::little>     le_ushort_t;
  typedef unaligned_type<int, etl::endian::little>                le_int_t;
  typedef unaligned_type<unsigned int, etl::endian::little>       le_uint_t;
  typedef unaligned_type<long, etl::endian::little>               le_long_t;
  typedef unaligned_type<unsigned long, etl::endian::little>      le_ulong_t;
  typedef unaligned_type<long long, etl::endian::little>          le_long_long_t;
  typedef unaligned_type<unsigned long long, etl::endian::little> le_ulong_long_t;
#if ETL_USING_8BIT_TYPES
  typedef unaligned_type<int8_t, etl::endian::little>  le_int8_t;
  typedef unaligned_type<uint8_t, etl::endian::little> le_uint8_t;
#endif
  typedef unaligned_type<int16_t, etl::endian::little>  le_int16_t;
  typedef unaligned_type<uint16_t, etl::endian::little> le_uint16_t;
  typedef unaligned_type<int32_t, etl::endian::little>  le_int32_t;
  typedef unaligned_type<uint32_t, etl::endian::little> le_uint32_t;
#if ETL_USING_64BIT_TYPES
  typedef unaligned_type<int64_t, etl::endian::little>  le_int64_t;
  typedef unaligned_type<uint64_t, etl::endian::little> le_uint64_t;
#endif
  typedef unaligned_type<float, etl::endian::little>       le_float_t;
  typedef unaligned_type<double, etl::endian::little>      le_double_t;
  typedef unaligned_type<long double, etl::endian::little> le_long_double_t;

  // Big Endian
  typedef unaligned_type<char, etl::endian::big>               be_char_t;
  typedef unaligned_type<signed char, etl::endian::big>        be_schar_t;
  typedef unaligned_type<unsigned char, etl::endian::big>      be_uchar_t;
  typedef unaligned_type<short, etl::endian::big>              be_short_t;
  typedef unaligned_type<unsigned short, etl::endian::big>     be_ushort_t;
  typedef unaligned_type<int, etl::endian::big>                be_int_t;
  typedef unaligned_type<unsigned int, etl::endian::big>       be_uint_t;
  typedef unaligned_type<long, etl::endian::big>               be_long_t;
  typedef unaligned_type<unsigned long, etl::endian::big>      be_ulong_t;
  typedef unaligned_type<long long, etl::endian::big>          be_long_long_t;
  typedef unaligned_type<unsigned long long, etl::endian::big> be_ulong_long_t;
#if ETL_USING_8BIT_TYPES
  typedef unaligned_type<int8_t, etl::endian::big>  be_int8_t;
  typedef unaligned_type<uint8_t, etl::endian::big> be_uint8_t;
#endif
  typedef unaligned_type<int16_t, etl::endian::big>  be_int16_t;
  typedef unaligned_type<uint16_t, etl::endian::big> be_uint16_t;
  typedef unaligned_type<int32_t, etl::endian::big>  be_int32_t;
  typedef unaligned_type<uint32_t, etl::endian::big> be_uint32_t;
#if ETL_USING_64BIT_TYPES
  typedef unaligned_type<int64_t, etl::endian::big>  be_int64_t;
  typedef unaligned_type<uint64_t, etl::endian::big> be_uint64_t;
#endif
  typedef unaligned_type<float, etl::endian::big>       be_float_t;
  typedef unaligned_type<double, etl::endian::big>      be_double_t;
  typedef unaligned_type<long double, etl::endian::big> be_long_double_t;

  // Network Order
  typedef be_char_t       net_char_t;
  typedef be_schar_t      net_schar_t;
  typedef be_uchar_t      net_uchar_t;
  typedef be_short_t      net_short_t;
  typedef be_ushort_t     net_ushort_t;
  typedef be_int_t        net_int_t;
  typedef be_uint_t       net_uint_t;
  typedef be_long_t       net_long_t;
  typedef be_ulong_t      net_ulong_t;
  typedef be_long_long_t  net_long_long_t;
  typedef be_ulong_long_t net_ulong_long_t;
#if ETL_USING_8BIT_TYPES
  typedef be_int8_t  net_int8_t;
  typedef be_uint8_t net_uint8_t;
#endif
  typedef be_int16_t  net_int16_t;
  typedef be_uint16_t net_uint16_t;
  typedef be_int32_t  net_int32_t;
  typedef be_uint32_t net_uint32_t;
#if ETL_USING_64BIT_TYPES
  typedef be_int64_t  net_int64_t;
  typedef be_uint64_t net_uint64_t;
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

#if ETL_HAS_CONSTEXPR_ENDIANNESS
  // Host order
  typedef unaligned_type_ext<char, etl::endianness::value()>               host_char_ext_t;
  typedef unaligned_type_ext<signed char, etl::endianness::value()>        host_schar_ext_t;
  typedef unaligned_type_ext<unsigned char, etl::endianness::value()>      host_uchar_ext_t;
  typedef unaligned_type_ext<short, etl::endianness::value()>              host_short_ext_t;
  typedef unaligned_type_ext<unsigned short, etl::endianness::value()>     host_ushort_ext_t;
  typedef unaligned_type_ext<int, etl::endianness::value()>                host_int_ext_t;
  typedef unaligned_type_ext<unsigned int, etl::endianness::value()>       host_uint_ext_t;
  typedef unaligned_type_ext<long, etl::endianness::value()>               host_long_ext_t;
  typedef unaligned_type_ext<unsigned long, etl::endianness::value()>      host_ulong_ext_t;
  typedef unaligned_type_ext<long long, etl::endianness::value()>          host_long_long_ext_t;
  typedef unaligned_type_ext<unsigned long long, etl::endianness::value()> host_ulong_long_ext_t;
  #if ETL_USING_8BIT_TYPES
  typedef unaligned_type_ext<int8_t, etl::endianness::value()>  host_int8_ext_t;
  typedef unaligned_type_ext<uint8_t, etl::endianness::value()> host_uint8_ext_t;
  #endif
  typedef unaligned_type_ext<int16_t, etl::endianness::value()>  host_int16_ext_t;
  typedef unaligned_type_ext<uint16_t, etl::endianness::value()> host_uint16_ext_t;
  typedef unaligned_type_ext<int32_t, etl::endianness::value()>  host_int32_ext_t;
  typedef unaligned_type_ext<uint32_t, etl::endianness::value()> host_uint32_ext_t;
  #if ETL_USING_64BIT_TYPES
  typedef unaligned_type_ext<int64_t, etl::endianness::value()>  host_int64_ext_t;
  typedef unaligned_type_ext<uint64_t, etl::endianness::value()> host_uint64_ext_t;
  #endif
  typedef unaligned_type_ext<float, etl::endianness::value()>       host_float_ext_t;
  typedef unaligned_type_ext<double, etl::endianness::value()>      host_double_ext_t;
  typedef unaligned_type_ext<long double, etl::endianness::value()> host_long_double_ext_t;
#endif

  // Little Endian
  typedef unaligned_type_ext<char, etl::endian::little>               le_char_ext_t;
  typedef unaligned_type_ext<signed char, etl::endian::little>        le_schar_ext_t;
  typedef unaligned_type_ext<unsigned char, etl::endian::little>      le_uchar_ext_t;
  typedef unaligned_type_ext<short, etl::endian::little>              le_short_ext_t;
  typedef unaligned_type_ext<unsigned short, etl::endian::little>     le_ushort_ext_t;
  typedef unaligned_type_ext<int, etl::endian::little>                le_int_ext_t;
  typedef unaligned_type_ext<unsigned int, etl::endian::little>       le_uint_ext_t;
  typedef unaligned_type_ext<long, etl::endian::little>               le_long_ext_t;
  typedef unaligned_type_ext<unsigned long, etl::endian::little>      le_ulong_ext_t;
  typedef unaligned_type_ext<long long, etl::endian::little>          le_long_long_ext_t;
  typedef unaligned_type_ext<unsigned long long, etl::endian::little> le_ulong_long_ext_t;
#if ETL_USING_8BIT_TYPES
  typedef unaligned_type_ext<int8_t, etl::endian::little>  le_int8_ext_t;
  typedef unaligned_type_ext<uint8_t, etl::endian::little> le_uint8_ext_t;
#endif
  typedef unaligned_type_ext<int16_t, etl::endian::little>  le_int16_ext_t;
  typedef unaligned_type_ext<uint16_t, etl::endian::little> le_uint16_ext_t;
  typedef unaligned_type_ext<int32_t, etl::endian::little>  le_int32_ext_t;
  typedef unaligned_type_ext<uint32_t, etl::endian::little> le_uint32_ext_t;
#if ETL_USING_64BIT_TYPES
  typedef unaligned_type_ext<int64_t, etl::endian::little>  le_int64_ext_t;
  typedef unaligned_type_ext<uint64_t, etl::endian::little> le_uint64_ext_t;
#endif
  typedef unaligned_type_ext<float, etl::endian::little>       le_float_ext_t;
  typedef unaligned_type_ext<double, etl::endian::little>      le_double_ext_t;
  typedef unaligned_type_ext<long double, etl::endian::little> le_long_double_ext_t;

  // Big Endian
  typedef unaligned_type_ext<char, etl::endian::big>               be_char_ext_t;
  typedef unaligned_type_ext<signed char, etl::endian::big>        be_schar_ext_t;
  typedef unaligned_type_ext<unsigned char, etl::endian::big>      be_uchar_ext_t;
  typedef unaligned_type_ext<short, etl::endian::big>              be_short_ext_t;
  typedef unaligned_type_ext<unsigned short, etl::endian::big>     be_ushort_ext_t;
  typedef unaligned_type_ext<int, etl::endian::big>                be_int_ext_t;
  typedef unaligned_type_ext<unsigned int, etl::endian::big>       be_uint_ext_t;
  typedef unaligned_type_ext<long, etl::endian::big>               be_long_ext_t;
  typedef unaligned_type_ext<unsigned long, etl::endian::big>      be_ulong_ext_t;
  typedef unaligned_type_ext<long long, etl::endian::big>          be_long_long_ext_t;
  typedef unaligned_type_ext<unsigned long long, etl::endian::big> be_ulong_long_ext_t;
#if ETL_USING_8BIT_TYPES
  typedef unaligned_type_ext<int8_t, etl::endian::big>  be_int8_ext_t;
  typedef unaligned_type_ext<uint8_t, etl::endian::big> be_uint8_ext_t;
#endif
  typedef unaligned_type_ext<int16_t, etl::endian::big>  be_int16_ext_t;
  typedef unaligned_type_ext<uint16_t, etl::endian::big> be_uint16_ext_t;
  typedef unaligned_type_ext<int32_t, etl::endian::big>  be_int32_ext_t;
  typedef unaligned_type_ext<uint32_t, etl::endian::big> be_uint32_ext_t;
#if ETL_USING_64BIT_TYPES
  typedef unaligned_type_ext<int64_t, etl::endian::big>  be_int64_ext_t;
  typedef unaligned_type_ext<uint64_t, etl::endian::big> be_uint64_ext_t;
#endif
  typedef unaligned_type_ext<float, etl::endian::big>       be_float_ext_t;
  typedef unaligned_type_ext<double, etl::endian::big>      be_double_ext_t;
  typedef unaligned_type_ext<long double, etl::endian::big> be_long_double_ext_t;

  // Network Order
  typedef be_char_ext_t       net_char_ext_t;
  typedef be_schar_ext_t      net_schar_ext_t;
  typedef be_uchar_ext_t      net_uchar_ext_t;
  typedef be_short_ext_t      net_short_ext_t;
  typedef be_ushort_ext_t     net_ushort_ext_t;
  typedef be_int_ext_t        net_int_ext_t;
  typedef be_uint_ext_t       net_uint_ext_t;
  typedef be_long_ext_t       net_long_ext_t;
  typedef be_ulong_ext_t      net_ulong_ext_t;
  typedef be_long_long_ext_t  net_long_long_ext_t;
  typedef be_ulong_long_ext_t net_ulong_long_ext_t;
#if ETL_USING_8BIT_TYPES
  typedef be_int8_ext_t  net_int8_ext_t;
  typedef be_uint8_ext_t net_uint8_ext_t;
#endif
  typedef be_int16_ext_t  net_int16_ext_t;
  typedef be_uint16_ext_t net_uint16_ext_t;
  typedef be_int32_ext_t  net_int32_ext_t;
  typedef be_uint32_ext_t net_uint32_ext_t;
#if ETL_USING_64BIT_TYPES
  typedef be_int64_ext_t  net_int64_ext_t;
  typedef be_uint64_ext_t net_uint64_ext_t;
#endif
  typedef be_float_ext_t       net_float_ext_t;
  typedef be_double_ext_t      net_double_ext_t;
  typedef be_long_double_ext_t net_long_double_ext_t;

#if ETL_USING_CPP11
  template <typename T, int Endian>
  using unaligned_type_ext_t = typename etl::unaligned_type_ext<T, Endian>::type;
#endif

#if ETL_USING_CPP17
  template <typename T, int Endian>
  constexpr size_t unaligned_type_ext_t_v = etl::unaligned_type_ext<T, Endian>::Size;
#endif
} // namespace etl

#endif
