#define PROBLEM "https://judge.yosupo.jp/problem/sparse_matrix_det"

#include "math/linalg/bbla.hpp"
#include "ds/csr.hpp"

using mint = modint998244353;
using F = FieldAddSubMulDiv<mint>;

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, k;
  cin >> n >> k;
  vc<pair<int, pair<int, mint>>> entries;
  entries.reserve(k);
  repi(i, k)
  {
    int row, col;
    mint value;
    cin >> row >> col >> value;
    entries.eb(row, pair{col, value});
  }
  CSR<pair<int, mint>> a(n, entries);
  vc<mint> buffer(n);
  auto linear_map = [&](vc<mint> &v)
  {
    repi(i, n)
    {
      mint sum = 0;
      for (auto [j, value] : a[i]) sum += value * v[j];
      buffer[i] = sum;
    }
    v.swap(buffer);
  };
  cout << bbla::det<F>(n, linear_map) << '\n';
}
