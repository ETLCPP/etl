#pragma once

#include "functional.h"
#include "algorithm.h"
#include "array.h"
#include "flat_map.h"
#include "static_assert.h"
#include "type_traits.h"
#include "integral_limits.h"

namespace etl
{
  //***************************************************************************
  /// Histogram with a compile time start index.
  //***************************************************************************
  template <typename TKey, typename TCount, size_t Max_Keys, size_t Start_Index = etl::integral_limits<size_t>::max>
  class histogram 
    : public etl::unary_function<TKey, void>
  {
  public:

    ETL_STATIC_ASSERT(etl::is_integral<TKey>::value, "Only integral keys allowed");

    static ETL_CONSTANT size_t Max_Size = Max_Keys;

    typedef TKey   key_type;
    typedef TCount count_type;
    typedef TCount value_type;
    typedef typename etl::array<TCount, Max_Size>::const_iterator const_iterator;

    //*********************************
    /// Constructor
    //*********************************
    histogram()
    {
      accumulator.fill(count_type(0));
    }

    //*********************************
    /// Constructor
    //*********************************
    template <typename TIterator>
    histogram(TIterator first, TIterator last)
    {
      accumulator.fill(count_type(0));
      add(first, last);
    }

    //*********************************
    /// Copy constructor
    //*********************************
    histogram(const histogram& other)
    {
      accumulator = other.accumulator;
    }

#if ETL_CPP11_SUPPORTED
    //*********************************
    /// Move constructor
    //*********************************
    histogram(histogram&& other)
    {
      accumulator = etl::move(other.accumulator);
    }
#endif

    //*********************************
    /// Copy assignment
    //*********************************
    histogram& operator =(const histogram& rhs)
    {
      accumulator = rhs.accumulator;

      return *this;
    }

#if ETL_CPP11_SUPPORTED
    //*********************************
    /// Move assignment
    //*********************************
    histogram& operator =(histogram&& rhs)
    {
      accumulator = etl::move(rhs.accumulator);

      return *this;
    }
#endif

    //*********************************
    /// Beginning of the histogram.
    //*********************************
    const_iterator begin() const
    {
      return accumulator.begin();
    }

    //*********************************
    /// Beginning of the histogram.
    //*********************************
    const_iterator cbegin() const
    {
      return accumulator.cbegin();
    }

    //*********************************
    /// End of the histogram.
    //*********************************
    const_iterator end() const
    {
      return accumulator.begin();
    }

    //*********************************
    /// End of the histogram.
    //*********************************
    const_iterator cend() const
    {
      return accumulator.cbegin();
    }

    //*********************************
    /// Add
    //*********************************
    void add(key_type key)
    {
      ++accumulator[key - Start_Index];
    }

    //*********************************
    /// Add
    //*********************************
    template <typename TIterator>
    void add(TIterator first, TIterator last)
    {
      while (first != last)
      {
        add(*first++);
      }
    }

    //*********************************
    /// operator ()
    //*********************************
    void operator ()(key_type key)
    {
      add(key);
    }

    //*********************************
    /// operator ()
    //*********************************
    template <typename TIterator>
    void operator ()(TIterator first, TIterator last)
    {
      add(first, last);
    }

    //*********************************
    /// operator []
    //*********************************
    value_type operator [](key_type key) const
    {
      return accumulator[key];
    }

    //*********************************
    /// Clear the histogram.
    //*********************************
    void clear()
    {
      accumulator.fill(count_type(0));
    }

    //*********************************
    /// Size of the histogram.
    //*********************************
    ETL_CONSTEXPR size_t size() const
    {
      return Max_Keys;
    }

    //*********************************
    /// Max size of the histogram.
    //*********************************
    ETL_CONSTEXPR size_t max_size() const
    {
      return Max_Keys;
    }

    //*********************************
    /// Count of items in the histogram.
    //*********************************
    size_t count() const
    {
      return etl::accumulate(accumulator.begin(), accumulator.end(), size_t(0));
    }

  private:

    etl::array<count_type, Max_Size> accumulator;
  };

