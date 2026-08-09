#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/svp2d.hpp"

// Test focus: a linearly dependent two-vector basis still returns its shortest vector.
int main()
{
  auto v = svp2d(pair{2LL, 0LL}, pair{4LL, 0LL});
  assert((v == pair<ll, ll>{2, 0}));
  PRINT("Hello World");
}
