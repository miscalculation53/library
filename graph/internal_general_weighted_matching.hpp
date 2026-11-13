#pragma once

#include "../ds/csr.hpp"
#include "../ds/segtree/segtree.hpp"

namespace internal
{
template <class Cost>
struct WeightedMatchingEdge
{
  int from, to;
  Cost cost;
};

template <class Weight, class Cost, bool sparse>
class GeneralWeightedMatching
{
  enum Label { outer, inner, unlabeled };
  enum class Kind { augment, grow, shrink, expand, stop };

  struct Event
  {
    Weight remaining = 0;
    Kind kind = Kind::stop;
    int id = -1;

    bool operator==(const Event &other) const
    { return remaining == other.remaining && kind == other.kind && id == other.id; }
  };

  struct EventMaximum
  {
    using S = Event;
    static S op(const S &a, const S &b)
    {
      if (a.remaining != b.remaining) return a.remaining > b.remaining ? a : b;
      return a.kind <= b.kind ? a : b;
    }
    static S e() { return {}; }
  };

  struct Blossom
  {
    int parent = -1, base = -1, incoming = -1, tree = -1;
    Label label = unlabeled;
    Weight dual = 0;
    vc<int> children, cycle;
  };

  struct DenseCandidates
  {
    vc<int> between, best_outer, best_to, active_outer, position;
    vc<unsigned char> retired;
    vvc<int> members;
  };

  struct SparseCandidates
  {
    SegmentTree<EventMaximum> events;
    vc<int> first, previous, next, stack, changed;
    vc<Label> labels;
    vc<ull> updated;
    ull generation = 0;
  };

  int n, m, limit;
  const WeightedMatchingEdge<Cost> *edges;
  Weight maximum, remaining;
  vc<Blossom> nodes;
  vc<int> top, mate_arc, unused;
  vc<Weight> dual;
  CSR<int> outgoing;
  vc<ull> marks;
  ull stamp = 0;
  vc<pair<int, int>> restore;
  conditional_t<sparse, SparseCandidates, DenseCandidates> candidates;

  int from(int arc) const
  {
    const auto &e = edges[arc / 2];
    return (arc & 1) ? e.to : e.from;
  }

  int to(int arc) const { return from(arc ^ 1); }

  int predecessor(int b) const
  {
    const int arc = nodes[b].incoming;
    return arc == -1 ? -1 : top[from(arc)];
  }

  int common_ancestor(int a, int b)
  {
    ++stamp;
    while (true)
    {
      assert(a != -1 || b != -1);
      if (a != -1)
      {
        if (marks[a] == stamp) return a;
        marks[a] = stamp;
        a = predecessor(a);
      }
      swap(a, b);
    }
  }

  int child_of(int b, int v) const
  {
    while (nodes[v].parent != b)
    {
      v = nodes[v].parent;
      assert(v != -1);
    }
    return v;
  }

  bool is_top(int b) const { return nodes[b].base != -1 && nodes[b].parent == -1; }

  void set_event(int slot, const Event &event)
  {
    if (!(candidates.events.get(slot) == event)) candidates.events.set(slot, event);
  }

  Event edge_event(int id) const
  {
    const auto &e = edges[id];
    const int u = e.from, v = e.to;
    const Label a = candidates.labels[u], b = candidates.labels[v];
    if (a == inner || b == inner || (a != outer && b != outer) || top[u] == top[v]) return {};
    const Weight sum = dual[u] + dual[v], twice = 2 * Weight(e.cost);
    if (sum >= twice) return {};
    if (a != b) return {twice - sum, Kind::grow, id};
    const Weight difference = twice - sum;
    assert(difference % 2 == 0);
    const Kind kind = nodes[top[u]].tree == nodes[top[v]].tree ? Kind::shrink : Kind::augment;
    return {difference / 2, kind, id};
  }

  void refresh_edges()
  {
    ++candidates.generation;
    for (int v : candidates.changed) for (int arc : outgoing[v])
    {
      const int id = arc / 2;
      if (candidates.updated[id] == candidates.generation) continue;
      candidates.updated[id] = candidates.generation;
      const auto event = edge_event(id);
      assert(event.remaining <= remaining);
      set_event(id, event);
    }
    candidates.changed.clear();
  }

