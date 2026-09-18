#pragma once

#include "../template/template_all_but_modint.hpp"

#include "hash_map.hpp"
#include "safe_unordered_map.hpp"

/**
 * @brief デフォルト値つき map
 * @docs docs/ds/default_map.md
 */

namespace internal
{
  template <class Value, auto default_value>
  Value default_map_value()
  {
    if constexpr (is_invocable_v<decltype(default_value)>)
      return default_value();
    else
      return Value(default_value);
  }
}

// default_value は値、またはそれを返す引数なしの関数
template <class Key, class Value, auto default_value>
struct DefaultMap : map<Key, Value>
{
  Value operator[](const Key &key) const
  {
    auto it = this->find(key);
    return (it != this->end()) ? it->second : internal::default_map_value<Value, default_value>();
  }

  Value &operator[](const Key &key)
  {
    auto it = this->find(key);
    if (it == this->end())
      it = this->insert({key, internal::default_map_value<Value, default_value>()}).first;
    return it->second;
  }

  Value at(const Key &key) const { return operator[](key); }
  Value &at(const Key &key) { return operator[](key); }
};

// default_value は値、またはそれを返す引数なしの関数
template <class Key, class Value, auto default_value>
struct DefaultUnorderedMap : unordered_map<Key, Value, safe_hash>
{
  Value operator[](const Key &key) const
  {
    auto it = this->find(key);
    return (it != this->end()) ? it->second : internal::default_map_value<Value, default_value>();
  }

  Value &operator[](const Key &key)
  {
    auto it = this->find(key);
    if (it == this->end())
      it = this->insert({key, internal::default_map_value<Value, default_value>()}).first;
    return it->second;
  }

  Value at(const Key &key) const { return operator[](key); }
  Value &at(const Key &key) { return operator[](key); }
};

// default_value は値、またはそれを返す引数なしの関数
template <class Key, class Value, auto default_value, class Hash = safe_hash, class Equal = equal_to<Key>>
struct DefaultHashMap : HashMap<Key, Value, Hash, Equal>
{
  Value operator[](const Key &key) const
  {
    auto p = this->find_ptr(key);
    return p ? *p : internal::default_map_value<Value, default_value>();
  }

  Value &operator[](const Key &key)
  {
    auto p = this->find_ptr(key);
    if (!p)
      p = this->insert(key, internal::default_map_value<Value, default_value>()).first;
    return *p;
  }

  Value at(const Key &key) const { return operator[](key); }
  Value &at(const Key &key) { return operator[](key); }
};
