#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"
#include "geometry/geometry.hpp"
#include <boost/multiprecision/cpp_int.hpp>

using B = boost::multiprecision::cpp_int;
using P = Point<ll>;

int main()
{
  mt19937_64 rng(0x6E0A20261030ULL);
  auto value = [&]() { return ll(rng()%2'000'000'000'001ULL)-1'000'000'000'000LL; };
  for (int trial=0; trial<10000; ++trial)
  {
    P a(value(),value()), b(value(),value()), c(value(),value());
    B ax(a.x), ay(a.y), bx(b.x), by(b.y), cx(c.x), cy(c.y);
    assert(B(dot(a,b)) == ax*bx+ay*by);
    assert(B(cross(a,b,c)) == (bx-ax)*(cy-ay)-(by-ay)*(cx-ax));
    assert(B(distance2(a,b)) == (bx-ax)*(bx-ax)+(by-ay)*(by-ay));
  }
  auto small = [&]() { return ll(rng()%2'000'001)-1'000'000; };
  for (int trial=0; trial<3000; ++trial)
  {
    P a(small(),small()), b(small(),small()), c(small(),small()), d(small(),small());
    if (a==b || c==d) continue;
    Line<ll> l(a,b), m(c,d);
    B A1=B(b.y)-a.y, B1=B(a.x)-b.x, C1=B(b.x)*a.y-B(a.x)*b.y;
    B A2=B(d.y)-c.y, B2=B(c.x)-d.x, C2=B(d.x)*c.y-B(c.x)*d.y;
    B det=A1*B2-A2*B1;
    auto is=intersection(l,m);
    if (det==0) continue;
    assert(is.kind==IntersectionKind::Points);
    auto p=is.points[0];
    assert(B(p.x.num)*det==(B1*C2-B2*C1)*B(p.x.den));
    assert(B(p.y.num)*det==(C1*A2-C2*A1)*B(p.y.den));
    Circle<ll> o(c,ll(rng()%1'000'001));
    B dx=B(b.x)-a.x, dy=B(b.y)-a.y;
    B co=dx*(B(c.y)-a.y)-dy*(B(c.x)-a.x);
    bool expected=co*co<=B(o.r)*o.r*(dx*dx+dy*dy);
    assert(intersects(o,l)==expected);
  }
  PRINT("Hello World");
}
