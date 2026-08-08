#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief 要素の追加・上位 $k$ 個の管理
 * @docs docs/ds/topk_array.md
 */

template <class T, int k, class Compare = less<T>>
struct TopKArray
{
private:
  int sz;
  array<T, k> arr;

public:
  TopKArray() : sz(0), arr{} {}
  template <class Iter>
  TopKArray(Iter bg, Iter ed) : sz(0), arr{}
  {
    for (auto it = bg; it != ed; it++)
      add(*it);
  }
  TopKArray(initializer_list<T> ini) : TopKArray(ini.begin(), ini.end()) {}
  template <class V>
  TopKArray(const V &v) : TopKArray(v.begin(), v.end()) {}

  // O(k)
  void add(const T &val)
  {
    if (sz < k)
      arr[sz++] = val;
    else
    {
      if (!Compare()(val, arr[sz - 1]))
        return;
      arr[sz - 1] = val;
    }
    repi(i, sz - 2, -1, -1)
    {
      if (Compare()(arr[i + 1], arr[i]))
        swap(arr[i], arr[i + 1]);
      else
        return;
    }
  }
  template <class I = ll>
  I size() const { return sz; }
  // O(1)
  T get(int i) const
  {
    assert(0 <= i && i < sz);
    return arr[i];
  }
  // O(k)
  TopKArray merged(const TopKArray &other) const
  {
    TopKArray res{};
    int i = 0, j = 0;
    while (res.sz < k && (i < sz || j < other.sz))
    {
      if (j == other.sz || (i < sz && Compare()(arr[i], other.arr[j])))
        res.arr[res.sz++] = arr[i++];
      else
        res.arr[res.sz++] = other.arr[j++];
    }
    return res;
  }
  // O(k)
  void merge(const TopKArray &other) { *this = merged(other); }

  vc<T> content() const
  {
    vc<T> res(sz);
    repi(i, sz) res[i] = arr[i];
    return res;
  }
};

template <class T, int k, class Compare = less<T>>
struct TopKArrayMonoid
{
  using S = TopKArray<T, k, Compare>;
  static constexpr S op(const S &a, const S &b) { return a.merged(b); }
  static constexpr S e() { return {}; }
};
