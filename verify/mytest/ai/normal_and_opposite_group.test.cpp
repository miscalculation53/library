#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/algebra/algebra_base.hpp"
#include "math/algebra/algebra_basic_ops.hpp"

// Test focus: inversion swaps/negates both stored directions to produce the identity.
int main()
{
  using G = NormalAndOppositeGroup<GroupAddSub<int>>;
  G::S a(3, 5);
  auto b = G::inv(a);
  auto e = G::op(a, b);
  assert(e.normal == 0 && e.opposite == 0);
  PRINT("Hello World");
}
