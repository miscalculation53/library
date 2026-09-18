#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "graph/minimum_cycle.hpp"

// 全単純閉路の列挙と比較し、値・頂点の単純性・辺番号・向きを検証する。
struct BruteResult
{
  ll cost = INF;
  vl edges, vertices;
};

template <bool directed>
BruteResult brute_force(int n, const vc<tuple<int, int, ll>> &es)
{
  Graph<directed, ll> g(n, es);
  BruteResult ans{INF, vl(es.size(), INF), vl(n, INF)};
  vc<bool> used_vertex(n), used_edge(es.size());
  repi(s, n)
  {
    auto dfs = [&](auto &dfs, int v, ll sum) -> void
    {
      fec(e : g.out_edges(v))
      {
        if (used_edge[e.index]) continue;
        if (e.to == s)
        {
          const ll cost = sum + e.cost;
          chmin(ans.cost, cost);
          chmin(ans.edges[e.index], cost);
          repi(i, es.size()) if (used_edge[i]) chmin(ans.edges[i], cost);
          repi(i, n) if (used_vertex[i]) chmin(ans.vertices[i], cost);
          continue;
        }
        if (used_vertex[e.to]) continue;
        used_vertex[e.to] = used_edge[e.index] = true;
        dfs(dfs, e.to, sum + e.cost);
        used_vertex[e.to] = used_edge[e.index] = false;
      }
    };
    used_vertex[s] = true;
    dfs(dfs, s, 0);
    used_vertex[s] = false;
  }
  return ans;
}

template <bool directed, class Cost, class EdgeCost>
void check_result(int n, const vc<tuple<int, int, Cost>> &es,
                  const pair<Cost, vc<Edge<EdgeCost>>> &result, Cost expected, Cost inf)
{
  const auto &[weight, cycle] = result;
  assert(weight == expected);
  if (weight == inf || weight == -inf)
  {
    assert(cycle.empty());
    return;
  }
  assert(!cycle.empty());
  vc<bool> vertices(n), edges(es.size());
  int cur = cycle.front().from;
  Cost sum = 0;
  fec(e : cycle)
  {
    assert(e.from == cur);
    assert(0 <= cur && cur < n && !vertices[cur]);
    assert(0 <= e.index && e.index < int(es.size()) && !edges[e.index]);
    vertices[cur] = edges[e.index] = true;
    auto [u, v, w] = es[e.index];
    bool correct = e.from == u && e.to == v;
    if constexpr (!directed) correct |= e.from == v && e.to == u;
    assert(correct && e.cost == w);
    sum += e.cost;
    cur = e.to;
  }
  assert(cur == cycle.front().from && sum == weight);
}

template <bool directed, class Cost = ll>
void check(int n, const vc<tuple<int, int, ll>> &es)
{
  const auto brute = brute_force<directed>(n, es);
  const auto input = [&]
  {
    if constexpr (is_void_v<Cost>)
    {
      vc<pair<int, int>> uv;
      for (auto [u, v, w] : es) { assert(w == 1); uv.eb(u, v); }
      return Graph<directed>(n, uv);
    }
    else return Graph<directed, Cost>(n, es);
  }();
  const MinimumCycle g(input);
  static_assert(is_same_v<typename MinimumCycle<directed, Cost>::Result, pair<ll, vc<Edge<Cost>>>>);
  check_result<directed>(n, es, g.solve(), brute.cost, ll(INF));
  assert(g.edge_costs() == brute.edges);
  assert(g.vertex_costs() == brute.vertices);
  repi(v, n)
  {
    const auto result = g.solve_vertex(v);
    check_result<directed>(n, es, result, brute.vertices[v], ll(INF));
    if (result.first != INF) assert(result.second.front().from == v);
  }
  repi(eid, es.size())
  {
    const auto result = g.solve_edge(eid);
    check_result<directed>(n, es, result, brute.edges[eid], ll(INF));
    if (result.first != INF) assert(result.second.front().index == eid);
  }
}

