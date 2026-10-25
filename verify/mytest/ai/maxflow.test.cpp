#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "flow/maxflow.hpp"
#include <atcoder/maxflow>

using Input = vector<tuple<int, int, long long>>;

template <class Cap>
Cap solve(MaxFlow<Cap> &g, int mode, int s, int t, Cap limit = numeric_limits<Cap>::max())
{
  if (mode == 0) return g.flow(s, t, limit);
  if (mode == 1) return g.dinic(s, t, limit);
  return g.push_relabel(s, t, limit);
}

template <class Cap>
void check_state(const MaxFlow<Cap> &g, int s, int t, Cap value, bool maximal)
{
  vector<i128> balance(g.template size<int>(), 0);
  const auto es = g.edges();
  for (int i = 0; i < int(es.size()); i++)
  {
    const auto e = g.get_edge(i);
    assert(e.from == es[i].from && e.to == es[i].to && e.cap == es[i].cap && e.flow == es[i].flow);
    assert(Cap(0) <= e.flow && e.flow <= e.cap);
    balance[e.from] += i128(e.flow), balance[e.to] -= i128(e.flow);
  }
  for (int v = 0; v < int(balance.size()); v++)
    assert(balance[v] == (v == s ? i128(value) : v == t ? -i128(value) : 0));
  if (maximal)
  {
    const auto cut = g.min_cut(s);
    assert(cut[s] && !cut[t]);
    i128 capacity = 0;
    for (auto e : es) if (cut[e.from] && !cut[e.to]) capacity += i128(e.cap);
    assert(capacity == i128(value));
  }
}

long long brute_cut(int n, const Input &es, int s, int t)
{
  long long result = numeric_limits<long long>::max();
  for (int mask = 0; mask < (1 << n); mask++) if ((mask >> s & 1) && !(mask >> t & 1))
  {
    long long c = 0;
    for (auto [u, v, cap] : es) if ((mask >> u & 1) && !(mask >> v & 1)) c += cap;
    result = min(result, c);
  }
  return result;
}

void small_random()
{
  mt19937_64 rng(8152026);
  for (int trial = 0; trial < 4000; trial++)
  {
    int n = 2 + rng() % 8, m = rng() % 70;
    int s = rng() % n, t = (s + 1 + rng() % (n - 1)) % n;
    Input es;
    for (int i = 0; i < m; i++) es.emplace_back(rng() % n, rng() % n, rng() % 30);
    const auto expected = brute_cut(n, es, s, t);
    for (int mode = 0; mode < 3; mode++)
    {
      MaxFlow<ll> g(n);
      assert(g.size() == n && g.num_of_edges() == 0);
      for (auto [u, v, c] : es) g.add_edge(u, v, c);
      const ll limit = rng() % 60;
      ll f = solve(g, mode, s, t, limit);
      assert(f == min(expected, limit));
      check_state(g, s, t, f, f < limit);
      const auto snapshot = g.edges();
      assert(solve(g, (mode + 1) % 3, s, t, 0LL) == 0);
      for (int i = 0; i < m; i++) assert(g.get_edge(i).flow == snapshot[i].flow);
      f += solve(g, (mode + 1) % 3, s, t);
      assert(f == expected);
      check_state(g, s, t, f, true);
      assert(solve(g, (mode + 2) % 3, s, t) == 0);
      // 同じ残余グラフで逆方向に流すと、元のフローの取り消しも行える。
      const ll back = solve(g, mode, t, s, f);
      assert(back == f);
      check_state(g, s, t, 0LL, false);
    }
  }
}

