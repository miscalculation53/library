#define PROBLEM "https://yukicoder.me/problems/no/2578"

#define SINGLE_TESTCASE
// #define MULTI_TESTCASE
// #define AOJ_TESTCASE

#define FAST_IO
// #define FAST_CIO
// #define INTERACTIVE

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "template/template_all.hpp"

#include "math/prime/zeta_mobius_divisor_multiple_large.hpp"
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

int main()
{
  cauto CERR = [](string val, string color)
  {
    string s = "\033[" + color + "m" + val + "\033[m";
    #ifdef LOCAL
    cerr << s;
    #endif
    /* コードテストで確認する際にコメントアウトする
    cerr << val;
    //*/
  };

  #if defined FAST_IO and not defined LOCAL
  CERR("\n[FAST_IO]\n\n", "32");
  #endif
  #if defined FAST_CIO and not defined LOCAL
  CERR("\n[FAST_CIO]\n\n", "32");
  cin.tie(0);
  ios::sync_with_stdio(false);
  #endif
  cout << fixed << setprecision(20);

  test();
  init();

  #if defined AOJ_TESTCASE or (defined LOCAL and defined SINGLE_TESTCASE)
  CERR("\n[AOJ_TESTCASE]\n\n", "35");
  while (true)
  {
    dump("new testcase");
    main2();
  }
  #elif defined SINGLE_TESTCASE
  CERR("\n[SINGLE_TESTCASE]\n\n", "36");
  main2();
  #elif defined MULTI_TESTCASE
  CERR("\n[MULTI_TESTCASE]\n\n", "33");
  dump("T");
  IN(uint, T);
  while (T--)
  {
    dump("new testcase");
    main2();
  }
  #endif
}