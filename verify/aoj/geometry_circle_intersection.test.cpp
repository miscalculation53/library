#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_7_E"
#define FAST_IO
#include "geometry/circle.hpp"

int main()
{
  Circle<ll> a,b;
  READ(a,b);
  auto points=intersection(a,b).points;
  sort(points.begin(),points.end());
  PRINT(points.front(),points.back());
}
