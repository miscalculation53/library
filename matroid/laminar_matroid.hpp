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
      assert(-1 <= p && p < k);
    }
    tree = add_virtual_root(k, parent);
    this->parent = tree.parents();
    for (int g : this->group) assert(0 <= g && g < k);
    build({});
  }

  template <class P = int, class C = int, class G = int>
  LaminarMatroid(const vc<P> &parent, const vc<C> &capacity, const vc<G> &group)
      : LaminarMatroid(gen_vec(parent.size(), [&](int v)
        {
          const auto p = parent[v];
          if constexpr (is_signed_ext<P>) if (p == -1) return -1;
          assert(u128(p) < parent.size());
          return int(p);
        }), gen_vec(capacity.size(), [&](int v)
        {
          if constexpr (is_signed_ext<C>) assert(capacity[v] >= 0);
          return u128(capacity[v]) < group.size() ? int(capacity[v]) : int(group.size());
        }), gen_vec(group.size(), [&](int e)
        {
          assert(u128(group[e]) < parent.size());
          return int(group[e]);
        })) {}

  int size() const { return group.size(); }

  template <class I = int>
  void build(const vc<I> &s)
  {
    independent.assign(ALL(s));
    fill(ALL(selected), false);
    const int root = tree.root();
    count.assign(capacity.size(), 0);
    for (auto value : s)
    {
      assert(u128(value) < unsigned(size()));
      const int e = int(value);
      assert(!selected[e]);
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
