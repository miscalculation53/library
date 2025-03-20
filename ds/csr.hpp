#pragma once

#include "../template/template_all.hpp"

/**
 * @brief CSR
 * @docs docs/ds/csr.md
 */

template <class T>
struct CSR
{
private:
  // i (0 <= i < n) 行目を表すのは elist の [start[i], start[i+1])
  int n;
  vc<int> start;
  vc<T> elist;

  struct Row
  {
    using iterator = typename vc<T>::const_iterator;

  private:
    iterator begi, endi;

  public:
    Row(const iterator &begi, const iterator &endi) : begi(begi), endi(endi) {}
    inline iterator begin() const { return begi; }
    inline iterator end() const { return endi; }
    template <class I = ll>
    inline I size() const { return endi - begi; }
    inline bool empty() const { return size() == 0; }

    inline T get(int i) const
    {
      assert(0 <= i && i < size());
      return *(begi + i);
    }
    inline T front() const
    {
      assert(!empty());
      return *begi;
    }
    inline T back() const
    {
      assert(!empty());
      return *prev(endi);
    }
  };

public:
  CSR() {}
  // (i, elem) が格納された vector
  template <class I>
  CSR(int n, const vc<pair<I, T>> &ies) : n(n), elist(ies.size())
  {
    assert(n >= 0);
    start.assign(n, 0);
    fec([ i, e ] : ies)
    {
      assert(0 <= i && i < n);
      start[i]++;
    }
    start = cumlsum(start);
    auto cnt = start;
    fec([ i, e ] : ies) elist[cnt[i]++] = e;
  }
  // vv[i] に elem たちが格納された vector
  CSR(const vvc<T> &vv) : n(vv.size()), start(n + 1)
  {
    int m = 0;
    fec(row : vv) m += row.size();
    elist.resize(m);
    int k = 0;
    repi(i, n)
    {
      start[i] = k;
      fec(e : vv[i]) elist[k++] = e;
    }
    start.back() = m;
  }

  // i 行目
  Row row(int i) const
  {
    if (!(0 <= i && i < n))
      return Row(elist.begin(), elist.begin());
    return Row(elist.begin() + start[i], elist.begin() + start[i + 1]);
  }

  template <class I = ll>
  I size() const { return n; }

  vvc<T> to_vv() const
  {
    vvc<T> res(n);
    repi(i, n) res[i] = {elist.begin() + start[i], elist.begin() + start[i + 1]};
    return res;
  }
};