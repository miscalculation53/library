#pragma once

#include "math/prime/sieve/enumerate_multiplicative.hpp"

/**
 * @brief Powerful number 篩による乗法的関数の和
 * @docs docs/math/prime/sieve/powerful_number_sieve.md
 */

// 入力: 乗法的関数 g, h であって h(p) = 0 を満たすものに対する
// - getG: G(x) の値を得る関数 (x = ⌊n / i⌋ に対してのみ呼ばれる)
// - h_primepower: h(p^e) の値を得る関数
//
// 出力: f = g * h の F(N) 一点
// すべてが O(1) 時間のとき、O(√N) 時間
// getG(x) が O(x^a) 時間のとき、a<1/2 なら O(√N) 時間、a=1/2 なら O(√N log N) 時間、a>1/2 なら O(N^a) 時間
// R は可換環
template <class R, class GetG, class GetPrimePower>
typename R::S powerful_number_sieve(ll n, const GetG &getG, const GetPrimePower &h_primepower)
{
  using S = typename R::S;
  assert(n >= 0);
  if (n == 0) return R::e0();
  const int root = int(iroot(n, 2));
  const auto primes = LinearSieve::primes(root);
  const size_t count = primes.size<size_t>();
  vc<int> offset(count);
  vc<S> values;
  for (size_t i = 0; i < count; i++)
  {
    const ll p = primes[i];
    offset[i] = int(values.size());
    for (ll pe = p * p, e = 2; ; pe *= p, e++)
    {
      values.push_back(internal::eval_primepower<R>(h_primepower, PrimePower<ll>(p, int(e), pe)));
      if (pe > n / p) break;
    }
  }
  S answer = R::e0();
  auto dfs = [&](auto &&self, ll limit, size_t first, const S &weight) -> void
  {
    const S prefix = [&]() -> S
    {
      if constexpr (is_invocable_v<const GetG &, ll>) return getG(limit);
      else return getG(limit, R{});
    }();
    answer = R::add(answer, R::mul(weight, prefix));
    for (size_t i = first; i < count; i++)
    {
      const ll p = primes[i], square = p * p;
      if (square > limit) break;
      int at = offset[i];
      for (ll rest = limit / square; rest > 0; rest /= p, at++)
      {
        const S next = R::mul(weight, values[at]);
        if (next != R::e0()) self(self, rest, i + 1, next);
        // この指数が零でも、同じ素数のより大きな指数は調べる。
      }
    }
  };
  dfs(dfs, n, 0, R::e1());
  return answer;
}
