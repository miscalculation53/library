#pragma once

#include "../template/template_all_but_modint.hpp"
#include "../utils/is_integral_ext.hpp"
#include "../utils/make_unsigned_ext.hpp"
#include "../ds/csr.hpp"
#include "../ds/fibonacci_heap.hpp"

/**
 * @brief 区間供給に対応する最小費用 b-flow・最小費用流の slope
 * @docs docs/flow/min_cost_b_flow.md
 */

template <class Cap, class Cost, class Total = Cost>
class MinCostBFlow
{
  static_assert(is_integral_ext<Cap> && is_signed_ext<Cap>);
  static_assert(is_integral_ext<Cost> && is_signed_ext<Cost>);
  static_assert(!is_integral_ext<Total> || is_signed_ext<Total>);

public:
  using Balance = Cap;
  using Potential = Cost;
  using Capacity = Cap;
  enum class Algorithm { automatic, ssp, primal_dual, capacity_scaling, network_simplex };
  enum class Heap { automatic, binary, fibonacci, dense };
  struct edge
  {
    int from, to;
    Cap lower, upper;
    Cost cost;
    Cap flow;
    CPP_DUMP_DEFINE_DATA(from, to, lower, upper, cost, flow);
  };

private:
  struct ResidualEdge
  {
    int to, rev;
    Capacity cap;
    Potential cost;
  };
  int n = 0, solve_n = 0;
  vc<edge> input;
  vc<Balance> supply, supply_upper, excess;
  CSR<ResidualEdge> g;
  vc<int> pos, supply_pos;
  vc<Potential> price;
  Total value = 0;
  bool solved = false;
  Heap last_heap = Heap::binary;
  bool uniform_cost = true;
  Potential cost_unit = 0;
  mutable vc<Potential> normalized_price;

  // 符号付き整数の中間和・積は unsigned で計算し、最終結果の符号を復元する。
  // 数学的な結果が Total に収まる限り、途中で打ち消し合う大きな項も扱える。
  template <class Integer>
  static Integer from_bits(make_unsigned_ext_t<Integer> bits)
  {
    using U = make_unsigned_ext_t<Integer>;
    return bits <= U(numeric_limits<Integer>::max()) ? Integer(bits) : Integer(-Integer(U(~bits)) - 1);
  }
  static Total add_cost(const Total &current, Capacity amount, Potential marginal)
  {
    if constexpr (is_integral_ext<Total>)
    {
      using U = make_unsigned_ext_t<Total>;
      using Arithmetic = make_unsigned_ext_t<decltype(U{} + U{})>;
      return from_bits<Total>(U(Arithmetic(U(current)) + Arithmetic(U(amount)) * Arithmetic(U(marginal))));
    }
    else return current + Total(amount) * Total(marginal);
  }

  Heap select_heap(Heap h) const
  {
    if (h != Heap::automatic) return h;
    if (solve_n <= 1024 && ll(g.get_elist().size()) > ll(solve_n) * solve_n / 32) return Heap::dense;
    if (!uniform_cost && solve_n <= 4096 && ll(g.get_elist().size()) >= 12LL * solve_n) return Heap::fibonacci;
    return Heap::binary;
  }

  struct ShortestPaths
  {
    MinCostBFlow &owner;
    Heap heap;
    vc<Potential> dist;
    vc<int> prev, settled, zero;
    vc<int> first_child, next_child;
    vc<Balance> need, available;
    vc<unsigned char> done;
    pql<pair<Potential, int>> binary;
    FibonacciHeap<Potential> fibonacci;
    ll work = 0;
    int heap_log = 1;

    ShortestPaths(MinCostBFlow &owner, Heap heap)
        : owner(owner), heap(owner.select_heap(heap)), dist(owner.solve_n), prev(owner.solve_n), done(owner.solve_n),
          fibonacci(this->heap == Heap::fibonacci ? owner.solve_n : 0)
    {
      const ll keys = this->heap == Heap::binary ? owner.solve_n + ll(owner.g.get_elist().size()) + 1 : owner.solve_n + 1LL;
      for (ll p = 1; p < keys; p *= 2) heap_log++;
    }

