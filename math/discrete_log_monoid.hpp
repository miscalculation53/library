#pragma once

#include "../template/template_all_but_modint.hpp"
#include "../ds/hash_set.hpp"

/**
 * @brief モノイドとモノイド作用の離散対数
 * @docs docs/math/discrete_log_monoid.md
 */

//! https://maspypy.com/モノイド作用に関する離散対数問題

// M はモノイド、S は集合、M が S に作用
// 0 <= l <= r として、l <= n < r で act(a^n, s) == t となる最小の n
// 計算量: O(√(r - l)) 回の作用、累乗に O(log r) 回のモノイド積（二分累乗の場合）
template <class M, class S, class Act, class Hash = safe_hash, class Equal = equal_to<S>>
optional<ll> discrete_log_action(const typename M::S &a, S s, const S &t, ll l, ll r,
                                 Act &&act, Hash hash = {}, Equal equal = {})
{
  assert(0 <= l && l <= r);
  if (l == r) return nullopt;
  if (l == 1) s = act(a, std::as_const(s));
  else if (l > 1)
  {
    const auto shift = pow_monoid<M>(a, l);
    s = act(shift, std::as_const(s));
  }
  if (equal(s, t)) return l;

  const ll width = r - l;
  const ll block = iroot<ll>(width - 1, 2) + 1;
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
  for (ll base = 0; base < width; base += block)
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
      const ll length = min(block, width - base);
      for (ll j = 0; j < length; j++)
      {
        if (equal(current, t)) return l + base + j;
        current = act(a, std::as_const(current));
      }
      if (++misses == 2) return nullopt;
    }
    s = std::move(next);
  }
  return nullopt;
}

// 0 <= l <= r として、l <= n < r で a^n == b となる最小の n
// 計算量: O(log r + √(r - l)) 回のモノイド積（二分累乗の場合）
template <class M, class Hash = safe_hash, class Equal = equal_to<typename M::S>>
optional<ll> discrete_log_monoid(const typename M::S &a, const typename M::S &b, ll l, ll r,
                                 Hash hash = {}, Equal equal = {})
{
  return discrete_log_action<M>(a, M::e(), b, l, r,
      [](const typename M::S &g, const typename M::S &s) { return M::op(g, s); },
      std::move(hash), std::move(equal));
}
