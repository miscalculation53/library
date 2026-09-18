#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/prime/sieve/dirichlet_convolution.hpp"
#include "math/modint/binomial.hpp"

using mint = modint998244353;
using R = FieldAddSubMulDiv<mint>;
using D = DirichletSeries<R>;

// ABC428 G: log と s -> ds を組み合わせる。
vc<mint> necklace(int n, const vc<int> &weights)
{
  vc<mint> frequency(n + 1);
  for (int x : weights) frequency[x]++;
  const auto l = -(D::unit(n) - D(frequency)).log();
  D answer(n);
  const int degree = int(msb_pos(n));
  const auto phi = enumerate_multiplicative<RingAddSubMul<int>>(degree, totient_primepower);
  for (int d = 1; d <= degree; d++)
    answer += l.substitute_power(d) * (mint(phi[d]) * Binomial<mint>::inv(d));
  return answer.to_vector();
}

// 色の列をすべて生成し、実際に回転させた辞書順最小の列で同一視する。
vc<mint> brute_necklace(int n, const vc<int> &weights)
{
  vc<set<vc<int>>> seen(n + 1);
  vc<int> sequence;
  const auto dfs = [&](auto &&self, int product) -> void
  {
    if (!sequence.empty())
    {
      auto best = sequence, rotated = sequence;
      for (int i = 1; i < int(sequence.size()); i++)
      {
        rotate(rotated.begin(), rotated.begin() + 1, rotated.end());
        best = min(best, rotated);
      }
      seen[product].insert(best);
    }
    for (int color = 0; color < int(weights.size()); color++)
      if (product <= n / weights[color])
      {
        sequence.push_back(color);
        self(self, product * weights[color]);
        sequence.pop_back();
      }
  };
  dfs(dfs, 1);
  vc<mint> answer(n + 1);
  for (int i = 1; i <= n; i++) answer[i] = seen[i].size();
  return answer;
}

int main()
{
  assert(necklace(6, {2, 2, 3, 4}) == (vc<mint>{0, 0, 2, 1, 4, 0, 2}));
  assert(necklace(16, {2, 2, 2, 2}) == (vc<mint>{0, 0, 4, 0, 10, 0, 0, 0, 24, 0, 0, 0, 0, 0, 0, 0, 70}));
  assert(necklace(12, {10, 2, 2, 3, 8, 7, 11, 6, 12, 4, 9, 5, 3, 2}) ==
    (vc<mint>{0, 0, 3, 2, 7, 1, 7, 1, 15, 4, 4, 1, 24}));
  mt19937 rng(5540971);
  for (int round = 0; round < 100; round++)
  {
    vc<int> weights(1 + rng() % 4);
    for (int &x : weights) x = 2 + rng() % 7;
    const int n = 10 + rng() % 55;
    assert(necklace(n, weights) == brute_necklace(n, weights));
  }
  PRINT("Hello World");
}
