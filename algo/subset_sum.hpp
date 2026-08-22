#pragma once

#include "../ds/dynamic_bitset.hpp"

/**
 * @brief 部分和問題
 * @docs docs/algo/subset_sum.md
 */

struct SubsetSumFromFrequency
{
private:
  struct Node
  {
    int l, r, val, cnt;
  };

  struct Item
  {
    int w, node;
  };

  int s;
  vc<Node> nodes;
  vc<Item> items;
  vc<int> time, used;

  void build_dp()
  {
    time.assign(s + 1, SZ<int>(items) + 1);
    time[0] = 0;
    DynamicBitset dp(s + 1);
    dp.set(0);
    repi(t, SZ(items))
      dp.or_shift_left(items[t].w, [&](int i) { time[i] = t + 1; });
  }

  template <class T>
  void build(const vc<T> &freq)
  {
    static_assert(is_integral_ext<T>);
    vvc<int> ids(s + 1);
    int vmax = min(s, SZ<int>(freq) - 1);
    repi(v, 1, vmax + 1)
    {
      assert(freq[v] >= 0);
      int c = freq[v] < T(s / v) ? int(freq[v]) : s / v;
      for (ll k = 1; c > 0; k *= 2)
      {
        int x = int(min<ll>(c, k));
        c -= x;
        ids[v * x].eb(nodes.size());
        nodes.eb(Node{-1, -1, v, x});
      }
    }

    repi(w, 1, s + 1)
    {
      while (SZ(ids[w]) >= 3)
      {
        int l = ids[w].back();
        ids[w].pop_back();
        int r = ids[w].back();
        ids[w].pop_back();
        if (w <= s / 2)
        {
          ids[2 * w].eb(nodes.size());
          nodes.eb(Node{l, r, 0, 0});
        }
      }
      fec(node : ids[w]) items.eb(Item{w, node});
    }

    build_dp();
    used.assign(s + 1, 0);
  }

public:
  SubsetSumFromFrequency() : s(0), time(1, 0), used(1, 0) {}

  // freq[v] := 値 v の個数。smax 以下の部分和を前計算する。
  template <class T>
  SubsetSumFromFrequency(const vc<T> &freq, int smax) : s(smax)
  {
    assert(s >= 0);
    build(freq);
  }

  // x を作れるか返す。
  bool exists(int x) const
  {
    return 0 <= x && x <= s && time[x] <= SZ(items);
  }

  // x を作れるなら、使用する (値, 個数) の列を返す。列の順序は未規定。
  pair<bool, vc<pair<int, int>>> answer(int x)
  {
    if (!exists(x))
      return {false, {}};

    vc<int> selected;
    int y = x;
    repi(t, SZ(items) - 1, -1, -1)
    {
      int w = items[t].w;
      if (y >= w && time[y - w] <= t)
      {
        y -= w;
        selected.eb(items[t].node);
      }
    }

    vc<int> touched;
    while (!selected.empty())
    {
      int i = selected.back();
      selected.pop_back();
      if (nodes[i].l == -1)
      {
        if (used[nodes[i].val] == 0)
          touched.eb(nodes[i].val);
        used[nodes[i].val] += nodes[i].cnt;
      }
      else
      {
        selected.eb(nodes[i].l);
        selected.eb(nodes[i].r);
      }
    }

    vc<pair<int, int>> res;
    fec(v : touched) res.eb(v, used[v]), used[v] = 0;
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
  pair<bool, vc<bool>> answer(int x)
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
