#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "graph/two_edge_connected_components.hpp"
#include "tree/block_cut_tree.hpp"
#include "tree/add_virtual_root.hpp"

using Groups = vector<vector<int>>;

Groups normalized(Groups groups)
{
  for (auto &vs : groups) sort(vs.begin(), vs.end());
  sort(groups.begin(), groups.end());
  return groups;
}

// Compare canonical blocks with each block's vertex and edge rows kept together.
using CanonicalBlocks = vector<pair<vector<int>, vector<int>>>;
CanonicalBlocks canonical_blocks(const BiconnectedComponents& bcc)
{
  CanonicalBlocks result;
  for (int b = 0; b < bcc.size(); ++b)
  {
    auto vs = bcc.vertices(b).to_v(), es = bcc.edges(b).to_v();
    sort(vs.begin(), vs.end());
    sort(es.begin(), es.end());
    result.emplace_back(std::move(vs), std::move(es));
  }
  sort(result.begin(), result.end());
  return result;
}

template <class Cost, bool erasable>
void check(const GraphUndirected<Cost, erasable> &g)
{
  const int n = g.template size<int>();
  const auto es = g.edges();
  auto components = [&](int removed_vertex, int removed_edge, const vector<int> &bridges)
  {
    vector<int> ids(n, -1), queue;
    int k = 0;
    for (int root = 0; root < n; ++root) if (root != removed_vertex && ids[root] == -1)
    {
      ids[root] = k++;
      queue = {root};
      for (int i = 0; i < int(queue.size()); ++i) for (const auto &e : es)
      {
        if (e.index == removed_edge || e.from == removed_vertex || e.to == removed_vertex
            || find(bridges.begin(), bridges.end(), e.index) != bridges.end()) continue;
        int to = -1;
        if (e.from == queue[i]) to = e.to;
        else if (e.to == queue[i]) to = e.from;
        if (to != -1 && ids[to] == -1) ids[to] = ids[root], queue.push_back(to);
      }
    }
    return pair{k, ids};
  };
  const auto [cc, connected] = components(-1, -1, {});
  vector<int> bridges, articulations;
  vector<vector<int>> removed(n);
  for (const auto &e : es)
    if (components(-1, e.index, {}).first > cc) bridges.push_back(e.index);
  for (int v = 0; v < n; ++v)
  {
    const auto result = components(v, -1, {});
    removed[v] = result.second;
    if (result.first > cc) articulations.push_back(v);
  }
  sort(bridges.begin(), bridges.end());

  const LowLink lowlink(g);
  assert(lowlink.size() == n);
  assert(lowlink.articulations() == articulations);
  auto got_bridges = lowlink.bridges();
  sort(got_bridges.begin(), got_bridges.end());
  assert(got_bridges == bridges);
  auto order = lowlink.dfs_order();
  sort(order.begin(), order.end());
  for (int v = 0; v < n; ++v)
  {
    assert(order[v] == v);
    assert(lowlink.dfs_order()[lowlink.order(v)] == v);
    assert(0 <= lowlink.low(v) && lowlink.low(v) <= lowlink.order(v));
    assert(lowlink.is_articulation(v)
        == (find(articulations.begin(), articulations.end(), v) != articulations.end()));
    if (auto p = lowlink.parent(v))
    {
      assert(lowlink.order(p->first) < lowlink.order(v));
      assert(any_of(es.begin(), es.end(), [&](const auto &e)
      { return e.index == p->second && minmax(e.from, e.to) == minmax(v, p->first); }));
    }
  }
  for (const auto &e : es)
  {
    const bool bridge = binary_search(bridges.begin(), bridges.end(), e.index);
    assert(lowlink.is_bridge(e) == bridge);
    assert(lowlink.is_bridge(e.to, e.from) == bridge);
  }

  const auto tecc = two_edge_connected_component_ids<int>(lowlink);
  static_assert(is_same_v<decltype(two_edge_connected_component_ids(g)), vector<ll>>);
  assert(tecc == two_edge_connected_component_ids<int>(g));
  const auto expected_tecc = components(-1, -1, bridges).second;
  for (int u = 0; u < n; ++u) for (int v = 0; v < n; ++v)
    assert((tecc[u] == tecc[v]) == (expected_tecc[u] == expected_tecc[v]));
  auto labels = tecc;
  sort(labels.begin(), labels.end());
  labels.erase(unique(labels.begin(), labels.end()), labels.end());
  for (int i = 0; i < int(labels.size()); ++i) assert(labels[i] == i);

  // 頂点部分集合を全列挙し、頂点を一つ除いても連結な極大集合を求める。
  vector<int> adjacency(n);
  for (const auto &e : es) if (e.from != e.to)
    adjacency[e.from] |= 1 << e.to, adjacency[e.to] |= 1 << e.from;
  auto subset_connected = [&](int mask)
  {
    if (mask == 0) return true;
    int seen = mask & -mask, rest = seen;
    while (rest)
    {
      int v = __builtin_ctz(unsigned(rest));
      rest &= rest - 1;
      int add = adjacency[v] & mask & ~seen;
      rest |= add, seen |= add;
    }
    return seen == mask;
  };
  vector<int> valid;
  for (int mask = 1; mask < (1 << n); ++mask)
  {
    if (__builtin_popcount(unsigned(mask)) == 1)
    {
      if (adjacency[__builtin_ctz(unsigned(mask))] == 0) valid.push_back(mask);
      continue;
    }
    bool ok = subset_connected(mask);
    for (int v = 0; v < n; ++v) if (mask >> v & 1) ok &= subset_connected(mask ^ (1 << v));
    if (ok) valid.push_back(mask);
  }
  Groups expected_blocks;
  for (int mask : valid)
  {
    if (any_of(valid.begin(), valid.end(), [&](int other)
      { return mask != other && (mask & other) == mask; })) continue;
    vector<int> vs;
    for (int v = 0; v < n; ++v) if (mask >> v & 1) vs.push_back(v);
    expected_blocks.push_back(vs);
  }

  const BiconnectedComponents bcc(g, lowlink), fresh_bcc(g);
  assert(bcc.num_of_vertices() == n);
  assert(canonical_blocks(bcc) == canonical_blocks(fresh_bcc));
  assert(normalized(bcc.vertex_groups().to_vv()) == normalized(expected_blocks));
  assert(bcc.vertex_groups().size() == bcc.size());
  assert(bcc.edge_groups().size() == bcc.size());
  vector<int> all_edges;
  for (int b = 0; b < bcc.size(); ++b)
  {
    vector<int> expected;
    int mask = 0;
    for (int v : bcc.vertices(b)) mask |= 1 << v;
    assert(__builtin_popcount(unsigned(mask)) == bcc.vertices(b).size());
    for (const auto &e : es)
      if (e.from != e.to && (mask >> e.from & 1) && (mask >> e.to & 1)) expected.push_back(e.index);
    auto got = bcc.edges(b).to_v();
    all_edges.insert(all_edges.end(), got.begin(), got.end());
    sort(got.begin(), got.end()), sort(expected.begin(), expected.end());
    assert(got == expected);
  }
  sort(all_edges.begin(), all_edges.end());
  vector<int> expected_edges;
  for (const auto &e : es) if (e.from != e.to) expected_edges.push_back(e.index);
  sort(expected_edges.begin(), expected_edges.end());
  assert(all_edges == expected_edges);

  const BlockCutTree bct(bcc), fresh_bct(g);
  const auto &forest = bct.forest();
  assert(bct.num_of_blocks() == bcc.size());
  assert(bct.size() == bcc.size() + int(articulations.size()));
  assert(bct.vertex_ids() == fresh_bct.vertex_ids());
  assert(forest.adj_matrix_ecnt() == fresh_bct.forest().adj_matrix_ecnt());
  int incidences = 0;
  for (int b = 0; b < bcc.size(); ++b)
  {
    assert(!bct.is_articulation_node(b) && !bct.articulation_vertex(b));
    for (int v : bcc.vertices(b))
    {
      const int id = bct.node_id(v);
      if (lowlink.is_articulation(v))
      {
        assert(id >= bcc.size() && bct.is_articulation_node(id));
        assert(bct.articulation_vertex(id) == v);
        int count = 0;
        for (const auto &e : forest.out_edges(b)) count += e.to == id;
        assert(count == 1);
        ++incidences;
      }
      else assert(id == b);
    }
  }
  assert(forest.num_of_edges() == incidences);
  assert(forest.num_of_edges() == forest.size() - cc);
  for (int u = 0; u < n; ++u)
  {
    vector<int> distance(bct.size(), -1), parent(bct.size(), -1), queue{bct.node_id(u)};
    distance[queue[0]] = 0;
    for (int i = 0; i < int(queue.size()); ++i) for (const auto &e : forest.out_edges(queue[i]))
      if (distance[e.to] == -1)
      {
        distance[e.to] = distance[queue[i]] + 1, parent[e.to] = queue[i];
        queue.push_back(e.to);
      }
    for (int v = 0; v < n; ++v)
    {
      assert((distance[bct.node_id(v)] >= 0) == (connected[u] == connected[v]));
      if (connected[u] != connected[v]) continue;
      vector<bool> on_path(bct.size());
      for (int x = bct.node_id(v); x != -1; x = parent[x]) on_path[x] = true;
      for (int x = 0; x < n; ++x) if (x != u && x != v && lowlink.is_articulation(x))
        assert(on_path[bct.node_id(x)] == (removed[x][u] != removed[x][v]));
    }
  }
}

