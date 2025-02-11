#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "../../itertools/bit.hpp"

void test1()
{
  const ll n = 6;
  vc<pll> sub1, sup1, sub2, sup2;
  rep(x, pow2(n)) rep(y, pow2(n))
  {
    if (bsubset(x, y))
      sub1.emplace_back(x, y);
    if (bsupset(y, x))
      sup1.emplace_back(x, y);
  }
  rep(y, pow2(n)) fec(x : bsubsets(y)) sub2.emplace_back(x, y);
  rep(x, pow2(n)) fec(y : bsupsets(n, x)) sup2.emplace_back(x, y);
  // dump(sub1 | cp::bin(n), sub2 | cp::bin(n));
  // dump(sup1 | cp::bin(n), sup2 | cp::bin(n));
  assert(sorted(sub1) == sorted(sub2));
  assert(sorted(sup1) == sorted(sup2));
}

int main()
{
  test1();

  cout << "Hello World" << endl;
}