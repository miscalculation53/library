#pragma once

#include "../ds/dynamic_bitset.hpp"

/**
 * @brief 部分和問題
 * @docs docs/algo/subset_sum.md
 */

struct SubsetSumFromFrequency
{
private:
  int s;
  vc<pair<int, int>> val_cnt;
  vc<int> time;

public:
  SubsetSumFromFrequency() : s(0), time(1, 0) {}

  // freq[v] := 値 v の個数
  // smax: 計算する部分和の範囲
  template <class T>
  SubsetSumFromFrequency(const vc<T> &freq, int smax) : s(smax)
  {
    static_assert(is_integral_ext<T>);
    assert(s >= 0);
    time.assign(s + 1, -1);
    time[0] = 0;
    DynamicBitset dp(s + 1);
    dp.set(0);
    int vmax = min(s, SZ<int>(freq) - 1);
    repi(v, 1, vmax + 1)
    {
      assert(freq[v] >= 0);
      int c = freq[v] < T(s / v) ? int(freq[v]) : s / v;
      for (ll k = 1; c > 0; k *= 2)
      {
        int x = int(min<ll>(c, k));
        c -= x;
        int t = val_cnt.size();
        val_cnt.eb(v, x);
        int w = v * x;
        dp.or_slice(w, s + 1, dp, 0, [&](int i) { time[i] = t + 1; });
      }
    }
  }

  // x を作れるか
  bool exists(int x) const
  {
    return 0 <= x && x <= s && time[x] != -1;
  }

  // x を作れる場合、second は (値, 個数) の列
  pair<bool, vc<pair<int, int>>> answer(int x) const
  {
    if (!exists(x))
      return {false, {}};

    vc<pair<int, int>> res;
    int y = x;
    repi(t, SZ(val_cnt) - 1, -1, -1)
    {
      auto [v, c] = val_cnt[t];
      int w = v * c;
      if (y >= w && time[y - w] != -1 && time[y - w] <= t)
      {
        y -= w;
        if (res.empty() || res.back().first != v)
          res.eb(v, 0);
        res.back().second += c;
      }
    }
    return {true, res};
  }
};

struct SubsetSum
{
private:
  int n, s, xmax;
  i128 negsum;
  vc<bool> isneg;
  vvc<int> ids;
  SubsetSumFromFrequency ss;

public:
  SubsetSum() : n(0), s(0), xmax(-1), negsum(0) {}

  // smax はクエリで聞かれる target の値の最大値。a は負要素を持ってもよい。
  template <class T>
  SubsetSum(const vc<T> &a, int smax) : n(SZ<int>(a)), xmax(smax), negsum(0)
  {
    static_assert(is_integral_ext<T>);
    isneg.assign(n, false);
    repi(i, n) if (a[i] < 0)
    {
      isneg[i] = true;
      negsum += i128(a[i]);
    }
    i128 shifted_s = i128(smax) - negsum;
    assert(shifted_s <= numeric_limits<int>::max());
    s = shifted_s < 0 ? 0 : int(shifted_s);

    ids.resize(s + 1);
    vc<int> freq(s + 1);
    repi(i, n)
    {
      i128 x = i128(a[i]);
      if (x < 0)
      {
        if (x < -i128(s))
          continue;
        x = -x;
      }
      if (x <= s)
      {
        int v = int(x);
        ids[v].eb(i);
        freq[v]++;
      }
    }
    ss = SubsetSumFromFrequency(freq, s);
  }

  // x を作れるか返す。
  bool exists(int x) const
  {
    i128 y = i128(x) - negsum;
    return x <= xmax && 0 <= y && y <= s && ss.exists(int(y));
  }

  // x を作れるなら、元配列の各要素を使用するか返す。
  pair<bool, vc<bool>> answer(int x) const
  {
    i128 y = i128(x) - negsum;
    if (x > xmax || !(0 <= y && y <= s))
      return {false, {}};
    auto [ok, cnt] = ss.answer(int(y));
    if (!ok)
      return {false, {}};

    vc<bool> res(n, false);
    for (auto [v, c] : cnt)
    {
      assert(c <= SZ(ids[v]));
      repi(k, c) res[ids[v][k]] = true;
    }
    repi(i, n) if (isneg[i]) res[i] = !res[i];
    return {true, res};
  }
};
