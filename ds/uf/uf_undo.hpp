#pragma once

#include "../../template/template_all.hpp"

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
  vc<tuple<int, int, typename UFData::VData, typename UFData::GData, bool>> his;

public:
  UnionFindUndo() {}
  UnionFindUndo(int n) : UF(n) {}
  using UF::gdat;
  using UF::leader;
  // 返り値: マージした後の新たな代表元
  template <class I = ll>
  I merge(int x, int y, const typename UFData::EWeight &w = 1)
  {
    x = leader(x), y = leader(y);
    his.eb(x, par[x], vdat[x], gdat, false);
    his.eb(y, par[y], vdat[y], gdat, false);
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
    repi(t, 2 * times)
    {
      assert(!his.empty());
      cauto & [ i, p, vd, gd, _ ] = his.back();
      par[i] = p, vdat[i] = vd, gdat = gd;
      his.pop_back();
    }
  }
  // 現在の状態の snapshot を撮る
  void snapshot()
  {
    if (!his.empty())
      std::get<4>(his.back()) = true;
  }
  // 直前に snapshot を撮った状態 (なければ初期状態) に戻す
  void rollback()
  {
    while (!his.empty())
    {
      auto &[i, p, vd, gd, snapshoot] = his.back();
      if (snapshoot)
      {
        snapshoot = false;
        break;
      }
      par[i] = p, vdat[i] = vd, gdat = gd;
      his.pop_back();
    }
  }
};