#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/dot_product.hpp"

// No minus, inv, or e1: only the interface needed for a sum of products.
struct SumProduct
{
  using S = ll;
  static S e0() { return 0; }
  static S add(S a, S b) { return a + b; }
  static S mul(S a, S b) { return a * b; }
};

// Encode a residue x as x+1. A modint element type alone must not enable
// the ordinary modular sum/product optimization for these different operations.
struct ShiftedResidues
{
  using S = modint998244353;
  static S e0() { return 1; }
  static S add(S a, S b) { return a + b - 1; }
  static S mul(S a, S b) { return (a - 1) * (b - 1) + 1; }
};

template <class S>
void check_modular_ring()
{
  using R = RingAddSubMul<S>;
  using SR = SemiRingFromMonoidMonoid<MonoidAdd<S>, MonoidMul<S>>;
  for (int n : {0, 1, 3, 4, 5, 15, 16, 17, 31, 32, 33, 129})
  {
    vc<S> a(n, S::mod() - 1), b = a;
    repi(mode, 2)
    {
      if (mode)
        repi(i, n) a[i] = randrange<ll>(0, S::mod()), b[i] = randrange<ll>(0, S::mod());
      S expected = 0, reversed_expected = 0;
      repi(i, n) expected += a[i] * b[i], reversed_expected += a[i] * b[n - 1 - i];
      assert(dot_product<R>(n, a.begin(), b.begin()) == expected);
      assert(dot_product<SR>(n, a.begin(), b.begin()) == expected);
      assert(dot_product<R>(n, a.begin(), b.rbegin()) == reversed_expected);
      assert(dot_product<SR>(n, a.begin(), b.rbegin()) == reversed_expected);
    }
  }
}

int main()
{
  mt.seed(20260910);
  vc<ll> a{4, 10, -2}, b{7, 1, 8};
  assert(dot_product<SumProduct>(3, a.begin(), b.begin()) == 22);
  assert(dot_product<SumProduct>(0, a.begin(), b.begin()) == 0);
  assert(dot_product<RingAddSubMul<ll>>(3, a.begin(), b.begin()) == 22);
  assert(dot_product<SemiRingMinPlus<ll>>(3, a.begin(), b.begin()) == 6);
  assert(dot_product<SemiRingMinPlus<ll>>(0, a.begin(), b.begin()) == SemiRingMinPlus<ll>::e0());

  vc<modint998244353> shifted_a{2, 3}, shifted_b{4, 5};
  assert(dot_product<ShiftedResidues>(2, shifted_a.begin(), shifted_b.begin()) == modint998244353(12));
  assert(dot_product<ShiftedResidues>(0, shifted_a.begin(), shifted_b.begin()) == modint998244353(1));

  check_modular_ring<static_modint32<12>>(); // Zero divisors: not a field.
  check_modular_ring<modint998244353>();
  check_modular_ring<modint61>(); // Generic path for 64 bit residues.
  for (int mod : {1, 12, 1 << 30, (1 << 30) + 1, 2147483647})
  {
    dynamic_modint32<79>::set_mod(mod);
    check_modular_ring<dynamic_modint32<79>>();
  }
  PRINT("Hello World");
}
