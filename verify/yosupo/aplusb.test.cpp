#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#define SINGLE_TESTCASE
// #define MULTI_TESTCASE
// #define AOJ_TESTCASE

#define FAST_IO

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "../../template/template_all.hpp"

void init() {}

void main2()
{
  LL(A, B);
  PRINT(A + B);
}

void test() {}

int main()
{
  cauto CERR = [](cauto &val)
  {
    #ifndef BOJ
      cerr << val;
    #endif
  };

  #if defined FAST_IO and not defined LOCAL
    CERR("[FAST_IO]\n\n");
    cin.tie(0);
    ios::sync_with_stdio(false);
  #endif
  cout << fixed << setprecision(20);

  test();
  init();

  #if defined AOJ_TESTCASE or (defined LOCAL and defined SINGLE_TESTCASE)
    CERR("[AOJ_TESTCASE]\n\n");
    while (true)
    {
      dump("new testcase");
      main2();
    }
  #elif defined SINGLE_TESTCASE
    CERR("[SINGLE_TESTCASE]\n\n");
    main2();
  #elif defined MULTI_TESTCASE
    CERR("[MULTI_TESTCASE]\n\n");
    int T;
    cin >> T;
    while (T--)
    {
      dump("new testcase");
      main2();
    }
  #endif
}