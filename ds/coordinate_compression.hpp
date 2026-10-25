#pragma once

#include "../template/template_all_but_modint.hpp"
#include "../algo/stable_argsort.hpp"

/**
 * @brief 座標圧縮
 * @docs docs/ds/coordinate_compression.md
 */

template <class T>
struct CoordinateCompression
{
  vc<T> vals;
  CoordinateCompression() {}
  CoordinateCompression(const vc<T> &vec)
  {
    if (vec.size() < 4096)
    {
      vals = sortuniqued(vec);
      return;
    }
    auto ord = stable_argsort(vec);
    vals.reserve(vec.size());
    for (int i : ord)
      if (vals.empty() || vals.back() != vec[i]) vals.eb(vec[i]);
  }

  // 添字 i に対応する値
  T get_val(const int i) const
  {
    assert(0 <= i && i < SZ(vals));
    return vals[i];
  }
  // 値 val に対応する添字。存在しなければ nullopt。
  template <class I = ll>
  optional<I> get_id(const T &val) const
  {
    auto it = lower_bound(ALL(vals), val);
    if (it == vals.end() || *it != val)
      return nullopt;
    return I(it - vals.begin());
  }

  template <class I = ll>
  I size() const { return vals.size(); }
  vc<T> content() const { return vals; }
};

// 座標圧縮した後の配列を返す
template <class T, class I = ll>
vc<I> compressed(const vc<T> &vec)
{
  if (vec.size() <= 1) return vc<I>(vec.size(), 0);
  if (vec.size() < 32)
  {
    CoordinateCompression cc(vec);
    vc<I> res(vec.size());
    repi(i, vec.size()) res[i] = *cc.get_id(vec[i]);
    return res;
  }
  auto ord = stable_argsort(vec);
  vc<I> res(vec.size());
  int id = -1, prev = -1;
  for (int i : ord)
  {
    if (prev == -1 || vec[prev] != vec[i]) id++;
    res[i] = id;
    prev = i;
  }
  return res;
}
// 同じ値には同じ id を振るが、id は配列内で先に現れるものから先に振る
template <class T, class I = ll>
vc<I> compressed_unordered(const vc<T> &vec)
{
  auto cv = compressed<T, int>(vec);
  vc<int> id(vec.size(), -1);
  vc<I> res(vec.size());
  int j = 0;
  repi(i, vec.size())
  {
    int &tmp = id[cv[i]];
    if (tmp == -1)
      tmp = j++;
    res[i] = tmp;
  }
  return res;
}
