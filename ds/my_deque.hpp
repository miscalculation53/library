#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief vector を循環バッファとして使う deque
 * @docs docs/ds/my_deque.md
 */

template <class T>
struct MyDeque
{
private:
  vc<T> d;
  int l = 0, sz = 0;

  int index(int i) const
  {
    i += l;
    return i < SZ<int>(d) ? i : i - SZ<int>(d);
  }

  void extend(int cap)
  {
    vc<T> nd(cap);
    repi(i, sz) nd[i] = std::move((*this)[i]);
    d.swap(nd), l = 0;
  }

  void prepare_push()
  {
    if (sz == SZ<int>(d))
      extend(max(1, 2 * sz));
  }

public:
  // n 要素を再確保せず格納できる容量を確保する
  void reserve(int n)
  {
    assert(n >= 0);
    if (n > SZ<int>(d))
      extend(n);
  }

  // 格納している要素数を返す
  template <class I = ll>
  I size() const { return sz; }
  // 空なら true を返す
  bool empty() const { return sz == 0; }

  // 先頭に x を追加する
  void push_front(const T &x)
  {
    prepare_push();
    if (--l < 0)
      l += SZ<int>(d);
    d[l] = x, sz++;
  }

  // 末尾に x を追加する
  void push_back(const T &x)
  {
    prepare_push();
    d[index(sz)] = x, sz++;
  }

  // 先頭要素を返す
  const T &front() const
  {
    assert(!empty());
    return d[l];
  }
  T &front()
  {
    assert(!empty());
    return d[l];
  }

  // 末尾要素を返す
  const T &back() const
  {
    assert(!empty());
    return d[index(sz - 1)];
  }
  T &back()
  {
    assert(!empty());
    return d[index(sz - 1)];
  }

  // 先頭要素を削除する
  void pop_front()
  {
    assert(!empty());
    if (++l == SZ<int>(d))
      l = 0;
    sz--;
  }

  // 末尾要素を削除する
  void pop_back()
  {
    assert(!empty());
    sz--;
  }

  // すべての要素を削除する。確保した容量は維持する
  void clear() { l = sz = 0; }

  // 先頭を 0 番目とした i 番目の要素を返す
  const T &operator[](int i) const { return d[index(i)]; }
  T &operator[](int i) { return d[index(i)]; }

  // 先頭を 0 番目とした i 番目の要素を範囲検査して返す
  const T &at(int i) const
  {
    assert(0 <= i && i < sz);
    return (*this)[i];
  }
  T &at(int i)
  {
    assert(0 <= i && i < sz);
    return (*this)[i];
  }
};
