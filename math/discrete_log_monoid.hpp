#pragma once

#include "../template/template_all_but_modint.hpp"
#include "../ds/hash_set.hpp"

/**
 * @brief モノイドとモノイド作用の離散対数
 * @docs docs/math/discrete_log_monoid.md
 */

//! https://maspypy.com/モノイド作用に関する離散対数問題

// M はモノイド、S は集合、M が S に作用
// 0 <= n < limit で act(a^n, s) == t となる最小の n
// 計算量: O(√limit) 回の作用、O(log N) 回のモノイド積
template <class M, class S, class Act, class Hash = safe_hash, class Equal = equal_to<S>>
optional<ll> discrete_log_action(const typename M::S &a, S s, const S &t, ll limit,
                                 Act &&act, Hash hash = {}, Equal equal = {})
{
  assert(limit >= 0);
  if (limit == 0) return nullopt;
  if (equal(s, t)) return 0;

  const ll block = iroot<ll>(limit - 1, 2) + 1;
  const auto jump = pow_monoid<M>(a, block);
  constexpr bool use_hash_set = is_default_constructible_v<S> && is_copy_assignable_v<S> && !is_same_v<S, bool>;
  auto target_steps = [&]
  {
    if constexpr (use_hash_set)
      return HashSet<S, Hash, Equal>(hash, equal);
    else
      return unordered_set<S, Hash, Equal>(0, hash, equal);
  }();
  target_steps.reserve(block);
  S y = t;
  for (ll j = 0; j < block; j++)
  {
    y = act(a, std::as_const(y));
    target_steps.insert(y);
  }

  int misses = 0;
  for (ll base = 0; base < limit; base += block)
  {
    S next = act(jump, std::as_const(s));
    const bool hit = [&]
    {
      if constexpr (use_hash_set)
        return target_steps.contains(next);
      else
        return target_steps.find(next) != target_steps.end();
    }();
    if (hit)
    {
      S current = s;
      const ll length = min(block, limit - base);
      for (ll j = 0; j < length; j++)
      {
        if (equal(current, t)) return base + j;
        current = act(a, std::as_const(current));
      }
      // 最初の衝突以降に t へ到達できるなら t は周期上にある。
      // その場合、次の衝突ブロックには必ず t が現れる。
      if (++misses == 2) return nullopt;
    }
    s = std::move(next);
  }
  return nullopt;
}

// 0 <= n < limit で a^n == b となる最小の n
// 計算量: O(√limit) 回のモノイド積
template <class M, class Hash = safe_hash, class Equal = equal_to<typename M::S>>
optional<ll> discrete_log_monoid(const typename M::S &a, const typename M::S &b, ll limit,
                                 Hash hash = {}, Equal equal = {})
{
  return discrete_log_action<M>(a, M::e(), b, limit,
      [](const typename M::S &g, const typename M::S &s) { return M::op(g, s); },
      std::move(hash), std::move(equal));
}
