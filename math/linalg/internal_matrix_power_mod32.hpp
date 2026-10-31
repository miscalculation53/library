#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <type_traits>
#include <utility>
#include <vector>

#include "../dot_product.hpp"

// Matrix similarity, Krylov bases, and the characteristic-polynomial identity.
namespace internal { namespace matrix_power_mod32_detail {

// Other exponent types keep the original public binary-power requirements.
template <class T>
inline constexpr bool supported_builtin_exponent_v =
    std::is_integral_v<T>
#if defined(__SIZEOF_INT128__)
    || std::is_same_v<T, __int128> || std::is_same_v<T, unsigned __int128>
#endif
    ;

template <class S>
using Matrix = std::vector<std::vector<S>>;

template <class S>
Matrix<S> identity(std::size_t n) {
    Matrix<S> r(n, std::vector<S>(n, S(0)));
    for (std::size_t i = 0; i < n; ++i) r[i][i] = S(1);
    return r;
}

template <class S>
void require_square(const Matrix<S>& a) {
    for (const auto& row : a) assert(row.size() == a.size());
}

template <class S>
Matrix<S> multiply(const Matrix<S>& a, const Matrix<S>& b) {
    const std::size_t n = a.size();
    Matrix<S> result(n, std::vector<S>(n, S(0)));
    Matrix<S> transposed(n, std::vector<S>(n, S(0)));
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < n; ++j) transposed[j][i] = b[i][j];
    }
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            result[i][j] = ::dot_product<FieldAddSubMulDiv<S>>(
                static_cast<int>(n), a[i].begin(), transposed[j].begin());
        }
    }
    return result;
}

// Requires only S(0), S(1), addition, multiplication, and copying.
template <class S, class T>
Matrix<S> power_over_ring(const Matrix<S>& a, T exponent) {
    require_square(a);
    if (exponent == 0) return identity<S>(a.size());
    Matrix<S> r = a;
    int top = 0;
    for (T x = exponent; x > 1; x >>= 1) ++top;
    for (int bit = top - 1; bit >= 0; --bit) {
        r = multiply(r, r);
        if ((exponent >> bit) & 1) r = multiply(r, a);
    }
    return r;
}

template <class T>
std::size_t binary_multiplication_count(T exponent) {
    if (exponent == 0) return 0;
    std::size_t bits = 0, ones = 0;
    for (; exponent; exponent >>= 1) {
        ++bits;
        ones += static_cast<int>(exponent & 1);
    }
    return bits + ones - 2;
}

template <class S, class T>
S scalar_power(S a, T exponent) {
    S r(1);
    while (exponent) {
        if (exponent & 1) r = r * a;
        exponent >>= 1;
        if (exponent) a = a * a;
    }
    return r;
}

// A row with one nonzero entry represents a weighted map on the indices.
// Composition gives both the destination and the product of the path weights.
template <class S, class T>
Matrix<S> power_weighted_map(std::vector<std::size_t> next,
                             std::vector<S> weight, T exponent,
                             bool transposed) {
    const std::size_t n = next.size();
    std::vector<std::size_t> destination(n), squared_next(n);
    std::vector<S> value(n, S(1)), squared_weight(n);
    for (std::size_t i = 0; i < n; ++i) destination[i] = i;
    while (exponent) {
        if (exponent & 1) {
            for (std::size_t i = 0; i < n; ++i) {
                const std::size_t at = destination[i];
                value[i] = value[i] * weight[at];
                destination[i] = next[at];
            }
        }
        exponent >>= 1;
        if (!exponent) break;
        for (std::size_t i = 0; i < n; ++i) {
            squared_next[i] = next[next[i]];
            squared_weight[i] = weight[i] * weight[next[i]];
        }
        next.swap(squared_next);
        weight.swap(squared_weight);
    }
    Matrix<S> result(n, std::vector<S>(n, S(0)));
    for (std::size_t i = 0; i < n; ++i) {
        if (transposed) result[destination[i]][i] = value[i];
        else result[i][destination[i]] = value[i];
    }
    return result;
}