void check_case(int n, const vector<pair<int, int>> &es, mt19937 &rng)
{
  check(GraphUndirected<>(n, es));
  vector<tuple<int, int, ll>> weighted;
  for (auto [u, v] : es) weighted.emplace_back(u, v, ll(rng()) - (1LL << 31));
  check(GraphUndirected<ll>(n, weighted));
  GraphUndirected<ll, true> erasable(n, weighted);
  vector<int> ids(es.size());
  iota(ids.begin(), ids.end(), 0);
  shuffle(ids.begin(), ids.end(), rng);
  for (int i = 0, count = ids.empty() ? 0 : rng() % (ids.size() + 1); i < count; ++i)
    erasable.erase_edge(ids[i]);
  check(erasable);
}

void large_cases()
{
  const int n = 250000;
  for (bool star : {false, true})
  {
    vector<pair<int, int>> es;
    for (int v = 1; v < n; ++v) es.emplace_back(star ? 0 : v - 1, v);
    const GraphUndirected<> g(n, es);
    const LowLink lowlink(g);
    assert(lowlink.bridges().size() == n - 1);
    assert(lowlink.articulations().size() == (star ? 1 : n - 2));
    const auto ids = two_edge_connected_component_ids<int>(lowlink);
    assert(*max_element(ids.begin(), ids.end()) == n - 1);
    const BiconnectedComponents bcc(g, lowlink);
    assert(bcc.size() == n - 1);
    for (int b = 0; b < bcc.size(); ++b)
      assert(bcc.vertices(b).size() == 2 && bcc.edges(b).size() == 1);
    const BlockCutTree bct(bcc);
    assert(bct.size() == n - 1 + (star ? 1 : n - 2));
    assert(bct.forest().num_of_edges() == bct.size() - 1);
  }
  const vector<pair<int, int>> parallel(250000, {0, 1});
  const GraphUndirected<> g(2, parallel);
  const BiconnectedComponents bcc(g);
  assert(bcc.size() == 1 && bcc.edges(0).size() == int(parallel.size()));
  assert(LowLink(g).bridges().empty());
}

