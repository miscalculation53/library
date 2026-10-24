#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#ifndef SBT_INTERVAL_HEADER
#define SBT_INTERVAL_HEADER "math/sbt.hpp"
#endif
#include SBT_INTERVAL_HEADER

template <class T>
void check(T a, T b, T c, T d, T p, T q)
{
  static_assert(is_same_v<decltype(abc408g<T>(a, b, c, d)), pair<T, T>>);
  assert((abc408g<T>(a, b, c, d) == pair<T, T>{p, q}));
}

// 小さい分母から走査し、同じ分母では下端を超える最小の分子を選ぶ。
void small_intervals()
{
  for (int a = 0; a <= 12; ++a)
    for (int b = 1; b <= 12; ++b)
      for (int c = 1; c <= 12; ++c)
        for (int d = 0; d <= 12; ++d)
          if (d == 0 || a * d < c * b)
            for (int q = 1; q <= b + d; ++q)
            {
              const int p = a * q / b + 1;
              if (d != 0 && p * d >= c * q) continue;
              check<int>(a, b, c, d, p, q);
              check<ll>(a, b, c, d, p, q);
              check<ull>(a, b, c, d, p, q);
              check<i128>(a, b, c, d, p, q);
              check<u128>(a, b, c, d, p, q);
              break;
            }
}

constexpr u128 join(ull high, ull low) { return (u128(high) << 64) | low; }

template <class T>
void width_selection_intervals()
{
  const T m = (T(1) << 63) - 1;
  check<T>(m - 1, 1, m, 1, 2 * m - 1, 2);
  check<T>(1, m, 1, m - 1, 2, 2 * m - 1);
  check<T>(m, 1, m, 0, m + 1, 1);
  check<T>(m, 1, m + 1, 1, 2 * m + 1, 2);
  check<T>(1, m + 1, 1, m, 2, 2 * m + 1);
  check<T>(m + 1, 1, m + 1, 0, m + 2, 1);
  check<T>(m - 1, m, 1, 1, m, m + 1);
  check<T>(1, m, m, 0, 1, 1);
  check<T>(1, 2, T(1) << 100, 0, 1, 1);
  const T left = m / 2, right = m / 3;
  check<T>(left, 2 * left, 2 * right, 3 * right, 3, 5);
  const T wide_left = (m + 1) / 2, wide_right = (m + 1) / 3;
  check<T>(wide_left, 2 * wide_left, 2 * wide_right, 3 * wide_right, 3, 5);
}

void wide_intervals()
{
  const u128 umax = ~u128(0);
  const i128 imax = i128(umax >> 1);
  check<i128>(0, 1, 1, imax - 1, 1, imax);
  check<i128>(imax - 2, imax - 1, 1, 1, imax - 1, imax);
  check<i128>(imax - 1, 1, 1, 0, imax, 1);
  check<u128>(0, 1, 1, umax - 1, 1, umax);
  check<u128>(umax - 2, umax - 1, 1, 1, umax - 1, umax);
  check<u128>(umax - 1, 1, 1, 0, umax, 1);

  const u128 f181 = join(0x1693dfbc7860370dULL, 0x967e9d05ebc4b0d9ULL);
  const u128 f182 = join(0x248808541d00277bULL, 0x0108a86eaffa7949ULL);
  const u128 f183 = join(0x3b1be81095605e88ULL, 0x978745749bbf2a22ULL);
  const u128 f184 = join(0x5fa3f064b2608603ULL, 0x988fede34bb9a36bULL);
  const u128 f185 = join(0x9abfd87547c0e48cULL, 0x30173357e778cd8dULL);
  check<i128>(f182, f183, f181, f182, f183, f184);
  check<u128>(f182, f183, f183, f184, f184, f185);
}

int main()
{
  small_intervals();
  width_selection_intervals<i128>();
  width_selection_intervals<u128>();
  wide_intervals();
  check<ll>(0, 1, 1, 1, 1, 2);
  check<ll>(1, 2, 3, 2, 1, 1);
  check<ll>(1, 2, 7, 0, 1, 1);
  check<ll>(1, 2, 1, 1, 2, 3);
  check<ll>(0, 5, 7, 0, 1, 1);
  check<ll>(7, 3, 100, 0, 3, 1);
  check<ll>(2, 4, 4, 6, 3, 5);
  check<ll>(1, 1, 2, 1, 3, 2);
  check<ll>(5, 2, 9, 2, 3, 1);
  cout << "Hello World\n";
}
