#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/linalg/bbla_rank_solve.hpp"
#include "math/linalg/bbla_invariant_factors.hpp"
#ifndef BBLA_EXTENSIONS_CORE_ONLY
#include "math/linalg/matrix.hpp"
#endif

using mint = modint998244353;
using F = FieldAddSubMulDiv<mint>;
using V = vc<mint>;
#ifndef BBLA_EXTENSIONS_CORE_ONLY
using M = Matrix<F>;
#endif

// Independent small-matrix oracle: determinantal divisors of xI-A.
template <class S> using Poly = vc<S>;
template <class S> void trim(Poly<S> &p) { while (!p.empty() && p.back() == S(0)) p.pop_back(); }
template <class S> Poly<S> product(const Poly<S> &a, const Poly<S> &b)
{
  if (a.empty() || b.empty()) return {};
  Poly<S> c(a.size() + b.size() - 1);
  repi(i, a.size()) repi(j, b.size()) c[i + j] += a[i] * b[j];
  trim(c); return c;
}
template <class S> pair<Poly<S>, Poly<S>> divide(Poly<S> a, const Poly<S> &b)
{
  trim(a); assert(!b.empty());
  Poly<S> q(max(0, int(a.size()) - int(b.size()) + 1));
  while (a.size() >= b.size())
  {
    int k = a.size() - b.size();
    S c = a.back() / b.back(); q[k] = c;
    repi(j, b.size()) a[k + j] -= c * b[j];
    trim(a);
  }
  trim(q); return {q, a};
}
template <class S> Poly<S> gcd_poly(Poly<S> a, Poly<S> b)
{
  while (!b.empty()) { auto r = divide(a, b).second; a = b; b = r; }
  if (!a.empty()) { S c = a.back().inv(); for (auto &x : a) x *= c; }
  return a;
}
template <class S> Poly<S> minor_polynomial(const vvc<S> &a, const vc<int> &rows, const vc<int> &cols)
{
  int k = rows.size();
  vc<int> perm(k); iota(perm.begin(), perm.end(), 0);
  Poly<S> res(k + 1);
  do
  {
    int inversions = 0;
    repi(i, k) repi(j, i + 1, k) inversions += perm[i] > perm[j];
    Poly<S> p{S(inversions & 1 ? -1 : 1)};
    repi(i, k)
    {
      int r = rows[i], c = cols[perm[i]];
      Poly<S> term{-a[r][c]};
      if (r == c) term.push_back(S(1));
      p = product(p, term);
    }
    repi(i, p.size()) res[i] += p[i];
  } while (next_permutation(perm.begin(), perm.end()));
  trim(res); return res;
}
template <class S> vc<Poly<S>> oracle_factors(const vvc<S> &a)
{
  int n = a.size();
  Poly<S> previous{S(1)};
  vc<Poly<S>> result;
  repi(k, 1, n + 1)
  {
    Poly<S> divisor;
    repi(rmask, 1 << n) if (__builtin_popcount(uint(rmask)) == k)
      repi(cmask, 1 << n) if (__builtin_popcount(uint(cmask)) == k)
      {
        vc<int> rows, cols;
        repi(i, n) { if (rmask >> i & 1) rows.eb(i); if (cmask >> i & 1) cols.eb(i); }
        divisor = gcd_poly(divisor, minor_polynomial(a, rows, cols));
      }
    auto qr = divide(divisor, previous);
    assert(qr.second.empty());
    if (qr.first.size() > 1) result.eb(qr.first);
    previous = divisor;
  }
  return result;
}
template <class S> vc<S> multiply(const vvc<S> &a, const vc<S> &v)
{
  vc<S> out(a.size());
  repi(i, a.size()) repi(j, v.size()) out[i] += a[i][j] * v[j];
  return out;
}
template <class S> void check_factors(const vvc<S> &a, const vc<Poly<S>> &expected)
{
  using K = FieldAddSubMulDiv<S>;
  int n = a.size();
  auto apply = [&](vc<S> &v) { v = multiply(a, v); };
  auto sample = []() { return S(randrange<ll>(0, S::mod())); };
  auto got = bbla::invariant_factors<K>(n, apply, sample);
  assert(got == expected);
  Poly<S> characteristic{S(1)};
  for (auto &p : expected) characteristic = product(characteristic, p);
  assert(bbla::characteristic_polynomial<K>(n, apply, sample) == characteristic);
  // Force characteristic_polynomial to use its deterministic fallback.
  assert(bbla::characteristic_polynomial<K>(n, apply, [] { return S(0); }) == characteristic);
}

