///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 John Wellbelove

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

#ifndef ETL_MULTI_SPAN_INCLUDED
#define ETL_MULTI_SPAN_INCLUDED

#include "platform.h"
#include "iterator.h"
#include "algorithm.h"
#include "vector.h"
#include "span.h"

///\defgroup multi_multi_span multi_span multi_span
/// Allows Scatter/Gather functionality
///\ingroup containers

namespace etl
{
  template <typename T>
  class multi_span
  {
  public:
    
    typedef T                                 element_type;
    typedef typename etl::remove_cv<T>::type  value_type;
    typedef size_t                            size_type;
    typedef T&                                reference;
    typedef const T&                          const_reference;
    typedef T*                                pointer;
    typedef const T*                          const_pointer;

    typedef etl::span<T>                      span_type;
    typedef etl::span<const span_type>        span_list_type;

    //*************************************************************************
    /// Iterator
    //*************************************************************************
    class iterator : public etl::iterator<ETL_OR_STD::bidirectional_iterator_tag, element_type>
    {
    public:

      friend class multi_span;
      friend class const_iterator;

      //*****************************************
      ETL_CONSTEXPR14 iterator()
        : p_span_list(ETL_NULLPTR)
        , p_current_span(ETL_NULLPTR)
        , p_value(ETL_NULLPTR)
      {
      }

      //*****************************************
      ETL_CONSTEXPR14 iterator(const iterator& other)
        : p_span_list(other.p_span_list)
        , p_current_span(other.p_current_span)
        , p_value(other.p_value)
      {
      }

      //*****************************************
      ETL_CONSTEXPR14 iterator& operator =(const iterator& rhs)
      {
        p_span_list    = rhs.p_span_list;
        p_current_span = rhs.p_current_span;
        p_value        = rhs.p_value;

        return *this;
      }

      //*****************************************
      ETL_CONSTEXPR14 iterator& operator ++()
      {
        if (p_current_span != p_span_list->end())
        {
          ++p_value;

          if (p_value == p_current_span->end())
          {
            do
            {
              ++p_current_span;
            } while ((p_current_span != p_span_list->end()) && p_current_span->empty());

            if (p_current_span != p_span_list->end())
            {
              p_value = p_current_span->begin();
            }
            else
            {
              p_value = ETL_NULLPTR;
            }
          }
        }

        return *this;
      }

      //*****************************************
      ETL_CONSTEXPR14 iterator operator ++(int)
      {
        iterator temp = *this;

        operator ++();

        return temp;
      }

      //*****************************************
      ETL_CONSTEXPR14 iterator& operator --()
      {
        if (p_current_span == p_span_list->end())
        {
          --p_current_span;
          p_value = p_current_span->end();
          --p_value;
        }
        else if ((p_current_span != p_span_list->begin()) || (p_value != p_current_span->begin()))
        {
          if (p_value == p_current_span->begin())
          {
            do
            {
              --p_current_span;
            } while ((p_current_span != p_span_list->begin()) && p_current_span->empty());

            p_value = p_current_span->end();
            --p_value;
          }
          else
          {
            --p_value;
          }
        }
        else
        {
          p_value = ETL_NULLPTR;
        }

        return *this;
      }

      //*****************************************
      ETL_CONSTEXPR14 iterator operator --(int)
      {
        iterator temp = *this;

        operator --();

        return temp;
      }

      //*************************************************************************
      /// * operator
      //*************************************************************************
      ETL_CONSTEXPR14 reference operator *()
      {
        return *p_value;
      }

      //*************************************************************************
      /// * operator
      //*************************************************************************
      ETL_CONSTEXPR14 const_reference operator *() const
      {
        return *p_value;
      }

      //*************************************************************************
      /// -> operator
      //*************************************************************************
      ETL_CONSTEXPR14 pointer operator ->()
      {
        return p_value;
      }

