#pragma once

#include "../ds/dynamic_bitset.hpp"

/**
 * @brief 部分和問題
 * @docs docs/algo/subset_sum.md
 */

struct SubsetSum;

struct SubsetSumFromFrequency
{
private:
  friend struct SubsetSum;

  int s;
  vc<pair<int, int>> val_cnt;
  DynamicBitset dp;
  vc<int> time;
  vc<DynamicBitset> history;
  bool keep_history;

  void build(const vc<pair<int, int>> &freq, int smax)
  {
    s = smax;
    assert(s >= 0);
    val_cnt.clear();
    history.clear();

    ll sm = 0;
    for (auto [v, c] : freq)
    {
      assert(v > 0 && c >= 0);
      if (v > s)
        continue;
      chmin(c, s / v);
      sm = min<ll>(s, sm + ll(v) * c);
    }
    s = int(sm);

    dp.resize(s + 1);
    dp.reset();
    dp.set(0);
    for (auto [v, c] : freq)
    {
      if (v > s)
        continue;
      chmin(c, s / v);
      for (ll k = 1; c > 0; k *= 2)
      {
        int x = int(min<ll>(c, k));
        c -= x;
        val_cnt.eb(v, x);
      }
    }

    ll bitset_bytes = ll((s + DynamicBitset::word_bits) / DynamicBitset::word_bits) * ll(sizeof(DynamicBitset::Word));
    keep_history = ll(val_cnt.size()) * bitset_bytes < ll(s + 1) * ll(sizeof(int));
    if (keep_history)
      history.reserve(val_cnt.size());
    else
    {
      time.assign(s + 1, -1);
      time[0] = 0;
    }

    int hi = 0;
    repi(t, SZ(val_cnt))
    {
      auto [v, c] = val_cnt[t];
      int w = v * c;
      int r = min(s, hi + w) + 1;
      if (keep_history)
      {
        dp.or_slice(w, r, dp, 0);
        history.eb(dp);
      }
      else
        dp.or_slice(w, r, dp, 0, [&](int i) { time[i] = t + 1; });
      hi = r - 1;
      if (hi == s && (t & 63) == 63 && dp.all())
      {
        val_cnt.resize(t + 1);
        if (keep_history)
          history.resize(t + 1);
        break;
      }
    }
  }

public:
  SubsetSumFromFrequency() : s(0), dp(1), time(1, 0), keep_history(false) { dp.set(0); }

  // freq[v] := 値 v の個数
  // smax: 計算する部分和の範囲
  template <class T>
  SubsetSumFromFrequency(const vc<T> &freq, int smax)
  {
    static_assert(is_integral_ext<T>);
    assert(smax >= 0);
    vc<pair<int, int>> f;
    int vmax = min(smax, SZ<int>(freq) - 1);
    repi(v, 1, vmax + 1)
    {
      assert(freq[v] >= 0);
      int c = freq[v] < T(smax / v) ? int(freq[v]) : smax / v;
      if (c)
        f.eb(v, c);
    }
    build(f, smax);
  }

  // x を作れるか
  bool exists(int x) const
  {
    return 0 <= x && x <= s && dp.test(x);
  }

  // x 番目: x を作れるか
  const DynamicBitset &reachable() const { return dp; }

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
      bool can = false;
      if (y >= w)
      {
        if (keep_history)
          can = t == 0 ? y == w : history[t - 1].test(y - w);
        else
          can = time[y - w] != -1 && time[y - w] <= t;
      }
      if (can)
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
  ll negsum;
  vc<bool> isneg;
  vc<pair<int, int>> elems;
  SubsetSumFromFrequency ss;

public:
  SubsetSum() : n(0), s(-1), xmax(-1), negsum(0) {}

  // smax はクエリで聞かれる target の値の最大値
  // a は負要素を持ってもよい
  template <class T>
  SubsetSum(const vc<T> &a, int smax) : n(SZ<int>(a)), xmax(smax), negsum(0)
  {
    static_assert(is_integral_ext<T>);
    isneg.assign(n, false);
    repi(i, n) if (a[i] < 0)
    {
      isneg[i] = true;
      negsum += ll(a[i]);
    }
    ll shifted_s = ll(smax) - negsum;
    assert(shifted_s <= numeric_limits<int>::max());
    if (shifted_s < 0)
    {
      s = -1;
      return;
    }

    ll sm = 0;
    fec(a_i : a)
    {
      ll x = ll(a_i);
      if (x < 0) x = -x;
      if (x <= shifted_s)
        sm = min(shifted_s, sm + x);
    }
    s = int(sm);

    repi(i, n)
    {
      ll x = ll(a[i]);
      if (x < 0)
      {
        if (x < -ll(s))
          continue;
        x = -x;
      }
      if (x <= s)
      {
        int v = int(x);
        if (v)
          elems.eb(v, i);
      }
    }
    sort(elems.begin(), elems.end());
    vc<pair<int, int>> freq;
    for (auto [v, i] : elems)
    {
      if (freq.empty() || freq.back().first != v)
        freq.eb(v, 0);
      ++freq.back().second;
    }
    ss.build(freq, s);
    s = ss.reachable().size() - 1;
  }

  // x を作れるか
  bool exists(int x) const
  {
    ll y = ll(x) - negsum;
    return x <= xmax && 0 <= y && y <= s && ss.exists(int(y));
  }

  // x 番目: min_sum() + x を作れるか
  const DynamicBitset &reachable() const
  {
    static const DynamicBitset empty;
    return s < 0 ? empty : ss.reachable();
  }

  // 作れる和の最小値
  ll min_sum() const { return negsum; }

  // x を作れる場合、second の i 番目: a[i] を使うかどうか
  pair<bool, vc<bool>> answer(int x) const
  {
    ll y = ll(x) - negsum;
    if (x > xmax || !(0 <= y && y <= s))
      return {false, {}};
    auto [ok, cnt] = ss.answer(int(y));
    if (!ok)
      return {false, {}};

    vc<bool> res(n, false);
    int p = elems.size();
    for (auto [v, c] : cnt)
    {
      while (p && elems[p - 1].first > v) p--;
      int q = p;
      while (q && elems[q - 1].first == v) q--;
      assert(p - q >= c);
      repi(k, c) res[elems[q + k].second] = true;
      p = q;
    }
    repi(i, n) if (isneg[i]) res[i] = !res[i];
    return {true, res};
  }
};
