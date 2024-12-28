#define PROBLEM "https://judge.yosupo.jp/problem/static_range_frequency"

#define SINGLE_TESTCASE
// #define MULTI_TESTCASE
// #define AOJ_TESTCASE

#define FAST_IO

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "../../template/template_all.hpp"

#include "../../ds/coordinate_compression.hpp"
#include "../../ds/group_index.hpp"

void init() {}

void main2()
{
  LL(N, Q);
  VEC(ll, N, A);
  CoordinateCompression cc(A);
  GroupIndex grp(compressed(A));
  rep(val, cc.size()) dump(val, grp.idxs(val));
  rep(_, Q)
  {
    LL(l, r, x);
    auto v = cc.get_id(x);
    dump(v, l, r, grp.lt_cnt(v, r), grp.lt_cnt(v, l));
    PRINT(grp.in_cnt(v, l, r));
  }
  dump(grp.to_vv() | cp::index());
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