#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_11_A&lang=jp"

#define SINGLE_TESTCASE
// #define MULTI_TESTCASE
// #define AOJ_TESTCASE

#define FAST_IO
// #define FAST_CIO
// #define INTERACTIVE

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "template/template_all_but_modint.hpp"

#include "ds/csr.hpp"

void init()
{
  oj(mt.seed(random_device()()));
}

void main2()
{ 
  LL(N);
  vvl vv(N);
  vpll ies;
  rep(i, N)
  {
    LL(u, k);
    u--;
    READVEC(k, vv.at(u));
    offset(vv.at(u), -1);
    fec(v : vv.at(i)) ies.push_back({u, v});
  }

  CSR csr1(vv), csr2(N, ies);
  assert(csr1.to_vv() == csr2.to_vv() && csr1.to_vv() == vv);
  dump(csr1.to_vv() | cp::index());
  vvl ans(N, vl(N, 0));
  rep(i, N)
  {
    fec(j : csr1.row(i)) ans.at(i).at(j) = 1;
  }
  PRINTV(ans);
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
