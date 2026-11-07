#pragma once

#include "internal_exchange_graph.hpp"
#include "internal_shortest_path.hpp"
#include "../utils/is_integral_ext.hpp"

/**
 * @brief 重み付きマトロイド交叉
 * @docs docs/matroid/weighted_matroid_intersection.md
 */

namespace internal
{
struct MatroidShortestPathAuto
{
  template <class Solver>
  auto operator()(Solver &solver, int source, int target) const { return solver.solve(source, target); }
};

template <class M1, class M2, class W, class Emit, class Solve = MatroidShortestPathAuto>
void weighted_matroid_intersection_run(M1 &first, M2 &second, const vc<W> &weight,
                                      int limit, Emit emit, Solve solve = {})
{
  if constexpr (is_integral_ext<W>) static_assert(is_signed_ext<W>);
  using Cost = MatroidPathCost<W>;
  const int n = first.size(), source = n, target = n + 1;
  assert(n == second.size() && int(weight.size()) == n && limit >= 0);
  vc<bool> used(n);
  vc<int> selected;
  // I はそれぞれの重みで、各 matroid の同じ要素数の独立集合中で最適。
  vc<W> first_weight = weight, second_weight(n, W{});
  emit(W{}, selected);
  for (int count = 0; count < limit; count++)
  {
    auto graph = matroid_exchange_graph(first, second, selected);
    optional<W> first_add, second_add;
    for (int e : graph.sources)
      if (!first_add || *first_add < first_weight[e]) first_add = first_weight[e];
    for (int e = 0; e < n; e++) if (graph.sink[e])
      if (!second_add || *second_add < second_weight[e]) second_add = second_weight[e];
    if (!first_add || !second_add) break;

    vc<tuple<int, int, Cost>> edges;
    edges.reserve(graph.graph.num_of_edges());
    for (int e = 0; e < n + 2; e++) for (auto edge : graph.graph.out_edges(e))
    {
      const int f = edge.to;
      W cost;
      if (e == source) cost = *first_add + -first_weight[f];
      else if (f == target) cost = *second_add + -second_weight[e];
      else cost = used[e] ? first_weight[e] + -first_weight[f]
                          : second_weight[f] + -second_weight[e];
      assert(!(cost < W{}));
      edges.eb(e, f, Cost(std::move(cost), f == target ? 0 : 1));
    }
    const GraphDirected<Cost> weighted_graph(n + 2, edges);
    ShortestPath<true, Cost> shortest(weighted_graph);
    const auto distance = solve(shortest, source, target);
    if (!distance[target].finite()) break;

    // target 確定後の未確定距離も、primary cost をここで clip すれば使える。
    const W bound = distance[target].weight;
    for (int e = 0; e < n; e++)
    {
      W delta = distance[e].finite() && distance[e].weight < bound ? distance[e].weight : bound;
      first_weight[e] += delta;
      second_weight[e] += -delta;
    }
    auto path = shortest.path_vertex(target);
    assert(path);
    for (int e : *path) if (e < n) used[e] = !used[e];
    selected = matroid_selected(used);
    W total{};
    for (int e : selected) total += weight[e];
    emit(total, selected);
  }
}
}

// 各要素数 k の最適解を result[k] = {重みの総和, 要素番号} に返す。
template <class M1, class M2, class W>
vc<pair<W, vc<int>>> weighted_matroid_intersection_slope(M1 first, M2 second,
                                                        const vc<W> &weight)
{
  vc<pair<W, vc<int>>> result;
  internal::weighted_matroid_intersection_run(first, second, weight, first.size(),
      [&](const W &total, const vc<int> &selected) { result.eb(total, selected); });
  return result;
}

// cardinality=-1 は最大要素数、その後に最大重み。それ以外は指定要素数。
template <class M1, class M2, class W>
optional<pair<W, vc<int>>> weighted_matroid_intersection(M1 first, M2 second,
                                                        const vc<W> &weight,
                                                        int cardinality = -1)
{
  assert(cardinality >= -1);
  optional<pair<W, vc<int>>> result;
  internal::weighted_matroid_intersection_run(first, second, weight,
      cardinality == -1 ? first.size() : cardinality,
      [&](const W &total, const vc<int> &selected) { result = pair<W, vc<int>>{total, selected}; });
  if (cardinality != -1 && int(result->second.size()) != cardinality) return nullopt;
  return result;
}
