#pragma once

#include "../../template/template_all_but_modint.hpp"
#include "modint.hpp"
#include "modint64.hpp"
#include "../prime/large/primality_test.hpp"

/**
 * @brief mod 素数 の平方根
 * @docs docs/math/modint/sqrt_mod.md
 */

// 平方根が存在しなければ (false, 0)
// 平方根が存在すれば (true, 平方根のひとつ)
// 平方根のもうひとつは -1 倍したもの
// O(log^2 p)
template <class mint>
pair<bool, mint> sqrt_mod(mint n)
{
  auto p = mint::mod();
  assert(is_prime(mint::mod()));
  if (n == 0)
    return {true, 0};
  if (p == 2)
    return {true, n};
  if (n.pow((p - 1) / 2) == -1)
    return {false, 0};
  auto q = p - 1;
  int s = 0;
  while (q % 2 == 0)
    q /= 2, s++;
  mint z;
  do
  {
    z = randrange(0, p);
  } while (z.pow((p - 1) / 2) != -1);
  int m = s;
  mint c = z.pow(q), t = n.pow(q), r = n.pow((q + 1) / 2);
  while (t != 1)
  {
    int m2 = 1;
    for (mint tmp = t * t; tmp != 1; tmp *= tmp, m2++)
      ;
    mint b = c.pow(1 << (m - m2 - 1));
    m = m2, c = b * b, t *= c, r *= b;
  }
  return {true, r};
}
