#pragma once

#include "radix_sort.hpp"

/**
 * @brief 安定なソート順の添字列
 * @docs docs/algo/stable_argsort.md
 */

template <class T, class Key>
vc<int> stable_argsort(const vc<T> &a, const Key &key)
{
  using K = decay_t<decltype(key(declval<const T &>()))>;
  if constexpr (is_integral_ext<K> && !is_same_v<K, bool>)
    if (a.size() >= 512)
    {
      if (is_sorted(ALL(a), [&](const auto &x, const auto &y) { return key(x) < key(y); }))
        return permid<int>(a.size());
      return radix_argsort(a, key);
    }
  auto ord = permid<int>(a.size());
  stable_sort(ALL(ord), [&](int i, int j) { return key(a[i]) < key(a[j]); });
  return ord;
}

template <class V>
vc<int> stable_argsort(const V &a)
{
  using T = decay_t<decltype(a[0])>;
  if constexpr (is_same_v<decay_t<V>, vc<T>>)
    return stable_argsort(a, [](const T &x) -> const T & { return x; });
  else
  {
    auto ord = permid<int>(a.size());
    stable_sort(ALL(ord), [&](int i, int j) { return a[i] < a[j]; });
    return ord;
  }
}