    void reset()
    {
      fill(ALL(dist), numeric_limits<Potential>::max());
      fill(ALL(prev), -1), fill(ALL(done), false);
      settled.clear(), zero.clear();
      binary = decltype(binary){};
      if (heap == Heap::fibonacci) fibonacci.clear();
      work += 3LL * owner.solve_n;
    }
    void push(int v, Potential d)
    {
      if (heap == Heap::binary) binary.push({d, v});
      else if (heap == Heap::fibonacci) fibonacci.push_or_decrease(v, d);
      work += heap == Heap::binary ? heap_log : 1;
    }
    int pop()
    {
      if (!zero.empty())
      {
        const int v = zero.back();
        zero.pop_back();
        work++;
        return v;
      }
      if (heap == Heap::binary)
      {
        work += heap_log;
        if (binary.empty()) return -1;
        const int v = binary.top().second;
        binary.pop();
        return v;
      }
      if (heap == Heap::fibonacci)
      {
        work += heap_log;
        if (fibonacci.empty()) return -1;
        const int v = fibonacci.top().second;
        fibonacci.pop();
        return v;
      }
      int v = -1;
      for (int u = 0; u < owner.solve_n; u++)
        if (!done[u] && dist[u] != numeric_limits<Potential>::max() && (v == -1 || dist[u] < dist[v])) v = u;
      work += owner.solve_n;
      return v;
    }
    // source == -1 なら全供給点から、そうでなければ source から探索する。
    int run(Capacity delta, int source = -1, int target = -1, bool all_targets = false)
    {
      reset();
      int remaining = 0, last_target = -1;
      if (source == -1)
      {
        for (int v = 0; v < owner.solve_n; v++)
        {
          if (owner.excess[v] >= Balance(delta)) dist[v] = 0, zero.eb(v);
          if (owner.excess[v] <= -Balance(delta)) remaining++;
        }
      }
      else dist[source] = 0, zero.eb(source);
      auto finish = [&](int v, Potential d)
      {
        for (int u : settled) owner.price[u] += dist[u] - d;
        return v;
      };
      auto &data = owner.g.get_elist();
      while (true)
      {
        const int v = pop();
        if (v == -1)
          return last_target == -1 ? -1 : finish(last_target, dist[settled.back()]);
        if (done[v]) continue;
        done[v] = true, settled.eb(v);
        if (target == -1 ? owner.excess[v] <= -Balance(delta) : v == target)
        {
          last_target = v;
          if (!all_targets || --remaining == 0) return finish(v, dist[v]);
        }
        const int begin = owner.g.offset(v), end = owner.g.offset(v + 1);
        for (int i = begin; i < end; i++)
        {
          work++;
          const auto &e = data[i];
          if (e.cap < delta || done[e.to]) continue;
          const Potential reduced = e.cost + owner.price[v] - owner.price[e.to];
          assert(reduced >= 0);
          const Potential next = dist[v] + reduced;
          if (dist[e.to] <= next) continue;
          dist[e.to] = next, prev[e.to] = i;
          if (next == dist[v]) zero.eb(e.to);
          else push(e.to, next);
        }
      }
    }
  };

