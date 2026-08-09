#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "ds/cumulative_sum/imos_2d.hpp"

// Test focus: boundary/empty rectangles and matrices with a zero-sized dimension are no-ops.
void test()
{
  Imos2D<> imos(2, 3);
  imos.add(2, 2, 0, 3, 10);
  imos.add(0, 2, 3, 3, 10);
  imos.add(1, 1, 1, 2, 10);
  assert(imos.content() == vvl(2, vl(3)));

  imos.add(0, 2, 1, 3, 4);
  assert(imos.content() == vvl({{0, 4, 4}, {0, 4, 4}}));

  Imos2D<> zero_rows(0, 3);
  zero_rows.add(0, 0, 0, 3, 10);
  assert(zero_rows.content().empty());

  Imos2D<> zero_columns(2, 0);
  zero_columns.add(0, 2, 0, 0, 10);
  assert(zero_columns.content() == vvl(2));
}

int main()
{
  test();

  cout << "Hello World" << endl;
}
