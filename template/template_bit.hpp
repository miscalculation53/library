#pragma once

#include "template_types.hpp"
#include "template_rep.hpp"

/**
 * @brief テンプレート（ビット演算）
 * @docs docs/template/template_bit.md
 */

inline constexpr ull pow2(auto k) { return 1ULL << k; }
inline constexpr ull MASK(auto k) { return (1ULL << k) - 1ULL; }

#if __cplusplus < 202002L
// x == 0 ならば 0、そうでなければ 1 + floor(log2(x))
// 0, 1, 2, 2, 3, 3, 3, 3, 4, 4, ... 
inline constexpr ull bit_width(ull x) { return x == 0 ? 0 : 64 - __builtin_clzll(x); }
// 0, 1, 2, 2, 4, 4, 4, 4, 8, 8, ...
inline constexpr ull bit_floor(ull x) { return x == 0 ? 0ULL : 1ULL << (bit_width(x) - 1); }
// 1, 1, 2, 4, 4, 8, 8, 8, 8, 16, ...
inline constexpr ull bit_ceil(ull x) { return x == 0 ? 1ULL : 1ULL << bit_width(x - 1); }
inline constexpr ull countr_zero(ull x) { assert(x != 0); return __builtin_ctzll(x); }
inline constexpr ull popcount(ull x) { return __builtin_popcountll(x); }
inline constexpr bool has_single_bit(ull x) { return popcount(x) == 1; }
#else
// 0, 1, 2, 2, 3, 3, 3, 3, 4, 4, ... 
inline constexpr ll bit_width(ll x) { return std::bit_width((ull)x); }
// 0, 1, 2, 2, 4, 4, 4, 4, 8, 8, ...
inline constexpr ll bit_floor(ll x) { return std::bit_floor((ull)x); }
// 1, 1, 2, 4, 4, 8, 8, 8, 8, 16, ...
inline constexpr ll bit_ceil(ll x) { return std::bit_ceil((ull)x); }
inline constexpr ll countr_zero(ll x) { assert(x != 0); return std::countr_zero((ull)x); }
inline constexpr ll popcount(ll x) { return std::popcount((ull)x); }
inline constexpr bool has_single_bit(ll x) { return std::has_single_bit((ull)x); }
#endif

inline constexpr ull lsb_pos(ull x) { assert(x != 0); return countr_zero(x); }
inline constexpr ull msb_pos(ull x) { assert(x != 0); return bit_width(x) - 1; }
inline constexpr ull lsb_mask(ull x) { assert(x != 0); return x & -x; }
inline constexpr ull msb_mask(ull x) { assert(x != 0); return bit_floor(x); }

inline constexpr bool btest(ull x, uint k) { return (x >> k) & 1; }
template <class T>
inline void bset(T &x, uint k, bool b = 1) { b ? x |= (1ULL << k) : x &= ~(1ULL << k); }
template <class T>
inline void bflip(T &x, uint k) { x ^= (1ULL << k); }
inline constexpr bool bsubset(ull x, ull y) { return (x & y) == x; }
inline constexpr bool bsupset(ull x, ull y) { return (x & y) == y; }
inline constexpr ull bsetminus(ull x, ull y) { return x & ~y; }