  Capacity prepare(Balance lower_sum, bool intervals)
  {
    solved = false;
    normalized_price.clear();
    uniform_cost = true, cost_unit = 0;
    solve_n = n + intervals;
    excess = supply;
    if (intervals) excess.eb(-lower_sum);
    price.assign(solve_n, 0);
    vc<int> degree(solve_n);
    for (auto &e : input)
    {
      e.flow = e.cost < 0 ? e.upper : e.lower;
      if (e.from == e.to) continue;
      excess[e.from] -= Balance(e.flow), excess[e.to] += Balance(e.flow);
      if (e.lower != e.upper) degree[e.from]++, degree[e.to]++;
    }
    if (intervals) for (int v = 0; v < n; v++)
      if (supply[v] != supply_upper[v]) degree[n]++, degree[v]++;
    g = CSR<ResidualEdge>(degree);
    pos.assign(input.size(), -1);
    supply_pos.clear();
    if (intervals) supply_pos.assign(n, -1);
    vc<int> count(solve_n);
    auto &data = g.get_elist();
    Capacity maximum = 0;
    for (int i = 0; i < int(input.size()); i++)
    {
      const auto &e = input[i];
      if (e.from == e.to || e.lower == e.upper) continue;
      if (uniform_cost && e.cost != 0)
      {
        const Potential unit = e.cost < 0 ? -Potential(e.cost) : Potential(e.cost);
        if (cost_unit == 0) cost_unit = unit;
        else if (cost_unit != unit) uniform_cost = false;
      }
      const int a = g.offset(e.from) + count[e.from]++, b = g.offset(e.to) + count[e.to]++;
      const Capacity forward = Capacity(Balance(e.upper) - e.flow), backward = Capacity(Balance(e.flow) - e.lower);
      data[a] = {e.to, b, forward, Potential(e.cost)};
      data[b] = {e.from, a, backward, Potential(-e.cost)};
      pos[i] = a;
      maximum = max(maximum, max(forward, backward));
    }
    // div(v) = lower[v] + f(r -> v)、div(r) = -sum(lower)。
    // 幅のある供給だけを費用0の辺へ変換し、補助頂点・辺は公開APIから隠す。
    if (intervals) for (int v = 0; v < n; v++) if (supply[v] != supply_upper[v])
    {
      const Capacity width = supply_upper[v] - supply[v];
      const int a = g.offset(n) + count[n]++, b = g.offset(v) + count[v]++;
      data[a] = {v, b, width, Potential(0)};
      data[b] = {n, a, Capacity(0), Potential(0)};
      supply_pos[v] = a;
      maximum = max(maximum, width);
    }
    return maximum;
  }
  bool balanced() const
  {
    for (Balance x : excess) if (x != 0) return false;
    return true;
  }
  Capacity augment(int target, const vc<int> &prev)
  {
    auto &data = g.get_elist();
    int source = target;
    Capacity amount = numeric_limits<Capacity>::max();
    while (prev[source] != -1)
    {
      const auto &e = data[prev[source]];
      amount = min(amount, e.cap);
      source = data[e.rev].to;
    }
    if (excess[source] < Balance(amount)) amount = Capacity(excess[source]);
    if (-excess[target] < Balance(amount)) amount = Capacity(-excess[target]);
    assert(amount > 0);
    for (int v = target; v != source;)
    {
      auto &e = data[prev[v]];
      e.cap -= amount, data[e.rev].cap += amount;
      v = data[e.rev].to;
    }
    excess[source] -= Balance(amount), excess[target] += Balance(amount);
    return amount;
  }
  // 最短路の森で流せる分を、木上の需要の集計と配分の2走査でまとめて流す。
  void augment_forest(ShortestPaths &search, Capacity delta)
  {
    auto &first = search.first_child, &next = search.next_child;
    auto &need = search.need, &available = search.available;
    first.assign(solve_n, -1), next.resize(solve_n);
    need.assign(solve_n, 0), available.assign(solve_n, 0);
    auto &data = g.get_elist();
    const Balance unit = Balance(delta);
    for (int v : search.settled)
    {
      if (excess[v] <= -unit) need[v] = (-excess[v] / unit) * unit;
      const int i = search.prev[v];
      if (i != -1)
      {
        const int p = data[data[i].rev].to;
        next[v] = first[p], first[p] = v;
      }
    }
    for (int j = int(search.settled.size()) - 1; j >= 0; j--)
    {
      const int v = search.settled[j], i = search.prev[v];
      if (i != -1)
      {
        const int p = data[data[i].rev].to;
        const Capacity cap = data[i].cap - data[i].cap % delta;
        need[p] += min(need[v], Balance(cap));
      }
      else available[v] = min<Balance>(need[v], (excess[v] / unit) * unit);
    }
    for (int v : search.settled)
    {
      Balance amount = available[v];
      // 親から流す前の需要を使う。delta 未満の端数はそのまま残す。
      if (search.prev[v] != -1)
      {
        const Balance before = excess[v] - amount;
        if (before < 0) amount -= min<Balance>(amount, (-before / unit) * unit);
      }
      for (int child = first[v]; child != -1 && amount > 0; child = next[child])
      {
        auto &e = data[search.prev[child]];
        const Capacity cap = e.cap - e.cap % delta;
        const Capacity sent = Capacity(min({amount, need[child], Balance(cap)}));
        available[child] = Balance(sent);
        amount -= Balance(sent);
        e.cap -= sent, data[e.rev].cap += sent;
        excess[v] -= Balance(sent), excess[child] += Balance(sent);
      }
    }
    search.work += 6LL * solve_n;
  }
  // Primal network simplex。人工辺の費用は (人工流量, 元の費用) の辞書順で扱う。
  //! 参考: Kiraly--Kovacs, Efficient implementations of minimum-cost flow algorithms, §3.4.
  struct NetworkSimplex
  {
    struct Arc
    {
      int from, to, pos, slot;
      Capacity cap, flow;
      Potential cost;
    };
    MinCostBFlow &owner;
    int vertices, real_arcs = 0, cursor = 0, block_size;
    vc<Arc> arcs;
    vc<int> parent, parent_arc, depth, phase, first_child, prev_sibling, next_sibling, order;
    vc<Potential> price;
    ll work = 0;
    bool feasible = false;

