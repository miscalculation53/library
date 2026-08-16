#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"

#include "algebra/min_max_sum.hpp"

// Test focus: each aggregate monoid merges exactly the fields it stores and has an empty identity.
void test()
{
  using ML = MonoidSumWithLength<ll>;
  auto l = ML::op(ML::S(2), ML::S(3, 12));
  assert(l.len == 4 && l.val == 14);

  using MM = MonoidMinMax<ll, 1000>;
  auto mm = MM::op(MM::S(3), MM::S(-2, 7));
  assert(mm.mn == -2 && mm.mx == 7);
  mm = MM::op(MM::e(), mm);
  assert(mm.mn == -2 && mm.mx == 7);

  using Mn = MonoidMinSum<ll, 1000>;
  auto mn = Mn::op(Mn::S(4, 2), Mn::S(-1, 3));
  assert(mn.mn == -1 && mn.sum == 5 && mn.len == 5);

  using Mx = MonoidMaxSum<ll, 1000>;
  auto mx = Mx::op(Mx::S(4, 2), Mx::S(-1, 3));
  assert(mx.mx == 4 && mx.sum == 5 && mx.len == 5);

  using Mnx = MonoidMinMaxSum<ll, 1000>;
  auto mnx = Mnx::op(Mnx::S(4, 2), Mnx::S(-1, 3));
  assert(mnx.mn == -1 && mnx.mx == 4 && mnx.sum == 5 && mnx.len == 5);
}

int main()
{
  test();
  cout << "Hello World" << endl;
}
