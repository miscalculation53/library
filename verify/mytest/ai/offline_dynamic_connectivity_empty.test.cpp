#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/offline_dynamic_connectivity.hpp"

// Test focus: an empty time axis invokes no callback.
int main()
{
  int called = 0;
  OfflineDynamicConnectivity<int> dc(0);
  dc.run([](int) {}, [] {}, [&](int) { called++; });
  assert(called == 0);
  PRINT("Hello World");
}
