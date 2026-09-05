#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/linalg/dynamic_matrix_mod2.hpp"

// Test focus: runtime-sized rows support [] assignment, addition, and exponentiation.
// Slice copies in inv/solve preserve columns and the appended RHS at 64-bit
// boundaries; also cover empty dimensions, singular and inconsistent systems.
// Rounded XOR ranges in REF/RREF agree with bitwise elimination, including
// skipped pivot columns and leading zero columns across word boundaries.
vvc<int> reduce_naive(vvc<int> a, bool rref)
{
  int n = a.size(), m = n == 0 ? 0 : a[0].size();
  for (int i = 0, j = 0; i < n && j < m; j++)
  {
    int pivot = i;
    while (pivot < n && !a[pivot][j])
      pivot++;
    if (pivot == n)
      continue;
    swap(a[i], a[pivot]);
    repi(k, rref ? 0 : i + 1, n) if (k != i && a[k][j])
      repi(c, m) a[k][c] ^= a[i][c];
    i++;
  }
  return a;
}

DynamicBitset multiply(const DynamicMatrixMod2 &a, const DynamicBitset &x)
{
  DynamicBitset b(a.size());
  repi(i, a.size()) repi(j, a.m) if (a[i].test(j) && x.test(j)) b.flip(i);
  return b;
}

void test_slice_copy()
{
  mt19937_64 rng(20260831);
  for (int n : {0, 1, 2, 63, 64, 65, 127, 128, 129})
  {
    DynamicMatrixMod2 a(n, n, 1), identity(n, n, 1);
    if (n > 1)
      repi(_, 8 * n)
      {
        int i = rng() % n, j = rng() % (n - 1);
        if (j >= i)
          j++;
        a[i] ^= a[j];
      }
    auto [ok, inverse] = a.inv();
    assert(ok && inverse.m == n && SZ(inverse) == n);
    assert(a * inverse == identity && inverse * a == identity);

    DynamicBitset x(n);
    repi(i, n) if (rng() & 1) x.set(i);
    auto [solvable, sol, basis] = a.solve(multiply(a, x));
    assert(solvable && sol == x && basis.empty());
    if (n)
    {
      a[0].reset();
      assert(!a.inv().first);
      DynamicBitset rhs(n);
      rhs.set(0);
      assert(!get<0>(a.solve(rhs)));
    }
  }

  for (int n : {0, 1, 63, 64, 65})
    for (int m : {0, 1, 63, 64, 65, 127, 128, 129})
    {
      DynamicMatrixMod2 a(n, m);
      repi(i, n) repi(j, m) if (rng() & 1) a[i].set(j);
      DynamicBitset x(m);
      repi(j, m) if (rng() & 1) x.set(j);
      auto b = multiply(a, x);
      auto [ok, sol, basis] = a.solve(b);
      assert(ok && sol.size() == m && multiply(a, sol) == b);
      assert(SZ(basis) == m - a.rank());
      fec(v : basis) assert(v.size() == m && v.any() && multiply(a, v).none());
      DynamicMatrixMod2 kernel(basis.size(), m);
      repi(i, basis.size()) kernel[i] = basis[i];
      assert(kernel.rank() == SZ(basis));

      for (int prefix : {0, m / 2})
      {
        fem(row : a) row.reset_range(0, prefix);
        for (bool rref : {false, true})
        {
          auto reduced = a.row_reduction(rref);
          assert(reduced.m == m);
          assert(reduced.to_vvi<int>() == reduce_naive(a.to_vvi<int>(), rref));
        }
      }
    }
}

int main()
{
  DynamicMatrixMod2 a(2, 2, 1);
  a[0][1] = true;
  assert(a[0][1]);
  assert(!a[1][0]);

  auto zero = a + a;
  assert(zero[0].none() && zero[1].none());

  auto cube = a.pow(3);
  assert(cube.to_vstr() == vc<string>({"11", "01"}));
  test_slice_copy();
  PRINT("Hello World");
}
