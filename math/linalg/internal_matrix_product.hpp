#pragma once

#include "../../ds/csr.hpp"
#include "../dot_product.hpp"

// C[i][j] = sum_k A[i][k] B[k][j].

namespace internal
{
template <class F, int BS, class A, class B, class C>
void matrix_product(int n, int m, int p, const A &a, const B &b, C &c)
{
  using S = typename F::S;
  const S zero = F::e0();
  if (n == m && m == p && n <= 8)
  {
    bool upper = true;
    for (int i = 1; i < n && upper; ++i)
      for (int j = 0; j < i; ++j)
        if (a[i][j] != zero || b[i][j] != zero)
        {
          upper = false;
          break;
        }
    if (upper)
    {
      for (int i = 0; i < n; ++i)
        for (int k = i; k < m; ++k)
        {
          S x = a[i][k];
          if (x == zero)
            continue;
          for (int j = k; j < p; ++j)
            c[i][j] = F::add(c[i][j], F::mul(x, b[k][j]));
        }
      return;
    }
  }
  if (n >= 16 && m >= 16 && p >= 16)
  {
    ll left = 0, right = 0;
    for (int i = 0; i < n; ++i)
      for (int k = 0; k < m; ++k)
        left += a[i][k] != zero;
    if (!left)
      return;
    vc<int> sizes(m);
    for (int k = 0; k < m; ++k)
      for (int j = 0; j < p; ++j)
        if (b[k][j] != zero)
        {
          ++sizes[k];
          ++right;
        }
    if (!right)
      return;
    if (right * 4 <= ll(m) * p)
    {
      CSR<pair<int, S>> rows(sizes);
      for (int k = 0; k < m; ++k)
      {
        int at = 0;
        for (int j = 0; j < p; ++j)
          if (b[k][j] != zero)
            rows[k][at++] = {j, b[k][j]};
      }
      for (int i = 0; i < n; ++i)
        for (int k = 0; k < m; ++k)
        {
          S x = a[i][k];
          if (x == zero)
            continue;
          for (const auto &[j, y] : rows[k])
            c[i][j] = F::add(c[i][j], F::mul(x, y));
        }
      return;
    }
    if constexpr (dot_product_mod32<F>::value)
    {
      if (left * 2 >= ll(n) * m)
      {
        vc<S> bt(size_t(p) * m);
        for (int k = 0; k < m; ++k)
          for (int j = 0; j < p; ++j)
            bt[size_t(j) * m + k] = b[k][j];
        for (int i = 0; i < n; ++i)
          for (int j = 0; j < p; ++j)
            c[i][j] = dot_product<F>(m, a[i].begin(), bt.begin() + size_t(j) * m);
        return;
      }
    }
    if (left * 4 <= ll(n) * m)
    {
      for (int i = 0; i < n; ++i)
        for (int k = 0; k < m; ++k)
        {
          S x = a[i][k];
          if (x == zero)
            continue;
          for (int j = 0; j < p; ++j)
            c[i][j] = F::add(c[i][j], F::mul(x, b[k][j]));
        }
      return;
    }
  }
  for (int ii = 0; ii < n; ii += BS)
    for (int kk = 0; kk < m; kk += BS)
      for (int jj = 0; jj < p; jj += BS)
        for (int i = ii; i < min(ii + BS, n); ++i)
          for (int k = kk; k < min(kk + BS, m); ++k)
          {
            S x = a[i][k];
            if (x == zero)
              continue;
            for (int j = jj; j < min(jj + BS, p); ++j)
              c[i][j] = F::add(c[i][j], F::mul(x, b[k][j]));
          }
}
} // namespace internal
