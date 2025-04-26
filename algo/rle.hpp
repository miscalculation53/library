#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief ランレングス圧縮
 * @docs docs/algo/rle.md
 */

// 区間は [left, right)
template <class T, class I = ll>
struct rle_info
{
  T val;
  I len, left, right;
  rle_info(T val, I len, I left, I right) : val(val), len(len), left(left), right(right) {}
};
#ifdef LOCAL
CPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC(val, len, left, right);
#endif
template <class T, class I = ll>
vc<rle_info<T, I>> rle(const vc<T> &a)
{
  vc<rle_info<T>> res;
  repi(i, a.size())
  {
    if (res.empty() || res.back().val != a[i])
      res.eb(a[i], 1, i, i + 1);
    else
      res.back().len++, res.back().right++;
  }
  return res;
}
vc<rle_info<char>> rle(const string &s)
{
  vc<char> a(ALL(s));
  return rle(a);
}
