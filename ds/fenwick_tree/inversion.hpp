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

template <class I>
struct InversionSlider
{
  int n, l, r;
  ll inversion_num;
  vc<I> vec;
  FenwickTree01<> fw;

  InversionSlider() {}
  InversionSlider(const vc<I> &vec)
  : n(vec.size()), l(0), r(0), inversion_num(0), vec(vec), fw(vec.size())
  { assert(is_permutation(vec)); }

  void lpp()
  {
    int a = vec[l];
    inversion_num -= fw.sum(0, a);
    fw.set(a, 0);
    l++;
  }
  void rpp()
  {
    int a = vec[r];
    inversion_num += fw.sum(a + 1, n);
    fw.set(a, 1);
    r++;
  }
  void lmm()
  {
    l--;
    int a = vec[l];
    fw.set(a, 1);
    inversion_num += fw.sum(0, a);
  }
  void rmm()
  {
    r--;
    int a = vec[r];
    fw.set(a, 0);
    inversion_num -= fw.sum(a + 1, n);
  }

  void set(int nl, int nr)
  {
    while (nl < l)
      lmm();
    while (r < nr)
      rpp();
    while (l < nl)
      lpp();
    while (nr < r)
      rmm();
  }
};
