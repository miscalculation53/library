#define PROBLEM "https://judge.yosupo.jp/problem/predecessor_problem"

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
  LL(N, Q);
  STR(T);
  set<ll> st;
  rep(i, N) if (T.at(i) == '1') st.insert(i);
  rep(_, Q)
  {
    LL(c, k);
    if (c == 0)
      st.insert(k);
    else if (c == 1)
      st.erase(k);
    else if (c == 2)
      PRINT(st.find(k) != st.end());
    else if (c == 3)
    {
      auto it = geq_min(st, k);
      if (it == st.end())
        PRINT(-1);
      else
        PRINT(*it);
    }
    else if (c == 4)
    {
      auto it = leq_max(st, k);
      if (it == st.end())
        PRINT(-1);
      else
        PRINT(*it);
    }
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