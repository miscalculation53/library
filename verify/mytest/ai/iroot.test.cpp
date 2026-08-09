#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_math.hpp"
#include "template/template_inout.hpp"

// Test focus: root comparisons near unsigned overflow boundaries, including u128 input.
int main()
{
  u128 x = u128(1) << 59;
  assert(iroot<u128>(x * x - 1, 2) == x - 1);
  assert(iroot<ull>(numeric_limits<ull>::max(), 2) == 4294967295ULL);
  PRINT("Hello World");
}
