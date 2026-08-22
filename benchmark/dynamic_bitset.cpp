#include "template/template_all_but_modint.hpp"
#include "ds/dynamic_bitset.hpp"

constexpr int bit_n = 1 << 20;
constexpr int shift_count = 2000;
constexpr int matrix_n = 1024;
constexpr int matrix_m = 8192;
constexpr int matrix_mul_n = 512;

volatile ull dynamic_bitset_sink = 0;

template <class F>
double median_ms(F f)
{
  constexpr int rounds = 7;
  array<double, rounds> times;
  repi(i, rounds)
  {
    auto begin = chrono::steady_clock::now();
    dynamic_bitset_sink ^= f();
    auto end = chrono::steady_clock::now();
    times[i] = chrono::duration<double, milli>(end - begin).count();
  }
  sort(ALL(times));
  return times[rounds / 2];
}

ull fixed_shift(const vc<int> &shifts)
{
  bitset<bit_n> dp;
  dp[0] = 1;
  fec(k : shifts) dp |= dp << k;
  return dp.count();
}

ull dynamic_shift(const vc<int> &shifts)
{
  DynamicBitset dp(bit_n);
  dp.set(0);
  fec(k : shifts) dp |= dp << k;
  return dp.count();
}

ull dynamic_fused_shift(const vc<int> &shifts)
{
  DynamicBitset dp(bit_n);
  dp.set(0);
  fec(k : shifts) dp.or_shift_left(k);
  return dp.count();
}

ull raw_fused_shift(const vc<int> &shifts)
{
  vc<ull> dp(bit_n / 64);
  dp[0] = 1;
  fec(k : shifts)
  {
    int dw = k >> 6, db = k & 63;
    repi(i, SZ(dp) - 1, dw - 1, -1)
    {
      ull x = dp[i - dw] << db;
      if (db && i > dw)
        x |= dp[i - dw - 1] >> (64 - db);
      dp[i] |= x;
    }
  }
  ull res = 0;
  fec(x : dp) res += __builtin_popcountll(x);
  return res;
}

ull fixed_subset_sum(const vc<int> &shifts)
{
  bitset<bit_n> dp, diff;
  dp[0] = 1;
  ull checksum = 0;
  fec(k : shifts)
  {
    diff = (dp << k) & ~dp;
    for (int i = diff._Find_first(); i < bit_n; i = diff._Find_next(i))
      checksum += i;
    dp |= dp << k;
  }
  return checksum ^ dp.count();
}

ull fixed_subset_sum_reuse_shift(const vc<int> &shifts)
{
  bitset<bit_n> dp, shifted, diff;
  dp[0] = 1;
  ull checksum = 0;
  fec(k : shifts)
  {
    shifted = dp << k;
    diff = shifted & ~dp;
    for (int i = diff._Find_first(); i < bit_n; i = diff._Find_next(i))
      checksum += i;
    dp |= shifted;
  }
  return checksum ^ dp.count();
}

ull dynamic_subset_sum(const vc<int> &shifts)
{
  DynamicBitset dp(bit_n);
  dp.set(0);
  ull checksum = 0;
  fec(k : shifts) dp.or_shift_left(k, [&](int i) { checksum += i; });
  return checksum ^ dp.count();
}

ull dynamic_subset_sum_operators(const vc<int> &shifts)
{
  DynamicBitset dp(bit_n), diff(bit_n);
  dp.set(0);
  ull checksum = 0;
  fec(k : shifts)
  {
    diff = dp;
    dp |= dp << k;
    diff ^= dp;
    for (int i = diff.find_first(); i < bit_n; i = diff.find_next(i))
      checksum += i;
  }
  return checksum ^ dp.count();
}

ull raw_subset_sum(const vc<int> &shifts)
{
  vc<ull> dp(bit_n / 64);
  dp[0] = 1;
  ull checksum = 0;
  fec(k : shifts)
  {
    int dw = k >> 6, db = k & 63;
    repi(i, SZ(dp) - 1, dw - 1, -1)
    {
      ull x = dp[i - dw] << db;
      if (db && i > dw)
        x |= dp[i - dw - 1] >> (64 - db);
      ull diff = x & ~dp[i];
      while (diff)
      {
        checksum += (i << 6) + __builtin_ctzll(diff);
        diff &= diff - 1;
      }
      dp[i] |= x;
    }
  }
  ull cnt = 0;
  fec(x : dp) cnt += __builtin_popcountll(x);
  return checksum ^ cnt;
}

ull fixed_matrix_xor()
{
  vc<bitset<matrix_m>> a(matrix_n);
  repi(i, matrix_n) repi(j, 0, matrix_m, 97) a[i][(j + i * 13) % matrix_m] = 1;
  repi(_, 40) repi(i, matrix_n) a[i] ^= a[(i * 119 + 1) % matrix_n];
  ull res = 0;
  fec(row : a) res += row.count();
  return res;
}

