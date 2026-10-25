#pragma once

#include "../../template/template_all_but_modint.hpp"

/*! SPDX-License-Identifier: CC0-1.0
 * Independently designed from GF(2) row operations and the stated API.
 */

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

namespace matrix_mod2_detail {
using word = std::uint64_t;
inline int low_bit(word x) { return __builtin_ctzll(x); }
constexpr int word_count(int columns) { return columns / 64 + (columns % 64 != 0); }
inline word tail_mask(int m) { return m % 64 ? (word(1) << (m % 64)) - 1 : ~word(0); }

template<class GetWord>
int rank_narrow(int n, int m, GetWord get) {
    std::array<word, 64> basis;
    std::fill(basis.begin(), basis.begin() + m, word(0));
    const word mask = tail_mask(m);
    int rank = 0;
    for (int i = 0; i < n; ++i) {
        word x = get(i, 0) & mask;
        while (x) {
            const int p = low_bit(x);
            if (basis[p]) x ^= basis[p];
            else {
                basis[p] = x;
                if (++rank == m) return rank;
                break;
            }
        }
    }
    return rank;
}

template<class GetWord>
int rank_few_rows(int n, int m, GetWord get) {
    std::array<word, 64> combination, original, value;
    for (int i = 0; i < n; ++i) combination[i] = word(1) << i;
    const int width = word_count(m);
    int rank = 0;
    for (int w = 0; w < width; ++w) {
        for (int i = 0; i < n; ++i) original[i] = get(i, w);
        if (w + 1 == width) for (int i = 0; i < n; ++i) original[i] &= tail_mask(m);
        for (int i = rank; i < n; ++i) {
            word x = combination[i], y = 0;
            while (x) { y ^= original[low_bit(x)]; x &= x - 1; }
            value[i] = y;
        }
        while (rank < n) {
            int pivot = rank;
            while (pivot < n && !value[pivot]) ++pivot;
            if (pivot == n) break;
            std::swap(value[rank], value[pivot]);
            std::swap(combination[rank], combination[pivot]);
            const word bit = value[rank] & -value[rank];
            for (int i = rank + 1; i < n; ++i) if (value[i] & bit) {
                value[i] ^= value[rank];
                combination[i] ^= combination[rank];
            }
            if (++rank == n) return rank;
        }
    }
    return rank;
}

template<class GetWord>
int rank_tall(int n, int m, GetWord get) {
    const int width = word_count(m);
    std::vector<word> basis(std::size_t(m) * width), current(width);
    std::vector<unsigned char> occupied(m);
    int rank = 0;
    for (int i = 0; i < n; ++i) {
        for (int w = 0; w < width; ++w) current[w] = get(i, w);
        current.back() &= tail_mask(m);
        for (int w = 0; w < width;) {
            if (!current[w]) { ++w; continue; }
            const int pivot = w * 64 + low_bit(current[w]);
            word* b = basis.data() + std::size_t(pivot) * width;
            if (occupied[pivot]) {
                for (int j = w; j < width; ++j) current[j] ^= b[j];
            } else {
                occupied[pivot] = 1;
                std::copy(current.begin() + w, current.end(), b + w);
                if (++rank == m) return rank;
                break;
            }
        }
    }
    return rank;
}

struct elimination {
    int n, columns, left_width, row_width;
    std::vector<word> storage;
    std::vector<word*> rows;
    std::vector<int> pivots;

    template<class GetWord>
    elimination(int n_, int m, bool inverse, GetWord get)
        : n(n_), columns(m), left_width(word_count(m)), row_width(left_width * (inverse ? 2 : 1)),
          storage(std::size_t(n) * row_width), rows(n) {
        pivots.reserve(std::min(n, m));
        for (int i = 0; i < n; ++i) {
            rows[i] = storage.data() + std::size_t(i) * row_width;
            for (int w = 0; w < left_width; ++w) rows[i][w] = get(i, w);
            rows[i][left_width - 1] &= tail_mask(m);
            if (inverse) rows[i][left_width + i / 64] = word(1) << (i % 64);
        }
    }

