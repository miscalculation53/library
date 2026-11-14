#pragma once

#include "../algebra/algebra_base.hpp"
#include "../algebra/trivial.hpp"
#include "../utils/integer_arithmetic.hpp"

/**
 * @brief 動的な森の頂点・辺のパス積とパス更新
 * @docs docs/tree/link_cut_tree.md
 */

namespace link_cut_tree_detail
{
template <class A, bool Lazy>
struct ActionType { using F = monostate; };
template <class A>
struct ActionType<A, true> { using F = typename A::F; };

template <class A, bool Edge, bool Lazy>
class Forest
{
public:
  using S = typename A::S;
  using F = typename ActionType<A, Lazy>::F;

private:
  using ProductMonoid = NormalAndOppositeMonoid<A>;
  using Product = typename ProductMonoid::S;
  using Action = conditional_t<Edge, pair<F, F>, F>;
  using PathElement = conditional_t<Edge, pair<int, int>, int>;
  static constexpr int none = -1;

  struct Node
  {
    array<int, 2> child{none, none};
    int parent = none;
    int size = 1;
    conditional_t<Edge, pair<int, int>, monostate> ends{};
    bool reversed = false;
    conditional_t<Lazy, optional<Action>, monostate> pending;
    conditional_t<Edge, Product, S> value;
    Product product;

    explicit Node(const S &x) : value(x), product(x, x)
    {
      if constexpr (Edge) ends = {none, none};
    }
    Node(int u, int v, const S &forward, const S &backward)
        : ends(u, v), value(forward, backward), product(value) {}

    local(auto content() const
    {
      if constexpr (Edge)
        return tie(child, parent, size, reversed, ends, value.normal, value.opposite,
                   product.normal, product.opposite, pending);
      else
        return tie(child, parent, size, reversed, value, product.normal, product.opposite, pending);
    })
  };

  int n_ = 0;
  int free_edge_ = none;
  vc<Node> nodes_;
  vc<int> ancestors_;

  void check_vertex(int v) const { assert(0 <= v && v < n_); (void)v; }
  int path_size(int v) const { return v == none ? 0 : nodes_[v].size; }
  bool carries_value(const Node &x) const
  {
    if constexpr (Edge) return x.ends.first != none;
    else return true;
  }
  bool has_values(int v) const
  {
    if constexpr (Edge) return v != none && (nodes_[v].size > 1 || carries_value(nodes_[v]));
    else return v != none;
  }
  const S &forward_value(const Node &x) const
  {
    if constexpr (Edge) return x.value.normal;
    else return x.value;
  }
  bool is_aux_root(int v) const
  {
    const int p = nodes_[v].parent;
    return p == none || (nodes_[p].child[0] != v && nodes_[p].child[1] != v);
  }

  void pull(int v)
  {
    Node &x = nodes_[v];
    const auto [l, r] = x.child;
    x.size = path_size(l) + 1 + path_size(r);
    if constexpr (Edge)
    {
      if (!carries_value(x))
      {
        if (l == none) x.product = r == none ? x.value : nodes_[r].product;
        else x.product = r == none ? nodes_[l].product : ProductMonoid::op(nodes_[l].product, nodes_[r].product);
        return;
      }
      x.product = x.value;
    }
    else x.product = Product(x.value, x.value);
    if (l != none) x.product = ProductMonoid::op(nodes_[l].product, x.product);
    if (r != none) x.product = ProductMonoid::op(x.product, nodes_[r].product);
  }

  void reverse(int v)
  {
    if (v == none) return;
    Node &x = nodes_[v];
    swap(x.child[0], x.child[1]);
    swap(x.product.normal, x.product.opposite);
    x.reversed = !x.reversed;
    if constexpr (Edge)
    {
      swap(x.ends.first, x.ends.second);
      swap(x.value.normal, x.value.opposite);
      if constexpr (Lazy)
        if (x.pending) swap(x.pending->first, x.pending->second);
    }
  }

