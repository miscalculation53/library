#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief 部分文字列の一致判定 → LCP、LCP → 部分文字列の辞書順比較
 * @docs docs/string/lcp_compare.md
 */

// ある文字列 s, t に対し s[i, ...) と t[j, ...) の LCP を求めたいとする
// ただし s, t は与えられず、s[l1, r1) と t[l2, r2) の一致判定ができるとする
// 一致判定をする関数 bool is_equal(l1, r1, l2, r2) を渡す
// ただし、r1, r2 が範囲外ならこの関数は false を返すようにすること
template <class I = ll, class IS_EQUAL>
I calc_lcp(int i, int j, const IS_EQUAL &is_equal)
{
  auto judge = [&](int len) -> bool
  { return is_equal(i, i + len, j, j + len); };
  return expsearch(judge, 0, true).first;
}

// ある文字列 s, t に対し s[l1, r1) と t[l2, r2) を辞書順比較したいとする
// ただし s, t は与えられず、s[l1, ...) と t[l2, ...) の LCP が計算できるとする
// lcp には s[l1, ...) と t[l2, ...) の LCP を渡す
// このとき文字列の比較を、文字 s[i] と t[j] の比較に帰着し、(i, j) を返す
// ただし範囲外の文字はどの文字より小さいと考える (文字列長以上のインデックスが返る)
pair<int, int> compare_substr(int l1, int r1, int l2, int r2, int lcp)
{
  assert(l1 <= r1 && l2 <= r2);
  lcp = min({lcp, r1 - l1, r2 - l2});
  return {l1 + lcp == r1 ? INT_MAX : l1 + lcp, l2 + lcp == r2 ? INT_MAX : l2 + lcp};
};
