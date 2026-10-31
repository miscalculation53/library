// Packed exact arithmetic with an owned in-place elementary workspace.
#ifndef INDEPENDENT_BIGINT_DIVISION_V2_20261024_HPP
#define INDEPENDENT_BIGINT_DIVISION_V2_20261024_HPP

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <utility>
#include <vector>

namespace independent_bigint_division_v2_20261024 {

using magnitude = std::vector<long long>;

template <int B>
constexpr std::size_t native_digit_capacity() {
    static_assert(B >= 2, "The radix must be at least two.");
    using wide = std::uint64_t;
    constexpr wide limit = std::numeric_limits<wide>::max();
    constexpr wide maximum_digit = static_cast<wide>(B) - 1;
    constexpr wide threshold = (limit - maximum_digit) / B;
    wide largest = 0;
    std::size_t count = 0;
    while (largest <= threshold) {
        largest = largest * B + maximum_digit;
        ++count;
    }
    return count;
}

template <int B, class Multiply>
class magnitude_divider {
    static_assert(B >= 2, "The radix must be at least two.");
    using wide = std::uint64_t;
    static constexpr std::size_t elementary_cutoff = 32;
    Multiply& multiply_;

    static void trim(magnitude& a) {
        while (!a.empty() && a.back() == 0) a.pop_back();
    }

    static int compare(const magnitude& a, const magnitude& b) {
        if (a.size() != b.size()) return a.size() < b.size() ? -1 : 1;
        for (std::size_t i = a.size(); i > 0; --i) {
            if (a[i - 1] != b[i - 1]) return a[i - 1] < b[i - 1] ? -1 : 1;
        }
        return 0;
    }

    static magnitude radix_power(std::size_t exponent) {
        magnitude result(exponent + 1, 0);
        result.back() = 1;
        return result;
    }

    static magnitude shift(const magnitude& a, std::size_t places) {
        if (a.empty()) return {};
        magnitude result(places, 0);
        result.insert(result.end(), a.begin(), a.end());
        return result;
    }

    static magnitude high_part(const magnitude& a, std::size_t drop) {
        if (a.size() <= drop) return {};
        return magnitude(a.begin() + drop, a.end());
    }

    static void increment(magnitude& a) {
        for (std::size_t i = 0; i < a.size(); ++i) {
            if (a[i] != B - 1) {
                ++a[i];
                return;
            }
            a[i] = 0;
        }
        a.push_back(1);
    }

    static void decrement(magnitude& a) {
        assert(!a.empty());
        for (std::size_t i = 0; i < a.size(); ++i) {
            if (a[i] != 0) {
                --a[i];
                trim(a);
                return;
            }
            a[i] = B - 1;
        }
        assert(false);
    }

    static void add(magnitude& a, const magnitude& b) {
        if (a.size() < b.size()) a.resize(b.size(), 0);
        wide carry = 0;
        std::size_t i = 0;
        for (; i < b.size(); ++i) {
            wide value = static_cast<wide>(a[i]) + b[i] + carry;
            carry = value >= static_cast<wide>(B);
            if (carry) value -= B;
            a[i] = static_cast<long long>(value);
        }
        while (carry && i < a.size()) {
            if (a[i] == B - 1) {
                a[i] = 0;
                ++i;
            } else {
                ++a[i];
                carry = 0;
            }
        }
        if (carry) a.push_back(1);
    }

    // 前提は a >= b。借りの計算は [-B, B-1] に収まる。
    static void subtract(magnitude& a, const magnitude& b) {
        assert(compare(a, b) >= 0);
        long long borrow = 0;
        for (std::size_t i = 0; i < b.size() || borrow; ++i) {
            long long value = a[i] - (i < b.size() ? b[i] : 0) - borrow;
            borrow = value < 0;
            if (borrow) value += B;
            a[i] = value;
        }
        trim(a);
    }

    static magnitude multiply_small(const magnitude& a, wide factor) {
        assert(factor < static_cast<wide>(B));
        if (a.empty() || factor == 0) return {};
        magnitude result;
        result.reserve(a.size() + 1);
        wide carry = 0;
        for (long long digit : a) {
            const wide value = static_cast<wide>(digit) * factor + carry;
            result.push_back(static_cast<long long>(value % B));
            carry = value / B;
        }
        if (carry) result.push_back(static_cast<long long>(carry));
        return result;
    }

    static std::pair<magnitude, wide> divide_small(const magnitude& a, wide divisor) {
        assert(divisor > 0 && divisor < static_cast<wide>(B));
        magnitude q(a.size(), 0);
        wide remainder = 0;
        for (std::size_t i = a.size(); i > 0; --i) {
            const wide value = remainder * B + static_cast<wide>(a[i - 1]);
            q[i - 1] = static_cast<long long>(value / divisor);
            remainder = value % divisor;
        }
        trim(q);
        return {std::move(q), remainder};
    }

    // 呼出側は入力の桁数が native_digit_capacity<B>() 以下であることを保つ。
    static wide pack_native(const magnitude& a) {
        wide value = 0;
        for (std::size_t i = a.size(); i > 0; --i)
            value = value * B + static_cast<wide>(a[i - 1]);
        return value;
    }

