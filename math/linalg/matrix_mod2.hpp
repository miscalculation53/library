#pragma once

#include "../../template/template_all_but_modint.hpp"

/**
 * @brief 行列（$\mathbb{F}_2$）
 * @docs docs/math/linalg/matrix_mod2.md
 */

template <size_t MAX_M>
struct MatrixMod2 : vc<bitset<MAX_M>>
{
  using M = MatrixMod2;
  using V = bitset<MAX_M>;
  using vc<bitset<MAX_M>>::vector;
  using vc<bitset<MAX_M>>::operator=;
  using vc<bitset<MAX_M>>::size;

  int m;
  MatrixMod2(int n, int m, int diag = 0, int non_diag = 0) : m(m)
  {
    assert(m <= (int)MAX_M);
    this->resize(n);
    repi(i, n) repi(j, m)(*this)[i][j] = i == j ? diag : non_diag;
  }
  template <class T>
  MatrixMod2(const vvc<T> &vec)
  {
    const int n = vec.size();
    m = n == 0 ? 0 : vec[0].size();
    assert(m <= (int)MAX_M);
    this->resize(n);
    repi(i, n)
    {
      assert(SZ(vec[i]) == m);
      repi(j, m)
      {
        assert(vec[i][j] == T(0) || vec[i][j] == T(1));
        (*this)[i][j] = vec[i][j];
      }
    }
  }
  MatrixMod2(const vc<string> &vec)
  {
    const int n = vec.size();
    m = n == 0 ? 0 : vec[0].size();
    assert(m <= (int)MAX_M);
    this->resize(n);
    repi(i, n)
    {
      assert(SZ(vec[i]) == m);
      repi(j, m)
      {
        assert(vec[i][j] == '0' || vec[i][j] == '1');
        (*this)[i][j] = vec[i][j] - '0';
      }
    }
  }
  template <class T = ll>
  vvc<T> to_vvi() const
  {
    const int n = size();
    vvc<T> res(n, vc<T>(m));
    repi(i, n) repi(j, m) res[i][j] = (*this)[i][j];
    return res;
  }
  vc<string> to_vstr() const
  {
    const int n = size();
    vc<string> res(n, string(m, '0'));
    repi(i, n) repi(j, m) res[i][j] += (*this)[i][j];
    return res;
  }

  M operator-() const { return *this; }
  
  // 加算・減算 O(nm/w)
  M &operator+=(const M &b)
  {
    const int n = size();
    repi(i, n)(*this)[i] ^= b[i];
    return *this;
  }
  M &operator-=(const M &b) { return *this += b; }
  M operator+(const M &b) const { return M(*this) += b; }
  M operator-(const M &b) const { return M(*this) -= b; }

  // 乗算 O(nmk/w)
  template <size_t MAX_K>
  MatrixMod2<MAX_K> operator*(const MatrixMod2<MAX_K> &b) const
  {
    const int n = size(), m_ = b.size(), k = b.m;
    assert(m == m_);
    MatrixMod2<MAX_K> res(n, k);
    repi(i, n)
    {
      auto row = (*this)[i];
      for (int j = row._Find_first(); j < m; j = row._Find_next(j))
        res[i] ^= b[j];
    }
    return res;
  }
  template <size_t MAX_K>
  MatrixMod2<MAX_K> operator*=(const MatrixMod2<MAX_K> &b) { return *this = *this * b; }

  // 正方行列
  // O(n^3 log k / w)
  template <class T>
  M pow(T k) const
  {
    const int n = size();
    assert(n == m);
    M res(n, n, 1), tmp(*this);
    while (k > 0)
    {
      if (k & 1)
        res *= tmp;
      tmp *= tmp;
      k >>= 1;
    }
    return res;
  }

  // rref が true の場合、簡約行列にしたものを返す
  // rref が false の場合、階段行列にしたものを返す
  // O(nm min(n, m) / w)
  M row_reduction(bool rref = false) const
  {
    const int n = size();
    M a(*this);
    for (int i = 0, j = 0; i < n && j < m; j++)
    {
      repi(k, i, n)
      {
        if (a[k][j])
        {
          swap(a[i], a[k]);
          break;
        }
      }
      if (a[i][j] == 0)
        continue;
      if (rref) repi(k, i) if (a[k][j]) a[k] ^= a[i];
      repi(k, i + 1, n) if (a[k][j]) a[k] ^= a[i];
      i++;
    }
    return a;
  }
  // O(nm min(n, m) / w)
  template <class I = ll>
  I rank() const
  {
    const int n = size();
    M a = row_reduction();
    I res = 0;
    repi(i, n) if (a[i] != 0) res++;
    return res;
  }
  // 正方行列、O(n^3/w)
  template <class I = ll>
  I det() const
  {
    const int n = size();
    assert(n == m);
    M a = row_reduction();
    I res = 1;
    repi(i, n) res &= a[i][i];
    return res;
  }
  // 正方行列、O(n^3/w)
  // (存在するか, 存在する場合逆行列)
  pair<bool, M> inv() const
  {
    const int n = size();
    assert(n == m);
    MatrixMod2<MAX_M * 2> a(n, 2 * n);
    repi(i, n) repi(j, n) a[i][j] = (*this)[i][j];
    repi(i, n) a[i][n + i] = 1;
    auto b = a.row_reduction(true);
    repi(i, n) if (b[i][i] == 0) return {false, {}};
    M res(n, n);
    repi(i, n) repi(j, n) res[i][j] = b[i][n + j];
    return {true, res};
  }

  // (解が存在するか, 解のひとつ, 基底)
  // O(nm min(n, m) / w + m^2)
  template <size_t MAX_N>
  tuple<bool, V, vc<V>> solve(const bitset<MAX_N> &b) const
  {
    const int n = size();
    assert(n <= (int)MAX_N);
    MatrixMod2<MAX_M + 1> a(n, m + 1);
    repi(i, n) rep(j, m) a[i][j] = (*this)[i][j];
    repi(i, n) a[i][m] = b[i];
    a = a.row_reduction(true);
    int rk = 0;
    repi(i, n) if (a[i] != 0) rk++;

    V sol;
    vc<V> basis;
    vc<int> piv(m, -1);
    for (int i = 0, j = 0; i < rk; i++)
    {
      while (j < m && a[i][j] == 0)
        j++;
      if (j == m)
        return {false, {}, {}};
      sol[j] = a[i][m], piv[j] = i;
    }
    repi(j, m)
    {
      if (piv[j] != -1)
        continue;
      V ba;
      ba[j] = 1;
      repi(k, j) if (piv[k] != -1) ba[k] = a[piv[k]][j];
      basis.eb(ba);
    }
    return {true, sol, basis};
  }
};
