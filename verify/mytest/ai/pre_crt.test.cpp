#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include <array>
#include <cassert>
#include <cstdint>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <vector>

#ifndef PRE_CRT_HEADER
#define PRE_CRT_HEADER "math/crt.hpp"
#endif
#include PRE_CRT_HEADER

namespace
{
template <bool Floor> struct TestInteger
{
  int64_t value;
  TestInteger(int64_t x = 0) : value(x) {}
  friend TestInteger operator+(TestInteger a, TestInteger b) { return a.value + b.value; }
  friend TestInteger operator-(TestInteger a, TestInteger b) { return a.value - b.value; }
  friend TestInteger operator*(TestInteger a, TestInteger b) { return a.value * b.value; }
  friend TestInteger operator/(TestInteger a, TestInteger b)
  {
    int64_t q = a.value / b.value, r = a.value % b.value;
    if (Floor && r != 0 && (r < 0) != (b.value < 0)) --q;
    return q;
  }
  friend TestInteger operator%(TestInteger a, TestInteger b)
  {
    int64_t r = a.value % b.value;
    if (Floor && r != 0 && (r < 0) != (b.value < 0)) r += b.value;
    return r;
  }
  friend bool operator==(TestInteger a, TestInteger b) { return a.value == b.value; }
  friend bool operator!=(TestInteger a, TestInteger b) { return a.value != b.value; }
  friend bool operator<(TestInteger a, TestInteger b) { return a.value < b.value; }
  friend bool operator>(TestInteger a, TestInteger b) { return a.value > b.value; }
  friend bool operator>=(TestInteger a, TestInteger b) { return a.value >= b.value; }
  friend TestInteger gcd(TestInteger a, TestInteger b)
  {
    while (b.value != 0) { TestInteger r = a % b; a = b; b = r; }
    return a;
  }
};

bool holds(int x, const std::vector<int> &rs, const std::vector<int> &ms)
{
  for (size_t i = 0; i < rs.size(); ++i)
    if ((x - rs[i]) % ms[i] != 0) return false;
  return true;
}

std::vector<int> prime_allocation(const std::vector<int> &ms)
{
  std::map<int, std::pair<int, size_t>> best;
  for (size_t i = 0; i < ms.size(); ++i)
  {
    int m = ms[i];
    for (int p = 2; p * p <= m; ++p)
    {
      if (m % p != 0) continue;
      int power = 1;
      do { m /= p; power *= p; } while (m % p == 0);
      if (power >= best[p].first) best[p] = {power, i};
    }
    if (m > 1 && m >= best[m].first) best[m] = {m, i};
  }
  std::vector<int> out(ms.size(), 1);
  for (const auto &entry : best)
    out[entry.second.second] *= entry.second.first;
  return out;
}

void check_small(const std::vector<int> &rs, const std::vector<int> &input)
{
  int period = 1;
  for (int m : input) period = std::lcm(period, m);
  bool exists = false;
  for (int x = 0; x < period; ++x) exists |= holds(x, rs, input);
  auto ms = input;
  assert(pre_crt(rs, ms) == exists);
  if (exists)
  {
    assert(ms == prime_allocation(input));
    for (int x = 0; x < period; ++x)
      assert(holds(x, rs, input) == holds(x, rs, ms));
  }
}

template <class R, class M>
void expect_success(R rs, M ms, const M &expected)
{
  const R saved = rs;
  assert(pre_crt(rs, ms));
  assert(rs == saved && ms == expected);
}

template <class R, class M>
void expect_false(const R &rs, M ms)
{
  assert(!pre_crt(rs, ms));
}

template <class M>
void expect_alias(M ms, const M &expected)
{
  assert(pre_crt(ms, ms));
  assert(ms == expected);
}

template <bool Floor> void check_generic()
{
  using T = TestInteger<Floor>;
  const int64_t lo = std::numeric_limits<int64_t>::lowest();
  const int64_t hi = std::numeric_limits<int64_t>::max();
  expect_success(std::vector<T>{lo, lo, lo}, std::vector<T>{288, 432, 96},
                 std::vector<T>{1, 27, 32});
  expect_success(std::array<T, 2>{lo, hi}, std::array<unsigned, 2>{3, 6},
                 std::array<unsigned, 2>{1, 6});
  expect_success(std::array<int32_t, 2>{std::numeric_limits<int32_t>::lowest(), 2147483647},
                 std::array<T, 2>{3, 6}, std::array<T, 2>{1, 6});
  expect_false(std::array<T, 2>{0, 1}, std::array<T, 2>{4, 6});
  expect_alias(std::vector<T>{12, 18, 8, 27, 1}, std::vector<T>{1, 1, 8, 27, 1});
}
}

