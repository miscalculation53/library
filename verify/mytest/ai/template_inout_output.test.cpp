#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_inout.hpp"

// Test focus: tuple streaming and zero/one/many-argument output formatting are exact.
int main()
{
  istringstream input("-170141183460469231731687303715884105728 0 -0");
  i128 x = 42;
  input >> x;
  assert(x == numeric_limits<i128>::lowest());
  input >> x;
  assert(x == 0);
  input >> x;
  assert(x == 0);
  x = 42;
  input >> x;
  assert(input.fail() && x == 42);

  ostringstream os;
  os << tuple{1, string("a"), 2};
  assert(os.str() == "1 a 2");

  ostringstream out;
  auto *buf = cout.rdbuf(out.rdbuf());
  internal::COUTW();
  internal::COUTW(1, ":", 2);
  internal::COUTP();
  internal::COUTP(3);
  internal::COUTP(4, 5);
  cout.rdbuf(buf);
  assert(out.str() == "1:2\n3\n4 5\n");

  PRINT("Hello World");
}
