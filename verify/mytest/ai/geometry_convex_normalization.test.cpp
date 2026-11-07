#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "geometry/point_set.hpp"

template <class T>
void check_type()
{
  using P = Point<T>;
  using D = decltype(distance2(P{}, P{}));
  assert(!convex_diameter(vector<P>{}));
  vector<P> same(8, P(T(3), T(-4)));
  auto one = convex_diameter(same);
  assert(one && one->first == same[0] && one->second == same[0]);
  vector<P> line{{T(0), T(0)}, {T(3), T(-6)}, {T(-2), T(4)}, {T(1), T(-2)}, {T(0), T(0)}};
  auto ends = convex_diameter(line);
  assert(ends && ends->first == P(T(-2), T(4)) && ends->second == P(T(3), T(-6)));
  vector<P> rectangle{{T(0), T(0)}, {T(2), T(0)}, {T(4), T(0)}, {T(4), T(3)},
                      {T(4), T(6)}, {T(2), T(6)}, {T(0), T(6)}, {T(0), T(3)}};
  for (int reverse_order = 0; reverse_order < 2; reverse_order++)
  {
    for (int shift = 0; shift < 8; shift++)
    {
      vector<P> repeated;
      for (auto p : rectangle) repeated.push_back(p), repeated.push_back(p);
      repeated.push_back(repeated[0]);
      auto original = repeated;
      auto result = convex_diameter(repeated);
      assert(result && distance2(result->first, result->second) == D(52));
      assert(repeated == original);
      rotate(rectangle.begin(), rectangle.begin() + 1, rectangle.end());
    }
    reverse(rectangle.begin(), rectangle.end());
  }
  mt19937 rng(20261102);
  for (int trial = 0; trial < 1000; trial++)
  {
    vector<P> points;
    for (int i = 0, n = rng() % 30; i < n; i++)
      points.emplace_back(T(int(rng() % 31) - 15), T(int(rng() % 31) - 15));
    auto hull = convex_hull(points, true);
    auto original = hull;
    D expected = 0;
    for (auto p : points) for (auto q : points) expected = max(expected, distance2(p, q));
    for (int orientation = 0; orientation < 2; orientation++)
    {
      auto result = convex_diameter(hull);
      assert(bool(result) == !points.empty());
      if (result) assert(distance2(result->first, result->second) == expected);
      reverse(hull.begin(), hull.end());
    }
    assert(hull == original);
  }
}

int main()
{
  check_type<int>(); check_type<ll>(); check_type<i128>();
  check_type<Rational<int>>(); check_type<Rational<ll>>(); check_type<Rational<i128>>();
  check_type<float>(); check_type<double>(); check_type<ld>(); check_type<f128>();
  using P = Point<ld>;
  const ld tiny = EPS / 100;
  vector<P> p{{0, 0}, {3 * tiny, 0}, {0, 4 * tiny}};
  auto result = convex_diameter(p);
  assert(result && distance2(result->first, result->second) == distance2(p[1], p[2]));
  cout << "Hello World\n";
}
