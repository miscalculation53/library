#pragma once

/*!
 * Portions corresponding to AtCoder Library (CC0-1.0): two-congruence merge.
 * Upstream: atcoder/math.hpp
 * https://github.com/atcoder/ac-library/tree/864245a00b00dd008d1abfdc239618fdb7d139da
 * License: https://github.com/atcoder/ac-library/blob/864245a00b00dd008d1abfdc239618fdb7d139da/LICENSE
 */

#include "../template/template_all_but_modint.hpp"

#include "extgcd.hpp"
#include "../utils/larger_int.hpp"
#include "../utils/integer_arithmetic.hpp"
#include "modint/modint_internal_barrett32.hpp"

/**
 * @brief 中国剰余定理 (CRT)
 * @docs docs/math/crt.md
 */

// 解 (r, m)。存在しなければ nullopt。
template <class T = ll, class R0, class R1, class M0, class M1>
constexpr optional<pair<T, T>> crt2(R0 r0_, R1 r1_, M0 m0_, M1 m1_)
{
  T m0 = m0_, m1 = m1_;
  assert(m0 >= 1 && m1 >= 1);
  T r0 = safemod(r0_, m0), r1 = safemod(r1_, m1);
  if (m0 < m1)
    swap(r0, r1), swap(m0, m1);
  if (m0 % m1 == 0)
  {
    if (r0 % m1 != r1)
      return nullopt;
    return pair<T, T>{r0, m0};
  }
  auto [g, im, _] = extgcd<T>(m0, m1);
  T u1 = m1 / g;
  if ((r1 - r0) % g)
    return nullopt;
  T x = (r1 - r0) / g % u1 * im % u1;
  r0 += x * m0;
  m0 *= u1;
  if (r0 < 0)
    r0 += m0;
  return pair<T, T>{r0, m0};
}

// 解 (r, m)。存在しなければ nullopt。
template <class T = ll, class V1, class V2>
constexpr optional<pair<T, T>> crt(const V1 &rs, const V2 &ms)
{
  assert(rs.size() == ms.size());
  const int n = rs.size();
  T r = 0, m = 1;
  repi(i, n)
  {
    const auto next = crt2<T>(r, rs[i], m, ms[i]);
    if (!next) return nullopt;
    r = next->first, m = next->second;
  }
  return pair<T, T>{r, m};
}

namespace crt_internal
{
namespace crt_word_detail
{
  template <class U>
  constexpr U add_mod(U a, U b, U m)
  {
    return a >= m - b ? a - (m - b) : a + b;
  }

  template <class U>
  constexpr U sub_mod(U a, U b, U m)
  {
    return a >= b ? a - b : m - (b - a);
  }

  template <class U>
  constexpr U mul_mod(U a, U b, U m)
  {
    if constexpr (sizeof(U) <= sizeof(uint32_t))
      return U((uint64_t(a) * uint64_t(b)) % uint64_t(m));
    else if constexpr (sizeof(U) <= sizeof(uint64_t))
      return U((u128(a) * u128(b)) % u128(m));
    else
    {
      U out = 0;
      while (b != 0)
      {
        if ((b & 1) != 0) out = add_mod(out, a, m);
        b >>= 1;
        if (b != 0) a = add_mod(a, a, m);
      }
      return out;
    }
  }

  template <class U>
  constexpr U inverse_mod(U a, U m)
  {
    U r0 = m, r1 = a, t0 = 0, t1 = 1;
    bool negative = false;
    while (r1 != 1)
    {
      assert(r1 != 0);
      const U q = r0 / r1;
      const U next_r = r0 % r1;
      const U next_t = t0 + q * t1;
      r0 = r1;
      r1 = next_r;
      t0 = t1;
      t1 = next_t;
      negative = !negative;
    }
    return negative ? m - t1 : t1;
  }

  template <class U, class R>
  constexpr U normalize(R x, U m)
  {
    if constexpr (is_same_v<R, bool>) return U(x) % m;
    else
    {
      using W = common_type_t<unsigned_arithmetic_t<R>, unsigned_arithmetic_t<U>>;
      const U r = U(W(unsigned_abs(x)) % W(m));
      return is_negative(x) && r != 0 ? m - r : r;
    }
  }

