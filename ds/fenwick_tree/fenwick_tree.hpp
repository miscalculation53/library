#pragma once

#include "../template/template_all_but_modint.hpp"

#include "../math/algebra/algebra_basic_ops.hpp"

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
    repi(i, n) add(i, v[i]);
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
  // sum[0, r) >= w となる最小の r (なければ n)
  template <class I = ll>
  I geq_min(S w) const
  {
    int k = bit_ceil(n);
    int x = 0;
    while (k > 0)
    {
      if (x + k - 1 < n && dat[x + k] < w)
      {
        w = G::op(w, G::inv(dat[x + k]));
        x += k;
      }
      k >>= 1;
    }
    return x;
  }
  // 整数の普通の足し算で、要素が非負のとき
  // sum[0, r) < w となる最大の r (なければ -1)
  template <class I = ll>
  inline I lt_max(S w) const { return geq_min<I>(w) - 1; }
  // 整数の普通の足し算で、要素が非負のとき
  // sum[0, r) > w となる最小の r (なければ n)
  template <class I = ll>
  inline I gt_min(S w) const { return geq_min<I>(w + 1); }
  // 整数の普通の足し算で、要素が非負のとき
  // sum[0, r) <= w となる最大の r (なければ -1)
  template <class I = ll>
  inline I leq_max(S w) const { return gt_min<I>(w) - 1; }

  // 要素が [0, n) の多重集合を管理するのに使ったとき、k 番目の値 (なければ n)
  template <class I = ll>
  inline I kth_of_multiset(S k) const { return gt_min<I>(k); }

  vc<S> content() const
  {
    vc<S> res(n);
    repi(i, n) res[i] = get(i);
    return res;
  }
};
