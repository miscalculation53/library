#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/linalg/matrix.hpp"
#include "math/linalg/matrix_array.hpp"
#include "math/linalg/matrix_mod2.hpp"
#include "math/linalg/matrix_mod2_dynamic.hpp"

template <class T> void storage()
{
  using F = RingAddSubMul<T>;
  Matrix<F> a(2, 3);
  a[0] = {T(1), T(2), T(3)};
  a.row(1) = a.row(0);
  a.at(1, 1) = T(4);
  assert(a(0, 1) == T(2) && a(1, 1) == T(4));
  auto b = a;
  b[0][0] = T(7);
  assert(a[0][0] == T(1));
  auto c = move(b);
  assert(b.empty() && b.shape() == make_pair(0LL, 0LL));
  c = move(c);
  assert(c[0][0] == T(7));
  b = move(c);
  assert(c.empty() && b[0][0] == T(7));
  auto row = a[0];
  row[1] = T(5);
  assert(a[0][1] == T(5));
  const auto &ca = a;
  static_assert(is_same_v<decltype(ca(0, 0)), typename vc<T>::const_reference>);
  assert(distance(a.begin(), a.end()) == 2 && a.begin()[1][1] == T(4));
  a.swap_rows(0, 1);
  assert(a[0][1] == T(4));
  vvc<T> converted = a;
  assert(a == converted && a.to_vvi() == converted);
  const auto transposed = ca.transpose();
  assert(transposed.shape() == make_pair(3LL, 2LL));
  assert(transposed[1] == vc<T>({T(4), T(5)}) && transposed.transpose() == a);
  assert(ca.column(1) == transposed[1]);
  const auto chosen_rows = ca.select_rows(vc<i128>{1, 0, 1});
  assert(chosen_rows.shape() == make_pair(3LL, 3LL));
  assert(chosen_rows[0] == a[1] && chosen_rows[1] == a[0] && chosen_rows[2] == a[1]);
  const auto chosen_columns = ca.select_columns(vc<u128>{2, 1, 2});
  assert(chosen_columns == Matrix<F>({{T(3), T(4), T(3)}, {T(3), T(5), T(3)}}));
  assert(ca.select_rows({1})[0] == ca[1]);
  assert(ca.select_columns({0}).column(0) == ca.column(0));

  Matrix<F> row_diagonal(2, 2), column_diagonal(3, 3);
  row_diagonal[0][0] = T(2);
  column_diagonal[0][0] = T(2), column_diagonal[2][2] = T(3);
  auto scaled = a;
  assert(&scaled.scale_rows({T(2), T(0)}) == &scaled && scaled == row_diagonal * a);
  assert(&scaled.scale_columns({T(2), T(0), T(3)}) == &scaled);
  assert(scaled == row_diagonal * a * column_diagonal);

  Matrix<F> zero_rows(0, 3), zero_columns(3, 0);
  assert(zero_rows.shape() == make_pair(0LL, 3LL));
  assert(zero_columns.shape() == make_pair(3LL, 0LL));
  assert(zero_rows.transpose() == zero_columns && zero_columns.transpose() == zero_rows);
  assert(zero_rows.column(2).empty() && zero_rows.select_columns({2, 0}).shape() == make_pair(0LL, 2LL));
  assert(zero_columns.select_rows({2, 1}).shape() == make_pair(2LL, 0LL));
  assert(a.select_rows({}).shape() == make_pair(0LL, 3LL));
  assert(a.select_columns({}).shape() == make_pair(2LL, 0LL));
  auto empty_product = zero_columns * zero_rows;
  assert(empty_product.shape() == make_pair(3LL, 3LL));
  for (auto r : empty_product)
    for (auto x : r)
      assert(x == T(0));
}

void bits()
{
  DynamicBitset v(65, true);
  assert(v.word_count() == 2 && v.word(0) == ~0ULL && v.word(1) == 1);
  v.set_word(1, ~0ULL);
  assert(v.count() == 65);
  MatrixMod2Dynamic a(3, 65, 0, 1);
  assert(a.row(2).count() == 64);
  auto r = a.row(0);
  r.reset_range(1, 64);
  assert(r.count() == 1 && r.test(64));
  r.set_range(0, 65);
  assert(r.all());
  r.flip_range(1, 64);
  assert(r.count() == 2);
  assert(r.find_first() == 0 && r.find_next(0) == 64 && r.find_next(64) == 65);
  assert(r.find_first(false) == 1 && r.find_next(63, false) == 65);
  auto b = a;
  b[1] = b[0];
  b[0][0] = false;
  assert(b[1].test(0) && a[0].test(0));
  b = move(b);
  assert(b.size() == 3);
  auto c = move(b);
  assert(b.shape() == make_pair(0LL, 0LL));
  MatrixMod2Dynamic z(0, 65);
  assert(z.shape() == make_pair(0LL, 65LL));
  assert(z.transpose().shape() == make_pair(65LL, 0LL));
  const auto transposed = a.transpose();
  assert(transposed.shape() == make_pair(65LL, 3LL));
  assert(transposed[2][1] && !transposed[2][0] && transposed.transpose() == a);
  auto sol = z.solve(DynamicBitset(0));
  assert(sol && sol->first.size() == 65 && sol->second.size() == 65);
}

int main()
{
  storage<int>();
  storage<ll>();
  storage<i128>();
  storage<u128>();
  storage<double>();
  storage<modint998244353>();
  Matrix<RingAddSubMul<bool>> a(2, 3);
  a(0, 0) = true;
  a[1] = a[0];
  assert(a[1][0]);
  a.swap_rows(0, 1);
  const auto &ca = a;
  static_assert(is_same_v<decltype(ca(0, 0)), bool>);
  assert(a.transpose().transpose() == a);
  assert(ca.select_columns({0, 0}).column(1) == ca.column(0));
  using F = RingAddSubMul<int>;
  MatrixArray<F, 2, 3, 16> fixed;
  fixed[1][2] = 5;
  static_assert(is_same_v<decltype(fixed.transpose()), MatrixArray<F, 3, 2, 16>>);
  assert(fixed.transpose()[2][1] == 5 && fixed.transpose().transpose() == fixed);
  MatrixMod2<2, 65> fixed_binary;
  fixed_binary[1][64] = true;
  static_assert(is_same_v<decltype(fixed_binary.transpose()), MatrixMod2<65, 2>>);
  assert(fixed_binary.transpose()[64][1] && fixed_binary.transpose().transpose() == fixed_binary);
  Matrix<FieldAddSubMulDiv<modint998244353>> z(0, 3);
  auto s = z.solve(vc<modint998244353>{});
  assert(s && s->first.size() == 3 && s->second.size() == 3);
  bits();
  cout << "Hello World\n";
}
