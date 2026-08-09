#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/uf/uf.hpp"

// Test focus: edge metadata accumulation accepts a non-arithmetic string value.
int main()
{
  UnionFind<UFDataEverything<string>> uf(2);
  uf.merge(0, 1, string("x"));
  assert(uf.get_vdata(0).esum == "x");
  PRINT("Hello World");
}
