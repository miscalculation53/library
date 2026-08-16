#define PROBLEM "https://judge.yosupo.jp/problem/queue_operate_all_composite"

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
using mint = modint998244353;

#include "ds/swag.hpp"
#include "algebra/affine_function.hpp"

void init()
{
  oj(mt.seed(random_device()()));
}

void main2()
{
  FoldableQueue<OppositeMonoid<GroupAffineFunction<mint>>> que;
  LL(Q);
  rep(_, Q)
  {
    LL(t);
    if (t == 0)
    {
      IN(mint, a, b);
      que.push({a, b});
    }
    else if (t == 1)
    {
      que.pop();
    }
    else if (t == 2)
    {
      IN(mint, x);
      auto [a, b] = que.all_prod();
      PRINT(a * x + b);
    }
  }
}

void test()
{

}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
