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

#ifndef __ETL_LARGEST__
#define __ETL_LARGEST__

///\defgroup largest largest
///\ingroup utilities

#include "type_traits.h"

namespace etl 
{
	//***************************************************************************
	/// Template to determine the largest type and size.
  /// Supports up to 16 types.
	/// Defines 'value_type' which is the type of the largest parameter.
	/// Defines 'size' which is the size of the largest parameter.
  ///\ingroup largest
	//***************************************************************************
	template <typename T1,         typename T2  = void, typename T3  = void, typename T4  = void,
			      typename T5  = void, typename T6  = void, typename T7  = void, typename T8  = void,
			      typename T9  = void, typename T10 = void, typename T11 = void, typename T12 = void,
			      typename T13 = void, typename T14 = void, typename T15 = void, typename T16 = void>
	struct largest_type
	{
	private:

    // Declaration.
		template <const bool Boolean, typename TrueType, typename FalseType>
		struct choose_type;

    // Specialisation for 'true'.
    // Defines 'type' as 'TrueType'.
    template <typename TrueType, typename FalseType>
    struct choose_type<true, TrueType, FalseType>
    {
      typedef TrueType type;
    };

		// Specialisation for 'false'. 
    // Defines 'type' as 'FalseType'.
    template <typename TrueType, typename FalseType>
    struct choose_type<false, TrueType, FalseType>
		{
      typedef FalseType type;
		};

	public:

		// Define 'largest_other' as 'largest_type' with all but the first parameter. 
		typedef typename largest_type<T2,  T3,  T4,  T5,  T6,  T7,  T8,  T9,  T10, T11, T12, T13, T14, T15, T16>::type largest_other;

		// Set 'type' to be the largest of the first parameter and any of the others.
    // This is recursive.
    typedef typename choose_type<(sizeof(T1) > sizeof(largest_other)), // Boolean
									               T1,                                   // TrueType
									               largest_other>                        // FalseType
									               ::type type;                          // The largest type of the two.

		// The size of the largest type.
		enum
		{
			size = sizeof(type)
		};
	};

  //***************************************************************************
	// Specialisation for one template parameter.
	//***************************************************************************
	template <typename T1>
	struct largest_type<T1,   void, void, void,
			      		      void, void, void, void, 
					            void, void, void, void, 
					            void, void, void, void>
	{
		typedef T1 type;

		enum
		{
			size = sizeof(type)
		};
	};

  //***************************************************************************
  /// Template to determine the largest alignment.
  /// Supports up to 16 types.
  /// Defines <b>value</b> which is the largest alignment of all the parameters.
  ///\ingroup largest
  //***************************************************************************
  template <typename T1,         typename T2  = void, typename T3  = void, typename T4  = void,
            typename T5  = void, typename T6  = void, typename T7  = void, typename T8  = void,
            typename T9  = void, typename T10 = void, typename T11 = void, typename T12 = void,
            typename T13 = void, typename T14 = void, typename T15 = void, typename T16 = void>
  struct largest_alignment
  {
    // Determine the largest size.
    template <const size_t size1, const size_t size2>
    struct max_size
    {
      enum
      {
        value = (size1 > size2) ? size1 : size2
      };
    };

    // All of the alignments.
    enum
    {
      t1 = alignment_of<T1>::value,
      t2 = alignment_of<T2>::value,
      t3 = alignment_of<T3>::value,
      t4 = alignment_of<T4>::value,
      t5 = alignment_of<T5>::value,
      t6 = alignment_of<T6>::value,
      t7 = alignment_of<T7>::value,
      t8 = alignment_of<T8>::value,
      t9 = alignment_of<T9>::value,
      t10 = alignment_of<T10>::value,
      t11 = alignment_of<T11>::value,
      t12 = alignment_of<T12>::value,
      t13 = alignment_of<T13>::value,
      t14 = alignment_of<T14>::value,
      t15 = alignment_of<T15>::value,
      t16 = alignment_of<T16>::value,
    };

  public:

    // The largest of all of them.
    enum
    {
      value = max_size<t1, max_size<t2, max_size<t3, max_size<t4, max_size<t5, max_size<t6, max_size<t7, max_size<t8, 
              max_size<t9, max_size<t10, max_size<t11, max_size<t12, max_size<t13, max_size<t14, max_size<t15, t16>
              ::value>::value>::value>::value>::value>::value>::value>::value>
              ::value>::value>::value>::value>::value>::value>::value
    };
  };
}

#endif
