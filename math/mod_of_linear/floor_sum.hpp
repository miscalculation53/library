#pragma once

#include "../../template/template_all_but_modint.hpp"
#include "../../utils/integer_arithmetic.hpp"
#include "../../utils/mul_add_divmod.hpp"

/**
 * @brief floor sum・一次式の剰余の個数
 * @docs docs/math/mod_of_linear/floor_sum.md
 */

namespace internal
{
// 0 <= a,b < m。商は n 以下なので U に収まる。
template <bool small = false, class U>
pair<U, U> floor_sum_divmod(U n, U m, U a, U b)
{
  if constexpr (small)
  {
    const ull y = ull(a) * n + b;
    if (y < m) return {0, U(y)};
    return {U(y / m), U(y % m)};
  }
  else return mul_add_divmod(a, n, b, m);
}

// n*(n-1)/2。2 で割る側を先に選び、C 上では乗算だけを行う。
template <class C, class U>
C floor_sum_triangle(U n)
{
  if (n == 0) return C(0);
  return n % 2 == 0 ? C(n / 2) * C(n - 1) : C(n) * C((n - 1) / 2);
}

// 非負入力に対するユークリッド互除法型の本体。
template <class C, bool small, class U>
C floor_sum_unsigned_core(U n, U m, U a, U b)
{
  C ans = 0;
  while (n > 0)
  {
    ans += C(a / m) * floor_sum_triangle<C>(n) + C(b / m) * C(n);
    a %= m;
    b %= m;
    auto [q, r] = floor_sum_divmod<small>(n, m, a, b);
    if (q == 0) break;
    n = q;
    b = r;
    swap(a, m);
  }
  return ans;
}

// 入口では 0 <= a,b < m。n,m は反復中に増えない。
template <class C, class U>
C floor_sum_unsigned(U n, U m, U a, U b)
{
  if (a == 0) return C(0);
  if constexpr (sizeof(U) <= sizeof(uint))
    return floor_sum_unsigned_core<C, true>(ull(n), ull(m), ull(a), ull(b));
  else
  {
    // a*n+b <= (m-1)*(n+1) < 2^64。ループは ull のまま計算する。
    if ((n | m) <= numeric_limits<uint>::max())
      return floor_sum_unsigned_core<C, true>(ull(n), ull(m), ull(a), ull(b));
    if constexpr (sizeof(U) > sizeof(ull))
      if ((n | m) <= numeric_limits<ull>::max())
        return floor_sum_unsigned_core<C, false>(ull(n), ull(m), ull(a), ull(b));
    return floor_sum_unsigned_core<C, false>(n, m, a, b);
  }
}

} // namespace internal

// sum_{i=l}^{r-1} floor((a*i+b)/m)。整数の R には下位 sizeof(R)*8 bit を返す。
template <class R, class L, class H, class M, class A, class B>
R floor_sum(L l, H r, M m, A a, B b)
{
  using U = common_type_t<unsigned int, make_unsigned_ext_t<L>, make_unsigned_ext_t<H>,
                          make_unsigned_ext_t<M>, make_unsigned_ext_t<A>, make_unsigned_ext_t<B>>;
  using C = unsigned_arithmetic_t<R>;
  assert(m > 0);
  const bool nl = is_negative(l), nr = is_negative(r);
  const U ul = unsigned_abs(l), ur = unsigned_abs(r);
  assert(nl != nr ? nl : (nl ? ul >= ur : ul <= ur));
  const U mod = U(m);

  auto normalize = [&](auto x) -> pair<C, U>
  {
    const U v = unsigned_abs(x), rem = v % mod;
    C q = C(v / mod);
    if (is_negative(x))
      return {C(0) - q - C(int(rem != 0)), rem == 0 ? U(0) : mod - rem};
    return {q, rem};
  };
  const auto na = normalize(a), nb = normalize(b);
  const C qa = na.first, qb = nb.first;
  const U ra = na.second, rb = nb.second;
  // F(x+1)-F(x)=floor((a*x+b)/m), F(0)=0 を満たす離散原始関数。
  // 負の x は添字を反転する。a*l+b や r-l を作る必要がない。
  auto prefix = [&](U n, bool negative) -> C
  {
    if (n == 0) return C(0);
    C q = qb;
    U rem = rb;
    if (negative)
    {
      q = qa - qb + C(int(ra > rb));
      rem = ra > rb ? ra - rb - 1 : mod - 1 - (rb - ra);
    }
    return qa * internal::floor_sum_triangle<C>(n) + q * C(n)
         + internal::floor_sum_unsigned<C>(n, mod, ra, rem);
  };
  const C ans = prefix(ur, nr) - prefix(ul, nl);
  if constexpr (is_integral_ext<R>) return wrapping_cast<R>(ans);
  else return R(ans);
}

template <class R, class N, class M, class A, class B>
R floor_sum(N n, M m, A a, B b)
{
  assert(!is_negative(n));
  return floor_sum<R>(0, n, m, a, b);
}

// (a*i+b) mod m (0 <= i < n) のうち [l,r) に入る項数。
inline ll count_mod_of_linear(ll n, ll m, ll a, ll b, ll l, ll r)
{
  assert(n >= 0 && m >= 1);
  assert(0 <= l && l <= r && r <= m);
  if (l == r) return 0;
  a %= m;
  b %= m;
  if (a < 0) a += m;
  if (b < 0) b += m;
  // 各総和は mod 2^64 でよい。差は [0,n] に収まる。
  return floor_sum<ull>(n, m, a, ull(b) + m - l)
       - floor_sum<ull>(n, m, a, ull(b) + m - r);
}

inline ll count_mod_of_linear(ll n, ll m, ll a, ll l, ll r)
{
  return count_mod_of_linear(n, m, a, 0, l, r);
}
