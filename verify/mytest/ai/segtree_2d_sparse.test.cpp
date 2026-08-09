#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "ds/cumulative_sum/cumulative_sum.hpp"
#include "ds/fenwick_tree/fenwick_tree.hpp"
#include "ds/2d/sparse/segtree.hpp"

// Test focus: point-ID updates, duplicate coordinates, content, and empty input.
void test_segtree_2d_sparse_modify()
{
  using M = MonoidAdd<ll>;
  vc<tuple<ll, ll, ll>> init;
  vc<pair<ll, ll>> points;
  map<pair<ll, ll>, ll> a;
  repi(x, 12) repi(j, 7)
  {
    ll y = 13 * j + 5 * (x % 3) - 40;
    ll value = x - 2 * j;
    init.eb(x, y, value);
    points.eb(x, y);
    a[{x, y}] += value;
  }
  init.eb(3, -40, 17);
  points.eb(3, -40);
  a[{3, -40}] += 17;

  SegmentTree2DSparse<M, ll> seg(init);
  mt19937_64 rng(987654321);

  repi(iter, 2000)
  {
    int point_id = rng() % points.size();
    auto [x, y] = points[point_id];
    ll old = a[{x, y}];
    if (rng() & 1)
    {
      ll add = ll(rng() % 101) - 50;
      int called = 0;
      seg.modify(point_id, [&](ll &value)
      {
        called++;
        assert(value == old);
        value += add;
      });
      assert(called == 1);
      a[{x, y}] += add;
    }
    else
    {
      ll value = ll(rng() % 201) - 100;
      seg.set(point_id, value);
      a[{x, y}] = value;
    }
    assert((seg.get(point_id) == a[{x, y}]));

    ll lx = rng() % 13, rx = rng() % 13;
    if (lx > rx) swap(lx, rx);
    ll ly = ll(rng() % 121) - 60;
    ll ry = ll(rng() % 121) - 60;
    if (ly > ry) swap(ly, ry);
    ll expected = 0;
    for (auto [point, value] : a)
    {
      auto [px, py] = point;
      if (lx <= px && px < rx && ly <= py && py < ry) expected += value;
    }
    assert(seg.sum(lx, rx, ly, ry) == expected);
  }

  ll expected = 0;
  for (auto [point, value] : a) expected += value;
  assert(seg.all_sum() == expected);

  map<pair<ll, ll>, ll> got;
  for (auto [x, y, value] : seg.content()) got[{x, y}] = value;
  for (auto it = a.begin(); it != a.end();)
    if (it->second == 0)
      it = a.erase(it);
    else
      ++it;
  assert(got == a);

  SegmentTree2DSparse<M, ll> empty(vc<tuple<ll, ll, ll>>{});
  assert(empty.sum(-10, 10, -10, 10) == 0);
  assert(empty.all_sum() == 0);
  assert(empty.content().empty());
}

// Test focus: cumulative-sum and Fenwick inner structures satisfy the sum interface.
void test_segtree_2d_sparse_inner_sum()
{
  using G = GroupAddSub<int>;
  vc<tuple<int, int, int>> init{{1, 4, 3}, {2, 2, 5}, {5, 3, 7}};

  SegmentTree2DSparse<G, int, CumulativeSum> cs(init);
  assert(cs.sum(0, 3, 0, 5) == 8);
  assert(cs.sum(1, 0, 5) == 3);
  assert(cs.all_sum() == 15);

  SegmentTree2DSparse<G, int, FenwickTree> fw(init);
  assert(fw.sum(0, 6, 3, 5) == 10);
  fw.modify(1, [&](int &x) { x += 4; });
  assert(fw.sum(0, 3, 0, 5) == 12);
  assert(fw.all_sum() == 19);
}

int main()
{
  test_segtree_2d_sparse_modify();
  test_segtree_2d_sparse_inner_sum();
  cout << "Hello World" << endl;
}
