#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief 独立性判定からのマトロイド oracle
 * @docs docs/matroid/independence_matroid.md
 */

template <class Independent>
struct IndependenceMatroid
{
private:
  int n;
  Independent independent;
  vc<int> selected;
  vc<bool> used;

public:
  IndependenceMatroid(int n, Independent independent)
      : n(n), independent(move(independent)), used(n)
  { assert(n >= 0); }

  int size() const { return n; }

  template <class I = int>
  void build(const vc<I> &elements)
  {
    fill(ALL(used), false);
    for (auto value : elements)
    {
      assert(u128(value) < unsigned(n));
      const int e = int(value);
      assert(!used[e]);
      used[e] = true;
    }
    selected.assign(ALL(elements));
    assert(independent(as_const(selected)));
  }

  optional<vc<int>> circuit(int e) const
  {
    assert(0 <= e && e < n && !used[e]);
    auto candidate = selected;
    candidate.eb(e);
    if (independent(candidate)) return nullopt;
    vc<int> result;
    for (int i = 0; i < int(selected.size()); i++)
    {
      candidate[i] = e, candidate.pop_back();
      if (independent(candidate)) result.eb(selected[i]);
      candidate[i] = selected[i], candidate.eb(e);
    }
    return result;
  }
};
