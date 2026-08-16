#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief 部分和問題
 * @docs docs/algo/subset_sum.md
 */

struct SubsetSum
{
private:
  int n, s, negsum;
  vc<bool> isneg;

  vc<int> nid;  // i 番目の要素を使うかどうかは nid[i] を使うかどうかと一致
  vvc<int> ids; // ids[j] := 値が j となる添字
  vc<int> time; // time[j] := 初めて j が作れるようになった時刻

  template <uint bslen>
  void build(const vc<int> &apos)
  {
    if ((int)bslen <= s)
    {
      build<2 * bslen>(apos);
      return;
    }

    nid = permid<int>(n);
    ids.resize(s + 1);
    repi(i, n) if (apos[i] <= s) ids[apos[i]].eb(i);
    repi(j, s + 1)
    {
      while (SZ(ids[j]) >= 3)
      {
        int i1 = ids[j].back();
        ids[j].pop_back();
        int i2 = ids[j].back();
        ids[j].pop_back();
        if (2 * j <= s)
        {
          nid[i1] = nid[i2] = nid.size();
          ids[2 * j].eb(nid.size());
          nid.eb(nid.size());
        }
      }
    }

    time.assign(s + 1, 2 * s + 3);
    time[0] = 0;
    bitset<bslen> dp, diff;
    dp[0] = 1;
    repi(j, s + 1) repi(k, ids[j].size())
    {
      int t = 2 * j + k + 1;
      diff = (dp << j) & ~dp;
      for (int l = diff._Find_first(); l <= s; l = diff._Find_next(l))
        time[l] = t;
      dp |= dp << j;
    }
  }

public:
  SubsetSum() {}
  // smax はクエリで聞かれる target の値の最大値
  // s := smax - (a の負要素の和) として O(n + min(n,s,√(sum |a_i|))*s/w)
  template <class T>
  SubsetSum(const vc<T> &a, int smax) : n(a.size()), negsum(0)
  {
    isneg.assign(n, false);
    repi(i, n) if (a[i] < 0) isneg[i] = true, negsum += a[i];
    s = max(0, smax - negsum);
    vc<int> apos(n, s + 1);
    repi(i, n)
    {
      if (a[i] < 0 && -a[i] <= s)
        apos[i] = -a[i];
      else if (a[i] >= 0 && a[i] <= s)
        apos[i] = a[i];
    }
    build<1>(apos);
  }

  // x を作れるか
  bool exists(int x)
  {
    ll y = x - negsum;
    if (!(0 <= y && y <= s))
      return false;
    return time[y] < 2 * s + 3;
  }
  // t を作れるか判定し、作れるなら復元
  pair<bool, vc<bool>> answer(int x)
  {
    ll y = x - negsum;
    if (!(0 <= y && y <= s))
      return {false, {}};
    if (time[y] == 2 * s + 3)
      return {false, {}};
    vc<bool> res(nid.size(), false);
    repi(j, y, -1, -1) repi(k, SZ(ids[j]) - 1, -1, -1)
    {
      int i = ids[j][k], t = 2 * j + k + 1;
      if (y >= j && time[y - j] < t)
        y -= j, res[i] = true;
    }
    repi(i, SZ(res) - 1, -1, -1) res[i] = res[nid[i]];
    repi(i, n) if (isneg[i]) res[i] = !res[i];
    return {true, {res.begin(), res.begin() + n}};
  }
};
