#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "ds/cumulative_sum/imos.hpp"

// Test focus: empty intervals, an interval ending at n, and a zero-length array are no-ops.
void test()
{
  Imos<> imos(3);
  imos.add(3, 3, 10);
  imos.add(1, 1, 10);
  assert(imos.content() == vl({0, 0, 0}));

  imos.add(0, 3, 2);
  assert(imos.content() == vl({2, 2, 2}));

  Imos<> empty(0);
  empty.add(0, 0, 10);
  assert(empty.content().empty());
}

int main()
{
  test();

  cout << "Hello World" << endl;
}
