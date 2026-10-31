#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"
#include "geometry/point.hpp"

using P = Point<ll>;
constexpr P origin{}, east{1, 0}, north{0, 1}, west{-1, 0}, south{0, -1};
static_assert(polar_less(origin, east) && polar_less(east, north));
static_assert(polar_less(north, west) && polar_less(west, south));
static_assert(polar_equal_to(P(1, 1), P(7, 7)));
static_assert(!polar_equal_to(P(1, 1), P(-1, -1)));
static_assert(polar_less_equal(east, P(8, 0)) && polar_greater_equal(east, P(8, 0)));
static_assert(polar_greater(south, east) && polar_not_equal_to(origin, east));
static_assert(polar_equal_to(origin, origin) && !polar_less(origin, origin));
static_assert(polar_less(P(4'000'000'000LL, 3'000'000'000LL), P(-4'000'000'000LL, 3'000'000'000LL)));
static_assert(polar_less(Point<double>{1, 0}, Point<double>{1, 1e-12}));
static_assert(polar_equal_to(Point<f128>{f128(1), f128(1)}, Point<f128>{f128(2), f128(2)}));

// Primitive integer directions give an independent angle oracle on this small grid.
long double oracle_angle(pair<ll, ll> p)
{
  if (p == pair<ll, ll>{0, 0}) return -1;
  ll g = gcd(abs(p.first), abs(p.second));
  long double a = atan2l(p.second / g, p.first / g);
  return a < 0 ? a + 2 * acosl(-1) : a;
}

template <class T> void check_type()
{
  using PT = Point<T>;
  vector<pair<ll, ll>> xy;
  for (ll x = -3; x <= 3; ++x) for (ll y = -3; y <= 3; ++y) xy.emplace_back(x, y);
  xy.emplace_back(0, 0);
  xy.emplace_back(2, 2);
  vector<PT> points(xy.begin(), xy.end());
  for (size_t i = 0; i < points.size(); ++i) for (size_t j = 0; j < points.size(); ++j)
  {
    const auto &a = points[i];
    const auto &b = points[j];
    long double x = oracle_angle(xy[i]), y = oracle_angle(xy[j]);
    assert(polar_less(a, b) == (x < y));
    assert(polar_less_equal(a, b) == (x <= y));
    assert(polar_greater(a, b) == (x > y));
    assert(polar_greater_equal(a, b) == (x >= y));
    assert(polar_equal_to(a, b) == (x == y));
    assert(polar_not_equal_to(a, b) == (x != y));
  }
  auto ordered = sorted(points, polar_less);
  auto descending = sorted(points, polar_greater);
  assert(points == vector<PT>(xy.begin(), xy.end()));
  assert(is_sorted(ordered.begin(), ordered.end(), polar_less));
  assert(is_sorted(descending.begin(), descending.end(), polar_greater));
  auto queries = xy;
  queries.emplace_back(7, 2);
  queries.emplace_back(-5, 7);
  queries.emplace_back(5, -7);
  for (auto q : queries)
  {
    const PT key(q);
    ll first = 0, last = 0;
    for (auto p : xy)
    {
      first += oracle_angle(p) < oracle_angle(q);
      last += oracle_angle(p) <= oracle_angle(q);
    }
    assert(lower_bound(ordered.begin(), ordered.end(), key, polar_less) - ordered.begin() == first);
    assert(upper_bound(ordered.begin(), ordered.end(), key, polar_less) - ordered.begin() == last);
    assert(geq_min(ordered, key, polar_less) == first);
    assert(gt_min(ordered, key, polar_less) == last);
    assert(lt_max(ordered, key, polar_less) == first - 1);
    assert(leq_max(ordered, key, polar_less) == last - 1);
    assert(geq_min(descending, key, polar_greater) == ll(points.size()) - last);
    assert(gt_min(descending, key, polar_greater) == ll(points.size()) - first);
#if __cplusplus >= 202002L
    assert(ranges::lower_bound(ordered, key, polar_less) - ordered.begin() == first);
    assert(ranges::upper_bound(ordered, key, polar_less) - ordered.begin() == last);
#endif
  }
  auto distinct = ordered;
  distinct.erase(unique(distinct.begin(), distinct.end(), polar_equal_to), distinct.end());
  set<PT, remove_cv_t<decltype(polar_less)>> directions(points.begin(), points.end());
  assert(directions.size() == distinct.size());
#if __cplusplus >= 202002L
  ranges::sort(points, polar_less);
  static_assert(strict_weak_order<decltype(polar_less), const PT &, const PT &>);
#endif
}

void fractional_and_small_real()
{
  using Q = Rational<i128>;
  Point<Q> a{Q(1, 2), Q(1, 3)}, b{Q(3, 2), Q(1)}, c{Q(-1, 2), Q(-1, 3)};
  assert(polar_equal_to(a, b) && polar_not_equal_to(a, c));
  assert(polar_less(a, c) && polar_greater(c, a));
  Point<double> x{1, 0}, y{1, 1e-12}, z{1, -1e-12}, negative_zero{-0.0, 0.0};
  assert(polar_less(x, y) && polar_less(y, z));
  assert(polar_not_equal_to(x, y) && polar_equal_to(negative_zero, Point<double>{}));
  assert(polar_less(negative_zero, x));
  vector<P> empty;
  assert(geq_min(empty, east, polar_less) == 0 && gt_min(empty, east, polar_less) == 0);
}

// Exercise the user's sort-and-binary-search pattern against half-plane enumeration.
void half_plane_queries()
{
  mt19937 rng(481);
  for (int trial = 0; trial < 300; ++trial)
  {
    int n = 1 + rng() % 15;
    vector<P> points;
    while (int(points.size()) < n)
    {
      P p{ll(rng() % 17) - 8, ll(rng() % 17) - 8};
      if (p == P{}) continue;
      bool valid = true;
      for (P q : points) if (cross(p, q) == 0) valid = false;
      if (valid) points.push_back(p);
    }
    sort(points.begin(), points.end(), polar_less);
    ll answer = 0, expected = 0;
    for (auto p : points)
    {
      auto q = -p;
      if (polar_less(q, p)) swap(p, q);
      ll l = geq_min(points, p, polar_less), r = geq_min(points, q, polar_less);
      answer = max({answer, r - l, n - r + l});
      l = gt_min(points, p, polar_less); r = gt_min(points, q, polar_less);
      answer = max({answer, r - l, n - r + l});
    }
    for (P p : points) for (ll side : {-1LL, 1LL})
    {
      P normal = rot90(p) * 1024LL + p * side;
      ll positive = 0, negative = 0;
      for (P q : points)
      {
        auto d = dot(normal, q);
        assert(d != 0);
        positive += d > 0;
        negative += d < 0;
      }
      expected = max({expected, positive, negative});
    }
    assert(answer == expected);
  }
}

int main()
{
  check_type<int>(); check_type<ll>(); check_type<i128>();
  check_type<Rational<ll>>(); check_type<Rational<i128>>();
  check_type<float>(); check_type<double>(); check_type<long double>(); check_type<f128>();
  fractional_and_small_real();
  half_plane_queries();
  cout << "Hello World\n";
}
