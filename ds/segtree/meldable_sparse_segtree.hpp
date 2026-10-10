#pragma once

#include "../../template/template_all_but_modint.hpp"

/**
 * @brief 併合可能な疎なセグメント木の共有ノードプール
 * @docs docs/ds/segtree/meldable_sparse_segtree.md
 */

template <class M>
struct MeldableSparseSegmentTreePool
{
  using S = typename M::S;
  class Tree;

private:
  using Root = int;
  static constexpr Root null = -1;
  struct Node
  {
    S prod;
    array<Root, 2> child;
    Node(const S &prod, Root left = null, Root right = null) : prod(prod), child{left, right} {}
  };
  ll n;
  vc<Node> nodes;

  void check_root(Root root) const
  {
    assert(root == null || (0 <= root && root < SZ<int>(nodes)));
  }
  S node_prod(Root root) const { return root == null ? M::e() : nodes[root].prod; }
  Root make_node(const S &prod, Root left = null, Root right = null)
  {
    Root root = nodes.size();
    nodes.eb(prod, left, right);
    return root;
  }
  void pull(Root root)
  {
    nodes[root].prod = M::op(node_prod(nodes[root].child[0]), node_prod(nodes[root].child[1]));
  }

  template <class F>
  Root modify_impl(Root root, ll p, const F &f, ll a, ll b)
  {
    if (b - a == 1)
    {
      if (root == null)
      {
        S value = M::e();
        f(value);
        return make_node(value);
      }
      f(nodes[root].prod);
      return root;
    }
    ll c = a + (b - a) / 2;
    int d = p < c ? 0 : 1;
    Root child = root == null ? null : nodes[root].child[d];
    child = modify_impl(child, p, f, d == 0 ? a : c, d == 0 ? c : b);
    if (root == null)
    {
      Root left = d == 0 ? child : null, right = d == 1 ? child : null;
      return make_node(M::op(node_prod(left), node_prod(right)), left, right);
    }
    nodes[root].child[d] = child;
    pull(root);
    return root;
  }

  Root erase_impl(Root root, ll p, ll a, ll b, bool &erased)
  {
    if (root == null) return null;
    if (b - a == 1)
    {
      erased = true;
      return null;
    }
    ll c = a + (b - a) / 2;
    int d = p < c ? 0 : 1;
    Root child = erase_impl(nodes[root].child[d], p, d == 0 ? a : c, d == 0 ? c : b, erased);
    if (!erased) return root;
    nodes[root].child[d] = child;
    if (nodes[root].child[0] == null && nodes[root].child[1] == null) return null;
    pull(root);
    return root;
  }

  template <class F>
  Root merge_impl(Root x, Root y, ll a, ll b, const F &f)
  {
    if (x == null) return y;
    if (y == null) return x;
    if (b - a == 1)
      nodes[x].prod = f(nodes[x].prod, nodes[y].prod);
    else
    {
      ll c = a + (b - a) / 2;
      nodes[x].child[0] = merge_impl(nodes[x].child[0], nodes[y].child[0], a, c, f);
      nodes[x].child[1] = merge_impl(nodes[x].child[1], nodes[y].child[1], c, b, f);
      pull(x);
    }
    return x;
  }

  Root clone_impl(Root root)
  {
    if (root == null) return null;
    Node node = nodes[root];
    Root res = make_node(node.prod);
    Root left = clone_impl(node.child[0]), right = clone_impl(node.child[1]);
    nodes[res].child = {left, right};
    return res;
  }

  S prod_impl(Root root, ll l, ll r, ll a, ll b) const
  {
    if (root == null || b <= l || r <= a) return M::e();
    if (l <= a && b <= r) return nodes[root].prod;
    ll c = a + (b - a) / 2;
    return M::op(prod_impl(nodes[root].child[0], l, r, a, c),
                 prod_impl(nodes[root].child[1], l, r, c, b));
  }

