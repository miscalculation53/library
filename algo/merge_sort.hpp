#pragma once

#include "../template/template_all.hpp"

/**
 * @brief マージソート
 * @docs docs/algo/merge_sort.md
 */

template <class T>
void merge_sort(vc<T> &v, cauto &comp)
{
  const int n = v.size();
  if (n <= 1)
    return;
  vc<T> l{v.begin(), v.begin() + n / 2};
  vc<T> r{v.begin() + n / 2, v.end()};
  merge_sort(l, comp);
  merge_sort(r, comp);
  merge(ALL(l), ALL(r), v.begin(), comp);
}