#pragma once

#include "../../template/template_all_but_modint.hpp"
#include "../algebra/algebra_basic_ops.hpp"
#include "bbla.hpp"
#include "../../ds/csr.hpp"
#include "../modint/modint.hpp"

/**
 * @brief 行列
 * @docs docs/math/linalg/matrix.md
 */

// F は体（一部の操作は環や半環で OK）
template <class F, int BS = 32>
struct Matrix : vvc<typename F::S>
{
  using S = typename F::S;
  using M = Matrix;
  using V = vc<S>;
  using vvc<S>::vector;
  using vvc<S>::operator=;

  Matrix(int n, int m, const S &diag = F::e0(), const S &non_diag = F::e0())
  {
    *this = vvc<S>(n, vc<S>(m, non_diag));
    repi(i, min(n, m))(*this)[i][i] = diag;
  }
  Matrix(const vvc<S> &a) { *this = a; }

  template <class I = ll>
  pair<I, I> shape() const
  {
    const int n = (*this).size();
    if (n == 0)
      return {0, 0};
    const int m = (*this)[0].size();
    return {n, m};
  }

  M operator-() const
  {
    auto [n, m] = shape<int>();
    M res(*this);
    repi(i, n) repi(j, m) res[i][j] = F::minus(res[i][j]);
    return res;
  }
  M &operator+=(const M &b)
  {
    assert(shape<int>() == b.shape<int>());
    auto [n, m] = shape<int>();
    repi(i, n) repi(j, m) (*this)[i][j] = F::add((*this)[i][j], b[i][j]);
    return *this;
  }
  M &operator-=(const M &b)
  {
    assert(shape<int>() == b.shape<int>());
    auto [n, m] = shape<int>();
    repi(i, n) repi(j, m) (*this)[i][j] = F::add((*this)[i][j], F::minus(b[i][j]));
    return *this;
  }
  M &operator*=(const S &x)
  {
    auto [n, m] = shape<int>();
    repi(i, n) repi(j, m) (*this)[i][j] = F::mul((*this)[i][j], x);
    return *this;
  }
  M &operator/=(const S &x) { return *this *= F::inv(x); }
  V operator*(const V &v) const
  {
    auto [n, m] = shape<int>();
    assert(SZ(v) == m);
    V res(n, F::e0());
    repi(i, n)
    {
      S sm = F::e0();
      repi(j, m) sm = F::add(sm, F::mul((*this)[i][j], v[j]));
      res[i] = sm;
    }
    return res;
  }
  M operator*(const M &b) const
  {
    auto [n, m] = shape<int>();
    auto [m_, p] = b.shape<int>();
    assert(m == m_);
    M res(n, p);
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
    auto [n, m] = shape<int>();
    assert(n == m);
    M res(n, n, F::e1()), tmp(*this);
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
    auto [n, m] = shape<int>();
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
  // modint で疎行列の場合、要素数を k として O(n^2 + nk)
  S det() const
  {
    auto [n, m] = shape<int>();
    assert(n == m);
    if (n == 0)
      return 1;
    if constexpr (has_mod<S>())
    {
      if (S::mod() >= ipow(10, 8))
      {
        S cand = (*this)[0][0];
        int cnt = 0;
        repi(i, n) repi(j, n)
        {
          S x = (*this)[i][j];
          if (cnt == 0)
            cand = x, cnt = 1;
          else if (cand == x)
            cnt++;
          else
            cnt--;
        }
        int k = n * n;
        repi(i, n) repi(j, n) if ((*this)[i][j] == cand) k--;
        if (k < n * n / 8)
          return det_sparse(cand);
      }
    }
    return get<2>(row_reduction());
  }
  // 正方行列
  // 要素数を k として O(n^2 + nk)
  template <class RandomSample = decltype(bbla::random_sample_mint<typename F::S>)>
  S det_sparse(const S &majority = F::e0(), const RandomSample &random_sample = bbla::random_sample_mint) const
  {
    auto [n, m] = shape<int>();
    assert(n == m);
    vc<pair<int, pair<int, S>>> elms;
    const S minus_majority = F::minus(majority);
    repi(i, n) repi(j, n)
    {
      S val = (*this)[i][j];
      if (val != majority)
        elms.eb(i, pair{j, F::add(val, minus_majority)});
    }
    CSR<pair<int, S>> csr(n, elms);
    auto linear_map = [&](vc<S> &x)
    {
      S sm = F::e0();
      fec(xi : x) sm = F::add(sm, xi);
      sm = F::mul(majority, sm);
      vc<S> y(n, sm);
      repi(i, n)
      {
        S yi = sm;
        fec([j, val] : csr[i]) yi = F::add(yi, F::mul(val, x[j]));
        y[i] = yi;
      }
      swap(x, y);
    };
    return bbla::det<F>(n, linear_map, random_sample);
  }

  // 正方行列
  // (存在するか, 存在する場合逆行列)
  pair<bool, M> inv() const
  {
    auto [n, m] = shape<int>();
    assert(n == m);
    M a(n, 2 * n);
    repi(i, n) repi(j, n) a[i][j] = (*this)[i][j];
    repi(i, n) a[i][n + i] = F::e1();
    M b = get<0>(a.row_reduction(true));
    repi(i, n) if (b[i][i] == F::e0()) return {false, {}};
    M res(n, n);
    repi(i, n) repi(j, n) res[i][j] = b[i][n + j];
    return {true, res};
  }

  // (解が存在するか, 解のひとつ, 基底)
  // O(nm min(n, m) + m^2)
  tuple<bool, V, vc<V>> solve(const V &b) const
  {
    auto [n, m] = shape<int>();
    assert(SZ(b) == n);
    M a(*this);
    repi(i, n) a[i].eb(b[i]);
    auto [a_, rk, _] = a.row_reduction(true);
    swap(a, a_);

    V sol(m);
    vc<V> basis;
    vc<int> piv(m, -1);
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
      V ba(m);
      ba[j] = F::e1();
      repi(k, j) if (piv[k] != -1) ba[k] = F::minus(a[piv[k]][j]);
      basis.eb(ba);
    }
    return {true, sol, basis};
  }
};
