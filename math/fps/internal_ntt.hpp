#pragma once
#include "../convolution/convolution.hpp"
#include "../modint/binomial.hpp"

namespace internal
{
// NTT work is measured in L*log2(L). Coefficients are fitted in benchmark/fps_production_threshold_*.csv.
enum class FPSSparseOperation
{
  inv,
  div,
  exp,
  pow,
  sqrt
};
inline ll fps_ntt_weight(int n)
{
  return ll(n) * countr_zero(unsigned(n));
}
inline ll fps_inv_weight(int n, bool native)
{
  int N = bit_ceil(n);
  if (native && n >= 512 && ll(n) * 4 <= ll(N) * 3)
  {
    int s = min(8, N / 2), m = N / (2 * s), r = (n + m - 1) / m;
    return (4 * r + s - 3) * fps_ntt_weight(2 * m) + fps_inv_weight(m, true);
  }
  ll cost = 0;
  for (int m = 1; m < n; m *= 2)
    cost += native ? 5 * fps_ntt_weight(2 * m) : 6 * fps_ntt_weight(2 * m) + 9 * fps_ntt_weight(4 * m);
  return cost;
}
inline ll fps_exp_bs_weight(int n)
{
  int N = bit_ceil(n);
  ll cost = 0;
  for (int m = 2; m < n; m *= 2)
    cost += 5 * fps_ntt_weight(m) + 6 * fps_ntt_weight(2 * m);
  if (N >= 4)
    cost -= 3 * fps_ntt_weight(N) - 5 * fps_ntt_weight(N / 2);
  return cost;
}
inline ll fps_exp_weight(int n, bool native)
{
  if (native && n >= 512)
  {
    int N = bit_ceil(n), s = min(n <= 4096 ? 4 : 16, N / 2), m = N / (2 * s);
    int high = (n + m - 1) / m - s;
    return (7 * s + 6 * high - 4) * fps_ntt_weight(2 * m) + fps_exp_bs_weight(m) + fps_inv_weight(m, true);
  }
  if (native)
    return fps_exp_bs_weight(n);
  ll cost = 0;
  for (int m = 2; m < n; m *= 2)
    cost += 6 * fps_ntt_weight(m) + 36 * fps_ntt_weight(2 * m);
  return cost;
}
inline ll fps_sqrt_weight(int n, bool native)
{
  if (native && n >= 512)
  {
    int N = bit_ceil(n), s = min(16, N), m = N / s, r = (n + m - 1) / m;
    return (4 * r - 3) * fps_ntt_weight(2 * m) + fps_sqrt_weight(m, true) + fps_inv_weight(m, true);
  }
  ll cost = 0;
  for (int m = 1; m < n; m *= 2)
    cost += (native ? 5 : 15) * fps_ntt_weight(2 * m) + fps_inv_weight(m, native);
  return cost;
}
template <class F> bool fps_use_sparse(const F &f, int n, FPSSparseOperation op, int offset = 0)
{
  using mint = typename F::value_type;
  if (n <= 64)
    return true;
  bool native = ntt_ok<mint>(bit_ceil(n));
  ll inverse = fps_inv_weight(n, native), cost = inverse;
  int scale = 14, repetitions = 1;
  if (op == FPSSparseOperation::div || op == FPSSparseOperation::pow)
    cost += (ntt_ok<mint>(2 * n - 1) ? 3 : 9) * fps_ntt_weight(bit_ceil(2 * n - 1));
  if (op == FPSSparseOperation::exp)
    cost = fps_exp_weight(n, native), scale = 18;
  if (op == FPSSparseOperation::pow)
    cost += fps_exp_weight(n, native), repetitions = 2;
  if (op == FPSSparseOperation::sqrt)
    cost = fps_sqrt_weight(n, native), scale = 17, repetitions = 2;
  if (!native)
    scale = 25;
  if constexpr (!internal::ordinary_mod32<mint>::value)
    scale = max(1, scale / 2);
  ll budget = cost * scale / (10 * repetitions), work = 0;
  for (int j = 1; j < n && offset + j < f.sz(); ++j)
    if (f[offset + j] != 0)
    {
      work += n - j;
      if (work > budget)
        return false;
    }
  return true;
}

// For f=c+O(x^d), 3d>=n leaves at most the quadratic term of a scalar Taylor expansion.
template <class F>
optional<F> fps_tail_power(const F &f, int n, typename F::value_type constant, typename F::value_type linear,
                           typename F::value_type quadratic)
{
  using mint = typename F::value_type;
  int d = 1;
  while (d < min(n, f.sz()) && f[d] == 0)
    ++d;
  if (d < min(n, f.sz()) && ll(d) * 3 < n)
    return nullopt;
  F result = f.resized(n) * linear;
  if (n)
    result[0] = constant;
  if (ll(d) * 2 < n && d < f.sz())
  {
    int need = n - 2 * d;
    F h(need);
    repi(i, min(need, f.sz() - d)) h[i] = f[d + i];
    F square = (h * h).resized(need) * quadratic;
    repi(i, need) result[2 * d + i] += square[i];
  }
  return result;
}

template <class F> F fps_inv_newton(const F &input, int n)
{
  using mint = typename F::value_type;
  if (n == 0)
    return {};
  F f, g2, g{input.get(0).inv()};
  for (int m = 1; m < n; m *= 2)
  {
    if (ntt_ok<mint>(2 * m))
    {
      f = input.resized(2 * m), g2 = F(g);
      ntt(f);
      g2.resize(2 * m), ntt(g2);
      repi(i, 2 * m) f[i] *= g2[i];
      intt(f);
      f >>= m;
      f.resize(2 * m), ntt(f);
      repi(i, 2 * m) f[i] *= g2[i];
      intt(f);
      mint iz = mint(2 * m).inv();
      iz *= -iz;
      repi(i, m) f[i] *= iz;
      g.insert(g.end(), f.begin(), f.begin() + m);
    }
    else
      g = (g * mint(2) - g * g * input.resized(2 * m)).resized(2 * m);
  }
  return g.resized(n);
}

template <class F> F fps_block_product(const vc<F> &a, const vc<F> &b, int k, int m, bool delayed = false)
{
  using mint = typename F::value_type;
  F freq(2 * m);
  bool square = &a == &b;
  if constexpr (internal::ordinary_mod32<mint>::value)
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
      for (int carry : {0, 1})
        for (int i = max(0, k - carry - int(b.size()) + 1); i <= k - carry && i < int(a.size()); ++i)
          if (!a[i].empty() && !b[k - carry - i].empty())
          {
            int other = k - carry - i;
            if (square && i > other)
              continue;
            int times = square && i < other ? 2 : 1;
            if (pending + times > batch)
              flush();
            repi(j, 2 * m)
            {
              ull v = ull(a[i][j].val()) * b[other][j].val() * times;
              if (carry && j >= m)
                negative[j] += v;
              else
                positive[j] += v;
            }
            pending += times;
            if (pending == batch)
              flush();
          }
      if (pending)
        flush();
      intt(freq);
      freq.resize(m);
      mint iz = mint(2 * m).inv();
      for (auto &v : freq)
        v *= iz;
      return freq;
    }
  for (int i = max(0, k - int(b.size()) + 1); i <= k && i < int(a.size()); ++i)
    if (!a[i].empty() && !b[k - i].empty())
      repi(j, 2 * m) freq[j] += a[i][j] * b[k - i][j];
  for (int i = max(0, k - int(b.size())); i < k && i < int(a.size()); ++i)
    if (!a[i].empty() && !b[k - 1 - i].empty())
      repi(j, 2 * m)
      {
        mint v = a[i][j] * b[k - 1 - i][j];
        if (j < m)
          freq[j] += v;
        else
          freq[j] -= v;
      }
  intt(freq);
  freq.resize(m);
  mint iz = mint(2 * m).inv();
  for (auto &v : freq)
    v *= iz;
  return freq;
}

