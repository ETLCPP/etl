///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl

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

#ifndef __ETL_VARIANT__
#define __ETL_VARIANT__

#include <stdint.h>

#include "array.h"
#include "largest.h"
#include "exception.h"
#include "type_traits.h"
#include "integral_limits.h"
#include "static_assert.h"
#include "alignment.h"

#ifndef ETL_THROW_EXCEPTIONS
#include "error_handler.h"
#endif

#if defined(COMPILER_KEIL)
  #pragma diag_suppress 940
	#pragma diag_suppress 111
#endif

//*****************************************************************************
///\defgroup variant variant
/// A class that can contain one a several specified types in a type safe manner.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  namespace __private_variant__
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
    variant_exception(const char* what)
      : exception(what)
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
    variant_incorrect_type_exception()
      : variant_exception("variant: unsupported type")
    {
    }
  };

  //***************************************************************************
  /// 'type Id' exception for the variant class.
  ///\ingroup variant
  //***************************************************************************
  class variant_invalid_type_id_exception : public variant_exception
  {
  public:
    variant_invalid_type_id_exception()
      : variant_exception("variant: invalid type id")
    {
    }
  };

  //***************************************************************************
  /// A template class that can store any of the types defined in the template parameter list.
  /// Supports up to 8 types.
  ///\ingroup variant
  //***************************************************************************
  template <typename T1,
            typename T2 = __private_variant__::no_type<2>,
            typename T3 = __private_variant__::no_type<3>,
            typename T4 = __private_variant__::no_type<4>,
            typename T5 = __private_variant__::no_type<5>,
            typename T6 = __private_variant__::no_type<6>,
            typename T7 = __private_variant__::no_type<7>,
            typename T8 = __private_variant__::no_type<8> >
  class variant
  {
  private:

    // All types of variant are friends.
    template <typename U1, typename U2, typename U3, typename U4, typename U5, typename U6, typename U7, typename U8>
    friend class variant;

    //***************************************************************************
    /// The largest type.
    //***************************************************************************
    typedef typename largest_type<T1, T2, T3, T4, T5, T6, T7, T8>::type largest_t;

    //***************************************************************************
    /// The largest alignment.
    //***************************************************************************
    static const size_t ALIGNMENT = etl::largest_alignment<T1, T2, T3, T4, T5, T6, T7, T8>::value;

    //***************************************************************************
    /// The type used for ids.
    //***************************************************************************
    typedef uint8_t type_id_t;

    //***************************************************************************
    /// The id a unsupported types.
    //***************************************************************************
    static const type_id_t UNSUPPORTED_TYPE_ID = integral_limits<type_id_t>::max;

    //***************************************************************************
    /// Do we pass this type by value?
    //***************************************************************************
    template <typename T>
    struct pass_by_value : integral_constant<bool, etl::is_fundamental<T>::value || etl::is_pointer<T>::value>
    {
    };

    //***************************************************************************
    /// Define the type for a parameter.
    //***************************************************************************
    template <typename T, bool>
    struct type_definition;

    //***************************************************************************
    /// Pass by value.
    //***************************************************************************
    template <typename T>
    struct type_definition<T, true>
    {
      typedef T type;
    };

    //***************************************************************************
    /// Pass by const reference.
    //***************************************************************************
    template <typename T>
    struct type_definition<T, false>
    {
      typedef const T& type;
    };

    //***************************************************************************
    /// Determines the type for parameters.
    //***************************************************************************
    template <typename T>
    struct parameter_type : public type_definition<T, pass_by_value<T>::value>
    {
    };

    //***************************************************************************
    /// Short form of no_type placeholders.
    //***************************************************************************
    typedef __private_variant__::no_type<2> no_type2;
    typedef __private_variant__::no_type<3> no_type3;
    typedef __private_variant__::no_type<4> no_type4;
    typedef __private_variant__::no_type<5> no_type5;
    typedef __private_variant__::no_type<6> no_type6;
    typedef __private_variant__::no_type<7> no_type7;
    typedef __private_variant__::no_type<8> no_type8;

    //***************************************************************************
    /// Lookup the id of type.
    //***************************************************************************
    template <typename T>
    struct Type_Id_Lookup
    {
      static const uint8_t type_id = etl::is_same<T, T1>::value ? 0 :
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

      virtual void read(typename parameter_type<R1>::type value) = 0;
      virtual void read(typename parameter_type<R2>::type value) = 0;
      virtual void read(typename parameter_type<R3>::type value) = 0;
      virtual void read(typename parameter_type<R4>::type value) = 0;
      virtual void read(typename parameter_type<R5>::type value) = 0;
      virtual void read(typename parameter_type<R6>::type value) = 0;
      virtual void read(typename parameter_type<R7>::type value) = 0;
      virtual void read(typename parameter_type<R8>::type value) = 0;
    };

    //*************************************************************************
    /// Define the reader type for 7 types.
    //*************************************************************************
    template <typename R1, typename R2, typename R3, typename R4, typename R5, typename R6, typename R7>
    class reader_type<R1, R2, R3, R4, R5, R6, R7, no_type8>
    {
    public:

      friend class variant;

      virtual void read(typename parameter_type<R1>::type value) = 0;
      virtual void read(typename parameter_type<R2>::type value) = 0;
      virtual void read(typename parameter_type<R3>::type value) = 0;
      virtual void read(typename parameter_type<R4>::type value) = 0;
      virtual void read(typename parameter_type<R5>::type value) = 0;
      virtual void read(typename parameter_type<R6>::type value) = 0;
      virtual void read(typename parameter_type<R7>::type value) = 0;

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

      virtual void read(typename parameter_type<R1>::type value) = 0;
      virtual void read(typename parameter_type<R2>::type value) = 0;
      virtual void read(typename parameter_type<R3>::type value) = 0;
      virtual void read(typename parameter_type<R4>::type value) = 0;
      virtual void read(typename parameter_type<R5>::type value) = 0;
      virtual void read(typename parameter_type<R6>::type value) = 0;

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

      virtual void read(typename parameter_type<R1>::type value) = 0;
      virtual void read(typename parameter_type<R2>::type value) = 0;
      virtual void read(typename parameter_type<R3>::type value) = 0;
      virtual void read(typename parameter_type<R4>::type value) = 0;
      virtual void read(typename parameter_type<R5>::type value) = 0;

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

      virtual void read(typename parameter_type<R1>::type value) = 0;
      virtual void read(typename parameter_type<R2>::type value) = 0;
      virtual void read(typename parameter_type<R3>::type value) = 0;
      virtual void read(typename parameter_type<R4>::type value) = 0;

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

      virtual void read(typename parameter_type<R1>::type value) = 0;
      virtual void read(typename parameter_type<R2>::type value) = 0;
      virtual void read(typename parameter_type<R3>::type value) = 0;

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

      virtual void read(typename parameter_type<R1>::type value) = 0;
      virtual void read(typename parameter_type<R2>::type value) = 0;

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

      virtual void read(typename parameter_type<R1>::type value) = 0;

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

      TBase& operator()(uint8_t* p_data, uint8_t typeId)
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
          default:
#ifdef ETL_THROW_EXCEPTIONS
            throw variant_invalid_type_id_exception();
#else
            error_handler::error(variant_invalid_type_id_exception());
#endif
            return reinterpret_cast<TBase&>(*p_data);
						break;
        }
      }

      const TBase& operator()(uint8_t* p_data, uint8_t typeId) const
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
          default:
