#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../fenwick_tree/fenwick_tree_01.hpp"
#include "../csr.hpp"

/**
 * @brief 一点更新・区間頻度（オフライン）
 * @docs docs/ds/query/point_set_range_frequency.md
 */

template <class T>
struct PointSetRangeFrequency
{
private:
  int n;
  vc<T> a;
  using P = pair<int, T>;
  using Q = tuple<int, int, T>;
  vc<variant<P, Q>> qs;
  vc<T> vs;

public:
  PointSetRangeFrequency() {}
  PointSetRangeFrequency(const vc<T> &a) : n(a.size()), a(a), vs(a) {}

  // p 番目を v にするクエリを追加する
  void point_set(int p, const T &v)
  {
    assert(0 <= p && p < n);
    qs.eb(P(p, v));
    vs.eb(v);
  }
  // [l, r) での x の頻度を答えるクエリを追加する
  void range_frequency(int l, int r, const T &x)
  {
    assert(0 <= l && l <= r && r <= n);
    qs.eb(Q(l, r, x));
  }

  // クエリにまとめて答える
  template <class I = ll>
  vc<I> run()
  {
    sortunique(vs);
    vc<int> js(n);
    vc<pair<int, int>> jps;
    repi(p, n)
    {
      js[p] = LB(vs, a[p]);
      jps.eb(js[p], p);
    }
    fec(q : qs)
    {
      if (holds_alternative<P>(q))
      {
        cauto & [ p, v ] = get<P>(q);
        jps.eb(LB(vs, v), p);
      }
    }
    CSR<int> csr(vs.size(), sortuniqued(jps));
    vc<FenwickTree01<I>> fws(vs.size());
    repi(j, vs.size()) fws[j] = FenwickTree01<I>(csr[j].size());
    vc<int> is(n);
    repi(p, n)
    {
      is[p] = LB(csr[js[p]], p);
      fws[js[p]].set(is[p], 1);
    }
    vc<I> res;
    int k = n;
    fec(q : qs)
    {
      if (holds_alternative<P>(q))
      {
        cauto & [ p, v ] = get<P>(q);
        fws[js[p]].set(is[p], 0);
        js[p] = jps[k++].first;
        is[p] = LB(csr[js[p]], p);
        fws[js[p]].set(is[p], 1);
      }
      else if (holds_alternative<Q>(q))
      {
        cauto & [ l, r, x ] = get<Q>(q);
        const int j = LB(vs, x);
        if (j == SZ(vs) || vs[j] != x)
        {
          res.eb(0);
          continue;
        }
        const int li = LB(csr[j], l), ri = LB(csr[j], r);
        res.eb(fws[j].sum(li, ri));
      }
    }
    return res;
  }
};
