#pragma once

#include "polynomial.generated.hpp"

// Codeforces Gym 102978D: sum_i c[i] * product_{j=0}^{k-1}(a[i]+b[j])。
// 基底変換 -> 多点評価という順方向コードを、自動生成した転置で逆に辿る。
template <class mint>
std::vector<mint> do_use_fft(const std::vector<mint> &a, const std::vector<mint> &b,
                            const std::vector<mint> &c)
{
  const int n = a.size(), m = bit_ceil(n + 1);
  assert(b.size() == a.size() && c.size() == a.size());
  std::vector<std::vector<mint>> pa(2 * m), pb(2 * m);
  for (int i = 0; i < m; ++i)
  {
    pa[m + i] = {i < n ? -a[i] : mint(0), mint(1)};
    pb[m + i] = {i < n ? b[i] : mint(0), mint(1)};
  }
  for (int i = m - 1; i > 0; --i)
  {
    pa[i] = convolution(pa[2 * i], pa[2 * i + 1]);
    pb[i] = convolution(pb[2 * i], pb[2 * i + 1]);
  }
  auto weights = c;
  weights.resize(m);
  auto coefficients = evaluate_tree_transpose<mint>(weights, m, pa, 1, m);
  auto answer = newton_basis_transpose<mint>(coefficients, m, pb, 1);
  return std::vector<mint>(answer.begin() + 1, answer.begin() + n + 1);
}
