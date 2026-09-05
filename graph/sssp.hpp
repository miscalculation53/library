#pragma once

#include "../template/template_all_but_modint.hpp"

#include "graph.hpp"
#include "../ds/my_queue.hpp"
#include "../utils/is_integral_ext.hpp"
#include "../utils/resolved_value.hpp"

/**
 * @brief 単一始点最短路問題
 * @docs docs/graph/sssp.md
 */

// 参考: https://hitonanode.github.io/cplib-cpp/graph/shortest_path.hpp
// todo:
// - fibonacci heap を用いた dijkstra の高速化
// - SPFA 等

template <bool is_directed, class Cost, auto infty = INF>
struct ShortestPath : Graph<is_directed, Cost>
{
  using Graph<is_directed, Cost>::Graph;
  using Graph<is_directed, Cost>::size;
  using Graph<is_directed, Cost>::num_of_edges;
  using Graph<is_directed, Cost>::out_edges;
  using Graph<is_directed, Cost>::out_arcs;
  using Graph<is_directed, Cost>::edges;

private:
  static constexpr decltype(auto) inf() { return resolved_value<Cost, infty>(); }

  vc<Cost> dists;
  vc<Edge<Cost>> prv;
  int source = -1;
  bool solved_all = false;

  void init_solve(int s, int t)
  {
    source = s;
    solved_all = (t == -1);
  }

public:
  vc<Cost> bfs(int s, int t = -1)
  {
    const int n = size();
    assert(0 <= s && s < n);
    init_solve(s, t);
    dists.assign(n, inf());
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
      fe(e : out_arcs(v))
      {
        if (chmin(dists[e.to], dists[v] + e.cost))
        {
          prv[e.to] = Edge<Cost>(v, e.to, e.cost, e.index);
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
    init_solve(s, t);
    dists.assign(n, inf());
    prv.assign(n, {});
    vc<int> cur{int(s)}, nxt;
    vc<unsigned char> used(n, false);
    dists[s] = 0;
    while (!cur.empty() || !nxt.empty())
    {
      if (cur.empty()) cur.swap(nxt);
      int v = cur.back();
      cur.pop_back();
      if (used[v])
        continue;
      used[v] = true;
      if (v == t)
        break;
      fe(e : out_arcs(v))
      {
        if (chmin(dists[e.to], dists[v] + e.cost))
        {
          prv[e.to] = Edge<Cost>(v, e.to, e.cost, e.index);
          if (e.cost == 0)
            cur.eb(e.to);
          else
            nxt.eb(e.to);
        }
      }
    }
    return dists;
  }

  // 0 以上 max_cost 以下の整数重みについて Dial 法で最短路を求める
  vc<Cost> dial(int s, int max_cost, int t = -1)
  {
    static_assert(is_integral_ext<Cost>);
    const int n = size();
    assert(0 <= s && s < n);
    assert(0 <= max_cost && max_cost < INT_MAX);
    init_solve(s, t);
    dists.assign(n, inf());
    prv.assign(n, {});

    const int bcnt = max_cost + 1;
    vc<int> head(bcnt, -1), nxt(n, -2), pre(n, -2);
    int que_size = 0;
    auto erase = [&](int v, int b)
    {
      const int p = pre[v], q = nxt[v];
      if (p == -1)
        head[b] = q;
      else
        nxt[p] = q;
      if (q != -1)
        pre[q] = p;
      nxt[v] = pre[v] = -2;
      que_size--;
    };
    auto push = [&](int v, int b)
    {
      nxt[v] = head[b], pre[v] = -1;
      if (head[b] != -1)
        pre[head[b]] = v;
      head[b] = v, que_size++;
    };

    Cost cur = 0;
    dists[s] = 0;
    push(s, 0);
    while (que_size)
    {
      int b = int(cur % bcnt);
      while (head[b] == -1)
      {
        cur++;
        if (++b == bcnt)
          b = 0;
      }
      const int v = head[b];
      erase(v, b);
      assert(dists[v] == cur);
      if (v == t)
        break;
      fec(e : out_arcs(v))
      {
        Cost nd = dists[v] + e.cost;
        if (nd < dists[e.to])
        {
          if (pre[e.to] != -2)
            erase(e.to, int(dists[e.to] % bcnt));
          dists[e.to] = nd, prv[e.to] = Edge<Cost>(v, e.to, e.cost, e.index);
          push(e.to, int(nd % bcnt));
        }
      }
    }
    return dists;
  }

  vc<Cost> dijkstra(int s, int t = -1)
  {
    const int n = size();
    assert(0 <= s && s < n);
    init_solve(s, t);
    dists.assign(n, inf());
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
      fec(e : out_arcs(v))
      {
        Cost nd = dists[v] + e.cost;
        if (chmin(dists[e.to], nd))
        {
          prv[e.to] = Edge<Cost>(v, e.to, e.cost, e.index);
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
    init_solve(s, t);
    dists.assign(n, inf());
    prv.assign(n, {});
    vc<bool> ok(n, false);
    dists[s] = 0;
    repi(_, n)
    {
      Cost mn = inf();
      int v = -1;
      repi(u, n) if (!ok[u] && chmin(mn, dists[u])) v = u;
      if (v == -1)
        break;
      if (v == t)
        break;
      ok[v] = true;
      fec(e : out_arcs(v))
      {
        if (chmin(dists[e.to], dists[v] + e.cost))
          prv[e.to] = Edge<Cost>(v, e.to, e.cost, e.index);
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
    init_solve(s, -1);
    dists.assign(n, inf());
    prv.assign(n, {});
    dists[s] = 0;
    repi(t, 2 * n)
    {
      repi(v, n)
      {
        if (dists[v] == inf())
          continue;
        fec(e : out_arcs(v))
        {
          Cost nd = dists[v] == -inf() ? -inf() : dists[v] + e.cost;
          if (dists[e.to] > nd)
          {
            prv[e.to] = Edge<Cost>(v, e.to, e.cost, e.index);
            if (t == n - 1)
              dists[e.to] = -inf();
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
    Cost wplus1 = -inf();
    Cost max_cost = 0;
    bool wpluscnt_geq2 = false;
    repi(v, size()) fec(e : out_arcs(v))
    {
      if (e.cost < 0)
      {
        neg = true;
        break;
      }
      chmax(max_cost, e.cost);
    }
    repi(v, size()) fec(e : out_arcs(v))
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
      {
        if constexpr (is_integral_ext<Cost>)
        {
          const u128 c = u128(max_cost);
          const ull lg = max<ull>(1, bit_width(ull(n)) - 1);
          if (c < u128(INT_MAX) && u128(n) * c <= u128(m) * lg)
            return dial(s, int(c), t);
        }
        return dijkstra(s, t);
      }
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
    assert(SZ(dists) == n && source != -1 && "solve(s) is not called");
    assert(solved_all && "solve(s, t) must not stop at t");
    assert(dists[source] != -inf() && "the shortest-path graph must be finite");
    vc<mint> cnt(n, 0);
    vc<int> indeg(n, 0);
    int active = 0;
    repi(v, n) if (dists[v] != inf() && dists[v] != -inf()) active++;
    repi(v, n)
    {
      if (dists[v] == inf() || dists[v] == -inf())
        continue;
      fec(e : out_arcs(v))
      {
        if (dists[e.to] != inf() && dists[e.to] != -inf() &&
            dists[e.to] == dists[v] + e.cost)
          indeg[e.to]++;
      }
    }
    MyQueue<int> que;
    repi(v, n) if (dists[v] != inf() && dists[v] != -inf() && indeg[v] == 0) que.push(v);
    cnt[source] = 1;
    int processed = 0;
    while (!que.empty())
    {
      auto v = que.front();
      que.pop();
      processed++;
      fec(e : out_arcs(v))
      {
        if (dists[e.to] != inf() && dists[e.to] != -inf() &&
            dists[e.to] == dists[v] + e.cost)
        {
          cnt[e.to] += cnt[v];
          if (--indeg[e.to] == 0)
            que.push(e.to);
        }
      }
    }
    assert(processed == active && "the shortest-path graph must be a DAG");
    return cnt;
  }
};
