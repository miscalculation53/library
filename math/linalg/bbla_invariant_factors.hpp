#pragma once

#include "bbla.hpp"

/**
 * @brief Black Box Linear Algebra（特性多項式・不変因子）
 * @docs docs/math/linalg/bbla.md
 */

namespace bbla
{
namespace detail
{
template <class F>
void trim_polynomial(vc<typename F::S> &p)
{
  while (!p.empty() && p.back() == F::e0()) p.pop_back();
}

template <class F>
pair<vc<typename F::S>, vc<typename F::S>> polynomial_divmod(
    vc<typename F::S> a, const vc<typename F::S> &b)
{
  using S = typename F::S;
  assert(!b.empty() && b.back() != F::e0());
  trim_polynomial<F>(a);
  int k = int(a.size()) - int(b.size()) + 1;
  if (k <= 0) return {{}, move(a)};
  vc<S> q(k, F::e0());
  S inv_lead = F::inv(b.back());
  repi(i, k - 1, -1, -1)
  {
    S c = F::mul(a[i + int(b.size()) - 1], inv_lead);
    q[i] = c;
    if (c == F::e0()) continue;
    S minus_c = F::minus(c);
    repi(j, b.size()) a[i + j] = F::add(a[i + j], F::mul(minus_c, b[j]));
  }
  trim_polynomial<F>(q);
  trim_polynomial<F>(a);
  return {move(q), move(a)};
}

template <class F>
vc<typename F::S> multiply_polynomials(const vc<typename F::S> &a, const vc<typename F::S> &b)
{
  using S = typename F::S;
  if (a.empty() || b.empty()) return {};
  vc<S> c(a.size() + b.size() - 1, F::e0());
  repi(i, a.size()) repi(j, b.size()) c[i + j] = F::add(c[i + j], F::mul(a[i], b[j]));
  return c;
}

// 挿入した元のベクトルに対する従属関係を復元できる、rollback 可能な基底。
template <class F>
struct KrylovBasis
{
  using S = typename F::S;
  using V = vc<S>;
  int n;
  vc<V> original, echelon, representation;
  vc<int> pivot;

