#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"
#include "geometry/geometry.hpp"

// Run with an empty input to keep this verification program self-contained.
// The dedicated local run also injects these tokens into the FAST_IO buffer.
int main()
{
  using Q = Rational<ll>;
#if defined FAST_IO && !defined LOCAL
  string input = "1 2 3 4  0 0 2 3  0 1 2 1  2 3 5  1/2 -3/4  0 0 4/3 2/3  1/3 2/3 5/3";
  copy(input.begin(),input.end(),fastio::ibuf);
  fastio::pir = uint32_t(input.size());
  vector<Point<ll>> p(2);
  Segment<ll> s;
  Line<ll> l;
  Circle<ll> c;
  Point<Q> q;
  Segment<Q> sq;
  Circle<Q> cq;
  READ(p,s,l,c,q,sq,cq);
  assert(p[0] == Point<ll>(1,2) && p[1] == Point<ll>(3,4));
  assert(s.a == Point<ll>() && s.b == Point<ll>(2,3));
  assert(l.a == Point<ll>(0,1) && l.b == Point<ll>(2,1));
  assert(c.c == Point<ll>(2,3) && c.r == 5);
  assert(q == Point<Q>(Q(1,2),Q(-3,4)));
  assert(sq.b == Point<Q>(Q(4,3),Q(2,3)) && cq.r == Q(5,3));
  PRINT(p,s,l,c,q,sq,cq);
  const string expected = "1 2 3 4 0 0 2 3 0 1 2 1 2 3 5 1/2 -3/4 0/1 0/1 4/3 2/3 1/3 2/3 5/3\n";
  assert(string(fastio::obuf,fastio::por) == expected);
  fastio::por = 0;
  vector<Point<f128>> fp(1);
  fp[0] = Point<f128>(f128(1)/3,f128(2)/3);
  PRINT(fp);
  assert(string(fastio::obuf,fastio::por).find("0.333333333333333333333333333333") != string::npos);
  fastio::por = 0;
#endif
  PRINT("Hello World");
}
