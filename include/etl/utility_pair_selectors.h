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

#ifndef ETL_UTILITY_PAIR_SELECTORS_H
#define ETL_UTILITY_PAIR_SELECTORS_H

namespace etl
{

  /**
   * @brief Functor to select @ref pair::first
   *
   * @ref select1st is a functor object that takes a single argument, a @ref pair, and returns the @ref pair::first element.
   *
   * @b Example
   * @snippet test_utility.cpp test_select1st_example
   *
   * @tparam TPair The function object's argument type.
   *
   * @see select2nd
   */
  template <typename TPair>
  struct select1st {

    typedef typename TPair::first_type type;  ///< type of member @ref pair::first.

    /**
     * @brief Function call that return @c p.first.
     * @return a reference to member @ref pair::first of the @c pair `p`
     */
    type& operator()(TPair& p) const
    {
      return p.first;
    }


    /**
     * @copydoc operator()(TPair&)const
     */
    const type& operator()(const TPair& p) const
    {
      return p.first;
    }
  };

  /**
   * @brief Functor to select @ref pair::second
   *
   * @ref select2nd is a functor object that takes a single argument, a @ref pair, and returns the @ref pair::second element.
   *
   * @b Example
   * @snippet test_utility.cpp test_select2nd_example
   *
   * @tparam TPair The function object's argument type.
   *
   * @see select1st
   */
  template <typename TPair>
  struct select2nd
  {
    typedef typename TPair::second_type type;  ///< type of member @ref pair::second.

    /**
     * @brief Function call. The return value is `p.second`.
     * @return a reference to member `second` of the pair `p`.
     */
    type& operator()(TPair& p) const
    {
      return p.second;
    }

    /**
     * @copydoc operator()(TPair&)const
     */
    const type& operator()(const TPair& p) const
    {
      return p.second;
    }
  };

}  // namespace etl

#endif  // ETL_UTILITY_PAIR_SELECTORS_H
