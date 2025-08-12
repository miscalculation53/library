#pragma once

#include "../../../template/template_all_but_modint.hpp"

#include "../../fenwick_tree/fenwick_tree.hpp"

/**
 * @brief 矩形和（オフライン）
 * @docs docs/ds/2d/offline/rectangle_sum.md
 */

// 全部の point add をした後に rectangle sum
// 長方形領域が (-∞, x) × (-∞, y) の形の場合 (逆元いらない)
// M: 可換モノイド
// I: 座標の型
template <class M, class I>
struct LowerLeftSum
{
  using S = typename M::S;

private:
  struct P
  {
    I x, y;
    S w;
    P(I x, I y, const S &w) : x(x), y(y), w(w) {}
    bool operator<(const P &rhs) const { return x < rhs.x; }
  };
  struct Q
  {
    I x, y;
    int qi;
    Q(I x, I y, int qi) : x(x), y(y), qi(qi) {}
    bool operator<(const Q &rhs) const { return x < rhs.x; }
  };
  vc<P> ps;
  vc<Q> qs;
  vc<I> ys;

public:
  // (x, y) に重み w を加えるクエリを追加
  void point_add(I x, I y, const S &w)
  {
    ps.eb(x, y, w);
    ys.eb(y);
  }
  // (-∞, rx) × (-∞, ry) の重み和を答えるクエリを追加
  void rectangle_sum(I rx, I ry) { qs.eb(rx, ry, qs.size()); }
  // クエリにまとめて答える
  vc<S> run()
  {
    const int n = ps.size(), q = qs.size();
    sort(ALL(ps)), sort(ALL(qs));
    sortunique(ys);
    FenwickTree<M> fw(ys.size());
    vc<S> res(q, M::e());
    for (int i = 0, j = 0; j < q; j++)
    {
      while (i < n && ps[i].x < qs[j].x)
      {
        fw.add(LB(ys, ps[i].y), ps[i].w);
        i++;
      }
      const int qi = qs[j].qi;
      res[qi] = fw.sum(LB(ys, qs[j].y));
    }
    return res;
  }

  void clear() { ps.clear(), qs.clear(), ys.clear(); }
};

// 全部の point add をした後に rectangle sum
// G: 可換群
// I: 座標の型
template <class G, class I>
struct RectangleSum
{
  using S = typename G::S;

private:
  struct P
  {
    I x, y;
    S w;
    P(I x, I y, const S &w) : x(x), y(y), w(w) {}
    bool operator<(const P &rhs) const { return x < rhs.x; }
  };
  struct Q
  {
    I x, ly, ry;
    int qi;
    Q(I x, I ly, I ry, int qi) : x(x), ly(ly), ry(ry), qi(qi) {}
    bool operator<(const Q &rhs) const { return x < rhs.x; }
  };
  vc<P> ps;
  vc<Q> qs;
  vc<I> ys;

public:
  // (x, y) に重み w を加えるクエリを追加
  void point_add(I x, I y, const S &w)
  {
    ps.eb(x, y, w);
    ys.eb(y);
  }
  // [lx, rx) × [ly, ry) の重み和を答えるクエリを追加
  void rectangle_sum(I lx, I rx, I ly, I ry)
  {
    qs.eb(lx, ly, ry, qs.size());
    qs.eb(rx, ly, ry, qs.size());
  }
  // クエリにまとめて答える
  vc<S> run()
  {
    const int n = ps.size(), q = qs.size();
    sort(ALL(ps)), sort(ALL(qs));
    sortunique(ys);
    FenwickTree<G> fw(ys.size());
    vc<S> res(q / 2, G::e());
    for (int i = 0, j = 0; j < q; j++)
    {
      while (i < n && ps[i].x < qs[j].x)
      {
        fw.add(LB(ys, ps[i].y), ps[i].w);
        i++;
      }
      const int qi = qs[j].qi;
      const S s = fw.sum(LB(ys, qs[j].ly), LB(ys, qs[j].ry));
      if (qi & 1)
        res[qi >> 1] = G::op(res[qi >> 1], s);
      else
        res[qi >> 1] = G::op(res[qi >> 1], G::inv(s));
    }
    return res;
  }

  void clear() { ps.clear(), qs.clear(), ys.clear(); }
};