  template <class F>
  void enumerate_impl(Root root, ll l, ll r, ll a, ll b, const F &f) const
  {
    if (root == null || b <= l || r <= a) return;
    if (b - a == 1)
    {
      f(a, nodes[root].prod);
      return;
    }
    ll c = a + (b - a) / 2;
    enumerate_impl(nodes[root].child[0], l, r, a, c, f);
    enumerate_impl(nodes[root].child[1], l, r, c, b, f);
  }

public:
  explicit MeldableSparseSegmentTreePool(ll n = 0) : n(n) { assert(n >= 0); }

  ll universe_size() const { return n; }
  size_t node_count() const { return nodes.size(); }
  void reserve(size_t count) { nodes.reserve(count); }

private:
  bool empty(Root root) const { check_root(root); return root == null; }

  // 未登録なら M::e() から始め、f をちょうど1回適用する。
  template <class F>
  void modify(Root &root, ll p, const F &f)
  {
    check_root(root);
    assert(0 <= p && p < n);
    root = modify_impl(root, p, f, 0, n);
  }
  void set(Root &root, ll p, const S &value)
  {
    modify(root, p, [&](S &current) { current = value; });
  }

  bool contains(Root root, ll p) const
  {
    check_root(root);
    assert(0 <= p && p < n);
    ll a = 0, b = n;
    while (root != null && b - a > 1)
    {
      ll c = a + (b - a) / 2;
      if (p < c) root = nodes[root].child[0], b = c;
      else root = nodes[root].child[1], a = c;
    }
    return root != null;
  }

  S get(Root root, ll p) const
  {
    check_root(root);
    assert(0 <= p && p < n);
    ll a = 0, b = n;
    while (root != null && b - a > 1)
    {
      ll c = a + (b - a) / 2;
      if (p < c) root = nodes[root].child[0], b = c;
      else root = nodes[root].child[1], a = c;
    }
    return node_prod(root);
  }

  bool erase(Root &root, ll p)
  {
    check_root(root);
    assert(0 <= p && p < n);
    bool erased = false;
    root = erase_impl(root, p, 0, n, erased);
    return erased;
  }

  // a を併合結果の根に更新し、b を空にする。
  template <class F>
  void merge(Root &a, Root &b, const F &f)
  {
    check_root(a), check_root(b);
    if (&a == &b) return;
    assert(a == null || b == null || a != b);
    a = merge_impl(a, b, 0, n, f);
    b = null;
  }
  void merge(Root &a, Root &b)
  {
    merge(a, b, [](const S &x, const S &y) { return M::op(x, y); });
  }

  Root clone(Root root) { check_root(root); return clone_impl(root); }

  S prod(Root root, ll l, ll r) const
  {
    check_root(root);
    assert(0 <= l && l <= r && r <= n);
    return l == r ? M::e() : prod_impl(root, l, r, 0, n);
  }
  S all_prod(Root root) const { check_root(root); return node_prod(root); }

  // g(prod(root, l, r)) が true となる最大の r を返す。
  template <class G>
  ll max_right_ok(Root root, ll l, const G &g) const
  {
    check_root(root);
    assert(0 <= l && l <= n);
    assert(g(M::e()));
    S sm = M::e();
    auto dfs = [&](auto &&self, Root i, ll a, ll b) -> ll
    {
      if (i == null || b <= l) return -1;
      if (l <= a)
      {
        S value = M::op(sm, nodes[i].prod);
        if (g(value))
        {
          sm = std::move(value);
          return -1;
        }
        if (b - a == 1) return a;
      }
      ll c = a + (b - a) / 2;
      ll res = self(self, nodes[i].child[0], a, c);
      return res == -1 ? self(self, nodes[i].child[1], c, b) : res;
    };
    ll res = dfs(dfs, root, 0, n);
    return res == -1 ? n : res;
  }

