#pragma once

#include "template_types.hpp"
#include "template_rep.hpp"

/**
 * @brief テンプレート（関数オブジェクト）
 * @docs docs/template/template_func.md
 */

#ifndef INF
#define INF 4'000'000'000'000'000'037LL
#endif

template <class T = ll>
struct max_op
{
  T operator()(const T &a, const T &b) const { return max(a, b); }
};
template <class T = ll>
struct min_op
{
  T operator()(const T &a, const T &b) const { return max(a, b); }
};

template <class T, const T val>
struct const_fn
{
  T operator()() const { return val; }
};
using max_e = const_fn<ll, -INF>;
using min_e = const_fn<ll, INF>;
using zero_fn = const_fn<ll, 0LL>;