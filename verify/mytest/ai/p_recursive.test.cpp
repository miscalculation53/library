#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/fps/p_recursive.hpp"
#include "math/fps/berlekamp_massey.hpp"

template <class mint>
using Rec = vc<FormalPowerSeries<mint>>;

template <class mint>
bool valid(const vc<mint> &a, const Rec<mint> &c)
{
  if (c.size() < 2 || c.back().empty() || c.back().back() != 1) return false;
  const int r = c.size() - 1;
  for (int n = 0; n + r < SZ(a); ++n)
  {
    mint sum = 0;
    repi(j, r + 1) sum += c[j].eval(mint(n)) * a[n + j];
    if (sum != 0) return false;
  }
  return true;
}

template <class mint>
void known_sequences()
{
  const int len = 1000;
  vc<mint> factorial(len, 1), catalan(len, 1), involution(len, 1), fib(len);
  fib[1] = 1;
  for (int i = 1; i < len; ++i)
  {
    factorial[i] = factorial[i - 1] * i;
    // 二項係数の差から Catalan 数を計算する。
    catalan[i] = Binomial<mint>::C(2 * i, i) - Binomial<mint>::C(2 * i, i - 1);
    if (i >= 2)
    {
      involution[i] = involution[i - 1] + mint(i - 1) * involution[i - 2];
      fib[i] = fib[i - 1] + fib[i - 2];
    }
  }
  const vc<vc<mint>> seqs = {factorial, catalan, involution, fib};
  const vc<Rec<mint>> recs = {{{-1, -1}, {1}}, {{-2, -4}, {2, 1}},
                             {{-1, -1}, {-1}, {1}}, {{-1}, {-1}, {1}}};
  for (int t = 0; t < 4; ++t)
  {
    const auto &seq = seqs[t];
    vc<mint> prefix(seq.begin(), seq.begin() + 30);
    auto guessed = guess_p_recursive(prefix, 4, 2);
    assert(valid(seq, guessed));
    assert(guessed.size() == recs[t].size());
    for (const auto &c : {recs[t], guessed})
    {
      assert(p_recursive_extend(prefix, c, len) == seq);
      for (int k : {0, 1, 29, 30, 31, 63, 64, 65, 255, 256, 257, 999})
      {
        assert(p_recursive_nth(prefix, c, k) == seq[k]);
      }
      assert(p_recursive_extend(prefix, c, 7) == vc<mint>(seq.begin(), seq.begin() + 7));
    }
  }
  assert(guess_p_recursive(vc<mint>(30, 0), 3, 2) == (Rec<mint>{{}, {1}}));
  assert(guess_p_recursive<mint>({}, 3, 2).empty());
  assert(guess_p_recursive<mint>({1, 2}, 1, 0).empty());
  assert(guess_p_recursive(vc<mint>(factorial.begin(), factorial.begin() + 30), 1, 0).empty());
  assert(p_recursive_extend<mint>({1}, recs[0], 0).empty());
}

template <class mint>
void random_checks(int trials)
{
  mt19937 rng(20260922);
  for (int it = 0; it < trials; ++it)
  {
    const int r = 1 + rng() % 4, d = rng() % 4, len = 200 + rng() % 400;
    Rec<mint> c(r + 1, FormalPowerSeries<mint>(d + 1));
    for (auto &f : c) for (auto &x : f) x = rng() % 30;
    // 分母に根があれば別の定数項を試す。
    for (;;)
    {
      bool ok = true;
      for (int n = 0; n < len; ++n) if (c[r].eval(mint(n)) == 0) ok = false;
      if (ok) break;
      c[r][0] += 1;
    }
    vc<mint> seq(r);
    for (auto &x : seq) x = rng() % 30;
    // 添字の独立な直接計算と比較する。
    for (int k = r; k < len; ++k)
    {
      mint num = 0;
      repi(j, r) num -= c[j].eval(mint(k - r)) * seq[k - r + j];
      seq.eb(num / c[r].eval(mint(k - r)));
    }
    const int known = r + (r + 1) * (d + 1) + 12;
    vc<mint> prefix(seq.begin(), seq.begin() + known);
    auto guessed = guess_p_recursive(prefix, r, d);
    assert(valid(seq, guessed));
    for (const auto &rec : {c, guessed})
    {
      assert(p_recursive_extend(prefix, rec, len) == seq);
      for (int k : {known, known + 1, 127, len - 1})
      {
        assert(p_recursive_nth(prefix, rec, k) == seq[k]);
      }
    }
    if (d == 0)
    {
      auto bm = berlekamp_massey<FieldAddSubMulDiv<mint>>(prefix);
      assert(guessed.size() == bm.size());
    }
  }
}

void singularities()
{
  using mint = static_modint32<101>;
  // (n-5)(a[n+1]-a[n])=0。n=5 の遷移は初期値の追加で越える。
  const Rec<mint> c = {{5, -1}, {-5, 1}};
  assert(p_recursive_nth<mint>({3}, c, 5) == 3);
  auto prefix = p_recursive_extend<mint>({3}, c, 6);
  assert(prefix == vc<mint>(6, 3));
  prefix.eb(7);
  assert(p_recursive_nth(prefix, c, 50) == 7);
  assert(p_recursive_extend(prefix, c, 51).back() == 7);
  assert(p_recursive_nth<mint>({3}, {{1}, {}}, 0) == 3);
  // 全係数の共通因子も、特異点での制約を保存するために残す。
  vc<mint> jump(30, 7);
  fill(jump.begin(), jump.begin() + 6, mint(3));
  auto guessed = guess_p_recursive(jump, 1, 1);
  assert(valid(jump, guessed));
  assert(guessed.back().eval(5) == 0);
  assert(p_recursive_nth(jump, guessed, 50) == 7);
}

