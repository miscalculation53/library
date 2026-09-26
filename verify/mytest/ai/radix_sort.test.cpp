#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "algo/radix_sort.hpp"

template <class I>
void check(const vc<I> &a)
{
  auto expected = permid<int>(a.size());
  stable_sort(ALL(expected), [&](int i, int j) { return a[i] < a[j]; });
  assert(radix_argsort(a) == expected);
  auto sorted = a;
  radix_sort(sorted);
  repi(i, a.size()) assert(sorted[i] == a[expected[i]]);

  vc<pair<I, int>> records;
  repi(i, a.size()) records.emplace_back(a[i], i);
  int calls = 0;
  auto key = [&](const pair<I, int> &x) -> const I & { calls++; return x.first; };
  assert(radix_argsort(records, key) == expected);
  assert(calls <= int(a.size()));
  radix_sort(records, key);
  repi(i, a.size()) assert(records[i] == make_pair(a[expected[i]], expected[i]));
}

template <class I>
void test_type()
{
  mt19937_64 rng(8128);
  check<I>({});
  check<I>({numeric_limits<I>::lowest()});
  check<I>({numeric_limits<I>::max(), 0, numeric_limits<I>::lowest(), 1, 0});
  for (int n : {2, 31, 32, 33, 255, 256, 1000, 65535, 65536, 65537})
  {
    vc<I> a(n);
    for (I &x : a)
    {
      if constexpr (sizeof(I) > 8)
        x = I((u128(rng()) << 64) | rng());
      else
        x = I(rng());
    }
    a[0] = numeric_limits<I>::lowest(), a[1] = numeric_limits<I>::max();
    check(a);
    for (I &x : a) x = I(int(rng() % 13) - 6);
    check(a);
    sort(ALL(a));
    check(a);
    reverse(ALL(a));
    check(a);
    fill(ALL(a), numeric_limits<I>::max());
    check(a);
  }
}

void test_constant_digits()
{
  mt19937_64 rng(57721);
  for (int n : {1000, 70000})
  {
    vc<ull> a(n);
    // Lower digits and upper digits are constant; only the middle byte varies.
    for (ull &x : a) x = 0xfedc00000000abcdULL | ((rng() % 256) << 32);
    check(a);
    vc<u128> b(n);
    for (u128 &x : b) x = (u128(1) << 127) | (u128(rng() % 256) << 80) | 123;
    check(b);
  }
}

struct MoveOnly
{
  ll key;
  unique_ptr<int> id;
  MoveOnly(ll key, int id) : key(key), id(make_unique<int>(id)) {}
  MoveOnly(MoveOnly &&) = default;
  MoveOnly &operator=(MoveOnly &&) = default;
};

void test_move_only()
{
  vc<MoveOnly> a;
  vc<pair<ll, int>> expected;
  repi(i, 2000)
  {
    ll key = (i * 17) % 23 - 11;
    a.emplace_back(key, i);
    expected.emplace_back(key, i);
  }
  stable_sort(ALL(expected), [](const auto &x, const auto &y) { return x.first < y.first; });
  radix_sort(a, [](const MoveOnly &x) { return x.key; });
  repi(i, a.size()) assert(a[i].id && make_pair(a[i].key, *a[i].id) == expected[i]);
}

int main()
{
  test_type<signed char>();
  test_type<unsigned char>();
  test_type<short>();
  test_type<unsigned short>();
  test_type<int>();
  test_type<uint>();
  test_type<ll>();
  test_type<ull>();
  test_type<i128>();
  test_type<u128>();
  test_constant_digits();
  test_move_only();
  PRINT("Hello World");
}
