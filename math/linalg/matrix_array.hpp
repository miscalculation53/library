#pragma once

#include "../../template/template_all_but_modint.hpp"
#include "../algebra/algebra_basic_ops.hpp"
#include "bbla.hpp"
#include "../../ds/csr.hpp"
#include "../modint/modint.hpp"

/**
 * @brief 行列（array）
 * @docs docs/math/linalg/matrix_array.md
 */

// F は体（一部の操作は環や半環で OK）
template <class F, int n, int m, int BS = 32>
struct MatrixArray : array<array<typename F::S, m>, n>
{
  using S = typename F::S;
  using M = MatrixArray;
  using Vn = array<S, n>;
  using Vm = array<S, m>;

  MatrixArray(const S &diag = F::e0(), const S &non_diag = F::e0())
  {
    repi(i, n) repi(j, m)(*this)[i][j] = i == j ? diag : non_diag;
  }
  MatrixArray(const array<array<S, m>, n> &a)
  { repi(i, n) repi(j, m) (*this)[i][j] = a[i][j]; }

  static constexpr MatrixArray e0() { return MatrixArray(F::e0(), F::e0()); }
  static constexpr MatrixArray e1() { return MatrixArray(F::e1(), F::e0()); }

  M operator-() const
  {
    M res(*this);
    repi(i, n) repi(j, m) res[i][j] = F::minus(res[i][j]);
    return res;
  }
  M &operator+=(const M &b)
  {
    repi(i, n) repi(j, m) (*this)[i][j] = F::add((*this)[i][j], b[i][j]);
    return *this;
  }
  M &operator-=(const M &b)
  {
    repi(i, n) repi(j, m) (*this)[i][j] = F::add((*this)[i][j], F::minus(b[i][j]));
    return *this;
  }
  M &operator*=(const S &x)
  {
    repi(i, n) repi(j, m) (*this)[i][j] = F::mul((*this)[i][j], x);
    return *this;
  }
  M &operator/=(const S &x) { return *this *= F::inv(x); }
  Vn operator*(const Vm &v) const
  {
    Vn res;
    fill(ALL(res), F::e0());
    repi(i, n)
    {
      S sm = F::e0();
      repi(j, m) sm = F::add(sm, F::mul((*this)[i][j], v[j]));
      res[i] = sm;
    }
    return res;
  }
  template <int p>
  MatrixArray<F, n, p> operator*(const MatrixArray<F, m, p> &b) const
  {
    MatrixArray<F, n, p> res;
    repi(i, n) repi(j, p) res[i][j] = F::e0();
    repi(ii, 0, n, BS) repi(kk, 0, m, BS) repi(jj, 0, p, BS)
    {
      repi(i, ii, min(ii + BS, n)) repi(k, kk, min(kk + BS, m))
      {
        S aik = (*this)[i][k];
        if (aik == F::e0())
          continue;
        repi(j, jj, min(jj + BS, p)) res[i][j] = F::add(res[i][j], F::mul(aik, b[k][j]));
      }
    }
    return res;
  }
  template <class T = ll>
  M pow(T k) const
  {
    static_assert(n == m);
    M res(F::e1()), tmp(*this);
    while (k > 0)
    {
      if (k & 1)
        res *= tmp;
      tmp *= tmp;
      k >>= 1;
    }
    return res;
  }

  M operator+(const M &a) const { return M(*this) += a; }
  M operator-(const M &a) const { return M(*this) -= a; }
  M operator*(const S &x) const { return M(*this) *= x; }
  M operator/(const S &x) const { return M(*this) /= x; }
  M &operator*=(const M &a) { return *this = *this * a; }

  // 体であることが必要
  // 返り値:
  // - mat: rref が true なら簡約行列、false なら階段行列を返す
  // - rank
  // - det: n==m のとき行列式
  // O(nm min(n, m))
  template <class I = ll>
  tuple<M, I, S> row_reduction(bool rref = false) const
  {
    M a(*this);
    I rk = 0;
    S de = F::e1();
    for (int i = 0, j = 0; i < n && j < m; j++)
    {
      repi(k, i, n)
      {
        if (a[k][j] != F::e0())
        {
          swap(a[k], a[i]);
          if (k != i)
            de = F::minus(de);
          break;
        }
      }
      if (a[i][j] == F::e0())
      {
        de = 0;
        continue;
      }
      de = F::mul(de, a[i][j]);
      S aij_inv = F::inv(a[i][j]);
      repi(l, m) a[i][l] = F::mul(a[i][l], aij_inv);
      if (rref)
      {
        repi(k, i)
        {
          S akj = a[k][j];
          repi(l, m) a[k][l] = F::add(a[k][l], F::minus(F::mul(a[i][l], akj)));
        }
      }
      repi(k, i + 1, n)
      {
        S akj = a[k][j];
        repi(l, m) a[k][l] = F::add(a[k][l], F::minus(F::mul(a[i][l], akj)));
      }
      i++;
      rk++;
    }
    return {a, rk, de};
  }
  // O(nm min(n, m))
  template <class I = ll>
  I rank() const { return get<1>(row_reduction()); }
  // 正方行列
  // O(n^3)
  S det() const
  {
    static_assert(n == m);
    if (n == 0)
      return 1;
    return get<2>(row_reduction());
  }
  // 正方行列
  // (存在するか, 存在する場合逆行列)
  pair<bool, M> inv() const
  {
    static_assert(n == m);
    MatrixArray<F, n, 2 * n> a;
    repi(i, n) repi(j, n) a[i][j] = (*this)[i][j];
    repi(i, n) repi(j, n) a[i][n + j] = i == j ? F::e1() : F::e0();
    auto b = get<0>(a.row_reduction(true));
    repi(i, n) if (b[i][i] == F::e0()) return {false, {}};
    M res(n, n);
    repi(i, n) repi(j, n) res[i][j] = b[i][n + j];
    return {true, res};
  }

  // (解が存在するか, 解のひとつ, 基底)
  // O(nm min(n, m) + m^2)
  tuple<bool, Vm, vc<Vm>> solve(const Vn &b) const
  {
    MatrixArray<F, n, m + 1> a;
    repi(i, n) repi(j, m) a[i][j] = (*this)[i][j];
    repi(i, n) a[i][m] = b[i];
    auto [a_, rk, _] = a.row_reduction(true);
    swap(a, a_);

    Vm sol;
    fill(ALL(sol), F::e0());
    vc<Vm> basis;
    array<int, m> piv;
    fill(ALL(piv), -1);
    for (int i = 0, j = 0; i < rk; i++)
    {
      while (j < m && a[i][j] == F::e0())
        j++;
      if (j == m)
        return {false, {}, {}};
      sol[j] = a[i][m], piv[j] = i;
    }
    repi(j, m)
    {
      if (piv[j] != -1)
        continue;
      Vm ba;
      fill(ALL(ba), F::e0());
      ba[j] = F::e1();
      repi(k, j) if (piv[k] != -1) ba[k] = F::minus(a[piv[k]][j]);
      basis.eb(ba);
    }
    return {true, sol, basis};
  }
};
