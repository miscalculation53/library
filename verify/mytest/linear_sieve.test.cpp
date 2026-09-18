#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/prime/sieve/linear_sieve.hpp"
#include "math/prime/large/factorize.hpp"

void test_primes()
{
  auto trial_primes = [](int n)
  {
    vc<int> res;
    for (int p = 2; p <= n; p++)
    {
      bool prime = true;
      for (int d = 2; d <= p / d; d++) if (p % d == 0) prime = false;
      if (prime) res.push_back(p);
    }
    return res;
  };
  for (int limit : {-7, 0, 1, 2, 3, 4, 7, 11, 12, 97, 250})
  {
    const auto ps = LinearSieve::primes(limit);
    static_assert(is_same_v<decltype(*ps.begin()), ll>);
    static_assert(is_same_v<decltype(ps[0]), ll>);
    static_assert(is_same_v<decltype(ps.size()), ll>);
    static_assert(is_same_v<decltype(ps.size<int>()), int>);
    static_assert(is_same_v<decltype(ps.size<size_t>()), size_t>);
    const auto expected = trial_primes(limit);
    assert(ps.size<size_t>() == expected.size() && ps.empty() == expected.empty());
    assert(vc<int>(ps.begin(), ps.end()) == expected);
    for (size_t i = 0; i < ps.size<size_t>(); i++) assert(ps[i] == expected[i]);
    const auto ints = LinearSieve::primes<int>(limit);
    static_assert(is_same_v<decltype(*ints.begin()), int>);
    static_assert(is_same_v<decltype(ints[0]), int>);
    static_assert(is_same_v<decltype(ints.size()), ll>);
    static_assert(is_same_v<typename decltype(ints.begin())::value_type, int>);
    assert(ints.size<int>() == int(expected.size()));
    assert(vc<int>(ints.begin(), ints.end()) == expected);
  }

  const auto ps = LinearSieve::primes(37), copied = ps;
  const auto unsigned_primes = LinearSieve::primes<ull>(37);
  static_assert(is_same_v<decltype(*unsigned_primes.begin()), ull>);
  static_assert(is_same_v<decltype(unsigned_primes[0]), ull>);
  auto it = ps.begin();
  LinearSieve::reserve(4096);
  assert(*it++ == 2 && *it == 3);
  assert(vc<int>(copied.begin(), copied.end()) == trial_primes(37));
  assert(vc<int>(unsigned_primes.begin(), unsigned_primes.end()) == trial_primes(37));
  vc<int> actual;
  for (int p : ps)
  {
    // ループ内での再確保と、上限が異なる範囲の入れ子。
    LinearSieve::is_prime(10007);
    actual.push_back(p);
    const auto inner = LinearSieve::primes(p);
    assert(vc<int>(inner.begin(), inner.end()) == trial_primes(p));
  }
  assert(actual == trial_primes(37));
  assert(ps.size<size_t>() == actual.size());
  const int prepared = LinearSieve::n;
  assert(LinearSieve::primes(1).empty());
  assert(LinearSieve::primes(10).size() == 4);
  assert(LinearSieve::n == prepared);

  const auto large = LinearSieve::primes(100000);
  const auto p = large[large.size() - 1];
  assert(p * p > INT_MAX);
}

void test1()
{
  const ll N = 10000;
  rep(n, 1, N + 1)
  {
    vc<PrimePower<ll>> fac1 = LinearSieve::factorize<ll>(n);
    vc<PrimePower<ll>> fac2 = factorize(n);
    assert(fac1 == fac2);
  }
}

int main()
{
  test_primes();
  test1();

  PRINT("Hello World");
}
