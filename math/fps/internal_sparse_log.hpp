#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <numeric>
#include <type_traits>
#include <utility>
#include <vector>

#include "../modint/internal_block_dot32.hpp"

namespace internal::sparse_fps
{

using std::size_t, std::uint32_t, std::uint64_t;

template <class Mint, class = void>
struct has_canonical32_interface : std::false_type
{
};

template <class Mint>
struct has_canonical32_interface<
    Mint, std::void_t<decltype(Mint::mod()),
                      decltype(std::declval<const Mint &>().val())>>
    : std::bool_constant<
          std::is_integral_v<decltype(Mint::mod())> &&
          std::is_integral_v<decltype(std::declval<const Mint &>().val())> &&
          sizeof(decltype(Mint::mod())) <= sizeof(uint32_t) &&
          sizeof(decltype(std::declval<const Mint &>().val())) <=
              sizeof(uint32_t)>
{
};

template <class Mint>
struct input_shape
{
  int stride = 1;
  int length = 0;
  std::vector<std::pair<int, Mint>> terms;
};

template <class Fps>
auto describe_input(const Fps &f, int n)
{
  using Mint = typename Fps::value_type;
  input_shape<Mint> shape;
  int common_degree = 0;
  const int stop = std::min(n, static_cast<int>(f.size()));
  for (int deg = 1; deg < stop; ++deg)
  {
    if (f[deg] != Mint(0))
    {
      shape.terms.emplace_back(deg, f[deg]);
      common_degree = std::gcd(common_degree, deg);
    }
  }
  if (common_degree != 0)
    shape.stride = common_degree;
  shape.length = n == 0 ? 0 : (n - 1) / shape.stride + 1;
  for (auto &term : shape.terms)
    term.first /= shape.stride;
  return shape;
}

inline size_t block_size32(uint32_t p)
{
  assert(p >= 2);
  const uint64_t largest = uint64_t(p) - 1;
  const auto safe = (std::numeric_limits<uint64_t>::max() - largest) / (largest * largest);
  return static_cast<size_t>(std::min<uint64_t>(16, safe));
}

// Prime modulus and 0 <= n <= modulus are the existing log preconditions.
template <bool UseCanonical32, class Fps>
Fps logarithm(const Fps &f, int n)
{
  using Mint = typename Fps::value_type;
  static_assert(!UseCanonical32 || has_canonical32_interface<Mint>::value,
                "32 bit scalar path requires canonical val() and mod()");
  assert(n >= 0);
  assert(!f.empty() && f[0] == Mint(1));
  assert(static_cast<uint64_t>(n) <=
         static_cast<uint64_t>(Mint::mod()));
  Fps answer(n);
  if (n <= 1)
    return answer;
  const auto shape = describe_input(f, n);

  if constexpr (UseCanonical32)
  {
    const auto p = static_cast<uint32_t>(Mint::mod());
    std::vector<std::pair<int, uint32_t>> terms;
    terms.reserve(shape.terms.size());
    std::vector<uint32_t> values(shape.length);
    for (const auto &term : shape.terms)
    {
      const auto value = static_cast<uint32_t>(term.second.val());
      terms.emplace_back(term.first, p - value);
      values[term.first] = static_cast<uint32_t>(
          uint64_t(term.first) * value % p);
    }
    const size_t block = block_size32(p);
    size_t active = 0;
    for (int deg = 1; deg < shape.length; ++deg)
    {
      while (active < terms.size() && terms[active].first <= deg)
        ++active;
      size_t i = 0;
      values[deg] = block_dot32(active, p, block, values[deg], [&]()
      {
        const auto &[offset, value] = terms[i++];
        return uint64_t(value) * values[deg - offset];
      });
    }
    std::vector<uint32_t> inverses(shape.length);
    if (shape.length > 1)
      inverses[1] = 1;
    for (int deg = 2; deg < shape.length; ++deg)
    {
      const auto quotient = p / static_cast<uint32_t>(deg);
      const auto remainder = p % static_cast<uint32_t>(deg);
      inverses[deg] = static_cast<uint32_t>(
          p - uint64_t(quotient) * inverses[remainder] % p);
    }
    for (int deg = 1; deg < shape.length; ++deg)
      answer[deg * shape.stride] = Mint(
          uint64_t(values[deg]) * inverses[deg] % p);
  }
  else
  {
    auto terms = shape.terms;
    std::vector<Mint> values(shape.length, Mint(0));
    for (auto &term : terms)
    {
      values[term.first] = Mint(term.first) * term.second;
      term.second = -term.second;
    }
    size_t active = 0;
    for (int deg = 1; deg < shape.length; ++deg)
    {
      while (active < terms.size() && terms[active].first <= deg)
        ++active;
      Mint coefficient = values[deg];
      for (size_t j = 0; j < active; ++j)
        coefficient += terms[j].second * values[deg - terms[j].first];
      values[deg] = coefficient;
    }
    std::vector<Mint> inverses(shape.length, Mint(0));
    if (shape.length > 1)
      inverses[1] = Mint(1);
    const auto p = Mint::mod();
    for (int deg = 2; deg < shape.length; ++deg)
      inverses[deg] = -Mint(p / deg) * inverses[p % deg];
    for (int deg = 1; deg < shape.length; ++deg)
      answer[deg * shape.stride] = values[deg] * inverses[deg];
  }
  return answer;
}

}
