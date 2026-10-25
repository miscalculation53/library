#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../../algebra/algebra_basic_ops.hpp"
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
  static const int B = 8 * sizeof(Word);
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
    const int li = l / B, ri = r / B;
    if (li == ri)
    {
      const Word lm = (Word(1) << (l % B)) - 1;
      const Word rm = (Word(1) << (r % B)) - 1;
      return popcount(dat[li] & rm & ~lm);
    }
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

  // sum[0, r) < w となる最大の r (なければ nullopt)
  template <class I = ll>
  inline optional<I> lt_max(T w) const
  {
    if (w <= 0)
      return nullopt;
    if (w > sum(n))
      return n;
    const auto [i, v] = *fw.lt_max_id_sum(w);
    I res = B * i + kth_bit_pos(dat[i], w - v - 1);
    return res;
  }
  // sum[0, r) >= w となる最小の r (なければ nullopt)
  template <class I = ll>
  optional<I> geq_min(T w) const
  {
    auto r = lt_max<I>(w);
    I index = r ? *r + 1 : 0;
    return index > n ? nullopt : optional<I>(index);
  }
  // sum[0, r) <= w となる最大の r (なければ nullopt)
  template <class I = ll>
  inline optional<I> leq_max(T w) const { return lt_max<I>(w + 1); }
  // sum[0, r) > w となる最小の r (なければ nullopt)
  template <class I = ll>
  inline optional<I> gt_min(T w) const { return geq_min<I>(w + 1); }

  // 要素が [0, size()) の集合を管理するのに使ったとき、集合のサイズを返す
  inline T size_of_set() const { return sum(n); }

  // 要素が [0, size()) の集合を管理するのに使ったとき、値 x は (あれば) 何番目か
  template <class I = ll>
  inline I order_in_set(int x) const { return sum(x); }
  // 要素が [0, size()) の集合を管理するのに使ったとき、k 番目の値
  // 範囲外の k は nullopt
  template <class I = ll>
  inline optional<I> kth_in_set(int k) const
  {
    if (k < 0) return nullopt;
    auto key = leq_max<I>(k);
    return key && *key < n ? key : nullopt;
  }
  // 要素が [0, size()) の集合を管理するのに使ったとき、x 未満で最大の要素が**何番目か** (なければ nullopt)
  template <class I = ll>
  inline optional<I> lt_max_in_set(T x) const
  {
    if (x < 0)
      return nullopt;
    T count = sum(min(x, T(n)));
    return count == 0 ? nullopt : optional<I>(I(count - 1));
  }
  // 要素が [0, size()) の集合を管理するのに使ったとき、x 以下で最大の要素が**何番目か** (なければ nullopt)
  template <class I = ll>
  inline optional<I> leq_max_in_set(T x) const { return lt_max_in_set<I>(x + 1); }
  // 要素が [0, size()) の集合を管理するのに使ったとき、x 以上で最小の要素が**何番目か** (なければ nullopt)
  template <class I = ll>
  inline optional<I> geq_min_in_set(T x) const
  {
    T rank = sum(clamp(x, T(0), T(n)));
    return rank == size_of_set() ? nullopt : optional<I>(I(rank));
  }
  // 要素が [0, size()) の集合を管理するのに使ったとき、x 超過で最小の要素が**何番目か** (なければ nullopt)
  template <class I = ll>
  inline optional<I> gt_min_in_set(T x) const { return geq_min_in_set<I>(x + 1); }

  string content() const
  {
    string res;
    repi(i, n) res += get(i) + '0';
    return res;
  }
};
