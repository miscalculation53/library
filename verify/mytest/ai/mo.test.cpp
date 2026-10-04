#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/mo/mo.hpp"
#include "ds/fenwick_tree/inversion.hpp"

template <class I>
void check_ranges(int n, const vc<pair<I, I>> &lrs, mt19937 &rng)
{
  vc<int> a(n), expected(lrs.size());
  for (int &x : a) x = rng() % 7;
  repi(i, lrs.size())
  {
    set<int> values;
    for (int j = lrs[i].first; j < lrs[i].second; j++) values.insert(a[j]);
    expected[i] = values.size();
  }
  vc<int> active(n), count(7), seen(lrs.size());
  int distinct = 0, l = 0, r = 0;
  auto add = [&](int i, bool)
  {
    assert(active[i] == 0);
    active[i] = 1;
    distinct += (count[a[i]]++ == 0);
  };
  auto del = [&](int i, bool)
  {
    assert(active[i] == 1);
    active[i] = 0;
    distinct -= (--count[a[i]] == 0);
  };
  auto rem = [&](int qid)
  {
    assert(seen[qid]++ == 0 && distinct == expected[qid]);
    repi(i, n) assert(active[i] == (lrs[qid].first <= i && i < lrs[qid].second));
  };
  auto add_l = [&](int nl, int nr) { assert(nl == l - 1 && nr == r); add(--l, true); };
  auto add_r = [&](int nl, int nr) { assert(nl == l && nr == r); add(r++, false); };
  auto del_l = [&](int nl, int nr) { assert(nl == l && nr == r); del(l++, true); };
  auto del_r = [&](int nl, int nr) { assert(nl == l && nr == r - 1); del(--r, false); };
  mo(n, lrs, add_l, add_r, del_l, del_r, rem);
  for (int s : seen) assert(s == 1);

  fill(ALL(active), 0), fill(ALL(count), 0), fill(ALL(seen), 0);
  distinct = 0;
  mo(n, lrs, add, del, rem);
  for (int s : seen) assert(s == 1);

  InversionSlider slider(a);
  slider.set(n / 2, n);
  fill(ALL(seen), 0);
  mo(n, lrs, slider, [&](int qid)
  {
    ll inversions = 0;
    for (int i = lrs[qid].first; i < lrs[qid].second; i++)
      for (int j = i + 1; j < lrs[qid].second; j++) inversions += a[i] > a[j];
    assert(slider.inversion_num == inversions && seen[qid]++ == 0);
  });
  for (int s : seen) assert(s == 1);
}

void test_orders()
{
  mt19937 rng(12345);
  for (int n : {0, 1, 7, 100, 2000000000})
    for (int q : {0, 1, 2, 40, 1000})
    {
      vc<pair<uint, uint>> lrs(q);
      for (auto &[l, r] : lrs) l = rng() % (uint(n) + 1), r = rng() % (uint(n) + 1);
      auto ord = internal::mo_order(lrs);
      assert(is_permutation(ord));
      ll cost = 0;
      repi(i, 1, q)
      {
        cost += abs(ll(lrs[ord[i]].first) - lrs[ord[i - 1]].first);
        cost += abs(ll(lrs[ord[i]].second) - lrs[ord[i - 1]].second);
      }
      assert(internal::mo_order_cost(lrs, ord) == cost);
      auto by_right = radix_argsort(lrs, [](const auto &lr) { return lr.second; });
      // For every candidate, independently check the block and right-end order.
      for (int b : {max(1, n / 2), max(1, n)}) for (int t : {0, 1})
      {
        auto candidate = internal::mo_order_params(lrs, by_right, n, b, t);
        auto key = [&](int i)
        {
          ll block = (ll(lrs[i].first) + ll(t) * b / 2) / b;
          return pair<ll, ll>{block, (block & 1) ? -ll(lrs[i].second) : ll(lrs[i].second)};
        };
        assert(is_permutation(candidate));
        assert(is_sorted(ALL(candidate), [&](int i, int j) { return key(i) < key(j); }));
      }
    }
  vc<pair<int, int>> large{{0, 0}, {1000000000, 1000000000}, {0, 0}};
  assert(internal::mo_order_cost(large, vc<int>{0, 1, 2}) == 4000000000LL);
}

int main()
{
  mt19937 rng(20260926);
  for (int n : {0, 1, 2, 10, 40})
  {
    vc<pair<int, int>> lrs;
    repi(l, n + 1) repi(r, l, n + 1) lrs.emplace_back(l, r);
    shuffle(ALL(lrs), rng);
    check_ranges(n, lrs, rng);
    check_ranges(n, vc<pair<ll, ll>>{{0, n}, {n, n}, {0, n}}, rng);
    check_ranges(n, vc<pair<int, int>>{}, rng);
  }
  test_orders();
  PRINT("Hello World");
}
