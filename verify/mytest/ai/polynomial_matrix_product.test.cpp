#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/fps/polynomial_matrix_product.hpp"

template <class mint>
using PM = vvc<FormalPowerSeries<mint>>;
template <class mint>
using Mat = Matrix<FieldAddSubMulDiv<mint>>;

template <class mint>
Mat<mint> naive_product(const PM<mint> &a, ll l, ll r)
{
  const int dim = a.size();
  Mat<mint> res(dim, dim, 1);
  for (ll t = l; t < r; ++t)
  {
    Mat<mint> b(dim, dim);
    repi(i, dim) repi(j, dim) b[i][j] = a[i][j].eval(mint(t));
    res = b * res;
  }
  return res;
}

template <class mint>
void random_checks(int trials)
{
  mt19937 rng(20260921);
  for (int it = 0; it < trials; ++it)
  {
    const int dim = 1 + rng() % 4, d = rng() % 8;
    PM<mint> a(dim, vc<FormalPowerSeries<mint>>(dim));
    for (auto &row : a) for (auto &f : row)
    {
      f.resize(d + 1);
      for (auto &x : f) x = rng() % 11;
      if (rng() % 4 == 0) f.clear();
      if (rng() % 4 == 0) f.resize(f.size() + 3);
    }
    const ll l = it % 2 ? (1LL << 40) + rng() % 1000 : rng() % 1000;
    const ll n = rng() % 700;
    assert(polynomial_matrix_product(a, l, l + n) == naive_product(a, l, l + n));
  }
}

template <class mint>
void boundaries()
{
  const PM<mint> a = {{{1, 1}, {1}}, {{1}, {}}};
  for (int n : {0, 1, 2, 31, 32, 33, 63, 64, 65, 255, 256, 257, 4095, 4096, 4097})
    assert(polynomial_matrix_product(a, 11, 11 + n) == naive_product(a, 11, 11 + n));
  const ll hi = numeric_limits<ll>::max();
  assert(polynomial_matrix_product(a, hi - 65, hi) == naive_product(a, hi - 65, hi));
  assert(polynomial_matrix_product<mint>({}, 0, hi).empty());
  assert(polynomial_matrix_product<mint>({{{}}}, 0, 0)[0][0] == 1);
  assert(polynomial_matrix_product<mint>({{{}}}, 0, hi)[0][0] == 0);
  assert(polynomial_matrix_product<mint>({{{3, 0, 0}}}, 0, hi)[0][0] == mint(3).pow(hi));
}

template <class mint>
void periods()
{
  const PM<mint> a = {{{2, 1}, {1}}, {{1}, {1, 1}}};
  const ll p = mint::mod(), n = numeric_limits<ll>::max() - 17, l = 17;
  auto period = naive_product(a, l, l + p);
  auto tail = naive_product(a, l, l + n % p);
  assert(polynomial_matrix_product(a, l, l + n) == tail * period.pow(n / p));
}

int main()
{
  random_checks<modint998244353>(100);
  random_checks<modint1000000007>(50);
  random_checks<static_modint32<2>>(30);
  random_checks<static_modint32<3>>(30);
  random_checks<static_modint32<17>>(50);
  random_checks<static_modint32<101>>(50);
  boundaries<modint998244353>();
  boundaries<modint1000000007>();
  periods<static_modint32<2>>();
  periods<static_modint32<17>>();
  periods<static_modint32<101>>();
  // 高次数・小さい標数で補間点が足りない場合。
  using tiny = static_modint32<37>;
  PM<tiny> high(2, vc<FormalPowerSeries<tiny>>(2, FormalPowerSeries<tiny>(30, 1)));
  assert(polynomial_matrix_product(high, 0, 100) == naive_product(high, 0, 100));
  // 動的 mod の切替でも評価点シフトの前計算を更新する。
  using dyn = dynamic_modint32<821>;
  for (int p : {998244353, 1000000007, 101, 998244353})
  {
    dyn::set_mod(p);
    random_checks<dyn>(10);
  }
  cout << "Hello World\n";
}
