#pragma once

#include "template_types.hpp"
#include "template_rep.hpp"

/**
 * @brief テンプレート（ランダム生成）
 * @docs docs/template/template_random.md
 */

mt19937_64 mt;

// [l, r] から等確率
template <class T = ll, class U1, class U2>
T randint(U1 l, U2 r)
{
  assert(T(l) <= T(r));
  return T(l) + mt() % (T(r) - T(l) + 1);
}
// [l, r) から等確率
template <class T = ll, class U1, class U2>
T randrange(U1 l, U2 r)
{
  assert(T(l) < T(r));
  return T(l) + mt() % (T(r) - T(l));
}

// [l, r) から相異なる k 個を選ぶ
// does_sort: ソートするかどうか
template <int k, bool does_sort, class T = ll, class U1, class U2>
array<T, k> random_sample_range_array(U1 l, U2 r)
{
  assert(T(r) - T(l) >= T(k));
  array<T, k> res;
  repi(i, k) res[i] = randint<T>(T(l), T(r) - T(k));
  sort(ALL(res));
  repi(i, k) res[i] += i;
  if (!does_sort)
    shuffle(ALL(res), mt);
  return res;
}
// [l, r) から相異なる k 個を選ぶ
// does_sort: ソートするかどうか
template <bool does_sort, class T = ll, class U1, class U2>
vc<T> random_sample_range_vector(U1 l, U2 r, int k)
{
  assert(T(r) - T(l) >= T(k));
  vc<T> res(k);
  repi(i, k) res[i] = randint<T>(T(l), T(r) - T(k));
  sort(ALL(res));
  repi(i, k) res[i] += i;
  if (!does_sort)
    shuffle(ALL(res), mt);
  return res;
}
