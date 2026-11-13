#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "matroid/matroid.hpp"

using F = FieldAddSubMulDiv<modint998244353>;
using S = F::S;

void backend_cases(int d, int pairs, int support)
{
  vc<tuple<int, int, S>> entries_a, entries_b;
  for (int e = 0; e < 2 * pairs; e++) for (int k = 0; k < support; k++)
  {
    entries_a.emplace_back((e + 17 * k) % d, e, S(e + k + 1));
    entries_b.emplace_back((11 * e + 7 * k) % d, e, S(2 * e + k + 1));
  }
  SparseMatrix<F> a(d, 2 * pairs, entries_a), b(d, 2 * pairs, entries_b);
  auto pair_list = internal::linear_matroid_detail::consecutive_pairs(2 * pairs);
  internal::linear_matroid_detail::SparseParityRank<F> parity(a, pair_list);
  internal::linear_matroid_detail::SparseIntersectionRank<F> intersection(a, b);
  auto dense_a = LinearMatroid<F>(a.to_dense()), dense_b = LinearMatroid<F>(b.to_dense());
  int expected_parity = linear_matroid_parity_size(dense_a, 2);
  int expected_intersection = linear_matroid_intersection_size(dense_a, dense_b, 2);
  assert(linear_matroid_parity_size(a, 2) == expected_parity);
  assert(linear_matroid_intersection_size(a, b, 2) == expected_intersection);
  if (pairs <= 16)
  {
    auto parity_ids = linear_matroid_parity_restore(a, 2);
    assert(int(parity_ids.size()) == expected_parity);
    vc<int> columns;
    for (int id : parity_ids) columns.push_back(2 * id), columns.push_back(2 * id + 1);
    dense_a.build(columns);
    auto common = linear_matroid_intersection_restore(a, b, 2);
    assert(int(common.size()) == expected_intersection);
    dense_a.build(common), dense_b.build(common);
  }
  if (d <= 8) assert(parity.dense && intersection.dense);
  if (d == 256 && support == 1 && pairs >= 128) assert(!parity.dense && !intersection.dense);
}

int main()
{
  mt.seed(20261109);
  for (int support : {1, 2, 4}) backend_cases(8, 4, support);
  backend_cases(256, 8, 1);
  backend_cases(256, 128, 1);
  backend_cases(128, 128, 4);
  SparseMatrix<F> a(2000, 4, {{20, 0, 1}, {1999, 1, 1}, {20, 2, 1}, {1999, 3, 1}});
  SparseMatrix<F> b(3000, 4, {{30, 0, 1}, {2999, 1, 1}, {2999, 2, 1}, {30, 3, 1}});
  auto pairs = internal::linear_matroid_detail::consecutive_pairs(4);
  internal::linear_matroid_detail::SparseParityRank<F> parity(a, pairs);
  internal::linear_matroid_detail::SparseIntersectionRank<F> intersection(a, b);
  assert(parity.dense && intersection.dense);
  assert(linear_matroid_parity_size(a, 2) == 1);
  assert(linear_matroid_parity_restore(a, 2).size() == 1);
  assert(linear_matroid_intersection_size(a, b, 2) == 2);
  assert(linear_matroid_intersection_restore(a, b, 2).size() == 2);
  auto selected = a.select_rows(vc<u128>{1999, 20, 1999});
  assert((selected.shape<int>() == pair<int, int>(3, 4)));
  assert(selected(0, 1) == 1 && selected(1, 0) == 1 && selected(2, 3) == 1);
  assert(a.select_rows({}).shape<int>().first == 0);
  using F2 = FieldAddSubMulDiv<static_modint32<2>>;
  SparseMatrix<F2> identity(2, 2, {{0, 0, 1}, {1, 1, 1}});
  assert(identity.rank() == 2 && identity.det() == 1);
  auto one = [] { return F2::e1(); };
  assert(linear_matroid_parity_size(identity, one) == 1);
  vc<pair<int, int>> single_pair{{0, 1}};
  vc<unsigned char> active{1};
  assert(internal::linear_matroid_detail::sparse_parity_rank(identity, single_pair, active, one) == 0);
  cout << "Hello World\n";
}