  void apply_node(int v, const Action &f)
  {
    if (!has_values(v)) return;
    Node &x = nodes_[v];
    if constexpr (Edge)
    {
      if (carries_value(x))
      {
        x.value.normal = A::mapping(f.first, x.value.normal);
        x.value.opposite = A::mapping(f.second, x.value.opposite);
      }
      x.product.normal = A::mapping(f.first, x.product.normal);
      x.product.opposite = A::mapping(f.second, x.product.opposite);
      if (x.pending)
        x.pending = Action{A::composition(f.first, x.pending->first),
                           A::composition(f.second, x.pending->second)};
      else x.pending = f;
    }
    else
    {
      x.value = A::mapping(f, x.value);
      x.product.normal = A::mapping(f, x.product.normal);
      x.product.opposite = A::mapping(f, x.product.opposite);
      if (x.pending) x.pending = A::composition(f, *x.pending);
      else x.pending = f;
    }
  }

  void push(int v)
  {
    Node &x = nodes_[v];
    if (x.reversed)
    {
      reverse(x.child[0]);
      reverse(x.child[1]);
      x.reversed = false;
    }
    if constexpr (Lazy)
      if (x.pending)
      {
        apply_node(x.child[0], *x.pending);
        apply_node(x.child[1], *x.pending);
        x.pending.reset();
      }
  }

  void rotate(int v)
  {
    const int p = nodes_[v].parent, g = nodes_[p].parent;
    const int side = nodes_[p].child[1] == v;
    if (!is_aux_root(p)) nodes_[g].child[nodes_[g].child[1] == p] = v;
    nodes_[v].parent = g;
    const int middle = nodes_[v].child[side ^ 1];
    nodes_[p].child[side] = middle;
    if (middle != none) nodes_[middle].parent = p;
    nodes_[v].child[side ^ 1] = p;
    nodes_[p].parent = v;
    pull(p);
    pull(v);
  }

  void splay(int v)
  {
    ancestors_.clear();
    for (int x = v; !is_aux_root(x); x = nodes_[x].parent)
      ancestors_.push_back(nodes_[x].parent);
    for (auto it = ancestors_.rbegin(); it != ancestors_.rend(); ++it) push(*it);
    push(v);
    while (!is_aux_root(v))
    {
      int p = nodes_[v].parent;
      if (!is_aux_root(p))
      {
        int g = nodes_[p].parent;
        bool aligned = (nodes_[p].child[1] == v) == (nodes_[g].child[1] == p);
        rotate(aligned ? p : v);
      }
      rotate(v);
    }
  }

  int access(int v)
  {
    int last = none;
    for (int x = v; x != none; x = nodes_[x].parent)
    {
      splay(x);
      nodes_[x].child[1] = last;
      pull(x);
      last = x;
    }
    splay(v);
    return last;
  }

  int access_both(int u, int v)
  {
    access(u);
    if (u == v) return u;
    const int ancestor = access(v);
    return nodes_[u].parent == none ? none : ancestor;
  }

  int extreme(int v, int side)
  {
    push(v);
    while (nodes_[v].child[side] != none)
    {
      v = nodes_[v].child[side];
      push(v);
    }
    splay(v);
    return v;
  }

  int root_node(int v)
  {
    access(v);
    return extreme(v, 0);
  }

  void make_root(int v)
  {
    access(v);
    reverse(v);
  }

  int parent_node(int v)
  {
    access(v);
    const int l = nodes_[v].child[0];
    return l == none ? none : extreme(l, 1);
  }

  int parent_vertex(int p) const
  {
    if constexpr (Edge) return p == none ? none : nodes_[p].ends.first;
    else return p;
  }

  void detach_parent(int v)
  {
    access(v);
    const int l = nodes_[v].child[0];
    nodes_[l].parent = none;
    nodes_[v].child[0] = none;
    pull(v);
  }