template <bool directed>
void tests()
{
  check<directed>(0, {});
  check<directed>(5, {});
  check<directed>(1, {{0, 0, 7}, {0, 0, 3}});
  check<directed>(1, {{0, 0, 0}});
  check<directed>(2, {{0, 1, 0}}); // 0 重みでも無向辺の往復は不可
  check<directed>(2, {{0, 1, 5}, {0, 1, 2}, {1, 0, 3}});
  check<directed>(4, {{0, 1, 1}, {1, 2, 2}, {1, 3, 3}, {0, 2, 4}, {3, 0, 5}});
  check<directed>(3, {{0, 1, 1000000000LL}, {1, 2, 1000000000LL}, {2, 0, 1000000000LL}});
  check<directed>(7, {{0, 1, 0}, {1, 2, 0}, {2, 3, 0}, {3, 1, 0},
                    {4, 5, 1}, {5, 6, 1}, {6, 4, 1}}); // 0 の共通接頭辞を除く
  check<directed>(5, {{0, 1, 10}, {1, 2, 1}, {2, 3, 1}, {3, 1, 1}});
  check<directed>(5, {{0, 1, 1}, {0, 2, 1}, {1, 3, 1}, {2, 3, 1}, {3, 4, 0}, {4, 0, 1}});
  // APSP 側が全始点 SSSP を選ぶ疎グラフ。
  check<directed>(80, {{0, 1, 2}, {1, 2, 3}, {2, 0, 4}});

  // 単純グラフの全列挙（無向 n=5、有向 n=4）。
  const int n = directed ? 4 : 5;
  vc<pair<int, int>> possible;
  repi(u, n) repi(v, n) if (directed ? u != v : u < v) possible.eb(u, v);
  repi(mask, 1 << possible.size())
  {
    vc<tuple<int, int, ll>> es;
    repi(i, possible.size()) if (mask >> i & 1) es.eb(possible[i].first, possible[i].second, 1);
    check<directed>(n, es);
    check<directed, void>(n, es);
  }

  // 非連結・自己ループ・多重辺・0 重み・同距離を含む固定 seed の乱択。
  mt19937 rng(20260907 + directed);
  repi(tc, 1200)
  {
    int n = 1 + rng() % 7, m = rng() % 15;
    vc<tuple<int, int, ll>> es;
    repi(i, m)
    {
      ll w = tc % 3 == 0 ? 3 : tc % 3 == 1 ? ll(rng() % 2) * 5 : ll(rng() % 11);
      es.eb(rng() % n, rng() % n, w);
    }
    check<directed>(n, es);
  }
}

template <bool directed>
void unweighted_tests()
{
  check<directed, void>(0, {});
  check<directed, void>(5, {});
  check<directed, void>(1, {{0, 0, 1}, {0, 0, 1}});
  check<directed, void>(2, {{0, 1, 1}});
  check<directed, void>(2, {{0, 1, 1}, {0, 1, 1}, {1, 0, 1}});
  check<directed, void>(7, {{0, 1, 1}, {1, 2, 1}, {2, 0, 1}, {2, 3, 1}, {4, 5, 1}});
  mt19937 rng(20260913 + directed);
  repi(tc, 300)
  {
    int n = 1 + rng() % 7, m = rng() % 15;
    vc<tuple<int, int, ll>> es;
    repi(i, m) es.eb(rng() % n, rng() % n, 1);
    check<directed, void>(n, es);
  }
}

constexpr int int_infinity() { return 1000000000; }

