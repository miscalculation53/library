#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../../math/algebra/algebra_basic_ops.hpp"

/**
 * @brief 双対セグメント木
 * @docs docs/ds/segtree/dual_segtree.md
 */

template <class A>
struct DualSegmentTree
{
  using S = typename A::S;
  using F = typename A::F;

private:
  int n, siz, lg;
  vc<S> dat;
  vc<F> lazy;
  void all_apply(int i, const F &f)
  {
    if (0 <= i - siz && i - siz < n)
      dat[i - siz] = A::mapping(f, dat[i - siz]);
    if (i < siz)
      lazy[i] = A::composition(f, lazy[i]);
  }
  void push(int i)
  {
    all_apply(2 * i, lazy[i]);
    all_apply(2 * i + 1, lazy[i]);
    lazy[i] = A::id();
  }

public:
  DualSegmentTree() {}
  DualSegmentTree(int n) : DualSegmentTree(vc<S>(n, A::e())) {}
  template <class Iter>
  DualSegmentTree(const Iter &bg, const Iter &ed) : DualSegmentTree(vc<S>(bg, ed)) {}
  template <class T>
  DualSegmentTree(const vc<T> &vec)
  : n(vec.size()), siz(bit_ceil(vec.size())), lg(countr_zero(siz)),
  dat(vec), lazy(siz, A::id()) {}

  void set(int p, const S &x)
  {
    assert(0 <= p && p < n);
    p += siz;
    repi(i, lg, 0, -1) push(p >> i);
    dat[p - siz] = x;
  }
  S get(int p)
  {
    assert(0 <= p && p < n);
    p += siz;
    repi(i, lg, 0, -1) push(p >> i);
    return dat[p - siz];
  }
  void apply(int p, const F &f)
  {
    assert(0 <= p && p < n);
    p += siz;
    repi(i, lg, 0, -1) push(p >> i);
    dat[p - siz] = A::mapping(f, dat[p - siz]);
  }
  void apply(int l, int r, const F &f)
  {
    assert(0 <= l && l <= r && r <= n);
    if (l == r)
      return;
    l += siz, r += siz;
    repi(i, lg, 0, -1)
    {
      if (((l >> i) << i) != l) push(l >> i);
      if (((r >> i) << i) != r) push((r - 1) >> i);
    }
    while (l < r)
    {
      if (l & 1) all_apply(l++, f);
      if (r & 1) all_apply(--r, f);
      l >>= 1, r >>= 1;
    }
  }

  vc<S> content()
  {
    vc<S> res(n);
    repi(i, n) res[i] = get(i);
    return res;
  }
};

template <class M>
struct ActCompoundAssignment
{
  using S = typename M::S;
  using F = typename M::S;
  static constexpr S mapping(const F &f, const S &x) { return M::op(f, x); }
  static constexpr F composition(const F &f, const F &g) { return M::op(f, g); }
  static constexpr F id() { return M::e(); }
};
