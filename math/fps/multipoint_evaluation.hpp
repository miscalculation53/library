#pragma once
#include "fps.hpp"
#include "../convolution/middle_product.hpp"

/**
 * @brief 多項式の多点評価
 * @docs docs/math/fps/multipoint_evaluation.md
 */
namespace internal
{
template <class mint> struct FPSMultipointTree
{
  using F = FormalPowerSeries<mint>;
  int count, base;

  vc<F> product, spectrum;
  explicit FPSMultipointTree(const vc<mint> &xs)
      : count(xs.size()), base(bit_ceil(max(1, count))), product(2 * base), spectrum(2 * base)
  {
    repi(i, base) product[base + i] = {1, i < count ? -xs[i] : mint(0)};
    for (int i = base - 1; i > 0; --i)
    {
      F &a = product[2 * i], &b = product[2 * i + 1];
      int k = a.sz() - 1, z = 2 * k;
      if (k > 32 && ntt_ok<mint>(z) &&
          min(a.sz() - std::count(a.begin(), a.end(), mint(0)),
              b.sz() - std::count(b.begin(), b.end(), mint(0))) > 60)
      {
        F x = a.resized(z), y = b.resized(z);
        ntt(x), ntt(y);
        spectrum[2 * i] = x, spectrum[2 * i + 1] = y;
        repi(j, z) x[j] *= y[j];
        intt(x);
        mint iz = mint(z).inv();
        for (auto &v : x)
          v *= iz;
        x[0] = 1;
        x.push_back(a.back() * b.back());
        product[i] = std::move(x);
      }
      else
        product[i] = a * b;
    }
  }
  vc<mint> evaluate(F f) const
  {
    if (count == 0 || f.empty())
      return vc<mint>(count);
    int n = f.sz();
    F inv = product[1].resized(n).inv(n);
    f.resize(n + base - 1);
    F root = middle_product(f, inv);
    vc<F> value(2 * base);
    value[1] = std::move(root);
    repi(i, 1, base)
    {
      if (!spectrum[2 * i].empty())
      {
        int z = value[i].sz(), k = z / 2;
        F v = std::move(value[i]);
        reverse(v.begin() + 1, v.end());
        ntt(v);
        F left(z), right(z);
        repi(j, z)
        {
          left[j] = v[j] * spectrum[2 * i + 1][j];
          right[j] = v[j] * spectrum[2 * i][j];
        }
        intt(left), intt(right);
        reverse(left.begin() + 1, left.end()), reverse(right.begin() + 1, right.end());
        mint iz = mint(z).inv();
        left.resize(k), right.resize(k);
        for (auto &v : left)
          v *= iz;
        for (auto &v : right)
          v *= iz;
        value[2 * i] = std::move(left), value[2 * i + 1] = std::move(right);
      }
      else
      {
        value[2 * i] = middle_product(value[i], product[2 * i + 1]);
        value[2 * i + 1] = middle_product(value[i], product[2 * i]);
        F().swap(value[i]);
      }
    }
    vc<mint> result(count);
    repi(i, count) result[i] = value[base + i][0];
    return result;
  }
};
} // namespace internal

template <class mint> vc<mint> multipoint_evaluation(const FormalPowerSeries<mint> &f, const vc<mint> &xs)
{
  if (f.sz() <= 64 || xs.size() <= 1)
  {
    vc<mint> res(xs.size());
    repi(i, xs.size()) res[i] = f.eval(xs[i]);
    return res;
  }
  return internal::FPSMultipointTree<mint>(xs).evaluate(f);
}
