#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "uf.hpp"

/**
 * @brief undo 可能 UnionFind
 * @docs docs/ds/uf/uf_undo.md
 */

template <class UFData>
struct UnionFindUndo : UnionFind<UFData, false>
{
  using UF = UnionFind<UFData, false>;
  friend UFData;

protected:
  using UF::par;
  using UF::vdat;
  vc<tuple<int, int, typename UFData::VData, typename UFData::GData>> his;
  int snap_time;

public:
  UnionFindUndo() : snap_time(0) {}
  UnionFindUndo(int n) : UF(n), snap_time(0) {}
  using UF::gdat;
  using UF::leader;
  // 返り値: マージした後の新たな代表元
  template <class I = ll>
  I merge(int x, int y, const typename UFData::EWeight &w = 1)
  {
    x = leader(x), y = leader(y);
    his.eb(x, par[x], vdat[x], gdat);
    his.eb(y, par[y], vdat[y], gdat);
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
  // times 回の辺追加を undo する
  // 制約: すでに times 回の辺追加が行われている
  void undo(int times = 1)
  {
    repi(_, 2 * times)
    {
      assert(!his.empty());
      cauto & [ i, p, vd, gd ] = his.back();
      par[i] = p, vdat[i] = vd, gdat = gd;
      his.pop_back();
    }
  }
  // 初期状態に戻す
  void reset() { undo(SZ(his) / 2); }
  // 現在の状態の snapshot を撮る (保持できる snapshot は 1 個まで)
  void snapshot() { snap_time = his.size(); }
  // snapshot を撮った状態に戻す (保持できる snapshot は 1 個まで)
  void rollback() { undo((SZ(his) - snap_time) / 2); }
};
