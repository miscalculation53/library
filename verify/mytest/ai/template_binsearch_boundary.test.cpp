#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_binsearch.hpp"

// Test focus: endpoint arithmetic is overflow-safe and exponential search grows both ways.
int main()
{
  auto [ok1, ng1] = binsearch<ll>(LMD(x, x <= 0), numeric_limits<ll>::lowest(), numeric_limits<ll>::max());
  assert(ok1 == 0 && ng1 == 1);
  auto [ok2, ng2] = expsearch<ll>(LMD(x, x < 1'000'000'000'000LL), 0, true);
  assert(ok2 == 999'999'999'999LL && ng2 == 1'000'000'000'000LL);
  auto [ok3, ng3] = expsearch<ll>(LMD(x, x > -1'000'000'000'000LL), 0, false);
  assert(ok3 == -999'999'999'999LL && ng3 == -1'000'000'000'000LL);
  cout << "Hello World" << endl;
}
