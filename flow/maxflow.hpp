#pragma once

#include "../template/template_all_but_modint.hpp"
#include "../utils/is_integral_ext.hpp"
#include "../ds/csr.hpp"

/**
 * @brief 最大流・最小カット
 * @docs docs/flow/maxflow.md
 */

template <class Cap>
struct MaxFlow
{
  static_assert(is_integral_ext<Cap> && !is_same_v<Cap, bool>);

  struct edge
  {
    int from, to;
    Cap cap, flow;
    CPP_DUMP_DEFINE_DATA(from, to, cap, flow);
  };

private:
  struct ResidualEdge
  {
    int to, rev;
    Cap cap;
  };

  int n = 0;
  mutable CSR<ResidualEdge> g;
  mutable vc<int> pos;
  mutable vc<edge> pending;
  // pos >= 0 は残余辺、pos < 0 は originals 内の元の辺を指す。
  // originals の各行は集約辺の構成要素。最後の行は自己ループ・容量 0 の辺。
  mutable CSR<edge> originals;
  mutable vc<int> merged_pos;
  mutable vc<Cap> merged_flow;
  mutable int built = 0;
  mutable bool dirty = false;

  bool merge_parallel_edges() const
  {
    // サンプルの約 1/64 以上が多重辺なら、集約用の配列を作る。
    array<uint64_t, 1024> seen{};
    const int m = num_of_edges<int>(), samples = min(m, 1024);
    int repeated = 0, omitted = 0;
    for (int j = 0; j < samples; j++)
    {
      // 64 箇所から連続する 16 本ずつ採り、隣接して追加された多重辺も拾う。
      const int i = m <= 1024 ? j : int(ll(j / 16) * m / 64) + j % 16;
      const auto e = get_edge(i);
      if (e.from == e.to || e.cap == 0) { omitted++; continue; }
      const uint64_t key = (uint64_t(e.from) << 32) | uint32_t(e.to);
      const int slot = (key * 0x9e3779b97f4a7c15ULL) >> 54;
      if (seen[slot] == key) repeated++;
      seen[slot] = key;
    }
    // 除外する辺が多ければ、まずフィルタだけ行う軽い構築を選ぶ。
    return 2 * omitted <= samples && repeated >= max(1, samples / 64);
  }

  Cap finish_flow(Cap result)
  {
    const auto &data = g.get_elist();
    for (int k = 0; k < int(merged_pos.size()); k++)
    {
      const Cap current = data[data[merged_pos[k]].rev].cap;
      const Cap previous = merged_flow[k];
      if (current == previous) continue;
      Cap remaining = current > previous ? current - previous : previous - current;
      for (auto &e : originals[k])
      {
        const Cap f = min<Cap>(remaining, current > previous ? e.cap - e.flow : e.flow);
        if (current > previous) e.flow += f;
        else e.flow -= f;
        remaining -= f;
        if (remaining == 0) break;
      }
      assert(remaining == 0);
      merged_flow[k] = current;
    }
    return result;
  }

  void check_flow(int s, int t, Cap limit) const
  {
    assert(0 <= s && s < n && 0 <= t && t < n && s != t);
    assert(Cap(0) <= limit);
  }

  Cap flow_bound(int s, int t, Cap limit) const
  {
    auto cut_bound = [&](int v, bool reverse)
    {
      Cap sum = 0;
      for (const auto &e : g[v]) if (e.to != v)
      {
        const Cap c = reverse ? g.get_elist()[e.rev].cap : e.cap;
        if (c >= limit - sum) return limit;
        sum += c;
      }
      return sum;
    };
    limit = min(limit, cut_bound(s, false));
    return min(limit, cut_bound(t, true));
  }

