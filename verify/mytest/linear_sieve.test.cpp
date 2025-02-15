#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "../../math/prime/linear_sieve.hpp"
#include "../../math/prime/factorize.hpp"

void test1()
{
  const ll N = 10000;
  rep(n, 1, N + 1)
  {
    vc<PrimePower<ll>> fac1 = LinearSieve::factorize<ll>(n);
    vc<PrimePower<ll>> fac2 = factorize(n);
    assert(fac1 == fac2);
  }
}

int main()
{
  test1();

  PRINT("Hello World");
}