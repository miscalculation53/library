#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"
#include "geometry/geometry.hpp"

using P = Point<ll>;
using L = Line<ll>;
using S = Segment<ll>;
using C = Circle<ll>;
using Q = Rational<i128>;
using PQ = Point<Q>;

constexpr Point<int> cp1{2, 3}, cp2{-1, 4};
constexpr Point<ll> pair_point = pair{2, 3};
constexpr pair<const int, short> mixed_pair{2, 3};
static_assert(pair_point == P(2, 3) && P(mixed_pair) == pair_point);
static_assert(is_same_v<decltype(Point(pair{2, 3LL})), P>);
static_assert(is_same_v<decltype(Point(mixed_pair)), Point<int>>);
static_assert(is_convertible_v<pair<int, short>, P>);
static_assert(!is_constructible_v<P, pair<string, int>>);
static_assert(!is_convertible_v<Point<int>, P>);
static_assert(is_trivially_copyable_v<P>);
constexpr bool pair_assignment()
{
  P p;
  p = mixed_pair;
  return p == pair_point;
}
static_assert(pair_assignment());
static_assert(cp1 + cp2 == Point<int>(1, 7));
static_assert(dot(cp1, cp2) == 10 && cross(cp1, cp2) == 11);
static_assert(is_same_v<decltype(dot(P{}, P{})), i128>);
static_assert(is_same_v<decltype(P{} / 2LL), PQ>);
static_assert(is_same_v<decltype(distance2(L{}, P{})), Q>);
static_assert(is_same_v<decltype(intersection(C{}, L{})), Intersection<ld>>);
static_assert(is_same_v<decltype(norm(Point<f128>{})), f128>);

bool near(long double a, long double b, long double eps = 1e-8L)
{ return abs(a - b) <= eps * (1 + abs(a) + abs(b)); }
template <class T> long double as_ld(const T &x) { return geometry_detail::cast<long double>(x); }

// Segment membership from coordinate bounds and an independently expanded determinant.
i128 determinant(P a, P b, P c)
{ return i128(a.x) * (b.y - c.y) + i128(b.x) * (c.y - a.y) + i128(c.x) * (a.y - b.y); }
int sign128(i128 x) { return (x > 0) - (x < 0); }
bool on(S s, P p)
{
  return determinant(s.a, s.b, p) == 0 && min(s.a.x, s.b.x) <= p.x && p.x <= max(s.a.x, s.b.x) &&
         min(s.a.y, s.b.y) <= p.y && p.y <= max(s.a.y, s.b.y);
}
bool crossing(S s, S t)
{
  if (on(s, t.a) || on(s, t.b) || on(t, s.a) || on(t, s.b)) return true;
  return sign128(determinant(s.a, s.b, t.a)) * sign128(determinant(s.a, s.b, t.b)) < 0 &&
         sign128(determinant(t.a, t.b, s.a)) * sign128(determinant(t.a, t.b, s.b)) < 0;
}

