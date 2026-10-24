#define PROBLEM "https://judge.yosupo.jp/problem/two_edge_connected_components"

#define SINGLE_TESTCASE
#ifndef LOCAL
#define FAST_IO
#endif

#include "graph/two_edge_connected_components.hpp"
#include "ds/group_index.hpp"

void init() {}
void main2()
{
  INT(n, m);
  VEC(pll, m, es);
  const auto ids = two_edge_connected_component_ids<int>(GraphUndirected<>(n, es));
  const GroupIndexRaw groups(ids);
  PRINT(groups.size());
  for (auto [id, vs] : groups) PRINT(vs.size(), vs.to_v());
}
void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
