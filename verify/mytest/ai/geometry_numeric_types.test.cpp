#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"
#include "geometry/geometry.hpp"

using Q = Rational<i128>;
static_assert(is_same_v<decltype(dot(Point<int>{}, Point<int>{})), ll>);
static_assert(is_same_v<decltype(cross(Point<int>{}, Point<int>{}, Point<int>{})), ll>);
static_assert(is_same_v<decltype(distance2(Point<int>{}, Point<int>{})), ll>);
static_assert(is_same_v<decltype(dot(Point<ll>{}, Point<ll>{})), i128>);
static_assert(is_same_v<decltype(dot(Point<Rational<int>>{}, Point<Rational<int>>{})), Rational<ll>>);
static_assert(is_same_v<geometry_field_t<int>, Q>);
static_assert(is_same_v<decltype(Line<int>{}.vec()), Point<ll>>);
constexpr Point<int> a{-100000000, 0}, b{100000000, 0}, c{0, 100000000};
static_assert(cross(a, b, c) == 20000000000000000LL);
static_assert(distance2(a, b) == 40000000000000000LL);

template <class T> bool near(const T &x, long double expected)
{
  long double v = geometry_detail::cast<long double>(x);
  return abs(v - expected) < 1e-5L * (1 + abs(expected));
}

template <class T> void default_types()
{
  using P = Point<T>; using L = Line<T>; using S = Segment<T>; using C = Circle<T>;
  static_assert(is_same_v<decltype(norm(P{})), ld>);
  static_assert(is_same_v<decltype(distance(P{}, P{})), ld>);
  static_assert(is_same_v<decltype(distance(L{}, P{})), ld>);
  static_assert(is_same_v<decltype(distance(S{}, L{})), ld>);
  static_assert(is_same_v<decltype(distance(C{}, P{})), ld>);
  static_assert(is_same_v<decltype(distance(C{}, L{})), ld>);
  static_assert(is_same_v<decltype(distance(C{}, S{})), ld>);
  static_assert(is_same_v<decltype(distance(C{}, C{})), ld>);
  static_assert(is_same_v<decltype(argument(P{})), ld>);
  static_assert(is_same_v<decltype(angle(P{}, P{})), ld>);
  static_assert(is_same_v<decltype(rotate(P{}, 0)), Point<ld>>);
  static_assert(is_same_v<decltype(unit(P{})), optional<Point<ld>>>);
  static_assert(is_same_v<decltype(perimeter(vector<P>{})), ld>);
  static_assert(is_same_v<decltype(intersection(C{}, L{}, 1e-20L)), Intersection<ld>>);
  static_assert(is_same_v<decltype(tangents(C{}, P{}, 1e-20L)), Tangents<ld>>);
  static_assert(is_same_v<decltype(common_tangents(C{}, C{})), Tangents<ld>>);
  static_assert(is_same_v<decltype(incenter(P{}, P{}, P{})), optional<Point<ld>>>);
  static_assert(is_same_v<decltype(incircle(P{}, P{}, P{})), optional<Circle<ld>>>);
  static_assert(is_same_v<decltype(circumcircle(P{}, P{}, P{})), optional<Circle<ld>>>);
  static_assert(is_same_v<decltype(signed_area2(vector<P>{})), larger_int_t<T>>);
}

