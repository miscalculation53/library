#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "shift_of_sampling_points.hpp"

/*! SPDX-License-Identifier: CC0-1.0
 * New precomputation independently designed from finite-field interpolation,
 * polynomial products, and convolution; existing dependency notices retained.
 */

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <vector>

#include "internal_factorial_tail_product.hpp"

namespace factorial_many_detail {
template<class mint>
std::vector<mint> convolution_reused(const std::vector<mint>& left,
                                     const std::vector<mint>& right) {
    if (left.empty() || right.empty()) return {};
    const std::size_t total = left.size()+right.size()-1;
    const auto& small = left.size() <= right.size() ? left : right;
    const auto& large = left.size() <= right.size() ? right : left;
    std::size_t first = 0, last = small.size();
    while (first < last && small[first] == mint(0)) ++first;
    while (last > first && small[last-1] == mint(0)) --last;
    if (first == last) return std::vector<mint>(total);
    const std::size_t width = last-first;
    std::size_t transform = 1;
    while (transform < width*8) transform *= 2;
    if (width <= 8 || large.size() < transform || !ntt_ok<mint>(int(transform)))
        return ::convolution(left,right);
    std::vector<mint> frequency(transform), work(transform), result(total);
    std::copy(small.begin()+first,small.begin()+last,frequency.begin());
    ::ntt(frequency);
    const std::size_t chunk = transform-width+1;
    const mint scale = mint(transform).inv();
    for (std::size_t begin = 0; begin < large.size(); begin += chunk) {
        const std::size_t count = std::min(chunk,large.size()-begin);
        std::copy(large.begin()+begin,large.begin()+begin+count,work.begin());
        std::fill(work.begin()+count,work.end(),mint(0));
        ::ntt(work);
        for (std::size_t i = 0; i < transform; ++i) work[i] *= frequency[i];
        ::intt(work);
        for (std::size_t i = 0; i < count+width-1; ++i)
            result[first+begin+i] += work[i]*scale;
    }
    return result;
}

// v[j] is a polynomial of degree at most v.size()-1 evaluated at j.
// Compute that polynomial at start,start+1,... with finite-field zero handling.
template<class Field, class Convolution, class Inverse, class Canonical>
std::vector<Field> shift(const std::vector<Field>& v, Field start, std::size_t count,
                         const std::vector<Field>& inverse_factorials,
                         Convolution convolution, Inverse inverse, Canonical canonical) {
    assert(!v.empty());
    if (!count) return {};
    const std::size_t degree = v.size() - 1;
    if (!degree) return std::vector<Field>(count,v[0]);
    std::vector<Field> weights(v.size());
    for (std::size_t j = 0; j <= degree; ++j) {
        weights[j] = v[j] * inverse_factorials[j] * inverse_factorials[degree-j];
        if ((degree-j) & 1) weights[j] = -weights[j];
    }
    const std::size_t length = degree + count;
    std::vector<Field> denominators(length), prefix(length+1), reciprocals(length);
    prefix[0] = 1;
    Field current = start - Field(degree);
    for (std::size_t i = 0; i < length; ++i) {
        denominators[i] = current;
        prefix[i+1] = current == Field(0) ? prefix[i] : prefix[i] * current;
        current += Field(1);
    }
    Field accumulated_inverse = inverse(prefix.back());
    for (std::size_t i = length; i-- > 0;) {
        if (denominators[i] != Field(0)) {
            reciprocals[i] = accumulated_inverse * prefix[i];
            accumulated_inverse *= denominators[i];
        }
    }
    const auto middle = convolution(weights,reciprocals);
    Field window = 1;
    std::size_t zeroes = 0;
    for (std::size_t j = 0; j <= degree; ++j) {
        if (denominators[j] == Field(0)) ++zeroes;
        else window *= denominators[j];
    }
    std::vector<Field> result(count);
    Field coordinate = start;
    for (std::size_t i = 0; i < count; ++i) {
        if (zeroes) result[i] = v[std::size_t(canonical(coordinate))];
        else result[i] = middle[degree+i] * window;
        coordinate += Field(1);
        if (i+1 == count) break;
        if (denominators[i] == Field(0)) --zeroes;
        else window *= reciprocals[i];
        if (denominators[i+degree+1] == Field(0)) ++zeroes;
        else window *= denominators[i+degree+1];
    }
    return result;
}

// Anchors[q] is (q*block-1)! for q>=1. Anchors[0] is the empty product.
template<class Field, class Convolution, class Inverse, class Canonical>
std::vector<Field> build(std::uint64_t prime, std::uint64_t block, Convolution convolution,
                         Inverse inverse, Canonical canonical, std::size_t full) {
    assert(block && !(block & (block-1)) && block < prime);
    assert(std::uint64_t(full) <= prime/block);
    if (!full) return {Field(1)};
    const std::size_t degree_limit = std::size_t(block);
    std::vector<Field> inverse_factorials(degree_limit+1);
    Field factorial = 1;
    for (std::size_t j = 1; j <= degree_limit; ++j) factorial *= Field(j);
    inverse_factorials.back() = inverse(factorial);
    for (std::size_t j = degree_limit; j; --j)
        inverse_factorials[j-1] = inverse_factorials[j] * Field(j);
    const Field inverse_block = inverse(Field(block));
    // H_d(t*block)=product_{j=0}^{d-1}(t*block+j), t=0,...,d.
    // The known zero at t=0 lets each doubling request exactly 2d new
    // shifted evaluations, keeping a full convolution length at 4d.
    std::vector<Field> values{Field(0),Field(block)};
    for (std::size_t d = 1; d < degree_limit; d *= 2) {
        const auto extra = shift(values,Field(d+1),d,inverse_factorials,convolution,inverse,canonical);
        const auto shifted = shift(values,Field(1)+Field(d)*inverse_block,d*2,
                                   inverse_factorials,convolution,inverse,canonical);
        std::vector<Field> next(d*2+1);
        for (std::size_t t = 1; t <= d*2; ++t)
            next[t] = (t <= d ? values[t] : extra[t-d-1]) * shifted[t-1];
        values = std::move(next);
    }
    if (full > values.size()) {
        auto extra = shift(values,Field(values.size()),full-values.size(),inverse_factorials,
                           convolution,inverse,canonical);
        values.insert(values.end(),extra.begin(),extra.end());
    }
    std::vector<Field> anchors(full+1,Field(1));
    for (std::uint64_t j = 1; j < block; ++j) anchors[1] *= Field(j);
    for (std::size_t q = 2; q <= full; ++q) anchors[q] = anchors[q-1] * values[q-1];
    return anchors;
}

} // namespace factorial_many_detail

