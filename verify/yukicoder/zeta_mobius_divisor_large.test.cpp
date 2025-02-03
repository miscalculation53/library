#define PROBLEM "https://yukicoder.me/problems/no/2578"

#define SINGLE_TESTCASE
// #define MULTI_TESTCASE
// #define AOJ_TESTCASE

#define FAST_IO

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "../../template/template_all.hpp"

#include "../../math/prime/zeta_mobius_divisor_multiple_large.hpp"
#include "../../math/algebraic_struct.hpp"

#include "../../math/modint/modint.hpp"
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
      h.get_by_d(A.at(i)) *= 1 + W.at(i);
    }
    dump(h.to_map());
    auto g = zm.zeta_divisor<MonoidMul<mint>>(h);
    dump(g.to_map());
    
    auto f = zm.mobius_divisor<GroupAddSub<mint>>(g);
    dump(f.to_map());
    mint ans = f.get_by_d(M);
    if (M == 1)
      ans--;
    PRINT(ans);

    mint ans2 = zm.mobius_divisor_point<GroupAddSub<mint>>(g, M);
    if (M == 1)
      ans2--;
    dump(ans2);
    assert(ans == ans2);
  }
}

void test() {}

int main()
{
  cauto CERR = [](cauto &val)
  {
    #ifdef LOCAL
    cerr << val;
    #endif
  };

  #if defined FAST_IO and not defined LOCAL
  CERR("\033[33m \n[FAST_IO]\n\n \033[m");
  cin.tie(0);
  ios::sync_with_stdio(false);
  #endif
  cout << fixed << setprecision(20);

  test();
  init();

  #if defined AOJ_TESTCASE or (not defined NOT_AOJ and defined LOCAL and defined SINGLE_TESTCASE)
  CERR("\033[35m \n[AOJ_TESTCASE]\n\n \033[m");
  while (true)
  {
    dump("new testcase");
    main2();
  }
  #elif defined SINGLE_TESTCASE
  CERR("\033[36m \n[SINGLE_TESTCASE]\n\n \033[m");
  main2();
  #elif defined MULTI_TESTCASE
  CERR("\033[32m \n[MULTI_TESTCASE]\n\n \033[m");
  uint T;
  cin >> T;
  while (T--)
  {
    dump("new testcase");
    main2();
  }
  #endif
}