#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"
#include "geometry/geometry.hpp"

using P = Point<ll>;
using Q = Rational<i128>;
using PQ = Point<Q>;

template <class T> string show(const vector<T> &values)
{
  ostringstream out;
  for (const auto &value : values) out << '[' << value << ']';
  return out.str();
}

template <class T> vector<Point<T>> canonical(vector<Point<T>> p)
{
  p = convex_hull(p);
  reorder_polygon(p);
  return p;
}

void containment()
{
  assert(contains_convex(vector<P>{}, P()) == Location::Outside);
  assert(contains_convex(vector<P>{P()}, P()) == Location::Boundary);
  assert(contains_convex(vector<P>{P(),P()}, P(1,0)) == Location::Outside);
  mt19937_64 rng(0xC01A20261031ULL);
  for (int trial = 0; trial < 800; ++trial)
  {
    vector<P> points;
    for (int i = 0, n = int(rng()%30); i < n; ++i)
    {
      ll x = ll(rng()%13)-6, y = ll(rng()%13)-6;
      if (trial%5 == 0) y = 2*x+1;
      points.emplace_back(x,y);
    }
    for (bool keep : {false,true})
    {
      auto hull = convex_hull(points,keep);
      for (size_t rotation = 0; rotation < max(size_t(1),hull.size()); ++rotation)
      {
        for (ll x = -8; x <= 8; ++x) for (ll y = -8; y <= 8; ++y)
        {
          P q(x,y);
          auto got = contains_convex(hull,q), expected = contains(hull,q);
          if (got != expected)
            cerr << "containment trial=" << trial << " keep=" << keep << " rotation=" << rotation
                 << " q=" << q << " polygon=" << show(hull) << " got=" << int(got) << " expected=" << int(expected) << '\n';
          assert(got == expected);
        }
        if (!hull.empty()) rotate(hull.begin(),hull.begin()+1,hull.end());
      }
      if (!hull.empty())
      {
        auto closed = hull;
        closed.push_back(closed.front());
        for (auto q : points) assert(contains_convex(closed,q) != Location::Outside);
      }
    }
  }
  vector<P> line{{-5,0},{-2,0},{0,0},{3,0},{5,0}};
  for (int direction = 0; direction < 2; ++direction)
  {
    for (size_t i = 0; i < line.size(); ++i)
    {
      for (ll x = -6; x <= 6; ++x) for (ll y = -1; y <= 1; ++y)
        assert(contains_convex(line,P(x,y)) == contains(line,P(x,y)));
      rotate(line.begin(),line.begin()+1,line.end());
    }
    reverse(line.begin(),line.end());
  }
}

void reorder()
{
  vector<P> empty;
  reorder_polygon(empty);
  assert(empty.empty());
  vector<P> p{{3,1},{2,3},{0,1},{2,-1},{4,-1}};
  auto original = p;
  reorder_polygon(p);
  assert(p.front() == P(2,-1));
  assert((p == vector<P>{{2,-1},{4,-1},{3,1},{2,3},{0,1}}));
  for (size_t i = 0; i < p.size(); ++i)
  {
    rotate(original.begin(),original.begin()+1,original.end());
    auto copy = original;
    reorder_polygon(copy);
    assert(copy == p);
  }
}

