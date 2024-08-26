#pragma once

#include "template_types.hpp"
#include "template_rep.hpp"
#include "template_math.hpp"
#if __cplusplus < 202002L
  ull bit_ceil(ull x) { ull y = 1; while (y < x) y <<= 1; return y; }
  ull bit_floor(ull x) { ull y = 1; while (y <= x) y <<= 1; return y >> 1; }
  ull bit_width(ull x) { ull y = 1, z = 0; while (y <= x) y <<= 1, z++; return z; }
  ull countr_zero(ull x) { return __builtin_ctzll(x); }
  ull popcount(ull x) { return __builtin_popcountll(x); }
  ull has_single_bit(ull x) { return popcount(x) == 1; }
#endif
ull lsb_pos(ull x) { assert(x != 0); return countr_zero(x); }
ull msb_pos(ull x) { assert(x != 0); return bit_width(x) - 1; }
ull lsb_mask(ull x) { assert(x != 0); return x & -x; }
ull msb_mask(ull x) { assert(x != 0); return bit_floor(x); }
bool btest(ull x, uint k) { return (x >> k) & 1; }
template <class T> void bset(T &x, uint k, bool b = 1) { b ? x |= (1ULL << k) : x &= ~(1ULL << k); }
template <class T> void bflip(T &x, uint k) { x ^= (1ULL << k); }
bool bsubset(ull x, ull y) { return (x & y) == x; }
template <class T> vector<pair<T, T>> bsubsets(T x) { vector<pair<T, T>> res; for (T y = x; y > 0; y = (y - 1) & x) res.emplace_back(make_pair(y, x & ~y)); res.emplace_back(make_pair(0, x)); return res; }