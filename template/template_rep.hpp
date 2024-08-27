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
#define rep1(i, n) for (ll i = 0; i < ll(n); i++)
#define rep2(i, l, r) for (ll i = ll(l); i < ll(r); i++)
#define rep3(i, l, r, d) for (ll i = ll(l); (d) > 0 ? i < ll(r) : i > ll(r); i += d)
#define rep(...) overload4(__VA_ARGS__, rep3, rep2, rep1)(__VA_ARGS__)
#define repi1(i, n) for (int i = 0; i < int(n); i++)
#define repi2(i, l, r) for (int i = int(l); i < int(r); i++)
#define repi3(i, l, r, d) for (int i = int(l); (d) > 0 ? i < int(r) : i > int(r); i += d)
#define repi(...) overload4(__VA_ARGS__, repi3, repi2, repi1)(__VA_ARGS__)