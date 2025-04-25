#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=3184"

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

#include "math/set/kronecker_power.hpp"

void init()
{
  oj(mt.seed(random_device()()));
}

void main2()
{
  LL(Q, M);
  VEC(string, Q, S);
  fem(s : S) rep(i, M) s.at(i)--;
  vl A(Q);
  rep(i, Q) A.at(i) = stol(S.at(i), nullptr, 3);
  dump(S, A);

  vl f(ipow(3, M), INF);
  rep(i, Q) chmin(f.at(A.at(i)), i);
  auto g = zeta_supset_general<MonoidMin<ll>, 3>(f);
  dump(g | cp::index());
  string ans(Q, '0');
  rep(i, Q)
  {
    vl s = reversed(base_repr(A.at(i), 3, M));
    rep(j, M)
    {
      if (s.at(j) == 2)
        continue;
      ll na = A.at(i) + ipow(3, j);
      dump(s, A.at(i), na);
      if (g.at(na) < i)
        ans.at(i) = '1';
    }
  }
  PRINT(ans);
}

void test()
{

}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