  // 容量の共通単位で測った通過容量・流量を使い、Dinic の予算倍率を選ぶ。
  // 自己ループを除き、現在の残余ネットワークをそのまま調べる。
  int dinic_budget_factor(int s, int t, Cap progress) const
  {
    Cap unit = 0;
    auto check = [&](const vc<bool> &use)
    {
      auto selected = [&](int v) { return use.empty() || use[v]; };
      unit = [&]
      {
        Cap result = 0;
        for (int v = 0; v < n; v++) if (selected(v))
          for (const auto &e : g[v]) if (e.to != v && selected(e.to))
          {
            Cap a = e.cap;
            while (a != 0)
            {
              const Cap b = result % a;
              result = a, a = b;
            }
            if (result == 1) return result;
          }
        return result;
      }();
      if (unit == 0) return true;
      for (int v = 0; v < n; v++) if (v != s && v != t && selected(v))
      {
        int in = 0, out = 0;
        auto add = [&](int &sum, Cap cap)
        {
          if (sum > 8) return;
          const Cap value = unit == 1 ? cap : cap / unit;
          sum = value > Cap(8 - sum) ? 9 : sum + int(value);
        };
        for (const auto &e : g[v]) if (e.to != v && selected(e.to))
        {
          add(out, e.cap), add(in, g.get_elist()[e.rev].cap);
          if (in > 8 && out > 8) return false;
        }
      }
      return true;
    };
    if (check({})) return 8;
    // 増加路に含まれない成分や行き止まりは、選択の判定から除く。
    auto use = min_cut(s);
    vc<bool> reaches_t(n, false);
    vc<int> que(n);
    int front = 0, back = 0;
    reaches_t[t] = true, que[back++] = t;
    while (front < back)
    {
      const int v = que[front++];
      for (const auto &e : g[v])
        if (!reaches_t[e.to] && g.get_elist()[e.rev].cap > 0)
          reaches_t[e.to] = true, que[back++] = e.to;
    }
    for (int v = 0; v < n; v++) use[v] = use[v] && reaches_t[v];
    // 行き止まりを除いた端点のカットで、残りの流量を評価し直す。
    auto small_remaining_cut = [&](int v, bool reverse)
    {
      Cap remaining = progress;
      for (const auto &e : g[v]) if (e.to != v && use[e.to])
      {
        const Cap cap = reverse ? g.get_elist()[e.rev].cap : e.cap;
        if (cap > remaining) return false;
        remaining -= cap;
      }
      return true;
    };
    if (small_remaining_cut(s, false) || small_remaining_cut(t, true)) return 8;
    if (check(use)) return 8;
    // 小容量の辺を外した到達集合から、流量が小さいことを証明できる場合。
    vc<bool> large_reachable(n, false);
    front = back = 0;
    large_reachable[s] = true, que[back++] = s;
    while (front < back)
    {
      const int v = que[front++];
      for (const auto &e : g[v]) if (use[e.to] && !large_reachable[e.to])
      {
        const Cap value = unit == 1 ? e.cap : e.cap / unit;
        if (value <= 8) continue;
        if (e.to == t) return 1;
        large_reachable[e.to] = true, que[back++] = e.to;
      }
    }
    int cut = 0;
    for (int v = 0; v < n; v++) if (large_reachable[v])
      for (const auto &e : g[v]) if (use[e.to] && !large_reachable[e.to])
      {
        const Cap value = unit == 1 ? e.cap : e.cap / unit;
        if (value > Cap(32 - cut)) return 1;
        cut += int(value);
      }
    return 64;
  }

