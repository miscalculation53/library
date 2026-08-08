#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "fenwick_tree.hpp"

/**
 * @brief 区間加算一点取得
 * @docs docs/ds/fenwick_tree/range_add_point_get.md
 */

template <class G>
struct RangeAddPointGet
{
  using S = typename G::S;

private:
  int n;
  FenwickTree<G> fw;

public:
  RangeAddPointGet() {}
  RangeAddPointGet(int n) : n(n), fw(n + 1) {}
  RangeAddPointGet(const vc<S> &v) : RangeAddPointGet(v.size())
  {
    repi(i, n) add(i, v[i]);
  }

  void add(int l, int r, S x)
  {
    assert(0 <= l && l <= r && r <= n);
    fw.add(l, x);
    fw.add(r, G::inv(x));
  }
  void add(int p, S x) { add(p, p + 1, x); }
  void set(int p, S x) { add(p, p + 1, G::op(G::inv(get(p)), x)); }
  S get(int p)
  {
    assert(0 <= p && p < n);
    return fw.sum(0, p + 1);
  }

  vc<S> content()
  {
    vc<S> res(n);
    repi(i, n) res[i] = get(i);
    return res;
  }
};
