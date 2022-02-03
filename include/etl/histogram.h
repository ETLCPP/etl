#pragma once

#include "platform.h"
#include "functional.h"
#include "algorithm.h"
#include "array.h"
#include "flat_map.h"
#include "static_assert.h"
#include "type_traits.h"
#include "integral_limits.h"

namespace etl
{
  namespace private_histogram
  {
    //***************************************************************************
    /// Base for histograms.
    //***************************************************************************
    template <typename TCount, size_t Max_Size_>
    class histogram_common
    {
    public:

      ETL_STATIC_ASSERT(etl::is_integral<TCount>::value, "Only integral count allowed"); 

      static ETL_CONSTANT size_t Max_Size = Max_Size_;

      typedef typename etl::array<TCount, Max_Size>::const_iterator const_iterator;

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
        return accumulator.end();
      }

      //*********************************
      /// End of the histogram.
      //*********************************
      const_iterator cend() const
      {
        return accumulator.cend();
      }

      //*********************************
      /// Clear the histogram.
      //*********************************
      void clear()
      {
        accumulator.fill(TCount(0));
      }

      //*********************************
      /// Size of the histogram.
      //*********************************
      ETL_CONSTEXPR size_t size() const
      {
        return Max_Size;
      }

      //*********************************
      /// Max size of the histogram.
      //*********************************
      ETL_CONSTEXPR size_t max_size() const
      {
        return Max_Size;
      }

      //*********************************
      /// Count of items in the histogram.
      //*********************************
      size_t count() const
      {
        return etl::accumulate(accumulator.begin(), accumulator.end(), size_t(0));
      }

    protected:

      etl::array<TCount, Max_Size> accumulator;
    };
  }

  //***************************************************************************
  /// Histogram with a compile time start index.
  //***************************************************************************
  template <typename TKey, typename TCount, size_t Max_Size, int32_t Start_Index = etl::integral_limits<int32_t>::max>
  class histogram 
    : public etl::private_histogram::histogram_common<TCount, Max_Size>
    , public etl::unary_function<TKey, void>
  {
  public:

    ETL_STATIC_ASSERT(etl::is_integral<TKey>::value, "Only integral keys allowed");
    ETL_STATIC_ASSERT(etl::is_integral<TCount>::value, "Only integral count allowed");   

    typedef TKey   key_type;
    typedef TCount count_type;
    typedef TCount value_type;

    //*********************************
    /// Constructor
    //*********************************
    histogram()
    {
      this->accumulator.fill(count_type(0));
    }

    //*********************************
    /// Constructor
    //*********************************
    template <typename TIterator>
    histogram(TIterator first, TIterator last)
    {
      this->accumulator.fill(count_type(0));
      add(first, last);
    }

    //*********************************
    /// Copy constructor
    //*********************************
    histogram(const histogram& other)
    {
      this->accumulator = other.accumulator;
    }

#if ETL_CPP11_SUPPORTED
    //*********************************
    /// Move constructor
    //*********************************
    histogram(histogram&& other)
    {
      this->accumulator = etl::move(other.accumulator);
    }
#endif

    //*********************************
    /// Copy assignment
    //*********************************
    histogram& operator =(const histogram& rhs)
    {
      this->accumulator = rhs.accumulator;

      return *this;
    }

#if ETL_CPP11_SUPPORTED
    //*********************************
    /// Move assignment
    //*********************************
    histogram& operator =(histogram&& rhs)
    {
      this->accumulator = etl::move(rhs.accumulator);

      return *this;
    }
#endif

    //*********************************
    /// Add
    //*********************************
    void add(key_type key)
    {
      ++this->accumulator[key - Start_Index];
    }

    //*********************************
    /// Add
    //*********************************
    template <typename TIterator>
    void add(TIterator first, TIterator last)
    {
      while (first != last)
      {
        add(*first);
        ++first;
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
      return this->accumulator[key];
    }
  };

  //***************************************************************************
  /// Histogram with a run time start index.
  //***************************************************************************
  template<typename TKey, typename TCount, size_t Max_Size>
  class histogram<TKey, TCount, Max_Size, etl::integral_limits<int32_t>::max>
    : public etl::private_histogram::histogram_common<TCount, Max_Size>
    , public etl::unary_function<TKey, void>
  {
  public:

    ETL_STATIC_ASSERT(etl::is_integral<TKey>::value, "Only integral keys allowed");
    ETL_STATIC_ASSERT(etl::is_integral<TCount>::value, "Only integral count allowed");

    typedef TKey   key_type;
    typedef TCount count_type;
    typedef TCount value_type;

    //*********************************
    /// Constructor
    //*********************************
    explicit histogram(key_type start_index_)
      : start_index(start_index_)
    {
      this->accumulator.fill(count_type(0));
    }

    //*********************************
    /// Constructor
    //*********************************
    template <typename TIterator>
    histogram(key_type start_index_, TIterator first, TIterator last)
      : start_index(start_index_)
    {
      this->accumulator.fill(count_type(0));
      add(first, last);
    }

    //*********************************
    /// Copy constructor
    //*********************************
    histogram(const histogram& other)
    {
      this->accumulator = other.accumulator;
    }

#if ETL_CPP11_SUPPORTED
    //*********************************
    /// Move constructor
    //*********************************
    histogram(histogram&& other)
    {
      this->accumulator = etl::move(other.accumulator);
    }
#endif

    //*********************************
    /// Copy assignment
    //*********************************
    histogram& operator =(const histogram& rhs)
    {
      this->accumulator = rhs.accumulator;

      return *this;
    }

#if ETL_CPP11_SUPPORTED
    //*********************************
    /// Move assignment
    //*********************************
    histogram& operator =(histogram&& rhs)
    {
      this->accumulator = etl::move(rhs.accumulator);

      return *this;
    }
#endif

    //*********************************
    /// Add
    //*********************************
    void add(key_type key)
    {
      ++this->accumulator[key - start_index];
    }

    //*********************************
    /// Add
    //*********************************
    template <typename TIterator>
    void add(TIterator first, TIterator last)
    {
      while (first != last)
      {
        add(*first);
        ++first;
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
      return this->accumulator[key];
    }

  private:

    key_type start_index;
  };

  //***************************************************************************
  /// Histogram for sparse keys.
  //***************************************************************************
  template<typename TKey, typename TCount, size_t Max_Size_>
  class sparse_histogram : public etl::unary_function<TKey, void>
  {
  private:

    typedef etl::flat_map<TKey, TCount, Max_Size_> accumulator_type;

  public:

    ETL_STATIC_ASSERT(etl::is_integral<TCount>::value, "Only integral count allowed");

    static ETL_CONSTANT size_t Max_Size = Max_Size_;

    typedef TKey   key_type;
    typedef TCount count_type;
    typedef typename accumulator_type::value_type     value_type;
    typedef typename accumulator_type::const_iterator const_iterator;

  public:

    //*********************************
    /// Constructor
    //*********************************
    sparse_histogram()
    {
    }

    //*********************************
    /// Constructor
    //*********************************
    template <typename TIterator>
    sparse_histogram(TIterator first, TIterator last)
    {
      add(first, last);
    }

    //*********************************
    /// Copy constructor
    //*********************************
    sparse_histogram(const sparse_histogram& other)
    {
      this->accumulator = other.accumulator;
    }

#if ETL_CPP11_SUPPORTED
    //*********************************
    /// Move constructor
    //*********************************
    sparse_histogram(sparse_histogram&& other)
    {
      accumulator = etl::move(other.accumulator);
    }
#endif

    //*********************************
    /// Copy assignment
    //*********************************
    sparse_histogram& operator =(const sparse_histogram& rhs)
    {
      accumulator = rhs.accumulator;
      
      return *this;
    }

#if ETL_CPP11_SUPPORTED
    //*********************************
    /// Move assignment
    //*********************************
    sparse_histogram& operator =(sparse_histogram&& rhs)
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
        add(*first);
        ++first;
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

      typename accumulator_type::const_iterator itr = accumulator.find(key);

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
      return Max_Size;
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
        sum += (*itr).second;
        ++itr;
      }

      return sum;
    }

  private:

    etl::flat_map<key_type, count_type, Max_Size> accumulator;
  };
}
