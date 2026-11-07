#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "matroid/graphic_matroid.hpp"
#include "matroid/bicircular_matroid.hpp"
#include "matroid/laminar_matroid.hpp"

template <class M, class Predicate>
void check_circuits(M matroid, const Predicate &independent)
{
  const int n = matroid.size();
  for (int mask = 0; mask < (1 << n); mask++) if (independent(mask))
  {
    vc<int> ids;
    for (int e = n - 1; e >= 0; e--) if (mask >> e & 1) ids.push_back(e);
    matroid.build(ids);
    for (int e = 0; e < n; e++) if (!(mask >> e & 1))
    {
      auto c = as_const(matroid).circuit(e);
      const bool can_add = independent(mask | (1 << e));
      assert(bool(c) == !can_add);
      if (c)
      {
        vc<int> expected;
        for (int f = 0; f < n; f++) if (mask >> f & 1)
          if (independent((mask ^ (1 << f)) | (1 << e))) expected.push_back(f);
        sort(ALL(*c));
        assert(*c == expected);
      }
    }
    auto copied = matroid;
    auto moved = move(copied);
    for (int e = 0; e < n; e++) if (!(mask >> e & 1))
      assert(moved.circuit(e) == matroid.circuit(e));
  }
  matroid.build({});
}

bool independent_graph(int vertices, const vc<pair<int, int>> &edges, int mask, bool bicircular)
{
  vc<int> label(vertices);
  iota(ALL(label), 0);
  for (int e = 0; e < int(edges.size()); e++) if (mask >> e & 1)
  {
    const auto [u, v] = edges[e];
    int a = label[u], b = label[v];
    if (a == b && !bicircular) return false;
    for (int &c : label) if (c == b) c = a;
  }
  vc<int> count(vertices), size(vertices);
  for (int c : label) size[c]++;
  for (int e = 0; e < int(edges.size()); e++) if (mask >> e & 1)
    count[label[edges[e].first]]++;
  for (int c = 0; c < vertices; c++) if (count[c] > size[c]) return false;
  return true;
}

void random_cases()
{
  mt19937 rng(202611021);
  for (int trial = 0; trial < 400; trial++)
  {
    const int vertices = 1 + rng() % 7, n = rng() % 9;
    vc<pair<int, int>> edges;
    for (int e = 0; e < n; e++) edges.emplace_back(rng() % vertices, rng() % vertices);
    check_circuits(GraphicMatroid(vertices, edges), [&](int mask)
    { return independent_graph(vertices, edges, mask, false); });
    check_circuits(BicircularMatroid(vertices, edges), [&](int mask)
    { return independent_graph(vertices, edges, mask, true); });

    // 親の頂点番号が大小どちらにも現れる森。
    const int k = 1 + rng() % 8;
    vc<int> permutation(k), parent(k, -1), capacity(k), group(n);
    iota(ALL(permutation), 0);
    shuffle(ALL(permutation), rng);
    for (int v = 1; v < k; v++) if (rng() % 3)
      parent[permutation[v]] = permutation[rng() % v];
    for (int &c : capacity) c = rng() % 5;
    for (int &g : group) g = rng() % k;
    check_circuits(LaminarMatroid(parent, capacity, group), [&](int mask)
    {
      vc<int> count(k);
      for (int e = 0; e < n; e++) if (mask >> e & 1)
        for (int v = group[e]; v != -1; v = parent[v]) count[v]++;
      for (int v = 0; v < k; v++) if (count[v] > capacity[v]) return false;
      return true;
    });
  }
}

void long_graphic()
{
  constexpr int n = 100000;
  vc<pair<int, int>> edges;
  vc<int> ids;
  for (int v = 0; v + 1 < n; v++)
  {
    edges.emplace_back(v, v + 1);
    if (v % 2) swap(edges.back().first, edges.back().second);
    ids.push_back(v);
  }
  const int closing = edges.size();
  edges.emplace_back(n - 1, 0);
  const int loop = edges.size();
  edges.emplace_back(n / 2, n / 2);
  const int isolated = edges.size();
  edges.emplace_back(n - 1, n);
  mt19937 rng(202611022);
  shuffle(ALL(ids), rng);
  GraphicMatroid matroid(n + 100, edges);
  matroid.build(ids);
  auto circuit = matroid.circuit(closing);
  assert(circuit && int(circuit->size()) == n - 1);
  sort(ALL(*circuit));
  for (int e = 0; e < n - 1; e++) assert((*circuit)[e] == e);
  assert(matroid.circuit(loop) && matroid.circuit(loop)->empty());
  assert(!matroid.circuit(isolated));
  ids.erase(find(ALL(ids), n / 2));
  matroid.build(ids);
  assert(!matroid.circuit(closing));
  assert(!matroid.circuit(n / 2));
}

void long_bicircular()
{
  constexpr int n = 10000;
  vc<pair<int, int>> edges{{0, 0}, {n - 1, n - 1}};
  vc<int> ids{0, 1};
  int bridge = -1;
  for (int v = 0; v + 1 < n; v++)
  {
    if (v == n / 2) bridge = edges.size();
    else ids.push_back(edges.size());
    edges.emplace_back(v, v + 1);
  }
  // 枝の辺は circuit に含まれない。
  for (int v = 0; v < n; v++) ids.push_back(edges.size()), edges.emplace_back(v, n + v);
  const int second_loop = edges.size();
  edges.emplace_back(0, 0);
  BicircularMatroid matroid(2 * n + 100, edges);
  matroid.build(ids);
  auto circuit = matroid.circuit(bridge);
  assert(circuit && int(circuit->size()) == n);
  for (int e : *circuit) assert(e < n + 1 && e != bridge);
  assert(*matroid.circuit(second_loop) == vc<int>{0});
  matroid.build({});
  assert(!matroid.circuit(second_loop));
}

void long_laminar()
{
  constexpr int k = 100000;
  vc<int> parent(k), capacity(k, 2);
  for (int v = 0; v < k; v++) parent[v] = v + 1;
  parent.back() = -1;
  capacity[0] = 1;
  LaminarMatroid matroid(parent, capacity, {0, 0, k - 1});
  matroid.build({0, 2});
  assert(*matroid.circuit(1) == vc<int>{0});
  matroid.build({2});
  assert(!matroid.circuit(0));
  LaminarMatroid roots(vc<int>(k, -1), vc<int>(k, 1), {0, k / 2, k - 1, 0});
  roots.build({0, 1, 2});
  assert(*roots.circuit(3) == vc<int>{0});
}

int main()
{
  check_circuits(GraphicMatroid(0, {}), [](int) { return true; });
  check_circuits(BicircularMatroid(0, {}), [](int) { return true; });
  check_circuits(LaminarMatroid({}, {}, {}), [](int) { return true; });
  random_cases();
  long_graphic();
  long_bicircular();
  long_laminar();
  cout << "Hello World\n";
}