  void place_sparse_vertices(int b)
  {
    candidates.stack.eb(b);
    const Label label = nodes[b].label;
    while (!candidates.stack.empty())
    {
      const int v = candidates.stack.back();
      candidates.stack.pop_back();
      if (v >= n)
      {
        const auto &children = nodes[v].children;
        candidates.stack.insert(candidates.stack.end(), ALL(children));
        continue;
      }
      top[v] = b;
      const Label previous = candidates.labels[v];
      if (previous == label) continue;
      if (previous == outer) dual[v] += remaining;
      if (previous == inner) dual[v] -= remaining;
      if (label == outer) dual[v] -= remaining;
      if (label == inner) dual[v] += remaining;
      candidates.labels[v] = label;
      candidates.changed.eb(v);
    }
  }

  void change_sparse_label(int b, Label label, int tree)
  {
    auto &node = nodes[b];
    if (node.tree != -1)
    {
      const int previous = candidates.previous[b], next = candidates.next[b];
      if (previous == -1) candidates.first[node.tree] = next;
      else candidates.next[previous] = next;
      if (next != -1) candidates.previous[next] = previous;
    }
    candidates.previous[b] = candidates.next[b] = -1;
    if (tree != -1)
    {
      const int first = candidates.first[tree];
      candidates.next[b] = first;
      if (first != -1) candidates.previous[first] = b;
      candidates.first[tree] = b;
    }
    if (b < n || node.label == label) return;
    const Weight elapsed = maximum - remaining;
    if (node.label == outer) node.dual -= remaining;
    if (node.label == inner) node.dual -= elapsed;
    if (label == outer) node.dual += remaining;
    if (label == inner) node.dual += elapsed;
    if (node.label == inner || label == inner)
    {
      Event event;
      if (label == inner && node.dual < maximum)
        event = {maximum - node.dual, Kind::expand, b};
      set_event(m + (b - n), event);
    }
  }

  void set_label(int b, Label label, int incoming, int tree)
  {
    if constexpr (sparse) change_sparse_label(b, label, tree);
    nodes[b].label = label;
    nodes[b].incoming = incoming;
    nodes[b].tree = tree;
  }

  Weight slack(int arc) const
  {
    const auto &e = edges[arc / 2];
    return dual[e.from] + dual[e.to] - 2 * Weight(e.cost);
  }

  int &between(int b, int c)
  {
    if (b < c) swap(b, c);
    return candidates.between[size_t(b) * (b + 1) / 2 + c];
  }

  void improve(int &best, int arc) const
  {
    if (arc != -1 && (best == -1 || slack(arc) < slack(best))) best = arc;
  }

  void add_dense_outer_edges(int u)
  {
    const int b = top[u];
    for (int arc : outgoing[u])
    {
      const int v = to(arc), c = top[v];
      if (b == c) continue;
      const Weight value = slack(arc);
      if (nodes[c].label == outer)
      {
        int &best = between(b, c);
        if (best != -1 && slack(best) <= value) continue;
        best = arc;
        for (int x : {b, c})
        {
          int &entry = candidates.best_outer[x];
          if (entry == -1 || value < slack(entry)) entry = arc;
        }
      }
      else
      {
        int &entry = candidates.best_to[v];
        if (entry == -1 || value < slack(entry)) entry = arc;
      }
    }
  }

  void insert_dense_outer(int b)
  {
    candidates.position[b] = candidates.active_outer.size();
    candidates.active_outer.eb(b);
  }

  void remove_dense_outer(int b)
  {
    const int last = candidates.active_outer.back(), position = candidates.position[b];
    candidates.active_outer[position] = last;
    candidates.position[last] = position;
    candidates.active_outer.pop_back();
    candidates.position[b] = -1;
  }

  void mark_outer(int b, int incoming, int tree)
  {
    set_label(b, outer, incoming, tree);
    if constexpr (!sparse)
    {
      candidates.best_outer[b] = -1;
      for (int c : candidates.active_outer) between(b, c) = -1;
      insert_dense_outer(b);
      for (int v : candidates.members[b]) add_dense_outer_edges(v);
    }
  }

  void contract_dense(int b)
  {
    const auto &children = nodes[b].children;
    for (int child : children) if (nodes[child].label == outer) remove_dense_outer(child);
    candidates.best_outer[b] = -1;
    for (int c : candidates.active_outer)
    {
      int best = -1;
      for (int child : children) if (nodes[child].label == outer) improve(best, between(child, c));
      between(b, c) = best;
      improve(candidates.best_outer[b], best);
    }
    insert_dense_outer(b);
    for (int child : children)
    {
      const auto &members = candidates.members[child];
      for (int v : members) top[v] = b;
      candidates.members[b].insert(candidates.members[b].end(), ALL(members));
    }
    for (int child : children) if (nodes[child].label == inner)
      for (int v : candidates.members[child]) add_dense_outer_edges(v);
  }

