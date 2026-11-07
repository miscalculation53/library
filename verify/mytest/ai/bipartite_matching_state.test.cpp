#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "flow/bipartite_matching_state.hpp"
#include "flow/bipartite_matching.hpp"

bool cover(const vvc<int> &adj, const vc<int> &left, int i = 0, int used = 0)
{
  if (i == int(left.size())) return true;
  for (int v : adj[left[i]]) if (!(used >> v & 1))
    if (cover(adj, left, i + 1, used | (1 << v))) return true;
  return false;
}

void check_matching(const BipartiteMatchingState &state, const vvc<int> &adj)
{
  int count = 0;
  for (int u = 0; u < state.left_size(); u++) if (state.left_mate()[u] != -1)
  {
    int v = state.left_mate()[u];
    assert(state.right_mate()[v] == u);
    assert(find(ALL(adj[u]), v) != adj[u].end());
    count++;
  }
  for (int v = 0; v < state.right_size(); v++) if (state.right_mate()[v] != -1)
    assert(state.left_mate()[state.right_mate()[v]] == v);
  assert(count == state.matching_size());
}

int main()
{
  mt19937_64 rng(20261102);
  for (int trial = 0; trial < 500; trial++)
  {
    const int n = rng() % 8, r = rng() % 7;
    vvc<int> adj(n);
    for (auto &row : adj) for (int v = 0; v < r; v++)
      if (rng() % 3 == 0)
      {
        row.eb(v);
        if (rng() % 2) row.eb(v);
      }
    BipartiteMatchingState state(r, adj);
    for (int step = 0; step < 60 && n; step++)
    {
      int e = rng() % n;
      if (state.left_mate()[e] != -1)
      {
        assert(state.unmatch(e) == 1);
        assert(state.unmatch(e) == 0);
      }
      else
      {
        vc<int> selected;
        for (int u = 0; u < n; u++) if (state.left_mate()[u] != -1) selected.eb(u);
        const auto saved = state.left_mate();
        auto result = state.search(e);
        assert(state.left_mate() == saved && result.left.front() == e);
        auto added = selected;
        added.eb(e);
        bool feasible = cover(adj, added);
        assert(bool(result.free_right) == feasible);
        if (!feasible)
        {
          set<int> reachable(ALL(result.left)), right(ALL(result.right));
          assert(reachable.size() == right.size() + 1);
          for (int u : reachable) for (int v : adj[u]) assert(right.count(v));
          for (int f : selected)
          {
            auto swapped = selected;
            *find(ALL(swapped), f) = e;
            assert(cover(adj, swapped) == bool(reachable.count(f)));
          }
        }
        assert(state.augment(e) == feasible);
      }
      check_matching(state, adj);
    }
    int best = 0;
    for (int mask = 0; mask < (1 << n); mask++)
    {
      vc<int> selected;
      for (int u = 0; u < n; u++) if (mask >> u & 1) selected.eb(u);
      if (cover(adj, selected)) best = max(best, int(selected.size()));
    }
    assert(state.maximize() == best);
    check_matching(state, adj);
    auto directed = state.alternating_graph();
    set<pair<int, int>> actual, expected;
    for (auto edge : directed.edges()) actual.emplace(edge.from, edge.to);
    for (int u = 0; u < n; u++)
    {
      for (int v : adj[u]) expected.emplace(u, n + v);
      if (state.left_mate()[u] != -1) expected.emplace(n + state.left_mate()[u], u);
    }
    assert(actual == expected);
    auto copied = state;
    state.clear();
    assert(state.matching_size() == 0 && copied.matching_size() == best);
    assert(state.maximize() == best);
  }
  for (int trial = 0; trial < 30; trial++)
  {
    constexpr int n = 100, r = 120;
    vvc<int> adj(n);
    vc<pair<int, int>> edges;
    for (int e = 0; e < 1700; e++)
    {
      int u = rng() % n, v = rng() % r;
      adj[u].eb(v), edges.emplace_back(u, n + v);
    }
    BipartiteMatchingState state(r, CSR<int>(adj));
    vc<bool> color(n + r);
    for (int v = n; v < n + r; v++) color[v] = true;
    assert(state.maximize() == int(bipartite_matching(GraphUndirected<>(n + r, edges), color).size()));
  }
  cout << "Hello World\n";
}
