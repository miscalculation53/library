#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=0560"

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

#include "ds/cumulative_sum/cumulative_sum_2d.hpp"

void init() {}

void main2()
{
  LL(M, N, K);
  VEC(string, M, S);
  auto A = gen_vec(M, [&](ll i)
                   { return stov(S.at(i), "JOI"); });
  array<vvl, 3> B;
  B.fill(vvl(M, vl(N, 0)));
  rep(i, M) rep(j, N) B.at(A.at(i).at(j)).at(i).at(j) = 1;
  array<CumulativeSum2D<>, 3> cums;
  rep(k, 3) cums.at(k) = CumulativeSum2D(B.at(k));
  rep(_, K)
  {
    LL(li, lj, ri, rj);
    li--, lj--;
    array<ll, 3> ans;
    rep(k, 3) ans.at(k) = cums.at(k).sum(li, ri, lj, rj);
    PRINT(ans);
  }
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