/**
 * @brief 高速階乗・等差数列の積
 * @docs docs/math/fps/factorial.md
 */

//! https://suisen-kyopro.hatenablog.com/entry/2023/11/22/201600
//! 同じ数学的手法の CC0 実装: https://github.com/suisen-cp/cp-library-cpp/blob/c49e847ae29ed11510ad9cd7713f5ec77ddf20b1/library/math/factorial_large.hpp
// パラメータ B = 2^k をとって、
// 前計算 O(B log B + (p log p) / B)、クエリ O(B + log p)
// パラメータの値 (デフォルト値はクエリ数 10^5 を想定):
// - NTT friendly のときデフォルトで k = 9 (B = 512)
// - NTT friendly でないときデフォルトで k = 11 (B = 2048)
// - クエリ数が非常に少ないなら k = 13 (B = 8192) とする
template <class mint>
struct Factorial
{
private:
  int k;
  vc<mint> y, z, fac;

public:
  Factorial(int k_ = -1)
  {
    if (k_ == -1) k_ = ntt_ok<mint>(1 << 22) ? 9 : 11;
    assert(k_ >= 0);
    const std::uint64_t prime = std::uint64_t(mint::mod());
    int max_k = 0;
    for (std::uint64_t bound = prime/2; bound >= 2; bound /= 2) ++max_k;
    k = min(k_,max_k);
    const std::uint64_t block = std::uint64_t(1) << k;
    auto inverse = [](mint x) { return x.inv(); };
    auto canonical = [](mint x) { return std::uint64_t(x.val()); };
    auto multiply = [](const std::vector<mint>& a,const std::vector<mint>& b) {
      return factorial_many_detail::convolution_reused(a,b);
    };
    const std::size_t usable = std::size_t((prime-1)/block);
    const bool reflect = (prime-1)%block == 0;
    const std::size_t lower_count = reflect ? usable/2 : std::size_t(prime/block);
    fac = factorial_many_detail::build<mint>(prime,block,multiply,inverse,canonical,lower_count);
    for (std::size_t q = 1; q < fac.size(); ++q) fac[q] *= mint(std::uint64_t(q)*block);
    if (reflect)
    {
      // Wilson: (qB)! ((p-1-qB)!) = (-1)^(qB+1).
      std::vector<mint> prefix(fac.size()+1,mint(1));
      for (std::size_t q = 0; q < fac.size(); ++q) prefix[q+1] = prefix[q]*fac[q];
      mint all_inverse = prefix.back().inv();
      const std::size_t retained = fac.size();
      fac.resize(std::size_t(prime/block)+1);
      for (std::size_t q = retained; q-- > 0;)
      {
        const mint inverse_factorial = all_inverse*prefix[q];
        all_inverse *= fac[q];
        fac[usable-q] = (std::uint64_t(q)*block)&1 ? inverse_factorial : -inverse_factorial;
      }
    }
  }
  mint factorial(ll n) const
  {
    assert(n >= 0);
    const std::uint64_t prime = std::uint64_t(mint::mod()), value = std::uint64_t(n);
    if (value >= prime) return mint(0);
    if (!value) return mint(1);
    const std::uint64_t block = std::uint64_t(1) << k;
    const std::uint64_t q = value/block, lower = q*block;
    std::uint64_t upper = prime-1;
    mint upper_factorial = -mint(1);
    if (q+1 < fac.size()) {
      const std::uint64_t next = (q+1)*block;
      if (next <= upper) { upper = next; upper_factorial = fac[std::size_t(q+1)]; }
    }
    if (upper-value < value-lower) {
      const mint tail = factorial_tail_product_detail::rising_product_candidate(
        mint(value+1), int(upper-value));
      return upper_factorial/tail;
    }
    return fac[std::size_t(q)] * factorial_tail_product_detail::rising_product_candidate(
      mint(lower+1), int(value-lower));
  }