template <class T, class R> void real_api()
{
  using P = Point<T>; using L = Line<T>; using S = Segment<T>; using C = Circle<T>;
  P p{T(3), T(4)}, zero{};
  C circle(zero, T(5)), far_circle(P(T(20), T(0)), T(5));
  L line(P(T(-10), T(0)), P(T(10), T(0)));
  S segment(line.a, line.b), outside(P(T(8), T(0)), P(T(9), T(0)));
  static_assert(is_same_v<decltype(norm<R>(p)), R>);
  static_assert(is_same_v<decltype(distance<R>(line, p)), R>);
  static_assert(is_same_v<decltype(distance<R>(circle, outside)), R>);
  static_assert(is_same_v<decltype(intersection<R>(circle, line)), Intersection<R>>);
  static_assert(is_same_v<decltype(intersection<R>(line, circle)), Intersection<R>>);
  static_assert(is_same_v<decltype(tangents<R>(circle, p)), Tangents<R>>);
  assert(near(norm<R>(p), 5) && near(distance<R>(zero, p), 5));
  assert(near(distance<R>(line, p), 4) && near(distance<R>(p, line), 4));
  assert(near(distance<R>(segment, p), 4) && near(distance<R>(p, segment), 4));
  assert(near(distance<R>(line, segment), 0) && near(distance<R>(segment, line), 0));
  assert(near(distance<R>(circle, p), 0) && near(distance<R>(p, circle), 0));
  assert(near(distance<R>(circle, outside), 3) && near(distance<R>(outside, circle), 3));
  assert(near(distance<R>(circle, far_circle), 10));
  assert(near(distance<R>(circle, line), 0) && near(distance<R>(line, circle), 0));
  assert(near(argument<R>(P(T(1), T(0))), 0));
  assert(near(angle<R>(P(T(1), T(0)), P(T(0), T(1))), acosl(-1) / 2));
  auto rotated = rotate<R>(p, R(0));
  const P const_point = p;
  assert(rotated == rotate<R>(const_point, R(0)) && near(rotated.x, 3) && near(rotated.y, 4));
  auto direction = unit<R>(p);
  assert(direction && near(direction->x, 0.6L) && near(direction->y, 0.8L));
  assert(!unit<R>(zero));
  auto hits = intersection<R>(circle, line);
  assert(hits.points.size() == 2 && intersection<R>(line, circle).points == hits.points);
  assert(near(hits.points.front().x, -5) && near(hits.points.back().x, 5));
  assert(intersection<R>(circle, segment).points.size() == 2);
  assert(intersection<R>(segment, circle).points.size() == 2);
  assert(intersection<R>(circle, C(P(T(6), T(0)), T(5))).points.size() == 2);
  assert(tangents<R>(circle, P(T(13), T(0))).lines.size() == 2);
  assert(tangents<R>(P(T(13), T(0)), circle).lines.size() == 2);
  assert(common_tangents<R>(circle, far_circle).lines.size() == 4);
  P x{T(4), T(0)}, y{T(0), T(3)};
  assert(near(perimeter<R>(vector<P>{zero, x, y}), 12));
  auto inner = incenter<R>(zero, x, y);
  auto inc = incircle<R>(zero, x, y);
  auto outer = circumcircle<R>(zero, x, y);
  assert(inner && near(inner->x, 1) && near(inner->y, 1));
  assert(inc && near(inc->r, 1) && outer && near(outer->r, 2.5L));
  assert((outer->c == Point<R>(R(2), R(1.5))));
  assert(!incenter<R>(zero, zero, zero) && !incircle<R>(zero, zero, zero) && !circumcircle<R>(zero, zero, zero));
}

void high_degree_integer()
{
  constexpr int scale = 100000000;
  using P = Point<int>;
  auto center = circumcenter(a, b, c);
  assert(center && *center == Point<Q>{});
  auto foot = projection(Line<int>({0,0}, {scale,scale}), P(scale,0));
  assert(foot == Point<Q>(Q(scale / 2), Q(scale / 2)));
  Circle<int> circle(P{}, scale);
  Line<int> inside({0,scale-1}, {scale,scale-1}), outside({0,scale+1}, {scale,scale+1});
  assert(intersects(circle, inside) && !intersects(circle, outside));
  assert(intersection<f128>(circle, inside).points.size() == 2);
  assert(intersection<f128>(circle, outside).empty());
  Segment<int> s({-scale,0}, {scale,0});
  assert(distance2(s, P(0,scale)) == Q(i128(scale) * scale));
  mt19937 rng(20261101);
  for (int trial = 0; trial < 2000; ++trial)
  {
    P x{int(rng()%200000001)-scale, int(rng()%200000001)-scale};
    P y{int(rng()%200000001)-scale, int(rng()%200000001)-scale};
    if (x == y) continue;
    Line<int> line(x,y);
    assert(intersects(circle,line) == intersects(circle.cast<ll>(),line.cast<ll>()));
  }
}

void requested_precision()
{
  ll value = (1LL << 54) + 1;
  Point<ll> p{value,0};
  assert(norm<f128>(p) == f128(value));
  assert(unit<f128>(p)->x == f128(1));
  assert(perimeter<f128>(vector<Point<ll>>{{0,0},p}) == f128(2) * f128(value));
  Circle<ll> circle({0,0},1);
  assert(distance<f128>(circle,p) == f128(value) - 1);
  assert(distance<f128>(p,circle) == f128(value) - 1);
  auto hit = intersection<f128>(Circle<ll>(p,1), Line<ll>({value-2,0},{value+2,0}));
  assert(hit.points.size()==2 && hit.points[0].x == f128(value)-1 && hit.points[1].x == f128(value)+1);
  Point<float> large{1e20f,1e20f};
  assert(isfinite(norm<double>(large)) && isfinite(distance<double>(Point<float>{},large)));
  auto h = intersection<double>(Circle<float>({0,0},1e20f), Line<float>({-1e20f,0},{1e20f,0}));
  assert(h.points.size()==2 && isfinite(h.points[0].x));
}

template <class T> void check()
{
  default_types<T>();
  real_api<T, float>(); real_api<T, double>(); real_api<T, long double>(); real_api<T, f128>();
}

int main()
{
  check<int>(); check<ll>(); check<i128>(); check<Rational<int>>(); check<Rational<ll>>(); check<Q>();
  check<float>(); check<double>(); check<long double>(); check<f128>();
  high_degree_integer(); requested_precision();
  PRINT("Hello World");
}
