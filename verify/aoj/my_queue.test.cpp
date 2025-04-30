#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_3_B"

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

#include "ds/my_queue.hpp"

void init()
{
  oj(mt.seed(random_device()()));
}

void main2()
{
  LL(N, X);
  using P = pair<string, ll>;
  VEC(P, N, ST);
  MyQueue<P> que;
  fec(st : ST) que.push(st);
  vc<P> ans;
  ll cur = 0;
  while (!que.empty())
  {
    auto [s, t] = que.front();
    que.pop();
    ll dt = min(X, t);
    t -= dt;
    cur += dt;
    if (t == 0)
      ans.eb(s, cur);
    else
      que.push({s, t});
    dump(que.content(), cur);
  }
  PRINTV(ans);
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
