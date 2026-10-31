#pragma once

#include "../../ds/dynamic_bitset.hpp"

// Packed bit references and non-owning matrix rows.

namespace internal
{
struct matrix_bit_reference
{
  using Word = DynamicBitset::Word;
  Word *value;
  Word mask;
  operator bool() const { return (*value & mask) != 0; }
  matrix_bit_reference &operator=(bool b)
  {
    if (b)
      *value |= mask;
    else
      *value &= ~mask;
    return *this;
  }
  matrix_bit_reference &operator=(const matrix_bit_reference &b) { return *this = bool(b); }
  matrix_bit_reference &flip()
  {
    *value ^= mask;
    return *this;
  }
};

template <bool Const> struct matrix_mod2_row
{
  using Word = DynamicBitset::Word;
  using Pointer = conditional_t<Const, const Word *, Word *>;
  Pointer data;
  int n;
  int size() const { return n; }
  int word_count() const { return n / 64 + (n % 64 != 0); }
  bool empty() const { return n == 0; }
  Word word(int i) const
  {
    assert(0 <= i && i < word_count());
    return data[i];
  }
  Word tail_mask() const { return n % 64 ? (Word(1) << (n % 64)) - 1 : ~Word(0); }
  matrix_mod2_row &set_word(int i, Word x)
  {
    assert(0 <= i && i < word_count());
    data[i] = i + 1 == word_count() ? x & tail_mask() : x;
    return *this;
  }
  bool test(int i) const
  {
    assert(0 <= i && i < n);
    return (data[i / 64] >> (i % 64)) & 1;
  }
  auto operator[](int i) const
  {
    assert(0 <= i && i < n);
    if constexpr (Const)
      return test(i);
    else
      return matrix_bit_reference{data + i / 64, Word(1) << (i % 64)};
  }
  matrix_mod2_row &set(int i, bool value = true)
  {
    (*this)[i] = value;
    return *this;
  }
  matrix_mod2_row &set()
  {
    for (int w = 0; w < word_count(); ++w)
      set_word(w, ~Word(0));
    return *this;
  }
  matrix_mod2_row &reset(int i) { return set(i, false); }
  matrix_mod2_row &reset()
  {
    for (int w = 0; w < word_count(); ++w)
      data[w] = 0;
    return *this;
  }
  matrix_mod2_row &flip(int i)
  {
    (*this)[i].flip();
    return *this;
  }
  matrix_mod2_row &flip()
  {
    for (int w = 0; w < word_count(); ++w)
      set_word(w, ~data[w]);
    return *this;
  }
  template <int Op> matrix_mod2_row &change_range(int l, int r)
  {
    assert(0 <= l && l <= r && r <= n);
    for (int w = l / 64; l < r && w <= (r - 1) / 64; ++w)
    {
      int lo = max(l, w * 64) - w * 64, hi = min(r, (w + 1) * 64) - w * 64;
      Word mask = (hi == 64 ? ~Word(0) : (Word(1) << hi) - 1) & (~Word(0) << lo);
      if constexpr (Op == 0)
        data[w] &= ~mask;
      if constexpr (Op == 1)
        data[w] |= mask;
      if constexpr (Op == 2)
        data[w] ^= mask;
    }
    return *this;
  }
  matrix_mod2_row &reset_range(int l, int r) { return change_range<0>(l, r); }
  matrix_mod2_row &set_range(int l, int r) { return change_range<1>(l, r); }
  matrix_mod2_row &flip_range(int l, int r) { return change_range<2>(l, r); }
  bool any() const
  {
    for (int w = 0; w < word_count(); ++w)
      if (data[w])
        return true;
    return false;
  }
  bool none() const { return !any(); }
  bool all() const { return count() == n; }
  int count() const
  {
    int c = 0;
    for (int w = 0; w < word_count(); ++w)
      c += __builtin_popcountll(data[w]);
    return c;
  }
  int find_first(bool value = true) const { return find_next(-1, value); }
  int find_next(int i, bool value = true) const
  {
    assert(i >= -1);
    if (i >= n - 1)
      return n;
    ++i;
    for (int w = i / 64; w < word_count(); ++w)
    {
      Word x = value ? data[w] : ~data[w];
      if (w == i / 64)
        x &= ~Word(0) << (i % 64);
      if (w + 1 == word_count())
        x &= tail_mask();
      if (x)
        return w * 64 + __builtin_ctzll(x);
    }
    return n;
  }
  template <class Row> matrix_mod2_row &assign(const Row &b)
  {
    assert(n == b.size());
    for (int w = 0; w < word_count(); ++w)
      data[w] = b.word(w);
    return *this;
  }
  matrix_mod2_row &operator=(const matrix_mod2_row &b) { return assign(b); }
  template <class Row> matrix_mod2_row &operator=(const Row &b) { return assign(b); }
  template <class Row> matrix_mod2_row &operator^=(const Row &b)
  {
    assert(n == b.size());
    for (int w = 0; w < word_count(); ++w)
      data[w] ^= b.word(w);
    return *this;
  }
  template <class Row> bool operator==(const Row &b) const
  {
    if (n != b.size())
      return false;
    for (int w = 0; w < word_count(); ++w)
      if (data[w] != b.word(w))
        return false;
    return true;
  }
  template <class Row> bool operator!=(const Row &b) const { return !(*this == b); }
  operator DynamicBitset() const
  {
    DynamicBitset result(n);
    for (int w = 0; w < word_count(); ++w)
      result.set_word(w, data[w]);
    return result;
  }
};
template <bool Const> bool operator==(const DynamicBitset &a, const matrix_mod2_row<Const> &b)
{
  return b == a;
}
} // namespace internal
