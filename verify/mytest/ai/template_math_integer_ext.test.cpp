#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_math.hpp"
#include "math/bigint.hpp"

using Big = BigInteger<>;

static_assert(abs(i128(-7)) == 7);
static_assert(abs(~u128(0)) == ~u128(0));
static_assert(is_same_v<decltype(gcd(i128(6), ll(4))), i128>);
static_assert(is_same_v<decltype(gcd(i128(-6), u128(4))), u128>);
static_assert(gcd(i128(-6), u128(4)) == 2);
static_assert(gcd(numeric_limits<i128>::lowest(), i128(3)) == 1);
static_assert(gcd(numeric_limits<i128>::lowest(), u128(0)) == (u128(1) << 127));
static_assert(gcd(ll(-4), i128(6)) == 2 && gcd(ull(4), u128(6)) == 2);
static_assert(lcm(-(i128(1) << 100), i128(3) << 100) == (i128(3) << 100));
static_assert(lcm(numeric_limits<i128>::lowest(), 0) == 0);
static_assert(lcm(3, i128(4)) == 12 && lcm(3, u128(4)) == 12);
static_assert(safemod<i128>(numeric_limits<i128>::max(), numeric_limits<i128>::lowest()) == -1);
static_assert(safemod<i128>(numeric_limits<i128>::lowest(), -1) == 0);

template <class T>
void check(T a, T b)
{
  const Big ba(a), bb(b), hi(numeric_limits<T>::max()), lo(numeric_limits<T>::lowest());
  const Big magnitude = ba.abs();
  if constexpr (is_signed_ext<T> || is_same_v<T, u128>)
    if (magnitude <= hi) assert(Big(abs(a)) == magnitude);
  const Big g = gcd(ba, bb), l = lcm(ba, bb);
  assert(Big(gcd(unsigned_abs(a), unsigned_abs(b))) == g);
  if (is_same_v<T, i128> || is_same_v<T, u128> || (magnitude <= hi && bb.abs() <= hi))
  {
    if (g <= hi) assert(Big(gcd(a, b)) == g);
    if (l <= hi) assert(Big(lcm(a, b)) == l);
  }
  if (b == 0) return;
  // 多倍長整数を参照に、境界値でも中間計算を広げずに丸められることを確認する。
  const Big f = divfloor(ba, bb), c = divceil(ba, bb);
  if (lo <= f && f <= hi) assert(Big(divfloor<T>(a, b)) == f);
  if (lo <= c && c <= hi) assert(Big(divceil<T>(a, b)) == c);
  assert(Big(safemod<T>(a, b)) == safemod(ba, bb));
}

template <class T>
void test_type()
{
  const T lo = numeric_limits<T>::lowest(), hi = numeric_limits<T>::max();
  vc<T> values{T(0), T(1), T(2), T(3), T(7), T(hi / 2), hi, T(hi - 1), lo};
  if constexpr (is_signed_ext<T>)
    for (T x : {T(-1), T(-2), T(-3), T(-7), T(lo / 2), T(lo + 1)}) values.push_back(x);
  for (T a : values)
    for (T b : values) check(a, b);
  mt19937_64 rng(20261011);
  for (int i = 0; i < 100; ++i)
  {
    const u128 a = (u128(rng()) << 64) | rng(), b = (u128(rng()) << 64) | rng();
    check(wrapping_cast<T>(a), wrapping_cast<T>(b));
  }
}

int main()
{
  test_type<signed char>(); test_type<unsigned char>();
  test_type<short>(); test_type<unsigned short>();
  test_type<int>(); test_type<uint>();
  test_type<ll>(); test_type<ull>();
  test_type<i128>(); test_type<u128>();
  cout << "Hello World\n";
}
