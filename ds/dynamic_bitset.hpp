#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief 長さを実行時に決める bitset
 * @docs docs/ds/dynamic_bitset.md
 */

struct DynamicBitset
{
  using Word = ull;
  static constexpr int word_bits = numeric_limits<Word>::digits;

private:
  int n;
  vc<Word> dat;

  static int word_size(int n) { return (n + word_bits - 1) / word_bits; }
  static Word low_mask(int k)
  {
    return k == word_bits ? ~Word(0) : (Word(1) << k) - 1;
  }
  Word last_mask() const
  {
    return n % word_bits == 0 ? ~Word(0) : low_mask(n % word_bits);
  }
  void trim()
  {
    if (!dat.empty())
      dat.back() &= last_mask();
  }

  Word get_word(int i) const
  {
    int w = i / word_bits, b = i % word_bits;
    Word x = dat[w] >> b;
    if (b && w + 1 < SZ(dat))
      x |= dat[w + 1] << (word_bits - b);
    return x;
  }

  template <int op, class F>
  DynamicBitset &apply_slice(int l, int r, const DynamicBitset &b, int bl, const F &f)
  {
    assert(0 <= l && l <= r && r <= n);
    assert(0 <= bl && bl + r - l <= b.n);
    if (l == r)
      return *this;

    auto apply_word = [&](int w)
    {
      int lo = max(l, w * word_bits), hi = min(r, (w + 1) * word_bits);
      int len = hi - lo, shift = lo % word_bits;
      Word mask = low_mask(len) << shift;
      Word x = (b.get_word(bl + lo - l) & low_mask(len)) << shift;
      Word old = dat[w], y;
      if constexpr (op == 0)
        y = x;
      else if constexpr (op == 1)
        y = old & x;
      else if constexpr (op == 2)
        y = old | x;
      else
        y = old ^ x;
      dat[w] = (old & ~mask) | (y & mask);
      if constexpr (!is_same_v<F, nullptr_t>)
      {
        Word added = dat[w] & ~old & mask;
        while (added)
        {
          f(w * word_bits + __builtin_ctzll(added));
          added &= added - 1;
        }
      }
    };

    int lw = l / word_bits, rw = (r - 1) / word_bits;
    if (this == &b && bl < l && l < bl + r - l)
      repi(w, rw, lw - 1, -1) apply_word(w);
    else
      repi(w, lw, rw + 1) apply_word(w);
    return *this;
  }

public:
  struct Reference
  {
    DynamicBitset *bs;
    int pos;

    operator bool() const { return bs->test(pos); }
    Reference &operator=(bool value)
    {
      bs->set(pos, value);
      return *this;
    }
    Reference &operator=(const Reference &ref) { return *this = bool(ref); }
    Reference &flip()
    {
      bs->flip(pos);
      return *this;
    }
  };

  DynamicBitset() : n(0) {}
  // 長さ n、全ビット value で初期化する。
  DynamicBitset(int n, bool value = false) : n(n), dat(n < 0 ? 0 : word_size(n), value ? ~Word(0) : 0)
  {
    assert(n >= 0);
    trim();
  }
  // s の右端を 0 bit 目として初期化する。
  explicit DynamicBitset(const string &s, char zero = '0', char one = '1') : n(0)
  {
    assign(s, zero, one);
  }

  // ビット数を返す。
  int size() const { return n; }
  // 長さが 0 か返す。
  bool empty() const { return n == 0; }
  // 再確保せずに保持できるビット数を返す。
  int capacity() const { return int(dat.capacity()) * word_bits; }

  // 再確保せずに保持できる長さを n 以上にする。
  void reserve(int n)
  {
    assert(n >= 0);
    dat.reserve(word_size(n));
  }

  // 長さを n に変更し、新しく増えたビットを value にする。
  void resize(int n, bool value = false)
  {
    assert(n >= 0);
    int old = this->n;
    dat.resize(word_size(n));
    this->n = n;
    if (value && old < n)
      set_range(old, n);
    trim();
  }

  // s の右端を 0 bit 目として代入する。
  DynamicBitset &assign(const string &s, char zero = '0', char one = '1')
  {
    assert(zero != one);
    assert(s.size() <= size_t(numeric_limits<int>::max()));
    n = int(s.size());
    dat.assign(word_size(n), 0);
    repi(i, n)
    {
      char c = s[n - 1 - i];
      assert(c == zero || c == one);
      if (c == one)
        dat[i / word_bits] |= Word(1) << (i % word_bits);
    }
    return *this;
  }

  // i 番目のビットを返す。
  bool test(int i) const
  {
    assert(0 <= i && i < n);
    return dat[i / word_bits] >> (i % word_bits) & 1;
  }
  bool operator[](int i) const { return test(i); }
  Reference operator[](int i)
  {
    assert(0 <= i && i < n);
    return {this, i};
  }

