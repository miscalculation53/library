#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/uf/uf_potential.hpp"

// Test focus: chained potentials and accumulated edge metadata stay consistent.
int main()
{
  UnionFindPotentialEverything<GroupAddSub<ll>> uf(3);
  uf.merge(0, 1, 5);
  uf.merge(1, 2, 7);
  assert(uf.diff(0, 2) == 12);
  assert(uf.get_vdata(0).esum == 12);
  PRINT("Hello World");
}
