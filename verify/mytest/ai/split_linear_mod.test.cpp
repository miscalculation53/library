#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/mod_of_linear/split_linear_mod.hpp"

void check(ll n, ll m, ll a, ll b)
{
  auto parts = split_linear_mod(n, m, a, b);
  vl seen(n);
  for (auto p : parts)
  {
    assert(p.n > 0 && p.gap > 0);
    for (ll j = 0; j < p.n; ++j)
    {
      ll i = p.origin_index(j);
      assert(0 <= i && i < n);
      ++seen[i];
      ll v = (i128(a) * i + b) % m;
      if (v < 0) v += m;
      assert(p(j) == v);
    }
  }
  for (ll c : seen) assert(c == 1);
  if (n > 1) assert(parts.size() <= 4 * sqrtl(n) + 4);
}

int main()
{
  for (ll m = 1; m <= 35; ++m) for (ll a = 0; a < m; ++a)
    for (ll b = 0; b < m; ++b) for (ll n = 0; n <= m / gcd(a, m); ++n)
      check(n, m, a, b);
  mt19937_64 rng(20260923);
  const ll hi = numeric_limits<ll>::max(), lo = numeric_limits<ll>::min();
  // a*i や value+step*len を直接作ると ll を超えるケース。
  check(3, hi, hi / 2 + 1, 0);
  check(3, hi, hi - 1, hi - 1);
  check(5, hi, -1, 0);
  check(5, hi, 2 * (hi / 3) + 1, hi - 1);
  // 巨大な部分列の両端。値と添字の式の積はそれぞれ ll に収まる。
  const LinearModArithmetic increasing{hi, 1, 0, 0, 1};
  const LinearModArithmetic decreasing{hi, -1, hi - 1, 0, 1};
  const LinearModArithmetic sparse{hi / 3, -3, hi - 1, 2, 3};
  for (auto p : {increasing, decreasing, sparse})
    for (ll j : {0LL, p.n / 2, p.n - 1})
    {
      assert(p(j) == i128(p.a) * j + p.b);
      assert(p.origin_index(j) == i128(p.gap) * j + p.l);
    }
  for (int it = 0; it < 10000; ++it)
  {
    ll m = 1 + rng() % hi;
    ll a = ll(rng() % hi), b = ll(rng() % hi);
    if (it & 1) a = -a;
    if (it & 2) b = -b;
    check(min<ll>(rng() % 100, m / gcd(a % m, m)), m, a, b);
  }
  check(50000, hi, lo, lo);
  check(50000, hi, 5702887, hi - 1);
  // 長さだけを使って、大きい n でも分割数を確認する。
  auto parts = split_linear_mod(1'000'000'000, 1'000'000'007, 444444444, 314159265);
  ll total = 0;
  for (auto p : parts) total += p.n;
  assert(total == 1'000'000'000);
  assert(parts.size() < 4 * sqrtl(total) + 4);
  cout << "Hello World\n";
}
