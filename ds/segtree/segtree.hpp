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
  SegmentTree(const vc<S> &vec) : n(vec.size()), siz(bit_ceil(vec.size())), dat(2 * siz, M::e())
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

  // g が単調のとき g( prod[l, r) ) == true となる最大の r
  // 制約: g( e() ) == true
  template <class I = ll, class G>
  I max_right(int l, const G &g)
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
    } while ((l & -l) != l);
    return n;
  }

  // g が単調のとき g( prod[l, r) ) == true となる最小の l
  // 制約: g( e() ) == true
  template <class I = ll, class G>
  I min_left(int r, const G &g)
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
