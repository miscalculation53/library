#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "ds/dynamic_bitset.hpp"

// Test focus: string conversion, dynamic sizing, range and slice operations,
// shifts, bitwise operations, and 0/1 searches agree with simple implementations.
void verify(const DynamicBitset &bs, const vc<bool> &a)
{
  int n = a.size(), cnt = 0;
  assert(bs.size() == n);
  repi(i, n)
  {
    assert(bs.test(i) == a[i]);
    cnt += a[i];
  }
  assert(bs.count() == cnt);
  assert(bs.any() == (cnt != 0));
  assert(bs.none() == (cnt == 0));
  assert(bs.all() == (cnt == n));

  string s(n, '0');
  repi(i, n) if (a[i]) s[n - 1 - i] = '1';
  assert(bs.to_string() == s);

  int first = n, first_zero = n;
  repi(i, n)
  {
    if (first == n && a[i]) first = i;
    if (first_zero == n && !a[i]) first_zero = i;
  }
  assert(bs.find_first() == first);
  assert(bs.find_first(false) == first_zero);

  repi(i, n)
  {
    int next = n, next_zero = n;
    repi(j, i + 1, n)
    {
      if (next == n && a[j]) next = j;
      if (next_zero == n && !a[j]) next_zero = j;
    }
    assert(bs.find_next(i) == next);
    assert(bs.find_next(i, false) == next_zero);
  }

  int last = -1, last_zero = -1;
  repi(i, n)
  {
    if (a[i]) last = i;
    if (!a[i]) last_zero = i;
  }
  assert(bs.find_last() == last);
  assert(bs.find_last(false) == last_zero);
  repi(i, n + 1)
  {
    int prev = -1, prev_zero = -1;
    repi(j, i)
    {
      if (a[j]) prev = j;
      if (!a[j]) prev_zero = j;
    }
    assert(bs.find_prev(i) == prev);
    assert(bs.find_prev(i, false) == prev_zero);
  }

  repi(_, 20)
  {
    int l = n == 0 ? 0 : randint(0, n);
    int r = randint(l, n);
    int range_cnt = 0;
    repi(i, l, r) range_cnt += a[i];
    assert(bs.count(l, r) == range_cnt);
    assert(bs.any(l, r) == (range_cnt != 0));
    assert(bs.none(l, r) == (range_cnt == 0));
    assert(bs.all(l, r) == (range_cnt == r - l));
  }
}

void shift_left(vc<bool> &a, int k)
{
  int n = a.size();
  repi(i, n - 1, -1, -1) a[i] = i >= k && a[i - k];
}

void shift_right(vc<bool> &a, int k)
{
  int n = a.size();
  repi(i, n) a[i] = i + k < n && a[i + k];
}

void test_slice(mt19937 &rng)
{
  repi(step, 5000)
  {
    int n = rng() % 258, m = rng() % 258;
    bool self = rng() & 1;
    DynamicBitset a(n), b(m);
    vc<bool> av(n), bv(m);
    repi(i, n) if (rng() & 1) a.set(i), av[i] = true;
    repi(i, m) if (rng() & 1) b.set(i), bv[i] = true;

    int sn = self ? n : m;
    int len = rng() % (min(n, sn) + 1);
    int l = rng() % (n - len + 1), bl = rng() % (sn - len + 1);
    int op = rng() % 4;
    vc<bool> old = av, src = self ? av : bv, called(n);

    auto apply = [&](const DynamicBitset &s)
    {
      if (op == 0)
        a.assign_slice(l, l + len, s, bl);
      else if (op == 1)
        a.and_slice(l, l + len, s, bl);
      else if (op == 2)
        a.or_slice(l, l + len, s, bl, [&](int i)
                   {
                     assert(!called[i]);
                     called[i] = true;
                   });
      else
        a.xor_slice(l, l + len, s, bl);
    };
    if (self)
      apply(a);
    else
      apply(b);

    repi(i, len)
    {
      bool x = old[l + i], y = src[bl + i];
      if (op == 0)
        av[l + i] = y;
      else if (op == 1)
        av[l + i] = x && y;
      else if (op == 2)
        av[l + i] = x || y;
      else
        av[l + i] = x != y;
      assert(called[l + i] == (op == 2 && !x && y));
    }
    repi(i, n) if (i < l || l + len <= i) assert(!called[i]);
    verify(a, av);
  }
}

// Test focus: aligned suffix XOR keeps untouched bits and zero padding intact,
// including empty bitsets, non-multiple-of-64 sizes, and self XOR.
void test_aligned_xor_slice()
{
  mt19937_64 rng(20260901);
  for (int n : {0, 1, 63, 64, 65, 127, 128, 129, 255, 256, 257})
    for (int l = 0; l <= n; l += 64)
      for (bool self : {false, true})
        repi(_, 100)
        {
          DynamicBitset a(n), b(n);
          repi(i, n) a.set(i, rng() & 1), b.set(i, rng() & 1);
          auto old = a, src = self ? a : b;
          a.xor_slice(l, n, self ? a : b, l);
          int count = 0;
          repi(i, n)
          {
            bool value = old.test(i) != (i >= l && src.test(i));
            assert(a.test(i) == value);
            count += value;
          }
          assert(a.count() == count);
          a.resize(n + 64);
          repi(i, n, n + 64) assert(!a.test(i));
        }
}

