/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2026 Sergei Shirokov

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

#include "unit_test_framework.h"

#include "data.h"

#include "etl/compare.h"
#include "etl/intrusive_avl_tree.h"
#include "etl/optional.h"

#include <array>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

typedef TestDataM<int>   ItemM;
typedef TestDataNDC<int> ItemNDC;

namespace
{
  typedef etl::intrusive_avl_tree_base<0>::link_type ZeroLink;
  typedef etl::intrusive_avl_tree_base<1>::link_type FirstLink;
  typedef etl::intrusive_avl_tree_base<2>::link_type SecondLink;

  //***************************************************************************
  class ItemNDCNode
    : public ZeroLink
    , public FirstLink
  {
  public:

    using compare = etl::compare<ItemNDCNode>;

    ItemNDCNode(const int value, const int index)
      : data(value, index)
    {
    }

    friend bool operator<(const ItemNDCNode& lhs, const ItemNDCNode& rhs)
    {
      return lhs.data < rhs.data;
    }

    friend bool operator==(const ItemNDCNode& lhs, const ItemNDCNode& rhs)
    {
      return lhs.data == rhs.data;
    }

    struct CompareByValue
    {
      int                           value;
      etl::optional<etl::exception> excptn;

      explicit CompareByValue(const int value_, const etl::optional<etl::exception>& excptn_ = etl::nullopt)
        : value(value_)
        , excptn(excptn_)
      {
      }

      ETL_NODISCARD
      int operator()(const ItemNDCNode& other) const
      {
        if (excptn.has_value())
        {
          throw etl::exception(excptn.value());
        }
        return value - other.data.value;
      }

    }; // CompareByValue

    ETL_NODISCARD
    static compare::cmp_result always_less(const ItemNDCNode& other)
    {
      (void)other;
      return compare::Less;
    }

    ETL_NODISCARD
    static compare::cmp_result always_greater(const ItemNDCNode& other)
    {
      (void)other;
      return compare::Greater;
    }

    ETL_NODISCARD
    static compare::cmp_result compare_prepend_dups(const ItemNDCNode& lhs, const ItemNDCNode& rhs)
    {
      const auto cmp = compare::cmp(lhs, rhs);
      return (cmp != compare::Equal) ? cmp : compare::Less;
    }

    ETL_NODISCARD
    static compare::cmp_result compare_append_dups(const ItemNDCNode& lhs, const ItemNDCNode& rhs)
    {
      const auto cmp = compare::cmp(lhs, rhs);
      return (cmp != compare::Equal) ? cmp : compare::Greater;
    }

    ItemNDC data;

  }; // ItemNDCNode

  //***************************************************************************
  class ItemMNode : public SecondLink
  {
  public:

    explicit ItemMNode(const int value)
      : data(value)
    {
    }

    ETL_NODISCARD
    friend bool operator<(const ItemMNode& lhs, const ItemMNode& rhs)
    {
      return lhs.data < rhs.data;
    }

    ETL_NODISCARD
    friend bool operator==(const ItemMNode& lhs, const ItemMNode& rhs)
    {
      return lhs.data == rhs.data;
    }

    struct CompareByValue
    {
      int value;

      ETL_NODISCARD
      int operator()(const ItemMNode& other) const
      {
        return value - other.data.value;
      }
    };

    ETL_NODISCARD
    static int compare(const ItemMNode& lhs, const ItemMNode& rhs)
    {
      return lhs.data.value - rhs.data.value;
    }

    ItemM data;

  }; // ItemMNode

  //***************************************************************************
  typedef etl::intrusive_avl_tree<ItemNDCNode>    DataNDC0;
  typedef etl::intrusive_avl_tree<ItemNDCNode, 1> DataNDC1;
  typedef etl::intrusive_avl_tree<ItemMNode, 2>   DataM;

  typedef std::vector<ItemMNode>   InitialDataM;
  typedef std::vector<ItemNDCNode> InitialDataNDC;

