#pragma once

#include "../template/template_all_but_modint.hpp"
#include "internal_dump_content.hpp"

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

  vc<pair<Key, Value>> content() const { return {this->begin(), this->end()}; }
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

  vc<pair<Key, Value>> content() const { return {this->begin(), this->end()}; }
};

// default_value は値、またはそれを返す引数なしの関数
template <class Key, class Value, auto default_value, class Hash = safe_hash, class Equal = equal_to<Key>>
struct DefaultHashMap : HashMap<Key, Value, Hash, Equal>
{
  Value operator[](const Key &key) const
  {
    auto p = this->find_ptr(key);
    return p ? p->get() : internal::default_map_value<Value, default_value>();
  }

  Value &operator[](const Key &key)
  {
    auto p = this->find_ptr(key);
    if (p) return p->get();
    return *this->insert(key, internal::default_map_value<Value, default_value>()).first;
  }

  Value at(const Key &key) const { return operator[](key); }
  Value &at(const Key &key) { return operator[](key); }
};

#ifdef LOCAL
namespace cpp_dump::_detail
{
  template <class Key, class Value, auto default_value>
  inline constexpr bool is_container<DefaultMap<Key, Value, default_value>> = false;

  template <class Key, class Value, auto default_value>
  inline string export_object_generic(
      const DefaultMap<Key, Value, default_value> &value, const string &indent,
      size_t last_line_length, size_t current_depth, bool fail_on_newline,
      const export_command &command)
  {
    return export_named_content<library_dump_detail::MapFields>(
        value, [&]() -> decltype(auto) { return (value); },
        indent, last_line_length, current_depth, fail_on_newline, command);
  }

  template <class Key, class Value, auto default_value>
  inline constexpr bool is_container<DefaultUnorderedMap<Key, Value, default_value>> = false;

  template <class Key, class Value, auto default_value>
  inline string export_object_generic(
      const DefaultUnorderedMap<Key, Value, default_value> &value, const string &indent,
      size_t last_line_length, size_t current_depth, bool fail_on_newline,
      const export_command &command)
  {
    return export_named_content<library_dump_detail::MapFields>(
        value, [&]() -> decltype(auto) { return (value); },
        indent, last_line_length, current_depth, fail_on_newline, command);
  }

  template <class Key, class Value, auto default_value, class Hash, class Equal>
  inline constexpr bool is_container<DefaultHashMap<Key, Value, default_value, Hash, Equal>> = false;

  template <class Key, class Value, auto default_value, class Hash, class Equal>
  inline string export_object_generic(
      const DefaultHashMap<Key, Value, default_value, Hash, Equal> &value, const string &indent,
      size_t last_line_length, size_t current_depth, bool fail_on_newline,
      const export_command &command)
  {
    return export_named_content<library_dump_detail::MapFields>(
        value, [&]() -> decltype(auto) { return (value); },
        indent, last_line_length, current_depth, fail_on_newline, command);
  }

} // namespace cpp_dump::_detail
#endif