// Test focus: unsigned, most-significant-bit-first ordering agrees with integers
// and equal-length binary strings, including empty and multiword bitsets.
void check_comparison(const DynamicBitset &a, const DynamicBitset &b, int cmp)
{
  assert((a == b) == (cmp == 0));
  assert((a != b) == (cmp != 0));
  assert((a < b) == (cmp < 0));
  assert((a <= b) == (cmp <= 0));
  assert((a > b) == (cmp > 0));
  assert((a >= b) == (cmp >= 0));
}

void test_comparison()
{
  for (int n = 0; n <= 8; n++)
  {
    vc<DynamicBitset> values;
    repi(x, 1 << n)
    {
      DynamicBitset a(n);
      repi(i, n) a.set(i, (x >> i) & 1);
      values.eb(a);
    }
    repi(x, 1 << n) repi(y, 1 << n)
      check_comparison(values[x], values[y], (x > y) - (x < y));
  }

  mt19937_64 rng(20260901);
  for (int n : {0, 1, 63, 64, 65, 127, 128, 129, 255, 256, 257})
  {
    if (n)
    {
      DynamicBitset high(n), low(n, true);
      high.set(n - 1);
      low.reset(n - 1);
      check_comparison(high, low, 1);
      repi(i, n)
      {
        auto a = high, b = high;
        a.reset(i);
        b.set(i);
        check_comparison(a, b, -1);
      }
    }
    vc<DynamicBitset> values;
    vc<string> strings;
    repi(_, 100)
    {
      string sa(n, '0'), sb(n, '0');
      repi(i, n) sa[i] += rng() & 1, sb[i] += rng() & 1;
      DynamicBitset a(sa), b(sb);
      check_comparison(a, b, sa.compare(sb));
      check_comparison(a, a, 0);
      values.eb(a);
      values.eb(a);
      strings.eb(sa);
      strings.eb(sa);
    }
    sort(ALL(values));
    sort(ALL(strings));
    repi(i, values.size()) assert(values[i].to_string() == strings[i]);
    set<DynamicBitset> unique_values(ALL(values));
    set<string> unique_strings(ALL(strings));
    assert(unique_values.size() == unique_strings.size());
  }
  assert(DynamicBitset("1") != DynamicBitset("01"));
  assert(DynamicBitset() != DynamicBitset("0"));
}

int main()
{
  {
    DynamicBitset bs("0010110"), custom("abbaa", 'a', 'b');
    assert(bs.size() == 7);
    assert(bs.to_string() == "0010110");
    assert(!bs.test(0) && bs.test(1) && bs.test(2) && !bs.test(3));
    assert(custom.to_string('a', 'b') == "abbaa");

    stringstream ss("000101");
    ss >> bs;
    assert(bs.size() == 6 && bs.to_string() == "000101");
    assert(bs[0] && !bs[1]);
    bs[1] = true;
    bs[1].flip();
    stringstream out;
    out << bs;
    assert(out.str() == "000101");

    bs.assign("1100");
    assert(bs.size() == 4 && bs.to_string() == "1100");
  }

  mt19937 rng(123456789);
  DynamicBitset bs;
  vc<bool> a;
  repi(step, 3000)
  {
    int op = rng() % 13, n = a.size();
    if (op == 0)
    {
      int m = rng() % 257;
      bool value = rng() & 1;
      int old = n;
      bs.resize(m, value);
      a.resize(m, value);
      repi(i, min(old, m)) assert(bs.test(i) == a[i]);
    }
    else if (op == 1)
    {
      bool value = rng() & 1;
      bs.push_back(value);
      a.eb(value);
    }
    else if (op == 2 && n)
    {
      bs.pop_back();
      a.pop_back();
    }
    else if (op <= 5 && n)
    {
      int i = rng() % n;
      if (op == 3)
      {
        bool value = rng() & 1;
        bs.set(i, value);
        a[i] = value;
      }
      else if (op == 4)
      {
        bs.reset(i);
        a[i] = false;
      }
      else
      {
        bs.flip(i);
        a[i] = !a[i];
      }
    }
    else if (op <= 8)
    {
      int l = n == 0 ? 0 : rng() % (n + 1);
      int r = l + (n == l ? 0 : rng() % (n - l + 1));
      if (op == 6)
      {
        bs.set_range(l, r);
        repi(i, l, r) a[i] = true;
      }
      else if (op == 7)
      {
        bs.reset_range(l, r);
        repi(i, l, r) a[i] = false;
      }
      else
      {
        bs.flip_range(l, r);
        repi(i, l, r) a[i] = !a[i];
      }
    }
    else if (op <= 10)
    {
      int k = rng() % 300;
      if (op == 9)
      {
        bs <<= k;
        shift_left(a, k);
      }
      else if (op == 10)
      {
        bs >>= k;
        shift_right(a, k);
      }
    }
    else
    {
      DynamicBitset b(n);
      vc<bool> c(n);
      repi(i, n) if (rng() & 1) b.set(i), c[i] = true;
      if (op == 11)
      {
        bs ^= b;
        repi(i, n) a[i] = a[i] != c[i];
      }
      else
      {
        bs = ~(bs & b);
        repi(i, n) a[i] = !(a[i] && c[i]);
      }
    }
    verify(bs, a);
  }
  test_slice(rng);
  test_aligned_xor_slice();
  test_comparison();
  cout << "Hello World" << endl;
}