      //*************************************************************************
      /// -> operator
      //*************************************************************************
      ETL_CONSTEXPR14 const_pointer operator ->() const
      {
        return p_value;
      }

      //*************************************************************************
      /// == operator
      //*************************************************************************
      ETL_CONSTEXPR14 friend bool operator ==(const iterator& lhs, const iterator& rhs)
      {
        return (lhs.p_current_span == rhs.p_current_span) && (lhs.p_value == rhs.p_value);
      }

      //*************************************************************************
      /// != operator
      //*************************************************************************
      ETL_CONSTEXPR14 friend bool operator !=(const iterator& lhs, const iterator& rhs)
      {
        return !(lhs == rhs);
      }

    private:

      typedef const span_type*                  span_pointer;
      typedef const span_list_type*             span_list_pointer;
      typedef typename span_list_type::iterator span_list_iterator;

      //*****************************************
      ETL_CONSTEXPR14 iterator(const span_list_type& span_list_, span_list_iterator p_current_span_)
        : p_span_list(&span_list_)
        , p_current_span(p_current_span_)
        , p_value(ETL_NULLPTR)
      {
        if (p_current_span != p_span_list->end())
        {
          while ((p_current_span != p_span_list->end()) && p_current_span->empty())
          {
            ++p_current_span;
          }
          
          if (p_current_span != p_span_list->end())
          {
            p_value = p_current_span->begin();
          }
          else
          {
            p_value = ETL_NULLPTR;
          }
        }
      }

      span_list_pointer p_span_list;
      span_pointer      p_current_span;
      pointer           p_value;
    };

    //*************************************************************************
    /// Const Iterator
    //*************************************************************************
    class const_iterator : public etl::iterator<ETL_OR_STD::bidirectional_iterator_tag, const element_type>
    {
    public:

      friend class multi_span;

      //*****************************************
      ETL_CONSTEXPR14 const_iterator()
        : p_span_list(ETL_NULLPTR)
        , p_current_span(ETL_NULLPTR)
        , p_value(ETL_NULLPTR)
      {
      }

      //*****************************************
      ETL_CONSTEXPR14 const_iterator(const const_iterator& other)
        : p_span_list(other.p_span_list)
        , p_current_span(other.p_current_span)
        , p_value(other.p_value)
      {
      }

      //*****************************************
      ETL_CONSTEXPR14 const_iterator& operator =(const const_iterator& rhs)
      {
        p_span_list    = rhs.p_span_list;
        p_current_span = rhs.p_current_span;
        p_value        = rhs.p_value;

        return *this;
      }

      //*****************************************
      ETL_CONSTEXPR14 const_iterator(const etl::multi_span<T>::iterator& other)
        : p_span_list(other.p_span_list)
        , p_current_span(other.p_current_span)
        , p_value(other.p_value)
      {
      }

      //*****************************************
      ETL_CONSTEXPR14 const_iterator& operator =(const etl::multi_span<T>::iterator& rhs)
      {
        p_span_list    = rhs.p_span_list;
        p_current_span = rhs.p_current_span;
        p_value        = rhs.p_value;

        return *this;
      }

      //*****************************************
      ETL_CONSTEXPR14 const_iterator& operator ++()
      {
        if (p_current_span != p_span_list->end())
        {
          ++p_value;

          if (p_value == p_current_span->end())
          {
            do
            {
              ++p_current_span;
            } while ((p_current_span != p_span_list->end()) && p_current_span->empty());

            if (p_current_span != p_span_list->end())
            {
              p_value = p_current_span->begin();
            }
            else
            {
              p_value = ETL_NULLPTR;
            }
          }
        }

        return *this;
      }

      //*****************************************
      ETL_CONSTEXPR14 const_iterator operator ++(int)
      {
        const_iterator temp = *this;

        operator ++();

        return temp;
      }

