#define PROBLEM "https://judge.yosupo.jp/problem/dynamic_graph_vertex_add_component_sum"

#define SINGLE_TESTCASE
// #define MULTI_TESTCASE
// #define AOJ_TESTCASE

#define FAST_IO
// #define FAST_CIO
// #define INTERACTIVE

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "template/template_all.hpp"

#include "ds/uf/uf_undo.hpp"
#include "ds/offline_dynamic_connectivity.hpp"

vl A;
struct UFData
{
  struct VData
  {
    VData() {}
    ll vsum;
    VData(int i) { vsum = A.at(i); }
  };
  struct GData
  {
    GData() {}
    GData(int) {}
  };
  using EWeight = int;
  template <class UF>
  static void add_edge_diff(UF &uf, int x, int y, EWeight)
  {
    VData &xd = uf.vdat[x], &yd = uf.vdat[y];
    xd.vsum += yd.vsum;
  }
  template <class UF>
  static void add_edge_same(UF &, int, EWeight) {}
};

void init() {}

void main2()
{
  LL(N, Q);
  VEC(ll, N, Ain);
  A = Ain;

  vc<tlll> queries(Q);
  rep(q, Q)
  {
    LL(t);
    if (t == 3)
    {
      LL(v);
      queries.at(q) = {t, v, -1};
    }
    else
    {
      LL(u, v);
      if (t == 0 || t == 1) if (u > v)
          swap(u, v);
      queries.at(q) = {t, u, v};
    }
  }

  // Obj
  // 1 u v: u と v を結ぶ辺
  // 2 v x: v に重み x を加算
  using Obj = tlll;
  vc<pair<ll, Obj>> time_obj;
  rep(q, Q)
  {
    auto [t, u, v] = queries.at(q);
    if (t == 0 || t == 1)
      time_obj.eb(q, Obj{1, u, v});
  }
  auto span_obj = add_del_to_span(Q, time_obj);
  rep(q, Q)
  {
    auto [t, v, x] = queries.at(q);
    if (t == 2)
      span_obj.eb(q, Q, Obj{2, v, x});
  }

  UnionFindUndo<UFData> uf(N);
  auto add = [&](Obj obj)
  {
    auto [t, u, v] = obj;
    if (t == 1)
      uf.merge(u, v);
    else if (t == 2)
    {
      uf.merge(u, u);
      uf.get_vdata(u).vsum += v;
    }
  };
  auto undo = [&]()
  { uf.undo(); };
  auto run = [&](int q)
  {
    auto [t, v, _] = queries.at(q);
    if (t == 3)
      PRINT(uf.get_vdata(v).vsum);
  };
  offline_dynamic_connectivity(Q, span_obj, add, undo, run);
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