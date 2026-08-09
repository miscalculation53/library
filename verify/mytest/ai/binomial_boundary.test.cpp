#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/modint/binomial.hpp"

// Test focus: arguments crossing the modulus and signed inverse normalization.
int main()
{
  using mint = static_modint32<7>;
  assert(Binomial<mint>::C(8, 2) == 0);
  assert(Binomial<mint>::P(8, 2) == 0);
  assert(Binomial<mint>::inv(8) == 1);
  assert(Binomial<mint>::inv(-1) == -1);
  PRINT("Hello World");
}
