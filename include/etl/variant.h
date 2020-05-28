///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

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

#ifndef ETL_VARIANT_INCLUDED
#define ETL_VARIANT_INCLUDED

#include <stdint.h>

#include <new>

#include "platform.h"
#include "utility.h"
#include "array.h"
#include "largest.h"
#include "exception.h"
#include "type_traits.h"
#include "integral_limits.h"
#include "static_assert.h"
#include "alignment.h"
#include "error_handler.h"
#include "null_type.h"

#if defined(ETL_COMPILER_KEIL)
  #pragma diag_suppress 940
  #pragma diag_suppress 111
#endif

#undef ETL_FILE
#define ETL_FILE "24"

//*****************************************************************************
///\defgroup variant variant
/// A class that can contain one a several specified types in a type safe manner.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  namespace private_variant
  {
    //*************************************************************************
    /// Placeholder for unused template parameters.
    /// This class is never instantiated.
    //*************************************************************************
    template <const size_t ID>
    struct no_type
    {
    };
  }

  //***************************************************************************
  /// Base exception for the variant class.
  ///\ingroup variant
  //***************************************************************************
  class variant_exception : public exception
  {
  public:
    variant_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// 'Unsupported type' exception for the variant class.
  ///\ingroup variant
  //***************************************************************************
  class variant_incorrect_type_exception : public variant_exception
  {
  public:
    variant_incorrect_type_exception(string_type file_name_, numeric_type line_number_)
      : variant_exception(ETL_ERROR_TEXT("variant: unsupported type", ETL_FILE"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// A template class that can store any of the types defined in the template parameter list.
  /// Supports up to 8 types.
  ///\ingroup variant
  //***************************************************************************
  template <typename T1,
            typename T2 = etl::null_type<2>,
            typename T3 = etl::null_type<3>,
            typename T4 = etl::null_type<4>,
            typename T5 = etl::null_type<5>,
            typename T6 = etl::null_type<6>,
            typename T7 = etl::null_type<7>,
            typename T8 = etl::null_type<8> >
  class variant
  {
  public:

    //***************************************************************************
    /// The type used for ids.
    //***************************************************************************
    typedef uint_least8_t type_id_t;

    //***************************************************************************
    /// The id a unsupported types.
    //***************************************************************************
    static const type_id_t UNSUPPORTED_TYPE_ID = integral_limits<type_id_t>::max;

  private:

    // All types of variant are friends.
    template <typename U1, typename U2, typename U3, typename U4, typename U5, typename U6, typename U7, typename U8>
    friend class variant;

    //***************************************************************************
    /// The largest type.
    //***************************************************************************
    typedef typename largest_type<T1, T2, T3, T4, T5, T6, T7, T8>::type largest_t;

    //***************************************************************************
    /// The largest size.
    //***************************************************************************
    static const size_t SIZE = sizeof(largest_t);

    //***************************************************************************
    /// The largest alignment.
    //***************************************************************************
    static const size_t ALIGNMENT = etl::largest_alignment<T1, T2, T3, T4, T5, T6, T7, T8>::value;

    //***************************************************************************
    /// Short form of no_type placeholders.
    //***************************************************************************
    typedef etl::null_type<2> no_type2;
    typedef etl::null_type<3> no_type3;
    typedef etl::null_type<4> no_type4;
    typedef etl::null_type<5> no_type5;
    typedef etl::null_type<6> no_type6;
    typedef etl::null_type<7> no_type7;
    typedef etl::null_type<8> no_type8;

    //***************************************************************************
    /// Lookup the id of type.
    //***************************************************************************
    template <typename T>
    struct Type_Id_Lookup
    {
      static const uint_least8_t type_id = etl::is_same<T, T1>::value ? 0 :
                                           etl::is_same<T, T2>::value ? 1 :
                                           etl::is_same<T, T3>::value ? 2 :
                                           etl::is_same<T, T4>::value ? 3 :
                                           etl::is_same<T, T5>::value ? 4 :
                                           etl::is_same<T, T6>::value ? 5 :
                                           etl::is_same<T, T7>::value ? 6 :
                                           etl::is_same<T, T8>::value ? 7 :
                                           UNSUPPORTED_TYPE_ID;
    };

    //***************************************************************************
    /// Lookup for the id of type.
    //***************************************************************************
    template <typename T>
    struct Type_Is_Supported : public integral_constant<bool,
                                                       is_same<T, T1>::value ||
                                                       is_same<T, T2>::value ||
                                                       is_same<T, T3>::value ||
                                                       is_same<T, T4>::value ||
                                                       is_same<T, T5>::value ||
                                                       is_same<T, T6>::value ||
                                                       is_same<T, T7>::value ||
                                                       is_same<T, T8>::value>
    {
    };

  public:

    //***************************************************************************
    /// Destructor.
    //***************************************************************************
    ~variant()
    {
      destruct_current();
    }

    //*************************************************************************
    //**** Reader types *******************************************************
    //*************************************************************************

    //*************************************************************************
    /// Base reader type functor class.
    /// Allows for typesafe access to the stored value types.
    /// Define the reader type for 8 types.
    //*************************************************************************
    template <typename R1, typename R2 = no_type2, typename R3 = no_type3, typename R4 = no_type4, typename R5 = no_type5, typename R6 = no_type6, typename R7 = no_type7, typename R8 = no_type8>
    class reader_type
    {
    public:

      friend class variant;

      virtual void read(typename etl::parameter_type<R1>::type value) = 0;
      virtual void read(typename etl::parameter_type<R2>::type value) = 0;
      virtual void read(typename etl::parameter_type<R3>::type value) = 0;
      virtual void read(typename etl::parameter_type<R4>::type value) = 0;
      virtual void read(typename etl::parameter_type<R5>::type value) = 0;
      virtual void read(typename etl::parameter_type<R6>::type value) = 0;
      virtual void read(typename etl::parameter_type<R7>::type value) = 0;
      virtual void read(typename etl::parameter_type<R8>::type value) = 0;
    };

    //*************************************************************************
    /// Define the reader type for 7 types.
    //*************************************************************************
    template <typename R1, typename R2, typename R3, typename R4, typename R5, typename R6, typename R7>
    class reader_type<R1, R2, R3, R4, R5, R6, R7, no_type8>
    {
    public:

      friend class variant;

      virtual void read(typename etl::parameter_type<R1>::type value) = 0;
      virtual void read(typename etl::parameter_type<R2>::type value) = 0;
      virtual void read(typename etl::parameter_type<R3>::type value) = 0;
      virtual void read(typename etl::parameter_type<R4>::type value) = 0;
      virtual void read(typename etl::parameter_type<R5>::type value) = 0;
      virtual void read(typename etl::parameter_type<R6>::type value) = 0;
      virtual void read(typename etl::parameter_type<R7>::type value) = 0;

    private:

      void read(no_type8&) {};
    };

    //*************************************************************************
    /// Define the reader type for 6 types.
    //*************************************************************************
    template <typename R1, typename R2, typename R3, typename R4, typename R5, typename R6>
    class reader_type<R1, R2, R3, R4, R5, R6, no_type7, no_type8>
    {
    public:

      friend class variant;

      virtual void read(typename etl::parameter_type<R1>::type value) = 0;
      virtual void read(typename etl::parameter_type<R2>::type value) = 0;
      virtual void read(typename etl::parameter_type<R3>::type value) = 0;
      virtual void read(typename etl::parameter_type<R4>::type value) = 0;
      virtual void read(typename etl::parameter_type<R5>::type value) = 0;
      virtual void read(typename etl::parameter_type<R6>::type value) = 0;

    private:

      void read(no_type7&) {};
      void read(no_type8&) {};
    };

    //*************************************************************************
    /// Define the reader type for 5 types.
    //*************************************************************************
    template <typename R1, typename R2, typename R3, typename R4, typename R5>
    class reader_type<R1, R2, R3, R4, R5, no_type6, no_type7, no_type8>
    {
    public:

      friend class variant;

      virtual void read(typename etl::parameter_type<R1>::type value) = 0;
      virtual void read(typename etl::parameter_type<R2>::type value) = 0;
      virtual void read(typename etl::parameter_type<R3>::type value) = 0;
      virtual void read(typename etl::parameter_type<R4>::type value) = 0;
      virtual void read(typename etl::parameter_type<R5>::type value) = 0;

    private:

      void read(no_type6&) {};
      void read(no_type7&) {};
      void read(no_type8&) {};
    };

    //*************************************************************************
    /// Define the reader type for 4 types.
    //*************************************************************************
    template <typename R1, typename R2, typename R3, typename R4>
    class reader_type<R1, R2, R3, R4, no_type5, no_type6, no_type7, no_type8>
    {
    public:

      friend class variant;

      virtual void read(typename etl::parameter_type<R1>::type value) = 0;
      virtual void read(typename etl::parameter_type<R2>::type value) = 0;
      virtual void read(typename etl::parameter_type<R3>::type value) = 0;
      virtual void read(typename etl::parameter_type<R4>::type value) = 0;

    private:

      void read(no_type5&) {};
      void read(no_type6&) {};
      void read(no_type7&) {};
      void read(no_type8&) {};
    };

    //*************************************************************************
    /// Define the reader type for 3 types.
    //*************************************************************************
    template <typename R1, typename R2, typename R3>
    class reader_type<R1, R2, R3, no_type4, no_type5, no_type6, no_type7, no_type8>
    {
    public:

      friend class variant;

      virtual void read(typename etl::parameter_type<R1>::type value) = 0;
      virtual void read(typename etl::parameter_type<R2>::type value) = 0;
      virtual void read(typename etl::parameter_type<R3>::type value) = 0;

    private:

      void read(no_type4&) {};
      void read(no_type5&) {};
      void read(no_type6&) {};
      void read(no_type7&) {};
      void read(no_type8&) {};
    };

    //*************************************************************************
    /// Define the reader type for 2 types.
    //*************************************************************************
    template <typename R1, typename R2>
    class reader_type<R1, R2, no_type3, no_type4, no_type5, no_type6, no_type7, no_type8>
    {
    public:

      friend class variant;

      virtual void read(typename etl::parameter_type<R1>::type value) = 0;
      virtual void read(typename etl::parameter_type<R2>::type value) = 0;

    private:

      void read(no_type3&) {};
      void read(no_type4&) {};
      void read(no_type5&) {};
      void read(no_type6&) {};
      void read(no_type7&) {};
      void read(no_type8&) {};
    };

    //*************************************************************************
    /// Define the reader type for 1 type.
    //*************************************************************************
    template <typename R1>
    class reader_type<R1, no_type2, no_type3, no_type4, no_type5, no_type6, no_type7, no_type8>
    {
    public:

      friend class variant;

      virtual void read(typename etl::parameter_type<R1>::type value) = 0;

    private:

      void read(no_type2&) {};
      void read(no_type3&) {};
      void read(no_type4&) {};
      void read(no_type5&) {};
      void read(no_type6&) {};
      void read(no_type7&) {};
      void read(no_type8&) {};
    };

    //*************************************************************************
    //**** Up-cast functors ***************************************************
    //*************************************************************************

    //*************************************************************************
    /// Base upcast_functor for eight types.
    //*************************************************************************
    template <typename TBase, typename U1, typename U2 = no_type2, typename U3 = no_type3, typename U4 = no_type4, typename U5 = no_type5, typename U6 = no_type6, typename U7 = no_type7, typename U8 = no_type8>
    class upcast_functor
    {
    public:

      TBase& operator()(uint_least8_t* p_data, uint_least8_t typeId)
      {
        switch (typeId)
        {
          case 0: return reinterpret_cast<U1&>(*p_data);
          case 1: return reinterpret_cast<U2&>(*p_data);
          case 2: return reinterpret_cast<U3&>(*p_data);
          case 3: return reinterpret_cast<U4&>(*p_data);
          case 4: return reinterpret_cast<U5&>(*p_data);
          case 5: return reinterpret_cast<U6&>(*p_data);
          case 6: return reinterpret_cast<U7&>(*p_data);
          case 7: return reinterpret_cast<U8&>(*p_data);
          default: return reinterpret_cast<TBase&>(*reinterpret_cast<TBase*>(0));
        }
      }

      const TBase& operator()(uint_least8_t* p_data, uint_least8_t typeId) const
      {
        switch (typeId)
        {
          case 0: return reinterpret_cast<const U1&>(*p_data);
          case 1: return reinterpret_cast<const U2&>(*p_data);
          case 2: return reinterpret_cast<const U3&>(*p_data);
          case 3: return reinterpret_cast<const U4&>(*p_data);
          case 4: return reinterpret_cast<const U5&>(*p_data);
          case 5: return reinterpret_cast<const U6&>(*p_data);
          case 6: return reinterpret_cast<const U7&>(*p_data);
          case 7: return reinterpret_cast<const U8&>(*p_data);
          default: return reinterpret_cast<const TBase&>(*reinterpret_cast<const TBase*>(0));
        }
      }
    };

    //*************************************************************************
    /// Upcast_functor for seven types.
    //*************************************************************************
    template <typename TBase, typename U1, typename U2, typename U3, typename U4, typename U5, typename U6, typename U7>
    class upcast_functor<TBase, U1, U2, U3, U4, U5, U6, U7, no_type8>
    {
    public:

      TBase& operator()(uint_least8_t* p_data, uint_least8_t typeId)
      {
        switch (typeId)
        {
          case 0: return reinterpret_cast<U1&>(*p_data);
          case 1: return reinterpret_cast<U2&>(*p_data);
          case 2: return reinterpret_cast<U3&>(*p_data);
          case 3: return reinterpret_cast<U4&>(*p_data);
          case 4: return reinterpret_cast<U5&>(*p_data);
          case 5: return reinterpret_cast<U6&>(*p_data);
          case 6: return reinterpret_cast<U7&>(*p_data);
          default: return reinterpret_cast<TBase&>(*reinterpret_cast<TBase*>(0));
        }
      }

      const TBase& operator()(uint_least8_t* p_data, uint_least8_t typeId) const
      {
        switch (typeId)
        {
          case 0: return reinterpret_cast<const U1&>(*p_data);
          case 1: return reinterpret_cast<const U2&>(*p_data);
          case 2: return reinterpret_cast<const U3&>(*p_data);
          case 3: return reinterpret_cast<const U4&>(*p_data);
          case 4: return reinterpret_cast<const U5&>(*p_data);
          case 5: return reinterpret_cast<const U6&>(*p_data);
          case 6: return reinterpret_cast<const U7&>(*p_data);
          default: return reinterpret_cast<const TBase&>(*reinterpret_cast<const TBase*>(0));
        }
      }
    };

    //*************************************************************************
    /// Upcast_functor for six types.
    //*************************************************************************
    template <typename TBase, typename U1, typename U2, typename U3, typename U4, typename U5, typename U6>
    class upcast_functor<TBase, U1, U2, U3, U4, U5, U6, no_type7, no_type8>
    {
    public:

      TBase& operator()(uint_least8_t* p_data, uint_least8_t typeId)
      {
        switch (typeId)
        {
          case 0: return reinterpret_cast<U1&>(*p_data);
          case 1: return reinterpret_cast<U2&>(*p_data);
          case 2: return reinterpret_cast<U3&>(*p_data);
          case 3: return reinterpret_cast<U4&>(*p_data);
          case 4: return reinterpret_cast<U5&>(*p_data);
          case 5: return reinterpret_cast<U6&>(*p_data);
          default: return reinterpret_cast<TBase&>(*reinterpret_cast<TBase*>(0));
        }
      }

      const TBase& operator()(uint_least8_t* p_data, uint_least8_t typeId) const
      {
        switch (typeId)
        {
          case 0: return reinterpret_cast<const U1&>(*p_data);
          case 1: return reinterpret_cast<const U2&>(*p_data);
          case 2: return reinterpret_cast<const U3&>(*p_data);
          case 3: return reinterpret_cast<const U4&>(*p_data);
          case 4: return reinterpret_cast<const U5&>(*p_data);
          case 5: return reinterpret_cast<const U6&>(*p_data);
          default: return reinterpret_cast<const TBase&>(*reinterpret_cast<const TBase*>(0));
        }
      }
    };

    //*************************************************************************
    /// Upcast_functor for five types.
    //*************************************************************************
    template <typename TBase, typename U1, typename U2, typename U3, typename U4, typename U5>
    class upcast_functor<TBase, U1, U2, U3, U4, U5, no_type6, no_type7, no_type8>
    {
    public:

      TBase& operator()(uint_least8_t* p_data, uint_least8_t typeId)
      {
        switch (typeId)
        {
          case 0: return reinterpret_cast<U1&>(*p_data);
          case 1: return reinterpret_cast<U2&>(*p_data);
          case 2: return reinterpret_cast<U3&>(*p_data);
          case 3: return reinterpret_cast<U4&>(*p_data);
          case 4: return reinterpret_cast<U5&>(*p_data);
          default: return reinterpret_cast<TBase&>(*reinterpret_cast<TBase*>(0));
        }
      }

      const TBase& operator()(uint_least8_t* p_data, uint_least8_t typeId) const
      {
        switch (typeId)
        {
          case 0: return reinterpret_cast<const U1&>(*p_data);
          case 1: return reinterpret_cast<const U2&>(*p_data);
          case 2: return reinterpret_cast<const U3&>(*p_data);
          case 3: return reinterpret_cast<const U4&>(*p_data);
          case 4: return reinterpret_cast<const U5&>(*p_data);
          default: return reinterpret_cast<const TBase&>(*reinterpret_cast<const TBase*>(0));
        }
      }
    };

    //*************************************************************************
    /// Upcast_functor for four types.
    //*************************************************************************
    template <typename TBase, typename U1, typename U2, typename U3, typename U4>
    class upcast_functor<TBase, U1, U2, U3, U4, no_type5, no_type6, no_type7, no_type8>
    {
    public:

      TBase& operator()(uint_least8_t* p_data, uint_least8_t typeId)
      {
        switch (typeId)
        {
          case 0: return reinterpret_cast<U1&>(*p_data);
          case 1: return reinterpret_cast<U2&>(*p_data);
          case 2: return reinterpret_cast<U3&>(*p_data);
          case 3: return reinterpret_cast<U4&>(*p_data);
          default: return reinterpret_cast<TBase&>(*reinterpret_cast<TBase*>(0));
        }
      }

      const TBase& operator()(uint_least8_t* p_data, uint_least8_t typeId) const
      {
        switch (typeId)
        {
          case 0: return reinterpret_cast<const U1&>(*p_data);
          case 1: return reinterpret_cast<const U2&>(*p_data);
          case 2: return reinterpret_cast<const U3&>(*p_data);
          case 3: return reinterpret_cast<const U4&>(*p_data);
          default: return reinterpret_cast<const TBase&>(*reinterpret_cast<const TBase*>(0));
        }
      }
    };

    //*************************************************************************
    /// Upcast_functor for three types.
    //*************************************************************************
    template <typename TBase, typename U1, typename U2, typename U3>
    class upcast_functor<TBase, U1, U2, U3, no_type4, no_type5, no_type6, no_type7, no_type8>
    {
    public:

      TBase& operator()(uint_least8_t* p_data, uint_least8_t typeId)
      {
        switch (typeId)
        {
          case 0: return reinterpret_cast<U1&>(*p_data);
          case 1: return reinterpret_cast<U2&>(*p_data);
          case 2: return reinterpret_cast<U3&>(*p_data);
          default: return reinterpret_cast<TBase&>(*reinterpret_cast<TBase*>(0));
        }
      }

      const TBase& operator()(uint_least8_t* p_data, uint_least8_t typeId) const
      {
        switch (typeId)
        {
          case 0: return reinterpret_cast<const U1&>(*p_data);
          case 1: return reinterpret_cast<const U2&>(*p_data);
          case 2: return reinterpret_cast<const U3&>(*p_data);
          default: return reinterpret_cast<const TBase&>(*reinterpret_cast<const TBase*>(0));
        }
      }
    };

    //*************************************************************************
    /// Upcast_functor for two types.
    //*************************************************************************
    template <typename TBase, typename U1, typename U2>
    class upcast_functor<TBase, U1, U2, no_type3, no_type4, no_type5, no_type6, no_type7, no_type8>
    {
    public:

      TBase& operator()(uint_least8_t* p_data, uint_least8_t typeId)
      {
        switch (typeId)
        {
          case 0: return reinterpret_cast<U1&>(*p_data);
          case 1: return reinterpret_cast<U2&>(*p_data);
          default: return reinterpret_cast<TBase&>(*reinterpret_cast<TBase*>(0));
        }
      }

      const TBase& operator()(uint_least8_t* p_data, uint_least8_t typeId) const
      {
        switch (typeId)
        {
          case 0: return reinterpret_cast<const U1&>(*p_data);
          case 1: return reinterpret_cast<const U2&>(*p_data);
          default: return reinterpret_cast<const TBase&>(*reinterpret_cast<const TBase*>(0));
        }
      }
    };

    //*************************************************************************
    /// Upcast_functor for one type.
    //*************************************************************************
    template <typename TBase, typename U1>
    class upcast_functor<TBase, U1, no_type2, no_type3, no_type4, no_type5, no_type6, no_type7, no_type8>
    {
    public:

      TBase& operator()(uint_least8_t* p_data, uint_least8_t typeId)
      {
        switch (typeId)
        {
          case 0: return reinterpret_cast<U1&>(*p_data);
          default: return reinterpret_cast<TBase&>(*reinterpret_cast<TBase*>(0));
        }
      }

      const TBase& operator()(uint_least8_t* p_data, uint_least8_t typeId) const
      {
        switch (typeId)
        {
          case 0: return reinterpret_cast<const U1&>(*p_data);
          default: return reinterpret_cast<const TBase&>(*reinterpret_cast<const TBase*>(0));
        }
      }
    };

    //***************************************************************************
    /// The base type for derived readers.
    //***************************************************************************
    typedef reader_type<T1, T2, T3, T4, T5, T6, T7, T8> reader;

    //***************************************************************************
    /// Default constructor.
    /// Sets the state of the instance to containing no valid data.
    //***************************************************************************
    variant()
      : type_id(UNSUPPORTED_TYPE_ID)
    {
    }

    //***************************************************************************
    /// Constructor that catches any types that are not supported.
    /// Forces a ETL_STATIC_ASSERT.
    //***************************************************************************
    template <typename T>
    variant(const T& value)
    {
      ETL_STATIC_ASSERT(Type_Is_Supported<T>::value, "Unsupported type");

      ::new (static_cast<T*>(data)) T(value);
      type_id = Type_Id_Lookup<T>::type_id;
    }

    //***************************************************************************
    /// Copy constructor.
    ///\param other The other variant object to copy.
    //***************************************************************************
    variant(const variant& other)
    {
      switch (other.type_id)
      {
        case 0:  ::new (static_cast<T1*>(data)) T1(other.get<T1>()); break;
        case 1:  ::new (static_cast<T2*>(data)) T2(other.get<T2>()); break;
        case 2:  ::new (static_cast<T3*>(data)) T3(other.get<T3>()); break;
        case 3:  ::new (static_cast<T4*>(data)) T4(other.get<T4>()); break;
        case 4:  ::new (static_cast<T5*>(data)) T5(other.get<T5>()); break;
        case 5:  ::new (static_cast<T6*>(data)) T6(other.get<T6>()); break;
        case 6:  ::new (static_cast<T7*>(data)) T7(other.get<T7>()); break;
        case 7:  ::new (static_cast<T8*>(data)) T8(other.get<T8>()); break;
        default: break;
      }

      type_id = other.type_id;
    }

#if ETL_CPP11_SUPPORTED && ETL_NOT_USING_STLPORT && !defined(ETL_VARIANT_FORCE_CPP03)
    //*************************************************************************
    /// Emplace with variadic constructor parameters.
    //*************************************************************************
    template <typename T, typename... Args>
    T& emplace(Args&&... args)
    {
      ETL_STATIC_ASSERT(Type_Is_Supported<T>::value, "Unsupported type");

      destruct_current();
      ::new (static_cast<T*>(data)) T(etl::forward<Args>(args)...);
      type_id = Type_Id_Lookup<T>::type_id;

      return *static_cast<T*>(data);
    }
#else
    //***************************************************************************
    /// Emplace with one constructor parameter.
    //***************************************************************************
    template <typename T, typename TP1>
    T& emplace(const TP1& value1)
    {
      ETL_STATIC_ASSERT(Type_Is_Supported<T>::value, "Unsupported type");

      destruct_current();
      ::new (static_cast<T*>(data)) T(value1);
      type_id = Type_Id_Lookup<T>::type_id;

      return *static_cast<T*>(data);
    }

    //***************************************************************************
    /// Emplace with two constructor parameters.
    //***************************************************************************
    template <typename T, typename TP1, typename TP2>
    T& emplace(const TP1& value1, const TP2& value2)
    {
      ETL_STATIC_ASSERT(Type_Is_Supported<T>::value, "Unsupported type");

      destruct_current();
      ::new (static_cast<T*>(data)) T(value1, value2);
      type_id = Type_Id_Lookup<T>::type_id;

      return *static_cast<T*>(data);
    }

    //***************************************************************************
    /// Emplace with three constructor parameters.
    //***************************************************************************
    template <typename T, typename TP1, typename TP2, typename TP3>
    T& emplace(const TP1& value1, const TP2& value2, const TP3& value3)
    {
      ETL_STATIC_ASSERT(Type_Is_Supported<T>::value, "Unsupported type");

      destruct_current();
      ::new (static_cast<T*>(data)) T(value1, value2, value3);
      type_id = Type_Id_Lookup<T>::type_id;

      return *static_cast<T*>(data);
    }

    //***************************************************************************
    /// Emplace with four constructor parameters.
    //***************************************************************************
    template <typename T, typename TP1, typename TP2, typename TP3, typename TP4>
    T& emplace(const TP1& value1, const TP2& value2, const TP3& value3, const TP4& value4)
    {
      ETL_STATIC_ASSERT(Type_Is_Supported<T>::value, "Unsupported type");

      destruct_current();
      ::new (static_cast<T*>(data)) T(value1, value2, value3, value4);
      type_id = Type_Id_Lookup<T>::type_id;

      return *static_cast<T*>(data);
    }
#endif

    //***************************************************************************
    /// Assignment operator for T1 type.
    ///\param value The value to assign.
    //***************************************************************************
    template <typename T>
    variant& operator =(const T& value)
    {
      ETL_STATIC_ASSERT(Type_Is_Supported<T>::value, "Unsupported type");

      destruct_current();
      ::new (static_cast<T*>(data)) T(value);
      type_id = Type_Id_Lookup<T>::type_id;

      return *this;
    }

    //***************************************************************************
    /// Assignment operator for variant type.
    ///\param other The variant to assign.
    //***************************************************************************
    variant& operator =(const variant& other)
    {
      if (this != &other)
      {
        destruct_current();

        switch (other.type_id)
        {
        case 0:  ::new (static_cast<T1*>(data)) T1(other.get<T1>()); break;
        case 1:  ::new (static_cast<T2*>(data)) T2(other.get<T2>()); break;
        case 2:  ::new (static_cast<T3*>(data)) T3(other.get<T3>()); break;
        case 3:  ::new (static_cast<T4*>(data)) T4(other.get<T4>()); break;
        case 4:  ::new (static_cast<T5*>(data)) T5(other.get<T5>()); break;
        case 5:  ::new (static_cast<T6*>(data)) T6(other.get<T6>()); break;
        case 6:  ::new (static_cast<T7*>(data)) T7(other.get<T7>()); break;
        case 7:  ::new (static_cast<T8*>(data)) T8(other.get<T8>()); break;
        default: break;
        }

        type_id = other.type_id;
      }

      return *this;
    }

    //***************************************************************************
    /// Checks if the type is the same as the current stored type.
    /// For variants with the same type declarations.
    ///\return <b>true</b> if the types are the same, otherwise <b>false</b>.
    //***************************************************************************
    bool is_same_type(const variant& other) const
    {
      return type_id == other.type_id;
    }

    //***************************************************************************
    /// Checks if the type is the same as the current stored type.
    /// For variants with differing declarations.
    ///\return <b>true</b> if the types are the same, otherwise <b>false</b>.
    //***************************************************************************
    template <typename U1, typename U2, typename U3, typename U4, typename U5, typename U6, typename U7, typename U8>
    bool is_same_type(const variant<U1, U2, U3, U4, U5, U6, U7, U8>& other) const
    {
      bool is_same = false;

      switch (other.type_id)
      {
        case 0: is_same = (type_id == Type_Id_Lookup<U1>::type_id); break;
        case 1: is_same = (type_id == Type_Id_Lookup<U2>::type_id); break;
        case 2: is_same = (type_id == Type_Id_Lookup<U3>::type_id); break;
        case 3: is_same = (type_id == Type_Id_Lookup<U4>::type_id); break;
        case 4: is_same = (type_id == Type_Id_Lookup<U5>::type_id); break;
        case 5: is_same = (type_id == Type_Id_Lookup<U6>::type_id); break;
        case 6: is_same = (type_id == Type_Id_Lookup<U7>::type_id); break;
        case 7: is_same = (type_id == Type_Id_Lookup<U8>::type_id); break;
        default: break;
      }

      return is_same;
    }

    //***************************************************************************
    /// Calls the supplied reader instance.
    /// The 'read' function appropriate to the current type is called with the stored value.
    //***************************************************************************
    void call(reader& r)
    {
      switch (type_id)
      {
        case 0: r.read(static_cast<T1&>(data)); break;
        case 1: r.read(static_cast<T2&>(data)); break;
        case 2: r.read(static_cast<T3&>(data)); break;
        case 3: r.read(static_cast<T4&>(data)); break;
        case 4: r.read(static_cast<T5&>(data)); break;
        case 5: r.read(static_cast<T6&>(data)); break;
        case 6: r.read(static_cast<T7&>(data)); break;
        case 7: r.read(static_cast<T8&>(data)); break;
        default: break;
      }
    }

    //***************************************************************************
    /// Checks whether a valid value is currently stored.
    ///\return <b>true</b> if the value is valid, otherwise <b>false</b>.
    //***************************************************************************
    bool is_valid() const
    {
      return type_id != UNSUPPORTED_TYPE_ID;
    }

    //***************************************************************************
    /// Checks to see if the type currently stored is the same as that specified in the template parameter.
    ///\return <b>true</b> if it is the specified type, otherwise <b>false</b>.
    //***************************************************************************
    template <typename T>
    bool is_type() const
    {
      return type_id == Type_Id_Lookup<T>::type_id;
    }

    //***************************************************************************
    /// Gets the index of the type currently stored or UNSUPPORTED_TYPE_ID
    //***************************************************************************
    size_t index() const
    {
      return type_id;
    }

    //***************************************************************************
    /// Clears the value to 'no valid stored value'.
    //***************************************************************************
    void clear()
    {
      destruct_current();
    }

    //***************************************************************************
    /// Gets the value stored as the specified template type.
    /// Throws a variant_incorrect_type_exception if the actual type is not that specified.
    ///\return A reference to the value.
    //***************************************************************************
    template <typename T>
    T& get()
    {
      ETL_STATIC_ASSERT(Type_Is_Supported<T>::value, "Unsupported type");
      ETL_ASSERT(is_type<T>(), ETL_ERROR(variant_incorrect_type_exception));

      return static_cast<T&>(data);
    }

    //***************************************************************************
    /// Gets the value stored as the specified template type.
    /// Throws a variant_incorrect_type_exception if the actual type is not that specified.
    ///\return A const reference to the value.
    //***************************************************************************
    template <typename T>
    const T& get() const
    {
      ETL_STATIC_ASSERT(Type_Is_Supported<T>::value, "Unsupported type");
      ETL_ASSERT(is_type<T>(), ETL_ERROR(variant_incorrect_type_exception));

      return static_cast<const T&>(data);
    }

    //***************************************************************************
    /// Gets the value stored as the specified template type.
    ///\return A reference to the value.
    //***************************************************************************
    template <typename TBase>
    TBase& upcast()
    {
      return upcast_functor<TBase, T1, T2, T3, T4, T5, T6, T7, T8>()(data, type_id);
    }

    //***************************************************************************
    /// Gets the value stored as the specified template type.
    ///\return A const reference to the value.
    //***************************************************************************
    template <typename TBase>
    const TBase& upcast() const
    {
      return upcast_functor<TBase, T1, T2, T3, T4, T5, T6, T7, T8>()(data, type_id);
    }

    //***************************************************************************
    /// Conversion operators for each type.
    //***************************************************************************
    operator T1&() { return get<T1>(); }
    operator T2&() { return get<T2>(); }
    operator T3&() { return get<T3>(); }
    operator T4&() { return get<T4>(); }
    operator T5&() { return get<T5>(); }
    operator T6&() { return get<T6>(); }
    operator T7&() { return get<T7>(); }
    operator T8&() { return get<T8>(); }

    //***************************************************************************
    /// Checks if the template type is supported by the implementation of variant..
    ///\return <b>true</b> if the type is supported, otherwise <b>false</b>.
    //***************************************************************************
    template <typename T>
    static bool is_supported_type()
    {
      return Type_Is_Supported<T>::value;
    }

  private:

    //***************************************************************************
    /// Destruct the current occupant of the variant.
    //***************************************************************************
    void destruct_current()
    {
      switch (type_id)
      {
        case 0: { static_cast<T1*>(data)->~T1(); break; }
        case 1: { static_cast<T2*>(data)->~T2(); break; }
        case 2: { static_cast<T3*>(data)->~T3(); break; }
        case 3: { static_cast<T4*>(data)->~T4(); break; }
        case 4: { static_cast<T5*>(data)->~T5(); break; }
        case 5: { static_cast<T6*>(data)->~T6(); break; }
        case 6: { static_cast<T7*>(data)->~T7(); break; }
        case 7: { static_cast<T8*>(data)->~T8(); break; }
        default: { break; }
      }

      type_id = UNSUPPORTED_TYPE_ID;
    }

    //***************************************************************************
    /// The internal storage.
    /// Aligned on a suitable boundary, which should be good for all types.
    //***************************************************************************
    typename etl::aligned_storage<SIZE, ALIGNMENT>::type data;

    //***************************************************************************
    /// The id of the current stored type.
    //***************************************************************************
    type_id_t type_id;
  };
}

#undef ETL_FILE

#endif
