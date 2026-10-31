#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/coordinate_compression.hpp"
#include "ds/flat_map.hpp"
#include "ds/dynamic_bitset.hpp"
#include "ds/fast_integer_set.hpp"
#include "ds/meldable_integer_set.hpp"
#include "ds/group_index.hpp"
#include "ds/binary_trie.hpp"
#include "ds/fenwick_tree/fenwick_tree_01.hpp"

// Optional values and directional search boundaries agree with direct enumeration.
int main()
{
  static_assert(is_same_v<decltype(CoordinateCompression<int>{}.get_id(0)), optional<ll>>);
  static_assert(is_same_v<decltype(FenwickTree<GroupAddSub<ll>>(0).lt_max_id_sum(0)), optional<pair<ll, ll>>>);
  CoordinateCompression<int> cc(vc<int>{0, 4, 4});
  assert(cc.get_id(0) == 0 && cc.get_id(4) == 1 && !cc.get_id(2));
  assert(!cc.get_id<unsigned>(-1));
  FlatMap<int, int> map(vc<int>{0, 4});
  map[0] = 7;
  assert(map.contains(0) && !map.contains(2) && map.at(0) == 7);
  mt19937 rng(20261025);
  for (int n : {0, 1, 2, 31, 63, 64, 65, 127})
  {
    FastIntegerSet fast(n);
    MeldableIntegerSetPool pool(n);
    auto meld = pool.make_set();
    DynamicBitset bits(n);
    FenwickTree01<ll> fw01(n);
    FenwickTree<GroupAddSub<ll>> fw(n);
    BinaryTrie<8> trie;
    vc<ll> a(n);
    for (int step = 0; step < 150; ++step)
    {
      if (n)
      {
        int p = rng() % n, value = rng() % 3;
        a[p] = value;
        if (value) fast.insert(p), meld.insert(p); else fast.erase(p), meld.erase(p);
        bits.set(p, value != 0);
        fw01.set(p, value != 0);
        fw.set(p, value);
        trie.set(p, value);
      }
      vc<int> keys, repeated;
      repi(i, n) if (a[i]) keys.eb(i);
      repi(i, n) repi(j, a[i]) repeated.eb(i);
      assert(fast.content() == keys && meld.content() == vc<ll>(ALL(keys)));
      assert(vc<int>(fast.begin(), fast.end()) == keys);
      assert(vc<ll>(meld.begin(), meld.end()) == vc<ll>(ALL(keys)));
      assert(fast.min_element() == (keys.empty() ? n : keys.front()));
      assert(meld.max_element() == (keys.empty() ? -1 : keys.back()));
      repi(x, -1, n + 2)
      {
        optional<int> lo, hi;
        for (int p : keys) { if (p <= x) lo = p; if (p >= x && !hi) hi = p; }
        assert(fast.prev(x) == lo.value_or(-1) && fast.next(x) == hi.value_or(n));
        assert(meld.leq_max(x) == lo.value_or(-1) && meld.geq_min(x) == hi.value_or(n));
      }
      for (bool value : {false, true})
      {
        repi(x, -1, n)
        {
          optional<int> expected;
          repi(p, x + 1, n) if (bool(a[p]) == value) { expected = p; break; }
          assert(bits.find_next(x, value) == expected.value_or(n));
        }
        repi(x, n + 1)
        {
          optional<int> expected;
          repi(p, x) if (bool(a[p]) == value) expected = p;
          assert(bits.find_prev(x, value) == expected.value_or(-1));
        }
      }
      ll total = repeated.size();
      repi(w, -1, total + 3)
      {
        optional<ll> lt, le, ge, gt;
        ll sum = 0;
        repi(r, n + 1)
        {
          if (sum < w) lt = r;
          if (sum <= w) le = r;
          if (sum >= w && !ge) ge = r;
          if (sum > w && !gt) gt = r;
          if (r < n) sum += a[r];
        }
        assert(fw.lt_max(w) == lt.value_or(-1) && fw.leq_max(w) == le.value_or(-1));
        assert(fw.geq_min(w) == ge.value_or(n + 1) && fw.gt_min(w) == gt.value_or(n + 1));
        auto pair = fw.lt_max_id_sum(w);
        assert(bool(pair) == bool(lt));
        if (pair) assert(pair->first == *lt && pair->second == fw.sum(*lt));
      }
      assert(fw.lt_max<i128>(0) == -1 && fw.geq_min<unsigned>(0) == 0u);
      repi(k, -1, total + 2)
      {
        optional<ll> expected = 0 <= k && k < total ? optional<ll>(repeated[k]) : nullopt;
        assert(fw.kth_in_multiset(k) == expected && trie.kth_by(k, identity{}) == expected);
      }
      repi(k, -1, int(keys.size()) + 2)
      {
        optional<ll> expected = 0 <= k && k < int(keys.size()) ? optional<ll>(keys[k]) : nullopt;
        assert(fw01.kth_in_set(k) == expected && meld.kth(k) == expected);
      }
      repi(x, n)
      {
        optional<ll> lo, hi;
        for (int p : keys) { if (p < x) lo = p; if (p >= x && !hi) hi = p; }
        assert(trie.lt_max_by(x, identity{}) == lo && trie.geq_min_by(x, identity{}) == hi);
        auto expected_rank = hi ? optional<ll>(lower_bound(ALL(repeated), x) - repeated.begin()) : nullopt;
        assert(fw.geq_min_in_multiset(x) == expected_rank.value_or(total));
      }
      auto min_key = keys.empty() ? nullopt : optional<ll>(keys.front());
      auto max_key = keys.empty() ? nullopt : optional<ll>(keys.back());
      assert(trie.min_element_by(identity{}) == min_key && trie.max_element_by(identity{}) == max_key);
      GroupIndex<ll> groups(a);
      repi(v, 4) repi(i, -1, n + 1)
      {
        optional<ll> lo, hi;
        repi(p, n) if (a[p] == v) { if (p < i) lo = p; if (p >= i && !hi) hi = p; }
        assert(groups.lt_max(v, i) == lo.value_or(-1) && groups.geq_min(v, i) == hi.value_or(n));
      }
    }
  }
  PRINT("Hello World");
}