void medium_random()
{
  mt19937_64 rng(1830339);
  for (int trial = 0; trial < 250; trial++)
  {
    const int n = 130 + rng() % 250, m = 600 + rng() % 5000;
    Input es;
    const ll maximum = trial % 3 == 0 ? 1 : trial % 3 == 1 ? 1000 : 1000000000000LL;
    for (int i = 0; i < m; i++) es.emplace_back(rng() % n, rng() % n, rng() % (maximum + 1));
    atcoder::mf_graph<ll> reference(n);
    for (auto [u, v, c] : es) reference.add_edge(u, v, c);
    const ll expected = reference.flow(0, n - 1);
    for (int mode = 0; mode < 3; mode++)
    {
      MaxFlow<ll> g(n);
      for (auto [u, v, c] : es) g.add_edge(u, v, c);
      ll total = 0;
      for (int call = 0; call < 3; call++)
      {
        const ll limit = (call + 1) * (expected / 5 + 1);
        const ll f = solve(g, (mode + call) % 3, 0, n - 1, limit);
        assert(f == min(expected - total, limit));
        total += f;
        check_state(g, 0, n - 1, total, false);
      }
      total += solve(g, mode, 0, n - 1);
      assert(total == expected);
      check_state(g, 0, n - 1, total, true);
      // 辺の追加後も同じ辺番号と残余グラフを使う。
      const int id = g.add_edge(0, n - 1, 100);
      assert(id == m && g.num_of_edges() == m + 1);
      assert(solve(g, mode, 0, n - 1) == 100);
      check_state(g, 0, n - 1, total + 100, true);
    }
  }
}

void mutations()
{
  mt19937_64 rng(417738);
  for (int trial = 0; trial < 100; trial++)
  {
    const int n = 2 + rng() % 30;
    MaxFlow<ll> g(n);
    atcoder::mf_graph<ll> reference(n);
    for (int i = 0; i < 100; i++)
    {
      int u = rng() % n, v = rng() % n;
      ll c = rng() % 100;
      assert(g.add_edge(u, v, c) == reference.add_edge(u, v, c));
    }
    for (int step = 0; step < 40; step++)
    {
      const int id = rng() % 100;
      const ll cap = rng() % 100, f = rng() % (cap + 1);
      g.change_edge(id, cap, f);
      reference.change_edge(id, cap, f);
      assert(g.min_cut(0) == reference.min_cut(0));
      // 任意の change_edge にも ACL と同じ残余ネットワークとして応答する。
      const int s = rng() % n, t = (s + 1 + rng() % (n - 1)) % n;
      const ll limit = rng() % 100;
      assert(solve(g, step % 3, s, t, limit) == reference.flow(s, t, limit));
      // 最適解の各辺の流れ方は方式ごとに異なるため、変更後の状態を同期する。
      for (int i = 0; i < 100; i++)
      {
        auto e = g.get_edge(i);
        reference.change_edge(i, e.cap, e.flow);
      }
    }
  }
}

template <class Cap>
void large_capacities()
{
  const Cap mx = numeric_limits<Cap>::max();
  for (int mode = 0; mode < 3; mode++)
  {
    MaxFlow<Cap> g(5);
    // 始点から出る容量の総和は Cap を超える。答えは Cap に収まる。
    g.add_edge(0, 0, mx);
    g.add_edge(0, 1, mx);
    g.add_edge(0, 2, mx);
    g.add_edge(1, 3, mx);
    g.add_edge(2, 3, mx);
    g.add_edge(3, 4, mx);
    const Cap limit = mx / 3;
    assert(solve(g, mode, 0, 4, limit) == limit);
    assert(solve(g, (mode + 1) % 3, 0, 4) == mx - limit);
    assert(solve(g, mode, 0, 4) == 0);
    assert(g.get_edge(0).flow == 0);
    // 到達できない終点に向けて供給した余剰も、始点へすべて返す。
    MaxFlow<Cap> dead(4);
    dead.add_edge(0, 1, mx), dead.add_edge(1, 2, mx);
    assert(solve(dead, mode, 0, 3) == 0);
    assert(dead.get_edge(0).flow == 0 && dead.get_edge(1).flow == 0);
  }
}

