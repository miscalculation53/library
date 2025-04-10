#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../../math/algebra/algebra_basic_ops.hpp"
#include "uf.hpp"

/**
 * @brief ポテンシャルつき UnionFind
 * @docs docs/ds/uf/uf_potential.md
 */

// G は群で、G::S は UFData::EWeight と一致
template <class G, class UFData>
struct UnionFindPotentialBase : UnionFind<UFData, true>
{
  using UF = UnionFind<UFData, true>;

protected:
  using UF::par;
  vc<typename G::S> weight_;
  vc<bool> valid_;

public:
  UnionFindPotentialBase() {}
  UnionFindPotentialBase(int n)
  : UF(n), weight_(n, G::e()), valid_(n, true) {}
  using UF::same;

  int leader(int x) override
  {
    assert(0 <= x && x < SZ<int>(par));
    if (par[x] < 0)
      return x;
    int lx = leader(par[x]);
    weight_[x] = G::op(weight_[par[x]], weight_[x]);
    return par[x] = lx;
  }

  // どの情報も無視しなかったとして、x を含む連結成分の情報が valid かどうか
  bool valid(int x) { return valid_[leader(x)]; }

  // same(x, y) のときのみ a[x]^{-1} a[y] が定まるので、それを返す
  // ただし、invalid な情報は無視するものとする
  typename G::S diff(int x, int y)
  {
    assert(same(x, y));
    return G::op(G::inv(weight_[x]), weight_[y]);
  }

  // a[x]^{-1} a[y] == w であるという情報を追加する
  // 返り値: (invalid な情報は無視したとして、) この情報が valid かどうか
  bool merge(int x, int y, typename UFData::EWeight w)
  {
    int lx = leader(x), ly = leader(y);
    if (lx == ly)
    {
      bool ok = G::op(G::inv(weight_[x]), weight_[y]) == w;
      if (!ok)
        valid_[lx] = false;
      UFData::add_edge_same(*this, lx, w);
      return ok;
    }
    w = G::op(G::op(weight_[x], w), G::inv(weight_[y]));
    if (-par[lx] < -par[ly])
      swap(lx, ly), w = G::inv(w);
    par[lx] += par[ly], par[ly] = lx;
    weight_[ly] = w;
    if (!valid_[ly])
      valid_[lx] = false;
    UFData::add_edge_diff(*this, lx, ly, w);
    return true;
  }
};

// G は群
template <class G>
using UnionFindPotential = UnionFindPotentialBase<G, UFDataEmpty<typename G::S>>;
// G は群
template <class G>
using UnionFindPotentialEverything = UnionFindPotentialBase<G, UFDataEverything<typename G::S>>;
