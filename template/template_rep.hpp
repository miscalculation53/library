#pragma once

#include "template_types.hpp"

/**
 * @brief テンプレート（rep）
 * @docs docs/template/template_rep.md
 */

/**
 * 参考：
 * https://trap.jp/post/1224/
*/

#define overload4(_1, _2, _3, _4, name, ...) name
#define rep1(i, n) for (ll i = 0, nnnnn = ll(n); i < nnnnn; i++)
#define rep2(i, l, r) for (ll i = ll(l), rrrrr = ll(r); i < rrrrr; i++)
#define rep3(i, l, r, d) for (ll i = ll(l), rrrrr = ll(r), ddddd = ll(d); ddddd > 0 ? i < rrrrr : i > rrrrr; i += d)
#define rep(...) overload4(__VA_ARGS__, rep3, rep2, rep1)(__VA_ARGS__)
#define repi1(i, n) for (int i = 0, nnnnn = int(n); i < nnnnn; i++)
#define repi2(i, l, r) for (int i = int(l), rrrrr = int(r); i < rrrrr; i++)
#define repi3(i, l, r, d) for (int i = int(l), rrrrr = int(r), ddddd = int(d); ddddd > 0 ? i < rrrrr : i > rrrrr; i += d)
#define repi(...) overload4(__VA_ARGS__, repi3, repi2, repi1)(__VA_ARGS__)

#define fe(...) for (auto __VA_ARGS__)
#define fec(...) for (cauto &__VA_ARGS__)
#define fem(...) for (auto &__VA_ARGS__)