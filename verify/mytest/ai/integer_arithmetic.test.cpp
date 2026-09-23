#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "utils/integer_arithmetic.hpp"
#include "math/bigint.hpp"
#include "math/modint/modint.hpp"

using Big = BigInteger<>;

static_assert(!is_negative(0U) && !is_negative(~u128(0)) && is_negative(i128(-1)));
static_assert(unsigned_abs(numeric_limits<ll>::min()) == (1ULL << 63));
static_assert(unsigned_abs(numeric_limits<i128>::min()) == (u128(1) << 127));
static_assert(is_same_v<unsigned_arithmetic_t<short>, uint>);
static_assert(is_same_v<unsigned_arithmetic_t<unsigned short>, uint>);
static_assert(is_same_v<unsigned_arithmetic_t<ll>, ull>);
static_assert(is_same_v<unsigned_arithmetic_t<i128>, u128>);
static_assert(is_same_v<unsigned_arithmetic_t<modint998244353>, modint998244353>);
static_assert(is_same_v<unsigned_arithmetic_t<Big>, Big>);
static_assert(wrapping_cast<ll>(~0ULL) == -1);
static_assert(wrapping_cast<signed char>(255U) == -1);
static_assert(wrapping_cast<i128>(-1LL) == -1);
static_assert(wrapping_cast<ull>(short(-1)) == ~0ULL);

template <class R, class T>
void check_cast(T x)
{
  Big modulus = 1;
  for (size_t i = 0; i < sizeof(R) * 8; ++i) modulus *= 2;
  Big expected = safemod(Big(x), modulus);
  if constexpr (is_signed_ext<R>)
    if (expected >= modulus / 2) expected -= modulus;
  assert(Big(wrapping_cast<R>(x)) == expected);
}

template <class T>
void check(T x)
{
  const Big value(x);
  assert(is_negative(x) == (value < 0));
  assert(Big(unsigned_abs(x)) == (value < 0 ? -value : value));
  check_cast<signed char>(x); check_cast<unsigned char>(x);
  check_cast<short>(x); check_cast<unsigned short>(x);
  check_cast<int>(x); check_cast<uint>(x);
  check_cast<ll>(x); check_cast<ull>(x);
  check_cast<i128>(x); check_cast<u128>(x);
}

int main()
{
  // 新しい絶対値を使う BigInteger の整数コンストラクタも独立に確認する。
  assert(Big(numeric_limits<ll>::min()).to_string() == "-9223372036854775808");
  assert(Big(numeric_limits<ull>::max()).to_string() == "18446744073709551615");
  assert(Big(numeric_limits<i128>::min()).to_string() == "-170141183460469231731687303715884105728");
  assert(Big(~u128(0)).to_string() == "340282366920938463463374607431768211455");
  for (int i = -128; i <= 127; ++i) check(static_cast<signed char>(i));
  for (int i = 0; i <= 255; ++i) check(static_cast<unsigned char>(i));
  check(numeric_limits<short>::min()); check(numeric_limits<short>::max());
  check(numeric_limits<unsigned short>::max());
  check(numeric_limits<int>::min()); check(numeric_limits<int>::max());
  check(numeric_limits<uint>::max());
  check(numeric_limits<ll>::min()); check(numeric_limits<ll>::max());
  check(numeric_limits<ull>::max());
  check(numeric_limits<i128>::min()); check(numeric_limits<i128>::max());
  check(~u128(0));
  mt19937_64 rng(20260924);
  for (int i = 0; i < 100; ++i)
  {
    check(rng());
    check(wrapping_cast<ll>(rng()));
    check((u128(rng()) << 64) | rng());
  }
  using U = unsigned_arithmetic_t<unsigned short>;
  assert(U(65535) * U(65535) == 4294836225U);
  cout << "Hello World\n";
}
