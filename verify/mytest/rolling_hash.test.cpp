#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "string/rolling_hash.hpp"
#include "string/lcp_compare.hpp"

using mint = modint61;

void test1()
{
  dump(RollingHash<>::base());
  rep(i, 100)
  {
    string a, b;
    rep(_, randint(0, 1)) a += 'a' + randrange(0, 3);
    rep(_, randint(0, 1)) b += 'a' + randrange(0, 3);
    auto rh1 = RollingHash(a) + RollingHash(b);
    auto rh2 = RollingHash(a + b);
    if (rh1 != rh2)
    {
      dump(RollingHash<>::base());
      dump(a, RollingHash(a).size(), RollingHash(a).hash());
      dump(b, RollingHash(b).size(), RollingHash(b).hash());
      dump(a + b, RollingHash(a + b).size(), RollingHash(a + b).hash());
      dump(rh1.size(), rh1.hash());
      assert(false);
    }
  }
}

void test2()
{
  dump(RollingHash<>::base());
  rep(i, 100)
  {
    string a, b, c;
    rep(_, randint(0, 1)) a += 'a' + randrange(0, 3);
    rep(_, randint(0, 1)) b += 'a' + randrange(0, 3);
    rep(_, randint(0, 1)) c += 'a' + randrange(0, 3);
    auto rh1 = RollingHash(a) + RollingHash(b) + RollingHash(c);
    auto rh2 = RollingHash(a + b + c);
    if (rh1 != rh2)
    {
      dump(RollingHash<>::base());
      dump(a, RollingHash(a).size(), RollingHash(a).hash());
      dump(b, RollingHash(b).size(), RollingHash(b).hash());
      dump(c, RollingHash(c).size(), RollingHash(c).hash());
      dump(a + b + c, RollingHash(a + b + c).size(), RollingHash(a + b + c).hash());
      dump(rh1.size(), rh1.hash());
      assert(false);
    }
  }
}

void test3()
{
  dump(RollingHashSubstring<>::base());
  string s;
  rep(_, randint(0, 3)) s += 'a' + randrange(0, 3);
  RollingHashSubstring<mint> rh(s);
  rep(i, 10)
  {
    string add;
    rep(_, randint(0, 3)) add += 'a' + randrange(0, 3);
    s += add;
    rh.push_back(add);

    ll n = s.size();
    rep(l1, n) rep(r1, l1, n + 1) rep(l2, n) rep(r2, l2, n + 1)
    {
      string t = s.substr(l1, r1 - l1), u = s.substr(l2, r2 - l2);
      ll god = t < u ? -1 : t == u ? 0 : 1;
      ll ans = compare_substr_rh(s, rh, l1, r1, s, rh, l2, r2);
      if (god != ans)
      {
        dump(s);
        dump(l1, r1, t);
        dump(l2, r2, u);
        dump(god, ans);
        dump(rh.hash(l1, r1), rh.hash(l2, r2));
        assert(false);
      }
    }
  }
}

int main()
{
  rep(t, 100)
  {
    dump(t);
    test1();
  }
  rep(t, 100)
  {
    dump(t);
    test2();
  }
  rep(t, 10)
  {
    dump(t);
    test3();
  }
  PRINT("Hello World");
}