// Verify A = u v^T exactly, then use A^e = (v^T u)^(e-1) A.
template <class S, class T>
std::optional<Matrix<S>> rank_one_power(const Matrix<S>& a, T exponent) {
    const std::size_t n = a.size();
    std::size_t pivot_row = 0, pivot_column = 0;
    bool found = false;
    for (std::size_t i = 0; i < n && !found; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            if (!(a[i][j] == S(0))) {
                pivot_row = i;
                pivot_column = j;
                found = true;
                break;
            }
        }
    }
    if (!found) return a;
    const S inverse = S(1) / a[pivot_row][pivot_column];
    std::vector<S> u(n), v(n);
    for (std::size_t i = 0; i < n; ++i) {
        u[i] = a[i][pivot_column];
        v[i] = a[pivot_row][i] * inverse;
    }
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            if (!(a[i][j] == u[i] * v[j])) return std::nullopt;
        }
    }
    S inner(0);
    for (std::size_t i = 0; i < n; ++i) inner = inner + v[i] * u[i];
    const S factor = scalar_power(inner, exponent - 1);
    Matrix<S> result = a;
    for (auto& row : result) {
        for (S& entry : row) entry = entry * factor;
    }
    return result;
}

template <class S>
std::optional<Matrix<S>> invert(const Matrix<S>& a) {
    const std::size_t n = a.size();
    Matrix<S> work = a;
    Matrix<S> result = identity<S>(n);
    for (std::size_t k = 0; k < n; ++k) {
        std::size_t pivot = k;
        while (pivot < n && work[pivot][k] == S(0)) ++pivot;
        if (pivot == n) return std::nullopt;
        if (pivot != k) {
            std::swap(work[pivot], work[k]);
            std::swap(result[pivot], result[k]);
        }
        const S inverse = S(1) / work[k][k];
        for (std::size_t j = k + 1; j < n; ++j) {
            work[k][j] = work[k][j] * inverse;
        }
        for (std::size_t j = 0; j < n; ++j) {
            result[k][j] = result[k][j] * inverse;
        }
        work[k][k] = S(1);
        for (std::size_t i = 0; i < n; ++i) {
            if (i == k) continue;
            const S factor = work[i][k];
            for (std::size_t j = k + 1; j < n; ++j) {
                work[i][j] = work[i][j] - factor * work[k][j];
            }
            for (std::size_t j = 0; j < n; ++j) {
                result[i][j] = result[i][j] - factor * result[k][j];
            }
            work[i][k] = S(0);
        }
    }
    return result;
}

template <class S>
std::vector<S> apply_vector(const Matrix<S>& a, const std::vector<S>& v) {
    const std::size_t n = a.size();
    std::vector<S> result(n, S(0));
    for (std::size_t i = 0; i < n; ++i) {
        result[i] = ::dot_product<FieldAddSubMulDiv<S>>(
            static_cast<int>(n), a[i].begin(), v.begin());
    }
    return result;
}

// recurrence encodes x^n = sum recurrence[i] * x^i.
template <class S>
std::vector<S> polynomial_multiply(const std::vector<S>& a,
                                   const std::vector<S>& b,
                                   const std::vector<S>& recurrence) {
    const std::size_t n = recurrence.size();
    std::vector<S> product(2 * n - 1, S(0));
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            product[i + j] = product[i + j] + a[i] * b[j];
        }
    }
    for (std::size_t d = product.size(); d-- > n;) {
        const S value = product[d];
        for (std::size_t i = 0; i < n; ++i) {
            product[d - n + i] = product[d - n + i] + value * recurrence[i];
        }
    }
    product.resize(n);
    return product;
}

template <class S, class T>
std::vector<S> polynomial_power(T exponent,
                               const std::vector<S>& recurrence) {
    const std::size_t n = recurrence.size();
    std::vector<S> result(n, S(0)), base(n, S(0));
    result[0] = S(1);
    if (n == 1) base[0] = recurrence[0];
    else base[1] = S(1);
    while (exponent) {
        if (exponent & 1) result = polynomial_multiply(result, base, recurrence);
        exponent >>= 1;
        if (exponent) base = polynomial_multiply(base, base, recurrence);
    }
    return result;
}

