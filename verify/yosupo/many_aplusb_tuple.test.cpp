#define PROBLEM "https://judge.yosupo.jp/problem/many_aplusb"

// #define SINGLE_TESTCASE
#define MULTI_TESTCASE
// #define AOJ_TESTCASE

#define FAST_IO

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "../../template/template_all.hpp"

void init() {}

void main2()
{
  ll r = rand() % 5;
  if (r == 0)
  {
    LL(A, B);
    PRINT(A + B);
  }
  else if (r == 1)
  {
    IN(pll, p);
    PRINT(p.first + p.second);
  }
  else if (r == 2)
  {
    using T = tuple<ll, ll>;
    IN(T, t);
    PRINT(get<0>(t) + get<1>(t));
  }
  else if (r == 3)
  {
    ARR(ll, 2, A);
    PRINT(A.at(0) + A.at(1));
  }
  else if (r == 4)
  {
    VEC(ll, 2, A);
    PRINT(A.at(0) + A.at(1));
  }
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

  #if defined AOJ_TESTCASE or (not defined NOT_AOJ and defined LOCAL and defined SINGLE_TESTCASE)
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