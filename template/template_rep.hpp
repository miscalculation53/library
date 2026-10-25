#pragma once

#include "template_types.hpp"

/**
 * @brief テンプレート（rep）
 * @docs docs/template/template_rep.md
 */

#define overload4(_1, _2, _3, _4, name, ...) name
#define LIBRARY_REPEAT_LOOP(T, i, l, r, d) \
  for (auto [i, library_repeat_end, library_repeat_step] = array<T, 3>{T(l), T(r), T(d)}; \
       library_repeat_step > 0 ? i < library_repeat_end : i > library_repeat_end; \
       i += library_repeat_step)
#define rep1(i, n) LIBRARY_REPEAT_LOOP(ll, i, 0, n, 1)
#define rep2(i, l, r) LIBRARY_REPEAT_LOOP(ll, i, l, r, 1)
#define rep3(i, l, r, d) LIBRARY_REPEAT_LOOP(ll, i, l, r, d)
#define rep(...) overload4(__VA_ARGS__, rep3, rep2, rep1)(__VA_ARGS__)
#define repi1(i, n) LIBRARY_REPEAT_LOOP(int, i, 0, n, 1)
#define repi2(i, l, r) LIBRARY_REPEAT_LOOP(int, i, l, r, 1)
#define repi3(i, l, r, d) LIBRARY_REPEAT_LOOP(int, i, l, r, d)
#define repi(...) overload4(__VA_ARGS__, repi3, repi2, repi1)(__VA_ARGS__)

#define fe(...) for (auto __VA_ARGS__)
#define fec(...) for (cauto &__VA_ARGS__)
#define fem(...) for (auto &__VA_ARGS__)
