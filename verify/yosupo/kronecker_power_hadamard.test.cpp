#define PROBLEM "https://judge.yosupo.jp/problem/bitwise_xor_convolution"

#define SINGLE_TESTCASE
// #define MULTI_TESTCASE
// #define AOJ_TESTCASE

#define FAST_IO
// #define FAST_CIO
// #define INTERACTIVE

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "template/template_all.hpp"

#include "math/modint/modint.hpp"
using mint = modint998244353;

#include "math/set/kronecker_power.hpp"

void hadamard(vc<mint> &A)
{
  kronecker_power_destructive<RingAddSubMul<mint>, 2>({{{1, 1}, {1, -1}}}, A);
}

void init() {}

void main2()
{
  LL(N);
  VEC(mint, 1 << N, A, B);
  hadamard(A), hadamard(B);
  dump(A, B);
  rep(i, 1 << N) A.at(i) *= B.at(i);
  hadamard(A);
  mint div = mint(1 << N).inv();
  rep(i, 1 << N) A.at(i) *= div;
  PRINT(A);
}

void test()
{
  /*
  local(
    rep(testcase, 100000)
    {
      cout << endl;
      dump(testcase);


      // ----- generate cases -----
      ll N = 1 + rand() % 5;
      vl A(N);
      rep(i, N) A.at(i) = 1 + rand() % 10;
      // --------------------------

      // ------ check output ------
      #define INPUT A
      auto god = naive(INPUT);
      auto ans = solve(INPUT);
      if (god != ans)
      {
        dump(INPUT);
        dump(god, ans);
        exit(0);
      }
      // --------------------------
    }
    dump("ok");
  );
  //*/
}

int main()
{
  cauto CERR = [](string val, string color)
  {
    string s = "\033[" + color + "m" + val + "\033[m";
    #ifdef LOCAL
    cerr << s;
    #endif
    /* コードテストで確認する際にコメントアウトを外す
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