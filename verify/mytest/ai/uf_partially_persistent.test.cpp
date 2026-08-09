#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/uf/uf_partially_persistent.hpp"

// Test focus: metadata queries before, at, and after a merge use the requested time.
int main()
{
  UnionFindPartiallyPersistent<UFDataEverything<>> uf(3);
  assert(uf.get_vdata(-10, 1).vsum == 1);
  assert(uf.get_gdata(-10).cmp_cnt == 3);
  uf.merge(0, 1);
  assert(uf.get_vdata(0, 0).vsum == 1);
  assert(uf.get_vdata(1, 1).vsum == 2);
  assert(uf.get_vdata(100, 1).vsum == 2);
  assert(uf.get_gdata(100).cmp_cnt == 2);
  assert((uf.group_ids<int>() == vc<int>{0, 0, 1}));
  PRINT("Hello World");
}
