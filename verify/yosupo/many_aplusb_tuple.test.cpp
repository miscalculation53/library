#define PROBLEM "https://judge.yosupo.jp/problem/many_aplusb"

// #define SINGLE_TESTCASE
#define MULTI_TESTCASE
// #define AOJ_TESTCASE

#ifndef LOCAL
#define FAST_IO
// #define FAST_CIO
// #define INTERACTIVE
#endif

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "template/template_all_but_modint.hpp"

void init() {}

void main2()
{
  ll r = rand() % 5;
  if (r == 0)
  {
    LL(A, B);
    PRINT(A + B);
  }
  else if (r == 1)
  {
    IN(pll, p);
    PRINT(p.first + p.second);
  }
  else if (r == 2)
  {
    using T = tuple<ll, ll>;
    IN(T, t);
    PRINT(get<0>(t) + get<1>(t));
  }
  else if (r == 3)
  {
    ARR(ll, 2, A);
    PRINT(A.at(0) + A.at(1));
  }
  else if (r == 4)
  {
    VEC(ll, 2, A);
    PRINT(A.at(0) + A.at(1));
  }
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
