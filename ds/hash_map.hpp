#pragma once

#include "../template/template_all_but_modint.hpp"

#include "safe_unordered_map.hpp"

/**
 * @brief open addressing によるハッシュマップ
 * @docs docs/ds/hash_map.md
 */

template <class Key, class Value, class Hash = safe_hash, class Equal = equal_to<Key>>
struct HashMap
{
private:
  static constexpr size_t npos = numeric_limits<size_t>::max();
  vc<pair<Key, Value>> dat;
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
      if (ctrl[i] == fp && equal(dat[i].first, key))
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
    vc<pair<Key, Value>> old_dat = std::move(dat);
    vc<unsigned char> old_ctrl = std::move(ctrl);
    dat.resize(cap), ctrl.assign(cap, 0), mask = cap - 1;
    num = 0;
    repi(i, old_ctrl.size())
      if (old_ctrl[i])
      {
        size_t h = hasher(old_dat[i].first), j = empty_index(h);
        dat[j] = std::move(old_dat[i]), ctrl[j] = fingerprint(h), num++;
      }
  }

  void prepare_insert()
  {
    if (ctrl.empty())
      rebuild(8);
    else if ((num + 1) * 10 > ctrl.size() * 7)
      rebuild(2 * ctrl.size());
  }

public:
  HashMap() {}

  // n 要素を再ハッシュせず格納できる容量を確保する
  void reserve(size_t n)
  {
    size_t cap = 8;
    while (cap * 7 < n * 10)
      cap *= 2;
    if (cap > ctrl.size())
      rebuild(cap);
  }

  // 格納している要素数を返す
  size_t size() const { return num; }
  // 空なら true を返す
  bool empty() const { return num == 0; }

  // key が存在すれば対応する値へのポインタ、存在しなければ nullptr を返す
  Value *find_ptr(const Key &key)
  {
    size_t i = find_index(key, hasher(key));
    return i == npos ? nullptr : &dat[i].second;
  }
  const Value *find_ptr(const Key &key) const
  {
    size_t i = find_index(key, hasher(key));
    return i == npos ? nullptr : &dat[i].second;
  }

  // key が存在すれば true を返す
  bool contains(const Key &key) const { return find_ptr(key) != nullptr; }

  // key に対応する値を返す
  Value &at(const Key &key)
  {
    Value *p = find_ptr(key);
    assert(p != nullptr);
    return *p;
  }
  const Value &at(const Key &key) const
  {
    const Value *p = find_ptr(key);
    assert(p != nullptr);
    return *p;
  }

  // key に対応する値を返し、存在しなければ Value{} を挿入する
  Value &operator[](const Key &key)
  {
    size_t h = hasher(key), i = find_index(key, h);
    if (i != npos)
      return dat[i].second;
    prepare_insert(), i = empty_index(h);
    dat[i] = {key, Value{}}, ctrl[i] = fingerprint(h), num++;
    return dat[i].second;
  }

  // key, value を挿入し、挿入した値へのポインタと新規挿入かを返す
  pair<Value *, bool> insert(const Key &key, const Value &value)
  {
    size_t h = hasher(key), i = find_index(key, h);
    if (i != npos)
      return {&dat[i].second, false};
    prepare_insert(), i = empty_index(h);
    dat[i] = {key, value}, ctrl[i] = fingerprint(h), num++;
    return {&dat[i].second, true};
  }

  // key に value を設定し、新規挿入なら true を返す
  bool insert_or_assign(const Key &key, const Value &value)
  {
    auto [p, inserted] = insert(key, value);
    if (!inserted)
      *p = value;
    return inserted;
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
      size_t home = hasher(dat[i].first) & mask;
      if (((i - home) & mask) >= ((hole - home) & mask))
      {
        dat[hole] = std::move(dat[i]), ctrl[hole] = ctrl[i], hole = i;
      }
      i = (i + 1) & mask;
    }
    dat[hole] = {}, ctrl[hole] = 0, num--;
    return true;
  }

  // 全要素を削除する
  void clear()
  {
    repi(i, ctrl.size())
      if (ctrl[i])
        dat[i] = {};
    fill(ALL(ctrl), 0), num = 0;
  }
};
