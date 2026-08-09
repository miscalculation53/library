#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/mo/mo.hpp"

// Test focus: zero queries invoke neither window updates nor callbacks.
int main()
{
  int called = 0;
  mo(0, vc<pair<int, int>>{}, [](int, bool) {}, [](int, bool) {}, [&](int) { called++; });
  assert(called == 0);
  PRINT("Hello World");
}
