#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_2_B"
#define FAST_IO
#include "geometry/line.hpp"

int main()
{
  INT(q);
  while(q--)
  {
    Segment<ll> a,b;
    READ(a,b);
    PRINT(int(intersects(a,b)));
  }
}
