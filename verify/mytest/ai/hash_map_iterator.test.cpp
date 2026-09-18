#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/default_map.hpp"

struct IdentityHash
{
  size_t operator()(int x) const { return x; }
};

struct CollisionHash
{
  size_t operator()(int x) const { return x % 8; }
};

template <class Map>
void check(const Map &mp, const map<int, int> &expected)
{
  map<int, int> actual;
  for (const auto &[key, val] : mp)
    assert(actual.emplace(key, val).second);
  assert(actual == expected && mp.size() == expected.size());
  assert(size_t(distance(mp.cbegin(), mp.cend())) == mp.size());
}

// Test focus: range-for supports copies, mutable references, and const references.
void test_iteration()
{
  using Map = HashMap<int, int, IdentityHash>;
  using Iter = Map::iterator;
  using ConstIter = Map::const_iterator;
  static_assert(is_same_v<decltype(*declval<Iter>()), pair<int, int> &>);
  static_assert(is_same_v<decltype(*declval<ConstIter>()), const pair<int, int> &>);
  static_assert(is_convertible_v<Iter, ConstIter>);
  static_assert(!is_convertible_v<ConstIter, Iter>);
#if __cplusplus >= 202002L
  static_assert(forward_iterator<Iter> && forward_iterator<ConstIter>);
#endif

  Map mp;
  const Map &cmp = mp;
  assert(mp.begin() == mp.end() && cmp.begin() == cmp.end());
  assert(mp.cbegin() == mp.cend());
  assert(Iter{} == Iter{} && ConstIter{} == ConstIter{});
  mp.reserve(100);
  check(mp, {});
  assert(mp.begin() == mp.end());

  map<int, int> expected;
  for (int key : {0, 4, 6, 14, 22, 255})
    mp[key] = expected[key] = key * 10;
  check(mp, expected);

  for (auto [key, val] : mp)
  {
    val++;
    assert(mp.at(key) == val - 1);
  }
  check(mp, expected);

  for (auto &[key, val] : mp)
    val += 5;
  for (auto &[key, val] : expected)
    val += 5;
  check(mp, expected);

  auto it = mp.begin();
  auto saved = it;
  ConstIter cit = it;
  assert(it == cit && cit == it);
  assert(!(it != cit) && !(cit != it));
  auto old = it++;
  assert(old == saved && saved == mp.begin());
  assert(it != cit && cit != it);
  ++cit;
  assert(cit == it);
  assert(&saved->second == mp.find_ptr(saved->first));
  saved->second = -100;
  expected[saved->first] = -100;
  check(mp, expected);

  vector<pair<int, int>> entries(mp.begin(), mp.end());
  map<int, int> copied(entries.begin(), entries.end());
  assert(copied == expected);
  Map another = mp;
  assert(mp.begin() != another.begin() && mp.end() != another.end());
  check(another, expected);

  mp.clear();
  assert(mp.begin() == mp.end());
  check(mp, {});
  mp[255] = 7;
  auto last = mp.begin();
  assert(last->first == 255 && last->second == 7);
  assert(++last == mp.end());
}

// Test focus: collision chains, backward-shift deletion, growth, and reserve preserve iteration.
void test_collisions()
{
  HashMap<int, int, IdentityHash> wrap;
  wrap[6] = 60;
  wrap[14] = 140;
  wrap[22] = 220;
  check(wrap, {{6, 60}, {14, 140}, {22, 220}});
  assert(wrap.erase(6));
  check(wrap, {{14, 140}, {22, 220}});

  HashMap<int, int, CollisionHash> mp;
  map<int, int> expected;
  mt19937 rng(20260908);
  repi(iter, 5000)
  {
    int key = rng() % 128;
    int type = rng() % 4;
    if (type == 0)
    {
      assert(mp.erase(key) == (expected.erase(key) != 0));
    }
    else if (type == 1)
    {
      int val = rng() % 10000;
      mp.insert_or_assign(key, val);
      expected[key] = val;
    }
    else
    {
      mp[key]++;
      expected[key]++;
    }
    if (iter % 1000 == 999)
      mp.reserve(iter);
    if (iter % 31 == 0)
      check(mp, expected);
  }
  check(mp, expected);
}

int factory_calls = 0;
int make_default() { return ++factory_calls; }

// Test focus: DefaultHashMap iteration only visits stored keys and never calls the factory.
void test_default_hash_map()
{
  DefaultHashMap<int, int, make_default> mp;
  assert(mp.begin() == mp.end() && factory_calls == 0);
  assert(mp[1] == 1 && mp.at(2) == 2);
  check(mp, {{1, 1}, {2, 2}});
  for (auto &[key, val] : mp)
    val += 10;
  check(mp, {{1, 11}, {2, 12}});
  assert(mp.size() == 2 && factory_calls == 2);
}

int main()
{
  test_iteration();
  test_collisions();
  test_default_hash_map();
  cout << "Hello World" << endl;
}
