#pragma once

#include "template/template_all_but_modint.hpp"
#include "math/prime/sieve/enumerate_multiplicative.hpp"
#include "math/modint/modint.hpp"
#include "math/modint/inv_many.hpp"

/**
 * @brief Dirichlet 級数（先頭項を持つ）
 * @docs docs/math/prime/sieve/dirichlet_convolution.md
 */

namespace internal
{
  template <class R, class = void>
  struct dirichlet_has_inv : false_type {};
  template <class R>
  struct dirichlet_has_inv<R, void_t<decltype(R::inv(declval<const typename R::S &>()))>> : true_type {};

  // 要素型が modint でも、独自の環演算なら特殊化しない。
  template <class R>
  struct dirichlet_modint_field : false_type {};
  template <class S>
  struct dirichlet_modint_field<FieldAddSubMulDiv<S>> : is_modint<S> {};

  template <class R>
  vc<typename R::S> dirichlet_integers(int n)
  {
    vc<typename R::S> res(n + 1, R::e0());
    for (int i = 1; i <= n; i++)
      res[i] = R::add(res[i - 1], R::e1());
    return res;
  }

  // 1, ..., n はすべて可逆。R ごとに保持し、不足分だけ inv_many で追加する。
  // 同じ範囲の再利用では逆元計算をしない。参照先は法変更・伸長時に更新される。
  template <class R>
  const vc<typename R::S> &dirichlet_integer_inverses(int n)
  {
    static_assert(dirichlet_has_inv<R>::value, "R::inv is required");
    using S = typename R::S;
    assert(n >= 0);
    static vc<S> inverse{R::e0(), R::e1()};
    static S last_integer = R::e1();
    if constexpr (has_mod<R>::value || has_mod<S>::value)
    {
      const auto current_mod = []
      {
        if constexpr (has_mod<R>::value) return R::mod();
        else return S::mod();
      }();
      static auto cached_mod = current_mod;
      if (cached_mod != current_mod)
      {
        cached_mod = current_mod;
        inverse = {R::e0(), R::e1()};
        last_integer = R::e1();
      }
    }
    if (n < int(inverse.size())) return inverse;
    vc<S> added;
    added.reserve(n + 1 - inverse.size());
    S current = last_integer;
    for (int i = int(inverse.size()); i <= n; i++)
    {
      current = R::add(current, R::e1());
      assert(current != R::e0());
      added.push_back(current);
    }
    const auto added_inverse = inv_many<R>(added);
    inverse.insert(inverse.end(), added_inverse.begin(), added_inverse.end());
    last_integer = current;
    return inverse;
  }

  template <class R>
  struct dirichlet_divisor
  {
    using S = typename R::S;
    static constexpr bool exact_integer = is_integral_ext<S> && is_same_v<R, RingAddSubMul<S>>;
    S value;
    bool identity;
    explicit dirichlet_divisor(const S &a) : value(a), identity(a == R::e1())
    {
      if (identity)
        return;
      assert(a != R::e0());
      if constexpr (dirichlet_has_inv<R>::value)
        value = R::inv(a);
      else if constexpr (!exact_integer)
        assert(a == R::minus(R::e1()));
    }
    S operator()(const S &a) const
    {
      if (identity)
        return a;
      if constexpr (dirichlet_has_inv<R>::value)
        return R::mul(a, value);
      else if constexpr (exact_integer)
        return a / value; // 通常の整数環では、割り切れる商だけを扱う。
      else
        return R::minus(a); // inv を持たない一般の環では、除数は ±1。
    }
  };
}

// f(1), ..., f(n) を保持する
template <class R>
struct DirichletSeries
{
  using S = typename R::S;

private:
  int n_;
  vc<S> f_;
  bool multiplicative_;

public:
  explicit DirichletSeries(int n = 0) : n_(n), multiplicative_(false)
  {
    assert(0 <= n);
    f_.assign(n + 1, R::e0());
  }

  explicit DirichletSeries(vc<S> values, bool multiplicative = false)
    : n_(0), f_(move(values)), multiplicative_(multiplicative)
  {
    if (f_.empty())
      f_.push_back(R::e0());
    n_ = int(f_.size()) - 1;
    f_[0] = R::e0();
    assert(!multiplicative_ || n_ == 0 || f_[1] == R::e1());
  }

  template <class F>
  DirichletSeries(int n, const F &f_primepower, bool completely_multiplicative = false)
    : DirichletSeries(completely_multiplicative
        ? enumerate_completely_multiplicative<R>(n, f_primepower)
        : enumerate_multiplicative<R>(n, f_primepower), true) {}

  static DirichletSeries unit(int n)
  {
    DirichletSeries res(n);
    if (n >= 1)
      res.f_[1] = R::e1();
    res.multiplicative_ = true;
    return res;
  }

