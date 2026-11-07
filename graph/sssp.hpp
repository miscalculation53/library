#pragma once

#include "../template/template_all_but_modint.hpp"

#include "graph.hpp"
#include "internal_spfa.hpp"
#include "../ds/my_queue.hpp"
#include "../ds/fibonacci_heap.hpp"
#include "../utils/is_integral_ext.hpp"
#include "../utils/resolved_infty.hpp"

/**
 * @brief 単一・複数始点最短路
 * @docs docs/graph/sssp.md
 */

//! 参考: https://hitonanode.github.io/cplib-cpp/graph/shortest_path.hpp

template <bool is_directed, class Cost, auto infty = nullptr>
struct ShortestPath
{
  using Dist = decay_t<decltype(Edge<Cost>::cost)>;

private:
  const Graph<is_directed, Cost> &g;
  static constexpr decltype(auto) inf() { return resolved_infty<Dist, infty>(); }

  vc<Dist> dists;
  vc<Edge<Cost>> prv;
  vc<int> sources;
  bool solved_all = false;

  template <class Starts>
  void init_solve(const Starts &starts, int t)
  {
    const int n = g.size();
    solved_all = (t == -1);
    dists.assign(n, inf());
    prv.assign(n, {});
    sources.clear();
    for (auto s : starts)
    {
      assert(0 <= s && u128(s) < u128(n));
      if (dists[s] == inf())
      {
        dists[s] = 0;
        sources.eb(int(s));
      }
    }
  }

public:
  explicit ShortestPath(const Graph<is_directed, Cost> &g) : g(g) {}
  ShortestPath(const Graph<is_directed, Cost> &&) = delete;

  vc<Dist> bfs(int s, int t = -1) { return bfs(array<int, 1>{s}, t); }

  template <class Starts, class = decltype(begin(declval<const Starts &>()))>
  vc<Dist> bfs(const Starts &s, int t = -1)
  {
    init_solve(s, t);
    MyQueue<int> que;
    for (int v : sources) que.push(v);
    while (!que.empty())
    {
      auto v = que.front();
      que.pop();
      if (v == t)
        break;
      fe(e : g.out_edges(v))
      {
        if (chmin(dists[e.to], dists[v] + e.cost))
        {
          prv[e.to] = e;
          que.push(e.to);
        }
      }
    }
    return dists;
  }

  vc<Dist> bfs01(int s, int t = -1) { return bfs01(array<int, 1>{s}, t); }

