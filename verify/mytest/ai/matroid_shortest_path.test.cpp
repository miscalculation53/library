#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "matroid/internal_shortest_path.hpp"

// W の構築・等値比較・無限大に依存せず、順序付き加法群として使う。
struct OrderedWeight
{
  ll value = 0;
  OrderedWeight() = default;
  OrderedWeight(int) = delete;
  static OrderedWeight make(ll value)
  {
    OrderedWeight result;
    result.value = value;
    return result;
  }
  friend OrderedWeight operator+(const OrderedWeight &a, const OrderedWeight &b)
  { return make(a.value + b.value); }
  friend OrderedWeight operator-(const OrderedWeight &a) { return make(-a.value); }
  friend bool operator<(const OrderedWeight &a, const OrderedWeight &b) { return a.value < b.value; }
};

template <class W, class Make>
void check_cost(const Make &make)
{
  using C = internal::MatroidPathCost<W>;
  const C zero = 0, a(make(3), 4), b(make(-2), 8), inf = C::infty();
  assert(zero == C{} && zero.finite() && !zero.is_infinite());
  assert(a + b == C(make(1), 12));
  assert(a - b == C(make(5), -4));
  assert(-a == C(make(-3), -4));
  auto value = a;
  value += b;
  value -= b;
  assert(value == a);
  assert(C(make(3), 3) < a && C(make(2), 100) < a);
  assert(a <= a && a >= a && !(a != a));
  assert(-inf < zero && zero < inf && inf == inf && -inf == -inf);
  assert(!inf.finite() && inf.is_infinite());
  assert(inf + a == inf && a + -inf == -inf && inf + inf == inf);
  assert(inf - a == inf && a - inf == -inf);
  assert(resolved_infty<C>() == inf);
}

template <class W, class Make>
void check_algorithms(const Make &make)
{
  using C = internal::MatroidPathCost<W>;
  const C inf = C::infty();
  const GraphDirected<C> negative(7, vc<tuple<int, int, C>>{
      {0, 1, C(make(3), 1)}, {1, 2, C(make(-5), 1)}, {2, 4, C(make(0), 1)},
      {2, 1, C(make(5), 1)}, {0, 3, C(make(-2), 1)}, {3, 4, C(make(0), 1)},
      {0, 4, C(make(-2), 4)}, {4, 5, C(make(0), 0)}, {0, 5, C(make(-1), 1)}});
  ShortestPath<true, C> bellman(negative);
  for (const auto &distance : {bellman.bellman_ford(0), bellman.spfa(0), bellman.solve(0, 5)})
  {
    assert(distance[5] == C(make(-2), 2));
    assert(distance[6] == inf);
    assert(bellman.path_vertex(5) == optional<vc<int>>({{0, 3, 4, 5}}));
    assert(bellman.path_vertex(0) == optional<vc<int>>({{0}}));
    assert(bellman.path_edge(0) && bellman.path_edge(0)->empty());
    assert(!bellman.path_vertex(6) && !bellman.path_edge(6));
    assert(bellman.path_edge(5)->size() == 3);
  }

  const GraphDirected<C> nonnegative(7, vc<tuple<int, int, C>>{
      {0, 1, C(make(0), 1)}, {1, 2, C(make(0), 1)}, {2, 4, C(make(1), 1)},
      {0, 3, C(make(1), 1)}, {3, 4, C(make(0), 1)}, {0, 4, C(make(1), 4)},
      {4, 5, C(make(0), 0)}, {0, 5, C(make(2), -100)}});
  ShortestPath<true, C> dijkstra(nonnegative);
  for (const auto &distance : {dijkstra.dijkstra(0, 5), dijkstra.dijkstra_dense(0, 5),
                               dijkstra.dijkstra_fibonacci(0, 5), dijkstra.solve(0, 5)})
  {
    assert(distance[5] == C(make(1), 2));
    assert(distance[6] == inf);
    assert(dijkstra.path_vertex(5) == optional<vc<int>>({{0, 3, 4, 5}}));
    assert(!dijkstra.path_vertex(6));
  }

  const GraphDirected<C> cycle(5, vc<tuple<int, int, C>>{
      {0, 1, C(make(0), 1)}, {1, 2, C(make(-1), 1)}, {2, 1, C(make(0), 1)},
      {0, 3, C(make(5), 1)}});
  ShortestPath<true, C> unbounded(cycle);
  const auto distance = unbounded.bellman_ford(0);
  assert(unbounded.spfa(0) == distance);
  assert(distance[0] == C(0) && distance[1] == -inf && distance[2] == -inf);
  assert(distance[3] == C(make(5), 1) && distance[4] == inf);
  for (int v : {1, 2, 4}) assert(!unbounded.path_vertex(v) && !unbounded.path_edge(v));

  const GraphDirected<C> same_weight(4, vc<tuple<int, int, C>>{
      {0, 1, C(make(0), 1)}, {1, 2, C(make(0), 1)}, {0, 2, C(make(0), 1)}});
  ShortestPath<true, C> automatic_bfs(same_weight);
  assert(automatic_bfs.solve(0)[2] == C(make(0), 1));
  assert(automatic_bfs.path_vertex(2) == optional<vc<int>>({{0, 2}}));
}

int main()
{
  static_assert(!is_constructible_v<OrderedWeight, int>);
  auto make_ll = [](ll value) { return value; };
  auto make_i128 = [](ll value) { return i128(value); };
  check_cost<ll>(make_ll);
  check_algorithms<ll>(make_ll);
  check_cost<i128>(make_i128);
  check_algorithms<i128>(make_i128);
  check_cost<OrderedWeight>(OrderedWeight::make);
  check_algorithms<OrderedWeight>(OrderedWeight::make);

  using C = internal::MatroidPathCost<i128>;
  const i128 large = i128(1) << 100;
  const GraphDirected<C> input(3, vc<tuple<int, int, C>>{
      {0, 1, C(large, 1)}, {1, 2, C(-2 * large, 1)}, {0, 2, C(0, 5)}});
  ShortestPath<true, C> shortest(input);
  assert(shortest.solve(0)[2] == C(-large, 2));
  cout << "Hello World\n";
}