    void forward(bool require_full = false) {
        constexpr int block = 8;
        std::vector<unsigned char> pattern(n);
        std::vector<word> table;
        int rank = 0;
        for (int w = 0; w < left_width && rank < n; ++w) {
            while (rank < n) {
                std::fill(pattern.begin() + rank, pattern.end(), 0);
                int count = 0;
                for (; count < block && rank + count < n; ++count) {
                    const int r = rank + count;
                    int pivot = r;
                    while (pivot < n && !rows[pivot][w]) ++pivot;
                    if (pivot == n) break;
                    std::swap(rows[r], rows[pivot]);
                    std::swap(pattern[r], pattern[pivot]);
                    const word bit = rows[r][w] & -rows[r][w];
                    pivots.push_back(w * 64 + low_bit(bit));
                    for (int i = r + 1; i < n; ++i) if (rows[i][w] & bit) {
                        rows[i][w] ^= rows[r][w];
                        pattern[i] ^= 1u << count;
                    }
                }
                if (!count) break;
                const int start = w + 1, length = row_width - start;
                if (length) {
                    for (int j = 0; j < count; ++j) {
                        unsigned p = pattern[rank + j];
                        while (p) {
                            const int b = __builtin_ctz(p); p &= p - 1;
                            for (int k = start; k < row_width; ++k) rows[rank + j][k] ^= rows[rank + b][k];
                        }
                    }
                    int active = 0;
                    for (int i = rank + count; i < n; ++i) active += pattern[i] != 0;
                    if (active > (1 << count)) {
                        table.resize(std::size_t(1 << count) * length);
                        std::fill(table.begin(), table.begin() + length, 0);
                        for (unsigned p = 1; p < (1u << count); ++p) {
                            const unsigned previous = p & (p - 1);
                            const int b = __builtin_ctz(p);
                            for (int k = 0; k < length; ++k)
                                table[std::size_t(p) * length + k] = table[std::size_t(previous) * length + k] ^ rows[rank + b][start + k];
                        }
                        for (int i = rank + count; i < n; ++i) if (pattern[i]) {
                            const word* add = table.data() + std::size_t(pattern[i]) * length;
                            for (int k = 0; k < length; ++k) rows[i][start + k] ^= add[k];
                        }
                    } else {
                        for (int i = rank + count; i < n; ++i) {
                            unsigned p = pattern[i];
                            while (p) {
                                const int b = __builtin_ctz(p); p &= p - 1;
                                for (int k = start; k < row_width; ++k) rows[i][k] ^= rows[rank + b][k];
                            }
                        }
                    }
                }
                rank += count;
            }
            if (require_full && std::size_t(rank) < std::min(std::size_t(columns), (std::size_t(w) + 1) * 64)) return;
        }
    }

    std::vector<word> inverse_result() {
        constexpr int block = 8;
        std::vector<word> table;
        for (int end = n; end > 0;) {
            const int begin = std::max(0, end - block), count = end - begin;
            for (int i = end - 1; i >= begin; --i)
                for (int j = i + 1; j < end; ++j)
                    if ((rows[i][pivots[j] / 64] >> (pivots[j] % 64)) & 1)
                        for (int w = 0; w < left_width; ++w) rows[i][left_width + w] ^= rows[j][left_width + w];
            if (begin > (1 << count)) {
                table.resize(std::size_t(1 << count) * left_width);
                std::fill(table.begin(), table.begin() + left_width, 0);
                for (unsigned p = 1; p < (1u << count); ++p) {
                    const unsigned previous = p & (p - 1);
                    const int b = __builtin_ctz(p);
                    for (int w = 0; w < left_width; ++w)
                        table[std::size_t(p) * left_width + w] = table[std::size_t(previous) * left_width + w] ^ rows[begin + b][left_width + w];
                }
                for (int i = 0; i < begin; ++i) {
                    unsigned p = 0;
                    for (int j = 0; j < count; ++j)
                        p |= unsigned((rows[i][pivots[begin + j] / 64] >> (pivots[begin + j] % 64)) & 1) << j;
                    if (p) for (int w = 0; w < left_width; ++w) rows[i][left_width + w] ^= table[std::size_t(p) * left_width + w];
                }
            } else {
                for (int i = 0; i < begin; ++i)
                    for (int j = begin; j < end; ++j)
                        if ((rows[i][pivots[j] / 64] >> (pivots[j] % 64)) & 1)
                            for (int w = 0; w < left_width; ++w) rows[i][left_width + w] ^= rows[j][left_width + w];
            }
            end = begin;
        }
        std::vector<word> result(std::size_t(n) * left_width);
        for (int i = 0; i < n; ++i)
            std::copy(rows[i] + left_width, rows[i] + row_width, result.data() + std::size_t(pivots[i]) * left_width);
        return result;
    }
};

template<class GetWord>
int rank(int n, int m, GetWord get) {
    if (!n || !m) return 0;
    if (m == 1) {
        for (int i = 0; i < n; ++i) if (get(i, 0) & 1) return 1;
        return 0;
    }
    if (n == 1) {
        const int width = word_count(m);
        for (int w = 0; w < width; ++w)
            if (get(0, w) & (w + 1 == width ? tail_mask(m) : ~word(0))) return 1;
        return 0;
    }
    if (m <= 64) return rank_narrow(n, m, get);
    if (n <= 64) return rank_few_rows(n, m, get);
    if (std::int64_t(n) > 4 * std::int64_t(m)) return rank_tall(n, m, get);
    elimination work(n, m, false, get);
    work.forward();
    return int(work.pivots.size());
}

template<class GetWord>
std::optional<std::vector<word>> inverse(int n, GetWord get) {
    if (!n) return std::vector<word>();
    elimination work(n, n, true, get);
    work.forward(true);
    if (int(work.pivots.size()) != n) return std::nullopt;
    return work.inverse_result();
}
}

