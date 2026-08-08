#pragma once

#include "../../../template/template_all_but_modint.hpp"

#include "ds/segtree/segtree.hpp"
#include "../../csr.hpp"
#include "common.hpp"

/**
 * @brief $2$ 次元セグメント木（疎、クエリ点先読み）
 * @docs docs/ds/2d/sparse/segtree.md
 */

// 初期化の際、更新されうる点と初期値をすべて与える
// 点更新は、構築時に与えた点の添字 point_id で指定する
// セグメント木にデータ構造 D<M> を載せる
// D のデフォルトは SegmentTree
// 更新がないなら (disjoint) sparse table とかも
// D<M> に set, get があるなら set, get, modify が使える
// M: 可換モノイド
// I: 座標の型
template <class M, class I, template <class...> class D = SegmentTree>
struct SegmentTree2DSparse
{
  using S = typename M::S;
  using DS = D<M>;

private:
  int n, siz, lg;
  vc<I> xs;
  struct Y
  {
    I y;
    S w;
    Y() {}
    Y(I y, S w) : y(y), w(w) {}
    bool operator<(const Y &rhs) const { return y < rhs.y; }
  };
  struct Handle
  {
    int x, root_y, leaf_y;
    I y;
    Handle(int x, int root_y, int leaf_y, I y)
        : x(x), root_y(root_y), leaf_y(leaf_y), y(y) {}
  };
  struct Level
  {
    CSR<I> ys;
    vc<pair<int, int>> to;
  };
  vc<Level> levels;
  vc<DS> dss;
  vc<Handle> handles;

public:
  SegmentTree2DSparse() {}
  SegmentTree2DSparse(const vc<tuple<I, I, S>> &xyws)
  {
    fec([x, y, w] : xyws)
    {
      xs.eb(x);
    }
    sortunique(xs);
    n = SZ(xs);
    siz = bit_ceil(n);
    lg = countr_zero((unsigned)siz);
    levels.reserve(lg + 1);
    dss.resize(2 * siz);
    vc<S> ws;
    {
      vvc<Y> rows(siz);
      fec([ x, y, w ] : xyws) rows[LB(xs, x)].eb(y, w);
      vc<Y> nys;
      vc<int> row_sizes(siz);
      repi(x, siz)
      {
        sort(ALL(rows[x]));
        nys.clear();
        fec(yw : rows[x])
        {
          if (nys.empty() || nys.back().y != yw.y)
            nys.eb(yw);
          else
            nys.back().w = M::op(nys.back().w, yw.w);
        }
        rows[x].swap(nys);
        row_sizes[x] = rows[x].size() + 1;
      }
      Level leaf;
      leaf.ys = CSR<I>(row_sizes);
      ws.assign(leaf.ys.get_elist().size(), M::e());
      repi(x, siz)
      {
        const int off = leaf.ys.offset(x);
        vc<S> vec(rows[x].size());
        repi(k, rows[x].size())
        {
          leaf.ys.get_elist()[off + k] = rows[x][k].y;
          ws[off + k] = vec[k] = move(rows[x][k].w);
        }
        leaf.ys.get_elist()[off + rows[x].size()] = numeric_limits<I>::max();
        dss[siz + x] = DS(vec);
      }
      levels.push_back(move(leaf));
    }
    repi(h, 1, lg + 1)
    {
      const Level &prv = levels[h - 1];
      const int m = siz >> h;
      vc<int> row_sizes(m);
      repi(v, m)
      {
        const auto a = prv.ys[2 * v], b = prv.ys[2 * v + 1];
        int i = 0, j = 0;
        while (i < a.size() && j < b.size())
        {
          row_sizes[v]++;
          if (a[i] == b[j])
            i++, j++;
          else if (a[i] < b[j])
            i++;
          else
            j++;
        }
      }
      Level nxt;
      nxt.ys = CSR<I>(row_sizes);
      nxt.to.resize(nxt.ys.get_elist().size());
      vc<S> nws(nxt.ys.get_elist().size(), M::e());
      repi(v, m)
      {
        const auto a = prv.ys[2 * v], b = prv.ys[2 * v + 1];
        const int ao = prv.ys.offset(2 * v), bo = prv.ys.offset(2 * v + 1);
        const int off = nxt.ys.offset(v);
        int i = 0, j = 0, k = 0;
        while (i < a.size() && j < b.size())
        {
          const int pos = off + k++;
          nxt.to[pos] = {i, j};
          if (a[i] == b[j])
          {
            nxt.ys.get_elist()[pos] = a[i];
            nws[pos] = M::op(ws[ao + i], ws[bo + j]);
            i++, j++;
          }
          else if (a[i] < b[j])
          {
            nxt.ys.get_elist()[pos] = a[i];
            nws[pos] = ws[ao + i];
            i++;
          }
          else
          {
            nxt.ys.get_elist()[pos] = b[j];
            nws[pos] = ws[bo + j];
            j++;
          }
        }
        assert(k == row_sizes[v]);
        vc<S> vec(row_sizes[v] - 1);
        repi(t, vec.size()) vec[t] = nws[off + t];
        dss[(siz >> h) + v] = DS(vec);
      }
      levels.push_back(move(nxt));
      ws = move(nws);
    }
    vc<S>().swap(ws);
    handles.reserve(xyws.size());
    fec([x, y, w] : xyws)
    {
      int xi = LB(xs, x);
      const auto root = levels[lg].ys[0], leaf = levels[0].ys[xi];
      int root_y = LB(root, y), leaf_y = LB(leaf, y);
      assert(root_y + 1 < root.size() && root[root_y] == y);
      assert(leaf_y + 1 < leaf.size() && leaf[leaf_y] == y);
      handles.eb(xi, root_y, leaf_y, y);
    }
  }

