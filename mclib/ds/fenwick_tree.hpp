#pragma once

// http://hos.ac/slides/20140319_bit.pdf
template<class S>
struct FenwickTree
{
private:
  int n;
  vector<S> dat;

public:
  FenwickTree() {}
  FenwickTree(int n) : n(n), dat(n + 1, 0) {}

  S sum(int i) const
  {
    assert(0 <= i && i <= n);
    S s = 0;
    while (i > 0)
    {
      s += dat[i];
      i -= i & -i;
    }
    return s;
  }
  S sum(int l, int r) const
  {
    assert(l <= r);
    return sum(r) - sum(l);
  }

  S get(int i) const { return sum(i, i + 1); }

  void add(int i, S x)
  {
    assert(0 <= i && i < n);
    i++;
    while (i <= n)
    {
      dat[i] += x;
      i += i & -i;
    }
  }
  void set(int i, S x) { add(i, x - get(i)); }

  // 和が w 以上になる最小の index (w < 0 なら -1)
  int lower_bound(S w) const
  {
    if (w < 0) return -1;
    int k = __bit_ceil(n);
    int x = 0;
    while (k > 0)
    {
      if (x + k - 1 < n && dat[x + k] < w)
      {
        w -= dat[x + k];
        x += k;
      }
      k >>= 1;
    }
    return x;
  }
  // 和が w より大きくなる最小の index (w <= 0 なら -1)
  int upper_bound(S w) const { return lower_bound(w + 1); }
};