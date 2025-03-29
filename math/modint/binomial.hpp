#pragma once

#include "../../template/template_all_but_modint.hpp"

/**
 * @brief 二項係数
 * @docs docs/math/modint/binomial.md
 */

template <class T>
struct Binomial
{
private:
  static decltype(T::mod()) mod;
  static vc<T> fac_, finv_, inv_;

public:
  static void reserve(int n)
  {
    if (mod != T::mod())
    {
      mod = T::mod();
      fac_ = {1, 1}, finv_ = {1, 1}, inv_ = {0, 1};
    }
    int i = fac_.size();
    chmin(n, T::mod() - 1);
    if (n < i)
      return;
    fac_.resize(n + 1), finv_.resize(n + 1), inv_.resize(n + 1);
    for (; i <= n; i++)
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
  static T inv(T n)
  {
    assert(n != 0);
    reserve(n.val());
    return inv_[n.val()];
  }

  static T P(int n, int k)
  {
    if (n < k)
      return 0;
    if (n < 0 || k < 0)
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
template <class T> decltype(T::mod()) Binomial<T>::mod{};
template <class T> vc<T> Binomial<T>::fac_{};
template <class T> vc<T> Binomial<T>::finv_{};
template <class T> vc<T> Binomial<T>::inv_{};