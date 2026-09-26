#pragma once

#include "math/linalg/linear_transpose_cpp.hpp"

// S は通常の mint、または線形演算を記録する Value<mint>。
template <class S, class mint>
vc<S> newton_basis_cpp(const vc<S> &x, const vc<vc<mint>> &products, int node = 1)
{
  const int n = x.size();
  if (n == 1) return x;
  auto left = newton_basis_cpp(vc<S>(x.begin(), x.begin() + n / 2), products, 2 * node);
  auto right = newton_basis_cpp(vc<S>(x.begin() + n / 2, x.end()), products, 2 * node + 1);
  // S = Value<mint> のときも、ADL で畳み込みをひとつの演算として記録する。
  auto y = convolution(right, products[2 * node]);
  repi(i, left.size()) y[i] += left[i];
  return y;
}

template <class S, class mint>
vc<S> evaluate_tree_cpp(const vc<S> &x, const vc<vc<mint>> &products, int node, int count)
{
  if (count == 1) return {FormalPowerSeries<S>(x).eval(S(-products[node][0]))};
  auto left = linear_transpose::polynomial_mod(x, products[2 * node]);
  auto right = linear_transpose::polynomial_mod(x, products[2 * node + 1]);
  auto a = evaluate_tree_cpp(left, products, 2 * node, count / 2);
  auto b = evaluate_tree_cpp(right, products, 2 * node + 1, count / 2);
  a.insert(a.end(), b.begin(), b.end());
  return a;
}

template <class mint>
vc<mint> do_use_fft_cpp(const vc<mint> &a, const vc<mint> &b, const vc<mint> &c)
{
  assert(a.size() == b.size() && b.size() == c.size());
  const int n = a.size(), m = bit_ceil(n + 1);
  vc<vc<mint>> pa(2 * m), pb(2 * m);
  repi(i, m)
  {
    pa[m + i] = {i < n ? -a[i] : mint(0), mint(1)};
    pb[m + i] = {i < n ? b[i] : mint(0), mint(1)};
  }
  repi(i, m - 1, 0, -1)
  {
    pa[i] = convolution(pa[2 * i], pa[2 * i + 1]);
    pb[i] = convolution(pb[2 * i], pb[2 * i + 1]);
  }
  auto forward = [&](const auto &u)
  {
    auto h = newton_basis_cpp(u, pb);
    return evaluate_tree_cpp(h, pa, 1, m);
  };
  auto weights = c;
  weights.resize(m);
  auto answer = linear_transpose::transpose<mint>(m, weights, forward);
  return vc<mint>(answer.begin() + 1, answer.begin() + n + 1);
}