void basic()
{
  pair<int, ll> xy{3, 4};
  P from_pair = xy;
  assert(norm2(from_pair) == 25 && from_pair == P(3, 4));
  vector<P> pair_points{pair{1, 2}, xy};
  pair_points.emplace_back(xy);
  assert((pair_points == vector<P>{{1, 2}, {3, 4}, {3, 4}}));
  assert(([](P p) { return p.x + p.y; })(xy) == 7);
  assert((PQ(pair{1, 2}) == PQ(Q(1), Q(2))));
  assert(P() == P(0, 0));
  assert(rot90(P(3, 4)) == P(-4, 3));
  assert(norm2(P(3, 4)) == 25 && distance(P(), P(3, 4)) == 5);
  assert((P(1, 3) / 2LL == PQ(Q(1, 2), Q(3, 2))));
  assert(!unit(P()));
  assert(almost_equal(*unit(P(3, 4)), Point<ld>(0.6, 0.8)));
  assert(almost_equal(rotate(P(3, 4), acos(ld(-1)) / 2), Point<ld>(-4, 3)));
  assert(near(argument(P(-1, 0)), acos(ld(-1))));
  P p(4'000'000'000LL, 3'000'000'000LL);
  assert(norm2(p) == i128(25) * 1'000'000'000LL * 1'000'000'000LL);
  assert(cross(P(-4'000'000'000LL, 0), P(4'000'000'000LL, 0), P(0, 4'000'000'000LL)) ==
         i128(32) * 1'000'000'000LL * 1'000'000'000LL);
  vector<P> order{{0,-1},{-2,0},{0,0},{0,1},{1,0},{-1,0},{2,0}};
  sort(order.begin(), order.end(), polar_less);
  vector<P> directions{{0,0},{1,0},{1,0},{0,1},{-1,0},{-1,0},{0,-1}};
  for (size_t i = 0; i < order.size(); ++i) assert(polar_equal_to(order[i], directions[i]));
  for (auto a : order) for (auto b : order) for (auto c : order)
    if (polar_less(a,b) && polar_less(b,c)) assert(polar_less(a,c));
  Point<f128> fp{f128(3), f128(4)};
  assert(norm(fp) == f128(5));
  assert(almost_equal(rotate(fp, f128(0)), fp));
  assert(!almost_equal(Point<ld>(0,0), Point<ld>(1e-8,0)));
  assert(almost_equal(Point<ld>(0,0), Point<ld>(1e-12,0)));
  Point<ld> small{1e-12,0};
  assert(small != Point<ld>() && Point<ld>() < small);
  auto cq = circumcenter(P(0,0), P(4,0), P(0,2));
  assert(cq && *cq == PQ(Q(2), Q(1)));
  assert(!circumcenter(P(), P(1,1), P(2,2)));
  assert(!incircle(P(), P(), P(1,1)));
  auto ci = incircle(P(), P(4,0), P(0,3));
  assert(ci && near(ci->r, 1) && almost_equal(ci->c, Point<ld>(1,1)));
  auto cc = circumcircle(P(), P(4,0), P(0,3));
  assert(cc && near(cc->r, 2.5) && almost_equal(cc->c, Point<ld>(2,1.5)));
  istringstream in("1/2 -3/4  0 0 2 4  2 3 5");
  PQ q;
  S seg;
  C circle;
  in >> q >> seg >> circle;
  assert(q == PQ(Q(1,2), Q(-3,4)) && seg.a == P() && seg.b == P(2,4) && circle.c == P(2,3) && circle.r == 5);
  ostringstream out;
  out << q << ' ' << seg << ' ' << circle;
  assert(out.str() == "1/2 -3/4 0 0 2 4 2 3 5");
#ifdef LOCAL
  cp::options::es_style = cp::types::es_style_t::no_es;
  cp::options::max_line_width = 1000;
  for (const auto &text : {cp::export_var(P(2,3)), cp::export_var(vector<P>{{2,3}}), cp::export_var(optional<P>(P(2,3)))})
    assert(text.find("x= 2") != string::npos && text.find("y= 3") != string::npos);
  auto text = cp::export_var(C(P(2,3),5));
  assert(text.find("c=") != string::npos && text.find("r= 5") != string::npos);
  text = cp::export_var(S(P(2,3),P(4,5)));
  assert(text.find("a=") != string::npos && text.find("b=") != string::npos);
  text = cp::export_var(PQ(Q(1,2),Q(3,4)));
  assert(text.find("x= 1/2") != string::npos && text.find("y= 3/4") != string::npos);
  text = cp::export_var(intersection(L(P(),P(1,0)),L(P(),P(0,1))));
  assert(text.find("points=") != string::npos && text.find("IntersectionKind::Points") != string::npos);
#endif
}

void lines()
{
  vector<P> grid;
  for (ll x = -1; x <= 1; ++x) for (ll y = -1; y <= 1; ++y) grid.emplace_back(x,y);
  for (P a : grid) for (P b : grid) for (P c : grid) for (P d : grid)
  {
    S s(a,b), t(c,d);
    bool expected = crossing(s,t);
    assert(intersects(s,t) == expected);
    auto is = intersection(s,t);
    assert(!is.empty() == expected);
    assert(is.kind != IntersectionKind::Line && is.kind != IntersectionKind::Circle);
    for (auto p : is.points)
    { assert(contains(s.cast<Q>(),p)); assert(contains(t.cast<Q>(),p)); }
    assert((distance2(s,t) == Q(0)) == expected);
    assert(distance2(s,t) == distance2(t,s));
    if (a != b && c != d)
    {
      L l(a,b), m(c,d);
      auto il = intersection(l,m);
      assert(!il.empty() == intersects(l,m));
      for (auto p : il.points) assert(contains(l.cast<Q>(),p) && contains(m.cast<Q>(),p));
      auto ls = intersection(l,t);
      assert(!ls.empty() == intersects(l,t));
      for (auto p : ls.points) assert(contains(l.cast<Q>(),p) && contains(t.cast<Q>(),p));
    }
    auto foot = closest_point(s,c);
    assert(contains(s.cast<Q>(),foot));
    assert(distance2(s,c) == distance2(foot,PQ(c)));
    if (a != b)
    {
      L l(a,b);
      auto proj = projection(l,c), refl = reflection(l,c);
      assert(contains(l.cast<Q>(),proj));
      assert(dot(proj - PQ(c), PQ(l.vec())) == Q(0));
      assert(refl + PQ(c) == proj * Q(2));
      assert(distance2(l,c) == distance2(proj,PQ(c)));
      assert(near(distance(l,t), sqrt(as_ld(distance2(l,t)))));
    }
  }
  auto is = intersection(S(P(0,0),P(4,0)),S(P(2,0),P(6,0)));
  assert(is.kind == IntersectionKind::Segment && is.infinite() && is.points.size() == 2);
  assert(is.points[0] == PQ(Q(2),Q(0)) && is.points[1] == PQ(Q(4),Q(0)));
  auto line = intersection(L(P(),P(1,0)),L(P(3,0),P(2,0)));
  assert(line.kind == IntersectionKind::Line);
  // Fractional coordinates continue through exact geometry.
  Line<Q> a(PQ(Q(0),Q(1,2)),PQ(Q(1),Q(1,2)));
  Line<Q> b(PQ(Q(1,3),Q(0)),PQ(Q(1,3),Q(1)));
  auto rational_cross = intersection(a,b);
  assert(rational_cross.points[0] == PQ(Q(1,3),Q(1,2)));
}

void circles()
{
  C c(P(),5);
  assert(contains(c,P()) == Location::Inside && contains(c,P(3,4)) == Location::Boundary);
  assert(contains(c,P(6,0)) == Location::Outside);
  assert(intersection(c,c).kind == IntersectionKind::Circle);
  assert(intersection(C(P(),0),C(P(),0)).points.size() == 1);
  assert(tangents(C(P(),0),P()).infinite);
  assert(common_tangents(C(P(),0),C(P(),0)).infinite);
  assert(common_tangents(C(P(),0),C(P(1,0),0)).lines.size() == 1);
  assert(common_tangents(c,c).infinite);
  assert(common_tangents(c,C(P(),4)).lines.empty());
  assert(intersection(C(P(),0),L(P(-1,0),P(1,0))).points.size() == 1);
  assert(intersection(C(P(),0),S(P(),P())).points.size() == 1);
  assert(!intersects(C(P(),0),S(P(1,0),P(1,0))));
  for (ll dx = -12; dx <= 12; ++dx) for (ll dy = -3; dy <= 3; ++dy)
  for (ll r = 0; r <= 6; ++r)
  {
    C d(P(dx,dy),r);
    auto is = intersection(c,d);
    assert(!is.empty() == intersects(c,d));
    if (!is.infinite())
    {
      assert(is.points.size() <= 2);
      for (auto p : is.points)
      { assert(near(distance(p,Point<ld>()),5)); assert(near(distance(p,Point<ld>(dx,dy)),r)); }
    }
    auto ts = common_tangents(c,d);
    if (!ts.infinite) for (auto l : ts.lines)
    { assert(near(distance(l,Point<ld>()),5)); assert(near(distance(l,Point<ld>(dx,dy)),r)); }
    auto pt = tangents(c,P(dx,dy));
    for (auto l : pt.lines)
    { assert(contains(l,Point<ld>(dx,dy),1e-7)); assert(near(distance(l,Point<ld>()),5)); }
    assert(near(distance(c,d),distance(d,c)));
  }
  for (ll ax = -7; ax <= 7; ++ax) for (ll ay = -7; ay <= 7; ++ay)
  for (ll bx = -2; bx <= 2; ++bx) for (ll by = -2; by <= 2; ++by)
  {
    S s(P(ax,ay),P(bx,by));
    auto is = intersection(c,s);
    assert(!is.empty() == intersects(c,s));
    for (auto p : is.points)
    { assert(near(norm(p),5)); assert(contains(s.cast<ld>(),p,1e-7)); }
    if (s.a != s.b)
    {
      L l(s.a,s.b);
      auto il = intersection(c,l);
      assert(!il.empty() == intersects(c,l));
      for (auto p : il.points) { assert(near(norm(p),5)); assert(contains(l.cast<ld>(),p,1e-7)); }
    }
  }
}

void polygons()
{
  vector<P> square{{0,0},{4,0},{4,4},{0,4}};
  assert(area2(square) == 32 && area(square) == Q(16) && perimeter(square) == 16);
  assert(contains(square,P(2,2)) == Location::Inside);
  assert(contains(square,P(4,2)) == Location::Boundary);
  assert(contains(square,P(5,2)) == Location::Outside);
  assert(is_convex(square));
  auto cut = convex_cut(square,L(P(1,-1),P(1,5)));
  assert(area(cut) == Q(4));
  assert(convex_cut(vector<P>{},L()).empty());
  assert(contains(vector<P>{},P()) == Location::Outside);
  assert(contains(vector<P>{P()},P()) == Location::Boundary);
  assert(contains(vector<P>{P(),P(2,0)},P(1,0)) == Location::Boundary);
  assert(!closest_pair(vector<P>{}) && !closest_pair(vector<P>{P()}));
  assert(!convex_diameter(vector<P>{}));
  auto one = convex_diameter(vector<P>{P(3,4)});
  assert(one && one->first == one->second);
  vector<P> repeated{{0,0},{0,0},{1,0},{2,0},{2,1},{2,2},{1,2},{0,2},{0,1},{0,0}};
  auto diameter = convex_diameter(repeated);
  assert(diameter && distance2(diameter->first,diameter->second) == 8);
  mt19937_64 rng(20261030);
  for (int trial = 0; trial < 3000; ++trial)
  {
    int n = int(rng()%45);
    vector<P> p;
    for (int i = 0; i < n; ++i)
    {
      ll x = ll(rng()%21)-10, y = ll(rng()%21)-10;
      if (trial%7 == 0) y = 3*x+1;
      p.emplace_back(x,y);
    }
    auto near_pair = closest_pair(p);
    i128 minimum = i128(1)<<100, maximum = 0;
    for (int i = 0; i < n; ++i) for (int j = i+1; j < n; ++j)
    { auto d = distance2(p[i],p[j]); minimum = min(minimum,d); maximum = max(maximum,d); }
    assert(bool(near_pair) == (n >= 2));
    if (near_pair) assert(distance2(near_pair->first,near_pair->second) == minimum);
    for (bool keep : {false,true})
    {
      auto h = convex_hull(p,keep);
      assert(is_convex(h));
      assert(signed_area2(h) >= 0);
      for (auto q : p) assert(contains(h,q) != Location::Outside);
      // Every directed hull edge supports the full original point set.
      if (h.size() >= 3) for (size_t i = 0; i < h.size(); ++i)
        for (auto q : p) assert(determinant(h[i],h[(i+1)%h.size()],q) >= 0);
      auto far_pair = convex_diameter(h);
      assert(bool(far_pair) == (n > 0));
      if (far_pair) assert(distance2(far_pair->first,far_pair->second) == maximum);
      reverse(h.begin(),h.end());
      far_pair = convex_diameter(h);
      if (far_pair) assert(distance2(far_pair->first,far_pair->second) == maximum);
    }
  }
}

template <class T> void coordinate_type()
{
  using PT = Point<T>;
  using F = geometry_field_t<T>;
  const PT a(T(0),T(0)), b(T(4),T(0)), c(T(0),T(3));
  Line<T> l(a,b), m(a,c);
  Segment<T> s(a,c);
  auto p = projection(l,c);
  assert(p == Point<F>(F(0),F(0)));
  auto is = intersection(l,m);
  assert(is.kind == IntersectionKind::Points && is.points[0] == Point<F>(F(0),F(0)));
  assert(intersects(l,s) && perpendicular(l,s));
  assert(near(as_ld(distance2(l,c)),9));
  assert(near(as_ld(distance(s,b)),4));
  assert(near(as_ld(angle(b,c)),acos(-1.0L)/2,1e-6L));
  assert(near(as_ld(norm(PT(T(3),T(4)))),5));
  auto center = circumcenter(a,b,c);
  assert(center && near(as_ld(center->x),2) && near(as_ld(center->y),1.5));
  vector<PT> v{a,b,c,a};
  auto h = convex_hull(v);
  assert(h.size() == 3 && area(h) == F(6));
  assert(closest_pair(v) && convex_diameter(h));
  auto moved = move(h);
  auto copied = moved;
  assert(copied == moved);
  auto cut = convex_cut(copied,Line<T>(a,PT(T(1),T(1))));
  assert(!cut.empty());
  auto circle = Circle<T>(a,T(5));
  assert(intersection(circle,l).points.size() == 2);
  assert(intersection(circle, Circle<T>(PT(T(6),T(0)),T(5))).points.size() == 2);
  assert(common_tangents(circle, Circle<T>(PT(T(12),T(0)),T(5))).lines.size() == 4);
  PT mutable_point = b;
  assert(almost_equal(rotate(mutable_point,0),b.template cast<geometry_real_t<T>>()));
}

void fractional_points()
{
  mt19937_64 rng(123456789);
  for (int trial=0; trial<300; ++trial)
  {
    vector<P> integer;
    vector<PQ> rational;
    vector<Point<double>> real;
    for (int i=0; i<20; ++i)
    {
      P p(ll(rng()%201)-100,ll(rng()%201)-100);
      integer.push_back(p);
      rational.push_back(p/8LL);
      real.push_back(Point<double>(double(p.x)/8,double(p.y)/8));
    }
    auto a=closest_pair(integer);
    auto b=closest_pair(rational);
    auto c=closest_pair(real);
    assert(Q(distance2(a->first,a->second),64)==distance2(b->first,b->second));
    assert(near(as_ld(distance2(a->first,a->second))/64,distance2(c->first,c->second)));
    auto hi=convex_hull(integer);
    auto hq=convex_hull(rational);
    assert(hi.size()==hq.size());
    for (size_t i=0; i<hi.size(); ++i) assert(hi[i]/8LL==hq[i]);
    auto di=convex_diameter(hi);
    auto dq=convex_diameter(hq);
    assert(Q(distance2(di->first,di->second),64)==distance2(dq->first,dq->second));
  }
}

int main()
{
  basic(); lines(); circles(); polygons();
  coordinate_type<int>(); coordinate_type<ll>(); coordinate_type<i128>();
  coordinate_type<Rational<ll>>(); coordinate_type<Q>();
  coordinate_type<float>(); coordinate_type<double>(); coordinate_type<long double>(); coordinate_type<f128>();
  fractional_points();
  PRINT("Hello World");
}
