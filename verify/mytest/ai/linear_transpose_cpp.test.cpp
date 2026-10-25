#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/linalg/linear_transpose_cpp.hpp"
#include "tools/transpose_examples/do_use_fft_cpp.hpp"
#include "ds/fenwick_tree/fenwick_tree.hpp"
#include "math/set/zeta_mobius.hpp"
#include "math/prime/sieve/zeta_mobius_divisor_multiple.hpp"

std::mt19937 random_engine(387529);

template <class T, class Forward>
void check_cpp_forward(int n, const Forward &forward)
{
  auto program = linear_transpose::record<T>(n, forward);
  vc<T> x(n), seed(program.output_size()), expected(n);
  for (auto &v : x) v = int(random_engine() % 11) - 5;
  for (auto &v : seed) v = int(random_engine() % 11) - 5;
  // 順方向に基底ベクトルを与えて行列の各列を作り、転置を独立に求める。
  repi(j, n)
  {
    vc<T> unit(n);
    unit[j] = 1;
    auto column = forward(unit);
    assert(column.size() == seed.size());
    repi(i, seed.size()) expected[j] += column[i] * seed[i];
  }
  assert(program.transpose(seed) == expected);
  assert(program.transpose(vc<T>(seed.size())) == vc<T>(n));
  assert(linear_transpose::check<T>(n, seed.size(), forward,
    [&](const auto &y) { return program.transpose(y); }));
}

template <class T>
void check_scalar_code()
{
  for (int n : {0, 1, 2, 5, 8, 17})
  {
    check_cpp_forward<T>(n, [](auto x)
    {
      using S = typename decltype(x)::value_type;
      S acc = 0;
      repi(i, x.size())
      {
        acc += x[i];
        x[i] = 2 * x[i] + acc;
        x[i] -= x[i] + acc;
        x[i] *= -3;
      }
      std::reverse(x.begin(), x.end());
      if (!x.empty())
      {
        x[0] += x.back();
        x.back() = x[0];
        x.push_back(-x[0]);
        x.push_back(S(0));
      }
      return x;
    });
    vc<int> kind(15), left(15), right(15);
    repi(t, kind.size())
    {
      kind[t] = random_engine() % 2;
      left[t] = random_engine() % (n + 1), right[t] = random_engine() % (n + 1);
      if (left[t] > right[t]) swap(left[t], right[t]);
    }
    check_cpp_forward<T>(n + kind.size(), [&](const auto &x)
    {
      using S = typename std::decay_t<decltype(x)>::value_type;
      FenwickTree<GroupAddSub<S>> tree(vc<S>(x.begin(), x.begin() + n));
      vc<S> y(kind.size());
      repi(t, kind.size())
      {
        if (kind[t] == 0 && left[t] < n) tree.add(left[t], x[n + t]);
        else y[t] = tree.sum(left[t], right[t]);
      }
      return y;
    });
    check_cpp_forward<T>(n + 1, [&](const auto &x)
    {
      using S = typename std::decay_t<decltype(x)>::value_type;
      return zeta_divisor<MonoidAdd<S>>(x);
    });
  }
  for (int n : {0, 1, 2, 8, 32})
  {
    auto forward = [](const auto &x)
    {
      using S = typename std::decay_t<decltype(x)>::value_type;
      return zeta_subset<MonoidAdd<S>>(x);
    };
    check_cpp_forward<T>(n, forward);
    vc<T> a(n);
    repi(i, n) a[i] = i - 2;
    assert(linear_transpose::record<T>(n, forward).transpose(a) == zeta_supset<MonoidAdd<T>>(a));
  }
}

template <class T>
void check_polynomial_code()
{
  for (int n : {0, 1, 2, 7, 65})
  {
    vc<T> kernel(67);
    for (auto &v : kernel) v = int(random_engine() % 11) - 5;
    auto forward = [&](const auto &x) { return convolution(x, kernel); };
    check_cpp_forward<T>(n, forward);
    check_cpp_forward<T>(n, [&](const auto &x)
    {
      using S = typename std::decay_t<decltype(x)>::value_type;
      FormalPowerSeries<S> f(x), g(kernel.begin(), kernel.end());
      return (f * g + f.diff()).resized(n + 2);
    });
    auto divisor = kernel;
    divisor.back() = 1;
    check_cpp_forward<T>(n, [&](const auto &x)
    {
      auto y = linear_transpose::polynomial_mod(x, divisor);
      y.resize(n + 2);
      if (!x.empty()) y[0] += x[0] / T(3);
      return y;
    });
  }
}

void check_errors()
{
  using T = modint998244353;
  auto rejects = [](const auto &f)
  {
    bool caught = false;
    try { linear_transpose::record<T>(2, f); }
    catch (const std::logic_error &) { caught = true; }
    assert(caught);
  };
  rejects([](auto x) { x[0] *= x[1]; return x; });
  rejects([](auto x) { x[0] += 1; return x; });
  rejects([](auto x) { if (x[0] == 0) x[1] += x[0]; return x; });
  rejects([](auto x) { if (x[0]) x[1] += x[0]; return x; });
  rejects([](auto x) { x[0] /= x[1]; return x; });
  rejects([](auto x) { return convolution(x, x); });
  rejects([](auto x) { x[0] = 1; return x; });
  rejects([](auto x) { x[0] = x[1].val(); return x; });
  linear_transpose::Recorder<T> a, b;
  auto x = a.input(1), y = b.input(1);
  bool caught = false;
  try { auto z = x[0] + y[0]; (void)z; }
  catch (const std::logic_error &) { caught = true; }
  assert(caught);
}

void check_do_use_fft_cpp()
{
  using T = modint998244353;
  for (int n : {0, 1, 2, 3, 7, 8, 9, 31, 33, 128, 257}) repi(trial, 4)
  {
    vc<T> a(n), b(n), c(n), expected(n), products(n, T(1));
    for (auto *v : {&a, &b, &c}) for (auto &x : *v) x = int(random_engine() % 11) - 5;
    repi(k, n) repi(i, n)
    {
      products[i] *= a[i] + b[k];
      expected[k] += c[i] * products[i];
    }
    assert(do_use_fft_cpp(a, b, c) == expected);
  }
}

int main(int argc, char **argv)
{
  using T = modint998244353;
  check_scalar_code<long long>();
  check_scalar_code<T>();
  check_polynomial_code<T>();
  check_polynomial_code<modint1000000007>();
  for (int n : {0, 1, 2, 8, 128})
  {
    check_cpp_forward<T>(n, [](auto x) { if (!x.empty()) ntt(x); return x; });
    check_cpp_forward<T>(n, [](auto x) { if (!x.empty()) intt(x); return x; });
  }
  check_errors();
  check_do_use_fft_cpp();
  if (argc > 1)
  {
    // 全種類のブロックを含む出力を、別の C++ としてコンパイルする検査用。
    auto f = [](auto x)
    {
      x[0] = x[0] / T(2);
      auto y = convolution(x, vc<T>{2, 3});
      auto z = linear_transpose::polynomial_mod(y, vc<T>{1, 2, 1});
      z.resize(4);
      ntt(z), intt(z);
      return z;
    };
    auto p = linear_transpose::record<T>(3, f);
    std::ofstream out(argv[1]);
    p.write_cpp(out, "generated_blocks", "modint998244353");
    auto expected = p.transpose({1, 2, 3, 4});
    out << "int main() { assert((generated_blocks({1,2,3,4}) == std::vector<modint998244353>{";
    repi(i, expected.size()) out << (i ? "," : "") << expected[i];
    out << "})); }\n";
  }
  cout << "Hello World" << endl;
}
