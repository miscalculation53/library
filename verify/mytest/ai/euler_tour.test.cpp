#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "graph/tree/euler_tour.hpp"

// Test focus: the no-LCA specialization still records every vertex depth correctly.
void test()
{
  const vc<int> par = {-1, 0, 0, 1, 1, 2};
  const vl dep = {0, 1, 1, 2, 2, 2};

  EulerTour<false> et(par.size(), par);
  repi(v, par.size()) assert(et.depth(v) == dep[v]);
}

int main()
{
  test();

  cout << "Hello World" << endl;
}
