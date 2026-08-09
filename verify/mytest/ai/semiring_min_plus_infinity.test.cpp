#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/algebra/algebra_basic_ops.hpp"

// Test focus: an explicitly supplied finite infinity reaches both semiring identities.
int main()
{
  using SR = SemiRingMinPlus<int, 123>;
  assert(SR::e0() == 123);
  static_assert(MonoidMin<int, 123>::e() == 123);
  PRINT("Hello World");
}