  int n() const { return n_; }
  bool is_multiplicative() const { return multiplicative_; }
  const S &f(int i) const
  {
    assert(0 <= i && i <= n_);
    return f_[i];
  }
  void set_f(int i, const S &value)
  {
    assert(1 <= i && i <= n_);
    f_[i] = value;
    multiplicative_ = false;
  }
  vc<S> to_vector() const & { return f_; }
  vc<S> to_vector() &&
  {
    vc<S> res{R::e0()};
    res.swap(f_);
    n_ = 0;
    multiplicative_ = false;
    return res;
  }
  vc<S> content() const { return to_vector(); }

  DirichletSeries operator-() const
  {
    DirichletSeries res(*this);
    for (int i = 1; i <= n_; i++)
      res.f_[i] = R::minus(res.f_[i]);
    res.multiplicative_ = false;
    return res;
  }
  DirichletSeries &operator+=(const DirichletSeries &g)
  {
    assert(n_ == g.n_);
    for (int i = 1; i <= n_; i++)
      f_[i] = R::add(f_[i], g.f_[i]);
    multiplicative_ = false;
    return *this;
  }
  DirichletSeries &operator-=(const DirichletSeries &g)
  {
    assert(n_ == g.n_);
    for (int i = 1; i <= n_; i++)
      f_[i] = R::add(f_[i], R::minus(g.f_[i]));
    multiplicative_ = false;
    return *this;
  }
  DirichletSeries &operator*=(const S &a)
  {
    const S value = a;
    for (int i = 1; i <= n_; i++)
      f_[i] = R::mul(f_[i], value);
    multiplicative_ = false;
    return *this;
  }
  DirichletSeries &operator/=(const S &a)
  {
    const internal::dirichlet_divisor<R> divide(a);
    for (int i = 1; i <= n_; i++)
      f_[i] = divide(f_[i]);
    multiplicative_ = false;
    return *this;
  }
  DirichletSeries operator+(const DirichletSeries &g) const { return DirichletSeries(*this) += g; }
  DirichletSeries operator-(const DirichletSeries &g) const { return DirichletSeries(*this) -= g; }
  DirichletSeries operator*(const S &a) const { return DirichletSeries(*this) *= a; }
  DirichletSeries operator/(const S &a) const { return DirichletSeries(*this) /= a; }
  friend DirichletSeries operator*(const S &a, const DirichletSeries &f) { return f * a; }

private:
  // a: 乗法的、b: 任意
  static vc<S> prod_half_multiplicative(const vc<S> &a, const vc<S> &b)
  {
    const int n = int(a.size()) - 1;
    vc<S> res(b);
    for (int p : LinearSieve::primes(n))
    {
      for (int i = n / p; i >= 1; i--)
      {
        int q = p, m = i;
        while (true)
        {
          res[i * p] = R::add(res[i * p], R::mul(a[q], res[m]));
          if (m % p != 0)
            break;
          q *= p;
          m /= p;
        }
      }
    }
    return res;
  }

  DirichletSeries convolve_impl(const DirichletSeries &g, bool force_sparse) const
  {
    assert(n_ == g.n_);
    if (force_sparse || !multiplicative_ || !g.multiplicative_)
    {
      vc<int> a, b;
      for (int i = 1; i <= n_; i++)
      {
        if (f_[i] != R::e0()) a.push_back(i);
        if (g.f_[i] != R::e0()) b.push_back(i);
      }
      size_t end = b.size();
      long double work = 0;
      for (int i : a)
      {
        while (end && b[end - 1] > n_ / i) --end;
        work += end;
      }
      const long double dense_work = n_ * (multiplicative_ || g.multiplicative_
        ? 1 + logl(1 + logl(n_ + 1.0L)) : 1 + logl(n_ + 1.0L));
      if (force_sparse || work <= dense_work / 2)
      {
        DirichletSeries res(n_);
        for (int i : a)
          for (int j : b)
          {
            if (j > n_ / i) break;
            res.f_[i * j] = R::add(res.f_[i * j], R::mul(f_[i], g.f_[j]));
          }
        res.multiplicative_ = multiplicative_ && g.multiplicative_;
        return res;
      }
    }
    if (multiplicative_ && g.multiplicative_)
    {
      auto primepower = [&](const PrimePower<int> &q)
      {
        S value = R::e0();
        for (int a = q.pe, b = 1;; a /= q.p, b *= q.p)
        {
          value = R::add(value, R::mul(f_[a], g.f_[b]));
          if (a == 1)
            break;
        }
        return value;
      };
      return DirichletSeries(n_, primepower);
    }
    if (multiplicative_)
      return DirichletSeries(prod_half_multiplicative(f_, g.f_));
    if (g.multiplicative_)
      return DirichletSeries(prod_half_multiplicative(g.f_, f_));
    DirichletSeries res(n_);
    for (int i = 1; i <= n_; i++)
      for (int j = 1; j <= n_ / i; j++)
        res.f_[i * j] = R::add(res.f_[i * j], R::mul(f_[i], g.f_[j]));
    return res;
  }
public:
  DirichletSeries convolve(const DirichletSeries &g) const { return convolve_impl(g, false); }
  DirichletSeries convolve_sparse(const DirichletSeries &g) const { return convolve_impl(g, true); }
  DirichletSeries operator*(const DirichletSeries &g) const { return convolve(g); }
  DirichletSeries &operator*=(const DirichletSeries &g) { return *this = *this * g; }

