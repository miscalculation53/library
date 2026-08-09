#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "ds/undo.hpp"

// Test focus: value history supports multi-undo, snapshots, rollback, and reset.
void test()
{
  ValUndo<int> x(0);
  x.set(1);
  x.set(2);
  x.set(3);
  x.undo(2);
  assert(x.get() == 1);

  x.undo(0);
  assert(x.get() == 1);

  x.snapshot();
  x.set(4);
  x.set(5);
  x.rollback();
  assert(x.get() == 1);

  x.reset();
  assert(x.get() == 0);
}

// Test focus: vector reset also invalidates an older snapshot before later rollback.
void test_vector()
{
  VectorUndo<int> v(vc<int>{0});
  v.set(0, 1);
  v.snapshot();
  v.set(0, 2);
  v.reset();
  v.set(0, 3);
  v.rollback();
  assert(v.get(0) == 0);
}

int main()
{
  test();
  test_vector();

  cout << "Hello World" << endl;
}
