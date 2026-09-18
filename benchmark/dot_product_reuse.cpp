// g++-15 -std=c++20 -O2 -I. benchmark/dot_product_reuse.cpp -o /tmp/dot-reuse-bench
#include "math/linalg/matrix.hpp"
#include "math/linalg/matrix_array.hpp"
#include "math/convolution/convolution.hpp"
#include "algebra/polynomial_ring.hpp"
#include "math/set/subset_convolution.hpp"
#include "math/set/kronecker_power.hpp"

using mint = modint998244353;
using R = RingAddSubMul<mint>;
using V = vc<mint>;
using M = Matrix<R>;

// Before this change (2026-09-10); retained for reproducible comparisons.
namespace before
{
V matrix_vector(const M &a, const V &v)
{
  auto [n, m] = a.shape<int>();
  V res(n);
  repi(i, n)
  {
    mint sum = 0;
    repi(j, m) sum += a[i][j] * v[j];
    res[i] = sum;
  }
  return res;
}
M matrix_product(const M &a, const M &b)
{
  auto [n, m] = a.shape<int>();
  const int p = b.shape<int>().second;
  M res(n, p);
  constexpr int BS = 32;
  repi(ii, 0, n, BS) repi(kk, 0, m, BS) repi(jj, 0, p, BS)
    repi(i, ii, min(ii + BS, n)) repi(k, kk, min(kk + BS, m))
    {
      mint aik = a[i][k];
      if (aik == mint(0)) continue;
      repi(j, jj, min(jj + BS, p)) res[i][j] += aik * b[k][j];
    }
  return res;
}
mint convolution_point(const V &a, const V &b, int p)
{
  int n = a.size(), m = b.size();
  mint res = 0;
  repi(i, max(0, p - m + 1), min(n, p + 1)) res += a[i] * b[p - i];
  return res;
}
V convolution_naive(const V &a, const V &b)
{
  int n = a.size(), m = b.size();
  const int cnta = n - count(ALL(a), 0), cntb = m - count(ALL(b), 0);
  V c(n + m - 1);
  if (ll(m) * cnta > ll(n) * cntb)
  {
    repi(j, m)
    {
      if (b[j] == 0) continue;
      repi(i, n) c[i + j] += a[i] * b[j];
    }
  }
  else
  {
    repi(i, n)
    {
      if (a[i] == 0) continue;
      repi(j, m) c[i + j] += a[i] * b[j];
    }
  }
  return c;
}
V polynomial_product(const V &a, const V &b)
{
  if (a.empty() || b.empty()) return {};
  V c(a.size() + b.size() - 1);
  repi(i, a.size()) repi(j, b.size()) c[i + j] += a[i] * b[j];
  return c;
}
template <int k>
V kronecker(const array<array<mint, k>, k> &a, V v)
{
  auto map = [&](const array<mint, k> &x)
  {
    array<mint, k> y{};
    repi(i, k) repi(j, k) y[i] += a[i][j] * x[j];
    return y;
  };
  tensor_power_array_destructive<k>(map, v);
  return v;
}
} // namespace before

namespace after
{
V matrix_vector(const M &a, const V &v) { return a * v; }
M matrix_product(const M &a, const M &b) { return a * b; }
mint convolution_point(const V &a, const V &b, int p) { return convolution_point_get(a, b, p); }
V convolution_naive(const V &a, const V &b) { return internal::convolution_naive(a, b); }
template <int k>
V kronecker(const array<array<mint, k>, k> &a, const V &v)
{ return kronecker_power_array<R, k>(a, v); }
} // namespace after

namespace before
{
template <class R>
vc<typename R::S> subset_convolution
(const vc<typename R::S> &a, const vc<typename R::S> &b)
{
  // using P = PolynomialRingArray<R, 30>;
  using F = array<typename R::S, 30>;
  assert(a.size() == b.size());
  const int len = a.size();
  if (len == 0)
    return {};
  assert(has_single_bit(len));
  const int lg = countr_zero(len);
  static vc<int> pc{0};
  if (int i = pc.size(); i <= len)
  {
    pc.resize(len);
    for (; i < len; i++)
      pc[i] = pc[i - (i & -i)] + 1;
  }
  vc<F> fa(len), fb(len);
  repi(i, len)
  {
    fill(ALL(fa[i]), R::e0()), fill(ALL(fb[i]), R::e0());
    fa[i][pc[i]] = a[i], fb[i][pc[i]] = b[i];
  }
  // or_convolution_destructive<P>(fa, fb);
  auto zeta = [&](vc<F> &f) -> void
  {
    for (int w = 1; w < len; w <<= 1)
    {
      repi(k, 0, len, w * 2) repi(i, w)
      {
        int s = k + i, t = s + w;
        repi(j, pc[t]) f[t][j] = R::add(f[t][j], f[s][j]);
      }
    }
  };
  zeta(fa), zeta(fb);
  repi(s, len)
  {
    F fc;
    fill(ALL(fc), R::e0());
    repi(i, lg + 1) repi(j, lg + 1 - i) fc[i + j] = R::add(fc[i + j], R::mul(fa[s][i], fb[s][j]));
    swap(fa[s], fc);
  }
  for (int w = len >> 1; w; w >>= 1)
  {
    repi(k, 0, len, w * 2) repi(i, w)
    {
      int s = k + i, t = s + w;
      repi(j, pc[t], lg + 1) fa[t][j] = R::add(fa[t][j], R::minus(fa[s][j]));
    }
  }
  vc<typename R::S> c(len);
  repi(i, len) c[i] = fa[i][pc[i]];
  return c;
}
}

