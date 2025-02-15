#define PROBLEM "https://yukicoder.me/problems/no/1255"

// #define SINGLE_TESTCASE
#define MULTI_TESTCASE
// #define AOJ_TESTCASE

#define FAST_IO
// #define FAST_CIO
// #define INTERACTIVE

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "template/template_all.hpp"

#include "math/prime/order_primitive_root.hpp"

void init() {}

void main2()
{
  LL(N);
  if (N == 1)
    PRINT(1);
  else
    PRINT(order_mod(2, 2 * N - 1, factorize(carmichael(factorize(2 * N - 1)))));

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