// A cyclic vector yields K^-1 A K = C, with C the recurrence companion.
// The candidate is deterministic; invertibility decides correctness exactly.
template <class S, class T>
std::optional<Matrix<S>> try_cyclic_power(const Matrix<S>& a, T exponent) {
    const std::size_t n = a.size();
    Matrix<S> basis(n, std::vector<S>(n, S(0)));
    std::vector<S> v(n, S(1));
    for (std::size_t column = 0; column < n; ++column) {
        for (std::size_t i = 0; i < n; ++i) basis[i][column] = v[i];
        v = apply_vector(a, v);
        if (column == 0 && n > 1) {
            bool constant = true;
            for (std::size_t i = 1; i < n; ++i) {
                if (!(v[i] == v[0])) constant = false;
            }
            if (constant) return std::nullopt;
        }
    }
    auto inverse = invert(basis);
    if (!inverse) return std::nullopt;
    const std::vector<S> recurrence = apply_vector(*inverse, v);
    std::vector<S> column = polynomial_power(exponent, recurrence);
    Matrix<S> companion_value(n, std::vector<S>(n, S(0)));
    for (std::size_t j = 0; j < n; ++j) {
        for (std::size_t i = 0; i < n; ++i) companion_value[i][j] = column[i];
        if (j + 1 == n) break;
        const S last = column[n - 1];
        for (std::size_t i = n - 1; i > 0; --i) {
            column[i] = column[i - 1] + last * recurrence[i];
        }
        column[0] = last * recurrence[0];
    }
    return multiply(multiply(basis, companion_value), *inverse);
}

// Similarity elimination uses only division by a verified nonzero field value.
template <class S>
std::vector<S> characteristic_recurrence(Matrix<S> h) {
    const std::size_t n = h.size();
    for (std::size_t k = 0; k + 2 < n; ++k) {
        std::size_t pivot = k + 1;
        while (pivot < n && h[pivot][k] == S(0)) ++pivot;
        if (pivot == n) continue;
        if (pivot != k + 1) {
            std::swap(h[pivot], h[k + 1]);
            for (auto& row : h) std::swap(row[pivot], row[k + 1]);
        }
        const S inverse = S(1) / h[k + 1][k];
        for (std::size_t i = k + 2; i < n; ++i) {
            const S factor = h[i][k] * inverse;
            h[i][k] = S(0);
            for (std::size_t j = k + 1; j < n; ++j) {
                h[i][j] = h[i][j] - factor * h[k + 1][j];
            }
            for (std::size_t j = 0; j < n; ++j) {
                h[j][k + 1] = h[j][k + 1] + factor * h[j][i];
            }
        }
    }
    std::vector<std::vector<S>> determinant(n + 1);
    determinant[0] = {S(1)};
    for (std::size_t m = 1; m <= n; ++m) {
        auto& current = determinant[m];
        current.assign(m + 1, S(0));
        for (std::size_t d = 0; d < m; ++d) {
            current[d + 1] = current[d + 1] + determinant[m - 1][d];
            current[d] = current[d] - h[m - 1][m - 1] * determinant[m - 1][d];
        }
        S product(1);
        for (std::size_t i = m - 1; i-- > 0;) {
            product = product * h[i + 1][i];
            const S factor = h[i][m - 1] * product;
            for (std::size_t d = 0; d <= i; ++d) {
                current[d] = current[d] - factor * determinant[i][d];
            }
        }
    }
    std::vector<S> recurrence(n, S(0));
    for (std::size_t i = 0; i < n; ++i) recurrence[i] = S(0) - determinant[n][i];
    return recurrence;
}

inline std::size_t evaluation_block_size(std::size_t n) {
    std::size_t s = 1;
    while (s * s < n) ++s;
    return s;
}

