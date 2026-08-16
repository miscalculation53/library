#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "graph/tree/euler_tour_path_prod.hpp"
#include "algebra/algebra_basic_ops.hpp"

// Test focus: root-path products and point updates work without constructing LCA support.
void test_without_lca()
{
  const vc<int> par = {-1, 0, 0, 1, 1, 2};
  const vl val = {1, 2, 3, 4, 5, 6};

  using G = GroupAddSub<ll>;
  PathProd<G, false> path_prod(par.size(), par, val);
  assert(path_prod.prod(4) == 8);

  path_prod.set(1, 10);
  assert(path_prod.get(1) == 10);
  assert(path_prod.prod(4) == 16);
}

// Test focus: a custom linear RMQ and custom forward/reverse segment trees compose correctly.
void test_custom_rmq()
{
  const vc<int> par = {-1, 0, 0, 1, 1, 2};
  const vl val = {1, 2, 3, 4, 5, 6};

  using G = GroupAddSub<ll>;
  using OppG = OppositeGroup<G>;
  using RMQ = LinearRMQ<MonoidMin<ull>>;
  PathProd<G, true, SegmentTree<G>, SegmentTree<OppG>, RMQ> path_prod(par.size(), par, val);
  assert(path_prod.prod(3, 5) == 16);
}

int main()
{
  test_without_lca();
  test_custom_rmq();

  cout << "Hello World" << endl;
}
