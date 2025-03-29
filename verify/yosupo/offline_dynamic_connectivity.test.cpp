#define PROBLEM "https://judge.yosupo.jp/problem/dynamic_graph_vertex_add_component_sum"

#define SINGLE_TESTCASE
// #define MULTI_TESTCASE
// #define AOJ_TESTCASE

#define FAST_IO
// #define FAST_CIO
// #define INTERACTIVE

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
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
