#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief 添字付き Fibonacci heap
 * @docs docs/ds/fibonacci_heap.md
 */

// less<Key> で最小ヒープ。添字は [0,n)、同じ添字は一度に一つまで保持する。
template <class Key, class Compare = less<Key>>
class FibonacciHeap
{
  struct Node
  {
    Key key{};
    int parent = -1, child = -1, prev = -1, next = -1, degree = 0;
    bool marked = false, present = false;
  };
  vc<Node> nodes;
  vc<int> roots, degrees;
  Compare comp;
  int minimum = -1, count = 0;

  void detach(int v)
  {
    nodes[nodes[v].prev].next = nodes[v].next;
    nodes[nodes[v].next].prev = nodes[v].prev;
    nodes[v].prev = nodes[v].next = v;
  }
  void insert_after(int u, int v)
  {
    const int next = nodes[u].next;
    nodes[v].prev = u, nodes[v].next = next;
    nodes[u].next = v, nodes[next].prev = v;
  }
  void add_root(int v)
  {
    nodes[v].parent = -1, nodes[v].marked = false;
    if (minimum == -1) minimum = v, nodes[v].prev = nodes[v].next = v;
    else
    {
      insert_after(minimum, v);
      if (comp(nodes[v].key, nodes[minimum].key)) minimum = v;
    }
  }
  void cut(int v, int p)
  {
    if (nodes[p].child == v)
      nodes[p].child = nodes[v].next == v ? -1 : nodes[v].next;
    nodes[p].degree--;
    detach(v);
    add_root(v);
  }

public:
  explicit FibonacciHeap(int n = 0, Compare comp = Compare{}) : nodes(n), comp(std::move(comp)) {}
  int size() const { return count; }
  bool empty() const { return count == 0; }
  bool contains(int id) const
  {
    assert(0 <= id && id < int(nodes.size()));
    return nodes[id].present;
  }
  const Key &key(int id) const { assert(contains(id)); return nodes[id].key; }
  pair<Key, int> top() const
  {
    assert(!empty());
    return {nodes[minimum].key, minimum};
  }
  void push(int id, const Key &key)
  {
    assert(!contains(id));
    auto &v = nodes[id];
    v.key = key;
    v.parent = v.child = -1, v.prev = v.next = id, v.degree = 0;
    v.marked = false, v.present = true;
    add_root(id);
    count++;
  }
  void decrease_key(int id, const Key &key)
  {
    assert(contains(id) && !comp(nodes[id].key, key));
    nodes[id].key = key;
    int p = nodes[id].parent;
    if (p != -1 && comp(key, nodes[p].key))
    {
      cut(id, p);
      while (nodes[p].parent != -1)
      {
        if (!nodes[p].marked) { nodes[p].marked = true; break; }
        const int parent = nodes[p].parent;
        cut(p, parent);
        p = parent;
      }
    }
    if (comp(key, nodes[minimum].key)) minimum = id;
  }
  void push_or_decrease(int id, const Key &key)
  {
    if (contains(id)) decrease_key(id, key);
    else push(id, key);
  }
  void pop()
  {
    assert(!empty());
    const int z = minimum;
    int child = nodes[z].child;
    if (child != -1)
    {
      int v = child;
      do
      {
        const int next = nodes[v].next;
        add_root(v);
        v = next;
      } while (v != child);
    }
    const int next = nodes[z].next;
    detach(z);
    nodes[z].child = -1, nodes[z].present = false;
    count--;
    if (count == 0) { minimum = -1; return; }
    roots.clear();
    int v = next;
    do { roots.eb(v); v = nodes[v].next; } while (v != next);
    degrees.clear();
    for (int x : roots)
    {
      // 子にした根は根リストから外すので、未処理の根を再び辿らずに済む。
      if (nodes[x].parent != -1) continue;
      int d = nodes[x].degree;
      while (true)
      {
        if (d >= int(degrees.size())) degrees.resize(d + 1, -1);
        int y = degrees[d];
        if (y == -1) break;
        degrees[d] = -1;
        if (comp(nodes[y].key, nodes[x].key)) swap(x, y);
        detach(y);
        nodes[y].parent = x, nodes[y].marked = false;
        if (nodes[x].child == -1) nodes[x].child = y;
        else insert_after(nodes[x].child, y);
        d = ++nodes[x].degree;
      }
      degrees[d] = x;
    }
    minimum = -1;
    for (int x : degrees) if (x != -1)
      if (minimum == -1 || comp(nodes[x].key, nodes[minimum].key)) minimum = x;
  }
  void clear()
  {
    for (auto &v : nodes) v.present = false;
    minimum = -1, count = 0;
    roots.clear(), degrees.clear();
  }
};
