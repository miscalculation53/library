#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "algebra/min_count.hpp"

// Test focus: equal extrema combine their multiplicities.
void test()
{
  using M = MonoidMaxCount<ll>;
  M::S a{4, 3}, b{4, 2};

  auto x = M::op(a, b);
  assert(x.mx == 4 && x.cnt == 5);
}

int main()
{
  test();

  cout << "Hello World" << endl;
}