  explicit KrylovBasis(int n) : n(n) {}
  int size() const { return original.size(); }
  void rollback(int k)
  {
    original.resize(k);
    echelon.resize(k);
    representation.resize(k);
    pivot.resize(k);
  }
  // true: 挿入した。false: x = sum coeff[i] * original[i]。
  bool insert(const V &x, V &coeff)
  {
    assert(int(x.size()) == n);
    const int k = size();
    V v = x;
    coeff.assign(k, F::e0());
    repi(i, k)
    {
      S c = v[pivot[i]];
      if (c == F::e0()) continue;
      S minus_c = F::minus(c);
      repi(j, n) v[j] = F::add(v[j], F::mul(minus_c, echelon[i][j]));
      repi(j, i + 1) coeff[j] = F::add(coeff[j], F::mul(c, representation[i][j]));
    }
    int p = 0;
    while (p < n && v[p] == F::e0()) p++;
    if (p == n) return false;
    S scale = F::inv(v[p]);
    for (S &x : v) x = F::mul(x, scale);
    V expression(k + 1, F::e0());
    repi(i, k) expression[i] = F::minus(F::mul(coeff[i], scale));
    expression[k] = scale;
    original.eb(x);
    echelon.eb(move(v));
    representation.eb(move(expression));
    pivot.eb(p);
    return true;
  }
};

template <class F, class LinearMap, class RandomSample>
optional<vc<vc<typename F::S>>> invariant_factors_trial(int n, const LinearMap &apply,
                                                     const RandomSample &sample)
{
  using S = typename F::S;
  using V = vc<S>;
  KrylovBasis<F> basis(n);
  vc<V> factors;
  vc<int> starts;
  while (basis.size() < n)
  {
    const int start = basis.size();
    V v(n), relation;
    for (auto &x : v) x = sample();
    V cur = v;
    while (basis.insert(cur, relation)) apply(cur);
    const int d = basis.size() - start;
    if (d == 0) continue;
    V p(d + 1, F::e0());
    repi(i, d) p[i] = F::minus(relation[start + i]);
    p[d] = F::e1();
    if (!factors.empty() && !polynomial_divmod<F>(factors.back(), p).second.empty())
      return nullopt;

    // p(A)v = sum_j g_j(A)v_j。p | g_j を確認して v から前の部分空間の成分を除く。
    bool corrected = false;
    repi(j, factors.size())
    {
      int l = starts[j], r = l + int(factors[j].size()) - 1;
      V g(relation.begin() + l, relation.begin() + r);
      auto qr = polynomial_divmod<F>(move(g), p);
      if (!qr.second.empty()) return nullopt;
      repi(i, qr.first.size())
      {
        S c = F::minus(qr.first[i]);
        if (c == F::e0()) continue;
        corrected = true;
        repi(k, n) v[k] = F::add(v[k], F::mul(c, basis.original[l + i][k]));
      }
    }
    if (corrected)
    {
      basis.rollback(start);
      cur = v;
      repi(i, d)
      {
        if (!basis.insert(cur, relation)) return nullopt;
        apply(cur);
      }
      // 補正後のブロックが閉じていることも、元の作用で検算する。
      repi(i, d) repi(j, n)
        cur[j] = F::add(cur[j], F::mul(p[i], basis.original[start + i][j]));
      for (auto &x : cur) if (x != F::e0()) return nullopt;
    }
    starts.eb(start);
    factors.eb(move(p));
  }
  reverse(factors.begin(), factors.end());
  return factors;
}
} // namespace detail

// det(xI-A)。昇冪順・monic。n=0 は {1}。
// 最初に射影最小多項式を試し、次数 n なら O(n^2+nt) 時間・O(n) 空間。
// それ以外は決定的な Krylov 分解で O(n^3+nt) 時間・O(n^2) 空間。返り値は常に正しい。
template <class F, class LinearMap, class RandomSample = decltype(random_sample_mint<typename F::S>)>
vc<typename F::S> characteristic_polynomial(int n, const LinearMap &apply,
                                           const RandomSample &random_sample = random_sample_mint)
{
  using S = typename F::S;
  using V = vc<S>;
  assert(n >= 0);
  if (n == 0) return {F::e1()};
  auto m = minimal_polynomial<F>(n, apply, random_sample);
  if (int(m.size()) == n + 1)
  {
    for (auto &x : m) x = F::minus(x);
    return m;
  }
  detail::KrylovBasis<F> basis(n);
  V result{F::e1()}, relation;
  repi(coordinate, n)
  {
    if (basis.size() == n) break;
    const int start = basis.size();
    V cur(n, F::e0());
    cur[coordinate] = F::e1();
    while (basis.insert(cur, relation)) apply(cur);
    const int d = basis.size() - start;
    if (d == 0) continue;
    V p(d + 1, F::e0());
    repi(i, d) p[i] = F::minus(relation[start + i]);
    p[d] = F::e1();
    result = detail::multiply_polynomials<F>(result, p);
  }
  return result;
}

// f_1 | f_2 | ... の順の不変因子。各多項式は昇冪順・monic、定数多項式は含まない。
// 各 Krylov ブロックの独立性・閉性・整除関係を検査する Las Vegas 法。
// 十分大きいサンプル集合を仮定。通常 O(n^3+nt) 時間・O(n^2) 空間。
// 最初の射影で次数 n を得た場合は O(n^2+nt) 時間・O(n) 空間。
template <class F, class LinearMap, class RandomSample = decltype(random_sample_mint<typename F::S>)>
vc<vc<typename F::S>> invariant_factors(int n, const LinearMap &apply,
                                      const RandomSample &random_sample = random_sample_mint)
{
  assert(n >= 0);
  if (n == 0) return {};
  auto m = minimal_polynomial<F>(n, apply, random_sample);
  if (int(m.size()) == n + 1)
  {
    for (auto &x : m) x = F::minus(x);
    return {move(m)};
  }
  while (true)
  {
    auto result = detail::invariant_factors_trial<F>(n, apply, random_sample);
    if (result) return move(*result);
  }
}
} // namespace bbla
