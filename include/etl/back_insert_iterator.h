//
// Created by Eric Vantillard on 14/09/2022.
//

#ifndef ETL_BACK_INSERT_ITERATOR_H
#define ETL_BACK_INSERT_ITERATOR_H

#include "iterator.h"

namespace etl
{

  /**
   * @brief  Turns assignment into insertion.
   *
   * These are output iterators, constructed from a container-of-T.
   * Assigning a T to the iterator appends it to the container using push_back.
   *
   * @tparam TContainer
   */
  template <class TContainer>
  class back_insert_iterator : public iterator<output_iterator_tag, void, void, void, void>
  {
  public:
    /// A nested typedef for the type of whatever container you used.
    typedef TContainer container_type;

    /// The only way to create this %iterator is with a container.
    explicit back_insert_iterator(TContainer& c)
      : container_(addressof(c))
    {
    }

    /**
     * This kind of %iterator doesn't really have a @a position in the
     * container (you can think of the position as being permanently at
     * the end, if you like).  Assigning a value to the %iterator will
     * always append the value to the end of the container.
     *
     * @param value An instance of whatever type container_type::const_reference is;
     *              presumably a reference-to-const T for container<T>.
     * @return This %iterator, for chained operations.
     */
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

    /// Simply returns *this.
    back_insert_iterator& operator*()
    {
      return (*this);
    }

    /// Simply returns *this.  (This %iterator does not @a move.)
    back_insert_iterator& operator++()
    {
      return (*this);
    }

    /// Simply returns *this.  (This %iterator does not @a move.)
    back_insert_iterator operator++(int)
    {
      return (*this);
    }

  protected:
    TContainer* container_;
  };

  /**
   * This wrapper function helps in creating back_insert_iterator instances.
   * Typing the name of the %iterator requires knowing the precise full
   * type of the container, which can be tedious and impedes generic
   * programming.  Using this function lets you take advantage of automatic
   * template parameter deduction, making the compiler match the correct types for you.
   *
   * @tparam TContainer The container type.
   * @param container A container of arbitrary type.
   * @return An instance of back_insert_iterator working on @p container.
   */
  template <class TContainer>
  inline ETL_OR_STD::back_insert_iterator<TContainer> back_inserter(TContainer& container)
  {
    return ETL_OR_STD::back_insert_iterator<TContainer>(container);
  }

}  // namespace etl

#endif  // ETL_BACK_INSERT_ITERATOR_H