    NetworkSimplex(MinCostBFlow &owner, const vc<Balance> &imbalance)
        : owner(owner), vertices(owner.solve_n), parent(vertices + 1),
          parent_arc(vertices + 1), depth(vertices + 1), phase(vertices + 1),
          first_child(vertices + 1, -1), prev_sibling(vertices + 1), next_sibling(vertices + 1), price(vertices + 1)
    {
      auto &data = owner.g.get_elist();
      arcs.reserve(data.size() / 2 + vertices);
      auto append = [&](int i)
      {
        if (i == -1) return;
        const auto &e = data[i];
        const Capacity cap = e.cap + data[e.rev].cap;
        arcs.push_back({data[e.rev].to, e.to, i, -1, cap, e.cost < 0 ? cap : Capacity(0), e.cost});
      };
      for (int i : owner.pos) append(i);
      for (int i : owner.supply_pos) append(i);
      real_arcs = arcs.size();
      for (int v = 0; v < vertices; v++)
      {
        const int a = arcs.size();
        const bool outward = imbalance[v] >= 0;
        arcs.push_back({outward ? v : vertices, outward ? vertices : v, -1, v,
                        numeric_limits<Capacity>::max(), outward ? Capacity(imbalance[v]) : Capacity(-imbalance[v]), Potential(0)});
        attach(v, vertices, a);
        depth[v] = 1, phase[v] = outward ? -1 : 1;
      }
      parent[vertices] = -1;
      block_size = max(10, int(sqrt(double(arcs.size()))));
      work += vertices + ll(arcs.size());
    }
    void detach(int v)
    {
      const int p = parent[v], previous = prev_sibling[v], next = next_sibling[v];
      if (previous == -1) first_child[p] = next;
      else next_sibling[previous] = next;
      if (next != -1) prev_sibling[next] = previous;
    }
    void attach(int v, int p, int a)
    {
      parent[v] = p, parent_arc[v] = a;
      prev_sibling[v] = -1, next_sibling[v] = first_child[p];
      if (first_child[p] != -1) prev_sibling[first_child[p]] = v;
      first_child[p] = v;
    }
    void update_subtree(int start)
    {
      order.clear(), order.eb(start);
      for (int j = 0; j < int(order.size()); j++)
      {
        const int v = order[j];
        const int p = parent[v];
        const auto &e = arcs[parent_arc[v]];
        const int sign = e.from == p ? 1 : -1;
        depth[v] = depth[p] + 1;
        phase[v] = phase[p] + sign * int(e.pos == -1);
        price[v] = price[p] + (sign == 1 ? e.cost : Potential(-e.cost));
        for (int u = first_child[v]; u != -1; u = next_sibling[u]) order.eb(u);
      }
      work += 8LL * order.size() + 1;
    }
    int entering_arc()
    {
      int best = -1, block = 0;
      pair<int, Potential> score{0, Potential(0)};
      for (int scanned = 0; scanned < int(arcs.size()); scanned++)
      {
        const int a = cursor++;
        if (cursor == int(arcs.size())) cursor = 0;
        const auto &e = arcs[a];
        work++;
        if (e.slot == -1)
        {
          const int direction = e.flow == 0 ? 1 : -1;
          const int first = (int(e.pos == -1) + phase[e.from] - phase[e.to]) * direction;
          Potential second = e.cost + price[e.from] - price[e.to];
          if (direction == -1) second = -second;
          const pair<int, Potential> candidate{first, second};
          if (candidate < score) score = candidate, best = a;
        }
        if (++block == block_size)
        {
          if (best != -1) return best;
          block = 0;
        }
      }
      return best;
    }
    Capacity residual(int a, int from) const
    {
      const auto &e = arcs[a];
      if (e.from != from) return e.flow;
      return e.pos == -1 ? numeric_limits<Capacity>::max() : Capacity(e.cap - e.flow);
    }
    void push(int a, int from, Capacity amount)
    {
      auto &e = arcs[a];
      if (e.from == from) e.flow += amount;
      else e.flow -= amount;
    }
    void pivot(int entering)
    {
      const auto &e = arcs[entering];
      const int u = e.flow == 0 ? e.from : e.to, v = e.flow == 0 ? e.to : e.from;
      int a = u, b = v;
      while (a != b)
      {
        if (depth[a] >= depth[b]) a = parent[a];
        else b = parent[b];
        work++;
      }
      const int join = a;
      Capacity amount = residual(entering, u);
      int leaving = -1, moved = -1;
      bool leave_v_side = false;
      // 同量なら u 側は根から遠い辺、v 側は根に近い辺を選ぶ。
      // v 側を優先して、各頂点から根へ正の量を流せる木を保つ。
      for (int x = u; x != join; x = parent[x])
      {
        const Capacity cap = residual(parent_arc[x], parent[x]);
        if (cap < amount) amount = cap, leaving = parent_arc[x], moved = x;
        work++;
      }
      for (int x = v; x != join; x = parent[x])
      {
        const Capacity cap = residual(parent_arc[x], x);
        if (cap <= amount) amount = cap, leaving = parent_arc[x], moved = x, leave_v_side = true;
        work++;
      }
      push(entering, u, amount);
      for (int x = u; x != join; x = parent[x]) push(parent_arc[x], parent[x], amount);
      for (int x = v; x != join; x = parent[x]) push(parent_arc[x], x, amount);
      if (leaving == -1) return;
      const int slot = arcs[leaving].slot;
      arcs[leaving].slot = -1, arcs[entering].slot = slot;
      const int start = leave_v_side ? v : u, outside = leave_v_side ? u : v;
      int x = start, p = outside, next_arc = entering;
      while (true)
      {
        const int old_parent = parent[x], old_arc = parent_arc[x];
        detach(x), attach(x, p, next_arc);
        if (x == moved) break;
        p = x, next_arc = old_arc, x = old_parent;
        work++;
      }
      update_subtree(start);
    }
    void finish()
    {
      feasible = true;
      for (int i = real_arcs; i < int(arcs.size()); i++) if (arcs[i].flow != 0) feasible = false;
      if (!feasible) return;
      auto &data = owner.g.get_elist();
      for (int i = 0; i < real_arcs; i++)
      {
        const auto &e = arcs[i];
        data[e.pos].cap = e.cap - e.flow, data[data[e.pos].rev].cap = e.flow;
      }
      // 残余辺では phase[from] >= phase[to]。同じ phase 内だけ Dijkstra を使い、
      // phase の大きい順に処理して、数値の巨大定数を使わず通常の双対解へ戻す。
      Potential offset = 0;
      for (int v = 0; v < vertices; v++) offset = max(offset, price[v]);
      vc<Potential> dist(vertices);
      FibonacciHeap<pair<int, Potential>> que(vertices);
      for (int v = 0; v < vertices; v++)
        dist[v] = offset - price[v], que.push(v, {-phase[v], dist[v]});
      while (!que.empty())
      {
        const auto [key, v] = que.top();
        const Potential d = key.second;
        que.pop();
        for (const auto &e : owner.g[v]) if (e.cap != 0)
        {
          assert(phase[v] >= phase[e.to]);
          const Potential next_dist = d + e.cost + price[v] - price[e.to];
          if (next_dist < dist[e.to])
            dist[e.to] = next_dist, que.push_or_decrease(e.to, {-phase[e.to], next_dist});
        }
      }
      for (int v = 0; v < vertices; v++) owner.price[v] = dist[v] + price[v] - offset;
      fill(ALL(owner.excess), Balance(0));
    }
    bool run(ll budget = -1)
    {
      const ll start = work;
      while (true)
      {
        const int entering = entering_arc();
        if (entering == -1) { finish(); return true; }
        pivot(entering);
        if (budget >= 0 && work - start >= budget) return false;
      }
    }
  };

