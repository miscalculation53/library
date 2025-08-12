#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "convolution.hpp"

/**
 * @brief $2$ 倍の長さの NTT
 * @docs docs/math/convolution/ntt_doubling.md
 */

// a はある列 b を ntt したものであるとする
// b を長さ 2 倍にしてゼロ埋めした列の ntt を a に代入する (破壊的変更)
template <class mint>
void ntt_doubling(vc<mint> &a)
{
  static const internal::fft_info<mint> info;
  const int z = a.size(), lg = bit_width(z) - 1;
  assert(z == (1 << lg));
  vc<mint> b = a;
  intt(b);
  const mint iz = mint(z).inv();
  fem(bi : b) bi *= iz;
  vc<mint> c(z);
  mint root = info.root[lg + 1], pw = 1;
  repi(i, b.size()) c[i] = pw * b[i], pw *= root;
  ntt(c);
  a.insert(a.end(), c.begin(), c.end());
}