  void contract(int a, int b, int arc)
  {
    const int root = common_ancestor(a, b), id = unused.back();
    unused.pop_back();
    auto &blossom = nodes[id];
    blossom.base = nodes[root].base;
    blossom.dual = 0;
    set_label(id, outer, nodes[root].incoming, nodes[root].tree);
    for (int p = a; p != root; p = predecessor(p))
    {
      blossom.children.eb(p);
      blossom.cycle.eb(nodes[p].incoming);
    }
    blossom.children.eb(root);
    reverse(ALL(blossom.children));
    reverse(ALL(blossom.cycle));
    blossom.cycle.eb(arc);
    for (int p = b; p != root; p = predecessor(p))
    {
      blossom.children.eb(p);
      blossom.cycle.eb(nodes[p].incoming ^ 1);
    }
    assert(blossom.children.size() % 2 == 1);
    for (int child : blossom.children) nodes[child].parent = id;
    if constexpr (sparse)
    {
      for (int child : blossom.children) set_label(child, unlabeled, -1, -1);
      place_sparse_vertices(id);
    }
    else contract_dense(id);
  }

  void expand(int id)
  {
    auto &blossom = nodes[id];
    const auto &children = blossom.children;
    const int size = children.size(), tree = blossom.tree;
    int incoming = blossom.incoming;
    const int entry = child_of(id, to(incoming));
    int i = find(ALL(children), entry) - children.begin();
    assert(i < size);
    set_label(id, unlabeled, -1, -1);
    for (int b : children)
    {
      nodes[b].parent = -1;
      if constexpr (!sparse)
      {
        set_label(b, unlabeled, -1, -1);
        for (int v : candidates.members[b]) top[v] = b;
        candidates.best_outer[b] = -1;
      }
    }
    const int direction = (i % 2 == 0) ? -1 : 1;
    auto next = [&](int j) { return (j + direction + size) % size; };
    auto arc_along = [&](int j) { return direction == 1 ? blossom.cycle[j] : blossom.cycle[next(j)] ^ 1; };
    while (i != 0)
    {
      set_label(children[i], inner, incoming, tree);
      const int matched_arc = arc_along(i);
      i = next(i);
      mark_outer(children[i], matched_arc, tree);
      incoming = arc_along(i);
      i = next(i);
    }
    set_label(children[0], inner, incoming, tree);
    if constexpr (sparse)
    {
      for (int b : children) place_sparse_vertices(b);
      blossom = Blossom{};
    }
    else
    {
      blossom.base = -1;
      blossom.children.clear();
      blossom.cycle.clear();
      candidates.members[id].clear();
    }
    unused.eb(id);
  }

  void grow(int arc)
  {
    const int b = top[to(arc)], matched = mate_arc[nodes[b].base];
    assert(nodes[b].label == unlabeled && matched != -1);
    const int c = top[to(matched)], tree = nodes[top[from(arc)]].tree;
    assert(nodes[c].label == unlabeled);
    set_label(b, inner, arc, tree);
    mark_outer(c, matched, tree);
    if constexpr (sparse)
    {
      place_sparse_vertices(b);
      place_sparse_vertices(c);
    }
  }

  void connect(int arc)
  {
    mate_arc[from(arc)] = arc;
    mate_arc[to(arc)] = arc ^ 1;
  }

  void expose(int b, int v)
  {
    if (nodes[b].base == v) return;
    restore.eb(b, v);
    while (!restore.empty())
    {
      auto [id, base] = restore.back();
      restore.pop_back();
      auto &blossom = nodes[id];
      if (blossom.base == base) continue;
      const int child = child_of(id, base), size = blossom.children.size();
      const int pos = find(ALL(blossom.children), child) - blossom.children.begin();
      assert(pos < size);
      restore.eb(child, base);
      for (int j = 1; j < size; j += 2)
      {
        const int k = (pos + j) % size, arc = blossom.cycle[k];
        restore.eb(blossom.children[k], from(arc));
        restore.eb(blossom.children[(k + 1) % size], to(arc));
        connect(arc);
      }
      rotate(blossom.children.begin(), blossom.children.begin() + pos, blossom.children.end());
      rotate(blossom.cycle.begin(), blossom.cycle.begin() + pos, blossom.cycle.end());
      blossom.base = base;
    }
  }

