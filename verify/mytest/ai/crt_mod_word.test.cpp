#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>

#ifndef CRT_WORD_HEADER
#define CRT_WORD_HEADER "math/crt.hpp"
#endif
#include CRT_WORD_HEADER
#include "math/modint/modint.hpp"
#include "math/bigint.hpp"

namespace
{
using U = uint64_t;
constexpr auto ordinary = crt_mod_constexpr<U>(std::array<unsigned, 2>{0, 1},
                                              std::array<unsigned, 2>{3, 7});
static_assert(ordinary.first == 15 && ordinary.second == 21);
constexpr auto signed_ordinary = crt_mod_constexpr<U>(std::array<int, 2>{0, 1},
                                                     std::array<int, 2>{3, 7});
static_assert(signed_ordinary.first == 15 && signed_ordinary.second == 21);
constexpr auto empty = crt_mod_constexpr<U>(std::array<int, 0>{}, std::array<unsigned, 0>{});
static_assert(empty.first == 0 && empty.second == 1);
constexpr auto ones = crt_mod_constexpr<U>(std::array<int, 2>{0, 0}, std::array<bool, 2>{true, true});
static_assert(ones.first == 0 && ones.second == 1);

#ifdef __SIZEOF_INT128__
using Wide = __uint128_t;
constexpr Wide maximum = ~Wide(0);
constexpr auto single = crt_mod_constexpr<U>(std::array<Wide, 1>{Wide(1) << 70},
                                            std::array<Wide, 1>{(Wide(1) << 80) + 7});
static_assert(single.first == 0 && single.second == 7);
constexpr auto product = crt_mod_constexpr<U>(std::array<Wide, 2>{0, maximum - 2},
                                             std::array<Wide, 2>{maximum, maximum - 1});
static_assert(product.first == 3 && product.second == 2);
constexpr auto inverse = crt_mod_constexpr<U>(std::array<Wide, 2>{0, 1},
                                             std::array<Wide, 2>{maximum - 1, maximum});
static_assert(inverse.first == 4 && inverse.second == 2);
#endif
}

int main()
{
  const auto signed32 = crt_mod<U>(std::vector<int>{0, 1}, std::vector<int>{3, 7});
  assert(signed32.first == 15 && signed32.second == 21);
  const auto signed64 = crt_mod<U>(std::vector<int64_t>{0, 1}, std::vector<int64_t>{3, 7});
  assert(signed64.first == 15 && signed64.second == 21);
  const auto const_int = crt_mod<U>(std::array<const int, 2>{0, 1}, std::array<const int, 2>{3, 7});
  assert(const_int.first == 15 && const_int.second == 21);
  const auto const_unsigned = crt_mod<U>(std::array<const unsigned, 2>{0, 1},
                                        std::array<const unsigned, 2>{3, 7});
  assert(const_unsigned.first == 15 && const_unsigned.second == 21);
  const auto const_long = crt_mod<U>(std::array<const long long, 2>{0, 1},
                                    std::array<const long long, 2>{3, 7});
  assert(const_long.first == 15 && const_long.second == 21);
  const auto const_ulong = crt_mod<U>(std::array<const unsigned long long, 2>{0, 1},
                                     std::array<const unsigned long long, 2>{3, 7});
  assert(const_ulong.first == 15 && const_ulong.second == 21);
  const auto shifted = crt_mod<U>(std::vector<int>{-3, 15}, std::vector<unsigned>{3, 7});
  assert(shifted.first == 15 && shifted.second == 21);
  const auto unsigned32 = crt_mod<U>(std::array<unsigned, 1>{1}, std::array<unsigned, 1>{(1U << 31) + 1});
  assert(unsigned32.first == 1 && unsigned32.second == (U(1) << 31) + 1);
  const auto unsigned64 = crt_mod<U>(std::array<int, 1>{-1}, std::array<U, 1>{~U(0)});
  assert(unsigned64.first == ~U(0) - 1 && unsigned64.second == ~U(0));
  const auto boolean = crt_mod<U>(std::vector<bool>{false, true}, std::vector<unsigned>{3, 7});
  assert(boolean.first == 15 && boolean.second == 21);
  const auto units = crt_mod<U>(std::vector<int64_t>{std::numeric_limits<int64_t>::lowest(), -1},
                               std::vector<bool>{true, true});
  assert(units.first == 0 && units.second == 1);
  const auto reduced = crt_mod<modint998244353>(std::vector<int>{0, 1}, std::vector<unsigned>{3, 7});
  assert(reduced.first.val() == 15 && reduced.second.val() == 21);
  const auto exact_small = crt_mod<BigInteger<>>(std::vector<int>{0, 1}, std::vector<int>{3, 7});
  assert(exact_small.first == BigInteger<>(15) && exact_small.second == BigInteger<>(21));

#ifdef __SIZEOF_INT128__
  const auto endpoints = crt_mod<U>(
    std::array<__int128_t, 2>{std::numeric_limits<__int128_t>::lowest(),
                             std::numeric_limits<__int128_t>::max()},
    std::array<unsigned, 2>{3, 7});
  assert(endpoints.first == 1 && endpoints.second == 21);
  const auto exact = crt_mod<BigInteger<>>(std::array<Wide, 2>{0, maximum - 2},
                                          std::array<Wide, 2>{maximum, maximum - 1});
  const BigInteger<> m(maximum);
  assert(exact.first == m * BigInteger<>(maximum - 2) && exact.second == m * BigInteger<>(maximum - 1));
#endif

  std::cout << "Hello World" << std::endl;
}