  struct ScalingState
  {
    Capacity delta = 0;
    bool refine = true;
    explicit ScalingState(Capacity maximum)
    {
      if (maximum == 0) return;
      delta = 1;
      while (delta <= maximum / 2) delta *= 2;
    }
  };
  // 各段階の residual-capacity >= delta の辺で双対実行可能性を保つ。
  // 最短路の更新・増加を完了した境界で停止し、後から再開できる。
  bool capacity_scaling(ShortestPaths &search, ScalingState &state, ll budget = -1)
  {
    const ll start = search.work;
    auto &data = g.get_elist();
    while (state.delta != 0)
    {
      if (state.refine)
      {
        for (int v = 0; v < solve_n; v++) for (auto &e : g[v])
          if (e.cap >= state.delta && e.cost + price[v] - price[e.to] < 0)
          {
            const Capacity amount = e.cap - e.cap % state.delta;
            e.cap -= amount, data[e.rev].cap += amount;
            excess[v] -= Balance(amount), excess[e.to] += Balance(amount);
          }
        search.work += solve_n + ll(data.size());
        state.refine = false;
      }
      const int target = search.run(state.delta, -1, -1, true);
      if (target == -1) state.delta /= 2, state.refine = true;
      else augment_forest(search, state.delta);
      if (budget >= 0 && search.work - start >= budget) return state.delta == 0;
    }
    return true;
  }
  bool successive_shortest_paths(ShortestPaths &search, ll budget = -1)
  {
    const ll start = search.work;
    while (!balanced())
    {
      const int target = search.run(Capacity(1));
      if (target == -1) return true;
      augment(target, search.prev);
      if (budget >= 0 && search.work - start >= budget) return balanced();
    }
    return true;
  }
  bool primal_dual(ShortestPaths &search, ll budget = -1)
  {
    const ll start = search.work;
    while (!balanced())
    {
      if (search.run(Capacity(1), -1, -1, true) == -1) return true;
      augment_forest(search, Capacity(1));
      if (budget >= 0 && search.work - start >= budget) return balanced();
    }
    return true;
  }
  void collect()
  {
    value = 0;
    const auto &data = g.get_elist();
    for (int i = 0; i < int(input.size()); i++)
    {
      auto &e = input[i];
      if (pos[i] != -1) e.flow = Cap(Balance(e.upper) - Balance(data[pos[i]].cap));
      value = add_cost(value, e.flow, e.cost);
    }
    normalized_price.clear();
  }

public:
  MinCostBFlow() = default;
  explicit MinCostBFlow(int n) : n(n), supply(n), supply_upper(n) { assert(n >= 0); }
  explicit MinCostBFlow(const vc<Cap> &b) : n(b.size()), supply(ALL(b)), supply_upper(ALL(b)) {}
  template <class I = ll> I size() const { return n; }
  template <class I = ll> I num_of_edges() const { return input.size(); }
  void set_supply(int v, Cap lower, Cap upper)
  {
    assert(0 <= v && v < n && lower <= upper);
    supply[v] = lower, supply_upper[v] = upper;
    solved = false;
  }
  void add_supply(int v, Cap lower, Cap upper)
  {
    assert(0 <= v && v < n && lower <= upper);
    supply[v] += lower, supply_upper[v] += upper;
    solved = false;
  }
  void add_supply(int v, Cap amount) { add_supply(v, amount, amount); }
  Cap get_supply(int v) const
  {
    assert(solved && 0 <= v && v < n);
    if (supply_pos.empty() || supply_pos[v] == -1) return supply[v];
    const auto &data = g.get_elist();
    return supply[v] + data[data[supply_pos[v]].rev].cap;
  }
  int add_edge(int from, int to, Cap lower, Cap upper, Cost cost)
  {
    assert(0 <= from && from < n && 0 <= to && to < n && lower <= upper);
    const int id = input.size();
    input.push_back({from, to, lower, upper, cost, lower});
    solved = false;
    return id;
  }
  int add_edge(int from, int to, Cap upper, Cost cost) { return add_edge(from, to, Cap(0), upper, cost); }
  edge get_edge(int i) const { assert(0 <= i && i < int(input.size())); return input[i]; }
  vc<edge> edges() const { return input; }

