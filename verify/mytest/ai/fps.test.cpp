#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "math/fps/fps.hpp"

template <class mint>
using Fps = FormalPowerSeries<mint>;

template <class mint>
Fps<mint> dense_fps(int n, int seed, bool zero_constant = false)
{
  Fps<mint> f(n);
  repi(i, n) f[i] = mint((ll)(i + seed) * (i + seed + 3) + 1);
  if (zero_constant && n > 0)
    f[0] = 0;
  return f;
}

template <class mint>
Fps<mint> naive_mul(const Fps<mint> &f, const Fps<mint> &g, int d = -1)
{
  if (f.empty() || g.empty())
    return {};
  if (d < 0)
    d = f.size() + g.size() - 1;
  Fps<mint> res(d);
  repi(i, f.size()) repi(j, g.size())
  {
    if (i + j >= d)
      break;
    res[i + j] += f[i] * g[j];
  }
  return res;
}

template <class mint>
Fps<mint> naive_exp(const Fps<mint> &f, int d)
{
  assert(f.get(0) == 0);
  Fps<mint> g(d);
  if (d == 0)
    return g;
  g[0] = 1;
  repi(k, 1, d)
  {
    repi(i, 1, k + 1) g[k] += mint(i) * f.get(i) * g[k - i];
    g[k] /= mint(k);
  }
  return g;
}

template <class mint>
void test_multiplication()
{
  const auto f = dense_fps<mint>(80, 1);
  const auto g = dense_fps<mint>(75, 2);
  assert(f * g == naive_mul(f, g));
}

template <class mint>
void test_inverse()
{
  constexpr int n = 384;
  auto f = dense_fps<mint>(n, 3);
  f[0] = 1;

  const auto g = f.inv(n);
  auto product = naive_mul(f, g, n);
  assert(product[0] == 1);
  repi(i, 1, n) assert(product[i] == 0);
}

template <class mint>
void test_exp(int n)
{
  const auto f = dense_fps<mint>(n, 4, true);
  const auto expected = naive_exp(f, n);
  const auto actual = f.exp(n);
  assert(actual == expected);
  assert(actual.log(n) == f);
}

template <class mint>
void test_pow(int n)
{
  auto f = dense_fps<mint>(n, 5);
  f[0] = 1;
  const auto expected = naive_mul(f, f, n);
  assert(f.pow(2, n) == expected);
  assert(f.pow(2) == expected);
  assert(f.pow(0, 0).empty());
}

template <class mint>
void test_sqrt()
{
  constexpr int n = 256;
  auto root = dense_fps<mint>(n, 6);
  root[0] = 1;
  const auto square = naive_mul(root, root, n);

  auto [ok, actual] = square.sqrt(n);
  assert(ok);
  assert(naive_mul(actual, actual, n) == square);
}

template <class mint>
void test_poly_division()
{
  constexpr int n = 220;
  auto quotient = dense_fps<mint>(n, 7);
  auto divisor = dense_fps<mint>(n, 8);
  divisor.back() = 1;
  const auto dividend = naive_mul(quotient, divisor);
  assert(dividend.div_poly(divisor) == quotient);
}

// Test focus: dense FPS algorithms agree with naive formulas for both convolution paths.
template <class mint>
void test_common_operations(int exp_size, int pow_size)
{
  test_multiplication<mint>();
  test_inverse<mint>();
  test_exp<mint>(exp_size);
  test_pow<mint>(pow_size);
  test_sqrt<mint>();
  test_poly_division<mint>();
}

int main()
{
  test_common_operations<modint998244353>(512, 160);
  test_common_operations<modint1000000007>(3072, 1344);

  cout << "Hello World" << endl;
}
