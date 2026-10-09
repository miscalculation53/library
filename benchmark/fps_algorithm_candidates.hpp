#pragma once

#include "math/convolution/convolution.hpp"
#include "math/fps/fps.hpp"

namespace fps_algorithm_audit
{
struct TransformCounts
{
  map<int, array<ll, 2>> lengths;
  map<pair<int,int>, array<ll, 2>> kernels;
  void add(int n, int direction, int mod) { ++lengths[n][direction]; ++kernels[{mod,n}][direction]; }
  ll calls() const
  {
    ll res = 0;
    for (auto [n, a] : lengths) res += a[0] + a[1];
    return res;
  }
  ll weight() const
  {
    ll res = 0;
    for (auto [n, a] : lengths) res += ll(n) * countr_zero(unsigned(n)) * (a[0] + a[1]);
    return res;
  }
};
inline TransformCounts *recording = nullptr;
template <class V> void forward(V &v)
{
  if (recording) recording->add(v.size(), 0, V::value_type::mod());
  ::ntt(v);
}
template <class V> void backward(V &v)
{
  if (recording) recording->add(v.size(), 1, V::value_type::mod());
  ::intt(v);
}
template <class mint> vc<mint> native_convolution(vc<mint> a, vc<mint> b)
{
  int length = a.size() + b.size() - 1, z = bit_ceil(length);
  if (a == b)
  {
    a.resize(z), forward(a);
    for (auto &v : a) v *= v;
  }
  else
  {
    a.resize(z), b.resize(z), forward(a), forward(b);
    repi(i, z) a[i] *= b[i];
  }
  backward(a);
  mint iz = mint(z).inv();
  for (auto &v : a) v *= iz;
  a.resize(length);
  return a;
}
template <int prime, class mint>
vc<static_modint32<prime>> prime_convolution(const vc<mint> &a, const vc<mint> &b)
{
  using M = static_modint32<prime>;
  vc<M> aa(a.size()), bb(b.size());
  repi(i, a.size()) aa[i] = a[i].val();
  repi(i, b.size()) bb[i] = b[i].val();
  return native_convolution(std::move(aa), std::move(bb));
}
// Timed runs use the actual convolution. Profiling reproduces its NTT calls.
template <class mint> vc<mint> convolve(const vc<mint> &a, const vc<mint> &b)
{
  if (!recording) return ::convolution(a, b);
  if (a.empty() || b.empty()) return {};
  int cnta = count_if(a.begin(), a.end(), [](mint x){ return x != 0; });
  int cntb = count_if(b.begin(), b.end(), [](mint x){ return x != 0; });
  if (ntt_ok<mint>(a.size() + b.size() - 1))
  {
    if (min(cnta, cntb) <= 60) return internal::convolution_naive(a, b);
    return native_convolution(a, b);
  }
  if (min(cnta, cntb) <= 300) return internal::convolution_naive(a, b);
  auto x = prime_convolution<469762049>(a, b);
  auto y = prime_convolution<1811939329>(a, b);
  auto z = prime_convolution<2013265921>(a, b);
  constexpr array<int, 3> primes{469762049, 1811939329, 2013265921};
  vc<mint> c(x.size());
  repi(i, c.size()) c[i] = crt_mod_constexpr<mint>(array<ll,3>{x[i].val(),y[i].val(),z[i].val()}, primes).first;
  return c;
}
} // namespace fps_algorithm_audit

// Keep the earlier baseline intact, while counting all transforms in this audit.
#define ntt fps_algorithm_audit::forward
#define intt fps_algorithm_audit::backward
#define convolution fps_algorithm_audit::convolve
#include "fps_constant_factor_variants.hpp"
#undef convolution
#undef intt
#undef ntt

