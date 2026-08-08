#pragma once

#include "../../template/template_all_but_modint.hpp"

/**
 * @brief セグメント木
 * @docs docs/ds/segtree/segtree.md
 */

template <class M>
struct SegmentTree
{
  using S = typename M::S;

private:
  int n, siz;
  vc<S> dat;
  void update(int i) { dat[i] = M::op(dat[2 * i], dat[2 * i + 1]); }

public:
  SegmentTree() {}
  SegmentTree(int n) : SegmentTree(vc<S>(n, M::e())) {}
  template <class Iter>
  SegmentTree(const Iter &bg, const Iter &ed) : SegmentTree(vc<S>(bg, ed)) {}
  template <class T>
  SegmentTree(const vc<T> &vec) : n(vec.size()), siz(bit_ceil(vec.size())), dat(2 * siz, M::e())
  {
    repi(i, vec.size()) dat[siz + i] = vec[i];
    repi(i, siz - 1, 0, -1) update(i);
  }

  void set(int p, const S &x)
  {
    assert(0 <= p && p < n);
    p += siz;
    dat[p] = x;
    while (p)
      p >>= 1, update(p);
  }

  S get(int p) const
  {
    assert(0 <= p && p < n);
    return dat[siz + p];
  }
  S prod(int l, int r) const
  {
    assert(0 <= l && l <= r && r <= n);
    l += siz, r += siz;
    S sml = M::e(), smr = M::e();
    while (l < r)
    {
      if (l & 1)
        sml = M::op(sml, dat[l++]);
      if (r & 1)
        smr = M::op(dat[--r], smr);
      l >>= 1, r >>= 1;
    }
    return M::op(sml, smr);
  }
  S all_prod() const { return dat[1]; }

  // g(prod(l, r)) が true となる最大の r を返す
  template <class G>
  int max_right_ok(int l, const G &g) const
  {
    assert(0 <= l && l <= n);
    assert(g(M::e()));
    if (l == n)
      return n;
    l += siz;
    S sm = M::e();
    do
    {
      while (l % 2 == 0)
        l >>= 1;
      if (!g(M::op(sm, dat[l])))
      {
        while (l < siz)
        {
          l *= 2;
          if (g(M::op(sm, dat[l])))
          {
            sm = M::op(sm, dat[l]);
            l++;
          }
        }
        return l - siz;
      }
      sm = M::op(sm, dat[l]);
      l++;
    } while ((l & -l) != l);
    return n;
  }

  // g(prod(l, r)) が true となる最小の l を返す
  template <class G>
  int min_left_ok(int r, const G &g) const
  {
    assert(0 <= r && r <= n);
    assert(g(M::e()));
    if (r == 0)
      return 0;
    r += siz;
    S sm = M::e();
    do
    {
      r--;
      while (r > 1 && (r % 2))
        r >>= 1;
      if (!g(M::op(dat[r], sm)))
      {
        while (r < siz)
        {
          r = 2 * r + 1;
          if (g(M::op(dat[r], sm)))
          {
            sm = M::op(dat[r], sm);
            r--;
          }
        }
        return r + 1 - siz;
      }
      sm = M::op(dat[r], sm);
    } while ((r & -r) != r);
    return 0;
  }

  vc<S> content() const
  {
    vc<S> res(n);
    repi(i, n) res[i] = get(i);
    return res;
  }
};
