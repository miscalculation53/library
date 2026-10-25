#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/fps/rational_gf_2d.hpp"
#include "math/fps/berlekamp_massey.hpp"
#include "math/modint/binomial.hpp"

template <class T>
vvc<T> prefix(const vvc<T> &a, int h, int w)
{
  vvc<T> b(h, vc<T>(w));
  repi(i, h) repi(j, w) b[i][j] = a[i][j];
  return b;
}

// Q = 1 + R として、P * (1 - R + R^2 - ...) を直接計算する。
// 項を順に求める漸化式とは独立な参照実装。
template <class T>
vvc<T> geometric_series(const vvc<T> &p, const vvc<T> &q, int h, int w)
{
  vvc<T> res(h, vc<T>(w)), term(h, vc<T>(w));
  repi(i, min<int>(h, p.size())) repi(j, min<int>(w, p[i].size())) term[i][j] = p[i][j] / q[0][0];
  repi(k, h + w - 1)
  {
    repi(i, h) repi(j, w) res[i][j] += term[i][j];
    vvc<T> next(h, vc<T>(w));
    repi(u, SZ(q)) repi(v, SZ(q[u])) if ((u || v) && q[u][v] != T(0))
    {
      const T c = -q[u][v] / q[0][0];
      repi(i, h - u) repi(j, w - v) next[i + u][j + v] += c * term[i][j];
    }
    term = move(next);
  }
  return res;
}

template <class T>
void check_normalized(const pair<vvc<T>, vvc<T>> &gf)
{
  const auto &[p, q] = gf;
  assert(!q.empty() && !q[0].empty() && q[0][0] == T(1));
  for (const auto &f : {p, q})
  {
    assert(f.empty() || !f.back().empty());
    for (const auto &row : f) assert(row.empty() || row.back() != T(0));
  }
}

template <class T>
void examples()
{
  // 二項係数 C(i+j, i): 1/(1-x-y)。
  vvc<T> binomial(24, vc<T>(27));
  repi(i, 24) repi(j, 27)
    binomial[i][j] = Binomial<T>::C(i + j, i);
  auto b = guess_rational_gf_2d(prefix(binomial, 5, 7));
  assert(b && b->first == (vvc<T>{{1}}) && b->second == (vvc<T>{{1, -1}, {-1}}));
  check_normalized(*b);
  assert(rational_gf_2d_coefficients(b->first, b->second, 24, 27) == binomial);

  // Delannoy 数: sum_k C(i,k) C(j,k) 2^k = 1/(1-x-y-xy) の係数。
  vvc<T> delannoy(24, vc<T>(27));
  repi(i, 24) repi(j, 27)
  {
    T pw = 1;
    repi(k, min(i, j) + 1)
    {
      delannoy[i][j] += Binomial<T>::C(i, k) * Binomial<T>::C(j, k) * pw;
      pw += pw;
    }
  }
  auto d = guess_rational_gf_2d(prefix(delannoy, 7, 6));
  assert(d && d->first == (vvc<T>{{1}}) && d->second == (vvc<T>{{1, -1}, {-1, -1}}));
  assert(rational_gf_2d_coefficients(d->first, d->second, 24, 27) == delannoy);
  assert(!guess_rational_gf_2d(prefix(delannoy, 7, 6), 1));

  // 対角線上だけ 1: 1/(1-xy)。境界の式も使って復元する。
  vvc<T> diagonal(20, vc<T>(20));
  repi(i, 20) diagonal[i][i] = 1;
  auto diag = guess_rational_gf_2d(prefix(diagonal, 6, 6));
  assert(diag && diag->second == (vvc<T>{{1}, {0, -1}}));
  assert(rational_gf_2d_coefficients(diag->first, diag->second, 20, 20) == diagonal);

  // 分子・分母で異なる次数を指定する。
  const vvc<T> p = {{2, 0, 3}, {}, {0, 5}}, q = {{1, -2}, {-3}};
  const auto a = geometric_series(p, q, 16, 17);
  auto asymmetric = pade_2d(prefix(a, 7, 8), 3, 1);
  assert(asymmetric && asymmetric->first == p && asymmetric->second == q);
  assert(!pade_2d(prefix(a, 7, 8), 2, 1));
  auto guessed = guess_rational_gf_2d(prefix(a, 7, 8));
  assert(guessed && rational_gf_2d_coefficients(guessed->first, guessed->second, 16, 17) == a);
  auto altered = prefix(a, 7, 8);
  altered.back().back() += 1;
  assert(!pade_2d(altered, 3, 1));
}