    static magnitude unpack_native(wide value) {
        if (value == 0) return {};
        if (value < static_cast<wide>(B))
            return {static_cast<long long>(value)};
        std::array<long long, native_digit_capacity<B>()> temporary;
        std::size_t used = 0;
        while (value) {
            temporary[used++] = static_cast<long long>(value % B);
            value /= B;
        }
        return magnitude(temporary.begin(), temporary.begin() + used);
    }

    static std::pair<magnitude, magnitude> divide_native_values(
        wide x, wide y, const magnitude& a) {
        if (x < y) return {{}, a};
        if (x == y) return {{1}, {}};
        const wide q = x / y;
        const wide r = x - q * y;
        return {unpack_native(q), unpack_native(r)};
    }

    static std::pair<magnitude, magnitude> divide_native(
        const magnitude& a, const magnitude& b) {
        return divide_native_values(pack_native(a), pack_native(b), a);
    }

    magnitude product(const magnitude& a, const magnitude& b) {
        if (a.empty() || b.empty()) return {};
        if (a.size() == 1) return multiply_small(b, static_cast<wide>(a[0]));
        if (b.size() == 1) return multiply_small(a, static_cast<wide>(b[0]));
        return multiply_(a, b);
    }

    // b の最上位桁 >= floor(B/2)。u は値で所有する作業配列。
    // 各 window の過大見積りは高々 3。積の減算と addback を直接行う。
    static std::pair<magnitude, magnitude> elementary_divide(
        magnitude u, const magnitude& b) {
        assert(!b.empty() && b.back() >= B / 2);
        if (u.size() <= native_digit_capacity<B>() &&
            b.size() <= native_digit_capacity<B>())
            return divide_native(u, b);
        if (b.size() == 1) {
            auto result = divide_small(u, static_cast<wide>(b[0]));
            magnitude r;
            if (result.second) r.push_back(static_cast<long long>(result.second));
            return {std::move(result.first), std::move(r)};
        }
        const std::size_t n = b.size();
        if (u.size() < n) return {{}, std::move(u)};
        const std::size_t positions = u.size() - n + 1;
        magnitude q(positions, 0);
        u.reserve(u.size() + 1);
        u.push_back(0);
        for (std::size_t pos = positions; pos > 0; --pos) {
            const std::size_t offset = pos - 1;
            const wide upper = static_cast<wide>(u[offset + n]);
            const wide next = static_cast<wide>(u[offset + n - 1]);
            wide estimate = std::min<wide>(B - 1, (upper * B + next) / b.back());
            if (estimate == 0) {
                assert(u[offset + n] == 0);
                continue;
            }

            wide product_carry = 0;
            long long borrow = 0;
            for (std::size_t i = 0; i < n; ++i) {
                const wide product = static_cast<wide>(b[i]) * estimate + product_carry;
                product_carry = product / B;
                long long value = u[offset + i] - static_cast<long long>(product % B) - borrow;
                borrow = value < 0;
                if (borrow) value += B;
                u[offset + i] = value;
            }
            long long top = u[offset + n] - static_cast<long long>(product_carry) - borrow;
            bool negative = top < 0;
            if (negative) top += B;
            u[offset + n] = top;

            while (negative) {
                --estimate;
                wide carry = 0;
                for (std::size_t i = 0; i < n; ++i) {
                    wide value = static_cast<wide>(u[offset + i]) + b[i] + carry;
                    carry = value >= static_cast<wide>(B);
                    if (carry) value -= B;
                    u[offset + i] = static_cast<long long>(value);
                }
                wide value = static_cast<wide>(u[offset + n]) + carry;
                negative = value < static_cast<wide>(B);
                if (!negative) value -= B;
                u[offset + n] = static_cast<long long>(value);
            }
            assert(u[offset + n] == 0);
            q[offset] = static_cast<long long>(estimate);
        }
        magnitude r(u.begin(), u.begin() + n);
        trim(q);
        trim(r);
        return {std::move(q), std::move(r)};
    }

    // b は n 桁で最上位桁 >= floor(B/2)。floor(B^(2n)/b) を返す。
    magnitude reciprocal(const magnitude& b) {
        const std::size_t n = b.size();
        assert(n > 0 && b.back() >= B / 2);
        if (n <= elementary_cutoff) {
            return elementary_divide(radix_power(2 * n), b).first;
        }

        const std::size_t k = (n + 1) / 2 + 3;
        const std::size_t f = n - k;
        magnitude upper = high_part(b, f);
        if (std::any_of(b.begin(), b.begin() + f,
                        [](long long digit) { return digit != 0; })) {
            increment(upper);
        }
        magnitude y;
        if (upper.size() > k) {
            // ceil(b/B^f) = B^k の境界では逆数がちょうど B^k。
            y = radix_power(k);
        } else {
            y = reciprocal(upper);
        }

        // x0 = y*B^f。T >= 0 とし、床を保った Newton 更新を行う。
        magnitude error = radix_power(n + k);
        subtract(error, product(b, y));
        magnitude delta = high_part(product(y, error), 2 * k);
        magnitude x = shift(y, f);
        add(x, delta);

        // B^(2n)-b*x = T*B^f-b*delta。上式の打消しを先に使う。
        magnitude remainder = shift(error, f);
        subtract(remainder, product(b, delta));
        if (compare(remainder, b) >= 0) {
            subtract(remainder, b);
            increment(x);
        }
        assert(compare(remainder, b) < 0);
        return x;
    }