  void augment(int arc)
  {
    for (int v : {from(arc), to(arc)})
    {
      while (true)
      {
        const int b = top[v], incoming = nodes[b].incoming;
        expose(b, v);
        if (incoming == -1) break;
        const int c = top[from(incoming)], next = nodes[c].incoming;
        expose(c, to(next));
        connect(next);
        v = from(next);
      }
    }
    connect(arc);
  }

  void erase_sparse_tree(int root)
  {
    while (candidates.first[root] != -1)
    {
      const int b = candidates.first[root];
      set_label(b, unlabeled, -1, -1);
      place_sparse_vertices(b);
    }
  }

  void erase_dense_trees(int first, int second)
  {
    fill(ALL(candidates.retired), false);
    for (int b = 0; b < limit; b++)
    {
      const auto &blossom = nodes[b];
      if (!is_top(b) || (blossom.tree != first && blossom.tree != second)) continue;
      if (blossom.label == outer)
      {
        candidates.retired[b] = true;
        remove_dense_outer(b);
      }
      set_label(b, unlabeled, -1, -1);
    }
    for (int b : candidates.active_outer)
    {
      int &best = candidates.best_outer[b];
      if (best == -1) continue;
      const auto &edge = edges[best / 2];
      if (!candidates.retired[top[edge.from]] && !candidates.retired[top[edge.to]]) continue;
      best = -1;
      Weight minimum = numeric_limits<Weight>::max();
      for (int c : candidates.active_outer) if (c != b)
      {
        const int arc = between(b, c);
        if (arc == -1) continue;
        const Weight value = slack(arc);
        if (value >= minimum) continue;
        best = arc;
        minimum = value;
        if (minimum == 0) break;
      }
    }
    for (int v = 0; v < n; v++)
    {
      const int b = top[v];
      if (nodes[b].label == outer) continue;
      int &best = candidates.best_to[v];
      if (!candidates.retired[b] && (best == -1 || !candidates.retired[top[from(best)]])) continue;
      best = -1;
      Weight minimum = numeric_limits<Weight>::max();
      for (int arc : outgoing[v])
      {
        if (nodes[top[to(arc)]].label != outer) continue;
        const Weight value = slack(arc);
        if (value >= minimum) continue;
        best = arc ^ 1;
        minimum = value;
        if (minimum == 0) break;
      }
    }
  }

  Event next_sparse_event()
  {
    while (true)
    {
      auto event = candidates.events.all_prod();
      if (event.kind == Kind::stop || event.kind == Kind::expand) return event;
      const auto &e = edges[event.id];
      if (top[e.from] == top[e.to])
      {
        assert(event.kind == Kind::shrink);
        candidates.events.set(event.id, Event{});
        continue;
      }
      event.id *= 2;
      if (event.kind == Kind::grow && candidates.labels[e.to] == outer) event.id ^= 1;
      return event;
    }
  }

  Event next_dense_event()
  {
    Weight delta = remaining;
    Kind kind = Kind::stop;
    int id = -1;
    auto consider = [&](Weight amount, Kind next, int target)
    {
      if (amount < delta) delta = amount, kind = next, id = target;
    };
    for (int v = 0; v < n && delta != 0; v++)
    {
      const int arc = candidates.best_to[v];
      if (arc != -1 && nodes[top[v]].label == unlabeled) consider(slack(arc), Kind::grow, arc);
    }
    for (int b : candidates.active_outer)
    {
      const int arc = candidates.best_outer[b];
      if (arc == -1) continue;
      const auto &edge = edges[arc / 2];
      const int a = top[edge.from], c = top[edge.to];
      const Weight value = slack(arc);
      assert(a != c && value % 2 == 0);
      const Weight amount = value / 2;
      if (amount > delta || (amount == delta && kind == Kind::stop)) continue;
      const bool joins = nodes[a].tree != nodes[c].tree;
      if (amount < delta || (joins && kind != Kind::augment))
      {
        delta = amount;
        kind = joins ? Kind::augment : Kind::shrink;
        id = arc;
        if (delta == 0 && joins) break;
      }
    }
    for (int b = n; b < limit && delta != 0; b++)
      if (is_top(b) && nodes[b].label == inner) consider(nodes[b].dual, Kind::expand, b);
    return {remaining - delta, kind, id};
  }

