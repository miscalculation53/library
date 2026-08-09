#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "math/algebra/algebra_basic_ops.hpp"
#include "math/algebra/polynomial_ring.hpp"

// Test focus: multiplying by the canonical empty polynomial returns canonical zero.
void test()
{
  using R = PolynomialRingVector<RingAddSubMul<ll>>;
  const R::S zero = R::e0();
  const R::S f = {1, 2, 3};

  assert(R::mul(zero, f).empty());
  assert(R::mul(f, zero).empty());
  assert(R::mul(zero, zero).empty());
}

int main()
{
  test();

  cout << "Hello World" << endl;
}
