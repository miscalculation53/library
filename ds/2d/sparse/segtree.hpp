#pragma once

#include "../../../template/template_all_but_modint.hpp"

#include "ds/segtree/segtree.hpp"

/**
 * @brief $2$ 次元セグメント木（疎、クエリ点先読み）
 * @docs docs/ds/2d/sparse/segtree.md
 */

// 初期化の際、add クエリの点と重みを与える
// **このとき x 方向の座圧は自動ではやらない**
// .
// セグメント木にデータ構造 DS を載せる
// DS は典型的には SegmentTree<M> を載せる (M は 2 回書くことになる)
// 更新がないなら (disjoint) sparse table とかも
// DS に set, get, prod があるなら set, get, prod が使える
// I: 座標の型
template <class DS, class M, class I>
struct SegmentTree2DSparse
{
  using S = typename M::S;

private:
  int n, siz;
  struct Y
  {
    I y;
    int i, j;
    S w;
    Y() {}
    Y(I y, int i = -1, int j = -1, S w = M::e()) : y(y), i(i), j(j), w(w) {}
    bool operator<(const Y &rhs) const { return y < rhs.y; }
  };
  vvc<Y> ys;
  vc<DS> dss;
  #define lb(vec, val) (lower_bound(ALL(vec), (val)) - vec.begin())

public:
  SegmentTree2DSparse() {}
  SegmentTree2DSparse(const vc<tuple<I, I, S>> &xyws)
  {
    n = 0;
    fec([ x, y, w ] : xyws) { chmax(n, x + 1); }
    siz = bit_ceil(n);
    ys.resize(2 * siz);
    fec([ x, y, w ] : xyws) { ys[siz + x].eb(y, -1, -1, w); }
    vc<Y> nys;
    repi(x, siz)
    {
      sort(ALL(ys[siz + x]));
      nys.clear();
      fec(yy : ys[siz + x])
      {
        if (nys.empty() || nys.back().y != yy.y)
          nys.eb(yy);
        else
          nys.back().w = M::op(nys.back().w, yy.w);
      }
      swap(nys, ys[siz + x]);
      ys[siz + x].eb(numeric_limits<I>::max());
    }
    repi(x, siz - 1, 0, -1)
    {
      cauto &a = ys[2 * x];
      cauto &b = ys[2 * x + 1];
      int i = 0, j = 0;
      while (i < SZ(a) && j < SZ(b))
      {
        I ai = a[i].y, bj = b[j].y;
        if (ai == bj)
        {
          ys[x].eb(ai, i, j, M::op(a[i].w, b[j].w));
          i++, j++;
        }
        else if (ai < bj)
        {
          ys[x].eb(ai, i, j, a[i].w);
          i++;
        }
        else if (ai > bj)
        {
          ys[x].eb(bj, i, j, b[j].w);
          j++;
        }
      }
    }
    dss.resize(2 * siz);
    repi(x, 1, 2 * siz)
    {
      vc<S> vec(SZ(ys[x]) - 1);
      repi(k, SZ(ys[x]) - 1) vec[k] = ys[x][k].w;
      dss[x] = DS(vec);
    }
  }

  // (x, y) の値を val に変更する
  // (x, y) が構築時に与えられていることが必要
  void set(I x, I y, const S &val)
  {
    assert(0 <= x && x < n);
    auto dfs = [&](auto dfs, int p, int k, int a, int b) -> void
    {
      if (b - a == 1)
      {
        dss[p].set(k, val);
        return;
      }
      const int c = (a + b) / 2;
      const int i = ys[p][k].i, j = ys[p][k].j;
      if (x < c)
        dfs(dfs, 2 * p, i, a, c);
      else
        dfs(dfs, 2 * p + 1, j, c, b);
      S nval = M::e();
      if (ys[2 * p][i].y == y)
        nval = M::op(nval, dss[2 * p].get(i));
      if (ys[2 * p + 1][j].y == y)
        nval = M::op(nval, dss[2 * p + 1].get(j));
      dss[p].set(k, nval);
    };
    dfs(dfs, 1, lb(ys[1], y), 0, siz);
  }

  // (x, y) の値を取得する
  S get(I x, I y) const
  {
    if (!(0 <= x && x < n))
      return M::e();
    x += siz;
    int k = lb(ys[x], y);
    if (ys[x][k].y != y)
      return M::e();
    return dss[x].get(k);
  }

  // x × [ly, ry) の積を取得する
  S prod(I x, I ly, I ry) const
  {
    if (!(0 <= x && x < n))
      return M::e();
    assert(ly <= ry);
    x += siz;
    return dss[x].prod(lb(ys[x], ly), lb(ys[x], ry));
  }

  // [lx, rx) × [ly, ry) の積を取得する
  S prod(I lx, I rx, I ly, I ry) const
  {
    assert(lx <= rx);
    assert(ly <= ry);
    S res = M::e();
    auto dfs = [&](auto dfs, int p, int lk, int rk, int a, int b) -> void
    {
      if (b <= lx || rx <= a)
        return;
      if (lx <= a && b <= rx)
      {
        res = M::op(res, dss[p].prod(lk, rk));
        return;
      }
      const int c = (a + b) / 2;
      dfs(dfs, 2 * p    , ys[p][lk].i, ys[p][rk].i, a, c);
      dfs(dfs, 2 * p + 1, ys[p][lk].j, ys[p][rk].j, c, b);
    };
    dfs(dfs, 1, lb(ys[1], ly), lb(ys[1], ry), 0, siz);
    return res;
  }
  S all_prod() const { return dss[1].all_prod(); }

  vc<tuple<I, I, S>> content() const
  {
    vc<tuple<I, I, S>> res;
    repi(x, n)
    {
      fec([y, i, j, w] : ys[siz + x])
      {
        if (y == numeric_limits<I>::max())
          continue;
        S s = get(x, y);
        if (s != M::e())
          res.eb(x, y, s);
      }
    }
    return res;
  }

  #undef lb
};
