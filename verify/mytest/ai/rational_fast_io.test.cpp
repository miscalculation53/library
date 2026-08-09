#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#define FAST_IO

#include "template/template_all_but_modint.hpp"
#include "math/modint/modint.hpp"
#include "math/rational.hpp"

// Test focus: FAST_IO overload resolution compiles for integer and modint rationals.
int main()
{
  if (false)
  {
    PRINT(Rational<ll>(2, 4));
    PRINT(Rational<modint998244353>(2, 4));
  }
  PRINT("Hello World");
}