  // h * g = *this を満たす h
  // 整数型では各除算が割り切れることを仮定
  // 両方が乗法的: O(n)
  // g のみ乗法的: O(n loglog n)
  // 一般: O(n log n)
  DirichletSeries operator/(const DirichletSeries &g) const
  {
    assert(n_ == g.n_);
    if (n_ == 0)
      return DirichletSeries(vc<S>{R::e0()}, multiplicative_ && g.multiplicative_);
    if (multiplicative_ && g.multiplicative_)
    {
      LinearSieve::reserve(n_);
      DirichletSeries res = unit(n_);
      for (int i = 2; i <= n_; i++)
      {
        const auto q = LinearSieve::lpf_[i];
        if (i != q.pe)
          res.f_[i] = R::mul(res.f_[i / q.pe], res.f_[q.pe]);
        else
        {
          res.f_[i] = f_[i];
          for (int a = 1, b = i; b > 1; a *= q.p, b /= q.p)
            res.f_[i] = R::add(res.f_[i], R::minus(R::mul(res.f_[a], g.f_[b])));
        }
      }
      return res;
    }
    if (g.multiplicative_)
      return DirichletSeries(prod_half_multiplicative(g.inv().f_, f_));
    DirichletSeries res(f_);
    const internal::dirichlet_divisor<R> divide(g.f_[1]);
    for (int i = 1; i <= n_; i++)
    {
      res.f_[i] = divide(res.f_[i]);
      for (int j = 2; j <= n_ / i; j++)
        res.f_[i * j] = R::add(res.f_[i * j], R::minus(R::mul(res.f_[i], g.f_[j])));
    }
    return res;
  }
  DirichletSeries &operator/=(const DirichletSeries &g) { return *this = *this / g; }
  DirichletSeries inv() const { return unit(n_) / *this; }

  // Df(n) = Ω(n) f(n)
  DirichletSeries diff() const
  {
    DirichletSeries res(n_);
    if (n_ <= 1) return res;
    LinearSieve::Omega(n_);
    const auto integers = internal::dirichlet_integers<R>(int(msb_pos(n_)));
    for (int i = 2; i <= n_; i++)
      res.f_[i] = R::mul(integers[LinearSieve::Omega(i)], f_[i]);
    return res;
  }

  // f(1) = 0
  DirichletSeries integ(const S &constant = R::e0()) const
  {
    static_assert(internal::dirichlet_has_inv<R>::value, "R::inv is required");
    DirichletSeries res(n_);
    if (n_ == 0) return res;
    assert(f_[1] == R::e0());
    res.f_[1] = constant;
    LinearSieve::Omega(n_);
    const auto &inverse = internal::dirichlet_integer_inverses<R>(int(msb_pos(n_)));
    for (int i = 2; i <= n_; i++)
      res.f_[i] = R::mul(inverse[LinearSieve::Omega(i)], f_[i]);
    return res;
  }

  // f(1) = 1
  DirichletSeries log() const
  {
    static_assert(internal::dirichlet_has_inv<R>::value, "R::inv is required");
    if (n_ == 0) return DirichletSeries(n_);
    assert(f_[1] == R::e1());
    if (!multiplicative_)
      return (diff() / *this).integ();
    DirichletSeries res(n_);
    LinearSieve::reserve(n_);
    const auto integers = internal::dirichlet_integers<R>(int(msb_pos(n_)));
    const auto &inverse = internal::dirichlet_integer_inverses<R>(int(msb_pos(n_)));
    for (int i = 2; i <= n_; i++)
    {
      const auto q = LinearSieve::lpf_[i];
      if (i != q.pe) continue;
      S value = R::mul(integers[q.e], f_[i]);
      for (int a = q.p, b = i / q.p, j = 1; b > 1; a *= q.p, b /= q.p, j++)
        value = R::add(value, R::minus(R::mul(R::mul(integers[j], res.f_[a]), f_[b])));
      res.f_[i] = R::mul(value, inverse[q.e]);
    }
    return res;
  }