  SUITE(test_intrusive_avl_tree)
  {
    //*************************************************************************
    struct SetupFixture
    {
      InitialDataNDC sorted_data;
      InitialDataNDC unsorted_data;
      InitialDataM   sorted_data_moveable;

      SetupFixture()
      {
        sorted_data.clear();
        for (int i = 0; i < 31; ++i)
        {
          sorted_data.emplace_back(i, i);
          sorted_data_moveable.emplace_back(i);
        }

        constexpr std::array<size_t, 31> unsorted_order{
          1, 0, 3, 2, 5, 4, 7, 6, 9, 8, 11, 10, 13, 12, 15, 14, 17, 16, 19, 18, 21, 20, 23, 22, 25, 24, 27, 26, 30, 29, 28,
        };
        unsorted_data.clear();
        for (const auto idx : unsorted_order)
        {
          unsorted_data.emplace_back(static_cast<int>(idx), static_cast<int>(idx));
        }
      }

      template <typename Iterator>
      int verify_link(const Iterator it) const
      {
        if (!it.has_value())
        {
          return 0;
        }

        const auto parent      = it.get_parent();
        const auto left_child  = it.get_child(false);
        const auto right_child = it.get_child(true);
        if (parent.has_value())
        {
          const auto parent_left_child  = parent.get_child(false);
          const auto parent_right_child = parent.get_child(true);
          CHECK((it == parent_left_child) || (it == parent_right_child));
        }

        const int left_height = verify_link(left_child);
        if (left_child.has_value())
        {
          CHECK(it == left_child.get_parent());
        }

        const int right_height = verify_link(right_child);
        if (right_child.has_value())
        {
          CHECK(it == right_child.get_parent());
        }

        const int_least8_t balance_factor = it.get_balance_factor();
        CHECK((-1 <= balance_factor) && (balance_factor <= 1));
        CHECK_EQUAL(right_height - left_height, static_cast<int>(balance_factor));
        return 1 + etl::max(right_height, left_height);
      }

      template <typename Tree>
      void verify_tree(const Tree& tree) const
      {
        typedef typename Tree::value_type                            value_type;
        typedef etl::reverse_iterator<typename Tree::const_iterator> rev_it;

        if (tree.empty())
        {
          CHECK(tree.begin() == tree.end());
          CHECK(!tree.get_root().has_value());
        }
        else
        {
          CHECK(tree.get_root().has_value());
          CHECK(!tree.get_root().get_parent().has_value());
        }

        const auto root = tree.get_root();
        verify_link(root);
        CHECK(etl::is_sorted(tree.begin(), tree.end()));
        CHECK(etl::is_sorted(rev_it(tree.end()), rev_it(tree.begin()), etl::greater<value_type>()));
        CHECK(tree.min() == tree.begin());
        CHECK(tree.max() == --tree.end());
      }

      template <typename Tree>
      ETL_NODISCARD
      std::string to_graphviz(const Tree& tree) const
      {
        std::ostringstream ss;
        ss << "// https://magjac.com/graphviz-visual-editor/\n";
        ss << "// OR paste to `dot -Tsvg > output.svg`.\n//\n";
        ss << "digraph {\n";
        ss << "node[style=filled,fontcolor=white,shape=egg,fillcolor=black,fontsize=28,fontname=Menlo];\n";

        const auto beg = tree.begin();
        const auto end = tree.end();
        ss << "origin[fillcolor=gray];";
        for (auto curr = beg; curr != end; ++curr)
        {
          ss << curr->data.index;
          ss << "[label=" << curr->data.value;
          const auto balance_factor = curr.get_balance_factor();
          if (balance_factor != 0)
          {
            const std::string bf_color = (balance_factor < 0) ? "blue" : "orangered";
            ss << ",fillcolor=" << bf_color;
          }
          ss << "];";
        }
        ss << "\n";
        for (auto curr = beg; curr != end; ++curr)
        {
          if (auto child = curr.get_child(false))
          {
            ss << curr->data.index;
            ss << ":sw->" << child->data.index << ":n;";
          }
          if (auto child = curr.get_child(true))
          {
            ss << curr->data.index;
            ss << ":se->" << child->data.index << ":n;";
          }
        }
        if (auto root = tree.get_root())
        {
          ss << "origin:sw->" << root->data.index << ":n[label=root,fontsize=28];";
        }
        ss << "\n}\n\n";
        return ss.str();
      }
    };

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_default_constructor)
    {
      DataNDC0 data0;
      DataNDC1 data1;

      CHECK(data0.empty());
      CHECK_EQUAL(0, data0.size());
      CHECK(data1.empty());
      CHECK_EQUAL(0, data1.size());

      CHECK(data0.max() == data0.end());
      CHECK(data0.min() == data0.end());
      CHECK(data0.begin() == data0.end());
      CHECK(data0.cbegin() == data0.cend());
      CHECK(data0.lower_bound(ItemNDCNode::always_less) == data0.end());
      CHECK(data0.lower_bound(ItemNDCNode::always_greater) == data0.end());
      CHECK(data0.upper_bound(ItemNDCNode::always_less) == data0.end());
      CHECK(data0.upper_bound(ItemNDCNode::always_greater) == data0.end());

      CHECK(data1.max() == data1.end());
      CHECK(data1.min() == data1.end());
      CHECK(data1.begin() == data1.end());
      CHECK(data1.cbegin() == data1.cend());
      CHECK(data1.lower_bound(ItemNDCNode::always_less) == data1.end());
      CHECK(data1.lower_bound(ItemNDCNode::always_greater) == data1.end());
      CHECK(data1.upper_bound(ItemNDCNode::always_less) == data1.end());
      CHECK(data1.upper_bound(ItemNDCNode::always_greater) == data1.end());

      verify_tree(data0);
      verify_tree(data1);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_range_append_duplicates)
    {
      InitialDataNDC duplicate_data;
      duplicate_data.emplace_back(10, 0);
      duplicate_data.emplace_back(5, 1);
      duplicate_data.emplace_back(10, 2);
      duplicate_data.emplace_back(15, 3);
      duplicate_data.emplace_back(10, 4);
      duplicate_data.emplace_back(7, 5);
      duplicate_data.emplace_back(10, 6);
      duplicate_data.emplace_back(12, 7);

      DataNDC0 data0(duplicate_data.begin(), duplicate_data.end(), ItemNDCNode::compare_append_dups);
      verify_tree(data0);
      CHECK_EQUAL(duplicate_data.size(), data0.size());

      std::vector<std::pair<int, int>> actual_values_idx;
      for (auto it = data0.begin(); it != data0.end(); ++it)
      {
        actual_values_idx.emplace_back(it->data.value, it->data.index);
      }
      const std::vector<std::pair<int, int>> expected_values_idx{
        std::make_pair(5, 1),  std::make_pair(7, 5),  std::make_pair(10, 0), std::make_pair(10, 2),
        std::make_pair(10, 4), std::make_pair(10, 6), std::make_pair(12, 7), std::make_pair(15, 3),
      };
      CHECK(actual_values_idx == expected_values_idx);
      CHECK_EQUAL(data0.min()->data.index, 1);
      CHECK_EQUAL(data0.max()->data.index, 3);

      size_t duplicate_count = 0;
      for (auto it = data0.begin(); it != data0.end(); ++it)
      {
        if (it->data.value == 10)
        {
          ++duplicate_count;
        }
      }
      CHECK_EQUAL(4U, duplicate_count);

      while (true)
      {
        auto it = data0.find(ItemNDCNode::CompareByValue{10});
        if (it == data0.end())
        {
          break;
        }
        data0.erase(it);
        verify_tree(data0);
      }

      std::vector<int> values_after_erase;
      for (auto it = data0.begin(); it != data0.end(); ++it)
      {
        values_after_erase.push_back(it->data.value);
      }
      const std::vector<int> expected_after_erase{5, 7, 12, 15};
      CHECK(values_after_erase == expected_after_erase);
      CHECK(data0.find(ItemNDCNode::CompareByValue{10}) == data0.end());
      CHECK_EQUAL(data0.min()->data.index, 1);
      CHECK_EQUAL(data0.max()->data.index, 3);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_range_prepend_duplicates)
    {
      InitialDataNDC duplicate_data;
      duplicate_data.emplace_back(10, 0);
      duplicate_data.emplace_back(7, 1);
      duplicate_data.emplace_back(10, 2);
      duplicate_data.emplace_back(15, 3);
      duplicate_data.emplace_back(10, 4);
      duplicate_data.emplace_back(7, 5);
      duplicate_data.emplace_back(10, 6);
      duplicate_data.emplace_back(12, 7);

      DataNDC0 data0(duplicate_data.begin(), duplicate_data.end(), ItemNDCNode::compare_prepend_dups);
      verify_tree(data0);
      CHECK_EQUAL(duplicate_data.size(), data0.size());

      std::vector<std::pair<int, int>> actual_values_idx;
      for (auto it = data0.begin(); it != data0.end(); ++it)
      {
        actual_values_idx.emplace_back(it->data.value, it->data.index);
      }
      const std::vector<std::pair<int, int>> expected_values_idx{
        std::make_pair(7, 5),  std::make_pair(7, 1),  std::make_pair(10, 6), std::make_pair(10, 4),
        std::make_pair(10, 2), std::make_pair(10, 0), std::make_pair(12, 7), std::make_pair(15, 3),
      };
      CHECK(actual_values_idx == expected_values_idx);
      CHECK_EQUAL(data0.min()->data.index, 5);
      CHECK_EQUAL(data0.max()->data.index, 3);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_range_avoid_duplicates)
    {
      InitialDataNDC duplicate_data;
      duplicate_data.emplace_back(10, 0);
      duplicate_data.emplace_back(7, 1);
      duplicate_data.emplace_back(10, 2);
      duplicate_data.emplace_back(15, 3);
      duplicate_data.emplace_back(10, 4);
      duplicate_data.emplace_back(7, 5);
      duplicate_data.emplace_back(10, 6);
      duplicate_data.emplace_back(12, 7);

      DataNDC0 data0(duplicate_data.begin(), duplicate_data.end(), ItemNDCNode::compare::cmp);
      verify_tree(data0);

      std::vector<std::pair<int, int>> actual_values_idx;
      for (auto it = data0.begin(); it != data0.end(); ++it)
      {
        actual_values_idx.emplace_back(it->data.value, it->data.index);
      }
      const std::vector<std::pair<int, int>> expected_values_idx{
        std::make_pair(7, 1),
        std::make_pair(10, 0),
        std::make_pair(12, 7),
        std::make_pair(15, 3),
      };
      CHECK(actual_values_idx == expected_values_idx);
      CHECK_EQUAL(data0.min()->data.index, 1);
      CHECK_EQUAL(data0.max()->data.index, 3);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_with_bad_iterator)
    {
      auto action = [this]()
      {
        // Note end/begin order -> should throw!
        DataNDC0 data0(sorted_data.end(), sorted_data.begin(), ItemNDCNode::compare::cmp);
      };
      CHECK_THROW(action(), etl::intrusive_avl_tree_iterator_exception);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_partial_construction_unlinks_inserted_items)
    {
      InitialDataNDC nodes;
      nodes.emplace_back(0, 0);
      nodes.emplace_back(1, 1);
      nodes.emplace_back(2, 2);
      nodes.emplace_back(3, 3);
      nodes.emplace_back(4, 4);

      DataNDC0 other_tree;
      other_tree.find_or_insert(ItemNDCNode::CompareByValue{2}, [&nodes] { return &nodes[2]; });

      auto construct_failing_tree = [&]()
      {
        DataNDC0 data0(nodes.begin(), nodes.end(), ItemNDCNode::compare::cmp);
      };
      CHECK_THROW(construct_failing_tree(), etl::intrusive_avl_tree_value_is_already_linked);

      // The foreign tree still owns the conflicting node.
      CHECK_EQUAL(1, other_tree.size());
      verify_tree(other_tree);
      {
        const auto it = other_tree.find(ItemNDCNode::CompareByValue{2});
        CHECK(it.has_value());
        CHECK_EQUAL(&nodes[2], it.get());
      }

      // Previously inserted nodes from the failed construction must have been unlinked.
      other_tree.erase(other_tree.find(ItemNDCNode::CompareByValue{2}));
      CHECK(other_tree.empty());

      DataNDC0 recovered(nodes.begin(), nodes.end(), ItemNDCNode::compare::cmp);
      CHECK_EQUAL(nodes.size(), recovered.size());
      verify_tree(recovered);
      CHECK(std::equal(recovered.begin(), recovered.end(), nodes.begin()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_empty_begin_end_root)
    {
      DataNDC0 data0;

      CHECK(data0.begin() == data0.end());
      CHECK(!data0.get_root().has_value());

      const DataNDC0::const_iterator begin = data0.begin();
      const DataNDC0::const_iterator end   = data0.end();
      CHECK(begin == end);

      CHECK(data0.cbegin() == data0.cend());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_iterator)
    {
      typedef etl::reverse_iterator<DataNDC0::iterator> rev_it;

      DataNDC0 data0(sorted_data.begin(), sorted_data.end(), ItemNDCNode::compare::cmp);
      verify_tree(data0);

      bool are_equal = std::equal(data0.begin(), data0.end(), sorted_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(rev_it(data0.end()), rev_it(data0.begin()), sorted_data.rbegin());
      CHECK(are_equal);

      auto curr = data0.begin();
      CHECK(curr);
      CHECK(curr.has_value());
      const auto& front = *curr;
      CHECK_EQUAL(&front, curr.get());
      CHECK_EQUAL(front.data.value, sorted_data.front().data.value);
      CHECK_EQUAL(curr->data.value, sorted_data.front().data.value);
      auto prev = curr++;
      CHECK(curr == prev.get_parent());
      CHECK(prev == curr.get_child(false));
      CHECK(curr != data0.begin());
      CHECK(prev == data0.begin());
      CHECK(prev-- == data0.begin());
      CHECK(prev == data0.end());

      curr = data0.end();
      CHECK(!curr);
      CHECK(!curr.has_value());
      CHECK(nullptr == curr.get());
      CHECK(curr-- == data0.end());
      CHECK(curr != data0.end());
      CHECK(curr);
      CHECK(curr.has_value());
      CHECK_EQUAL(curr->data.value, sorted_data.back().data.value);

      prev = curr--;
      CHECK(curr == prev.get_parent());
      CHECK(prev == curr.get_child(true));

      curr = data0.get_root();
      CHECK(curr.has_value());
      CHECK_EQUAL(curr->data.value, sorted_data.at(sorted_data.size() / 2).data.value);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_iterator_default)
    {
      DataNDC0::iterator it;

      CHECK(nullptr == it.get());
      CHECK_EQUAL(false, it.has_value());
      CHECK_EQUAL(false, static_cast<bool>(it));

      CHECK_EQUAL(0, it.get_balance_factor());
      CHECK_EQUAL(false, it.get_parent().has_value());
      CHECK_EQUAL(false, it.get_child(true).has_value());
      CHECK_EQUAL(false, it.get_child(false).has_value());

      ++it;
      CHECK(nullptr == it.get());
      CHECK_EQUAL(false, it.has_value());
      CHECK_EQUAL(false, static_cast<bool>(it));

      --it;
      CHECK(nullptr == it.get());
      CHECK_EQUAL(false, it.has_value());
      CHECK_EQUAL(false, static_cast<bool>(it));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_iterator)
    {
      typedef etl::reverse_iterator<DataNDC0::const_iterator> rev_it;

      const DataNDC0 data0(unsorted_data.begin(), unsorted_data.end(), ItemNDCNode::compare::cmp);
      verify_tree(data0);

#ifdef ETL_AVL_DUMP_GRAPHVIZ
      const auto tree_dump = to_graphviz(data0);
      std::cout << tree_dump;
#endif

      bool are_equal = std::equal(data0.begin(), data0.end(), sorted_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(rev_it(data0.cend()), rev_it(data0.cbegin()), sorted_data.rbegin());
      CHECK(are_equal);

      auto curr = data0.begin();
      CHECK(curr);
      CHECK(curr.has_value());
      const auto& front = *curr;
      CHECK_EQUAL(&front, curr.get());
      CHECK_EQUAL(front.data.value, sorted_data.front().data.value);
      CHECK_EQUAL(curr->data.value, sorted_data.front().data.value);
      auto prev = curr++;
      CHECK(curr == prev.get_parent());
      CHECK(prev == curr.get_child(false));
      CHECK(curr != data0.begin());
      CHECK(prev == data0.begin());
      CHECK(prev-- == data0.begin());
      CHECK(prev == data0.end());

      curr = data0.end();
      CHECK(!curr);
      CHECK(!curr.has_value());
      CHECK(nullptr == curr.get());
      CHECK(curr-- == data0.end());
      CHECK(curr);
      CHECK(curr.has_value());
      CHECK(curr != data0.end());
      CHECK_EQUAL(curr->data.value, sorted_data.back().data.value);

      prev = curr--;
      CHECK(curr == prev.get_parent());
      CHECK(prev == curr.get_child(true));

      curr = data0.get_root();
      CHECK(curr.has_value());
      CHECK_EQUAL(curr->data.value, sorted_data.at(sorted_data.size() / 2).data.value);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_iterator_default)
    {
      DataNDC0::const_iterator it;

      CHECK(nullptr == it.get());
      CHECK_EQUAL(false, it.has_value());
      CHECK_EQUAL(false, static_cast<bool>(it));

      CHECK_EQUAL(0, it.get_balance_factor());
      CHECK_EQUAL(false, it.get_parent().has_value());
      CHECK_EQUAL(false, it.get_child(true).has_value());
      CHECK_EQUAL(false, it.get_child(false).has_value());

      ++it;
      CHECK(nullptr == it.get());
      CHECK_EQUAL(false, it.has_value());
      CHECK_EQUAL(false, static_cast<bool>(it));

      --it;
      CHECK(nullptr == it.get());
      CHECK_EQUAL(false, it.has_value());
      CHECK_EQUAL(false, static_cast<bool>(it));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_iterator_misuse_assertions)
    {
      DataNDC0 data0(sorted_data.begin(), sorted_data.end(), ItemNDCNode::compare::cmp);

      auto dereference_default_iterator = []()
      {
        DataNDC0::iterator it;
        return (*it).data.value;
      };
      CHECK_THROW(dereference_default_iterator(), etl::intrusive_avl_tree_iterator_exception);

      auto arrow_default_iterator = []()
      {
        DataNDC0::iterator it;
        return it->data.value;
      };
      CHECK_THROW(arrow_default_iterator(), etl::intrusive_avl_tree_iterator_exception);

      auto dereference_end_iterator = [&]()
      {
        return (*data0.end()).data.value;
      };
      CHECK_THROW(dereference_end_iterator(), etl::intrusive_avl_tree_iterator_exception);

      auto arrow_end_iterator = [&]()
      {
        return data0.end()->data.value;
      };
      CHECK_THROW(arrow_end_iterator(), etl::intrusive_avl_tree_iterator_exception);

      auto erase_default_iterator = [&]()
      {
        DataNDC0::iterator it;
        data0.erase(it);
      };
      CHECK_THROW(erase_default_iterator(), etl::intrusive_avl_tree_iterator_exception);

      auto erase_end_iterator = [&]()
      {
        data0.erase(data0.end());
      };
      CHECK_THROW(erase_end_iterator(), etl::intrusive_avl_tree_iterator_exception);

      // Try foreign tree begin iterator.
      {
        DataNDC0 data0b;
        auto     erase_foreign_iterator = [&]()
        {
          data0b.erase(data0.begin());
        };
        CHECK_THROW(erase_foreign_iterator(), etl::intrusive_avl_tree_iterator_exception);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find)
    {
      DataNDC0 data0(sorted_data.begin(), sorted_data.end(), ItemNDCNode::compare::cmp);

      auto iterator = data0.find(ItemNDCNode::always_less);
      CHECK(!iterator.has_value());
      CHECK(iterator == data0.end());

      iterator = data0.find(ItemNDCNode::always_greater);
      CHECK(!iterator.has_value());
      CHECK(iterator == data0.end());

      iterator = data0.find(ItemNDCNode::CompareByValue{5});
      CHECK(iterator.has_value());
      CHECK(iterator != data0.end());
      CHECK_EQUAL(iterator->data, sorted_data[5].data);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_const)
    {
      const DataNDC0 data0(sorted_data.begin(), sorted_data.end(), ItemNDCNode::compare::cmp);

      auto iterator = data0.find(ItemNDCNode::always_less);
      CHECK(!iterator.has_value());
      CHECK(iterator == data0.end());

      iterator = data0.find(ItemNDCNode::always_greater);
      CHECK(!iterator.has_value());
      CHECK(iterator == data0.end());

      iterator = data0.find(ItemNDCNode::CompareByValue{5});
      CHECK(iterator != data0.end());
      CHECK(iterator.has_value());
      CHECK_EQUAL(iterator->data, sorted_data[5].data);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_throwing)
    {
      const DataNDC0 data0(sorted_data.begin(), sorted_data.end(), ItemNDCNode::compare::cmp);

      const ItemNDCNode::CompareByValue throwing_compare{0, etl::exception("13", __FILE__, __LINE__)};
      CHECK_THROW(data0.find(throwing_compare), etl::exception);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_or_insert)
    {
      DataNDC0 data0;

      ItemNDCNode node0a(0, 0);
      ItemNDCNode node0b(0, 1);

      // Insert new.
      {
        CHECK(data0.empty());
        const auto it_mod = data0.find_or_insert(ItemNDCNode::CompareByValue{0}, [&node0a] { return &node0a; });
        CHECK(!data0.empty());
        CHECK_EQUAL(1, data0.size());
        CHECK_EQUAL(data0.min().get(), &node0a);
        CHECK_EQUAL(data0.max().get(), &node0a);
        verify_tree(data0);

        CHECK(it_mod.second);
        CHECK(it_mod.first.has_value());
        CHECK(it_mod.first != data0.end());
        CHECK_EQUAL(&node0a, it_mod.first.get());
      }

      // Find existing.
      {
        const auto it_mod = data0.find_or_insert(ItemNDCNode::CompareByValue{0}, [&node0b] { return &node0b; });

        CHECK(!it_mod.second);
        CHECK(it_mod.first.has_value());
        CHECK(it_mod.first != data0.end());
        CHECK_EQUAL(&node0a, it_mod.first.get());
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_or_insert_unsorted)
    {
      DataNDC0 data0;
      verify_tree(data0);

      for (auto& item : unsorted_data)
      {
        data0.find_or_insert(ItemNDCNode::CompareByValue{item.data.value}, [&item] { return &item; });
        verify_tree(data0);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_or_insert_null_factory)
    {
      DataNDC0 data0;

      const auto it_mod = data0.find_or_insert(ItemNDCNode::always_greater, [] { return nullptr; });
      CHECK(data0.empty());
      CHECK_EQUAL(0, data0.size());
      verify_tree(data0);

      CHECK(!it_mod.second);
      CHECK(!it_mod.first.has_value());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_or_insert_already_linked)
    {
      DataNDC0 data0;

      ItemNDCNode node0(0, 0);

      auto insert = [&]()
      {
        return data0.find_or_insert(ItemNDCNode::always_greater, [&node0] { return &node0; });
      };

      auto it_mod = insert();
      CHECK(!data0.empty());
      CHECK_EQUAL(1, data0.size());
      verify_tree(data0);

      // Insert the same node again -> should throw.
      {
        CHECK_THROW(insert(), etl::intrusive_avl_tree_value_is_already_linked);
        CHECK(!data0.empty());
        CHECK_EQUAL(1, data0.size());
        verify_tree(data0);
      }

      // But it's ok to erase it first, and then reinsert.
      {
        data0.erase(it_mod.first);
        CHECK_EQUAL(0, data0.size());

        it_mod = insert();
        CHECK_EQUAL(1, data0.size());

        CHECK(it_mod.second);
        CHECK(it_mod.first.has_value());
        CHECK(it_mod.first != data0.end());
        CHECK_EQUAL(&node0, it_mod.first.get());
        verify_tree(data0);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_or_insert_throwing)
    {
      DataNDC0 data0(sorted_data.begin(), sorted_data.end(), ItemNDCNode::compare::cmp);
      CHECK_EQUAL(sorted_data.size(), data0.size());

      // Try throwing factory.
      {
        auto throwing_action = [&]()
        {
          return data0.find_or_insert(ItemNDCNode::always_greater, []() -> ItemNDCNode* { throw etl::exception("123", __FILE__, __LINE__); });
        };
        CHECK_THROW(throwing_action(), etl::exception);
        CHECK_EQUAL(sorted_data.size(), data0.size());
        verify_tree(data0);
        CHECK(std::equal(data0.begin(), data0.end(), sorted_data.begin()));
      }

      // Try throwing comparator.
      {
        auto throwing_action = [&]()
        {
          return data0.find_or_insert([](const ItemNDCNode&) -> int { throw etl::exception("321", __FILE__, __LINE__); },
                                      []() -> ItemNDCNode* { return nullptr; });
        };
        CHECK_THROW(throwing_action(), etl::exception);
        CHECK_EQUAL(sorted_data.size(), data0.size());
        verify_tree(data0);
        CHECK(std::equal(data0.begin(), data0.end(), sorted_data.begin()));
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase)
    {
      DataNDC0 data0(sorted_data.begin(), sorted_data.end(), ItemNDCNode::compare::cmp);
      verify_tree(data0);

      size_t expected_size = sorted_data.size();
      for (const auto& item : unsorted_data)
      {
        const DataNDC0::iterator it = data0.find(ItemNDCNode::CompareByValue{item.data.value});
        CHECK(it != data0.end());

        CHECK_EQUAL(expected_size, data0.size());
        const DataNDC0::iterator next_it = data0.erase(it);
        CHECK_EQUAL(--expected_size, data0.size());
        verify_tree(data0);
        if (next_it != data0.end())
        {
          CHECK(next_it.has_value());
          const auto& next = *next_it;
          CHECK(item < next);
        }
      }
      CHECK(data0.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_const_it)
    {
      DataNDC0 data0(sorted_data.begin(), sorted_data.end(), ItemNDCNode::compare::cmp);
      verify_tree(data0);

      size_t expected_size = sorted_data.size();
      for (const auto& item : unsorted_data)
      {
        const DataNDC0::const_iterator it = data0.find(ItemNDCNode::CompareByValue{item.data.value});
        CHECK(it != data0.end());

        CHECK_EQUAL(expected_size, data0.size());
        const DataNDC0::iterator next_it = data0.erase(it);
        CHECK_EQUAL(--expected_size, data0.size());
        verify_tree(data0);
        if (next_it != data0.end())
        {
          CHECK(next_it.has_value());
          const auto& next = *next_it;
          CHECK(item < next);
        }
      }
      CHECK(data0.empty());
    }

    //*************************************************************************
    struct Inserter
    {
      InitialDataNDC& nodes;

      template <typename Tree>
      void operator()(Tree& tree, const size_t n) const
      {
        const ItemNDCNode::CompareByValue comp{static_cast<int>(n)};
        const auto                        it = tree.find(comp);
        if (it == tree.end())
        {
          bool factory_was_called = false;
          auto it_mod             = tree.find_or_insert(comp,
                                                        [&]
                                                        {
                                              factory_was_called = true;
                                              return &nodes.at(n);
                                            });
          CHECK(it_mod.second);
          CHECK(factory_was_called);
          CHECK_EQUAL(n, it_mod.first->data.value);
          CHECK_EQUAL(n, it_mod.first->data.index);
        }
        else
        {
          CHECK_EQUAL(n, it->data.value);
          CHECK_EQUAL(n, it->data.index);
          auto it_mod = tree.find_or_insert(comp,
                                            []
                                            {
                                              CHECK_MESSAGE("Should not be called!")
                                              CHECK(false);
                                              return nullptr;
                                            });
          CHECK(it == it_mod.first);
          CHECK_FALSE(it_mod.second);
        }
      }
    };
    struct Eraser
    {
      template <typename Tree>
      void operator()(Tree& tree, const size_t n) const
      {
        const ItemNDCNode::CompareByValue comp{static_cast<int>(n)};
        auto                              it = tree.find(comp);
        if (it != tree.end())
        {
          CHECK_EQUAL(n, it->data.value);
          CHECK_EQUAL(n, it->data.index);
          tree.erase(it);
          it = tree.find(comp);
          CHECK(it == tree.end());
        }
      }
    };
    TEST_FIXTURE(SetupFixture, test_random_insert_and_erase)
    {
      // Deliberately seeded with fixed number, so that if it fails then always in the same way.
      std::mt19937 mte(123);

      InitialDataNDC   nodes;
      constexpr size_t N = 256;
      for (size_t i = 0; i < N; ++i)
      {
        nodes.emplace_back(static_cast<int>(i), static_cast<int>(i));
      }

      DataNDC0 data0;
      DataNDC1 data1;

      auto makeRandomNumber = [&mte](const size_t n) -> size_t
      {
        return mte() % n;
      };
      const Eraser   erase_item;
      const Inserter insert_item{nodes};

      for (size_t i = 0; i < 10000; ++i)
      {
        const auto number = makeRandomNumber(N);
        switch (makeRandomNumber(4))
        {
          case 0: erase_item(data0, number); break;
          case 1: erase_item(data1, number); break;
          case 2: insert_item(data0, number); break;
          case 3: insert_item(data1, number); break;
        }

        verify_tree(data0);
        verify_tree(data1);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_move_item)
    {
      const DataM data0(sorted_data_moveable.begin(), sorted_data_moveable.end(), ItemMNode::compare);
      verify_tree(data0);

      const ItemMNode min_item{std::move(sorted_data_moveable.front())};
      CHECK(min_item.data.valid);
      CHECK_FALSE(sorted_data_moveable.front().data.valid);
      CHECK(min_item.data.value == 0);
      auto it = data0.begin();
      CHECK(it.get() == &min_item);
      CHECK_EQUAL(sorted_data_moveable.size(), data0.size());
      verify_tree(data0);

      const ItemMNode max_item{std::move(sorted_data_moveable.back())};
      CHECK(max_item.data.value == static_cast<int>(sorted_data_moveable.size() - 1));
      it = --data0.end();
      CHECK(it.get() == &max_item);
      CHECK_EQUAL(sorted_data_moveable.size(), data0.size());
      verify_tree(data0);

      auto            item_idx = sorted_data_moveable.size() / 2;
      const ItemMNode root_item{std::move(sorted_data_moveable.at(item_idx))};
      CHECK_EQUAL(root_item.data.value, item_idx);
      it = data0.get_root();
      CHECK(it.has_value());
      CHECK(it.get() == &root_item);
      CHECK_EQUAL(sorted_data_moveable.size(), data0.size());
      verify_tree(data0);

      item_idx = 11;
      const ItemMNode item{std::move(sorted_data_moveable.at(item_idx))};
      CHECK(item.data.value == static_cast<int>(item_idx));
      it = data0.find(ItemMNode::CompareByValue{static_cast<int>(item_idx)});
      CHECK(it.get() == &item);
      CHECK_EQUAL(sorted_data_moveable.size(), data0.size());
      verify_tree(data0);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_move)
    {
      DataM       data0a(sorted_data_moveable.begin(), sorted_data_moveable.end(), ItemMNode::compare);
      const DataM data0b(std::move(data0a));
      CHECK(data0a.empty());
      CHECK_EQUAL(0, data0a.size());
      CHECK_EQUAL(sorted_data_moveable.size(), data0b.size());
      verify_tree(data0b);
      CHECK(std::equal(data0b.begin(), data0b.end(), sorted_data_moveable.begin()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_move_empty)
    {
      DataM       data0a;
      const DataM data0b(std::move(data0a));

      CHECK(data0a.empty());
      CHECK_EQUAL(0, data0a.size());
      CHECK(data0b.empty());
      CHECK_EQUAL(0, data0b.size());
      verify_tree(data0a);
      verify_tree(data0b);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_move_single_and_mutate)
    {
      InitialDataM nodes;
      nodes.emplace_back(1);
      nodes.emplace_back(2);

      DataM data0a(nodes.begin(), nodes.begin() + 1, ItemMNode::compare);
      verify_tree(data0a);

      DataM data0b(std::move(data0a));
      CHECK(data0a.empty());
      CHECK_EQUAL(0, data0a.size());
      CHECK_EQUAL(1, data0b.size());
      verify_tree(data0a);
      verify_tree(data0b);

      const auto it_mod = data0b.find_or_insert(ItemMNode::CompareByValue{2}, [&nodes] { return &nodes.back(); });
      CHECK(it_mod.second);
      CHECK(it_mod.first.has_value());
      CHECK_EQUAL(2, it_mod.first->data.value);
      CHECK_EQUAL(2, data0b.size());
      verify_tree(data0b);

      const std::vector<int> expected_values{1, 2};
      std::vector<int>       actual_values;
      for (const auto& it : data0b)
      {
        actual_values.push_back(it.data.value);
      }
      CHECK(actual_values == expected_values);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_move_assignment_item)
    {
      DataM data0(sorted_data_moveable.begin(), sorted_data_moveable.end(), ItemMNode::compare);
      verify_tree(data0);

      // Replace item 11 with a temp item.
      {
        ItemMNode replacement(100);
        replacement = std::move(sorted_data_moveable.at(11));

        CHECK(replacement.data.valid);
        CHECK_FALSE(sorted_data_moveable.at(11).data.valid);
        CHECK_TRUE(replacement.data.valid);
        CHECK_EQUAL(11, replacement.data.value);
        CHECK_EQUAL(sorted_data_moveable.size(), data0.size());
        CHECK(data0.find(ItemMNode::CompareByValue{11}).get() == &replacement);
        verify_tree(data0);
      }
      CHECK_EQUAL(sorted_data_moveable.size() - 1, data0.size());
      verify_tree(data0);

      // Try to move (item 5) into already linked (item 3).
      {
        sorted_data_moveable.at(3) = std::move(sorted_data_moveable.at(5));
        CHECK_TRUE(sorted_data_moveable.at(3).data.valid);
        CHECK_FALSE(sorted_data_moveable.at(5).data.valid);
        CHECK_EQUAL(5, sorted_data_moveable.at(3).data.value);
        CHECK_EQUAL(sorted_data_moveable.size() - 2, data0.size());
        verify_tree(data0);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_move_assignment_item_self)
    {
      DataM data0(sorted_data_moveable.begin(), sorted_data_moveable.end(), ItemMNode::compare);
      verify_tree(data0);

      ItemMNode&       self     = sorted_data_moveable.at(11);
      ItemMNode* const self_ptr = &self;
      *self_ptr                 = std::move(self);

      CHECK(self.data.valid);
      CHECK_EQUAL(11, self.data.value);
      CHECK_EQUAL(sorted_data_moveable.size(), data0.size());
      CHECK(data0.find(ItemMNode::CompareByValue{11}).get() == self_ptr);
      verify_tree(data0);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_move_assignment_item_over_linked_target)
    {
      DataM     data0(sorted_data_moveable.begin(), sorted_data_moveable.end(), ItemMNode::compare);
      ItemMNode replacement(100);

      const auto inserted = data0.find_or_insert(ItemMNode::CompareByValue{100}, [&replacement] { return &replacement; });
      CHECK(inserted.second);
      CHECK_EQUAL(sorted_data_moveable.size() + 1U, data0.size());
      verify_tree(data0);

      replacement = std::move(sorted_data_moveable.at(11));

      CHECK(replacement.data.valid);
      CHECK_FALSE(sorted_data_moveable.at(11).data.valid);
      CHECK_EQUAL(11, replacement.data.value);
      CHECK_EQUAL(sorted_data_moveable.size(), data0.size());
      CHECK(data0.find(ItemMNode::CompareByValue{11}).get() == &replacement);
      CHECK(data0.find(ItemMNode::CompareByValue{100}) == data0.end());
      verify_tree(data0);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment_move)
    {
      InitialDataM lhs_nodes;
      lhs_nodes.emplace_back(40);
      lhs_nodes.emplace_back(41);
      InitialDataM rhs_nodes;
      rhs_nodes.emplace_back(1);
      rhs_nodes.emplace_back(2);
      rhs_nodes.emplace_back(3);

      DataM data0a(lhs_nodes.begin(), lhs_nodes.end(), ItemMNode::compare);
      DataM data0b(rhs_nodes.begin(), rhs_nodes.end(), ItemMNode::compare);
      verify_tree(data0a);
      verify_tree(data0b);

      data0a = std::move(data0b);

      CHECK(data0b.empty());
      CHECK_EQUAL(0, data0b.size());
      CHECK_EQUAL(rhs_nodes.size(), data0a.size());
      verify_tree(data0a);
      verify_tree(data0b);
      CHECK(std::equal(data0a.begin(), data0a.end(), rhs_nodes.begin()));

      DataM      recovery;
      const auto it40 = recovery.find_or_insert(ItemMNode::CompareByValue{40}, [&lhs_nodes] { return &lhs_nodes[0]; });
      const auto it41 = recovery.find_or_insert(ItemMNode::CompareByValue{41}, [&lhs_nodes] { return &lhs_nodes[1]; });
      CHECK(it40.second);
      CHECK(it41.second);
      CHECK_EQUAL(2, recovery.size());
      verify_tree(recovery);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment_move_self)
    {
      DataM data0(sorted_data_moveable.begin(), sorted_data_moveable.end(), ItemMNode::compare);
      verify_tree(data0);

      DataM* const self_ptr = &data0;
      *self_ptr             = std::move(data0);

      CHECK_EQUAL(sorted_data_moveable.size(), data0.size());
      verify_tree(data0);
      for (size_t i = 0; i < sorted_data_moveable.size(); ++i)
      {
        CHECK(data0.find(ItemMNode::CompareByValue{static_cast<int>(i)}).get() == &sorted_data_moveable[i]);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_swap_member)
    {
      InitialDataM lhs_nodes;
      lhs_nodes.emplace_back(40);
      lhs_nodes.emplace_back(41);
      InitialDataM rhs_nodes;
      rhs_nodes.emplace_back(1);
      rhs_nodes.emplace_back(2);
      rhs_nodes.emplace_back(3);

      DataM data0a(lhs_nodes.begin(), lhs_nodes.end(), ItemMNode::compare);
      DataM data0b(rhs_nodes.begin(), rhs_nodes.end(), ItemMNode::compare);
      verify_tree(data0a);
      verify_tree(data0b);

      data0a.swap(data0b);

      CHECK_EQUAL(rhs_nodes.size(), data0a.size());
      CHECK_EQUAL(lhs_nodes.size(), data0b.size());
      verify_tree(data0a);
      verify_tree(data0b);
      CHECK(std::equal(data0a.begin(), data0a.end(), rhs_nodes.begin()));
      CHECK(std::equal(data0b.begin(), data0b.end(), lhs_nodes.begin()));

      // Try also self-swap.
      data0a.swap(data0a);
      verify_tree(data0a);
      CHECK(std::equal(data0a.begin(), data0a.end(), rhs_nodes.begin()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_swap_adl)
    {
      InitialDataM lhs_nodes;
      lhs_nodes.emplace_back(50);
      lhs_nodes.emplace_back(51);
      InitialDataM rhs_nodes;
      rhs_nodes.emplace_back(5);
      rhs_nodes.emplace_back(6);
      rhs_nodes.emplace_back(7);

      DataM data0a(lhs_nodes.begin(), lhs_nodes.end(), ItemMNode::compare);
      DataM data0b(rhs_nodes.begin(), rhs_nodes.end(), ItemMNode::compare);
      verify_tree(data0a);
      verify_tree(data0b);

      swap(data0a, data0b);

      CHECK_EQUAL(rhs_nodes.size(), data0a.size());
      CHECK_EQUAL(lhs_nodes.size(), data0b.size());
      verify_tree(data0a);
      verify_tree(data0b);
      CHECK(std::equal(data0a.begin(), data0a.end(), rhs_nodes.begin()));
      CHECK(std::equal(data0b.begin(), data0b.end(), lhs_nodes.begin()));

      // Try also self-swap.
      swap(data0a, data0a);
      verify_tree(data0a);
      CHECK(std::equal(data0a.begin(), data0a.end(), rhs_nodes.begin()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_clear)
    {
      DataM data0a(sorted_data_moveable.begin(), sorted_data_moveable.end(), ItemMNode::compare);
      CHECK_EQUAL(sorted_data_moveable.size(), data0a.size());
      data0a.clear();
      CHECK(data0a.empty());
      CHECK_EQUAL(0, data0a.size());
      verify_tree(data0a);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, visit_in_order)
    {
      const std::vector<int> ids{0, 1, 2, 3, 4, 5};
      InitialDataNDC         data;
      for (const auto idx : ids)
      {
        data.emplace_back(idx, idx);
      }
      DataNDC0 data0(data.begin(), data.end(), ItemNDCNode::compare::cmp);

      // In-order, LNR
      {
        std::vector<int> order;
        data0.visit_in_order(false, [&order](ItemNDCNode& item) { order.push_back(item.data.value); });
        const std::vector<int> expected{0, 1, 2, 3, 4, 5};
        CHECK(order == expected);
      }

      // Reverse In-order, RNL
      {
        std::vector<int> order;
        data0.visit_in_order(true, [&order](ItemNDCNode& item) { order.push_back(item.data.value); });
        const std::vector<int> expected{5, 4, 3, 2, 1, 0};
        CHECK(order == expected);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, visit_in_order_const)
    {
      const std::vector<int> ids{0, 1, 2, 3, 4, 5};
      InitialDataNDC         data;
      for (const auto idx : ids)
      {
        data.emplace_back(idx, idx);
      }
      const DataNDC0 data0(data.begin(), data.end(), ItemNDCNode::compare::cmp);

      // In-order, LNR
      {
        std::vector<int> order;
        data0.visit_in_order(false, [&order](const ItemNDCNode& item) { order.push_back(item.data.value); });
        const std::vector<int> expected{0, 1, 2, 3, 4, 5};
        CHECK(order == expected);
      }

      // Reverse In-order, RNL
      {
        std::vector<int> order;
        data0.visit_in_order(true, [&order](const ItemNDCNode& item) { order.push_back(item.data.value); });
        const std::vector<int> expected{5, 4, 3, 2, 1, 0};
        CHECK(order == expected);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, visit_post_order)
    {
      const std::vector<int> ids{0, 1, 2, 3, 4, 5};
      InitialDataNDC         data;
      for (const auto idx : ids)
      {
        data.emplace_back(idx, idx);
      }
      DataNDC0 data0(data.begin(), data.end(), ItemNDCNode::compare::cmp);

      // Post-order, LRN
      {
        std::vector<int> order;
        data0.visit_post_order(false, [&order](ItemNDCNode& item) { order.push_back(item.data.value); });
        const std::vector<int> expected{0, 2, 1, 5, 4, 3};
        CHECK(order == expected);
      }

      // Reverse post-order, RLN
      {
        std::vector<int> order;
        data0.visit_post_order(true, [&order](ItemNDCNode& item) { order.push_back(item.data.value); });
        const std::vector<int> expected{5, 4, 2, 0, 1, 3};
        CHECK(order == expected);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, visit_post_order_const)
    {
      const std::vector<int> ids{0, 1, 2, 3, 4, 5};
      InitialDataNDC         data;
      for (const auto idx : ids)
      {
        data.emplace_back(idx, idx);
      }
      const DataNDC0 data0(data.begin(), data.end(), ItemNDCNode::compare::cmp);

      // Post-order, LRN
      {
        std::vector<int> order;
        data0.visit_post_order(false, [&order](const ItemNDCNode& item) { order.push_back(item.data.value); });
        const std::vector<int> expected{0, 2, 1, 5, 4, 3};
        CHECK(order == expected);
      }

      // Reverse post-order, RLN
      {
        std::vector<int> order;
        data0.visit_post_order(true, [&order](const ItemNDCNode& item) { order.push_back(item.data.value); });
        const std::vector<int> expected{5, 4, 2, 0, 1, 3};
        CHECK(order == expected);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, visit_pre_order)
    {
      const std::vector<int> ids{0, 1, 2, 3, 4, 5};
      InitialDataNDC         data;
      for (const auto idx : ids)
      {
        data.emplace_back(idx, idx);
      }
      DataNDC0 data0(data.begin(), data.end(), ItemNDCNode::compare::cmp);

      // Pre-order, NLR
      {
        std::vector<int> order;
        data0.visit_pre_order(false, [&order](ItemNDCNode& item) { order.push_back(item.data.value); });
        const std::vector<int> expected{3, 1, 0, 2, 4, 5};
        CHECK(order == expected);
      }

      // Reverse pre-order, NRL
      {
        std::vector<int> order;
        data0.visit_pre_order(true, [&order](ItemNDCNode& item) { order.push_back(item.data.value); });
        const std::vector<int> expected{3, 4, 5, 1, 2, 0};
        CHECK(order == expected);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, visit_pre_order_const)
    {
      const std::vector<int> ids{0, 1, 2, 3, 4, 5};
      InitialDataNDC         data;
      for (const auto idx : ids)
      {
        data.emplace_back(idx, idx);
      }
      const DataNDC0 data0(data.begin(), data.end(), ItemNDCNode::compare::cmp);

      // Pre-order, NLR
      {
        std::vector<int> order;
        data0.visit_pre_order(false, [&order](const ItemNDCNode& item) { order.push_back(item.data.value); });
        const std::vector<int> expected{3, 1, 0, 2, 4, 5};
        CHECK(order == expected);
      }

      // Reverse pre-order, NRL
      {
        std::vector<int> order;
        data0.visit_pre_order(true, [&order](const ItemNDCNode& item) { order.push_back(item.data.value); });
        const std::vector<int> expected{3, 4, 5, 1, 2, 0};
        CHECK(order == expected);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_lower_upper_bound)
    {
      // idx     1  5   0   2   4   6   8   9   7   3
      // vals = [7, 7, 10, 10, 10, 10, 10, 10, 12, 15]
      InitialDataNDC duplicate_data;
      duplicate_data.emplace_back(10, 0);
      duplicate_data.emplace_back(7, 1);
      duplicate_data.emplace_back(10, 2);
      duplicate_data.emplace_back(15, 3);
      duplicate_data.emplace_back(10, 4);
      duplicate_data.emplace_back(7, 5);
      duplicate_data.emplace_back(10, 6);
      duplicate_data.emplace_back(12, 7);
      duplicate_data.emplace_back(10, 8);
      duplicate_data.emplace_back(10, 9);

      DataNDC0 data0(duplicate_data.begin(), duplicate_data.end(), ItemNDCNode::compare_append_dups);
      verify_tree(data0);

      CHECK(data0.lower_bound(ItemNDCNode::always_greater) == data0.end());
      CHECK(data0.upper_bound(ItemNDCNode::always_greater) == data0.end());
      CHECK_EQUAL(data0.lower_bound(ItemNDCNode::always_less)->data.index, 1); // 7(1)
      CHECK_EQUAL(data0.upper_bound(ItemNDCNode::always_less)->data.index, 1); // 7(1)

      // 0
      CHECK_EQUAL(data0.lower_bound(ItemNDCNode::CompareByValue(0))->data.index, 1); // 7(1)
      CHECK_EQUAL(data0.upper_bound(ItemNDCNode::CompareByValue(0))->data.index, 1); // 7(1)
      // 7
      CHECK_EQUAL(data0.lower_bound(ItemNDCNode::CompareByValue(7))->data.index, 1); // 7(1)
      CHECK_EQUAL(data0.upper_bound(ItemNDCNode::CompareByValue(7))->data.index, 0); // 10(0)
      // 8
      CHECK_EQUAL(data0.lower_bound(ItemNDCNode::CompareByValue(8))->data.index, 0); // 10(0)
      CHECK_EQUAL(data0.upper_bound(ItemNDCNode::CompareByValue(8))->data.index, 0); // 10(0)
      // 10
      const auto lower10 = data0.lower_bound(ItemNDCNode::CompareByValue(10));
      const auto upper10 = data0.upper_bound(ItemNDCNode::CompareByValue(10));
      CHECK_EQUAL(lower10->data.value, 10); // 10(0)
      CHECK_EQUAL(lower10->data.index, 0);
      CHECK_EQUAL(upper10->data.value, 12); // 12(7)
      CHECK_EQUAL(upper10->data.index, 7);
      CHECK_EQUAL(std::distance(lower10, upper10), 6); // duplicates of 10s
      // 11
      CHECK_EQUAL(data0.lower_bound(ItemNDCNode::CompareByValue(11))->data.index, 7); // 12(7)
      CHECK_EQUAL(data0.upper_bound(ItemNDCNode::CompareByValue(11))->data.index, 7); // 12(7)
      // 12
      CHECK_EQUAL(data0.lower_bound(ItemNDCNode::CompareByValue(12))->data.index, 7); // 12(7)
      CHECK_EQUAL(data0.upper_bound(ItemNDCNode::CompareByValue(12))->data.index, 3); // 15(3)
      // 13
      CHECK_EQUAL(data0.lower_bound(ItemNDCNode::CompareByValue(13))->data.index, 3); // 15(3)
      CHECK_EQUAL(data0.upper_bound(ItemNDCNode::CompareByValue(13))->data.index, 3); // 15(3)
      // 15
      CHECK_EQUAL(data0.lower_bound(ItemNDCNode::CompareByValue(15))->data.index, 3); // 15(3)
      CHECK(data0.upper_bound(ItemNDCNode::CompareByValue(15)) == data0.end());
      // 16
      CHECK(data0.lower_bound(ItemNDCNode::CompareByValue(16)) == data0.end());
      CHECK(data0.upper_bound(ItemNDCNode::CompareByValue(16)) == data0.end());

      // Make sure that `lower_bound` and `upper_bound` work on const tree.
      const DataNDC0& const_data0 = data0;
      CHECK_EQUAL(const_data0.lower_bound(ItemNDCNode::CompareByValue(12))->data.index, 7); // 12(7)
      CHECK_EQUAL(const_data0.upper_bound(ItemNDCNode::CompareByValue(12))->data.index, 3); // 15(3)
    }
  }

} // namespace
