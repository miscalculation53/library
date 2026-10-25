#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/segtree/sparse_segtree.hpp"
#include "ds/segtree/sparse_lazy_segtree.hpp"
#include "math/modint/modint.hpp"
#include "algebra/acted_monoid/add_sum.hpp"
#include "algebra/acted_monoid/affine_sum.hpp"
#include "algebra/acted_monoid/upd_sum.hpp"
#include "algebra/acted_monoid/add_min_sum.hpp"
#include "algebra/acted_monoid/upd_min_sum.hpp"
#include "algebra/acted_monoid/chmin_chmax_add_min_max_sum.hpp"
#include "algebra/acted_monoid/ariupd_min.hpp"
#include "algebra/index_power_sum.hpp"
#include "algebra/majority_vote.hpp"
#include "algebra/min_count.hpp"

constexpr ll N = 1'000'000'000'000'000'000LL;

// Test focus: the supplied binary flip example retains lengths through the ll limit.
void test_affine_flip()
{
  using AM = ActedMonoidAffineSum<ull>;
  mt19937_64 rng(123456789);
  for (ll n : {ll(INT_MAX), ll(INT_MAX) + 1, 1LL << 40, N, LLONG_MAX})
  {
    SparseLazySegmentTree<AM> seg(n, 0);
    vc<ll> xs{0, 1, 10, n / 3, n / 2, n - 10, n - 1, n};
    vc<ull> a(xs.size() - 1);
    assert(seg.all_prod().len == n && seg.all_prod().val == 0);
    seg.apply(0, n, {-1ull, 1ull});
    fill(ALL(a), 1);
    assert(seg.all_prod().val == ull(n));

    repi(iter, 300)
    {
      int l = rng() % xs.size(), r = rng() % xs.size();
      if (l > r) swap(l, r);
      seg.apply(xs[l], xs[r], {-1ull, 1ull});
      repi(i, l, r) a[i] ^= 1;
      l = rng() % xs.size(), r = rng() % xs.size();
      if (l > r) swap(l, r);
      ull sum = 0, total = 0;
      repi(i, a.size())
      {
        ull part = a[i] * ull(xs[i + 1] - xs[i]);
        total += part;
        if (l <= i && i < r) sum += part;
        assert(seg.get(xs[i]).len == 1 && seg.get(xs[i]).val == a[i]);
      }
      auto got = seg.prod(xs[l], xs[r]);
      assert(got.len == xs[r] - xs[l] && got.val == sum);
      assert(seg.all_prod().len == n && seg.all_prod().val == total);
    }

    auto pred = [](const AM::S &s) { return s.len <= 10; };
    assert(seg.max_right_ok(n / 2, pred) == n / 2 + 10);
    assert(seg.min_left_ok(n / 2, pred) == n / 2 - 10);
  }
}

template <class AM, class G>
void test_sum_actions(G get_sum, ll after_second, ll n = N)
{
  using L = decltype(typename AM::S().len);
  SparseLazySegmentTree<AM> seg(n, typename AM::S(0));
  seg.apply(0, n, typename AM::F(2));
  assert(seg.all_prod().len == L(n) && get_sum(seg.all_prod()) == 2 * n);
  seg.set(n - 1, typename AM::S(4));
  seg.apply(10, n - 1, typename AM::F(1));
  auto got = seg.prod(n / 2, n);
  assert(got.len == L(n - n / 2));
  assert(get_sum(got) == after_second * (n - n / 2 - 1) + 4);
  assert(get_sum(seg.get(0)) == 2 && get_sum(seg.get(n - 1)) == 4);
}

// Test focus: all length-bearing sum actions use their real monoids on implicit ranges.
template <class L = ll>
void test_sum_monoids(ll n = N)
{
  static_assert(is_same_v<decltype(typename MonoidLenSum<ll, L>::S().len), L>);
  static_assert(is_same_v<decltype(typename MonoidMinSum<ll, nullptr, L>::S().len), L>);
  static_assert(is_same_v<decltype(typename MonoidMaxSum<ll, nullptr, L>::S().len), L>);
  static_assert(is_same_v<decltype(typename MonoidMinMaxSum<ll, nullptr, L>::S().len), L>);
  auto val = [](const auto &s) { return s.val; };
  auto sum = [](const auto &s) { return s.sum; };
  test_sum_actions<ActedMonoidAddSum<ll, L>>(val, 3, n);
  test_sum_actions<ActedMonoidUpdSum<ll, L>>(val, 1, n);
  test_sum_actions<ActedMonoidAddMinSum<ll, nullptr, L>>(sum, 3, n);
  test_sum_actions<ActedMonoidAddMaxSum<ll, nullptr, L>>(sum, 3, n);
  test_sum_actions<ActedMonoidAddMinMaxSum<ll, nullptr, L>>(sum, 3, n);
  test_sum_actions<ActedMonoidUpdMinSum<ll, nullptr, L>>(sum, 1, n);
  test_sum_actions<ActedMonoidUpdMaxSum<ll, nullptr, L>>(sum, 1, n);
  test_sum_actions<ActedMonoidUpdMinMaxSum<ll, nullptr, L>>(sum, 1, n);

  using Mod = ActedMonoidAffineSum<modint998244353, L>;
  static_assert(is_same_v<decltype(typename Mod::S().len), L>);
  SparseLazySegmentTree<Mod> seg(n, typename Mod::S(0));
  seg.apply(0, n, {2, 3});
  assert(seg.all_prod().len == L(n));
  assert(seg.all_prod().val == modint998244353(3) * modint998244353(n));

  // Widening len also preserves support for 32-bit value types.
  using Add = ActedMonoidAddSum<int, L>;
  using Affine = ActedMonoidAffineSum<int, L>;
  using Upd = ActedMonoidUpdSum<int, L>;
  assert(Add::mapping(2, {4, 3}).val == 11);
  assert(Affine::mapping({2, 3}, {4, 3}).val == 18);
  assert(Upd::mapping(2, {4, 3}).val == 8);
}

