#pragma once

#include "ordered_map.hpp"

/**
 * @brief キー順・順位区間の作用とモノイド積を持つ map
 * @docs docs/ds/bbst/lazy_ordered_map.md
 */

template <class Key, class AM, class Compare = less<Key>>
using LazyOrderedMapTree = bbst_detail::OrderedMapTreeImpl<Key, AM, Compare, true>;
