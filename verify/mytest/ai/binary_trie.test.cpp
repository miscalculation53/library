#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "ds/binary_trie.hpp"

// Test focus: generic monoid values, XOR ordering, predicate searches, and key boundaries.
struct BinaryTrieTestMonoid
{
  struct S
  {
    ll cnt, sum;
    friend bool operator==(const S &a, const S &b) { return a.cnt == b.cnt && a.sum == b.sum; }
  };
  static S op(S a, S b) { return {a.cnt + b.cnt, a.sum + b.sum}; }
  static S e() { return {0, 0}; }
};

void test_random()
{
  constexpr int width = 6;
  constexpr int n = 1 << width;
  using M = BinaryTrieTestMonoid;
  using S = M::S;
  BinaryTrie<width, M> bt;
  array<S, n> a{};
  array<bool, n> registered{};
  const auto cnt = [](const S &s) { return s.cnt; };
  bt.reserve(1 << 10);
  assert(as_const(bt).content().empty());

  auto verify = [&]()
  {
    S all = M::e();
    map<ll, S> expected;
    repi(i, n)
    {
      assert(bt.get(i) == a[i]);
      all = M::op(all, a[i]);
      if (registered[i]) expected.emplace(i, a[i]);
    }
    assert(bt.all_sum() == all);
    assert(as_const(bt).content() == expected);

    repi(_, 20)
    {
      ll x = randint(0, n - 1);
      ll v = randint(0, n - 1);
      S lt = M::e(), leq = M::e(), geq = M::e(), gt = M::e();
      vc<ll> keys;
      repi(key, n)
      {
        ll y = key ^ x;
        if (y < v) lt = M::op(lt, a[key]);
        if (y <= v) leq = M::op(leq, a[key]);
        if (y >= v) geq = M::op(geq, a[key]);
        if (y > v) gt = M::op(gt, a[key]);
        rep(_, a[key].cnt) keys.eb(key);
      }
      sort(ALL(keys), [&](ll p, ll q) { return (p ^ x) < (q ^ x); });

      assert(bt.lt_sum(v, x) == lt);
      assert(bt.leq_sum(v, x) == leq);
      assert(bt.geq_sum(v, x) == geq);
      assert(bt.gt_sum(v, x) == gt);

      repi(k, SZ(keys)) assert(bt.kth_by(k, cnt, x) == keys[k]);
      if (!keys.empty())
      {
        assert(bt.min_element_by(cnt, x) == keys.front());
        assert(bt.max_element_by(cnt, x) == keys.back());
      }

      auto check = [&](ll got, const vc<ll> &cand, bool take_max)
      {
        if (cand.empty())
          assert(got == -1);
        else
        {
          ll ans = cand.front();
          fec(key : cand)
          {
            if (take_max ? (key ^ x) > (ans ^ x) : (key ^ x) < (ans ^ x))
              ans = key;
          }
          assert(got == ans);
        }
      };
      vc<ll> lt_keys, leq_keys, geq_keys, gt_keys;
      repi(key, n) if (a[key].cnt > 0)
      {
        ll y = key ^ x;
        if (y < v) lt_keys.eb(key);
        if (y <= v) leq_keys.eb(key);
        if (y >= v) geq_keys.eb(key);
        if (y > v) gt_keys.eb(key);
      }
      check(bt.lt_max_by(v, cnt, x), lt_keys, true);
      check(bt.leq_max_by(v, cnt, x), leq_keys, true);
      check(bt.geq_min_by(v, cnt, x), geq_keys, false);
      check(bt.gt_min_by(v, cnt, x), gt_keys, false);

      ll lower = randint(0, n - 1), upper = randint(0, n - 1);
      ll lim = randint(0, max(0LL, all.cnt + 2));
      auto pred = [&](const S &s) { return s.cnt <= lim; };

      ll sum = 0, right_ng = -1, right_ok = -1;
      repi(y, lower, n)
      {
        sum += a[y ^ x].cnt;
        if (sum > lim)
        {
          right_ng = y;
          break;
        }
        if (a[y ^ x].cnt > 0)
          right_ok = y;
      }
      auto got_right_ng = bt.max_right_ng(lower, pred, x);
      if (right_ng == -1)
        assert(got_right_ng == n);
      else
        assert(got_right_ng == (right_ng ^ x));
      auto got_right_ok = bt.max_right_ok_by(lower, pred, cnt, x);
      assert(got_right_ok == (right_ok == -1 ? -1 : right_ok ^ x));

      sum = 0;
      ll left_ng = -1, left_ok = n;
      repi(y, ll(upper), -1, -1)
      {
        sum += a[y ^ x].cnt;
        if (sum > lim)
        {
          left_ng = y;
          break;
        }
        if (a[y ^ x].cnt > 0)
          left_ok = y;
      }
      auto got_left_ng = bt.min_left_ng(upper, pred, x);
      if (left_ng == -1)
        assert(got_left_ng == -1);
      else
        assert(got_left_ng == (left_ng ^ x));
      auto got_left_ok = bt.min_left_ok_by(upper, pred, cnt, x);
      assert(got_left_ok == (left_ok == n ? n : left_ok ^ x));
    }
  };

  repi(_, 300)
  {
    int t = randint(0, 3);
    ll key = randint(0, n - 1);
    if (t < 3) registered[key] = true;
    if (t == 0)
    {
      ll c = randint(0, 3);
      S val = c == 0 ? M::e() : S{c, randint(-20, 20)};
      bt.set(key, val);
      a[key] = val;
    }
    else if (t == 1)
    {
      S val{randint(0, 2), randint(-20, 20)};
      if (val.cnt == 0)
        val = M::e();
      bt.add(key, val);
      a[key] = M::op(a[key], val);
    }
    else if (t == 2)
    {
      bt.modify(key, [&](S &val) { val = M::op(val, S{1, 2}); });
      a[key] = M::op(a[key], S{1, 2});
    }
    else
    {
      ll x = randint(0, n - 1);
      array<S, n> b{};
      array<bool, n> next_registered{};
      repi(i, n) b[i ^ x] = a[i];
      repi(i, n) next_registered[i ^ x] = registered[i];
      a = b;
      registered = next_registered;
      bt.xor_all(x);
    }
    verify();
  }

  bt.clear();
  assert(as_const(bt).content().empty());
  assert(bt.all_sum() == M::e());
  repi(i, n) assert(bt.get(i) == M::e());
}