  // 終点側からの blocking flow。深い探索は明示的なスタックを使う。
  // budget に達した場合も、スタックを戻してフロー保存則を満たしてから返す。
  template <bool adaptive>
  Cap run_dinic(int s, int t, Cap limit, ll &budget, bool &finished, int *structure = nullptr)
  {
    vc<int> level(n), iter(n), que(n), path;
    vc<Cap> bound, sent;
    Cap total = 0;
    ll work = 0;
    finished = true;
    while (total < limit)
    {
      fill(ALL(level), -1);
      if constexpr (adaptive) work += n;
      int front = 0, back = 0;
      level[s] = 0;
      que[back++] = s;
      while (front < back && level[t] == -1)
      {
        const int v = que[front++];
        for (const auto &e : g[v])
        {
          if constexpr (adaptive) work++;
          if (e.cap == 0 || level[e.to] != -1) continue;
          level[e.to] = level[v] + 1;
          if (e.to == t) break;
          que[back++] = e.to;
        }
      }
      if (level[t] == -1) return total;
      // 長い経路では増加ごとの処理も重くなるので、早めに切り替える。
      ll phase_budget = budget * (level[t] <= 16 ? 8 : 2);
      if constexpr (adaptive)
        if (work >= phase_budget)
        {
          if (structure && *structure == 0)
          {
            *structure = total >= limit - total ? 8 : dinic_budget_factor(s, t, total);
            budget *= *structure, phase_budget *= *structure;
          }
          if (work >= phase_budget) { finished = false; return total; }
        }

      fill(ALL(iter), 0);
      if constexpr (adaptive) work += n;
      bool stop = false;
      if (level[t] <= 64)
      {
        auto dfs = [&](auto &self, int v, Cap up) -> Cap
        {
          if (v == s) return up;
          Cap res = 0;
          const int level_v = level[v];
          const auto row = g[v];
          for (int &i = iter[v]; i < int(row.size()); i++)
          {
            if constexpr (adaptive)
              if (++work >= phase_budget) { stop = true; return res; }
            auto &e = row[i];
            if (level_v <= level[e.to] || g.get_elist()[e.rev].cap == 0) continue;
            const Cap f = self(self, e.to, min<Cap>(up - res, g.get_elist()[e.rev].cap));
            e.cap += f, g.get_elist()[e.rev].cap -= f;
            res += f;
            if (res == up || stop) return res;
          }
          level[v] = n;
          return res;
        };
        total += dfs(dfs, t, limit - total);
        if (stop) { finished = false; return total; }
        continue;
      }
      if (path.empty()) path.resize(n), bound.resize(n), sent.resize(n);
      int depth = 0;
      path[0] = t, bound[0] = limit - total, sent[0] = 0;
      while (true)
      {
        const int v = path[depth];
        if (v != s && sent[depth] < bound[depth] && !stop)
        {
          int &i = iter[v];
          const auto row = g[v];
          while (i < int(row.size()))
          {
            if constexpr (adaptive)
              if (++work >= phase_budget) { stop = true; break; }
            const auto &e = row[i];
            if (level[e.to] < level[v] && g.get_elist()[e.rev].cap > 0) break;
            i++;
          }
          if (!stop && i < int(row.size()))
          {
            const auto &e = row[i];
            const Cap next_bound = min<Cap>(bound[depth] - sent[depth], g.get_elist()[e.rev].cap);
            path[++depth] = e.to;
            bound[depth] = next_bound, sent[depth] = 0;
            continue;
          }
          if (!stop) level[v] = n;
        }

        const Cap f = v == s ? bound[depth] : sent[depth];
        if (depth == 0)
        {
          total += f;
          break;
        }
        const int p = path[--depth];
        const auto row = g[p];
        auto &e = row[iter[p]];
        e.cap += f;
        g.get_elist()[e.rev].cap -= f;
        sent[depth] += f;
        if (sent[depth] < bound[depth]) iter[p]++;
      }
      if (stop) { finished = false; return total; }
    }
    return total;
  }

public:
  MaxFlow() = default;
  explicit MaxFlow(int n) : n(n)
  {
    assert(0 <= n);
  }

  template <class I = ll>
  I size() const { return n; }
  template <class I = ll>
  I num_of_edges() const { return built + pending.size(); }

  int add_edge(int from, int to, Cap cap)
  {
    assert(0 <= from && from < n && 0 <= to && to < n);
    assert(Cap(0) <= cap);
    const int id = num_of_edges<int>();
    pending.push_back({from, to, cap, 0});
    return id;
  }