  template <class mint, class U, class V, class A, class B>
  constexpr void append(U residue, U modulus, size_t i, const V &ms,
                        A &remainders, B &coefficients, mint &answer, mint &period)
  {
    using M = remove_cv_t<typename V::value_type>;
    const U digit = mul_mod(sub_mod(residue, remainders[i], modulus),
                            inverse_mod(coefficients[i], modulus), modulus);
    answer = answer + period * mint(M(digit));
    period = period * mint(ms[i]);
    for (size_t j = i + 1; j < ms.size(); ++j)
    {
      const U m = U(ms[j]);
      if (m == 1) continue;
      const U coefficient = coefficients[j];
      remainders[j] = add_mod(remainders[j], mul_mod(coefficient, digit % m, m), m);
      coefficients[j] = mul_mod(coefficient, modulus % m, m);
    }
  }
}

template <class mint, class V1, class V2>
pair<mint, mint> crt_mod_word(const V1 &rs, const V2 &ms)
{
  using R = remove_cv_t<typename V1::value_type>;
  using M = remove_cv_t<typename V2::value_type>;
  static_assert(is_integral_ext<M>);
  assert(rs.size() == ms.size());
  if constexpr (is_same_v<M, bool>)
  {
    for (size_t i = 0; i < ms.size(); ++i) assert(ms[i]);
    return {mint(0), mint(1)};
  }
  else
  {
    static_assert(is_integral_ext<R>);
    using U = make_unsigned_ext_t<M>;
    vector<U> remainders(ms.size(), 0), coefficients(ms.size(), 1);
    for (size_t i = 0; i < ms.size(); ++i)
    {
      assert(ms[i] >= 1);
      if (ms[i] == 1) coefficients[i] = 0;
    }
    mint answer(0), period(1);
    for (size_t i = 0; i < ms.size(); ++i)
    {
      const U m = U(ms[i]);
      if (m == 1) continue;
      const U residue = crt_word_detail::normalize<U>(R(rs[i]), m);
      crt_word_detail::append(residue, m, i, ms, remainders, coefficients, answer, period);
    }
    return {answer, period};
  }
}

template <class mint, class R, class M0, size_t n>
constexpr pair<mint, mint> crt_mod_word_constexpr(const array<R, n> &rs, const array<M0, n> &ms)
{
  using M = remove_cv_t<M0>;
  static_assert(is_integral_ext<M>);
  if constexpr (is_same_v<M, bool>)
  {
    for (size_t i = 0; i < n; ++i)
    {
      assert(ms[i]);
      assert(rs[i] >= 0 && rs[i] < ms[i]);
    }
    return {mint(0), mint(1)};
  }
  else
  {
    using U = make_unsigned_ext_t<M>;
    array<U, n> remainders{}, coefficients{};
    for (size_t i = 0; i < n; ++i)
    {
      assert(ms[i] >= 1);
      assert(rs[i] >= 0 && rs[i] < ms[i]);
      coefficients[i] = ms[i] == 1 ? 0 : 1;
    }
    mint answer(0), period(1);
    for (size_t i = 0; i < n; ++i)
    {
      const U m = U(ms[i]);
      if (m == 1) continue;
      crt_word_detail::append(U(rs[i]), m, i, ms, remainders, coefficients, answer, period);
    }
    return {answer, period};
  }
}

namespace crt_word_dispatch
{
  constexpr bool constant_evaluation() { return __builtin_is_constant_evaluated(); }

  template <class V>
  constexpr bool runtime_requires_word(const V &ms)
  {
    using M = remove_cv_t<typename V::value_type>;
    if constexpr (!is_integral_ext<M>) return false;
    else if constexpr (is_same_v<M, bool> || sizeof(M) > sizeof(uint64_t)) return true;
    else if constexpr (is_signed_ext<M>) return false;
    else
    {
      constexpr uint64_t limit = sizeof(M) <= sizeof(uint32_t) ? uint64_t(INT_MAX) : uint64_t(LLONG_MAX);
      for (size_t i = 0; i < ms.size(); ++i)
        if (uint64_t(ms[i]) > limit) return true;
      return false;
    }
  }

  template <class M0>
  constexpr bool array_requires_word()
  {
    using M = remove_cv_t<M0>;
    if constexpr (!is_integral_ext<M>) return false;
    else return !is_signed_ext<M> || sizeof(M) > sizeof(uint64_t) || constant_evaluation();
  }
}
}

