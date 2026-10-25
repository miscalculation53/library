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

private:
  // Independently derived root descent: retain the deepest sibling range
  // containing a result while following the boundary prefix.
  template <bool forward>
  static int fast_set_edge(ull bits)
  {
    if constexpr (forward) return __builtin_ctzll(bits);
    else return 63 - __builtin_clzll(bits);
  }

  template <bool forward>
  int fast_set_word(int level, int node, int bound) const
  {
    const ull children = seg[level][node];
    const int digit = (bound >> (6 * (level - 1))) & 63;
    const ull pivot = ull(1) << digit;
    if (children & pivot)
    {
      const int child = (node << 6) + digit;
      if (level == 1) return child;
      const int found = fast_set_word<forward>(level - 1, child, bound);
      if (found >= 0) return found;
    }

    ull candidates;
    if constexpr (forward) candidates = children & ~(pivot | (pivot - 1));
    else candidates = children & (pivot - 1);
    if (candidates == 0) return -1;

    int child = (node << 6) + fast_set_edge<forward>(candidates);
    for (--level; level >= 1; --level)
      child = (child << 6) + fast_set_edge<forward>(seg[level][child]);
    return child;
  }

  template <bool forward>
  static ull fast_set_window(ull bits, int offset)
  {
    const ull pivot = ull(1) << offset;
    if constexpr (forward) return bits & ~(pivot - 1);
    else return bits & (pivot | (pivot - 1));
  }

  // 近い summary word を先に調べ、残る範囲を根から探索する。
  template <bool forward>
  [[gnu::noinline]] int fast_set_far(int next_summary) const
  {
    const int missing = forward ? n : -1;
    if (next_summary < 0 || next_summary >= int(seg[1].size())) return missing;
    const auto key_at_edge = [&](int word)
    {
      return (word << 6) + fast_set_edge<forward>(seg[0][word]);
    };
    const ull adjacent = seg[1][next_summary];
    if (adjacent)
      return key_at_edge((next_summary << 6) + fast_set_edge<forward>(adjacent));
    const int parent = next_summary >> 6;
    const ull candidates = fast_set_window<forward>(seg[2][parent], next_summary & 63);
    if (candidates)
    {
      const int selected = (parent << 6) + fast_set_edge<forward>(candidates);
      const int word = (selected << 6) + fast_set_edge<forward>(seg[1][selected]);
      return key_at_edge(word);
    }
    if (seg.size() == 3) return missing;
    const int remaining = forward ? ((parent + 1) << 12) : (parent << 12) - 1;
    if (remaining < 0 || remaining >= int(seg[0].size())) return missing;
    const int found = fast_set_word<forward>(int(seg.size()) - 1, 0, remaining);
    return found < 0 ? missing : key_at_edge(found);
  }

  template <bool forward>
  [[gnu::always_inline]] inline int fast_set_neighbor(int x) const
  {
    const int missing = forward ? n : -1;
    if (num == 0) return missing;
    if constexpr (forward)
    {
      if (x >= n) return n;
      if (x < 0) x = 0;
    }
    else
    {
      if (x < 0) return -1;
      if (x >= n) x = n - 1;
    }

    const int word = x >> 6;
    const ull pivot = ull(1) << (x & 63);
    ull candidates;
    if constexpr (forward) candidates = seg[0][word] & ~(pivot - 1);
    else candidates = seg[0][word] & (pivot | (pivot - 1));
    if (candidates) return (word << 6) + fast_set_edge<forward>(candidates);

    const int bound = word + (forward ? 1 : -1);
    if (bound < 0 || bound >= int(seg[0].size())) return missing;
    const int summary = bound >> 6;
    const ull nearby = fast_set_window<forward>(seg[1][summary], bound & 63);
    if (nearby)
    {
      const int selected = (summary << 6) + fast_set_edge<forward>(nearby);
      return (selected << 6) + fast_set_edge<forward>(seg[0][selected]);
    }
    if (seg.size() == 2) return missing;
    return fast_set_far<forward>(summary + (forward ? 1 : -1));
  }

public:
  optional<int> next(int x) const
  {
    int key = fast_set_neighbor<true>(x);
    return key == n ? nullopt : optional<int>(key);
  }
  optional<int> prev(int x) const
  {
    int key = fast_set_neighbor<false>(x);
    return key < 0 ? nullopt : optional<int>(key);
  }

  optional<int> geq_min(int x) const { return next(x); }
  optional<int> gt_min(int x) const { return x >= n - 1 ? nullopt : next(x + 1); }
  optional<int> leq_max(int x) const { return prev(x); }
  optional<int> lt_max(int x) const { return x <= 0 ? nullopt : prev(x - 1); }
  optional<int> min_element() const { return next(0); }
  optional<int> max_element() const { return prev(n - 1); }

  template <class F>
  void enumerate(int l, int r, const F &f) const
  {
    assert(0 <= l && l <= r && r <= n);
    for (int x = fast_set_neighbor<true>(l); x < r; x = fast_set_neighbor<true>(x + 1)) f(x);
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
    const_iterator &operator++() { key = st->fast_set_neighbor<true>(key + 1); return *this; }
    const_iterator operator++(int) { auto old = *this; ++*this; return old; }
    bool operator==(const const_iterator &other) const { return st == other.st && key == other.key; }
    bool operator!=(const const_iterator &other) const { return !(*this == other); }
  };
  using iterator = const_iterator;
  iterator begin() const { return iterator(this, fast_set_neighbor<true>(0)); }
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
