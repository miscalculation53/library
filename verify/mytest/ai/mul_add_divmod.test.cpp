#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "utils/mul_add_divmod.hpp"
#include "math/bigint.hpp"

using Big = BigInteger<>;
constexpr auto example = mul_add_divmod(7ULL, 100ULL, 3ULL, 11ULL);
static_assert(example.first == 63 && example.second == 10);

template <class U>
void check(U a, U b, U c, U m)
{
  auto [q, r] = mul_add_divmod(a, b, c, m);
  const Big expected = Big(a) * Big(b) + Big(c);
  assert(Big(q) == expected / Big(m));
  assert(Big(r) == expected % Big(m));
  assert(q <= b && r < m);
}

template <class U>
void boundaries_and_random()
{
  const U hi = numeric_limits<U>::max();
  for (U m : {U(1), U(2), U(hi / 2), hi})
    for (U b : {U(0), U(1), U(hi / 2), hi})
      for (U a : {U(0), U(m / 2), U(m - 1)})
        for (U c : {U(0), U(m - 1)}) check(a, b, c, m);
  mt19937_64 rng(20260924);
  auto draw = [&]() { return U((u128(rng()) << 64) | rng()); };
  for (int i = 0; i < 1000; ++i)
  {
    const U m = draw() | 1;
    check(U(draw() % m), draw(), U(draw() % m), m);
  }
}

int main()
{
  for (uint m = 1; m <= 16; ++m)
    for (uint a = 0; a < m; ++a)
      for (uint b = 0; b <= 255; ++b)
        for (uint c = 0; c < m; ++c)
        {
          auto [q, r] = mul_add_divmod(static_cast<unsigned char>(a), static_cast<unsigned char>(b),
                                      static_cast<unsigned char>(c), static_cast<unsigned char>(m));
          assert(q == (a * b + c) / m && r == (a * b + c) % m);
        }
  boundaries_and_random<unsigned char>();
  boundaries_and_random<unsigned short>();
  boundaries_and_random<uint>();
  boundaries_and_random<ull>();
  boundaries_and_random<u128>();
  cout << "Hello World\n";
}
