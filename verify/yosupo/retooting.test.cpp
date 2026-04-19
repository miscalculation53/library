#define PROBLEM "https://judge.yosupo.jp/problem/tree_path_composite_sum"

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
#include "graph/tree/rerooting.hpp"

struct M
{
  struct S
  {
    mint val;
    int siz;
    S() {}
    S(mint val, int siz) : val(val), siz(siz) {}
  };
  static constexpr S op(const S &a, const S &b)
  { return {a.val + b.val, a.siz + b.siz}; }
  static constexpr S e() { return {0, 0}; }
};
#ifdef LOCAL
CPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC(val, siz);
#endif

void init()
{
  oj(mt.seed(random_device()()));
}

void main2()
{
  LL(N);
  VEC(mint, N, A);
  VEC(tllll, N - 1, UVBC);
  UNZIP(UVBC, U, V, B, C);
  ZIP(UV, U, V);

  using S = typename M::S;
  auto pute = [&](S x, int e) -> S
  { return {B.at(e) * x.val + C.at(e) * x.siz, x.siz}; };
  auto putv = [&](S x, int v) -> S
  { return {x.val + A.at(v), x.siz + 1}; };
  auto res = rerooting<M>(N, UV, pute, putv);
  vc<mint> ans(N);
  rep(i, N) ans.at(i) = res.at(i).val;
  PRINT(ans);
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
