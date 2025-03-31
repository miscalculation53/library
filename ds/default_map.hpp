#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief デフォルト値つき map
 * @docs docs/ds/default_map.md
 */

template <class Key, class Value, auto default_value>
struct DefaultMap : map<Key, Value>
{
  Value operator[](const Key &key) const
  {
    auto it = this->find(key);
    return (it != this->end()) ? it->second : default_value();
  }

  Value &operator[](const Key &key)
  {
    auto it = this->find(key);
    if (it == this->end())
      it = this->insert({key, default_value()}).first;
    return it->second;
  }

  Value at(const Key &key) const { return (*this)[key]; }
  Value &at(const Key &key) { return (*this)[key]; }
};
