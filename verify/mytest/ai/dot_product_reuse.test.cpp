#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/linalg/matrix.hpp"
#include "math/linalg/matrix_array.hpp"
#include "math/convolution/convolution.hpp"
#include "algebra/polynomial_ring.hpp"
#include "math/set/subset_convolution.hpp"
#include "math/set/kronecker_power.hpp"

// Independent reference kernels retain one reduction per multiplication.
template <class R>
vc<typename R::S> polynomial_reference(const vc<typename R::S> &a, const vc<typename R::S> &b)
{
  if (a.empty() || b.empty()) return {};
  vc<typename R::S> c(a.size() + b.size() - 1, R::e0());
  repi(i, a.size()) repi(j, b.size()) c[i + j] = R::add(c[i + j], R::mul(a[i], b[j]));
  return c;
}

template <class R>
void test_matrix()
{
  using S = typename R::S;
  for (auto [n, m, p] : vc<tuple<int, int, int>>{
      {0, 0, 0}, {3, 0, 0}, {1, 1, 1}, {3, 17, 4}, {16, 16, 16}, {17, 33, 19}, {32, 15, 16}, {32, 32, 32}})
    repi(mode, 4)
    {
      Matrix<R> a(n, m), b(m, p);
      repi(i, n) repi(j, m)
        if ((i + j) % 4 < mode) a[i][j] = mode == 3 ? S(-1) : S(1 + i * m + j);
      repi(i, m) repi(j, p) b[i][j] = mode == 3 ? S(-1) : S(3 + i * p + j);
      Matrix<R> expected(n, p);
      repi(i, n) repi(j, p) repi(k, m)
        expected[i][j] = R::add(expected[i][j], R::mul(a[i][k], b[k][j]));
      assert(a * b == expected);
      vc<S> v(m, S(-1)), want(n, R::e0());
      repi(i, n) repi(j, m) want[i] = R::add(want[i], R::mul(a[i][j], v[j]));
      assert(a * v == want);
    }
}

template <class R, int n, int m, int p>
void test_matrix_array()
{
  using S = typename R::S;
  repi(mode, 4)
  {
    MatrixArray<R, n, m> a;
    MatrixArray<R, m, p> b;
    repi(i, n) repi(j, m)
      if ((i + j) % 4 < mode) a[i][j] = mode == 3 ? S(-1) : S(1 + i * m + j);
    repi(i, m) repi(j, p) b[i][j] = mode == 3 ? S(-1) : S(3 + i * p + j);
    MatrixArray<R, n, p> expected;
    repi(i, n) repi(j, p) repi(k, m)
      expected[i][j] = R::add(expected[i][j], R::mul(a[i][k], b[k][j]));
    assert(a * b == expected);
    array<S, m> v; v.fill(S(-1));
    array<S, n> want; want.fill(R::e0());
    repi(i, n) repi(j, m) want[i] = R::add(want[i], R::mul(a[i][j], v[j]));
    assert(a * v == want);
  }
}

template <class R, int n>
void test_polynomial_array()
{
  using S = typename R::S;
  array<S, n> a, b;
  repi(i, n) a[i] = S(i - 3), b[i] = S(2 * i + 7);
  auto expected = polynomial_reference<R>(vc<S>(a.begin(), a.end()), vc<S>(b.begin(), b.end()));
  expected.resize(n, R::e0());
  auto c = PolynomialRingArray<R, n>::mul(a, b);
  assert(vc<S>(c.begin(), c.end()) == expected);
}

template <class R>
void test_polynomials_and_subsets()
{
  using S = typename R::S;
  for (int n : {0, 1, 4, 15, 16, 17, 33, 61})
    for (int m : {0, 1, 3, 16, 33})
    {
      vc<S> a(n), b(m);
      repi(i, n) a[i] = S(i - 3);
      repi(i, m) b[i] = S(2 * i + 7);
      auto expected = polynomial_reference<R>(a, b);
      while (!expected.empty() && expected.back() == R::e0()) expected.pop_back();
      assert(PolynomialRingVector<R>::mul(a, b) == expected);
    }
  test_polynomial_array<R, 0>();
  test_polynomial_array<R, 1>();
  test_polynomial_array<R, 16>();
  test_polynomial_array<R, 33>();
  assert(subset_convolution<R>({}, {}).empty());
  for (int lg : {0, 1, 4, 8})
  {
    int len = 1 << lg;
    vc<S> a(len), b(len), expected(len, R::e0());
    repi(i, len) a[i] = S(i - 3), b[i] = S(2 * i + 7);
    repi(s, len)
    {
      for (int t = s;; t = (t - 1) & s)
      {
        expected[s] = R::add(expected[s], R::mul(a[t], b[s ^ t]));
        if (t == 0) break;
      }
    }
    assert(subset_convolution<R>(a, b) == expected);
  }
}

