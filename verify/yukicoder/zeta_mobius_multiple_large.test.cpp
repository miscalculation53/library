#define PROBLEM "https://yukicoder.me/problems/no/2578"

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

#include "math/prime/large/zeta_mobius_divisor_multiple_large.hpp"
#include "math/algebra/algebra_basic_ops.hpp"

#include "math/modint/modint.hpp"
using mint = modint998244353;

void init() {}

void main2()
{
  LL(T, M);
  ZetaMobiusDivisorMultipleLarge zm(M);
  rep(_, T)
  {
    LL(N, B, C, D);
    VEC(ll, N, A);
    vc<mint> W(N);
    W.at(0) = B;
    rep(i, 1, N) W.at(i) = C * W.at(i - 1) + D;

    auto h = zm.divisor_map<mint>([&](ll)
                                  { return 1; });
    rep(i, N)
    {
      if (M % A.at(i) != 0)
        continue;
      h.get_by_d(M / A.at(i)) *= 1 + W.at(i);
    }
    dump(h.to_map());
    auto g = zm.zeta_multiple<MonoidMul<mint>>(h);
    dump(g.to_map());

    auto f = zm.mobius_multiple<GroupAddSub<mint>>(g);
    dump(f.to_map());
    mint ans = f.get_by_d(1);
    if (M == 1)
      ans--;
    PRINT(ans);

    mint ans2 = zm.mobius_multiple_point<GroupAddSub<mint>>(g, 1);
    if (M == 1)
      ans2--;
    dump(ans2);
    assert(ans == ans2);
  }
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
