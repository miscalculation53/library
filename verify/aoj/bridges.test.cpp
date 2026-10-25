#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_3_B"

#define SINGLE_TESTCASE
#ifndef LOCAL
#define FAST_IO
#endif

#include "graph/lowlink.hpp"

void init() {}
void main2()
{
  INT(n, m);
  VEC(pll, m, es);
  const LowLink lowlink(GraphUndirected<>(n, es));
  vc<pll> bridges;
  for (int eid : lowlink.bridges()) bridges.eb(minmax(es[eid].first, es[eid].second));
  sort(ALL(bridges));
  for (auto [u, v] : bridges) PRINT(u, v);
}
void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
