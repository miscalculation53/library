#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/fenwick_tree/range_add_point_get.hpp"

struct X
{
  int v;
  friend bool operator==(X a, X b) { return a.v == b.v; }
};
struct GX
{
  using S = X;
  static S op(S a, S b) { return {a.v + b.v}; }
  static S e() { return {0}; }
  static S inv(S a) { return {-a.v}; }
};

// Test focus: set/get work for an abstract group value without arithmetic operators.
int main()
{
  RangeAddPointGet<GX> fw(vc<X>{{1}, {2}, {3}});
  fw.set(1, {10});
  assert(fw.get(0) == X{1});
  assert(fw.get(1) == X{10});
  assert(fw.get(2) == X{3});
  PRINT("Hello World");
}
