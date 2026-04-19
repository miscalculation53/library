#pragma once

#include "../../template/template_all_but_modint.hpp"

/**
 * @brief 遅延セグメント木
 * @docs docs/ds/segtree/lazy_segtree.md
 */

template <class AM>
struct LazySegmentTree
{
  using S = typename AM::S;
  using F = typename AM::F;

private:
  int n, siz, lg;
  vc<S> dat;
  vc<F> lazy;
  void update(int i) { dat[i] = AM::op(dat[2 * i], dat[2 * i + 1]); }
  void all_apply(int i, const F &f)
  {
    dat[i] = AM::mapping(f, dat[i]);
    if (i < siz)
      lazy[i] = AM::composition(f, lazy[i]);
  }
  void push(int i)
  {
    all_apply(2 * i, lazy[i]);
    all_apply(2 * i + 1, lazy[i]);
    lazy[i] = AM::id();
  }

public:
  LazySegmentTree() {}
  LazySegmentTree(int n) : LazySegmentTree(vc<S>(n, AM::e())) {}
  template <class Iter>
  LazySegmentTree(const Iter &bg, const Iter &ed) : LazySegmentTree(vc<S>(bg, ed)) {}
  template <class T>
  LazySegmentTree(const vc<T> &vec)
      : n(vec.size()), siz(bit_ceil(vec.size())), lg(countr_zero(siz))
  {
    dat.resize(2 * siz, AM::e()), lazy.resize(siz, AM::id());
    repi(i, vec.size()) dat[siz + i] = vec[i];
    repi(i, siz - 1, 0, -1) update(i);
  }

  void set(int p, const S &x)
  {
    assert(0 <= p && p < n);
    p += siz;
    repi(i, lg, 0, -1) push(p >> i);
    dat[p] = x;
    repi(i, 1, lg + 1) update(p >> i);
  }
  S get(int p)
  {
    assert(0 <= p && p < n);
    p += siz;
    repi(i, lg, 0, -1) push(p >> i);
    return dat[p];
  }
  void apply(int p, const F &f)
  {
    assert(0 <= p && p < n);
    p += siz;
    repi(i, lg, 0, -1) push(p >> i);
    dat[p] = AM::mapping(f, dat[p]);
    repi(i, 1, lg + 1) update(p >> i);
  }
  void apply(int l, int r, const F &f)
  {
    assert(0 <= l && l <= r && r <= n);
    if (l == r)
      return;
    l += siz, r += siz;
    repi(i, lg, 0, -1)
    {
      if (((l >> i) << i) != l)
        push(l >> i);
      if (((r >> i) << i) != r)
        push((r - 1) >> i);
    }
    int ml = l, mr = r;
    while (l < r)
    {
      if (l & 1)
        all_apply(l++, f);
      if (r & 1)
        all_apply(--r, f);
      l >>= 1, r >>= 1;
    }
    l = ml, r = mr;
    repi(i, 1, lg + 1)
    {
      if (((l >> i) << i) != l)
        update(l >> i);
      if (((r >> i) << i) != r)
        update((r - 1) >> i);
    }
  }
  S prod(int l, int r)
  {
    assert(0 <= l && l <= r && r <= n);
    if (l == r)
      return AM::e();
    l += siz, r += siz;
    repi(i, lg, 0, -1)
    {
      if (((l >> i) << i) != l)
        push(l >> i);
      if (((r >> i) << i) != r)
        push((r - 1) >> i);
    }
    S sml = AM::e(), smr = AM::e();
    while (l < r)
    {
      if (l & 1)
        sml = AM::op(sml, dat[l++]);
      if (r & 1)
        smr = AM::op(dat[--r], smr);
      l >>= 1, r >>= 1;
    }
    return AM::op(sml, smr);
  }
  S all_prod() const { return dat[1]; }

  vc<S> content()
  {
    vc<S> res(n);
    repi(i, n) res[i] = get(i);
    return res;
  }

  // g が単調のとき g( prod[l, r) ) == true となる最大の r
  // 制約: g( e() ) == true
  template <class I = ll, class G>
  I max_right(int l, const G &g)
  {
    assert(0 <= l && l <= n);
    assert(g(AM::e()));
    if (l == n)
      return n;
    l += siz;
    S sm = AM::e();
    do
    {
      while (l % 2 == 0)
        l >>= 1;
      if (!g(AM::op(sm, dat[l])))
      {
        while (l < siz)
        {
          push(l);
          l *= 2;
          if (g(AM::op(sm, dat[l])))
          {
            sm = AM::op(sm, dat[l]);
            l++;
          }
        }
        return l - siz;
      }
      sm = AM::op(sm, dat[l]);
      l++;
    } while ((l & -l) != l);
    return n;
  }

  // g が単調のとき g( prod[l, r) ) == true となる最小の l
  // 制約: g( e() ) == true
  template <class I = ll, class G>
  I min_left(int r, const G &g)
  {
    assert(0 <= r && r <= n);
    assert(g(AM::e()));
    if (r == 0)
      return 0;
    r += siz;
    repi(i, lg, 0, -1) push((r - 1) >> i);
    S sm = AM::e();
    do
    {
      r--;
      while (r > 1 && (r % 2))
        r >>= 1;
      if (!g(AM::op(dat[r], sm)))
      {
        while (r < siz)
        {
          push(r);
          r = 2 * r + 1;
          if (g(AM::op(dat[r], sm)))
          {
            sm = AM::op(dat[r], sm);
            r--;
          }
        }
        return r + 1 - siz;
      }
      sm = AM::op(dat[r], sm);
    } while ((r & -r) != r);
    return 0;
  }
};
