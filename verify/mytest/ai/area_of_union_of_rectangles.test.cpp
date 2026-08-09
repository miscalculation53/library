#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "ds/2d/offline/area_of_union_of_rectangles.hpp"

int main()
{
  {
    AreaOfUnionOfRectangles<ll> area;
    // Test focus: an empty input, which is outside the Yosupo problem constraints.
    assert(area.run() == 0);
  }
  {
    AreaOfUnionOfRectangles<ll> area;
    // Test focus: degenerate rectangles must not change the union area.
    area.rectangle_add(-2, -2, -3, 4);
    area.rectangle_add(-2, 5, 1, 1);
    area.rectangle_add(-2, 1, -3, 2);
    assert(area.run() == 15);
  }
  cout << "Hello World" << endl;
}