template <class L>
void test_count_type()
{
  using Vote = MajorityVote<int, L>;
  static_assert(is_same_v<decltype(typename Vote::S().cnt), L>);
  auto vote = Vote::op({7, 10}, {7, 20});
  assert(vote.cand == 7 && vote.cnt == 30);
  vote = Vote::op(vote, {8, 29});
  assert(vote.cand == 7 && vote.cnt == 1);

  using AM = ActedMonoidChminChmaxAddMinMaxSum<ll, 1000, L>;
  static_assert(is_same_v<decltype(typename AM::S().len), L>);
  static_assert(is_same_v<decltype(typename AM::S().mn_cnt), L>);
  static_assert(is_same_v<decltype(typename AM::S().mx_cnt), L>);
  auto x = AM::op(typename AM::S(1, 10), typename AM::S(3, 20));
  assert(x.len == 30 && x.sum == 70 && x.mn_cnt == 10 && x.mx_cnt == 20);
  x = AM::mapping(AM::F::chmin(2), x);
  assert(!x.fail && x.sum == 50);
  x = AM::mapping(AM::F::chmax(2), x);
  assert(!x.fail && x.sum == 60 && x.mn_cnt == 30 && x.mx_cnt == 30);
}

// Test focus: a selected 128-bit length survives both aggregation and scalar products.
void test_wide_length()
{
  constexpr i128 len = i128(1) << 100;
  using Affine = ActedMonoidAffineSum<i128, i128>;
  SparseLazySegmentTree<Affine> seg(2, Affine::S(len, 0));
  seg.apply(0, 2, {1, 3});
  assert(seg.all_prod().len == 2 * len && seg.all_prod().val == 6 * len);
  assert(seg.get(1).len == len && seg.get(1).val == 3 * len);
  using Add = ActedMonoidAddSum<i128, i128>;
  using Upd = ActedMonoidUpdSum<i128, i128>;
  assert(Add::mapping(-1, {len, 3 * len}).val == 2 * len);
  assert(Upd::mapping(3, {len, 0}).val == 3 * len);

  using Mn = MonoidMinSum<i128, nullptr, i128>;
  using Mx = MonoidMaxSum<i128, nullptr, i128>;
  using MM = MonoidMinMaxSum<i128, nullptr, i128>;
  assert(Mn::S(3, len).sum == 3 * len);
  assert(Mx::S(3, len).sum == 3 * len);
  assert(MM::S(3, len).sum == 3 * len);
  using AddMM = ActedMonoidAddMinMaxSum<i128, nullptr, i128>;
  assert(AddMM::mapping(-1, MM::S(3, len)).sum == 2 * len);

  using Mod = ActedMonoidAffineSum<modint998244353, u128>;
  auto mod = Mod::mapping({1, 3}, Mod::S(u128(len), 0));
  assert(mod.len == u128(len) && mod.val == modint998244353(len) * 3);

  using Beats = ActedMonoidChminChmaxAddMinMaxSum<i128, nullptr, i128>;
  auto x = Beats::op(Beats::S(1, len), Beats::S(3, len));
  assert(x.len == 2 * len && x.sum == 4 * len && x.mn_cnt == len && x.mx_cnt == len);
  x = Beats::mapping(Beats::F::chmin(2), x);
  assert(!x.fail && x.sum == 3 * len);
  x = Beats::mapping(Beats::F::chmax(2), x);
  assert(!x.fail && x.sum == 4 * len && x.mn_cnt == 2 * len && x.mx_cnt == 2 * len);

  using Vote = MajorityVote<int, i128>;
  auto vote = Vote::op({7, len}, {7, len});
  assert(vote.cand == 7 && vote.cnt == 2 * len);
}