  optional<Total> flow(Algorithm algorithm = Algorithm::automatic, Heap heap = Heap::automatic)
  {
    Balance sum = 0;
    bool intervals = false;
    for (int v = 0; v < n; v++) sum += supply[v], intervals |= supply[v] != supply_upper[v];
    solved = false;
    if (!intervals && sum != 0) return nullopt;
    const Capacity maximum = prepare(sum, intervals);
    int positive = 0, negative = 0;
    Balance initial_surplus = 0;
    for (Balance x : excess)
    {
      positive += x > 0, negative += x < 0;
      if (x > 0) initial_surplus += x;
    }
    last_heap = select_heap(heap);
    const bool multiple = min(positive, negative) > 8;
    if (heap == Heap::automatic && (multiple || initial_surplus <= 8) && last_heap == Heap::fibonacci) last_heap = Heap::binary;
    unique_ptr<NetworkSimplex> simplex;
    const bool small_capacitated = solve_n <= 512 && maximum > 1;
    const bool sparse_unit = solve_n <= 4096 && maximum == 1 && !multiple && initial_surplus > solve_n / 8
                             && ll(g.get_elist().size()) <= 32LL * solve_n;
    if (algorithm == Algorithm::automatic && initial_surplus > 8 && !uniform_cost && (small_capacitated || sparse_unit))
    {
      simplex = make_unique<NetworkSimplex>(*this, excess);
      if (simplex->run(256LL * (solve_n + ll(g.get_elist().size()) + 1)))
      {
        solved = balanced();
        collect();
        return solved ? optional<Total>(value) : nullopt;
      }
    }
    ShortestPaths search(*this, last_heap);
    ScalingState scaling(maximum);
    if (algorithm == Algorithm::network_simplex)
    {
      NetworkSimplex simplex(*this, excess);
      simplex.run();
    }
    else if (algorithm == Algorithm::capacity_scaling) capacity_scaling(search, scaling);
    else if (algorithm == Algorithm::primal_dual || (algorithm == Algorithm::automatic && maximum <= 1 && multiple))
      primal_dual(search);
    else if (algorithm == Algorithm::ssp || maximum <= 1) successive_shortest_paths(search);
    else
    {
      ll budget = 32LL * (solve_n + ll(g.get_elist().size()) + 1);
      auto primary = [&](ll quota)
      { return multiple ? primal_dual(search, quota) : successive_shortest_paths(search, quota); };
      bool finished = primary(budget);
      if (!finished && simplex) finished = simplex->run(budget);
      if (!finished)
      {
        Balance remaining = 0;
        for (Balance x : excess) if (x > 0) remaining += x;
        const Balance progress = initial_surplus - remaining;
        if (progress > 0 && remaining / 16 <= progress)
        {
          budget *= 32;
          finished = primary(budget);
        }
      }
      if (!finished)
      {
        // 非スケーリング側の状態を保存し、容量スケーリング・simplex と交互に進める。
        // 予算を倍増し、入力依存の切替判断に加えて SSP の上界も保つ。
        CSR<ResidualEdge> other_graph = g;
        vc<Balance> other_excess = excess;
        vc<Potential> other_price = price;
        auto exchange_state = [&]
        {
          swap(g, other_graph), excess.swap(other_excess), price.swap(other_price);
        };
        while (!capacity_scaling(search, scaling, budget))
        {
          if (simplex && simplex->run(budget)) break;
          exchange_state();
          if (primary(budget)) break;
          exchange_state();
          budget *= 2;
        }
      }
    }
    solved = balanced();
    collect();
    return solved ? optional<Total>(value) : nullopt;
  }