void test_sparse_boundary_keys()
{
  BinaryTrie<8> bt;
  const auto cnt = identity{};
  bt.set(2, 1);
  bt.set(100, 1);
  auto pred = [](ll sum) { return sum <= 1; };

  assert(bt.max_right_ng(0, pred) == 100);
  assert(bt.max_right_ok_by(0, pred, cnt) == 2);
  assert(bt.min_left_ng(100, pred) == 2);
  assert(bt.min_left_ok_by(100, pred, cnt) == 100);

  constexpr ll x = 127;
  assert(bt.max_right_ng(0, pred, x) == 2);
  assert(bt.max_right_ok_by(0, pred, cnt, x) == 100);

  bt.set(2, 0);
  assert((as_const(bt).content() == map<ll, ll>{{2, 0}, {100, 1}}));
  assert(bt.max_right_ng(0, pred) == 256);
  assert(bt.max_right_ok_by(0, pred, cnt) == 100);
}

void test_width_63()
{
  using BT = BinaryTrie<63>;
  using Boundary = typename BT::Boundary;
  static_assert(is_same_v<Boundary, i128>);
  BT bt;
  const auto cnt = identity{};
  constexpr ll hi = 1LL << 62;
  constexpr ll mx = numeric_limits<ll>::max();
  constexpr Boundary limit = Boundary(1) << 63;
  bt.set(0, 2);
  bt.set(hi, 3);
  bt.set(mx, 1);
  assert(bt.lt_sum(mx) == 5);
  assert(bt.leq_sum(mx) == 6);
  assert(bt.geq_sum(0) == 6);
  assert(bt.gt_sum(mx) == 0);
  assert(bt.kth_by(0LL, cnt) == 0);
  assert(bt.kth_by(2LL, cnt) == hi);
  assert(bt.kth_by(5LL, cnt) == mx);

  auto all_ok = [](ll s) { return s <= 6; };
  assert(bt.max_right_ng(0, all_ok) == limit);
  assert(bt.max_right_ok_by(0, all_ok, cnt) == mx);

  auto max_is_ng = [](ll s) { return s < 1; };
  assert(bt.min_left_ng(mx, max_is_ng) == mx);
  assert(bt.min_left_ok_by(mx, max_is_ng, cnt) == limit);

  bt.xor_all(mx);
  assert((as_const(bt).content() == map<ll, ll>{{0, 1}, {mx ^ hi, 3}, {mx, 2}}));
  assert(bt.get(mx) == 2);
  assert(bt.get(mx ^ hi) == 3);
  assert(bt.get(0) == 1);
  assert(bt.min_element_by(cnt, mx) == mx);
  assert(bt.max_element_by(cnt, mx) == 0);
  assert(bt.gt_min_by(mx, cnt) == -1);
  assert(bt.lt_max_by(0, cnt) == -1);

  auto first_ng = [](ll s) { return s < 2; };
  assert(bt.max_right_ng(mx, first_ng) == mx);
  assert(bt.max_right_ok_by(mx, first_ng, cnt) == -1);

  bt.add(hi, 4);
  assert(bt.all_sum() == 10 && bt.get(hi) == 4);
  assert(bt.kth_by(5LL, cnt, mx) == hi);
  assert(bt.kth_by(9LL, cnt, mx) == 0);
  bt.add(mx, -2);
  assert(bt.all_sum() == 8 && bt.get(mx) == 0);
  assert(bt.min_element_by(cnt, mx) == hi);
  assert(bt.max_element_by(cnt, mx) == 0);
}

