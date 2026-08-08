#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief $2$ 次元グリッドの回転
 * @docs docs/algo/rot90.md
 */

// 12
// 34 -> 246
// 56    135
// (反時計回り)
template <class VV, class U = ll>
VV rot90(const VV &a, U k = 1)
{
  if (a.empty())
    return {};
  const int n = a.size(), m = a[0].size();
  k = (k % 4 + 4) % 4;
  if (k == 0)
    return a;
  else if (k == 1)
  {
    VV b(m);
    repi(j, m) b[j].resize(n);
    repi(i, n)
    {
      assert(SZ<int>(a[i]) == m);
      repi(j, m) b[m - 1 - j][i] = a[i][j];
    }
    return b;
  }
  else if (k == 2)
  {
    VV b(n);
    repi(i, n) b[i].resize(m);
    repi(i, n)
    {
      assert(SZ<int>(a[i]) == m);
      repi(j, m) b[n - 1 - i][m - 1 - j] = a[i][j];
    }
    return b;
  }
  else
  {
    VV b(m);
    repi(j, m) b[j].resize(n);
    repi(i, n)
    {
      assert(SZ<int>(a[i]) == m);
      repi(j, m) b[j][n - 1 - i] = a[i][j];
    }
    return b;
  }
}
