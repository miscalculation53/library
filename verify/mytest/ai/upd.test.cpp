#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"

#include "algebra/upd.hpp"

// Test focus: update composition keeps the later non-identity assignment in both representations.
void test()
{
  using M = MonoidUpd<int, -1>;
  assert(M::op(M::e(), 3) == 3);
  assert(M::op(5, 3) == 5);

  using MF = MonoidUpdFlag<int>;
  MF::S id, three(3), five(5);
  auto f = MF::op(id, three);
  assert(!f.is_id && f.val == 3);
  f = MF::op(five, three);
  assert(!f.is_id && f.val == 5);
}

int main()
{
  test();
  cout << "Hello World" << endl;
}
