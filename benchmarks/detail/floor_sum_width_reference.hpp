#pragma once
// 変更前の実装を基準に、計算幅の選択だけを変えて比較する。
// method: 0=型で固定、1=入口で32 bit、2=各反復で32 bit、3=積が収まれば64 bit除算。
// 5=入口で判定して ull のループ、6=積の上界を入口で判定。
#include "math/mod_of_linear/floor_sum.hpp"
namespace floor_sum_width_reference
{
namespace internal
{
template <class R, bool = is_integral_ext<R>>
struct floor_sum_accumulator { using type = R; };

template <class R>
struct floor_sum_accumulator<R, true>
{
  // 小さい整数型の integral promotion でも符号付きの積を作らない。
  using type = common_type_t<unsigned int, make_unsigned_ext_t<R>>;
};

template <class T>
bool floor_sum_negative(T x)
{
  if constexpr (is_signed_ext<T>) return x < 0;
  return false;
}

template <class U, class T>
U floor_sum_magnitude(T x)
{
  return floor_sum_negative(x) ? U(0) - U(x) : U(x);
}

// 0 <= a,b < m。商は n 以下なので U に収まる。
template <class U>
pair<U, U> floor_sum_divmod(U n, U m, U a, U b)
{
  if constexpr (sizeof(U) < sizeof(u128))
  {
    using W = larger_int_t<U>;
    const W y = W(a) * n + b;
    return {U(y / m), U(y % m)};
  }
  else
  {
    if (a == 0 || n <= (numeric_limits<U>::max() - b) / a)
    {
      const U y = a * n + b;
      return {y / m, y % m};
    }
    // 128 bit 入力では、商・剰余を保ちながら n の各 bit を読む。
    U q = 0, r = 0;
    for (int i = numeric_limits<U>::digits - 1; i >= 0; --i)
    {
      q *= 2;
      if (r >= m - r) r -= m - r, ++q;
      else r += r;
      if ((n >> i) & 1)
      {
        if (r >= m - a) r -= m - a, ++q;
        else r += a;
      }
    }
    if (r >= m - b) r -= m - b, ++q;
    else r += b;
    return {q, r};
  }
}

template <class C, class U>
C floor_sum_triangle(U n)
{
  if (n == 0) return C(0);
  return n % 2 == 0 ? C(n / 2) * C(n - 1) : C(n) * C((n - 1) / 2);
}

template <class C, class U>
C floor_sum_product64(U n, U m, U a, U b)
{
  C ans = 0;
  while (n > 0)
  {
    ans += C(a / m) * floor_sum_triangle<C>(n) + C(b / m) * C(n);
    a %= m;
    b %= m;
    const ull y = ull(a) * n + b;
    if (y < m) break;
    n = y / m;
    b = y % m;
    swap(a, m);
  }
  return ans;
}

template <int method, class C, class U>
C floor_sum_unsigned(U n, U m, U a, U b)
{
  if constexpr (method == 5 && sizeof(U) > 4)
  {
    if ((n | m) <= numeric_limits<unsigned int>::max())
      return floor_sum_product64<C>(ull(n), ull(m), ull(a), ull(b));
  }
  if constexpr (method == 6 && sizeof(U) == 8)
  {
    if (u128(m - 1) * n + (m - 1) <= numeric_limits<ull>::max())
      return floor_sum_product64<C>(ull(n), ull(m), ull(a), ull(b));
  }
  if constexpr (method == 1 && sizeof(U) > 4)
  {
    if ((n | m) <= numeric_limits<unsigned int>::max())
      return floor_sum_unsigned<0, C>(uint(n), uint(m), uint(a), uint(b));
  }
  C ans = 0;
  while (n > 0)
  {
    if constexpr (method == 2 && sizeof(U) > 4)
    {
      if ((n | m | a | b) <= numeric_limits<unsigned int>::max())
        return ans + floor_sum_unsigned<0, C>(uint(n), uint(m), uint(a), uint(b));
    }
    ans += C(a / m) * floor_sum_triangle<C>(n) + C(b / m) * C(n);
    a %= m;
    b %= m;
    auto divmod = [&]() -> pair<U, U>
    {
      if constexpr (method == 3 && sizeof(U) == 8)
      {
        const u128 y = u128(a) * n + b;
        if (y <= numeric_limits<ull>::max())
          return {U(ull(y) / m), U(ull(y) % m)};
        return {U(y / m), U(y % m)};
      }
      else return floor_sum_divmod(n, m, a, b);
    };
    auto [q, r] = divmod();
    if (q == 0) break;
    n = q;
    b = r;
    swap(a, m);
  }
  return ans;
}

template <class R, class C>
R floor_sum_result(C ans)
{
  if constexpr (is_integral_ext<R> && is_signed_ext<R>)
  {
    using U = make_unsigned_ext_t<R>;
    const U bits = U(ans);
    if (bits <= U(numeric_limits<R>::max())) return R(bits);
    return R(-1) - R(U(~bits));
  }
  else return R(ans);
}
} // namespace internal

// sum_{i=l}^{r-1} floor((a*i+b)/m)。整数の R には下位 sizeof(R)*8 bit を返す。
template <int method, class R, class L, class H, class M, class A, class B>
R floor_sum(L l, H r, M m, A a, B b)
{
  using U = common_type_t<unsigned int, make_unsigned_ext_t<L>, make_unsigned_ext_t<H>,
                          make_unsigned_ext_t<M>, make_unsigned_ext_t<A>, make_unsigned_ext_t<B>>;
  using C = typename internal::floor_sum_accumulator<R>::type;
  assert(m > 0);
  const bool nl = internal::floor_sum_negative(l), nr = internal::floor_sum_negative(r);
  const U ul = internal::floor_sum_magnitude<U>(l), ur = internal::floor_sum_magnitude<U>(r);
  assert(nl != nr ? nl : (nl ? ul >= ur : ul <= ur));
  const U mod = U(m);

  auto normalize = [&](auto x) -> pair<C, U>
  {
    const U v = internal::floor_sum_magnitude<U>(x), rem = v % mod;
    C q = C(v / mod);
    if (internal::floor_sum_negative(x))
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
         + internal::floor_sum_unsigned<method, C>(n, mod, ra, rem);
  };
  return internal::floor_sum_result<R>(prefix(ur, nr) - prefix(ul, nl));
}

template <int method, class R, class N, class M, class A, class B>
R floor_sum(N n, M m, A a, B b)
{
  assert(!internal::floor_sum_negative(n));
  return floor_sum<method, R>(0, n, m, a, b);
}

}
