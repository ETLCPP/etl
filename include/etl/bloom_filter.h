///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 John Wellbelove

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

#ifndef ETL_BLOOM_FILTER_INCLUDED
#define ETL_BLOOM_FILTER_INCLUDED

#include "platform.h"
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
  namespace private_bloom_filter
  {
    // Placeholder null hash for defaulted template parameters.
    struct null_hash
    {
      template <typename T>
      size_t operator ()(T)
      {
        return 0;
      }
    };
  }

  //***************************************************************************
  /// An implementation of a bloom filter.
  /// Allows up to three hashes to be defined.
  /// Hashes must support the () operator and define 'argument_type'.
  ///\tparam DESIRED_WIDTH The desired number of hash results that can be stored. Rounded up to best fit the underlying bitset.
  ///\tparam THash1        The first hash generator class.
  ///\tparam THash2        The second hash generator class. If omitted, uses the null hash.
  ///\tparam THash3        The third hash generator class.  If omitted, uses the null hash.
  /// The hash classes must define <b>argument_type</b>.
  ///\ingroup bloom_filter
  //***************************************************************************
  template <size_t DESIRED_WIDTH,
            typename     THash1,
            typename     THash2 = private_bloom_filter::null_hash,
            typename     THash3 = private_bloom_filter::null_hash>
  class bloom_filter
  {
  private:

    typedef typename etl::parameter_type<typename THash1::argument_type>::type parameter_t;
    typedef private_bloom_filter::null_hash null_hash;

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
    void add(parameter_t key)
    {
      flags.set(get_hash<THash1>(key));

      if (!etl::is_same<THash2, null_hash>::value)
      {
        flags.set(get_hash<THash2>(key));
      }

      if (!etl::is_same<THash3, null_hash>::value)
      {
        flags.set(get_hash<THash3>(key));
      }
    }

    //***************************************************************************
    /// Tests a key to see if it exists in the filter.
    ///\param  key The key to test.
    ///\return <b>true</b> if the key exists in the filter.
    //***************************************************************************
    bool exists(parameter_t key) const
    {
      bool exists1 = flags[get_hash<THash1>(key)];
      bool exists2 = true;
      bool exists3 = true;

      // Do we have a second hash?
      if (!etl::is_same<THash2, null_hash>::value)
      {
        exists2 = flags[get_hash<THash2>(key)];
      }

      // Do we have a third hash?
      if (!etl::is_same<THash3, null_hash>::value)
      {
        exists3 = flags[get_hash<THash3>(key)];
      }

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