  // 元の辺を同方向ごとに集約し、探索用の残余グラフを構築する。
  void build() const
  {
    if (pending.empty() && !dirty && g.template size<int>() == n) return;
    const int m = num_of_edges<int>();
    if (!merge_parallel_edges())
    {
      vc<int> degree(n, 0);
      int omitted = 0;
      for (int i = 0; i < m; i++)
      {
        const auto e = get_edge(i);
        if (e.from == e.to || e.cap == 0) omitted++;
        else degree[e.from]++, degree[e.to]++;
      }
      CSR<ResidualEdge> next_graph(degree);
      CSR<edge> next_originals(vc<int>{omitted});
      vc<int> next_pos(m), count(n, 0);
      auto &data = next_graph.get_elist();
      int special = 0;
      for (int i = 0; i < m; i++)
      {
        const auto e = get_edge(i);
        if (e.from == e.to || e.cap == 0)
        {
          next_originals.get_elist()[special] = e;
          next_pos[i] = ~special++;
          continue;
        }
        const int a = next_graph.offset(e.from) + count[e.from]++;
        const int b = next_graph.offset(e.to) + count[e.to]++;
        data[a] = {e.to, b, Cap(e.cap - e.flow)};
        data[b] = {e.from, a, e.flow};
        next_pos[i] = a;
      }
      swap(g, next_graph), swap(originals, next_originals);
      pos.swap(next_pos);
      vc<int>().swap(merged_pos), vc<Cap>().swap(merged_flow);
      built = pos.size(), dirty = false;
      vc<edge>().swap(pending);
      return;
    }
    vc<int> degree(n, 0);
    for (int i = 0; i < m; i++)
    {
      const auto e = get_edge(i);
      if (e.from != e.to && e.cap > 0) degree[e.from]++;
    }
    CSR<int> outgoing(degree);
    vc<int> count(n, 0);
    for (int i = 0; i < m; i++)
    {
      const auto e = get_edge(i);
      if (e.from != e.to && e.cap > 0)
        outgoing.get_elist()[outgoing.offset(e.from) + count[e.from]++] = i;
    }
    vc<edge> combined;
    vc<int> group(m, -1), last(n, -1), multiplicity;
    combined.reserve(outgoing.get_elist().size());
    multiplicity.reserve(outgoing.get_elist().size());
    for (int v = 0; v < n; v++) for (int i : outgoing[v])
    {
      const auto e = get_edge(i);
      int k = last[e.to];
      // 容量の総和が型を越える場合は、別の集約辺へ分ける。
      if (k == -1 || combined[k].from != v || e.cap > numeric_limits<Cap>::max() - combined[k].cap)
      {
        k = combined.size();
        last[e.to] = k;
        combined.eb(e), multiplicity.eb(1);
      }
      else
      {
        combined[k].cap += e.cap, combined[k].flow += e.flow;
        multiplicity[k]++;
      }
      group[i] = k;
    }
    fill(ALL(degree), 0);
    vc<int> special_row(combined.size(), -1), row_sizes;
    vc<int> next_merged_pos;
    vc<Cap> next_merged_flow;
    for (int k = 0; k < int(combined.size()); k++)
    {
      const auto &e = combined[k];
      degree[e.from]++, degree[e.to]++;
      if (multiplicity[k] > 1)
      {
        special_row[k] = row_sizes.size();
        row_sizes.eb(multiplicity[k]);
        next_merged_flow.eb(e.flow);
      }
    }
    row_sizes.eb(m - outgoing.get_elist().size());
    CSR<edge> next_originals(row_sizes);
    CSR<ResidualEdge> next_graph(degree);
    vc<int> next_pos(m), combined_pos(combined.size(), -1);
    next_merged_pos.resize(next_merged_flow.size());
    fill(ALL(count), 0);
    auto &data = next_graph.get_elist();
    for (int i = 0; i < m; i++)
    {
      const int k = group[i];
      if (k == -1 || combined_pos[k] != -1) continue;
      const auto &e = combined[k];
      const int a = next_graph.offset(e.from) + count[e.from]++;
      const int b = next_graph.offset(e.to) + count[e.to]++;
      data[a] = {e.to, b, Cap(e.cap - e.flow)};
      data[b] = {e.from, a, e.flow};
      combined_pos[k] = a;
      if (special_row[k] != -1) next_merged_pos[special_row[k]] = a;
    }
    count.assign(row_sizes.size(), 0);
    for (int i = 0; i < m; i++)
    {
      const int k = group[i];
      if (k != -1 && special_row[k] == -1) next_pos[i] = combined_pos[k];
      else
      {
        const int row = k == -1 ? int(row_sizes.size()) - 1 : special_row[k];
        const int index = next_originals.offset(row) + count[row]++;
        next_originals.get_elist()[index] = get_edge(i);
        next_pos[i] = ~index;
      }
    }
    swap(g, next_graph);
    swap(originals, next_originals);
    pos.swap(next_pos);
    merged_pos.swap(next_merged_pos), merged_flow.swap(next_merged_flow);
    built = pos.size();
    dirty = false;
    vc<edge>().swap(pending);
  }

