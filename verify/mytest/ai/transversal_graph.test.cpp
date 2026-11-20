#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "matroid/transversal_matroid.hpp"
#include "flow/bipartite_matching.hpp"

struct LiteralColor
{
  bool side;
  friend bool operator==(LiteralColor a, int b) { return int(a.side) == b; }
};

bool cover(const vvc<int> &adjacency, int mask, int used = 0)
{
  if (!mask) return true;
  const int e = __builtin_ctz(unsigned(mask));
  for (int v : adjacency[e]) if (!(used >> v & 1))
    if (cover(adjacency, mask ^ (1 << e), used | (1 << v))) return true;
  return false;
}

template <class Cost, bool Erasable, class Color>
void exercise(const GraphUndirected<Cost, Erasable> &graph, const vc<Color> &color)
{
  const int vertices = graph.template size<int>();
  vc<int> left, right;
  for (int v = 0; v < vertices; v++)
    (color[v] == 1 ? right : left).push_back(v);
  const int n = left.size(), r = right.size();
  vvc<int> adjacency(n);
  for (int e = 0; e < n; e++) for (auto edge : graph.out_edges(left[e]))
    adjacency[e].push_back(find(ALL(right), edge.to) - right.begin());

  BipartiteMatchingState state(graph, color), compact(r, adjacency);
  TransversalMatroid matroid(graph, color);
  assert(state.left_size() == n && state.right_size() == r && matroid.size() == n);
  for (int e = 0; e < n; e++)
  {
    assert(state.left_vertex(e) == left[e] && matroid.vertex(e) == left[e]);
    assert(state.left_index(left[e]) == optional<int>(e));
    assert(matroid.element(left[e]) == optional<int>(e));
    assert(!state.right_index(left[e]));
  }
  for (int v = 0; v < r; v++)
  {
    assert(state.right_vertex(v) == right[v]);
    assert(state.right_index(right[v]) == optional<int>(v));
    assert(!state.left_index(right[v]) && !matroid.element(right[v]));
  }
  assert(!state.left_index(-1) && !state.right_index(vertices));
  for (int e = 0; e < n; e++)
  {
    assert(state.augment(e) == compact.augment(e));
    assert(state.left_mate() == compact.left_mate());
    assert(state.right_mate() == compact.right_mate());
  }
  assert(state.maximize() == int(bipartite_matching(graph, color).size()));
  for (auto [u, v] : state.matching())
  {
    assert(color[u] == 0 && color[v] == 1);
    bool found = false;
    for (auto edge : graph.out_edges(u)) found |= edge.to == v;
    assert(found);
  }
  auto copied = state;
  state.clear();
  assert(state.matching().empty() && copied.matching_size() == compact.matching_size());
  assert(copied.maximize() == compact.maximize());

  using S = static_modint32<998244353>;
  using F = FieldAddSubMulDiv<S>;
  int calls = 0;
  auto sample = [&] { return S(++calls); };
  auto member = matroid.linear_representation<F>(sample);
  const int member_calls = calls;
  calls = 0;
  auto factory = transversal_linear_representation<F>(graph, color, sample);
  assert(member_calls == calls && member == factory);
  calls = 0;
  assert(factory == transversal_linear_representation<F>(r, adjacency, sample));
  assert(member_calls == calls && factory.template shape<int>() == make_pair(r, n));

  for (int mask = 0; mask < (1 << n); mask++) if (cover(adjacency, mask))
  {
    vc<int> selected;
    for (int e = 0; e < n; e++) if (mask >> e & 1) selected.push_back(e);
    matroid.build(selected);
    for (int e = 0; e < n; e++) if (!(mask >> e & 1))
    {
      auto circuit = matroid.circuit(e);
      assert(bool(circuit) != cover(adjacency, mask | (1 << e)));
      if (!circuit) continue;
      set<int> members(ALL(*circuit));
      assert(members.size() == circuit->size());
      for (int f : selected)
        assert(bool(members.count(f)) == cover(adjacency, (mask ^ (1 << f)) | (1 << e)));
      calls = 0;
      assert(matroid.linear_representation<F>(sample) == factory);
    }
  }
}

int main()
{
  mt19937 rng(20261103);
  for (int trial = 0; trial < 100; trial++)
  {
    const int n = rng() % 8, r = rng() % 7, total = n + r;
    vc<int> order(total);
    iota(ALL(order), 0);
    shuffle(ALL(order), rng);
    vc<bool> color(total, true);
    vc<ll> wide_color(total, 1);
    vc<LiteralColor> literal_color(total, {true});
    for (int i = 0; i < n; i++)
      color[order[i]] = false, wide_color[order[i]] = 0, literal_color[order[i]] = {false};
    vc<pair<int, int>> edges;
    for (int i = 0; i < n; i++) for (int j = n; j < total; j++) if (rng() % 3 == 0)
    {
      edges.emplace_back(order[i], order[j]);
      if (rng() % 2) edges.emplace_back(order[i], order[j]);
    }
    exercise(GraphUndirected<>(total, edges), color);
    exercise(GraphUndirected<ll>(total, edges, -13), wide_color);
    GraphUndirected<int, true> erasable(total, edges, 7);
    for (int e = 0; e < int(edges.size()); e++) if (rng() % 4 == 0) erasable.erase_edge(e);
    exercise(erasable, literal_color);
  }
  auto saved = []
  {
    GraphUndirected<int, true> graph(3, vc<pair<int, int>>{{0, 1}, {2, 1}}, 9);
    vc<int> color{0, 1, 0};
    TransversalMatroid matroid(graph, color);
    graph.erase_edge(0);
    return matroid;
  }();
  saved.build({0});
  assert(saved.circuit(1) == optional<vc<int>>({0}));
  assert(saved.vertex(1) == 2 && saved.element(2) == optional<int>(1));
  cout << "Hello World\n";
}
