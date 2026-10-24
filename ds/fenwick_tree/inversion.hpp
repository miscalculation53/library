#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "fenwick_tree_01.hpp"
#include "../../algo/stable_argsort.hpp"

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
  auto p = stable_argsort(v);
  FenwickTree01 fw(n);
  ll res = 0;
  int processed = 0;
  fe(i : p)
  {
    res += processed - fw.sum(i + 1);
    fw.set(i, 1);
    processed++;
  }
  return res;
}

template <class T = ll>
struct InversionSlider
{
  int n = 0, l = 0, r = 0;
  ll inversion_num = 0;
  vc<int> vec;
  FenwickTree01<> fw{0};

  InversionSlider() = default;
  InversionSlider(const vc<T> &a) : n(a.size()), vec(n), fw(n)
  {
    auto p = stable_argsort(a);
    repi(i, n) vec[p[i]] = i;
  }

  void lpp()
  {
    assert(l < r);
    int a = vec[l];
    inversion_num -= fw.sum(a);
    fw.set(a, 0);
    l++;
  }
  void rpp()
  {
    assert(r < n);
    int a = vec[r];
    inversion_num += (r - l) - fw.sum(a + 1);
    fw.set(a, 1);
    r++;
  }
  void lmm()
  {
    assert(0 < l);
    l--;
    int a = vec[l];
    fw.set(a, 1);
    inversion_num += fw.sum(a);
  }
  void rmm()
  {
    assert(l < r);
    r--;
    int a = vec[r];
    fw.set(a, 0);
    inversion_num -= (r - l) - fw.sum(a + 1);
  }

  void set(int nl, int nr)
  {
    assert(0 <= nl && nl <= nr && nr <= n);
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
