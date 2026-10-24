#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#ifndef SBT_INTERVAL_HEADER
#define SBT_INTERVAL_HEADER "math/sbt.hpp"
#endif
#include SBT_INTERVAL_HEADER
#include "math/bigint.hpp"

namespace
{
using Integer = BigInteger<>;

// 剰余演算と組込み整数への変換を持たない整数型。
struct NoModInteger
{
  Integer value;
  explicit NoModInteger(int x) : value(x) {}
  explicit NoModInteger(const Integer &x) : value(x) {}
  NoModInteger(const NoModInteger &) = default;
  NoModInteger &operator=(const NoModInteger &) = default;
  friend NoModInteger operator+(const NoModInteger &a, const NoModInteger &b)
  { return NoModInteger(a.value + b.value); }
  friend NoModInteger operator-(const NoModInteger &a, const NoModInteger &b)
  { assert(a.value >= b.value); return NoModInteger(a.value - b.value); }
  friend NoModInteger operator*(const NoModInteger &a, const NoModInteger &b)
  { return NoModInteger(a.value * b.value); }
  friend NoModInteger operator/(const NoModInteger &a, const NoModInteger &b)
  { return NoModInteger(a.value / b.value); }
  friend bool operator<(const NoModInteger &a, const NoModInteger &b) { return a.value < b.value; }
  friend bool operator>(const NoModInteger &a, const NoModInteger &b) { return a.value > b.value; }
  friend bool operator==(const NoModInteger &a, const NoModInteger &b) { return a.value == b.value; }
};

template <class T>
T convert(const Integer &x)
{
  if constexpr (is_same_v<T, Integer>) return x;
  else return NoModInteger(x);
}

template <class T>
void check(const Integer &a, const Integer &b, const Integer &c, const Integer &d,
           const Integer &p, const Integer &q)
{
  const auto answer = abc408g<T>(convert<T>(a), convert<T>(b), convert<T>(c), convert<T>(d));
  static_assert(is_same_v<decltype(answer), const pair<T, T>>);
  assert(answer.first == convert<T>(p));
  assert(answer.second == convert<T>(q));
}

template <class T>
void generic_cases()
{
  check<T>(1, 2, 2, 3, 3, 5);
  check<T>(0, 1, 1, 1, 1, 2);
  check<T>(1, 2, 3, 2, 1, 1);
  check<T>(1, 2, 7, 0, 1, 1);
  check<T>(1, 2, 1, 1, 2, 3);
  check<T>(0, 5, 7, 0, 1, 1);
  check<T>(2, 4, 4, 6, 3, 5);
  check<T>(1, 1, 2, 1, 3, 2);
  check<T>(5, 2, 9, 2, 3, 1);
  Integer n(1);
  for (int i = 0; i < 79; ++i) n *= 10;
  n += 123;
  check<T>(0, 1, 1, n, 1, n + 1);
  check<T>(n - 1, n, 1, 1, n, n + 1);
  check<T>(n, 1, 1, 0, n + 1, 1);
  check<T>(n - 1, 1, n, 1, 2 * n - 1, 2);
  check<T>(2 * n, 5 * n, 4 * (n + 7), 9 * (n + 7), 3, 7);
}
}

int main()
{
  generic_cases<Integer>();
  generic_cases<NoModInteger>();
  cout << "Hello World\n";
}
