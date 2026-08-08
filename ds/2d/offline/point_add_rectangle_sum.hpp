#pragma once

#include "../../../template/template_all_but_modint.hpp"

#include "rectangle_sum.hpp"

/**
 * @brief 点加算・矩形和（オフライン）
 * @docs docs/ds/2d/offline/point_add_rectangle_sum.md
 */

// 長方形領域が (-∞, x) × (-∞, y) の形の場合 (逆元いらない)
// M: 可換モノイド
// I: 座標の型
template <class M, class I>
struct PointAddLowerLeftSum
{
  using S = typename M::S;

private:
  using Fn = function<S(S)>;
  using P = tuple<I, I, S>;
  using PF = tuple<I, I, int, int>;
  using Q = pair<I, I>;
  vc<variant<P, PF, Q>> qs;
  vc<Fn> fs;
  LowerLeftSum<M, I> rs;
  int qn = 0;

public:
  // (x, y) に重み w の点を追加
  void point_add(I x, I y, const S &w) { qs.eb(P(x, y, w)); }
  // 前の rectangle sum のクエリを参照した重みの点を追加
  // f は S(S) で、res[qi] を引数にとり、重みを返す関数
  template <class F>
  void point_add_fn(I x, I y, int qi, F f)
  {
    int fi = fs.size();
    fs.eb(move(f));
    qs.eb(PF(x, y, qi, fi));
  }
  // (-∞, rx) × (-∞, ry) の重み和を答えるクエリを追加
  void rectangle_sum(I rx, I ry)
  {
    qs.eb(Q(rx, ry));
    qn++;
  }
  // クエリにまとめて答える
  vc<S> run()
  {
    vc<S> res(qn, M::e());
    auto dfs = [&](auto dfs, int l, int r, int li, int ri) -> void
    {
      if (r - l <= 1)
        return;
      const int m = (l + r) / 2;
      int mi = li;
      repi(i, l, m)
      {
        if (holds_alternative<Q>(qs[i]))
          mi++;
      }
      dfs(dfs, l, m, li, mi);
      if (mi < ri)
      {
        rs.clear();
        bool has_point = false;
        repi(i, l, m)
        {
          if (holds_alternative<P>(qs[i]))
          {
            cauto & [ x, y, w ] = get<P>(qs[i]);
            rs.point_add(x, y, w);
            has_point = true;
          }
        }
        if (has_point)
        {
          repi(i, m, r)
          {
            if (holds_alternative<Q>(qs[i]))
            {
              cauto & [ x, y ] = get<Q>(qs[i]);
              rs.rectangle_sum(x, y);
            }
          }
          vc<S> tmp = rs.run();
          repi(i, mi, ri) res[i] = M::op(res[i], tmp[i - mi]);
        }
      }
      repi(i, m, r)
      {
        if (holds_alternative<PF>(qs[i]))
        {
          auto [x, y, qi, fi] = get<PF>(qs[i]);
          if (li <= qi && qi < mi)
          {
            S w = fs[fi](res[qi]);
            fs[fi] = {};
            qs[i] = P(x, y, w);
          }
        }
      }
      dfs(dfs, m, r, mi, ri);
    };
    dfs(dfs, 0, qs.size(), 0, qn);
    return res;
  }
};

// G: 可換群
// I: 座標の型
template <class G, class I>
struct PointAddRectangleSum
{
  using S = typename G::S;

private:
  using Fn = function<S(S)>;
  using P = tuple<I, I, S>;
  using PF = tuple<I, I, int, int>;
  using Q = tuple<I, I, I, I>;
  vc<variant<P, PF, Q>> qs;
  vc<Fn> fs;
  RectangleSum<G, I> rs;
  int qn = 0;

public:
  // (x, y) に重み w の点を追加
  void point_add(I x, I y, const S &w) { qs.eb(P(x, y, w)); }
  // 前の rectangle sum のクエリを参照した重みの点を追加
  // f は S(S) で、res[qi] を引数にとり、重みを返す関数
  template <class F>
  void point_add_fn(I x, I y, int qi, F f)
  {
    int fi = fs.size();
    fs.eb(move(f));
    qs.eb(PF(x, y, qi, fi));
  }
  // [lx, rx) × [ly, ry) の重み和を答えるクエリを追加
  void rectangle_sum(I lx, I rx, I ly, I ry)
  {
    qs.eb(Q(lx, rx, ly, ry));
    qn++;
  }
  // クエリにまとめて答える
  vc<S> run()
  {
    vc<S> res(qn, G::e());
    auto dfs = [&](auto dfs, int l, int r, int li, int ri) -> void
    {
      if (r - l <= 1)
        return;
      const int m = (l + r) / 2;
      int mi = li;
      repi(i, l, m)
      {
        if (holds_alternative<Q>(qs[i]))
          mi++;
      }
      dfs(dfs, l, m, li, mi);
      if (mi < ri)
      {
        rs.clear();
        bool has_point = false;
        repi(i, l, m)
        {
          if (holds_alternative<P>(qs[i]))
          {
            cauto & [ x, y, w ] = get<P>(qs[i]);
            rs.point_add(x, y, w);
            has_point = true;
          }
        }
        if (has_point)
        {
          repi(i, m, r)
          {
            if (holds_alternative<Q>(qs[i]))
            {
              cauto & [ lx, rx, ly, ry ] = get<Q>(qs[i]);
              rs.rectangle_sum(lx, rx, ly, ry);
            }
          }
          vc<S> tmp = rs.run();
          repi(i, mi, ri) res[i] = G::op(res[i], tmp[i - mi]);
        }
      }
      repi(i, m, r)
      {
        if (holds_alternative<PF>(qs[i]))
        {
          auto [x, y, qi, fi] = get<PF>(qs[i]);
          if (li <= qi && qi < mi)
          {
            S w = fs[fi](res[qi]);
            fs[fi] = {};
            qs[i] = P(x, y, w);
          }
        }
      }
      dfs(dfs, m, r, mi, ri);
    };
    dfs(dfs, 0, qs.size(), 0, qn);
    return res;
  }
};
