#pragma once

#include "../template/template_all_but_modint.hpp"
#include "graph.hpp"

/**
 * @brief クリーク列挙
 * @docs docs/graph/cliques.md
 */

template <class I = ll>
class CliqueRange
{
  struct Data
  {
    vc<int> vertices;
    CSR<int> forward;

    template <class Cost, bool is_erasable>
    explicit Data(const GraphUndirected<Cost, is_erasable> &g) : vertices(g.size())
    {
      const int n = g.size();
      iota(vertices.begin(), vertices.end(), 0);
      // 次数順に向きづけると、各頂点から先の候補数は sqrt(2m) 以下。
      sort(vertices.begin(), vertices.end(), [&](int u, int v)
      { return pair{g.out_edges(u).size(), u} < pair{g.out_edges(v).size(), v}; });
      vc<int> rank(n);
      repi(i, n) rank[vertices[i]] = i;
      vc<pair<int, int>> es;
      es.reserve(g.num_of_edges());
      repi(u, n) for (auto e : g.out_edges(u))
        if (rank[u] < rank[e.to]) es.emplace_back(rank[u], rank[e.to]);
      forward = CSR<int>(n, es);
      forward.sortunique();
    }
  };
  shared_ptr<const Data> data;

  struct State
  {
    struct Frame
    {
      vc<int> candidates;
      int pos = 0;
    };
    shared_ptr<const Data> data;
    int root = 0;
    vc<Frame> stack;
    vc<I> clique;

    explicit State(shared_ptr<const Data> data) : data(move(data)) {}

    bool next(vc<I> &value)
    {
      while (true)
      {
        if (stack.empty())
        {
          if (root == int(data->vertices.size())) return false;
          const int u = root++;
          clique = {I(data->vertices[u])};
          stack.push_back({data->forward[u].to_v(), 0});
        }
        else
        {
          auto &frame = stack.back();
          if (frame.pos == int(frame.candidates.size()))
          {
            stack.pop_back();
            clique.pop_back();
            continue;
          }
          const int u = frame.candidates[frame.pos++];
          const auto row = data->forward[u];
          vc<int> candidates;
          set_intersection(frame.candidates.begin() + frame.pos, frame.candidates.end(),
                           row.begin(), row.end(), back_inserter(candidates));
          clique.push_back(I(data->vertices[u]));
          stack.push_back({move(candidates), 0});
        }
        value = clique;
        return true;
      }
    }
  };

public:
  class Iterator
  {
    friend class CliqueRange;
    shared_ptr<State> state;
    vc<I> value;
    explicit Iterator(shared_ptr<const Data> data)
        : state(make_shared<State>(move(data))) { ++*this; }

  public:
    using iterator_category = input_iterator_tag;
    using value_type = vc<I>;
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
      return state->data == other.state->data && value == other.value;
    }
    bool operator!=(const Iterator &other) const { return !(*this == other); }
  };

  template <class Cost, bool is_erasable>
  explicit CliqueRange(const GraphUndirected<Cost, is_erasable> &g)
      : data(make_shared<Data>(g)) {}

  Iterator begin() const { return Iterator(data); }
  Iterator end() const { return Iterator(); }
};

// 無向グラフの空でないクリークを、頂点列として一度ずつ列挙する。
template <class I = ll, class Cost, bool is_erasable>
CliqueRange<I> cliques(const GraphUndirected<Cost, is_erasable> &g)
{
  return CliqueRange<I>(g);
}
