#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief ビットリバース
 * @docs docs/bit/bit_reverse.md
 */

uint32_t bit_reverse32(uint32_t x)
{
  x = ((x & 0x55555555) << 1) | ((x & 0xAAAAAAAA) >> 1);
  x = ((x & 0x33333333) << 2) | ((x & 0xCCCCCCCC) >> 2);
  x = ((x & 0x0F0F0F0F) << 4) | ((x & 0xF0F0F0F0) >> 4);
  x = ((x & 0x00FF00FF) << 8) | ((x & 0xFF00FF00) >> 8);
  return (x << 16) | (x >> 16);
}
// [0, pw2) の範囲の i をビットリバース
int bitrev(int pw2, int i) { return bit_reverse32(i) >> (32 - countr_zero(pw2)); }