ull dynamic_matrix_xor()
{
  vc<DynamicBitset> a(matrix_n, DynamicBitset(matrix_m));
  repi(i, matrix_n) repi(j, 0, matrix_m, 97) a[i].set((j + i * 13) % matrix_m);
  repi(_, 40) repi(i, matrix_n) a[i] ^= a[(i * 119 + 1) % matrix_n];
  ull res = 0;
  fec(row : a) res += row.count();
  return res;
}

ull fixed_matrix_multiply()
{
  vc<bitset<matrix_mul_n>> a(matrix_mul_n), b(matrix_mul_n), res(matrix_mul_n);
  repi(i, matrix_mul_n) repi(j, matrix_mul_n)
  {
    a[i][j] = (i * 101 + j * 37) % 17 == 0;
    b[i][j] = (i * 43 + j * 73) % 19 == 0;
  }
  repi(i, matrix_mul_n)
  {
    auto row = a[i];
    for (int j = row._Find_first(); j < matrix_mul_n; j = row._Find_next(j))
      res[i] ^= b[j];
  }
  ull sum = 0;
  fec(row : res) sum += row.count();
  return sum;
}

ull dynamic_matrix_multiply()
{
  vc<DynamicBitset> a(matrix_mul_n, DynamicBitset(matrix_mul_n));
  vc<DynamicBitset> b(matrix_mul_n, DynamicBitset(matrix_mul_n));
  vc<DynamicBitset> res(matrix_mul_n, DynamicBitset(matrix_mul_n));
  repi(i, matrix_mul_n) repi(j, matrix_mul_n)
  {
    a[i].set(j, (i * 101 + j * 37) % 17 == 0);
    b[i].set(j, (i * 43 + j * 73) % 19 == 0);
  }
  repi(i, matrix_mul_n)
  {
    auto row = a[i];
    for (int j = row.find_first(); j < matrix_mul_n; j = row.find_next(j))
      res[i] ^= b[j];
  }
  ull sum = 0;
  fec(row : res) sum += row.count();
  return sum;
}

int main()
{
  mt19937 rng(123456789);
  vc<int> shifts(shift_count);
  repi(i, shift_count) shifts[i] = rng() % 1000 + 1;

  ull expected = fixed_shift(shifts);
  assert(dynamic_shift(shifts) == expected);
  assert(dynamic_fused_shift(shifts) == expected);
  assert(raw_fused_shift(shifts) == expected);
  assert(fixed_subset_sum(shifts) == dynamic_subset_sum(shifts));
  assert(fixed_subset_sum(shifts) == dynamic_subset_sum_operators(shifts));
  assert(fixed_subset_sum(shifts) == fixed_subset_sum_reuse_shift(shifts));
  assert(fixed_subset_sum(shifts) == raw_subset_sum(shifts));
  assert(fixed_matrix_xor() == dynamic_matrix_xor());
  assert(fixed_matrix_multiply() == dynamic_matrix_multiply());

  cout << "shift-or: n=" << bit_n << ", operations=" << shift_count << '\n';
  cout << fixed << setprecision(3);
  cout << "std::bitset expression  " << median_ms([&] { return fixed_shift(shifts); }) << " ms\n";
  cout << "Dynamic expression      " << median_ms([&] { return dynamic_shift(shifts); }) << " ms\n";
  cout << "Dynamic fused          " << median_ms([&] { return dynamic_fused_shift(shifts); }) << " ms\n";
  cout << "raw words fused        " << median_ms([&] { return raw_fused_shift(shifts); }) << " ms\n";

  cout << "subset-sum shift and enumerate new bits\n";
  cout << "std::bitset old       " << median_ms([&] { return fixed_subset_sum(shifts); }) << " ms\n";
  cout << "std::bitset reuse     " << median_ms([&] { return fixed_subset_sum_reuse_shift(shifts); }) << " ms\n";
  cout << "Dynamic operators     " << median_ms([&] { return dynamic_subset_sum_operators(shifts); }) << " ms\n";
  cout << "Dynamic fused         " << median_ms([&] { return dynamic_subset_sum(shifts); }) << " ms\n";
  cout << "raw words fused       " << median_ms([&] { return raw_subset_sum(shifts); }) << " ms\n";

  cout << "matrix xor: " << matrix_n << " x " << matrix_m << ", 40 passes\n";
  cout << "std::bitset rows       " << median_ms(fixed_matrix_xor) << " ms\n";
  cout << "Dynamic rows          " << median_ms(dynamic_matrix_xor) << " ms\n";
  cout << "matrix multiply-like: " << matrix_mul_n << " x " << matrix_mul_n << '\n';
  cout << "std::bitset rows       " << median_ms(fixed_matrix_multiply) << " ms\n";
  cout << "Dynamic rows          " << median_ms(dynamic_matrix_multiply) << " ms\n";
  cerr << dynamic_bitset_sink << '\n';
}