  void detach(int v, int p)
  {
    detach_parent(v);
    if constexpr (Edge)
    {
      detach_parent(p);
      nodes_[p] = Node(A::e());
      nodes_[p].parent = free_edge_;
      free_edge_ = p;
    }
    else (void)p;
  }

  template <class Query>
  auto on_path(int u, int v, const Query &query) -> decltype(query(v))
  {
    check_vertex(u);
    check_vertex(v);
    const int old_root = root_node(u);
    struct RestoreRoot
    {
      Forest &forest;
      int root;
      ~RestoreRoot() noexcept(false)
      {
        if (root != none) forest.make_root(root);
      }
      CPP_DUMP_DEFINE_DATA(root)
    } restore{*this, old_root != u ? old_root : none};
    if (old_root != u) reverse(old_root);
    access(v);
    if (old_root == v || nodes_[old_root].parent != none) return query(v);
    return {};
  }

  int select(int v, int k)
  {
    for (;;)
    {
      push(v);
      const int l = nodes_[v].child[0], left_size = path_size(l);
      if (k < left_size) v = l;
      else if (k == left_size)
      {
        splay(v);
        return v;
      }
      else
      {
        k -= left_size + 1;
        v = nodes_[v].child[1];
      }
    }
  }

  template <class Pred>
  optional<PathElement> first_prefix(int v, Pred &pred)
  {
    if (!has_values(v) || !pred(std::as_const(nodes_[v].product.normal))) return nullopt;
    S prefix = A::e();
    for (;;)
    {
      push(v);
      const Node &x = nodes_[v];
      const auto [l, r] = x.child;
      if (has_values(l))
      {
        S joined = A::op(prefix, nodes_[l].product.normal);
        if (pred(std::as_const(joined)))
        {
          v = l;
          continue;
        }
        prefix = std::move(joined);
      }
      if (carries_value(x))
      {
        prefix = A::op(prefix, forward_value(x));
        if (pred(std::as_const(prefix)))
        {
          splay(v);
          if constexpr (Edge) return nodes_[v].ends;
          else return v;
        }
      }
      assert(r != none);
      v = r;
    }
  }

protected:
  Forest() = default;
  Forest(const Forest &) = default;
  Forest &operator=(const Forest &) = default;
  Forest(Forest &&other) noexcept { *this = std::move(other); }
  Forest &operator=(Forest &&other) noexcept
  {
    if (this != &other)
    {
      n_ = std::exchange(other.n_, 0);
      free_edge_ = std::exchange(other.free_edge_, none);
      nodes_ = std::move(other.nodes_);
      ancestors_ = std::move(other.ancestors_);
    }
    return *this;
  }

  explicit Forest(int n) : n_(n)
  {
    assert(n >= 0);
    nodes_.reserve(Edge && n > 0 ? 2 * size_t(n) - 1 : size_t(n));
    const S identity = A::e();
    for (int v = 0; v < n; ++v) nodes_.emplace_back(identity);
  }

  template <class It>
  Forest(It first, It last) : n_(last - first)
  {
    nodes_.reserve(n_);
    for (; first != last; ++first) nodes_.emplace_back(S(*first));
  }

  bool can_link(int u, int v)
  {
    check_vertex(u);
    check_vertex(v);
    return u != v && access_both(v, u) == none;
  }

  bool link_vertices(int u, int v)
  {
    if (!can_link(u, v)) return false;
    reverse(u);
    nodes_[u].parent = v;
    return true;
  }

  bool link_edge(int u, int v, const S &forward, const S &backward)
  {
    if (!can_link(u, v)) return false;
    int e = free_edge_;
    if (e == none)
    {
      e = nodes_.size();
      nodes_.emplace_back(v, u, backward, forward);
    }
    else
    {
      const int next = nodes_[e].parent;
      auto &x = nodes_[e];
      x.value = Product(backward, forward);
      x.product = x.value;
      x.ends = {v, u};
      free_edge_ = next;
    }
    reverse(u);
    nodes_[u].parent = e;
    nodes_[e].parent = v;
    return true;
  }

