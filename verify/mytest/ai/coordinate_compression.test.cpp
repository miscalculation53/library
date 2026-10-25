#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/coordinate_compression.hpp"

template <class T>
void check(const vc<T> &a)
{
  const CoordinateCompression cc(a);
  map<T, int> ranks, first;
  for (const auto &x : a) ranks.emplace(x, 0);
  vc<T> values;
  for (auto &[x, id] : ranks) id = values.size(), values.eb(x);
  assert(cc.content() == values && cc.template size<int>() == int(values.size()));
  auto ids = compressed<T, int>(a);
  auto unordered = compressed_unordered<T, int>(a);
  auto order = stable_argsort(a), expected = permid<int>(a.size());
  stable_sort(ALL(expected), [&](int i, int j) { return a[i] < a[j]; });
  assert(order == expected);
  repi(i, a.size())
  {
    auto [it, inserted] = first.emplace(a[i], first.size());
    assert(ids[i] == ranks.at(a[i]) && unordered[i] == it->second);
    assert(cc.template get_id<int>(a[i]) == ids[i]);
  }
  repi(i, values.size()) assert(cc.get_val(i) == values[i]);
}

struct Item
{
  ll key;
  unique_ptr<int> value;
  Item(ll key, int value) : key(key), value(make_unique<int>(value)) {}
};

int main()
{
  mt19937_64 rng(20261119);
  for (int n : {0, 1, 2, 8, 31, 32, 33, 127, 511, 512, 513, 1024, 4095, 4096, 4097, 65536})
  {
    vl a(n);
    vc<i128> wide(n);
    repi(i, n)
    {
      a[i] = i % 3 == 0 ? LLONG_MIN : i % 3 == 1 ? LLONG_MAX : ll(rng());
      wide[i] = i128(rng()) * (i128(1) << 60);
      if (i & 1) wide[i] = -wide[i];
    }
    check(a);
    check(wide);
  }
  check<bool>({true, false, true, true, false});
  check<string>({"pear", "apple", "", "pear", "apple"});
  check<pair<int, int>>({{1, 2}, {-1, 3}, {1, 2}, {1, 0}});
  vc<string> words(2000);
  for (auto &w : words) w = string(40, 'a') + to_string(rng() % 100);
  check(words);
  vl ascending(5000);
  iota(ALL(ascending), -2500);
  check(ascending);
  check(vl(5000, 7));
  reverse(ALL(ascending));
  check(ascending);
  vc<Item> records;
  repi(i, 1500) records.emplace_back(int(rng() % 17) - 8, i);
  auto expected = permid<int>(records.size());
  stable_sort(ALL(expected), [&](int i, int j) { return records[i].key < records[j].key; });
  auto ord = stable_argsort(records, [](const Item &x) { return x.key; });
  assert(ord == expected);
  repi(i, records.size()) assert(*records[i].value == i);
  assert(!CoordinateCompression<int>(vc<int>{1, 3}).get_id(2));
  cout << "Hello World" << endl;
}
