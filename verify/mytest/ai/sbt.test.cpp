#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/sbt.hpp"

template <class T>
void check_equal(const SBTNode<T> &a, const SBTNode<T> &b)
{
  assert(tie(a.p, a.q, a.r, a.s) == tie(b.p, b.q, b.r, b.s));
}

template <class T>
void test_nodes()
{
  for (int a = 1; a <= 50; ++a)
    for (int b = 1; b <= 50; ++b)
    {
      SBTNode<T> expected;
      T depth = 0;
      while (expected.num() * b != expected.den() * a)
      {
        expected.descend(expected.num() * b < expected.den() * a ? 'R' : 'L', 1);
        ++depth;
      }
      SBTNode<T> node{T(a), T(b)};
      check_equal(node, expected);
      assert(node.depth() == depth);
      check_equal(SBTNode<T>(node.encode_path()), expected);
      auto ancestor = node;
      ancestor.ascend(depth);
      check_equal(ancestor, SBTNode<T>());
      check_equal(node.lca(ancestor), ancestor);
    }

  const T m = numeric_limits<T>::max();
  const array<pair<T, T>, 5> fractions = {{{m, 1}, {1, m}, {m, m - 1}, {m - 1, m}, {m, m}}};
  const array<SBTNode<T>, 5> expected = {
      SBTNode<T>(m - 1, 1, 1, 0), SBTNode<T>(0, 1, 1, m - 1),
      SBTNode<T>(1, 1, m - 1, m - 2), SBTNode<T>(m - 2, m - 1, 1, 1), SBTNode<T>()};
  for (int i = 0; i < 5; ++i)
  {
    SBTNode<T> node(fractions[i].first, fractions[i].second);
    check_equal(node, expected[i]);
    check_equal(SBTNode<T>(node.encode_path()), expected[i]);
    assert(node.depth() == (i == 4 ? T(0) : m - 1));
    node.ascend(node.depth());
    check_equal(node, SBTNode<T>());
  }
}

template <class T>
SBTNode<T> check_search(T cap, T x, T y, bool strict, bool invert)
{
  auto judge = [&](T a, T b) -> bool
  {
    assert(a >= 0 && a <= cap && b >= 0 && b <= cap);
    const u128 lhs = u128(a) * y, rhs = u128(b) * x;
    return (strict ? lhs < rhs : lhs <= rhs) ^ invert;
  };
  auto node = sbt_search<T>(judge, cap);
  assert(judge(node.p, node.q) == judge(0, 1));
  assert(judge(node.r, node.s) == judge(1, 0));
  assert(u128(node.r) * node.q - u128(node.p) * node.s == 1);
  assert(node.p > cap - node.r || node.q > cap - node.s);
  return node;
}

void test_search_exhaustive()
{
  for (int cap = 1; cap <= 16; ++cap)
    for (int x = 0; x <= 35; ++x)
      for (int y = 0; y <= 35; ++y)
        for (bool strict : {false, true})
        {
          auto judge = [&](int a, int b) { return strict ? a * y < b * x : a * y <= b * x; };
          if (judge(0, 1) == judge(1, 0)) continue;
          SBTNode<int> expected;
          for (int a = 0; a <= cap; ++a)
            for (int b = 0; b <= cap; ++b)
            {
              if (gcd(a, b) != 1) continue;
              if (judge(a, b) == judge(0, 1))
              {
                if (a * expected.q > b * expected.p) expected.p = a, expected.q = b;
              }
              else if (a * expected.s < b * expected.r) expected.r = a, expected.s = b;
            }
          for (bool invert : {false, true})
            check_equal(check_search(cap, x, y, strict, invert), expected);
        }
}

template <class T>
void test_search_large()
{
  const T m = numeric_limits<T>::max();
  for (T cap : {T(1), T(2), T(m / 2), T(m - 1), m})
    for (T x : {T(1), T(2), T(m - 1), m})
      for (T y : {T(1), T(2), T(m - 1), m})
        for (bool strict : {false, true})
          for (bool invert : {false, true})
            check_search(cap, x, y, strict, invert);
  mt19937_64 rng(20261003);
  for (int i = 0; i < 25000; ++i)
  {
    T cap = 1 + rng() % ull(m), x = 1 + rng() % ull(m), y = 1 + rng() % ull(m);
    check_search(cap, x, y, bool(i & 1), bool(i & 2));
  }
}

template <class T>
void test_search_128()
{
  const T m = numeric_limits<T>::max();
  auto below_half = [](T a, T b) { return a <= b / 2; };
  check_equal(sbt_search<T>(below_half, m), SBTNode<T>(1, 2, m / 2 + 1, m));
  check_equal(sbt_search<T>([](T a, T b) { return a == 0 && b != 0; }, m), SBTNode<T>(0, 1, 1, m));
  check_equal(sbt_search<T>([](T, T b) { return b != 0; }, m), SBTNode<T>(m, 1, 1, 0));
}

int main()
{
  test_nodes<int>();
  test_nodes<uint>();
  test_nodes<ll>();
  test_nodes<ull>();
  test_nodes<i128>();
  test_nodes<u128>();
  test_search_exhaustive();
  test_search_large<int>();
  test_search_large<uint>();
  test_search_large<ll>();
  test_search_large<ull>();
  test_search_128<i128>();
  test_search_128<u128>();
  PRINT("Hello World");
}
