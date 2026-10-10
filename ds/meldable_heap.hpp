#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief 併合可能なヒープの共有ノードプール
 * @docs docs/ds/meldable_heap.md
 */

template <class T, class Compare = less<T>>
struct MeldableHeapPool
{
  class Heap;

private:
  using Root = int;
  static constexpr Root null = -1;
  struct Node
  {
    T value;
    array<Root, 2> child = {null, null};
    int rank = 1;
    template <class... Args>
    explicit Node(in_place_t, Args &&...args) : value(std::forward<Args>(args)...) {}
  };
  Compare comp;
  vc<Node> nodes;

  int rank_of(Root r) const { return r == null ? 0 : nodes[r].rank; }
  void check_root(Root r) const { assert(r == null || (0 <= r && r < SZ<int>(nodes))); }
  template <class... Args>
  Root make_node(Args &&...args)
  {
    Root r = nodes.size();
    nodes.emplace_back(in_place, std::forward<Args>(args)...);
    return r;
  }
  Root merge_impl(Root a, Root b)
  {
    if (a == null) return b;
    if (b == null) return a;
    if (comp(nodes[b].value, nodes[a].value)) swap(a, b);
    nodes[a].child[1] = merge_impl(nodes[a].child[1], b);
    if (rank_of(nodes[a].child[0]) < rank_of(nodes[a].child[1]))
      swap(nodes[a].child[0], nodes[a].child[1]);
    nodes[a].rank = rank_of(nodes[a].child[1]) + 1;
    return a;
  }
  Root clone_impl(Root r)
  {
    if (r == null) return null;
    Root res = nodes.size();
    nodes.push_back(nodes[r]);
    vc<pair<Root, Root>> todo = {{r, res}};
    while (!todo.empty())
    {
      auto [src, dst] = todo.back();
      todo.pop_back();
      auto children = nodes[src].child;
      repi(i, 2) if (children[i] != null)
      {
        Root child = nodes.size();
        nodes.push_back(nodes[children[i]]);
        nodes[dst].child[i] = child;
        todo.eb(children[i], child);
      }
    }
    return res;
  }

public:
  explicit MeldableHeapPool(size_t expected_pushes = 0, Compare comp = Compare{}) : comp(std::move(comp))
  { nodes.reserve(expected_pushes); }
  size_t node_count() const { return nodes.size(); }
  void reserve(size_t count) { nodes.reserve(count); }
  void clear() { nodes.clear(); }

  class Heap
  {
    friend struct MeldableHeapPool;
    MeldableHeapPool *pool;
    Root root;
    int count;
    Heap(MeldableHeapPool *pool, Root root, int count) : pool(pool), root(root), count(count) {}

  public:
    using value_type = T;
    using compare_type = Compare;
    Heap(const Heap &) = delete;
    Heap &operator=(const Heap &) = delete;
    Heap(Heap &&other) noexcept
    : pool(other.pool), root(exchange(other.root, null)), count(exchange(other.count, 0)) {}
    Heap &operator=(Heap &&other) noexcept
    {
      if (this != &other)
      {
        pool = other.pool;
        root = exchange(other.root, null);
        count = exchange(other.count, 0);
      }
      return *this;
    }
    int size() const { return count; }
    bool empty() const { return count == 0; }
    const T &top() const
    {
      assert(!empty());
      pool->check_root(root);
      return pool->nodes[root].value;
    }
    template <class... Args>
    void emplace(Args &&...args)
    {
      pool->check_root(root);
      Root r = pool->make_node(std::forward<Args>(args)...);
      root = pool->merge_impl(root, r);
      count++;
    }
    void push(const T &value) { emplace(value); }
    void push(T &&value) { emplace(std::move(value)); }
    void pop()
    {
      assert(!empty());
      pool->check_root(root);
      auto children = pool->nodes[root].child;
      root = pool->merge_impl(children[0], children[1]);
      count--;
    }
    void merge(Heap &other)
    {
      assert(pool == other.pool);
      pool->check_root(root), pool->check_root(other.root);
      if (this == &other) return;
      root = pool->merge_impl(root, other.root);
      count += exchange(other.count, 0);
      other.root = null;
    }
    Heap clone() const
    {
      pool->check_root(root);
      return Heap(pool, pool->clone_impl(root), count);
    }
    // 取り出す順に値をコピーする。ヒープとプールは変更しない。
    vc<T> content() const
    {
      pool->check_root(root);
      vc<Root> order;
      order.reserve(count);
      if (root != null) order.eb(root);
      repi(i, count) for (Root child : pool->nodes[order[i]].child)
        if (child != null) order.eb(child);
      sort(ALL(order), [&](Root a, Root b) { return pool->comp(pool->nodes[a].value, pool->nodes[b].value); });
      vc<T> res;
      res.reserve(count);
      for (Root r : order) res.eb(pool->nodes[r].value);
      return res;
    }
    void clear() { pool->check_root(root); root = null; count = 0; }
    void swap(Heap &other) noexcept
    {
      std::swap(pool, other.pool), std::swap(root, other.root), std::swap(count, other.count);
    }
    friend void swap(Heap &a, Heap &b) noexcept { a.swap(b); }
  };

  Heap make_heap() { return Heap(this, null, 0); }
  Heap make_heap(const T &value) { return Heap(this, make_node(value), 1); }
  Heap make_heap(T &&value) { return Heap(this, make_node(std::move(value)), 1); }
};

template <class T, class Compare = less<T>>
using MeldableHeap = typename MeldableHeapPool<T, Compare>::Heap;
