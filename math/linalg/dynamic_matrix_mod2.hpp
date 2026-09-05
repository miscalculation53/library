#pragma once

#include "../../ds/dynamic_bitset.hpp"

/**
 * @brief 列数を実行時に決める行列（$\mathbb{F}_2$）
 * @docs docs/math/linalg/dynamic_matrix_mod2.md
 */

struct DynamicMatrixMod2 : vc<DynamicBitset>
{
  using M = DynamicMatrixMod2;
  using V = DynamicBitset;

  int m;

  DynamicMatrixMod2() : m(0) {}

  // n 行 m 列で、対角成分を diag、それ以外を non_diag として初期化する。
  DynamicMatrixMod2(int n, int m, int diag = 0, int non_diag = 0) : m(m)
  {
    assert(n >= 0 && m >= 0);
    assert((diag == 0 || diag == 1) && (non_diag == 0 || non_diag == 1));
    this->assign(n, V(m, non_diag));
    if (diag != non_diag)
      repi(i, min(n, m)) (*this)[i].flip(i);
  }

  // vec を同じ添字の 0/1 行列として変換する。
  template <class T>
  DynamicMatrixMod2(const vvc<T> &vec)
  {
    const int n = vec.size();
    m = n == 0 ? 0 : vec[0].size();
    this->assign(n, V(m));
    repi(i, n)
    {
      assert(SZ(vec[i]) == m);
      repi(j, m)
      {
        assert(vec[i][j] == T(0) || vec[i][j] == T(1));
        if (vec[i][j] == T(1))
          (*this)[i].set(j);
      }
    }
  }

  // vec[i][j] を (i, j) 成分とする 0/1 行列へ変換する。
  DynamicMatrixMod2(const vc<string> &vec)
  {
    const int n = vec.size();
    m = n == 0 ? 0 : vec[0].size();
    this->assign(n, V(m));
    repi(i, n)
    {
      assert(SZ(vec[i]) == m);
      repi(j, m)
      {
        assert(vec[i][j] == '0' || vec[i][j] == '1');
        if (vec[i][j] == '1')
          (*this)[i].set(j);
      }
    }
  }

  // 通常の二次元 vector に変換する。
  template <class T = ll>
  vvc<T> to_vvi() const
  {
    const int n = this->size();
    vvc<T> res(n, vc<T>(m));
    repi(i, n) repi(j, m) res[i][j] = (*this)[i].test(j);
    return res;
  }

  // 各行を列番号順の 0/1 文字列に変換する。
  vc<string> to_vstr() const
  {
    const int n = this->size();
    vc<string> res(n, string(m, '0'));
    repi(i, n) repi(j, m) if ((*this)[i].test(j)) res[i][j] = '1';
    return res;
  }

  // (i, j) 成分を返す。
  bool test(int i, int j) const
  {
    assert(0 <= i && i < SZ(*this) && 0 <= j && j < m);
    return (*this)[i].test(j);
  }

  // (i, j) 成分を value にする。
  M &set(int i, int j, bool value = true)
  {
    assert(0 <= i && i < SZ(*this) && 0 <= j && j < m);
    (*this)[i].set(j, value);
    return *this;
  }

  // (i, j) 成分を反転する。
  M &flip(int i, int j)
  {
    assert(0 <= i && i < SZ(*this) && 0 <= j && j < m);
    (*this)[i].flip(j);
    return *this;
  }

  M operator-() const { return *this; }
  M &operator+=(const M &b)
  {
    assert(this->size() == b.size() && m == b.m);
    repi(i, this->size()) (*this)[i] ^= b[i];
    return *this;
  }
  M &operator-=(const M &b) { return *this += b; }
  M operator+(const M &b) const { return M(*this) += b; }
  M operator-(const M &b) const { return M(*this) -= b; }

  M operator*(const M &b) const
  {
    const int n = this->size(), k = b.m;
    assert(m == SZ(b));
    M res(n, k);
    repi(i, n)
      for (int j = (*this)[i].find_first(); j < m; j = (*this)[i].find_next(j))
        res[i] ^= b[j];
    return res;
  }
  M &operator*=(const M &b) { return *this = *this * b; }

  // 正方行列の k 乗を返す。
  template <class T>
  M pow(T k) const
  {
    const int n = this->size();
    assert(n == m && k >= 0);
    M res(n, n, 1), a(*this);
    while (k > 0)
    {
      if (k & 1)
        res *= a;
      a *= a;
      k >>= 1;
    }
    return res;
  }

  // rref なら簡約階段行列、そうでなければ階段行列を返す。
  M row_reduction(bool rref = false) const
  {
    const int n = this->size(), m = this->m;
    M a(*this);
    for (int i = 0, j = 0; i < n && j < m; j++)
    {
      int pivot = i;
      while (pivot < n && !a[pivot].test(j))
        pivot++;
      if (pivot == n)
        continue;
      std::swap(a[i], a[pivot]);
      int l = j / V::word_bits * V::word_bits;
      if (rref)
        repi(k, i) if (a[k].test(j)) a[k].xor_slice(l, m, a[i], l);
      repi(k, i + 1, n) if (a[k].test(j)) a[k].xor_slice(l, m, a[i], l);
      i++;
    }
    return a;
  }

  // 行列の rank を返す。
  template <class I = ll>
  I rank() const
  {
    M a = row_reduction();
    I res = 0;
    fec(row : a) if (row.any()) res++;
    return res;
  }

  // 正方行列の行列式を返す。
  template <class I = ll>
  I det() const
  {
    const int n = this->size();
    assert(n == m);
    M a = row_reduction();
    I res = 1;
    repi(i, n) res &= a[i].test(i);
    return res;
  }

  // 正方行列の逆行列が存在するかと、存在する場合は逆行列を返す。
  pair<bool, M> inv() const
  {
    const int n = this->size();
    assert(n == m);
    M a(n, 2 * n);
    repi(i, n)
    {
      a[i].assign_slice(0, n, (*this)[i], 0);
      a[i].set(n + i);
    }
    M b = a.row_reduction(true);
    repi(i, n) if (!b[i].test(i)) return {false, {}};
    M res(n, n);
    repi(i, n) res[i].assign_slice(0, n, b[i], n);
    return {true, res};
  }

  // Ax=b の解が存在するかと、解のひとつ、解空間の基底を返す。
  tuple<bool, V, vc<V>> solve(const V &b) const
  {
    const int n = this->size();
    assert(b.size() == n);
    M a(n, m + 1);
    repi(i, n)
    {
      a[i].assign_slice(0, m, (*this)[i], 0);
      if (b.test(i))
        a[i].set(m);
    }
    a = a.row_reduction(true);
    int rk = 0;
    repi(i, n) if (a[i].any()) rk++;

    V sol(m);
    vc<V> basis;
    vc<int> piv(m, -1);
    for (int i = 0, j = 0; i < rk; i++)
    {
      while (j < m && !a[i].test(j))
        j++;
      if (j == m)
        return {false, {}, {}};
      sol.set(j, a[i].test(m));
      piv[j] = i;
    }
    repi(j, m)
    {
      if (piv[j] != -1)
        continue;
      V v(m);
      v.set(j);
      repi(k, j) if (piv[k] != -1 && a[piv[k]].test(j)) v.set(k);
      basis.eb(move(v));
    }
    return {true, sol, basis};
  }
};
