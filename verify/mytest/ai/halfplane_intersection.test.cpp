#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "convex/halfplane_intersection.hpp"

// Test focus: bounded, unbounded, and empty intersections; redundant boundaries and IDs;
// exact rational vertices, randomized clipping, and different integer coefficient types.
using HPI = HalfPlaneIntersection<ll>;
using Status = HPI::Status;

long double real(const HPI::Fraction &x)
{
  return static_cast<long double>(x.num) / static_cast<long double>(x.den);
}

bool near(long double x, long double y, long double eps = 1e-9L)
{
  return abs(x - y) <= eps;
}

long double polygon_cross_sum(const vc<HPI::Point> &ps)
{
  long double res = 0;
  repi(i, ps.size())
  {
    const auto &p = ps[i], &q = ps[(i + 1) % ps.size()];
    res += real(p.x) * real(q.y) - real(p.y) * real(q.x);
  }
  return res;
}

long double polygon_area(const vc<HPI::Point> &ps) { return abs(polygon_cross_sum(ps)) / 2; }

void test_bounded_and_id()
{
  HPI hpi;
  hpi.add(1, 0, -3);
  hpi.add(0, 1, -5, 42);
  hpi.add(-1, 0, -2);
  hpi.add(0, -1, -4);
  auto res = hpi.intersection();
  assert(res.status == Status::Bounded);
  assert(res.boundaries.size() == 4);
  assert(res.vertices.size() == 4);
  assert(polygon_cross_sum(res.vertices) > 0);
  assert(near(polygon_area(res.vertices), 45));

  set<int> ids;
  for (auto h : res.boundaries) ids.insert(h.id);
  assert(ids == set<int>({0, 2, 3, 42}));
  for (auto p : res.vertices)
  {
    assert(real(p.x) <= 3 + 1e-9L && real(p.x) >= -2 - 1e-9L);
    assert(real(p.y) <= 5 + 1e-9L && real(p.y) >= -4 - 1e-9L);
  }

  hpi.clear();
  hpi.add(1, 0, 0);
  assert(hpi.intersection().boundaries[0].id == 0);
}

void test_unbounded()
{
  {
    HPI hpi;
    auto res = hpi.intersection();
    assert(res.status == Status::Unbounded);
    assert(res.boundaries.empty() && res.vertices.empty());
  }
  {
    HPI hpi;
    hpi.add(1, 0, -1, 10);
    auto res = hpi.intersection();
    assert(res.status == Status::Unbounded);
    assert(res.boundaries.size() == 1 && res.boundaries[0].id == 10);
    assert(res.vertices.empty());
  }
  {
    HPI hpi;
    hpi.add(-1, 0, 0, 10);
    hpi.add(0, -1, 0, 20);
    auto res = hpi.intersection();
    assert(res.status == Status::Unbounded);
    assert(res.boundaries.size() == 2);
    assert(res.vertices.size() == 1);
    assert(res.vertices[0].x == HPI::Fraction(0));
    assert(res.vertices[0].y == HPI::Fraction(0));
  }
  {
    HPI hpi;
    hpi.add(-1, 0, 0);
    hpi.add(1, 0, -1);
    auto res = hpi.intersection();
    assert(res.status == Status::Unbounded);
    assert(res.boundaries.size() == 2);
    assert(res.vertices.empty());
  }
}

void test_empty_and_redundant()
{
  {
    HPI hpi;
    hpi.add(1, 0, 0);
    hpi.add(-1, 0, 1);
    assert(hpi.intersection().status == Status::Empty);
  }
  {
    HPI hpi;
    hpi.add(0, 0, -1);
    assert(hpi.intersection().status == Status::Unbounded);
    hpi.add(0, 0, 1);
    assert(hpi.intersection().status == Status::Empty);
  }
  {
    HPI hpi;
    hpi.add(1, 0, -5, 10);
    hpi.add(1, 0, -1, 20);
    auto res = hpi.intersection();
    assert(res.status == Status::Unbounded);
    assert(res.boundaries.size() == 1);
    assert(res.boundaries[0].id == 20);
  }
  {
    HPI hpi;
    hpi.add(-1, 0, 0);
    hpi.add(1, 0, -1);
    hpi.add(0, -1, 0);
    hpi.add(0, 1, 0);
    assert(hpi.intersection().status == Status::Empty);
  }
  {
    HPI hpi;
    hpi.add(-1, 0, 0);
    hpi.add(1, 0, 0);
    hpi.add(0, -1, 0);
    hpi.add(0, 1, 0);
    assert(hpi.intersection().status == Status::Empty);
  }
}

