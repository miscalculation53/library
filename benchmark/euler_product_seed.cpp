#include "math/prime/sieve/euler_product_prefix_sum.hpp"
#include "math/prime/large/primality_test.hpp"

using R = RingAddSubMul<ll>;

// seed を直接渡す場合と、単位元から積を作って後で seed を掛ける場合の比較。
// 計測範囲は全商位置の結果を作る処理。入力の準備・照合は計測に含めない。
int main(int argc, char **argv)
{
  assert(argc == 4);
  const string problem = argv[1], method = argv[2];
  const ll n = stoll(argv[3]);
  assert(problem == "totient" || problem == "powerful");
  assert(method == "seed" || method == "after");
  assert(n >= 1);
  const int root = iroot(n, 2), cube = iroot(n, 3);
  vvc<pair<ll, ll>> factors;
  for (ll p : LinearSieve::primes(problem == "totient" ? root + 1 : cube))
  {
    factors.push_back(problem == "totient"
      ? vc<pair<ll, ll>>{{p - 1, 1}, {p, -1}}
      : vc<pair<ll, ll>>{{p * p * p, 1}});
  }
  const DirichletPrefixSum<R> seed(n, [&](ll x) -> ll
  { return problem == "totient" ? 1 + (x > root && is_prime(x + 1)) : iroot(x, 2); });
  const auto start = chrono::steady_clock::now();
  auto result = method == "seed"
    ? euler_product_prefix_sum(seed, factors)
    : euler_product_prefix_sum<R>(n, factors) * seed;
  const double seconds = chrono::duration<double>(chrono::steady_clock::now() - start).count();
  const auto other = method == "seed"
    ? euler_product_prefix_sum<R>(n, factors) * seed
    : euler_product_prefix_sum(seed, factors);
  ull checksum = 0;
  for (int i = 0; i < result.size(); i++)
  {
    const ll x = result.value(i);
    assert(result.F(x) == other.F(x));
    checksum = checksum * 1000000007 + result.F(x);
  }
  if (problem == "powerful")
  {
    // 独立な a^2 b^3 (b は squarefree) の列挙で全保持位置を照合する。
    vc<bool> squarefree(cube + 1, true);
    for (ll p : LinearSieve::primes(int(iroot(cube, 2))))
    {
      for (int j = p * p; j <= cube; j += p * p) squarefree[j] = false;
    }
    vc<ll> values;
    for (ll b = 1; b <= cube; b++) if (squarefree[b])
      for (ll a = 1, limit = n / (b * b * b); a <= limit / a; a++) values.push_back(a * a * b * b * b);
    sort(values.begin(), values.end());
    size_t at = 0;
    for (int i = 0; i < result.size(); i++)
    {
      ll x = result.value(i);
      while (at < values.size() && values[at] <= x) at++;
      assert(result.F(x) == ll(at));
    }
  }
  cout << problem << ',' << method << ',' << n << ',' << seconds << ',' << result.F(n) << ',' << checksum << '\n';
}
