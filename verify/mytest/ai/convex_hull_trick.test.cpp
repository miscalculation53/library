#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "math/rational.hpp"
#include "convex/convex_hull_trick.hpp"

// Test focus: min/max objectives, both slope modes, IDs, segments, empty results,
// monotone queries, and rational/floating-point coordinates.
mt19937_64 cht_rng(1234567);

ll random_ll(ll l, ll r)
{
  return l + cht_rng() % (r - l + 1);
}

template <bool minimize>
ll brute(const vc<pair<ll, ll>> &lines, ll x)
{
  ll ans = minimize ? numeric_limits<ll>::max() : numeric_limits<ll>::lowest();
  for (auto [a, b] : lines)
  {
    ll value = a * x + b;
    if constexpr (minimize)
      ans = min(ans, value);
    else
      ans = max(ans, value);
  }
  return ans;
}

template <bool minimize, class CHT>
void verify_hull(CHT &cht, const vc<pair<ll, ll>> &lines)
{
  rep3(x, -100, 101, 1)
  {
    auto [value, line] = cht.query_with_line(x);
    auto line_only = cht.query_line(x);
    assert(value == brute<minimize>(lines, x));
    assert(value == line.a * x + line.b);
    assert(line_only.a * x + line_only.b == value);
    assert(0 <= line.id && line.id < SZ(lines));
    assert((lines[line.id] == pair{line.a, line.b}));
  }

  auto segments = cht.segments();
  assert(!segments.empty());
  assert(segments.front().left == -ll(INF));
  assert(segments.back().right == ll(INF));
  repi(i, 1, segments.size())
  {
    if constexpr (minimize)
      assert(segments[i - 1].line.a > segments[i].line.a);
    else
      assert(segments[i - 1].line.a < segments[i].line.a);
    assert(segments[i].left == segments[i - 1].right);
  }
  rep3(x, -100, 101, 1)
  {
    ll got = minimize ? numeric_limits<ll>::max() : numeric_limits<ll>::lowest();
    for (auto segment : segments)
    {
      auto line = segment.line;
      if constexpr (minimize)
        got = min(got, line.a * x + line.b);
      else
        got = max(got, line.a * x + line.b);
    }
    assert(got == brute<minimize>(lines, x));
  }
  for (auto segment : segments)
  {
    auto line = segment.line;
    assert(0 <= line.id && line.id < SZ(lines));
    assert((lines[line.id] == pair{line.a, line.b}));
  }

  vc<ll> queries;
  rep3(x, -100, 101, 1) queries.eb(x);
  cht.reset_monotone_query();
  for (ll x : queries) assert(cht.query_monotone(x) == brute<minimize>(lines, x));
  reverse(queries.begin(), queries.end());
  cht.reset_monotone_query();
  for (ll x : queries)
  {
    auto [value, line] = cht.query_monotone_with_line(x);
    assert(value == brute<minimize>(lines, x));
    assert(value == line.a * x + line.b);
    assert(0 <= line.id && line.id < SZ(lines));
    assert((lines[line.id] == pair{line.a, line.b}));
  }
}

template <bool minimize>
void test_arbitrary_slope()
{
  using Compare = conditional_t<minimize, less<>, greater<>>;
  repi(iter, 100)
  {
    ConvexHullTrick<ll, Compare> cht;
    vc<pair<ll, ll>> lines;
    repi(i, 30)
    {
      ll a = random_ll(-20, 20), b = random_ll(-50, 50);
      int id = lines.size();
      lines.eb(a, b);
      cht.add_line(a, b, id);
    }
    verify_hull<minimize>(cht, lines);

    ll a = random_ll(-20, 20), b = random_ll(-50, 50);
    int id = lines.size();
    lines.eb(a, b);
    cht.add_line(a, b, id);
    verify_hull<minimize>(cht, lines);
  }
}

template <bool minimize>
void test_monotone_slope()
{
  using Compare = conditional_t<minimize, less<>, greater<>>;
  repi(iter, 100)
  {
    vc<pair<ll, ll>> lines;
    repi(i, 30) lines.eb(random_ll(-20, 20), random_ll(-50, 50));
    sort(lines.begin(), lines.end());
    if (iter & 1) reverse(lines.begin(), lines.end());

    ConvexHullTrickMonotoneSlope<ll, Compare> cht;
    repi(i, lines.size()) cht.add_line(lines[i].first, lines[i].second, i);
    verify_hull<minimize>(cht, lines);
  }
}

void test_empty_and_clear()
{
  ConvexHullTrick<ll, less<>, 1234567> min_cht;
  assert(min_cht.query(10) == 1234567);
  assert((min_cht.query_line(10) == ConvexHullTrickLine<ll>{0, 1234567, -1}));
  assert((min_cht.query_with_line(10).second == ConvexHullTrickLine<ll>{0, 1234567, -1}));

  ConvexHullTrickMonotoneSlope<ll, greater<>, 7654321> max_cht;
  assert(max_cht.query(-10) == -7654321);
  assert((max_cht.query_monotone_line(-10) == ConvexHullTrickLine<ll>{0, -7654321, -1}));
  assert((max_cht.query_monotone_with_line(-10).second == ConvexHullTrickLine<ll>{0, -7654321, -1}));

  min_cht.add_line(3, 4, 42);
  assert(min_cht.query_with_line(5).second.id == 42);
  min_cht.clear();
  assert(min_cht.query(5) == 1234567);
}