  edge get_edge(int i) const
  {
    assert(0 <= i && i < num_of_edges<int>());
    if (i >= built) return pending[i - built];
    if (pos[i] < 0) return originals.get_elist()[~pos[i]];
    const auto &data = g.get_elist();
    const auto &e = data[pos[i]], &r = data[e.rev];
    return {r.to, e.to, Cap(e.cap + r.cap), r.cap};
  }

  vc<edge> edges() const
  {
    vc<edge> res;
    const int m = num_of_edges<int>();
    res.reserve(m);
    for (int i = 0; i < m; i++) res.eb(get_edge(i));
    return res;
  }

  void change_edge(int i, Cap new_cap, Cap new_flow)
  {
    assert(0 <= i && i < num_of_edges<int>());
    assert(Cap(0) <= new_flow && new_flow <= new_cap);
    if (i >= built)
    {
      auto &e = pending[i - built];
      e.cap = new_cap, e.flow = new_flow;
      return;
    }
    if (pos[i] < 0)
    {
      auto &e = originals.get_elist()[~pos[i]];
      if (e.from != e.to && (e.cap != new_cap || e.flow != new_flow)) dirty = true;
      e.cap = new_cap, e.flow = new_flow;
      return;
    }
    auto &data = g.get_elist();
    auto &e = data[pos[i]], &r = data[e.rev];
    e.cap = new_cap - new_flow, r.cap = new_flow;
    if (new_cap == 0) dirty = true;
  }