int main()
{
  for (int n = 0; n <= 3; ++n)
  {
    std::vector<int> rs(n), ms(n);
    std::function<void(int)> enumerate = [&](int i)
    {
      if (i == n) { check_small(rs, ms); return; }
      for (int m = 1; m <= 6; ++m)
      {
        ms[i] = m;
        for (int r = 0; r < m; ++r)
        {
          rs[i] = r + (i % 3 - 1) * m;
          enumerate(i + 1);
        }
      }
    };
    enumerate(0);
  }

  using I = int64_t;
  using U = uint64_t;
  const I lo = std::numeric_limits<I>::lowest();
  const I hi = std::numeric_limits<I>::max();
  const U uhi = std::numeric_limits<U>::max();
  expect_success(std::array<I, 0>{}, std::array<U, 0>{}, std::array<U, 0>{});
  expect_success(std::vector<I>{lo}, std::vector<I>{hi}, std::vector<I>{hi});
  expect_success(std::array<I, 3>{lo, hi, -1}, std::array<U, 3>{1, 1, 1},
                 std::array<U, 3>{1, 1, 1});
  expect_success(std::vector<int>{17, 17, 17}, std::vector<int>{288, 432, 96},
                 std::vector<int>{1, 27, 32});
  expect_success(std::vector<bool>{true, true}, std::vector<int>{3, 6},
                 std::vector<int>{1, 6});
  expect_false(std::vector<bool>{false, true}, std::vector<int>{2, 4});
  expect_success(std::array<I, 2>{17, 17}, std::array<I, 2>{I(1) << 60, I(1) << 60},
                 std::array<I, 2>{1, I(1) << 60});
  expect_alias(std::array<I, 2>{I(1) << 60, I(1) << 60}, std::array<I, 2>{1, I(1) << 60});
  check_generic<false>();
  check_generic<true>();
  const std::array<const int, 2> qualified_rs{1, 1};
  std::array<volatile int, 2> qualified_ms{6, 10};
  assert(pre_crt(qualified_rs, qualified_ms));
  assert(qualified_ms[0] == 3 && qualified_ms[1] == 10);
  expect_success(std::array<int32_t, 2>{std::numeric_limits<int32_t>::lowest(), 2147483647},
                 std::array<int32_t, 2>{3, 6}, std::array<int32_t, 2>{1, 6});
  expect_success(std::array<I, 2>{lo, hi}, std::array<I, 2>{3, 6},
                 std::array<I, 2>{1, 6});
  expect_success(std::array<I, 2>{lo, hi}, std::array<I, 2>{hi, hi - 1},
                 std::array<I, 2>{hi, hi - 1});
  expect_success(std::array<U, 2>{0, uhi}, std::array<U, 2>{3, 6},
                 std::array<U, 2>{1, 6});
  expect_success(std::array<int32_t, 2>{-1, -1}, std::array<U, 2>{uhi, uhi - 1},
                 std::array<U, 2>{uhi, uhi - 1});
  expect_success(std::array<U, 2>{U(1) << 63, 2}, std::array<int32_t, 2>{2147483647, 2147483647},
                 std::array<int32_t, 2>{1, 2147483647});
  expect_false(std::vector<int>{0, 1}, std::vector<int>{4, 6});
  expect_false(std::array<I, 2>{lo, hi}, std::array<U, 2>{2, 4});
  expect_alias(std::vector<int>{12, 18, 8, 27, 1}, std::vector<int>{1, 1, 8, 27, 1});
  expect_alias(std::array<U, 2>{uhi, uhi}, std::array<U, 2>{1, uhi});
  expect_alias(std::array<int, 0>{}, std::array<int, 0>{});

#ifdef __SIZEOF_INT128__
  using WideI = __int128_t;
  using WideU = __uint128_t;
  const WideI wide_lo = std::numeric_limits<WideI>::lowest();
  const WideI wide_hi = std::numeric_limits<WideI>::max();
  const WideU wide_uhi = std::numeric_limits<WideU>::max();
  expect_success(std::array<WideI, 2>{wide_lo, wide_hi}, std::array<WideU, 2>{wide_uhi, wide_uhi},
                 std::array<WideU, 2>{1, wide_uhi});
  expect_success(std::array<WideU, 2>{WideU(1) << 100, 128}, std::array<int32_t, 2>{2147483647, 2147483647},
                 std::array<int32_t, 2>{1, 2147483647});
  expect_success(std::array<WideI, 2>{wide_lo, 2147483647}, std::array<uint32_t, 2>{4294967295U, 4294967295U},
                 std::array<uint32_t, 2>{1, 4294967295U});
  expect_success(std::array<WideU, 2>{wide_uhi, 3}, std::array<I, 2>{hi, hi},
                 std::array<I, 2>{1, hi});
  WideU power3 = 1;
  for (int e = 0; e < 80; ++e) power3 *= 3;
  expect_success(std::vector<WideU>(4, 0),
                 std::vector<WideU>{WideU(1) << 127, power3, 3 * (WideU(1) << 126), 2 * (power3 / 3)},
                 std::vector<WideU>{WideU(1) << 127, power3, 1, 1});
  expect_false(std::array<WideI, 2>{wide_lo, wide_hi}, std::array<WideU, 2>{2, 4});
  expect_alias(std::array<WideU, 2>{wide_uhi, wide_uhi}, std::array<WideU, 2>{1, wide_uhi});
#endif

  std::cout << "Hello World" << std::endl;
}
