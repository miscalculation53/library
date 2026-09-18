// g++-15 -std=c++20 -O2 -I. benchmark/bbla.cpp -o /tmp/bbla-bench
// /tmp/bbla-bench [dimension=2000] [repetitions=5]
#include "math/linalg/matrix.hpp"

// Pre-change implementations from 8943a007, retained for reproducible comparisons.
namespace before
{
template <class F>
vc<typename F::S> berlekamp_massey(const vc<typename F::S> &a)
{
  using S = typename F::S;
  const int n = a.size();
  vc<S> b, c;
  int pos = -1;
  S x = F::e0();
  repi(i, n)
  {
    const int d = c.size();
    S y = a[i];
    repi(j, d) y = F::add(y, F::minus(F::mul(c[j], a[i - 1 - j])));
    if (y == F::e0())
      continue;
    
    if (c.empty())
    {
      c.assign(i + 1, F::e0());
      pos = i;
      x = y;
      continue;
    }

    S z = F::mul(y, F::inv(x));
    int d2 = i - pos + b.size();
    vc<S> tmp;
    if (d2 >= d)
    {
      tmp = c;
      c.resize(d2, F::e0());
    }
    c[i - 1 - pos] = F::add(c[i - 1 - pos], z);
    repi(j, b.size()) c[i - pos + j] = F::add(c[i - pos + j], F::minus(F::mul(z, b[j])));
    if (d2 >= d)
      pos = i, x = y, swap(tmp, b);
  }
  c.insert(c.begin(), F::minus(F::e1()));
  return c;
}

template <class mint>
mint random_sample_mint() { return randrange(1, mint::mod()); }

// 線型写像 linear_map: S^n → S^n の最小多項式を求める
// linear_map は void(vc<S>) として渡す (破壊的に変更する関数)
// 時間計算量: O(n^2 + nt), t は linear_map の計算量
template <class F, class LinearMap, class RandomSample = decltype(random_sample_mint<typename F::S>)>
vc<typename F::S> minimal_polynomial(int n, const LinearMap &linear_map, const RandomSample &random_sample = random_sample_mint)
{
  using S = typename F::S;
  assert(n > 0);
  vc<S> u(n, F::e0()), v(n, F::e0());
  repi(j, n) u[j] = random_sample(), v[j] = random_sample();
  vc<S> a(2 * n + 1, F::e0());
  repi(i, 2 * n + 1)
  {
    S sm = F::e0();
    repi(j, n) sm = F::add(sm, F::mul(u[j], v[j]));
    a[i] = sm;
    linear_map(v);
  }
  return reversed(berlekamp_massey<F>(a));
}

// 線型写像 linear_map: S^n → S^n とベクトル y: S^n を受け取り、
// 線型方程式 y = linear_map(x) を解く
// linear_map は void(vc<S>) として渡す (破壊的に変更する関数)
// 制約: linear_map は非特異
// 時間計算量: O(n^2 + nt), t は linear_map の計算量
template <class F, class LinearMap, class RandomSample = decltype(random_sample_mint<typename F::S>)>
vc<typename F::S> solve(const LinearMap &linear_map, vc<typename F::S> y, const RandomSample &random_sample = random_sample_mint)
{
  using S = typename F::S;
  const int n = y.size();
  if (n == 0)
    return {};
  auto m = minimal_polynomial<F>(n, linear_map, random_sample);
  assert(m[0] != F::e0());
  S minus_inv_m0 = F::minus(F::inv(m[0]));
  vc<S> res(n, F::e0());
  repi(i, 1, m.size())
  {
    S coef = F::mul(m[i], minus_inv_m0);
    repi(j, n)
    {
      auto &resj = res[j];
      resj = F::add(resj, F::mul(coef, y[j]));
    }
    linear_map(y);
  }
  return res;
}

// 線型写像 linear_map: S^n → S^n の表現行列の行列式を求める
// linear_map は void(vc<S>) として渡す (破壊的に変更する関数)
// 時間計算量: O(n^2 + nt), t は linear_map の計算量
template <class F, class LinearMap, class RandomSample = decltype(random_sample_mint<typename F::S>)>
typename F::S det(int n, const LinearMap &linear_map, const RandomSample &random_sample = random_sample_mint)
{
  using S = typename F::S;
  if (n == 0)
    return 1;
  while (true)
  {
    vc<S> d(n);
    repi(i, n) d[i] = random_sample();
    auto linear_map_ad = [&](vc<S> &v)
    {
      repi(i, n) v[i] = F::mul(v[i], d[i]);
      linear_map(v);
    };
    auto m = minimal_polynomial<F>(n, linear_map_ad, random_sample);
    if (m[0] == F::e0())
      return F::e0();
    if (SZ(m) != n + 1)
      continue;
    S detd = F::e1();
    fec(di : d) detd = F::mul(detd, di);
    S res = F::mul(m[0], F::inv(detd));
    return n & 1 ? res : F::minus(res);
  }
}


  template <class F, class RandomSample = decltype(random_sample_mint<typename F::S>)>
  typename F::S matrix_det_sparse(const Matrix<F> &a, const typename F::S &majority = F::e0(), const RandomSample &random_sample = random_sample_mint)
  {
    using S = typename F::S;
    auto [n, m] = a.template shape<int>();
    assert(n == m);
    vc<pair<int, pair<int, S>>> elms;
    const S minus_majority = F::minus(majority);
    repi(i, n) repi(j, n)
    {
      S val = a[i][j];
      if (val != majority)
        elms.eb(i, pair{j, F::add(val, minus_majority)});
    }
    CSR<pair<int, S>> csr(n, elms);
    auto linear_map = [&](vc<S> &x)
    {
      S sm = F::e0();
      fec(xi : x) sm = F::add(sm, xi);
      sm = F::mul(majority, sm);
      vc<S> y(n, sm);
      repi(i, n)
      {
        S yi = sm;
        fec([j, val] : csr[i]) yi = F::add(yi, F::mul(val, x[j]));
        y[i] = yi;
      }
      swap(x, y);
    };
    return det<F>(n, linear_map, random_sample);
  }

} // namespace before