  Cap dinic(int s, int t, Cap flow_limit = numeric_limits<Cap>::max())
  {
    check_flow(s, t, flow_limit);
    if (flow_limit == 0) return 0;
    build();
    flow_limit = flow_bound(s, t, flow_limit);
    if (flow_limit == 0) return 0;
    bool finished;
    ll budget = 0;
    return finish_flow(run_dinic<false>(s, t, flow_limit, budget, finished));
  }

private:
  // Highest-label push-relabel。仮想始点から上限分だけ供給し、超過分は返す。
  // 余剰流の総和も flow_limit 以下に保つので、容量の合計を別の型で保持せずに済む。
  // bounded では予算ごとに resume を呼び、別の探索を進めてから再開する。
  template <bool bounded, class F>
  Cap run_push_relabel(int s, int t, Cap flow_limit, ll budget, bool &finished, F resume)
  {
    const int source = n, N = n + 1, max_h = 2 * N;
    // 仮想辺 s -> source は別に保持し、CSR の構造を固定する。
    Cap return_capacity = flow_limit;

    vc<Cap> excess(N, 0);
    vc<int> height(N), iter(N), count(max_h + 1), head(max_h + 1, -1);
    vc<int> next(N), que(N);
    vc<unsigned char> active(N, false);
    int highest = -1;
    ll work = 0;
    ll spent = 0;
    bool interrupted = false;
    if constexpr (bounded) spent = 8LL * N;
    auto exhausted = [&]
    {
      if constexpr (bounded)
        if (spent >= budget)
        {
          const ll extension = resume();
          if (extension == 0) { interrupted = true; return true; }
          budget = spent + extension;
        }
      return false;
    };
    const ll global_budget = 2LL * g.get_elist().size() + 6LL * N;

    auto activate = [&](int v)
    {
      if (v == source || v == t || excess[v] == 0 || active[v]) return;
      assert(height[v] < max_h);
      next[v] = head[height[v]], head[height[v]] = v;
      active[v] = true;
      highest = max(highest, height[v]);
    };
    auto rebuild_active = [&](int current)
    {
      if constexpr (bounded) spent += 4LL * N;
      fill(ALL(head), -1);
      fill(ALL(active), false);
      highest = -1;
      for (int v = 0; v < n; v++) if (v != current) activate(v);
    };
    auto global_relabel = [&]
    {
      if constexpr (bounded) spent += 4LL * N;
      fill(ALL(height), max_h);
      fill(ALL(iter), 0);
      // 終点へ送れる頂点の距離を求める。
      int front = 0, back = 0;
      height[t] = 0, que[back++] = t;
      while (front < back)
      {
        const int v = que[front++];
        for (const auto &e : g[v])
        {
          if constexpr (bounded) spent++;
          if (height[e.to] == max_h && g.get_elist()[e.rev].cap > 0)
            height[e.to] = height[v] + 1, que[back++] = e.to;
        }
      }
      // 残りは仮想始点へ返す距離を使う。
      front = back = 0;
      height[source] = N;
      if (height[s] == max_h && return_capacity > 0)
        height[s] = N + 1, que[back++] = s;
      while (front < back)
      {
        const int v = que[front++];
        for (const auto &e : g[v])
        {
          if constexpr (bounded) spent++;
          if (height[e.to] == max_h && g.get_elist()[e.rev].cap > 0)
            height[e.to] = height[v] + 1, que[back++] = e.to;
        }
      }
      fill(ALL(count), 0);
      for (int h : height) count[h]++;
      rebuild_active(-1);
      work = 0;
    };

    excess[s] = flow_limit;
    global_relabel();
    while (highest >= 0 && !exhausted())
    {
      if constexpr (bounded) spent++;
      if (head[highest] == -1) { highest--; continue; }
      const int v = head[highest];
      head[highest] = next[v], active[v] = false;
      const auto row = g[v];
      while (excess[v] > 0 && !exhausted())
      {
        int &i = iter[v];
        const int degree = row.size();
        if (i == degree + (v == s))
        {
          const int old = height[v];
          int h = max_h;
          if (v == s)
          {
            work++;
            if constexpr (bounded) spent++;
            if (return_capacity > 0) h = N + 1;
          }
          for (const auto &e : g[v])
          {
            work++;
            if constexpr (bounded) spent++;
            if (e.cap > 0) h = min(h, height[e.to] + 1);
          }
          assert(h < max_h);
          height[v] = h, i = 0;
          count[old]--, count[h]++;
          if (old < N && count[old] == 0)
          {
            if constexpr (bounded) spent += n;
            // gap より上の頂点は、終点へ到達する経路を持たない。
            for (int u = 0; u < n; u++) if (old < height[u] && height[u] < N)
            {
              count[height[u]]--;
              height[u] = N + 1, count[N + 1]++, iter[u] = 0;
            }
            rebuild_active(v);
          }
          continue;
        }
        work++;
        if constexpr (bounded) spent++;
        if (i == degree)
        {
          if (return_capacity > 0 && height[v] == N + 1)
          {
            const Cap f = min(excess[v], return_capacity);
            return_capacity -= f;
            excess[v] -= f, excess[source] += f;
          }
          else i++;
          continue;
        }
        auto &e = row[i];
        if (e.cap > 0 && height[v] == height[e.to] + 1)
        {
          const Cap f = min(excess[v], e.cap);
          e.cap -= f, g.get_elist()[e.rev].cap += f;
          excess[v] -= f, excess[e.to] += f;
          activate(e.to);
        }
        else i++;
      }
      if (excess[v] > 0) break;
      if (work >= global_budget) global_relabel();
    }
    finished = !interrupted;
    const Cap result = excess[t];
    return result;
  }

public:
  Cap push_relabel(int s, int t, Cap flow_limit = numeric_limits<Cap>::max())
  {
    check_flow(s, t, flow_limit);
    if (flow_limit == 0) return 0;
    build();
    flow_limit = flow_bound(s, t, flow_limit);
    if (flow_limit == 0) return 0;
    bool finished;
    return finish_flow(run_push_relabel<false>(s, t, flow_limit, 0, finished, [] { return 0LL; }));
  }

