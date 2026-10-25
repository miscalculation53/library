#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/segtree/segtree_range.hpp"

#if __cplusplus >= 202002L
static_assert(ranges::input_range<decltype(SegmentTreeRange(0).range_to_nodes_from_left(0, 0))>);
static_assert(ranges::input_range<decltype(SegmentTreeRange(0).range_to_nodes_from_bottom(0, 0))>);
static_assert(ranges::input_range<decltype(SegmentTreeRange(1).point_to_nodes_from_bottom(0))>);
static_assert(ranges::input_range<decltype(SegmentTreeRange(1).point_to_nodes_from_top(0))>);
#endif

// 区間を二分する素朴な探索で、左からの最小分解を求める。
vl reference_nodes(ll s, ll l, ll r)
{
  vl res;
  auto dfs = [&](auto &&self, ll i, ll a, ll b) -> void
  {
    if (l == r || b <= l || r <= a) return;
    if (l <= a && b <= r)
    {
      res.push_back(i);
      return;
    }
    const ll c = a + (b - a) / 2;
    self(self, 2 * i, a, c);
    self(self, 2 * i + 1, c, b);
  };
  dfs(dfs, 1, 0, s);
  return res;
}

template <class Range>
void check_range(const Range &range, const vl &expected)
{
  assert(range.empty() == expected.empty());
  assert(range.to_v() == expected);
  assert(vl(range.begin(), range.end()) == expected);
  const auto copy = range;
  assert(copy.to_v() == expected);
  assert(copy.begin() == range.begin());
  auto it = range.begin(), independent = it;
  for (ll i : expected)
  {
    assert(it != range.end());
    auto old = it++;
    assert(*old == i);
    assert(*independent == i);
    ++independent;
    assert(it == independent);
  }
  assert(it == range.end());
  for (ll i : range)
  {
    for (ll j : range)
    {
      assert(j == expected.front());
      break;
    }
    assert(i == expected.front());
    break;
  }
  assert(range.to_v() == expected);
}

void check_small()
{
  for (int n = 0; n <= 65; ++n)
  {
    const SegmentTreeRange seg(n);
    ll s = 1;
    while (s < n) s *= 2;
    assert(seg.size() == n && seg.leaf_size() == s && seg.node_count() == 2 * s - 1);
    vc<pair<ll, ll>> segments(2 * s);
    segments[1] = {0, s};
    for (ll i = 1; i < 2 * s; ++i)
    {
      auto [a, b] = segments[i];
      assert(seg.node_to_range(i) == segments[i]);
      assert(seg.range_to_node(a, b) == i);
      assert(seg.range_length(i) == b - a);
      assert(seg.is_leaf(i) == (b - a == 1));
      int depth = 0;
      for (ll k = s; k > b - a; k /= 2) ++depth;
      assert(seg.depth(i) == depth);
      if (i < s)
      {
        ll c = (a + b) / 2;
        segments[2 * i] = {a, c};
        segments[2 * i + 1] = {c, b};
      }
      vl ancestors;
      for (ll j = 1; j <= i; ++j)
      {
        auto [l, r] = segments[j];
        if (l <= a && b <= r) ancestors.push_back(j);
      }
      check_range(seg.ancestors_from_top(i), ancestors);
      reverse(ancestors.begin(), ancestors.end());
      check_range(seg.ancestors_from_bottom(i), ancestors);
    }
    for (int p = 0; p < n; ++p)
    {
      ll i = seg.point_to_node(p);
      assert(i == s + p && seg.node_to_point(i) == p);
      vl expected;
      for (ll j = 1; j < 2 * s; ++j)
      {
        auto [l, r] = segments[j];
        if (l <= p && p < r) expected.push_back(j);
      }
      check_range(seg.point_to_nodes_from_top(p), expected);
      reverse(expected.begin(), expected.end());
      check_range(seg.point_to_nodes_from_bottom(p), expected);
    }
    for (int l = 0; l <= n; ++l) for (int r = l; r <= n; ++r)
    {
      vl expected = reference_nodes(s, l, r);
      check_range(seg.range_to_nodes_from_left(l, r), expected);
      ll next = l;
      for (ll i : expected)
      {
        auto [a, b] = segments[i];
        assert(a == next);
        next = b;
      }
      assert(next == r);
      sort(expected.begin(), expected.end(), [&](ll a, ll b)
      {
        auto [al, ar] = segments[a];
        auto [bl, br] = segments[b];
        return make_pair(ar - al, al) < make_pair(br - bl, bl);
      });
      check_range(seg.range_to_nodes_from_bottom(l, r), expected);
    }
  }
}

