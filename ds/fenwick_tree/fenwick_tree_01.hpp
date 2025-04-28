#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../../math/algebra/algebra_basic_ops.hpp"
#include "fenwick_tree.hpp"
#include "../../bit/kth_bit.hpp"

/**
 * @brief 01 列に対する Fenwick Tree
 * @docs docs/ds/fenwick_tree/fenwick_tree_01.md
 */

template <class T = ll, class Word = uint64_t>
struct FenwickTree01
{
private:
  const int B = 8 * sizeof(Word);
  int n;
  vc<Word> dat;
  FenwickTree<GroupAddSub<T>> fw;

public:
  FenwickTree01() {}
  FenwickTree01(int n) : n(n), dat(n / B + 1), fw(n / B + 1) {}
  template <class U>
  FenwickTree01(const vc<U> &v) : n(v.size())
  {
    dat.resize(n / B + 1);
    repi(i, n)
    {
      assert(v[i] == T(0) || v[i] == T(1));
      bset(dat[i / B], i % B, v[i]);
    }
    vc<T> vec(dat.size());
    repi(i, n / B + 1) vec[i] = popcount(dat[i]);
    fw = decltype(fw)(vec);
  }

  template <class I = ll>
  I size() const { return n; }

  // [0, r)
  T sum(int r) const
  {
    assert(0 <= r && r <= n);
    int res = fw.sum(r / B);
    res += popcount(dat[r / B] & ((Word(1) << (r % B)) - 1));
    return res;
  }
  // [l, r)
  T sum(int l, int r) const
  {
    assert(0 <= l && l <= r && r <= n);
    return sum(r) - sum(l);
  }
  bool get(int i) const
  {
    assert(0 <= i && i < n);
    return btest(dat[i / B], i % B);
  }

  void set(int i, bool b)
  {
    assert(0 <= i && i < n);
    if (btest(dat[i / B], i % B) == b)
      return;
    bset(dat[i / B], i % B, b);
    fw.add(i / B, b ? 1 : -1);
  }

  // sum[0, r) < w となる最大の r (なければ -1)
  template <class I = ll>
  inline I lt_max(T w) const
  {
    if (w <= 0)
      return -1;
    if (w > sum(n))
      return n;
    const auto [i, v] = fw.lt_max_id_sum(w);
    I res = B * i + kth_bit_pos(dat[i], w - v - 1);
    return res;
  }
  // sum[0, r) >= w となる最小の r (なければ size())
  template <class I = ll>
  I geq_min(T w) const { return lt_max<I>(w) + 1; }
  // sum[0, r) <= w となる最大の r (なければ -1)
  template <class I = ll>
  inline I leq_max(T w) const { return lt_max<I>(w + 1); }
  // sum[0, r) > w となる最小の r (なければ size())
  template <class I = ll>
  inline I gt_min(T w) const { return geq_min<I>(w + 1); }

  // 要素が [0, size()) の集合を管理するのに使ったとき、集合のサイズを返す
  inline T size_of_set() const { return sum(n); }

  // 要素が [0, size()) の集合を管理するのに使ったとき、値 x は (あれば) 何番目か
  template <class I = ll>
  inline I order_in_set(int x) const { return sum(x); }
  // 要素が [0, size()) の集合を管理するのに使ったとき、k 番目の値
  // ただし、k < 0 なら -1, k >= size_of_set() なら size()
  template <class I = ll>
  inline I kth_in_set(int k) const
  {
    if (k < 0)
      return -1;
    return leq_max<I>(k);
  }
  // 要素が [0, size()) の集合を管理するのに使ったとき、x 未満で最大の要素が**何番目か** (なければ -1)
  template <class I = ll>
  inline I lt_max_in_set(T x) const
  {
    if (x < 0)
      return -1;
    return sum(min(x, T(n))) - 1;
  }
  // 要素が [0, size()) の集合を管理するのに使ったとき、x 以下で最大の要素が**何番目か** (なければ -1)
  template <class I = ll>
  inline I leq_max_in_set(T x) const { return lt_max_in_set<I>(x + 1); }
  // 要素が [0, size()) の集合を管理するのに使ったとき、x 以上で最小の要素が**何番目か** (なければ size())
  template <class I = ll>
  inline I geq_min_in_set(T x) const
  {
    if (x > n)
      return sum(n);
    return sum(max(T(0), x));
  }
  // 要素が [0, size()) の集合を管理するのに使ったとき、x 超過で最小の要素が**何番目か** (なければ size())
  template <class I = ll>
  inline I gt_min_in_set(T x) const { return geq_min_in_set<I>(x + 1); }

  string content() const
  {
    string res;
    repi(i, n) res += get(i) + '0';
    return res;
  }
};
