#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/default_map.hpp"

ll dflt1() { return INF; }

void test1()
{
  DefaultMap<ll, ll, dflt1> mp;
  assert(mp[0] == INF);
  assert(mp.at(0) == INF);
  chmin(mp[0], 1000);
  assert(mp[0] == 1000);
  chmin(mp.at(0), 100);
  assert(mp[0] == 100);
}

vl dflt2() { return {100, 200}; }

void test2()
{
  DefaultMap<ll, vl, dflt2> mp;
  assert((mp[0] == vl{100, 200}));
  assert((mp.at(0) == vl{100, 200}));
  mp[0].eb(300);
  assert((mp[0] == vl{100, 200, 300}));
  assert((mp.at(0) == vl{100, 200, 300}));
}

int main()
{
  test1();
  test2();

  cout << "Hello World" << endl;
}