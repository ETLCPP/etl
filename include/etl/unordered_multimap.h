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

#ifndef ETL_UNORDERED_MULTIMAP_INCLUDED
#define ETL_UNORDERED_MULTIMAP_INCLUDED

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
#include "pool.h"
#include "error_handler.h"
#include "exception.h"
#include "debug_count.h"
#include "iterator.h"
#include "placement_new.h"
#include "initializer_list.h"

#include <stddef.h>

//*****************************************************************************
///\defgroup unordered_multimap unordered_multimap
/// A unordered_multimap with the capacity defined at compile time.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //***************************************************************************
  /// Exception for the unordered_multimap.
  ///\ingroup unordered_multimap
  //***************************************************************************
  class unordered_multimap_exception : public etl::exception
  {
  public:

    unordered_multimap_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : etl::exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Full exception for the unordered_multimap.
  ///\ingroup unordered_multimap
  //***************************************************************************
  class unordered_multimap_full : public etl::unordered_multimap_exception
  {
  public:

    unordered_multimap_full(string_type file_name_, numeric_type line_number_)
      : etl::unordered_multimap_exception(ETL_ERROR_TEXT("unordered_multimap:full", ETL_UNORDERED_MULTIMAP_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Out of range exception for the unordered_multimap.
  ///\ingroup unordered_multimap
  //***************************************************************************
  class unordered_multimap_out_of_range : public etl::unordered_multimap_exception
  {
  public:

    unordered_multimap_out_of_range(string_type file_name_, numeric_type line_number_)
      : etl::unordered_multimap_exception(ETL_ERROR_TEXT("unordered_multimap:range", ETL_UNORDERED_MULTIMAP_FILE_ID"B"), file_name_, line_number_)
    {}
  };

  //***************************************************************************
  /// Iterator exception for the unordered_multimap.
  ///\ingroup unordered_multimap
  //***************************************************************************
  class unordered_multimap_iterator : public etl::unordered_multimap_exception
  {
  public:

    unordered_multimap_iterator(string_type file_name_, numeric_type line_number_)
      : etl::unordered_multimap_exception(ETL_ERROR_TEXT("unordered_multimap:iterator", ETL_UNORDERED_MULTIMAP_FILE_ID"C"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// The base class for specifically sized unordered_multimap.
  /// Can be used as a reference type for all unordered_multimap containing a specific type.
  ///\ingroup unordered_multimap
  //***************************************************************************
  template <typename TKey, typename T, typename THash = etl::hash<TKey>, typename TKeyEqual = etl::equal_to<TKey> >
  class iunordered_multimap
  {
  public:

    typedef ETL_OR_STD::pair<const TKey, T> value_type;

    typedef TKey              key_type;
    typedef T                 mapped_type;
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

    typedef etl::forward_link<0> link_t; // Default link.

    //*********************************************************************
    // The nodes that store the elements.
    struct node_t : public link_t
    {
      node_t(const_reference key_value_pair_)
        : key_value_pair(key_value_pair_)
      {
      }

      value_type key_value_pair;
    };

    friend bool operator ==(const node_t& lhs, const node_t& rhs)
    {
      return (lhs.key_value_pair.first  == rhs.key_value_pair.first) &&
             (lhs.key_value_pair.second == rhs.key_value_pair.second);
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
    class iterator : public etl::iterator<ETL_OR_STD::forward_iterator_tag, T>
    {
    public:

      typedef typename etl::iterator<ETL_OR_STD::forward_iterator_tag, T>::value_type value_type;
      typedef typename iunordered_multimap::key_type        key_type;
      typedef typename iunordered_multimap::mapped_type     mapped_type;
      typedef typename iunordered_multimap::hasher          hasher;
      typedef typename iunordered_multimap::key_equal       key_equal;
      typedef typename iunordered_multimap::reference       reference;
      typedef typename iunordered_multimap::const_reference const_reference;
      typedef typename iunordered_multimap::pointer         pointer;
      typedef typename iunordered_multimap::const_pointer   const_pointer;
      typedef typename iunordered_multimap::size_type       size_type;

      friend class iunordered_multimap;
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
        return inode->key_value_pair;
      }

      //*********************************
      pointer operator &() const
      {
        return &(inode->key_value_pair);
      }

      //*********************************
      pointer operator ->() const
      {
        return &(inode->key_value_pair);
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
      local_iterator       inode;
    };

    //*********************************************************************
    class const_iterator : public etl::iterator<ETL_OR_STD::forward_iterator_tag, const T>
    {
    public:

      typedef typename etl::iterator<ETL_OR_STD::forward_iterator_tag, const T>::value_type value_type;
      typedef typename iunordered_multimap::key_type        key_type;
      typedef typename iunordered_multimap::mapped_type     mapped_type;
      typedef typename iunordered_multimap::hasher          hasher;
      typedef typename iunordered_multimap::key_equal       key_equal;
      typedef typename iunordered_multimap::reference       reference;
      typedef typename iunordered_multimap::const_reference const_reference;
      typedef typename iunordered_multimap::pointer         pointer;
      typedef typename iunordered_multimap::const_pointer   const_pointer;
      typedef typename iunordered_multimap::size_type       size_type;

      friend class iunordered_multimap;
      friend class iterator;

      //*********************************
      const_iterator()
      {
      }

      //*********************************
      const_iterator(const typename iunordered_multimap::iterator& other)
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
        return inode->key_value_pair;
      }

      //*********************************
      const_pointer operator &() const
      {
        return &(inode->key_value_pair);
      }

      //*********************************
      const_pointer operator ->() const
      {
        return &(inode->key_value_pair);
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
      local_iterator       inode;
    };

    typedef typename etl::iterator_traits<iterator>::difference_type difference_type;

    //*********************************************************************
    /// Returns an iterator to the beginning of the unordered_multimap.
    ///\return An iterator to the beginning of the unordered_multimap.
    //*********************************************************************
    iterator begin()
    {
      return iterator((pbuckets + number_of_buckets), first, first->begin());
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the unordered_multimap.
    ///\return A const iterator to the beginning of the unordered_multimap.
    //*********************************************************************
    const_iterator begin() const
    {
      return const_iterator((pbuckets + number_of_buckets), first, first->begin());
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the unordered_multimap.
    ///\return A const iterator to the beginning of the unordered_multimap.
    //*********************************************************************
    const_iterator cbegin() const
    {
      return const_iterator((pbuckets + number_of_buckets), first, first->begin());
    }

    //*********************************************************************
    /// Returns an iterator to the beginning of the unordered_multimap bucket.
    ///\return An iterator to the beginning of the unordered_multimap bucket.
    //*********************************************************************
    local_iterator begin(size_t i)
    {
      return pbuckets[i].begin();
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the unordered_multimap bucket.
    ///\return A const iterator to the beginning of the unordered_multimap bucket.
    //*********************************************************************
    const_local_iterator begin(size_t i) const
    {
      return pbuckets[i].cbegin();
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the unordered_multimap bucket.
    ///\return A const iterator to the beginning of the unordered_multimap bucket.
    //*********************************************************************
    const_local_iterator cbegin(size_t i) const
    {
      return pbuckets[i].cbegin();
    }

    //*********************************************************************
    /// Returns an iterator to the end of the unordered_multimap.
    ///\return An iterator to the end of the unordered_multimap.
    //*********************************************************************
    iterator end()
    {
      return iterator((pbuckets + number_of_buckets), last, last->end());
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the unordered_multimap.
    ///\return A const iterator to the end of the unordered_multimap.
    //*********************************************************************
    const_iterator end() const
    {
      return const_iterator((pbuckets + number_of_buckets), last, last->end());
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the unordered_multimap.
    ///\return A const iterator to the end of the unordered_multimap.
    //*********************************************************************
    const_iterator cend() const
    {
      return const_iterator((pbuckets + number_of_buckets), last, last->end());
    }

    //*********************************************************************
    /// Returns an iterator to the end of the unordered_multimap bucket.
    ///\return An iterator to the end of the unordered_multimap bucket.
    //*********************************************************************
    local_iterator end(size_t i)
    {
      return pbuckets[i].end();
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the unordered_multimap bucket.
    ///\return A const iterator to the end of the unordered_multimap bucket.
    //*********************************************************************
    const_local_iterator end(size_t i) const
    {
      return pbuckets[i].cend();
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the unordered_multimap bucket.
    ///\return A const iterator to the end of the unordered_multimap bucket.
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

    //*********************************************************************
    /// Returns the size of the bucket key.
    ///\return The bucket size of the bucket key.
    //*********************************************************************
    size_type bucket_size(key_parameter_t key) const
    {
      size_t index = bucket(key);

      return etl::distance(pbuckets[index].begin(), pbuckets[index].end());
    }

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
    /// Assigns values to the unordered_multimap.
    /// If asserts or exceptions are enabled, emits unordered_multimap_full if the unordered_multimap does not have enough free space.
    /// If asserts or exceptions are enabled, emits unordered_multimap_iterator if the iterators are reversed.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*********************************************************************
    template <typename TIterator>
    void assign(TIterator first_, TIterator last_)
    {
#if ETL_IS_DEBUG_BUILD
      difference_type d = etl::distance(first_, last_);
      ETL_ASSERT(d >= 0, ETL_ERROR(unordered_multimap_iterator));
      ETL_ASSERT(size_t(d) <= max_size(), ETL_ERROR(unordered_multimap_full));
#endif

      clear();

      while (first_ != last_)
      {
        insert(*first_);
        ++first_;
      }
    }

    //*********************************************************************
    /// Inserts a value to the unordered_multimap.
    /// If asserts or exceptions are enabled, emits unordered_multimap_full if the unordered_multimap is already full.
    ///\param value The value to insert.
    //*********************************************************************
    iterator insert(const_reference key_value_pair)
    {
      iterator result = end();

      ETL_ASSERT(!full(), ETL_ERROR(unordered_multimap_full));

      const key_type&    key = key_value_pair.first;

      // Get the hash index.
      size_t index = get_bucket_index(key);

      // Get the bucket & bucket iterator.
      bucket_t* pbucket = pbuckets + index;
      bucket_t& bucket = *pbucket;

      // The first one in the bucket?
      if (bucket.empty())
      {
        // Get a new node.
        node_t& node = create_data_node();
        ::new (&node.key_value_pair) value_type(key_value_pair);
        ETL_INCREMENT_DEBUG_COUNT

        // Just add the pointer to the bucket;
        bucket.insert_after(bucket.before_begin(), node);
        adjust_first_last_markers_after_insert(pbucket);

        result = iterator((pbuckets + number_of_buckets), pbucket, pbucket->begin());
      }
      else
      {
        // Step though the bucket looking for a place to insert.
        local_iterator inode_previous = bucket.before_begin();
        local_iterator inode = bucket.begin();

        while (inode != bucket.end())
        {
          // Do we already have this key?
          if (key_equal_function(inode->key_value_pair.first, key))
          {
            break;
          }

          ++inode_previous;
          ++inode;
        }

        // Get a new node.
        node_t& node = create_data_node();
        ::new (&node.key_value_pair) value_type(key_value_pair);
        ETL_INCREMENT_DEBUG_COUNT

        // Add the node to the end of the bucket;
        bucket.insert_after(inode_previous, node);
        adjust_first_last_markers_after_insert(&bucket);
        ++inode_previous;

        result = iterator((pbuckets + number_of_buckets), pbucket, inode_previous);
      }

      return result;
    }

#if ETL_USING_CPP11
    //*********************************************************************
    /// Inserts a value to the unordered_multimap.
    /// If asserts or exceptions are enabled, emits unordered_multimap_full if the unordered_multimap is already full.
    ///\param value The value to insert.
    //*********************************************************************
    iterator insert(rvalue_reference key_value_pair)
    {
      iterator result = end();

      ETL_ASSERT(!full(), ETL_ERROR(unordered_multimap_full));

      const key_type&    key = key_value_pair.first;

      // Get the hash index.
      size_t index = get_bucket_index(key);

      // Get the bucket & bucket iterator.
      bucket_t* pbucket = pbuckets + index;
      bucket_t& bucket = *pbucket;

      // The first one in the bucket?
      if (bucket.empty())
      {
        // Get a new node.
        node_t& node = create_data_node();
        ::new (&node.key_value_pair) value_type(etl::move(key_value_pair));
        ETL_INCREMENT_DEBUG_COUNT

          // Just add the pointer to the bucket;
          bucket.insert_after(bucket.before_begin(), node);
        adjust_first_last_markers_after_insert(pbucket);

        result = iterator((pbuckets + number_of_buckets), pbucket, pbucket->begin());
      }
      else
      {
        // Step though the bucket looking for a place to insert.
        local_iterator inode_previous = bucket.before_begin();
        local_iterator inode = bucket.begin();

        while (inode != bucket.end())
        {
          // Do we already have this key?
          if (key_equal_function(inode->key_value_pair.first, key))
          {
            break;
          }

          ++inode_previous;
          ++inode;
        }

        // Get a new node.
        node_t& node = create_data_node();
        ::new (&node.key_value_pair) value_type(etl::move(key_value_pair));
        ETL_INCREMENT_DEBUG_COUNT

          // Add the node to the end of the bucket;
          bucket.insert_after(inode_previous, node);
        adjust_first_last_markers_after_insert(&bucket);
        ++inode_previous;

        result = iterator((pbuckets + number_of_buckets), pbucket, inode_previous);
      }

      return result;
    }
#endif

    //*********************************************************************
    /// Inserts a value to the unordered_multimap.
    /// If asserts or exceptions are enabled, emits unordered_multimap_full if the unordered_multimap is already full.
    ///\param position The position to insert at.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(const_iterator, const_reference key_value_pair)
    {
      return insert(key_value_pair);
    }

#if ETL_USING_CPP11
    //*********************************************************************
    /// Inserts a value to the unordered_multimap.
    /// If asserts or exceptions are enabled, emits unordered_multimap_full if the unordered_multimap is already full.
    ///\param position The position to insert at.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(const_iterator, rvalue_reference key_value_pair)
    {
      return insert(etl::move(key_value_pair));
    }
#endif

    //*********************************************************************
    /// Inserts a range of values to the unordered_multimap.
    /// If asserts or exceptions are enabled, emits unordered_multimap_full if the unordered_multimap does not have enough free space.
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
        if (key_equal_function(icurrent->key_value_pair.first, key))
        {
          bucket.erase_after(iprevious);          // Unlink from the bucket.
          icurrent->key_value_pair.~value_type(); // Destroy the value.
          pnodepool->release(&*icurrent);         // Release it back to the pool.
          adjust_first_last_markers_after_erase(&bucket);
          ++n;
          icurrent = iprevious;
          ETL_DECREMENT_DEBUG_COUNT
        }
        else
        {
          ++iprevious;
        }

        ++icurrent;
      }

      return n;
    }

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

      bucket.erase_after(iprevious);          // Unlink from the bucket.
      icurrent->key_value_pair.~value_type(); // Destroy the value.
      pnodepool->release(&*icurrent);         // Release it back to the pool.
      adjust_first_last_markers_after_erase(&bucket);
      ETL_DECREMENT_DEBUG_COUNT

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

      // Make a note of the last.
      iterator result((pbuckets + number_of_buckets), last_.get_bucket_list_iterator(), last_.get_local_iterator());

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

      // Until we reach the end.
      while ((icurrent != iend) || (pbucket != pend_bucket))
      {

        local_iterator inext = pbucket->erase_after(iprevious); // Unlink from the bucket.
        icurrent->key_value_pair.~value_type(); // Destroy the value.
        pnodepool->release(&*icurrent);         // Release it back to the pool.
        adjust_first_last_markers_after_erase(pbucket);
        ETL_DECREMENT_DEBUG_COUNT

        icurrent = inext;

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

      return result;
    }

    //*************************************************************************
    /// Clears the unordered_multimap.
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

        while ((l != end()) && key_equal_function(key, l->first))
        {
          ++l;
          ++n;
        }
      }

      return n;
    }

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
          if (key_equal_function(key, inode->key_value_pair.first))
          {
            return iterator((pbuckets + number_of_buckets), pbucket, inode);
          }

          ++inode;
        }
      }

      return end();
    }

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
          if (key_equal_function(key, inode->key_value_pair.first))
          {
            return const_iterator((pbuckets + number_of_buckets), pbucket, inode);
          }

          ++inode;
        }
      }

      return end();
    }

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

        while ((l != end()) && key_equal_function(key, l->first))
        {
          ++l;
        }
      }

      return ETL_OR_STD::pair<iterator, iterator>(f, l);
    }

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

        while ((l != end()) && key_equal_function(key, l->first))
        {
          ++l;
        }
      }

      return ETL_OR_STD::pair<const_iterator, const_iterator>(f, l);
    }

    //*************************************************************************
    /// Gets the size of the unordered_multimap.
    //*************************************************************************
    size_type size() const
    {
      return pnodepool->size();
    }

    //*************************************************************************
    /// Gets the maximum possible size of the unordered_multimap.
    //*************************************************************************
    size_type max_size() const
    {
      return pnodepool->max_size();
    }

    //*************************************************************************
    /// Gets the maximum possible size of the unordered_multimap.
    //*************************************************************************
    size_type capacity() const
    {
      return pnodepool->max_size();
    }

    //*************************************************************************
    /// Checks to see if the unordered_multimap is empty.
    //*************************************************************************
    bool empty() const
    {
      return pnodepool->empty();
    }

    //*************************************************************************
    /// Checks to see if the unordered_multimap is full.
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
    iunordered_multimap& operator = (const iunordered_multimap& rhs)
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
    /// Move assignment operator.
    //*************************************************************************
    iunordered_multimap& operator = (iunordered_multimap&& rhs)
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

  protected:

    //*********************************************************************
    /// Constructor.
    //*********************************************************************
    iunordered_multimap(pool_t& node_pool_, bucket_t* pbuckets_, size_t number_of_buckets_, hasher key_hash_function_, key_equal key_equal_function_)
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
    /// Initialise the unordered_multimap.
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
              it->key_value_pair.~value_type();
              ++it;
              ETL_DECREMENT_DEBUG_COUNT
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
    void move(iterator first, iterator last)
    {
      while (first != last)
      {
        iterator temp = first;
        ++temp;
        insert(etl::move(*first));
        first = temp;
      }
    }
#endif

  private:

    //*************************************************************************
    /// Create a node.
    //*************************************************************************
    node_t& create_data_node()
    {
      node_t* (etl::ipool::*func)() = &etl::ipool::allocate<node_t>;
      return *(pnodepool->*func)();
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
    void adjust_first_last_markers_after_erase(bucket_t* pcurrent)
    {
      if (empty())
      {
        first = pbuckets;
        last = pbuckets;
      }
      else
      {
        if (pcurrent == first)
        {
          // We erased the first so, we need to search again from where we erased.
          while (first->empty())
          {
            ++first;
          }
        }
        else if (pcurrent == last)
        {
          // We erased the last, so we need to search again. Start from the first, go no further than the current last.
          bucket_t* pcurrent = first;
          bucket_t* pend = last;

          last = first;

          while (pcurrent != pend)
          {
            if (!pcurrent->empty())
            {
              last = pcurrent;
            }

            ++pcurrent;
          }
        }
      }
    }

    // Disable copy construction.
    iunordered_multimap(const iunordered_multimap&);

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
    ETL_DECLARE_DEBUG_COUNT

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
#if defined(ETL_POLYMORPHIC_UNORDERED_MULTIMAP) || defined(ETL_POLYMORPHIC_CONTAINERS)
  public:
    virtual ~iunordered_multimap()
    {
    }
#else
  protected:
    ~iunordered_multimap()
    {
    }
#endif
  };

  //***************************************************************************
  /// Equal operator.
  ///\param lhs Reference to the first unordered_multimap.
  ///\param rhs Reference to the second unordered_multimap.
  ///\return <b>true</b> if the arrays are equal, otherwise <b>false</b>
  ///\ingroup unordered_multimap
  //***************************************************************************
  template <typename TKey, typename TMapped, typename TKeyCompare>
  bool operator ==(const etl::iunordered_multimap<TKey, TMapped, TKeyCompare>& lhs, const etl::iunordered_multimap<TKey, TMapped, TKeyCompare>& rhs)
  {
    const bool sizes_match = (lhs.size() == rhs.size());
    bool elements_match = true;

    if (sizes_match)
    {
      for (size_t i = 0; (i < lhs.bucket_count()) && elements_match; ++i)
      {
        if (!etl::is_permutation(lhs.begin(i), lhs.end(i), rhs.begin(i)))
        {
          elements_match = false;
        }
      }
    }

    return (sizes_match && elements_match);
  }

  //***************************************************************************
  /// Not equal operator.
  ///\param lhs Reference to the first unordered_multimap.
  ///\param rhs Reference to the second unordered_multimap.
  ///\return <b>true</b> if the arrays are not equal, otherwise <b>false</b>
  ///\ingroup unordered_multimap
  //***************************************************************************
  template <typename TKey, typename TMapped, typename TKeyCompare>
  bool operator !=(const etl::iunordered_multimap<TKey, TMapped, TKeyCompare>& lhs, const etl::iunordered_multimap<TKey, TMapped, TKeyCompare>& rhs)
  {
    return !(lhs == rhs);
  }

  //*************************************************************************
  /// A templated unordered_multimap implementation that uses a fixed size buffer.
  //*************************************************************************
  template <typename TKey, typename TValue, const size_t MAX_SIZE_, const size_t MAX_BUCKETS_ = MAX_SIZE_, typename THash = etl::hash<TKey>, typename TKeyEqual = etl::equal_to<TKey> >
  class unordered_multimap : public etl::iunordered_multimap<TKey, TValue, THash, TKeyEqual>
  {
  private:

    typedef etl::iunordered_multimap<TKey, TValue, THash, TKeyEqual> base;

  public:

    static ETL_CONSTANT size_t MAX_SIZE    = MAX_SIZE_;
    static ETL_CONSTANT size_t MAX_BUCKETS = MAX_BUCKETS_;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    unordered_multimap(const THash& hash = THash(), const TKeyEqual& equal = TKeyEqual())
      : base(node_pool, buckets, MAX_BUCKETS, hash, equal)
    {
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    unordered_multimap(const unordered_multimap& other)
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
    unordered_multimap(unordered_multimap&& other)
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
    unordered_multimap(TIterator first_, TIterator last_, const THash& hash = THash(), const TKeyEqual& equal = TKeyEqual())
      : base(node_pool, buckets, MAX_BUCKETS, hash, equal)
    {
      base::assign(first_, last_);
    }

#if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    /// Construct from initializer_list.
    //*************************************************************************
    unordered_multimap(std::initializer_list<ETL_OR_STD::pair<TKey, TValue>> init, const THash& hash = THash(), const TKeyEqual& equal = TKeyEqual())
      : base(node_pool, buckets, MAX_BUCKETS_, hash, equal)
    {
      base::assign(init.begin(), init.end());
    }
#endif

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~unordered_multimap()
    {
      base::initialise();
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    unordered_multimap& operator = (const unordered_multimap& rhs)
    {
      base::operator=(rhs);

      return *this;
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Move assignment operator.
    //*************************************************************************
    unordered_multimap& operator = (unordered_multimap&& rhs)
    {
      base::operator=(etl::move(rhs));

      return *this;
    }
#endif

  private:

    /// The pool of nodes used for the unordered_multimap.
    etl::pool<typename base::node_t, MAX_SIZE> node_pool;

    /// The buckets of node lists.
    typename base::bucket_t buckets[MAX_BUCKETS_];
  };

  //*************************************************************************
  /// Template deduction guides.
  //*************************************************************************
#if ETL_USING_CPP17 && ETL_HAS_INITIALIZER_LIST
  template <typename... TPairs>
  unordered_multimap(TPairs...) -> unordered_multimap<typename etl::nth_type_t<0, TPairs...>::first_type,
                                                      typename etl::nth_type_t<0, TPairs...>::second_type,
                                                      sizeof...(TPairs)>;
#endif

  //*************************************************************************
  /// Make
  //*************************************************************************
#if ETL_USING_CPP11 && ETL_HAS_INITIALIZER_LIST
  template <typename TKey, typename T, typename THash = etl::hash<TKey>, typename TKeyEqual = etl::equal_to<TKey>, typename... TPairs>
  constexpr auto make_unordered_multimap(TPairs&&... pairs) -> etl::unordered_multimap<TKey, T, sizeof...(TPairs), sizeof...(TPairs), THash, TKeyEqual>
  {
    return { {etl::forward<TPairs>(pairs)...} };
  }
#endif
}

#endif