int main()
{
  mt19937 rng(20261022);
  assert(LowLink().size() == 0 && BiconnectedComponents().size() == 0 && BlockCutTree().size() == 0);
  check_case(0, {}, rng);
  check_case(1, {}, rng);
  check_case(1, {{0, 0}, {0, 0}}, rng);
  check_case(2, {{0, 1}, {0, 1}, {1, 1}}, rng);
  check_case(7, {{0, 1}, {1, 2}, {2, 0}, {2, 3}, {3, 4}, {4, 2}, {5, 5}}, rng);
  for (int n = 0; n <= 5; ++n)
  {
    vector<pair<int, int>> possible;
    for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v) possible.emplace_back(u, v);
    for (int mask = 0; mask < (1 << possible.size()); ++mask)
    {
      vector<pair<int, int>> es;
      for (int i = 0; i < int(possible.size()); ++i) if (mask >> i & 1) es.push_back(possible[i]);
      check_case(n, es, rng);
    }
  }
  for (int trial = 0; trial < 1200; ++trial)
  {
    int n = rng() % 9, m = n ? rng() % 25 : 0;
    vector<pair<int, int>> es;
    while (m--) es.emplace_back(rng() % n, rng() % n);
    check_case(n, es, rng);
  }
  const BlockCutTree bct(GraphUndirected<>(4, vector<pair<int, int>>{{0, 1}, {1, 2}}));
  vector<pair<int, int>> forest;
  for (const auto &e : bct.forest().edges()) forest.emplace_back(e.from, e.to);
  const auto tree = add_virtual_root(bct.size(), forest);
  assert(tree.size() == bct.size() + 1);
  assert(tree.dist(bct.node_id(0), bct.node_id(2)) == 2);
  large_cases();
  cout << "Hello World\n";
}
