#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/fps/fps_2d.hpp"
#include "math/fps/rational_gf_2d.hpp"

template <class mint>
using Fps2D = FormalPowerSeries2D<mint>;

template <class mint>
Fps2D<mint> slow_product(const Fps2D<mint> &a, const Fps2D<mint> &b, int h, int w)
{
  Fps2D<mint> c(h, w);
  repi(i, min<int>(h, a.size())) repi(j, min<int>(w, a[i].size()))
    repi(u, min<int>(h - i, b.size())) repi(v, min<int>(w - j, b[u].size()))
      c[i + u][j + v] += a[i][j] * b[u][v];
  return c;
}

template <class mint>
Fps2D<mint> slow_exp(const Fps2D<mint> &f, int h, int w)
{
  using F = Fps2D<mint>;
  F res(h, w), term = F{{1}}.resized(h, w);
  mint factorial = 1;
  repi(k, h + w - 1)
  {
    if (k) factorial *= k;
    res += term / factorial;
    term = slow_product(term, f, h, w);
  }
  return res;
}

template <class mint>
Fps2D<mint> slow_log(const Fps2D<mint> &f, int h, int w)
{
  using F = Fps2D<mint>;
  F res(h, w), g = f - F{{1}}, term = F{{1}}.resized(h, w);
  repi(k, 1, h + w - 1)
  {
    term = slow_product(term, g, h, w);
    res += term * (mint(k & 1 ? 1 : -1) / k);
  }
  return res;
}

template <class mint>
void basic_operations()
{
  using F = Fps2D<mint>;
  F f{{1, 2}, {}, {3, 0, 4}}, g{{2}, {5, 6}};
  assert(f.shape() == make_pair(3, 3));
  assert(F{}.shape() == make_pair(0, 0));
  assert(F(3, 0).shape() == make_pair(3, 0));
  assert(f.get(-1, 0) == 0 && f.get(0, -1) == 0 && f.get(1, 2) == 0 && f.get(3, 0) == 0);
  assert(f.resized(2, 3) == (F{{1, 2, 0}, {0, 0, 0}}));
  assert(f.transposed().transposed() == f.resized(3, 3));
  assert(f.transposed() == (F{{1, 0, 3}, {2, 0, 0}, {0, 0, 4}}));
  assert(f.eval(2, 3) == 163);
  assert(F{}.eval(2, 3) == 0);
  F small{{1, 0}, {0}, {0, 0, 0}};
  small.shrink();
  assert(small == (F{{1}}));
  F zeros(4, 5);
  zeros.shrink();
  assert(zeros.empty());
  assert((f + g).resized(4, 4) == (F{{3, 2}, {5, 6}, {3, 0, 4}}).resized(4, 4));
  assert(((f + g) - g).resized(4, 4) == f.resized(4, 4));
  assert(-(-f) == f && f * mint(7) / mint(7) == f && mint(3) * f == f * mint(3));
  auto self = f;
  self += self;
  assert(self == f * mint(2));
  self -= self;
  self.shrink();
  assert(self.empty());
  auto [gh, gw] = g.shape();
  assert(f * g == slow_product(f, g, 3 + gh - 1, 3 + gw - 1));
  self = f;
  self *= self;
  assert(self == slow_product(f, f, 5, 5));
  assert((F{} * f).empty() && (F(3, 0) * f).empty());
  // y^2 * y^2 を x の項に混入させない。
  assert((F{{0, 0, 1}} * F{{0, 0, 1}}) == (F{{0, 0, 0, 0, 1}}));
  assert((F{{0, 0, 1}}.mul(F{{0, 0, 1}}, 2, 3) == F(2, 3)));

  assert(f.diff_x() == (F{{0, 0, 0}, {6, 0, 8}}));
  assert(f.diff_y() == (F{{2, 0}, {0, 0}, {0, 8}}));
  assert(f.integ_x().diff_x() == f.resized(3, 3));
  assert(f.integ_y().diff_y() == f.resized(3, 3));
  assert(f.integ_x().transposed() == f.transposed().integ_y());
  assert(f.diff_x().transposed() == f.transposed().diff_y());
  assert((f * g).diff_x() == f.diff_x() * g + f * g.diff_x());
  assert((f * g).diff_y() == f.diff_y() * g + f * g.diff_y());
  assert(F{}.diff_x().empty() && F{}.diff_y().empty());
}

template <class mint>
void lengths()
{
  using F = Fps2D<mint>;
  for (int h : {0, 1, 2, 3, 7}) for (int w : {0, 1, 2, 5, 8})
  {
    const F zero(h, w), one = F{{1}}.resized(h, w);
    assert(F{{1}}.inv(h, w) == one);
    assert(F{{1}}.log(h, w) == zero);
    assert(F{}.exp(h, w) == one);
    assert(F{}.pow(0, h, w) == one && F{}.pow(3, h, w) == zero);
    assert(F{{1}}.pow(-7, h, w) == one);
    const F f{{1, 2}, {3, 4}}, g{{2, 1}, {}, {5}};
    assert(f.mul(g, h, w) == slow_product(f, g, h, w));
    assert(f.resized(20, 20).mul(g.resized(25, 25), h, w) == f.mul(g, h, w));
    assert(f.div(g, h, w) == F(rational_gf_2d_coefficients<mint>(f, g, h, w)));
    assert(f.pow(2, h, w) == slow_product(f, f, h, w));
    assert(f.pow(-1, h, w) == f.inv(h, w));
    if (h && w)
    {
      assert(f.inv(h, w).size() == size_t(h));
      for (const auto &row : f.inv(h, w)) assert(row.size() == size_t(w));
      assert(f.log(h, w).exp(h, w) == f.resized(h, w));
      F input = f - F{{1}};
      assert(input.exp(h, w).log(h, w) == input.resized(h, w));
    }
  }
}