#include <climits>
#include <cstring>
#include <limits>
#include <type_traits>

namespace matrix_mod2_detail {
// The fast path assumes the bit layout validated in the tested GNU 15 build:
// contiguous little-endian words; bit j is word j/64, bit j%64.
// Size and trivial-copyability checks supplement that validation; they do not prove layout.
template<std::size_t Capacity>
constexpr bool native_bitset_words =
#if defined(__GLIBCXX__) && defined(_GLIBCXX_RELEASE) && _GLIBCXX_RELEASE == 15 && \
    CHAR_BIT == 8 && ULONG_MAX == UINT64_MAX && defined(__BYTE_ORDER__) && \
    defined(__ORDER_LITTLE_ENDIAN__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    Capacity != 0 && std::is_trivially_copyable<std::bitset<Capacity>>::value &&
    sizeof(std::bitset<Capacity>) == (Capacity / 64 + (Capacity % 64 != 0)) * sizeof(word);
#else
    false;
#endif

template<std::size_t Capacity>
word read_word(const std::bitset<Capacity>& row, int index, std::size_t columns = Capacity) {
    if constexpr (native_bitset_words<Capacity>) {
        word value;
        std::memcpy(&value, reinterpret_cast<const unsigned char*>(&row) + std::size_t(index) * sizeof(word), sizeof(word));
        return value;
    } else {
        word value = 0;
        const std::size_t begin = std::size_t(index) * 64;
        const unsigned count = unsigned(std::min(std::size_t(64), std::size_t(columns) - begin));
        for (unsigned bit = 0; bit < count; ++bit) value |= word(row[begin + bit]) << bit;
        return value;
    }
}

template<std::size_t Capacity>
void write_words(std::bitset<Capacity>& row, const word* source, int columns) {
    const int width = word_count(columns);
    if constexpr (native_bitset_words<Capacity>) {
        if (width) std::memcpy(reinterpret_cast<unsigned char*>(&row), source, std::size_t(width) * sizeof(word));
    } else {
        for (int bit = 0; bit < columns; ++bit) row[bit] = (source[bit / 64] >> (bit % 64)) & 1;
    }
}
}


/**
 * @brief 行列（$\mathbb{F}_2$）
 * @docs docs/math/linalg/matrix_mod2.md
 */

template <size_t MAX_M>
struct MatrixMod2 : vc<bitset<MAX_M>>
{
  using M = MatrixMod2;
  using V = bitset<MAX_M>;
  using vc<bitset<MAX_M>>::vector;
  using vc<bitset<MAX_M>>::operator=;
  using vc<bitset<MAX_M>>::size;

