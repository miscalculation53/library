#pragma once

#include "../tree/add_virtual_root.hpp"

/**
 * @brief laminar matroid
 * @docs docs/matroid/laminar_matroid.md
 */
class LaminarMatroid
{
  RootedTree tree;
  vc<int> capacity, group, count, independent, parent;
  vc<bool> selected;

public:
  LaminarMatroid(vc<int> parent, vc<int> capacity, vc<int> group)
      : capacity(move(capacity)), group(move(group)), selected(this->group.size())
  {
    const int k = parent.size();
    assert(int(this->capacity.size()) == k);
    for (int v = 0; v < k; v++)
    {
      assert(this->capacity[v] >= 0);
      const int p = parent[v];
      assert(-1 <= p && p < k && p != v);
    }
    tree = add_virtual_root(k, parent);
    this->parent = tree.parents();
    for (int g : this->group) assert(0 <= g && g < k);
    build({});
  }

  int size() const { return group.size(); }

  void build(const vc<int> &s)
  {
    independent = s;
    fill(ALL(selected), false);
    const int root = tree.root();
    count.assign(capacity.size(), 0);
    for (int e : s)
    {
      assert(0 <= e && e < size() && !selected[e]);
      selected[e] = true;
      count[group[e]]++;
    }
    const auto &order = tree.top_down_vertices();
    for (auto it = order.rbegin(); it != order.rend(); ++it) if (*it != root)
    {
      const int v = *it;
      assert(count[v] <= capacity[v]);
      const int p = parent[v];
      if (p != root) count[p] += count[v];
    }
  }

  optional<vc<int>> circuit(int e) const
  {
    assert(0 <= e && e < size() && !selected[e]);
    for (int v = group[e]; v != tree.root(); v = parent[v]) if (count[v] == capacity[v])
    {
      vc<int> result;
      for (int f : independent)
        if (tree.is_ancestor(v, group[f])) result.push_back(f);
      return result;
    }
    return nullopt;
  }
};