template <class mint>
void random_checks()
{
  using F = Fps2D<mint>;
  mt19937 rng(20260921);
  repi(it, 90)
  {
    const int h = 1 + rng() % 7, w = 1 + rng() % 8;
    F f(1 + rng() % 8), g(1 + rng() % 9);
    for (auto *a : {&f, &g}) for (auto &row : *a)
    {
      row.resize(rng() % 9);
      for (auto &x : row) x = int(rng() % 21) - 10;
    }
    assert(f.mul(g, h, w) == slow_product(f, g, h, w));
    const int k = rng() % 6;
    F power = F{{1}}.resized(h, w);
    repi(i, k) power = slow_product(power, f, h, w);
    assert(f.pow(k, h, w) == power);
    f = f.resized(h, w);
    f[0][0] = 1;
    const F inverse = f.inv(h, w);
    assert(inverse == F(rational_gf_2d_coefficients<mint>({{1}}, f, h, w)));
    assert(slow_product(f, inverse, h, w) == F{{1}}.resized(h, w));
    assert(f.log(h, w) == slow_log(f, h, w));
    assert(f.log(h, w).transposed() == f.transposed().log(w, h));
    f[0][0] = 0;
    const F exponential = f.exp(h, w);
    assert(exponential == slow_exp(f, h, w));
    assert(exponential.transposed() == f.transposed().exp(w, h));
    assert(exponential.log(h, w) == f);
  }
}

template <class mint>
void known_series()
{
  using F = Fps2D<mint>;
  constexpr int h = 67, w = 49;
  const F q{{1, -1}, {-1}};
  const F a = q.inv(h, w);
  repi(i, h) repi(j, w) assert(a[i][j] == Binomial<mint>::C(i + j, i));
  // 先ほどの母関数推定と相互運用する。
  auto guessed = guess_rational_gf_2d<mint>(a.resized(5, 6));
  assert(guessed && F(guessed->first).div(guessed->second, h, w) == a);

  // exp(x+y+xy): 独立な閉形式と照合する。
  F f{{0, 1}, {1, 1}};
  const F e = f.exp(h, w);
  repi(i, h) repi(j, w)
  {
    mint expected = 0;
    repi(k, min(i, j) + 1)
      expected += Binomial<mint>::finv(i - k) * Binomial<mint>::finv(j - k) * Binomial<mint>::finv(k);
    assert(e[i][j] == expected);
  }
  assert(e.log(h, w) == f.resized(h, w));

  // 両入力が密な畳み込みと非2冪の長方形。
  F dense(h, w);
  repi(i, h) repi(j, w) dense[i][j] = (i * 73 + j * 91 + i * j * 17 + 3) % 103;
  dense[0][0] = 1;
  auto inverse = dense.inv(h, w);
  assert(slow_product(dense, inverse, h, w) == F{{1}}.resized(h, w));
  assert(dense.log(h, w).exp(h, w) == dense);
}

template <class mint>
void one_variable()
{
  using F = Fps2D<mint>;
  using P = FormalPowerSeries<mint>;
  P f(25);
  repi(i, f.size()) f[i] = i * i + i + 1;
  F row{f};
  assert(row.inv(1, 40)[0] == f.inv(40));
  assert(row.log(1, 40)[0] == f.log(40));
  f[0] = 0;
  row[0] = f;
  assert(row.exp(1, 40)[0] == f.exp(40));
  assert(row.transposed().exp(40, 1).transposed() == row.exp(1, 40));
  assert(row.pow(5, 1, 40)[0] == f.pow(5, 40));
}

void small_characteristic()
{
  using mint = static_modint32<5>;
  using F = Fps2D<mint>;
  // 全次数 4 まで。log, exp で割る数が標数に達する直前。
  const F f{{0, 1, 2}, {3, 4, 1}, {2, 1, 4}};
  assert(f.exp(3, 3) == slow_exp(f, 3, 3));
  assert(f.exp(3, 3).log(3, 3) == f);
  assert((f + F{{1}}).log(3, 3) == slow_log(f + F{{1}}, 3, 3));
  assert(f.integ_x().diff_x() == f && f.integ_y().diff_y() == f);
  // 逆数・整数乗は標数を越える長さも扱う。
  using tiny = static_modint32<2>;
  using B = Fps2D<tiny>;
  const B q{{1, 1}, {1, 1}};
  assert(q.inv(13, 15) == B(13, vc<tiny>(15, 1)));
  assert(q.pow(8, 13, 15) == (B{{1}, {}, {}, {}, {}, {}, {}, {}, {1}} * B{{1, 0, 0, 0, 0, 0, 0, 0, 1}}).resized(13, 15));
}

void extreme_exponents()
{
  using mint = modint998244353;
  using F = Fps2D<mint>;
  const ll lo = numeric_limits<ll>::min(), hi = numeric_limits<ll>::max();
  const F f{{3, 1}, {2}};
  assert(f.pow(lo, 2, 2).mul(f.pow(hi, 2, 2), 2, 2) == f.inv(2, 2));
  assert(F{{3}}.pow(lo, 3, 4) == F{{mint(3).inv().pow(1ULL << 63)}}.resized(3, 4));
  assert((F{{0, 1}, {1}}.pow(hi, 3, 4) == F(3, 4)));
}

int main()
{
  basic_operations<modint998244353>();
  lengths<modint998244353>();
  random_checks<modint998244353>();
  random_checks<modint1000000007>();
  known_series<modint998244353>();
  known_series<modint1000000007>();
  one_variable<modint998244353>();
  one_variable<modint1000000007>();
  small_characteristic();
  extreme_exponents();
  cout << "Hello World\n";
}
