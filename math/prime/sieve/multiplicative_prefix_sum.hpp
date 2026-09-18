#pragma once

#include "math/prime/sieve/dirichlet_prefix_sum.hpp"
#include "ds/fenwick_tree/fenwick_tree.hpp"

/**
 * @brief 素数上の和・乗法的関数の和
 * @docs docs/math/prime/sieve/multiplicative_prefix_sum.md
 */

namespace internal
{
  template <class R>
  DirichletPrefixSum<R> prime_prefix_sum_sieve(const DirichletPrefixSum<R> &a)
  {
    const ll n = a.n();
    const int root = int(iroot(n, 2));
    assert(n == 0 || a.F(1) == R::e1());
    DirichletPrefixSum<R> dp(n, [&](ll x) { return R::add(a.F(x), R::minus(R::e1())); });
    for (int p : LinearSieve::primes(root))
    {
      const auto fp = R::add(a.F(p), R::minus(a.F(p - 1))), before = dp.F(p - 1);
      auto update = [&](ll x)
      {
        dp.setF(x, R::add(dp.F(x), R::minus(R::mul(fp,
          R::add(dp.F(x / p), R::minus(before))))));
      };
      for (ll j = 1; j <= n / (ll(root) + 1) && n / j >= ll(p) * p; j++) update(n / j);
      for (ll x = root; x >= ll(p) * p; x--) update(x);
    }
    return dp;
  }

  template <class R, class GetPrimePower>
  DirichletPrefixSum<R> multiplicative_prefix_sum_sieve(
    const DirichletPrefixSum<R> &prime_sum, const GetPrimePower &f_primepower)
  {
    using S = typename R::S;
    const ll n = prime_sum.n();
    const int root = int(iroot(n, 2));
    assert(n == 0 || prime_sum.F(1) == R::e0());
    const auto primes = LinearSieve::primes(root);
    DirichletPrefixSum<R> res(n, [&](ll x) { return prime_sum.F(x); });
    for (int pi = primes.size<int>() - 1; pi >= 0; pi--)
    {
      const int p = primes[pi];
      vc<pair<ll, S>> powers;
      for (ll q = p, e = 1;; q *= p, e++)
      {
        powers.emplace_back(q, internal::eval_primepower<R>(f_primepower, PrimePower<ll>(p, int(e), q)));
        if (q > n / p) break;
      }
      const S before = prime_sum.F(p);
      auto update = [&](ll x)
      {
        S value = res.F(x);
        for (size_t e = 0; e + 1 < powers.size() && powers[e + 1].first <= x; e++)
        {
          value = R::add(value, R::mul(powers[e].second,
            R::add(res.F(x / powers[e].first), R::minus(before))));
          value = R::add(value, powers[e + 1].second);
        }
        res.setF(x, value);
      };
      for (ll j = 1; j <= n / (ll(root) + 1) && n / j >= ll(p) * p; j++) update(n / j);
      for (ll x = root; x >= ll(p) * p; x--) update(x);
    }
    return DirichletPrefixSum<R>(n, [&](ll x) { return R::add(res.F(x), R::e1()); }, true);
  }

  template <class R>
  struct multiplicative_sieve_updates
  {
    ll n, bound;
    int root, large, top;
    dirichlet_prefix_sum_index coordinates;
    FenwickTree<GroupOfRingAdd<R>> bit;

    explicit multiplicative_sieve_updates(ll n)
      : n(n), root(int(iroot(n, 2))), large(int(n / (ll(root) + 1))),
        top(min(large, int(iroot(n, 3)))), coordinates(n)
    {
      bound = n / (ll(top) + 1);
      bit = FenwickTree<GroupOfRingAdd<R>>(root + large - top);
    }
    int index(ll x) const
    { return coordinates.index(x); }
    void add(ll v, const typename R::S &value)
    {
      assert(1 <= v && v <= bound);
      if (value != R::e0()) bit.add(index(v), value);
    }
    typename R::S operator[](ll x) const
    {
      assert(0 <= x && x <= bound);
      return bit.sum(index(x) + 1);
    }
    template <class Apply>
    void apply(DirichletPrefixSum<R> &dp, const Apply &op) const
    {
      for (int x = 1; x <= root; x++) dp.setF(x, op(dp.F(x), (*this)[x]));
      for (int j = large; j > top; j--) dp.setF(n / j, op(dp.F(n / j), (*this)[n / j]));
    }
  };

