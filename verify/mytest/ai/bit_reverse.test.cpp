#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "bit/bit_reverse.hpp"

// Test focus: zero-width input and reversal of exactly the requested low bits.
int main()
{
  assert(bitrev(1, 0) == 0);
  assert(bitrev(8, 3) == 6);
  PRINT("Hello World");
}
