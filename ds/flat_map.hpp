#pragma once

#include "../template/template_all_but_modint.hpp"
#include "internal_dump_content.hpp"

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
  
  const Value &operator[](const Key &key) const
  {
    const auto i = cc.get_id(key);
    assert(i);
    return vals[*i];
  }
  Value &operator[](const Key &key)
  {
    const auto i = cc.get_id(key);
    assert(i);
    return vals[*i];
  }
  const Value &at(const Key &key) const { return operator[](key); }
  Value &at(const Key &key) { return operator[](key); }

  bool contains(const Key &key) { return cc.get_id(key).has_value(); }

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
  vc<pair<Key, Value>> content() const
  {
    vc<pair<Key, Value>> res;
    res.reserve(size());
    for (auto entry : *this) res.eb(std::move(entry));
    return res;
  }
};

#ifdef LOCAL
namespace cpp_dump::_detail
{
  template <class Key, class Value>
  inline constexpr bool is_container<FlatMap<Key, Value>> = false;

  template <class Key, class Value>
  inline string export_object_generic(
      const FlatMap<Key, Value> &value, const string &indent,
      size_t last_line_length, size_t current_depth, bool fail_on_newline,
      const export_command &command)
  {
    return export_named_content<library_dump_detail::MapFields>(
        value, [&]() -> decltype(auto) { return (value); },
        indent, last_line_length, current_depth, fail_on_newline, command);
  }

} // namespace cpp_dump::_detail
#endif
