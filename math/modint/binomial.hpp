#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "modint.hpp"

/**
 * @brief 二項係数
 * @docs docs/math/modint/binomial.md
 */

template <class T>
struct Binomial
{
private:
  inline static decltype(T::mod()) mod;
  
public:
  inline static vc<T> fac_, finv_, inv_;
  static void reserve(int n)
  {
    if constexpr (is_dynamic_modint_v<T>)
    {
      if (mod != T::mod())
      {
        mod = T::mod();
        fac_ = {1, 1}, finv_ = {1, 1}, inv_ = {0, 1};
      }
    }
    else
    {
      if (fac_.empty())
        fac_ = {1, 1}, finv_ = {1, 1}, inv_ = {0, 1};
    }
    if (n < SZ(fac_))
      return;
    chmin(n, T::mod() - 1);
    int si = fac_.size();
    fac_.resize(n + 1), finv_.resize(n + 1), inv_.resize(n + 1);
    repi(i, si, n + 1)
    {
      fac_[i] = fac_[i - 1] * T::raw(i);
      inv_[i] = -inv_[T::mod() % i] * T::raw(T::mod() / i);
      finv_[i] = finv_[i - 1] * inv_[i];
    }
  }
  static T fac(int n)
  {
    assert(n >= 0);
    if (n >= T::mod())
      return 0;
    reserve(n);
    return fac_[n];
  }
  static T finv(int n)
  {
    assert(n < T::mod());
    if (n < 0)
      return 0;
    reserve(n);
    return finv_[n];
  }
  static T inv(int n)
  {
    n %= T::mod();
    if (n < 0)
      n += T::mod();
    assert(n != 0);
    reserve(n);
    return inv_[n];
  }

  static T P(int n, int k)
  {
    if (n < k)
      return 0;
    if (n < 0 || k < 0)
      return 0;
    if (n >= T::mod())
      return 0;
    reserve(n);
    return fac_[n] * finv_[n - k];
  }
  static T C(int n, int k)
  {
    if (n < k)
      return 0;
    if (n < 0 || k < 0)
      return 0;
    if (n >= T::mod())
      return 0;
    reserve(n);
    return fac_[n] * finv_[k] * finv_[n - k];
  }
  static T H(int n, int k)
  {
    if (n == 0 && k == 0)
      return 1;
    return C(n + k - 1, k);
  }
};
