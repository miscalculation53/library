#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "string/rolling_hash.hpp"

using RH = RollingHash2<string, modint61, 7>;
using Value = RollingHashValue2<modint61, 7>;
using Deque = RollingHashDeque2<string, modint61, 7>;
using Concat = RollingHashConcat2<string, modint61, 7>;

// Test focus: two independently based components agree with direct polynomial evaluation.
void test_hash()
{
  string s = "abracadabra";
  RH rh(s);
  static_assert(is_same_v<decltype(rh.hash_value(0, 0)), array<modint61, 2>>);
  auto base = RH::base();
  assert(base[0] != base[1]);

  rep(l, SZ(s) + 1) rep(r, l, SZ(s) + 1)
  {
    array<modint61, 2> expected = {};
    rep(i, l, r) rep(j, 2) expected[j] = expected[j] * base[j] + s[i];
    assert(rh.hash_value(l, r) == expected);
    assert(rh.hash(l, r) == Value(s.substr(l, r - l)));
  }
  assert(rh.lcp("abracad") == 7);
  assert(rh < string("abrb"));
}

// Test focus: views, owning concatenations, and double-ended updates preserve both hashes.
void test_sequences()
{
  string s = "abcdef", t = "XYZ";
  RH rh(s);
  Deque deq(t);
  deq.push_front(rh.substr(1, 4));
  deq += "12";
  assert(deq.content() == "bcdXYZ12");
  assert(deq.hash() == Value("bcdXYZ12"));

  auto view = "0" + rh.substr(2, 5) + rh.substr(0, 2);
  assert(view.content() == "0cdeab");
  assert(view.hash() == Value("0cdeab"));

  Concat cat = "{";
  chmin(cat, view);
  assert(cat.content() == "0cdeab");
  assert(cat == string("0cdeab"));
}

int main()
{
  test_hash();
  test_sequences();
  PRINT("Hello World");
}
