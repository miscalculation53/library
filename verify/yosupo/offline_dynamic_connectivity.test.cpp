#define PROBLEM "https://judge.yosupo.jp/problem/dynamic_graph_vertex_add_component_sum"

#define SINGLE_TESTCASE
// #define MULTI_TESTCASE
// #define AOJ_TESTCASE

#ifndef LOCAL
#define FAST_IO
// #define FAST_CIO
// #define INTERACTIVE
#endif

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "template/template_all_but_modint.hpp"

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

  // Obj: 1 u v は辺、2 v x は頂点への加算。
  using Obj = tlll;
  OfflineDynamicConnectivity<Obj> dc(Q);
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
      if (t == 0)
        dc.add(q, Obj{1, u, v});
      else if (t == 1)
        dc.erase_if(q, Obj{1, u, v});
      else if (t == 2)
        dc.add(q, Obj{2, u, v});
    }
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
  dc.run(add, undo, run);
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