void negative_cycle_reports()
{
  // 全体版は Floyd / Johnson のどちらで検出しても同じ報告を返す。
  for (int n : {6, 80})
  {
    vc<tuple<int, int, ll>> es = {
        {0, 1, 2}, {1, 0, 3}, {2, 3, -2}, {3, 2, 1}, {4, 2, 0}, {3, 5, 0}};
    GraphDirected<ll> input(n, es);
    const MinimumCycle mc(input);
    check_result<true>(n, es, mc.solve(), -ll(INF), ll(INF));
    // 全要素 -infty は一括計算を打ち切った印。非連結な成分の要素も含む。
    assert(mc.vertex_costs() == vl(n, -INF));
    assert(mc.edge_costs() == vl(es.size(), -INF));
    check_result<true>(n, es, mc.solve_vertex(0), 5LL, ll(INF));
    check_result<true>(n, es, mc.solve_edge(0), 5LL, ll(INF));
    for (int s : {2, 3, 4})
      check_result<true>(n, es, mc.solve_vertex(s), -ll(INF), ll(INF));
    // 指定辺を足すと負閉路になる場合も、-infty で報告する。
    for (int eid : {2, 3})
      check_result<true>(n, es, mc.solve_edge(eid), -ll(INF), ll(INF));
    // 帰路の探索中に負閉路を検出すれば、帰路の終点への到達性によらず報告する。
    check_result<true>(n, es, mc.solve_edge(4), -ll(INF), ll(INF));
    check_result<true>(n, es, mc.solve_vertex(5), ll(INF), ll(INF));
    check_result<true>(n, es, mc.solve_edge(5), ll(INF), ll(INF));
    // 正の閉路と、そこから到達できる負閉路が共存する場合。
    es.eb(1, 2, 0);
    input = GraphDirected<ll>(n, es);
    check_result<true>(n, es, mc.solve_vertex(0), -ll(INF), ll(INF));
    check_result<true>(n, es, mc.solve_edge(0), -ll(INF), ll(INF));
  }

  // 自己ループと明示した infty。通常の閉路復元へ進まず空の辺列を返す。
  vc<tuple<int, int, int>> es = {{0, 0, -1}};
  GraphDirected<int> input(2, es);
  const MinimumCycle<true, int, int_infinity> mc(input);
  for (const auto &res : {mc.solve(), mc.solve_vertex(0), mc.solve_edge(0)})
    check_result<true>(2, es, res, -int_infinity(), int_infinity());
  assert(mc.vertex_costs() == vc<int>(2, -int_infinity()));
  assert(mc.edge_costs() == vc<int>(1, -int_infinity()));
  check_result<true>(2, es, mc.solve_vertex(1), int_infinity(), int_infinity());
}

int main()
{
  tests<false>();
  tests<true>();
  unweighted_tests<false>();
  unweighted_tests<true>();
  vc<tuple<int, int, int>> es = {{0, 1, 2}, {1, 2, 3}, {2, 0, 4}};
  Graph<false, int> g_input(3, es);
  MinimumCycle<false, int, 1000000000> g(g_input);
  check_result<false>(3, es, g.solve(), 9, 1000000000);
  check_result<false>(3, es, g.solve_vertex(1), 9, 1000000000);
  check_result<false>(3, es, g.solve_edge(2), 9, 1000000000);
  Graph<true, int> h_input(3, es);
  MinimumCycle<true, int, int_infinity> h(h_input);
  check_result<true>(3, es, h.solve(), 9, 1000000000);
  check_result<true>(3, es, h.solve_vertex(1), 9, 1000000000);
  check_result<true>(3, es, h.solve_edge(2), 9, 1000000000);
  // 有限の最適解より大きい候補の加算を infty で打ち切る。
  GraphUndirected<int> large(3, vc<tuple<int, int, int>>{
      {0, 1, 800000000}, {1, 2, 800000000}, {2, 0, 800000000},
      {0, 0, 1}, {1, 1, 1}, {2, 2, 1}});
  MinimumCycle bounded(large);
  assert(bounded.solve().first == 1 && bounded.solve_vertex(0).first == 1);
  assert((bounded.vertex_costs() == vc<int>{1, 1, 1}));
  Graph<false> unweighted_input(3, vc<pair<int, int>>{{0, 1}, {1, 2}, {2, 0}});
  MinimumCycle unweighted(unweighted_input);
  assert(unweighted.solve().first == 3);
  // APSP 側が Johnson を選ぶ疎グラフ。指定版は SSSP の Bellman–Ford を使う。
  check<true>(80, {{0, 1, -3}, {1, 2, 5}, {2, 0, 1}});
  mt19937 rng(20260908);
  repi(tc, 300)
  {
    int n = 1 + rng() % 7;
    vl h(n);
    for (auto &x : h) x = int(rng() % 11) - 5;
    vc<tuple<int, int, ll>> edges;
    repi(i, 12)
    {
      int u = rng() % n, v = rng() % n;
      edges.eb(u, v, ll(rng() % 6) + h[v] - h[u]);
    }
    check<true>(n, edges);
  }
  negative_cycle_reports();
  PRINT("Hello World");
}
