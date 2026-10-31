#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/linalg/matrix.hpp"
#include "math/linalg/matrix_array.hpp"

// Compare every coefficient with the defining sum, including sparse-path thresholds.
template <class F> void dynamic_products()
{
  using S = typename F::S;
  mt19937 rng(20261027);
  for (auto [n, m, p] : vc<tuple<int, int, int>>{{0, 17, 19},
                                                 {20, 0, 21},
                                                 {17, 19, 0},
                                                 {1, 1, 1},
                                                 {4, 4, 4},
                                                 {8, 8, 8},
                                                 {16, 16, 16},
                                                 {17, 19, 21},
                                                 {33, 35, 37},
                                                 {64, 65, 63}})
    for (int mode = 0; mode < 10; ++mode)
    {
      Matrix<F> a(n, m), b(m, p), expected(n, p);
      for (int i = 0; i < n; ++i)
        for (int k = 0; k < m; ++k)
        {
          bool take = mode != 0;
          if (mode == 2)
            take = i == k;
          if (mode == 4)
            take = i <= k;
          if (mode == 5)
            take = i >= k;
          if (mode >= 6)
            take = rng() % 16 < unsigned(mode - 5);
          if (take)
            a(i, k) = S(int(rng() % 11) - 5);
        }
      for (int k = 0; k < m; ++k)
        for (int j = 0; j < p; ++j)
        {
          bool take = mode != 1;
          if (mode == 3)
            take = k == j;
          if (mode == 4)
            take = k <= j;
          if (mode == 5)
            take = k >= j;
          if (mode >= 6)
            take = rng() % 16 < unsigned(mode - 5);
          if (take)
            b(k, j) = S(int(rng() % 11) - 5);
        }
      for (int i = 0; i < n; ++i)
        for (int j = 0; j < p; ++j)
          for (int k = 0; k < m; ++k)
            expected(i, j) = F::add(expected(i, j), F::mul(a(i, k), b(k, j)));
      auto result = a * b;
      assert(result.shape() == make_pair(ll(n), ll(p)) && result == expected);
      if (n == m && m == p)
      {
        auto self = a;
        self *= self;
        assert(self == a * a);
      }
    }
}

template <class F, int N, int M, int P> void fixed_products()
{
  MatrixArray<F, N, M> a;
  MatrixArray<F, M, P> b;
  MatrixArray<F, N, P> expected;
  for (int i = 0; i < N; ++i)
    for (int k = 0; k < M; ++k)
      a(i, k) = typename F::S(i <= k ? i + k + 1 : 0);
  for (int k = 0; k < M; ++k)
    for (int j = 0; j < P; ++j)
      b(k, j) = typename F::S(k == j ? k + 1 : 0);
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < P; ++j)
      for (int k = 0; k < M; ++k)
        expected(i, j) = F::add(expected(i, j), F::mul(a(i, k), b(k, j)));
  assert(a * b == expected);
}

int main()
{
  dynamic_products<RingAddSubMul<ll>>();
  dynamic_products<RingAddSubMul<i128>>();
  dynamic_products<RingAddSubMul<modint998244353>>();
  dynamic_products<RingAddSubMul<static_modint32<17>>>();
  fixed_products<RingAddSubMul<ll>, 4, 4, 4>();
  fixed_products<RingAddSubMul<modint998244353>, 4, 4, 4>();
  fixed_products<RingAddSubMul<modint998244353>, 17, 19, 21>();
  fixed_products<RingAddSubMul<modint998244353>, 0, 19, 21>();
  cout << "Hello World\n";
}
