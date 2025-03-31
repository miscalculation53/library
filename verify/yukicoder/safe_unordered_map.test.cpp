#define PROBLEM "https://yukicoder.me/problems/no/8016"

#define SINGLE_TESTCASE
// #define MULTI_TESTCASE
// #define AOJ_TESTCASE

#ifndef LOCAL
#define FAST_IO
// #define FAST_CIO
// #define INTERACTIVE
#endif

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "template/template_all_but_modint.hpp"

#include "ds/safe_unordered_map.hpp"

void init() {}

void main2()
{
  LL(N, M);
  VEC(ll, N, A);
  VEC(ll, M, B);

  unordered_map<ll, ll, safe_hash> mp;
  fec(a : A) mp[a]++;

  vl C(M);
  rep(i, M) C.at(i) = mp[B.at(i)];
  PRINT(C);
}

void test()
{

}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
