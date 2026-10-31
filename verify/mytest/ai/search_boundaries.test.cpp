#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/pbds_set.hpp"
#include "ds/fenwick_tree/fenwick_tree_01.hpp"
#include "ds/group_index.hpp"

// Exhaust all small frequency arrays, including empty and repeated prefixes.
template <class Count, class Word>
void check_prefixes()
{
  for (int n = 0; n <= 6; ++n)
  {
    int cases = 1;
    for (int i = 0; i < n; ++i) cases *= 3;
    for (int mask = 0; mask < cases; ++mask)
    {
      vc<Count> a(n), bits(n);
      int digits = mask;
      for (int i = 0; i < n; ++i, digits /= 3)
        a[i] = digits % 3, bits[i] = a[i] != 0;
      FenwickTree<GroupAddSub<Count>> fw(a);
      FenwickTree01<Count, Word> fw01(bits);
      vc<Count> prefix{0}, prefix01{0};
      for (int i = 0; i < n; ++i)
        prefix.eb(prefix.back() + a[i]), prefix01.eb(prefix01.back() + bits[i]);
      for (int w = -1; w <= 2 * n + 1; ++w)
      {
        assert(fw.lt_max(w) == lt_max(prefix, w));
        assert(fw.leq_max(w) == leq_max(prefix, w));
        assert(fw.template geq_min<unsigned>(w) == geq_min<unsigned>(prefix, w));
        assert(fw.template gt_min<i128>(w) == gt_min<i128>(prefix, w));
        assert(fw01.template lt_max<i128>(w) == lt_max<i128>(prefix01, w));
        assert(fw01.leq_max(w) == leq_max(prefix01, w));
        assert(fw01.geq_min(w) == geq_min(prefix01, w));
        assert(fw01.template gt_min<unsigned>(w) == gt_min<unsigned>(prefix01, w));
      }
      assert(!fw.template kth_in_multiset<unsigned>(-1));
      assert(!fw01.template kth_in_set<unsigned>(-1));
    }
  }
}

// Predecessor/successor ranks retain the insertion boundary on both sides.
int main()
{
  static_assert(is_same_v<decltype(geq_min(vc<int>{}, 0)), ll>);
  static_assert(is_same_v<decltype(pbds_set<int>{}.lt_max_id(0)), ll>);
  pbds_set<int> single;
  pbds_multiset<int> repeated;
  for (int x : {-1, 0, 1})
  {
    assert(single.lt_max_id(x) == -1 && single.geq_min_id(x) == 0);
    assert(repeated.leq_max_id(x) == -1 && repeated.gt_min_id(x) == 0);
  }
  repeated.insert(2);
  repeated.insert(2);
  assert(repeated.leq_max_id(2) == 1 && repeated.gt_min_id(2) == 2);
  assert(repeated.lt_max_id(2) == -1 && repeated.geq_min_id(2) == 0);
  GroupIndex<int, true, i128> groups(vc<int>{2, 1, 2});
  assert(groups.lt_max(2, 0) == -1 && groups.gt_min(2, 2) == 3);
  assert(groups.geq_min(9, 0) == 3 && groups.leq_max(9, 2) == -1);
  vc<pair<int, int>> values{{7, 0}, {4, 1}, {4, 2}, {1, 3}};
  auto proj = [](const auto &entry) { return entry.first; };
  assert(geq_min(values, 8, greater<>{}, proj) == 0);
  assert(lt_max(values, 8, greater<>{}, proj) == -1);
  assert(geq_min(values, 0, greater<>{}, proj) == 4);
  assert(leq_max(values, 4, greater<>{}, proj) == 2);
  check_prefixes<ll, uint8_t>();
  check_prefixes<i128, uint64_t>();
  PRINT("Hello World");
}
