#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief マージソート
 * @docs docs/algo/merge_sort.md
 */

template <class T, class Compare>
void merge_sort(vc<T> &v, const Compare &comp)
{
  const int n = v.size();
  if (n <= 1)
    return;
  vc<T> buf = v;
  // 読み出し元と書き込み先を交互にし、同じ補助配列を再利用する。
  auto sort_range = [&](auto &&self, vc<T> &src, vc<T> &dst, int l, int r) -> void
  {
    if (r - l <= 1) return;
    const int m = l + (r - l) / 2;
    self(self, dst, src, l, m);
    self(self, dst, src, m, r);
    merge(src.begin() + l, src.begin() + m,
          src.begin() + m, src.begin() + r, dst.begin() + l, comp);
  };
  sort_range(sort_range, buf, v, 0, n);
}
