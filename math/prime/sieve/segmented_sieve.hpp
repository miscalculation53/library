#pragma once

#include "template/template_all_but_modint.hpp"
#include "math/prime/sieve/linear_sieve.hpp"

/**
 * @brief 区間篩
 * @docs docs/math/prime/sieve/segmented_sieve.md
 */

// l <= x <= r, p | x, p != x を満たす素数 p と整数 x の組を列挙する。
struct segmented_sieve
{
private:
  ll l_, r_;

  struct State
  {
    ll l, r, x = 0;
    vc<ll> rem;
    LinearSieve::PrimeRange<> primes;
    size_t prime_index = 0, rem_index = 0;

    State(ll l, ll r) : l(l), r(r), rem(r - l + 1),
                       primes(LinearSieve::primes(int(iroot(r, 2))))
    {
      for (size_t i = 0; i < rem.size(); ++i) rem[i] = l + ll(i);
    }

    pair<ll, ll> next()
    {
      while (prime_index < primes.size<size_t>())
      {
        const ll p = primes[prime_index];
        if (x == 0)
        {
          const ll q = max(2LL, divceil(l, p));
          if (q > r / p)
          {
            ++prime_index;
            continue;
          }
          x = q * p;
        }
        const ll current = x;
        ll &rest = rem[current - l];
        while (rest % p == 0) rest /= p;
        if (r - current >= p)
          x += p;
        else
          x = 0, ++prime_index;
        return {p, current};
      }
      while (rem_index < rem.size())
      {
        const size_t i = rem_index++;
        const ll current = l + ll(i);
        if (rem[i] > 1 && rem[i] != current)
          return {rem[i], current};
      }
      return {0, 0};
    }
  };

public:
  class Iterator
  {
    friend struct segmented_sieve;
    shared_ptr<State> state_;
    pair<ll, ll> value_{};
    Iterator(ll l, ll r) : state_(make_shared<State>(l, r)) { ++*this; }

  public:
    using iterator_category = input_iterator_tag;
    using value_type = pair<ll, ll>;
    using difference_type = ptrdiff_t;
    using pointer = void;
    using reference = value_type;

    Iterator() = default;
    value_type operator*() const { return value_; }
    Iterator &operator++()
    {
      value_ = state_->next();
      if (value_.first == 0) state_.reset();
      return *this;
    }
    Iterator operator++(int) { auto old = *this; ++*this; return old; }
    bool operator==(const Iterator &other) const
    {
      if (!state_ || !other.state_) return !state_ && !other.state_;
      return state_->l == other.state_->l && state_->r == other.state_->r && value_ == other.value_;
    }
    bool operator!=(const Iterator &other) const { return !(*this == other); }
  };

  segmented_sieve(ll l, ll r) : l_(l), r_(r) { assert(1 <= l && l <= r); }
  Iterator begin() const { return Iterator(l_, r_); }
  Iterator end() const { return Iterator(); }
};

// res[x - l] に x の素因数分解を、素因数の昇順で格納する。
vvc<PrimePower<ll>> segmented_factorize(ll l, ll r)
{
  const auto range = segmented_sieve(l, r);
  vvc<PrimePower<ll>> res(r - l + 1);
  for (auto [p, x] : range)
  {
    auto [e, pe, rest] = ord_pow_div(x, p);
    res[x - l].emplace_back(p, e, pe);
  }
  for (size_t i = 0; i < res.size(); ++i)
  {
    const ll x = l + ll(i);
    if (x >= 2 && res[i].empty()) res[i].emplace_back(x);
  }
  return res;
}
