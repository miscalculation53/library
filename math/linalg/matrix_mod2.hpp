#pragma once

#include "internal_matrix_mod2.hpp"

#include <climits>
#include <cstring>
#include <limits>
#include <type_traits>

namespace matrix_mod2_detail
{
// The fast path assumes the bit layout validated in the tested GNU 15 build:
// contiguous little-endian words; bit j is word j/64, bit j%64.
// Size and trivial-copyability checks supplement that validation; they do not prove layout.
template <std::size_t Capacity>
constexpr bool native_bitset_words =
#if defined(__GLIBCXX__) && defined(_GLIBCXX_RELEASE) && _GLIBCXX_RELEASE == 15 &&                 \
    CHAR_BIT == 8 && ULONG_MAX == UINT64_MAX && defined(__BYTE_ORDER__) &&                         \
    defined(__ORDER_LITTLE_ENDIAN__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    Capacity != 0 && std::is_trivially_copyable<std::bitset<Capacity>>::value &&
    sizeof(std::bitset<Capacity>) == (Capacity / 64 + (Capacity % 64 != 0)) * sizeof(word);
#else
    false;
#endif

template <std::size_t Capacity>
word read_word(const std::bitset<Capacity> &row, int index, std::size_t columns = Capacity)
{
  if constexpr (native_bitset_words<Capacity>)
  {
    word value;
    std::memcpy(&value,
                reinterpret_cast<const unsigned char *>(&row) + std::size_t(index) * sizeof(word),
                sizeof(word));
    return value;
  }
  else
  {
    word value = 0;
    const std::size_t begin = std::size_t(index) * 64;
    const unsigned count = unsigned(std::min(std::size_t(64), std::size_t(columns) - begin));
    for (unsigned bit = 0; bit < count; ++bit)
      value |= word(row[begin + bit]) << bit;
    return value;
  }
}

template <std::size_t Capacity>
void write_words(std::bitset<Capacity> &row, const word *source, int columns)
{
  const int width = word_count(columns);
  if constexpr (native_bitset_words<Capacity>)
  {
    if (width)
      std::memcpy(reinterpret_cast<unsigned char *>(&row), source,
                  std::size_t(width) * sizeof(word));
  }
  else
  {
    for (int bit = 0; bit < columns; ++bit)
      row[bit] = (source[bit / 64] >> (bit % 64)) & 1;
  }
}
} // namespace matrix_mod2_detail

/**
 * @brief 固定サイズの行列（$\mathbb{F}_2$）
 * @docs docs/math/linalg/matrix_mod2.md
 */
template <size_t N, size_t C = N> struct MatrixMod2 : array<bitset<C>, N>
{
  using M = MatrixMod2;
  using Vn = bitset<N>;
  using Vm = bitset<C>;
  using V = Vm;
  using Base = array<Vm, N>;
  using Base::at;
  static constexpr int n = N, m = C;
  MatrixMod2(int diag = 0, int non_diag = 0)
  {
    assert((diag == 0 || diag == 1) && (non_diag == 0 || non_diag == 1));
    if (non_diag)
      for (auto &r : *this)
        r.set();
    if (diag != non_diag)
      for (size_t i = 0; i < min(N, C); ++i)
        (*this)[i].flip(i);
  }
  MatrixMod2(const Base &a) : Base(a) {}
  template <class T> MatrixMod2(const vvc<T> &a)
  {
    assert(a.size() == N);
    for (size_t i = 0; i < N; ++i)
    {
      assert(a[i].size() == C);
      for (size_t j = 0; j < C; ++j)
      {
        assert(a[i][j] == T(0) || a[i][j] == T(1));
        (*this)[i][j] = a[i][j] == T(1);
      }
    }
  }
  MatrixMod2(const vc<string> &a)
  {
    assert(a.size() == N);
    for (size_t i = 0; i < N; ++i)
    {
      assert(a[i].size() == C);
      for (size_t j = 0; j < C; ++j)
      {
        assert(a[i][j] == '0' || a[i][j] == '1');
        (*this)[i][j] = a[i][j] == '1';
      }
    }
  }
  template <class I = ll> static constexpr pair<I, I> shape() { return {I(N), I(C)}; }
  static M e0() { return M(); }
  static M e1() { return M(1); }
  auto &row(int i) { return (*this)[i]; }
  const auto &row(int i) const { return (*this)[i]; }
  auto operator()(int i, int j) { return (*this)[i][j]; }
  bool operator()(int i, int j) const { return (*this)[i][j]; }
  auto at(int i, int j)
  {
    assert(0 <= i && i < n && 0 <= j && j < m);
    return (*this)[i][j];
  }
  bool at(int i, int j) const
  {
    assert(0 <= i && i < n && 0 <= j && j < m);
    return (*this)[i][j];
  }
  bool test(int i, int j) const { return at(i, j); }
  M &set(int i, int j, bool value = true)
  {
    at(i, j) = value;
    return *this;
  }
  M &flip(int i, int j)
  {
    at(i, j).flip();
    return *this;
  }
  template <class T = ll> vvc<T> to_vvi() const
  {
    vvc<T> a(N, vc<T>(C));
    repi(i, n) repi(j, m) a[i][j] = (*this)[i][j];
    return a;
  }
  vc<string> to_vstr() const
  {
    vc<string> a(N, string(C, '0'));
    repi(i, n) repi(j, m) if ((*this)[i][j]) a[i][j] = '1';
    return a;
  }
  M operator-() const { return *this; }
  M &operator+=(const M &b)
  {
    repi(i, n)(*this)[i] ^= b[i];
    return *this;
  }
  M &operator-=(const M &b) { return *this += b; }
  M operator+(const M &b) const { return M(*this) += b; }
  M operator-(const M &b) const { return M(*this) -= b; }
  Vn operator*(const Vm &v) const
  {
    Vn result;
    repi(i, n) result[i] = (((*this)[i] & v).count() & 1);
    return result;
  }
  friend Vm operator*(const Vn &v, const M &a)
  {
    Vm result;
    for (int w = 0; w < matrix_mod2_detail::word_count(n); ++w)
    {
      auto bits = matrix_mod2_detail::read_word(v, w, N);
      while (bits)
      {
        int i = w * 64 + matrix_mod2_detail::low_bit(bits);
        bits &= bits - 1;
        result ^= a[i];
      }
    }
    return result;
  }
  template <size_t K> MatrixMod2<N, K> operator*(const MatrixMod2<C, K> &b) const
  {
    MatrixMod2<N, K> result;
    repi(i, n) for (int w = 0; w < matrix_mod2_detail::word_count(m); ++w)
    {
      auto bits = matrix_mod2_detail::read_word((*this)[i], w, C);
      while (bits)
      {
        int k = w * 64 + matrix_mod2_detail::low_bit(bits);
        bits &= bits - 1;
        result[i] ^= b[k];
      }
    }
    return result;
  }
  M &operator*=(const MatrixMod2<C, C> &b) { return *this = *this * b; }
  template <class T> M pow(T k) const
  {
    static_assert(N == C);
    assert(k >= 0);
    M result(1), a(*this);
    while (k > 0)
    {
      if (k & 1)
        result *= a;
      k >>= 1;
      if (k > 0)
        a *= a;
    }
    return result;
  }
  M row_reduction(bool rref = false) const
  {
    M a(*this);
    for (int i = 0, j = 0; i < n && j < m; ++j)
    {
      int pivot = i;
      while (pivot < n && !a[pivot][j])
        ++pivot;
      if (pivot == n)
        continue;
      swap(a[i], a[pivot]);
      for (int k = rref ? 0 : i + 1; k < n; ++k)
        if (k != i && a[k][j])
          a[k] ^= a[i];
      ++i;
    }
    return a;
  }
  template <class I = ll> I rank() const
  {
    if constexpr (matrix_mod2_detail::native_bitset_words<C> || C <= 64)
    {
      auto get = [&](int i, int w)
      {
        if constexpr (C <= 64)
          return matrix_mod2_detail::word((*this)[i].to_ullong());
        else
          return matrix_mod2_detail::read_word((*this)[i], w, C);
      };
      return I(matrix_mod2_detail::rank(n, m, get));
    }
    else
    {
      M a = row_reduction();
      I rank = 0;
      for (const auto &r : a)
        rank += r.any();
      return rank;
    }
  }
  template <class I = ll> I det() const
  {
    static_assert(N == C);
    if constexpr (N == 0)
      return I(1);
    if constexpr (matrix_mod2_detail::native_bitset_words<C> || C <= 64)
    {
      auto get = [&](int i, int w)
      {
        if constexpr (C <= 64)
          return matrix_mod2_detail::word((*this)[i].to_ullong());
        else
          return matrix_mod2_detail::read_word((*this)[i], w, C);
      };
      if (n <= 64)
        return I(matrix_mod2_detail::rank_narrow(n, m, get) == n);
      matrix_mod2_detail::elimination work(n, m, false, get);
      work.forward(true);
      return I(int(work.pivots.size()) == n);
    }
    else
      return I(rank() == n);
  }
  optional<M> inv() const
  {
    static_assert(N == C);
    if constexpr (matrix_mod2_detail::native_bitset_words<C> || C <= 64)
    {
      auto get = [&](int i, int w)
      {
        if constexpr (C <= 64)
          return matrix_mod2_detail::word((*this)[i].to_ullong());
        else
          return matrix_mod2_detail::read_word((*this)[i], w, C);
      };
      auto result = matrix_mod2_detail::inverse(n, get);
      if (!result)
        return nullopt;
      M a;
      if constexpr (N > 0)
        repi(i, n) matrix_mod2_detail::write_words(
            a[i], result->data() + size_t(i) * matrix_mod2_detail::word_count(n), n);
      return a;
    }
    else
    {
      MatrixMod2<N, 2 * C> a;
      repi(i, n)
      {
        repi(j, m) a[i][j] = (*this)[i][j];
        a[i][m + i] = 1;
      }
      auto b = a.row_reduction(true);
      repi(i, n) if (!b[i][i]) return nullopt;
      M result;
      repi(i, n) repi(j, m) result[i][j] = b[i][m + j];
      return result;
    }
  }
  optional<pair<Vm, vc<Vm>>> solve(const Vn &b) const
  {
    MatrixMod2<N, C + 1> a;
    repi(i, n)
    {
      repi(j, m) a[i][j] = (*this)[i][j];
      a[i][m] = b[i];
    }
    a = a.row_reduction(true);
    Vm sol;
    vc<Vm> basis;
    vc<int> pivot(m, -1);
    for (int i = 0, j = 0; i < n && a[i].any(); ++i)
    {
      while (j < m && !a[i][j])
        ++j;
      if (j == m)
        return nullopt;
      sol[j] = a[i][m];
      pivot[j] = i;
    }
    repi(j, m) if (pivot[j] < 0)
    {
      Vm v;
      v[j] = 1;
      repi(k, j) if (pivot[k] >= 0 && a[pivot[k]][j]) v[k] = 1;
      basis.push_back(v);
    }
    return pair{move(sol), move(basis)};
  }
};