// (r, m)
// ms[i] たちは pairwise coprime
template <class mint, class V1, class V2>
pair<mint, mint> crt_mod(const V1 &rs, const V2 &ms)
{
  using R = remove_cv_t<typename V1::value_type>;
  using M = remove_cv_t<typename V2::value_type>;
  if constexpr (is_same_v<M, bool> || (is_integral_ext<R> && is_integral_ext<M> && sizeof(M) > sizeof(uint64_t)))
    return crt_internal::crt_mod_word<mint>(rs, ms);
  else
  {
    if constexpr (is_integral_ext<R> && is_integral_ext<M>)
      if (crt_internal::crt_word_dispatch::runtime_requires_word(ms))
        return crt_internal::crt_mod_word<mint>(rs, ms);
    using T = decay_t<decltype(ms[0])>;
    assert(rs.size() == ms.size());
    const int n = rs.size();
    mint r = 0, m = 1;

    // 小さい法は 4 本以上で計算幅を切り替え、16 本以上で Barrett を使う。
    bool small = sizeof(T) <= 4;
    if constexpr (sizeof(T) > 4)
      if (n >= 4)
      {
        small = true;
        for (auto mod : ms)
          if (mod > INT_MAX) { small = false; break; }
      }
    if (sizeof(T) <= 4 || (small && n >= 16))
    {
      vc<internal::barrett32> ba;
      ba.reserve(n);
      repi(i, n) ba.eb(ms[i]);
      vc<uint> rr(n, 0), mm(n, 1);
      repi(i, n)
      {
        assert(ms[i] >= 1);
        auto [g, im, _] = extgcd<ll>(mm[i], ms[i]);
        assert(g == 1);
        if (im < 0)
          im += ms[i];
        ll diff = rs[i] % ll(ms[i]);
        if (diff < 0) diff += ms[i];
        diff -= rr[i];
        if (diff < 0) diff += ms[i];
        uint t = ba[i].mul(diff, im);
        r += t * m, m *= ms[i];
        repi(j, i + 1, n)
        {
          rr[j] += ba[j].mul(t, mm[j]);
          if (rr[j] >= (uint)ms[j])
            rr[j] -= ms[j];
          mm[j] = ba[j].mul(mm[j], ms[i]);
        }
      }
    }
    else
    {
      auto solve = [&](auto word)
      {
        using W = decltype(word);
        vc<ull> rr(n, 0), mm(n, 1);
        repi(i, n)
        {
          assert(ms[i] >= 1);
          auto [g, im, _] = extgcd<ll>(mm[i], ms[i]);
          assert(g == 1);
          if (im < 0)
            im += ms[i];
          ll diff = rs[i] % ll(ms[i]);
          if (diff < 0) diff += ms[i];
          diff -= ll(rr[i]);
          if (diff < 0) diff += ms[i];
          ull t = (ull)(W(diff) * im % ms[i]);
          r += t * m, m *= ms[i];
          repi(j, i + 1, n)
          {
            rr[j] += (ull)(W(t) * mm[j] % ms[j]);
            if (rr[j] >= (ull)ms[j]) rr[j] -= ms[j];
            mm[j] = (ull)(W(mm[j]) * ms[i] % ms[j]);
          }
        }
      };
      if (small) solve(ull(0));
      else solve(u128(0));
    }
    return {r, m};
  }
}

// (r, m)
// 引数は array
// ms[i] たちがコンパイル時定数であることを仮定
// ms[i] たちは pairwise coprime
// 0 <= rs[i] < ms[i]
template <class mint, class U1, class U2, size_t n>
constexpr pair<mint, mint> crt_mod_constexpr(const array<U1, n> &rs, const array<U2, n> &ms)
{
  if constexpr (is_integral_ext<remove_cv_t<U2>> && (!is_signed_ext<remove_cv_t<U2>> || sizeof(U2) > sizeof(uint64_t)))
    return crt_internal::crt_mod_word_constexpr<mint>(rs, ms);
  else
  {
    if constexpr (is_integral_ext<remove_cv_t<U2>>)
      if (crt_internal::crt_word_dispatch::array_requires_word<U2>())
        return crt_internal::crt_mod_word_constexpr<mint>(rs, ms);
    if constexpr (sizeof(U2) > 4)
    {
      bool small = true;
      for (auto mod : ms)
        if (mod > INT_MAX) { small = false; break; }
      if (small)
      {
        array<int, n> residues{}, moduli{};
        repi(i, n) residues[i] = rs[i], moduli[i] = ms[i];
        return crt_mod_constexpr<mint>(residues, moduli);
      }
    }
    using T = larger_int_t<U2>;
    assert(rs.size() == ms.size());
    mint r = 0, m = 1;
    array<T, n> rr{}, mm{};
    fill(ALL(mm), 1);
    repi(i, n)
    {
      assert(ms[i] >= U2(1));
      assert(U1(0) <= rs[i] && U2(rs[i]) < ms[i]);
      auto [g, im, _] = extgcd<T>(mm[i], ms[i]);
      assert(g == 1);
      T t = safemod<T>((rs[i] - rr[i]) * im, ms[i]);
      r += t * m, m *= ms[i];
      repi(j, i + 1, n)
      {
        rr[j] += t * mm[j] % ms[j];
        if (rr[j] >= ms[j])
          rr[j] -= ms[j];
        mm[j] *= ms[i], mm[j] %= ms[j];
      }
    }
    return {r, m};
  }
}

namespace pre_crt_v2_adl
{
  void gcd() = delete;
  template <class T>
  auto call(const T &a, const T &b) -> decltype(gcd(a, b)) { return gcd(a, b); }
}

