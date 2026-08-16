#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "algebra/acted_monoid/add_min_count.hpp"

// Test focus: ties merge their counts, while range addition preserves the count.
void test()
{
  using AM = ActedMonoidAddMaxCount<ll>;
  AM::S a{3, 2}, b{5, 1}, c{5, 4};

  auto x = AM::op(a, b);
  assert(x.mx == 5 && x.cnt == 1);

  x = AM::op(b, c);
  assert(x.mx == 5 && x.cnt == 5);

  x = AM::mapping(7, x);
  assert(x.mx == 12 && x.cnt == 5);
}

int main()
{
  test();

  cout << "Hello World" << endl;
}