using mint = modint998244353;
using F = FieldAddSubMulDiv<mint>;
using V = vc<mint>;
volatile ull bbla_sink = 0;
ull checksum(const V &v)
{
  ull h = v.size();
  for (auto x : v) h = h * 1000003 + x.val();
  return h;
}
template <class Old, class New>
void bench(const string &name, int n, int reps, Old old_run, New new_run)
{
  vc<double> old_times, new_times;
  auto measure = [&](auto run, vc<double> &times)
  {
    mt.seed(12345);
    auto start = chrono::steady_clock::now();
    ull result = run();
    times.eb(chrono::duration<double, milli>(chrono::steady_clock::now() - start).count());
    bbla_sink = bbla_sink ^ result;
    return result;
  };
  for (int r = -1; r < reps; r++)
  {
    ull a, b;
    if (r & 1) { a = measure(old_run, old_times); b = measure(new_run, new_times); }
    else { b = measure(new_run, new_times); a = measure(old_run, old_times); }
    assert(a == b);
    if (r == -1) old_times.clear(), new_times.clear();
  }
  sort(old_times.begin(), old_times.end());
  sort(new_times.begin(), new_times.end());
  double a = old_times[reps / 2], b = new_times[reps / 2];
  cout << name << ',' << n << ',' << a << ',' << b << ',' << a / b << '\n';
}
int main(int argc, char **argv)
{
  const int n = argc > 1 ? atoi(argv[1]) : 2000;
  const int reps = argc > 2 ? atoi(argv[2]) : 5;
  assert(n > 0 && reps > 0);
  cout << fixed << setprecision(3) << "case,n,before_ms,after_ms,speedup\n";
  mt.seed(6789);
  V a(2 * n), diag(n), upper(n), b(n);
  for (auto &x : a) x = randrange(1, mint::mod());
  repi(i, n)
  {
    diag[i] = randrange(1, mint::mod());
    upper[i] = randrange(1, mint::mod());
    b[i] = randrange(1, mint::mod());
  }
  bench("BM_random", n, reps,
        [&] { return checksum(before::berlekamp_massey<F>(a)); },
        [&] { return checksum(berlekamp_massey<F>(a)); });
  auto bidiagonal = [&](V &v)
  {
    repi(i, n) v[i] = diag[i] * v[i] + (i + 1 < n ? upper[i] * v[i + 1] : mint(0));
  };
  bench("minpoly_bidiagonal", n, reps,
        [&] { return checksum(before::minimal_polynomial<F>(n, bidiagonal)); },
        [&] { return checksum(bbla::minimal_polynomial<F>(n, bidiagonal)); });
  bench("det_bidiagonal", n, reps,
        [&] { return before::det<F>(n, bidiagonal).val(); },
        [&] { return bbla::det<F>(n, bidiagonal).val(); });
  bench("solve_bidiagonal", n, reps,
        [&] { return checksum(before::solve<F>(bidiagonal, b)); },
        [&] { return checksum(bbla::solve<F>(bidiagonal, b)); });
  auto identity = [](V &) {};
  bench("minpoly_identity", n, reps,
        [&] { return checksum(before::minimal_polynomial<F>(n, identity)); },
        [&] { return checksum(bbla::minimal_polynomial<F>(n, identity)); });
  bench("solve_identity", n, reps,
        [&] { return checksum(before::solve<F>(identity, b)); },
        [&] { return checksum(bbla::solve<F>(identity, b)); });
  auto few_eigenvalues = [&](V &v) { repi(i, n) v[i] *= mint(i % 32 + 1); };
  bench("solve_32_eigenvalues", n, reps,
        [&] { return checksum(before::solve<F>(few_eigenvalues, b)); },
        [&] { return checksum(bbla::solve<F>(few_eigenvalues, b)); });
  mt.seed(23456);
  vc<pair<int, pair<int, mint>>> entries;
  repi(i, n) repi(j, 4) entries.eb(i, pair{int(randrange(0, n)), mint(randrange(1, mint::mod()))});
  CSR<pair<int, mint>> csr(n, entries);
  V buffer(n);
  auto sparse = [&](V &v)
  {
    repi(i, n)
    {
      mint sum = 0;
      for (auto [j, x] : csr[i]) sum += x * v[j];
      buffer[i] = sum;
    }
    v.swap(buffer);
  };
  bench("minpoly_sparse_4n", n, reps,
        [&] { return checksum(before::minimal_polynomial<F>(n, sparse)); },
        [&] { return checksum(bbla::minimal_polynomial<F>(n, sparse)); });
  bench("det_sparse_4n", n, reps,
        [&] { return before::det<F>(n, sparse).val(); },
        [&] { return bbla::det<F>(n, sparse).val(); });
  Matrix<F> mat(n, n);
  repi(i, n)
  {
    mat[i][i] = diag[i];
    if (i + 1 < n) mat[i][i + 1] = upper[i];
  }
  bench("Matrix_det_sparse", n, reps,
        [&] { return before::matrix_det_sparse<F>(mat).val(); },
        [&] { return mat.det_sparse().val(); });
}