void test_segment_boundaries()
{
  ConvexHullTrick<ll> cht;
  cht.add_line(2, 1, 10);
  cht.add_line(0, 0, 20);
  auto segments = cht.segments();
  assert(segments.size() == 2);
  assert(segments[0].left == -ll(INF));
  assert(segments[0].right == -1);
  assert(segments[0].line.id == 10);
  assert(segments[1].left == -1);
  assert(segments[1].right == ll(INF));
  assert(segments[1].line.id == 20);
  assert(cht.query_with_line(-1).second.id == 10);
  assert(cht.query_with_line(0).second.id == 20);
}

template <class CHT>
void verify_auto_id()
{
  CHT cht;
  cht.add_line(1, 0);
  cht.add_line(0, -5, 42);
  cht.add_line(-1, 0);
  assert(cht.query_with_line(-10).second.id == 0);
  assert(cht.query_with_line(0).second.id == 42);
  assert(cht.query_with_line(10).second.id == 2);

  cht.clear();
  cht.add_line(0, 1);
  assert(cht.query_with_line(0).second.id == 0);
}

void test_auto_id()
{
  verify_auto_id<ConvexHullTrick<>>();
  verify_auto_id<ConvexHullTrickMonotoneSlope<>>();
}

void test_floating_point()
{
#if __cplusplus >= 202002L
  ConvexHullTrick<long double> cht;
  vc<pair<long double, long double>> lines;
  repi(i, 50)
  {
    long double a = random_ll(-100, 100) / 7.0L;
    long double b = random_ll(-100, 100) / 11.0L;
    lines.eb(a, b);
    cht.add_line(a, b, i);
  }
  repi(i, 100)
  {
    long double x = random_ll(-1000, 1000) / 13.0L;
    long double ans = numeric_limits<long double>::max();
    for (auto [a, b] : lines) ans = min(ans, a * x + b);
    assert(abs(cht.query(x) - ans) < 1e-15L);
  }
#endif
}

template <bool minimize>
void test_rational()
{
  using R = Rational<ll>;
  using Compare = conditional_t<minimize, less<>, greater<>>;
  vc<pair<ll, ll>> lines;
  repi(i, 50) lines.eb(random_ll(-100, 100), random_ll(-100, 100));

  ConvexHullTrick<ll, Compare, INF, R> coordinate_cht;
  ConvexHullTrick<R, Compare> direct_cht;
  repi(i, lines.size())
  {
    auto [a, b] = lines[i];
    coordinate_cht.add_line(a, b, i);
    direct_cht.add_line(R(a), R(b), i);
  }

  repi(_, 500)
  {
    R x(random_ll(-100, 100), random_ll(1, 100));
    optional<R> expected;
    for (auto [a, b] : lines)
    {
      R value = R(a) * x + b;
      if (!expected || (minimize ? value < *expected : *expected < value))
        expected = value;
    }

    auto [value1, line1] = coordinate_cht.query_with_line(x);
    auto [value2, line2] = direct_cht.query_with_line(x);
    assert(value1 == *expected && value2 == *expected);
    assert(value1 == R(line1.a) * x + line1.b);
    assert(value2 == line2.a * x + line2.b);
  }

  vc<R> queries;
  repi(_, 500) queries.eb(random_ll(-100, 100), random_ll(1, 100));
  sort(ALL(queries));
  coordinate_cht.reset_monotone_query();
  direct_cht.reset_monotone_query();
  for (R x : queries)
  {
    optional<R> expected;
    for (auto [a, b] : lines)
    {
      R value = R(a) * x + b;
      if (!expected || (minimize ? value < *expected : *expected < value))
        expected = value;
    }
    assert(coordinate_cht.query_monotone(x) == *expected);
    assert(direct_cht.query_monotone(x) == *expected);
  }

  sort(lines.begin(), lines.end());
  ConvexHullTrickMonotoneSlope<ll, Compare, INF, R> monotone_cht;
  repi(i, lines.size()) monotone_cht.add_line(lines[i].first, lines[i].second, i);
  repi(_, 500)
  {
    R x(random_ll(-100, 100), random_ll(1, 100));
    optional<R> expected;
    for (auto [a, b] : lines)
    {
      R value = R(a) * x + b;
      if (!expected || (minimize ? value < *expected : *expected < value))
        expected = value;
    }
    assert(monotone_cht.query(x) == *expected);
  }
}

void test_long_double_coordinate()
{
  ConvexHullTrick<ll, less<>, INF, long double> cht;
  vc<pair<ll, ll>> lines;
  repi(i, 50)
  {
    ll a = random_ll(-1000, 1000), b = random_ll(-1000, 1000);
    lines.eb(a, b);
    cht.add_line(a, b, i);
  }
  repi(_, 500)
  {
    ll x = random_ll(-1000, 1000), y = random_ll(1, 1000);
    auto [value, line] = cht.query_with_line((long double)x / y);
    (void)value;
    i128 got = i128(line.a) * x + i128(line.b) * y;
    i128 expected = numeric_limits<i128>::max();
    for (auto [a, b] : lines) expected = min(expected, i128(a) * x + i128(b) * y);
    assert(got == expected);
  }
}

int main()
{
  test_arbitrary_slope<true>();
  test_arbitrary_slope<false>();
  test_monotone_slope<true>();
  test_monotone_slope<false>();
  test_empty_and_clear();
  test_segment_boundaries();
  test_auto_id();
  test_floating_point();
  test_rational<true>();
  test_rational<false>();
  test_long_double_coordinate();
  cout << "Hello World" << endl;
}
