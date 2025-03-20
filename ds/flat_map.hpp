#pragma once

#include "../template/template_all.hpp"

#include "coordinate_compression.hpp"

/**
 * @brief キーがすべて先にわかる場合の map
 * @docs docs/ds/flat_map.md
 */

template <class Key, class Value>
struct FlatMap
{
  CoordinateCompression<Key> cc;
  vc<Value> vals;

  FlatMap() {}
  FlatMap(const vc<Key> &keys) : cc(keys), vals(cc.size()) {}
  Value &operator[](const Key &key)
  {
    const int i = cc.get_id(key);
    assert(i != -1);
    return vals[i];
  }
  Value &at(const Key &key) { return operator[](key); }
  bool contains(const Key &key) { return cc.get_id(key) != -1; }

  template <class I = ll>
  inline I size() const { return cc.size(); }
  inline bool empty() const { return size() == 0; }
  struct Iterator
  {
  private:
    int i;
    const FlatMap &mp;
  public:
    Iterator(int i, const FlatMap &mp) : i(i), mp(mp) {}
    pair<Key, Value> operator*() const
    {
      assert(i != mp.cc.size());
      return pair{mp.cc.vals[i], mp.vals[i]};
    }
    Iterator &operator++()
    {
      i++;
      return *this;
    }
    bool operator!=(const Iterator &other) const { return i != other.i; }
  };
  Iterator begin() const { return Iterator(0, *this); }
  Iterator end() const { return Iterator(cc.size(), *this); }
};