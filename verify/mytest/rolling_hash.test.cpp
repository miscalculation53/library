#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "string/rolling_hash.hpp"

void test_value()
{
  rep(_, 100)
  {
    string a, b, c;
    rep(_, randint(0, 5)) a += 'a' + randrange(0, 3);
    rep(_, randint(0, 5)) b += 'a' + randrange(0, 3);
    rep(_, randint(0, 5)) c += 'a' + randrange(0, 3);
    assert(RollingHashValue(a) + RollingHashValue(b) == RollingHashValue(a + b));
    assert(RollingHashValue(a) + RollingHashValue(b) + RollingHashValue(c) == RollingHashValue(a + b + c));
  }
}

void test_range()
{
  rep(_, 100)
  {
    string s;
    rep(_, randint(0, 10)) s += 'a' + randrange(0, 3);
    RollingHash rh(s);
    rep(l1, SZ(s) + 1) rep(r1, l1, SZ(s) + 1) rep(l2, SZ(s) + 1) rep(r2, l2, SZ(s) + 1)
    {
      string a = s.substr(l1, r1 - l1), b = s.substr(l2, r2 - l2);
      auto x = rh.substr(l1, r1), y = rh.substr(l2, r2);
      assert(x.hash() == RollingHashValue(a));
      assert(x.compare(y) == (a < b ? -1 : a > b ? 1 : 0));
      assert((x < y) == (a < b));
      assert((x == y) == (a == b));
    }
  }
}

int main()
{
  test_value();
  test_range();
  PRINT("Hello World");
}
