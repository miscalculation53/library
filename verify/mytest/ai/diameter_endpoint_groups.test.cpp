#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "tree/diameter.hpp"

vc<pair<int, int>> decode_tree(int n, int code)
{
  vc<int> sequence(max(0, n - 2)), degree(n, 1);
  for (int &v : sequence) v = code % n, code /= n, ++degree[v];
  vc<pair<int, int>> es;
  for (int v : sequence)
  {
    int leaf = 0;
    while (degree[leaf] != 1) ++leaf;
    es.eb(leaf, v);
    --degree[leaf], --degree[v];
  }
  if (n >= 2)
  {
    int u = 0;
    while (degree[u] != 1) ++u;
    int v = u + 1;
    while (degree[v] != 1) ++v;
    es.eb(u, v);
  }
  return es;
}

void check(int n, const vc<pair<int, int>> &es, const vc<ll> &weights)
{
  vvc<pair<int, ll>> adj(n);
  repi(i, es.size())
  {
    auto [u, v] = es[i];
    adj[u].eb(v, weights[i]), adj[v].eb(u, weights[i]);
  }
  vvc<ll> ds(n, vc<ll>(n));
  ll diameter = 0;
  repi(s, n)
  {
    vc<int> par(n, -1), que{s};
    par[s] = s;
    for (int i = 0; i < int(que.size()); ++i)
      for (auto [to, w] : adj[que[i]]) if (to != par[que[i]])
      {
        par[to] = que[i];
        ds[s][to] = ds[s][que[i]] + w;
        diameter = max(diameter, ds[s][to]);
        que.eb(to);
      }
  }
  const Diameter d(n, es, weights);
  const auto groups = d.endpoint_groups();
  vc<int> membership(n, -1);
  int previous = -1;
  repi(i, groups.size())
  {
    const auto &group = groups[i];
    assert(!group.empty() && previous < group.front() && is_sorted(ALL(group)));
    previous = group.front();
    for (int v : group)
    {
      assert(0 <= v && v < n && membership[v] == -1);
      membership[v] = i;
    }
  }
  repi(v, n) assert((membership[v] != -1) == (*max_element(ALL(ds[v])) == diameter));
  ll expected = 0;
  repi(u, n) repi(v, u + 1, n)
  {
    assert((membership[u] != -1 && membership[v] != -1 && membership[u] != membership[v]) == (ds[u][v] == diameter));
    if (u % 3 == v % 3 && ds[u][v] == diameter) ++expected;
  }
  ll answer = 0;
  array<ll, 3> seen{};
  for (const auto &group : groups)
  {
    array<ll, 3> local{};
    for (int v : group) ++local[v % 3];
    repi(c, 3) answer += seen[c] * local[c], seen[c] += local[c];
  }
  assert(answer == expected);
  if (all_of(ALL(weights), [](ll w) { return w == 1; }))
    assert(Diameter(n, es).endpoint_groups() == groups);
  auto copy = d;
  assert(move(copy).endpoint_groups() == groups);
}

int main()
{
  repi(n, 1, 6)
  {
    int trees = 1, assignments = 1;
    repi(i, max(0, n - 2)) trees *= n;
    repi(i, n - 1) assignments *= 3;
    repi(code, trees)
    {
      auto es = decode_tree(n, code);
      repi(assignment, assignments)
      {
        vc<ll> weights(n - 1);
        int value = assignment;
        for (ll &w : weights) w = value % 3, value /= 3;
        check(n, es, weights);
      }
    }
  }
  const vc<pair<int, int>> zero_center{{0, 1}, {0, 2}, {0, 3}, {1, 4}, {1, 5}};
  const Diameter d(6, zero_center, vc<ll>{0, 1, 1, 1, 1});
  assert(d.endpoint_groups() == vvc<int>({{2}, {3}, {4}, {5}}));
  auto reversed_edges = zero_center;
  reverse(ALL(reversed_edges));
  for (auto &[u, v] : reversed_edges) swap(u, v);
  assert(Diameter(6, reversed_edges, vc<ll>{1, 1, 1, 1, 0}).endpoint_groups() == d.endpoint_groups());
  auto [centered, costs2] = d.virtual_center_tree();
  assert(centered.size() == 6 && centered.root() == 0);
  assert(centered.parent(4) == 1 && centered.parent(5) == 1);
  assert(centered.lca(4, 5) != centered.root());
  const Diameter all_zero(6, zero_center, vc<ll>(5, 0));
  assert(all_zero.endpoint_groups() == vvc<int>({{0}, {1}, {2}, {3}, {4}, {5}}));
  mt19937 rng(20261106);
  repi(trial, 200)
  {
    int n = 6 + rng() % 35;
    vc<int> labels(n);
    iota(ALL(labels), 0);
    shuffle(ALL(labels), rng);
    vc<pair<int, int>> es;
    vc<ll> weights;
    repi(v, 1, n) es.eb(labels[v], labels[rng() % v]), weights.eb(rng() % 8);
    check(n, es, weights);
  }
  for (int mode = 0; mode < 3; ++mode)
  {
    const int n = 200000;
    vc<pair<int, int>> es;
    repi(v, 1, n) es.eb(mode == 2 ? 0 : v - 1, v);
    const Diameter d(n, es, vc<ll>(n - 1, mode == 0 ? 0 : 1));
    const auto groups = d.endpoint_groups();
    if (mode == 0)
    {
      assert(groups.size() == size_t(n));
      repi(v, n) assert(groups[v] == vc<int>{v});
    }
    else if (mode == 1) assert(groups == vvc<int>({{0}, {n - 1}}));
    else
    {
      assert(groups.size() == size_t(n - 1));
      repi(v, 1, n) assert(groups[v - 1] == vc<int>{v});
    }
  }
  cout << "Hello World" << endl;
}
