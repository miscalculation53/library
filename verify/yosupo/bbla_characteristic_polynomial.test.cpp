#define PROBLEM "https://judge.yosupo.jp/problem/characteristic_polynomial"

#include "math/linalg/bbla_invariant_factors.hpp"

using mint = modint998244353;
using F = FieldAddSubMulDiv<mint>;

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n;
  cin >> n;
  vvc<mint> a(n, vc<mint>(n));
  repi(i, n) repi(j, n) cin >> a[i][j];
  auto apply = [&](vc<mint> &v)
  {
    vc<mint> out(n);
    repi(i, n) out[i] = dot_product<F>(n, a[i].begin(), v.begin());
    v.swap(out);
  };
  auto p = bbla::characteristic_polynomial<F>(n, apply);
  repi(i, p.size()) cout << p[i] << (i + 1 == int(p.size()) ? '\n' : ' ');
}
