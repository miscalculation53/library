#pragma once

#include "graph.hpp"
#include "internal_general_weighted_matching.hpp"
#include "../ds/uf/uf.hpp"
#include "../utils/is_integral_ext.hpp"

/**
 * @brief 一般グラフ最大重みマッチング
 * @docs docs/graph/general_weighted_matching.md
 */

namespace internal
{
inline bool weighted_matching_uses_dense(int n, int m)
{
  const ull twice_edges = 2ULL * m;
  if (16 * twice_edges < ull(n) * (n - 1)) return false;
  return 4 * u128(twice_edges) * twice_edges >= 81 * u128(n) * n * n;
}

template <class Cost>
void weighted_matching_component(int n, const WeightedMatchingEdge<Cost> *edges, const int *indices,
                                 int m, vc<int> &result)
{
  if (n == 2)
  {
    result.eb(indices[0]);
    return;
  }
  auto maximum = edges[0].cost;
  for (int i = 1; i < m; i++) maximum = max(maximum, edges[i].cost);
  const bool dense = weighted_matching_uses_dense(n, m);
  const size_t start = result.size();
  auto solve = [&](auto weight)
  {
    using Weight = decltype(weight);
    if (dense) GeneralWeightedMatching<Weight, Cost, false>(n, edges, m, weight).solve(result);
    else GeneralWeightedMatching<Weight, Cost, true>(n, edges, m, weight).solve(result);
  };
  if constexpr (sizeof(Cost) < sizeof(ull)) solve(ull(maximum));
  else if (u128(maximum) <= numeric_limits<ull>::max() / 4) solve(ull(maximum));
  else
  {
    assert(u128(maximum) <= numeric_limits<u128>::max() / 4);
    solve(u128(maximum));
  }
  for (size_t i = start; i < result.size(); i++) result[i] = indices[result[i]];
}
}

// 最大重みマッチングを元の辺番号の配列で返す。重みは整数で、正の最大値の4倍が u128 に収まること。
template <class Cost, bool erasable>
vc<int> general_weighted_matching(const GraphUndirected<Cost, erasable> &g)
{
  static_assert(is_void_v<Cost> || is_integral_ext<Cost>);
  using InputCost = conditional_t<is_void_v<Cost>, bool, Cost>;
  using InputEdge = internal::WeightedMatchingEdge<InputCost>;
  const int n = g.template size<int>();
  vc<int> last(n, -1), renumber(n, -1);
  vc<InputEdge> edges;
  vc<int> indices;
  edges.reserve(g.num_of_edges());
  indices.reserve(g.num_of_edges());
  for (int u = 0; u < n; u++)
  {
    const int begin = edges.size();
    for (const auto &e : g.out_edges(u))
    {
      const int v = e.to;
      if (u >= v || e.cost <= 0) continue;
      const InputCost cost(e.cost);
      int &slot = last[v];
      if (slot >= begin)
      {
        if (edges[slot].cost < cost)
        {
          edges[slot].cost = cost;
          indices[slot] = e.index;
        }
      }
      else
      {
        slot = edges.size();
        edges.push_back({u, v, cost});
        indices.eb(e.index);
        renumber[u] = renumber[v] = 0;
      }
    }
  }
  vc<int> result;
  if (edges.empty()) return result;

  int active = 0;
  for (int &v : renumber) if (v != -1) v = active++;
  if (active != n) for (auto &e : edges)
  {
    e.from = renumber[e.from];
    e.to = renumber[e.to];
  }
  result.reserve(active / 2);
  UnionFind<UFDataEmpty<>> uf(active);
  for (const auto &e : edges)
  {
    const int root = uf.merge<int>(e.from, e.to);
    if (uf.size<int>(root) == active)
    {
      internal::weighted_matching_component(active, edges.data(), indices.data(), edges.size(), result);
      return result;
    }
  }

  const vc<int> group = uf.group_ids<int>();
  const int count = *max_element(ALL(group)) + 1;
  vc<int> vertices(count), edge_counts(count);
  for (int v = 0; v < active; v++) renumber[v] = vertices[group[v]]++;
  for (const auto &e : edges) edge_counts[group[e.from]]++;
  CSR<InputEdge> components(edge_counts);
  vc<int> component_indices(edges.size());
  fill(ALL(edge_counts), 0);
  for (int i = 0; i < int(edges.size()); i++)
  {
    auto e = edges[i];
    const int k = group[e.from];
    e.from = renumber[e.from];
    e.to = renumber[e.to];
    const int slot = edge_counts[k]++;
    components[k][slot] = e;
    component_indices[components.offset(k) + slot] = indices[i];
  }
  vc<InputEdge>().swap(edges);
  vc<int>().swap(indices);
  for (int k = 0; k < count; k++)
  {
    const auto row = components[k];
    internal::weighted_matching_component(vertices[k], &row.front(), component_indices.data() + components.offset(k),
                                          row.size(), result);
  }
  return result;
}
