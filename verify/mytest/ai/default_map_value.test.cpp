#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/default_map.hpp"

int factory_calls = 0;

vl make_default()
{
  return {++factory_calls, 200};
}

// Test focus: value and lambda defaults preserve insertion and const lookup semantics.
template <class Map>
void test_scalar()
{
  Map mp;
  const Map &cmp = mp;
  static_assert(is_same_v<decltype(mp[0]), ll &>);
  static_assert(is_same_v<decltype(mp.at(0)), ll &>);
  static_assert(is_same_v<decltype(cmp[0]), ll>);
  static_assert(is_same_v<decltype(cmp.at(0)), ll>);

  assert(cmp[0] == INF && cmp.at(1) == INF);
  assert(mp.empty());
  assert(mp[0] == INF && mp.size() == 1);
  mp[0] = 100;
  assert(mp.at(0) == 100 && cmp[0] == 100 && cmp.at(0) == 100);
  assert(mp.at(1) == INF && mp.size() == 2);
  mp.at(1) = 200;
  assert(mp[1] == 200);
  mp.erase(0);
  assert(cmp[0] == INF && mp.size() == 1);
  assert(mp[0] == INF && mp.size() == 2);
}

// Test focus: factories run for each missing key lookup, never for an existing key.
template <class Map>
void test_factory()
{
  factory_calls = 0;
  Map mp;
  const Map &cmp = mp;
  assert(factory_calls == 0);
  assert((cmp[0] == vl{1, 200}));
  assert((cmp.at(0) == vl{2, 200}));
  assert(mp.empty() && factory_calls == 2);

  mp[0].push_back(300);
  assert((mp.at(0) == vl{3, 200, 300}));
  assert((cmp[0] == vl{3, 200, 300}));
  assert((cmp.at(0) == vl{3, 200, 300}));
  auto copy = cmp[0];
  copy.push_back(400);
  assert(mp[0].size() == 3 && factory_calls == 3);

  assert((mp.at(1) == vl{4, 200}));
  assert((mp[0] == vl{3, 200, 300}));
  assert(mp.size() == 2 && factory_calls == 4);
}

template <template <class, class, auto> class Map>
void test_defaults()
{
  test_scalar<Map<ll, ll, INF>>();
  test_factory<Map<ll, vl, make_default>>();

  Map<ll, ll, -1> negative;
  assert(negative[0] == -1);
  Map<ll, bool, false> flags;
  assert(!flags[0]);
  flags[0] = true;
  assert(flags[0] && !flags.at(1));

#if __cplusplus >= 202002L
  test_scalar<Map<ll, ll, [] { return INF; }>>();
  test_factory<Map<ll, vl, [] { return make_default(); }>>();

  Map<ll, array<int, 2>, array<int, 2>{10, 20}> arrays;
  arrays[0][0] = 30;
  assert((arrays[1] == array<int, 2>{10, 20}));
  assert(arrays[0][0] == 30);
#endif
}

struct ModHash
{
  size_t operator()(int x) const { return x % 8; }
};

struct ModEqual
{
  bool operator()(int x, int y) const { return x % 1024 == y % 1024; }
};

// Test focus: HashMap operations and custom hash/equality work with default values.
void test_hash_map()
{
  DefaultHashMap<int, ll, -7, ModHash, ModEqual> mp;
  const auto &cmp = mp;
  assert(mp.find_ptr(6) == nullptr && cmp.find_ptr(6) == nullptr);
  assert(!mp.contains(6) && mp.empty());
  assert(mp[6] == -7);
  mp[6] = 60;
  assert(mp[1030] == 60 && mp.size() == 1);
  mp[14] = 140;
  assert(mp.at(22) == -7);
  assert(mp.erase(14));
  assert(cmp[6] == 60 && cmp[22] == -7);
  assert(!mp.contains(14));

  auto [p, inserted] = mp.insert(6, 600);
  assert(!inserted && *p == 60);
  assert(mp.insert_or_assign(14, 140));
  assert(!mp.insert_or_assign(14, 141));
  assert(*cmp.find_ptr(14) == 141);

  mp.clear();
  repi(i, 300)
  {
    assert(mp[i] == -7);
    mp[i] = i * 10;
  }
  mp.reserve(1000);
  repi(i, 300)
    assert(cmp.at(i) == i * 10);
  assert(cmp.at(300) == -7 && mp.size() == 300);
  mp.clear();
  assert(mp.empty() && cmp[0] == -7 && !mp.contains(0));

  DefaultHashMap<pair<ll, ll>, ll, -1> pairs;
  assert((pairs[{1, 2}] == -1));
  pairs[{1, 2}] = 12;
  assert(pairs.at({1, 2}) == 12 && pairs.at({2, 1}) == -1);
}

int main()
{
  test_defaults<DefaultMap>();
  test_defaults<DefaultUnorderedMap>();
  test_defaults<DefaultHashMap>();
  test_hash_map();
  cout << "Hello World" << endl;
}