void test_small_factors()
{
  using S = static_modint32<2>;
  for (int n = 0; n <= 3; n++)
    repi(mask, 1 << (n * n))
    {
      vvc<S> a(n, vc<S>(n));
      repi(i, n) repi(j, n) a[i][j] = (mask >> (n * i + j)) & 1;
      check_factors(a, oracle_factors(a));
    }
  repi(n, 5) repi(t, 20)
  {
    vvc<mint> a(n, V(n));
    repi(i, n) repi(j, n) a[i][j] = randrange(0, 4);
    check_factors(a, oracle_factors(a));
  }
}

#ifndef BBLA_EXTENSIONS_CORE_ONLY
void test_similarity()
{
  for (const vc<V> &factors : vc<vc<V>>{
         {{0, 1}}, {{0, 1}, {0, 1}, {0, 1}},
         {{0, 1}, {0, 0, 1}, {0, 0, 0, 1}},
         {{-1, 1}, {1, -2, 1}, {-1, 3, -3, 1}},
         {{1, 0, 1}, {1, 0, 2, 0, 1}},
         {{0, 1}, {0, -1, 1}, {0, 1, -2, 1}}})
  {
    int n = 0;
    for (const auto &p : factors) n += p.size() - 1;
    M companion(n, n);
    int offset = 0;
    for (const auto &p : factors)
    {
      int d = p.size() - 1;
      repi(i, d - 1) companion[offset + i + 1][offset + i] = 1;
      repi(i, d) companion[offset + i][offset + d - 1] = -p[i];
      offset += d;
    }
    repi(t, 25)
    {
      M change(n, n, 1);
      repi(step, 4 * n)
      {
        int i = randrange(0, n), j = randrange(0, n);
        if (i == j) continue;
        mint c = randrange(0, mint::mod());
        repi(k, n) change[i][k] += c * change[j][k];
      }
      auto inverse = change.inv(); assert(inverse.first);
      M a = change * companion * inverse.second;
      check_factors<mint>(a, factors);
    }
  }
}

void test_rank_solve()
{
  for (int rows = 0; rows <= 9; rows++)
    for (int cols = 0; cols <= 9; cols++)
      repi(t, 10)
      {
        M a(rows, cols);
        repi(i, rows) repi(j, cols) a[i][j] = t == 0 ? 0 : randrange(0, 4);
        if (rows > 1 && t % 3 == 0) a.back() = a.front();
        if (cols > 1 && t % 3 == 1) repi(i, rows) a[i].back() = a[i].front();
        auto apply = [&](V &v) { assert(int(v.size()) == cols); v = multiply<mint>(a, v); };
        auto transpose = [&](V &v)
        {
          assert(int(v.size()) == rows);
          V out(cols);
          repi(i, rows) repi(j, cols) out[j] += a[i][j] * v[i];
          v = move(out);
        };
        assert(bbla::rank<F>(rows, cols, apply, transpose) == a.rank());
        V x(cols), b(rows);
        for (auto &v : x) v = randrange(0, mint::mod());
        if (t % 2) b = multiply<mint>(a, x);
        else for (auto &v : b) v = randrange(0, 4);
        M augmented = a;
        repi(i, rows) augmented[i].eb(b[i]);
        bool solvable = augmented.rank() == a.rank();
        auto result = bbla::solve_general<F>(rows, cols, apply, transpose, b);
        assert(result.solvable == solvable);
        if (result.solvable)
        {
          assert(int(result.solution.size()) == cols && result.certificate.empty());
          assert(multiply<mint>(a, result.solution) == b);
        }
        else
        {
          assert(result.solution.empty() && int(result.certificate.size()) == rows);
          auto atw = result.certificate;
          transpose(atw);
          assert(atw == V(cols));
          mint dot = 0;
          repi(i, rows) dot += b[i] * result.certificate[i];
          assert(dot != mint(0));
        }
      }
}
#endif

