#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/linalg/matrix.hpp"
#include "math/fps/bmbm.hpp"

using mint = modint998244353;
using F = FieldAddSubMulDiv<mint>;
using V = vc<mint>;
using M = Matrix<F>;

// Exercise the generic field path with a type that has no modint operations.
struct F3
{
  using S = int;
  static S add(S a, S b) { return (a + b) % 3; }
  static S mul(S a, S b) { return a * b % 3; }
  static S minus(S a) { return (3 - a) % 3; }
  static S inv(S a) { assert(a); return a; }
  static S e0() { return 0; }
  static S e1() { return 1; }
};

template <class S>
void test_dot()
{
  using K = FieldAddSubMulDiv<S>;
  for (int n : {0, 1, 3, 4, 5, 15, 16, 17, 31, 32, 33, 129, 1001})
  {
    vc<S> a(n, S::mod() - 1), b = a;
    repi(mode, 2)
    {
      if (mode)
        repi(i, n) a[i] = randrange<ll>(0, S::mod()), b[i] = randrange<ll>(0, S::mod());
      S expected = 0, reversed_expected = 0;
      repi(i, n) expected += a[i] * b[i], reversed_expected += a[i] * b[n - 1 - i];
      assert(dot_product<K>(n, a.begin(), b.begin()) == expected);
      assert(dot_product<K>(n, a.begin(), b.rbegin()) == reversed_expected);
    }
  }
}

void test_bm()
{
  // Independent exhaustive oracle: try every GF(2) recurrence, including
  // finite prefixes whose shortest recurrence has degree greater than n/2.
  using S = static_modint32<2>;
  using K = FieldAddSubMulDiv<S>;
  for (int n = 0; n <= 10; n++)
    for (int mask = 0; mask < (1 << n); mask++)
    {
      vc<S> a(n);
      repi(i, n) a[i] = (mask >> i) & 1;
      auto c = berlekamp_massey<K>(a);
      int want = 0;
      for (; want <= n; want++)
      {
        bool found = false;
        repi(coeff, 1 << want)
        {
          bool ok = true;
          for (int i = want; i < n; i++)
          {
            int x = 0;
            repi(j, want) x ^= ((coeff >> j) & 1) & ((mask >> (i - 1 - j)) & 1);
            if (x != ((mask >> i) & 1)) { ok = false; break; }
          }
          if (ok) { found = true; break; }
        }
        if (found) break;
      }
      assert(int(c.size()) == want + 1 && c[0] == S(-1));
      for (int i = want; i < n; i++)
      {
        S x = 0;
        repi(j, 1, want + 1) x += c[j] * a[i - j];
        assert(x == a[i]);
      }
      BerlekampMassey<K> online;
      repi(i, n)
      {
        online.append(a[i]);
        assert(online.coefficients() == berlekamp_massey<K>(vc<S>(a.begin(), a.begin() + i + 1)));
      }
    }
  repi(t, 100)
  {
    int d = randint(1, 30);
    V c(d), a(4 * d);
    repi(i, d) c[i] = randrange(0, mint::mod()), a[i] = randrange(0, mint::mod());
    repi(i, d, SZ(a)) repi(j, d) a[i] += c[j] * a[i - 1 - j];
    auto got = berlekamp_massey<F>(V(a.begin(), a.begin() + 2 * d));
    repi(i, 2 * d, SZ(a))
    {
      mint x = 0;
      repi(j, 1, SZ(got)) x += got[j] * a[i - j];
      assert(x == a[i]);
    }
  }
}

void check_matrix(const M &a)
{
  const int n = a.size();
  int calls = 0;
  auto map = [&](V &v) { calls++; v = a * v; };
  const mint expected_det = get<2>(a.row_reduction());
  assert(bbla::det<F>(n, map) == expected_det);
  assert(a.det_sparse() == expected_det);
  assert(a.det_sparse(mint(1)) == expected_det);
  calls = 0;
  auto m = bbla::minimal_polynomial<F>(n, map);
  assert(calls == max(0, 2 * n - 1));
  assert(m.back() == mint(-1) && int(m.size()) <= n + 1);
  repi(j, n)
  {
    V v(n); v[j] = 1;
    assert(bbla::apply_polynomial<F>(map, v, m) == V(n));
  }
  V u(n), v(n);
  repi(j, n) u[j] = randrange(0, mint::mod()), v[j] = randrange(0, mint::mod());
  auto mv = bbla::minimal_polynomial_vector<F>(map, v);
  assert(bbla::apply_polynomial<F>(map, v, mv) == V(n));
  if (mv[0] == mint(0))
  {
    auto kernel = bbla::apply_polynomial<F>(map, v, V(mv.begin() + 1, mv.end()));
    assert(kernel != V(n));
    assert(a * kernel == V(n));
  }
  for (int count : {0, 1, 2, 2 * n + 3})
  {
    calls = 0;
    auto seq = bbla::krylov_sequence<F>(map, u, v, count);
    assert(calls == max(0, count - 1));
    auto w = v;
    repi(i, count)
    {
      mint x = 0;
      repi(j, n) x += u[j] * w[j];
      assert(seq[i] == x);
      w = a * w;
    }
  }
  V p{2, 0, 3, 5, 0, 0}, w = v, expected(n);
  for (auto coef : p)
  {
    repi(j, n) expected[j] += coef * w[j];
    w = a * w;
  }
  assert(bbla::apply_polynomial<F>(map, v, p) == expected);
  assert(bbla::apply_polynomial<F>(map, v, {}) == V(n));
  if (expected_det != mint(0))
  {
    auto x = bbla::solve<F>(map, v);
    assert(a * x == v);
  }
  calls = 0;
  assert(bbla::solve<F>(map, V(n)) == V(n) && calls == 0);
}

