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
  return uniform_int_distribution<T>(T(l), T(r))(mt);
}
// [l, r) から等確率
template <class T = ll, class U1, class U2>
T randrange(U1 l, U2 r)
{
  assert(T(l) < T(r));
  return uniform_int_distribution<T>(T(l), T(r) - 1)(mt);
}

// [l, r) から一様ランダムな実数を返す
template <class T = double, class U1, class U2>
T randreal(U1 l, U2 r)
{
  assert(T(l) < T(r));
  return uniform_real_distribution<T>(T(l), T(r))(mt);
}

// 確率 p で true を返す
bool randbool(double p)
{
  assert(0 <= p && p <= 1);
  return bernoulli_distribution(p)(mt);
}

namespace internal
{
template <bool does_sort, class V, class T>
void random_sample_range(V &res, T l, T r)
{
  int k = res.size();
  T n = r - l;
  if (k <= 256)
  {
    repi(i, k)
    {
      T j = n - T(k) + T(i), x = randint<T>(0, j);
      if (find(res.begin(), res.begin() + i, x) != res.begin() + i)
        x = j;
      res[i] = x;
    }
  }
  else
  {
    unordered_set<T> used;
    used.reserve(2 * size_t(k));
    repi(i, k)
    {
      T j = n - T(k) + T(i), x = randint<T>(0, j);
      if (!used.insert(x).second)
        x = j, used.insert(x);
      res[i] = x;
    }
  }
  for (T &x : res) x += l;
  if constexpr (does_sort)
    sort(res.begin(), res.end());
  else
    shuffle(res.begin(), res.end(), mt);
}
}; // namespace internal

// [l, r) から相異なる k 個を選ぶ
// does_sort: ソートするかどうか
template <int k, bool does_sort, class T = ll, class U1, class U2>
array<T, k> random_sample_range_array(U1 l, U2 r)
{
  assert(T(r) - T(l) >= T(k));
  array<T, k> res;
  internal::random_sample_range<does_sort>(res, T(l), T(r));
  return res;
}
// [l, r) から相異なる k 個を選ぶ
// does_sort: ソートするかどうか
template <bool does_sort, class T = ll, class U1, class U2>
vc<T> random_sample_range_vector(U1 l, U2 r, int k)
{
  assert(k >= 0);
  assert(T(r) - T(l) >= T(k));
  vc<T> res(k);
  internal::random_sample_range<does_sort>(res, T(l), T(r));
  return res;
}
