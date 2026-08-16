#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "algebra/algebra_basic_ops.hpp"

// Test focus: componentwise subtraction and negation work for pair, array, and tuple,
// including the GroupAddSub<pll> use case.
int main()
{
  pll p{7, -2}, q{3, 5};
  auto p_add = p + q;
  auto p_sub = p - q;
  auto p_neg = -p;
  assert((p == pll{7, -2}));
  assert((p_add == pll{10, 3}));
  assert((p_sub == pll{4, -7}));
  assert((p_neg == pll{-7, 2}));
  p -= q;
  assert((p == pll{4, -7}));
  assert((GroupAddSub<pll>::op(p, q) == pll{7, -2}));
  assert((GroupAddSub<pll>::inv(q) == pll{-3, -5}));

  array<int, 3> a{7, -2, 4}, b{3, 5, -6};
  auto a_add = a + b;
  auto a_sub = a - b;
  auto a_neg = -a;
  assert((a == array<int, 3>{7, -2, 4}));
  assert((a_add == array<int, 3>{10, 3, -2}));
  assert((a_sub == array<int, 3>{4, -7, 10}));
  assert((a_neg == array<int, 3>{-7, 2, -4}));
  a -= b;
  assert((a == array<int, 3>{4, -7, 10}));

  tuple<int, ll, int> x{7, -2, 4}, y{3, 5, -6};
  auto x_add = x + y;
  auto x_sub = x - y;
  auto x_neg = -x;
  assert((x == tuple<int, ll, int>{7, -2, 4}));
  assert((x_add == tuple<int, ll, int>{10, 3, -2}));
  assert((x_sub == tuple<int, ll, int>{4, -7, 10}));
  assert((x_neg == tuple<int, ll, int>{-7, 2, -4}));
  x -= y;
  assert((x == tuple<int, ll, int>{4, -7, 10}));

  PRINT("Hello World");
}