void deep_paths()
{
  constexpr int n = 200000;
  for (int mode = 0; mode < 3; mode++)
  {
    MaxFlow<int> g(n);
    for (int i = 0; i + 1 < n; i++) g.add_edge(i, i + 1, 3);
    assert(solve(g, mode, 0, n - 1, 2) == 2);
    assert(solve(g, mode, 0, n - 1) == 1);
    check_state(g, 0, n - 1, 3, true);
  }
}

void layered()
{
  // 長い経路、狭いカット、取り消しを含む残余辺で自動切替も検証する。
  constexpr int layers = 35, width = 20, n = layers * width + 2;
  for (int pattern = 0; pattern < 8; pattern++)
  {
    MaxFlow<ll> g(n);
    atcoder::mf_graph<ll> reference(n);
    auto add = [&](int u, int v, ll c) { g.add_edge(u, v, c), reference.add_edge(u, v, c); };
    for (int j = 0; j < width; j++) add(0, 1 + j, 1000 + j);
    for (int k = 0; k + 1 < layers; k++)
      for (int i = 0; i < width; i++) for (int j = 0; j < width; j++)
        if ((i + 3 * j + pattern) % (pattern + 2) == 0)
          add(1 + k * width + i, 1 + (k + 1) * width + j, 2 + (i + j) % 100);
    for (int j = 0; j < width; j++) add(1 + (layers - 1) * width + j, n - 1, 1000 + j);
    const ll expected = reference.flow(0, n - 1);
    for (int mode = 0; mode < 3; mode++)
    {
      auto copy = g;
      const ll f = solve(copy, mode, 0, n - 1);
      assert(f == expected);
      check_state(copy, 0, n - 1, f, true);
    }
  }
}

void adaptive_continuation()
{
  constexpr int n = 2500;
  MaxFlow<ll> base(n);
  for (int i = 0; i + 1 < n - 1; i++) base.add_edge(i, i + 1, n);
  for (int i = 1; i < n - 1; i++) base.add_edge(i, n - 1, 1);
  for (ll limit : {700LL, 2000LL, 10000LL})
  {
    auto g = base;
    const ll f = g.flow(0, n - 1, limit);
    assert(f == min<ll>(n - 2, limit));
    check_state(g, 0, n - 1, f, f < limit);
    const ll rest = g.dinic(0, n - 1);
    assert(f + rest == n - 2);
    check_state(g, 0, n - 1, f + rest, true);
    assert(g.push_relabel(n - 1, 0, f + rest) == f + rest);
    check_state(g, 0, n - 1, 0LL, false);
  }
}

void anti_dinic()
{
  // No.3668 の最大制約内の Zadeh 型ネットワーク。最小カット値は k^2 * phases。
  constexpr int k = 83, phases = 458, len = 2 * phases - 1;
  constexpr ll expected = ll(k) * k * phases;
  int n = 0;
  auto vertices = [&](int size)
  {
    vector<int> vs(size);
    for (int &v : vs) v = n++;
    return vs;
  };
  auto source = vertices(len), p = vertices(2), q = vertices(2);
  auto a = vertices(k), b = vertices(k), sink = vertices(len);
  reverse(sink.begin(), sink.end());
  MaxFlow<ll> base(n);
  for (int j = 1; j < len; j++)
    base.add_edge(source[j - 1], source[j], expected), base.add_edge(sink[j], sink[j - 1], expected);
  for (int phase = 0; phase < phases; phase++)
    base.add_edge(source[2 * phase], p[phase % 2], k * k),
    base.add_edge(q[phase % 2], sink[2 * phase], k * k);
  for (int i = 0; i < k; i++)
  {
    base.add_edge(p[0], a[i], expected), base.add_edge(p[1], b[i], expected);
    base.add_edge(a[i], q[1], expected), base.add_edge(b[i], q[0], expected);
    for (int j = 0; j < k; j++) base.add_edge(a[i], b[j], 1);
  }
  assert(n == 2000 && base.num_of_edges() == 9965);
  MaxFlow<ll> parallel(n);
  for (const auto &e : base.edges())
    parallel.add_edge(e.from, e.to, e.cap / 2), parallel.add_edge(e.from, e.to, e.cap - e.cap / 2);
  // 集約後に方式を切り替えた場合も、元の辺へ流量を戻す。
  for (const auto *initial : {&base, &parallel}) for (int mode : {0, 2})
  {
    auto g = *initial;
    const ll first = solve(g, mode, 0, n - 1, expected / 2);
    assert(first == expected / 2);
    check_state(g, 0, n - 1, first, false);
    const ll rest = solve(g, mode, 0, n - 1);
    assert(first + rest == expected);
    check_state(g, 0, n - 1, expected, true);
  }
}

