#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/uf/uf_undo.hpp"

// Test focus: reset clears snapshot history, so a later rollback returns to initial state.
int main()
{
  UnionFindUndo<UFDataEmpty<>> uf(3);
  uf.merge(0, 1);
  uf.snapshot();
  uf.merge(1, 2);
  uf.reset();
  uf.merge(0, 2);
  uf.rollback();
  assert(!uf.same(0, 1) && !uf.same(0, 2));
  PRINT("Hello World");
}
