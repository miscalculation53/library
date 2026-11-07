#pragma once

#include "convolution_ntt.hpp"
#include "../crt.hpp"

/**
 * @brief 畳み込み
 * @docs docs/math/convolution/convolution.md
 */

namespace internal
{

template <size_t j, int mod, class T, size_t k>
void convolution_crt_helper(const vc<T> &a, const vc<T> &b, vc<array<T, k>> &cs)
{
  using mint = static_modint32<mod>;
  const int n = a.size(), m = b.size();
  auto c = convolution_ntt(vc<mint>(ALL(a)), vc<mint>(ALL(b)));
  repi(i, n + m - 1) cs[i][j] = c[i].val();
}

// n == 0 or m == 0 はバグるので注意
template <int ...ms, class T>
vc<T> convolution_crt(const vc<T> &a, const vc<T> &b)
{
  const int n = a.size(), m = b.size();

  constexpr size_t k = sizeof...(ms);
  vc<array<T, k>> cs(n + m - 1);
  constexpr array<int, k> ms_arr = {ms...};
  [&]<size_t... Is>(index_sequence<Is...>)
  {
    (convolution_crt_helper<Is, ms_arr[Is], T, k>(a, b, cs), ...);
  }(make_index_sequence<k>{});

  vc<T> c(n + m - 1);
  repi(i, n + m - 1)
  {
    const auto solution = crt(cs[i], ms_arr);
    assert(solution);
    c[i] = solution->first;
  }
  return c;
}

// n == 0 or m == 0 はバグるので注意
template <class mint, int... ms, class T>
vc<mint> convolution_crt_mod(const vc<T> &a, const vc<T> &b)
{
  const int n = a.size(), m = b.size();

  constexpr size_t k = sizeof...(ms);
  vc<array<T, k>> cs(n + m - 1);
  constexpr array<int, k> ms_arr = {ms...};
  [&]<size_t... Is>(index_sequence<Is...>)
  {
    (convolution_crt_helper<Is, ms_arr[Is], T, k>(a, b, cs), ...);
  }(make_index_sequence<k>{});

  vc<mint> c(n + m - 1);
  repi(i, n + m - 1) c[i] = crt_mod_constexpr<mint>(cs[i], ms_arr).first;
  return c;
}

}  // namespace internal

// ntt-friendly なら普通、そうでなければ 3 つの mod
// mod は 10^9 程度を想定、列の長さは合計 2^26 程度
template <class mint, typename = std::enable_if_t<!std::is_integral<mint>::value>>
vc<mint> convolution(const vc<mint> &a, const vc<mint> &b)
{
  const int n = a.size(), m = b.size();
  if (n == 0 || m == 0)
    return {};
  const int cnta = n - count(ALL(a), 0), cntb = m - count(ALL(b), 0);
  if (cnta == 0 || cntb == 0) return vc<mint>(n + m - 1);
  if (ntt_ok<mint>(n + m - 1))
  {
    if (min(cnta, cntb) <= 60 || internal::convolution_sparse_preferred(a, b, cnta, cntb, 1))
      return internal::convolution_naive(a, b);
    return internal::convolution_ntt(a, b);
  }
  else
  {
    if (min(cnta, cntb) <= 300 || internal::convolution_sparse_preferred(a, b, cnta, cntb, 3))
      return internal::convolution_naive(a, b);
    assert(ntt_ok<static_modint32<469762049>>(n + m - 1) && "|a| + |b| - 1 <= 2^26");
    vc<ll> a_(n), b_(m);
    repi(i, n) a_[i] = a[i].val();
    repi(j, m) b_[j] = b[j].val();
    return internal::convolution_crt_mod<mint, 469762049, 1811939329, 2013265921>(a_, b_);
  }
}

template <int mod = 998244353, class T, typename = enable_if_t<is_integral<T>::value>>
vc<T> convolution(const vc<T> &a, const vc<T> &b)
{
  using mint = static_modint32<mod>;
  auto c = convolution(vc<mint>(ALL(a)), vc<mint>(ALL(b)));
  vc<T> c_(c.size());
  repi(i, c.size()) c_[i] = c[i].val();
  return c_;
}
