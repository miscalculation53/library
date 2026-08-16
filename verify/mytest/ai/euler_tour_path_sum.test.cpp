#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "graph/tree/euler_tour_path_sum.hpp"
#include "algebra/algebra_basic_ops.hpp"

// Test focus: root-path sums and point updates work without constructing LCA support.
void test_without_lca()
{
  const vc<int> par = {-1, 0, 0, 1, 1, 2};
  const vl val = {1, 2, 3, 4, 5, 6};

  using G = GroupAddSub<ll>;
  PathSum<G, false> path_sum(par.size(), par, val);
  assert(path_sum.sum(4) == 8);

  path_sum.set(1, 10);
  path_sum.add(4, 5);
  assert(path_sum.get(1) == 10);
  assert(path_sum.sum(4) == 21);
}

// Test focus: PathSum accepts a custom linear RMQ implementation.
void test_custom_rmq()
{
  const vc<int> par = {-1, 0, 0, 1, 1, 2};
  const vl val = {1, 2, 3, 4, 5, 6};

  using G = GroupAddSub<ll>;
  using RMQ = LinearRMQ<MonoidMin<ull>>;
  PathSum<G, true, FenwickTree<G>, RMQ> path_sum(par.size(), par, val);
  assert(path_sum.sum(3, 5) == 16);
}

int main()
{
  test_without_lca();
  test_custom_rmq();

  cout << "Hello World" << endl;
}