    static bool is_radix_power(const magnitude& b) {
        return b.back() == 1 &&
               std::all_of(b.begin(), b.end() - 1,
                           [](long long digit) { return digit == 0; });
    }

public:
    explicit magnitude_divider(Multiply& multiply) : multiply_(multiply) {}

    std::pair<magnitude, magnitude> divide(const magnitude& a, const magnitude& b) {
        assert(!b.empty());
        if (a.size() == 1 && b.size() == 1)
            return divide_native_values(static_cast<wide>(a[0]), static_cast<wide>(b[0]), a);
        const int order = compare(a, b);
        if (order < 0) return {{}, a};
        if (order == 0) return {{1}, {}};
        if (a.size() <= native_digit_capacity<B>())
            return divide_native(a, b);
        if (b.size() == 1) {
            auto result = divide_small(a, static_cast<wide>(b[0]));
            magnitude r;
            if (result.second) r.push_back(static_cast<long long>(result.second));
            return {std::move(result.first), std::move(r)};
        }
        if (is_radix_power(b)) {
            const std::size_t split = b.size() - 1;
            magnitude q(a.begin() + split, a.end());
            magnitude r(a.begin(), a.begin() + split);
            trim(r);
            return {std::move(q), std::move(r)};
        }

        const std::size_t quotient_digits = a.size() - b.size() + 1;
        if (quotient_digits + 1 < b.size()) {
            // 同数の下位桁を捨てた商は真の商以上、高々 1 過大。
            const std::size_t drop = b.size() - (quotient_digits + 1);
            magnitude upper_a = high_part(a, drop);
            magnitude upper_b = high_part(b, drop);
            magnitude q = divide(upper_a, upper_b).first;
            magnitude trial = product(q, b);
            if (compare(trial, a) > 0) {
                decrement(q);
                subtract(trial, b);
            }
            magnitude r = a;
            subtract(r, trial);
            return {std::move(q), std::move(r)};
        }

        // d*b は元と同じ桁数で、最上位桁が floor(B/2) 以上になる。
        const wide factor = static_cast<wide>(B) /
                            (static_cast<wide>(b.back()) + 1);
        magnitude scaled_a, scaled_b;
        const magnitude* aa = &a;
        const magnitude* bb = &b;
        if (factor != 1) {
            scaled_a = multiply_small(a, factor);
            scaled_b = multiply_small(b, factor);
            aa = &scaled_a;
            bb = &scaled_b;
        }

        std::pair<magnitude, magnitude> result;
        const std::size_t n = bb->size();
        if (n <= elementary_cutoff) {
            if (factor == 1) {
                magnitude workspace;
                workspace.reserve(a.size() + 1);
                workspace.insert(workspace.end(), a.begin(), a.end());
                result = elementary_divide(std::move(workspace), b);
            } else {
                result = elementary_divide(std::move(scaled_a), scaled_b);
            }
        } else {
            magnitude inverse = reciprocal(*bb);
            const std::size_t m = aa->size();
            magnitude q(m - n + 1, 0), r;
            for (std::size_t pos = m; pos > 0;) {
                const std::size_t count = std::min(n, pos);
                const std::size_t begin = pos - count;
                magnitude current(aa->begin() + begin, aa->begin() + pos);
                current.insert(current.end(), r.begin(), r.end());
                trim(current);
                if (compare(current, *bb) < 0) {
                    r = std::move(current);
                    pos = begin;
                    continue;
                }
                magnitude part = high_part(product(current, inverse), 2 * n);
                magnitude trial = product(part, *bb);
                r = std::move(current);
                subtract(r, trial);
                if (compare(r, *bb) >= 0) {
                    subtract(r, *bb);
                    increment(part);
                }
                assert(part.size() <= count && begin + part.size() <= q.size());
                for (std::size_t i = 0; i < part.size(); ++i) q[begin + i] = part[i];
                pos = begin;
            }
            trim(q);
            result = {std::move(q), std::move(r)};
        }
        if (factor != 1) {
            auto unscaled = divide_small(result.second, factor);
            assert(unscaled.second == 0);
            result.second = std::move(unscaled.first);
        }
        return result;
    }
};

template <int B, class Multiply>
std::pair<magnitude, magnitude> divide_magnitude(
    const magnitude& a, const magnitude& b, Multiply multiply) {
    magnitude_divider<B, Multiply> divider(multiply);
    return divider.divide(a, b);
}

}  // namespace independent_bigint_division_v2_20261024

#endif
