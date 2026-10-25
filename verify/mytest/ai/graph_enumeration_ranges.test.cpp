#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "graph/triangles.hpp"
#include "graph/cliques.hpp"

using Triple = tuple<int, int, int>;

#if __cplusplus >= 202002L
static_assert(ranges::input_range<const TriangleRange>);
static_assert(ranges::input_range<const CliqueRange<int>>);
#endif
static_assert(is_same_v<iterator_traits<TriangleRange::Iterator>::value_type, Triple>);
static_assert(is_same_v<iterator_traits<CliqueRange<>::Iterator>::value_type, vl>);

template <class Range>
auto collect(const Range &range)
{
  using T = typename iterator_traits<decltype(range.begin())>::value_type;
  return vc<T>(range.begin(), range.end());
}

template <class Range>
void check_iterator(const Range &range)
{
  const auto expected = collect(range);
  assert(collect(range) == expected);
  const auto copy = range;
  assert(collect(copy) == expected);
  assert(range.begin() == range.begin());
  assert(range.begin() == copy.begin());
  auto it = range.begin(), independent = range.begin();
  if (expected.empty())
  {
    assert(it == range.end());
    return;
  }
  assert(*it == expected.front());
  assert(*it.operator->() == expected.front());
  auto old = it++;
  assert(*old == expected.front());
  assert(*independent == expected.front());
  ++independent;
  assert(it == independent);
  for (int i = 1; i < int(expected.size()); ++i, ++it)
  {
    assert(it != range.end());
    assert(*it == expected[i]);
  }
  assert(it == range.end());

  // 同じ範囲での入れ子、break、continue、再走査。
  int i = 0;
  for (const auto &value : range)
  {
    assert(value == expected[i]);
    for (const auto &inner : range)
    {
      assert(inner == expected.front());
      break;
    }
    assert(value == expected[i]);
    if (++i == 3) break;
  }
  assert(i == min(3, int(expected.size())));
  i = 0;
  for (const auto &value : range)
  {
    if (++i % 2) continue;
    assert(value == expected[i - 1]);
  }
  assert(i == int(expected.size()));
}

template <class Cost, bool erasable>
void check_graph(const GraphUndirected<Cost, erasable> &g)
{
  const int n = g.size();
  vc<int> adj(n);
  repi(u, n) for (auto e : g.out_edges(u)) adj[u] |= 1 << e.to;
  vc<int> expected, expected_triangles;
  for (int mask = 1; mask < (1 << n); ++mask)
  {
    bool ok = true;
    repi(u, n) if (mask >> u & 1)
      ok &= ((mask ^ (1 << u)) & ~adj[u]) == 0;
    if (ok)
    {
      expected.push_back(mask);
      if (__builtin_popcount(unsigned(mask)) == 3) expected_triangles.push_back(mask);
    }
  }

  const auto cs = cliques<int>(g);
  const auto ts = triangles(g);
  vc<int> got, got_triangles;
  for (const auto &vs : cs)
  {
    assert(!vs.empty());
    int mask = 0;
    for (int v : vs)
    {
      assert(0 <= v && v < n && !(mask >> v & 1));
      mask |= 1 << v;
    }
    got.push_back(mask);
  }
  for (auto [u, v, w] : ts)
  {
    assert(0 <= u && u < n && 0 <= v && v < n && 0 <= w && w < n);
    assert(u != v && v != w && w != u);
    got_triangles.push_back((1 << u) | (1 << v) | (1 << w));
  }
  sort(got.begin(), got.end());
  sort(got_triangles.begin(), got_triangles.end());
  assert(got == expected);
  assert(got_triangles == expected_triangles);
  check_iterator(cs);
  check_iterator(ts);
}

