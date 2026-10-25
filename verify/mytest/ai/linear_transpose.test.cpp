#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "tools/transpose_examples/basic.generated.hpp"
#include "tools/transpose_examples/do_use_fft.hpp"
#include "math/set/zeta_mobius.hpp"
#include "math/prime/sieve/zeta_mobius_divisor_multiple.hpp"
#include "math/fps/multipoint_evaluation.hpp"

std::mt19937 engine(513982);

template <class T>
vc<T> random_vector(int n)
{
  vc<T> a(n);
  for (auto &x : a) x = int(engine() % 21) - 10;
  return a;
}

template <class T>
void test_basic()
{
  for (int n : {1, 2, 3, 7, 16, 33}) repi(trial, 20)
  {
    const int q = engine() % 30;
    vc<int> l(q), r(q), kind(q);
    repi(i, q)
    {
      l[i] = engine() % n, r[i] = engine() % (n + 1);
      if (l[i] > r[i]) swap(l[i], r[i]);
      kind[i] = engine() % 2;
    }
    auto x = random_vector<T>(n), w = random_vector<T>(q);
    vc<T> sums(q), adds(n);
    repi(i, q) repi(j, l[i], r[i]) sums[i] += x[j], adds[j] += w[i];
    assert(range_sum(x, l, r) == sums);
    assert(range_sum_transpose(w, n, l, r) == adds);
    assert(range_sum_check<T>(n, l, r));

    auto input = random_vector<T>(n + q), cur = vc<T>(input.begin(), input.begin() + n);
    vc<T> expected(q), gradient(n + q), accumulated(n);
    repi(t, q)
    {
      if (kind[t] == 0) cur[l[t]] += input[n + t];
      else repi(i, l[t], r[t]) expected[t] += cur[i];
    }
    repi(t, q - 1, -1, -1)
    {
      if (kind[t] == 0) gradient[n + t] = accumulated[l[t]];
      else repi(i, l[t], r[t]) accumulated[i] += w[t];
    }
    copy(ALL(accumulated), gradient.begin());
    assert(fenwick_queries(input, n, kind, l, r) == expected);
    assert(fenwick_queries_transpose(w, n + q, n, kind, l, r) == gradient);
  }
  for (int bits = 0; bits <= 8; ++bits)
  {
    auto x = random_vector<T>(1 << bits);
    assert(subset_zeta(x) == zeta_subset<MonoidAdd<T>>(x));
    assert(subset_zeta_transpose(x, x.size()) == zeta_supset<MonoidAdd<T>>(x));
  }
  for (int n : {0, 1, 2, 17, 100, 513})
  {
    auto x = random_vector<T>(n + 1);
    vc<int> primes;
    for (auto p : LinearSieve::primes(n)) primes.push_back(p);
    assert(divisor_zeta(x, primes) == zeta_divisor<MonoidAdd<T>>(x));
    assert(divisor_zeta_transpose(x, x.size(), primes) == zeta_multiple<MonoidAdd<T>>(x));
  }
  assert(range_sum<T>({}, {}, {}).empty());
  assert(range_sum_transpose<T>({}, 0, {}, {}).empty());
}

template <class T>
void test_polynomial()
{
  for (int n : {0, 1, 2, 3, 63, 64, 65, 200, 301})
    for (int m : {0, 1, 2, 11, 127, 256})
    {
      auto a = random_vector<T>(n), b = random_vector<T>(m);
      auto w = random_vector<T>(n + m + 3);
      vc<T> expected(n);
      if (m > 0 && n > 0)
        repi(i, n) repi(j, m) expected[i] += b[j] * w[i + j];
      assert(linear_transpose::convolution_transpose(w, b, n) == expected);
      w.resize((n == 0 || m == 0) ? 0 : n + m - 1);
      assert(fixed_convolution_transpose(w, n, b) == expected);
      assert(fixed_convolution_check<T>(n, b));
      w.resize(w.size() / 2);
      fill(ALL(expected), T(0));
      if (n > 0 && m > 0)
        repi(i, n) repi(j, m) if (i + j < (int)w.size()) expected[i] += b[j] * w[i + j];
      assert(linear_transpose::convolution_transpose(w, b, n) == expected);
      b.resize(m + 2), b.back() = 1;
      assert(fixed_remainder_check<T>(n, b));
      const auto remainder = fixed_remainder(a, b);
      auto naive = a;
      naive.resize(max(a.size(), b.size()));
      for (int i = n - 1; i >= (int)b.size() - 1; --i)
      {
        const T lead = naive[i];
        repi(j, b.size()) naive[i - (int)b.size() + 1 + j] -= lead * b[j];
      }
      naive.resize(b.size() - 1);
      assert(remainder == naive);
    }
}

void test_do_use_fft()
{
  using T = modint998244353;
  for (int n : {0, 1, 2, 3, 7, 8, 9, 31, 32, 33, 127, 129, 257}) repi(t, 8)
  {
    auto a = random_vector<T>(n), b = random_vector<T>(n), c = random_vector<T>(n);
    vc<T> expected(n), products(n, T(1));
    repi(k, n) repi(i, n)
    {
      products[i] *= a[i] + b[k];
      expected[k] += c[i] * products[i];
    }
    assert(do_use_fft(a, b, c) == expected);
  }
  assert((do_use_fft<T>({1, 2, 3}, {4, 5, 6}, {7, 8, 9}) == vc<T>{146, 1050, 8694}));
  for (int n : {1, 2, 8, 128})
  {
    auto roots = random_vector<T>(n);
    vc<vc<T>> products(2 * n);
    repi(i, n) products[n + i] = {-roots[i], 1};
    repi(i, n - 1, 0, -1) products[i] = convolution(products[2 * i], products[2 * i + 1]);
    auto f = random_vector<T>(n + 3);
    assert(evaluate_tree(f, products, 1, n) == multipoint_evaluation(FormalPowerSeries<T>(f), roots));
    assert(evaluate_tree_check<T>(f.size(), products, 1, n));
    assert(newton_basis_check<T>(n, products, 1));
  }
}

int main()
{
  test_basic<long long>();
  test_basic<modint998244353>();
  test_polynomial<modint998244353>();
  test_polynomial<modint1000000007>();
  test_polynomial<static_modint32<17>>();
  using dynamic_mint = dynamic_modint32<734>;
  dynamic_mint::set_mod(1000000007);
  test_polynomial<dynamic_mint>();
  for (int n : {0, 1, 2, 4, 128, 512})
  {
    assert(fourier_check<modint998244353>(n));
    assert(inverse_fourier_check<modint998244353>(n));
  }
  test_do_use_fft();
  cout << "Hello World" << endl;
}