  template <class R>
  DirichletPrefixSum<R> prime_prefix_sum_sieve_2_3(const DirichletPrefixSum<R> &a)
  {
    using S = typename R::S;
    const ll n = a.n();
    if (n == 0) return {};
    assert(a.F(1) == R::e1());
    const int root = int(iroot(n, 2)), cut = int(iroot(n, 6)), cube = int(iroot(n, 3));
    const auto primes = LinearSieve::primes(root);
    vc<S> weight(root + 1, R::e0());
    for (int p : primes) weight[p] = R::add(a.F(p), R::minus(a.F(p - 1)));
    DirichletPrefixSum<R> dp(n, [&](ll x) { return R::add(a.F(x), R::minus(R::e1())); });
    internal::multiplicative_sieve_updates<R> delta(n);
    const auto sub = [](const S &x, const S &y) { return R::add(x, R::minus(y)); };
    auto lucy = [&](int p)
    {
      const S before = dp.F(p - 1);
      auto update = [&](ll x) { dp.setF(x, sub(dp.F(x), R::mul(weight[p], sub(dp.F(x / p), before)))); };
      for (int j = 1; j <= delta.large && n / j >= ll(p) * p; j++) update(n / j);
      for (ll x = root; x >= ll(p) * p; x--) update(x);
    };
    size_t pi = 0;
    while (pi < primes.size<size_t>() && primes[pi] <= cut) lucy(primes[pi++]);
    for (; pi < primes.size<size_t>() && primes[pi] <= cube; pi++)
    {
      const int p = primes[pi];
      const S before = dp.F(p - 1);
      for (int j = 1; j <= delta.top && n / j >= ll(p) * p; j++)
      {
        const ll x = n / j, y = x / p;
        S value = dp.F(y);
        if (y <= delta.bound) value = sub(value, delta[y]);
        dp.setF(x, sub(dp.F(x), R::mul(weight[p], sub(value, before))));
      }
      auto dfs = [&](auto &&self, ll v, size_t first, const S &f) -> void
      {
        if (v != p) delta.add(v, f);
        for (size_t j = first; j < primes.size<size_t>(); j++)
        {
          const int q = primes[j];
          if (q > delta.bound / v) break;
          self(self, v * q, j, R::mul(f, weight[q]));
        }
      };
      dfs(dfs, p, pi, weight[p]);
    }
    delta.apply(dp, sub);
    while (pi < primes.size<size_t>()) lucy(primes[pi++]);
    return dp;
  }

