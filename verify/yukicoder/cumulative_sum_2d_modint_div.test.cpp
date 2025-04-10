#define PROBLEM "https://yukicoder.me/problems/no/1141"

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

#include "math/modint/modint.hpp"
using mint = modint1000000007;
#include "math/modint/modint_div.hpp"
using mintd = ModintDiv<mint>;
#include "ds/cumulative_sum/cumulative_sum_2d.hpp"

void init() {}

void main2()
{
  LL(H, W);
  VEC2(ll, H, W, A);
  vvc<mintd> B(H, vc<mintd>(W));
  rep(i, H) rep(j, W) B.at(i).at(j) = mintd(A.at(i).at(j));
  CumulativeSum2D<GroupMulDiv<mintd>> cum(B);
  LL(Q);
  rep(_, Q)
  {
    LL(r, c);
    r--, c--;
    dump(cum.sum(H, W));
    dump(cum.sum(0, H, c, c + 1));
    dump(cum.sum(r, r + 1, 0, W));
    dump(B.at(r).at(c));
    mintd ans = cum.sum(H, W) / cum.sum(0, H, c, c + 1) / cum.sum(r, r + 1, 0, W) * B.at(r).at(c);
    auto [ok, anss] = ans.to_modint();
    PRINT(anss);
  }
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