// Test focus: a 32-bit unsigned value keeps its modular arithmetic with a 64-bit length.
void test_unsigned_values()
{
  using Add = ActedMonoidAddSum<uint>;
  using Affine = ActedMonoidAffineSum<uint>;
  using Upd = ActedMonoidUpdSum<uint>;
  const uint expected = uint(N) * UINT_MAX;
  SparseLazySegmentTree<Add> add(N, Add::S(0));
  SparseLazySegmentTree<Affine> affine(N, Affine::S(0));
  SparseLazySegmentTree<Upd> upd(N, Upd::S(0));
  add.apply(0, N, UINT_MAX);
  affine.apply(0, N, {1, UINT_MAX});
  upd.apply(0, N, UINT_MAX);
  assert(add.all_prod().val == expected);
  assert(affine.all_prod().val == expected);
  assert(upd.all_prod().val == expected);
  assert(add.prod(N / 2, N).val == uint(N - N / 2) * UINT_MAX);
  assert(affine.get(N - 1).val == UINT_MAX && upd.get(N - 1).val == UINT_MAX);
  MonoidMinSum<uint, UINT_MAX>::S x(UINT_MAX, N);
  assert(x.len == N && x.sum == expected);
}

// Test focus: ordinary sparse trees can aggregate explicitly weighted lengths and counts.
void test_weighted_sparse_values()
{
  using M = MonoidMinMaxSum<ll>;
  SparseSegmentTree<M> seg(N, M::e());
  seg.set(5, M::S(1, N / 2));
  seg.set(N - 1, M::S(3, N - N / 2));
  auto got = seg.prod(0, N);
  assert(got.len == N && got.sum == 2 * N && got.mn == 1 && got.mx == 3);
  assert(seg.prod(6, N).len == N - N / 2);

  using Vote = MajorityVote<int>;
  SparseSegmentTree<Vote> votes(N, Vote::e());
  votes.set(5, Vote::S(7, N / 2));
  votes.set(N - 1, Vote::S(7, N - N / 2));
  assert(votes.all_prod().cand == 7 && votes.all_prod().cnt == N);
  votes.set(N - 1, Vote::S(8, N / 2 - 1));
  assert(votes.all_prod().cand == 7 && votes.all_prod().cnt == 1);

  auto mn = MonoidMinCount<ll>::op({2, N / 2}, {2, N - N / 2});
  auto mx = MonoidMaxCount<ll>::op({2, N / 2}, {2, N - N / 2});
  assert(mn.cnt == N && mx.cnt == N);

  using Beats = ActedMonoidChminChmaxAddMinMaxSum<ll>;
  auto x = Beats::op(Beats::S(1, N / 2), Beats::S(3, N - N / 2));
  assert(x.len == N && x.mn_cnt == N / 2 && x.mx_cnt == N - N / 2);
  x = Beats::mapping(Beats::F::chmin(2), x);
  assert(!x.fail && x.sum == N / 2 + 2 * (N - N / 2));
  x = Beats::mapping(Beats::F::chmax(2), x);
  assert(!x.fail && x.sum == 2 * N && x.mn_cnt == N && x.mx_cnt == N);
}

// Test focus: index-bearing monoids also preserve sparse 64-bit coordinates.
void test_large_indices()
{
  using M = MonoidIndexPowerSum<ll, 1>;
  SparseSegmentTree<M> seg(N, M::e());
  seg.set(N - 10, M::S(N - 10, 2));
  seg.set(N - 1, M::S(N - 1, 3));
  auto got = seg.all_prod();
  assert(got.val == 5 && got.power_sum[0] == 2 && got.power_sum[1] == 2 * N - 11);

  using Mn = ActedMonoidAriUpdMin<ll>;
  using Mx = ActedMonoidAriUpdMax<ll>;
  using MM = ActedMonoidAriUpdMinMax<ll>;
  auto mn = Mn::op(Mn::S(N - 10, 0), Mn::S(N - 1, 0));
  auto mx = Mx::op(Mx::S(N - 10, 0), Mx::S(N - 1, 0));
  auto mm = MM::op(MM::S(N - 10, 0), MM::S(N - 1, 0));
  assert(mn.l == N - 10 && mn.r == N);
  assert(mx.l == N - 10 && mx.r == N);
  assert(mm.l == N - 10 && mm.r == N);
  assert(Mn::mapping(Mn::F({0, 1}), mn).mn == N - 10);
  assert(Mn::mapping(Mn::F({0, -1}), mn).mn == -(N - 1));
  assert(Mx::mapping(Mx::F({0, 1}), mx).mx == N - 1);
  assert(Mx::mapping(Mx::F({0, -1}), mx).mx == -(N - 10));
  auto positive = MM::mapping(MM::F({0, 1}), mm);
  auto negative = MM::mapping(MM::F({0, -1}), mm);
  assert(positive.mn == N - 10 && positive.mx == N - 1);
  assert(negative.mn == -(N - 1) && negative.mx == -(N - 10));
}

int main()
{
  test_affine_flip();
  test_sum_monoids();
  test_sum_monoids<int>(97);
  test_sum_monoids<unsigned short>(97);
  test_sum_monoids<ull>(97);
  test_sum_monoids<i128>(97);
  test_sum_monoids<u128>(97);
  test_count_type<int>();
  test_count_type<unsigned short>();
  test_count_type<ull>();
  test_count_type<i128>();
  test_wide_length();
  test_unsigned_values();
  test_weighted_sparse_values();
  test_large_indices();
  cout << "Hello World" << endl;
}