void check_small_graphs()
{
  // 5頂点以下の単純無向グラフをすべて調べる。
  for (int n = 0; n <= 5; ++n)
  {
    vc<pair<int, int>> all;
    repi(u, n) repi(v, u + 1, n) all.emplace_back(u, v);
    for (int mask = 0; mask < (1 << all.size()); ++mask)
    {
      vc<pair<int, int>> es;
      repi(i, all.size()) if (mask >> i & 1) es.push_back(all[i]);
      check_graph(GraphUndirected<>(n, es));
    }
  }
  mt19937 rng(20260926);
  repi(n, 6, 11) repi(trial, 40)
  {
    vc<tuple<int, int, ll>> es;
    repi(u, n) repi(v, u + 1, n) if (int(rng() % 100) < trial * 3)
      es.emplace_back(u, v, ll(rng()));
    shuffle(es.begin(), es.end(), rng);
    for (auto &[u, v, cost] : es) if (rng() % 2) swap(u, v);
    GraphUndirected<ll, true> g(n, es);
    check_graph(g);
    if (!es.empty())
    {
      g.erase_edge(rng() % es.size());
      check_graph(g);
    }
  }
}

void check_lifetime()
{
  const vc<pair<int, int>> es{{0, 1}, {0, 2}, {0, 3}, {1, 2}, {1, 3}, {2, 3}};
  GraphUndirected<void, true> g(4, es);
  const auto ts = triangles(g);
  const auto cs = cliques(g);
  g.erase_edge(0);
  assert(collect(ts).size() == 4 && collect(cs).size() == 15);
  assert(collect(triangles(g)).size() == 2 && collect(cliques(g)).size() == 11);

  // 元のグラフと範囲の両方が一時オブジェクトでも走査を続けられる。
  auto ti = triangles(GraphUndirected<>(4, es)).begin();
  auto ci = cliques<unsigned>(GraphUndirected<>(4, es)).begin();
  int count = 0;
  for (; ti != TriangleRange::Iterator(); ++ti) ++count;
  assert(count == 4);
  count = 0;
  for (; ci != CliqueRange<unsigned>::Iterator(); ++ci) ++count;
  assert(count == 15);
}

void check_clique_parallel_edges()
{
  // クリーク列挙は従来どおり自己ループ・多重辺をまとめて扱う。
  const GraphUndirected<> g(4, vc<pair<int, int>>{
    {0, 0}, {0, 1}, {1, 0}, {0, 1}, {1, 2}, {2, 0}, {2, 2}, {3, 3}});
  vc<int> masks;
  for (const auto &vs : cliques<int>(g))
  {
    int mask = 0;
    for (int v : vs)
    {
      assert(!(mask >> v & 1));
      mask |= 1 << v;
    }
    masks.push_back(mask);
  }
  sort(masks.begin(), masks.end());
  assert((masks == vc<int>{1, 2, 3, 4, 5, 6, 7, 8}));
}

void check_large_graphs()
{
  // 全結果を先に生成すると終わらない完全グラフで、先頭だけ消費する。
  vc<pair<int, int>> es;
  repi(u, 64) repi(v, u + 1, 64) es.emplace_back(u, v);
  const GraphUndirected<> complete(64, es);
  int count = 0;
  for (const auto &vs : cliques<int>(complete))
  {
    assert(!vs.empty());
    if (++count == 128) break;
  }
  assert(count == 128);
  count = 0;
  for (auto [u, v, w] : triangles(complete))
  {
    assert(u != v && v != w && w != u);
    if (++count == 128) break;
  }
  assert(count == 128);

  const int n = 20000;
  for (bool star : {false, true})
  {
    es.clear();
    if (star) repi(v, 1, n) es.emplace_back(0, v);
    const GraphUndirected<> g(n, es);
    count = 0;
    for (const auto &vs : cliques<int>(g))
    {
      assert(vs.size() == 1 || (star && vs.size() == 2));
      ++count;
    }
    assert(count == (star ? 2 * n - 1 : n));
    const auto ts = triangles(g);
    assert(ts.begin() == ts.end());
  }
}

int main()
{
  check_small_graphs();
  check_lifetime();
  check_clique_parallel_edges();
  check_large_graphs();
  cout << "Hello World" << endl;
}
