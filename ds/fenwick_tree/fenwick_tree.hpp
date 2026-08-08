#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../../math/algebra/algebra_basic_ops.hpp"

/**
 * @brief Fenwick Tree
 * @docs docs/ds/fenwick_tree/fenwick_tree.md
 */

// G は可換群 (prefix だけなどであれば可換モノイドでも OK)
template <class G>
struct FenwickTree
{
  using S = typename G::S;

private:
  int n;
  vc<S> dat;

public:
  FenwickTree() {}
  FenwickTree(int n) : n(n), dat(n + 1, G::e()) {}
  FenwickTree(const vc<S> &v) : FenwickTree(v.size())
  {
    repi(i, n) dat[i + 1] = v[i];
    repi(i, 1, n + 1)
    {
      int p = i + (i & -i);
      if (p <= n)
        dat[p] = G::op(dat[p], dat[i]);
    }
  }

  template <class I = ll>
  I size() const { return n; }

  // [0, r)
  // 逆元は必要ない
  S sum(int r) const
  {
    assert(0 <= r && r <= n);
    S s = G::e();
    while (r > 0)
    {
      s = G::op(s, dat[r]);
      r -= r & -r;
    }
    return s;
  }
  // [l, r)
  // 逆元が必要
  S sum(int l, int r) const
  {
    assert(0 <= l && l <= r && r <= n);
    return G::op(G::inv(sum(l)), sum(r));
  }
  // 逆元が必要
  S get(int i) const
  {
    assert(0 <= i && i < n);
    return sum(i, i + 1);
  }

  // 逆元は必要ない
  void add(int i, S x)
  {
    assert(0 <= i && i < n);
    i++;
    while (i <= n)
    {
      dat[i] = G::op(dat[i], x);
      i += i & -i;
    }
  }
  // 逆元が必要
  void set(int i, S x) { add(i, G::op(G::inv(get(i)), x)); }

  // 整数の普通の足し算で、要素が非負のとき
  // sum[0, r) < w となる最小の r (なければ -1)
  // と、その r に対する sum[0, r) のペア
  template <class I = ll>
  pair<I, S> lt_max_id_sum(S w) const
  {
    if (w <= G::e())
      return {-1, G::e()};
    int k = bit_floor(n);
    int x = 0;
    S v = G::e();
    while (k > 0)
    {
      if (x + k <= n)
      {
        S nv = G::op(v, dat[x + k]);
        if (nv < w)
          v = nv, x += k;
      }
      k >>= 1;
    }
    return {x, v};
  }

  // 整数の普通の足し算で、要素が非負のとき
  // sum[0, r) < w となる最小の r (なければ -1)
  template <class I = ll>
  I lt_max(S w) const { return lt_max_id_sum<I>(w).first; }
  // 整数の普通の足し算で、要素が非負のとき
  // sum[0, r) >= w となる最大の r (なければ n+1)
  template <class I = ll>
  inline I geq_min(S w) const { return lt_max<I>(w) + 1; }
  // 整数の普通の足し算で、要素が非負のとき
  // sum[0, r) <= w となる最大の r (なければ -1)
  template <class I = ll>
  inline I leq_max(S w) const { return lt_max<I>(w + 1); }
  // 整数の普通の足し算で、要素が非負のとき
  // sum[0, r) > w となる最小の r (なければ n+1)
  template <class I = ll>
  inline I gt_min(S w) const { return geq_min<I>(w + 1); }

  // 要素が [0, size()) の多重集合を管理するのに使ったとき、多重集合のサイズを返す
  inline S size_of_multiset() const { return sum(n); }

  // 要素が [0, size()) の多重集合を管理するのに使ったとき、値 x は何番目から何番目か [l, r)
  inline pair<S, S> order_in_multiset(int x) const { return {sum(x), sum(x + 1)}; }
  // 要素が [0, size()) の多重集合を管理するのに使ったとき、k 番目の値
  // ただし、k < 0 なら -1, k >= size_of_multiset() なら size()
  template <class I = ll>
  inline I kth_in_multiset(S k) const
  {
    if (k < 0)
      return -1;
    return leq_max<I>(k);
  }
  // 要素が [0, size()) の多重集合を管理するのに使ったとき、x 未満で最大の要素が**何番目か** (なければ -1)
  template <class T, class I = ll>
  inline I lt_max_in_multiset(T x) const
  {
    return sum(clamp(x, T(0), T(n))) - 1;
  }
  // 要素が [0, size()) の多重集合を管理するのに使ったとき、x 以下で最大の要素が**何番目か** (なければ -1)
  template <class T, class I = ll>
  inline I leq_max_in_multiset(T x) const { return lt_max_in_multiset<T, I>(x + 1); }
  // 要素が [0, size()) の多重集合を管理するのに使ったとき、x 以上で最小の要素が**何番目か** (なければ size())
  template <class T, class I = ll>
  inline I geq_min_in_multiset(T x) const
  {
    return sum(clamp(x, T(0), T(n)));
  }
  // 要素が [0, size()) の多重集合を管理するのに使ったとき、x 超過で最小の要素が**何番目か** (なければ size())
  template <class T, class I = ll>
  inline I gt_min_in_multiset(T x) const { return geq_min_in_multiset<T, I>(x + 1); }

  vc<S> content() const
  {
    vc<S> res(n);
    repi(i, n) res[i] = get(i);
    return res;
  }
};
