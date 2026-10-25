#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "algo/rle.hpp"

struct EqOnly
{
  int x;
  friend bool operator==(const EqOnly &a, const EqOnly &b) { return a.x == b.x; }
};

// Test focus: default equality needs only operator==, and a custom equivalence controls
// run values, lengths, and original half-open intervals for vectors and strings.
int main()
{
  {
    vc<EqOnly> a = {{1}, {1}, {2}};
    auto runs = rle(a);
    assert(runs.size() == 2);
    assert(runs[0].val.x == 1 && runs[0].len == 2 && runs[0].left == 0 && runs[0].right == 2);
    assert(runs[1].val.x == 2 && runs[1].len == 1 && runs[1].left == 2 && runs[1].right == 3);
  }
  {
    vc<int> a = {1, 3, 2, 4, 6, 7};
    auto runs = rle(a, [](int x, int y) { return x % 2 == y % 2; });
    assert(runs.size() == 3);
    assert(runs[0].val == 1 && runs[0].len == 2 && runs[0].left == 0 && runs[0].right == 2);
    assert(runs[1].val == 2 && runs[1].len == 3 && runs[1].left == 2 && runs[1].right == 5);
    assert(runs[2].val == 7 && runs[2].len == 1 && runs[2].left == 5 && runs[2].right == 6);
  }
  {
    string s = "aABbC";
    auto runs = rle(s, [](char x, char y)
                    { return tolower((unsigned char)x) == tolower((unsigned char)y); });
    assert(runs.size() == 3);
    assert(runs[0].val == 'a' && runs[0].len == 2);
    assert(runs[1].val == 'B' && runs[1].len == 2);
    assert(runs[2].val == 'C' && runs[2].len == 1);
  }
  PRINT("Hello World");
}