template <class T>
void boundaries()
{
  assert(!guess_rational_gf_2d<T>({}));
  assert(!guess_rational_gf_2d<T>({{}, {}}));
  assert(!pade_2d<T>({}, 0, 0));
  assert(!pade_2d<T>({{}}, 0, 0));
  auto constant = guess_rational_gf_2d<T>({{7}});
  assert(constant && constant->first == (vvc<T>{{7}}) && constant->second == (vvc<T>{{1}}));
  auto zero = guess_rational_gf_2d(vvc<T>(6, vc<T>(9)));
  assert(zero && zero->first.empty() && zero->second == (vvc<T>{{1}}));
  // 非定数の列を次数 0 に制限する場合と、未知数に対して係数が不足する場合。
  assert(!pade_2d<T>({{1, 2}}, 0, 0));
  assert(!pade_2d<T>({{1, 2}}, 1, 1));
  assert(!guess_rational_gf_2d<T>({{1, 2}}, 100));
  auto polynomial = pade_2d<T>({{1, 2}}, 1, 0);
  assert(polynomial && polynomial->first == (vvc<T>{{1, 2}}));
  // 過大な上限でも、既知の単項式の範囲だけを探索する。
  const int hi = numeric_limits<int>::max();
  assert(pade_2d<T>({{1, 2}}, hi, 0) == polynomial);
  assert(guess_rational_gf_2d<T>({{1, 2}}, hi) == guess_rational_gf_2d<T>({{1, 2}}, 10));
  assert(rational_gf_2d_coefficients<T>({}, {{2}}, 3, 4) == vvc<T>(3, vc<T>(4)));
  assert(rational_gf_2d_coefficients<T>({{1}}, {{1}}, 0, 4).empty());
  assert(rational_gf_2d_coefficients<T>({{1}}, {{1}}, 3, 0) == vvc<T>(3));
}

template <class T>
void random_checks()
{
  mt19937 rng(20260921);
  repi(it, 60)
  {
    const int dp = rng() % 4, dq = rng() % 4;
    vvc<T> p(dp + 1), q(dq + 1);
    repi(i, dp + 1)
    {
      p[i].resize(dp + 1 - i);
      for (auto &x : p[i]) x = int(rng() % 9) - 4;
    }
    repi(i, dq + 1)
    {
      q[i].resize(dq + 1 - i);
      for (auto &x : q[i]) x = int(rng() % 9) - 4;
    }
    q[0][0] = 1 + rng() % 5;
    auto a = geometric_series(p, q, 15, 16);
    assert(rational_gf_2d_coefficients(p, q, 15, 16) == a);
    const auto known = prefix(a, 9, 10);
    auto fixed = pade_2d(known, dp, dq);
    auto guessed = guess_rational_gf_2d(known, max(dp, dq));
    assert(fixed && guessed);
    for (const auto &gf : {*fixed, *guessed})
    {
      check_normalized(gf);
      assert(rational_gf_2d_coefficients(gf.first, gf.second, 15, 16) == a);
    }
    assert(guess_rational_gf_2d(known, 10) == guessed);
  }
}

