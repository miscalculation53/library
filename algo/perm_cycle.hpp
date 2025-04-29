#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief 順列のサイクル分解
 * @docs docs/algo/perm_cycle.md
 */

// 順列をサイクル分解したもの
template <class I>
struct PermCycle
{
  // i 番目の要素の (サイクルの番号, サイクル内での順番)
  vc<pair<I, I>> cycle_id;
  vvc<I> cycles;

  PermCycle() {}
  PermCycle(const vc<I> &p)
  {
    assert(is_permutation(p));
    const int n = p.size();
    cycle_id.resize(n);
    vc<bool> visited(n, false);
    repi(si, n)
    {
      if (visited[si])
        continue;
      int i = si;
      cycles.eb();
      do
      {
        visited[i] = true;
        cycles.back().eb(i);
        cycle_id[i] = {SZ<int>(cycles) - 1, SZ<int>(cycles.back()) - 1};
        i = p[i];
      } while (i != si);
    }
  }

  // p^k[i] を返す
  I get_pow(ll k, int i)
  {
    cauto & [ j, l ] = cycle_id[i];
    const int len = cycles[j].size();
    return cycles[j][safemod(l + k, len)];
  }
};

// p^k を返す
template <class I>
vc<I> perm_pow(const vc<I> &p, ll k)
{
  const int n = p.size();
  PermCycle cy(p);
  vc<I> q(n);
  repi(i, n) q[i] = cy.get_pow(k, i);
  return q;
}
