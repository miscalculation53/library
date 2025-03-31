#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief unordered_map の安全なハッシュ関数
 * @docs docs/ds/safe_unordered_map.md
 */

// https://xuzijian629.hatenablog.com/entry/2019/03/31/130708

struct safe_hash
{
  static ull splitmix64(ull x)
  {
    // http://xorshift.di.unimi.it/splitmix64.c
    x += 0x9e3779b97f4a7c15;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
    x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
    return x ^ (x >> 31);
  }

  size_t operator()(ull x) const
  {
    static const ull FIXED_RANDOM = random_device()();
    return splitmix64(x ^ FIXED_RANDOM);
  }
};