  pair<int, int> find_edge(int u, int v)
  {
    check_vertex(u);
    check_vertex(v);
    if (u == v) return {none, none};
    int p = parent_node(u);
    if (parent_vertex(p) == v) return {u, p};
    p = parent_node(v);
    if (parent_vertex(p) == u) return {v, p};
    return {none, none};
  }

  S get_vertex(int v)
  {
    check_vertex(v);
    access(v);
    return nodes_[v].value;
  }

  void set_vertex(int v, const S &value)
  {
    check_vertex(v);
    access(v);
    nodes_[v].value = value;
    pull(v);
  }

  optional<S> get_edge(int u, int v)
  {
    const int e = find_edge(u, v).second;
    if (e == none) return nullopt;
    const auto &x = nodes_[e];
    return x.ends.first == u ? x.value.normal : x.value.opposite;
  }

  bool set_edge(int u, int v, const S &forward, const S *backward)
  {
    const int e = find_edge(u, v).second;
    if (e == none) return false;
    auto &x = nodes_[e];
    const bool aligned = x.ends.first == u;
    (aligned ? x.value.normal : x.value.opposite) = forward;
    if (backward) (aligned ? x.value.opposite : x.value.normal) = *backward;
    pull(e);
    return true;
  }

  template <class Pred>
  optional<PathElement> first(int u, int v, Pred &pred)
  {
    return on_path(u, v, [&](int x) { return first_prefix(x, pred); });
  }

  bool apply_path(int u, int v, const Action &f)
  {
    return on_path(u, v, [&](int x)
    {
      apply_node(x, f);
      return true;
    });
  }

public:
  local(auto content() const { return tie(n_, free_edge_, nodes_); })

  // 森の頂点数を返す。
  int size() const { return n_; }

  // u と v が同じ連結成分にあるかを返す。
  bool same(int u, int v)
  {
    check_vertex(u);
    check_vertex(v);
    return u == v || access_both(u, v) != none;
  }

  // v の成分の根を返す。
  int root(int v)
  {
    check_vertex(v);
    return root_node(v);
  }

  // v を成分の根にする。
  void reroot(int v)
  {
    check_vertex(v);
    make_root(v);
  }

  // v の親を返す。v が根なら nullopt。
  optional<int> parent(int v)
  {
    check_vertex(v);
    const int p = parent_vertex(parent_node(v));
    if (p == none) return nullopt;
    return p;
  }

  // 現在の根に関する最小共通祖先を返す。別成分なら nullopt。
  optional<int> lca(int u, int v)
  {
    check_vertex(u);
    check_vertex(v);
    const int p = access_both(u, v);
    if (p == none) return nullopt;
    return p;
  }

  // 辺 (u, v) を削除し、削除したかを返す。
  bool cut(int u, int v)
  {
    const auto [child, p] = find_edge(u, v);
    if (child == none) return false;
    detach(child, p);
    return true;
  }

  // v とその親を結ぶ辺を削除し、削除したかを返す。
  bool cut_parent(int v)
  {
    check_vertex(v);
    const int p = parent_node(v);
    if (p == none) return false;
    detach(v, p);
    return true;
  }

  // u から v へのパスの順序で積を返す。別成分なら nullopt。
  optional<S> prod(int u, int v)
  {
    return on_path(u, v, [&](int x) -> optional<S> { return nodes_[x].product.normal; });
  }

  // u から v までの辺数を返す。別成分なら nullopt。
  optional<int> dist(int u, int v)
  {
    return on_path(u, v, [&](int x) -> optional<int>
    {
      return (nodes_[x].size - 1) / (Edge ? 2 : 1);
    });
  }