  //***************************************************************************
  /// Histogram with a run time start index.
  //***************************************************************************
  template<typename TKey, typename TCount, size_t Max_Keys>
  class histogram<TKey, TCount, Max_Keys, etl::integral_limits<size_t>::max>
    : public etl::unary_function<TKey, void>
  {
  public:

    ETL_STATIC_ASSERT(etl::is_integral<TKey>::value, "Only integral keys allowed");

    static ETL_CONSTANT size_t Max_Size = Max_Keys;

    typedef TKey   key_type;
    typedef TCount count_type;
    typedef TCount value_type;
    typedef typename etl::array<TCount, Max_Size>::const_iterator const_iterator;

    //*********************************
    /// Constructor
    //*********************************
    explicit histogram(key_type start_index_)
      : start_index(start_index_)
    {
      accumulator.fill(count_type(0));
    }

    //*********************************
    /// Constructor
    //*********************************
    template <typename TIterator>
    histogram(key_type start_index_, TIterator first, TIterator last)
      : start_index(start_index_)
    {
      accumulator.fill(count_type(0));
      add(first, last);
    }

    //*********************************
    /// Copy constructor
    //*********************************
    histogram(const histogram& other)
    {
      accumulator = other.accumulator;
    }

#if ETL_CPP11_SUPPORTED
    //*********************************
    /// Move constructor
    //*********************************
    histogram(histogram&& other)
    {
      accumulator = etl::move(other.accumulator);
    }
#endif

    //*********************************
    /// Copy assignment
    //*********************************
    histogram& operator =(const histogram& rhs)
    {
      accumulator = rhs.accumulator;

      return *this;
    }

#if ETL_CPP11_SUPPORTED
    //*********************************
    /// Move assignment
    //*********************************
    histogram& operator =(histogram&& rhs)
    {
      accumulator = etl::move(rhs.accumulator);

      return *this;
    }
#endif

    //*********************************
    /// Beginning of the histogram.
    //*********************************
    const_iterator begin() const
    {
      return accumulator.begin();
    }

    //*********************************
    /// Beginning of the histogram.
    //*********************************
    const_iterator cbegin() const
    {
      return accumulator.cbegin();
    }

    //*********************************
    /// End of the histogram.
    //*********************************
    const_iterator end() const
    {
      return accumulator.begin();
    }

    //*********************************
    /// End of the histogram.
    //*********************************
    const_iterator cend() const
    {
      return accumulator.cbegin();
    }

    //*********************************
    /// Add
    //*********************************
    void add(key_type key)
    {
      ++accumulator[key - start_index];
    }

    //*********************************
    /// Add
    //*********************************
    template <typename TIterator>
    void add(TIterator first, TIterator last)
    {
      while (first != last)
      {
        add(*first++);
      }
    }

    //*********************************
    /// operator ()
    //*********************************
    void operator ()(key_type key)
    {
      add(key);
    }

    //*********************************
    /// operator ()
    //*********************************
    template <typename TIterator>
    void operator ()(TIterator first, TIterator last)
    {
      add(first, last);
    }

    //*********************************
    /// operator []
    //*********************************
    value_type operator [](key_type key) const
    {
      return accumulator[key];
    }

    //*********************************
    /// Clear the histogram.
    //*********************************
    void clear()
    {
      accumulator.fill(count_type(0));
    }

    //*********************************
    /// Size of the histogram.
    //*********************************
    ETL_CONSTEXPR size_t size() const
    {
      return Max_Keys;
    }

    //*********************************
    /// Max size of the histogram.
    //*********************************
    ETL_CONSTEXPR size_t max_size() const
    {
      return Max_Keys;
    }

    //*********************************
    /// Count of items in the histogram.
    //*********************************
    size_t count() const
    {
      return etl::accumulate(accumulator.begin(), accumulator.end(), size_t(0));
    }

  private:

    key_type start_index;
    etl::array<count_type, Max_Size> accumulator;
  };