ull digest(mint x) { return x.val(); }
template <class C> ull digest(const C &c)
{
  ull value = 0;
  for (const auto &x : c) value = (value ^ digest(x)) * 1000000007ULL;
  return value;
}
volatile ull sink = 0;
template <class Old, class New>
void measure(string name, int iterations, const Old &old, const New &now)
{
  assert(old() == now());
  vc<double> times[2];
  repi(round, -1, 5)
  {
    repi(order, 2)
    {
      int which = order ^ (round & 1);
      auto start = chrono::steady_clock::now();
      repi(i, iterations)
      {
        asm volatile("" ::: "memory");
        if (which) sink = digest(now());
        else sink = digest(old());
      }
      double us = chrono::duration<double, micro>(chrono::steady_clock::now() - start).count() / iterations;
      if (round >= 0) times[which].eb(us);
    }
  }
  for (auto &v : times) sort(v.begin(), v.end());
  cout << name << ',' << times[0][2] << ',' << times[1][2] << ',' << times[0][2] / times[1][2] << '\n';
}
V random_vector(int n, int percent = 100)
{
  V v(n);
  for (auto &x : v) if (randrange(0, 100) < percent) x = randrange(1, mint::mod());
  return v;
}
template <int k>
void bench_kronecker(int len)
{
  array<array<mint, k>, k> a;
  for (auto &row : a) for (auto &x : row) x = randrange(1, mint::mod());
  auto v = random_vector(len);
  measure("kronecker_k" + to_string(k) + "_n" + to_string(len), 3,
      [&] { return before::kronecker<k>(a, v); }, [&] { return after::kronecker<k>(a, v); });
}
template <int n>
void bench_arrays()
{
  using A = MatrixArray<R, n, n>;
  A a, b;
  array<mint, n> v;
  for (auto &row : a) for (auto &x : row) x = randrange(1, mint::mod());
  for (auto &row : b) for (auto &x : row) x = randrange(1, mint::mod());
  for (auto &x : v) x = randrange(1, mint::mod());
  measure("matrix_array_vector_" + to_string(n), 100, [&]
  {
    array<mint, n> out{};
    repi(i, n)
    {
      mint sum = 0;
      repi(j, n) sum += a[i][j] * v[j];
      out[i] = sum;
    }
    return out;
  }, [&] { return a * v; });
  measure("matrix_array_product_" + to_string(n), 3, [&]
  {
    A out;
    constexpr int BS = 32;
    repi(ii, 0, n, BS) repi(kk, 0, n, BS) repi(jj, 0, n, BS)
      repi(i, ii, min(ii + BS, n)) repi(k, kk, min(kk + BS, n))
      {
        mint aik = a[i][k];
        if (aik == mint(0)) continue;
        repi(j, jj, min(jj + BS, n)) out[i][j] += aik * b[k][j];
      }
    return out;
  }, [&] { return a * b; });
  measure("polynomial_array_" + to_string(n), 100, [&]
  {
    array<mint, n> out{};
    repi(i, n) repi(j, n - i) out[i + j] += v[i] * a[0][j];
    return out;
  }, [&] { return PolynomialRingArray<R, n>::mul(v, a[0]); });
}
int main()
{
  mt.seed(20260910);
  cout << fixed << setprecision(3) << "case,before_us,after_us,speedup\n";
  for (int n : {8, 32, 128, 256})
  {
    M a(n, n), b(n, n);
    for (auto &row : a) row = random_vector(n);
    for (auto &row : b) row = random_vector(n);
    V v = random_vector(n);
    measure("matrix_vector_" + to_string(n), 100, [&] { return before::matrix_vector(a, v); },
        [&] { return after::matrix_vector(a, v); });
    measure("matrix_product_" + to_string(n), max(2, 1000000 / (n * n * n)), [&] { return before::matrix_product(a, b); },
        [&] { return after::matrix_product(a, b); });
  }
  for (int percent : {0, 1, 10, 25, 50})
  {
    int n = 128;
    M a(n, n), b(n, n);
    for (auto &row : a) row = random_vector(n, percent);
    for (auto &row : b) row = random_vector(n);
    measure("matrix_product_density" + to_string(percent), 2, [&] { return before::matrix_product(a, b); },
        [&] { return after::matrix_product(a, b); });
  }
  for (int n : {4, 16, 32, 60, 256})
  {
    V a = random_vector(n), b = random_vector(n);
    measure("convolution_point_" + to_string(n), 1000, [&] { return before::convolution_point(a, b, n - 1); },
        [&] { return after::convolution_point(a, b, n - 1); });
    measure("convolution_naive_" + to_string(n), 100, [&] { return before::convolution_naive(a, b); },
        [&] { return after::convolution_naive(a, b); });
    measure("polynomial_vector_" + to_string(n), 100, [&] { return before::polynomial_product(a, b); },
        [&] { return PolynomialRingVector<R>::mul(a, b); });
  }
  for (int percent : {1, 10, 50, 100})
  {
    V a = random_vector(60, percent), b = random_vector(4096, percent);
    measure("convolution_60x4096_density" + to_string(percent), 10, [&] { return before::convolution_naive(a, b); },
        [&] { return after::convolution_naive(a, b); });
  }
  for (int lg : {8, 12, 16})
  {
    V a = random_vector(1 << lg), b = random_vector(1 << lg);
    measure("subset_convolution_" + to_string(lg), 2, [&] { return before::subset_convolution<R>(a, b); },
        [&] { return subset_convolution<R>(a, b); });
  }
  bench_arrays<8>();
  bench_arrays<32>();
  bench_arrays<128>();
  bench_kronecker<2>(1 << 16);
  bench_kronecker<4>(1 << 16);
  bench_kronecker<8>(1 << 15);
  bench_kronecker<16>(1 << 16);
}
