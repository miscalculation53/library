#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief CSR
 * @docs docs/ds/csr.md
 */

template <class T, bool is_erasable = false>
struct CSR
{
protected:
  int n, m;
  // i (0 <= i < n) 行目を表すのは elist の [start[i], start[i+1])
  // pop_back する場合は [start[i], start[i] + len[i])
  vc<int> start;
  vc<T> elist;
  vc<int> len;
  inline int get_last(int i) const
  {
    if constexpr (is_erasable)
      return start[i] + len[i];
    else
      return start[i + 1];
  }

  struct Row
  {
    using iterator = typename vc<T>::iterator;

  private:
    iterator begi, endi;

  public:
    Row(const iterator &begi, const iterator &endi) : begi(begi), endi(endi) {}
    inline iterator begin() const { return begi; }
    inline iterator end() const { return endi; }
    template <class I = ll>
    inline I size() const { return endi - begi; }
    inline bool empty() const { return size() == 0; }

    inline T &operator[](int i) const { return *(begi + i); }
    inline T &at(int i) const
    {
      assert(0 <= i && i < size());
      return *(begi + i);
    }

    inline T &front() const
    {
      assert(!empty());
      return *begi;
    }
    inline T &back() const
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
  CSR(int n, const vc<pair<I, T>> &ies) : n(n), m(ies.size()), start(n, 0), elist(m)
  {
    if constexpr (is_erasable)
      len.resize(n);
    fec([ i, e ] : ies)
    {
      assert(0 <= i && i < n);
      start[i]++;
    }
    start = cumlsum(start);
    if constexpr (is_erasable)
      repi(i, n) len[i] = start[i + 1] - start[i];
    auto cnt = start;
    repi(j, m)
    {
      cauto & [ i, e ] = ies[j];
      int &k = cnt[i];
      elist[k] = e;
      k++;
    }
  }
  // vv[i] に elem たちが格納された vector
  CSR(const vvc<T> &vv) : n(vv.size()), start(n + 1, 0)
  {
    m = 0;
    fec(row : vv) m += row.size();
    elist.resize(m);
    if constexpr (is_erasable)
      len.resize(n);
    for (int i = 0, j = 0; i < n; i++)
    {
      start[i] = j;
      if constexpr (is_erasable)
        len[i] = vv[i].size();
      fec(e : vv[i])
      {
        elist[j] = e;
        j++;
      }
    }
    start.back() = m;
  }

  Row operator[](int i) { return Row(elist.begin() + start[i], elist.begin() + get_last(i)); }
  Row operator[](int i) const
  {
    auto beg = const_cast<vc<T> &>(elist).begin();
    return Row(beg + start[i], beg + get_last(i));
  }
  Row at(int i)
  {
    if (!(0 <= i && i < n))
      return Row(elist.begin(), elist.begin());
    return Row(elist.begin() + start[i], elist.begin() + get_last(i));
  }
  Row at(int i) const
  {
    auto beg = const_cast<vc<T> &>(elist).begin();
    if (!(0 <= i && i < n))
      return Row(beg, beg);
    return Row(beg + start[i], beg + get_last(i));
  }

  void pop_back(int i)
  {
    static_assert(is_erasable);
    assert(len[i] > 0);
    len[i]--;
  }

  template <class I = ll>
  I size() const { return n; }

  vvc<T> to_vv() const
  {
    vvc<T> res(n);
    repi(i, n) res[i] = {elist.begin() + start[i], elist.begin() + get_last(i)};
    return res;
  }

  vc<T> &get_elist() { return elist; }
  const vc<T> &get_elist() const { return elist; }
};