  // 最適解から s の供給区間を +x、t の供給区間を -x ずらす追加費用の折れ線。
  vc<pair<Cap, Total>> slope(int s, int t, Cap limit = numeric_limits<Cap>::max(), Heap heap = Heap::automatic)
  {
    assert(solved && 0 <= s && s < n && 0 <= t && t < n && s != t && limit >= 0);
    vc<pair<Cap, Total>> result{{0, Total(0)}};
    if (limit == 0) return result;
    if (heap == Heap::automatic && limit <= 8 && select_heap(heap) == Heap::fibonacci) heap = Heap::binary;
    ShortestPaths search(*this, heap);
    excess[s] = Balance(limit), excess[t] = -Balance(limit);
    Cap sent = 0;
    Total cost = 0;
    Potential previous = 0;
    bool has_previous = false;
    while (sent < limit && search.run(Capacity(1), s, t) != -1)
    {
      const Potential marginal = price[t] - price[s];
      const Capacity amount = augment(t, search.prev);
      sent += Cap(amount), cost = add_cost(cost, amount, marginal);
      if (has_previous && previous == marginal) result.pop_back();
      result.eb(sent, cost);
      previous = marginal, has_previous = true;
    }
    excess[s] = excess[t] = 0;
    supply[s] += Balance(sent), supply[t] -= Balance(sent);
    supply_upper[s] += Balance(sent), supply_upper[t] -= Balance(sent);
    collect();
    return result;
  }
  pair<Cap, Total> flow(int s, int t, Cap limit = numeric_limits<Cap>::max(), Heap heap = Heap::automatic)
  { return slope(s, t, limit, heap).back(); }

  // 全頂点を始点とする reweighted Dijkstra。区間供給では補助頂点の値を0にする。
  vc<Potential> potential() const
  {
    assert(solved);
    if (int(normalized_price.size()) == n) return normalized_price;
    vc<Potential> dist(solve_n);
    pql<pair<Potential, int>> que;
    Potential offset = 0;
    for (Potential p : price) offset = max(offset, p);
    for (int v = 0; v < solve_n; v++) dist[v] = offset - price[v], que.push({dist[v], v});
    while (!que.empty())
    {
      const auto [d, v] = que.top();
      que.pop();
      if (dist[v] != d) continue;
      for (const auto &e : g[v]) if (e.cap != 0)
      {
        const Potential next = d + e.cost + price[v] - price[e.to];
        if (next < dist[e.to]) dist[e.to] = next, que.push({next, e.to});
      }
    }
    if (solve_n != n) offset = dist[n] + price[n];
    normalized_price.resize(n);
    for (int v = 0; v < n; v++) normalized_price[v] = dist[v] + price[v] - offset;
    return normalized_price;
  }
};
