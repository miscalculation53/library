#pragma once

#include "template/template_all_but_modint.hpp"
#include "math/prime/prime_power.hpp"

/**
 * @brief 線形篩
 * @docs docs/math/prime/sieve/linear_sieve.md
 */

struct LinearSieve
{
private:
  static vc<int> primes_;

public:
  static int n;
  static vc<PrimePower<int>> lpf_;

  template <class P = ll>
  class PrimeRange
  {
    friend struct LinearSieve;
    size_t count_;
    explicit PrimeRange(size_t count) : count_(count) {}

  public:
    class Iterator
    {
      friend class PrimeRange;
      size_t index_ = 0;
      explicit Iterator(size_t index) : index_(index) {}

    public:
      using iterator_category = input_iterator_tag;
      using value_type = P;
      using difference_type = ptrdiff_t;
      using pointer = void;
      using reference = P;
      Iterator() = default;
      P operator*() const { return P(primes_[index_]); }
      Iterator &operator++() { ++index_; return *this; }
      Iterator operator++(int) { auto old = *this; ++*this; return old; }
      bool operator==(const Iterator &other) const { return index_ == other.index_; }
      bool operator!=(const Iterator &other) const { return !(*this == other); }
    };

    template <class I = ll>
    I size() const { return I(count_); }
    bool empty() const { return count_ == 0; }
    P operator[](size_t i) const { assert(i < count_); return P(primes_[i]); }
    Iterator begin() const { return Iterator(0); }
    Iterator end() const { return Iterator(count_); }
  };

  // limit 以下の素数を昇順に
  template <class P = ll>
  static PrimeRange<P> primes(int limit)
  {
    reserve(limit);
    return PrimeRange<P>(upper_bound(primes_.begin(), primes_.end(), limit) - primes_.begin());
  }

  static void reserve(int n_)
  {
    if (n_ <= n)
      return;
    n = max(n_, 2 * n);
    lpf_.resize(n + 1);
    for (int d = 2; d <= n; d++)
    {
      if (lpf_[d].p == -1)
      {
        lpf_[d] = PrimePower<int>(d, 1, d);
        primes_.eb(d);
      }
      fec(p : primes_)
      {
        if (p > n / d || p > lpf_[d].p)
          break;
        if (lpf_[d].p == p)
          lpf_[p * d] = PrimePower<int>(p, lpf_[d].e + 1, lpf_[d].pe * p);
        else
          lpf_[p * d] = PrimePower<int>(p, 1, p);
      }
    }
  }

  template <class P = int>
  static PrimePower<P> lpf(int n)
  {
    assert(n >= 1);
    reserve(n);
    return lpf_[n];
  }

  static bool is_prime(int n)
  {
    if (n <= 1)
      return false;
    return lpf(n).p == n;
  }

  // 重複を含めた素因数の個数 Ω(n)
  static int Omega(int n)
  {
    assert(1 <= n);
    static vc<unsigned char> table{0, 0};
    if (n >= int(table.size()))
    {
      reserve(n);
      const int first = int(table.size());
      table.resize(n + 1);
      for (int i = first; i <= n; i++)
        table[i] = table[i / lpf_[i].p] + 1;
    }
    return table[n];
  }

  // 計算量: O(n の素因数の種類数) = O(log n / loglog n)
  template <class P = int>
  static vc<PrimePower<P>> factorize(int n)
  {
    assert(n >= 1);
    reserve(n);
    vc<PrimePower<P>> res;
    while (n > 1)
    {
      res.eb(lpf_[n]);
      n /= lpf_[n].pe;
    }
    return res;
  }
};
vc<PrimePower<int>> LinearSieve::lpf_{};
int LinearSieve::n{};
vc<int> LinearSieve::primes_{};
