#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_dump.hpp"

int main()
{
  int x = 0;
  auto increment = [&]() { x += 1, dump(x); };
  increment();
  assert(x == 1);
  int y = (dump(x), x + 1);
  assert(y == 2);

#ifndef LOCAL
  dump(++x);
  x += 1, dump(++x);
  assert(x == 2);
  dump(local_only_variable | cp::hex());
#endif

  cout << "Hello World" << endl;
}
