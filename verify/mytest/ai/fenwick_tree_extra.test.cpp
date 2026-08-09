#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/fenwick_tree/fenwick_tree.hpp"

struct Box
{
  int x;
};

struct BoxGroup
{
  using S = Box;
  static S op(S a, S b) { return {a.x + b.x}; }
  static S e() { return {0}; }
  static S inv(S a) { return {-a.x}; }
};

// Test focus: FenwickTree accepts group values without arithmetic operators, and
// multiset searches do not narrow a large query value to the index type.
int main()
{
  FenwickTree<BoxGroup> boxes(vc<Box>{{2}, {3}, {5}});
  assert(boxes.sum(3).x == 10);
  assert(boxes.sum(1, 3).x == 8);

  FenwickTree<GroupAddSub<ll>> fw(3);
  fw.add(1, 1);
  assert((fw.leq_max_in_multiset<ll, int>(1LL << 40) == 0));
  assert((fw.gt_min_in_multiset<ll, int>(1LL << 40) == 1));

  PRINT("Hello World");
}