  template <class Starts, class = decltype(begin(declval<const Starts &>()))>
  vc<Dist> bfs01(const Starts &s, int t = -1)
  {
    const int n = g.size();
    init_solve(s, t);
    vc<int> cur = sources, nxt;
    vc<unsigned char> used(n, false);
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
      fe(e : g.out_edges(v))
      {
        if (chmin(dists[e.to], dists[v] + e.cost))
        {
          prv[e.to] = e;
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
  vc<Dist> dial(int s, int max_cost, int t = -1) { return dial(array<int, 1>{s}, max_cost, t); }

  // 始点集合から、0 以上 max_cost 以下の整数重みについて最短路を求める
  template <class Starts, class = decltype(begin(declval<const Starts &>()))>
  vc<Dist> dial(const Starts &s, int max_cost, int t = -1)
  {
    static_assert(is_integral_ext<Dist>);
    const int n = g.size();
    assert(0 <= max_cost);
    init_solve(s, t);

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

    Dist cur = 0;
    for (int v : sources) push(v, 0);
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
      fec(e : g.out_edges(v))
      {
        Dist nd = dists[v] + e.cost;
        if (nd < dists[e.to])
        {
          if (pre[e.to] != -2)
            erase(e.to, int(dists[e.to] % bcnt));
          dists[e.to] = nd, prv[e.to] = e;
          push(e.to, int(nd % bcnt));
        }
      }
    }
    return dists;
  }

  vc<Dist> dijkstra(int s, int t = -1) { return dijkstra(array<int, 1>{s}, t); }

  template <class Starts, class = decltype(begin(declval<const Starts &>()))>
  vc<Dist> dijkstra(const Starts &s, int t = -1)
  {
    init_solve(s, t);
    pql<pair<Dist, int>> pque;
    for (int v : sources) pque.push({0, v});
    while (!pque.empty())
    {
      auto [d, v] = pque.top();
      pque.pop();
      if (v == t)
        break;
      if (dists[v] != d)
        continue;
      fec(e : g.out_edges(v))
      {
        Dist nd = dists[v] + e.cost;
        if (chmin(dists[e.to], nd))
        {
          prv[e.to] = e;
          pque.push({nd, e.to});
        }
      }
    }
    return dists;
  }

  vc<Dist> dijkstra_dense(int s, int t = -1) { return dijkstra_dense(array<int, 1>{s}, t); }

  template <class Starts, class = decltype(begin(declval<const Starts &>()))>
  vc<Dist> dijkstra_dense(const Starts &s, int t = -1)
  {
    const int n = g.size();
    init_solve(s, t);
    vc<bool> ok(n, false);
    repi(_, n)
    {
      Dist mn = inf();
      int v = -1;
      repi(u, n) if (!ok[u] && chmin(mn, dists[u])) v = u;
      if (v == -1)
        break;
      if (v == t)
        break;
      ok[v] = true;
      fec(e : g.out_edges(v))
      {
        if (chmin(dists[e.to], dists[v] + e.cost))
          prv[e.to] = e;
      }
    }
    return dists;
  }

  vc<Dist> dijkstra_fibonacci(int s, int t = -1) { return dijkstra_fibonacci(array<int, 1>{s}, t); }

  template <class Starts, class = decltype(begin(declval<const Starts &>()))>
  vc<Dist> dijkstra_fibonacci(const Starts &s, int t = -1)
  {
    const int n = g.size();
    init_solve(s, t);
    FibonacciHeap<Dist> que(n);
    for (int v : sources) que.push(v, Dist(0));
    while (!que.empty())
    {
      const auto [d, v] = que.top();
      que.pop();
      if (v == t) break;
      for (const auto &e : g.out_edges(v))
      {
        if constexpr (is_integral_ext<Dist>)
          if (e.cost >= inf() - d) continue;
        const Dist next = d + e.cost;
        if (next < dists[e.to])
        {
          dists[e.to] = next, prv[e.to] = e;
          que.push_or_decrease(e.to, next);
        }
      }
    }
    return dists;
  }

  // 最短のウォークとして求める (いくらでも小さくなるなら -infty)
  // O(nm)
  vc<Dist> bellman_ford(int s) { return bellman_ford(array<int, 1>{s}); }

  // 始点集合から最短のウォークを求める。いくらでも小さくなる距離は -infty。
  template <class Starts, class = decltype(begin(declval<const Starts &>()))>
  vc<Dist> bellman_ford(const Starts &s)
  {
    const int n = g.size();
    init_solve(s, -1);
    repi(t, 2 * n)
    {
      bool changed = false;
      repi(v, n)
      {
        if (dists[v] == inf())
          continue;
        fec(e : g.out_edges(v))
        {
          Dist nd = dists[v] == -inf() ? -inf() : dists[v] + e.cost;
          if (dists[e.to] > nd)
          {
            changed = true;
            prv[e.to] = e;
            if (t == n - 1)
              dists[e.to] = -inf();
            else
              dists[e.to] = nd;
          }
        }
      }
      if (!changed) break;
    }
    repi(v, n) if (dists[v] == -inf()) prv[v] = {};
    return dists;
  }

  // 最短のウォークを求める。到達不能は infty、負閉路の影響を受ける距離は -infty。
  vc<Dist> spfa(int s) { return spfa(array<int, 1>{s}); }

  // 始点集合から最短のウォークを求める。重複した始点は1つとして扱う。
  template <class Starts, class = decltype(begin(declval<const Starts &>()))>
  vc<Dist> spfa(const Starts &s)
  {
    init_solve(s, -1);
    internal::spfa_relax<false>(g, dists, sources, Dist(inf()),
        [&](int v, const Edge<Cost> &e) { prv[v] = e; });
    return dists;
  }

  vc<Dist> solve(int s, int t = -1) { return solve(array<int, 1>{s}, t); }

  template <class Starts, class = decltype(begin(declval<const Starts &>()))>
  vc<Dist> solve(const Starts &s, int t = -1)
  {
    if constexpr (is_void_v<Cost>)
      return bfs(s, t);
    else
    {
      bool neg = false;
      int zcnt = 0;
      Dist wplus1 = -inf();
      Dist max_cost = 0;
      bool wpluscnt_geq2 = false;
      repi(v, g.size()) fec(e : g.out_edges(v))
      {
        if (e.cost < 0)
        {
          neg = true;
          break;
        }
        chmax(max_cost, e.cost);
      }
      repi(v, g.size()) fec(e : g.out_edges(v))
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
        const ll n = g.size(), m = g.num_of_edges();
        if (n * n < (m << 4))
          return dijkstra_dense(s, t);
        else
        {
          if constexpr (is_integral_ext<Dist>)
          {
            const u128 c = u128(max_cost);
            const ull lg = max<ull>(1, bit_width(ull(n)) - 1);
            if (c <= u128(m) * lg / u128(n))
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
  }

  // solve(s, t) の後に呼ぶ
  // prv[v] := s から v への最短パスのひとつで v に行くために使った辺 (なければ from=-1, to=-1, index=-1)
  // (t を指定して打ち切っていた場合、t に到達する最短パスのひとつで使った辺は正しく記録される)
  vc<Edge<Cost>> prev_edges() const
  {
    const int n = g.size();
    assert(SZ(dists) == n && "solve(s, t) is not called");
    return prv;
  }

  // solve(s, t) の後に呼ぶ
  // (sssp の実行で t を指定して打ち切っていても動作する)
  // 有限の最短経路は辺列、距離が ±infty なら nullopt。
  optional<vc<Edge<Cost>>> path_edge(int t) const
  {
    const int n = g.size();
    assert(SZ(dists) == n && "solve(s, t) is not called");
    assert(0 <= t && t < n);
    vc<Edge<Cost>> res;
    if (dists[t] == inf() || dists[t] == -inf()) return nullopt;
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

  // 有限の最短経路の頂点列。両端を含む。
  optional<vc<int>> path_vertex(int t) const
  {
    const int n = g.size();
    assert(SZ(dists) == n && "solve(s, t) is not called");
    assert(0 <= t && t < n);
    if (dists[t] == inf() || dists[t] == -inf()) return nullopt;
    vc<int> res{t};
    while (prv[t].index != -1)
    {
      t = prv[t].from;
      res.eb(t);
    }
    reverse(res.begin(), res.end());
    return res;
  }

  // cnt[v] := 始点集合から v への最短パスの個数
  // (**sssp の実行で t を指定して打ち切っていてはいけない**)
  template <class mint>
  vc<mint> count_paths() const
  {
    const int n = g.size();
    assert(SZ(dists) == n && "solve(s) is not called");
    assert(solved_all && "solve(s, t) must not stop at t");
    for (int s : sources)
      assert(dists[s] != -inf() && "the shortest-path graph must be finite");
    vc<mint> cnt(n, 0);
    vc<int> indeg(n, 0);
    int active = 0;
    repi(v, n) if (dists[v] != inf() && dists[v] != -inf()) active++;
    repi(v, n)
    {
      if (dists[v] == inf() || dists[v] == -inf())
        continue;
      fec(e : g.out_edges(v))
      {
        if (dists[e.to] != inf() && dists[e.to] != -inf() &&
            dists[e.to] == dists[v] + e.cost)
          indeg[e.to]++;
      }
    }
    MyQueue<int> que;
    repi(v, n) if (dists[v] != inf() && dists[v] != -inf() && indeg[v] == 0) que.push(v);
    for (int s : sources) if (dists[s] == 0) cnt[s] = 1;
    int processed = 0;
    while (!que.empty())
    {
      auto v = que.front();
      que.pop();
      processed++;
      fec(e : g.out_edges(v))
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
