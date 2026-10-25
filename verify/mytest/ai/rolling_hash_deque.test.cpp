#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "string/rolling_hash.hpp"

// Test focus: all owning rolling-hash types can be initialized from strings and string literals.
void test_string_constructor()
{
  string s = "abc";
  RollingHashValue val1 = s, val2 = "abc";
  RollingHash rh1 = s, rh2 = "abc";
  RollingHashConcat cat1 = s, cat2 = "abc", cat3 = rh1;
  RollingHashDeque deq1 = s, deq2 = "abc";

  assert(val1 == val2);
  assert(rh1.content() == s && rh2.content() == s);
  assert(cat1.content() == s && cat2.content() == s && cat3.content() == s);
  assert(deq1.content() == s && deq2.content() == s);

  RollingHashConcat saved;
  {
    RollingHashConcat owned = "xyz";
    saved = owned.substr() + owned.substr(0, 0);
  }
  assert(saved.content() == "xyz");
}

// Test focus: mixed double-ended updates preserve every substring hash.
void test_random_updates()
{
  rep(tc, 100)
  {
    RollingHashDeque deq;
    string s;
    rep(q, 100)
    {
      int t = randrange(0, s.empty() ? 2 : 4);
      char c = 'a' + randrange(0, 3);
      if (t == 0) deq.push_front(c), s.insert(s.begin(), c);
      if (t == 1) deq.push_back(c), s.push_back(c);
      if (t == 2) deq.pop_front(), s.erase(s.begin());
      if (t == 3) deq.pop_back(), s.pop_back();

      assert(deq.content() == s);
      assert(deq.hash() == RollingHashValue(s));
      rep(l, SZ(s) + 1) rep(r, l, SZ(s) + 1)
      {
        assert(deq.hash(l, r) == RollingHashValue(s.substr(l, r - l)));
      }
    }
  }
}

// Test focus: range overloads preserve order when pushing to either end.
void test_ranges()
{
  string s = "abcdef", t = "XYZ";
  RollingHash rh(s);
  RollingHashDeque deq(t);
  deq.push_front(rh.substr(1, 4));
  deq.push_back(rh.substr(4));
  deq.push_front(string("01"));
  deq += string("23");
  assert(deq.content() == "01bcdXYZef23");

  auto x = rh.substr(0, 3) + deq;
  assert(x.content() == "abc01bcdXYZef23");
  assert(x.hash() == RollingHashValue(x.content()));

  string before = deq.content();
  deq.push_front(deq);
  assert(deq.content() == before + before);
  before = deq.content();
  deq.push_back(deq);
  assert(deq.content() == before + before);
  deq.clear();
  assert(deq.empty() && deq.hash() == RollingHashValue());
}

// Test focus: repeated S + answer comparisons use O(1) substring hashes after each front insertion.
void test_push_front_dp()
{
  vc<string> s = {"cba", "cb", "c"};
  vc<RollingHash<>> rh;
  for (const string &x : s) rh.emplace_back(x);
  RollingHashDeque ans;
  rep(_, 2)
  {
    auto it = min_element(rh.begin(), rh.end(), [&](const auto &x, const auto &y)
    {
      return x + ans < y + ans;
    });
    ans.push_front(*it);
  }
  assert(ans.content() == "cbac");
}

// Test focus: scalar conversions select the one-element overload while ranges select the sequence overload.
void test_push_overload()
{
  RollingHashDeque<vc<ll>> deq;
  deq.push_back(1);
  deq.push_back(vc<int>{2, 3});
  deq.push_front(0);
  deq.push_front(vc<int>{-2, -1});
  deq += 4;
  deq += vc<int>{5, 6};
  assert(deq.content() == vc<ll>({-2, -1, 0, 1, 2, 3, 4, 5, 6}));
}

int main()
{
  test_string_constructor();
  test_random_updates();
  test_ranges();
  test_push_front_dp();
  test_push_overload();
  PRINT("Hello World");
}
