#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief CSR
 * @docs docs/ds/csr.md
 */

template <class T>
struct CSR
{
protected:
  int n, m;
  // i (0 <= i < n) 行目を表すのは elist の [start[i], start[i+1])
  vc<int> start;
  vc<T> elist;
  vc<int> eid_to_elistid;

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

    inline T operator[](int i) const { return *(begi + i); }
    inline T at(int i) const
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

    vc<T> to_v() const { return vc<T>(ALL(*this)); }
  };

public:
  CSR() {}
  // (i, elem) が格納された vector
  template <class I>
  CSR(int n, const vc<pair<I, T>> &ies) : n(n), m(ies.size()), start(n, 0), elist(m), eid_to_elistid(m)
  {
    fec([ i, e ] : ies)
    {
      assert(0 <= i && i < n);
      start[i]++;
    }
    start = cumlsum(start);
    auto cnt = start;
    repi(j, m)
    {
      cauto &[i, e] = ies[j];
      int &k = cnt[i];
      elist[k] = e;
      eid_to_elistid[j] = k;
      k++;
    }
  }
  // vv[i] に elem たちが格納された vector
  CSR(const vvc<T> &vv) : n(vv.size()), start(n + 1, 0)
  {
    m = 0;
    fec(row : vv) m += row.size();
    elist.resize(m);
    eid_to_elistid.resize(m);
    int k = 0;
    for (int i = 0, j = 0; i < n; i++)
    {
      start[i] = k;
      fec(e : vv[i])
      {
        elist[k] = e;
        eid_to_elistid[j++] = k;
        k++;
      }
    }
    start.back() = m;
  }

  Row operator[](int i) const { return Row(elist.begin() + start[i], elist.begin() + start[i + 1]); }
  Row at(int i) const
  {
    if (!(0 <= i && i < n))
      return Row(elist.begin(), elist.begin());
    return Row(elist.begin() + start[i], elist.begin() + start[i + 1]);
  }

  template <class I = ll>
  I size() const { return n; }

  const T &find_by_eid(int eid) const
  {
    assert(0 <= eid && eid < m);
    return elist[eid_to_elistid[eid]];
  }

  vvc<T> to_vv() const
  {
    vvc<T> res(n);
    repi(i, n) res[i] = {elist.begin() + start[i], elist.begin() + start[i + 1]};
    return res;
  }
};
