#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_4_A"
#define FAST_IO
#include "geometry/polygon.hpp"

int main()
{
  INT(n);
  vector<Point<ll>> p(n);
  READ(p);
  auto hull=convex_hull(p,true);
  auto first=min_element(hull.begin(),hull.end(),[](const auto &a,const auto &b)
      { return tie(a.y,a.x)<tie(b.y,b.x); });
  std::rotate(hull.begin(),first,hull.end());
  PRINT(hull.size());
  for(const auto &v:hull) PRINT(v);
}
