#pragma once

#include "maxflow.hpp"
#include "internal/capacity_sum.hpp"

/**
 * @brief 上限・下限付き最大流・最小流・実行可能循環流
 * @docs docs/flow/bounded_maxflow.md
 */

template <class Cap>
class BoundedMaxFlow
{
  static_assert(is_integral_ext<Cap> && is_signed_ext<Cap>);
  using Unsigned = make_unsigned_ext_t<Cap>;
  using Flow = MaxFlow<Unsigned>;

public:
  struct edge
  {
    int from, to;
    Cap lower, upper, flow;
    CPP_DUMP_DEFINE_DATA(from, to, lower, upper, flow);
  };

private:
  int n = 0;
  vc<edge> es;
  Flow residual;
  bool solved = false;

  bool prepare(int s = -1, int t = -1)
  {
    solved = false;
    residual = Flow(n + 2);
    vc<flow_internal::SignedCapacitySum<Cap>> balance(n);
    for (auto &e : es)
    {
      e.flow = e.lower;
      residual.add_edge(e.from, e.to, Unsigned(e.upper) - Unsigned(e.lower));
      if (e.from != e.to)
        balance[e.to].add(e.lower), balance[e.from].subtract(e.lower);
    }
    const Unsigned unlimited = numeric_limits<Unsigned>::max();
    if (s != -1)
    {
      // 両向きの補助辺により、実行可能な流量が負の場合も扱う。
      residual.add_edge(t, s, unlimited);
      residual.add_edge(s, t, unlimited);
    }
    const int source = n, sink = n + 1;
    vc<int> demands;
    for (int v = 0; v < n; v++)
      balance[v].magnitude().for_each_chunk([&](Unsigned amount)
      {
        if (balance[v].negative()) residual.add_edge(v, sink, amount);
        else demands.eb(residual.add_edge(source, v, amount));
      });
    // 需給の総和が容量型を越える場合も、同じ型の流量に分けて処理する。
    while (residual.flow(source, sink) == unlimited) {}
    for (int id : demands)
    {
      const auto e = residual.get_edge(id);
      if (e.flow != e.cap) return false;
    }
    for (int id = es.size(); id < residual.template num_of_edges<int>(); id++)
      residual.change_edge(id, 0, 0);
    solved = true;
    return true;
  }

  Cap value(int s) const
  {
    Unsigned result = 0;
    for (int id = 0; id < int(es.size()); id++)
    {
      const auto &e = es[id];
      const Unsigned f = Unsigned(e.lower) + residual.get_edge(id).flow;
      if (e.from == s) result += f;
      if (e.to == s) result -= f;
    }
    return Cap(result);
  }
  void save()
  {
    for (int id = 0; id < int(es.size()); id++)
      es[id].flow = Cap(Unsigned(es[id].lower) + residual.get_edge(id).flow);
  }
  void check_terminals(int s, int t) const
  { assert(0 <= s && s < n && 0 <= t && t < n && s != t); }
  static Unsigned augment(Flow &flow, int s, int t)
  {
    Unsigned result = 0;
    while (true)
    {
      const Unsigned f = flow.flow(s, t);
      result += f;
      if (f != numeric_limits<Unsigned>::max()) return result;
    }
  }

public:
  BoundedMaxFlow() = default;
  explicit BoundedMaxFlow(int n) : n(n) { assert(n >= 0); }
  template <class I = ll>
  I size() const { return n; }
  template <class I = ll>
  I num_of_edges() const { return es.size(); }

  int add_edge(int from, int to, Cap lower, Cap upper)
  {
    assert(0 <= from && from < n && 0 <= to && to < n);
    assert(lower <= upper);
    const int id = es.size();
    es.push_back({from, to, lower, upper, lower});
    solved = false;
    return id;
  }
  int add_edge(int from, int to, Cap upper) { return add_edge(from, to, 0, upper); }
  void change_edge(int id, Cap lower, Cap upper)
  {
    assert(0 <= id && id < int(es.size()) && lower <= upper);
    es[id].lower = lower, es[id].upper = upper, es[id].flow = lower;
    solved = false;
  }
  edge get_edge(int id) const
  {
    assert(0 <= id && id < int(es.size()));
    return es[id];
  }
  vc<edge> edges() const { return es; }

  // 各求解は追加済みの辺から解き直す。
  bool feasible()
  {
    if (!prepare()) return false;
    save();
    return true;
  }
  optional<Cap> flow(int s, int t)
  {
    check_terminals(s, t);
    if (!prepare(s, t)) return nullopt;
    const Cap initial = value(s);
    const Unsigned extra = augment(residual, s, t);
    save();
    return Cap(Unsigned(initial) + extra);
  }
  optional<Cap> min_flow(int s, int t)
  {
    check_terminals(s, t);
    if (!prepare(s, t)) return nullopt;
    const Cap initial = value(s);
    const Unsigned extra = augment(residual, t, s);
    save();
    return Cap(Unsigned(initial) - extra);
  }
  // 最小・最大流量を返す。辺の flow と残余グラフには最大流の解を残す。
  optional<pair<Cap, Cap>> flow_range(int s, int t)
  {
    check_terminals(s, t);
    if (!prepare(s, t)) return nullopt;
    const Cap initial = value(s);
    auto backward = residual;
    const Cap minimum = Cap(Unsigned(initial) - augment(backward, t, s));
    const Cap maximum = Cap(Unsigned(initial) + augment(residual, s, t));
    save();
    return pair{minimum, maximum};
  }
  // 最大流の計算後は 1 (true) が s 側、0 (false) が t 側。
  vc<bool> min_cut(int s) const
  {
    assert(solved && 0 <= s && s < n);
    auto result = residual.min_cut(s);
    result.resize(n);
    return result;
  }
};
