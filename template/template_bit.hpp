#pragma once

#include "template_types.hpp"
#include "template_rep.hpp"

/**
 * @brief テンプレート（ビット演算）
 * @docs docs/template/template_bit.md
 */

inline ull pow2(auto k) { return 1ULL << k; }
inline ull MASK(auto k) { return (1ULL << k) - 1; }

#if __cplusplus < 202002L
// x == 0 ならば 0、そうでなければ 1 + floor(log2(x))
// 0, 1, 2, 2, 3, 3, 3, 3, 4, 4, ... 
inline ull bit_width(ull x) { return x == 0 ? 0 : 64 - __builtin_clzll(x); }
// 0, 1, 2, 2, 4, 4, 4, 4, 8, 8, ...
inline ull bit_floor(ull x) { return x == 0 ? 0ULL : 1ULL << (bit_width(x) - 1); }
// 1, 1, 2, 4, 4, 8, 8, 8, 8, 16, ...
inline ull bit_ceil(ull x) { return x == 0 ? 1ULL : 1ULL << bit_width(x - 1); }
inline ull countr_zero(ull x) { assert(x != 0); return __builtin_ctzll(x); }
inline ull popcount(ull x) { return __builtin_popcountll(x); }
inline bool has_single_bit(ull x) { return popcount(x) == 1; }
#else
// 0, 1, 2, 2, 3, 3, 3, 3, 4, 4, ... 
inline ll bit_width(ll x) { return bit_width((ull)x); }
// 0, 1, 2, 2, 4, 4, 4, 4, 8, 8, ...
inline ll bit_floor(ll x) { return bit_floor((ull)x); }
// 1, 1, 2, 4, 4, 8, 8, 8, 8, 16, ...
inline ll bit_ceil(ll x) { return bit_ceil((ull)x); }
inline ll countr_zero(ll x) { assert(x != 0); return countr_zero((ull)x); }
inline ll popcount(ll x) { return popcount((ull)x); }
inline bool has_single_bit(ll x) { return has_single_bit((ull)x); }
#endif

inline ull lsb_pos(ull x) { assert(x != 0); return countr_zero(x); }
inline ull msb_pos(ull x) { assert(x != 0); return bit_width(x) - 1; }
inline ull lsb_mask(ull x) { assert(x != 0); return x & -x; }
inline ull msb_mask(ull x) { assert(x != 0); return bit_floor(x); }

inline bool btest(ull x, uint k) { return (x >> k) & 1; }
template <class T>
inline void bset(T &x, uint k, bool b = 1) { b ? x |= (1ULL << k) : x &= ~(1ULL << k); }
template <class T>
inline void bflip(T &x, uint k) { x ^= (1ULL << k); }
inline bool bsubset(ull x, ull y) { return (x & y) == x; }
inline bool bsupset(ull x, ull y) { return (x & y) == y; }
inline ull bsetminus(ull x, ull y) { return x & ~y; }

template <class T>
struct bsubsets
{
private:
  T x;
public:
  bsubsets(T x) : x(x) {}
  struct Iterator
  {
  private:
    T y;
    bool is_end;
    const bsubsets &bs;
  public:
    Iterator(T y, bool is_end, const bsubsets &bs) : y(y), is_end(is_end), bs(bs) {}
    T operator*() const { return y; }
    Iterator& operator++()
    {
      if (y == 0)
        is_end = true;
      y = (y - 1) & bs.x;
      return *this;
    }
    bool operator!=(const Iterator &other) const { return y != other.y || is_end != other.is_end; }
  };
  Iterator begin() const { return Iterator(x, false, *this); }
  Iterator end() const { return Iterator(x, true, *this); }
};
template <class T>
struct bsupsets
{
private:
  int n;
  T x;
public:
  bsupsets(int n, T x) : n(n), x(x) {}
  struct Iterator
  {
  private:
    T y;
    const bsupsets &bs;
  public:
    Iterator(T y, const bsupsets &bs) : y(y), bs(bs) {}
    T operator*() const { return y; }
    Iterator& operator++()
    {
      y = (y + 1) | bs.x;
      return *this;
    }
    bool operator!=(const Iterator &other) const { return y != other.y; }
  };
  Iterator begin() const { return Iterator(x, *this); }
  Iterator end() const { return Iterator((T(1) << n) | x, *this); }
};