template <class F> F fps_exp_bs(const F &h, int n, bool split_last = true)
{
  using mint = typename F::value_type;
  assert(n >= 0 && h.get(0) == 0);
  if (n == 0)
    return {};
  assert(ntt_ok<mint>(bit_ceil(n)));
  Binomial<mint>::reserve(bit_ceil(n) - 1);
  if (n == 1)
    return {1};
  F f{1, h.get(1)}, g{1}, g_fft{1, 1};
  for (int m = 2; m < n; m *= 2)
  {
    mint im = mint(m).inv(), i2m = im * Binomial<mint>::inv(2);
    F f_fft = f.resized(2 * m);
    ntt(f_fft);
    F old_g_fft = g_fft;
    {
      F error(m);
      repi(i, m) error[i] = f_fft[i] * old_g_fft[i];
      intt(error);
      repi(i, m / 2) error[i] = error[i + m / 2] * im;
      fill(error.begin() + m / 2, error.end(), mint(0));
      ntt(error);
      repi(i, m) error[i] *= old_g_fft[i];
      intt(error);
      repi(i, m / 2) g.push_back(-error[i] * im);
    }
    F r(m);
    repi(i, 1, min(m, h.sz())) r[i - 1] = mint(i) * h[i];
    ntt(r);
    repi(i, m) r[i] *= f_fft[i];
    intt(r);
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
      ntt(g1), ntt(s0), ntt(s1);
      F low(m), cross(m);
      repi(i, m)
      {
        low[i] = old_g_fft[i] * s0[i];
        cross[i] = old_g_fft[i] * s1[i] + g1[i] * s0[i];
      }
      intt(low), intt(cross);
      t.resize(m);
      repi(i, m) t[i] = low[i] * im;
      repi(i, m / 2) t[m / 2 + i] += cross[i] * im;
    }
    else
    {
      g_fft = g.resized(2 * m);
      ntt(g_fft);
      t = s.resized(2 * m);
      ntt(t);
      repi(i, 2 * m) t[i] *= g_fft[i];
      intt(t);
      t.resize(m);
      for (auto &v : t)
        v *= i2m;
    }
    F u(2 * m);
    repi(i, m) u[i] = h.get(m + i) - t[i] * Binomial<mint>::inv_[m + i];
    ntt(u);
    repi(i, 2 * m) u[i] *= f_fft[i];
    intt(u);
    repi(i, m) f.push_back(u[i] * i2m);
  }
  f.resize(n);
  return f;
}

