#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "ds/dynamic_bitset.hpp"

// Test focus: string conversion, dynamic sizing, range operations, shifts,
// bitwise operations, and 0/1 searches agree with simple implementations.
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
    int op = rng() % 15, n = a.size();
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
    else if (op <= 12)
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
      else if (op == 11)
      {
        vc<bool> old = a;
        vc<bool> changed(n);
        bs.or_shift_left(k, [&](int i)
                         {
                           assert(!changed[i]);
                           changed[i] = true;
                         });
        repi(i, n)
        {
          bool expected = i >= k && !old[i] && old[i - k];
          assert(changed[i] == expected);
          if (i >= k)
            a[i] = a[i] || old[i - k];
        }
      }
      else
      {
        vc<bool> old = a;
        vc<bool> changed(n);
        bs.or_shift_right(k, [&](int i)
                          {
                            assert(!changed[i]);
                            changed[i] = true;
                          });
        repi(i, n)
        {
          bool expected = i + k < n && !old[i] && old[i + k];
          assert(changed[i] == expected);
          if (i + k < n)
            a[i] = a[i] || old[i + k];
        }
      }
    }
    else
    {
      DynamicBitset b(n);
      vc<bool> c(n);
      repi(i, n) if (rng() & 1) b.set(i), c[i] = true;
      if (op == 13)
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
  cout << "Hello World" << endl;
}
