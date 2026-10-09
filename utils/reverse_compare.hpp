#pragma once

#include "../template/template_types.hpp"

/**
 * @brief 比較器の順序の反転
 * @docs docs/utils/reverse_compare.md
 */

template <class Compare>
struct ReverseCompare
{
  Compare comp;
  template <class T, class U>
  bool operator()(const T &a, const U &b) const { return comp(b, a); }
};

template <class Compare>
struct reverse_compare_type { using type = ReverseCompare<Compare>; };
template <class Compare>
struct reverse_compare_type<ReverseCompare<Compare>> { using type = Compare; };
template <class T>
struct reverse_compare_type<less<T>> { using type = greater<T>; };
template <class T>
struct reverse_compare_type<greater<T>> { using type = less<T>; };
template <class Compare>
using reverse_compare_t = typename reverse_compare_type<Compare>::type;

template <class Compare>
reverse_compare_t<Compare> reverse_compare(const Compare &comp)
{
  if constexpr (is_same_v<reverse_compare_t<Compare>, ReverseCompare<Compare>>)
    return {comp};
  else
    return {};
}
template <class Compare>
Compare reverse_compare(const ReverseCompare<Compare> &comp) { return comp.comp; }