  // i 番目のビットを value にする。
  DynamicBitset &set(int i, bool value = true)
  {
    assert(0 <= i && i < n);
    Word mask = Word(1) << (i % word_bits);
    if (value)
      dat[i / word_bits] |= mask;
    else
      dat[i / word_bits] &= ~mask;
    return *this;
  }
  // i 番目のビットを 0 にする。
  DynamicBitset &reset(int i) { return set(i, false); }
  // i 番目のビットを反転する。
  DynamicBitset &flip(int i)
  {
    assert(0 <= i && i < n);
    dat[i / word_bits] ^= Word(1) << (i % word_bits);
    return *this;
  }
  // 全ビットを 1 にする。
  DynamicBitset &set()
  {
    fill(ALL(dat), ~Word(0));
    trim();
    return *this;
  }
  // 全ビットを 0 にする。
  DynamicBitset &reset()
  {
    fill(ALL(dat), 0);
    return *this;
  }
  // 全ビットを反転する。
  DynamicBitset &flip()
  {
    fem(x : dat) x = ~x;
    trim();
    return *this;
  }

  // [l, r) のビットを value にする。
  DynamicBitset &set_range(int l, int r, bool value = true)
  {
    assert(0 <= l && l <= r && r <= n);
    if (!value)
      return reset_range(l, r);
    if (l == r)
      return *this;
    int lw = l / word_bits, rw = (r - 1) / word_bits;
    if (lw == rw)
      dat[lw] |= low_mask((r - 1) % word_bits + 1) & ~low_mask(l % word_bits);
    else
    {
      dat[lw] |= ~low_mask(l % word_bits);
      fill(dat.begin() + lw + 1, dat.begin() + rw, ~Word(0));
      dat[rw] |= low_mask((r - 1) % word_bits + 1);
    }
    trim();
    return *this;
  }

  // [l, r) のビットを 0 にする。
  DynamicBitset &reset_range(int l, int r)
  {
    assert(0 <= l && l <= r && r <= n);
    if (l == r)
      return *this;
    int lw = l / word_bits, rw = (r - 1) / word_bits;
    if (lw == rw)
      dat[lw] &= ~(low_mask((r - 1) % word_bits + 1) & ~low_mask(l % word_bits));
    else
    {
      dat[lw] &= low_mask(l % word_bits);
      fill(dat.begin() + lw + 1, dat.begin() + rw, 0);
      dat[rw] &= ~low_mask((r - 1) % word_bits + 1);
    }
    return *this;
  }

  // [l, r) のビットを反転する。
  DynamicBitset &flip_range(int l, int r)
  {
    assert(0 <= l && l <= r && r <= n);
    if (l == r)
      return *this;
    int lw = l / word_bits, rw = (r - 1) / word_bits;
    if (lw == rw)
      dat[lw] ^= low_mask((r - 1) % word_bits + 1) & ~low_mask(l % word_bits);
    else
    {
      dat[lw] ^= ~low_mask(l % word_bits);
      repi(w, lw + 1, rw) dat[w] = ~dat[w];
      dat[rw] ^= low_mask((r - 1) % word_bits + 1);
    }
    trim();
    return *this;
  }

  // 1 の個数を返す。
  int count() const
  {
    int res = 0;
    fec(x : dat) res += __builtin_popcountll(x);
    return res;
  }

  // [l, r) にある 1 の個数を返す。
  int count(int l, int r) const
  {
    assert(0 <= l && l <= r && r <= n);
    if (l == r)
      return 0;
    int lw = l / word_bits, rw = (r - 1) / word_bits;
    if (lw == rw)
      return __builtin_popcountll(dat[lw] & low_mask((r - 1) % word_bits + 1) & ~low_mask(l % word_bits));
    int res = __builtin_popcountll(dat[lw] & ~low_mask(l % word_bits));
    repi(w, lw + 1, rw) res += __builtin_popcountll(dat[w]);
    return res + __builtin_popcountll(dat[rw] & low_mask((r - 1) % word_bits + 1));
  }

  // 1 が存在するか返す。
  bool any() const
  {
    fec(x : dat) if (x) return true;
    return false;
  }
  // [l, r) に 1 が存在するか返す。
  bool any(int l, int r) const { return count(l, r) != 0; }
  // 1 が存在しないか返す。
  bool none() const { return !any(); }
  // [l, r) に 1 が存在しないか返す。
  bool none(int l, int r) const { return !any(l, r); }
  // 全ビットが 1 か返す。
  bool all() const { return count() == n; }
  // [l, r) の全ビットが 1 か返す。
  bool all(int l, int r) const { return count(l, r) == r - l; }

  // 最初の value、存在しなければ size() を返す。
  int find_first(bool value = true) const { return find_next(-1, value); }
  // i より右にある最初の value、存在しなければ size() を返す。
  int find_next(int i, bool value = true) const
  {
    assert(-1 <= i && i < n);
    i++;
    if (i == n)
      return n;
    int w = i / word_bits, b = i % word_bits;
    Word x = (value ? dat[w] : ~dat[w]) & (~Word(0) << b);
    while (true)
    {
      if (x)
      {
        int res = w * word_bits + __builtin_ctzll(x);
        return min(res, n);
      }
      if (++w == SZ(dat))
        return n;
      x = value ? dat[w] : ~dat[w];
    }
  }
  // 最後の value、存在しなければ -1 を返す。
  int find_last(bool value = true) const { return find_prev(n, value); }
  // i より左にある最初の value、存在しなければ -1 を返す。
  int find_prev(int i, bool value = true) const
  {
    assert(0 <= i && i <= n);
    if (i == 0)
      return -1;
    i--;
    int w = i / word_bits, b = i % word_bits;
    Word x = (value ? dat[w] : ~dat[w]) & low_mask(b + 1);
    while (true)
    {
      if (x)
        return w * word_bits + 63 - __builtin_clzll(x);
      if (w-- == 0)
        return -1;
      x = value ? dat[w] : ~dat[w];
    }
  }

