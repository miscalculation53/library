#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/linalg/sparse_matrix.hpp"

using F = FieldAddSubMulDiv<modint998244353>;
using S = F::S;
using SM = SparseMatrix<F>;

void equal(const Matrix<F> &a, const Matrix<F> &b)
{
  assert(a.shape<int>() == b.shape<int>());
  auto [n, m] = a.shape<int>();
  for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) assert(a[i][j] == b[i][j]);
}

struct TinyField
{
  struct S { int x = 0; friend bool operator==(S a, S b) { return a.x == b.x; }
    friend bool operator!=(S a, S b) { return !(a == b); } };
  static S make(int x) { S a; a.x = (x % 101 + 101) % 101; return a; }
  static S e0() { return make(0); }
  static S e1() { return make(1); }
  static S add(S a, S b) { return make(a.x + b.x); }
  static S minus(S a) { return make(-a.x); }
  static S mul(S a, S b) { return make(a.x * b.x); }
  static S inv(S a) { for (int i = 1; i < 101; i++) if (mul(a, make(i)) == e1()) return make(i); assert(false); return e0(); }
};

int main()
{
  mt.seed(20261109);
  SM empty;
  assert((empty.shape<int>() == pair<int, int>(0, 0)));
  assert(empty.det() == 1 && empty.rank() == 0);
  SM duplicate(3, 4, {{1, 2, 5}, {0, 1, 3}, {1, 2, -5}, {0, 1, 4}, {0, 0, 0}});
  assert(duplicate.nnz() == 1 && duplicate(0, 1) == 7 && duplicate(1, 2) == 0);
  vc<tuple<i128, u128, S>> wide{{1, 2, 5}, {0, 3, 7}};
  SM a(2, 4, wide);
  CSR<pair<u128, S>, true> csr(vvc<pair<u128, S>>{{{2, 3}, {1, 5}, {2, -3}}, {{0, 9}, {3, 99}}});
  csr.pop_back(1);
  SM b(4, csr);
  assert(b.nnz() == 2 && b(0, 1) == 5 && b(1, 0) == 9);
  auto copied = b;
  auto moved = std::move(copied);
  equal(b.to_dense(), moved.to_dense());
  auto mutable_sample = [state = make_unique<int>(1)]() mutable
  { return S((*state = int(ll(*state) * 48271 % 2147483647))); };
  assert(SM(2, 2, {{0, 0, 2}, {1, 1, 3}}).rank<int>(2, mutable_sample) == 2);
  SparseMatrix<TinyField> custom(2, 2, {{0, 0, TinyField::e1()}, {1, 1, TinyField::make(2)}});
  auto custom_sample = [] { return TinyField::make(randrange(1, 101)); };
  assert(custom.rank<int>(3, custom_sample) == 2);
  assert(custom.det(custom_sample) == TinyField::make(2));
  assert(custom.solve(vc<TinyField::S>{TinyField::make(3), TinyField::make(5)}, custom_sample).solvable);
  for (int trial = 0; trial < 300; trial++)
  {
    int n = randrange(0, 10), m = randrange(0, 10), p = randrange(0, 10);
    Matrix<F> x(n, m), y(m, p);
    vc<tuple<ll, u128, S>> entries;
    for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) if (randrange(0, 4) == 0)
    {
      S v = randrange(-4, 5), w = randrange(-4, 5);
      x[i][j] = v + w;
      entries.emplace_back(i, j, v);
      entries.emplace_back(i, j, w);
    }
    for (int i = 0; i < m; i++) for (int j = 0; j < p; j++) if (randrange(0, 4) == 0)
      y[i][j] = randrange(-4, 5);
    SM sx(n, m, entries), sy(y);
    equal(sx.to_dense(), x);
    equal(sx.transpose().transpose().to_dense(), x);
    for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) assert(sx(i, j) == x[i][j]);
    equal((sx * sy).to_dense(), x * y);
    equal(sx.multiply_dense(sy), x * y);
    equal(sx * y, x * y);
    equal(x * sy, x * y);
    vc<S> v(m), w(n), row_scale(n), col_scale(m);
    for (auto &z : v) z = randrange(-3, 4);
    for (auto &z : w) z = randrange(-3, 4);
    for (auto &z : row_scale) z = randrange(-1, 2);
    for (auto &z : col_scale) z = randrange(-1, 2);
    assert(sx * v == x * v && w * sx == w * x);
    auto scaled = sx;
    scaled.scale_rows(row_scale).scale_columns(col_scale);
    auto expected = x;
    for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) expected[i][j] *= row_scale[i] * col_scale[j];
    equal(scaled.to_dense(), expected);
    equal((sx * S(3)).to_dense(), x * S(3));
    assert(sx.rank<int>(2) == x.rank<int>());
    auto rhs = sx * v;
    auto solution = sx.solve(rhs);
    assert(solution.solvable && sx * solution.solution == rhs);
    for (auto &z : rhs) z = randrange(-3, 4);
    solution = sx.solve(rhs);
    if (solution.solvable) assert(sx * solution.solution == rhs);
    else
    {
      auto zero = solution.certificate * sx;
      assert(all_of(zero.begin(), zero.end(), [](S z) { return z == 0; }));
      assert(dot_product<F>(n, rhs.begin(), solution.certificate.begin()) != 0);
    }
    if (n == m) assert(sx.det() == x.det());
    if (n == m)
    {
      equal((sx * sx).to_dense(), x * x);
    }
  }
  cout << "Hello World\n";
}
