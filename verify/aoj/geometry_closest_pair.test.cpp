#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_5_A"
#define FAST_IO
#include "geometry/point_set.hpp"

int main()
{
  INT(n);
  vector<Point<ld>> p(n);
  READ(p);
  auto pair=closest_pair(p);
  PRINT(distance(pair->first,pair->second));
}