// Evaluate sum coefficient[i] A^i by grouping consecutive powers.
template <class S>
Matrix<S> evaluate_polynomial(const Matrix<S>& a,
                              const std::vector<S>& coefficient) {
    const std::size_t n = a.size();
    const std::size_t s = evaluation_block_size(n);
    const std::size_t groups = (n + s - 1) / s;
    std::vector<Matrix<S>> small(s);
    small[0] = identity<S>(n);
    for (std::size_t i = 1; i < s; ++i) small[i] = multiply(small[i - 1], a);
    Matrix<S> giant;
    if (groups > 1) giant = multiply(small[s - 1], a);
    Matrix<S> result(n, std::vector<S>(n, S(0)));
    for (std::size_t group = groups; group-- > 0;) {
        if (group + 1 < groups) result = multiply(result, giant);
        for (std::size_t i = 0; i < s && group * s + i < n; ++i) {
            const S value = coefficient[group * s + i];
            for (std::size_t row = 0; row < n; ++row) {
                for (std::size_t column = 0; column < n; ++column) {
                    result[row][column] = result[row][column] + value * small[i][row][column];
                }
            }
        }
    }
    return result;
}

// Requires an exact commutative field, equality, subtraction, and division.
// Finite fields of every characteristic are supported, including n >= p.
// Other coefficient algebras keep their original public binary-power path.
template <class S, class T>
Matrix<S> power_over_field(const Matrix<S>& a, T exponent) {
    require_square(a);
    const std::size_t n = a.size();
    if (exponent == 0) return identity<S>(n);
    if (exponent == 1 || n == 0) return a;

    std::vector<std::size_t> row_next(n), column_next(n), support;
    std::vector<S> row_weight(n, S(0)), column_weight(n, S(0));
    std::vector<bool> active(n, false);
    bool row_map = true, column_map = true;
    for (std::size_t i = 0; i < n; ++i) row_next[i] = column_next[i] = i;
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            if (a[i][j] == S(0)) continue;
            active[i] = active[j] = true;
            if (!(row_weight[i] == S(0))) row_map = false;
            if (!(column_weight[j] == S(0))) column_map = false;
            row_next[i] = j;
            row_weight[i] = a[i][j];
            column_next[j] = i;
            column_weight[j] = a[i][j];
        }
    }
    if (row_map) {
        return power_weighted_map(std::move(row_next), std::move(row_weight), exponent, false);
    }
    if (column_map) {
        return power_weighted_map(std::move(column_next), std::move(column_weight), exponent, true);
    }
    for (std::size_t i = 0; i < n; ++i) {
        if (active[i]) support.push_back(i);
    }
    if (support.size() < n) {
        const std::size_t r = support.size();
        Matrix<S> restricted(r, std::vector<S>(r));
        for (std::size_t i = 0; i < r; ++i) {
            for (std::size_t j = 0; j < r; ++j) restricted[i][j] = a[support[i]][support[j]];
        }
        const Matrix<S> powered = power_over_field(restricted, exponent);
        Matrix<S> result(n, std::vector<S>(n, S(0)));
        for (std::size_t i = 0; i < r; ++i) {
            for (std::size_t j = 0; j < r; ++j) result[support[i]][support[j]] = powered[i][j];
        }
        return result;
    }
    const std::size_t binary_cost = binary_multiplication_count(exponent);
    if (n < 24 || binary_cost <= 10) return power_over_ring(a, exponent);
    if (auto result = rank_one_power(a, exponent)) return std::move(*result);
    if (auto result = try_cyclic_power(a, exponent)) return std::move(*result);
    const std::size_t s = evaluation_block_size(n);
    const std::size_t groups = (n + s - 1) / s;
    // The fixed allowance accounts for characteristic-polynomial arithmetic.
    if (s + groups + 3 >= binary_cost) {
        return power_over_ring(a, exponent);
    }
    const auto recurrence = characteristic_recurrence(a);
    return evaluate_polynomial(a, polynomial_power(exponent, recurrence));
}

// Only the existing ordinary-mod32 algebra trait can call this helper.
// Static moduli reuse their declared mathematical property; dynamic moduli
// are checked from the definition of primality before field elimination.
template <class S>
bool is_prime_modulus() {
    if constexpr (::is_static_modint_v<S>) {
        return ::internal::policy_static<S::mod()>::is_prime;
    } else {
        const std::uint64_t p = S::mod();
        if (p < 2) return false;
        if (p % 2 == 0) return p == 2;
        for (std::uint64_t d = 3; d * d <= p; d += 2) {
            if (p % d == 0) return false;
        }
        return true;
    }
}

} }  // namespace internal::matrix_power_mod32_detail