template <class F> F fps_inv_block(const F &f, int n, int blocks, bool partial = true)
{
  using mint = typename F::value_type;
  assert(n >= 0 && f.get(0) != 0);
  if (!n)
    return {};
  if (n == 1)
    return {f[0].inv()};
  int N = bit_ceil(n), s = min(blocks, max(1, N / 2)), m = N / (2 * s);
  int r = partial ? (n + m - 1) / m : 2 * s, h = r - s;
  vc<F> ff(r), g(s), gf(s), df(h);
  g[0] = f.resized(m).inv(m);
  gf[0] = g[0].resized(2 * m);
  ntt(gf[0]);
  repi(k, r)
  {
    ff[k].resize(2 * m);
    repi(i, m) ff[k][i] = f.get(k * m + i);
    ntt(ff[k]);
  }
  repi(k, 1, s)
  {
    F v = fps_block_product(ff, gf, k, m, true).resized(2 * m);
    ntt(v);
    repi(i, 2 * m) v[i] *= gf[0][i];
    intt(v);
    v.resize(m);
    mint iz = -mint(2 * m).inv();
    for (auto &x : v)
      x *= iz;
    g[k] = std::move(v);
    gf[k] = g[k].resized(2 * m);
    ntt(gf[k]);
  }
  repi(k, h)
  {
    df[k] = -fps_block_product(ff, gf, s + k, m, true);
    df[k].resize(2 * m);
    ntt(df[k]);
  }
  F result(r * m);
  repi(k, s) copy(g[k].begin(), g[k].end(), result.begin() + k * m);
  repi(k, h)
  {
    F v = fps_block_product(df, gf, k, m, true);
    copy(v.begin(), v.end(), result.begin() + (s + k) * m);
  }
  result.resize(n);
  return result;
}

template <class F> F fps_sqrt_newton(const F &f, int n)
{
  using mint = typename F::value_type;
  if (n == 0)
    return {};
  F g{1};
  mint i2 = mint(2).inv();
  for (int m = 1; m < n; m *= 2)
  {
    F error = (f.resized(2 * m) - (g * g).resized(2 * m)) >> m;
    F correction = (error * g.inv(m)).resized(m) * i2;
    g.insert(g.end(), correction.begin(), correction.end());
  }
  return g.resized(n);
}

