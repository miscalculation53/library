#pragma once

#include "../../template/template_all.hpp"

#include "uf.hpp"
#include "../../math/algebra/algebra_basic_ops.hpp"

/**
 * @brief ポテンシャルつき UnionFind
 * @docs docs/ds/uf/uf.md
 */

// G は群
template <class G>
struct UnionFindPotential : UnionFind<UFDataEmpty<typename G::S>, true>
{
  using UFData = UFDataEmpty<typename G::S>;
  using UF = UnionFind<UFData, true>;

protected:
  vc<typename G::S> weight_;
  vc<bool> valid_;
  typename G::S weight(int x)
  {
    leader(x);
    return weight_[x];
  }

public:
  UnionFindPotential(int n)
  : UF(n), weight_(n, G::e()), valid_(n, true) {}
  using UF::par;
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
  // same(x, y) のときのみ a[x]^{-1} a[y] が定まる
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
      return ok;
    }
    w = G::op(G::op(weight_[x], w), G::inv(weight_[y]));
    if (-par[lx] < -par[ly])
      swap(lx, ly), w = G::inv(w);
    par[lx] += par[ly], par[ly] = lx;
    weight_[ly] = w;
    return true;
  }
};