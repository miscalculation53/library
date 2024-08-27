#define PROBLEM "https://judge.yosupo.jp/problem/many_aplusb_128bit"

// #define SINGLE_TESTCASE
#define MULTI_TESTCASE
// #define AOJ_TESTCASE

#define FAST_IO

#include "../../template/template_all.hpp"

void init() {}

void main2()
{
  i128 A, B;
  cin >> A >> B;
  dump(A, B);
  PRINT(A + B);
}

void test()
{
  /*
  #ifdef LOCAL
  rep(t, 100000)
  {
    dump(t);

    // ----- generate cases -----
    ll N = 1 + rand() % 5;
    vl A(N);
    rep(i, N) A.at(i) = 1 + rand() % 10;
    // --------------------------

    // ------ check output ------
    auto god = naive(A);
    auto ans = solve(A);
    if (god != ans)
    {
      dump(N, A);
      dump(god, ans);
      exit(0);
    }
    // --------------------------
  }
  dump("ok");
  #endif
  //*/
}

int main()
{
  #if defined FAST_IO and not defined LOCAL
    cerr << "[FAST_IO]\n\n";
    cin.tie(0);
    ios::sync_with_stdio(false);
  #endif
  cout << fixed << setprecision(20);

  init();
  test();

  #if defined AOJ_TESTCASE or (defined LOCAL and defined SINGLE_TESTCASE)
    cerr << "[AOJ_TESTCASE]\n\n";
    while (true) main2();
  #elif defined SINGLE_TESTCASE
    cerr << "[SINGLE_TESTCASE]\n\n";
    main2();
  #elif defined MULTI_TESTCASE
    cerr << "[MULTI_TESTCASE]\n\n";
    int T;
    cin >> T;
    while (T--) main2();
  #endif
}