#ifdef ETL_THROW_EXCEPTIONS
            throw variant_invalid_type_id_exception();
#else
            error_handler::error(variant_invalid_type_id_exception());
#endif
            return reinterpret_cast<TBase&>(*p_data);
				  	break;
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

      TBase& operator()(uint8_t* p_data, uint8_t typeId)
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
          default:
#ifdef ETL_THROW_EXCEPTIONS
            throw variant_invalid_type_id_exception();
#else
            error_handler::error(variant_invalid_type_id_exception());
#endif      
            return reinterpret_cast<TBase&>(*p_data);
				  	break;
        }
      }

      const TBase& operator()(uint8_t* p_data, uint8_t typeId) const
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
          default:
#ifdef ETL_THROW_EXCEPTIONS
            throw variant_invalid_type_id_exception();
#else
            error_handler::error(variant_invalid_type_id_exception());
#endif
            return reinterpret_cast<TBase&>(*p_data);
				  	break;
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

      TBase& operator()(uint8_t* p_data, uint8_t typeId)
      {
        switch (typeId)
        {
          case 0: return reinterpret_cast<U1&>(*p_data);
          case 1: return reinterpret_cast<U2&>(*p_data);
          case 2: return reinterpret_cast<U3&>(*p_data);
          case 3: return reinterpret_cast<U4&>(*p_data);
          case 4: return reinterpret_cast<U5&>(*p_data);
          case 5: return reinterpret_cast<U6&>(*p_data);
          default:
#ifdef ETL_THROW_EXCEPTIONS
            throw variant_invalid_type_id_exception();
#else
            error_handler::error(variant_invalid_type_id_exception());
#endif
            return reinterpret_cast<TBase&>(*p_data);
				  	break;
        }
      }

      const TBase& operator()(uint8_t* p_data, uint8_t typeId) const
      {
        switch (typeId)
        {
          case 0: return reinterpret_cast<const U1&>(*p_data);
          case 1: return reinterpret_cast<const U2&>(*p_data);
          case 2: return reinterpret_cast<const U3&>(*p_data);
          case 3: return reinterpret_cast<const U4&>(*p_data);
          case 4: return reinterpret_cast<const U5&>(*p_data);
          case 5: return reinterpret_cast<const U6&>(*p_data);
          default:
#ifdef ETL_THROW_EXCEPTIONS
            throw variant_invalid_type_id_exception();
#else
            error_handler::error(variant_invalid_type_id_exception());
#endif
            return reinterpret_cast<TBase&>(*p_data);
					  break;
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

      TBase& operator()(uint8_t* p_data, uint8_t typeId)
      {
        switch (typeId)
        {
          case 0: return reinterpret_cast<U1&>(*p_data);
          case 1: return reinterpret_cast<U2&>(*p_data);
          case 2: return reinterpret_cast<U3&>(*p_data);
          case 3: return reinterpret_cast<U4&>(*p_data);
          case 4: return reinterpret_cast<U5&>(*p_data);
          default:
#ifdef ETL_THROW_EXCEPTIONS
            throw variant_invalid_type_id_exception();
#else
            error_handler::error(variant_invalid_type_id_exception());
#endif
            return reinterpret_cast<TBase&>(*p_data);
					  break;
        }
      }

      const TBase& operator()(uint8_t* p_data, uint8_t typeId) const
      {
        switch (typeId)
        {
          case 0: return reinterpret_cast<const U1&>(*p_data);
          case 1: return reinterpret_cast<const U2&>(*p_data);
          case 2: return reinterpret_cast<const U3&>(*p_data);
          case 3: return reinterpret_cast<const U4&>(*p_data);
          case 4: return reinterpret_cast<const U5&>(*p_data);
          default:
#ifdef ETL_THROW_EXCEPTIONS
            throw variant_invalid_type_id_exception();
#else
            error_handler::error(variant_invalid_type_id_exception());
#endif
            return reinterpret_cast<TBase&>(*p_data);
					  break;
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

      TBase& operator()(uint8_t* p_data, uint8_t typeId)
      {
        switch (typeId)
        {
          case 0: return reinterpret_cast<U1&>(*p_data);
          case 1: return reinterpret_cast<U2&>(*p_data);
          case 2: return reinterpret_cast<U3&>(*p_data);
          case 3: return reinterpret_cast<U4&>(*p_data);
          default:
#ifdef ETL_THROW_EXCEPTIONS
            throw variant_invalid_type_id_exception();
#else
            error_handler::error(variant_invalid_type_id_exception());
#endif
            return reinterpret_cast<TBase&>(*p_data);
				  	break;
        }
      }

      const TBase& operator()(uint8_t* p_data, uint8_t typeId) const
      {
        switch (typeId)
        {
          case 0: return reinterpret_cast<const U1&>(*p_data);
          case 1: return reinterpret_cast<const U2&>(*p_data);
          case 2: return reinterpret_cast<const U3&>(*p_data);
          case 3: return reinterpret_cast<const U4&>(*p_data);
          default:
#ifdef ETL_THROW_EXCEPTIONS
            throw variant_invalid_type_id_exception();
#else
            error_handler::error(variant_invalid_type_id_exception());
#endif
            return reinterpret_cast<TBase&>(*p_data);
						break;
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

      TBase& operator()(uint8_t* p_data, uint8_t typeId)
      {
        switch (typeId)
        {
          case 0: return reinterpret_cast<U1&>(*p_data);
          case 1: return reinterpret_cast<U2&>(*p_data);
          case 2: return reinterpret_cast<U3&>(*p_data);
          default:
#ifdef ETL_THROW_EXCEPTIONS
            throw variant_invalid_type_id_exception();
#else
            error_handler::error(variant_invalid_type_id_exception());
#endif
            return reinterpret_cast<TBase&>(*p_data);
						break;
        }
      }

      const TBase& operator()(uint8_t* p_data, uint8_t typeId) const
      {
        switch (typeId)
        {
          case 0: return reinterpret_cast<const U1&>(*p_data);
          case 1: return reinterpret_cast<const U2&>(*p_data);
          case 2: return reinterpret_cast<const U3&>(*p_data);
          default:
#ifdef ETL_THROW_EXCEPTIONS
            throw variant_invalid_type_id_exception();
#else
            error_handler::error(variant_invalid_type_id_exception());
#endif
            return reinterpret_cast<TBase&>(*p_data);
						break;
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

      TBase& operator()(uint8_t* p_data, uint8_t typeId)
      {
        switch (typeId)
        {
          case 0: return reinterpret_cast<U1&>(*p_data);
          case 1: return reinterpret_cast<U2&>(*p_data);
          default:
#ifdef ETL_THROW_EXCEPTIONS
            throw variant_invalid_type_id_exception();
#else
            error_handler::error(variant_invalid_type_id_exception());
#endif
            return reinterpret_cast<TBase&>(*p_data);
						break;
        }
      }

      const TBase& operator()(uint8_t* p_data, uint8_t typeId) const
      {
        switch (typeId)
        {
          case 0: return reinterpret_cast<const U1&>(*p_data);
          case 1: return reinterpret_cast<const U2&>(*p_data);
          default:
#ifdef ETL_THROW_EXCEPTIONS
            throw variant_invalid_type_id_exception();
#else
            error_handler::error(variant_invalid_type_id_exception());
#endif
            return reinterpret_cast<TBase&>(*p_data);
						break;
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

      TBase& operator()(uint8_t* p_data, uint8_t typeId)
      {
        switch (typeId)
        {
          case 0: return reinterpret_cast<U1&>(*p_data);
          default:
#ifdef ETL_THROW_EXCEPTIONS
            throw variant_invalid_type_id_exception();
#else
            error_handler::error(variant_invalid_type_id_exception());
#endif
            return reinterpret_cast<TBase&>(*p_data);
						break;
        }
      }

      const TBase& operator()(uint8_t* p_data, uint8_t typeId) const
      {
        switch (typeId)
        {
          case 0: return reinterpret_cast<const U1&>(*p_data);
          default:
#ifdef ETL_THROW_EXCEPTIONS
            throw variant_invalid_type_id_exception();
#else
            error_handler::error(variant_invalid_type_id_exception());
#endif
            return reinterpret_cast<TBase&>(*p_data);
						break;
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
    /// Forces a STATIC_ASSERT.
    //***************************************************************************
    template <typename T>
    variant(T value)
    {
      STATIC_ASSERT(Type_Is_Supported<T>::value, "Unsupported type");

      new(static_cast<T*>(data)) T(value);
      type_id = Type_Id_Lookup<T>::type_id;
    }

    //***************************************************************************
    /// Copy constructor.
    ///\param other The other variant object to copy.
    //***************************************************************************
    variant(const variant& other)
      : data(other.data),
        type_id(other.type_id)
    {
    }

    //***************************************************************************
    /// Assignment operator for T1 type.
    ///\param value The value to assign.
    //***************************************************************************
    template <typename T>
    variant& operator =(typename parameter_type<T>::type value)
    {
      STATIC_ASSERT(Type_Is_Supported<T>::value, "Unsupported type");

      new(data) T(value);
      type_id = Type_Id_Lookup<T>::type_id;
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
      bool is_same_type = false;

      switch (other.type_id)
      {
        case 0: is_same_type = type_id == Type_Id_Lookup<U1>::type_id; break;
        case 1: is_same_type = type_id == Type_Id_Lookup<U2>::type_id; break;
        case 2: is_same_type = type_id == Type_Id_Lookup<U3>::type_id; break;
        case 3: is_same_type = type_id == Type_Id_Lookup<U4>::type_id; break;
        case 4: is_same_type = type_id == Type_Id_Lookup<U5>::type_id; break;
        case 5: is_same_type = type_id == Type_Id_Lookup<U6>::type_id; break;
        case 6: is_same_type = type_id == Type_Id_Lookup<U7>::type_id; break;
        case 7: is_same_type = type_id == Type_Id_Lookup<U8>::type_id; break;
        default: 
#ifdef ETL_THROW_EXCEPTIONS
          throw variant_invalid_type_id_exception();
#else
          error_handler::error(variant_invalid_type_id_exception());
#endif
					break;
      }

      return is_same_type;
    }
   
    //***************************************************************************
    /// Calls the supplied reader instance.
    /// The 'read' function appropriate to the current type is called with the stored value.
    //***************************************************************************
    void call(reader& reader)
    {
      switch (type_id)
      {
        case 0: reader.read(static_cast<T1&>(data)); break;
        case 1: reader.read(static_cast<T2&>(data)); break;
        case 2: reader.read(static_cast<T3&>(data)); break;
        case 3: reader.read(static_cast<T4&>(data)); break;
        case 4: reader.read(static_cast<T5&>(data)); break;
        case 5: reader.read(static_cast<T6&>(data)); break;
        case 6: reader.read(static_cast<T7&>(data)); break;
        case 7: reader.read(static_cast<T8&>(data)); break;
        default: 
#ifdef ETL_THROW_EXCEPTIONS
          throw variant_invalid_type_id_exception();
#else
          error_handler::error(variant_invalid_type_id_exception());
#endif
					break;
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
    /// Clears the value to 'no valid stored value'.
    //***************************************************************************
    void clear()
    {
      type_id = UNSUPPORTED_TYPE_ID;
    }

    //***************************************************************************
    /// Gets the value stored as the specified template type.
    /// Throws a variant_incorrect_type_exception if the actual type is not that specified.
    ///\return A reference to the value.
    //***************************************************************************
    template <typename T>
    T& get()
    {
      STATIC_ASSERT(Type_Is_Supported<T>::value, "Unsupported type");

      if (!is_type<T>())
      {
#ifdef ETL_THROW_EXCEPTIONS
        throw variant_incorrect_type_exception();
#else
        error_handler::error(variant_incorrect_type_exception());
#endif
      }

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
      STATIC_ASSERT(Type_Is_Supported<T>::value, "Unsupported type");

      if (!is_type<T>())
      {
#ifdef ETL_THROW_EXCEPTIONS
        throw variant_incorrect_type_exception();
#else
        error_handler::error(variant_incorrect_type_exception());
#endif
      }

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
    /// The internal storage.
    /// Aligned on a suitable boundary, which should be good for all types.
    //***************************************************************************
    typename etl::aligned_storage<sizeof(largest_t), etl::alignment_of<largest_t>::value>::type data;

    //***************************************************************************
    /// The id of the current stored type.
    //***************************************************************************
    type_id_t type_id;
  };
}

#endif
