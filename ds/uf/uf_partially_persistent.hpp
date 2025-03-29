#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "uf.hpp"

/**
 * @brief 部分永続 UnionFind
 * @docs docs/ds/uf/uf_partially_persistent.md
 */

template <class UFData>
struct UnionFindPartiallyPersistent
{
  friend UFData;

protected:
  vc<int> par;
  vc<typename UFData::VData> vdat;
  typename UFData::GData gdat;
  // upd_time[x]: 頂点 x が代表元でなくなった時刻
  vc<int> upd_time;
  int cur_time;
  // vhis[x]: (vdat[x] の変更時刻, 変更前の vdat[x]) を並べたもの
  vvc<pair<int, typename UFData::VData>> vhis;
  // ghis[t]: 時刻 t における gdat
  vc<typename UFData::GData> ghis;
  
public:
  UnionFindPartiallyPersistent(int n)
  : par(n, -1), vdat(n), gdat(n),
  upd_time(n, (int)1e9), cur_time(0),
  vhis(n, {{0, typename UFData::VData()}}), ghis{gdat}
  { repi(i, n) vdat[i] = typename UFData::VData(i); }

  int leader(int t, int x)
  {
    assert(0 <= x && x < SZ<int>(par));
    if (upd_time[x] > t)
      return x;
    return leader(t, par[x]);
  }
  typename UFData::VData &get_vdata(int t, int x)
  {
    x = leader(t, x);
    int i = leq_max(vhis[x], t, {}, [&](cauto &p)
                    { return p.first; });
    return vhis[x][i].second;
  }
  typename UFData::GData &get_gdata(int t) const { return ghis[t]; }
  bool same(int t, int x, int y) { return leader(t, x) == leader(t, y); }
  // 返り値: (新たな代表元, 辺の追加が行われた時刻)
  // 時刻は初期状態が 0 で、辺が追加されるごとに 1, 2, 3, ...
  template <class V = ll, class Time = ll>
  pair<V, Time> merge(int x, int y, const typename UFData::EWeight &w = 1)
  {
    x = leader(cur_time, x), y = leader(cur_time, y);
    cur_time++;
    if (x == y)
    {
      UFData::add_edge_same(*this, x, w);
      vhis[x].eb(cur_time, vdat[x]);
      ghis.eb(gdat);
      return {x, cur_time};
    }
    if (-par[x] < -par[y])
      swap(x, y);
    par[x] += par[y], par[y] = x;
    UFData::add_edge_diff(*this, x, y, w);
    vhis[x].eb(cur_time, vdat[x]);
    ghis.eb(gdat);
    upd_time[y] = cur_time;
    return {x, cur_time};
  }

  // 各頂点が属する連結成分の番号 (順番は未定義)
  // ACL の groups が欲しい場合: これに group_index を使う
  template <class I = ll>
  vc<I> group_ids()
  {
    const int n = par.size();
    vc<I> gid(n, -1);
    for (int v = 0, i = 0; v < n; v++)
    {
      int l = leader(v);
      if (gid[l] == -1)
        gid[l] = i++;
      gid[v] = gid[l];
    }
    return gid;
  }
};