void sums()
{
  assert(minkowski_sum(vector<P>{},vector<P>{P()}).empty());
  vector<P> large{{-4'000'000'000LL,-3'000'000'000LL},{4'000'000'000LL,-3'000'000'000LL},
                  {4'000'000'000LL,3'000'000'000LL},{-4'000'000'000LL,3'000'000'000LL}};
  assert(contains_convex(large,P()) == Location::Inside);
  assert(contains_convex(large,P(4'000'000'000LL,0)) == Location::Boundary);
  auto doubled = minkowski_sum(large,large);
  assert(doubled.size() == 4 && doubled[0] == P(-8'000'000'000LL,-6'000'000'000LL));
  mt19937_64 rng(0xA11C20261031ULL);
  for (int trial = 0; trial < 2000; ++trial)
  {
    auto make = [&]()
    {
      vector<P> p;
      for (int i = 0, n = int(rng()%18); i < n; ++i)
      {
        ll x = ll(rng()%15)-7, y = ll(rng()%15)-7;
        if (trial%7 == 0) y = 3*x-1;
        p.emplace_back(x,y);
      }
      p = convex_hull(p,bool(rng()%2));
      if (rng()%2) reverse(p.begin(),p.end());
      if (!p.empty())
      {
        rotate(p.begin(),p.begin()+rng()%p.size(),p.end());
        if (rng()%2) p.push_back(p.front());
        if (rng()%2) p.insert(p.begin(),p.front());
      }
      return p;
    };
    auto a = make(), b = make();
    vector<P> all;
    for (auto p : a) for (auto q : b) all.push_back(p+q);
    auto expected = canonical(all);
    auto before_a = a, before_b = b;
    assert(minkowski_sum(a,b) == expected);
    assert(minkowski_sum(b,a) == expected);
    assert(a == before_a && b == before_b);
    all.clear();
    for (auto p : a) for (auto q : a) all.push_back(p+q);
    assert(minkowski_sum(a,a) == canonical(all));
  }
}

vector<PQ> feasible_vertices(const vector<Line<ll>> &constraints)
{
  vector<PQ> candidates;
  for (size_t i = 0; i < constraints.size(); ++i) for (size_t j = i+1; j < constraints.size(); ++j)
  {
    auto cross = intersection(constraints[i],constraints[j]);
    if (cross.kind != IntersectionKind::Points) continue;
    auto p = cross.points[0];
    bool valid = true;
    for (const auto &l : constraints)
      if (orientation(PQ(l.a),PQ(l.b),p) < 0) { valid = false; break; }
    if (valid) candidates.push_back(p);
  }
  return canonical(candidates);
}

void clipping()
{
  const vector<P> box{{-5,-5},{5,-5},{5,5},{-5,5}};
  vector<Line<ll>> original;
  for (size_t i = 0; i < box.size(); ++i) original.emplace_back(box[i],box[(i+1)%box.size()]);
  mt19937_64 rng(0xC07C20261031ULL);
  for (int trial = 0; trial < 300; ++trial)
  {
    ConvexCut<ll> state(5);
    auto constraints = original;
    for (int step = 0; step < 12; ++step)
    {
      P a(ll(rng()%15)-7,ll(rng()%15)-7), b(ll(rng()%15)-7,ll(rng()%15)-7);
      if (a == b) b.x += 1;
      Line<ll> l(a,b);
      constraints.push_back(l);
      if (step%2) state.cut(l);
      else state.cut(a,b);
      auto expected = feasible_vertices(constraints);
      auto actual = state.nw_poly();
      if (actual != expected)
        cerr << "clipping trial=" << trial << " step=" << step << " actual=" << show(actual)
             << " expected=" << show(expected) << " constraints=" << show(constraints) << '\n';
      assert(actual == expected);
      assert(state.empty() == expected.empty() && state.size() == expected.size());
      auto copy = state;
      auto moved = move(copy);
      assert(moved.nw_poly() == expected);
      ConvexCut<ll> assigned(0);
      assigned = state;
      assigned.cut(P(100,0),P(100,1));
      assert(state.nw_poly() == expected);
    }
  }
  ConvexCut<ll> segment(vector<P>{{0,0},{4,0}});
  segment.cut(P(2,-1),P(2,1));
  assert((segment.nw_poly() == vector<PQ>{{Q(0),Q(0)},{Q(2),Q(0)}}));
  segment.cut(P(2,1),P(2,-1));
  assert((segment.nw_poly() == vector<PQ>{{Q(2),Q(0)}}));
  segment.cut(P(3,1),P(3,-1));
  assert(segment.empty());
  ConvexCut<ll> point(0);
  point.cut(P(),P(0,1));
  assert(point.nw_poly() == vector<PQ>{PQ()});
  point.cut(P(1,1),P(1,0));
  assert(point.empty());
  ConvexCut<ll> empty(vector<P>{});
  empty.cut(P(),P(1,0));
  assert(empty.empty());
  vector<P> clockwise{{0,0},{0,0},{0,4},{2,4},{4,4},{4,0},{0,0}};
  ConvexCut<ll> normalized(clockwise);
  auto initial = normalized.nw_poly();
  normalized.cut(P(1,-1),P(1,5));
  assert((normalized.nw_poly() == vector<PQ>{{Q(0),Q(0)},{Q(1),Q(0)},{Q(1),Q(4)},{Q(0),Q(4)}}));
  assert(initial.size() == 4 && clockwise.size() == 7);
  ConvexCut<double> precise(vector<Point<double>>{{0,0},{1e-7,0},{1e-7,1e-7},{0,1e-7}},0.0);
  assert(precise.size() == 4);
#ifdef LOCAL
  cp::options::es_style = cp::types::es_style_t::no_es;
  auto text = cp::export_var(ConvexCut<ll>(5));
  assert(text.find("content()") != string::npos && text.find("x=") != string::npos);
#endif
}

template <class T> void coordinate_type()
{
  using PT = Point<T>;
  vector<PT> p{{T(0),T(0)},{T(2),T(0)},{T(2),T(2)},{T(0),T(2)}};
  assert(contains_convex(p,PT(T(1),T(1))) == Location::Inside);
  assert(contains_convex(p,PT(T(2),T(1))) == Location::Boundary);
  auto sum = minkowski_sum(p,p);
  assert((sum == vector<PT>{{T(0),T(0)},{T(4),T(0)},{T(4),T(4)},{T(0),T(4)}}));
  ConvexCut<T> state(p);
  state.cut(PT(T(1),T(-1)),PT(T(1),T(3)));
  auto result = state.nw_poly();
  using F = geometry_field_t<T>;
  assert((result == vector<Point<F>>{{F(0),F(0)},{F(1),F(0)},{F(1),F(2)},{F(0),F(2)}}));
  auto q = PT(T(1),T(1));
  const auto &constant = p;
  assert(contains_convex(constant,q) == Location::Inside);
  vector<PT> translated;
  for (auto v : p) translated.push_back(v + PT(T(1),T(1)));
  auto before = translated;
  reorder_polygon(translated);
  assert(translated == before);
}

int main()
{
  reorder(); containment(); sums(); clipping();
  coordinate_type<int>(); coordinate_type<ll>(); coordinate_type<i128>();
  coordinate_type<Rational<ll>>(); coordinate_type<Q>();
  coordinate_type<float>(); coordinate_type<double>(); coordinate_type<long double>(); coordinate_type<f128>();
  ConvexCut deduced(vector<P>{{0,0},{2,0},{0,2}});
  static_assert(is_same_v<decltype(deduced),ConvexCut<ll>>);
  PRINT("Hello World");
}
