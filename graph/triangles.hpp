#pragma once

#include "../template/template_all_but_modint.hpp"
#include "graph.hpp"

/**
 * @brief 三角形列挙
 * @docs docs/graph/triangles.md
 */

class TriangleRange
{
  shared_ptr<const GraphDirected<>> g;

  struct State
  {
    shared_ptr<const GraphDirected<>> g;
    vc<int> marked;
    int u = 0, vi = 0, wi = 0;
    bool ready = false;

    explicit State(shared_ptr<const GraphDirected<>> g)
        : g(move(g)), marked(this->g->size(), -1) {}

    bool next(tuple<int, int, int> &value)
    {
      while (u < g->size())
      {
        const auto es = g->out_edges(u);
        if (!ready)
        {
          for (auto e : es) marked[e.to] = u;
          ready = true;
        }
        while (vi < es.size())
        {
          const int v = es[vi].to;
          const auto fs = g->out_edges(v);
          while (wi < fs.size())
          {
            const int w = fs[wi++].to;
            if (marked[w] == u)
            {
              value = {u, v, w};
              return true;
            }
          }
          ++vi;
          wi = 0;
        }
        ++u;
        vi = 0;
        ready = false;
      }
      return false;
    }
  };

public:
  class Iterator
  {
    friend class TriangleRange;
    shared_ptr<State> state;
    tuple<int, int, int> value{};
    explicit Iterator(shared_ptr<const GraphDirected<>> g)
        : state(make_shared<State>(move(g))) { ++*this; }

  public:
    using iterator_category = input_iterator_tag;
    using value_type = tuple<int, int, int>;
    using difference_type = ptrdiff_t;
    using pointer = const value_type *;
    using reference = const value_type &;

    Iterator() = default;
    reference operator*() const { return value; }
    pointer operator->() const { return &value; }
    Iterator &operator++()
    {
      if (!state->next(value)) state.reset();
      return *this;
    }
    Iterator operator++(int) { auto old = *this; ++*this; return old; }
    bool operator==(const Iterator &other) const
    {
      if (!state || !other.state) return !state && !other.state;
      return state->g == other.state->g && value == other.value;
    }
    bool operator!=(const Iterator &other) const { return !(*this == other); }
  };

  template <class Cost, bool is_erasable>
  explicit TriangleRange(const GraphUndirected<Cost, is_erasable> &source)
  {
    vc<pair<int, int>> es;
    es.reserve(source.num_of_edges());
    // 次数が大きい側から小さい側へ、同次数では頂点番号の昇順に向きづける。
    repi(u0, source.size()) for (auto e : source.out_edges(u0))
    {
      int u = u0, v = e.to;
      if (u >= v) continue;
      if (source.out_edges(u).size() < source.out_edges(v).size()) swap(u, v);
      es.emplace_back(u, v);
    }
    g = make_shared<GraphDirected<>>(source.size(), es);
  }

  Iterator begin() const { return Iterator(g); }
  Iterator end() const { return Iterator(); }
};

// 単純無向グラフの三角形を、頂点の組として一度ずつ列挙する。
template <class Cost, bool is_erasable>
TriangleRange triangles(const GraphUndirected<Cost, is_erasable> &g)
{
  return TriangleRange(g);
}
