#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/linalg/matrix.hpp"
#include "math/linalg/matrix_array.hpp"

template <class A, class B, class = void>
struct CanMultiply : false_type {};
template <class A, class B>
struct CanMultiply<A, B, void_t<decltype(declval<A>() * declval<B>())>> : true_type {};

template <class T>
void test_coefficients()
{
  using F = RingAddSubMul<T>;
  using A = MatrixArray<F, 2, 3>;
  const A a(array<array<T, 3>, 2>{{{T(1), T(2), T(3)}, {T(4), T(5), T(6)}}});
  const array<T, 2> row{T(7), T(8)};
  const array<T, 3> column{T(7), T(8), T(9)};
  static_assert(is_same_v<decltype(row * a), array<T, 3>>);
  static_assert(is_same_v<decltype(a * column), array<T, 2>>);
  static_assert(!CanMultiply<array<T, 3>, A>::value);
  static_assert(!CanMultiply<A, array<T, 2>>::value);
  assert((row * a == array<T, 3>{T(39), T(54), T(69)}));
  assert((a * column == array<T, 2>{T(50), T(122)}));

  const Matrix<F> dynamic(vvc<T>{{T(1), T(2), T(3)}, {T(4), T(5), T(6)}});
  const vc<T> vr(row.begin(), row.end()), vc_(column.begin(), column.end());
  static_assert(is_same_v<decltype(vr * dynamic), vc<T>>);
  assert((vr * dynamic == vc<T>{T(39), T(54), T(69)}));
  assert((dynamic * vc_ == vc<T>{T(50), T(122)}));
  Matrix<F> one_row(vvc<T>{vr});
  assert(vr * dynamic == (one_row * dynamic)[0]);

  MatrixArray<F, 0, 3> zero_rows;
  assert((array<T, 0>{} * zero_rows == array<T, 3>{T(0), T(0), T(0)}));
  assert((zero_rows * column == array<T, 0>{}));
  MatrixArray<F, 2, 0> zero_columns;
  assert((row * zero_columns == array<T, 0>{}));
  assert((zero_columns * array<T, 0>{} == array<T, 2>{T(0), T(0)}));
  Matrix<F> empty;
  assert((vc<T>{} * empty).empty());
  assert((empty * vc<T>{}).empty());
  Matrix<F> dynamic_zero_columns(2, 0);
  assert((vr * dynamic_zero_columns).empty());
  assert((dynamic_zero_columns * vc<T>{} == vc<T>{T(0), T(0)}));
}

using Block = MatrixArray<RingAddSubMul<ll>, 2, 2>;
struct BlockSemiring
{
  using S = Block;
  static S e0() { return S::e0(); }
  static S e1() { return S::e1(); }
  static S add(const S &a, const S &b) { return a + b; }
  static S mul(const S &a, const S &b) { return a * b; }
};

void test_noncommutative_coefficients()
{
  Block a, b;
  a[0] = {1, 2}; a[1] = {0, 1};
  b[0] = {1, 0}; b[1] = {3, 1};
  assert(a * b != b * a);
  MatrixArray<BlockSemiring, 1, 1> fixed;
  fixed[0][0] = a;
  const array<Block, 1> v{b};
  assert((v * fixed)[0] == b * a);
  assert((fixed * v)[0] == a * b);
  Matrix<BlockSemiring> dynamic(vvc<Block>{{a}});
  assert((vc<Block>{b} * dynamic)[0] == b * a);
  assert((dynamic * vc<Block>{b})[0] == a * b);
}

void test_wide_and_max_plus()
{
  using F = RingAddSubMul<i128>;
  MatrixArray<F, 1, 2> a;
  a[0] = {i128(1) << 80, -(i128(1) << 75)};
  auto row = array<i128, 1>{3} * a;
  assert(row[0] == (i128(3) << 80) && row[1] == -(i128(3) << 75));
  using SR = SemiRingMaxPlus<ll>;
  MatrixArray<SR, 2, 3> max_plus;
  max_plus[0] = {1, 2, 3}; max_plus[1] = {4, 5, 6};
  assert((array<ll, 2>{7, 8} * max_plus == array<ll, 3>{12, 13, 14}));
}

void test_random_associativity()
{
  using F = RingAddSubMul<ll>;
  mt19937 rng(20261026);
  repi(iter, 300)
  {
    MatrixArray<F, 2, 3> a;
    MatrixArray<F, 3, 4> b;
    for (auto &row : a) for (ll &x : row) x = ll(rng() % 7) - 3;
    for (auto &row : b) for (ll &x : row) x = ll(rng() % 7) - 3;
    array<ll, 2> row{ll(rng() % 7) - 3, ll(rng() % 7) - 3};
    array<ll, 4> column{1, 2, 3, 4};
    assert((row * a) * b == row * (a * b));
    assert(a * (b * column) == (a * b) * column);
    MatrixArray<F, 2, 3, 4> blocked(a);
    assert(row * blocked == row * a);
    Matrix<F, 4> da(2, 3), db(3, 4);
    repi(i, 2) repi(j, 3) da[i][j] = a[i][j];
    repi(i, 3) repi(j, 4) db[i][j] = b[i][j];
    vc<ll> dr(row.begin(), row.end());
    auto fixed = (row * a) * b;
    assert((dr * da) * db == vc<ll>(fixed.begin(), fixed.end()));
    assert(dr * (da * db) == vc<ll>(fixed.begin(), fixed.end()));
  }
}

int main()
{
  test_coefficients<int>();
  test_coefficients<ll>();
  test_coefficients<ull>();
  test_coefficients<i128>();
  test_coefficients<u128>();
  test_coefficients<double>();
  test_coefficients<modint998244353>();
  test_coefficients<static_modint32<1000000000>>();
  test_noncommutative_coefficients();
  test_wide_and_max_plus();
  test_random_associativity();
  cout << "Hello World\n";
}
