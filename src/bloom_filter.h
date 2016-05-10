///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

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

#ifndef __ETL_BLOOM_FILTER__
#define __ETL_BLOOM_FILTER__

#include "parameter_type.h"
#include "bitset.h"
#include "type_traits.h"
#include "binary.h"
#include "log.h"
#include "power.h"

///\defgroup bloom_filter bloom_filter
/// A Bloom filter
///\ingroup containers

namespace etl
{
  //***************************************************************************
  /// An implementation of a bloom filter.
  /// Allows up to three hashes to be defined.
  /// Hashes must support the () operator and define 'argument_type'.
  ///\tparam DESIRED_WIDTH The desired number of hash results that can be stored. Rounded up to best fit the underlying bitset.
  ///\tparam THash1        The first hash generator class.
  ///\tparam THash2        The second hash generator class. Default unused.
  ///\tparam THash3        The third hash generator class.  Default unused.
  /// The hash classes must define <b>argument_type</b>.
  ///\ingroup bloom_filter
  //***************************************************************************
  template <const size_t DESIRED_WIDTH,
            typename     THash1,
            typename     THash2 = void,
            typename     THash3 = void>
  class bloom_filter
  {
  private:

    typedef typename etl::parameter_type<typename THash1::argument_type>::type parameter_t;

  public:

    enum
    {
      // Make the most efficient use of the bitset.
      WIDTH = etl::bitset<DESIRED_WIDTH>::ALLOCATED_BITS
    };

    //***************************************************************************
    /// Clears the bloom filter of all entries.
	  //***************************************************************************
	  void clear()
	  {
	    flags.reset();
	  }

    //***************************************************************************
    /// Adds a key to the filter.
    ///\param key The key to add.
    //***************************************************************************
    template <typename T2 = THash2, typename T3 = THash3>
    typename etl::enable_if<etl::is_same<T2, void>::value && etl::is_same<T3, void>::value, void>::type
    add(parameter_t key)
    {
      flags.set(get_hash<THash1>(key));
    }

    //***************************************************************************
    /// Adds a key to the filter.
    ///\param key The key to add.
    //***************************************************************************
    template <typename T2 = THash2, typename T3 = THash3>
    typename etl::enable_if<!etl::is_same<T2, void>::value && etl::is_same<T3, void>::value, void>::type
    add(parameter_t key)
    {
      flags.set(get_hash<THash1>(key));
      flags.set(get_hash<THash2>(key));
    }
    
    //***************************************************************************
    /// Adds a key to the filter.
    ///\param key The key to add.
    //***************************************************************************
    template <typename T2 = THash2, typename T3 = THash3>
    typename etl::enable_if<!etl::is_same<T2, void>::value && !etl::is_same<T3, void>::value, void>::type
    add(parameter_t key)
    {
      flags.set(get_hash<THash1>(key));
      flags.set(get_hash<THash2>(key));
      flags.set(get_hash<THash3>(key));
    }

    //***************************************************************************
    /// Tests a key to see if it exists in the filter.
    ///\param  key The key to test.
    ///\return <b>true</b> if the key exists in the filter.
    //***************************************************************************
    template <typename T2 = THash2, typename T3 = THash3>
    typename etl::enable_if<etl::is_same<T2, void>::value && etl::is_same<T3, void>::value, bool>::type
    exists(parameter_t key) const
    {
      bool exists1 = flags[get_hash<THash1>(key)];

      return exists1;
    }

    //***************************************************************************
    /// Tests a key to see if it exists in the filter.
    ///\param  key The key to test.
    ///\return <b>true</b> if the key exists in the filter.
    //***************************************************************************
    template <typename T2 = THash2, typename T3 = THash3>
    typename etl::enable_if<!etl::is_same<T2, void>::value && etl::is_same<T3, void>::value, bool>::type
    exists(parameter_t key) const
    {
      bool exists1 = flags[get_hash<THash1>(key)];
      bool exists2 = flags[get_hash<THash2>(key)];

      return exists1 && exists2;
    }

    //***************************************************************************
    /// Tests a key to see if it exists in the filter.
    ///\param  key The key to test.
    ///\return <b>true</b> if the key exists in the filter.
    //***************************************************************************
    template <typename T2 = THash2, typename T3 = THash3>
    typename etl::enable_if<!etl::is_same<T2, void>::value && !etl::is_same<T3, void>::value, bool>::type
    exists(parameter_t key) const
    {
      bool exists1 = flags[get_hash<THash1>(key)];
      bool exists2 = flags[get_hash<THash2>(key)];
      bool exists3 = flags[get_hash<THash3>(key)];

      return exists1 && exists2 && exists3;
    }

    //***************************************************************************
    /// Returns the width of the Bloom filter.
    //***************************************************************************
    size_t width() const
    {
      return WIDTH;
    }

    //***************************************************************************
    /// Returns the percentage of usage. Range 0 to 100.
    //***************************************************************************
    size_t usage() const
    {
      return (100 * count()) / WIDTH;
    }

    //***************************************************************************
    /// Returns the number of filter flags set.
    //***************************************************************************
    size_t count() const
    {
      return flags.count();
    }

  private:

    //***************************************************************************
    /// Gets the hash for the key.
    ///\param  key The key.
    ///\return The hash value.
    //***************************************************************************
    template <typename THash>
    size_t get_hash(parameter_t key) const
    {
      size_t hash = THash()(key);

      // Fold the hash down to fit the width.
      return fold_bits<size_t, etl::log2<WIDTH>::value>(hash);
    }

    /// The Bloom filter flags.
    etl::bitset<WIDTH> flags;
  };
}

#endif