      //*****************************************
      ETL_CONSTEXPR14 const_iterator& operator --()
      {
        if (p_current_span == p_span_list->end())
        {
          --p_current_span;
          p_value = p_current_span->end();
          --p_value;
        }
        else if ((p_current_span != p_span_list->begin()) || (p_value != p_current_span->begin()))
        {
          if (p_value == p_current_span->begin())
          {
            do
            {
              --p_current_span;
            } while ((p_current_span != p_span_list->begin()) && p_current_span->empty());

            p_value = p_current_span->end();
            --p_value;
          }
          else
          {
            --p_value;
          }
        }
        else
        {
          p_value = ETL_NULLPTR;
        }

        return *this;
      }

      //*****************************************
      ETL_CONSTEXPR14 const_iterator operator --(int)
      {
        const_iterator temp = *this;

        operator --();

        return temp;
      }

      //*************************************************************************
      /// * operator
      //*************************************************************************
      ETL_CONSTEXPR14 const_reference operator *() const
      {
        return *p_value;
      }

      //*************************************************************************
      /// -> operator
      //*************************************************************************
      ETL_CONSTEXPR14 const_pointer operator ->() const
      {
        return p_value;
      }

      //*************************************************************************
      /// == operator
      //*************************************************************************
      ETL_CONSTEXPR14 friend bool operator ==(const const_iterator& lhs, const const_iterator& rhs)
      {
        return (lhs.p_current_span == rhs.p_current_span) && (lhs.p_value == rhs.p_value);
      }

      //*************************************************************************
      /// != operator
      //*************************************************************************
      ETL_CONSTEXPR14 friend bool operator !=(const const_iterator& lhs, const const_iterator& rhs)
      {
        return !(lhs == rhs);
      }

    private:

      typedef const span_type*                        span_pointer;
      typedef const span_list_type*                   span_list_pointer;
      typedef const typename span_list_type::iterator span_list_iterator;

      //*****************************************
      ETL_CONSTEXPR14 const_iterator(const span_list_type& span_list_, span_list_iterator p_current_span_)
        : p_span_list(&span_list_)
        , p_current_span(p_current_span_)
        , p_value(ETL_NULLPTR)
      {
        if (p_current_span != p_span_list->end())
        {
          while ((p_current_span != p_span_list->end()) && p_current_span->empty())
          {
            ++p_current_span;
          }

          if (p_current_span != p_span_list->end())
          {
            p_value = p_current_span->begin();
          }
          else
          {
            p_value = ETL_NULLPTR;
          }
        }
      }

      span_list_pointer p_span_list;
      span_pointer      p_current_span;
      const_pointer     p_value;
    };

    typedef ETL_OR_STD::reverse_iterator<iterator>       reverse_iterator;
    typedef ETL_OR_STD::reverse_iterator<const_iterator> const_reverse_iterator;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    ETL_CONSTEXPR14 multi_span(span_list_type span_list_)
      : span_list(span_list_)
    {
    }

    //*************************************************************************
    /// Construct from a container or other type that supports
    /// data() and size() member functions.
    //*************************************************************************
    template <typename TContainer>
    ETL_CONSTEXPR14 multi_span(TContainer& a) ETL_NOEXCEPT
      : span_list(a.data(), a.data() + a.size())
    {
    }

    //*************************************************************************
    /// Construct from a container or other type that supports
    /// data() and size() member functions.
    //*************************************************************************
    template <typename TContainer>
    ETL_CONSTEXPR14 multi_span(const TContainer& a) ETL_NOEXCEPT
      : span_list(a.data(), a.data() + a.size())
    {
    }

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    template <typename TIterator>
    ETL_CONSTEXPR14 multi_span(TIterator begin_, TIterator end_)
      : span_list(etl::to_address(begin_), etl::distance(begin_, end_))
    {
    }

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    template <typename TIterator>
    ETL_CONSTEXPR14 multi_span(TIterator begin_, size_t length_)
      : span_list(etl::to_address(begin_), length_)
    {
    }