  void update_dense_duals(Weight delta)
  {
    if (delta == 0) return;
    for (int v = 0; v < n; v++)
    {
      const Label label = nodes[top[v]].label;
      if (label == outer) dual[v] -= delta;
      if (label == inner) dual[v] += delta;
    }
    for (int b = n; b < limit; b++) if (is_top(b))
    {
      if (nodes[b].label == outer) nodes[b].dual += delta;
      if (nodes[b].label == inner) nodes[b].dual -= delta;
    }
  }

  void search()
  {
    int exposed = n;
    while (true)
    {
      Event event;
      if constexpr (sparse) event = next_sparse_event();
      else event = next_dense_event();
      if (event.kind == Kind::stop) return;
      assert(0 < event.remaining && event.remaining <= remaining);
      if constexpr (!sparse) update_dense_duals(remaining - event.remaining);
      remaining = event.remaining;
      if (event.kind == Kind::grow) grow(event.id);
      else if (event.kind == Kind::expand) expand(event.id);
      else
      {
        const int a = top[from(event.id)], b = top[to(event.id)];
        if (event.kind == Kind::shrink) contract(a, b, event.id);
        else
        {
          const int first = nodes[a].tree, second = nodes[b].tree;
          augment(event.id);
          exposed -= 2;
          if (exposed < 2) return;
          if constexpr (sparse)
          {
            erase_sparse_tree(first);
            erase_sparse_tree(second);
          }
          else erase_dense_trees(first, second);
        }
      }
      if constexpr (sparse) refresh_edges();
    }
  }

  void initialize_sparse()
  {
    candidates.first.resize(n);
    iota(ALL(candidates.first), 0);
    candidates.previous.assign(limit, -1);
    candidates.next.assign(limit, -1);
    candidates.labels.assign(n, outer);
    candidates.updated.resize(m);
    vc<Event> initial(m + (limit - n));
    for (int i = 0; i < m; i++) initial[i] = {Weight(edges[i].cost), Kind::augment, i};
    candidates.events = SegmentTree<EventMaximum>(initial);
  }

  void initialize_dense()
  {
    fill(ALL(dual), maximum);
    candidates.between.assign(size_t(limit) * (limit + 1) / 2, -1);
    candidates.best_outer.assign(limit, -1);
    candidates.best_to.assign(n, -1);
    candidates.active_outer.reserve(n);
    candidates.position.assign(limit, -1);
    candidates.retired.resize(limit);
    candidates.members.resize(limit);
    for (int v = 0; v < n; v++)
    {
      insert_dense_outer(v);
      candidates.members[v].eb(v);
    }
    for (int i = 0; i < m; i++)
    {
      const int u = edges[i].from, v = edges[i].to, arc = 2 * i;
      between(u, v) = arc;
      for (int b : {u, v})
      {
        int &best = candidates.best_outer[b];
        if (best == -1 || edges[best / 2].cost < edges[i].cost) best = arc;
      }
    }
  }

public:
  GeneralWeightedMatching(int n, const WeightedMatchingEdge<Cost> *edges, int m, Weight maximum)
      : n(n), m(m), limit(n + n / 2), edges(edges), maximum(maximum), remaining(maximum),
        nodes(limit), top(n), mate_arc(n, -1), dual(n), marks(limit)
  {
    vc<int> degree(n);
    for (int i = 0; i < m; i++) degree[edges[i].from]++, degree[edges[i].to]++;
    outgoing = CSR<int>(degree);
    fill(ALL(degree), 0);
    for (int i = 0; i < m; i++)
    {
      const int u = edges[i].from, v = edges[i].to;
      outgoing[u][degree[u]++] = 2 * i;
      outgoing[v][degree[v]++] = 2 * i + 1;
    }
    for (int v = 0; v < n; v++)
    {
      top[v] = nodes[v].base = nodes[v].tree = v;
      nodes[v].label = outer;
    }
    for (int b = limit - 1; b >= n; b--) unused.eb(b);
    if constexpr (sparse) initialize_sparse();
    else initialize_dense();
  }

  void solve(vc<int> &result)
  {
    if (m == 0) return;
    search();
    for (int v = 0; v < n; v++)
    {
      const int arc = mate_arc[v];
      if (arc != -1 && v < to(arc)) result.eb(arc / 2);
    }
  }
};
}
