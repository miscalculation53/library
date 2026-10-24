#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/fps/multipoint_evaluation.hpp"

template <long long p> void check_wide()
{
  using M = static_modint64<p>;
  using F = FormalPowerSeries<M>;
  static_assert(internal::isprime<p>);
  mt19937_64 rng(20261023);
  for (int n : {1, 2, 4, 8, 16, 32, 64, 128, 256, 512})
  {
    if (!ntt_ok<M>(n)) continue;
    F a(n);
    for (int i = 0; i < n; ++i)
      a[i] = i % 3 ? M(p - 1 - rng() % 23) : M(rng() % p);
    F transformed = a;
    ntt(transformed);
    if (n <= 32)
    {
      M w = M(internal::ntt_generator_constexpr(p)).pow((p - 1) / n);
      assert(w.pow(n) == 1);
      if (n > 1) assert(w.pow(n / 2) != 1);
      int bits = countr_zero(n);
      for (int i = 0; i < n; ++i)
      {
        int reversed = 0;
        for (int j = 0; j < bits; ++j) reversed = reversed * 2 + (i >> j & 1);
        M sum = 0, step = w.pow(reversed), power = 1;
        for (int j = 0; j < n; ++j) sum += a[j] * power, power *= step;
        assert(transformed[i] == sum);
      }
    }
    intt(transformed);
    for (auto &v : transformed) v *= M(n).inv();
    assert(transformed == a);
  }
  if (ntt_ok<M>(512))
  {
    F a(97), b(113);
    for (auto &v : a) v = M(p - 1 - rng() % 103);
    for (auto &v : b) v = M(p - 1 - rng() % 103);
    F expected(a.size() + b.size() - 1);
    for (int i = 0; i < a.sz(); ++i)
      for (int j = 0; j < b.sz(); ++j) expected[i + j] += a[i] * b[j];
    assert(a * b == expected);
    for (int n : {65, 129, 257, 513})
    {
      F h(n);
      for (int i = 1; i < n; ++i) h[i] = M(p - 1 - rng() % 103);
      auto e = h.exp(n);
      assert(e == h.exp_sparse(n));
      assert(e.log(n) == h);
      h[0] = 1;
      assert((h * h.inv(n)).resized(n) == F{1}.resized(n));
      auto square = (h * h).resized(n);
      auto root = square.sqrt(n);
      assert(root && (*root * *root).resized(n) == square);
      if (n == 257)
      {
        vc<M> xs(n);
        for (auto &v : xs) v = M(rng() % p);
        auto ys = multipoint_evaluation(h, xs);
        for (int i = 0; i < n; ++i) assert(ys[i] == h.eval(xs[i]));
      }
    }
  }
}

int main()
{
  check_wide<97>();
  check_wide<7340033>();
  check_wide<998244353>();
  check_wide<2113929217>();
  check_wide<3221225473LL>();
  check_wide<184683593729LL>();
  check_wide<2305843009213693951LL>();
  check_wide<9223372006790004737LL>();
  check_wide<9223372036854771239LL>();
  // p - 1 = 4q with a large prime q: prepare fourth roots without factoring q.
  static_assert(internal::isprime<2305843009213693723LL>);
  check_wide<9223372036854774893LL>();
  // p - 1 = 8q exercises the same boundary with eighth roots.
  static_assert(internal::isprime<1152921504606846269LL>);
  check_wide<9223372036854770153LL>();
  PRINT("Hello World");
}
