#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "utils/is_integral_ext.hpp"

// Test focus: extended signedness traits classify built-in and 128-bit integer types.
void test()
{
  static_assert(is_unsigned_ext<unsigned int>);
  static_assert(is_unsigned_ext<u128>);
  static_assert(!is_unsigned_ext<int>);
  static_assert(!is_unsigned_ext<i128>);
}

int main()
{
  test();

  cout << "Hello World" << endl;
}
