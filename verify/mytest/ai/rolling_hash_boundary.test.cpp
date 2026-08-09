#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "string/rolling_hash.hpp"

// Test focus: full-range hashing and repeated pop_back keep the logical size in sync.
int main()
{
  RollingHashSubstring<> rh(string("abc"));
  assert(rh.size() == 3);
  assert(rh.hash(0, rh.size()) == RollingHash(string("abc")));
  rep(_, 3) rh.pop_back();
  assert(rh.size() == 0);
  PRINT("Hello World");
}
