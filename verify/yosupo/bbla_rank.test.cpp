#define PROBLEM "https://judge.yosupo.jp/problem/matrix_rank"

#include "math/linalg/bbla_rank_solve.hpp"

using mint = modint998244353;
using F = FieldAddSubMulDiv<mint>;

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, m;
  cin >> n >> m;
  vvc<mint> a(n, vc<mint>(m)), at(m, vc<mint>(n));
  repi(i, n) repi(j, m) { cin >> a[i][j]; at[j][i] = a[i][j]; }
  auto apply = [&](vc<mint> &v)
  {
    vc<mint> out(n);
    repi(i, n) out[i] = dot_product<F>(m, a[i].begin(), v.begin());
    v.swap(out);
  };
  auto transpose = [&](vc<mint> &v)
  {
    vc<mint> out(m);
    repi(i, m) out[i] = dot_product<F>(n, at[i].begin(), v.begin());
    v.swap(out);
  };
  cout << bbla::rank<F>(n, m, apply, transpose) << '\n';
}
