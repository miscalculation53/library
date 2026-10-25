#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../../algebra/algebra_basic_ops.hpp"

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
    // 両端の prefix に共通する節点までで打ち切る。
    S right = G::e(), left = G::e();
    while (l < r)
    {
      right = G::op(right, dat[r]);
      r -= r & -r;
    }
    while (r < l)
    {
      left = G::op(left, dat[l]);
      l -= l & -l;
    }
    return G::op(G::inv(left), right);
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

private:
  pair<int, S> prefix_lt_raw(S w) const
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

public:
  // 非負の通常の整数和を想定する。該当する接頭辞がなければ nullopt。
  template <class I = ll>
  optional<pair<I, S>> lt_max_id_sum(S w) const
  {
    auto result = prefix_lt_raw(w);
    return result.first < 0 ? nullopt : optional<pair<I, S>>(pair<I, S>{I(result.first), std::move(result.second)});
  }
  template <class I = ll>
  optional<I> lt_max(S w) const
  {
    int r = prefix_lt_raw(w).first;
    return r < 0 ? nullopt : optional<I>(r);
  }
  template <class I = ll>
  optional<I> geq_min(S w) const
  {
    int r = prefix_lt_raw(w).first + 1;
    return r > n ? nullopt : optional<I>(r);
  }
  template <class I = ll>
  optional<I> leq_max(S w) const { return lt_max<I>(w + 1); }
  template <class I = ll>
  optional<I> gt_min(S w) const { return geq_min<I>(w + 1); }

  // 要素が [0, size()) の多重集合を管理するのに使ったとき、多重集合のサイズを返す
  inline S size_of_multiset() const { return sum(n); }

  // 要素が [0, size()) の多重集合を管理するのに使ったとき、値 x は何番目から何番目か [l, r)
  inline pair<S, S> order_in_multiset(int x) const { return {sum(x), sum(x + 1)}; }
  // 要素が [0, size()) の多重集合を管理するのに使ったとき、k 番目の値
  // 範囲外の k は nullopt
  template <class I = ll>
  inline optional<I> kth_in_multiset(S k) const
  {
    if (k < 0) return nullopt;
    auto key = leq_max<I>(k);
    return key && *key < n ? key : nullopt;
  }
  // 要素が [0, size()) の多重集合を管理するのに使ったとき、x 未満で最大の要素が**何番目か** (なければ nullopt)
  template <class T, class I = ll>
  inline optional<I> lt_max_in_multiset(T x) const
  {
    S count = sum(clamp(x, T(0), T(n)));
    return count == 0 ? nullopt : optional<I>(I(count - 1));
  }
  // 要素が [0, size()) の多重集合を管理するのに使ったとき、x 以下で最大の要素が**何番目か** (なければ nullopt)
  template <class T, class I = ll>
  inline optional<I> leq_max_in_multiset(T x) const { return lt_max_in_multiset<T, I>(x + 1); }
  // 要素が [0, size()) の多重集合を管理するのに使ったとき、x 以上で最小の要素が**何番目か** (なければ nullopt)
  template <class T, class I = ll>
  inline optional<I> geq_min_in_multiset(T x) const
  {
    S rank = sum(clamp(x, T(0), T(n)));
    return rank == size_of_multiset() ? nullopt : optional<I>(I(rank));
  }
  // 要素が [0, size()) の多重集合を管理するのに使ったとき、x 超過で最小の要素が**何番目か** (なければ nullopt)
  template <class T, class I = ll>
  inline optional<I> gt_min_in_multiset(T x) const { return geq_min_in_multiset<T, I>(x + 1); }

  vc<S> content() const
  {
    vc<S> res(dat.begin() + 1, dat.end());
    // 線形構築で親へ加えた値を、逆順に取り除く。
    for (int i = n; i > 0; i--)
    {
      const int p = i + (i & -i);
      if (p <= n) res[p - 1] = G::op(res[p - 1], G::inv(res[i - 1]));
    }
    return res;
  }
};
