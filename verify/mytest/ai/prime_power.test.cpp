#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/prime/prime_power.hpp"

// Test focus: cross-type equality and widened cached-power multiplication avoid overflow.
int main()
{
  assert(PrimePower<int>(2, 3) == PrimePower<ll>(2, 3));
  assert(PrimePower<int>(2, 3) != PrimePower<ll>(2, 4));

  PrimePower<int> pp(65'537, 1, 65'537);
  pp.mul_p();
  assert(pp.e == 2 && uint(pp.pe) == uint(uint64_t(65'537) * 65'537));

  auto fac = factorized_mul(
      vc{PrimePower<int>(65'537, 1, 65'537)},
      vc{PrimePower<int>(65'537, 1, 65'537)});
  assert(fac.size() == 1 && fac[0].e == 2);
  assert(uint(fac[0].pe) == uint(uint64_t(65'537) * 65'537));

  const ll p = 4'294'967'291LL;
  PrimePower<ll> pp64(p, 1, p);
  pp64.mul_p();
  assert(ull(pp64.pe) == ull(u128(p) * p));
  PRINT("Hello World");
}
