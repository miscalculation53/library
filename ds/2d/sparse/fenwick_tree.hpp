#pragma once

#include "../../../template/template_all_but_modint.hpp"

#include "ds/fenwick_tree/fenwick_tree.hpp"

/**
 * @brief $2$ 次元 Fenwick Tree（疎、クエリ点先読み）
 * @docs docs/ds/2d/sparse/fenwick_tree.md
 */

// 初期化の際、add クエリの点を与える
// **このとき x 方向の座圧は自動ではやらない**
// .
// G: 可換群 (一部の操作はモノイドでよい)
// I: 座標の型
template <class G, class I>
struct FenwickTree2DSparse
{
  using S = typename G::S;

private:
  int n;
  vvc<I> ys;
  vc<FenwickTree<G>> fw;

public:
  FenwickTree2DSparse() {}
  FenwickTree2DSparse(const vc<pair<I, I>> &xys)
  {
    n = 0;
    fec([ x, y ] : xys) { chmax(n, x + 1); }
    ys.resize(n + 1);
    fec([ x, y ] : xys)
    {
      int i = x + 1;
      while (i <= n)
      {
        ys[i].eb(y);
        i += i & -i;
      }
    }
    fw.resize(n + 1);
    repi(i, 1, n + 1)
    {
      sortunique(ys[i]);
      fw[i] = FenwickTree<G>(ys[i].size());
    }
  }

  // 点 (x, y) に重み w を加算する
  // (x, y) が構築時に与えられていることが必要
  // モノイドでよい
  void add(I x, I y, const S &w)
  {
    assert(0 <= x && x < n);
    int i = x + 1;
    while (i <= n)
    {
      int j = LB(ys[i], y);
      assert(j < (int)ys[i].size() && ys[i][j] == y);
      fw[i].add(j, w);
      i += i & -i;
    }
  }

  // 領域 (-∞, rx) × (-∞, ry) の重み和を求める
  // モノイドでよい
  S sum(I rx, I ry) const
  {
    rx = clamp(rx, I(0), I(n));
    int ri = rx;
    S s = G::e();
    while (ri > 0)
    {
      const int rj = LB(ys[ri], ry);
      s = G::op(s, fw[ri].sum(rj));
      ri -= ri & -ri;
    }
    return s;
  }
  // 領域 (-∞, rx) × [ly, ry) の重み和を求める
  // 群であることが必要
  S sum(I rx, I ly, I ry) const
  {
    rx = clamp(rx, I(0), I(n));
    assert(ly <= ry);
    int ri = rx;
    S s = G::e();
    while (ri > 0)
    {
      const int lj = LB(ys[ri], ly), rj = LB(ys[ri], ry);
      s = G::op(s, fw[ri].sum(lj, rj));
      ri -= ri & -ri;
    }
    return s;
  }
  // 領域 (-∞, rx) × [ly, ry) の重み和を求める
  // 群であることが必要
  S sum(I lx, I rx, I ly, I ry) const
  {
    assert(lx <= rx);
    return G::op(sum(rx, ly, ry), G::inv(sum(lx, ly, ry)));
  }

  vc<tuple<I, I, S>> content() const
  {
    vc<tuple<I, I, S>> res;
    repi(x, n)
    {
      fec(y : ys[x + 1])
      {
        S s = sum(x, x + 1, y, y + 1);
        if (s != G::e())
          res.eb(x, y, s);
      }
    }
    return res;
  }
};