namespace fps_algorithm_audit
{
template <class mint> using P = AuditFPS<mint, 15>;

// Bostan-Schost with a length-m cyclic derivative product.
// split_last uses the paper's final half-size products; paper_inverse uses fg^2.
template <class mint>
P<mint> exp_bs(const P<mint> &h, int n, bool split_last = false, bool paper_inverse = false)
{
  using F = P<mint>;
  assert(n >= 0 && h.get(0) == 0);
  if (n == 0) return {};
  assert(ntt_ok<mint>(bit_ceil(n)));
  Binomial<mint>::reserve(bit_ceil(n) - 1);
  if (n == 1) return {1};
  F f{1, h.get(1)}, g{1}, g_fft{1,1};
  for (int m = 2; m < n; m *= 2)
  {
    mint im = mint(m).inv(), i2m = im * Binomial<mint>::inv(2);
    F f_fft = f.resized(2 * m);
    forward(f_fft);
    F old_g_fft = g_fft;
    if (paper_inverse)
    {
      static const internal::fft_info<mint> info;
      mint root = info.root[bit_width(unsigned(m))], pw = 1;
      F odd(m);
      repi(i, g.sz()) odd[i] = g[i] * pw, pw *= root;
      forward(odd);
      F product(2 * m);
      repi(i, m) product[i] = f_fft[i] * old_g_fft[i] * old_g_fft[i];
      repi(i, m) product[m+i] = f_fft[m+i] * odd[i] * odd[i];
      backward(product);
      repi(i, m / 2, m) g.push_back(-product[i] * i2m);
    }
    else
    {
      F error(m);
      repi(i, m) error[i] = f_fft[i] * old_g_fft[i];
      backward(error);
      repi(i, m / 2) error[i] = error[i + m / 2] * im;
      fill(error.begin() + m / 2, error.end(), mint(0));
      forward(error);
      repi(i, m) error[i] *= old_g_fft[i];
      backward(error);
      repi(i, m / 2) g.push_back(-error[i] * im);
    }
    F r(m);
    repi(i, 1, min(m, h.sz())) r[i - 1] = mint(i) * h[i];
    forward(r);
    repi(i, m) r[i] *= f_fft[i];
    backward(r);
    repi(i, m) r[i] *= im;
    F s(m);
    repi(i, m)
    {
      mint derivative = i + 1 < m ? mint(i + 1) * f[i + 1] : mint(0);
      s[(i + 1) % m] = derivative - r[i];
    }
    F t;
    if (split_last && 2 * m >= n)
    {
      F g1(m), s0(m), s1(m);
      repi(i, m / 2) g1[i] = g[i + m / 2], s0[i] = s[i], s1[i] = s[i + m / 2];
      forward(g1), forward(s0), forward(s1);
      F low(m), cross(m);
      repi(i, m)
      {
        low[i] = old_g_fft[i] * s0[i];
        cross[i] = old_g_fft[i] * s1[i] + g1[i] * s0[i];
      }
      backward(low), backward(cross);
      t.resize(m);
      repi(i, m) t[i] = low[i] * im;
      repi(i, m / 2) t[m / 2 + i] += cross[i] * im;
    }
    else
    {
      g_fft = g.resized(2 * m);
      forward(g_fft);
      t = s.resized(2 * m);
      forward(t);
      repi(i, 2 * m) t[i] *= g_fft[i];
      backward(t);
      t.resize(m);
      for (auto &v : t) v *= i2m;
    }
    F u(2 * m);
    repi(i, m) u[i] = h.get(m + i) - t[i] * Binomial<mint>::inv_[m + i];
    forward(u);
    repi(i, 2 * m) u[i] *= f_fft[i];
    backward(u);
    repi(i, m) f.push_back(u[i] * i2m);
  }
  f.resize(n);
  return f;
}

// The coefficient of block k in a*b, using one inverse transform.
// Each block has m coefficients; spectra have length 2m in bit-reversed order.
template <class mint>
P<mint> block_product(const vc<P<mint>> &a, const vc<P<mint>> &b, int k, int m, bool delayed = false)
{
  P<mint> freq(2 * m);
  bool square = &a == &b;
  if (delayed)
  {
    const ull mod = mint::mod(), max_product = (mod - 1) * (mod - 1);
    const ull batch = numeric_limits<ull>::max() / max_product;
    square = square && mod <= (1ULL << 31);
    vc<ull> positive(2 * m), negative(2 * m);
    ull pending = 0;
    auto flush = [&]()
    {
      repi(j, 2 * m)
      {
        freq[j] += mint(positive[j] % mod);
        freq[j] -= mint(negative[j] % mod);
        positive[j] = negative[j] = 0;
      }
      pending = 0;
    };
    for (int carry : {0,1})
      for (int i = max(0, k - carry - int(b.size()) + 1); i <= k - carry && i < int(a.size()); ++i)
        if (!a[i].empty() && !b[k-carry-i].empty())
        {
          int other = k-carry-i;
          if (square && i > other) continue;
          int times = square && i < other ? 2 : 1;
          if (pending + times > batch) flush();
          repi(j, 2 * m)
          {
            ull v = ull(a[i][j].val()) * b[other][j].val() * times;
            if (carry && j >= m) negative[j] += v;
            else positive[j] += v;
          }
          pending += times;
          if (pending == batch) flush();
        }
    if (pending) flush();
    backward(freq);
    freq.resize(m);
    mint iz = mint(2 * m).inv();
    for (auto &v : freq) v *= iz;
    return freq;
  }
  for (int i = max(0, k - int(b.size()) + 1); i <= k && i < int(a.size()); ++i)
    if (!a[i].empty() && !b[k-i].empty())
      repi(j, 2 * m) freq[j] += a[i][j] * b[k-i][j];
  for (int i = max(0, k - int(b.size())); i < k && i < int(a.size()); ++i)
    if (!a[i].empty() && !b[k-1-i].empty())
      repi(j, 2 * m)
      {
        mint v = a[i][j] * b[k-1-i][j];
        if (j < m) freq[j] += v;
        else freq[j] -= v;
      }
  backward(freq);
  freq.resize(m);
  mint iz = mint(2 * m).inv();
  for (auto &v : freq) v *= iz;
  return freq;
}

// Harvey 2009, Algorithm 1. u is computed as the inverse of the seed.
template <class mint>
P<mint> exp_harvey(const P<mint> &f, int n, int blocks, bool delayed = true)
{
  using F = P<mint>;
  assert(n >= 0 && f.get(0) == 0);
  if (n == 0) return {};
  int N = bit_ceil(n), s = min(blocks, max(1, N / 2)), m = N / (2 * s);
  if (N == 1) return {1};
  assert(has_single_bit(unsigned(s)) && ntt_ok<mint>(2 * m));
  Binomial<mint>::reserve(N - 1);
  vc<F> g(s), gf(s), q(2 * s), qf(2 * s), ef(s);
  g[0] = exp_bs(f.resized(m), m);
  F u = g[0].inv(m).resized(2 * m);
  forward(u);
  gf[0] = g[0].resized(2 * m);
  forward(gf[0]);
  repi(k, s)
  {
    q[k].resize(m);
    repi(i, m) q[k][i] = mint(k * m + i) * f.get(k * m + i);
    qf[k] = q[k].resized(2 * m);
    forward(qf[k]);
  }
  repi(k, 1, s)
  {
    F phi = block_product(gf, qf, k, m, delayed).resized(2 * m);
    forward(phi);
    repi(i, 2 * m) phi[i] *= u[i];
    backward(phi);
    phi.resize(m);
    mint iz = mint(2 * m).inv();
    repi(i, m) phi[i] *= iz * Binomial<mint>::inv_[k * m + i];
    phi.resize(2 * m);
    forward(phi);
    repi(i, 2 * m) phi[i] *= gf[0][i];
    backward(phi);
    phi.resize(m);
    for (auto &v : phi) v *= iz;
    g[k] = std::move(phi);
    gf[k] = g[k].resized(2 * m);
    forward(gf[k]);
  }
  repi(k, s, 2 * s)
  {
    F value = block_product(qf, gf, k, m, delayed).resized(2 * m);
    forward(value);
    repi(i, 2 * m) value[i] *= u[i];
    backward(value);
    value.resize(m);
    mint iz = -mint(2 * m).inv();
    for (auto &v : value) v *= iz;
    q[k] = std::move(value);
    qf[k] = q[k].resized(2 * m);
    forward(qf[k]);
  }
  repi(k, s)
  {
    ef[k].resize(2 * m);
    repi(i, m) ef[k][i] = q[s+k][i] * Binomial<mint>::inv_[(s+k) * m + i] - f.get((s+k) * m + i);
    forward(ef[k]);
  }
  F result(N);
  repi(k, s)
  {
    copy(g[k].begin(), g[k].end(), result.begin() + k * m);
    F value = block_product(gf, ef, k, m, delayed);
    repi(i, m) result[(s+k) * m + i] = -value[i];
  }
  result.resize(n);
  return result;
}

template <class mint>
P<mint> exp_bs_convolution(const P<mint> &h, int n)
{
  using F = P<mint>;
  assert(n >= 0 && h.get(0) == 0);
  if (n == 0) return {};
  Binomial<mint>::reserve(bit_ceil(n) - 1);
  if (n == 1) return {1};
  F f{1, h.get(1)}, g{1};
  for (int m = 2; m < n; m *= 2)
  {
    F fg2 = f * (g * g);
    g.resize(m);
    repi(i, m / 2, m) g[i] = -fg2.get(i);
    F q(m - 1);
    repi(i, 1, min(m, h.sz())) q[i - 1] = mint(i) * h[i];
    F r = (f * q).circular_mod(m), s(m);
    repi(i, m)
      s[(i + 1) % m] = (i + 1 < m ? mint(i + 1) * f[i + 1] : mint(0)) - r[i];
    F t = (g * s).resized(m), u(m);
    repi(i, m) u[i] = h.get(m + i) - t[i] * Binomial<mint>::inv_[m + i];
    F v = (f * u).resized(m);
    f.insert(f.end(), v.begin(), v.end());
  }
  f.resize(n);
  return f;
}

template <int prime, class mint>
vc<static_modint32<prime>> middle_prime(const P<mint> &a, const P<mint> &b, int z)
{
  using M = static_modint32<prime>;
  vc<M> x(z), y(z);
  repi(i, a.sz()) x[i] = a[i].val();
  repi(i, b.sz()) y[i] = b[b.sz()-1-i].val();
  forward(x), forward(y);
  repi(i, z) x[i] *= y[i];
  backward(x);
  int out = a.sz() - b.sz() + 1;
  vc<M> result(out);
  M iz = M(z).inv();
  repi(i, out) result[i] = x[b.sz()-1+i] * iz;
  return result;
}

// y[i] = sum_j a[i+j]*b[j]. Cyclic wrap lands strictly below b.size()-1.
template <class mint> P<mint> middle_product(const P<mint> &a, const P<mint> &b, bool short_crt = true)
{
  using F = P<mint>;
  assert(!b.empty() && a.size() >= b.size());
  int out = a.size() - b.size() + 1;
  int cutoff = ntt_ok<mint>(a.sz()) ? 32 : 128;
  if (min(out, b.sz()) <= cutoff)
  {
    F result(out);
    repi(i, out) result[i] = dot_product<RingAddSubMul<mint>>(b.sz(), a.begin() + i, b.begin());
    return result;
  }
  vc<pair<int,mint>> nz;
  repi(j, b.sz()) if (b[j] != 0)
  {
    nz.emplace_back(j, b[j]);
    if (int(nz.size()) > cutoff) break;
  }
  if (int(nz.size()) <= cutoff)
  {
    F result(out);
    for (auto [j, v] : nz) repi(i, out) result[i] += a[i+j] * v;
    return result;
  }
  if (ntt_ok<mint>(a.sz()))
  {
    int z = bit_ceil(a.sz());
    F x = a.resized(z), y = b.rev().resized(z);
    forward(x), forward(y);
    repi(i, z) x[i] *= y[i];
    backward(x);
    mint iz = mint(z).inv();
    F result(out);
    repi(i, out) result[i] = x[b.sz() - 1 + i] * iz;
    return result;
  }
  if constexpr (internal::ordinary_mod32<mint>::value)
    if (short_crt)
    {
      int z = bit_ceil(a.sz());
      auto x = middle_prime<469762049>(a, b, z);
      auto y = middle_prime<1811939329>(a, b, z);
      auto w = middle_prime<2013265921>(a, b, z);
      constexpr array<int,3> primes{469762049,1811939329,2013265921};
      F result(out);
      repi(i, out) result[i] = crt_mod_constexpr<mint>(array<ll,3>{x[i].val(),y[i].val(),w[i].val()},primes).first;
      return result;
    }
  F product = a * b.rev();
  return F(product.begin() + b.sz() - 1, product.begin() + a.sz());
}

template <class mint>
struct TransposedEvaluation
{
  using F = P<mint>;
  int count, base;
  bool cache, short_crt;
  vc<F> product, spectrum;
  explicit TransposedEvaluation(const vc<mint> &xs, bool cached, bool shorten_crt = true)
      : count(xs.size()), base(bit_ceil(max(1, count))), cache(cached), short_crt(shorten_crt), product(2 * base), spectrum(2 * base)
  {
    repi(i, base) product[base + i] = {1, i < count ? -xs[i] : mint(0)};
    for (int i = base - 1; i > 0; --i)
    {
      F &a = product[2 * i], &b = product[2 * i + 1];
      int k = a.sz() - 1, z = 2 * k;
      if (cache && k > 32 && ntt_ok<mint>(z) &&
          min(a.sz() - std::count(a.begin(), a.end(), mint(0)), b.sz() - std::count(b.begin(), b.end(), mint(0))) > 60)
      {
        F x = a.resized(z), y = b.resized(z);
        forward(x), forward(y);
        spectrum[2 * i] = x, spectrum[2 * i + 1] = y;
        repi(j, z) x[j] *= y[j];
        backward(x);
        mint iz = mint(z).inv();
        for (auto &v : x) v *= iz;
        x[0] = 1;
        x.push_back(a.back() * b.back());
        product[i] = std::move(x);
      }
      else product[i] = a * b;
    }
  }
  vc<mint> evaluate(F f) const
  {
    if (count == 0 || f.empty()) return vc<mint>(count);
    int n = f.sz();
    F inv = product[1].resized(n).inv(n);
    f.resize(n + base - 1);
    F root = middle_product(f, inv, short_crt);
    vc<F> value(2 * base);
    value[1] = std::move(root);
    repi(i, 1, base)
    {
      if (!spectrum[2 * i].empty())
      {
        int z = value[i].sz(), k = z / 2;
        F v = std::move(value[i]);
        reverse(v.begin() + 1, v.end());
        forward(v);
        F left(z), right(z);
        repi(j, z)
        {
          left[j] = v[j] * spectrum[2 * i + 1][j];
          right[j] = v[j] * spectrum[2 * i][j];
        }
        backward(left), backward(right);
        reverse(left.begin() + 1, left.end()), reverse(right.begin() + 1, right.end());
        mint iz = mint(z).inv();
        left.resize(k), right.resize(k);
        for (auto &v : left) v *= iz;
        for (auto &v : right) v *= iz;
        value[2 * i] = std::move(left), value[2 * i + 1] = std::move(right);
      }
      else
      {
        value[2 * i] = middle_product(value[i], product[2 * i + 1], short_crt);
        value[2 * i + 1] = middle_product(value[i], product[2 * i], short_crt);
        F().swap(value[i]);
      }
    }
    vc<mint> result(count);
    repi(i, count) result[i] = value[base + i][0];
    return result;
  }
};
template <class mint>
vc<mint> multipoint_transposed(const FormalPowerSeries<mint> &f, const vc<mint> &xs, bool cache = true, bool short_crt = true)
{
  return TransposedEvaluation<mint>(xs, cache, short_crt).evaluate(P<mint>(f.begin(), f.end()));
}
template <class mint>
vc<mint> multipoint_adaptive(const FormalPowerSeries<mint> &f, const vc<mint> &xs)
{
  if (f.sz() <= 64 || xs.size() <= 1)
  {
    vc<mint> result(xs.size());
    repi(i, xs.size()) result[i] = f.eval(xs[i]);
    return result;
  }
  return multipoint_transposed<mint>(f, xs, true);
}
} // namespace fps_algorithm_audit
