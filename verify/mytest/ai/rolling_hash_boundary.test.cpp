#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "string/rolling_hash.hpp"

// Test focus: empty and full ranges keep their length and hash value consistent.
int main()
{
  string s = "abc";
  RollingHash rh(s);
  assert(rh.size() == 3);
  assert(rh.hash() == RollingHashValue(s));
  assert(rh.substr(0, 0).hash() == RollingHashValue());
  assert(rh.substr().hash() == RollingHashValue(s));
  PRINT("Hello World");
}
