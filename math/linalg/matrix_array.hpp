#pragma once

#include "../../template/template_all_but_modint.hpp"
#include "../../algebra/algebra_basic_ops.hpp"
#include "bbla.hpp"
#include "../../ds/csr.hpp"
#include "../modint/modint.hpp"
#include "../dot_product.hpp"
#include "row_reduction_mod32.hpp"
#include "internal_matrix_power_mod32.hpp"
#include "internal_matrix_product.hpp"

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
  using Base = array<array<S, m>, n>;
  using Base::at;
  template<class I=ll> static constexpr pair<I,I> shape() { return {n,m}; }
  S &operator()(int i,int j) { return (*this)[i][j]; }
  const S &operator()(int i,int j) const { return (*this)[i][j]; }
  S &at(int i,int j) { assert(0<=i && i<n && 0<=j && j<m); return (*this)[i][j]; }
  const S &at(int i,int j) const { assert(0<=i && i<n && 0<=j && j<m); return (*this)[i][j]; }
  auto &row(int i) { return (*this)[i]; }
  const auto &row(int i) const { return (*this)[i]; }

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
    repi(i, n) res[i] = dot_product<F>(m, (*this)[i].begin(), v.begin());
    return res;
  }
  friend Vm operator*(const Vn &v, const M &a)
  {
    Vm res;
    fill(ALL(res), F::e0());
    repi(i, n) repi(j, m) res[j] = F::add(res[j], F::mul(v[i], a[i][j]));
    return res;
  }
  template <int p>
  MatrixArray<F, n, p> operator*(const MatrixArray<F, m, p> &b) const
  {
    MatrixArray<F, n, p> res;
    repi(i, n) repi(j, p) res[i][j] = F::e0();
    internal::matrix_product<F, BS>(n, m, p, *this, b, res);
    return res;
  }
  template <class T = ll>
  M pow(T k) const
  {
    static_assert(n == m);
    if (n == 0) return M(F::e1());
    if constexpr (internal::ordinary_mod32_field<F>::value &&
                  internal::matrix_power_mod32_detail::supported_builtin_exponent_v<T>)
    {
      if (n >= 24 && k > 0 && internal::matrix_power_mod32_detail::binary_multiplication_count(k) > 10 &&
          internal::matrix_power_mod32_detail::is_prime_modulus<S>())
      {
        vvc<S> a(n, vc<S>(n));
        repi(i, n) repi(j, n) a[i][j] = (*this)[i][j];
        auto value = internal::matrix_power_mod32_detail::power_over_field(a, k);
        M result;
        repi(i, n) repi(j, n) result[i][j] = value[i][j];
        return result;
      }
    }
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
    if constexpr (internal::ordinary_mod32_field<F>::value)
    {
      if (n >= 32 && m >= 32)
      {
        auto [rk, de] = internal::row_reduction_mod32<S>(a, n, m, rref);
        return {std::move(a), I(rk), de};
      }
    }
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
      // ピボットより左は零。浮動小数点でも消去済みの成分を明示的に保つ。
      repi(l, j + 1, m) a[i][l] = F::mul(a[i][l], aij_inv);
      a[i][j] = F::e1();
      if (rref)
      {
        repi(k, i)
        {
          S akj = a[k][j];
          if (akj == F::e0()) continue;
          a[k][j] = F::e0();
          repi(l, j + 1, m) a[k][l] = F::add(a[k][l], F::minus(F::mul(a[i][l], akj)));
        }
      }
      repi(k, i + 1, n)
      {
        S akj = a[k][j];
        if (akj == F::e0()) continue;
        a[k][j] = F::e0();
        repi(l, j + 1, m) a[k][l] = F::add(a[k][l], F::minus(F::mul(a[i][l], akj)));
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
  // 逆行列。存在しなければ nullopt。
  optional<M> inv() const
  {
    static_assert(n == m);
    MatrixArray<F, n, 2 * n> a;
    repi(i, n) repi(j, n) a[i][j] = (*this)[i][j];
    repi(i, n) repi(j, n) a[i][n + j] = i == j ? F::e1() : F::e0();
    auto b = get<0>(a.row_reduction(true));
    repi(i, n) if (b[i][i] == F::e0()) return nullopt;
    M res;
    repi(i, n) repi(j, n) res[i][j] = b[i][n + j];
    return res;
  }

  // (解のひとつ, 基底)。解が存在しなければ nullopt。
  // O(nm min(n, m) + m^2)
  optional<pair<Vm, vc<Vm>>> solve(const Vn &b) const
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
        return nullopt;
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
    return pair{move(sol), move(basis)};
  }
};