  int m;
  MatrixMod2(int n, int m, int diag = 0, int non_diag = 0) : m(m)
  {
    assert(0 <= m && size_t(m) <= MAX_M);
    this->resize(n);
    repi(i, n) repi(j, m)(*this)[i][j] = i == j ? diag : non_diag;
  }
  template <class T>
  MatrixMod2(const vvc<T> &vec)
  {
    const int n = vec.size();
    m = n == 0 ? 0 : vec[0].size();
    assert(0 <= m && size_t(m) <= MAX_M);
    this->resize(n);
    repi(i, n)
    {
      assert(SZ(vec[i]) == m);
      repi(j, m)
      {
        assert(vec[i][j] == T(0) || vec[i][j] == T(1));
        (*this)[i][j] = vec[i][j];
      }
    }
  }
  MatrixMod2(const vc<string> &vec)
  {
    const int n = vec.size();
    m = n == 0 ? 0 : vec[0].size();
    assert(0 <= m && size_t(m) <= MAX_M);
    this->resize(n);
    repi(i, n)
    {
      assert(SZ(vec[i]) == m);
      repi(j, m)
      {
        assert(vec[i][j] == '0' || vec[i][j] == '1');
        (*this)[i][j] = vec[i][j] - '0';
      }
    }
  }
  template <class T = ll>
  vvc<T> to_vvi() const
  {
    const int n = size();
    vvc<T> res(n, vc<T>(m));
    repi(i, n) repi(j, m) res[i][j] = (*this)[i][j];
    return res;
  }
  vc<string> to_vstr() const
  {
    const int n = size();
    vc<string> res(n, string(m, '0'));
    repi(i, n) repi(j, m) res[i][j] += (*this)[i][j];
    return res;
  }

  M operator-() const { return *this; }
  
  // 加算・減算 O(nm/w)
  M &operator+=(const M &b)
  {
    const int n = size();
    repi(i, n)(*this)[i] ^= b[i];
    return *this;
  }
  M &operator-=(const M &b) { return *this += b; }
  M operator+(const M &b) const { return M(*this) += b; }
  M operator-(const M &b) const { return M(*this) -= b; }

  // 乗算 O(nmk/w)
  template <size_t MAX_K>
  MatrixMod2<MAX_K> operator*(const MatrixMod2<MAX_K> &b) const
  {
    const int n = size(), m_ = b.size(), k = b.m;
    assert(m == m_);
    MatrixMod2<MAX_K> res(n, k);
    repi(i, n)
    {
      auto row = (*this)[i];
      for (int j = row._Find_first(); j < m; j = row._Find_next(j))
        res[i] ^= b[j];
    }
    return res;
  }
  template <size_t MAX_K>
  MatrixMod2<MAX_K> operator*=(const MatrixMod2<MAX_K> &b) { return *this = *this * b; }

  // 正方行列
  // O(n^3 log k / w)
  template <class T>
  M pow(T k) const
  {
    const int n = size();
    assert(n == m);
    M res(n, n, 1), tmp(*this);
    while (k > 0)
    {
      if (k & 1)
        res *= tmp;
      tmp *= tmp;
      k >>= 1;
    }
    return res;
  }

