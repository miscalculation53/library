#pragma once

#include "../../../template/template_all_but_modint.hpp"
#include "../../../math/algebra/algebra_basic_ops.hpp"
#include "../../fenwick_tree/fenwick_tree.hpp"
#include "../../csr.hpp"
#include "common.hpp"

/**
 * @brief $2$ 次元 Fenwick Tree（疎、クエリ点先読み）
 * @docs docs/ds/2d/sparse/fenwick_tree.md
 */

// 初期化の際、add クエリの点をすべて与える
// 点更新は、構築時に与えた点の添字 point_id で指定する
// M: 可換モノイド (矩形差分を取る操作では可換群)
// I: 座標の型
// D: 各外側ノードに載せるデータ構造
template <class M, class I, template <class...> class D = FenwickTree>
struct FenwickTree2DSparse
{
  using S = typename M::S;
  using DS = D<M>;

private:
  int n = 0;
  vc<I> xs;
  CSR<I> ys;
  vc<int> up;
  vc<DS> dss;
  vc<pair<I, I>> unique_points;
  vc<pair<int, int>> handles;

  S sum_prefix(int rx, I ry) const
  {
    S s = M::e();
    while (rx > 0)
    {
      const auto row = ys[rx];
      const int rj = LB(row, ry);
      s = M::op(s, sparse_2d::prefix_sum(dss[rx], rj));
      rx -= rx & -rx;
    }
    return s;
  }

  S sum_prefix(int rx, I ly, I ry) const
  {
    S s = M::e();
    while (rx > 0)
    {
      const auto row = ys[rx];
      const int lj = LB(row, ly), rj = LB(row, ry);
      s = M::op(s, sparse_2d::range_sum(dss[rx], lj, rj));
      rx -= rx & -rx;
    }
    return s;
  }

  S sum_equal_y(int rx, I y) const
  {
    S s = M::e();
    while (rx > 0)
    {
      const auto row = ys[rx];
      const int lj = LB(row, y);
      const int rj = upper_bound(ALL(row), y) - row.begin();
      s = M::op(s, sparse_2d::range_sum(dss[rx], lj, rj));
      rx -= rx & -rx;
    }
    return s;
  }

  void build(const vc<pair<I, I>> &xys, const vc<S> *ws)
  {
    unique_points = xys;
    fec([x, y] : xys) xs.eb(x);
    sortunique(xs);
    sortunique(unique_points);
    n = SZ(xs);
    vc<int> cnt(n + 1);
    handles.reserve(xys.size());
    fec([x, y] : xys)
    {
      int i = LB(xs, x) + 1;
      handles.eb(i, 0);
      for (int j = i; j <= n; j += j & -j) cnt[j]++;
    }
    ys = CSR<I>(cnt);
    vc<int> cur(n + 1);
    auto &elist = ys.get_elist();
    repi(k, xys.size())
    {
      I y = xys[k].second;
      for (int i = handles[k].first; i <= n; i += i & -i)
        elist[ys.offset(i) + cur[i]++] = y;
    }
    ys.sortunique();
    up.resize(ys.get_elist().size());
    repi(i, 1, n + 1)
    {
      int p = i + (i & -i);
      if (p <= n)
      {
        const auto row = ys[i], prow = ys[p];
        int j = 0;
        repi(k, row.size())
        {
          while (j < prow.size() && prow[j] < row[k]) j++;
          assert(j < prow.size() && prow[j] == row[k]);
          up[ys.offset(i) + k] = j;
        }
      }
    }
    repi(k, xys.size())
    {
      const int i = handles[k].first;
      const auto row = ys[i];
      int j = LB(row, xys[k].second);
      assert(j < row.size() && row[j] == xys[k].second);
      handles[k].second = j;
    }

    dss.resize(n + 1);
    if (ws)
    {
      assert(ws->size() == xys.size());
      vc<S> vals(ys.get_elist().size(), M::e());
      repi(k, xys.size())
      {
        auto [i, j] = handles[k];
        while (i <= n)
        {
          S &v = vals[ys.offset(i) + j];
          v = M::op(v, (*ws)[k]);
          int p = i + (i & -i);
          if (p > n) break;
          j = up[ys.offset(i) + j], i = p;
        }
      }
      repi(i, 1, n + 1)
      {
        const int l = ys.offset(i), r = ys.offset(i + 1);
        dss[i] = DS(vc<S>(vals.begin() + l, vals.begin() + r));
      }
    }
    else
    {
      repi(i, 1, n + 1)
        dss[i] = DS(vc<S>(ys[i].size(), M::e()));
    }
  }

public:
  FenwickTree2DSparse() {}
  FenwickTree2DSparse(const vc<pair<I, I>> &xys)
  {
    build(xys, nullptr);
  }

  // 初期値つきで構築し、xyws[i] の点を point_id = i とする
  FenwickTree2DSparse(const vc<tuple<I, I, S>> &xyws)
  {
    vc<pair<I, I>> xys;
    vc<S> ws;
    xys.reserve(xyws.size()), ws.reserve(xyws.size());
    fec([x, y, w] : xyws) xys.eb(x, y), ws.eb(w);
    build(xys, &ws);
  }

  // point_id に対応する点へ w を加算する
  // 可換モノイドでよい
  void add(int point_id, const S &w)
  {
    assert(0 <= point_id && point_id < SZ(handles));
    auto [i, j] = handles[point_id];
    while (i <= n)
    {
      sparse_2d::point_add<M>(dss[i], j, w);
      int p = i + (i & -i);
      if (p > n) break;
      j = up[ys.offset(i) + j], i = p;
    }
  }

  // 領域 (-∞, rx) × (-∞, ry) の重み和を求める
  // 可換モノイドでよい
  S sum(I rx, I ry) const
  {
    return sum_prefix(LB(xs, rx), ry);
  }
  // 領域 (-∞, rx) × [ly, ry) の重み和を求める
  // 可換モノイドで、D<M> が区間クエリを持つことが必要
  S sum(I rx, I ly, I ry) const
  {
    assert(ly <= ry);
    return sum_prefix(LB(xs, rx), ly, ry);
  }
  // 領域 [lx, rx) × [ly, ry) の重み和を求める
  // M が群であることが必要
  S sum(I lx, I rx, I ly, I ry) const
  {
    assert(lx <= rx);
    return M::op(sum(rx, ly, ry), M::inv(sum(lx, ly, ry)));
  }

  vc<tuple<I, I, S>> content() const
  {
    vc<tuple<I, I, S>> res;
    fec([x, y] : unique_points)
    {
      int xi = LB(xs, x);
      S s = M::op(M::inv(sum_equal_y(xi, y)), sum_equal_y(xi + 1, y));
      if (s != M::e()) res.eb(x, y, s);
    }
    return res;
  }
};