void selection_regressions()
{
  // 中間辺だけ大容量にしたマッチングと、容量の一様な拡大・小さい頂点容量。
  // 大容量の行き止まりを加えた場合も、分割実行と保存則を確認する。
  constexpr int side = 8000, t = 2 * side + 1, n = t + 4;
  for (int pattern = 0; pattern < 5; pattern++)
  {
    mt19937_64 rng(1);
    MaxFlow<ll> base(n);
    atcoder::mf_graph<ll> reference(n);
    auto add = [&](int u, int v, ll cap) { base.add_edge(u, v, cap), reference.add_edge(u, v, cap); };
    for (int i = 0; i < side; i++)
    {
      const ll cap = pattern == 1 ? 1000000 : pattern == 2 ? 1 + rng() % 8 : 1;
      add(0, 1 + i, cap), add(1 + side + i, t, cap);
    }
    for (int i = 0; i < side; i++) for (int j = 0; j < 3; j++)
      add(1 + i, 1 + side + rng() % side, 1000000000);
    if (pattern == 3)
      for (int i = 0; i < 3; i++) add(t + 1 + i, t + 1 + (i + 1) % 3, 1000000000);
    if (pattern == 4)
    {
      // 上限と局所的な頂点容量を大きくし、行き止まりを除いて再評価する例。
      add(0, t + 1, 1000000000), add(t + 2, t, 1000000000);
      for (int i = 2; i <= 50; i++) add(1, i, 1000000000), add(i, 1, 1000000000);
    }
    const ll expected = reference.flow(0, t);
    auto g = base;
    assert(g.flow(0, t) == expected);
    check_state(g, 0, t, expected, true);
    g = base;
    const ll first = g.flow(0, t, expected / 3);
    assert(first == expected / 3);
    check_state(g, 0, t, first, false);
    assert(first + g.flow(0, t) == expected);
    check_state(g, 0, t, expected, true);
    assert(g.flow(t, 0, expected) == expected);
    check_state(g, 0, t, 0LL, false);
  }
  // 再帰を使う短い探索と、明示的なスタックを使う深い探索の境界。
  for (int length : {63, 64, 65, 66})
  {
    MaxFlow<int> g(length + 1);
    for (int v = 0; v < length; v++)
      g.add_edge(v, v + 1, 1), g.add_edge(v, v + 1, 2);
    assert(g.flow(0, length, 2) == 2);
    assert(g.flow(0, length) == 1);
    check_state(g, 0, length, 3, true);
  }
  for (auto [paths, bottleneck] : array<pair<int, ll>, 3>{{{3, 1}, {12, 1}, {3, 100}}})
  {
    int n = 1;
    Input es;
    vector<int> ends;
    for (int i = 0; i < paths; i++)
    {
      int previous = 0, length = 400 + 3 * i;
      for (int j = 0; j < length; j++)
      {
        es.emplace_back(previous, n, j == length / 2 ? bottleneck : j == 1 ? 1000000007 : 1000000000);
        previous = n++;
      }
      ends.push_back(previous);
    }
    for (bool parallel : {false, true})
    {
      MaxFlow<ll> g(n + 1);
      for (auto [u, v, cap] : es)
      {
        if (parallel) g.add_edge(u, v, cap / 2), g.add_edge(u, v, cap - cap / 2);
        else g.add_edge(u, v, cap);
      }
      for (int v : ends) g.add_edge(v, n, 1000000000);
      const ll expected = paths * bottleneck;
      assert(g.flow(0, n) == expected);
      check_state(g, 0, n, expected, true);
      assert(g.flow(n, 0, expected) == expected);
      check_state(g, 0, n, 0LL, false);
    }
  }
}