struct BinaryTrieProductMonoid
{
  using S = ll;
  static S e() { return 1; }
  static S op(S a, S b) { return a * b; }
};

struct BinaryTrieAndMonoid
{
  using S = ull;
  static S e() { return ~0ULL; }
  static S op(S a, S b) { return a & b; }
};

template <class M>
void test_unit_registration(typename M::S active)
{
  using S = typename M::S;
  BinaryTrie<16, M> bt;
  map<ll, S> expected{{0, active}};
  bt.set(0, active);
  for (ll key : {8, 4, 12, 8192, 16384, 32768, 65535, 1, 2, 3})
  {
    bt.set(key, M::e());
    expected[key] = M::e();
    assert(bt.content() == expected && bt.all_sum() == active);
  }
  for (ll bound : {0, 1, 4, 8, 15, 32768, 65535})
  {
    S lower = M::e(), upper = M::e();
    for (auto [key, value] : expected)
    {
      if (key < bound) lower = M::op(lower, value);
      if (key > bound) upper = M::op(upper, value);
    }
    assert(bt.lt_sum(bound) == lower && bt.gt_sum(bound) == upper);
  }
  bt.xor_all(65535);
  map<ll, S> shifted;
  for (auto [key, value] : expected) shifted[key ^ 65535] = value;
  assert(bt.content() == shifted);
  bt.clear();
  assert(bt.content().empty() && bt.all_sum() == M::e());
  bt.set(12345, M::e());
  bt.set(12345, M::e());
  assert((bt.content() == map<ll, S>{{12345, M::e()}}));
}