    //*************************************************************************
    /// Copy Constructor.
    //*************************************************************************
    ETL_CONSTEXPR14 multi_span(const multi_span& other)
      : span_list(other.span_list)
    {
    }

    //*************************************************************************
    /// Assignment operator
    //*************************************************************************
    ETL_CONSTEXPR14 multi_span& operator =(const multi_span & other)
    {
      span_list = other.span_list;

      return *this;
    }

    //*************************************************************************
    /// 
    //*************************************************************************
    ETL_CONSTEXPR14 iterator begin() const
    {
      return iterator(span_list, span_list.begin());
    }

    //*************************************************************************
    /// 
    //*************************************************************************
    ETL_CONSTEXPR14 const_iterator cbegin() const
    {
      return const_iterator(span_list, span_list.cbegin());
    }

    //*************************************************************************
    /// 
    //*************************************************************************
    ETL_CONSTEXPR14 iterator end() const
    {
      return iterator(span_list, span_list.end());
    }

    //*************************************************************************
    /// 
    //*************************************************************************
    ETL_CONSTEXPR14 const_iterator cend() const
    {
      return const_iterator(span_list, span_list.cend());
    }

    //*************************************************************************
    /// 
    //*************************************************************************
    ETL_CONSTEXPR14 reverse_iterator rbegin() const
    {
      return reverse_iterator(end());
    }

    //*************************************************************************
    /// 
    //*************************************************************************
    ETL_CONSTEXPR14 reverse_iterator crbegin() const
    {
      return const_reverse_iterator(cend());
    }

    //*************************************************************************
    /// 
    //*************************************************************************
    ETL_CONSTEXPR14 reverse_iterator rend() const
    {
      return reverse_iterator(begin());
    }

    //*************************************************************************
    /// 
    //*************************************************************************
    ETL_CONSTEXPR14 const_reverse_iterator crend() const
    {
      return const_reverse_iterator(cbegin());
    }

    //*************************************************************************
    /// Returns a reference to the indexed value.
    //*************************************************************************
    ETL_CONSTEXPR14 reference operator[](size_t i) const
    {      
      // Find the span in the span list.
      size_t number_of_spans = span_list.size();

      size_t index = 0;

      while ((i >= span_list[index].size()) && (index < number_of_spans))
      {
        i -= span_list[index].size();
        ++index;
      }

      return span_list[index][i];
    }

    //*************************************************************************
    /// Returns the number of elements in the multi_span.
    //*************************************************************************
    ETL_CONSTEXPR14 size_t size() const ETL_NOEXCEPT
    {
      size_t total_n_spans = 0U;

      for (typename span_list_type::iterator itr = span_list.begin();
           itr != span_list.end(); 
           ++itr)
      {
        total_n_spans += itr->size();
      }

      return total_n_spans;
    }

    //*************************************************************************
    /// Returns <b>true</b> if the multi_span size is zero.
    //*************************************************************************
    ETL_CONSTEXPR14 bool empty() const ETL_NOEXCEPT
    {
      if (span_list.empty())
      {
        return true;
      }
      else
      {
        return size() == 0U;
      }
    }

    //*************************************************************************
    /// Returns the size of the multi_span.
    //*************************************************************************
    ETL_CONSTEXPR14 size_t size_bytes() const ETL_NOEXCEPT
    {
      size_t total_n_spans_bytes = 0U;

      for (typename span_list_type::iterator itr = span_list.begin(); 
           itr != span_list.end(); 
           ++itr)
      {
        total_n_spans_bytes += itr->size_bytes();
      }

      return total_n_spans_bytes;
    }

    //*************************************************************************
    /// Returns the number of spans in the multi_span.
    //*************************************************************************
    ETL_CONSTEXPR14 size_t size_spans() const ETL_NOEXCEPT
    {
      return span_list.size();
    }

  private:

    span_list_type span_list;
  };
}

#endif

