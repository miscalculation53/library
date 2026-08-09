#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/prime/sieve/segmented_sieve.hpp"

// Test focus: a zero-length target interval and repeated calls do not retain state.
int main()
{
  vc<ll> a, b;
  segmented_sieve(94, 94, [&](ll p, ll) { a.eb(p); });
  segmented_sieve(1, 10000, [](ll, ll) {});
  segmented_sieve(94, 94, [&](ll p, ll) { b.eb(p); });
  assert((a == vc<ll>{2, 47}));
  assert(a == b);
  PRINT("Hello World");
}
