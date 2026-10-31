#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/fenwick_tree/fenwick_tree.hpp"

struct Box
{
  int x;
};

struct BoxGroup
{
  using S = Box;
  inline static int operations = 0;
  static S op(S a, S b) { operations++; return {a.x + b.x}; }
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

  mt19937 rng(20261022);
  for (int n : {0, 1, 2, 3, 7, 8, 9, 31, 32, 33, 127, 128, 129})
  {
    vc<Box> values(n);
    for (auto &v : values) v.x = int(rng() % 101) - 50;
    FenwickTree<BoxGroup> tree(values);
    repi(step, 500)
    {
      if (n > 0)
      {
        int p = rng() % n, x = int(rng() % 101) - 50;
        if (step & 1) tree.add(p, {x}), values[p].x += x;
        else tree.set(p, {x}), values[p].x = x;
        assert(tree.get(p).x == values[p].x);
      }
      int l = rng() % (n + 1), r = rng() % (n + 1);
      if (l > r) swap(l, r);
      int expected = 0;
      repi(i, l, r) expected += values[i].x;
      assert(tree.sum(l, r).x == expected);
      BoxGroup::operations = 0;
      auto restored = tree.content();
      assert(BoxGroup::operations <= n);
      repi(i, n) assert(restored[i].x == values[i].x);
    }
    repi(l, n + 1)
    {
      int expected = 0;
      repi(r, l, n + 1)
      {
        assert(tree.sum(l, r).x == expected);
        if (r < n) expected += values[r].x;
      }
    }
  }

  FenwickTree<MonoidMin<int>> mins(vc<int>{8, 3, 6});
  mins.add(2, 1);
  assert(mins.sum(0) == MonoidMin<int>::e() && mins.sum(2) == 3 && mins.sum(3) == 1);

  FenwickTree<GroupAddSub<ll>> fw(3);
  fw.add(1, 1);
  assert((fw.leq_max_in_multiset<ll, int>(1LL << 40) == 0));
  assert((fw.gt_min_in_multiset<ll, int>(1LL << 40) == fw.size_of_multiset()));

  PRINT("Hello World");
}