namespace pre_crt_v2_detail
{
  template <class T, bool = is_integral_ext<T>> struct work_type { using type = T; };
  template <class T> struct work_type<T, true> { using type = make_unsigned_ext_t<T>; };
  template <class T, class = void> struct has_adl_gcd : false_type {};
  template <class T> struct has_adl_gcd<T, void_t<decltype(pre_crt_v2_adl::call(declval<T>(), declval<T>()))>> : true_type {};
  template <class T>
  constexpr bool unbounded_integer = numeric_limits<T>::is_specialized &&
                                     numeric_limits<T>::is_integer && !numeric_limits<T>::is_bounded;

  template <class T>
  T gcd(T a, T b)
  {
    if constexpr (!is_integral_ext<T> && has_adl_gcd<T>::value)
      return T(pre_crt_v2_adl::call(a, b));
    else
    {
      while (b != 0)
      {
        T r = a % b;
        a = b;
        b = r;
      }
      return a;
    }
  }

  template <class T>
  T add_mod(const T &a, const T &b, const T &m)
  {
    if (a >= m - b) return T(a - (m - b));
    return T(a + b);
  }

  template <class T, class R>
  T normalize(R x, const T &m)
  {
    if constexpr (is_same_v<R, bool>) return x ? T(T(1) % m) : T(0);
    else if constexpr (is_integral_ext<R> && is_integral_ext<T>)
    {
      using W = common_type_t<unsigned_arithmetic_t<R>, unsigned_arithmetic_t<T>>;
      const T r = T(W(unsigned_abs(x)) % W(m));
      return is_negative(x) && r != 0 ? m - r : r;
    }
    else if constexpr (unbounded_integer<R> && is_constructible_v<R, T> && is_constructible_v<T, R>)
    {
      const R modulus(m);
      R r = x % modulus;
      if (r < 0) r = r + modulus;
      return T(r);
    }
    else if constexpr (unbounded_integer<T> && is_constructible_v<T, R>)
    {
      T r = T(x) % m;
      if (r < 0) r = r + m;
      return r;
    }
    else
    {
      if (m == 1) return T(0);
      const bool negative = x < R(0);
      T r = 0, power = 1;
      while (x != R(0))
      {
        R quotient = x / R(2);
        R remainder = x % R(2);
        if (remainder != R(0)) r = add_mod(r, power, m);
        if (negative && remainder > R(0)) quotient = quotient + R(1);
        x = quotient;
        if (x != R(0)) power = add_mod(power, power, m);
      }
      if (negative && r != 0) r = T(m - r);
      return r;
    }
  }

  template <class T>
  void distribute(T &a, T &b, T g)
  {
    a = T(a / g);
    b = T(b / g);
    while (true)
    {
      const T d = gcd(a, g);
      if (d == 1) break;
      a = T(a * d);
      g = T(g / d);
    }
    b = T(b * g);
  }
}

// 破壊的に変更する
// 解が存在しないなら false を返す
// 解が存在するなら true を返し、ms[i] たちが pairwise coprime であるような等価な方程式に変換する
template <class V1, class V2>
bool pre_crt(const V1 &rs, V2 &ms)
{
  using R = remove_cv_t<typename V1::value_type>;
  using M = remove_cv_t<typename V2::value_type>;
  static_assert(!is_same_v<M, bool>);
  using T = typename pre_crt_v2_detail::work_type<M>::type;
  assert(rs.size() == ms.size());
  const size_t n = ms.size();
  if (n <= 1) return true;
  if (n == 2)
  {
    T a(ms[0]), b(ms[1]);
    if (a == 1 || b == 1) return true;
    if (a == b && R(rs[0]) == R(rs[1])) { ms[0] = M(1); return true; }
    const T g = a == b ? a : pre_crt_v2_detail::gcd(a, b);
    if (g == 1) return true;
    const R r0(rs[0]), r1(rs[1]);
    if (r0 != r1 && pre_crt_v2_detail::normalize<T>(r0, g) != pre_crt_v2_detail::normalize<T>(r1, g))
      return false;
    pre_crt_v2_detail::distribute(a, b, g);
    ms[0] = M(a);
    ms[1] = M(b);
    return true;
  }

  vector<T> residues(n);
  for (size_t i = 0; i < n; ++i)
    residues[i] = pre_crt_v2_detail::normalize<T>(R(rs[i]), T(ms[i]));
  for (size_t i = 0; i < n; ++i)
  {
    T a(ms[i]);
    if (a == 1) continue;
    for (size_t j = i + 1; j < n; ++j)
    {
      T b(ms[j]);
      if (b == 1) continue;
      const T g = pre_crt_v2_detail::gcd(a, b);
      if (g == 1) continue;
      if (residues[i] % g != residues[j] % g) return false;
      pre_crt_v2_detail::distribute(a, b, g);
      ms[j] = M(b);
      if (a == 1) break;
    }
    ms[i] = M(a);
  }
  return true;
}