// A = L * diag(I_r, 0) * U, with implicit invertible bidiagonal L and U.
// Both rectangular orientations, every rank, and inconsistent right-hand sides.
void test_constructed()
{
  for (auto [rows, cols] : vc<pair<int, int>>{{12, 19}, {19, 12}, {24, 24}})
    for (int r = 0; r <= min(rows, cols); r++)
    {
      auto lower = [](V &v) { for (int i = int(v.size()) - 1; i > 0; i--) v[i] += v[i - 1]; };
      auto upper = [](V &v) { repi(i, int(v.size()) - 1) v[i] += v[i + 1]; };
      auto apply = [&](V &v)
      {
        upper(v);
        v.resize(r); v.resize(rows);
        lower(v);
      };
      auto transpose = [&](V &v)
      {
        upper(v);
        v.resize(r); v.resize(cols);
        lower(v);
      };
      assert(bbla::rank<F>(rows, cols, apply, transpose) == r);
      V b(cols);
      for (auto &x : b) x = randrange(0, mint::mod());
      apply(b);
      auto result = bbla::solve_general<F>(rows, cols, apply, transpose, b);
      assert(result.solvable);
      apply(result.solution);
      assert(result.solution == b);
      if (r < rows)
      {
        b.assign(rows, 0); b[r] = 1; lower(b);
        result = bbla::solve_general<F>(rows, cols, apply, transpose, b);
        assert(!result.solvable);
        mint dot = 0;
        repi(i, rows) dot += b[i] * result.certificate[i];
        assert(dot != mint(0));
        transpose(result.certificate);
        assert(result.certificate == V(cols));
      }
    }

  // Larger repeated companion blocks: avoid testing only the degree-n fast path.
  V p{2, -3, 1};
  vc<V> expected;
  for (int d : {2, 4, 8})
  {
    V f{1};
    repi(i, d) f = product(f, p);
    expected.eb(f);
  }
  int n = 0;
  for (auto &f : expected) n += f.size() - 1;
  vvc<mint> a(n, V(n));
  int offset = 0;
  for (auto &f : expected)
  {
    int d = f.size() - 1;
    repi(i, d - 1) a[offset + i + 1][offset + i] = 1;
    repi(i, d) a[offset + i][offset + d - 1] = -f[i];
    offset += d;
  }
  check_factors(a, expected);
}

// A field whose elements have no overloaded arithmetic or modint interface.
struct IntField
{
  using S = int;
  static constexpr int p = 1000000007;
  static S e0() { return 0; }
  static S e1() { return 1; }
  static S add(S a, S b) { return (a + b) % p; }
  static S minus(S a) { return a ? p - a : 0; }
  static S mul(S a, S b) { return ll(a) * b % p; }
  static S inv(S a)
  {
    assert(a);
    S result = 1;
    for (int e = p - 2; e; e >>= 1, a = mul(a, a)) if (e & 1) result = mul(result, a);
    return result;
  }
};

void test_generic_field()
{
  auto apply = [](vc<int> &v) { v[0] = 0; v[2] = IntField::mul(v[2], 2); };
  auto sample = [] { return randrange(1, IntField::p); };
  assert(bbla::rank<IntField>(3, 3, apply, apply, 2, sample) == 2);
  auto result = bbla::solve_general<IntField>(3, 3, apply, apply, vc<int>{0, 7, 10}, sample);
  assert(result.solvable && (result.solution == vc<int>{0, 7, 5}));
  result = bbla::solve_general<IntField>(3, 3, apply, apply, vc<int>{1, 7, 10}, sample);
  assert(!result.solvable && result.certificate[0] != 0);
  assert(result.certificate[1] == 0 && result.certificate[2] == 0);
  vc<int> p{0, 2, IntField::p - 3, 1};
  assert(bbla::characteristic_polynomial<IntField>(3, apply, sample) == p);
  assert((bbla::invariant_factors<IntField>(3, apply, sample) == vc<vc<int>>{p}));
}

void test_retries()
{
  int draws = 0;
  auto identity = [](V &) {};
  auto sampler = [&]() { return mint(draws++ == 2 ? 0 : 1); };
  auto result = bbla::solve_general<F>(1, 1, identity, identity, V{1}, sampler);
  assert(result.solvable && result.solution == V{1} && draws > 3);

  V samples{0, 0, 0, 0, 1, 0, 0, 1, 1, 1};
  draws = 0;
  auto diagonal = [](V &v) { v[1] *= 2; };
  auto scripted = [&]() { assert(draws < int(samples.size())); return samples[draws++]; };
  auto factors = bbla::invariant_factors<F>(2, diagonal, scripted);
  assert((factors == vc<V>{{2, -3, 1}}) && draws == 10);
}

int main()
{
  mt.seed(20260910);
  test_small_factors();
#ifndef BBLA_EXTENSIONS_CORE_ONLY
  test_similarity();
  test_rank_solve();
#endif
  test_constructed();
  test_generic_field();
  test_retries();
  PRINT("Hello World");
}
