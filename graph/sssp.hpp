#pragma once

#include "../template/template_all_but_modint.hpp"

#include "graph.hpp"
#include "../ds/my_queue.hpp"

/**
 * @brief 単一始点最短路問題
 * @docs docs/graph/sssp.md
 */

// 参考: https://hitonanode.github.io/cplib-cpp/graph/shortest_path.hpp
// todo:
// - dial
// - deque の定数倍高速化
// - fibonacci heap を用いた dijkstra の高速化
// - SPFA 等

template <bool is_directed, class Cost, const Cost infty = INF>
struct ShortestPath : Graph<is_directed, Cost>
{
  using Graph<is_directed, Cost>::Graph;
  using Graph<is_directed, Cost>::size;
  using Graph<is_directed, Cost>::num_of_edges;
  using Graph<is_directed, Cost>::out_edges;
  using Graph<is_directed, Cost>::edges;

private:
  vc<Cost> dists;
  vc<Edge<Cost>> prv;

public:
  vc<Cost> bfs(int s, int t = -1)
  {
    const int n = size();
    assert(0 <= s && s < n);
    dists.assign(n, infty);
    prv.assign(n, {});
    MyQueue<int> que;
    dists[s] = 0;
    que.push(s);
    while (!que.empty())
    {
      auto v = que.front();
      que.pop();
      if (v == t)
        break;
      fe(e : out_edges(v))
      {
        if (chmin(dists[e.to], dists[e.from] + e.cost))
        {
          prv[e.to] = e;
          que.push(e.to);
        }
      }
    }
    return dists;
  }

  vc<Cost> bfs01(int s, int t = -1)
  {
    const int n = size();
    assert(0 <= s && s < n);
    dists.assign(n, infty);
    prv.assign(n, {});
    deque<int> deq;
    dists[s] = 0;
    deq.push_back(s);
    while (!deq.empty())
    {
      auto v = deq.front();
      deq.pop_front();
      if (v == t)
        break;
      fe(e : out_edges(v))
      {
        if (chmin(dists[e.to], dists[e.from] + e.cost))
        {
          prv[e.to] = e;
          if (e.cost == 0)
            deq.push_front(e.to);
          else
            deq.push_back(e.to);
        }
      }
    }
    return dists;
  }

  vc<Cost> dijkstra(int s, int t = -1)
  {
    const int n = size();
    assert(0 <= s && s < n);
    dists.assign(n, infty);
    prv.assign(n, {});
    pql<pair<Cost, int>> pque;
    dists[s] = 0;
    pque.push({0, s});
    while (!pque.empty())
    {
      auto [d, v] = pque.top();
      pque.pop();
      if (v == t)
        break;
      if (dists[v] != d)
        continue;
      fec(e : out_edges(v))
      {
        Cost nd = dists[e.from] + e.cost;
        if (chmin(dists[e.to], nd))
        {
          prv[e.to] = e;
          pque.push({nd, e.to});
        }
      }
    }
    return dists;
  }

  vc<Cost> dijkstra_dense(int s, int t = -1)
  {
    const int n = size();
    assert(0 <= s && s < n);
    dists.assign(n, infty);
    prv.assign(n, {});
    vc<bool> ok(n, false);
    dists[s] = 0;
    repi(_, n)
    {
      Cost mn = infty;
      int v = -1;
      repi(u, n) if (!ok[u] && chmin(mn, dists[u])) v = u;
      if (v == t)
        break;
      ok[v] = true;
      fec(e : out_edges(v))
      {
        if (chmin(dists[e.to], dists[e.from] + e.cost))
          prv[e.to] = e;
      }
    }
    return dists;
  }

  // 最短のウォークとして求める (いくらでも小さくなるなら -infty)
  // O(nm)
  vc<Cost> bellman_ford(int s)
  {
    const int n = size();
    assert(0 <= s && s < n);
    dists.assign(n, infty);
    prv.assign(n, {});
    dists[s] = 0;
    repi(t, 2 * n)
    {
      repi(v, n)
      {
        if (dists[v] == infty)
          continue;
        fec(e : out_edges(v))
        {
          Cost nd = dists[v] == -infty ? -infty : dists[v] + e.cost;
          if (dists[e.to] > nd)
          {
            prv[e.to] = e;
            if (t == n - 1)
              dists[e.to] = -infty;
            else
              dists[e.to] = nd;
          }
        }
      }
    }
    return dists;
  }

  vc<Cost> solve(int s, int t = -1)
  {
    bool neg = false;
    int zcnt = 0;
    Cost wplus1 = -infty;
    bool wpluscnt_geq2 = false;
    auto es = edges();
    fec(e : es)
    {
      if (e.cost < 0)
      {
        neg = true;
        break;
      }
    }
    fec(e : es)
    {
      if (e.cost == 0)
        zcnt++;
      if (e.cost > 0)
      {
        if (wplus1 < 0)
          wplus1 = e.cost;
        else if (wplus1 != e.cost)
        {
          wpluscnt_geq2 = true;
          break;
        }
      }
    }
    if (neg)
      return bellman_ford(s);
    else if (wpluscnt_geq2)
    {
      const ll n = size(), m = num_of_edges();
      if (n * n < (m << 4))
        return dijkstra_dense(s, t);
      else
        return dijkstra(s, t);
    }
    else
    {
      if (zcnt == 0)
        return bfs(s, t);
      else
        return bfs01(s, t);
    }
  }

  // solve(s, t) の後に呼ぶ
  // prv[v] := s から v への最短パスのひとつで v に行くために使った辺 (なければ from=-1, to=-1, cost=-1)
  // (t を指定して打ち切っていた場合、t に到達する最短パスのひとつで使った辺は正しく記録される)
  vc<Edge<Cost>> prev_edges()
  {
    const int n = size();
    assert(SZ(dists) == n && "solve(s, t) is not called");
    return prv;
  }

  // solve(s, t) の後に呼ぶ
  // (sssp の実行で t を指定して打ち切っていても動作する)
  vc<Edge<Cost>> path(int t)
  {
    const int n = size();
    assert(SZ(dists) == n && "solve(s, t) is not called");
    vc<Edge<Cost>> res;
    while (true)
    {
      cauto &e = prv[t];
      if (e.index == -1)
        break;
      t = e.from;
      res.eb(e.rev());
    }
    return rev_path(res);
  }

  // cnt[v] := s から v への最短パスの個数
  // (**sssp の実行で t を指定して打ち切っていてはいけない**)
  template <class mint>
  vc<mint> count_paths()
  {
    const int n = size();
    assert(SZ(dists) == n && "solve(s, t) is not called");
    vc<mint> cnt(n, 0);
    vc<bool> visited(n, false);
    MyQueue<int> que;
    repi(sv, n)
    {
      if (dists[sv] == 0)
      {
        cnt[sv] = 1;
        visited[sv] = true;
        que.push(sv);
      }
    }
    while (!que.empty())
    {
      auto v = que.front();
      que.pop();
      fec(e : out_edges(v))
      {
        if (dists[e.to] == dists[e.from] + e.cost)
        {
          cnt[e.to] += cnt[e.from];
          if (!visited[e.to])
          {
            visited[e.to] = true;
            que.push(e.to);
          }
        }
      }
    }
    return cnt;
  }
};
