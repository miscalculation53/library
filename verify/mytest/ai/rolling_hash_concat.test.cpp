#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "string/rolling_hash.hpp"

ll brute_lcp(const string &a, const string &b)
{
  ll i = 0;
  while (i < SZ(a) && i < SZ(b) && a[i] == b[i]) i++;
  return i;
}

ll brute_lcs(const string &a, const string &b)
{
  ll i = 0;
  while (i < SZ(a) && i < SZ(b) && a[SZ(a) - 1 - i] == b[SZ(b) - 1 - i]) i++;
  return i;
}

// Test focus: source slices can be appended and queried without materializing the concatenated string.
void test_concat()
{
  string src = "abracadabra";
  RollingHash rh(src);
  RollingHashConcat cat = rh.substr(1, 3);
  string actual = src.substr(1, 2);
  for (auto [l, r] : vc<pair<int, int>>{{3, 7}, {0, 4}, {7, 11}, {2, 2}, {1, 5}})
  {
    cat += rh.substr(l, r);
    actual += src.substr(l, r - l);
  }
  cat += rh;
  actual += src;

  assert(cat.size() == SZ(actual));
  assert(cat.content() == actual);
  assert(cat.hash() == RollingHashValue(actual));
  rep(i, SZ(actual)) assert(cat.get(i) == actual[i]);
  rep(l, SZ(actual) + 1) rep(r, l, SZ(actual) + 1)
  {
    assert(cat.hash(l, r) == RollingHashValue(actual.substr(l, r - l)));
  }

  string other = "abracadabracadabra";
  RollingHash rh_other(other);
  auto x = cat.substr(1, cat.size() - 1);
  auto y = rh_other.substr(1, other.size() - 1);
  string sx = actual.substr(1, actual.size() - 2), sy = other.substr(1, other.size() - 2);
  assert(x.content() == sx);
  assert(y.content() == sy);
  assert(x.lcp(y) == brute_lcp(sx, sy));
  assert(x.lcs(y) == brute_lcs(sx, sy));
  assert(x.compare(y) == (sx < sy ? -1 : sx > sy ? 1 : 0));
}

// Test focus: one concatenation can mix ranges owned by different source strings.
void test_multiple_sources()
{
  string a = "abcdef", b = "012345", c = "xyz";
  RollingHash ra(a), rb(b), rc(c);
  RollingHashConcat cat;
  cat += ra.substr(1, 4);
  cat += rb.substr(2, 6);
  cat += rc;
  cat += ra.substr(0, 2);
  string actual = "bcd2345xyzab";
  assert(cat.size() == SZ(actual));
  assert(cat.hash() == RollingHashValue(actual));
  rep(i, SZ(actual)) assert(cat.get(i) == actual[i]);
  rep(l, SZ(actual) + 1) rep(r, l, SZ(actual) + 1)
  {
    assert(cat.hash(l, r) == RollingHashValue(actual.substr(l, r - l)));
  }

  auto mixed = ra.substr(2, 5) + rb.substr(1, 4) + rc.substr() + ra.substr(0, 2);
  assert(mixed.hash() == RollingHashValue(string("cde123xyzab")));
  assert(mixed.content() == "cde123xyzab");
  assert(mixed.substr(2, 9).content() == "e123xyz");

  auto grouped = (ra.substr(0, 2) + rb.substr(0, 2)) +
                 (rc.substr(0, 2) + ra.substr(4, 6));
  assert(grouped.content() == "ab01xyef");

  RollingHashConcat empty1, empty2;
  assert(empty1.content().empty());
  assert(empty1 == empty2);
  assert(empty1 < cat);
}

// Test focus: a two-range view supports XY/YX comparisons without copying either range.
void test_view()
{
  string s = "mississippi";
  RollingHash rh(s);
  auto x = rh.substr(1, 5), y = rh.substr(6, 11);
  auto xy = x + y, yx = y + x;
  string sx = s.substr(1, 4), sy = s.substr(6, 5);
  assert(xy.hash() == RollingHashValue(sx + sy));
  assert(yx.hash() == RollingHashValue(sy + sx));
  assert(xy.compare(yx) == (sx + sy < sy + sx ? -1 : sx + sy > sy + sx ? 1 : 0));
  assert(xy.substr(2, 7).hash() == RollingHashValue((sx + sy).substr(2, 5)));
}

// Test focus: an owning concatenation can retain the best of differently shaped lightweight views.
void test_assign_view()
{
  string s = "dpdppd", t = "ddpdpd";
  RollingHash rs(s), rt(t);
  RollingHashConcat best = rs.substr();
  auto candidate = rs.substr(0, 1) + rt.substr(1, 5) + rs.substr(5, 6);
  chmin(best, candidate);
  assert(best.content() == min(s, string("ddpdpd")));

  auto from_concat = best.substr(1, 5) + rs.substr(0, 1);
  best = from_concat;
  assert(best.content() == "dpdpd");
}

// Test focus: value concatenation stores powers in the value and remains valid for huge logical lengths.
void test_large_value()
{
  RollingHashValue x(string("a"));
  rep(_, 40) x += x;
  assert(x.size() == (1LL << 40));
  RollingHashValue y(string("b"));
  assert((x + y).remove_prefix(x) == y);
}

// Test focus: non-string sequences and an explicitly selected modint use the same interface.
void test_generic_sequence()
{
  vc<int> a = {3, 1, 4, 1, 5};
  RollingHash<vc<int>, modint998244353, 1> rh(a);
  vc<int> b = {1, 4, 1};
  assert(rh.content() == a);
  assert(rh.substr(1, 4).content() == b);
  assert((rh.hash(1, 4) == RollingHashValue<modint998244353, 1>(b)));

  RollingHashConcat cat = rh.substr(1, 4);
  assert(cat.content() == b);
}

int main()
{
  test_concat();
  test_multiple_sources();
  test_view();
  test_assign_view();
  test_large_value();
  test_generic_sequence();
  PRINT("Hello World");
}
