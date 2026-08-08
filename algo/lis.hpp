#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief 最長増加部分列（LIS）
 * @docs docs/algo/lis.md
 */

#include "ds/group_index.hpp"

struct LIS
{
private:
  int n, len;
  vc<int> pos;
  GroupIndex<int> grp;

public:
  LIS() {}
  template <class T, class Compare = less<T>>
  LIS(const vc<T> &a, Compare comp = Compare()) : n(a.size()), pos(n)
  {
    vc<T> dp;
    dp.reserve(n);
    repi(i, n)
    {
      auto it = lower_bound(ALL(dp), a[i], comp);
      pos[i] = it - dp.begin();
      if (it == dp.end())
        dp.eb(a[i]);
      else
        *it = a[i];
    }

    len = dp.size();
    vc<int> candback(len, -1);
    repi(i, n - 1, -1, -1)
    {
      const int j = pos[i];
      if (j == len - 1 || (candback[j + 1] != -1 && comp(a[i], a[candback[j + 1]])))
        candback[j] = i;
      else
        pos[i] += len;
    }

    grp = GroupIndex<int>(pos);
  }

  // LIS の長さ
  template <class I = ll>
  I len_of_lis() { return len; }

  // i 番目の要素について、LIS に使われ得るなら LIS での位置、使われ得ないなら -1
  template <class I = ll>
  I pos_in_lis(int i)
  {
    assert(0 <= i && i < n);
    return pos[i] >= len ? -1 : pos[i];
  }

  // LDS (広義) への分解のひとつにおいて、i 番目の要素は何番目の LDS か
  template <class I = ll>
  I lds_id(int i)
  {
    assert(0 <= i && i < n);
    return pos[i] >= len ? pos[i] - len : pos[i];
  }

  // LIS の j 番目の要素として使われ得る添字たち (昇順)
  auto candidates(int j)
  {
    assert(0 <= j && j < len);
    return grp.idxs(j);
  }
};
