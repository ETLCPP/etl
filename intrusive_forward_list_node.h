///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl

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

#ifndef __ETL_INTRUSIVE_FORWARD_LIST_NODE__
#define __ETL_INTRUSIVE_FORWARD_LIST_NODE__

#include "error_handler.h"
#include "array.h"

namespace etl
{
  namespace __private_intrusive_forward_list__
  {
    //***************************************************************************
    /// The node element in the intrusive_forward_list.
    //***************************************************************************
    class intrusive_forward_list_node_base
    {
    public:

      virtual intrusive_forward_list_node_base* get_next(size_t index) const
      {
        return base_next;
      }

      virtual void set_next(size_t index, intrusive_forward_list_node_base* pnext)
      {
        base_next = pnext;
      }

    private:

      intrusive_forward_list_node_base* base_next;
    };
  }

  //***************************************************************************
  /// The node element in the intrusive_forward_list.
  //***************************************************************************
  template <const size_t SIZE>
  struct intrusive_forward_list_node : public __private_intrusive_forward_list__::intrusive_forward_list_node_base
  {
  public:

    __private_intrusive_forward_list__::intrusive_forward_list_node_base* get_next(size_t index) const
    {
#ifdef _DEBUG
      if (index >= SIZE)
      {
        ETL_ERROR(intrusive_forward_list_index_exception());
      }
#endif
      return next[index];
    }

    void set_next(size_t index, __private_intrusive_forward_list__::intrusive_forward_list_node_base* pnext)
    {
#ifdef _DEBUG
      if (index >= SIZE)
      {
        ETL_ERROR(intrusive_forward_list_index_exception());
      }
#endif
      next[index] = pnext;
    }

  private:

    etl::array<__private_intrusive_forward_list__::intrusive_forward_list_node_base*, SIZE> next;
  };
}

#endif