void test_rational_vertices()
{
  HPI hpi;
  hpi.add(-1, 0, 0);
  hpi.add(0, -1, 0);
  hpi.add(2, 2, -1);
  auto res = hpi.intersection();
  assert(res.status == Status::Bounded);
  assert(res.vertices.size() == 3);
  set<pair<HPI::Fraction, HPI::Fraction>> vertices;
  for (const auto &p : res.vertices) vertices.emplace(p.x, p.y);
  assert((vertices == set<pair<HPI::Fraction, HPI::Fraction>>{
                          {{0}, {0}}, {{0}, {1, 2}}, {{1, 2}, {0}}}));
  assert(near(polygon_area(res.vertices), 0.125L));
}

struct NaivePoint
{
  long double x, y;
};

vc<NaivePoint> clip(vc<NaivePoint> ps, ll a, ll b, ll c)
{
  vc<NaivePoint> res;
  repi(i, ps.size())
  {
    NaivePoint p = ps[i], q = ps[(i + 1) % ps.size()];
    long double pv = a * p.x + b * p.y + c;
    long double qv = a * q.x + b * q.y + c;
    bool pin = pv <= 1e-12L, qin = qv <= 1e-12L;
    if (pin) res.eb(p);
    if (pin != qin)
    {
      long double t = pv / (pv - qv);
      res.eb(NaivePoint{p.x + (q.x - p.x) * t,
                        p.y + (q.y - p.y) * t});
    }
  }
  return res;
}

long double naive_area(const vc<NaivePoint> &ps)
{
  long double res = 0;
  repi(i, ps.size())
  {
    const auto &p = ps[i], &q = ps[(i + 1) % ps.size()];
    res += p.x * q.y - p.y * q.x;
  }
  return abs(res) / 2;
}

void test_random()
{
  constexpr ll B = 1000;
  mt19937 rng(1234567);
  repi(_, 2000)
  {
    HPI hpi(B);
    vc<tuple<ll, ll, ll>> hs;
    int n = rng() % 16;
    repi(i, n)
    {
      ll a = int(rng() % 11) - 5, b = int(rng() % 11) - 5;
      if (a == 0 && b == 0) a = 1;
      ll c = int(rng() % 41) - 20;
      hs.eb(a, b, c);
      hpi.add(a, b, c, i);
    }

    vc<NaivePoint> ps = {{-B, -B}, {B, -B}, {B, B}, {-B, B}};
    for (auto [a, b, c] : hs) ps = clip(std::move(ps), a, b, c);
    long double area = naive_area(ps);
    auto res = hpi.intersection();
    if (ps.size() < 3 || area <= 1e-8L)
    {
      assert(res.status == Status::Empty);
      continue;
    }

    bool touches_box = false;
    for (auto p : ps)
      touches_box |= abs(abs(p.x) - B) <= 1e-8L || abs(abs(p.y) - B) <= 1e-8L;
    assert(res.status == (touches_box ? Status::Unbounded : Status::Bounded));
    if (!touches_box)
    {
      assert(polygon_cross_sum(res.vertices) > 0);
      assert(near(polygon_area(res.vertices), area, 1e-7L));
    }
    for (auto p : res.vertices)
      for (auto [a, b, c] : hs)
        assert(a * real(p.x) + b * real(p.y) + c <= 1e-7L);
  }
}

void test_int_explicit_infty()
{
  HalfPlaneIntersection<int> hpi(1'000'000'000);
  assert(hpi.intersection().status == HalfPlaneIntersection<int>::Status::Unbounded);
}

int main()
{
  test_bounded_and_id();
  test_unbounded();
  test_empty_and_redundant();
  test_rational_vertices();
  test_random();
  test_int_explicit_infty();
  cout << "Hello World" << endl;
}
