#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief 整数の分割を全探索
 * @docs docs/itertools/partition.md
 */

// 自然数 n の分割すべてを格納した vector を返す
template <class I = ll>
vvc<I> partitions(int n)
{
  vvc<I> res;
  vc<I> a;
  auto dfs = [&](auto dfs) -> void
  {
    if (n == 0)
    {
      res.eb(a);
      return;
    }
    const int m = a.empty() ? n : min(a.back(), I(n));
    repi(i, 1, m + 1)
    {
      n -= i;
      a.eb(i);
      dfs(dfs);
      a.pop_back();
      n += i;
    }
  };
  dfs(dfs);
  return res;
}