template <class S>
void test_convolution()
{
  using R = RingAddSubMul<S>;
  for (int n : {0, 1, 4, 15, 16, 17, 33, 61, 128})
    for (int m : {0, 1, 3, 16, 33, 73})
      repi(mode, 3)
      {
        vc<S> a(n), b(m);
        repi(i, n) if (mode != 0 || i % 17 == 0) a[i] = mode == 2 ? S(-1) : S(i * 3 + 5);
        repi(i, m) if (mode != 0 || i % 19 == 0) b[i] = mode == 2 ? S(-1) : S(i * 5 + 9);
        auto expected = polynomial_reference<R>(a, b);
        if (n && m) assert(internal::convolution_naive(a, b) == expected);
        for (int p = -2; p <= n + m + 1; p++)
          assert(convolution_point_get(a, b, p) == (p >= 0 && p < int(expected.size()) ? expected[p] : S(0)));
        if constexpr (internal::dot_product_mod32_value<S>::value)
          assert(convolution_point_get(a, b, INT_MAX) == S(0));
      }
}

template <class R, int k>
void test_kronecker()
{
  using S = typename R::S;
  array<array<S, k>, k> a;
  repi(i, k) repi(j, k) a[i][j] = S(1 + i + 2 * j);
  // Explicit A tensor A is an independent oracle for the two-stage action.
  vc<S> v(k * k), expected(k * k, R::e0());
  repi(i, k * k) v[i] = S(i + 1);
  repi(i, k * k) repi(j, k * k)
  {
    auto c = R::mul(a[i / k][j / k], a[i % k][j % k]);
    expected[i] = R::add(expected[i], R::mul(c, v[j]));
  }
  assert((kronecker_power_array<R, k>(a, v) == expected));
}

// Same modint storage, different zero and operations: must use the generic path.
struct ShiftedRing
{
  using S = modint998244353;
  static S e0() { return 1; }
  static S e1() { return 2; }
  static S add(S a, S b) { return a + b - 1; }
  static S minus(S a) { return 2 - a; }
  static S mul(S a, S b) { return (a - 1) * (b - 1) + 1; }
};

#if __cplusplus >= 202002L
static_assert(PolynomialRingArray<RingAddSubMul<ll>, 3>::mul({1, 2, 3}, {4, 5, 6})
    == array<ll, 3>{4, 13, 28});
#endif

template <class R>
void test_algebra()
{
  test_matrix<R>();
  test_matrix_array<R, 0, 0, 0>();
  test_matrix_array<R, 3, 0, 2>();
  test_matrix_array<R, 4, 17, 8>();
  test_matrix_array<R, 16, 17, 19>();
  test_matrix_array<R, 32, 33, 17>();
  test_polynomials_and_subsets<R>();
  test_kronecker<R, 2>();
  test_kronecker<R, 8>();
}

int main()
{
  test_algebra<RingAddSubMul<modint998244353>>();
  test_algebra<RingAddSubMul<ll>>();
  test_algebra<ShiftedRing>();
  test_matrix<SemiRingMinPlus<ll, 1000000>>();
  test_matrix_array<SemiRingMinPlus<ll, 1000000>, 16, 17, 19>();
  test_kronecker<SemiRingMinPlus<ll, 1000000>, 8>();
  test_convolution<modint998244353>();
  test_convolution<modint61>();
  for (int mod : {1, 12, 1 << 30, (1 << 30) + 1, 2147483647})
  {
    using S = dynamic_modint32<83>;
    S::set_mod(mod);
    test_algebra<RingAddSubMul<S>>();
    test_convolution<S>();
  }
  PRINT("Hello World");
}
