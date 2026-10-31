#pragma once

#include <array>
#include <cstdint>

// Symmetric products and forward differences.
// Contract: count >= 0; Mint supplies construction, +, -, *, +=, -=, *=.
namespace factorial_tail_product_detail {

template <class Mint>
Mint rising_product_direct(Mint first, int count) {
    if (count == 0) return Mint(1);
    Mint result = first;
    const Mint one(1);
    for (int i = 1; i < count; ++i) {
        first += one;
        result *= first;
    }
    return result;
}

// (a+i)(a+L-1-i) = a(a+L-1) + i(L-1-i).
template <class Mint>
Mint rising_product_pairs(Mint first, int count) {
    if (count < 2) return count == 0 ? Mint(1) : first;
    const int pairs = count / 2;
    const Mint two(2);
    Mint term = first * (first + Mint(count - 1));
    Mint difference(count - 2);
    Mint result = term;
    for (int i = 1; i < pairs; ++i) {
        term += difference;
        difference -= two;
        result *= term;
    }
    if (count & 1) result *= first + Mint(pairs);
    return result;
}

// Split 4m factors into two intervals of length 2m. For each i, multiply
// the symmetric pair from each interval. The resulting quartic has four
// explicit initial forward differences and constant fourth difference 24.
template <class Mint>
Mint rising_product_quartets(Mint first, int count) {
    const int groups = count / 4;
    if (groups == 0) return rising_product_pairs(first, count);
    const std::int64_t h = 2LL * groups - 1;
    const Mint left = first * (first + Mint(h));
    const Mint right =
        (first + Mint(2LL * groups)) * (first + Mint(4LL * groups - 1));
    const Mint sum = left + right;
    Mint d0 = left * right;
    Mint d1 = Mint((h - 1) * (h - 1)) + sum * Mint(h - 1);
    Mint d2 = Mint(2 * h * h - 12 * h + 14) - sum - sum;
    Mint d3(36 - 12 * h);
    const Mint d4(24);
    Mint result = d0;
    for (int i = 1; i < groups; ++i) {
        d0 += d1;
        d1 += d2;
        d2 += d3;
        d3 += d4;
        result *= d0;
    }
    Mint tail = first + Mint(4LL * groups);
    const Mint one(1);
    for (int i = 4 * groups; i < count; ++i) {
        result *= tail;
        tail += one;
    }
    return result;
}

// Degree is 8 or 16. Partition Degree*m factors into Degree/2 intervals
// of length 2m. A symmetric pair in interval j equals A_j+t_i, where
// A_j=(a+2mj)(a+2mj+2m-1), t_i=i(2m-1-i).
// The product over j is a monic polynomial of degree Degree in i.
template <int Degree, class Mint>
Mint rising_product_differences(Mint first, int count) {
    static_assert(Degree == 8 || Degree == 16);
    constexpr int pair_count = Degree / 2;
    const int groups = count / Degree;
    if (groups == 0) return rising_product_quartets(first, count);

    std::array<Mint, pair_count> anchors;
    const Mint width(2LL * groups);
    Mint anchor = first * (first + Mint(2LL * groups - 1));
    Mint jump = width * (first + first + Mint(4LL * groups - 1));
    const Mint jump_difference(8LL * groups * groups);
    for (int j = 0; j < pair_count; ++j) {
        anchors[j] = anchor;
        anchor += jump;
        jump += jump_difference;
    }

    std::array<Mint, Degree + 1> difference;
    Mint offset(0);
    Mint offset_difference(2LL * groups - 2);
    const Mint two(2);
    for (int i = 0; i < Degree; ++i) {
        Mint value = anchors[0] + offset;
        for (int j = 1; j < pair_count; ++j) {
            value *= anchors[j] + offset;
        }
        difference[i] = value;
        offset += offset_difference;
        offset_difference -= two;
    }
    for (int order = 1; order < Degree; ++order) {
        for (int i = Degree - 1; i >= order; --i) {
            difference[i] -= difference[i - 1];
        }
    }
    std::int64_t top_difference = 1;
    for (int i = 2; i <= Degree; ++i) top_difference *= i;
    difference[Degree] = Mint(top_difference);

    Mint result = difference[0];
    for (int i = 1; i < groups; ++i) {
        for (int order = 0; order < Degree; ++order) {
            difference[order] += difference[order + 1];
        }
        result *= difference[0];
    }
    const int core_count = groups * Degree;
    Mint tail = first + Mint(core_count);
    const Mint one(1);
    for (int i = core_count; i < count; ++i) {
        result *= tail;
        tail += one;
    }
    return result;
}

// An unmeasured policy included only as a complete source-level candidate.
// Compare individual kernels before selecting production thresholds.
template <class Mint>
Mint rising_product_candidate(Mint first, int count) {
    if (count < 32) return rising_product_pairs(first, count);
    if (count < 384) return rising_product_quartets(first, count);
    if (count < 2048) return rising_product_differences<8>(first, count);
    return rising_product_differences<16>(first, count);
}

}  // namespace factorial_tail_product_detail
