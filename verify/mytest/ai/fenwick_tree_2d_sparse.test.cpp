#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "ds/cumulative_sum/cumulative_sum.hpp"
#include "ds/segtree/segtree.hpp"
#include "ds/2d/sparse/fenwick_tree.hpp"

// Test focus: duplicate point IDs, large coordinates, rectangle sums, and content reconstruction.
void test_fenwick_tree_2d_sparse_point_id()
{
  vc<pair<ll, ll>> points;
  repi(i, 40)
    points.eb(1'000'000'000LL + 97 * i, -2'000'000'000LL + 131 * (i % 17));
  points.eb(points[7]);
  points.eb(points[19]);

  FenwickTree2DSparse<GroupAddSub<ll>, ll> fw(points);

  map<pair<ll, ll>, ll> a;
  mt19937_64 rng(1618033988);
  repi(iter, 5000)
  {
    int point_id = rng() % points.size();
    ll value = ll(rng() % 201) - 100;
    fw.add(point_id, value);
    a[points[point_id]] += value;

    ll lx = 999'999'900LL + rng() % 4100;
    ll rx = 999'999'900LL + rng() % 4100;
    if (lx > rx) swap(lx, rx);
    ll ly = -2'000'000'100LL + rng() % 2400;
    ll ry = -2'000'000'100LL + rng() % 2400;
    if (ly > ry) swap(ly, ry);
    ll expected = 0;
    for (auto [point, weight] : a)
    {
      auto [x, y] = point;
      if (lx <= x && x < rx && ly <= y && y < ry) expected += weight;
    }
    assert(fw.sum(lx, rx, ly, ry) == expected);
  }

  map<pair<ll, ll>, ll> got;
  for (auto [x, y, value] : fw.content()) got[{x, y}] = value;
  for (auto it = a.begin(); it != a.end();)
    if (it->second == 0)
      it = a.erase(it);
    else
      ++it;
  assert(got == a);
}

// Test focus: prefix sums only require a commutative monoid, not inverses.
void test_fenwick_tree_2d_sparse_monoid_prefix()
{
  vc<pair<int, int>> points{{2, 3}, {5, 1}, {2, 3}};
  FenwickTree2DSparse<MonoidAdd<int>, int> fw(points);
  fw.add(0, 4);
  fw.add(1, 7);
  fw.add(2, 9);
  assert(fw.sum(3, 4) == 13);
  assert(fw.sum(6, 4) == 20);
}

// Test focus: all query forms are defined for an empty point set.
void test_fenwick_tree_2d_sparse_empty()
{
  FenwickTree2DSparse<GroupAddSub<int>, int> fw(vc<pair<int, int>>{});
  assert(fw.sum(10, 10) == 0);
  assert(fw.sum(-10, 10, -10, 10) == 0);
  assert(fw.content().empty());
}

// Test focus: SegmentTree can be selected as the inner data structure.
void test_fenwick_tree_2d_sparse_inner_segment_tree()
{
  vc<tuple<int, int, int>> init{{1, 3, 4}, {4, 2, 7}, {1, 3, 5}};
  FenwickTree2DSparse<MonoidAdd<int>, int, SegmentTree> fw(init);
  assert(fw.sum(5, 4) == 16);
  assert(fw.sum(5, 2, 4) == 16);
  fw.add(0, 6);
  assert(fw.sum(2, 0, 4) == 15);
}

// Test focus: immutable initial data can use CumulativeSum as the inner structure.
void test_fenwick_tree_2d_sparse_inner_cumulative_sum()
{
  vc<tuple<int, int, int>> init{{1, 2, 3}, {3, 1, 4}, {1, 2, 5}, {5, 4, -2}};
  FenwickTree2DSparse<GroupAddSub<int>, int, CumulativeSum> fw(init);
  assert(fw.sum(4, 3) == 12);
  assert(fw.sum(0, 4, 0, 3) == 12);
  assert(fw.sum(0, 6, 0, 5) == 10);
}

int main()
{
  test_fenwick_tree_2d_sparse_point_id();
  test_fenwick_tree_2d_sparse_monoid_prefix();
  test_fenwick_tree_2d_sparse_empty();
  test_fenwick_tree_2d_sparse_inner_segment_tree();
  test_fenwick_tree_2d_sparse_inner_cumulative_sum();
  cout << "Hello World" << endl;
}
