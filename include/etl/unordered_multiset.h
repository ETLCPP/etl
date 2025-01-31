///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2016 John Wellbelove

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

#ifndef ETL_UNORDERED_MULTISET_INCLUDED
#define ETL_UNORDERED_MULTISET_INCLUDED

#include "platform.h"
#include "algorithm.h"
#include "iterator.h"
#include "functional.h"
#include "utility.h"
#include "pool.h"
#include "vector.h"
#include "intrusive_forward_list.h"
#include "hash.h"
#include "type_traits.h"
#include "nth_type.h"
#include "parameter_type.h"
#include "nullptr.h"
#include "error_handler.h"
#include "exception.h"
#include "debug_count.h"
#include "iterator.h"
#include "placement_new.h"
#include "initializer_list.h"

#include "private/comparator_is_transparent.h"

#include <stddef.h>

//*****************************************************************************
///\defgroup unordered_multiset unordered_multiset
/// A unordered_multiset with the capacity defined at compile time.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //***************************************************************************
  /// Exception for the unordered_multiset.
  ///\ingroup unordered_multiset
  //***************************************************************************
  class unordered_multiset_exception : public etl::exception
  {
  public:

    unordered_multiset_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : etl::exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Full exception for the unordered_multiset.
  ///\ingroup unordered_multiset
  //***************************************************************************
  class unordered_multiset_full : public etl::unordered_multiset_exception
  {
  public:

    unordered_multiset_full(string_type file_name_, numeric_type line_number_)
      : etl::unordered_multiset_exception(ETL_ERROR_TEXT("unordered_multiset:full", ETL_UNORDERED_MULTISET_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Out of range exception for the unordered_multiset.
  ///\ingroup unordered_multiset
  //***************************************************************************
  class unordered_multiset_out_of_range : public etl::unordered_multiset_exception
  {
  public:

    unordered_multiset_out_of_range(string_type file_name_, numeric_type line_number_)
      : etl::unordered_multiset_exception(ETL_ERROR_TEXT("unordered_multiset:range", ETL_UNORDERED_MULTISET_FILE_ID"B"), file_name_, line_number_)
    {}
  };

  //***************************************************************************
  /// Iterator exception for the unordered_multiset.
  ///\ingroup unordered_multiset
  //***************************************************************************
  class unordered_multiset_iterator : public etl::unordered_multiset_exception
  {
  public:

    unordered_multiset_iterator(string_type file_name_, numeric_type line_number_)
      : etl::unordered_multiset_exception(ETL_ERROR_TEXT("unordered_multiset:iterator", ETL_UNORDERED_MULTISET_FILE_ID"C"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// The base class for specifically sized unordered_multiset.
  /// Can be used as a reference type for all unordered_multiset containing a specific type.
  ///\ingroup unordered_multiset
  //***************************************************************************
  template <typename TKey, typename THash = etl::hash<TKey>, typename TKeyEqual = etl::equal_to<TKey> >
  class iunordered_multiset
  {
  public:

    typedef TKey              value_type;
    typedef TKey              key_type;
    typedef THash             hasher;
    typedef TKeyEqual         key_equal;
    typedef value_type&       reference;
    typedef const value_type& const_reference;
#if ETL_USING_CPP11
    typedef value_type&&      rvalue_reference;
#endif
    typedef value_type*       pointer;
    typedef const value_type* const_pointer;
    typedef size_t            size_type;

    typedef const TKey& key_parameter_t;

    typedef etl::forward_link<0> link_t;

    //*********************************************************************
    // The nodes that store the elements.
    struct node_t : public link_t
    {
      node_t(const_reference key_)
        : key(key_)
      {
      }

      value_type key;
    };

    friend bool operator ==(const node_t& lhs, const node_t& rhs)
    {
      return (lhs.key == rhs.key);
    }

    friend bool operator !=(const node_t& lhs, const node_t& rhs)
    {
      return !(lhs == rhs);
    }

  protected:

    typedef etl::intrusive_forward_list<node_t, link_t> bucket_t;
    typedef etl::ipool pool_t;

  public:

    // Local iterators iterate over one bucket.
    typedef typename bucket_t::iterator       local_iterator;
    typedef typename bucket_t::const_iterator const_local_iterator;

    //*********************************************************************
    class iterator : public etl::iterator<ETL_OR_STD::forward_iterator_tag, TKey>
    {
    public:

      typedef typename etl::iterator<ETL_OR_STD::forward_iterator_tag, TKey>::value_type value_type;
      typedef typename iunordered_multiset::key_type        key_type;
      typedef typename iunordered_multiset::hasher          hasher;
      typedef typename iunordered_multiset::key_equal       key_equal;
      typedef typename iunordered_multiset::reference       reference;
      typedef typename iunordered_multiset::const_reference const_reference;
      typedef typename iunordered_multiset::pointer         pointer;
      typedef typename iunordered_multiset::const_pointer   const_pointer;
      typedef typename iunordered_multiset::size_type       size_type;

      friend class iunordered_multiset;
      friend class const_iterator;

      //*********************************
      iterator()
      {
      }

      //*********************************
      iterator(const iterator& other)
        : pbuckets_end(other.pbuckets_end)
        , pbucket(other.pbucket)
        , inode(other.inode)
      {
      }

      //*********************************
      iterator& operator ++()
      {
        ++inode;

        // The end of this node list?
        if (inode == pbucket->end())
        {
          // Search for the next non-empty bucket.
          ++pbucket;
          while ((pbucket != pbuckets_end) && (pbucket->empty()))
          {
            ++pbucket;
          }

          // If not past the end, get the first node in the bucket.
          if (pbucket != pbuckets_end)
          {
            inode = pbucket->begin();
          }
        }

        return *this;
      }

      //*********************************
      iterator operator ++(int)
      {
        iterator temp(*this);
        operator++();
        return temp;
      }

      //*********************************
      iterator& operator =(const iterator& other)
      {
        pbuckets_end = other.pbuckets_end;
        pbucket = other.pbucket;
        inode = other.inode;
        return *this;
      }

      //*********************************
      reference operator *() const
      {
        return inode->key;
      }

      //*********************************
      pointer operator &() const
      {
        return &(inode->key);
      }

      //*********************************
      pointer operator ->() const
      {
        return &(inode->key);
      }

      //*********************************
      friend bool operator == (const iterator& lhs, const iterator& rhs)
      {
        return lhs.compare(rhs);
      }

      //*********************************
      friend bool operator != (const iterator& lhs, const iterator& rhs)
      {
        return !(lhs == rhs);
      }

    private:

      //*********************************
      iterator(bucket_t* pbuckets_end_, bucket_t* pbucket_, local_iterator inode_)
        : pbuckets_end(pbuckets_end_)
        , pbucket(pbucket_)
        , inode(inode_)
      {
      }

      //*********************************
      bool compare(const iterator& rhs) const
      {
        return rhs.inode == inode;
      }

      //*********************************
      bucket_t& get_bucket()
      {
        return *pbucket;
      }

      //*********************************
      bucket_t*& get_bucket_list_iterator()
      {
        return pbucket;
      }

      //*********************************
      local_iterator get_local_iterator()
      {
        return inode;
      }

      bucket_t* pbuckets_end;
      bucket_t* pbucket;
      local_iterator inode;
    };

    //*********************************************************************
    class const_iterator : public etl::iterator<ETL_OR_STD::forward_iterator_tag, const TKey>
    {
    public:

      typedef typename etl::iterator<ETL_OR_STD::forward_iterator_tag, const TKey>::value_type value_type;
      typedef typename iunordered_multiset::key_type        key_type;
      typedef typename iunordered_multiset::hasher          hasher;
      typedef typename iunordered_multiset::key_equal       key_equal;
      typedef typename iunordered_multiset::reference       reference;
      typedef typename iunordered_multiset::const_reference const_reference;
      typedef typename iunordered_multiset::pointer         pointer;
      typedef typename iunordered_multiset::const_pointer   const_pointer;
      typedef typename iunordered_multiset::size_type       size_type;

      friend class iunordered_multiset;
      friend class iterator;

      //*********************************
      const_iterator()
      {
      }

      //*********************************
      const_iterator(const typename iunordered_multiset::iterator& other)
        : pbuckets_end(other.pbuckets_end)
        , pbucket(other.pbucket)
        , inode(other.inode)
      {
      }

      //*********************************
      const_iterator(const const_iterator& other)
        : pbuckets_end(other.pbuckets_end)
        , pbucket(other.pbucket)
        , inode(other.inode)
      {
      }

      //*********************************
      const_iterator& operator ++()
      {
        ++inode;

        // The end of this node list?
        if (inode == pbucket->end())
        {
          // Search for the next non-empty bucket.

          ++pbucket;
          while ((pbucket != pbuckets_end) && (pbucket->empty()))
          {
            ++pbucket;
          }

          // If not past the end, get the first node in the bucket.
          if (pbucket != pbuckets_end)
          {
            inode = pbucket->begin();
          }
        }

        return *this;
      }

      //*********************************
      const_iterator operator ++(int)
      {
        const_iterator temp(*this);
        operator++();
        return temp;
      }

      //*********************************
      const_iterator& operator =(const const_iterator& other)
      {
        pbuckets_end = other.pbuckets_end;
        pbucket = other.pbucket;
        inode = other.inode;
        return *this;
      }

      //*********************************
      const_reference operator *() const
      {
        return inode->key;
      }

      //*********************************
      const_pointer operator &() const
      {
        return &(inode->key);
      }

      //*********************************
      const_pointer operator ->() const
      {
        return &(inode->key);
      }

      //*********************************
      friend bool operator == (const const_iterator& lhs, const const_iterator& rhs)
      {
        return lhs.compare(rhs);
      }

      //*********************************
      friend bool operator != (const const_iterator& lhs, const const_iterator& rhs)
      {
        return !(lhs == rhs);
      }

    private:

      //*********************************
      const_iterator(bucket_t* pbuckets_end_, bucket_t* pbucket_, local_iterator inode_)
        : pbuckets_end(pbuckets_end_)
        , pbucket(pbucket_)
        , inode(inode_)
      {
      }

      //*********************************
      bool compare(const const_iterator& rhs) const
      {
        return rhs.inode == inode;
      }

      //*********************************
      bucket_t& get_bucket()
      {
        return *pbucket;
      }

      //*********************************
      bucket_t*& get_bucket_list_iterator()
      {
        return pbucket;
      }

      //*********************************
      local_iterator get_local_iterator()
      {
        return inode;
      }

      bucket_t* pbuckets_end;
      bucket_t* pbucket;
      local_iterator inode;
    };

    typedef typename etl::iterator_traits<iterator>::difference_type difference_type;

    //*********************************************************************
    /// Returns an iterator to the beginning of the unordered_multiset.
    ///\return An iterator to the beginning of the unordered_multiset.
    //*********************************************************************
    iterator begin()
    {
      return iterator((pbuckets + number_of_buckets), first, first->begin());
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the unordered_multiset.
    ///\return A const iterator to the beginning of the unordered_multiset.
    //*********************************************************************
    const_iterator begin() const
    {
      return const_iterator((pbuckets + number_of_buckets), first, first->begin());
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the unordered_multiset.
    ///\return A const iterator to the beginning of the unordered_multiset.
    //*********************************************************************
    const_iterator cbegin() const
    {
      return const_iterator((pbuckets + number_of_buckets), first, first->begin());
    }

    //*********************************************************************
    /// Returns an iterator to the beginning of the unordered_multiset bucket.
    ///\return An iterator to the beginning of the unordered_multiset bucket.
    //*********************************************************************
    local_iterator begin(size_t i)
    {
      return pbuckets[i].begin();
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the unordered_multiset bucket.
    ///\return A const iterator to the beginning of the unordered_multiset bucket.
    //*********************************************************************
    const_local_iterator begin(size_t i) const
    {
      return pbuckets[i].cbegin();
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the unordered_multiset bucket.
    ///\return A const iterator to the beginning of the unordered_multiset bucket.
    //*********************************************************************
    const_local_iterator cbegin(size_t i) const
    {
      return pbuckets[i].cbegin();
    }

    //*********************************************************************
    /// Returns an iterator to the end of the unordered_multiset.
    ///\return An iterator to the end of the unordered_multiset.
    //*********************************************************************
    iterator end()
    {
      return iterator((pbuckets + number_of_buckets), last, last->end());
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the unordered_multiset.
    ///\return A const iterator to the end of the unordered_multiset.
    //*********************************************************************
    const_iterator end() const
    {
      return const_iterator((pbuckets + number_of_buckets), last, last->end());
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the unordered_multiset.
    ///\return A const iterator to the end of the unordered_multiset.
    //*********************************************************************
    const_iterator cend() const
    {
      return const_iterator((pbuckets + number_of_buckets), last, last->end());
    }

    //*********************************************************************
    /// Returns an iterator to the end of the unordered_multiset bucket.
    ///\return An iterator to the end of the unordered_multiset bucket.
    //*********************************************************************
    local_iterator end(size_t i)
    {
      return pbuckets[i].end();
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the unordered_multiset bucket.
    ///\return A const iterator to the end of the unordered_multiset bucket.
    //*********************************************************************
    const_local_iterator end(size_t i) const
    {
      return pbuckets[i].cend();
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the unordered_multiset bucket.
    ///\return A const iterator to the end of the unordered_multiset bucket.
    //*********************************************************************
    const_local_iterator cend(size_t i) const
    {
      return pbuckets[i].cend();
    }

    //*********************************************************************
    /// Returns the bucket index for the key.
    ///\return The bucket index for the key.
    //*********************************************************************
    size_type get_bucket_index(key_parameter_t key) const
    {
      return key_hash_function(key) % number_of_buckets;
    }

#if ETL_USING_CPP11
    //*********************************************************************
    /// Returns the bucket index for the key.
    ///\return The bucket index for the key.
    //*********************************************************************
    template <typename K, typename KE = TKeyEqual, etl::enable_if_t<comparator_is_transparent<KE>::value, int> = 0>
    size_type get_bucket_index(const K& key) const
    {
      return key_hash_function(key) % number_of_buckets;
    }
#endif

    //*********************************************************************
    /// Returns the size of the bucket key.
    ///\return The bucket size of the bucket key.
    //*********************************************************************
    size_type bucket_size(key_parameter_t key) const
    {
      size_t index = bucket(key);

      return etl::distance(pbuckets[index].begin(), pbuckets[index].end());
    }

#if ETL_USING_CPP11
    //*********************************************************************
    /// Returns the size of the bucket key.
    ///\return The bucket size of the bucket key.
    //*********************************************************************
    template <typename K, typename KE = TKeyEqual, etl::enable_if_t<comparator_is_transparent<KE>::value, int> = 0>
    size_type bucket_size(const K& key) const
    {
      size_t index = bucket(key);

      return etl::distance(pbuckets[index].begin(), pbuckets[index].end());
    }
#endif

    //*********************************************************************
    /// Returns the maximum number of the buckets the container can hold.
    ///\return The maximum number of the buckets the container can hold.
    //*********************************************************************
    size_type max_bucket_count() const
    {
      return number_of_buckets;
    }

    //*********************************************************************
    /// Returns the number of the buckets the container holds.
    ///\return The number of the buckets the container holds.
    //*********************************************************************
    size_type bucket_count() const
    {
      return number_of_buckets;
    }

    //*********************************************************************
    /// Assigns values to the unordered_multiset.
    /// If asserts or exceptions are enabled, emits unordered_multiset_full if the unordered_multiset does not have enough free space.
    /// If asserts or exceptions are enabled, emits unordered_multiset_iterator if the iterators are reversed.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*********************************************************************
    template <typename TIterator>
    void assign(TIterator first_, TIterator last_)
    {
#if ETL_IS_DEBUG_BUILD
      difference_type d = etl::distance(first_, last_);
      ETL_ASSERT(d >= 0, ETL_ERROR(unordered_multiset_iterator));
      ETL_ASSERT(size_t(d) <= max_size(), ETL_ERROR(unordered_multiset_full));
#endif

      clear();

      while (first_ != last_)
      {
        insert(*first_);
        ++first_;
      }
    }

    //*********************************************************************
    /// Inserts a value to the unordered_multiset.
    /// If asserts or exceptions are enabled, emits unordered_multiset_full if the unordered_multiset is already full.
    ///\param value The value to insert.
    //*********************************************************************
    ETL_OR_STD::pair<iterator, bool> insert(const_reference key)
    {
      ETL_OR_STD::pair<iterator, bool> result(end(), false);

      ETL_ASSERT(!full(), ETL_ERROR(unordered_multiset_full));

      // Get the hash index.
      size_t index = get_bucket_index(key);

      // Get the bucket & bucket iterator.
      bucket_t* pbucket = pbuckets + index;
      bucket_t& bucket = *pbucket;

      // The first one in the bucket?
      if (bucket.empty())
      {
        // Get a new node.
        node_t* node = allocate_data_node();
        node->clear();
        ::new (&node->key) value_type(key);
        ETL_INCREMENT_DEBUG_COUNT;

        // Just add the pointer to the bucket;
        bucket.insert_after(bucket.before_begin(), *node);
        adjust_first_last_markers_after_insert(&bucket);

        result.first = iterator((pbuckets + number_of_buckets), pbucket, pbucket->begin());
        result.second = true;
      }
      else
      {
        // Step though the bucket looking for a place to insert.
        local_iterator inode_previous = bucket.before_begin();
        local_iterator inode = bucket.begin();

        while (inode != bucket.end())
        {
          // Do we already have this key?
          if (key_equal_function(inode->key, key))
          {
            break;
          }

          ++inode_previous;
          ++inode;
        }

        // Get a new node.
        node_t* node = allocate_data_node();
        node->clear();
        ::new (&node->key) value_type(key);
        ETL_INCREMENT_DEBUG_COUNT;

        // Add the node to the end of the bucket;
        bucket.insert_after(inode_previous, *node);
        adjust_first_last_markers_after_insert(&bucket);
        ++inode_previous;

        result.first = iterator((pbuckets + number_of_buckets), pbucket, inode_previous);
        result.second = true;
      }

      return result;
    }

#if ETL_USING_CPP11
    //*********************************************************************
    /// Inserts a value to the unordered_multiset.
    /// If asserts or exceptions are enabled, emits unordered_multiset_full if the unordered_multiset is already full.
    ///\param value The value to insert.
    //*********************************************************************
    template <typename K, typename KE = TKeyEqual, etl::enable_if_t<comparator_is_transparent<KE>::value, int> = 0>
    ETL_OR_STD::pair<iterator, bool> insert(const K& key)
    {
      ETL_OR_STD::pair<iterator, bool> result(end(), false);

      ETL_ASSERT(!full(), ETL_ERROR(unordered_multiset_full));

      // Get the hash index.
      size_t index = get_bucket_index(key);

      // Get the bucket & bucket iterator.
      bucket_t* pbucket = pbuckets + index;
      bucket_t& bucket = *pbucket;

      // The first one in the bucket?
      if (bucket.empty())
      {
        // Get a new node.
        node_t* node = allocate_data_node();
        node->clear();
        ::new (&node->key) value_type(key);
        ETL_INCREMENT_DEBUG_COUNT;

        // Just add the pointer to the bucket;
        bucket.insert_after(bucket.before_begin(), *node);
        adjust_first_last_markers_after_insert(&bucket);

        result.first = iterator((pbuckets + number_of_buckets), pbucket, pbucket->begin());
        result.second = true;
      }
      else
      {
        // Step though the bucket looking for a place to insert.
        local_iterator inode_previous = bucket.before_begin();
        local_iterator inode = bucket.begin();

        while (inode != bucket.end())
        {
          // Do we already have this key?
          if (key_equal_function(inode->key, key))
          {
            break;
          }

          ++inode_previous;
          ++inode;
        }

        // Get a new node.
        node_t* node = allocate_data_node();
        node->clear();
        ::new (&node->key) value_type(key);
        ETL_INCREMENT_DEBUG_COUNT;

        // Add the node to the end of the bucket;
        bucket.insert_after(inode_previous, *node);
        adjust_first_last_markers_after_insert(&bucket);
        ++inode_previous;

        result.first = iterator((pbuckets + number_of_buckets), pbucket, inode_previous);
        result.second = true;
      }

      return result;
    }
#endif

#if ETL_USING_CPP11
    //*********************************************************************
    /// Inserts a value to the unordered_multiset.
    /// If asserts or exceptions are enabled, emits unordered_multiset_full if the unordered_multiset is already full.
    ///\param value The value to insert.
    //*********************************************************************
    ETL_OR_STD::pair<iterator, bool> insert(rvalue_reference key)
    {
      ETL_OR_STD::pair<iterator, bool> result(end(), false);

      ETL_ASSERT(!full(), ETL_ERROR(unordered_multiset_full));

      // Get the hash index.
      size_t index = get_bucket_index(key);

      // Get the bucket & bucket iterator.
      bucket_t* pbucket = pbuckets + index;
      bucket_t& bucket = *pbucket;

      // The first one in the bucket?
      if (bucket.empty())
      {
        // Get a new node.
        node_t* node = allocate_data_node();
        node->clear();
        ::new (&node->key) value_type(etl::move(key));
        ETL_INCREMENT_DEBUG_COUNT;

        // Just add the pointer to the bucket;
        bucket.insert_after(bucket.before_begin(), *node);
        adjust_first_last_markers_after_insert(&bucket);

        result.first = iterator((pbuckets + number_of_buckets), pbucket, pbucket->begin());
        result.second = true;
      }
      else
      {
        // Step though the bucket looking for a place to insert.
        local_iterator inode_previous = bucket.before_begin();
        local_iterator inode = bucket.begin();

        while (inode != bucket.end())
        {
          // Do we already have this key?
          if (key_equal_function(inode->key, key))
          {
            break;
          }

          ++inode_previous;
          ++inode;
        }

        // Get a new node.
        node_t* node = allocate_data_node();
        node->clear();
        ::new (&node->key) value_type(etl::move(key));
        ETL_INCREMENT_DEBUG_COUNT;

          // Add the node to the end of the bucket;
          bucket.insert_after(inode_previous, *node);
        adjust_first_last_markers_after_insert(&bucket);
        ++inode_previous;

        result.first = iterator((pbuckets + number_of_buckets), pbucket, inode_previous);
        result.second = true;
      }

      return result;
    }
#endif

    //*********************************************************************
    /// Inserts a value to the unordered_multiset.
    /// If asserts or exceptions are enabled, emits unordered_multiset_full if the unordered_multiset is already full.
    ///\param position The position to insert at.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(const_iterator /*position*/, const_reference key)
    {
      return insert(key).first;
    }

    //*********************************************************************
    /// Inserts a range of values to the unordered_multiset.
    /// If asserts or exceptions are enabled, emits unordered_multiset_full if the unordered_multiset does not have enough free space.
    ///\param position The position to insert at.
    ///\param first    The first element to add.
    ///\param last     The last + 1 element to add.
    //*********************************************************************
    template <class TIterator>
    void insert(TIterator first_, TIterator last_)
    {
      while (first_ != last_)
      {
        insert(*first_);
        ++first_;
      }
    }

    //*********************************************************************
    /// Erases an element.
    ///\param key The key to erase.
    ///\return The number of elements erased.
    //*********************************************************************
    size_t erase(key_parameter_t key)
    {
      size_t n = 0UL;
      size_t bucket_id = get_bucket_index(key);

      bucket_t& bucket = pbuckets[bucket_id];

      local_iterator iprevious = bucket.before_begin();
      local_iterator icurrent = bucket.begin();

      while (icurrent != bucket.end())
      {
        if (key_equal_function(icurrent->key, key))
        {
          delete_data_node(iprevious, icurrent, bucket);
          ++n;
          icurrent = iprevious;
        }
        else
        {
          ++iprevious;
        }

        ++icurrent;
      }

      return n;
    }

#if ETL_USING_CPP11
    //*********************************************************************
    /// Erases an element.
    ///\param key The key to erase.
    ///\return The number of elements erased. 0 or 1.
    //*********************************************************************
    template <typename K, typename KE = TKeyEqual, etl::enable_if_t<comparator_is_transparent<KE>::value, int> = 0>
    size_t erase(const K& key)
    {
      size_t n = 0UL;
      size_t bucket_id = get_bucket_index(key);

      bucket_t& bucket = pbuckets[bucket_id];

      local_iterator iprevious = bucket.before_begin();
      local_iterator icurrent = bucket.begin();

      while (icurrent != bucket.end())
      {
        if (key_equal_function(icurrent->key, key))
        {
          delete_data_node(iprevious, icurrent, bucket);
          ++n;
          icurrent = iprevious;
        }
        else
        {
          ++iprevious;
        }

        ++icurrent;
      }

      return n;
    }
#endif

    //*********************************************************************
    /// Erases an element.
    ///\param ielement Iterator to the element.
    //*********************************************************************
    iterator erase(const_iterator ielement)
    {
      // Make a note of the next one.
      iterator inext((pbuckets + number_of_buckets), ielement.get_bucket_list_iterator(), ielement.get_local_iterator());
      ++inext;

      bucket_t&      bucket = ielement.get_bucket();
      local_iterator iprevious = bucket.before_begin();
      local_iterator icurrent = ielement.get_local_iterator();

      // Find the node previous to the one we're interested in.
      while (iprevious->etl_next != &*icurrent)
      {
        ++iprevious;
      }

      delete_data_node(iprevious, icurrent, bucket);

      return inext;
    }

    //*********************************************************************
    /// Erases a range of elements.
    /// The range includes all the elements between first and last, including the
    /// element pointed by first, but not the one pointed to by last.
    ///\param first Iterator to the first element.
    ///\param last  Iterator to the last element.
    //*********************************************************************
    iterator erase(const_iterator first_, const_iterator last_)
    {
      // Erasing everything?
      if ((first_ == begin()) && (last_ == end()))
      {
        clear();
        return end();
      }

      // Get the starting point.
      bucket_t*      pbucket     = first_.get_bucket_list_iterator();
      bucket_t*      pend_bucket = last_.get_bucket_list_iterator();
      local_iterator iprevious   = pbucket->before_begin();
      local_iterator icurrent    = first_.get_local_iterator();
      local_iterator iend        = last_.get_local_iterator(); // Note: May not be in the same bucket as icurrent.

      // Find the node previous to the first one.
      while (iprevious->etl_next != &*icurrent)
      {
        ++iprevious;
      }

      // Remember the item before the first erased one.
      iterator ibefore_erased = iterator((pbuckets + number_of_buckets), pbucket, iprevious);

      // Until we reach the end.
      while ((icurrent != iend) || (pbucket != pend_bucket))
      {
        icurrent = delete_data_node(iprevious, icurrent, *pbucket);

        // Have we not reached the end?
        if ((icurrent != iend) || (pbucket != pend_bucket))
        {
          // At the end of this bucket?
          if ((icurrent == pbucket->end()))
          {
            // Find the next non-empty one.
            do
            {
              ++pbucket;
            } while (pbucket->empty());

            iprevious = pbucket->before_begin();
            icurrent  = pbucket->begin();
          }
        }
      }

      return ++ibefore_erased;
    }

    //*************************************************************************
    /// Clears the unordered_multiset.
    //*************************************************************************
    void clear()
    {
      initialise();
    }

    //*********************************************************************
    /// Counts an element.
    ///\param key The key to search for.
    ///\return 1 if the key exists, otherwise 0.
    //*********************************************************************
    size_t count(key_parameter_t key) const
    {
      size_t n = 0UL;
      const_iterator f = find(key);
      const_iterator l = f;

      if (l != end())
      {
        ++l;
        ++n;

        while ((l != end()) && key_equal_function(key, *l))
        {
          ++l;
          ++n;
        }
      }

      return n;
    }

#if ETL_USING_CPP11
    //*********************************************************************
    /// Counts an element.
    ///\param key The key to search for.
    ///\return 1 if the key exists, otherwise 0.
    //*********************************************************************
    template <typename K, typename KE = TKeyEqual, etl::enable_if_t<comparator_is_transparent<KE>::value, int> = 0>
    size_t count(const K& key) const
    {
      size_t n = 0UL;
      const_iterator f = find(key);
      const_iterator l = f;

      if (l != end())
      {
        ++l;
        ++n;

        while ((l != end()) && key_equal_function(key, *l))
        {
          ++l;
          ++n;
        }
      }

      return n;
    }
#endif

    //*********************************************************************
    /// Finds an element.
    ///\param key The key to search for.
    ///\return An iterator to the element if the key exists, otherwise end().
    //*********************************************************************
    iterator find(key_parameter_t key)
    {
      size_t index = get_bucket_index(key);

      bucket_t* pbucket = pbuckets + index;
      bucket_t& bucket = *pbucket;

      // Is the bucket not empty?
      if (!bucket.empty())
      {
        // Step though the list until we find the end or an equivalent key.
        local_iterator inode = bucket.begin();
        local_iterator iend = bucket.end();

        while (inode != iend)
        {
          // Do we have this one?
          if (key_equal_function(key, inode->key))
          {
            return iterator((pbuckets + number_of_buckets), pbucket, inode);
          }

          ++inode;
        }
      }

      return end();
    }

#if ETL_USING_CPP11
    //*********************************************************************
    /// Finds an element.
    ///\param key The key to search for.
    ///\return An iterator to the element if the key exists, otherwise end().
    //*********************************************************************
    template <typename K, typename KE = TKeyEqual, etl::enable_if_t<comparator_is_transparent<KE>::value, int> = 0>
    iterator find(const K& key)
    {
      size_t index = get_bucket_index(key);

      bucket_t* pbucket = pbuckets + index;
      bucket_t& bucket = *pbucket;

      // Is the bucket not empty?
      if (!bucket.empty())
      {
        // Step though the list until we find the end or an equivalent key.
        local_iterator inode = bucket.begin();
        local_iterator iend = bucket.end();

        while (inode != iend)
        {
          // Do we have this one?
          if (key_equal_function(key, inode->key))
          {
            return iterator((pbuckets + number_of_buckets), pbucket, inode);
          }

          ++inode;
        }
      }

      return end();
    }
#endif

    //*********************************************************************
    /// Finds an element.
    ///\param key The key to search for.
    ///\return An iterator to the element if the key exists, otherwise end().
    //*********************************************************************
    const_iterator find(key_parameter_t key) const
    {
      size_t index = get_bucket_index(key);

      bucket_t* pbucket = pbuckets + index;
      bucket_t& bucket = *pbucket;

      // Is the bucket not empty?
      if (!bucket.empty())
      {
        // Step though the list until we find the end or an equivalent key.
        local_iterator inode = bucket.begin();
        local_iterator iend = bucket.end();

        while (inode != iend)
        {
          // Do we have this one?
          if (key_equal_function(key, inode->key))
          {
            return iterator((pbuckets + number_of_buckets), pbucket, inode);
          }

          ++inode;
        }
      }

      return end();
    }

#if ETL_USING_CPP11
    //*********************************************************************
    /// Finds an element.
    ///\param key The key to search for.
    ///\return An iterator to the element if the key exists, otherwise end().
    //*********************************************************************
    template <typename K, typename KE = TKeyEqual, etl::enable_if_t<comparator_is_transparent<KE>::value, int> = 0>
    const_iterator find(const K& key) const
    {
      size_t index = get_bucket_index(key);

      bucket_t* pbucket = pbuckets + index;
      bucket_t& bucket = *pbucket;

      // Is the bucket not empty?
      if (!bucket.empty())
      {
        // Step though the list until we find the end or an equivalent key.
        local_iterator inode = bucket.begin();
        local_iterator iend = bucket.end();

        while (inode != iend)
        {
          // Do we have this one?
          if (key_equal_function(key, inode->key))
          {
            return iterator((pbuckets + number_of_buckets), pbucket, inode);
          }

          ++inode;
        }
      }

      return end();
    }
#endif
    
    //*********************************************************************
    /// Returns a range containing all elements with key key in the container.
    /// The range is defined by two iterators, the first pointing to the first
    /// element of the wanted range and the second pointing past the last
    /// element of the range.
    ///\param key The key to search for.
    ///\return An iterator pair to the range of elements if the key exists, otherwise end().
    //*********************************************************************
    ETL_OR_STD::pair<iterator, iterator> equal_range(key_parameter_t key)
    {
      iterator f = find(key);
      iterator l = f;

      if (l != end())
      {
        ++l;

        while ((l != end()) && key_equal_function(key, *l))
        {
          ++l;
        }
      }

      return ETL_OR_STD::pair<iterator, iterator>(f, l);
    }

#if ETL_USING_CPP11
    //*********************************************************************
    /// Returns a range containing all elements with key key in the container.
    /// The range is defined by two iterators, the first pointing to the first
    /// element of the wanted range and the second pointing past the last
    /// element of the range.
    ///\param key The key to search for.
    ///\return An iterator pair to the range of elements if the key exists, otherwise end().
    //*********************************************************************
    template <typename K, typename KE = TKeyEqual, etl::enable_if_t<comparator_is_transparent<KE>::value, int> = 0>
    ETL_OR_STD::pair<iterator, iterator> equal_range(const K& key)
    {
      iterator f = find(key);
      iterator l = f;

      if (l != end())
      {
        ++l;

        while ((l != end()) && key_equal_function(key, *l))
        {
          ++l;
        }
      }

      return ETL_OR_STD::pair<iterator, iterator>(f, l);
    }
#endif

    //*********************************************************************
    /// Returns a range containing all elements with key key in the container.
    /// The range is defined by two iterators, the first pointing to the first
    /// element of the wanted range and the second pointing past the last
    /// element of the range.
    ///\param key The key to search for.
    ///\return A const iterator pair to the range of elements if the key exists, otherwise end().
    //*********************************************************************
    ETL_OR_STD::pair<const_iterator, const_iterator> equal_range(key_parameter_t key) const
    {
      const_iterator f = find(key);
      const_iterator l = f;

      if (l != end())
      {
        ++l;

        while ((l != end()) && key_equal_function(key, *l))
        {
          ++l;
        }
      }

      return ETL_OR_STD::pair<const_iterator, const_iterator>(f, l);
    }

#if ETL_USING_CPP11
    //*********************************************************************
    /// Returns a range containing all elements with key key in the container.
    /// The range is defined by two iterators, the first pointing to the first
    /// element of the wanted range and the second pointing past the last
    /// element of the range.
    ///\param key The key to search for.
    ///\return A const iterator pair to the range of elements if the key exists, otherwise end().
    //*********************************************************************
    template <typename K, typename KE = TKeyEqual, etl::enable_if_t<comparator_is_transparent<KE>::value, int> = 0>
    ETL_OR_STD::pair<const_iterator, const_iterator> equal_range(const K& key) const
    {
      const_iterator f = find(key);
      const_iterator l = f;

      if (l != end())
      {
        ++l;

        while ((l != end()) && key_equal_function(key, *l))
        {
          ++l;
        }
      }

      return ETL_OR_STD::pair<const_iterator, const_iterator>(f, l);
    }
#endif

    //*************************************************************************
    /// Gets the size of the unordered_multiset.
    //*************************************************************************
    size_type size() const
    {
      return pnodepool->size();
    }

    //*************************************************************************
    /// Gets the maximum possible size of the unordered_multiset.
    //*************************************************************************
    size_type max_size() const
    {
      return pnodepool->max_size();
    }

    //*************************************************************************
    /// Gets the maximum possible size of the unordered_multiset.
    //*************************************************************************
    size_type capacity() const
    {
      return pnodepool->max_size();
    }

    //*************************************************************************
    /// Checks to see if the unordered_multiset is empty.
    //*************************************************************************
    bool empty() const
    {
      return pnodepool->empty();
    }

    //*************************************************************************
    /// Checks to see if the unordered_multiset is full.
    //*************************************************************************
    bool full() const
    {
      return pnodepool->full();
    }

    //*************************************************************************
    /// Returns the remaining capacity.
    ///\return The remaining capacity.
    //*************************************************************************
    size_t available() const
    {
      return pnodepool->available();
    }

    //*************************************************************************
    /// Returns the load factor = size / bucket_count.
    ///\return The load factor = size / bucket_count.
    //*************************************************************************
    float load_factor() const
    {
      return static_cast<float>(size()) / static_cast<float>(bucket_count());
    }

    //*************************************************************************
    /// Returns the function that hashes the keys.
    ///\return The function that hashes the keys..
    //*************************************************************************
    hasher hash_function() const
    {
      return key_hash_function;
    }

    //*************************************************************************
    /// Returns the function that compares the keys.
    ///\return The function that compares the keys..
    //*************************************************************************
    key_equal key_eq() const
    {
      return key_equal_function;
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    iunordered_multiset& operator = (const iunordered_multiset& rhs)
    {
      // Skip if doing self assignment
      if (this != &rhs)
      {
        key_hash_function  = rhs.hash_function();
        key_equal_function = rhs.key_eq();
        assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    iunordered_multiset& operator = (iunordered_multiset&& rhs)
    {
      // Skip if doing self assignment
      if (this != &rhs)
      {
        clear();
        key_hash_function  = rhs.hash_function();
        key_equal_function = rhs.key_eq();
        move(rhs.begin(), rhs.end());
      }

      return *this;
    }
#endif

    //*************************************************************************
    /// Check if the unordered_multiset contains the key.
    //*************************************************************************
    bool contains(key_parameter_t key) const
    {
      return find(key) != end();
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Check if the unordered_map contains the key.
    //*************************************************************************
    template <typename K, typename KE = TKeyEqual, etl::enable_if_t<comparator_is_transparent<KE>::value, int> = 0>
    bool contains(const K& key) const
    {
      return find(key) != end();
    }
#endif

  protected:

    //*********************************************************************
    /// Constructor.
    //*********************************************************************
    iunordered_multiset(pool_t& node_pool_, bucket_t* pbuckets_, size_t number_of_buckets_, hasher key_hash_function_, key_equal key_equal_function_)
      : pnodepool(&node_pool_)
      , pbuckets(pbuckets_)
      , number_of_buckets(number_of_buckets_)
      , first(pbuckets)
      , last(pbuckets)
      , key_hash_function(key_hash_function_)
      , key_equal_function(key_equal_function_)
    {
    }

    //*********************************************************************
    /// Initialise the unordered_multiset.
    //*********************************************************************
    void initialise()
    {
      if (!empty())
      {
        // For each bucket...
        for (size_t i = 0UL; i < number_of_buckets; ++i)
        {
          bucket_t& bucket = pbuckets[i];

          if (!bucket.empty())
          {
            // For each item in the bucket...
            local_iterator it = bucket.begin();

            while (it != bucket.end())
            {
              // Destroy the value contents.
              it->key.~value_type();
              ++it;
              ETL_DECREMENT_DEBUG_COUNT;
            }

            // Now it's safe to clear the bucket.
            bucket.clear();
          }
        }

        // Now it's safe to clear the entire pool in one go.
        pnodepool->release_all();
      }

      first = pbuckets;
      last  = first;
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Move from a range
    //*************************************************************************
    void move(iterator b, iterator e)
    {
      while (b != e)
      {
        iterator temp = b;
        ++temp;
        insert(etl::move(*b));
        b = temp;
      }
    }
#endif

  private:

    //*************************************************************************
    /// Create a node.
    //*************************************************************************
    node_t* allocate_data_node()
    {
      node_t* (etl::ipool::*func)() = &etl::ipool::allocate<node_t>;
      return (pnodepool->*func)();
    }

    //*********************************************************************
    /// Adjust the first and last markers according to the new entry.
    //*********************************************************************
    void adjust_first_last_markers_after_insert(bucket_t* pbucket)
    {
      if (size() == 1)
      {
        first = pbucket;
        last  = pbucket;
      }
      else
      {
        if (pbucket < first)
        {
          first = pbucket;
        }
        else if (pbucket > last)
        {
          last = pbucket;
        }
      }
    }

    //*********************************************************************
    /// Adjust the first and last markers according to the erased entry.
    //*********************************************************************
    void adjust_first_last_markers_after_erase(bucket_t* pbucket)
    {
      if (empty())
      {
        first = pbuckets;
        last  = pbuckets;
      }
      else
      {
        if (pbucket == first)
        {
          // We erased the first so, we need to search again from where we erased.
          while (first->empty())
          {
            ++first;
          }
        }
        else if (pbucket == last)
        {
          // We erased the last, so we need to search again. Start from the first, go no further than the current last.
          pbucket = first;
          bucket_t* pend = last;

          last = first;

          while (pbucket != pend)
          {
            if (!pbucket->empty())
            {
              last = pbucket;
            }

            ++pbucket;
          }
        }
      }
    }

    //*********************************************************************
    /// Delete a data node at the specified location.
    //*********************************************************************
    local_iterator delete_data_node(local_iterator iprevious, local_iterator icurrent, bucket_t& bucket)
    {
      local_iterator inext = bucket.erase_after(iprevious); // Unlink from the bucket.
      icurrent->key.~value_type();                          // Destroy the value.
      pnodepool->release(&*icurrent);                       // Release it back to the pool.
      adjust_first_last_markers_after_erase(&bucket);
      ETL_DECREMENT_DEBUG_COUNT;

      return inext;
    }

    // Disable copy construction.
    iunordered_multiset(const iunordered_multiset&);

    /// The pool of data nodes used in the list.
    pool_t* pnodepool;

    /// The bucket list.
    bucket_t* pbuckets;

    /// The number of buckets.
    const size_t number_of_buckets;

    /// The first and last iterators to buckets with values.
    bucket_t* first;
    bucket_t* last;

    /// The function that creates the hashes.
    hasher key_hash_function;

    /// The function that compares the keys for equality.
    key_equal key_equal_function;

    /// For library debugging purposes only.
    ETL_DECLARE_DEBUG_COUNT;

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
#if defined(ETL_POLYMORPHIC_UNORDERED_MULTISET) || defined(ETL_POLYMORPHIC_CONTAINERS)
  public:
    virtual ~iunordered_multiset()
    {
    }
#else
  protected:
    ~iunordered_multiset()
    {
    }
#endif
  };

  //***************************************************************************
  /// Equal operator.
  ///\param lhs Reference to the first unordered_multiset.
  ///\param rhs Reference to the second unordered_multiset.
  ///\return <b>true</b> if the arrays are equal, otherwise <b>false</b>
  ///\ingroup unordered_multiset
  //***************************************************************************
  template <typename TKey, typename THash, typename TKeyEqual>
  bool operator ==(const etl::iunordered_multiset<TKey, THash, TKeyEqual>& lhs, 
                   const etl::iunordered_multiset<TKey, THash, TKeyEqual>& rhs)
  {
    const bool sizes_match = (lhs.size() == rhs.size());
    bool elements_match = true;

    typedef typename etl::iunordered_multiset<TKey, THash, TKeyEqual>::const_iterator itr_t;

    if (sizes_match)
    {
      itr_t l_begin = lhs.begin();
      itr_t l_end   = lhs.end();

      while ((l_begin != l_end) && elements_match)
      {
        const TKey l_value = *l_begin;

        // See if the lhs keys exist in the rhs.
        ETL_OR_STD::pair<itr_t, itr_t> l_range = lhs.equal_range(l_value);
        ETL_OR_STD::pair<itr_t, itr_t> r_range = rhs.equal_range(l_value);

        if (r_range.first != rhs.end())
        {
          bool distance_match = (etl::distance(l_range.first, l_range.second) == etl::distance(r_range.first, r_range.second));

          if (distance_match)
          {
            elements_match = etl::is_permutation(l_range.first, l_range.second, r_range.first, r_range.second);
          }
          else
          {
            elements_match = false;
          }
        }
        else
        {
          elements_match = false;
        }

        ++l_begin;
      }
    }

    return (sizes_match && elements_match);
  }

  //***************************************************************************
  /// Not equal operator.
  ///\param lhs Reference to the first unordered_multiset.
  ///\param rhs Reference to the second unordered_multiset.
  ///\return <b>true</b> if the arrays are not equal, otherwise <b>false</b>
  ///\ingroup unordered_multiset
  //***************************************************************************
  template <typename TKey, typename THash, typename TKeyEqual>
  bool operator !=(const etl::iunordered_multiset<TKey, THash, TKeyEqual>& lhs, 
                   const etl::iunordered_multiset<TKey, THash, TKeyEqual>& rhs)
  {
    return !(lhs == rhs);
  }

  //*************************************************************************
  /// A templated unordered_multiset implementation that uses a fixed size buffer.
  //*************************************************************************
  template <typename TKey, const size_t MAX_SIZE_, size_t MAX_BUCKETS_ = MAX_SIZE_, typename THash = etl::hash<TKey>, typename TKeyEqual = etl::equal_to<TKey> >
  class unordered_multiset : public etl::iunordered_multiset<TKey, THash, TKeyEqual>
  {
  private:

    typedef etl::iunordered_multiset<TKey, THash, TKeyEqual> base;

  public:

    static ETL_CONSTANT size_t MAX_SIZE    = MAX_SIZE_;
    static ETL_CONSTANT size_t MAX_BUCKETS = MAX_BUCKETS_;


    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    unordered_multiset(const THash& hash = THash(), const TKeyEqual& equal = TKeyEqual())
      : base(node_pool, buckets, MAX_BUCKETS, hash, equal)
    {
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    unordered_multiset(const unordered_multiset& other)
      : base(node_pool, buckets, MAX_BUCKETS, other.hash_function(), other.key_eq())
    {
      // Skip if doing self assignment
      if (this != &other)
      {
        base::assign(other.cbegin(), other.cend());
      }
    }


#if ETL_USING_CPP11
    //*************************************************************************
    /// Move constructor.
    //*************************************************************************
    unordered_multiset(unordered_multiset&& other)
      : base(node_pool, buckets, MAX_BUCKETS, other.hash_function(), other.key_eq())
    {
      // Skip if doing self assignment
      if (this != &other)
      {
        base::move(other.begin(), other.end());
      }
    }
#endif

    //*************************************************************************
    /// Constructor, from an iterator range.
    ///\tparam TIterator The iterator type.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*************************************************************************
    template <typename TIterator>
    unordered_multiset(TIterator first_, TIterator last_, const THash& hash = THash(), const TKeyEqual& equal = TKeyEqual())
      : base(node_pool, buckets, MAX_BUCKETS, hash, equal)
    {
      base::assign(first_, last_);
    }

#if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    /// Construct from initializer_list.
    //*************************************************************************
    unordered_multiset(std::initializer_list<TKey> init, const THash& hash = THash(), const TKeyEqual& equal = TKeyEqual())
      : base(node_pool, buckets, MAX_BUCKETS, hash, equal)
    {
      base::assign(init.begin(), init.end());
    }
#endif

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~unordered_multiset()
    {
      base::initialise();
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    unordered_multiset& operator = (const unordered_multiset& rhs)
    {
      base::operator =(rhs);

      return *this;
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Move assignment operator.
    //*************************************************************************
    unordered_multiset& operator = (unordered_multiset&& rhs)
    {
      base::operator =(etl::move(rhs));

      return *this;
    }
#endif

  private:

    /// The pool of nodes used for the unordered_multiset.
    etl::pool<typename base::node_t, MAX_SIZE> node_pool;

    /// The buckets of node lists.
    typename base::bucket_t buckets[MAX_BUCKETS_];
  };

  //*************************************************************************
  /// Template deduction guides.
  //*************************************************************************
#if ETL_USING_CPP17 && ETL_HAS_INITIALIZER_LIST
  template <typename... T>
  unordered_multiset(T...) -> unordered_multiset<etl::nth_type_t<0, T...>, sizeof...(T)>;
#endif

  //*************************************************************************
  /// Make
  //*************************************************************************
#if ETL_USING_CPP11 && ETL_HAS_INITIALIZER_LIST
  template <typename TKey, typename THash = etl::hash<TKey>, typename TKeyEqual = etl::equal_to<TKey>, typename... T>
  constexpr auto make_unordered_multiset(T&&... keys) -> etl::unordered_multiset<TKey, sizeof...(T), sizeof...(T), THash, TKeyEqual>
  {
    return { etl::forward<T>(keys)... };
  }
#endif
}

#endif
