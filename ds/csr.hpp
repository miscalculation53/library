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

  template <class Iter>
  struct RowBase
  {
    using iterator = Iter;
    using reference = typename iterator_traits<iterator>::reference;

  private:
    iterator begi, endi;

  public:
    RowBase(const iterator &begi, const iterator &endi) : begi(begi), endi(endi) {}
    inline iterator begin() const { return begi; }
    inline iterator end() const { return endi; }
    template <class I = ll>
    inline I size() const { return endi - begi; }
    inline bool empty() const { return size() == 0; }

    inline reference operator[](int i) const { return *(begi + i); }
    inline reference at(int i) const
    {
      assert(0 <= i && i < size());
      return *(begi + i);
    }

    inline reference front() const
    {
      assert(!empty());
      return *begi;
    }
    inline reference back() const
    {
      assert(!empty());
      return *prev(endi);
    }

    vc<T> to_v() const { return vc<T>(ALL(*this)); }
  };
  using Row = RowBase<typename vc<T>::iterator>;
  using ConstRow = RowBase<typename vc<T>::const_iterator>;

public:
  CSR() {}
  // 各行の要素数を指定し、値初期化された CSR を構築する
  CSR(const vc<int> &row_sizes)
      : n(row_sizes.size())
  {
    fec(s : row_sizes) assert(s >= 0);
    start = cumlsum(row_sizes);
    m = start.back();
    elist.resize(m);
    if constexpr (is_erasable)
      len = row_sizes;
  }
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
  ConstRow operator[](int i) const
  { return ConstRow(elist.begin() + start[i], elist.begin() + get_last(i)); }
  Row at(int i)
  {
    if (!(0 <= i && i < n))
      return Row(elist.begin(), elist.begin());
    return Row(elist.begin() + start[i], elist.begin() + get_last(i));
  }
  ConstRow at(int i) const
  {
    if (!(0 <= i && i < n))
      return ConstRow(elist.begin(), elist.begin());
    return ConstRow(elist.begin() + start[i], elist.begin() + get_last(i));
  }

  void pop_back(int i)
  {
    static_assert(is_erasable);
    assert(len[i] > 0);
    len[i]--;
  }

  // 第 i 行の先頭の、平坦な要素列における添字を返す
  int offset(int i) const
  {
    assert(0 <= i && i <= n);
    return start[i];
  }

  // 各行をソートし、同じ行の重複要素を削除する
  void sortunique()
  {
    vc<int> nstart(n + 1);
    int k = 0;
    repi(i, n)
    {
      const int l = start[i], r = get_last(i);
      sort(elist.begin() + l, elist.begin() + r);
      auto ed = unique(elist.begin() + l, elist.begin() + r);
      nstart[i] = k;
      for (int j = l; j < ed - elist.begin(); j++, k++)
        if (j != k)
          elist[k] = move(elist[j]);
      if constexpr (is_erasable)
        len[i] = k - nstart[i];
    }
    nstart[n] = k;
    start.swap(nstart);
    elist.resize(k);
    m = k;
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
