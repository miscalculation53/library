#pragma once

#include "../../template/template_all_but_modint.hpp"

/**
 * @brief UnionFind
 * @docs docs/ds/uf/uf.md
 */

// UFData にデフォルトで用意されているもの
// - UFDataEmpty (何もなし、ACL 相当)
// - UFDataEverything (全部載せ)
template <class UFData, bool compress = true>
struct UnionFind
{
  friend UFData;

protected:
  vc<int> par;
  vc<typename UFData::VData> vdat;

public:
  typename UFData::GData gdat;

  UnionFind() {}
  UnionFind(int n) : par(n, -1), vdat(n), gdat(n)
  { repi(i, n) vdat[i] = typename UFData::VData(i); }

  virtual int leader(int x)
  {
    assert(0 <= x && x < SZ<int>(par));
    if (par[x] < 0)
      return x;
    if constexpr (compress)
      return par[x] = leader(par[x]);
    else
      return leader(par[x]);
  }
  // 頂点 x を含む連結成分の頂点数
  template <class I = ll>
  I size(int x) { return -par[leader(x)]; }
  typename UFData::VData &get_vdata(int x) { return vdat[leader(x)]; }
  bool same(int x, int y) { return leader(x) == leader(y); }
  // 返り値: マージした後の新たな代表元
  template <class I = ll>
  I merge(int x, int y, const typename UFData::EWeight &w = 1)
  {
    x = leader(x), y = leader(y);
    if (x == y)
    {
      UFData::add_edge_same(*this, x, w);
      return x;
    }
    if (-par[x] < -par[y])
      swap(x, y);
    par[x] += par[y], par[y] = x;
    UFData::add_edge_diff(*this, x, y, w);
    return x;
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

template <class EWeight_ = ll>
struct UFDataEmpty
{
  struct VData
  {
    VData() {}
    VData(int) {}
  };
  struct GData
  {
    GData() {}
    GData(int) {}
  };
  using EWeight = EWeight_;
  template <class UF>
  static void add_edge_diff(UF &, int, int, EWeight) {}
  template <class UF>
  static void add_edge_same(UF &, int, EWeight) {}
};

template <class EWeight_ = ll, bool need_vlist = false>
struct UFDataEverything
{
  struct VData
  {
    VData() {}

    ll vsum;
    ll esum;
    vc<ll> vlist;

    // 頂点 i の初期化
    VData(int i)
    {
      vsum = 1;
      esum = 0;
      if constexpr (need_vlist)
        vlist = {i};
    }
  };
  struct GData
  {
    GData() {}

    ll cmp_cnt;
    ll min_leader, max_leader;

    // 頂点数 n のグラフの初期化
    GData(int n)
    {
      cmp_cnt = n;
      min_leader = 0, max_leader = n - 1;
    }
  };
  using EWeight = EWeight_;
  template <class UF>
  static void add_edge_diff(UF &uf, int x, int y, EWeight w)
  {
    VData &xd = uf.vdat[x], &yd = uf.vdat[y];
    GData &gd = uf.gdat;
    xd.vsum += yd.vsum;
    xd.esum += w;
    if constexpr (need_vlist)
    {
      xd.vlist.insert(xd.vlist.end(), ALL(yd.vlist));
      yd.vlist.clear();
    }
    gd.cmp_cnt--;
    while (uf.leader(gd.min_leader) != gd.min_leader)
      gd.min_leader++;
    while (uf.leader(gd.max_leader) != gd.max_leader)
      gd.max_leader--;
  }
  template <class UF>
  static void add_edge_same(UF &uf, int x, EWeight w)
  {
    VData &xd = uf.vdat[x];
    xd.esum += w;
  }
};
