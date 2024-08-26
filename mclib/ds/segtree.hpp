#pragma once

template <typename S, auto op, auto e>
struct SegmentTree
{
private:
  int n, siz;
  vector<S> dat;
  void update(int i) { dat[i] = op(dat[2 * i], dat[2 * i + 1]); }

public:
  SegmentTree() {}
  SegmentTree(int n) { *this = SegmentTree(vector<S>(n, e())); }
  SegmentTree(const vector<S> &vec) : n(vec.size()), siz(bit_ceil(vec.size())), dat(2 * siz, e())
  {
    for (int i = 0; i < (int)vec.size(); i++) dat[siz + i] = vec[i];
    for (int i = siz - 1; i > 0; i--) update(i);
  }

  void set(int p, const S &x)
  {
    assert(0 <= p && p < n);
    p += siz;
    dat[p] = x;
    while (p > 0) p >>= 1, update(p);
  }

  S get(int p) const { assert(0 <= p && p < n); return dat[siz + p]; }
  S prod(int l, int r) const
  {
    assert(0 <= l && l <= r && r <= n);
    l += siz, r += siz;
    S resl = e(), resr = e();
    while (l < r)
    {
      if (l & 1) resl = op(resl, dat[l++]);
      if (r & 1) resr = op(dat[--r], resr);
      l >>= 1, r >>= 1;
    }
    return op(resl, resr);
  }
  S all_prod() const { return dat[1]; }

  /* todo
  // g が単調のとき g( prod(l, r) ) == true となる最大の r
  template<class G>
  int max_right(int l, const G &g)
  {

  }

  // g が単調のとき g( prod(l, r) ) == true となる最小の l
  template<class G>
  int min_left(int r, const G &g)
  {

  }
  //*/

  vector<S> content()
  {
    vector<S> res(n);
    for (int i = 0; i < n; i++) res[i] = get(i);
    return res;
  }
};