#pragma once

#include "../template/template_all_but_modint.hpp"
#include "algebra_base.hpp"

/**
 * @brief Boyer–Moore majority vote algorithm
 * @docs docs/algebra/majority_vote.md
 */

template <class T, class L = ll>
struct MajorityVote
{
  // 相異なる 2 要素をペアにして消すことを繰り返したら cand が cnt 個残る
  // このとき cand だけが過半数候補 (実際に過半数とは限らない)
  struct S
  {
    T cand;
    L cnt;
    S(T cand = T{}, L cnt = 1) : cand(cand), cnt(cnt) {}
  };
  static constexpr S op(const S &a, const S &b)
  {
    if (a.cand == b.cand)
      return {a.cand, L(a.cnt + b.cnt)};
    else if (a.cnt >= b.cnt)
      return {a.cand, L(a.cnt - b.cnt)};
    else
      return {b.cand, L(b.cnt - a.cnt)};
  }
  static constexpr S e() { return {T{}, 0}; }
};
#ifdef LOCAL
CPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC(cand, cnt);
#endif