void csr_rebuilds()
{
  MaxFlow<ll> empty;
  empty.build();
  MaxFlow<ll> isolated(4);
  const auto cut = isolated.min_cut(1);
  assert(!cut[0] && cut[1] && !cut[2] && !cut[3]);
  isolated.build();
  for (int mode = 0; mode < 3; mode++)
  {
    MaxFlow<ll> g(5);
    g.add_edge(0, 1, 10), g.add_edge(1, 4, 10);
    const int loop = g.add_edge(2, 2, 7);
    g.add_edge(4, 0, 0);
    g.change_edge(0, 10, 2), g.change_edge(1, 10, 2), g.change_edge(loop, 7, 7);
    auto copied = g;
    g.build(), g.build();
    assert(g.get_edge(loop).flow == 7);
    check_state(g, 0, 4, 2LL, false);
    assert(solve(g, mode, 0, 4) == 8);
    assert(solve(copied, mode, 0, 4) == 8);
    check_state(g, 0, 4, 10LL, true);
    check_state(copied, 0, 4, 10LL, true);
  }

  constexpr int n = 160, s = 0, t = n - 1;
  mt19937_64 rng(149630);
  MaxFlow<ll> g(n);
  atcoder::mf_graph<ll> reference(n);
  auto add = [&](int u, int v, ll cap)
  {
    assert(g.add_edge(u, v, cap) == reference.add_edge(u, v, cap));
  };
  for (int i = 0; i < 600; i++) add(rng() % n, rng() % n, rng() % 100);
  ll total = 0;
  for (int round = 0; round < 20; round++)
  {
    for (int i = 0; i < 20; i++) add(rng() % n, rng() % n, rng() % 100);
    const int v = 1 + 7 * round % (n - 2);
    add(s, v, round + 2), add(v, t, round + 3);
    const int id = rng() % g.num_of_edges<int>();
    auto e = g.get_edge(id);
    g.change_edge(id, e.cap + 10, e.flow);
    reference.change_edge(id, e.cap + 10, e.flow);
    auto copy = g;
    const auto before = g.edges();
    const MaxFlow<ll> &cg = g;
    cg.build(), cg.build();
    const auto after = g.edges();
    for (int i = 0; i < int(before.size()); i++)
      assert(before[i].from == after[i].from && before[i].to == after[i].to &&
             before[i].cap == after[i].cap && before[i].flow == after[i].flow);
    const ll limit = round % 3 == 0 ? 10 : numeric_limits<ll>::max();
    const ll expected = reference.flow(s, t, limit);
    const ll f = solve(g, round % 3, s, t, limit);
    assert(f == expected && solve(copy, (round + 1) % 3, s, t, limit) == f);
    total += f;
    check_state(g, s, t, total, f < limit);
    check_state(copy, s, t, total, f < limit);
    for (int i = 0; i < g.num_of_edges<int>(); i++)
    {
      e = g.get_edge(i);
      reference.change_edge(i, e.cap, e.flow);
    }
  }
}

template <class Cap>
void simplified_large_capacities()
{
  const Cap maximum = numeric_limits<Cap>::max(), half = maximum / 2;
  for (int mode = 0; mode < 3; mode++)
  {
    MaxFlow<Cap> g(3);
    for (int i = 0; i < 5; i++)
      g.add_edge(0, 1, half), g.add_edge(1, 2, half);
    const int loop = g.add_edge(1, 1, maximum);
    g.change_edge(loop, maximum, maximum);
    g.build();
    assert(solve(g, mode, 0, 2, maximum) == maximum);
    assert(solve(g, (mode + 1) % 3, 0, 2, maximum) == maximum);
    const Cap rest = (maximum - 5) / 2;
    assert(solve(g, (mode + 2) % 3, 0, 2, maximum) == rest);
    for (int i = 0; i < 10; i++) assert(g.get_edge(i).flow == half);
    assert(g.get_edge(loop).flow == maximum);
    assert(solve(g, mode, 2, 0, maximum) == maximum);
    assert(solve(g, mode, 0, 2, maximum) == maximum);
    for (int i = 0; i < 10; i++) assert(g.get_edge(i).flow == half);
  }
}