void test_solve_checks()
{
  M a{{1, 0}, {0, 2}};
  auto map = [&](V &v) { v = a * v; };
  int samples = 0;
  auto sample = [&]() { return samples++ < 2 ? mint(0) : mint(1); };
  auto x = bbla::solve<F>(map, V{1, 1}, sample);
  assert(a * x == V({1, 1}) && samples == 4);

  // A long initial run of zero observations must not trigger a wrong early exit.
  const int n = 128;
  V b(n); b.back() = 1;
  int calls = 0;
  auto jordan = [&](V &v) { calls++; repi(i, n - 1) v[i] += v[i + 1]; };
  samples = 0;
  auto coordinate = [&]() { return mint(samples++ % n == 0); };
  x = bbla::solve<F>(jordan, b, coordinate);
  jordan(x);
  assert(x == b && calls > 2 * n);

  calls = 0;
  auto identity = [&](V &) { calls++; };
  assert(bbla::solve<F>(identity, V(1000, 1)) == V(1000, 1));
  assert(calls <= 32);

  // A sampler may include zero; determinant preconditioning must reject it.
  samples = 0;
  auto with_zero = [&]() { return samples++ < 2 ? mint(0) : mint(randrange(1, mint::mod())); };
  assert(bbla::det<F>(2, map, with_zero) == mint(2));
}

void test_extensions()
{
  // The documented A^k v and u^T A^k v recipes, including the zero sequence.
  M a{{1, 1}, {1, 0}};
  V v{1, 0}, u{1, 2};
  auto map = [&](V &x) { x = a * x; };
  auto m = bbla::minimal_polynomial_vector<F>(map, v);
  using FPS = FormalPowerSeries<mint>;
  for (ll k : {0LL, 1LL, 2LL, 3LL, 100LL, 1'000'000'000'000'000'000LL})
  {
    V expected = a.pow(k) * v;
    auto p = FPS{0, 1}.pow_mod(k, FPS(m));
    assert(bbla::apply_polynomial<F>(map, v, p) == expected);
    auto seq = bbla::krylov_sequence<F>(map, u, v, 4);
    assert(bmbm(seq, k, false) == u[0] * expected[0] + u[1] * expected[1]);
    assert(bmbm(V(4), k, false) == mint(0));
  }
  auto generic_map = [](vc<int> &v) { v[0] = v[0] * 2 % 3; };
  auto sample = []() { return 1; };
  assert(bbla::solve<F3>(generic_map, vc<int>{1}, sample) == vc<int>{2});
  assert(bbla::det<F3>(1, generic_map, sample) == 2);
  vc<int> s{1, 2, 1, 2, 1, 2};
  assert((berlekamp_massey<F3>(s) == vc<int>{2, 2}));
}

int main()
{
  mt.seed(20260908);
  test_dot<modint998244353>();
  test_dot<modint1000000007>();
  test_dot<static_modint32<2147483647>>();
  test_dot<modint61>();
  for (int mod : {2, 998244353, 1073741824, 1073741825, 2147483647})
  {
    dynamic_modint32<42>::set_mod(mod);
    test_dot<dynamic_modint32<42>>();
  }
  dynamic_modint64_odd<42>::set_mod((1LL << 61) - 1);
  test_dot<dynamic_modint64_odd<42>>();
  test_bm();
  for (int n = 0; n <= 10; n++)
  {
    check_matrix(M(n, n));
    check_matrix(M(n, n, 1));
    check_matrix(M(n, n, 7));
    M nilpotent(n, n), jordan(n, n, 1);
    repi(i, n - 1) nilpotent[i][i + 1] = jordan[i][i + 1] = 1;
    check_matrix(nilpotent);
    check_matrix(jordan);
    repi(t, 40)
    {
      M a(n, n);
      repi(i, n) repi(j, n) a[i][j] = randint(0, 3);
      if (n > 1 && t % 2) a.back() = a.front();
      check_matrix(a);
    }
  }
  test_solve_checks();
  test_extensions();
  PRINT("Hello World");
}
