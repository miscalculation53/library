#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "algebra/monoid_power.hpp"

struct PlainAdd
{
  using S = i128;
  static constexpr S e() { return 0; }
  static constexpr S op(S a, S b) { return a + b; }
  static constexpr S inv(S a) { return -a; }
};

constexpr u128 wide = (u128(1) << 90) + 13;
static_assert(pow_monoid<PlainAdd>(3, wide) == 3 * i128(wide));
static_assert(pow_group<PlainAdd>(3, -i128(wide)) == -3 * i128(wide));
static_assert(pow_monoid<PlainAdd>(7, 0) == 0);

#include "algebra/from_integer.hpp"
#include "math/modint/modint.hpp"

struct Encoded
{
  int code = 99;
  constexpr explicit Encoded(int code) : code(code) {}
};

struct EncodedRing
{
  using S = Encoded;
  static constexpr S e0() { return S(10); }
  static constexpr S e1() { return S(11); }
  static constexpr S add(const S &a, const S &b) { return S(10 + (a.code + b.code - 20) % 17); }
  static constexpr S minus(const S &a) { return S(10 + (27 - a.code) % 17); }
  static constexpr S mul(const S &a, const S &b) { return S(10 + (a.code - 10) * (b.code - 10) % 17); }
};

static_assert(from_integer<EncodedRing>(0).code == 10);
static_assert(from_integer<EncodedRing>(20).code == 13);
static_assert(from_integer<EncodedRing>(-20).code == 24);

struct FastAdd
{
  using S = Encoded;
  static inline int calls = 0;
  static constexpr auto e = EncodedRing::e0;
  static constexpr auto op = EncodedRing::add;
  static constexpr auto inv = EncodedRing::minus;
  static S pow(const S &x, ll n)
  {
    calls++;
    return S(10 + int((x.code - 10) * (n % 17) % 17));
  }
};
struct EncodedMul
{
  using S = Encoded;
  static constexpr auto e = EncodedRing::e1;
  static constexpr auto op = EncodedRing::mul;
  static S inv(const S &x) { return x; }
};

struct CountedAdd
{
  using S = int;
  static inline int calls = 0;
  static S e() { return 0; }
  static S op(S a, S b) { calls++; return a + b; }
};

struct NarrowPow : PlainAdd
{
  static inline int calls = 0;
  static S pow(const S &x, ll k) { calls++; return x * k; }
};

int main()
{
  mt19937_64 rng(20261102);
  for (int trial = 0; trial < 10000; trial++)
  {
    ll x = ll(rng() % 2001) - 1000, k = ll(rng() % 2001) - 1000;
    assert(pow_group<PlainAdd>(x, k) == i128(x) * k);
    int residue = int((k % 17 + 17) % 17);
    assert(from_integer<EncodedRing>(k).code == 10 + residue);
    assert(from_integer<RingAddSubMul<ll>>(k) == k);
    assert(from_integer<FieldAddSubMulDiv<modint998244353>>(k) == modint998244353(k));
    assert((from_integer<RingFromGroupMonoid<FastAdd, EncodedMul>>(k).code == 10 + residue));
    assert((from_integer<FieldFromGroupGroup<FastAdd, EncodedMul>>(k).code == 10 + residue));
  }
  assert(FastAdd::calls > 0);
  assert(pow_monoid<NarrowPow>(2, 7) == 14 && NarrowPow::calls == 1);
  assert(OppositeMonoid<NarrowPow>::pow(2, wide) == 2 * i128(wide));
  assert(NarrowPow::calls == 1);
  CountedAdd::calls = 0;
  assert(pow_monoid<CountedAdd>(7, 0) == 0 && CountedAdd::calls == 0);
  assert(pow_monoid<CountedAdd>(7, 1) == 7 && CountedAdd::calls == 1);
  auto pointer = &pow_monoid<PlainAdd, int>;
  assert(pointer(5, 8) == 40);
  cout << "Hello World\n";
}