  // f(1) = 0
  DirichletSeries exp() const
  {
    static_assert(internal::dirichlet_has_inv<R>::value, "R::inv is required");
    if (n_ == 0) return unit(n_);
    assert(f_[1] == R::e0());
    LinearSieve::Omega(n_);
    const auto integers = internal::dirichlet_integers<R>(int(msb_pos(n_)));
    const auto &inverse = internal::dirichlet_integer_inverses<R>(int(msb_pos(n_)));
    bool primepowers_only = true;
    for (int i = 2; i <= n_; i++)
      if (f_[i] != R::e0() && LinearSieve::lpf_[i].pe != i)
      {
        primepowers_only = false;
        break;
      }
    DirichletSeries res = unit(n_);
    if (primepowers_only)
    {
      for (int i = 2; i <= n_; i++)
      {
        const auto q = LinearSieve::lpf_[i];
        if (i != q.pe)
          res.f_[i] = R::mul(res.f_[q.pe], res.f_[i / q.pe]);
        else
        {
          S value = R::e0();
          for (int a = q.p, b = i / q.p, j = 1;; a *= q.p, b /= q.p, j++)
          {
            value = R::add(value, R::mul(R::mul(integers[j], f_[a]), res.f_[b]));
            if (b == 1) break;
          }
          res.f_[i] = R::mul(value, inverse[q.e]);
        }
      }
    }
    else
    {
      const auto weighted = diff();
      for (int i = 1; i <= n_; i++)
      {
        if (i > 1) res.f_[i] = R::mul(res.f_[i], inverse[LinearSieve::Omega(i)]);
        if (res.f_[i] == R::e0()) continue;
        for (int j = 2; j <= n_ / i; j++)
          res.f_[i * j] = R::add(res.f_[i * j], R::mul(res.f_[i], weighted.f_[j]));
      }
      res.multiplicative_ = false;
    }
    return res;
  }

  template <class T, enable_if_t<is_same_v<T, S> && !is_integral_ext<T>, int> = 0>
  DirichletSeries pow(const T &exponent) const
  {
    static_assert(internal::dirichlet_has_inv<R>::value, "R::inv is required");
    if (n_ == 0) return unit(n_);
    assert(f_[1] == R::e1());
    if (exponent == R::e0()) return unit(n_);
    if (exponent == R::e1()) return *this;
    return (log() * exponent).exp();
  }

  DirichletSeries pow(ll exponent) const
  {
    assert(exponent >= 0);
    if (exponent == 0) return unit(n_);
    if (exponent == 1) return *this;
    if constexpr (internal::dirichlet_modint_field<R>::value)
    {
      // 小さい標数・合成数 mod でも、従来の除算不要の整数乗を残す。
      if (n_ >= 1 && f_[1] == R::e1())
      {
        bool invertible = true;
        for (int i = 2; i <= int(msb_pos(n_)); i++)
          if (S::mod() % i == 0) { invertible = false; break; }
        if (invertible)
          return pow(S(exponent));
      }
    }
    DirichletSeries res = unit(n_), base(*this);
    while (exponent > 0)
    {
      if (exponent & 1)
        res *= base;
      exponent >>= 1;
      if (exponent > 0)
        base *= base;
    }
    return res;
  }

  // A(s) -> A(ds)、つまり添字 i の係数を i^d に移す
  DirichletSeries substitute_power(ll d) const
  {
    assert(d >= 1);
    if (d == 1) return *this;
    DirichletSeries res(n_);
    res.multiplicative_ = multiplicative_;
    if (n_ >= 1) res.f_[1] = f_[1];
    for (int i = 2; i <= n_; i++)
    {
      int q = 1;
      for (ll e = 0; e < d; e++)
      {
        if (q > n_ / i) { q = 0; break; }
        q *= i;
      }
      if (q == 0) break;
      res.f_[q] = f_[i];
    }
    return res;
  }

  // FPS c に対して Σ c[j] * f^{*j}
  // f(1) = 0
  DirichletSeries compose_fps(const vc<S> &c) const
  {
    if (n_ == 0) return DirichletSeries(n_);
    assert(f_[1] == R::e0());
    if (c.empty()) return DirichletSeries(n_);
    const int degree = int(min(c.size() - 1, size_t(int(msb_pos(n_)))));
    vc<int> limits(degree + 1, n_);
    for (int j = 1; j <= degree; j++) limits[j] = limits[j - 1] / 2;
    vc<S> res(limits.back() + 1, R::e0());
    res[1] = c[degree];
    for (int j = degree - 1; j >= 0; j--)
    {
      vc<S> next(limits[j] + 1, R::e0());
      next[1] = c[j];
      for (int b = 1; b < int(res.size()); b++)
      {
        if (res[b] == R::e0()) continue;
        for (int a = 2; a <= limits[j] / b; a++)
          next[a * b] = R::add(next[a * b], R::mul(f_[a], res[b]));
      }
      res = move(next);
    }
    return DirichletSeries(move(res));
  }
};
