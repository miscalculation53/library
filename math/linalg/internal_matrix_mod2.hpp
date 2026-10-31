#pragma once

#include "../../template/template_all_but_modint.hpp"

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