void check_lifetime()
{
  const auto left = SegmentTreeRange(5).range_to_nodes_from_left(1, 5);
  const auto bottom = SegmentTreeRange(5).range_to_nodes_from_bottom(1, 5);
  check_range(left, {9, 5, 12});
  check_range(bottom, {9, 12, 5});
  auto li = SegmentTreeRange(5).range_to_nodes_from_left(1, 5).begin();
  auto bi = SegmentTreeRange(5).range_to_nodes_from_bottom(1, 5).begin();
  auto up = SegmentTreeRange(5).point_to_nodes_from_bottom(3).begin();
  auto down = SegmentTreeRange(5).point_to_nodes_from_top(3).begin();
  assert((vl(li, {}) == vl{9, 5, 12}));
  assert((vl(bi, {}) == vl{9, 12, 5}));
  assert((vl(up, {}) == vl{11, 5, 2, 1}));
  assert((vl(down, {}) == vl{1, 2, 5, 11}));
}

// 点更新と、非可換な区間積を組み合わせた利用例。
void check_point_update_range_concat()
{
  const int n = 19;
  const SegmentTreeRange seg(n);
  vc<string> dat(seg.node_count() + 1);
  string naive(n, ' ');
  for (int p = 0; p < n; ++p) dat[seg.point_to_node(p)] = ' ';
  for (ll i = seg.leaf_size() - 1; i > 0; --i) dat[i] = dat[2 * i] + dat[2 * i + 1];
  mt19937 rng(20261003);
  for (int step = 0; step < 500; ++step)
  {
    int p = rng() % n;
    char c = 'a' + rng() % 26;
    naive[p] = c;
    for (ll i : seg.point_to_nodes_from_bottom(p))
    {
      if (seg.is_leaf(i)) dat[i] = c;
      else dat[i] = dat[2 * i] + dat[2 * i + 1];
    }
    int l = rng() % (n + 1), r = rng() % (n + 1);
    if (l > r) swap(l, r);
    string got;
    for (ll i : seg.range_to_nodes_from_left(l, r)) got += dat[i];
    assert(got == naive.substr(l, r - l));
  }
}

void check_large()
{
  const int n = numeric_limits<int>::max();
  const SegmentTreeRange seg(n);
  assert(seg.leaf_size() == (1LL << 31));
  assert(seg.node_count() == (1LL << 32) - 1);
  assert(seg.node_to_point(seg.point_to_node(n - 1)) == n - 1);
  for (auto [l, r] : vc<pair<int, int>>{{0, n}, {0, 1 << 30}, {n - 1, n}, {1, n - 1}, {n, n}})
  {
    const auto expected = reference_nodes(seg.leaf_size(), l, r);
    check_range(seg.range_to_nodes_from_left(l, r), expected);
    auto bottom = seg.range_to_nodes_from_bottom(l, r).to_v();
    auto sorted = expected;
    sort(bottom.begin(), bottom.end());
    sort(sorted.begin(), sorted.end());
    assert(bottom == sorted);
  }
  assert(seg.point_to_nodes_from_top(n - 1).to_v().size() == 32);
}

int main()
{
  check_small();
  check_lifetime();
  check_point_update_range_concat();
  check_large();
  cout << "Hello World" << endl;
}
