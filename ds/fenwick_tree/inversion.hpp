#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "fenwick_tree_01.hpp"

/**
 * @brief 転倒数
 * @docs docs/ds/fenwick_tree/inversion.md
 */

// i < j かつ v[i] > v[j] を満たす (i, j) の個数
// 座圧していなくてもよい
template <class V>
ll inversion_number(const V &v)
{
  const int n = v.size();
  vc<int> p = permid<int>(n);
  stable_sort(ALL(p), [&](int i, int j)
       { return v[i] < v[j]; });
  FenwickTree01 fw(n);
  ll res = 0;
  fe(i : p)
  {
    ll tmp = fw.sum(i + 1, n);
    res += tmp;
    fw.set(i, 1);
  }
  return res;
}
