#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2995"

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
#include "graph/tree/dsu_on_tree.hpp"

void init()
{
  oj(mt.seed(random_device()()));
}

template <class EWeight_ = ll, bool need_vlist = false>
struct UFData
{
  struct VData
  {
    VData() {}

    ll vsum;
    ll esum;
    vc<ll> vlist;

    // 頂点 i の初期化
    VData(int i)
    {
      vsum = 1;
      esum = 0;
      if constexpr (need_vlist)
        vlist = {i};
    }
  };
  struct GData
  {
    GData() {}

    ll cmp_cnt;
    ll min_leader, max_leader;
    ll ans;

    // 頂点数 n のグラフの初期化
    GData(int n)
    {
      cmp_cnt = n;
      min_leader = 0, max_leader = n - 1;
      ans = 0;
    }
  };
  using EWeight = EWeight_;
  template <class UF>
  static void add_edge_diff(UF &uf, int x, int y, EWeight w)
  {
    VData &xd = uf.vdat[x], &yd = uf.vdat[y];
    GData &gd = uf.gdat;

    gd.ans -= min(xd.vsum, xd.esum);
    gd.ans -= min(yd.vsum, yd.esum);

    xd.vsum += yd.vsum;
    xd.esum += yd.esum + w;
    if constexpr (need_vlist)
    {
      xd.vlist.insert(xd.vlist.end(), ALL(yd.vlist));
      yd.vlist.clear();
    }
    gd.cmp_cnt--;
    while (uf.leader(gd.min_leader) != gd.min_leader)
      gd.min_leader++;
    while (uf.leader(gd.max_leader) != gd.max_leader)
      gd.max_leader--;

    gd.ans += min(xd.vsum, xd.esum);
  }
  template <class UF>
  static void add_edge_same(UF &uf, int x, EWeight w)
  {
    VData &xd = uf.vdat[x];
    GData &gd = uf.gdat;

    gd.ans -= min(xd.vsum, xd.esum);

    xd.esum += w;

    gd.ans += min(xd.vsum, xd.esum);
  }
};

void main2()
{
  LL(N, K);
  VEC(pll, N - 1, UV);
  offset(UV, pll{-1, -1});
  VEC(pll, N, CD);
  offset(CD, pll{-1, -1});
  UNZIP(CD, C, D);

  RootedTree T(N, UV, 0);
  UnionFindUndo<UFData<>> uf(K);
  vl ans(N);
  auto add = [&](ll v)
  { uf.merge(C[v], D[v]); };
  auto reset = [&](ll)
  { uf.reset(); };
  auto rem = [&](ll v)
  { ans[v] = uf.gdat.ans; };

  dsu_on_tree(T, add, reset, rem);
  PRINTV(ans);
}

void test()
{
  
}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