  //***************************************************************************
  /// Histogram for sparce keys.
  //***************************************************************************
  template<typename TKey, typename TCount, size_t Max_Keys>
  class sparce_histogram : public etl::unary_function<TKey, void>
  {
  private:

    typedef etl::flat_map<TKey, TCount, Max_Keys> accumulator_type;

  public:

    static ETL_CONSTANT size_t Max_Size = Max_Keys;

    typedef TKey   key_type;
    typedef TCount count_type;
    typedef typename accumulator_type::value_type     value_type;
    typedef typename accumulator_type::const_iterator const_iterator;

  public:

    //*********************************
    /// Constructor
    //*********************************
    sparce_histogram()
    {
    }

    //*********************************
    /// Constructor
    //*********************************
    template <typename TIterator>
    sparce_histogram(TIterator first, TIterator last)
    {
      add(first, last);
    }

    //*********************************
    /// Copy constructor
    //*********************************
    sparce_histogram(const sparce_histogram& other)
    {
      accumulator = other.accumulator;
    }

#if ETL_CPP11_SUPPORTED
    //*********************************
    /// Move constructor
    //*********************************
    sparce_histogram(sparce_histogram&& other)
    {
      accumulator = etl::move(other.accumulator);
    }
#endif

    //*********************************
    /// Copy assignment
    //*********************************
    sparce_histogram& operator =(const sparce_histogram& rhs)
    {
      accumulator = rhs.accumulator;
      
      return *this;
    }

#if ETL_CPP11_SUPPORTED
    //*********************************
    /// Move assignment
    //*********************************
    sparce_histogram& operator =(sparce_histogram&& rhs)
    {
      accumulator = etl::move(rhs.accumulator);

      return *this;
    }
#endif

    //*********************************
    /// Beginning of the histogram.
    //*********************************
    const_iterator begin() const
    {
      return accumulator.begin();
    }

    //*********************************
    /// Beginning of the histogram.
    //*********************************
    const_iterator cbegin() const
    {
      return accumulator.cbegin();
    }

    //*********************************
    /// End of the histogram.
    //*********************************
    const_iterator end() const
    {
      return accumulator.begin();
    }

    //*********************************
    /// End of the histogram.
    //*********************************
    const_iterator cend() const
    {
      return accumulator.cbegin();
    }

    //*********************************
    /// Add
    //*********************************
    void add(const key_type& key)
    {
      ++accumulator[key];
    }

    //*********************************
    /// Add
    //*********************************
    template <typename TIterator>
    void add(TIterator first, TIterator last)
    {
      while (first != last)
      {
        add(*first++);
      }
    }

    //*********************************
    /// operator ()
    //*********************************
    void operator ()(const key_type& key)
    {
      add(key);
    }

    //*********************************
    /// operator ()
    //*********************************
    template <typename TIterator>
    void operator ()(TIterator first, TIterator last)
    {
      add(first, last);
    }

    //*********************************
    /// operator []
    //*********************************
    const value_type& operator [](const key_type& key) const
    {
      static const value_type unused(key_type(), count_type(0));

      accumulator_type::const_iterator itr = accumulator.find(key);

      if (itr != accumulator.end())
      {
        return *itr;
      }
      else
      {
        return unused;
      }
    }

    //*********************************
    /// Clear the histogram.
    //*********************************
    void clear()
    {
      accumulator.clear();
    }

    //*********************************
    /// Size of the histogram.
    //*********************************
    size_t size() const
    {
      return accumulator.size();
    }

    //*********************************
    /// Max size of the histogram.
    //*********************************
    ETL_CONSTEXPR size_t max_size() const
    {
      return Max_Keys;
    }

    //*********************************
    /// Count of items in the histogram.
    //*********************************
    size_t count() const
    {
      count_type sum = count_type(0);

      const_iterator itr = accumulator.begin();

      while (itr != accumulator.end())
      {
        sum += (*itr++).second;
      }

      return sum;
    }

  private:

    etl::flat_map<key_type, count_type, Max_Size> accumulator;
  };
}