  DynamicBitset &operator&=(const DynamicBitset &b)
  {
    assert(n == b.n);
    repi(i, SZ(dat)) dat[i] &= b.dat[i];
    return *this;
  }
  DynamicBitset &operator|=(const DynamicBitset &b)
  {
    assert(n == b.n);
    repi(i, SZ(dat)) dat[i] |= b.dat[i];
    return *this;
  }
  DynamicBitset &operator^=(const DynamicBitset &b)
  {
    assert(n == b.n);
    repi(i, SZ(dat)) dat[i] ^= b.dat[i];
    return *this;
  }
  friend DynamicBitset operator&(DynamicBitset a, const DynamicBitset &b) { return a &= b; }
  friend DynamicBitset operator|(DynamicBitset a, const DynamicBitset &b) { return a |= b; }
  friend DynamicBitset operator^(DynamicBitset a, const DynamicBitset &b) { return a ^= b; }
  DynamicBitset operator~() const { return DynamicBitset(*this).flip(); }

  DynamicBitset &operator<<=(int k)
  {
    assert(k >= 0);
    if (k >= n)
      return reset();
    if (k == 0)
      return *this;
    int dw = k / word_bits, db = k % word_bits;
    repi(i, SZ(dat) - 1, dw - 1, -1)
    {
      Word x = dat[i - dw] << db;
      if (db && i > dw)
        x |= dat[i - dw - 1] >> (word_bits - db);
      dat[i] = x;
    }
    fill(dat.begin(), dat.begin() + dw, 0);
    trim();
    return *this;
  }

  DynamicBitset &operator>>=(int k)
  {
    assert(k >= 0);
    if (k >= n)
      return reset();
    if (k == 0)
      return *this;
    int dw = k / word_bits, db = k % word_bits, m = SZ(dat);
    repi(i, m - dw)
    {
      Word x = dat[i + dw] >> db;
      if (db && i + dw + 1 < m)
        x |= dat[i + dw + 1] << (word_bits - db);
      dat[i] = x;
    }
    fill(dat.end() - dw, dat.end(), 0);
    trim();
    return *this;
  }
  DynamicBitset operator<<(int k) const { return DynamicBitset(*this) <<= k; }
  DynamicBitset operator>>(int k) const { return DynamicBitset(*this) >>= k; }

  DynamicBitset &assign_slice(int l, int r, const DynamicBitset &b, int bl)
  {
    return apply_slice<0>(l, r, b, bl, nullptr);
  }
  DynamicBitset &and_slice(int l, int r, const DynamicBitset &b, int bl)
  {
    return apply_slice<1>(l, r, b, bl, nullptr);
  }
  DynamicBitset &or_slice(int l, int r, const DynamicBitset &b, int bl)
  {
    return apply_slice<2>(l, r, b, bl, nullptr);
  }
  template <class F>
  DynamicBitset &or_slice(int l, int r, const DynamicBitset &b, int bl, const F &f)
  {
    return apply_slice<2>(l, r, b, bl, f);
  }
  DynamicBitset &xor_slice(int l, int r, const DynamicBitset &b, int bl)
  {
    return apply_slice<3>(l, r, b, bl, nullptr);
  }

  // 末尾に value を追加する。
  void push_back(bool value)
  {
    int i = n;
    resize(n + 1);
    if (value)
      set(i);
  }
  // 末尾のビットを削除する。
  void pop_back()
  {
    assert(n > 0);
    resize(n - 1);
  }

  // 上位ビットから並べた文字列を返す。
  string to_string(char zero = '0', char one = '1') const
  {
    assert(zero != one);
    string res(n, zero);
    repi(i, n) if (test(i)) res[n - 1 - i] = one;
    return res;
  }

  friend bool operator==(const DynamicBitset &a, const DynamicBitset &b)
  {
    return a.n == b.n && a.dat == b.dat;
  }
  friend bool operator!=(const DynamicBitset &a, const DynamicBitset &b) { return !(a == b); }
};

#if defined LOCAL or not defined FAST_IO
istream &operator>>(istream &is, DynamicBitset &a)
{
  string s;
  is >> s;
  if (is)
    a.assign(s);
  return is;
}
ostream &operator<<(ostream &os, const DynamicBitset &a)
{
  return os << a.to_string();
}
#else
void rd1(DynamicBitset &a)
{
  string s;
  fastio::rd1(s);
  a.assign(s);
}
void wt1(const DynamicBitset &a)
{
  fastio::wt1(a.to_string());
}
#endif
