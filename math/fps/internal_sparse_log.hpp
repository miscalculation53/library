#pragma once

// Coefficient identities: f * f^{-1} = 1 and f * (x (log f)') = x f'.
// Select the scalar path explicitly and reuse f[0]^-1.

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <limits>
#include <numeric>
#include <type_traits>
#include <utility>
#include <vector>

namespace internal::sparse_fps
{

// The val() interface must return the canonical residue in [0, mod()).
// Construction from an integer must produce its residue.  No assumption is
// made about the representation stored inside Mint.
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
          sizeof(decltype(Mint::mod())) <= sizeof(std::uint32_t) &&
          sizeof(decltype(std::declval<const Mint &>().val())) <=
              sizeof(std::uint32_t)>
{
};

template <class Mint>
struct input_shape
{
  int stride = 1;
  int length = 0;
  std::vector<std::pair<int, Mint>> terms;
};

// If f(x) = F(x^d) modulo x^n, compute in the variable y = x^d.
template <class Fps>
auto describe_input(const Fps &f, int n)
{
  using Mint = typename Fps::value_type;
  input_shape<Mint> shape;
  int common_degree = 0;
  const int stop = std::min(n, static_cast<int>(f.size()));
  for (int degree = 1; degree < stop; ++degree)
  {
    if (f[degree] != Mint(0))
    {
      shape.terms.emplace_back(degree, f[degree]);
      common_degree = std::gcd(common_degree, degree);
    }
  }
  if (common_degree != 0)
    shape.stride = common_degree;
  shape.length = n == 0 ? 0 : (n - 1) / shape.stride + 1;
  for (auto &term : shape.terms)
    term.first /= shape.stride;
  return shape;
}

class dot32
{
  std::uint32_t modulus;
  std::size_t block_size;

public:
  explicit dot32(std::uint32_t p) : modulus(p)
  {
    assert(p >= 2);
    const std::uint64_t largest = std::uint64_t(p) - 1;
    const std::uint64_t safe_products =
        (std::numeric_limits<std::uint64_t>::max() - largest) /
        (largest * largest);
    block_size = static_cast<std::size_t>(
        std::min<std::uint64_t>(16, safe_products));
  }

  std::uint32_t operator()(
      const std::vector<std::pair<int, std::uint32_t>> &terms,
      const std::vector<std::uint32_t> &values, int degree,
      std::size_t active, std::uint32_t seed) const
  {
    std::uint64_t carry = seed;
    std::size_t first = 0;
    while (first < active)
    {
      const std::size_t stop = std::min(active, first + block_size);
      std::uint64_t s0 = carry, s1 = 0, s2 = 0, s3 = 0;
      for (; first + 4 <= stop; first += 4)
      {
        const auto &a = terms[first];
        const auto &b = terms[first + 1];
        const auto &c = terms[first + 2];
        const auto &d = terms[first + 3];
        s0 += std::uint64_t(a.second) * values[degree - a.first];
        s1 += std::uint64_t(b.second) * values[degree - b.first];
        s2 += std::uint64_t(c.second) * values[degree - c.first];
        s3 += std::uint64_t(d.second) * values[degree - d.first];
      }
      for (; first < stop; ++first)
      {
        const auto &term = terms[first];
        s0 += std::uint64_t(term.second) * values[degree - term.first];
      }
      carry = (s0 + s1 + s2 + s3) % modulus;
    }
    return static_cast<std::uint32_t>(carry);
  }
};

// Internal implementation of f.log(n).
// Prime modulus and 0 <= n <= modulus are the existing log preconditions.
template <bool UseCanonical32, class Fps>
Fps logarithm(const Fps &f, int n)
{
  using Mint = typename Fps::value_type;
  static_assert(!UseCanonical32 || has_canonical32_interface<Mint>::value,
                "32 bit scalar path requires canonical val() and mod()");
  assert(n >= 0);
  assert(!f.empty() && f[0] == Mint(1));
  assert(static_cast<std::uint64_t>(n) <=
         static_cast<std::uint64_t>(Mint::mod()));
  Fps answer(n);
  if (n <= 1)
    return answer;
  const auto shape = describe_input(f, n);

  if constexpr (UseCanonical32)
  {
    const auto p = static_cast<std::uint32_t>(Mint::mod());
    std::vector<std::pair<int, std::uint32_t>> terms;
    terms.reserve(shape.terms.size());
    std::vector<std::uint32_t> values(shape.length);
    for (const auto &term : shape.terms)
    {
      const auto value = static_cast<std::uint32_t>(term.second.val());
      terms.emplace_back(term.first, p - value);
      values[term.first] = static_cast<std::uint32_t>(
          std::uint64_t(term.first) * value % p);
    }
    const dot32 dot(p);
    std::size_t active = 0;
    for (int degree = 1; degree < shape.length; ++degree)
    {
      while (active < terms.size() && terms[active].first <= degree)
        ++active;
      values[degree] = dot(terms, values, degree, active, values[degree]);
    }
    std::vector<std::uint32_t> inverses(shape.length);
    if (shape.length > 1)
      inverses[1] = 1;
    for (int degree = 2; degree < shape.length; ++degree)
    {
      const auto quotient = p / static_cast<std::uint32_t>(degree);
      const auto remainder = p % static_cast<std::uint32_t>(degree);
      inverses[degree] = static_cast<std::uint32_t>(
          p - std::uint64_t(quotient) * inverses[remainder] % p);
    }
    for (int degree = 1; degree < shape.length; ++degree)
      answer[degree * shape.stride] = Mint(
          std::uint64_t(values[degree]) * inverses[degree] % p);
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
    std::size_t active = 0;
    for (int degree = 1; degree < shape.length; ++degree)
    {
      while (active < terms.size() && terms[active].first <= degree)
        ++active;
      Mint coefficient = values[degree];
      for (std::size_t j = 0; j < active; ++j)
        coefficient += terms[j].second * values[degree - terms[j].first];
      values[degree] = coefficient;
    }
    std::vector<Mint> inverses(shape.length, Mint(0));
    if (shape.length > 1)
      inverses[1] = Mint(1);
    const auto p = Mint::mod();
    for (int degree = 2; degree < shape.length; ++degree)
      inverses[degree] = -Mint(p / degree) * inverses[p % degree];
    for (int degree = 1; degree < shape.length; ++degree)
      answer[degree * shape.stride] = values[degree] * inverses[degree];
  }
  return answer;
}

} // namespace internal::sparse_fps
