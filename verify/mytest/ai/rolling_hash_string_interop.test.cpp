#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "string/rolling_hash.hpp"

// Test focus: strings and string literals can appear on either side of a rolling-hash concatenation.
void test_plus()
{
  string s = "ab", t = "ef";
  RollingHash rh = "cd";
  RollingHashDeque deq = "gh";
  RollingHashConcat cat = "ij";

  assert((s + rh).content() == "abcd");
  assert((rh + t).content() == "cdef");
  assert(("ab" + rh + "ef").content() == "abcdef");
  assert((s + deq + cat + "kl").content() == "abghijkl");

  RollingHashConcat saved;
  {
    string local = "xy";
    saved = local + rh + string("z");
  }
  assert(saved.content() == "xycdz");
}

// Test focus: comparisons and LCP/LCS accept strings in either operand order.
void test_compare()
{
  RollingHash rh = "abca";
  string same = "abca", less = "abb", greater = "abcz";

  assert(rh == same && same == rh);
  assert(!(rh != "abca") && !("abca" != rh));
  assert(less < rh && rh > less);
  assert(rh < greater && greater > rh);
  assert(less <= rh && rh >= less);
  assert(rh <= same && same >= rh);
  assert(rh.lcp("abda") == 2);
  assert(rh.lcs(string("zzca")) == 2);
  assert(rh.compare("abca") == 0);
  assert(rh.compare("abcb") < 0);
}

// Test focus: mutable rolling-hash sequences accept strings and literals directly.
void test_push_range()
{
  RollingHashDeque deq;
  deq += "cd";
  deq.push_front("ab");
  deq.push_back(string("ef"));
  assert(deq.content() == "abcdef");

  RollingHashConcat cat;
  cat += "ab";
  cat += string("cd");
  assert(cat.content() == "abcd");

  RollingHashValue val = "ab";
  assert(val == "ab" && "ab" == val);
  val += "cd";
  assert(val == "abcd");
  val = "ab";
  assert(val + "cd" == RollingHashValue("abcd"));
  assert(val + string("cd") == RollingHashValue("abcd"));
  assert(string("ab") + RollingHashValue("cd") == RollingHashValue("abcd"));
}

// Test focus: single characters concatenate in either order, including NUL and signed char values.
template <class Value>
void test_value_char()
{
  Value forward, backward;
  string s;
  for (int i = 0; i < 256; i++) s += char(i);
  for (int i = 0; i < SZ(s); i++)
  {
    char c = s[i];
    Value single = c;
    assert(single.size() == 1);
    assert(single == Value(string(1, c)));
    assert(forward + c == Value(s.substr(0, i + 1)));
    forward += c;
    backward = c + backward;
    string reversed = s.substr(0, i + 1);
    reverse(reversed.begin(), reversed.end());
    assert(forward == Value(s.substr(0, i + 1)));
    assert(backward == Value(reversed));
    assert((forward + c).remove_prefix(forward) == single);
  }
  assert('[' + forward + ']' == Value("[" + s + "]"));
  assert(string_view("[") + forward + string_view("]") == Value("[" + s + "]"));
}

// Test focus: an owning answer initialized by a sentinel accepts candidates with different expression shapes.
void test_different_number_of_parts()
{
  RollingHash rh = "abcdef";
  RollingHashConcat ans = "{";
  auto two = rh.substr(0, 2) + rh.substr(2, 4);
  auto three = rh.substr(4, 6) + rh.substr(2, 4) + rh.substr(0, 2);
  auto four = rh.substr(0, 0) + rh.substr(1, 3) + rh.substr(3, 5) + rh.substr(5, 6);

  chmin(ans, two);
  chmin(ans, three);
  chmin(ans, four);
  assert(ans.content() == "abcd");
}

int main()
{
  test_plus();
  test_compare();
  test_push_range();
  test_value_char<RollingHashValue<>>();
  test_value_char<RollingHashValue2<modint998244353, 42>>();
  test_different_number_of_parts();
  PRINT("Hello World");
}
