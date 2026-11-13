#define PROBLEM "https://atcoder.jp/contests/abc399/tasks/abc399_g"

#include "matroid/graphic_matroid.hpp"
#include "matroid/partition_matroid.hpp"

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  using F = FieldAddSubMulDiv<modint998244353>;
  int n, m, colors;
  cin >> n >> m >> colors;
  vc<int> capacity(colors), group(m), count(colors);
  for (auto &x : capacity) cin >> x;
  vc<pair<int, int>> edges(m);
  for (int e = 0; e < m; e++)
  {
    auto &[u, v] = edges[e];
    cin >> u >> v >> group[e];
    --u, --v, --group[e];
    count[group[e]]++;
  }
  auto graphic = GraphicMatroid(n, std::move(edges)).sparse_linear_representation();
  auto partition = PartitionMatroid(group, capacity).sparse_linear_representation();
  auto graphic_transpose = graphic.transpose();
  ll answer = 0;
  for (int trial = 0; trial < 2; trial++)
  {
    auto weighted = partition;
    weighted.scale_columns(gen_vec(m, [](int) { return random_modint<modint998244353>(); }));
    auto matrix = weighted.multiply_dense(graphic_transpose);
    LinearBasis<F> basis(n);
    int left = 0, row = 0;
    ll candidate = 0;
    for (int right = 0; right < colors; right++)
    {
      for (int k = 0; k < min(capacity[right], count[right]); k++, row++)
        basis.insert(vc<modint998244353>(matrix[row].begin(), matrix[row].end()), right);
      while (left <= right && basis.rank(left) == n - 1) left++;
      candidate += left;
    }
    chmax(answer, candidate);
  }
  cout << answer << '\n';
}
