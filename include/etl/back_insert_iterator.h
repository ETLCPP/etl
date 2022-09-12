//
// Created by Eric Vantillard on 14/09/2022.
//

#ifndef ETL_BACK_INSERT_ITERATOR_H
#define ETL_BACK_INSERT_ITERATOR_H

#include "iterator.h"

namespace etl
{

  template <class TContainer>
  class back_insert_iterator : public iterator<output_iterator_tag, void, void, void, void>
  {
  public:
    typedef TContainer container_type;

    explicit back_insert_iterator(TContainer& c)
      : container_(addressof(c))
    {
    }

    back_insert_iterator& operator=(const typename TContainer::value_type& value)
    {
      container_->push_back(value);
      return (*this);
    }

    back_insert_iterator& operator=(typename TContainer::value_type&& value)
    {
      container_->push_back(move(value));
      return (*this);
    }

    back_insert_iterator& operator*()
    {
      return (*this);
    }

    back_insert_iterator& operator++()
    {
      return (*this);
    }

    back_insert_iterator operator++(int)
    {
      return (*this);
    }

  protected:
    TContainer* container_;
  };

#if ETL_USING_STL

  template <class TContainer>
  inline std::back_insert_iterator<TContainer> back_inserter(TContainer& container)
  {
    return std::back_insert_iterator<TContainer>(container);
  }

#else

  template <class TContainer>
  inline back_insert_iterator<TContainer> back_inserter(TContainer& container)
  {
    return back_insert_iterator<TContainer>(container);
  }

#endif  // ETL_USING_STL

}  // namespace etl

#endif  // ETL_BACK_INSERT_ITERATOR_H