void test_saved_modify_reference()
{
  BinaryTrie<16> bt;
  bt.set(0, 2);
  ll *saved = nullptr;
  bt.modify(0, [&](ll &value)
  {
    saved = &value;
    for (ll key = 1; key <= 512; ++key) bt.set(key, 1);
    bt.reserve(8192);
    value = 7;
  });
  assert(bt.get(0) == 7 && bt.all_sum() == 519);

  *saved = 11;
  auto copied = bt;
  copied.set(0, 11);
  assert(copied.all_sum() == 523);
  auto moved = std::move(bt);
  moved.set(0, 11);
  assert(moved.all_sum() == 523);
  *saved = 13;
  moved.set(0, 13);
  assert(moved.all_sum() == 525 && copied.get(0) == 11);
  moved.modify(0, [&](ll &value)
  {
    value = 17;
    moved.set(0, 17);
    assert(moved.all_sum() == 529);
    value = 19;
  });
  assert(moved.all_sum() == 531);
  moved.clear();
  moved.set(0, 0);
  assert((moved.content() == map<ll, ll>{{0, 0}}));
}

struct BinaryTrieCountOrMonoid
{
  struct S
  {
    ull count, bits;
  };
  static S e() { return {0, 0}; }
  static S op(S a, S b) { return {a.count + b.count, a.bits | b.bits}; }
};

struct BinaryTrieRankOnly
{
  ull value = 0;
  BinaryTrieRankOnly() = default;
  explicit BinaryTrieRankOnly(ull value) : value(value) {}
  friend bool operator<=(BinaryTrieRankOnly a, BinaryTrieRankOnly b) { return a.value <= b.value; }
  friend bool operator<(BinaryTrieRankOnly a, ull b) { return a.value < b; }
  friend bool operator<=(ull a, BinaryTrieRankOnly b) { return a <= b.value; }
};

struct BinaryTrieCountView
{
  ull value = 0;
  friend bool operator<=(BinaryTrieCountView a, ull b) { return a.value <= b; }
  friend bool operator<(ull a, BinaryTrieCountView b) { return a < b.value; }
};

void test_generic_rank_and_large_count()
{
  using M = BinaryTrieCountOrMonoid;
  BinaryTrie<63, M> bt;
  constexpr ull maximum = numeric_limits<ull>::max();
  constexpr ll hi = 1LL << 62, last = numeric_limits<ll>::max();
  const auto count = [](const M::S &value) { return value.count; };
  bt.set(0, {maximum - 4, 1});
  bt.set(hi, {2, 2});
  bt.set(last, {2, 4});
  assert(bt.all_sum().count == maximum && bt.all_sum().bits == 7);
  assert(bt.kth_by(numeric_limits<ll>::max(), count) == 0);
  assert(bt.kth_by(static_cast<unsigned char>(127), count) == 0);
  assert(bt.kth_by(true, count) == 0);
  assert(bt.kth_by(maximum - 4, count) == hi);
  assert(bt.kth_by(maximum - 1, count) == last);
  assert(bt.kth_by(0, count, last) == last);
  assert(bt.kth_by(2, count, last) == hi);
  assert(bt.kth_by(4, count, last) == 0);

  bt.set(0, {1, 1});
  assert(bt.kth_by(BinaryTrieRankOnly{0}, count) == 0);
  assert(bt.kth_by(BinaryTrieRankOnly{2}, count) == hi);
  assert(bt.kth_by(BinaryTrieRankOnly{4}, count) == last);
  const auto view = [](const M::S &value) { return BinaryTrieCountView{value.count}; };
  assert(bt.kth_by(0ULL, view) == 0);
  assert(bt.kth_by(2ULL, view) == hi);
  assert(bt.kth_by(4ULL, view) == last);
}

int main()
{
  test_random();
  test_sparse_boundary_keys();
  test_width_63();
  test_unit_registration<MonoidAdd<ll>>(-7);
  test_unit_registration<BinaryTrieProductMonoid>(3);
  test_unit_registration<BinaryTrieAndMonoid>(0x5555555555555555ULL);
  test_saved_modify_reference();
  test_generic_rank_and_large_count();
  cout << "Hello World" << endl;
}