  // g(prod(root, l, r)) が true となる最小の l を返す。
  template <class G>
  ll min_left_ok(Root root, ll r, const G &g) const
  {
    check_root(root);
    assert(0 <= r && r <= n);
    assert(g(M::e()));
    S sm = M::e();
    auto dfs = [&](auto &&self, Root i, ll a, ll b) -> ll
    {
      if (i == null || r <= a) return -1;
      if (b <= r)
      {
        S value = M::op(nodes[i].prod, sm);
        if (g(value))
        {
          sm = std::move(value);
          return -1;
        }
        if (b - a == 1) return b;
      }
      ll c = a + (b - a) / 2;
      ll res = self(self, nodes[i].child[1], c, b);
      return res == -1 ? self(self, nodes[i].child[0], a, c) : res;
    };
    ll res = dfs(dfs, root, 0, n);
    return res == -1 ? 0 : res;
  }

  template <class F>
  void enumerate(Root root, ll l, ll r, const F &f) const
  {
    check_root(root);
    assert(0 <= l && l <= r && r <= n);
    if (l < r) enumerate_impl(root, l, r, 0, n, f);
  }
  map<ll, S> content(Root root) const
  {
    map<ll, S> res;
    enumerate(root, 0, n, [&](ll p, const S &value) { res.emplace_hint(res.end(), p, value); });
    return res;
  }

  // 根だけを空にする。ノード領域はプールに残す。
  void clear(Root &root) { check_root(root); root = null; }

public:
  // プールへの参照と根を持つ木。複製には clone を使う。
  class Tree
  {
    friend struct MeldableSparseSegmentTreePool;
    MeldableSparseSegmentTreePool *pool;
    Root root;
    Tree(MeldableSparseSegmentTreePool *pool, Root root) : pool(pool), root(root) {}

  public:
    using S = typename M::S;

    Tree(const Tree &) = delete;
    Tree &operator=(const Tree &) = delete;
    Tree(Tree &&other) noexcept : pool(other.pool), root(exchange(other.root, null)) {}
    Tree &operator=(Tree &&other) noexcept
    {
      if (this != &other)
      {
        pool = other.pool;
        root = exchange(other.root, null);
      }
      return *this;
    }

    ll universe_size() const { return pool->universe_size(); }
    bool empty() const { return pool->empty(root); }
    bool contains(ll p) const { return pool->contains(root, p); }
    S get(ll p) const { return pool->get(root, p); }
    void set(ll p, const S &value) { pool->set(root, p, value); }
    template <class F>
    void modify(ll p, const F &f) { pool->modify(root, p, f); }
    bool erase(ll p) { return pool->erase(root, p); }

    void merge(Tree &other)
    {
      assert(pool == other.pool);
      pool->merge(root, other.root);
    }
    template <class F>
    void merge(Tree &other, const F &f)
    {
      assert(pool == other.pool);
      pool->merge(root, other.root, f);
    }
    Tree clone() const { return Tree(pool, pool->clone(root)); }

    S prod(ll l, ll r) const { return pool->prod(root, l, r); }
    S all_prod() const { return pool->all_prod(root); }
    template <class G>
    ll max_right_ok(ll l, const G &g) const { return pool->max_right_ok(root, l, g); }
    template <class G>
    ll min_left_ok(ll r, const G &g) const { return pool->min_left_ok(root, r, g); }
    template <class F>
    void enumerate(ll l, ll r, const F &f) const { pool->enumerate(root, l, r, f); }
    map<ll, S> content() const { return pool->content(root); }
    void clear() { pool->clear(root); }

    void swap(Tree &other) noexcept
    {
      std::swap(pool, other.pool), std::swap(root, other.root);
    }
    friend void swap(Tree &a, Tree &b) noexcept { a.swap(b); }
  };

  Tree make_tree() { return Tree(this, null); }
  Tree make_tree(ll p, const S &value)
  {
    auto tree = make_tree();
    tree.set(p, value);
    return tree;
  }

  // 全ての木を無効にし、確保済みの容量を再利用する。
  void clear() { nodes.clear(); }
};

template <class M>
using MeldableSparseSegmentTree = typename MeldableSparseSegmentTreePool<M>::Tree;
