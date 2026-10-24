#pragma once

#include "maxflow.hpp"
#include "internal/capacity_sum.hpp"

/**
 * @brief Project Selection Problem（劣モジュラ二値関数の最小化）
 * @docs docs/flow/project_selection.md
 */

template <class Cost>
class ProjectSelection
{
  static_assert(is_integral_ext<Cost> && is_signed_ext<Cost>);
  using Unsigned = make_unsigned_ext_t<Cost>;
  struct Arc { int from, to; Unsigned cap; };
  static constexpr int source = -1, sink = -2;
  int n = 0, nodes = 0;
  vc<Arc> finite;
  vc<pair<int, int>> hard;
  flow_internal::CapacitySum<Unsigned> bound;
  Unsigned offset = 0;
  vc<bool> chosen;
  bool solved = false;

  void check_vertex(int v) const { assert(0 <= v && v < n); }
  void add_arc(int from, int to, Unsigned cap)
  {
    if (from == to || cap == 0) return;
    finite.push_back({from, to, cap});
    bound.add(cap);
    solved = false;
  }
  void add_unary(int v, Cost cost0, Cost cost1)
  {
    solved = false;
    if (cost0 <= cost1)
      offset += Unsigned(cost0), add_arc(v, sink, Unsigned(cost1) - Unsigned(cost0));
    else offset += Unsigned(cost1), add_arc(source, v, Unsigned(cost0) - Unsigned(cost1));
  }
  void add_reward(int v, Cost profit0, Cost profit1)
  {
    solved = false;
    if (profit0 >= profit1)
      offset -= Unsigned(profit0), add_arc(v, sink, Unsigned(profit0) - Unsigned(profit1));
    else offset -= Unsigned(profit1), add_arc(source, v, Unsigned(profit1) - Unsigned(profit0));
  }

public:
  ProjectSelection() = default;
  explicit ProjectSelection(int n) : n(n), nodes(n) { assert(n >= 0); }
  template <class I = ll>
  I size() const { return n; }

  void add_constant(Cost cost) { offset += Unsigned(cost); solved = false; }
  void add_cost(int v, Cost cost0, Cost cost1)
  { check_vertex(v); add_unary(v, cost0, cost1); }
  // 利益は負の費用として足す。solve() は常に費用の最小値を返す。
  void add_profit(int v, Cost profit0, Cost profit1)
  { check_vertex(v); add_reward(v, profit0, profit1); }
  // x[from] = 1, x[to] = 0 のときだけ非負の費用を加える。
  void add_penalty(int from, int to, Cost cost)
  {
    check_vertex(from), check_vertex(to);
    assert(cost >= 0);
    add_arc(from, to, Unsigned(cost));
  }
  void add_disagreement_cost(int u, int v, Cost cost)
  { add_penalty(u, v, cost); add_penalty(v, u, cost); }
  void add_pairwise_cost(int u, int v, Cost c00, Cost c01, Cost c10, Cost c11)
  {
    check_vertex(u), check_vertex(v);
    if (u == v) { add_unary(u, c00, c11); return; }
    flow_internal::SignedCapacitySum<Cost> slack;
    slack.add(c01), slack.add(c10), slack.subtract(c00), slack.subtract(c11);
    assert(!slack.negative()); // c00 + c11 <= c01 + c10
    add_unary(u, c00, c10);
    add_unary(v, c10, c11);
    offset -= Unsigned(c10);
    slack.magnitude().for_each_chunk([&](Unsigned cap) { add_arc(v, u, cap); });
  }
  void imply(int from, int to)
  {
    check_vertex(from), check_vertex(to);
    if (from != to) hard.emplace_back(from, to);
    solved = false;
  }
  void fix(int v, bool value)
  {
    check_vertex(v);
    hard.emplace_back(value ? source : v, value ? v : sink);
    solved = false;
  }
  // 指定した全変数が value のときだけ profit >= 0 の利益を加える。
  void add_all_profit(const vc<int> &vertices, bool value, Cost profit)
  {
    assert(profit >= 0);
    for (int v : vertices) check_vertex(v);
    if (vertices.empty()) { offset -= Unsigned(profit); solved = false; return; }
    if (profit == 0) return;
    const int extra = nodes++;
    add_reward(extra, value ? 0 : profit, value ? profit : 0);
    for (int v : vertices)
      hard.emplace_back(value ? extra : v, value ? v : extra);
    solved = false;
  }

  optional<Cost> solve()
  {
    solved = false;
    MaxFlow<Unsigned> flow(nodes + 2);
    auto vertex = [&](int v) { return v == source ? nodes : v == sink ? nodes + 1 : v; };
    for (const auto &e : finite) flow.add_edge(vertex(e.from), vertex(e.to), e.cap);
    // 全有限容量の和 + 1。大きい場合は同じ幅の複数の辺へ分ける。
    auto infinity = bound;
    infinity.add(1);
    for (const auto &e : hard)
    {
      const int from = vertex(e.first), to = vertex(e.second);
      infinity.for_each_chunk([&](Unsigned cap) { flow.add_edge(from, to, cap); });
    }
    Unsigned total = offset;
    const Unsigned unlimited = numeric_limits<Unsigned>::max();
    while (true)
    {
      const Unsigned f = flow.flow(nodes, nodes + 1);
      total += f;
      if (f != unlimited) break;
    }
    const auto cut = flow.min_cut(nodes);
    for (auto [u, v] : hard) if (cut[vertex(u)] && !cut[vertex(v)]) return nullopt;
    chosen.assign(cut.begin(), cut.begin() + n);
    solved = true;
    return Cost(total);
  }
  // 1 (true) が s 側・選択、0 (false) が t 側・非選択。
  vc<bool> assignment() const { assert(solved); return chosen; }
};