template <class F> F fps_sqrt_block(const F &f, int n, int blocks, bool partial = true)
{
  using mint = typename F::value_type;
  assert(n >= 0 && f.get(0) == 1);
  if (!n)
    return {};
  int N = bit_ceil(n), s = min(blocks, N), m = N / s, r = partial ? (n + m - 1) / m : s;
  F g = fps_sqrt_newton(f.resized(m), m);
  if (r == 1)
    return g.resized(n);
  F u = (g.inv(m) * mint(2).inv()).resized(2 * m);
  ntt(u);
  vc<F> gf(r);
  F result(r * m);
  copy(g.begin(), g.end(), result.begin());
  mint iz = mint(2 * m).inv();
  repi(k, 1, r)
  {
    gf[k - 1] = g.resized(2 * m);
    ntt(gf[k - 1]);
    F v = fps_block_product(gf, gf, k, m, true);
    repi(i, m) v[i] = f.get(k * m + i) - v[i];
    v.resize(2 * m);
    ntt(v);
    repi(i, 2 * m) v[i] *= u[i];
    intt(v);
    v.resize(m);
    for (auto &x : v)
      x *= iz;
    g = std::move(v);
    copy(g.begin(), g.end(), result.begin() + k * m);
  }
  result.resize(n);
  return result;
}

template <class F> F fps_exp_block(const F &f, int n, int blocks, bool delayed = true)
{
  using mint = typename F::value_type;
  assert(n >= 0 && f.get(0) == 0);
  if (n == 0)
    return {};
  int N = bit_ceil(n), s = min(blocks, max(1, N / 2)), m = N / (2 * s);
  if (N == 1)
    return {1};
  assert(has_single_bit(unsigned(s)) && ntt_ok<mint>(2 * m));
  Binomial<mint>::reserve(N - 1);
  int r = (n + m - 1) / m, high = r - s;
  vc<F> g(s), gf(s), q(r), qf(r), ef(high);
  g[0] = fps_exp_bs(f.resized(m), m);
  F u = g[0].inv(m).resized(2 * m);
  ntt(u);
  gf[0] = g[0].resized(2 * m);
  ntt(gf[0]);
  repi(k, s)
  {
    q[k].resize(m);
    repi(i, m) q[k][i] = mint(k * m + i) * f.get(k * m + i);
    qf[k] = q[k].resized(2 * m);
    ntt(qf[k]);
  }
  repi(k, 1, s)
  {
    F phi = fps_block_product(gf, qf, k, m, delayed).resized(2 * m);
    ntt(phi);
    repi(i, 2 * m) phi[i] *= u[i];
    intt(phi);
    phi.resize(m);
    mint iz = mint(2 * m).inv();
    repi(i, m) phi[i] *= iz * Binomial<mint>::inv_[k * m + i];
    phi.resize(2 * m);
    ntt(phi);
    repi(i, 2 * m) phi[i] *= gf[0][i];
    intt(phi);
    phi.resize(m);
    for (auto &v : phi)
      v *= iz;
    g[k] = std::move(phi);
    gf[k] = g[k].resized(2 * m);
    ntt(gf[k]);
  }
  repi(k, s, r)
  {
    F value = fps_block_product(qf, gf, k, m, delayed).resized(2 * m);
    ntt(value);
    repi(i, 2 * m) value[i] *= u[i];
    intt(value);
    value.resize(m);
    mint iz = -mint(2 * m).inv();
    for (auto &v : value)
      v *= iz;
    q[k] = std::move(value);
    qf[k] = q[k].resized(2 * m);
    ntt(qf[k]);
  }
  repi(k, high)
  {
    ef[k].resize(2 * m);
    repi(i, m) ef[k][i] = q[s + k][i] * Binomial<mint>::inv_[(s + k) * m + i] - f.get((s + k) * m + i);
    ntt(ef[k]);
  }
  F result(r * m);
  repi(k, s)
  {
    copy(g[k].begin(), g[k].end(), result.begin() + k * m);
  }
  repi(k, high)
  {
    F value = fps_block_product(gf, ef, k, m, delayed);
    repi(i, m) result[(s + k) * m + i] = -value[i];
  }
  result.resize(n);
  return result;
}

} // namespace internal
