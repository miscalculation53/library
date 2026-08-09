#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_algo.hpp"

// Test focus: direction tables have fixed types and order, and empty ranges are
// accepted by rotate and adjacent-difference helpers.
int main()
{
  static_assert(is_same_v<remove_cv_t<decltype(DRULgrid)>, array<pll, 4>>);
  assert((DRULgrid == array<pll, 4>{{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}}));
  assert((DRULplane == array<pll, 4>{{{0, -1}, {1, 0}, {0, 1}, {-1, 0}}}));

  vc<int> empty;
  rotate(empty, 10);
  assert(empty.empty());
  assert((adjd(empty) == vc<int>{0}));
  assert(adjd(empty, 1, 0).empty());
  cout << "Hello World" << endl;
}
