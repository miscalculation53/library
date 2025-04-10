#pragma once

#include "../../template/template_all_but_modint.hpp"

/**
 * @brief 零の重複度つき modint
 * @docs docs/math/modint/modint_div.md
 */

// 有理数を b*p^c (b は分母分子に p がない有理数、c は整数)
// と表したときの b (mod p) と c を保持する
// 掛け算割り算が可能
template <class mint>
struct ModintDiv
{
  mint b;
  ll c;
  ModintDiv() {}
  ModintDiv(mint r)
  {
    if (r == 0)
      b = 1, c = 1;
    else
      b = r, c = 0;
  }
  ModintDiv(mint b, ll c) : b(b), c(c) {}
  ModintDiv &operator*=(const ModintDiv &rhs)
  {
    b *= rhs.b, c += rhs.c;
    return *this;
  }
  ModintDiv &operator/=(const ModintDiv &rhs)
  {
    assert(rhs.b != 0);
    b /= rhs.b, c -= rhs.c;
    return *this;
  }
  ModintDiv inv() const
  {
    assert(b != 0);
    return {b.inv(), -c};
  }
  ModintDiv operator*(const ModintDiv &rhs) const { return ModintDiv(*this) *= rhs; }
  ModintDiv operator/(const ModintDiv &rhs) const { return ModintDiv(*this) /= rhs; }
  // (mod p で表現可能か, mod p での値)
  pair<bool, mint> to_modint() const
  {
    return {c >= 0, c == 0 ? b : mint(0)};
  }
  // r ≠ 0 -> (b, c) = (r, 0)
  // r = 0 -> (b, c) = (1, 1)
  // とする
  friend ModintDiv modint_to_modint_div(const mint &r)
  {
    return r == 0 ? ModintDiv(1, 1) : ModintDiv(r, 0);
  }
};
#ifdef LOCAL
CPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC(b, c);
#endif
