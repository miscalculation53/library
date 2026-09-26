#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/prime/sieve/segmented_sieve.hpp"

vc<PrimePower<ll>> trial_factorize(ll x)
{
  vc<PrimePower<ll>> res;
  for (ll p = 2; p <= x / p; ++p)
  {
    if (x % p != 0) continue;
    int e = 0;
    ll pe = 1;
    do { x /= p; ++e; pe *= p; } while (x % p == 0);
    res.emplace_back(p, e, pe);
  }
  if (x > 1) res.emplace_back(x);
  return res;
}

void check_interval(ll l, ll r)
{
  vvc<PrimePower<ll>> expected;
  vc<pair<ll, ll>> small, large;
  for (ll x = l; x <= r; ++x)
  {
    expected.push_back(trial_factorize(x));
    for (auto pp : expected.back())
    {
      if (pp.p == x) continue;
      (pp.p <= r / pp.p ? small : large).emplace_back(pp.p, x);
    }
  }
  sort(small.begin(), small.end());
  small.insert(small.end(), large.begin(), large.end());

  const auto range = segmented_sieve(l, r);
  assert((vc<pair<ll, ll>>(range.begin(), range.end()) == small));
  // 同じ範囲をもう一度列挙できる。
  vc<pair<ll, ll>> actual;
  for (auto [p, x] : range) actual.emplace_back(p, x);
  assert(actual == small);
  assert(segmented_factorize(l, r) == expected);
}

void test_iterators()
{
  using Iterator = segmented_sieve::Iterator;
  static_assert(is_same_v<iterator_traits<Iterator>::value_type, pair<ll, ll>>);
  static_assert(is_same_v<iterator_traits<Iterator>::iterator_category, input_iterator_tag>);
#if __cplusplus >= 202002L
  static_assert(input_iterator<Iterator>);
  static_assert(ranges::input_range<segmented_sieve>);
#endif
  assert(Iterator() == Iterator());
  const auto range = segmented_sieve(94, 94);
  const vc<pair<ll, ll>> expected{{2, 94}, {47, 94}};
  auto it = range.begin();
  auto copy = it;
  assert(it == copy && *it == expected[0]);
  assert(*it++ == expected[0]);
  assert(it != copy && *it == expected[1]);
  assert(*it++ == expected[1]);
  assert(it == range.end());

  // begin() ごとの走査は独立し、一時オブジェクトからも取得できる。
  auto first = range.begin(), second = range.begin();
  assert(first == second);
  ++first;
  assert(first != second && *first == expected[1] && *second == expected[0]);
  auto temporary = segmented_sieve(94, 94).begin();
  assert(*temporary == expected[0]);
  assert(distance(range.begin(), range.end()) == 2);
  const auto copied_range = range;
  assert((vc<pair<ll, ll>>(copied_range.begin(), copied_range.end()) == expected));
}

void test_shared_sieve()
{
  const auto range = segmented_sieve(94, 94);
  auto it = range.begin();
  LinearSieve::reserve(10000);
  assert((*it++ == pair<ll, ll>{2, 94}));
  assert((*it++ == pair<ll, ll>{47, 94}));
  assert(it == range.end());
  check_interval(94, 94);

  vc<pair<ll, ll>> actual;
  for (auto [p, x] : segmented_sieve(90, 110))
  {
    // 列挙中の共有篩の拡張と、別の区間の列挙。
    LinearSieve::reserve(30000);
    check_interval(1, 30);
    actual.emplace_back(p, x);
  }
  const auto outer = segmented_sieve(90, 110);
  assert((actual == vc<pair<ll, ll>>(outer.begin(), outer.end())));
}

int main()
{
  check_interval(1, 1);
  check_interval(2, 2);
  test_iterators();
  test_shared_sieve();
  for (ll l = 1; l <= 100; ++l)
    for (ll r = l; r <= 100; ++r)
      check_interval(l, r);
  check_interval(1, 10000);

  mt19937_64 gen(42);
  for (int t = 0; t < 200; ++t)
  {
    const ll l = 1 + gen() % 1000000;
    check_interval(l, l + gen() % 100);
  }
  for (ll x : {1LL << 32, 4294967291LL, 999983LL * 999983, 1LL << 40})
    check_interval(x - 2, x + 2);
  PRINT("Hello World");
}