void simplified_edges()
{
  for (int mode = 0; mode < 3; mode++)
  {
    MaxFlow<ll> g(4);
    g.add_edge(0, 1, 7), g.add_edge(0, 1, 11), g.add_edge(0, 1, 13);
    g.add_edge(1, 3, 31);
    const int zero = g.add_edge(0, 2, 0);
    g.add_edge(2, 3, 20);
    const int loop = g.add_edge(2, 2, 100);
    // 構築・追加流量 0 の探索では、各辺に設定した流量も保つ。
    g.change_edge(0, 7, 2), g.change_edge(1, 11, 3), g.change_edge(2, 13, 4);
    g.change_edge(3, 31, 9), g.change_edge(loop, 100, 37);
    g.build();
    assert(solve(g, mode, 0, 3, 0LL) == 0);
    assert(g.get_edge(0).flow == 2 && g.get_edge(1).flow == 3 && g.get_edge(2).flow == 4);
    check_state(g, 0, 3, 9LL, false);
    assert(solve(g, mode, 0, 3, 5LL) == 5);
    check_state(g, 0, 3, 14LL, false);
    assert(g.get_edge(loop).flow == 37);
    g.change_edge(zero, 20, 0);
    const auto e = g.get_edge(1);
    g.change_edge(1, 17, e.flow);
    g.change_edge(3, 37, 14);
    auto copy = g;
    const auto before = g.edges();
    g.build();
    for (int i = 0; i < g.num_of_edges<int>(); i++)
      assert(g.get_edge(i).cap == before[i].cap && g.get_edge(i).flow == before[i].flow);
    assert(solve(g, mode, 0, 3) == 43);
    assert(solve(copy, (mode + 1) % 3, 0, 3) == 43);
    check_state(g, 0, 3, 57LL, true);
    check_state(copy, 0, 3, 57LL, true);
    assert(solve(g, (mode + 2) % 3, 3, 0, 57LL) == 57);
    check_state(g, 0, 3, 0LL, false);
    assert(g.get_edge(loop).flow == 37);
    // 集約辺の一部を容量 0 にした後の再構築。
    g.change_edge(0, 0, 0);
    assert(solve(g, mode, 0, 3) == 50);
    check_state(g, 0, 3, 50LL, true);
  }
  simplified_large_capacities<signed char>();
  simplified_large_capacities<unsigned char>();
  simplified_large_capacities<short>();
  simplified_large_capacities<unsigned short>();
  simplified_large_capacities<int>();
  simplified_large_capacities<unsigned int>();
  simplified_large_capacities<ll>();
  simplified_large_capacities<unsigned long long>();
  simplified_large_capacities<i128>();
  simplified_large_capacities<u128>();
}

int main()
{
  MaxFlow<ll> empty;
  assert(empty.size() == 0 && empty.edges().empty());
  empty = MaxFlow<ll>(2);
  assert(empty.flow(0, 1) == 0);
  small_random();
  medium_random();
  mutations();
  large_capacities<signed char>();
  large_capacities<unsigned char>();
  large_capacities<short>();
  large_capacities<unsigned short>();
  large_capacities<int>();
  large_capacities<unsigned int>();
  large_capacities<ll>();
  large_capacities<unsigned long long>();
  large_capacities<i128>();
  large_capacities<u128>();
  deep_paths();
  layered();
  adaptive_continuation();
  anti_dinic();
  selection_regressions();
  csr_rebuilds();
  simplified_edges();
  cout << "Hello World\n";
}