  Cap flow(int s, int t, Cap flow_limit = numeric_limits<Cap>::max())
  {
    check_flow(s, t, flow_limit);
    if (flow_limit == 0) return 0;
    build();
    flow_limit = flow_bound(s, t, flow_limit);
    if (flow_limit == 0) return 0;
    // 小さいグラフでは Dinic を使う。それ以外は探索量で切り替える。
    bool finished;
    ll budget = n + ll(g.get_elist().size());
    if (n < 128 || g.get_elist().size() < 1024)
      return finish_flow(run_dinic<false>(s, t, flow_limit, budget, finished));
    int structure = 0;
    Cap total = run_dinic<true>(s, t, flow_limit, budget, finished, &structure);
    if (finished || total == flow_limit) return finish_flow(total);
    if (structure == 0)
    {
      structure = total >= flow_limit - total ? 8 : dinic_budget_factor(s, t, total);
      if (structure > 1)
      {
        budget *= structure;
        total += run_dinic<true>(s, t, flow_limit - total, budget, finished);
        if (finished || total == flow_limit) return finish_flow(total);
      }
    }

    // Push–Relabel も探索量を制限し、両方式の状態を保って交互に進める。
    // 試行予算を倍増することで、Dinic の計算量保証を保つ。
    vc<Cap> saved;
    saved.reserve(g.get_elist().size());
    for (const auto &e : g.get_elist()) saved.eb(e.cap);
    CSR<ResidualEdge> dinic_graph;
    Cap extra = 0;
    auto resume = [&]() -> ll
    {
      if (dinic_graph.template size<int>() == 0)
      {
        dinic_graph = g;
        // 保存した容量で Dinic の再開地点を復元する。
        int index = 0;
        for (auto &e : dinic_graph.get_elist()) e.cap = saved[index++];
        vc<Cap>().swap(saved);
      }
      swap(g, dinic_graph);
      budget *= 2;
      bool done;
      extra += run_dinic<true>(s, t, flow_limit - total - extra, budget, done);
      swap(g, dinic_graph);
      return done ? 0 : 8 * budget;
    };
    const Cap f = run_push_relabel<true>(s, t, flow_limit - total, 8 * budget, finished, resume);
    if (finished) return finish_flow(total + f);
    swap(g, dinic_graph);
    return finish_flow(total + extra);
  }

  // s-t 最大流を流した後は、1 (true) が s 側、0 (false) が t 側。
  // 各頂点が現在の残余グラフで s から到達可能かを返す。
  vc<bool> min_cut(int s) const
  {
    assert(0 <= s && s < n);
    build();
    vc<bool> seen(n, false);
    vc<int> que(n);
    int front = 0, back = 0;
    seen[s] = true, que[back++] = s;
    while (front < back)
    {
      const int v = que[front++];
      for (const auto &e : g[v]) if (e.cap > 0 && !seen[e.to])
        seen[e.to] = true, que[back++] = e.to;
    }
    return seen;
  }
};
