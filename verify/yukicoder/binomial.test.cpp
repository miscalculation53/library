#define PROBLEM "https://yukicoder.me/problems/no/117"

// #define SINGLE_TESTCASE
#define MULTI_TESTCASE
// #define AOJ_TESTCASE

// #define FAST_IO
#define FAST_CIO
// #define INTERACTIVE

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "template/template_all_but_modint.hpp"

#include "math/modint/modint.hpp"
// using mint = modint998244353;
using mint = modint1000000007;
// using mint = static_modint32<1000000000>;
// using mint = modint;
#include "math/modint/binomial.hpp"
using bi = Binomial<mint>;

void init() {}

void main2()
{
  CHAR(t);
  char _;
  READ(_);
  LL(N);
  READ(_);
  LL(K);
  READ(_);
  if (t == 'P')
    PRINT(bi::P(N, K));
  else if (t == 'C')
    PRINT(bi::C(N, K));
  else if (t == 'H')
    PRINT(bi::H(N, K));
}

void test()
{

}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
