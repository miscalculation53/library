#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief 要素の追加・上位 $k$ 個の管理
 * @docs docs/ds/topk_array.md
 */

template <class T, int k, class Compare = less<T>, class SameKey = void>
struct TopKArray
{
  static_assert(k >= 1);

private:
  int sz;
  array<T, k> arr;

  void move_up(int i)
  {
    if constexpr (k > 1)
      while (i > 0 && Compare()(arr[i], arr[i - 1]))
        swap(arr[i], arr[i - 1]), --i;
  }

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
  bool add(const T &val)
  {
    if constexpr (!is_void_v<SameKey>)
    {
      repi(i, sz) if (SameKey()(arr[i], val))
      {
        if (!Compare()(val, arr[i])) return false;
        arr[i] = val;
        move_up(i);
        return true;
      }
    }
    if (sz < k)
      arr[sz++] = val;
    else
    {
      if (!Compare()(val, arr[sz - 1]))
        return false;
      arr[sz - 1] = val;
    }
    move_up(sz - 1);
    return true;
  }
  template <class I = ll>
  I size() const { return sz; }
  // O(1)
  T get(int i) const
  {
    assert(0 <= i && i < sz);
    return arr[i];
  }
  // SameKey=void なら O(k)、それ以外は O(k^2)
  TopKArray merged(const TopKArray &other) const
  {
    TopKArray res{};
    int i = 0, j = 0;
    while (res.sz < k && (i < sz || j < other.sz))
    {
      const T &val = j == other.sz || (i < sz && Compare()(arr[i], other.arr[j]))
                         ? arr[i++] : other.arr[j++];
      if constexpr (is_void_v<SameKey>)
        res.arr[res.sz++] = val;
      else
        res.add(val);
    }
    return res;
  }
  // SameKey=void なら O(k)、それ以外は O(k^2)
  void merge(const TopKArray &other) { *this = merged(other); }

  vc<T> content() const
  {
    vc<T> res(sz);
    repi(i, sz) res[i] = arr[i];
    return res;
  }
};

template <class T, int k, class Compare = less<T>, class SameKey = void>
struct TopKArrayMonoid
{
  using S = TopKArray<T, k, Compare, SameKey>;
  static constexpr S op(const S &a, const S &b) { return a.merged(b); }
  static constexpr S e() { return {}; }
};
