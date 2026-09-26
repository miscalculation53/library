#pragma once

#include "../template/template_all_but_modint.hpp"
#include "../utils/make_unsigned_ext.hpp"

/**
 * @brief 基数ソート
 * @docs docs/algo/radix_sort.md
 */

// key(a[i]) の昇順に並べた添字列。同じキーでは元の順序を保つ。
template <class T, class Key>
vc<int> radix_argsort(const vc<T> &a, const Key &key)
{
  using K = decay_t<decltype(key(declval<const T &>()))>;
  static_assert(is_integral_ext<K> && !is_same_v<K, bool>);
  using U = make_unsigned_ext_t<K>;
  constexpr int width = sizeof(U) * CHAR_BIT;
  const U sign = is_signed_ext<K> ? U(1) << (width - 1) : U(0);
  const int n = a.size();
  vc<int> ord = permid<int>(n);
  if (n <= 1)
    return ord;

  vc<U> keys(n);
  U varying = 0;
  repi(i, n)
  {
    keys[i] = U(key(a[i])) ^ sign;
    varying |= keys[i] ^ keys[0];
  }
  if (varying == 0)
    return ord;

  // 小さい列は安定な挿入ソートで処理する。
  if (n <= 32)
  {
    repi(i, 1, n)
    {
      int x = ord[i], j = i;
      while (j > 0 && keys[x] < keys[ord[j - 1]])
        ord[j] = ord[j - 1], j--;
      ord[j] = x;
    }
    return ord;
  }

  const int bits = min(width, n < (1 << 16) ? 8 : 16);
  const int buckets = 1 << bits, mask = buckets - 1;
  vc<int> tmp(n), cnt(buckets);
  for (int shift = 0; shift < width; shift += bits)
  {
    // 全要素で等しい桁は順序を変えない。
    if (((varying >> shift) & mask) == 0)
      continue;
    fill(ALL(cnt), 0);
    for (U x : keys)
      cnt[(x >> shift) & mask]++;
    int sum = 0;
    for (int &c : cnt)
    {
      int count = c;
      c = sum;
      sum += count;
    }
    for (int i : ord)
      tmp[cnt[(keys[i] >> shift) & mask]++] = i;
    ord.swap(tmp);
  }
  return ord;
}

template <class I>
vc<int> radix_argsort(const vc<I> &a)
{
  return radix_argsort(a, [](const I &x) { return x; });
}

// key(a[i]) の昇順に安定ソートする。
template <class T, class Key>
void radix_sort(vc<T> &a, const Key &key)
{
  auto ord = radix_argsort(a, key);
  // 巡回置換ごとに移動し、要素型のデフォルト構築を省く。
  repi(i, a.size())
  {
    if (ord[i] == i)
      continue;
    T saved = move(a[i]);
    int j = i;
    while (ord[j] != i)
    {
      int next = ord[j];
      a[j] = move(a[next]);
      ord[j] = j;
      j = next;
    }
    a[j] = move(saved);
    ord[j] = j;
  }
}

template <class I>
void radix_sort(vc<I> &a)
{
  radix_sort(a, [](const I &x) { return x; });
}