void univariate()
{
  using T = modint998244353;
  mt19937 rng(42);
  for (int order = 1; order <= 5; ++order)
  {
    vc<T> c(order + 1), a(order);
    for (auto &x : c) x = 1 + rng() % 10;
    for (auto &x : a) x = rng() % 10;
    repi(i, order, 80)
    {
      T value = 0;
      repi(j, 1, order + 1) value += c[j] * a[i - j];
      a.eb(value);
    }
    vc<T> seq(a.begin(), a.begin() + 2 * order + 5);
    auto bm = berlekamp_massey<FieldAddSubMulDiv<T>>(seq);
    vc<T> q(bm.size());
    repi(i, SZ(bm)) q[i] = -bm[i];
    auto gf = guess_rational_gf_2d(vvc<T>{seq});
    assert(gf && gf->second == (vvc<T>{q}));
    assert(rational_gf_2d_coefficients(gf->first, gf->second, 1, 80)[0] == a);
    vvc<T> column(seq.size(), vc<T>(1));
    repi(i, SZ(seq)) column[i][0] = seq[i];
    auto transposed = guess_rational_gf_2d(column);
    assert(transposed);
    auto extended = rational_gf_2d_coefficients(transposed->first, transposed->second, 80, 1);
    repi(i, 80) assert(extended[i][0] == a[i]);
  }
}

template <class T>
void larger_degree()
{
  // 32 列以上の行列で、既存の高速な掃き出し経路も通す。
  mt19937 rng(71);
  vvc<T> p = {{1, 2}, {3}}, q(9);
  repi(i, 9)
  {
    q[i].resize(9 - i);
    for (auto &x : q[i]) x = rng() % 11;
  }
  q[0][0] = 1;
  auto a = geometric_series(p, q, 20, 21);
  const auto known = prefix(a, 18, 19);
  auto fixed = pade_2d(known, 1, 8);
  auto guessed = guess_rational_gf_2d(known);
  assert(fixed && guessed);
  assert(rational_gf_2d_coefficients(fixed->first, fixed->second, 20, 21) == a);
  assert(rational_gf_2d_coefficients(guessed->first, guessed->second, 20, 21) == a);
}

// F_2 上の小さい係数表と全分母候補を総当たりする。
void exhaustive()
{
  using T = static_modint32<2>;
  for (auto [h, w] : vc<pair<int, int>>{{1, 4}, {2, 3}, {3, 2}, {3, 3}})
    repi(bits, 1 << (h * w))
    {
      vvc<T> a(h, vc<T>(w));
      repi(i, h) repi(j, w) a[i][j] = bits >> (i * w + j) & 1;
      int best = -1;
      repi(dp, 3) repi(dq, 3)
      {
        vc<pair<int, int>> shifts;
        int p_terms = 0;
        repi(i, h) repi(j, w)
        {
          p_terms += i + j <= dp;
          if ((i || j) && i + j <= dq) shifts.eb(i, j);
        }
        bool found = false;
        if (h * w >= p_terms + SZ(shifts))
          repi(mask, 1 << SZ(shifts))
          {
            bool valid = true;
            repi(i, h) repi(j, w) if (i + j > dp)
            {
              T value = a[i][j];
              repi(k, SZ(shifts)) if (mask >> k & 1)
              {
                auto [u, v] = shifts[k];
                if (u <= i && v <= j) value += a[i - u][j - v];
              }
              if (value != 0) valid = false;
            }
            if (valid) { found = true; break; }
          }
        auto gf = pade_2d(a, dp, dq);
        assert(bool(gf) == found);
        if (gf)
        {
          check_normalized(*gf);
          assert(rational_gf_2d_coefficients(gf->first, gf->second, h, w) == a);
        }
        if (dp == dq && found && best == -1) best = dp;
      }
      auto guessed = guess_rational_gf_2d(a, 2);
      assert(bool(guessed) == (best != -1));
      if (guessed) assert(guessed == pade_2d(a, best, best));
    }
}

int main()
{
  examples<modint998244353>();
  examples<modint1000000007>();
  boundaries<modint998244353>();
  random_checks<modint998244353>();
  random_checks<modint1000000007>();
  univariate();
  larger_degree<modint998244353>();
  larger_degree<modint1000000007>();
  exhaustive();
  cout << "Hello World\n";
}
