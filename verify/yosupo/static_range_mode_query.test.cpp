#define PROBLEM "https://judge.yosupo.jp/problem/static_range_mode_query"

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

#include "ds/coordinate_compression.hpp"
#include "ds/mo/mo_rollback.hpp"
#include "ds/undo.hpp"

void init() {}

void main2()
{
  LL(N, Q);
  VEC(ll, N, A);
  VEC(pll, Q, LR);

  CoordinateCompression cc(A);
  auto B = compressed(A);

  VectorUndo<ll> cnt(N);
  ValRollback<ll> mx = 0, argmx = 0;

  vpll ans(Q);

  auto add = [&](int i, bool)
  {
    ll b = B[i];
    cnt.set(b, cnt.get(b) + 1);
    if (mx.get() < cnt.get(b))
    {
      mx.set(cnt.get(b));
      argmx.set(b);
    }
  };
  auto snapshot = [&]()
  {
    cnt.snapshot();
    mx.snapshot();
    argmx.snapshot();
  };
  auto rollback = [&]()
  {
    cnt.rollback();
    mx.rollback();
    argmx.rollback();
  };
  auto reset = [&]()
  {
    cnt.reset();
    mx.set(0);
    argmx.set(0);
  };
  auto rem = [&](ll q)
  {
    ans[q] = {cc.get_val(argmx.get()), mx.get()};
  };

  mo_rollback(N, LR, add, snapshot, rollback, reset, rem);
  PRINTV(ans);
}

void test()
{

}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
