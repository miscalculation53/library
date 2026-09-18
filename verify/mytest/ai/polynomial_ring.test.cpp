#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "algebra/algebra_basic_ops.hpp"
#include "algebra/polynomial_ring.hpp"
#include "math/modint/modint.hpp"

// Test focus: multiplying by the canonical empty polynomial returns canonical zero.
void test()
{
  using R = PolynomialRingVector<RingAddSubMul<ll>>;
  const R::S zero = R::e0();
  const R::S f = {1, 2, 3};

  assert(R::mul(zero, f).empty());
  assert(R::mul(f, zero).empty());
  assert(R::mul(zero, zero).empty());
  assert(R::add(f, R::minus(f)) == zero);
  assert(R::add({0, 0}, {0}) == zero);
  assert(R::minus({0, 0}) == zero);
  assert(R::mul({0, 0}, f) == zero);
  assert(R::mul(f, {0, 0}) == zero);
  assert((R::add({1, 2, 0}, {0, -2, 0, 0}) == R::S{1}));
  assert((R::mul({1, 2, 0}, {1, -2, 0}) == R::S{1, 0, -4}));

  using Z6 = PolynomialRingVector<RingAddSubMul<static_modint32<6>>>;
  assert(Z6::mul({2}, {3}) == Z6::e0());
  // 遅延剰余版の積でも、相殺した高次の係数を残さない。
  using M = PolynomialRingVector<RingAddSubMul<modint998244353>>;
  M::S a(20), b(20);
  a[0] = b[0] = 1;
  assert((M::mul(a, b) == M::S{1}));
}

int main()
{
  test();

  cout << "Hello World" << endl;
}
