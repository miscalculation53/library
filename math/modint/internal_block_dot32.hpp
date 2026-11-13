#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>

namespace internal
{

// next() returns the next product of two residues below mod.
// mod and block are positive, seed < mod, and block*(mod-1)^2+(mod-1) fits uint64_t.
template <class NextProduct>
inline std::uint32_t block_dot32(std::size_t n, std::uint32_t mod, std::size_t block,
                                 std::uint32_t seed, NextProduct next)
{
  std::uint64_t carry = seed;
  while (n)
  {
    const std::size_t count = std::min(n, block);
    std::size_t i = 0;
    std::uint64_t s0 = carry, s1 = 0, s2 = 0, s3 = 0;
    for (; i + 4 <= count; i += 4)
    {
      s0 += next();
      s1 += next();
      s2 += next();
      s3 += next();
    }
    for (; i < count; ++i) s0 += next();
    carry = (s0 + s1 + s2 + s3) % mod;
    n -= count;
  }
  return static_cast<std::uint32_t>(carry);
}

}
