#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "algo/merge_sort.hpp"

struct Item
{
  int key, index;
  Item() = delete;
  Item(int key, int index) : key(key), index(index) {}
  bool operator==(const Item &rhs) const { return key == rhs.key && index == rhs.index; }
};

// インタラクティブ用途の比較順序も、従来の二分再帰と一致させる。
template <class T, class Compare>
void reference_sort(vc<T> &v, const Compare &comp)
{
  if (v.size() <= 1) return;
  vc<T> l(v.begin(), v.begin() + v.size() / 2);
  vc<T> r(v.begin() + v.size() / 2, v.end());
  reference_sort(l, comp);
  reference_sort(r, comp);
  merge(ALL(l), ALL(r), v.begin(), comp);
}

int main()
{
  mt19937 rng(20261022);
  repi(n, 260) repi(trial, 5)
  {
    vc<Item> input;
    repi(i, n) input.eb(rng() % 17, i);
    auto actual = input, expected = input;
    vc<pair<int, int>> trace, expected_trace;
    auto less_key = [&](const Item &a, const Item &b)
    {
      trace.eb(a.index, b.index);
      return a.key < b.key;
    };
    auto reference_less = [&](const Item &a, const Item &b)
    {
      expected_trace.eb(a.index, b.index);
      return a.key < b.key;
    };
    merge_sort(actual, less_key);
    reference_sort(expected, reference_less);
    assert(actual == expected && trace == expected_trace);
    stable_sort(ALL(input), [](const Item &a, const Item &b) { return a.key < b.key; });
    assert(actual == input);
    merge_sort(actual, [](const Item &a, const Item &b) { return a.key > b.key; });
    stable_sort(ALL(input), [](const Item &a, const Item &b) { return a.key > b.key; });
    assert(actual == input);
  }
  cout << "Hello World" << endl;
}