  // rref が true の場合、簡約行列にしたものを返す
  // rref が false の場合、階段行列にしたものを返す
  // O(nm min(n, m) / w)
  M row_reduction(bool rref = false) const
  {
    const int n = size();
    M a(*this);
    for (int i = 0, j = 0; i < n && j < m; j++)
    {
      repi(k, i, n)
      {
        if (a[k][j])
        {
          swap(a[i], a[k]);
          break;
        }
      }
      if (a[i][j] == 0)
        continue;
      if (rref) repi(k, i) if (a[k][j]) a[k] ^= a[i];
      repi(k, i + 1, n) if (a[k][j]) a[k] ^= a[i];
      i++;
    }
    return a;
  }
  // C = MAX_M、w はワードのビット数。
  // O(nm + n min(n, m) C / w)
  template <class I = ll>
  I rank() const
  {
    if constexpr (matrix_mod2_detail::native_bitset_words<MAX_M>) {
      assert(0 <= m && size_t(m) <= MAX_M);
      auto get = [&](int row, int column) { return matrix_mod2_detail::read_word((*this)[row], column, m); };
      return I(matrix_mod2_detail::rank(int(size()), m, get));
    } else {
      assert(0 <= m && size_t(m) <= MAX_M);
      const int n = size();
      if (!n || !m) return I(0);
      if (m == 1) {
        for (int i = 0; i < n; ++i) if ((*this)[i][0]) return I(1);
        return I(0);
      }
      if constexpr (MAX_M <= 64) {
        auto get = [&](int row, int) { return matrix_mod2_detail::word((*this)[row].to_ullong()); };
        if (n == 1) return I((get(0, 0) & matrix_mod2_detail::tail_mask(m)) != 0);
        return I(matrix_mod2_detail::rank_narrow(n, m, get));
      }
      if (n == 1) {
        for (int j = 0; j < m; ++j) if ((*this)[0][j]) return I(1);
        return I(0);
      }
      M a(*this);
      int rank = 0;
      for (int j = 0; rank < n && j < m; ++j) {
        int pivot = rank;
        while (pivot < n && !a[pivot][j]) ++pivot;
        if (pivot == n) continue;
        if (pivot != rank) a[rank] ^= a[pivot];
        for (int i = rank + 1; i < n; ++i)
          if (a[i][j]) a[i] ^= a[rank];
        ++rank;
      }
      return I(rank);
    }
  }
  // 正方行列、O(n^2 + n^2 C / w)
  template <class I = ll>
  I det() const
  {
    if constexpr (matrix_mod2_detail::native_bitset_words<MAX_M>) {
      const int n = size();
      assert(n == m);
      assert(0 <= m && size_t(m) <= MAX_M);
      if (!n) return I(1);
      auto get = [&](int row, int column) { return matrix_mod2_detail::read_word((*this)[row], column, m); };
      if (n <= 64) return I(matrix_mod2_detail::rank_narrow(n, n, get) == n);
      matrix_mod2_detail::elimination work(n, n, false, get);
      work.forward(true);
      return I(int(work.pivots.size()) == n);
    } else {
      const int n = size();
      assert(n == m);
      assert(0 <= m && size_t(m) <= MAX_M);
      if (n <= 1) return I(n == 0 || (*this)[0][0]);
      M a = row_reduction();
      int res = 1;
      repi(i, n) res &= a[i][i];
      return I(res);
    }
  }
  // 正方行列、O(n^2 + n^2 C / w)
  // 逆行列。存在しなければ nullopt。
  optional<M> inv() const
  {
    if constexpr (matrix_mod2_detail::native_bitset_words<MAX_M>) {
      const int n = size();
      assert(n == m);
      assert(0 <= m && size_t(m) <= MAX_M);
      auto get = [&](int row, int column) { return matrix_mod2_detail::read_word((*this)[row], column, m); };
      auto result = matrix_mod2_detail::inverse(n, get);
      if (!result) return nullopt;
      M res(0, m);
      res.resize(n);
      const int width = matrix_mod2_detail::word_count(n);
      for (int row = 0; row < n; ++row)
        matrix_mod2_detail::write_words(res[row], result->data() + size_t(row) * width, n);
      return res;
    } else {
      const int n = size();
      assert(n == m);
      assert(0 <= m && size_t(m) <= MAX_M);
      if (!n) return M(0, 0);
      if (n == 1) {
        if (!(*this)[0][0]) return nullopt;
        M res(0, 1);
        res.resize(1);
        res[0][0] = 1;
        return res;
      }
      MatrixMod2<MAX_M * size_t(2)> a(n, 2 * n);
      repi(i, n) repi(j, n) a[i][j] = (*this)[i][j];
      repi(i, n) a[i][n + i] = 1;
      auto b = a.row_reduction(true);
      repi(i, n) if (b[i][i] == 0) return nullopt;
      M res(n, n);
      repi(i, n) repi(j, n) res[i][j] = b[i][n + j];
      return res;
    }
  }

  // (解のひとつ, 基底)。解が存在しなければ nullopt。
  // O(nm min(n, m) / w + m^2)
  template <size_t MAX_N>
  optional<pair<V, vc<V>>> solve(const bitset<MAX_N> &b) const
  {
    const int n = size();
    assert(size_t(n) <= MAX_N);
    MatrixMod2<MAX_M + 1> a(n, m + 1);
    repi(i, n) rep(j, m) a[i][j] = (*this)[i][j];
    repi(i, n) a[i][m] = b[i];
    a = a.row_reduction(true);
    int rk = 0;
    repi(i, n) if (a[i] != 0) rk++;

    V sol;
    vc<V> basis;
    vc<int> piv(m, -1);
    for (int i = 0, j = 0; i < rk; i++)
    {
      while (j < m && a[i][j] == 0)
        j++;
      if (j == m)
        return nullopt;
      sol[j] = a[i][m], piv[j] = i;
    }
    repi(j, m)
    {
      if (piv[j] != -1)
        continue;
      V ba;
      ba[j] = 1;
      repi(k, j) if (piv[k] != -1) ba[k] = a[piv[k]][j];
      basis.eb(ba);
    }
    return pair{move(sol), move(basis)};
  }
};
