///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.

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

#ifndef __etl_largest__
#define __etl_largest__

///\defgroup largest largest
///\ingroup utilities

namespace etl 
{
	//***************************************************************************
	/// Template to determine the largest type and size.
  /// Supports up to 16 types.
	/// Defines 'value_type' which is the type of the largest parameter.
	/// Defines 'size' which is the size of the largest parameter.
  ///\ingroup numeric
	//***************************************************************************
	template <typename T1,         typename T2  = void, typename T3  = void, typename T4  = void,
			      typename T5  = void, typename T6  = void, typename T7  = void, typename T8  = void,
			      typename T9  = void, typename T10 = void, typename T11 = void, typename T12 = void,
			      typename T13 = void, typename T14 = void, typename T15 = void, typename T16 = void>
	struct largest
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

		// Define 'largest_other' as 'largest' with all but the first parameter. 
		typedef typename largest<T2,  T3,  T4,  T5,  T6,  T7,  T8,  T9,  T10, T11, T12, T13, T14, T15, T16>::type largest_other;

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
	struct largest<T1,   void, void, void,
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
}

#endif
