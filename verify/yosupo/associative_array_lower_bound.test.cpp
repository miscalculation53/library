#define PROBLEM "https://judge.yosupo.jp/problem/associative_array"

#define SINGLE_TESTCASE
// #define MULTI_TESTCASE
// #define AOJ_TESTCASE

#define FAST_IO

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "../../template/template_all.hpp"

#include "../../ds/coordinate_compression.hpp"

void init() {}

void main2()
{
  LL(Q);
  vc<tlll> queries(Q);
  vc<ll> keys;
  rep(i, Q)
  {
    LL(t);
    if (t == 0)
    {
      LL(k, v);
      queries.at(i) = {t, k, v};
      keys.push_back(k);
    }
    else if (t == 1)
    {
      LL(k);
      queries.at(i) = {t, k, -1};
    }
  }
  CoordinateCompression cc(keys);
  vc<ll> vals(cc.size());
  fec([t, k, v] : queries)
  {
    if (t == 0)
    {
      ll i = cc.get_id(k);
      vals.at(i) = v;
    }
    else if (t == 1)
    {
      ll i = cc.get_id(k);
      if (i == -1)
        PRINT(0);
      else
        PRINT(vals.at(i));
    }
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