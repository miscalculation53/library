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
  assert(!uf.merge(0, 2, 13));
  assert(!uf.valid(1));
  for (int n : {3, 1, 7, 0, 7})
  {
    uf.reset(n);
    assert(uf.gdat.cmp_cnt == n);
    repi(i, n)
    {
      assert(uf.valid(i) && uf.size(i) == 1);
      assert(uf.get_vdata(i).vsum == 1 && uf.get_vdata(i).esum == 0);
      assert(uf.diff(i, i) == 0);
    }
    if (n > 1)
    {
      assert(uf.merge(0, n - 1, 19));
      assert(uf.diff(0, n - 1) == 19);
      assert(uf.get_vdata(n - 1).esum == 19);
      assert(!uf.merge(0, n - 1, 20));
    }
  }
  PRINT("Hello World");
}
