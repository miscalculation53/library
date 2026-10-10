#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief 64 分木による整数集合
 * @docs docs/ds/fast_integer_set.md
 */

struct FastIntegerSet
{
private:
  int n = 0, num = 0;
  vvc<ull> seg;

public:
  FastIntegerSet() = default;
  explicit FastIntegerSet(int n) : n(n)
  {
    assert(n >= 0);
    for (int m = n; m > 0;)
    {
      m = m / 64 + (m % 64 != 0);
      seg.eb(m, 0);
      if (m == 1) break;
    }
  }
  template <class F>
  FastIntegerSet(int n, const F &f) : FastIntegerSet(n)
  {
    repi(i, n) if (f(i))
    {
      seg[0][i >> 6] |= 1ULL << (i & 63);
      num++;
    }
    repi(h, 1, seg.size()) repi(i, seg[h - 1].size())
      if (seg[h - 1][i]) seg[h][i >> 6] |= 1ULL << (i & 63);
  }

  FastIntegerSet(const FastIntegerSet &) = default;
  FastIntegerSet &operator=(const FastIntegerSet &) = default;
  FastIntegerSet(FastIntegerSet &&other) noexcept { swap(other); }
  FastIntegerSet &operator=(FastIntegerSet &&other) noexcept
  {
    if (this != &other)
    {
      FastIntegerSet tmp;
      tmp.swap(other);
      swap(tmp);
    }
    return *this;
  }

  int universe_size() const { return n; }
  int size() const { return num; }
  bool empty() const { return num == 0; }

  bool contains(int x) const
  {
    assert(0 <= x && x < n);
    return (seg[0][x >> 6] >> (x & 63)) & 1;
  }
  bool operator[](int x) const { return contains(x); }

  bool insert(int x)
  {
    if (contains(x)) return false;
    num++;
    repi(h, seg.size())
    {
      ull &word = seg[h][x >> 6], old = word;
      word |= 1ULL << (x & 63);
      if (old) break;
      x >>= 6;
    }
    return true;
  }

  bool erase(int x)
  {
    if (!contains(x)) return false;
    num--;
    repi(h, seg.size())
    {
      ull &word = seg[h][x >> 6];
      word &= ~(1ULL << (x & 63));
      if (word) break;
      x >>= 6;
    }
    return true;
  }

  // x 以上の最小の要素。なければ n。
  int next(int x) const
  {
    if (x >= n || empty()) return n;
    x = max(x, 0);
    repi(h, seg.size())
    {
      if ((x >> 6) >= SZ<int>(seg[h])) break;
      ull word = seg[h][x >> 6] >> (x & 63);
      if (!word)
      {
        x = (x >> 6) + 1;
        continue;
      }
      x += __builtin_ctzll(word);
      repi(g, h - 1, -1, -1)
      {
        x <<= 6;
        x += __builtin_ctzll(seg[g][x >> 6]);
      }
      return x;
    }
    return n;
  }

  // x 以下の最大の要素。なければ -1。
  int prev(int x) const
  {
    if (x < 0 || empty()) return -1;
    x = min(x, n - 1);
    repi(h, seg.size())
    {
      if (x < 0) break;
      ull word = seg[h][x >> 6] << (63 - (x & 63));
      if (!word)
      {
        x = (x >> 6) - 1;
        continue;
      }
      x -= __builtin_clzll(word);
      repi(g, h - 1, -1, -1)
      {
        x <<= 6;
        x += 63 - __builtin_clzll(seg[g][x >> 6]);
      }
      return x;
    }
    return -1;
  }

  int geq_min(int x) const { return next(x); }
  int gt_min(int x) const { return x >= n - 1 ? n : next(x + 1); }
  int leq_max(int x) const { return prev(x); }
  int lt_max(int x) const { return x <= 0 ? -1 : prev(x - 1); }
  int min_element() const { return next(0); }
  int max_element() const { return prev(n - 1); }

  template <class F>
  void enumerate(int l, int r, const F &f) const
  {
    assert(0 <= l && l <= r && r <= n);
    for (int x = next(l); x < r; x = next(x + 1)) f(x);
  }

  void clear()
  {
    if (empty()) return;
    for (auto &level : seg) fill(ALL(level), 0);
    num = 0;
  }

  vc<int> content() const
  {
    vc<int> res;
    res.reserve(num);
    enumerate(0, n, [&](int x) { res.eb(x); });
    return res;
  }

  class const_iterator
  {
    friend struct FastIntegerSet;
    const FastIntegerSet *st = nullptr;
    int key = 0;
    const_iterator(const FastIntegerSet *st, int key) : st(st), key(key) {}

  public:
    using iterator_category = input_iterator_tag;
    using value_type = int;
    using difference_type = ptrdiff_t;
    using reference = int;
    using pointer = const int *;
    const_iterator() = default;
    int operator*() const { return key; }
    const int *operator->() const { return &key; }
    const_iterator &operator++() { key = st->next(key + 1); return *this; }
    const_iterator operator++(int) { auto old = *this; ++*this; return old; }
    bool operator==(const const_iterator &other) const { return st == other.st && key == other.key; }
    bool operator!=(const const_iterator &other) const { return !(*this == other); }
  };
  using iterator = const_iterator;
  iterator begin() const { return iterator(this, next(0)); }
  iterator end() const { return iterator(this, n); }
  const_iterator cbegin() const { return begin(); }
  const_iterator cend() const { return end(); }

  void swap(FastIntegerSet &other) noexcept
  {
    std::swap(n, other.n), std::swap(num, other.num);
    seg.swap(other.seg);
  }
  friend void swap(FastIntegerSet &a, FastIntegerSet &b) noexcept { a.swap(b); }
};