void exhaustive_guess()
{
  using mint = static_modint32<2>;
  for (int n = 0; n <= 8; ++n) for (int mask = 0; mask < (1 << n); ++mask)
  {
    vc<mint> a(n);
    repi(i, n) a[i] = mask >> i & 1;
    bool found[4][4] = {};
    for (int max_r = 1; max_r <= 3; ++max_r) for (int max_d = 0; max_d <= 3; ++max_d)
    {
      int best_r = 0, best_d = -1;
      for (int r = 1; r <= max_r && best_r == 0; ++r)
        for (int d = 0; d <= min(max_d, 1) && best_r == 0; ++d)
        {
          const int cols = (r + 1) * (d + 1);
          if (n - r < cols) continue;
          for (int bits = 1; bits < (1 << cols); ++bits)
          {
            Rec<mint> c(r + 1, FormalPowerSeries<mint>(d + 1));
            repi(j, r + 1)
            {
              repi(k, d + 1) c[j][k] = bits >> ((d + 1) * j + k) & 1;
              c[j].shrink();
            }
            if (valid(a, c))
            {
              best_r = r, best_d = d;
              break;
            }
          }
        }
      auto c = guess_p_recursive(a, max_r, max_d);
      found[max_r][max_d] = !c.empty();
      if (best_r == 0) assert(c.empty());
      else
      {
        assert(SZ(c) == best_r + 1 && valid(a, c));
        int actual_d = 0;
        for (const auto &f : c) chmax(actual_d, f.sz() - 1);
        assert(actual_d == best_d);
      }
      if (max_r > 1 && found[max_r - 1][max_d]) assert(found[max_r][max_d]);
      if (max_d > 0 && found[max_r][max_d - 1]) assert(found[max_r][max_d]);
    }
    // 小さい法でもデフォルト値をそのまま使える。
    assert(guess_p_recursive(a) == guess_p_recursive(a, 3, 3));
  }
}

template <class mint>
void guess_bounds()
{
  vc<mint> catalan(100), fib(100);
  repi(i, 100) catalan[i] = Binomial<mint>::C(2 * i, i) - Binomial<mint>::C(2 * i, i - 1);
  fib[1] = 1;
  repi(i, 2, 100) fib[i] = fib[i - 1] + fib[i - 2];
  const vc<vc<mint>> seqs = {catalan, fib};
  const vc<Rec<mint>> expected = {{{-2, -4}, {2, 1}}, {{-1}, {-1}, {1}}};
  repi(t, 2)
  {
    vc<mint> a(seqs[t].begin(), seqs[t].begin() + 10);
    assert(guess_p_recursive(a) == expected[t]);
    assert(guess_p_recursive(a, 10) == expected[t]);
    for (int r : {2, 3, 10, 1000, numeric_limits<int>::max()})
      for (int d : {1, 2, 10, 1000, numeric_limits<int>::max()})
      {
        auto c = guess_p_recursive(a, r, d);
        assert(c == expected[t]);
        assert(p_recursive_extend(a, c, 100) == seqs[t]);
      }
  }
  vc<mint> a(catalan.begin(), catalan.begin() + 10);
  assert(guess_p_recursive(a, 1, 1) == expected[0]);
  assert(guess_p_recursive(a, 1, 0).empty());
  assert(guess_p_recursive(vc<mint>(fib.begin(), fib.begin() + 10), 1, 10).empty());
}

template <class mint>
void large_factorial()
{
  // NTT と任意 mod の CRT 畳み込みの両方を直接計算と照合。
  constexpr int n = 1'000'000;
  mint expected = 1;
  for (int i = 1; i <= n; ++i) expected *= i;
  assert(p_recursive_nth<mint>({1}, {{-1, -1}, {1}}, n) == expected);
}

void large_indices()
{
  using mint = modint998244353;
  const ll hi = numeric_limits<ll>::max();
  // Wilson の定理による約 10^9 項先の確認。
  const Rec<mint> factorial = {{-1, -1}, {1}};
  assert(p_recursive_nth<mint>({1}, factorial, mint::mod() - 1) == -1);
  assert(p_recursive_nth<mint>({1}, factorial, hi) == 0);
  assert(p_recursive_nth<mint>({1}, {{-3}, {1}}, hi) == mint(3).pow(hi));
  // a[n]=n。上限の添字でも k+1 を作らずに計算する。
  assert(p_recursive_nth<mint>({0, 1}, {{1}, {-2}, {1}}, hi) == mint(hi));
  using tiny = static_modint32<17>;
  // 分母 x^2+3 は F_17 上で非零。比を常に 2 にして周期跨ぎを検証。
  Rec<tiny> periodic = {{-6, 0, -2}, {3, 0, 1}};
  assert(p_recursive_nth<tiny>({1}, periodic, hi) == tiny(2).pow(hi));
}

int main()
{
  known_sequences<modint998244353>();
  known_sequences<modint1000000007>();
  random_checks<modint998244353>(80);
  random_checks<modint1000000007>(30);
  singularities();
  exhaustive_guess();
  guess_bounds<modint998244353>();
  guess_bounds<modint1000000007>();
  large_factorial<modint998244353>();
  large_factorial<modint1000000007>();
  large_indices();
  cout << "Hello World\n";
}
