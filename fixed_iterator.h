///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.

Copyright(c) 2015 jwellbelove

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

#ifndef __ETL_FIXED_ITERATOR__
#define __ETL_FIXED_ITERATOR__

#include <iterator>

namespace etl
{
  template <typename TIterator>
  class fixed_iterator : public std::iterator<std::random_access_iterator_tag, TIterator>
  {
  public:

    //***************************************************************************
    fixed_iterator()
      : it(TIterator())
    {}

    //***************************************************************************
    fixed_iterator(TIterator it)
      : it(it)
    {
    }
    
    //***************************************************************************
    fixed_iterator& operator ++()
    {
      return *this;
    }
    
    //***************************************************************************
    fixed_iterator operator ++(int)
    {
      return *this;
    }

    //***************************************************************************
    fixed_iterator& operator --()
    {
      return *this;
    }
    
    //***************************************************************************
    fixed_iterator operator --(int)
    {
      return *this;
    }  
    
    //***************************************************************************
    typename std::iterator_traits<TIterator>::value_type operator *()
    {
      return *it;
    }
    
    //***************************************************************************
    const typename std::iterator_traits<TIterator>::value_type operator *() const
    {
      return *it;
    }

    //***************************************************************************
    TIterator operator ->()
    {
      return it;
    }  

    //***************************************************************************
    const TIterator operator ->() const
    {
      return it;
    }

    //***************************************************************************
    operator TIterator() const
    {
      return it;
    }

    //***************************************************************************
    fixed_iterator& operator +=(typename std::iterator_traits<TIterator>::difference_type offset)
    {
      return *this;
    } 
    
    //***************************************************************************
    fixed_iterator& operator -=(typename std::iterator_traits<TIterator>::difference_type offset)
    {
      return *this;
    }   

    //***************************************************************************
    fixed_iterator& operator =(TIterator new_it)
    {
      it = new_it;
      return *this;
    }

    //***************************************************************************
    void set(TIterator new_it)
    {
      it = new_it;
    }
    
    //***************************************************************************
    const TIterator& get() const
    {
      return it;
    }
    
  private:

    TIterator it; ///< The underlying iterator.
  };

  //*****************************************************************************
  template <typename TIterator>
  fixed_iterator<TIterator> make_fixed_iterator(TIterator it)
  {
    return fixed_iterator<TIterator>(it);
  }  
}

//*****************************************************************************
template <typename TIterator>
etl::fixed_iterator<TIterator>& operator +(etl::fixed_iterator<TIterator>& lhs,
                                           typename std::iterator_traits<TIterator>::difference_type rhs)
{
  return lhs;
}

//*****************************************************************************
template <typename TIterator>
etl::fixed_iterator<TIterator>& operator -(etl::fixed_iterator<TIterator>& lhs,
                                           typename std::iterator_traits<TIterator>::difference_type rhs)
{
  return lhs;
}

//*****************************************************************************
template <typename TIterator>
typename std::iterator_traits<TIterator>::difference_type operator -(etl::fixed_iterator<TIterator>& lhs,
                                                                     etl::fixed_iterator<TIterator>& rhs)
{
  return lhs.get() - rhs.get();
}

//*****************************************************************************
template <typename TIterator>
bool operator ==(const etl::fixed_iterator<TIterator>& lhs,
                 const etl::fixed_iterator<TIterator>& rhs)
{
  return lhs.get() == rhs.get();
}

//*****************************************************************************
template <typename TIterator>
bool operator ==(const etl::fixed_iterator<TIterator>& lhs,
                 TIterator rhs)
{
  return lhs.get() == rhs;
}

//*****************************************************************************
template <typename TIterator>
bool operator ==(TIterator lhs,
                 const etl::fixed_iterator<TIterator>& rhs)
{
  return lhs == rhs.get();
}


//*****************************************************************************
template <typename TIterator>
bool operator !=(const etl::fixed_iterator<TIterator>& lhs,
                 const etl::fixed_iterator<TIterator>& rhs)
{
  return !(lhs == rhs);
}

//*****************************************************************************
template <typename TIterator>
bool operator !=(const etl::fixed_iterator<TIterator>& lhs,
                 TIterator rhs)
{
  return !(lhs == rhs);
}

//*****************************************************************************
template <typename TIterator>
bool operator !=(TIterator& lhs,
                 const etl::fixed_iterator<TIterator>& rhs)
{
  return !(lhs == rhs);
}

#endif
