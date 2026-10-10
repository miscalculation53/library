#pragma once

#include "../template/template_all_but_modint.hpp"

#include "safe_unordered_map.hpp"

/**
 * @brief open addressing によるハッシュセット
 * @docs docs/ds/hash_set.md
 */

template <class Key, class Hash = safe_hash, class Equal = equal_to<Key>>
struct HashSet
{
private:
  static constexpr size_t npos = numeric_limits<size_t>::max();
  static constexpr bool nothrow_swap = is_nothrow_swappable_v<Hash> && is_nothrow_swappable_v<Equal>;
  static constexpr bool nothrow_move = nothrow_swap && is_nothrow_default_constructible_v<Hash> && is_nothrow_default_constructible_v<Equal>;
  vc<Key> dat;
  vc<unsigned char> ctrl;
  size_t num = 0, mask = 0;
  Hash hasher;
  Equal equal;

  static unsigned char fingerprint(size_t h)
  {
    ull x = h;
    return static_cast<unsigned char>(((x ^ (x >> 32)) & 127) + 1);
  }

  size_t find_index(const Key &key, size_t h) const
  {
    if (ctrl.empty())
      return npos;
    size_t i = h & mask;
    const unsigned char fp = fingerprint(h);
    while (ctrl[i])
    {
      if (ctrl[i] == fp && equal(dat[i], key))
        return i;
      i = (i + 1) & mask;
    }
    return npos;
  }

  size_t empty_index(size_t h) const
  {
    size_t i = h & mask;
    while (ctrl[i])
      i = (i + 1) & mask;
    return i;
  }

  void rebuild(size_t cap)
  {
    cap = max<size_t>(8, bit_ceil(cap));
    vc<Key> old_dat = std::move(dat);
    vc<unsigned char> old_ctrl = std::move(ctrl);
    dat.resize(cap), ctrl.assign(cap, 0), mask = cap - 1;
    num = 0;
    repi(i, old_ctrl.size())
      if (old_ctrl[i])
      {
        size_t h = hasher(old_dat[i]), j = empty_index(h);
        dat[j] = std::move(old_dat[i]), ctrl[j] = fingerprint(h), num++;
      }
  }

public:
  // 空きスロットを飛ばして走査する。キーは const 参照で返す。
  class const_iterator
  {
    friend struct HashSet;

    const HashSet *st = nullptr;
    size_t index = 0;

    void skip_empty()
    {
      while (index < st->ctrl.size() && !st->ctrl[index])
        index++;
    }

    const_iterator(const HashSet *st, size_t index) : st(st), index(index) { skip_empty(); }

  public:
    using iterator_category = forward_iterator_tag;
    using value_type = Key;
    using difference_type = ptrdiff_t;
    using reference = const Key &;
    using pointer = const Key *;

    const_iterator() = default;

    reference operator*() const { return st->dat[index]; }
    pointer operator->() const { return &st->dat[index]; }

    const_iterator &operator++()
    {
      index++;
      skip_empty();
      return *this;
    }
    const_iterator operator++(int)
    {
      auto old = *this;
      ++*this;
      return old;
    }

    bool operator==(const const_iterator &other) const { return st == other.st && index == other.index; }
    bool operator!=(const const_iterator &other) const { return !(*this == other); }
  };

  using iterator = const_iterator;

  HashSet() = default;
  HashSet(const HashSet &) = default;
  HashSet &operator=(const HashSet &) = default;
  HashSet(HashSet &&other) noexcept(nothrow_move) : HashSet() { swap(other); }
  HashSet &operator=(HashSet &&other) noexcept(nothrow_move)
  {
    if (this != &other)
    {
      HashSet tmp;
      tmp.swap(other);
      swap(tmp);
    }
    return *this;
  }

  iterator begin() const { return iterator(this, 0); }
  iterator end() const { return iterator(this, ctrl.size()); }
  const_iterator cbegin() const { return begin(); }
  const_iterator cend() const { return end(); }

  // n 要素を再ハッシュせず格納できる容量を確保する
  void reserve(size_t n)
  {
    size_t cap = 8;
    while (cap * 7 < n * 10)
      cap *= 2;
    if (cap > ctrl.size())
      rebuild(cap);
  }

  size_t size() const { return num; }
  bool empty() const { return num == 0; }

  // key が存在すれば格納されたキーへのポインタ、存在しなければ nullptr を返す
  const Key *find_ptr(const Key &key) const
  {
    size_t i = find_index(key, hasher(key));
    return i == npos ? nullptr : &dat[i];
  }

  bool contains(const Key &key) const { return find_ptr(key) != nullptr; }

  // key を挿入し、格納されたキーへのポインタと新規挿入かを返す
  pair<const Key *, bool> insert(const Key &key)
  {
    if (ctrl.empty())
      rebuild(8);
    size_t h = hasher(key), i = h & mask;
    const unsigned char fp = fingerprint(h);
    while (ctrl[i])
    {
      if (ctrl[i] == fp && equal(dat[i], key))
        return {&dat[i], false};
      i = (i + 1) & mask;
    }
    if ((num + 1) * 10 > ctrl.size() * 7)
      rebuild(2 * ctrl.size()), i = empty_index(h);
    dat[i] = key, ctrl[i] = fp, num++;
    return {&dat[i], true};
  }

  // key を削除し、存在していたなら true を返す
  bool erase(const Key &key)
  {
    if (ctrl.empty())
      return false;
    size_t hole = find_index(key, hasher(key));
    if (hole == npos)
      return false;
    size_t i = (hole + 1) & mask;
    while (ctrl[i])
    {
      size_t home = hasher(dat[i]) & mask;
      if (((i - home) & mask) >= ((hole - home) & mask))
      {
        dat[hole] = std::move(dat[i]), ctrl[hole] = ctrl[i], hole = i;
      }
      i = (i + 1) & mask;
    }
    dat[hole] = {}, ctrl[hole] = 0, num--;
    return true;
  }

  // 全要素を削除する。確保した容量は維持する。
  void clear()
  {
    repi(i, ctrl.size())
      if (ctrl[i])
        dat[i] = {};
    fill(ALL(ctrl), 0), num = 0;
  }

  void swap(HashSet &other) noexcept(nothrow_swap)
  {
    dat.swap(other.dat), ctrl.swap(other.ctrl);
    std::swap(num, other.num), std::swap(mask, other.mask);
    std::swap(hasher, other.hasher), std::swap(equal, other.equal);
  }
  friend void swap(HashSet &a, HashSet &b) noexcept(nothrow_swap) { a.swap(b); }
};
