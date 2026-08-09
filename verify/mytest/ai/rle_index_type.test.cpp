#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "algo/rle.hpp"

// Test focus: the selected index type propagates to rle_info and its offsets.
int main()
{
  auto a = rle<int, int>({1, 1, 2});
  static_assert(is_same_v<decltype(a), vc<rle_info<int, int>>>);
  assert(a.size() == 2 && a[0].len == 2 && a[1].left == 2);
  PRINT("Hello World");
}