  template <class R, class GetPrimePower>
  DirichletPrefixSum<R> multiplicative_prefix_sum_sieve_2_3(
    const DirichletPrefixSum<R> &prime_sum, const GetPrimePower &f_primepower)
  {
    using S = typename R::S;
    const ll n = prime_sum.n();
    if (n == 0) return DirichletPrefixSum<R>::unit(0);
    assert(prime_sum.F(1) == R::e0());
    const int root = int(iroot(n, 2)), cut = int(iroot(n, 6)), cube = int(iroot(n, 3));
    const auto primes = LinearSieve::primes(root);
    const size_t begin = LinearSieve::primes(cube).size<size_t>();
    vc<S> fp(root + 1, R::e0()), fp2(fp);
    for (int p : primes)
    {
      fp[p] = internal::eval_primepower<R>(f_primepower, PrimePower<ll>(p, 1, p));
      fp2[p] = internal::eval_primepower<R>(f_primepower, PrimePower<ll>(p, 2, ll(p) * p));
    }
    const auto sub = [](const S &x, const S &y) { return R::add(x, R::minus(y)); };
    DirichletPrefixSum<R> dp(n, [&](ll x)
    {
      S value = R::e1();
      if (x <= cube) return value;
      value = R::add(value, sub(prime_sum.F(x), prime_sum.F(cube)));
      for (size_t j = begin; j < primes.size<size_t>() && primes[j] <= x / primes[j]; j++)
      {
        const int p = primes[j];
        value = R::add(value, R::add(fp2[p], R::mul(fp[p], sub(prime_sum.F(x / p), prime_sum.F(p)))));
      }
      return value;
    });
    internal::multiplicative_sieve_updates<R> delta(n);
    for (size_t pi = begin; pi > 0;)
    {
      const int p = primes[--pi];
      vc<pair<ll, S>> powers;
      for (ll q = p, e = 1;; q *= p, e++)
      {
        powers.emplace_back(q, internal::eval_primepower<R>(f_primepower, PrimePower<ll>(p, int(e), q)));
        if (q > n / p) break;
      }
      auto update = [&](ll x)
      {
        S value = dp.F(x);
        for (const auto &[q, f] : powers)
        {
          if (q > x) break;
          const ll y = x / q;
          S previous = dp.F(y);
          if (p > cut && y <= delta.bound) previous = R::add(previous, delta[y]);
          value = R::add(value, R::mul(f, previous));
        }
        dp.setF(x, value);
      };
      if (p > cut)
      {
        for (int j = 1; j <= delta.top; j++) update(n / j);
        auto dfs = [&](auto &&self, ll v, size_t i, int e, ll pe, const S &other) -> void
        {
          const int q = primes[i];
          const S f = R::mul(other, e == 1 ? fp[q] :
            internal::eval_primepower<R>(f_primepower, PrimePower<ll>(q, e, pe)));
          delta.add(v, f);
          if (q <= delta.bound / v) self(self, v * q, i, e + 1, pe * q, other);
          for (size_t j = i + 1; j < primes.size<size_t>() && primes[j] <= delta.bound / v; j++)
            self(self, v * primes[j], j, 1, primes[j], f);
        };
        dfs(dfs, p, pi, 1, p, R::e1());
      }
      else
      {
        if (pi + 1 == begin || primes[pi + 1] > cut) delta.apply(dp, R::add);
        for (int j = 1; j <= delta.large; j++) update(n / j);
        for (int x = root; x >= p; x--) update(x);
      }
    }
    if (cut < 2) delta.apply(dp, R::add);
    return DirichletPrefixSum<R>(n, [&](ll x) { return dp.F(x); }, true);
  }

  // modint での実測に基づく目安
  inline constexpr ll multiplicative_prefix_sum_fenwick_threshold = 1'000'000;
} // namespace internal

// 入力: **完全**乗法的関数 g の DirichletPrefixSum
// 出力: g の素数のところだけある関数 g_prime の DirichletPrefixSum
// R は可換環
// 例: DirichletPrefixSum<R> zeta(N, zeta_prefix_sum), id(N, id_prefix_sum);
//     auto pi0 = prime_prefix_sum(zeta);  // pi0.F(x): x 以下の素数の個数
//     auto pi1 = prime_prefix_sum(id);    // pi1.F(x): x 以下の素数の総和
// O(N^{2/3}) 時間
template <class R>
DirichletPrefixSum<R> prime_prefix_sum(const DirichletPrefixSum<R> &g)
{
  return g.n() < internal::multiplicative_prefix_sum_fenwick_threshold
    ? internal::prime_prefix_sum_sieve(g) : internal::prime_prefix_sum_sieve_2_3(g);
}

// 入力: 乗法的関数 f について
//   - f の素数のところだけある関数 f_prime の DirichletPrefixSum
//   - f(p^e) を評価する関数 f_primepower (引数は .p, .e, .pe を持つ)
//
// 出力: f の DirichletPrefixSum
// R は可換環
// 例: auto phi = multiplicative_prefix_sum(pi1 - pi0, totient_primepower);
// ↑ これは φ(p) = p-1 なので
// O(N^{2/3}) 時間
template <class R, class GetPrimePower>
DirichletPrefixSum<R> multiplicative_prefix_sum(const DirichletPrefixSum<R> &f_prime,
  const GetPrimePower &f_primepower)
{
  return f_prime.n() < internal::multiplicative_prefix_sum_fenwick_threshold
    ? internal::multiplicative_prefix_sum_sieve(f_prime, f_primepower)
    : internal::multiplicative_prefix_sum_sieve_2_3(f_prime, f_primepower);
}
