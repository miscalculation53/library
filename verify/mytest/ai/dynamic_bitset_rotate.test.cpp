#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/dynamic_bitset.hpp"

// Test focus: rotations agree with moving individual bits, including partial
// words, signed distances, copied results, and interleaved size changes.
static_assert(is_same_v<decltype(declval<DynamicBitset &>().rotate_left(0)), DynamicBitset &>);
static_assert(is_same_v<decltype(declval<DynamicBitset &>().rotate_right(0)), DynamicBitset &>);
static_assert(is_same_v<decltype(declval<const DynamicBitset &>().rotated_left(0)), DynamicBitset>);
static_assert(is_same_v<decltype(declval<const DynamicBitset &>().rotated_right(0)), DynamicBitset>);

vc<bool> naive_rotate(const vc<bool> &a, ll k)
{
  int n = a.size();
  vc<bool> result(n);
  repi(i, n)
  {
    int j = (i + k) % n;
    if (j < 0) j += n;
    result[j] = a[i];
  }
  return result;
}

void check(const DynamicBitset &a, const vc<bool> &expected)
{
  assert(a.content() == expected);
  assert(a.count() == count(ALL(expected), true));
  int n = a.size();
  if (n % DynamicBitset::word_bits != 0)
    assert(a.word(a.word_count() - 1) >> (n % DynamicBitset::word_bits) == 0);
}

void check_rotation(const vc<bool> &bits, int k)
{
  DynamicBitset a(bits.size());
  repi(i, bits.size()) a[i] = bits[i];
  const auto original = a;
  const auto left = naive_rotate(bits, ll(k));
  const auto right = naive_rotate(bits, -ll(k));
  const auto l = original.rotated_left(k), r = original.rotated_right(k);
  check(l, left);
  check(r, right);
  check(original, bits);
  assert(l.rotated_right(k) == original);
  assert(r.rotated_left(k) == original);

  assert(&a.rotate_left(k) == &a);
  check(a, left);
  assert(&a.rotate_right(k) == &a);
  check(a, bits);
  assert(&a.rotate_right(k) == &a);
  check(a, right);
  assert(&a.rotate_left(k) == &a);
  check(a, bits);
}

int main()
{
  mt19937 rng(20261031);
  for (int n : {0, 1, 2, 3, 7, 31, 32, 33, 63, 64, 65, 66, 127, 128,
                129, 191, 192, 193, 255, 256, 257, 511, 512, 513, 1000})
  {
    repi(pattern, 7)
    {
      vc<bool> bits(n);
      repi(i, n)
      {
        if (pattern == 0) bits[i] = false;
        else if (pattern == 1) bits[i] = true;
        else if (pattern == 2) bits[i] = i % 2;
        else if (pattern == 3) bits[i] = i == 0;
        else if (pattern == 4) bits[i] = i == n - 1;
        else bits[i] = rng() & 1;
      }
      for (int k : {INT_MIN, INT_MIN + 1, -3 * n - 1, -2 * n, -n - 1, -n,
                    -65, -64, -63, -1, 0, 1, 63, 64, 65, n - 1, n,
                    n + 1, 2 * n, 3 * n + 1, INT_MAX - 1, INT_MAX})
        check_rotation(bits, k);
    }
  }

  repi(n, 11)
    repi(mask, 1 << n)
    {
      vc<bool> bits(n);
      repi(i, n) bits[i] = (mask >> i) & 1;
      repi(k, -n - 1, n + 2) check_rotation(bits, k);
    }

  DynamicBitset a;
  vc<bool> bits;
  repi(step, 6000)
  {
    int op = rng() % 5, n = bits.size();
    if (op == 0)
    {
      int m = rng() % 514;
      bool value = rng() & 1;
      a.resize(m, value);
      bits.resize(m, value);
    }
    else if (op == 1 && n > 0)
    {
      int i = rng() % n;
      auto ref = a[i];
      a.rotate_left(65);
      bits = naive_rotate(bits, 65);
      assert(bool(ref) == bits[i]);
      ref.flip();
      bits[i] = !bits[i];
    }
    else if (op <= 3)
    {
      int k = int(rng() % 4001) - 2000;
      if (op == 2)
        a.rotate_left(k), bits = naive_rotate(bits, k);
      else
        a.rotate_right(k), bits = naive_rotate(bits, -ll(k));
    }
    else
    {
      a.flip();
      repi(i, n) bits[i] = !bits[i];
    }
    check(a, bits);
  }

  vc<bool> large(100003);
  repi(i, large.size()) large[i] = rng() & 1;
  for (int k : {-100003, -65, -1, 0, 1, 63, 64, 65, 50001, 100002, 100003, INT_MAX})
    check_rotation(large, k);

  DynamicBitset moved("10010");
  auto copy = moved.rotated_left(1);
  assert(copy.to_string() == "00101");
  moved.rotate_right(1);
  assert(moved.to_string() == "01001");
  auto final = std::move(copy).rotated_right(1);
  assert(final.to_string() == "10010");
  cout << "Hello World" << endl;
}