  // prod_{i=0}^{n-1} (a*i+b)
  mint arithmetic_product(ll n, mint a, mint b) const
  {
    assert(n >= 0);
    if (n == 0) return 1;
    if (a == 0) return b.pow(n);
    auto direct = [&]()
    {
      mint res = 1, x = b;
      for (ll i = 0; i < n; ++i, x += a) res *= x;
      return res;
    };
    if (n <= 32) return direct();
    const ll p = mint::mod(), c = (b / a).val();
    if (c == 0 || n > p - c) return 0;
    const ll l = c - 1, r = c + n - 1, block = 1LL << k;
    const ll last = (p - 1) >> k;
    // R と p-1-L を B の倍数にする。既存の階乗表だけで R!/L! を得る。
    const ll L = p - 1 - (min(last, (p - c + block / 2) >> k) << k);
    const ll R = min(last, (r + block / 2) >> k) << k;
    if (n <= abs(L - l) + abs(R - r)) return direct();
    mint res = fac[R >> k] * fac[(p - 1 - L) >> k], den = 1;
    if (L % 2 == 0) res = -res;
    // 内側の区切りなら掛け足し、外側なら余分な因子を分母にまとめる。
    mint tail = 1;
    for (ll j = min(L, l) + 1; j <= max(L, l); ++j) tail *= mint::raw(j);
    if (L < l) den *= tail;
    else res *= tail;
    tail = 1;
    for (ll j = min(R, r) + 1; j <= max(R, r); ++j) tail *= mint::raw(j);
    if (R > r) den *= tail;
    else res *= tail;
    if (L < l || R > r) res /= den;
    return a.pow(n) * res;
  }
};
