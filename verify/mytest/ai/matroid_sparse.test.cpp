#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "matroid/matroid.hpp"

using F = FieldAddSubMulDiv<modint998244353>;
using S = F::S;

template <class G>
void equal_representation(const LinearMatroid<G> &a, const SparseMatrix<G> &b)
{
  auto x = a.matrix(), y = b.to_dense();
  assert(x.template shape<int>() == y.template shape<int>());
  auto [d, n] = x.template shape<int>();
  for (int i = 0; i < d; i++) for (int j = 0; j < n; j++) assert(x[i][j] == y[i][j]);
}

bool independent(const Matrix<F> &a, const vc<int> &indices)
{
  Matrix<F> b(a.shape<int>().first, indices.size());
  for (int e = 0; e < int(indices.size()); e++) for (int i = 0; i < a.shape<int>().first; i++)
    b[i][e] = a[i][indices[e]];
  return b.rank<int>() == int(indices.size());
}

int main()
{
  mt.seed(20261109);
  equal_representation(UniformMatroid(5, 2).linear_representation(), UniformMatroid(5, 2).sparse_linear_representation());
  equal_representation(PartitionMatroid({0, 1, 0, 1, 0}, {2, 0}).linear_representation(),
                       PartitionMatroid({0, 1, 0, 1, 0}, {2, 0}).sparse_linear_representation());
  auto graphic = GraphicMatroid(3, {{0, 0}, {0, 1}, {0, 1}, {1, 2}});
  equal_representation(graphic.linear_representation(), graphic.sparse_linear_representation());
  assert(graphic.sparse_linear_representation().nnz() == 6);
  BinaryMatroid<> binary({1, 2, 3});
  equal_representation(binary.linear_representation(), binary.sparse_linear_representation());
  TransversalMatroid transversal(3, vvc<int>{{0, 0, 2}, {1}, {0, 2}});
  const auto saved = mt;
  auto dense_transversal = transversal.linear_representation();
  mt = saved;
  equal_representation(dense_transversal, transversal.sparse_linear_representation());
  BicircularMatroid bicircular(3, {{0, 0}, {0, 1}, {1, 2}, {2, 0}});
  const auto saved_bicircular = mt;
  auto dense_bicircular = bicircular.linear_representation();
  mt = saved_bicircular;
  equal_representation(dense_bicircular, bicircular.sparse_linear_representation());
  SparseMatrix<F> empty(0, 0);
  assert(linear_matroid_intersection_restore(empty, empty).empty());
  assert(linear_matroid_parity_restore(empty, {}).empty());
  assert(linear_matroid_parity_size(empty) == 0);
  using F2 = FieldAddSubMulDiv<static_modint32<2>>;
  SparseMatrix<F2> binary_identity(2, 2, {{0, 0, 1}, {1, 1, 1}});
  auto binary_one = [] { return F2::e1(); };
  assert(linear_matroid_intersection_size(binary_identity, binary_identity, binary_one) == 2);
  assert(linear_matroid_parity_size(binary_identity, binary_one) == 1);
  for (int trial = 0; trial < 120; trial++)
  {
    int n = randrange(0, 9), da = randrange(0, 7), db = randrange(0, 7);
    Matrix<F> a(da, n), b(db, n);
    for (int i = 0; i < da; i++) for (int e = 0; e < n; e++) if (randrange(0, 3) == 0) a[i][e] = randrange(-3, 4);
    for (int i = 0; i < db; i++) for (int e = 0; e < n; e++) if (randrange(0, 3) == 0) b[i][e] = randrange(-3, 4);
    SparseMatrix<F> sa(a), sb(b);
    int optimum = 0;
    for (int mask = 0; mask < (1 << n); mask++)
    {
      vc<int> indices;
      for (int e = 0; e < n; e++) if (mask >> e & 1) indices.push_back(e);
      if (independent(a, indices) && independent(b, indices)) chmax(optimum, int(indices.size()));
    }
    assert(linear_matroid_intersection_size(sa, sb, 2) == optimum);
    auto indices = linear_matroid_intersection_restore(sa, sb, 2);
    assert(int(indices.size()) == optimum && independent(a, indices) && independent(b, indices));
    auto order = gen_vec(n, [](int e) { return e; });
    shuffle(order.begin(), order.end(), mt);
    vc<pair<i128, u128>> pairs;
    for (int e = 0; e + 1 < n; e += 2) pairs.emplace_back(order[e], order[e + 1]);
    optimum = 0;
    for (int mask = 0; mask < (1 << pairs.size()); mask++)
    {
      vc<int> elements;
      for (int e = 0; e < int(pairs.size()); e++) if (mask >> e & 1)
        elements.push_back(int(pairs[e].first)), elements.push_back(int(pairs[e].second));
      if (independent(a, elements)) chmax(optimum, int(elements.size()) / 2);
    }
    assert(linear_matroid_parity_size(sa, pairs, 2) == optimum);
    auto pair_ids = linear_matroid_parity_restore(sa, pairs, 2);
    vc<int> elements;
    for (int id : pair_ids) elements.push_back(int(pairs[id].first)), elements.push_back(int(pairs[id].second));
    assert(int(pair_ids.size()) == optimum && independent(a, elements));
    if (n % 2 == 0)
    {
      assert(linear_matroid_parity_size(sa, 2) == linear_matroid_parity_size(LinearMatroid<F>(a), 2));
      assert(linear_matroid_parity_restore(sa, 2).size() == linear_matroid_parity_restore(LinearMatroid<F>(a), 2).size());
    }
  }
  cout << "Hello World\n";
}
