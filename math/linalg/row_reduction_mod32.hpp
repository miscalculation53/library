#pragma once

#include "../modint/internal_mod32.hpp"

namespace internal
{
// 通常の modint32 の体に対する掃き出し。a は出力も兼ねる。
// mod + block * (mod - 1)^2 < 2^64 の範囲で行更新の剰余を遅らせる。
// ピボットの参照時・行の正規化時・返却時には必ず通常の代表元へ戻す。
template <class S, class Rows>
pair<int, S> row_reduction_mod32(Rows &a, int n, int m, bool rref)
{
  if (n == 0 || m == 0) return {0, S(1)};
  if (none_of(a.begin(), a.end(), [](const auto &row)
      { return any_of(row.begin(), row.end(), [](S x) { return x.val() != 0; }); }))
    return {0, S(0)};
  const ull mod = S::mod();
  const int block = mod <= (1u << 30) ? 16 : 4;
  vvc<ull> w(n, vc<ull>(m));
  vc<int> used(n);
  // first[k] より左は零（ピボットより下の行）。零行は m として以後飛ばす。
  vc<int> first(n);
  repi(i, n) repi(j, m) w[i][j] = a[i][j].val();
  int rk = 0;
  S de = 1;
  for (int i = 0, j = 0; i < n && j < m; ++j)
  {
    int k = i, next = m;
    for (; k < n; ++k)
    {
      while (first[k] < m && (w[k][first[k]] %= mod) == 0) ++first[k];
      if (first[k] == j) break;
      next = min(next, first[k]);
    }
    if (k == n)
    {
      de = 0;
      if (next == m) break;
      j = next - 1;
      continue;
    }
    if (k != i)
    {
      swap(w[k], w[i]);
      swap(used[k], used[i]);
      swap(first[k], first[i]);
      de = -de;
    }
    S pivot = S::raw(w[i][j]);
    de *= pivot;
    S inv = pivot.inv();
    repi(l, j, m) w[i][l] = (S::raw(w[i][l] % mod) * inv).val();
    used[i] = 0;
    repi(k, rref ? 0 : i + 1, n) if (k != i)
    {
      if (first[k] > j) continue;
      ull c = w[k][j] % mod;
      w[k][j] = 0;
      if (k > i) first[k] = j + 1;
      if (c == 0) continue;
      if (used[k] == block)
      {
        // j より左は以降更新しないので、残りの列だけを剰余に戻せばよい。
        repi(l, j + 1, m) w[k][l] %= mod;
        used[k] = 0;
      }
      c = mod - c;
      repi(l, j + 1, m) w[k][l] += c * w[i][l];
      ++used[k];
    }
    ++i;
    ++rk;
  }
  repi(i, n) repi(j, m) a[i][j] = S::raw(first[i] == m ? 0 : w[i][j] % mod);
  return {rk, de};
}
}
