#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "ds/2d/offline/point_add_rectangle_sum.hpp"

// Test focus: empty execution and point_add_fn composition beyond the Yosupo interface.
int main()
{
  {
    PointAddRectangleSum<GroupAddSub<ll>, ll> rs;
    assert(rs.run().empty());
  }
  {
    PointAddRectangleSum<GroupAddSub<ll>, ll> rs;
    rs.point_add(0, 0, 2);
    rs.rectangle_sum(-1, 1, -1, 1);
    rs.point_add_fn(2, 2, 0, [](ll x) { return 3 * x; });
    rs.rectangle_sum(-3, 3, -3, 3);
    rs.point_add_fn(-2, -2, 1, [](ll x) { return x + 1; });
    rs.rectangle_sum(-3, 3, -3, 3);
    assert((rs.run() == vc<ll>{2, 8, 17}));
  }
  cout << "Hello World" << endl;
}