  // point_id に対応する点の値を val に変更する
  void set(int point_id, const S &val)
  {
    modify(point_id, [&](S &x) { x = val; });
  }

  // point_id に対応する点の値を参照で受け取る関数 f を適用する
  template <class F>
  void modify(int point_id, const F &f)
  {
    assert(0 <= point_id && point_id < SZ(handles));
    const Handle &hd = handles[point_id];
    int x = hd.x, root_y = hd.root_y, leaf_y = hd.leaf_y;
    I y = hd.y;
    auto dfs = [&](auto dfs, int p, int dep, int v, int k, int a, int b) -> void
    {
      if (dep == 0)
      {
        assert(k == leaf_y);
        S val = dss[p].get(k);
        f(val);
        dss[p].set(k, val);
        return;
      }
      const int c = (a + b) / 2;
      const Level &lev = levels[dep];
      const auto [i, j] = lev.to[lev.ys.offset(v) + k];
      if (x < c)
        dfs(dfs, 2 * p, dep - 1, 2 * v, i, a, c);
      else
        dfs(dfs, 2 * p + 1, dep - 1, 2 * v + 1, j, c, b);
      const Level &ch = levels[dep - 1];
      S nval = M::e();
      if (ch.ys[2 * v][i] == y)
        nval = M::op(nval, dss[2 * p].get(i));
      if (ch.ys[2 * v + 1][j] == y)
        nval = M::op(nval, dss[2 * p + 1].get(j));
      dss[p].set(k, nval);
    };
    dfs(dfs, 1, lg, 0, root_y, 0, siz);
  }

  // point_id に対応する点の値を取得する
  S get(int point_id) const
  {
    assert(0 <= point_id && point_id < SZ(handles));
    const Handle &h = handles[point_id];
    return dss[siz + h.x].get(h.leaf_y);
  }

  // {x} × [ly, ry)
  S sum(I x, I ly, I ry) const
  {
    int xi = LB(xs, x);
    if (xi == n || xs[xi] != x)
      return M::e();
    assert(ly <= ry);
    const auto row = levels[0].ys[xi];
    return sparse_2d::range_sum(dss[siz + xi], LB(row, ly), LB(row, ry));
  }

  // [lx, rx) × [ly, ry)
  S sum(I lx, I rx, I ly, I ry) const
  {
    assert(lx <= rx);
    assert(ly <= ry);
    int ilx = LB(xs, lx), irx = LB(xs, rx);
    S res = M::e();
    auto dfs = [&](auto dfs, int p, int dep, int v, int lk, int rk, int a, int b) -> void
    {
      if (b <= ilx || irx <= a)
        return;
      if (ilx <= a && b <= irx)
      {
        res = M::op(res, sparse_2d::range_sum(dss[p], lk, rk));
        return;
      }
      const int c = (a + b) / 2;
      const Level &lev = levels[dep];
      const int off = lev.ys.offset(v);
      dfs(dfs, 2 * p, dep - 1, 2 * v,
          lev.to[off + lk].first, lev.to[off + rk].first, a, c);
      dfs(dfs, 2 * p + 1, dep - 1, 2 * v + 1,
          lev.to[off + lk].second, lev.to[off + rk].second, c, b);
    };
    const auto root = levels[lg].ys[0];
    dfs(dfs, 1, lg, 0, LB(root, ly), LB(root, ry), 0, siz);
    return res;
  }

  // すべての点の重み和を求める
  S all_sum() const
  {
    const auto root = levels[lg].ys[0];
    return sparse_2d::range_sum(dss[1], 0, root.size() - 1);
  }

  vc<tuple<I, I, S>> content() const
  {
    vc<tuple<I, I, S>> res;
    repi(x, n)
    {
      const auto row = levels[0].ys[x];
      repi(k, row.size() - 1)
      {
        S s = dss[siz + x].get(k);
        if (s != M::e())
          res.eb(xs[x], row[k], s);
      }
    }
    return res;
  }
};