  // u から v へ k 辺進んだ頂点を返す。該当する頂点がなければ nullopt。
  template <class I, enable_if_t<is_integral_ext<I>, int> = 0>
  optional<int> jump(int u, int v, I k)
  {
    check_vertex(u);
    check_vertex(v);
    if (is_negative(k) || u128(k) >= u128(n_)) return nullopt;
    return on_path(u, v, [&](int x) -> optional<int>
    {
      if (int(k) > (nodes_[x].size - 1) / (Edge ? 2 : 1)) return nullopt;
      return select(x, int(k) * (Edge ? 2 : 1));
    });
  }

  // パス上の全要素に f を作用させる。別成分なら false。
  template <bool Enabled = Lazy, enable_if_t<Enabled && Lazy, int> = 0>
  bool apply(int u, int v, const F &f)
  {
    if constexpr (Edge) return apply_path(u, v, {f, f});
    else return apply_path(u, v, f);
  }
};

template <class A, bool Lazy>
struct VertexTree : Forest<A, false, Lazy>
{
  using Base = Forest<A, false, Lazy>;
  using typename Base::S;

  VertexTree() = default;
  explicit VertexTree(int n) : Base(n) {}
  VertexTree(initializer_list<S> values) : Base(values.begin(), values.end()) {}
  template <class T>
  explicit VertexTree(const vc<T> &values) : Base(values.begin(), values.end()) {}

  // u と v の成分をつなぎ、追加したかを返す。連結後は u が v の子になる。
  bool link(int u, int v) { return this->link_vertices(u, v); }
  // 頂点 v の値を返す。
  S get(int v) { return this->get_vertex(v); }
  // 頂点 v の値を x に変更する。
  void set(int v, const S &x) { this->set_vertex(v, x); }

  // パス上の接頭辞積が初めて pred を満たす頂点を返す。
  template <class Pred>
  optional<int> first_vertex(int u, int v, Pred &&pred)
  {
    return this->first(u, v, pred);
  }
};

template <class A, bool Lazy>
struct EdgeTree : Forest<A, true, Lazy>
{
  using Base = Forest<A, true, Lazy>;
  using typename Base::S;
  using typename Base::F;
  using Base::apply;

  EdgeTree() = default;
  explicit EdgeTree(int n) : Base(n) {}

  // 両方向の値を x として辺を追加する。
  bool link(int u, int v, const S &x) { return link(u, v, x, x); }
  // u→v の値を x、v→u の値を y として辺を追加する。
  bool link(int u, int v, const S &x, const S &y) { return this->link_edge(u, v, x, y); }
  // u→v の辺値を返す。辺がなければ nullopt。
  optional<S> get(int u, int v) { return this->get_edge(u, v); }
  // u→v の辺値を x に変更する。辺がなければ false。
  bool set(int u, int v, const S &x) { return this->set_edge(u, v, x, nullptr); }
  // u→v の値を x、v→u の値を y に変更する。辺がなければ false。
  bool set(int u, int v, const S &x, const S &y) { return this->set_edge(u, v, x, &y); }

  // パス上の接頭辞積が初めて pred を満たす辺を通過順の端点で返す。
  template <class Pred>
  optional<pair<int, int>> first_edge(int u, int v, Pred &&pred)
  {
    return this->first(u, v, pred);
  }

  // u→v の各辺の値に f、逆向きの値に g を作用させる。別成分なら false。
  template <bool Enabled = Lazy, enable_if_t<Enabled && Lazy, int> = 0>
  bool apply(int u, int v, const F &f, const F &g)
  {
    return this->apply_path(u, v, {f, g});
  }
};
}

template <class M = GroupTrivial>
using LinkCutTree = link_cut_tree_detail::VertexTree<M, false>;
template <class M>
using EdgeLinkCutTree = link_cut_tree_detail::EdgeTree<M, false>;
template <class AM>
using LazyLinkCutTree = link_cut_tree_detail::VertexTree<AM, true>;
template <class AM>
using LazyEdgeLinkCutTree = link_cut_tree_detail::EdgeTree<AM, true>;
